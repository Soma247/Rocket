#include "abstractrocketelement.h"

std::ostream &operator<<(std::ostream &os, const matherial &mat){
    return os<<matherialheader<<'{'<<mat.name<<','<<mat.Ro<<','<<mat.SigmaV<<'}';
}

std::istream &operator>>(std::istream &in, matherial &mat){
    std::string name,header;
    char delim{0},footer{0};
    int tmp{0};
    double ro{0},sigma{0};
    while((tmp=in.get())!=EOF && isspace(tmp));
    in.unget();
    while((tmp=in.get())!=EOF && tmp!='{')
        header.push_back(static_cast<char>(tmp));

    if(tmp==EOF)return in;
    if(header!=matherialheader){
        in.clear(std::ios::failbit);
        return in;
    }
    while((tmp=in.get())!=EOF && tmp!=',')
        name.push_back(static_cast<char>(tmp));
    if(tmp==EOF)return in;

    in>>ro>>delim>>sigma>>footer;
    if(!in)return in;
    if(delim!=',' || footer!='}'|| ro<0 || sigma<0){
        in.clear(std::ios::failbit);
        return in;
    }
    mat=matherial(name,ro,sigma);
    return in;
}
