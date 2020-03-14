#ifndef GLQUADRULE_H
#define GLQUADRULE_H

#include<Eigen>
#include<iostream>

void GLQuadRule(int num_pts, int unit_length, Eigen::VectorXd& L_i, Eigen::VectorXd& W_i);

#endif
