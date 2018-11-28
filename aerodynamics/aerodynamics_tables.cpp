#include "aerodynamics_tables.h"
#include <float.h>


interp3DMat &aerodynamics_tables::get_5_1_CxpNoseCone(){
    static interp3DMat _5_1_CxpNoseCone{
        std::make_pair(1.5,
                       interpVector{
                           {0.6,0.044},
                           {0.8,0.120},
                           {1.0,0.340},
                           {1.2,0.390},
                           {1.4,0.372},
                           {1.6,0.335},
                           {1.8,0.308},
                           {2.0,0.286},
                           {2.5,0.268},
                           {3.0,0.252},
                           {3.5,0.245},
                           {4.0,0.240},
                           {4.5,0.237},
                           {5.0,0.236},
                       }
                       ),
                std::make_pair(2.0,
                               interpVector{
                                   {0.6,0.020},
                                   {0.8,0.120},
                                   {1.0,0.230},
                                   {1.2,0.279},
                                   {1.4,0.220},
                                   {1.6,0.200},
                                   {1.8,0.187},
                                   {2.0,0.178},
                                   {2.5,0.163},
                                   {3.0,0.150},
                                   {3.5,0.147},
                                   {4.0,0.142},
                                   {4.5,0.140},
                                   {5.0,0.138},
                               }
                               ),
                std::make_pair(2.5,
                               interpVector{
                                   {0.6,0.0150},
                                   {0.8,0.0350},
                                   {1.0,0.1800},
                                   {1.2,0.2000},
                                   {1.4,0.1600},
                                   {1.6,0.1450},
                                   {1.8,0.1350},
                                   {2.0,0.1300},
                                   {2.5,0.1200},
                                   {3.0,0.1150},
                                   {3.5,0.1090},
                                   {4.0,0.1030},
                                   {4.5,0.1010},
                                   {5.0,0.0980},
                               }
                               ),
                std::make_pair(3.0,
                               interpVector{
                                   {0.6,0.0045},
                                   {0.8,0.0200},
                                   {1.0,0.1200},
                                   {1.2,0.1400},
                                   {1.4,0.1200},
                                   {1.6,0.1100},
                                   {1.8,0.1050},
                                   {2.0,0.1000},
                                   {2.5,0.0920},
                                   {3.0,0.0850},
                                   {3.5,0.0800},
                                   {4.0,0.0780},
                                   {4.5,0.0760},
                                   {5.0,0.0750},
                               }
                               ),
                std::make_pair(4.0,
                               interpVector{
                                   {0.6,0.000},
                                   {0.8,0.020},
                                   {1.0,0.080},
                                   {1.2,0.090},
                                   {1.4,0.080},
                                   {1.6,0.075},
                                   {1.8,0.065},
                                   {2.0,0.063},
                                   {2.5,0.060},
                                   {3.0,0.058},
                                   {3.5,0.052},
                                   {4.0,0.051},
                                   {4.5,0.050},
                                   {5.0,0.050},
                               }
                               ),
                std::make_pair(5.0,
                               interpVector{
                                   {0.6,0.000},
                                   {0.8,0.010},
                                   {1.0,0.040},
                                   {1.2,0.060},
                                   {1.4,0.057},
                                   {1.6,0.056},
                                   {1.8,0.055},
                                   {2.0,0.055},
                                   {2.5,0.042},
                                   {3.0,0.040},
                                   {3.5,0.039},
                                   {4.0,0.038},
                                   {4.5,0.037},
                                   {5.0,0.038},
                               }
                               )

    };
    return  _5_1_CxpNoseCone;
}

