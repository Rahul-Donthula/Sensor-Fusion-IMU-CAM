#include "SensorDataParser.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <cstring>

using json = nlohmann::json;

const std::vector<std::string> SensorDataParser::TIME_FORMATS = {
    "%Y-%m-%dT%H:%M:%S",
    "%Y-%m-%d %H:%M:%S"
};

std::chrono::system_clock::time_point SensorDataParser::parseTimestamp(const std::string& ts_str) {
    std::tm tm = {};
    std::istringstream ss(ts_str);
    std::string main_ts = ts_str, frac_sec;
    size_t dot_pos = ts_str.find('.');

    if (dot_pos != std::string::npos) {
        main_ts = ts_str.substr(0, dot_pos);
        frac_sec = ts_str.substr(dot_pos + 1);
    }

    ss.imbue(std::locale("C"));

    for (const auto& format : TIME_FORMATS) {
        ss.clear();
        ss.str(main_ts);
        std::memset(&tm, 0, sizeof(tm));

        if (ss >> std::get_time(&tm, format.c_str()) && ss.eof()) break;
    }

    long microseconds = frac_sec.empty() ? 0 : std::stol(frac_sec.append(6 - frac_sec.size(), '0'));
    return std::chrono::system_clock::from_time_t(std::mktime(&tm)) + std::chrono::microseconds(microseconds);
}

std::vector<SensorEvent> SensorDataParser::parseSensorData(const std::string& filename) {
    std::vector<SensorEvent> events;
    std::ifstream file(filename);
    
    if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filename);

    json data;
    file >> data;

    for (const auto& entry : data) {
        for (auto& [cam_id, cam_data] : entry.items()) {
            if (!cam_data.contains("timestamp") || cam_data["timestamp"].get<std::string>().empty())
                throw std::runtime_error("Invalid timestamp for camera: " + cam_id);

            auto timestamp = parseTimestamp(cam_data["timestamp"].get<std::string>());
            
            if (!cam_data.contains("object_ids") || !cam_data.contains("object_positions_x_y"))
                throw std::runtime_error("Missing object data for camera: " + cam_id);

            const auto& object_ids = cam_data["object_ids"];
            const auto& positions = cam_data["object_positions_x_y"];

            if (object_ids.size() != positions.size())
                throw std::runtime_error("Mismatched object IDs and positions for " + cam_id);

            for (size_t i = 0; i < object_ids.size(); ++i) {
                const auto& pos = positions[i];
                if (!pos.is_array() || pos.size() != 2 || !pos[0].is_number() || !pos[1].is_number())
                    throw std::runtime_error("Invalid position data for " + cam_id);

                events.push_back({timestamp, cam_id, pos[0].get<double>(), pos[1].get<double>(), object_ids[i].get<int>()});
            }
        }
    }

    return events;
}
