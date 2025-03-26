#ifndef IMU_DATA_PARSER_HPP
#define IMU_DATA_PARSER_HPP

#include <vector>
#include <string>

struct IMUEvent {
    std::string timestamp;
    double id;
    double yaw;
    double heading;
    std::string status;
    double acceleration;
    double yawRate;  // Add this field to store calculated yaw rate
    double timestampSeconds;
};

class IMUDataParser {
public:
    static std::vector<IMUEvent> parseIMUData(const std::string& filename);
};

#endif // IMU_DATA_PARSER_HPP
