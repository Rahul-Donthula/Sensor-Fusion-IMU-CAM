#include "Utils.hpp"
#include <fstream>
#include <iostream>

// Write fused data to a CSV file
void Utils::writeToCSV(const std::string& filename, 
                       const std::vector<std::tuple<std::string, int, std::vector<std::vector<double>>, double, std::string, std::string>>& fusedData) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // CSV Header
    file << "Timestamp,ClusterID,X,Y,Heading,Status,SensorID\n";

    // Write data rows
    for (const auto& [timestamp, clusterID, positions, heading, status, sensorID] : fusedData) {
        for (const auto& pos : positions) {
            if (pos.size() == 2) {
                file << timestamp << "," << clusterID << ","
                     << pos[0] << "," << pos[1] << ","
                     << heading << "," << status << ","
                     << sensorID << "\n";
            } else {
                std::cerr << "Error: Invalid position data.\n";
            }
        }
    }

    std::cout << "CSV file saved: " << filename << std::endl;
}
