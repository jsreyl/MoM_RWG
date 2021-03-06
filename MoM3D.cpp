#include<iostream>
#include<cstdlib>

//Define indexation matrixes global to all scripts, declared at .h as external and defined at .cpp
#include"globalVars.h"//Includes eigen, cmath and complex

//Scripts containing functions to be used
#include"trimesh3D.h"
#include"triplot.h"
#include"edgemakeMoM.h"
#include"outside_edge.h"
#include"renumber_RWG.h"
#include"edge_conx_elem.h"
#include"find_local_dofs.h"
#include"compute_rho_c.h"
#include"fill_z_by_edge.h"
#include"fill_v_vector.h"
#include"post_proc_mom.h"

//Defining relevant constants
//Constants of nature, assuming free space, change as needed in the medium
const double eps_0=8.854e-12;//Electric Permitivity
const double mu_0 = 4.*M_PI*1e-7;//Magnetic Permeability
const double eta_0 = std::sqrt(mu_0/eps_0);//Impedance
const double c=1./std::sqrt(eps_0*mu_0);//Speed of light

//Incident field related quantities
const double frec = c;//Frecuency in Hertz
const double omega = 2.*M_PI*frec;//Angular frecuency, needed to calculate incident electric field
const double lambda = c/frec;//Wavelength
const double k = 2.*M_PI/lambda;//Wavenumber
const double EMag = 10;//Magnitude of the incident field

