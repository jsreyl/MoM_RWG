#include"outside_edge.h"

//Assuming the geometry is rectangular, the outer edges are simple to identify, just find the minumum and maximum values, then if any edge has both nodes in any of these coordinates, that means they're outer edges.
Eigen::VectorXi OutsideEdge(){
  Eigen::VectorXi dof_flag=Eigen::VectorXi::Ones(NUM_EDGES);//All edges are valid dofs until proven otherwise
  //Get max and min values to check for outer edges
  Eigen::Vector3d coord_min;
  Eigen::Vector3d coord_max;
  coord_min(0)=NODE_COORD.col(0).minCoeff();//x_min
  coord_min(1)=NODE_COORD.col(1).minCoeff();//y_min
  coord_min(2)=NODE_COORD.col(2).minCoeff();//z_min
  coord_max(0)=NODE_COORD.col(0).maxCoeff();//x_max
  coord_max(1)=NODE_COORD.col(1).maxCoeff();//y_max
  coord_max(2)=NODE_COORD.col(2).maxCoeff();//z_max

  //Now let's check all the edges and identify those who are outsiders
  double eps=1e-8;
  for(int i=0;i<NUM_EDGES;i++){
    int node0=EDGES(i,0); int node1=EDGES(i,1);
    if(abs(NODE_COORD(node0,1)-coord_min(1))<eps && abs(NODE_COORD(node1,1)-coord_min(1))<eps)// i.e. if y is the min
      dof_flag(i)=0;//This be not an internal edge
    if(abs(NODE_COORD(node0,1)-coord_max(1))<eps && abs(NODE_COORD(node1,1)-coord_max(1))<eps)// i.e. if y is the max
      dof_flag(i)=0;
    if(abs(NODE_COORD(node0,0)-coord_min(0))<eps && abs(NODE_COORD(node1,0)-coord_min(0))<eps)// i.e. if x is the min
      dof_flag(i)=0;
    if(abs(NODE_COORD(node0,0)-coord_max(0))<eps && abs(NODE_COORD(node1,0)-coord_max(0))<eps)// i.e. if x is the max
      dof_flag(i)=0;
  }
  return dof_flag;
}
