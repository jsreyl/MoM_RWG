//Here we indexate the triangles and nodes of the geometry, as well as giving the coordinates to each node.
//In this case we're building a flat rectangular plate with sides of length a and b divided in steps of Nx and Ny "squares" divided diagonally to create two triangles
//a.k.a. this is the first geometry by Rao et. al. in their 1982 paper
#include"trimesh3D.h"

void TriMesh3D(double a, double b, int Nx, int Ny, int ProbType){
  //Element index asignation doesn't depend on problem type, what depends are the coordinates of the nodes and how they are indexed.
  //On a future update NUM_ELEMENTS can be given as an argument along with a list of coordinates, that way non-rectangular shapes can be studied
  NUM_ELEMENTS=2*Nx*Ny;
  //Number the triangles the object is made of from lowest to highest
  ELEMENTS=Eigen::MatrixXi::Zero(NUM_ELEMENTS,3);//The matrix that will contain element->Vertex relations
  for(int j=0;j<Ny;j++)
    for(int i=0;i<Nx;i++){
      int elem_offset=j*2*Nx;
      int node_offset=j*(Nx+1);
      //Lower row L
      ELEMENTS.row(i+elem_offset)<<i+node_offset,i+1+node_offset,i+Nx+1+node_offset;
      //Upper row "Inverted L"
      ELEMENTS.row(i+elem_offset+Nx)<<i+1+node_offset,i+Nx+1+node_offset,i+Nx+2+node_offset;
    }
  NUM_NODES=(Nx+1)*(Ny+1);
  //Coordinates for each node, indexated as above
  double del_x=a/(1.*Nx);
  double del_y=b/(1.*Ny);
  int counter=0;
  NODE_COORD=Eigen::MatrixXd::Zero(NUM_NODES,3);
  switch(ProbType){
  case 0:{
    for(int j=0;j<Ny+1;j++)
      for(int i=0;i<Nx+1;i++){
	NODE_COORD.row(counter)<<i*del_x,j*del_y,0.0;//flat plate lying on z=0
	counter++;
      }
    break;
  }
  case 1:{
    for(int j=0;j<Ny+1;j++)
      for(int i=0;i<Nx+1;i++){
	NODE_COORD.row(counter)<<i*del_x,j*del_y,0.0;//flat plate lying on z=0
	counter++;
      }
    break;
  }
  case 2:{
    double Nx_flat=2*Nx/3.;
    for(int j=0;j<Ny+1;j++)
      for(int i=0;i<Nx+1;i++){
	if(i<Nx_flat)
	  NODE_COORD.row(counter)<<i*del_x,j*del_y,0.0;//flat plate
	else
	  NODE_COORD.row(counter)<<(Nx_flat)*del_x+(i-Nx_flat)*cos(50*M_PI/180.)*del_x,j*del_y,-(i-Nx_flat)*sin(50*M_PI/180.)*del_x;
	//bent at 2/3rds of it's lenth at an angle of 50°
	counter++;
      }
    break;
  }
  }
}