int main(){
  //Edge and vertex identation propierties
  LOCALEDGENODES.row(0)<<1,2;
  LOCALEDGENODES.row(1)<<0,2;
  LOCALEDGENODES.row(2)<<0,1;
  LOCALVERTEX(0)=0;
  LOCALVERTEX(1)=1;
  LOCALVERTEX(2)=2;
  //Problem type
  int ProbType=1;
  double L = 1.*lambda;
  double W =L;
  int Xmesh=6;
  int Ymesh=7;
  double theta_0 = 0;//Incident field angles
  double phi_0 = 0;
  std::cout<<"Choose problem type:\n";
  std::cout<<"0-flat plate L=0.15*lambda\n";
  std::cout<<"1-flat plate L=1.*lambda (Default)\n";
  std::cout<<"2-bent plate L=1.*lambda.\n";
  std::cin>>ProbType;
  switch (ProbType){
  case 0:
    L = 0.15*lambda;
    W =L;
    Xmesh=6;
    Ymesh=5;
    break;
  case 1:    
    break;
  case 2:
    L = 1.*lambda;
    W =L;
    Xmesh=6;
    Ymesh=6;
    theta_0=90.;
    phi_0=0;
    break;
  default:
    std::cout<<"Unknown problem\n";
    return 1;
    break;
  }

  bool sing=0;
  std::cout<<"Use singularity integration? 0->false, 1->true (Default 0):";
  std::cin>>sing;
  std::cout<<"singularity ="<<sing<<std::endl;

  //Define number of points to use in integration by quadrature
  int quad_pts=6;

  //Create the mesh
  TriMesh3D(L,W,Xmesh,Ymesh,ProbType);
  //Debugging
  /*  std::cout<<"Elements"<<ELEMENTS<<std::endl;
      std::cout<<"Node coords"<<NODE_COORD<<std::endl;*/

  //plotting the mesh on a 3d space via gnuplot
  TriPlot("mesh.data");

  Eigen::MatrixXd r_c(NUM_ELEMENTS,3);//Vector centres of the triangles
  for(int i=0;i<NUM_ELEMENTS;i++){
    r_c(i,0)=(NODE_COORD(ELEMENTS(i,0),0)+NODE_COORD(ELEMENTS(i,1),0)+NODE_COORD(ELEMENTS(i,2),0))/3.;
    r_c(i,1)=(NODE_COORD(ELEMENTS(i,0),1)+NODE_COORD(ELEMENTS(i,1),1)+NODE_COORD(ELEMENTS(i,2),1))/3.;
    r_c(i,2)=(NODE_COORD(ELEMENTS(i,0),2)+NODE_COORD(ELEMENTS(i,1),2)+NODE_COORD(ELEMENTS(i,2),2))/3.;
  }
  //  std::cout<<"r_c"<<r_c<<std::endl;

  //Now let's index our edges, also we need the edges length to calculate the base function used by Rao et al.
  EdgeMakeMoM();
  //  std::cout<<"ell"<<ELL<<std::endl;

  //We've got all the edges, but the base functions are calculated only on the edges that connect two triangles, so out actual number of degrees of freedom must exclude the outside edges
  Eigen::VectorXi dof_int = OutsideEdge();//This code assumes the geometry is rectangular
  //  std::cout<<"dof"<<dof_int<<std::endl;

  //Now that we know which edges we can use, let's renumber them
  Eigen::VectorXi dof_RWG;//i'th edge corresponds to j'th dof
  Eigen::VectorXi dof2edge;//j'th dof corresponds to i'th edge in the geometry
  RenumberRWG(dof_int,dof_RWG,dof2edge);//Last two are passed by reference so they can be modified
  /*  std::cout<<"dof_RWG"<<dof_RWG<<std::endl;
      std::cout<<"dof2edge"<<dof2edge<<std::endl;*/

  //The base functions also need to know for a certain edge, the sign index of two triangles connected, so we can create a connection matrix,
  //That is given an edge, which two elements does it connect and also what are the sign index of those
  EdgeConxElem(dof_int,dof_RWG);
  //std::cout<<"edge_pls_mns"<<ELEMENT_PLS_MNS<<std::endl;

  //We also want to be able to get local numbers for each face connected to an edge
  FindLocalDofs(dof_int,dof_RWG);
  //  std::cout<<"local_dofs"<<DOFLOCALNUM<<std::endl;

  //Now for the base function we need the vectors from the free vertex to the center of the triangle.
  Eigen::MatrixXd rho_c_pls(NUM_DOFS,3);
  Eigen::MatrixXd rho_c_mns(NUM_DOFS,3);
  ComputeRho_c(r_c,rho_c_pls,rho_c_mns);
  //  std::cout<<"rho_pls"<<rho_c_pls<<std::endl;
  //  std::cout<<"rho_mns"<<rho_c_mns<<std::endl;

  //This is where the fun begins

  //We want to solve I in our EFIE system
  //Z*I=V
  
  //Let's first fill the impedance matrix, we'll use the same method proposed by RWG, that is we'll reduce the EFIE equation to solving 4 integrals.
  //Eigen manages matrixes with coeficients belonging to std::complex, which is usefull considering the electric field is complex and consequently so are the potentials and other quantities calculated
  Eigen::MatrixXcd Z=Eigen::MatrixXcd::Zero(NUM_DOFS,NUM_DOFS);
  Z=FillZMatrixByEdge(omega, eps_0,mu_0,k,r_c,rho_c_pls,rho_c_mns,quad_pts,sing,dof2edge);
  //  std::cout<<"Z\n"<<Z<<std::endl;

  //Now we can fill the vector V, corrsponding to the incident electric field
  //That is eq 18
  Eigen::VectorXcd V=Eigen::VectorXcd::Zero(NUM_DOFS);
  V = FillVVector(rho_c_pls,rho_c_mns,EMag,theta_0,phi_0,dof2edge);
  //  std::cout<<"V\n"<<V<<std::endl;

  //Now we can finaly solve for I
  //V=Z*I => I =V*inverse(Z)
  Eigen::VectorXcd I=Eigen::VectorXcd::Zero(NUM_DOFS);

  I=Z.inverse()*V;
  //I=Z.colPivHouseholderQr().solve(V);
  //std::cout<<"I\n"<<I<<std::endl;

  PostProcessingMoM(I,EMag,dof2edge,eta_0,L,W,
  Xmesh,Ymesh,ProbType,quad_pts,sing);
  
  return 0;
}


