#include<iostream>

int main(){
  std::cout<<"gnuplot -e \"plot sin(x); pause -1\"\n";
  system("gnuplot");
}
