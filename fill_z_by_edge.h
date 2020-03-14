#ifndef FILLZBYEDGE_H
#define FILLZBYEDGE_H

#include"globalVars.h"
#include"integral_pq.h"

Eigen::MatrixXcd FillZMatrixByEdge(double omega, double eps_0, double mu_0, double k, Eigen::MatrixXd r_c, Eigen::MatrixXd rho_c_pls, Eigen::MatrixXd rho_c_mns, int quad_pts, bool sing, Eigen::VectorXi dof2edge);

void Potentials(int field_pt,int source_pt, int source_edge, int source_tri, double k, Eigen::MatrixXd r_c, Eigen::VectorXi dof2edge, int quad_pts, bool sing, double eps_0, double mu_0, double omega,  std::complex<double>& ScalarPot, Eigen::VectorXcd& MagVecPot);

#endif