interp4DMat& aerodynamics_tables::get_5_5_CxpStern(){
    static interp4DMat _5_5_CxpStern{

                std::make_pair(0,
                               interp3DMat {
                                       std::make_pair(2.0,
                                           interpVector{
                                               {0.6,0.075},
                                               {0.8,0.080},
                                               {1.0,0.120},
                                               {1.1,0.148},
                                               {1.2,0.145},
                                               {1.4,0.128},
                                               {1.6,0.115}
                                           }
                                       ),
                                       std::make_pair(3.0,
                                           interpVector{
                                               {0.6,0.072},
                                               {0.8,0.075},
                                               {1.0,0.102},
                                               {1.1,0.108},
                                               {1.2,0.106},
                                               {1.4,0.095},
                                               {1.6,0.088},
                                               {1.8,0.080}
                                           }
                                       ),
                                       std::make_pair(4.0,
                                           interpVector{
                                               {0.6,0.055},
                                               {0.8,0.067},
                                               {1.0,0.076},
                                               {1.1,0.080},
                                               {1.2,0.079},
                                               {1.4,0.069},
                                               {1.6,0.060},
                                               {1.8,0.055}
                                           }
                                       )
                                 }
                ),

                std::make_pair(0.5,
                               interp3DMat {
                                   std::make_pair(2.0,//lambdak=2
                                       interpVector{
                                           {0.6,0.061},
                                           {0.8,0.060},
                                           {1.0,0.068},
                                           {1.2,0.075},
                                           {1.3,0.080},
                                           {1.4,0.075},
                                           {1.6,0.067},
                                           {1.8,0.060},
                                           {2.2,0.055},
                                           {2.6,0.045}
                                       }
                                   ),
                                   std::make_pair(2.5,
                                       interpVector{
                                           {0.6,0.060},
                                           {1.4,0.050},
                                           {2.2,0.038},
                                           {2.6,0.036},
                                           {3.0,0.035}
                                       }
                                   ),
                                   std::make_pair(3.0,
                                       interpVector{
                                           {0.6,0.050},
                                           {0.8,0.042},
                                           {1.0,0.039},
                                           {1.2,0.035},
                                           {1.4,0.035},
                                           {1.6,0.037},
                                           {1.8,0.038},
                                           {2.2,0.033},
                                           {2.6,0.031},
                                           {3.0,0.030}
                                       }
                                   )

                               }
                  ),

                  std::make_pair(0.75,
                                 interp3DMat {
                                     std::make_pair(1.5,//lambdak=2
                                               interpVector{
                                                   {0.6,0.048},
                                                   {0.8,0.040},
                                                   {1.0,0.037},
                                                   {1.2,0.035},
                                                   {1.4,0.039},
                                                   {1.6,0.040},
                                                   {1.8,0.035},
                                                   {2.2,0.028},
                                                   {2.6,0.021}
                                               }
                                           ),
                                           std::make_pair(2.0,
                                               interpVector{
                                                   {0.6,0.041},
                                                   {0.8,0.031},
                                                   {1.0,0.022},
                                                   {1.2,0.018},
                                                   {1.4,0.019},
                                                   {1.6,0.020},
                                                   {1.8,0.018},
                                                   {2.2,0.014},
                                                   {2.6,0.012}
                                               }
                                           ),
                                           std::make_pair(3.0,
                                               interpVector{
                                                   {0.6,0.031},
                                                   {0.8,0.027},
                                                   {1.0,0.015},
                                                   {1.2,0.008},
                                                   {1.4,0.010},
                                                   {1.6,0.011},
                                                   {1.8,0.012},
                                                   {2.2,0.011},
                                                   {2.6,0.009},
                                                   {3.0,0.005}
                                               }
                                           )

                                       }
                     )



    };
    return _5_5_CxpStern;
}

interpVector& aerodynamics_tables::get_5_8_CpSternNu1(){
    static interpVector _5_8_CpSternNu1{
        {0.6,0.120},
        {0.8,0.125},
        {0.9,0.140},
        {1.0,0.190},
        {1.2,0.200},
        {1.4,0.190},
        {1.6,0.178},
        {1.8,0.160},
        {2.2,0.140},
        {2.6,0.120},
        {3.0,0.100},
        {3.6,0.080},
        {4.4,0.040}
    };
    return _5_8_CpSternNu1;
}

interp3DMat &aerodynamics_tables::get_5_9_knu(){
   static interp3DMat _5_9_knu{
                std::make_pair(1,
                               interpVector{
                                   {0.0,1.0},
                                   {0.1,0.6},
                                   {0.2,0.4}
                               }
                ),
                std::make_pair(1.5,
                               interpVector{
                                   {0.0,1.0},
                                   {0.2,0.5},
                                   {0.35,0.3}
                               }
                ),
                std::make_pair(2.0,
                               interpVector{
                                   {0.0,1.0},
                                   {0.2,0.6},
                                   {0.4,0.4}
                               }
                ),
                std::make_pair(3.0,
                               interpVector{
                                   {0.0,1.0},
                                   {0.1,0.8},
                                   {0.3,0.6},
                                   {0.6,0.52}
                               }
                )


    };
   return _5_9_knu;
}

