#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <regex>
#include "IMUDataParser.hpp"

std::string trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : "";
}

double parseTimestampToSeconds(const std::string& timestamp) {
    std::regex iso8601Regex(R"(^\d{4}-\d{2}-\d{2}T(\d{2}):(\d{2}):(\d{2}\.\d+)$)");
    std::smatch match;
    if (!std::regex_match(timestamp, match, iso8601Regex)) {
        throw std::invalid_argument("Invalid timestamp format");
    }
    return std::stoi(match[1]) * 3600 + std::stoi(match[2]) * 60 + std::stod(match[3]);
}

template <typename T>
bool safeConvert(const std::string& str, T& output) {
    try {
        if constexpr (std::is_integral_v<T>) {
            output = std::stoi(str);
        } else if constexpr (std::is_floating_point_v<T>) {
            output = std::stod(str);
        }
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<IMUEvent> IMUDataParser::parseIMUData(const std::string& filename) {
    std::vector<IMUEvent> imuEvents;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file: " + filename);
    }
    std::string line;
    std::getline(file, line);
    double prevYaw = 0.0, prevTimestampSeconds = 0.0;
    bool firstEntry = true;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string timestampStr, idStr, yawStr, headingStr, statusStr, accelStr;
        IMUEvent imuEvent;

        if (!(std::getline(ss, timestampStr, ',') &&
              std::getline(ss, idStr, ',') &&
              std::getline(ss, yawStr, ',') &&
              std::getline(ss, headingStr, ',') &&
              std::getline(ss, statusStr, ',') &&
              std::getline(ss, accelStr, ','))) {
            continue;
        }

        timestampStr = trim(timestampStr);
        idStr = trim(idStr);
        yawStr = trim(yawStr);
        headingStr = trim(headingStr);
        statusStr = trim(statusStr);
        accelStr = trim(accelStr);

        try {
            imuEvent.timestampSeconds = parseTimestampToSeconds(timestampStr);
        } catch (...) {
            continue;
        }

        if (!safeConvert(idStr, imuEvent.id) ||
            !safeConvert(yawStr, imuEvent.yaw) ||
            !safeConvert(headingStr, imuEvent.heading) ||
            !safeConvert(accelStr, imuEvent.acceleration)) {
            continue;
        }

        imuEvent.status = statusStr;

        if (!firstEntry) {
            double timeDiff = imuEvent.timestampSeconds - prevTimestampSeconds;
            double yawDiff = imuEvent.yaw - prevYaw;
            if (yawDiff > 180) yawDiff -= 360;
            if (yawDiff < -180) yawDiff += 360;
            imuEvent.yawRate = (timeDiff > 0) ? (yawDiff / timeDiff) : 0.0;
        } else {
            imuEvent.yawRate = 0.0;
            firstEntry = false;
        }

        prevYaw = imuEvent.yaw;
        prevTimestampSeconds = imuEvent.timestampSeconds;
        imuEvents.push_back(imuEvent);
    }
    file.close();
    return imuEvents;
}
