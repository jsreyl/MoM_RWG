#include"edge_conx_elem.h"

void EdgeConxElem(Eigen::VectorXi dof_flag,Eigen::VectorXi dof_RWG){
  //Each interior edge is shared by two triangles
  EDGECONEXELEMS.resize(NUM_DOFS,2);
  //We want
  //EDGECONEXELEMS(i_internal_edge)=connected_triangle_1,connected_triangle_2
  ELEMENT_PLS_MNS.resize(NUM_ELEMENTS,3);
  
  for(int i=0;i<NUM_EDGES;i++)//We check every edge
    if(dof_flag(i)){//if it's an internal edge
      int counter=0;//Always two triangles per edge there are, element 0 and 1.
      for(int j=0;j<NUM_ELEMENTS;j++)//So we search every triangle
	for(int k=0;k<3;k++){//And check if one of it's edges
	  if(ELEMENT_EDGES(j,k)==i){//is the one we started with,
	    EDGECONEXELEMS(dof_RWG(i),counter)=j;//that is the triangle is connected by that edge
	    if(counter==0){//And we assign it a sign
	      ELEMENT_PLS_MNS(j,k)=1;
	    }else{
	      ELEMENT_PLS_MNS(j,k)=-1;}
	    counter++;
	  }
	}
    }
}