interp4DMat &aerodynamics_tables::get_5_10_CxWavePlane(){
    static interp4DMat _5_10_CxWavePlane{
                std::make_pair(1,
                               interp3DMat {
                                   std::make_pair(1,
                                       interpVector{
                                           {0.0,1.57},
                                           {1.0,2.10},
                                           {2.0,1.60},
                                           {3.0,1.12},
                                           {4.0,0.80},
                                           {5.0,0.66},
                                           {6.0,0.58}
                                       }
                                   ),
                                   std::make_pair(2,
                                       interpVector{
                                           {0.0,0.89},
                                           {1.0,1.20},
                                           {2.0,1.50},
                                           {3.0,1.40},
                                           {4.0,1.00},
                                           {5.0,0.85},
                                           {6.0,0.65}
                                       }
                                   ),
                                   std::make_pair(3,
                                       interpVector{
                                           {0.0,0.45},
                                           {1.0,0.60},
                                           {2.0,0.90},
                                           {3.0,1.20},
                                           {4.0,1.10},
                                           {5.0,0.88},
                                           {6.0,0.70}
                                       }
                                   ),
                                   std::make_pair(4,
                                       interpVector{
                                           {0.0,0.21},
                                           {1.0,0.35},
                                           {2.0,0.50},
                                           {3.0,0.70},
                                           {4.0,0.92},
                                           {5.0,0.95},
                                           {6.0,0.80}
                                       }
                                   )
                               }
                ),

                std::make_pair(5,
                               interp3DMat {
                                   std::make_pair(1,//lambdak=2
                                   interpVector{
                                       {0.0,1.80},
                                       {1.0,0.98},
                                       {2.0,1.60},
                                       {3.0,1.20},
                                       {4.0,1.05},
                                       {5.0,0.70},
                                       {6.0,0.6}
                                   }
                                   ),
                                   std::make_pair(2,
                                   interpVector{
                                       {0.0,1.00},
                                       {1.0,1.35},
                                       {2.0,1.45},
                                       {3.0,1.15},
                                       {4.0,1.00},
                                       {5.0,0.80},
                                       {6.0,0.65}
                                   }
                                   ),
                                   std::make_pair(3.0,
                                   interpVector{
                                       {0.0,0.60},
                                       {1.0,0.80},
                                       {2.0,1.00},
                                       {3.0,1.08},
                                       {4.0,1.03},
                                       {5.0,0.86},
                                       {6.0,0.70}
                                   }
                                   ),
                                   std::make_pair(4.0,
                                   interpVector{
                                       {0.0,0.32},
                                       {1.0,0.40},
                                       {2.0,0.52},
                                       {3.0,0.70},
                                       {4.0,0.81},
                                       {5.0,0.86},
                                       {6.0,0.87},
                                       {7.0,0.60}
                                   }
                                   )

                               }
                               ),

                std::make_pair(10000000,
                               interp3DMat {
                                   std::make_pair(1,//lambdak=2
                                   interpVector{
                                       {0.0,1.95},
                                       {1.0,1.85},
                                       {2.0,1.60},
                                       {3.0,1.25},
                                       {4.0,0.90},
                                       {5.0,0.78},
                                       {6.0,0.62}
                                   }
                                   ),
                                   std::make_pair(2,
                                   interpVector{
                                       {0.0,1.31},
                                       {1.0,1.40},
                                       {2.0,1.34},
                                       {3.0,1.20},
                                       {4.0,1.00},
                                       {5.0,0.85},
                                       {6.0,0.69}
                                   }
                                   ),
                                   std::make_pair(3,
                                   interpVector{
                                       {0.0,0.78},
                                       {1.0,0.82},
                                       {2.0,1.00},
                                       {2.5,1.05},
                                       {3.0,1.00},
                                       {4.0,0.92},
                                       {5.0,0.86},
                                       {6.0,0.71}
                                   }
                                   ),
                                   std::make_pair(4,
                                   interpVector{
                                       {0.0,0.43},
                                       {1.0,0.50},
                                       {2.0,0.58},
                                       {3.0,0.67},
                                       {4.0,0.73},
                                       {5.0,0.80},
                                       {6.0,0.75}
                                   }
                                   ),

                               }
                  )
    };
    return _5_10_CxWavePlane;
}

interpVector& aerodynamics_tables::get_5_13_fi_CxWavePlane(){
    static interpVector _5_13_fi_CxWavePlane{
        {0,0},
        {0.5,0.2},
        {1.0,0.55},
        {1.5,0.8},
        {2,0.92},
        {2.5,1}
    };
    return _5_13_fi_CxWavePlane;
}

