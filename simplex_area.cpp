#include"simplex_area.h"

Eigen::Vector3d SimplexArea(Eigen::Vector3d q,Eigen::Vector3d a,
			    Eigen::Vector3d b, Eigen::Vector3d c){
  //define two vectors along the triangle sides and one from the vertex to the centre
  Eigen::Vector3d ap=a-c;
  Eigen::Vector3d bp=b-c;
  Eigen::Vector3d qp=q-c;
  double denom=ap.squaredNorm()*bp.squaredNorm()-ap.dot(bp)*ap.dot(bp);
  double alpha=(bp.squaredNorm()*ap.dot(qp)-ap.dot(bp)*bp.dot(qp))/denom;
  double beta=(ap.squaredNorm()*bp.dot(qp)-ap.dot(bp)*ap.dot(qp))/denom;

  Eigen::Vector3d simplex_areas=Eigen::Vector3d::Zero(3);
  simplex_areas<<alpha,beta,1.-alpha-beta;
  return simplex_areas;
}
