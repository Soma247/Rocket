#ifndef AERODYNAMICS_TABLES_H
#define AERODYNAMICS_TABLES_H
#include "interpolation/newtoninterp.h"

struct aerodynamics_tables{
    static interp3DMat& get_5_1_CxpNoseCone();
    static interp4DMat& get_5_5_CxpStern();
    static interpVector& get_5_8_CpSternNu1();//коэфф донного сопр без сужения
    static interp3DMat& get_5_9_knu();
    static interp4DMat& get_5_10_CxWavePlane();//волнового сопр крыльев
    static interpVector& get_5_13_fi_CxWavePlane();
    static interp3DMat& get_7_2_CyaNoseCyll();
    static interp4DMat& get_7_5_CyaPlane();
};

#endif // AERODYNAMICS_TABLES_H
