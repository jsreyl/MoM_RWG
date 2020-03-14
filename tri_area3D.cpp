#include"tri_area3D.h"

double TriArea3D(Eigen::Vector3d r1, Eigen::Vector3d r2, Eigen::Vector3d r3){
  //Calculating the area of a triangle is quite easy using linear algebra, wwe just need two vectors from one vertex to the other ones
  //their cross product will result in the area of a romboid, whic we divide by two to get the area of the triangle
  //Being r1, r2, r3 the coordinates of the vertices:
  Eigen::Vector3d area=0.5*((r1-r3).cross(r2-r3));
  return area.norm();
}
