#include "interpolation/newtoninterp.h"
#include <exception>
#include <algorithm>
#include <iostream>
double newtonInterp::deltafun(size_t first, size_t last, double abs,
                              const interpVector &arr,
                              std::vector<std::vector<double>>& deltarray){
    if(first==last-1) {return (arr[last].ord-arr[first].ord)/(arr[last].abs-arr[first].abs);}
    else if(first>=last)throw std::exception();
    else{
        double delta1{0},delta2{0};
        delta1=(deltarray[first+1][last]!=0.0)?deltarray[first+1][last]:deltafun(first+1,last,abs,arr,deltarray);
        delta2=(deltarray[first][last-1]!=0.0)?deltarray[first][last-1]:deltafun(first,last-1,abs,arr,deltarray);
        deltarray[first][last]=(delta1-delta2)/(arr[last].abs-arr[first].abs);
        return deltarray[first][last];
    }
}

interpVector newtonInterp::getpowervec(const interpVector &array, double abs, size_t power){
    if(power>=array.size())
        return interpVector{array};

    interpVector::const_iterator begin, end, cur;
    cur=std::find_if(array.begin(),array.end(),[&](const mypoint& mp){return mp.abs>=abs;});

    begin=cur-power/2;
    end=begin+power;

    if(begin<array.begin()){
        begin=array.begin();
        end=begin+power;
    }
    else if(end>array.end()){
        end=array.end();
        begin=end-power;
    }
    interpVector temp(power);
    std::copy(begin,end,temp.begin());
    return temp;
}

double newtonInterp::interpthis(const interpVector& array, double abs){
    
    size_t PolyPower{array.size()};
    double Psumm{array.begin()->ord},delt{1};
    
    std::vector<std::vector<double>>deltarr(PolyPower);
    for(auto& line:deltarr)
        line.resize(PolyPower);
    
    for (size_t i=1;i<PolyPower;i++){
        delt=1;
        for(size_t j=0;j<i;j++)
            delt*=abs-array[j].abs;
        Psumm+=delt*deltafun(0,i,abs,array,deltarr);
    }
    return Psumm;
}

double newtonInterp::interp2d(const interpVector& array,double abs, size_t power){
    return interpthis(getpowervec(array,abs,power),abs);
}

double newtonInterp::interp2d(const std::vector<double>& arrabs, const std::vector<double>& arrord,double abs, size_t power){
    size_t size=std::min(arrabs.size(),arrord.size());
    interpVector mparr(size);
    for(size_t i=0;i<size;i++){
        mparr[i].abs=arrabs[i];
        mparr[i].ord=arrord[i];
    }
    return interpthis(getpowervec(mparr,abs,power),abs);
}

//интерполируется вектор y(z=z0), состоящий из значений абсциссыz и ординаты у, полученной интерполяцией вектора у(x=x0,z=zi)
double newtonInterp::interp3d(const interp3DMat& mat, double x,  double z, size_t power_x, size_t power_z){//графики z(x,y) у меньше точек чем х и z, то-есть каждая таблица по z с графиком y(x)
    std::vector<mypoint>zy(mat.size());//вектор интерполированных точек у(z,x=const)

    for(size_t i=0;i<mat.size();i++){
        zy[i].abs=mat[i].first;//по z
        zy[i].ord=interp2d(mat[i].second,x,power_x);//у(z=mat[i].first,x=const)
    }

/*    for(mypoint& mp:zy)
        std::cout<<"z="<<mp.abs<<" y="<<mp.ord<<std::endl;*/
    return interp2d(zy,z,power_z);//интерполируем вектор y(z,x=const) и ищем y(z=const,x=const)
}

double newtonInterp::interp4d(const interp4DMat& mat, double x1, double x2, double x3, size_t power_x, size_t power_z, size_t power_k){
     std::vector<mypoint>ky(mat.size());//вектор у(к), где кi заданы первым членом пары, а уi(х=х0, z=z0,ki) через куб интерп
     for(size_t i=0;i<mat.size();i++){
         ky[i].abs=mat[i].first;//по k
         ky[i].ord=interp3d(mat[i].second,x1,x2,power_x,power_z);//у(z=mat[i].first,x=const)
     }
    return interp2d(ky,x3,power_k);
}

