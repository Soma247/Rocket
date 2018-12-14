#ifndef BALCALCITEM_H
#define BALCALCITEM_H
#include <QStandardItem>

class balcalcItem{
    enum class itemtype{Project,RocketModel,FlyTask,ConesCont,PlanesCont,EquipsCont,Cone,Plane,Equipment};

    explicit balcalcItem (itemtype type,QString name, balcalcItem *parent=nullptr);
    ~balcalcItem();

    void appendChild(balcalcItem *child); //Добавить узел-потомок
    balcalcItem *child(int position);//Вернуть дочерний элемент
    balcalcItem *parentItem(); //Вернуть родительский элемент
    void removeChildren(int position);  //Удалить потомка

    itemtype gettype()const;//Вернуть тип
    QString getName()const;//Вернуть имя
    int childCount() const; //Количество дочерних элементов
    int getPosition() const; //Вернуть номер строки элемента(c точки зрения предка)

private: //Внутреннее представление данных:
    QList<balcalcItem*> childrens; //Список дочерних элементов
    itemtype m_type; //тип данного узла
    QString m_name;  //наименование узла
    balcalcItem *m_parent; //Ссылка на родительский узел
};

#endif // BALCALCITEM_H
