#include"GL_quad_rule.h"

void GLQuadRule(int num_pts, int unit_length, Eigen::VectorXd& L_i, Eigen::VectorXd& W_i){
  L_i=Eigen::VectorXd::Zero(num_pts);
  W_i=Eigen::VectorXd::Zero(num_pts);
  switch (num_pts){
  case 1:
    L_i<<0;
    W_i<<2;
    break;
  case 2:
    L_i<<-1/sqrt(3.), 1/sqrt(3.);
    W_i<<1, 1;
    break;
  case 3:
    L_i<<-sqrt(3/5.), 0, sqrt(3/5.);
    W_i<<5/9., 8/9., 5/9.;
    break;
  case 4:
    L_i<<-sqrt((3.+2.*sqrt(6/5.))/7.), -sqrt((3.-2.*sqrt(6/5.))/7.),
      sqrt((3.-2.*sqrt(6/5.))/7.), sqrt((3.+2.*sqrt(6/5.))/7.);
    W_i<<(18.-sqrt(30.))/36., (18.+sqrt(30.))/36.,
      (18.+sqrt(30.))/36., (18.-sqrt(30.))/36.;
    break;
  case 5:
    L_i<<-sqrt(5.+2.*sqrt(10/7.))/3.,
      -sqrt(5.-2.*sqrt(10/7.))/3.,
      0.,
      sqrt(5.-2.*sqrt(10/7.))/3.,
      sqrt(5.+2.*sqrt(10/7.))/3.;
    W_i<<(322.-13.*sqrt(70.))/900.,
      (322.+13.*sqrt(70.))/900.,
      128/225.,
      (322.+13.*sqrt(70.))/900.,
      (322.-13.*sqrt(70.))/900.;
    break;
  default:
    std::cout<<"Unimplemented quadrature rule requested in tri_quad_rule\n";
    break;
  }

  if(unit_length){//Scaling to 0,1 range
    L_i = 0.5*L_i+0.5*Eigen::VectorXd::Ones(num_pts);
    W_i = 0.5*W_i;
  }
}
