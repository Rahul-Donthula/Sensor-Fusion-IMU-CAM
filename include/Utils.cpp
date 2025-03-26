#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <tuple>

class Utils {
public:
    static void writeToCSV(const std::string& filename, 
        const std::vector<std::tuple<std::string, int, std::vector<std::vector<double>>, double, std::string, std::string>>& fusedData);
};

#endif // UTILS_HPP
