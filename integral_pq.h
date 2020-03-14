#ifndef INTEGRAL_PQ
#define INTEGRAL_PQ

#include"globalVars.h"
#include"tri_area3D.h"
#include"intg_sing_SGF.h"
#include"tri_quad.h"

void Integral_pq(int p, int q, Eigen::VectorXd r_cp,
		 double k,int quad_pts, bool sing,
		 std::complex<double>& Ipq, std::complex<double>& Ipq_xi,
		 std::complex<double>& Ipq_eta, std::complex<double>& Ipq_zeta);

#endif
