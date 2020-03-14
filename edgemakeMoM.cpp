#include"edgemakeMoM.h"

void EdgeMakeMoM(){
  //We don't know the real number of edges (yet!) but the maximum number posssible is that when none of the triangles are connected, i.e. 3*NUM_ELEMENTS, instead of constantly resizing our edge array for each new element, we can fix it to this value and cut it down after the fact.
  EDGES.resize(3*NUM_ELEMENTS,2);
  //First element
  //EDGE(i)=(node1,node2); that is we want the indexes of the nodes that form the edge
  EDGES.row(0)<<ELEMENTS(0,LOCALEDGENODES(0,0)),ELEMENTS(0,LOCALEDGENODES(0,1));
  EDGES.row(1)<<ELEMENTS(0,LOCALEDGENODES(1,0)),ELEMENTS(0,LOCALEDGENODES(1,1));
  EDGES.row(2)<<ELEMENTS(0,LOCALEDGENODES(2,0)),ELEMENTS(0,LOCALEDGENODES(2,1));
  ELEMENT_EDGES.resize(NUM_ELEMENTS,3);
  Eigen::MatrixXi local_edges(3,1);
  ELEMENT_EDGES(0,0)=0;
  local_edges(0,0)=0;
  ELEMENT_EDGES(0,1)=1;
  local_edges(1,0)=1;
  ELEMENT_EDGES(0,2)=2;
  local_edges(2,0)=2;
  int edge_counter=3;
  //Other elements
  Eigen::MatrixXi TEMPEDGES(1,2);
  bool new_edge=0;//To check wheter we've assiged the edge already
  for(int i=1;i<NUM_ELEMENTS;i++)
    for(int j=0;j<3;j++){
      TEMPEDGES.row(0)<<ELEMENTS(i,LOCALEDGENODES(j,0)),ELEMENTS(i,LOCALEDGENODES(j,1));
      new_edge=1;//The edge is new unless we've assigned it already, let's check
      for(int k=0;k<edge_counter;k++){
	if (TEMPEDGES.row(0)==EDGES.row(k)){
	  new_edge=0;
	  ELEMENT_EDGES(i,j)=k;
	  break;
	}
      }
      if (new_edge){
	edge_counter++;
	EDGES.row(edge_counter-1)=TEMPEDGES.row(0);
	ELEMENT_EDGES(i,j)=edge_counter-1;
      }
    }
  NUM_EDGES=edge_counter;
  EDGES.conservativeResize(NUM_EDGES,2);
  //Edge lengths
  ELL.resize(NUM_EDGES);
  double x=0,y=0,z=0;
  for(int i=0;i<NUM_EDGES;i++){
    ELL(i)=(NODE_COORD.row(EDGES(i,1))-NODE_COORD.row(EDGES(i,0))).norm();
    }
}
