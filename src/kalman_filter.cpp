#include "kalman_filter.h"
#include<iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {

  x_ = F_ * x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
  // std::cout << x_ << std::endl;
  // std::cout << F_ << std::endl;
  // std::cout << Ft << std::endl;
  // std::cout << Q_ << std::endl;
  // std::cout << P_ << std::endl;

}

void KalmanFilter::Update(const VectorXd &z) {

  VectorXd z_pred = H_ * x_;
  // std::cout<<z_pred<<std::endl;
  VectorXd y = z - z_pred;
  MatrixXd Ht = H_.transpose();
  MatrixXd temp =  P_ * Ht;
  MatrixXd S = H_ * temp + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = temp;
  MatrixXd K = PHt * Si;

  //new estimate
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
  // std::cout << x_ << std::endl;
  // std::cout << P_ << std::endl;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {

    double px = x_[0];
    double py = x_[1];
    double vx = x_[2];
    double vy = x_[3];

    if(fabs(px) < 0.0001){
        px = 0.0001;
    }

    double rho = sqrt(px*px + py*py);
    if(fabs(rho) < 0.0001){
        rho = 0.0001;
    }

    double phi = atan2(py, px);
    double rho_dot = (px*vx + py*vy)/rho;

    VectorXd z_pred(3);
    z_pred << rho, phi, rho_dot;
    VectorXd y = z - z_pred;
    while (y(1)> M_PI) y(1)-=2.*M_PI;
    while (y(1)<-M_PI) y(1)+=2.*M_PI;
    MatrixXd Ht = H_.transpose();
    MatrixXd temp =  P_ * Ht;
    MatrixXd S = H_ * temp + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = temp;
    MatrixXd K = PHt * Si;

    x_ = x_ + (K * y);
    long x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;
}