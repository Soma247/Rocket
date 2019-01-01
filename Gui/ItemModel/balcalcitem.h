#ifndef BALCALCITEM_H
#define BALCALCITEM_H
#include <QStandardItem>

class balcalcItem{
public:
    enum class itemtype{Project,RocketModel,FlyTask,Nosecone,NoseconeCont,
                        TailStab,TailStabCont,Cone,ConesCont,PlanesCont,
                        Plane,Equipment,EquipsCont};//cont=container

    explicit balcalcItem (itemtype type,QString name, balcalcItem *parent=nullptr);
    ~balcalcItem();
    void appendChild(balcalcItem *child); //Добавить узел-потомок
    balcalcItem *child(int position);//Вернуть дочерний элемент
    int childCount() const; //Количество дочерних элементов
    int columnCount()const{return 1;}
    QVariant data(int, int role)const;
    int childNumber()const{return getPosition();}
    balcalcItem *parentItem(); //Вернуть родительский элемент
    bool insertChildren(int position, int,int); //Вставить потомков (строки)
    bool insertColumns(int, int){return true;} //Вставить столбцы
    bool removeChildren(int position, int count);
    bool removeAllChildrens();
    bool removeColumns(int , int ){return true;} //Удалить столбцы
    bool setData(int , const QVariant &value); //Установить данные
    bool removeChildren(int position);  //Удалить потомка
    void setparent(balcalcItem *parent);//установить предка(если неизвестен при создании)
    itemtype gettype()const;//Вернуть тип
    QString getName()const;//Вернуть имя
    int getPosition() const; //Вернуть номер строки элемента(c точки зрения предка)

private: //Внутреннее представление данных:
    QList<balcalcItem*> childrens; //Список дочерних элементов
    itemtype m_type; //тип данного узла
    QString m_name;  //наименование узла
    balcalcItem *m_parent; //Ссылка на родительский узел
};

#endif // BALCALCITEM_H
