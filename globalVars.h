#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include<Eigen>
#include<cmath>
#include<complex>

//Eigen counts from zero rather than from one like MatLab, thus the Matrix sizes should be one higher and we can fill the first element with zeroes when defining
extern Eigen::MatrixXi ELEMENTS;
extern Eigen::MatrixXd NODE_COORD;
extern Eigen::MatrixXi EDGES;
extern Eigen::MatrixXi ELEMENT_EDGES;
extern int NUM_NODES;
extern int NUM_ELEMENTS;
extern int NUM_EDGES;
extern Eigen::MatrixXi LOCALEDGENODES;
extern int NUM_DOFS;
extern Eigen::MatrixXi EDGECONEXELEMS;
extern Eigen::MatrixXi DOFLOCALNUM;
extern Eigen::VectorXd ELL;
extern Eigen::VectorXi LOCALVERTEX;
extern Eigen::MatrixXi ELEMENT_PLS_MNS;

#endif
