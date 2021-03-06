#ifndef FILLVVECTOR_H
#define FILLVVWCTOR_H

#include"globalVars.h"
#include<iostream>

Eigen::VectorXcd FillVVector(Eigen::MatrixXd rho_c_pls,Eigen::MatrixXd rho_c_mns,double EMag, double theta_0, double phi_0, Eigen::VectorXi dof2edge);

#endif
