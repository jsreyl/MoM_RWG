#include<Eigen>
#include<cmath>

//Eigen counts from zero rather than from one like MatLab, thus the Matrix sizes should be one higher and we can fill the first element with zeroes when defining
Eigen::MatrixXi ELEMENTS(1,3);
Eigen::MatrixXd NODE_COORD(1,3);
Eigen::MatrixXi EDGES(1,2);
Eigen::MatrixXi ELEMENT_EDGES(1,3);
int NUM_NODES;
int NUM_ELEMENTS;
int NUM_EDGES;
Eigen::MatrixXi LOCALEDGENODES(3,2);
int NUM_DOFS;
Eigen::MatrixXi EDGECONEXELEMS(1,2);
Eigen::MatrixXi DOFLOCALNUM(1,2);
Eigen::VectorXd ELL(1);
Eigen::VectorXi LOCALVERTEX(3);
Eigen::MatrixXi ELEMENT_PLS_MNS(1,3);
