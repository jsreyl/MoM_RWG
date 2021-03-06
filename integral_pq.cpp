#include"integral_pq.h"

const std::complex<double> J(0.,1.);

//This calculates the 4 integrals necessary for finding the potentials in the normalized coordinates xi,eta,zeta.(Eq34a-34d)
//notice p is the field point and q is the source point. That is we (the observer) are standing on triangle p, and we measure the contribution of triangle q (i.e. we integrate over q)
void Integral_pq(int p, int q, Eigen::VectorXd r_cp,
		 double k,int quad_pts, bool sing,
		 std::complex<double>& Ipq, std::complex<double>& Ipq_xi,
		 std::complex<double>& Ipq_eta, std::complex<double>& Ipq_zeta){
  Eigen::VectorXi qnodes=ELEMENTS.row(q);
  Eigen::MatrixXd n(3,3);
  n.row(0)=NODE_COORD.row(qnodes(0));
  n.row(1)=NODE_COORD.row(qnodes(1));
  n.row(2)=NODE_COORD.row(qnodes(2));

  //Calculate the are of the triangle based on the coordinates of it's vertices
  double area=TriArea3D(n.row(0),n.row(1),n.row(2));
  //std::cout<<"triangle_area "<<area<<"\n";
  //Now onward to calculate the integrals
  Ipq=0.;Ipq_xi=0.;Ipq_eta=0.;Ipq_zeta=0.;

  if(p==q && sing){//Calculate the singularity point using a special integration rule (analytic approach and evaluation)
    //The integral is calculated approximating the singularity and integrating by Gauss-Lagrange quadrature
    IntegrateSingularitySGF(k,r_cp,n.row(0),n.row(1),n.row(2),3,4,
			    Ipq,Ipq_xi,Ipq_eta,Ipq_zeta);
    Ipq=Ipq/(2.*area);
    Ipq_xi=Ipq_xi/(2.*area);
    Ipq_eta=Ipq_eta/(2.*area);
    Ipq_zeta=Ipq_zeta/(2.*area);
    //As written, it's not clear why the 2 factor is necessary to give results that match the paper
  }else{
    Eigen::VectorXd w=Eigen::VectorXd::Zero(quad_pts);//Weights for the gaussian quadrature
    Eigen::MatrixXd lambda=Eigen::MatrixXd::Zero(quad_pts,3);//evaluation points for gaussian quadrature, these are equivalent to the normalized area coordinates xi, eta, zeta(Eq. 28)
    //calculate the above for a triangle depending on the quad_pts
    TriQuad(quad_pts, w, lambda);
    w=0.5*w;//As above, the factor of 2 is not clear

    for(int i=0;i<quad_pts;i++){
      //Calculate r', eq. 30
      Eigen::VectorXd r_prime(3); double R_p;
      r_prime=lambda(i,0)*n.row(0)+lambda(i,1)*n.row(1)+lambda(i,2)*n.row(2);
      //calculate R_p eq.27
      R_p=(r_cp-r_prime).norm();
      //We also need the green's function when evaluating the integrals(Eq.34a-34d)
      
      std::complex<double> Green(0.,0.);
      Green=std::exp(-J*k*R_p)/R_p;

      //Now the integration using the weights
      Ipq=Ipq+w(i)*Green;
      Ipq_xi=Ipq_xi+w(i)*lambda(i,0)*Green;
      Ipq_eta=Ipq_eta+w(i)*lambda(i,1)*Green;
    }
    Ipq_zeta=Ipq-Ipq_xi-Ipq_eta;
    }
}