interp3DMat &aerodynamics_tables::get_7_2_CyaNoseCyll(){
    static interp3DMat _7_2_CyaConeCyl{
            std::make_pair(0,
                             interpVector{
                                          {0  ,0.035},
                                          {0.4,0.033},
                                          {0.8,0.033},
                                          {1.2,0.033},
                                          {1.4,0.033},
                                          {1.8,0.033}
                             }
            ), //первая пара, z=0
             std::make_pair(0.5,
                              interpVector{
                                           {0  ,0.039},
                                           {0.4,0.045},
                                           {0.8,0.046},
                                           {1.2,0.044},
                                           {1.4,0.042},
                                           {1.8,0.040}
                                     }
             ),    //2 пара, z=0.5
             std::make_pair(1,
                              interpVector{
                                           {0  ,0.039},
                                           {0.4,0.047},
                                           {0.8,0.052},
                                           {1.2,0.050},
                                           {1.4,0.047},
                                           {1.8,0.046}
                                            }
             ), //3 пара, z=1
                    std::make_pair(4,
                                     interpVector{
                                                  {0  ,0.039},
                                                  {0.4,0.047},
                                                  {0.8,0.055},
                                                  {1.2,0.058},
                                                  {1.4,0.060},
                                                  {1.8,0.061}
                                                   }
                    ) //3 пара, z=1

    };
return _7_2_CyaConeCyl;
}

interp4DMat &aerodynamics_tables::get_7_5_CyaPlane(){
  static interp4DMat _7_5_CyaPlane{
            std::make_pair(0,
                           interp3DMat {
                               std::make_pair(0.25,
                                   interpVector{
                                       {0.0,0.0320},
                                       {0.5,0.0335},
                                       {1.0,0.0332},
                                       {2.0,0.0270},
                                       {3.0,0.0210},
                                       {4.0,0.0160},
                                       {5.0,0.0130},
                                       {6.0,0.0110}
                                   }
                               ),
                               std::make_pair(0.5,
                                   interpVector{
                                       {0.0,0.0335},
                                       {1.0,0.0305},
                                       {2.0,0.0250},
                                       {3.0,0.0200},
                                       {4.0,0.0153},
                                       {5.0,0.0125},
                                       {6.0,0.0105}
                                   }
                               ),
                               std::make_pair(1.0,
                                   interpVector{
                                       {0.0,0.0280},
                                       {1.0,0.0260},
                                       {2.0,0.0223},
                                       {3.0,0.0187},
                                       {4.0,0.0145},
                                       {5.0,0.0122},
                                       {6.0,0.0103}
                                   }
                               ),
                               std::make_pair(1.5,
                                   interpVector{
                                       {0.0,0.0245},
                                       {1.0,0.0225},
                                       {2.0,0.0220},
                                       {3.0,0.0170},
                                       {4.0,0.140},
                                       {5.0,0.119},
                                       {6.0,0.100}
                                   }
                               )
                           }
            ),
                    std::make_pair(1,
                                   interp3DMat {
                                       std::make_pair(0.25,
                                           interpVector{
                                               {0.0,0.0300},
                                               {1.0,0.0310},
                                               {2.0,0.0260},
                                               {3.0,0.0200},
                                               {4.0,0.0155},
                                               {5.0,0.0128},
                                               {6.0,0.0110}
                                           }
                                       ),
                                       std::make_pair(0.5,
                                           interpVector{
                                               {0.0,0.0300},
                                               {1.0,0.0290},
                                               {2.0,0.0240},
                                               {3.0,0.0190},
                                               {4.0,0.0150},
                                               {5.0,0.0120},
                                               {6.0,0.0105}
                                           }
                                       ),
                                       std::make_pair(1.0,
                                           interpVector{
                                               {0.0,0.0270},
                                               {1.0,0.0250},
                                               {2.0,0.0213},
                                               {3.0,0.0180},
                                               {4.0,0.0150},
                                               {5.0,0.0120},
                                               {6.0,0.0103}
                                           }
                                       ),
                                       std::make_pair(1.5,
                                           interpVector{
                                               {0.0,0.0235},
                                               {1.0,0.0220},
                                               {2.0,0.0190},
                                               {3.0,0.0160},
                                               {4.0,0.0140},
                                               {5.0,0.0120},
                                               {6.0,0.0100}
                                           }
                                       )
                                   }
                    ),
                    std::make_pair(2,
                                   interp3DMat {
                                       std::make_pair(0.25,
                                           interpVector{
                                               {0.0,0.0250},
                                               {1.0,0.0287},
                                               {2.0,0.0240},
                                               {3.0,0.0192},
                                               {4.0,0.0160},
                                               {5.0,0.0130},
                                               {6.0,0.0110}
                                           }
                                       ),
                                       std::make_pair(0.5,
                                            interpVector{
                                               {0.0,0.0250},
                                               {1.0,0.0270},
                                               {2.0,0.0220},
                                               {3.0,0.0182},
                                               {4.0,0.0150},
                                               {5.0,0.0125},
                                               {6.0,0.0105}
                                             }
                                       ),
                                       std::make_pair(1.0,
                                           interpVector{
                                               {0.0,0.0245},
                                               {1.0,0.0230},
                                               {2.0,0.0200},
                                               {3.0,0.0170},
                                               {4.0,0.0140},
                                               {5.0,0.0120},
                                               {6.0,0.0103}
                                           }
                                       ),
                                       std::make_pair(1.5,
                                           interpVector{
                                               {0.0,0.0218},
                                               {1.0,0.0200},
                                               {2.0,0.0175},
                                               {3.0,0.0152},
                                               {4.0,0.0135},
                                               {5.0,0.0115},
                                               {6.0,0.0098}
                                           }
                                       )
                                   }
                    ),
                    std::make_pair(3,
                                   interp3DMat {
                                       std::make_pair(0.25,
                                           interpVector{
                                               {0.0,0.0240},
                                               {1.0,0.0250},
                                               {2.0,0.0220},
                                               {3.0,0.0185},
                                               {4.0,0.0156},
                                               {5.0,0.0132},
                                               {6.0,0.0112}
                                           }
                                       ),
                                       std::make_pair(0.5,
                                           interpVector{
                                               {0.0,0.0232},
                                               {0.5,0.0250},
                                               {1.0,0.0240},
                                               {2.0,0.0220},
                                               {3.0,0.0185},
                                               {4.0,0.0156},
                                               {5.0,0.0132},
                                               {6.0,0.0115}
                                           }
                                       ),
                                       std::make_pair(1.0,
                                           interpVector{
                                               {0.0,0.0230},
                                               {0.5,0.0240},
                                               {1.0,0.0235},
                                               {2.0,0.0200},
                                               {3.0,0.0172},
                                               {4.0,0.0150},
                                               {5.0,0.0122},
                                               {6.0,0.0112}
                                           }
                                       ),
                                       std::make_pair(1.5,
                                           interpVector{
                                               {0.0,0.0200},
                                               {1.0,0.0184},
                                               {2.0,0.0170},
                                               {3.0,0.0150},
                                               {4.0,0.0135},
                                               {5.0,0.0120},
                                               {6.0,0.0103}
                                           }
                                       )
                                   }
                    )
  };
  return _7_5_CyaPlane;
}

