#include"compute_rho_c.h"

void ComputeRho_c(Eigen::MatrixXd r_c, Eigen::MatrixXd& rho_c_pls, Eigen::MatrixXd& rho_c_mns){

  int elem_pls, elem_mns;
  int localnode_pls,localnode_mns;
  Eigen::MatrixXd vertex_pls(1,3);//These must be dynamic, since only same time matrix operations are allowed
  Eigen::MatrixXd vertex_mns(1,3);
  
  //Rho pls goes directed from the free vertex to the centre
  //Rho mns goes directed from the centre to the free vertex
  for(int i=0;i<NUM_DOFS;i++){//For each internal edge
    elem_pls=EDGECONEXELEMS(i,0);//There are two triangles, one plus
    elem_mns=EDGECONEXELEMS(i,1);//one minus
    //Each of them have a free vertex, we can use DOFLOCALNUMS, since the edge index is the same as the free vertex index
    localnode_pls=LOCALVERTEX(DOFLOCALNUM(i,0));
    localnode_mns=LOCALVERTEX(DOFLOCALNUM(i,1));
    //whose coordinates we want to know
    vertex_pls.row(0)= NODE_COORD.row(ELEMENTS(elem_pls,localnode_pls));
    vertex_mns.row(0)= NODE_COORD.row(ELEMENTS(elem_mns,localnode_mns));
    //Now the vector from the edge to the centre, remember r_c is the centre of the triangle, so we just substract vectors
    rho_c_pls.row(i)=r_c.row(elem_pls)-vertex_pls;
    rho_c_mns.row(i)=vertex_mns-r_c.row(elem_mns);
  }
}
