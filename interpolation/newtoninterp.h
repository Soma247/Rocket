#ifndef NEWTONINTERP_H
#define NEWTONINTERP_H
#include<vector>
#include <string>
struct mypoint{
    double abs;
    double ord;
    mypoint():abs(0),ord(0){}
    mypoint(double inx,double iny):abs(inx),ord(iny){}
    mypoint(const mypoint &inpoint):abs(inpoint.abs),ord(inpoint.ord){}
};//контейнер
typedef  std::vector<mypoint> interpVector;
typedef  std::vector<std::pair<double,std::vector<mypoint>>> interp3DMat;
typedef  std::vector<std::pair<double,interp3DMat>> interp4DMat;

class newtonInterp{
 public:
 static  double deltafun(size_t first, size_t last,double abs,const interpVector &arr,std::vector<std::vector<double>>& deltarray);//do not use this EVER

 static  std::vector<mypoint>getpowervec(const interpVector&array, double abs, size_t power);
 static  double interpthis(const interpVector&array, double abs);

public:
 static  double interp2d(const std::vector<double>& arrabs, const std::vector<double>& arrord, double abs, size_t power);
 static  double interp2d(const interpVector& array, double abs, size_t power);
 static  double interp3d(const interp3DMat& mat, double x, double z, size_t power_x,size_t power_z);
 static  double interp4d(const interp4DMat& mat, double x1, double x2, double x3, size_t power_x, size_t power_z,size_t power_k);
};
#endif // NEWTONINTERP_H


