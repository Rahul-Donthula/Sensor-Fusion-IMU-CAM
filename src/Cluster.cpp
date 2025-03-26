#include "Cluster.hpp"
#include <numeric>  // For std::accumulate

// Add a data point to the cluster
void Cluster::addData(double x, double y, double heading, const std::string& sensor_id) {
    data.push_back({x, y, heading, sensor_id});
}

// Get all cluster data
const std::vector<std::tuple<double, double, double, std::string>>& Cluster::getData() const {
    return data;
}

// Compute centroid (average position)
std::pair<double, double> Cluster::getCentroid() const {
    if (data.empty()) return {0.0, 0.0};

    double sumX = 0.0, sumY = 0.0;
    for (const auto& [x, y, heading, sensor] : data) {
        sumX += x;
        sumY += y;
    }
    return {sumX / data.size(), sumY / data.size()};
}

// Compute average heading
double Cluster::getAverageHeading() const {
    if (data.empty()) return 0.0;

    double totalHeading = std::accumulate(data.begin(), data.end(), 0.0,
        [](double sum, const auto& obj) { return sum + std::get<2>(obj); });

    return totalHeading / data.size();
}
