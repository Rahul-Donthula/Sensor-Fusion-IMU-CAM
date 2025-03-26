#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <vector>
#include <tuple>
#include <string>

class Cluster {
private:
    std::vector<std::tuple<double, double, double, std::string>> data;
    int f_id;
    std::string timestamp;

public:
    Cluster() : f_id(-1) {}
    explicit Cluster(int id) : f_id(id) {}
    
    void addData(double x, double y, double heading, const std::string& sensor_id);
    void setTimestamp(const std::string& ts) { timestamp = ts; }
    std::string getTimestamp() const { return timestamp; }
    int getId() const { return f_id; }
    const std::vector<std::tuple<double, double, double, std::string>>& getData() const;
    std::pair<double, double> getCentroid() const;
    double getAverageHeading() const;
};

#endif // CLUSTER_HPP
