#ifndef POSTPROCMOM_H
#define POSTPROCMOM_H

#include"globalVars.h"
#include<iostream>
#include<fstream>

void  PostProcessingMoM(Eigen::VectorXcd I, double EMag,
			Eigen::VectorXi dof2edge,double eta_0,
			double L,double W,
			int Xmesh,int Ymesh,int ProbType,
			int quad_pts,bool sing);

#endif
