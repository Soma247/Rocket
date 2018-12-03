TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

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
    ballisticcalc.cpp


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
    ballisticcalc.h

DISTFILES += \
    garbage.txt
