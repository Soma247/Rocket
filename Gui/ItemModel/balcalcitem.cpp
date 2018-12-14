#include "balcalcitem.h"

balcalcItem::balcalcItem(balcalcItem::itemtype type, QString name, balcalcItem *parent):
    m_type{type},m_name{name},m_parent{parent}{}

balcalcItem::~balcalcItem(){
    while(childrens.size())
        delete childrens.takeLast();
}

void balcalcItem::appendChild(balcalcItem *child){
    childrens.push_back(child);
}

balcalcItem *balcalcItem::child(int position){
    return childrens.at(position);
}

balcalcItem *balcalcItem::parentItem(){
    return m_parent;
}

void balcalcItem::removeChildren(int position){
    if(position<0||position>childrens.size())return;
    delete childrens.takeAt(position);
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
    if(m_parent)
        return m_parent->childrens.indexOf(const_cast<balcalcItem*>(this));
    return 0;
}
