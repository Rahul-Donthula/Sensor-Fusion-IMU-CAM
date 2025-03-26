#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <cmath>
#include <unordered_map>
#include "SensorDataParser.hpp"
#include "IMUDataParser.hpp"
#include "Cluster.hpp"
#include "KalmanFilter.hpp"
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(1000, 9999);

const double CLUSTER_RADIUS = 2.0;

void writeToCSV(const std::string& filename, const std::vector<std::tuple<std::string, int, std::vector<std::vector<double>>, double, std::string, std::vector<std::string>>>& fusedData) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    file << "timestamp,cluster_id,positions,filtered_heading,status,sensor_ids\n";
    for (const auto& data : fusedData) {
        file << std::get<0>(data) << "," << std::get<1>(data) << ",\"";
        for (size_t i = 0; i < std::get<2>(data).size(); ++i) {
            file << "[" << std::get<2>(data)[i][0] << "," << std::get<2>(data)[i][1] << "]";
            if (i < std::get<2>(data).size() - 1) file << " ";
        }
        file << "\"," << std::get<3>(data) << "," << std::get<4>(data) << ",\"";
        for (size_t i = 0; i < std::get<5>(data).size(); ++i) {
            file << std::get<5>(data)[i];
            if (i < std::get<5>(data).size() - 1) file << " ";
        }
        file << "\"\n";
    }
    file.close();
}

double distance(double x1, double y1, double x2, double y2) {
    return std::hypot(x2 - x1, y2 - y1);
}

std::string timePointToString(const std::chrono::time_point<std::chrono::system_clock>& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int main() {
    auto sensorEvents = SensorDataParser::parseSensorData("../data/task_cam_data.json");
    auto imuEvents = IMUDataParser::parseIMUData("../data/task_imu.csv");

    if (sensorEvents.empty() || imuEvents.empty()) return 1;

    std::map<std::string, IMUEvent> imuMap;
    for (const auto& imu : imuEvents) {
        if (!std::isnan(imu.heading) && !std::isnan(imu.acceleration)) {
            imuMap[imu.timestamp.substr(0, 19)] = imu;
        }
    }

    std::map<std::string, std::vector<SensorEvent>> timestampGroupedData;
    for (const auto& s : sensorEvents) {
        timestampGroupedData[timePointToString(s.timestamp).substr(0, 19)].push_back(s);
    }

    std::map<int, Cluster> clusters;
    KalmanFilter kf;
    std::vector<std::tuple<std::string, int, std::vector<std::vector<double>>, double, std::string, std::vector<std::string>>> fusedData;

    for (const auto& [timestamp, events] : timestampGroupedData) {
        std::vector<bool> clustered(events.size(), false);
        for (size_t i = 0; i < events.size(); ++i) {
            if (clustered[i]) continue;
            
            Cluster newCluster(distrib(gen));
            newCluster.setTimestamp(timestamp);
            double heading = imuMap.count(timestamp) ? imuMap[timestamp].heading : -1.0;
            newCluster.addData(events[i].x, events[i].y, heading, events[i].sensor_id);
            clustered[i] = true;
            
            for (size_t j = i + 1; j < events.size(); ++j) {
                if (!clustered[j] && distance(events[i].x, events[i].y, events[j].x, events[j].y) <= CLUSTER_RADIUS) {
                    newCluster.addData(events[j].x, events[j].y, heading, events[j].sensor_id);
                    clustered[j] = true;
                }
            }
            clusters[newCluster.getId()] = newCluster;
        }
    }

    for (const auto& [id, cluster] : clusters) {
        std::vector<std::vector<double>> positions;
        std::vector<std::string> sensorIds;
        for (const auto& obj : cluster.getData()) {
            positions.push_back({std::get<0>(obj), std::get<1>(obj)});
            sensorIds.push_back(std::get<3>(obj));
        }
        
        kf.update(cluster.getAverageHeading(), 0.0);
        std::string status = imuMap.count(cluster.getTimestamp()) ? imuMap[cluster.getTimestamp()].status : "UNKNOWN";
        
        fusedData.emplace_back(cluster.getTimestamp(), id, positions, kf.getFilteredHeading(), status, sensorIds);
    }

    writeToCSV("fused_data.csv", fusedData);
    return 0;
}
