#include"fill_z_by_edge.h"
#include<iostream>

const std::complex<double> J(0.,1.);

Eigen::MatrixXcd FillZMatrixByEdge(double omega, double eps_0, double mu_0, double k, Eigen::MatrixXd r_c, Eigen::MatrixXd rho_c_pls, Eigen::MatrixXd rho_c_mns, int quad_pts, bool sing, Eigen::VectorXi dof2edge){
  //Definitions
  int elem_p_pls, elem_q_pls, elem_p_mns, elem_q_mns;//Observation and measurement points
  //Potentials
  Eigen::VectorXcd MagVecPot=Eigen::VectorXcd::Zero(3);
  std::complex<double> ScalarPot(0.,0.);
  //pls
  Eigen::VectorXcd Amn_pls_source_pls=Eigen::VectorXcd::Zero(3);
  std::complex<double> Phimn_pls_source_pls(0.,0.);
  Eigen::VectorXcd Amn_pls_source_mns=Eigen::VectorXcd::Zero(3);
  std::complex<double> Phimn_pls_source_mns(0.,0.);
  Eigen::VectorXcd Amn_pls=Eigen::VectorXcd::Zero(3);
  std::complex<double> Phimn_pls(0.,0.);
  //mns
  Eigen::VectorXcd Amn_mns_source_pls=Eigen::VectorXcd::Zero(3);
  std::complex<double> Phimn_mns_source_pls(0.,0.);
  Eigen::VectorXcd Amn_mns_source_mns=Eigen::VectorXcd::Zero(3);
  std::complex<double> Phimn_mns_source_mns(0.,0.);
  Eigen::VectorXcd Amn_mns=Eigen::VectorXcd::Zero(3);
  std::complex<double> Phimn_mns(0.,0.);

  //impedance matrix
  Eigen::MatrixXcd Z=Eigen::MatrixXcd::Zero(NUM_DOFS,NUM_DOFS);
  int counter=0;  
  //Matrix filing
  //We're quite literally using equation 17 on the paper by RWG
  for(int m=0;m<NUM_DOFS;m++)
    for(int n=0;n<NUM_DOFS;n++){
      elem_p_pls=EDGECONEXELEMS(m,0);
      elem_p_mns=EDGECONEXELEMS(m,1);
      elem_q_pls=EDGECONEXELEMS(n,0);
      elem_q_mns=EDGECONEXELEMS(n,1);

      //We have 4 contributions
      //contribution from n+ and n- faces measured on m+
      Potentials(elem_p_pls,elem_q_pls,n,0,k,r_c,dof2edge,quad_pts,sing,eps_0,mu_0,omega,ScalarPot,MagVecPot);
      Amn_pls_source_pls=MagVecPot;
      Phimn_pls_source_pls=(-1.)*ScalarPot;
      Potentials(elem_p_pls,elem_q_mns,n,1,k,r_c,dof2edge,quad_pts,sing,eps_0,mu_0,omega,ScalarPot,MagVecPot);
      Amn_pls_source_mns=(-1.)*MagVecPot;
      Phimn_pls_source_mns=ScalarPot;
      Amn_pls=Amn_pls_source_pls+Amn_pls_source_mns;
      Phimn_pls=Phimn_pls_source_pls+Phimn_pls_source_mns;

      //contribution from n+ and n- faces measured from m-
      Potentials(elem_p_mns,elem_q_pls,n,0,k,r_c,dof2edge,quad_pts,sing,eps_0,mu_0,omega,ScalarPot,MagVecPot);
      Amn_mns_source_pls=MagVecPot;
      Phimn_mns_source_pls=(-1.)*ScalarPot;
      Potentials(elem_p_mns,elem_q_mns,n,1,k,r_c,dof2edge,quad_pts,sing,eps_0,mu_0,omega,ScalarPot,MagVecPot);
      Amn_mns_source_mns=(-1.)*MagVecPot;
      Phimn_mns_source_mns=ScalarPot;
      Amn_mns=Amn_mns_source_pls+Amn_mns_source_mns;
      Phimn_mns=Phimn_mns_source_pls+Phimn_mns_source_mns;

      //We've got all contributions ready, now we can build our impedance matrix
      //From eq 17
      //We're saving I for the current, also i and j are better as loop indexes
      //Also Eigen uses conjgate linear dot products on complex numbers so we must conjugate to get the usual product (remember we're multiplying a complex number Amn by a real number rho_c)
      Z(m,n)=J*omega*0.5*(Amn_pls.conjugate().dot(rho_c_pls.row(m))
			  +Amn_mns.conjugate().dot(rho_c_mns.row(m)))
			  +Phimn_mns-Phimn_pls;
      //Finally
      Z(m,n)=ELL(dof2edge(m))*Z(m,n);
      /*      if(counter<10){
	std::cout<<"a_pls"<<Amn_pls.conjugate().dot(rho_c_pls.row(m))<<"\n";
	std::cout<<"a_mns"<<Amn_mns.conjugate().dot(rho_c_mns.row(m))<<"\n";
	counter++;}
      */}
  return Z;
}


void Potentials(int field_pt,int source_pt, int source_edge, int source_tri, double k, Eigen::MatrixXd r_c, Eigen::VectorXi dof2edge, int quad_pts, bool sing, double eps_0, double mu_0, double omega, std::complex<double>& ScalarPot, Eigen::VectorXcd& MagVecPot){
  ScalarPot=0.;
  MagVecPot=Eigen::VectorXcd::Zero(3);
  //We're gonna calculate the potentials following the simplified integrals proposed by RWG, that is equations 32-34d
  //defining the integrals
  std::complex<double>Ipq(0.,0.);
  std::complex<double>Ipq_xi(0.,0.);
  std::complex<double>Ipq_eta(0.,0.);
  std::complex<double>Ipq_zeta(0.,0.);
  //calcularing the integrals at integral_pq.cpp (Eq. 34a-34d)
  Integral_pq(field_pt,source_pt,r_c.row(field_pt),k,quad_pts,
	      sing,Ipq,Ipq_xi,Ipq_eta,Ipq_zeta);
  //calculating the potentials(Eq. 32-33)
  int i=DOFLOCALNUM(source_edge, source_tri);
  //definig r for eq 32, remember these go to the edges, so we can find them with nodecoords using the local edge numbers
  Eigen::VectorXi qnodes=ELEMENTS.row(source_pt);
  Eigen::MatrixXd r(3,3);
  r.row(0)=NODE_COORD.row(qnodes(0));
  r.row(1)=NODE_COORD.row(qnodes(1));
  r.row(2)=NODE_COORD.row(qnodes(2));
  MagVecPot=(mu_0*ELL(dof2edge(source_edge))/(4*M_PI))*
    (Ipq_xi*r.row(0)+Ipq_eta*r.row(1)+Ipq_zeta*r.row(2)-Ipq*r.row(i));
  ScalarPot=(-J*ELL(dof2edge(source_edge))*Ipq)/(2.*M_PI*omega*eps_0);
  //  MagVecPot<<(0.,0.),(0.,0.),(0.,0.);
  //  ScalarPot=0.+J*0.;
}
