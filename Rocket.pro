TEMPLATE = app
#CONFIG -= app_bundle
QT += widgets
CONFIG +=  c++14

SOURCES += \
    main.cpp \
    abstractatmosphere.cpp \
    rocketmodel.cpp \
    rocketcalcexceptions.cpp \
    interpolation/newtoninterp.cpp \
    aerodynamics/aerodynamics.cpp \
    aerodynamics/aerodynamics_tables.cpp \
    rocket_elements/rocketmodule.cpp \
    rocket_elements/modules/conoid.cpp \
    rocket_elements/modules/engine.cpp \
    rocket_elements/modules/nosecone.cpp \
    rocket_elements/equipment.cpp \
    rocket_elements/abstractrocketelement.cpp \
    rocket_elements/plane/plane.cpp \
    rocket_elements/modules/hfuel.cpp \
    ballisticcalc.cpp \
    Gui/ItemModel/balcalcitemmodel.cpp \
    Gui/workwindow.cpp \
    Gui/ItemModel/balcalcitem.cpp \
    Gui/Dialogs/addconoiddialog.cpp \
    Gui/Dialogs/setnoseconedialog.cpp \
    Gui/Dialogs/errordialog.cpp \
    Gui/Dialogs/addplanedialog.cpp \
    Gui/Dialogs/addequipmentdialog.cpp \
    Gui/Dialogs/setflytask.cpp \
    Gui/Dialogs/choosewindow.cpp


HEADERS += \
    abstractatmosphere.h \
    rocketmodel.h \
    rocketcalcexceptions.h \
    interpolation/newtoninterp.h \
    aerodynamics/aerodynamics.h \
    aerodynamics/aerodynamics_tables.h \
    rocket_elements/rocketmodule.h \
    rocket_elements/modules/conoid.h \
    rocket_elements/modules/engine.h \
    rocket_elements/modules/nosecone.h \
    rocket_elements/equipment.h \
    rocket_elements/abstractrocketelement.h \
    rocket_elements/plane/plane.h \
    rocket_elements/modules/hfuel.h \
    ballisticcalc.h \
    Gui/ItemModel/balcalcitemmodel.h \
    Gui/workwindow.h \
    Gui/ItemModel/balcalcitem.h \
    Gui/Dialogs/addconoiddialog.h \
    Gui/Dialogs/setnoseconedialog.h \
    Gui/Dialogs/errordialog.h \
    Gui/Dialogs/addplanedialog.h \
    Gui/Dialogs/addequipmentdialog.h \
    Gui/Dialogs/setflytask.h \
    Gui/Dialogs/choosewindow.h

DISTFILES += \
    garbage.txt \
    resources/cont_red_circle.png \
    hardfuels.txt \
    materials.txt

FORMS += \
    Gui/workwindow.ui \
    Gui/Dialogs/addconoiddialog.ui \
    Gui/Dialogs/setnoseconedialog.ui \
    Gui/Dialogs/errordialog.ui \
    Gui/Dialogs/addplanedialog.ui \
    Gui/Dialogs/addequipmentdialog.ui \
    Gui/Dialogs/setflytask.ui \
    Gui/Dialogs/choosewindow.ui
