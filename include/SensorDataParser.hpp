#ifndef SENSOR_DATA_PARSER_HPP
#define SENSOR_DATA_PARSER_HPP

#include <vector>
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

struct SensorEvent {
    std::chrono::system_clock::time_point timestamp;
    std::string sensor_id;  
    double x;
    double y;
    int obj_id;
};

class SensorDataParser {
public:
    static std::vector<SensorEvent> parseSensorData(const std::string& filename);

private:
    static std::chrono::system_clock::time_point parseTimestamp(const std::string& ts_str);
    static const std::vector<std::string> TIME_FORMATS;  // Only declaration
};

#endif // SENSOR_DATA_PARSER_HPP
