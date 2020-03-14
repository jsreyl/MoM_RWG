#include"find_local_dofs.h"

void FindLocalDofs(Eigen::VectorXi dof_flag,Eigen::VectorXi dof_RWG){
  DOFLOCALNUM.resize(NUM_DOFS,2);//We have two faces for each internal edge, we want to give them local numbers.
  //that is
  //DOFLOCALNUM(i_internal_edge)=local_face_0,local_face_1
  
  for(int i=0;i<NUM_EDGES;i++){//We search each edge to find the conected elements
    bool edge_found=0;//We ain't got no edge yet
    bool terminate=0;//We terminate once we've found two faces for the edge
    for(int j=0;j<NUM_ELEMENTS;j++){//check each triangle, some may be connected to our edge.
      for(int k=0;k<3;k++){//each edge of the triangle
	if(ELEMENT_EDGES(j,k)==i){//the edge belongs to the element
	  if(!edge_found){//and we haven't found an edge yet
	    if(dof_flag(i))//and the edge we found is internal
	      DOFLOCALNUM(dof_RWG(i),0)=k;//we asign the i'th element as the k'th local variable
	    edge_found=1;//Now we've found an edge
	  }else{
	    if(dof_flag(i))//if the other edge we found is internal
	      DOFLOCALNUM(dof_RWG(i),1)=k;//we asign the other triangle as the k'th local variable
	    terminate=1;//We've found two faces so we can terminate
	    break;
	  }
	}
      }
      if(terminate) break;
    }
  }
}
