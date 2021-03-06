#include"intg_sing_SGF.h"

const std::complex<double>J(0.,1.);

void IntegrateSingularitySGF(double k, Eigen::VectorXd r,
			     Eigen::Vector3d r1, Eigen::Vector3d r2,
			     Eigen::Vector3d r3, int num_pts_rad,
			     int num_pts_trans,
			     std::complex<double>& Ipq,
			     std::complex<double>& Ipq_xi,
			     std::complex<double>& Ipq_eta,
			     std::complex<double>& Ipq_zeta){

  int num_pts_j=num_pts_rad;
  int num_pts_i=num_pts_trans;

  //Zero integral
  Eigen::VectorXcd I = Eigen::VectorXcd::Zero(3);
  Eigen::VectorXcd I1 = Eigen::VectorXcd::Zero(3);
  Eigen::VectorXcd I2 = Eigen::VectorXcd::Zero(3);
  Eigen::VectorXcd I3 = Eigen::VectorXcd::Zero(3);

  //vector normal to integration surface
  Eigen::Vector3d N = (r2-r1).cross(r3-r1);
  //The projection of r on r1
  Eigen::Vector3d r0 = r+((r1-r).dot(N))*N/N.squaredNorm();
  //To find the height of the projected observer point above the triangle, build a vector from observer point to projected oberver point
  Eigen::VectorXd r_vec=r-r0;
  //And project over the normal vector of the triangle
  double z = hat(N).dot(r_vec);

  Eigen::Vector3d xi0=SimplexArea(r0,r1,r2,r3);//coordinates of the projected obervation point from cartesian to simplex

  Eigen::Vector3d xi_pij=Eigen::Vector3d::Zero(3);

  Eigen::Vector3d r1_p =Eigen::Vector3d::Zero(3);
  Eigen::Vector3d r2_p =Eigen::Vector3d::Zero(3);
  Eigen::Vector3d r3_p =Eigen::Vector3d::Zero(3);
  Eigen::Matrix3d T =Eigen::Matrix3d::Zero(3,3);
  for(int t=0;t<3;t++){//on sub-triangles 0,1,2
    //Compute geometry of the sub-triangle
    r1_p =r0;
    switch(t){
    case 0:
      r2_p=r2;
      r3_p=r3;
      T<<xi0(0),0,0,
	xi0(1),1,0,
	xi0(2),0,1;
      break;
    case 1:
      r2_p=r3;
      r3_p=r1;
      T<<xi0(0),0,1,
	xi0(1),0,0,
	xi0(2),1,0;
      break;
    case 2:
      r2_p=r1;
      r3_p=r2;
      T<<xi0(0),1,0,
	xi0(1),0,1,
	xi0(2),0,0;
      break;
    }
    Eigen::Vector3d ell1_p=r3_p-r2_p;
    Eigen::Vector3d ell2_p=r1_p-r3_p;
    Eigen::Vector3d ell3_p=r2_p-r1_p;
    Eigen::Vector3d hat_n_p=hat(ell1_p.cross(ell2_p));
    double A_p=0.5*hat_n_p.dot(ell1_p.cross(ell2_p));
    Eigen::Vector3d h1_p=2.*A_p*(ell1_p.cross(hat_n_p))/ell1_p.squaredNorm();

    //Aproximate the integral using gaussian-legendre quadrature
    //Weights and evaluation points
    Eigen::VectorXd xi_i=Eigen::VectorXd::Zero(num_pts_i);
    Eigen::VectorXd w_i=Eigen::VectorXd::Zero(num_pts_i);
    Eigen::VectorXd xi_j=Eigen::VectorXd::Zero(num_pts_j);
    Eigen::VectorXd w_j=Eigen::VectorXd::Zero(num_pts_j);
    GLQuadRule(num_pts_i,1,xi_i,w_i);
    GLQuadRule(num_pts_j,1,xi_j,w_j);
    for(int j=0;j<num_pts_j;j++){
      xi_pij(0)=xi_j(j);
      double y_pj=h1_p.norm()*(1.-xi_j(j));
      double x_Lj = hat_n_p.dot(hat(h1_p).cross(ell2_p))*(1.-xi_pij(0));
      double x_Uj = -hat_n_p.dot(hat(h1_p).cross(ell3_p))*(1.-xi_pij(0));
      double u_Lj=asinh(x_Lj/sqrt(y_pj*y_pj+z*z));
      double u_Uj=asinh(x_Uj/sqrt(y_pj*y_pj+z*z));
      for(int i=0;i<num_pts_i;i++){
	double u_ij= u_Lj*(1.-xi_i(i))+u_Uj*xi_i(i);
	double x_pij = sqrt(y_pj*y_pj+z*z)*sinh(u_ij);
	xi_pij(2)= hat_n_p.dot(ell3_p.cross(hat(h1_p)*y_pj- hat(ell1_p)*x_pij))/(2.*A_p);
	xi_pij(1)= 1. - xi_pij(2) - xi_pij(0);
	Eigen::Vector3d xi_ij = T*xi_pij; //Original triangle area coordinates
	double R_ij = (r-r1*xi_ij(0)-r2*xi_ij(1)-r3*xi_ij(2)).norm();
	//Now we can compute the integrals using the weights by the GL method
	I(t)=I(t)+w_i(i)*w_j(j)*h1_p.norm()*(u_Uj-u_Lj)*exp(-J*k*R_ij);
      	I1(t)=I1(t)+w_i(i)*w_j(j)*h1_p.norm()*(u_Uj-u_Lj)*xi_ij(0)*exp(-J*k*R_ij);
      	I2(t)=I2(t)+w_i(i)*w_j(j)*h1_p.norm()*(u_Uj-u_Lj)*xi_ij(1)*exp(-J*k*R_ij);
      }
    }
  }
  //Now add the integral contributions over all sub-triangles
  Ipq=I.sum();
  Ipq_xi=I1.sum();
  Ipq_eta=I2.sum();
  Ipq_zeta=Ipq-Ipq_xi-Ipq_eta;
  /*  std::cout<<"Ipq "<<Ipq<<"\n";
  std::cout<<"Ipq_xi "<<Ipq_xi<<"\n";
  std::cout<<"Ipq_eta "<<Ipq_eta<<"\n";
  std::cout<<"Ipq_zeta "<<Ipq_zeta<<"\n";*/
  
  //  Ipq=0.;Ipq_xi=0;Ipq_eta=0.;Ipq_zeta=0.;
}

Eigen::Vector3d hat(Eigen::Vector3d a){
  Eigen::Vector3d hat_a=Eigen::Vector3d::Zero(3);
  if(a.norm()<1e-10)
    hat_a =0*a;
  else
    hat_a=a/a.norm();

  return hat_a;
}
