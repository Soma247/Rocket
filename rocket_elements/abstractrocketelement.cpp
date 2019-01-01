#include "abstractrocketelement.h"
#include <iostream>
std::ostream &operator<<(std::ostream &os, const material &mat){
    return os<<materialheader<<'{'<<mat.name<<','<<mat.Ro<<','<<mat.SigmaV<<'}';
}

std::istream &operator>>(std::istream &in, material &mat){
    std::string name,header;
    char delim{0},footer{0};
    int tmp{0};
    double ro{0},sigma{0};
    while((tmp=in.get())!=EOF && isspace(tmp));

    if(tmp==EOF)return in;//налало файла +- пробелы =конец
    in.unget();//вернуть первый непробельный символ
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));
    if(tmp==EOF){//после хедера конец файла
        in.clear(std::ios::failbit);
        return in;
    }

    if(header!=materialheader){
        in.clear(std::ios::failbit);
        return in;
    }
    while((tmp=in.get())!=EOF && tmp!=',')
        name.push_back(static_cast<char>(tmp));
    if(tmp==EOF){
        in.clear(std::ios::failbit);
        return in;
    }

    in>>ro>>delim>>sigma>>footer;
    if(delim!=',' || footer!='}'|| ro<0 || sigma<0){
        in.clear(std::ios::failbit);
        return in;
    }
    mat=material(name,ro,sigma);
    return in;
}

material::material(const std::string &matherialName, double Ro, double sigma):name{matherialName},Ro{Ro},SigmaV{sigma}{}

material::material(const material &mat){
    name=mat.name;
    Ro=mat.Ro;
    SigmaV=mat.SigmaV;
}

bool material::operator==(const material &second) const{
    return name==second.name&&
            Ro==second.Ro&&
            SigmaV==second.SigmaV;
}