interp3DMat &aerodynamics_tables::get_11_2_deltaXf(){
    static interp3DMat _11_2_deltaXf{
        std::make_pair(0.5,
                       interpVector{
                           {0  ,0.1},
                           {0.8,0.13},
                           {2.0,0.11}
                       }
                       ), //первая пара, z=0
                std::make_pair(1,
                               interpVector{
                                   {0  ,0.1},
                                   {0.4,0.2},
                                   {0.8,0.24},
                                   {1.2,0.25},
                                   {1.4,0.255},
                                   {1.8,0.26}
                               }
                               ),    //2 пара, z=0.5
                std::make_pair(2,
                               interpVector{
                                   {0  ,0.1},
                                   {0.4,0.2},
                                   {0.8,0.33},
                                   {1.2,0.45},
                                   {1.6,0.5},
                                   {2.0,0.53}
                               }
                               ), //первая пара, z=0
                std::make_pair(3,
                               interpVector{
                                   {0  ,0.1},
                                   {0.4,0.2},
                                   {0.8,0.33},
                                   {1.2,0.047},
                                   {1.6,0.055},
                                   {2.0,0.063}
                               }
                               ),    //2 пара, z=0.5
                std::make_pair(4,
                               interpVector{
                                   {0  ,0.1},
                                   {0.4,0.2},
                                   {0.8,0.33},
                                   {1.2,0.047},
                                   {1.6,0.06},
                                   {2.0,0.067}
                               }
                               )   //2 пара, z=0.5
    };
    return _11_2_deltaXf;
}
