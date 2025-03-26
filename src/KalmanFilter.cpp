#include "KalmanFilter.hpp"

KalmanFilter::KalmanFilter() {
    // Initialize state and matrices
    X = Eigen::VectorXd(2); // State: [heading, yaw_rate]
    P = Eigen::MatrixXd::Identity(2, 2); // Covariance matrix
    F = Eigen::MatrixXd::Identity(2, 2); // State transition matrix
    Q = Eigen::MatrixXd::Identity(2, 2) * 0.001; // Process noise
    H = Eigen::MatrixXd::Identity(2, 2); // Measurement matrix
    R = Eigen::MatrixXd::Identity(2, 2) * 0.1; // Measurement noise
}

void KalmanFilter::initialize(double heading, double yaw_rate) {
    X << heading, yaw_rate;
}

void KalmanFilter::predict(double dt) {
    // Update state transition matrix
    F(0, 1) = dt;

    // Predict new state
    X = F * X;
    X(0) = normalizeAngle(X(0));

    // Update uncertainty
    P = F * P * F.transpose() + Q;
}

void KalmanFilter::update(double measured_heading, double measured_yaw_rate) {
    Eigen::VectorXd Z(2); // Measurement vector
    Z << measured_heading, measured_yaw_rate;

    Eigen::VectorXd Y = Z - H * X; // Innovation (difference)
    Y(0) = normalizeAngle(Y(0));

    // Compute Kalman Gain
    Eigen::MatrixXd S = H * P * H.transpose() + R;
    Eigen::MatrixXd K = P * H.transpose() * S.llt().solve(Eigen::MatrixXd::Identity(2, 2));

    // Update state and covariance
    X += K * Y;
    X(0) = normalizeAngle(X(0));
    P = (Eigen::MatrixXd::Identity(2, 2) - K * H) * P;
}

double KalmanFilter::getFilteredHeading() const {
    return X(0);
}

// Helper function to normalize angle between -180 and 180 degrees
double KalmanFilter::normalizeAngle(double angle) {
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
    return angle;
}
