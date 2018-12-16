#include "balcalcitem.h"

balcalcItem::balcalcItem(balcalcItem::itemtype type, QString name, balcalcItem *parent):
    m_type{type},m_name{name},m_parent{parent}{}

balcalcItem::~balcalcItem(){
   qDeleteAll(childrens);
}

void balcalcItem::appendChild(balcalcItem *child){
    if(child){
        child->setparent(this);
        childrens.push_back(child);
    }
}

balcalcItem *balcalcItem::child(int position){
    if(position<0||position>childrens.size()-1)return nullptr;
    return childrens.at(position);
}

balcalcItem *balcalcItem::parentItem(){
    return m_parent;
}

bool balcalcItem::insertChildren(int position, int, int){
    if(position<0||position>childrens.size()-1)return false;
    childrens.insert(position,new balcalcItem(balcalcItem::itemtype::TailStabCont,"some child"));
    return true;
}

bool balcalcItem::removeChildren(int position, int count){

    for(;count>0&&position<=childrens.size()-1;count--)
        delete childrens.takeAt(count);
    return true;
}

bool balcalcItem::removeAllChildrens(){
    while (childrens.size())
        delete childrens.takeLast();

    return true;
}

bool balcalcItem::removeChildren(int position){
    if(position<0||position>childrens.size()-1)return 0;
    delete childrens.takeAt(position);
    return 1;
}

void balcalcItem::setparent(balcalcItem *parent){
    m_parent=parent;
}

balcalcItem::itemtype balcalcItem::gettype() const{
    return m_type;
}

QString balcalcItem::getName() const{
    return m_name;
}

int balcalcItem::childCount() const{
    return childrens.size();
}

int balcalcItem::getPosition() const{
    if(!m_parent)return  0;
    return m_parent->childrens.indexOf(const_cast<balcalcItem*>(this));


}
