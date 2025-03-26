#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

#include <Eigen/Dense>

class KalmanFilter {
public:
    KalmanFilter();

    void initialize(double initial_heading, double initial_yaw_rate);
    void predict(double delta_t);
    void update(double measured_heading, double measured_yaw_rate);

    double getFilteredHeading() const;

private:
    Eigen::VectorXd X;  // State vector [heading, yaw_rate]
    Eigen::MatrixXd P;  // Covariance matrix
    Eigen::MatrixXd F;  // State transition model
    Eigen::MatrixXd Q;  // Process noise covariance
    Eigen::MatrixXd H;  // Measurement matrix
    Eigen::MatrixXd R;  // Measurement noise covariance
    Eigen::MatrixXd I;  // Identity matrix
};

#endif // KALMAN_FILTER_HPP


