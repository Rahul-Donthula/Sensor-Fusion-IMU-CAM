# Sensor Fusion Data Processing

## Overview
This project processes and fuses data from different sensor sources (e.g., IMU and event-based sensors). It:
- Reads sensor data from **JSON** files.
- Reads IMU data from **CSV** files.
- Sorts events chronologically by timestamp.
- Applies a **Kalman Filter** to smooth IMU data.
- Outputs the processed data to a **CSV file**.

## Features
- **Multi-sensor data parsing**: Parses JSON and CSV data formats.
- **Event sorting**: Merges and sorts events by timestamp.
- **Kalman filtering**: Implements a simple Kalman filter for IMU data.
- **CSV output**: Saves processed data to `fused_data.csv`.

---

## Directory Structure
```
SensorFusion/
├── build/                     # Build directory (CMake output)
├── data/                      # Sample data files
│   ├── task_cam_data.json     # Sensor event data (JSON format)
│   ├── task_imu.csv           # IMU event data (CSV format)
│   ├── 
├── include/                   # Header files
│   ├── SensorDataParser.hpp   # JSON data parser
│   ├── IMUDataParser.hpp      # CSV IMU parser
│   ├── KalmanFilter.hpp       # Kalman filter class
│   ├── Cluster.hpp            # Cluster class
│   ├── Utils.hpp              # Utility functions (CSV writing)
├── src/                       # Source files
│   ├── SensorDataParser.cpp   # JSON data parser implementation
│   ├── IMUDataParser.cpp      # CSV IMU parser implementation
│   ├── KalmanFilter.cpp       # Kalman filter implementation
│   ├── Cluster.cpp            # Clustering implementation
│   ├── Utils.cpp              # CSV writing implementation
│   ├── main.cpp               # Entry point
├── CMakeLists.txt             # CMake build script
├── README.md                  # Project documentation
```

---

## Installation & Compilation
### **Requirements**
- C++17 or later
- CMake 3.10+
- Eigen3 (for Kalman filtering)
- JSON for Modern C++ (https://github.com/nlohmann/json)

### **Build Instructions**
```sh
# Clone repository
git clone https://github.com/yourusername/SensorFusion.git
cd SensorFusion

# Create and navigate to build directory
mkdir build && cd build

# Run CMake
cmake ..

# Compile project
make
```

---

## Usage
### **Running the Program**
```sh
./SensorFusion
```

###**Data Formats
'''sh
Camera Data (task_cam_data.json)
'''


### **Expected Output (Terminal Log)**
```sh
Processed event: sensor at 1710303020
Processed event: imu at 1710303021
Processed event: sensor at 1710303025
Processed event: imu at 1710303030
Output written to: ../data/output.csv
```

### **Output CSV (`output.csv`)**
```
timestamp,event_type
1710303020,sensor
1710303021,imu
1710303025,sensor
1710303030,imu
```

---

## License
This project is licensed under the MIT License.

---

## Contributing
Feel free to open issues or submit pull requests to enhance the project.

---

## Contact
For questions or collaboration, reach out via GitHub or email at **rahuldonthula@gmail.com**.

