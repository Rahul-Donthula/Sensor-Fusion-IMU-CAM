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
git clone https://github.com/Rahul-Donthula/SensorFusion.git
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

## Data Formats

### **Camera Data (task_cam_data.json)**
```sh
[
  {
    "cam_<ID>": {
      "frame_id": "12604",
      "timestamp": "YYYY-MM-DD HH:MM:SS.sss",
      "area": [1,2],
      "object_classes": ["forklift"],
      "bboxes": [[x1,y1,x2,y2]],
      "object_positions_x_y": [[x,y]],
      "object_area": [1],
      "object_ids": [<unique_sensor_specific_id>],
      "object_confidences": [0.0],
      "object_association_ids": [0],
      "vest_flags": [false]
    }
  }
]
```
### **IMU Data (task_imu.csv)**
```sh
timestamp,heading,status,acceleration
2025-03-14T11:56:45.033,285.4,DRIVING,0.12
2025-03-14T11:56:45.034,287.1,STATIONARY,0.0
```


### **Expected Output (Terminal Log)**
```sh
CSV file fused_data.csv written successfully.
```

### **Fused Data (`fused_data.csv`)**
```sh
timestamp,cluster_id,positions,filtered_heading,status,sensor_ids
2025-03-14 12:00:35,1000,"[311,1197]",359.091,STANDING,"cam_141"
2025-03-14 12:01:04,1001,"[522,1522]",187.619,STANDING,"cam_142"
2025-03-14 12:00:00,1002,"[332,1282]",126.774,STANDING,"cam_151"
```

---
## Troubleshooting

Ensure input files exist in the data/ directory.

Check file permissions for reading and writing.

If the compiled binary fails, rebuild using:
```sh
rm -rf build && mkdir build && cd build
cmake ..
make
```



