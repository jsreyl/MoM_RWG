#include"post_proc_mom.h"

void  PostProcessingMoM(Eigen::VectorXcd I, double EMag,
			Eigen::VectorXi dof2edge,double eta_0,
			double L,double W,
			int Xmesh,int Ymesh,int ProbType,
			int quad_pts,bool sing){
  Eigen::VectorXcd I_norm = eta_0*I/EMag;

  Eigen::MatrixXd ell_vec = Eigen::MatrixXd::Zero(NUM_EDGES,3);
  for(int i=0;i<NUM_EDGES;i++){
    ell_vec.row(i)=NODE_COORD.row(EDGES(i,1))-NODE_COORD.row(EDGES(i,0));
    ell_vec.row(i)=ell_vec.row(i)/ELL(i);
  }
  //Plot currents directed in x and cut at x=L/2, as in RWG problem 1 and 2
  double tol=ELL.maxCoeff()/1e8;
  //AA cut
  int ycounter=0;
  Eigen::VectorXcd InormAA(NUM_DOFS);//we don't know how many times we must count, but for sure it is less or equal than NUM_DOFS, so let make it as long as possible and cut later
  Eigen::VectorXd y_vert(NUM_DOFS);
  if(Xmesh%2==0){//The mesh is even in X
    for(int i=0;i<NUM_DOFS;i++){
      if(abs(ell_vec(dof2edge(i),1)-1.)<tol && abs(NODE_COORD(EDGES(dof2edge(i),0),0)-L/2.)<tol){//i.e. if distance across y=0
	ycounter++;
	InormAA(ycounter)=I_norm(i);
	y_vert(ycounter)=0.5*(NODE_COORD(EDGES(dof2edge(i),0),1)+NODE_COORD(EDGES(dof2edge(i),1),1));
      }
    }
  }else if(Xmesh%2==1){//The mesh is odd in X
    for(int i=0;i<NUM_DOFS;i++){
      double x_c = 0.5*(NODE_COORD(EDGES(dof2edge(i),0),0)+NODE_COORD(EDGES(dof2edge(i),1),0));//we search for the coordinate athe midle of every "cell" and cut in L/2
      if(abs(ell_vec(dof2edge(i),1.)-1)<tol && abs(x_c-L/2.)<tol){//i.e. if distance across y=0
	ycounter++;
	InormAA(ycounter)=I_norm(i);
	y_vert(ycounter)=0.5*(NODE_COORD(EDGES(dof2edge(i),0),1)+NODE_COORD(EDGES(dof2edge(i),1),1));
      }
    }
  }
  InormAA.conservativeResize(ycounter);
  y_vert.conservativeResize(ycounter);
  //BB cut
  //For bent plate (case 2) we want to measure the bent part aswell, thus the length for those triangles must be calculated as sqrt((x2-x1)^2+(z2-z1))
  int xcounter=0;
  Eigen::VectorXcd InormBB(NUM_DOFS);//we don't know how many times we must count, but for sure it is less or equal than NUM_DOFS, so let make it as long as possible and cut later
  Eigen::VectorXd x_hor(NUM_DOFS);
  double norm_side_length=0.;
  if(Ymesh%2==0){//The mesh is even in Y
    for(int i=0;i<NUM_DOFS;i++){
      norm_side_length=ell_vec(dof2edge(i),1);
      if(ProbType==2)//bent plate
	norm_side_length=sqrt(ell_vec(dof2edge(i),0)*ell_vec(dof2edge(i),0)+ell_vec(dof2edge(i),2)*ell_vec(dof2edge(i),0));		   
      if(abs(norm_side_length-1.)<tol && abs(NODE_COORD(EDGES(dof2edge(i),0),1)-L/2.)<tol){//i.e. if distance across y=0
	xcounter++;
	InormBB(xcounter)=I_norm(i);
	x_hor(xcounter)=0.5*(NODE_COORD(EDGES(dof2edge(i),0),0)+NODE_COORD(EDGES(dof2edge(i),1),0));
      }
    }
  }else if(Ymesh%2==1){//The mesh is odd in Y
    for(int i=0;i<NUM_DOFS;i++){
      double y_c = 0.5*(NODE_COORD(EDGES(dof2edge(i),0),1)+NODE_COORD(EDGES(dof2edge(i),1),1));//we search for the coordinate athe midle of every "cell" and cut in L/2
      norm_side_length=ell_vec(dof2edge(i),1);
      if(ProbType==2)//bent plate
	norm_side_length=sqrt(ell_vec(dof2edge(i),0)*ell_vec(dof2edge(i),0)+ell_vec(dof2edge(i),2)*ell_vec(dof2edge(i),0));		   
      if(abs(norm_side_length-1.)<tol && abs(y_c-L/2.)<tol){//i.e. if distance across y=0
	xcounter++;
	InormBB(xcounter)=I_norm(i);
	x_hor(xcounter)=0.5*(NODE_COORD(EDGES(dof2edge(i),0),0)+NODE_COORD(EDGES(dof2edge(i),1),0));
      }
    }
  }
  InormBB.conservativeResize(xcounter);
  x_hor.conservativeResize(xcounter);

  //Graphs are way cuter on python so we'll print the values on a .csv that python can read and graph from
  std::ofstream mf("BB_cut.csv");
  std::ofstream mf2("AA_cut.csv");
  //column titles for pandas
  mf<<"x_hor,InormBB\n";
  mf2<<"y_vert,InormAA\n";
  for(int i=0;i<xcounter;i++){
    mf<<x_hor(i)<<","<<abs(InormBB(i))<<"\n";
  }
  for(int i=0;i<ycounter;i++){
    mf2<<y_vert(i)<<","<<abs(InormAA(i))<<"\n";
  }
  mf.close();
  mf2.close();
  if(ProbType==0)
    system("python3 PostProcMoM.py 0 0.15 0.15 6");
  else if(ProbType==1)
    system("python3 PostProcMoM.py 1 1.0 1.0 6");
  else if(ProbType==2)
    system("python3 PostProcMoM.py 2 1.0 1.0 6");
}
