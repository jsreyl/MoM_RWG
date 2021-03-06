#include"tri_quad.h"

void TriQuad(int n, Eigen::VectorXd& w, Eigen::MatrixXd& lambda){
  w=Eigen::VectorXd::Zero(n);
  lambda=Eigen::MatrixXd::Zero(n,3);
  Eigen::VectorXd alpha=Eigen::VectorXd::Zero(n);
  Eigen::VectorXd beta=Eigen::VectorXd::Zero(n);
  Eigen::VectorXd gamma=Eigen::VectorXd::Zero(n);
  switch(n)
    {
    case 1:
      w(0) = 1;
      alpha(0) = 1./3;
      beta(0) = 1./3;
      gamma(0) = 1./3;
      break;
    case 3:
      w(0) = 1./3; 
      w(1) = w(0);
      w(2) = w(0);
      alpha(0) = 2./3;
      beta(0) = 1./6;
      gamma(0) = 1./6;
      alpha(1) = gamma(0);
      beta(1) = alpha(0);
      gamma(1) = beta(0);
      alpha(2) = beta(0);
      beta(2) = gamma(0);
      gamma(2) = alpha(0);
      break;
    case 6:
      w(0) =     0.223381589678011;
      w(1) = w(0);
      w(2) = w(0);
      w(3) =     0.109951743655322;
      w(4) = w(3);
      w(5) = w(3);
      alpha(0) = 0.108103018168070;
      beta(0)  = 0.445948490915965;
      gamma(0) = beta(0);
      alpha(1) = beta(0);
      alpha(2) = beta(0);
      beta(1)  = alpha(0);
      beta(2)  = beta(0);
      gamma(1) = beta(0);
      gamma(2) = alpha(0);
      alpha(3) = 0.816847572980459;
      beta(3)  = 0.091576213509771;
      gamma(3) = beta(3);
      alpha(4) = beta(3);
      alpha(5) = beta(3);
      beta(4)  = alpha(3);
      beta(5)  = beta(3);
      gamma(4) = beta(3);
      gamma(5) = alpha(3);
      break;
    case 12:
      {
	double w1 = 0.116786275726379;
	double w2 = 0.050844906370207;
	double w3 = 0.082851075618374;
	double a1 = 0.501426509658179;
	double b1 = 0.249286745170910;
	double a2 = 0.873821971016996;
	double b2 = 0.063089014491502;
	double a3 = 0.053145049844817;
	double b3 = 0.310352451033784;
	double g3 = 0.636502499121399;
	w(0) = w1;
	w(1) = w1;
	w(2) = w1;
	w(3) = w2;
	w(4) = w2;
	w(5) = w2;
	w(6) = w3;
	w(7) = w3;
	w(8) = w3;
	w(9) = w3;
	w(10) = w3;
	w(11) = w3;
	alpha(0)  = a1; beta(0) = b1;  gamma(0) = b1;
	alpha(1)  = b1; beta(1) = a1;  gamma(1) = b1;
	alpha(2)  = b1; beta(2) = b1;  gamma(2) = a1;
	alpha(3)  = a2; beta(3) = b2;  gamma(3) = b2;
	alpha(4)  = b2; beta(4) = a2;  gamma(4) = b2;
	alpha(5)  = b2; beta(5) = b2;  gamma(5) = a2;
	alpha(6)  = a3; beta(6) = b3;  gamma(6) = g3;
	alpha(7)  = a3; beta(7) = g3;  gamma(7) = b3;
	alpha(8)  = b3; beta(8) = g3;  gamma(8) = a3;
	alpha(9)  = b3; beta(9) = a3;  gamma(9) = g3;
	alpha(10) = g3; beta(10) = a3; gamma(10) = b3;
	alpha(11) = g3; beta(11) = b3; gamma(11) = a3;
	break;
      }
    default:
      std::cout<<"Unimplemented rule\n";
    }
  lambda.col(0) = alpha;
  lambda.col(1) = beta;
  lambda.col(2) = gamma;
}
