#include"fill_v_vector.h"

Eigen::VectorXcd FillVVector(Eigen::MatrixXd rho_c_pls,Eigen::MatrixXd rho_c_mns,double EMag, double theta_0, double phi_0, Eigen::VectorXi dof2edge){
  Eigen::VectorXcd V=Eigen::VectorXcd::Zero(NUM_DOFS);
  Eigen::MatrixXcd E=Eigen::MatrixXcd::Zero(NUM_DOFS,3);//Ex, Ey, Ez for each internal edge
  double eps=1e-8;

  if(abs(theta_0)<eps && abs(phi_0)<eps){//i.e. \vec{E}=Ex\hat{i}
    E.col(0)=EMag*Eigen::VectorXcd::Ones(NUM_DOFS);
  }else if(abs(theta_0-90.)<eps && abs(phi_0)<eps){//i.e. \vec{E}=Ey\hat{j}
    E.col(1)=EMag*Eigen::VectorXcd::Ones(NUM_DOFS);
  }else if(abs(theta_0)<eps && abs(phi_0-90)<eps){//i.e. E in z
    E.col(2)=EMag*Eigen::VectorXcd::Ones(NUM_DOFS);
  }else{
    std::cout<<"Incident angle not implemented (yet)!\n";
  }
  int elem_p_pls, elem_p_mns;
  for(int i=0;i<NUM_DOFS;i++){
    elem_p_pls=EDGECONEXELEMS(i,0);
    elem_p_mns=EDGECONEXELEMS(i,1);

    //Eq. 18
    V(i)=0.5*(E.row(elem_p_pls).dot(rho_c_pls.row(i))+E.row(elem_p_mns).dot(rho_c_mns.row(i)));
    V(i)=ELL(dof2edge(i))*V(i);
    }
    
  return V;
}
