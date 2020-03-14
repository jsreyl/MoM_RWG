#include"renumber_RWG.h"

void RenumberRWG(Eigen::VectorXi dof_free_flag, Eigen::VectorXi& dof_num, Eigen::VectorXi& dof2edge){
  dof_num.resize(NUM_EDGES);
  dof2edge.resize(NUM_EDGES);//this one might be smaller, we can trim it down after we're done
  int counter=0;
  for(int i=0;i<NUM_EDGES;i++)
    if(dof_free_flag(i)){
      dof_num(i)=counter;
      dof2edge(counter)=i;
      counter++;
    }
  NUM_DOFS=counter;
  //Trimming down
  dof2edge.conservativeResize(NUM_DOFS);
}
