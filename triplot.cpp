#include"triplot.h"

void TriPlot(const char* filename){
  std::ofstream mf(filename);
  mf<<NODE_COORD<<"\n";
  //for(int i=0;i<NODE_COORD.rows();i++)
  //  mf<<NODE_COORD(i,0)<<" "<<NODE_COORD(i,1)<<NODE_COORD(i,2)<<"\n";
  mf.close();
  std::cout<<"\n Entering gnuplot for visualization. Press [RETURN] to continue once you're done...\n\n ";
  system("gnuplot -e \"splot 'mesh.data' w lp;pause -1\"");
}
