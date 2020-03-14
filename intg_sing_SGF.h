#ifndef INTGSINGSGF_H
#define INTGSINGSGF_H

#include<Eigen>
#include<cmath>
#include<iostream>
#include<complex>
#include"simplex_area.h"
#include"GL_quad_rule.h"

void IntegrateSingularitySGF(double k, Eigen::VectorXd r_cp,
			     Eigen::Vector3d r1, Eigen::Vector3d r2,
			     Eigen::Vector3d r3, int num_pts_rad,
			     int num_pts_trans,
			     std::complex<double>& Ipq,
			     std::complex<double>& Ipq_xi,
			     std::complex<double>& Ipq_eta,
			     std::complex<double>& Ipq_zeta);

Eigen::Vector3d hat(Eigen::Vector3d a);

#endif
