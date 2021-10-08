/*
 * Parallel_acc.h
 *
 * Real-Time Workshop code generation for Simulink model "Parallel_acc.mdl".
 *
 * Model Version              : 1.359
 * Real-Time Workshop version : 7.0  (R2007b)  02-Aug-2007
 * C source code generated on : Wed Feb 10 08:25:54 2010
 */
#ifndef RTW_HEADER_Parallel_acc_h_
#define RTW_HEADER_Parallel_acc_h_
#ifndef Parallel_acc_COMMON_INCLUDES_
# define Parallel_acc_COMMON_INCLUDES_
#include <math.h>
#include <stdlib.h>
#include <float.h>
#define S_FUNCTION_NAME                simulink_only_sfcn
#define S_FUNCTION_LEVEL               2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "mwmathutil.h"
#include "rt_zcfcn.h"
#include "rt_nonfinite.h"
#include "rtlibsrc.h"
#endif                                 /* Parallel_acc_COMMON_INCLUDES_ */

#include "Parallel_acc_types.h"

/* Block signals (auto storage) */
typedef struct {
  real_T B_7_3_0;                      /* '<S12>/Saturation' */
  real_T B_7_4_0;                      /* '<S9>/SOC+' */
  real_T B_7_9_0;                      /* '<S19>/Medelv' */
  real_T B_7_17_0;                     /* '<S9>/Saturation1' */
  real_T B_7_20_0;                     /* '<S6>/acc->speed' */
  real_T B_7_21_0;                     /* '<Root>/m//s -> km//h1' */
  real_T B_7_22_0[4];                  /* '<S10>/v-buss2' */
  real_T B_7_23_0;                     /* '<S10>/Variation' */
  real_T B_7_26_0;                     /* '<S10>/Flat' */
  real_T B_7_27_0;                     /* '<S33>/SwitchControl' */
  real_T B_7_28_0;                     /* '<S10>/Switch' */
  real_T B_7_34_0;                     /* '<S23>/Saturation4' */
  real_T B_7_37_0;                     /* '<S21>/ideal ratio' */
  real_T B_7_40_0;                     /* '<S25>/Sum1' */
  real_T B_7_41_0;                     /* '<S26>/Memory4' */
  real_T B_7_48_0;                     /* '<S25>/Direct Look-Up Table (n-D)' */
  real_T B_7_49_0;                     /* '<S25>/Constant3' */
  real_T B_7_52_0;                     /* '<S25>/Saturation1' */
  real_T B_7_53_0;                     /* '<S5>/0.1 seconds time constant' */
  real_T B_7_54_0;                     /* '<S15>/Constant' */
  real_T B_7_55_0;                     /* '<S21>/real ICE speed' */
  real_T B_7_65_0;                     /* '<S3>/Stäng integralverkan vid stillastående' */
  real_T B_7_69_0;                     /* '<S3>/Constant' */
  real_T B_7_77_0;                     /* '<S7>/T*elm' */
  real_T B_7_83_0;                     /* '<S24>/Switch' */
  real_T B_7_84_0;                     /* '<S24>/Switch2' */
  real_T B_7_89_0;                     /* '<S15>/maxT' */
  real_T B_7_92_0;                     /* '<S6>/Torque-Force2' */
  real_T B_7_94_0;                     /* '<Root>/m//s -> km//h2' */
  real_T B_7_96_0;                     /* '<S8>/Constant13' */
  real_T B_7_97_0;                     /* '<S8>/Constant16' */
  real_T B_7_98_0;                     /* '<S8>/Constant19' */
  real_T B_7_105_0;                    /* '<S8>/roll resistance' */
  real_T B_7_108_0;                    /* '<S8>/Bromsande kraft' */
  real_T B_7_110_0;                    /* '<Root>/m//s -> km//h3' */
  real_T B_7_115_0;                    /* '<S6>/Torque-Force1' */
  real_T B_7_117_0;                    /* '<Root>/m//s -> km//h4' */
  real_T B_7_122_0;                    /* '<S4>/Gain1' */
  real_T B_7_126_0;                    /* '<S4>/P// n' */
  real_T B_7_127_0;                    /* '<Root>/Constant' */
  real_T B_7_128_0;                    /* '<S1>/Ptrm' */
  real_T B_7_132_0;                    /* '<S11>/P// n' */
  real_T B_7_134_0[2];                 /* '<Root>/m//s -> km//h' */
  real_T B_7_137_0;                    /* '<S12>/Constant' */
  real_T B_7_142_0;                    /* '<S1>/Charge efficiency' */
  real_T B_7_145_0;                    /* '<S1>/Product' */
  real_T B_7_147_0;                    /* '<S1>/Charging power' */
  real_T B_7_149_0;                    /* '<S1>/Discharging power' */
  real_T B_7_150_0;                    /* '<S1>/Ptrm1' */
  real_T B_7_155_0;                    /* '<S3>/Product1' */
  real_T B_7_158_0;                    /* '<S14>/Medelv' */
  real_T B_7_159_0;                    /* '<S4>/Power' */
  real_T B_7_161_0;                    /* '<S4>/Clock' */
  real_T B_7_162_0;                    /* '<S4>/rad//s -> rpm' */
  real_T B_7_164_0;                    /* '<S14>/Relay' */
  real_T B_7_165_0;                    /* '<S14>/Medelv1' */
  real_T B_7_171_0;                    /* '<S23>/-> w1' */
  real_T B_7_172_0;                    /* '<S21>/new ICE Torque' */
  real_T B_7_176_0;                    /* '<S5>/No torque' */
  real_T B_7_178_0;                    /* '<S5>/rad//s -> rpm' */
  real_T B_7_179_0;                    /* '<S5>/Constant1' */
  real_T B_7_182_0;                    /* '<S5>/Do not adress tables with negative torque' */
  real_T B_7_184_0;                    /* '<S5>/gear eff' */
  real_T B_7_185_0;                    /* '<S20>/SwitchControl' */
  real_T B_7_186_0;                    /* '<S5>/Mechanical power' */
  real_T B_7_188_0;                    /* '<S5>/Clock' */
  real_T B_7_193_0;                    /* '<S5>/gear eff1' */
  real_T B_7_194_0;                    /* '<S5>/Fuel power' */
  real_T B_7_198_0;                    /* '<S19>/Relay' */
  real_T B_7_199_0;                    /* '<S19>/Medelv1' */
  real_T B_7_201_0;                    /* '<S5>/MinMax' */
  real_T B_7_202_0;                    /* '<S5>/No Fuel Cons @ ICE off' */
  real_T B_7_203_0;                    /* '<S6>/Constant3' */
  real_T B_7_204_0;                    /* '<S10>/Switch1' */
  real_T B_7_207_0;                    /* '<S6>/g' */
  real_T B_7_212_0;                    /* '<S6>/Twheel1' */
  real_T B_7_214_0;                    /* '<S25>/Sum2' */
  real_T B_7_216_0;                    /* '<S25>/gear ' */
  real_T B_7_225_0;                    /* '<S25>/Constant4' */
  real_T B_7_226_0;                    /* '<S26>/Pulse Generator' */
  real_T B_7_229_0;                    /* '<S26>/Sum4' */
  real_T B_7_232_0;                    /* '<S28>/Switch' */
  real_T B_7_236_0;                    /* '<S28>/Clock' */
  real_T B_7_240_0;                    /* '<S28>/Look-Up Table' */
  real_T B_7_242_0;                    /* '<S28>/Switch1' */
  real_T B_7_246_0;                    /* '<S22>/SOC+' */
  real_T B_7_248_0;                    /* '<S22>/10% avvikelse ger 10 kW motoreffekt' */
  real_T B_7_250_0;                    /* '<S22>/Abs' */
  real_T B_7_251_0;                    /* '<S22>/Product' */
  real_T B_7_260_0;                    /* '<S32>/Gain2' */
  real_T B_7_264_0;                    /* '<S9>/-> L//mil1' */
  real_T B_6_0_0;                      /* '<S30>/Zero-Order Hold' */
  real_T B_5_2_0;                      /* '<S27>/Sum' */
  real_T B_4_0_0;                      /* '<S29>/Zero-Order Hold' */
  real_T B_7_266_0;                    /* '<S10>/Gain2' */
  real_T B_7_267_0;                    /* '<S10>/Gain1' */
  real_T B_7_255_0;                    /* '<S31>/SwitchControl' */
  uint8_T B_7_24_0;                    /* '<S33>/Constant' */
  uint8_T B_7_25_0;                    /* '<S33>/S-Function' */
  uint8_T B_7_180_0;                   /* '<S20>/Constant' */
  uint8_T B_7_181_0;                   /* '<S20>/S-Function' */
  uint8_T B_7_190_0;                   /* '<S16>/Constant' */
  uint8_T B_7_191_0;                   /* '<S16>/S-Function' */
  uint8_T B_7_253_0;                   /* '<S31>/Constant' */
  uint8_T B_7_254_0;                   /* '<S31>/S-Function' */
} BlockIO;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Memory4_PreviousInput;        /* '<S26>/Memory4' */
  real_T Memory2_PreviousInput;        /* '<S26>/Memory2' */
  real_T Memory1_PreviousInput;        /* '<S25>/Memory1' */
  real_T Memory3_PreviousInput;        /* '<S28>/Memory3' */
  real_T Memory2_PreviousInput_m;      /* '<S28>/Memory2' */
  real_T Memory1_PreviousInput_o;      /* '<S28>/Memory1' */
  real_T Memory4_PreviousInput_d;      /* '<S28>/Memory4' */
  real_T Memory_PreviousInput;         /* '<S27>/Memory' */
  struct {
    void *LoggedData;
  } FuelConsumption1_PWORK;            /* '<Root>/Fuel Consumption1' */

  struct {
    void *TimePtr;
    void *DataPtr;
    void *RSimInfoPtr;
  } vbuss2_PWORK;                      /* '<S10>/v-buss2' */

  struct {
    void *TUbufferPtrs[2];
  } TransportDelay2_PWORK;             /* '<S28>/Transport Delay2' */

  struct {
    void *LoggedData;
  } Powers_PWORK;                      /* '<Root>/Powers' */

  struct {
    void *LoggedData;
  } SOC_PWORK;                         /* '<Root>/SOC' */

  struct {
    void *LoggedData;
  } vv_PWORK;                          /* '<Root>/v* & v' */

  struct {
    void *LoggedData;
  } Scope_PWORK;                       /* '<S1>/Scope' */

  struct {
    void *LoggedData;
  } ToWorkspace1_PWORK;                /* '<S1>/To Workspace1' */

  struct {
    void *LoggedData;
  } EM_PWORK;                          /* '<S4>/EM' */

  struct {
    void *LoggedData;
  } ToWorkspace_PWORK;                 /* '<S4>/To Workspace' */

  struct {
    void *LoggedData;
  } ICE_PWORK;                         /* '<S5>/ICE' */

  struct {
    void *LoggedData;
  } ToWorkspace_PWORK_i;               /* '<S5>/To Workspace' */

  struct {
    void *LoggedData;
  } Scope_PWORK_p;                     /* '<S19>/Scope' */

  struct {
    void *LoggedData;
  } Forces_PWORK;                      /* '<S6>/Forces' */

  struct {
    void *LoggedData;
  } Scope_PWORK_e;                     /* '<S6>/Scope' */

  struct {
    void *TUbufferPtrs[2];
  } TransportDelay1_PWORK;             /* '<S28>/Transport Delay1' */

  struct {
    void *LoggedData;
  } Vxellda_PWORK;                     /* '<S21>/Växellåda' */

  struct {
    void *LoggedData;
  } ToWorkspace_PWORK_f;               /* '<S25>/To Workspace' */

  struct {
    void *LoggedData;
  } Scope_PWORK_c;                     /* '<S22>/Scope' */

  struct {
    void *LoggedData;
  } ToWorkspace_PWORK_ia;              /* '<S9>/To Workspace' */

  struct {
    void *LoggedData;
  } Wsmil_PWORK;                       /* '<S9>/Ws//mil' */

  struct {
    void *LoggedData;
  } Cycle_PWORK;                       /* '<S10>/Cycle' */

  int32_T clockTickCounter;            /* '<S26>/Pulse Generator' */
  struct {
    int_T PrevIndex;
  } vbuss2_IWORK;                      /* '<S10>/v-buss2' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
    int_T MaxNewBufSize;
  } TransportDelay2_IWORK;             /* '<S28>/Transport Delay2' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
    int_T MaxNewBufSize;
  } TransportDelay1_IWORK;             /* '<S28>/Transport Delay1' */

  int_T PreLookUpIndexSearch_MODE;     /* '<S25>/PreLook-Up Index Search' */
  int8_T reftime_SubsysRanBC;          /* '<S26>/ref time' */
  int8_T Subsystem2_SubsysRanBC;       /* '<S25>/Subsystem2' */
  int8_T reftime_SubsysRanBC_e;        /* '<S28>/ref time' */
  boolean_T Relay_Mode;                /* '<S25>/Relay' */
  boolean_T u_Mode;                    /* '<S7>/20' */
  boolean_T Stngintegralverkanvidstillasten;/* '<S3>/Stäng integralverkan vid stillastående' */
  boolean_T Relay2_Mode;               /* '<S3>/Relay2' */
  boolean_T u_Mode_n;                  /* '<S7>/90' */
  boolean_T Relay1_Mode;               /* '<S3>/Relay1' */
  boolean_T Relay_Mode_n;              /* '<S14>/Relay' */
  boolean_T NOICETorquetolowefficiency_Mode;/* '<S23>/NO ICE Torque @  to low efficiency' */
  boolean_T ICEalwaysathighpower_Mode; /* '<S23>/ICE always at high power' */
  boolean_T Relay_Mode_d;              /* '<S19>/Relay' */
  boolean_T wyy_Mode;                  /* '<S26>/w>yy' */
  boolean_T wxx_Mode;                  /* '<S26>/w<xx' */
  char pad_wxx_Mode[5];
} D_Work;

/* Continuous states (auto storage) */
typedef struct {
  real_T FuelEnergy_CSTATE;            /* '<S5>/Fuel Energy' */
  real_T Wbattstartsat70_CSTATE;       /* '<S12>/W batt starts at 70%' */
  real_T Integral_CSTATE;              /* '<S19>/Integral' */
  real_T Integral1_CSTATE;             /* '<S19>/Integral1' */
  real_T vs_CSTATE;                    /* '<S9>/v->s' */
  real_T accspeed_CSTATE;              /* '<S6>/acc->speed' */
  real_T Wbatt_CSTATE;                 /* '<S32>/Wbatt' */
  real_T usecondstimeconstant_CSTATE;  /* '<S5>/0.1 seconds time constant' */
  real_T Wbatt_CSTATE_m;               /* '<S3>/Wbatt' */
  real_T Chargingenergy_CSTATE;        /* '<S1>/Charging energy' */
  real_T Dicharingenergy_CSTATE;       /* '<S1>/Dicharing energy' */
  real_T Dicharingenergy1_CSTATE;      /* '<S1>/Dicharing energy1' */
  real_T Integral_CSTATE_o;            /* '<S14>/Integral' */
  real_T Integral1_CSTATE_i;           /* '<S14>/Integral1' */
} ContinuousStates;

/* State derivatives (auto storage) */
typedef struct {
  real_T FuelEnergy_CSTATE;            /* '<S5>/Fuel Energy' */
  real_T Wbattstartsat70_CSTATE;       /* '<S12>/W batt starts at 70%' */
  real_T Integral_CSTATE;              /* '<S19>/Integral' */
  real_T Integral1_CSTATE;             /* '<S19>/Integral1' */
  real_T vs_CSTATE;                    /* '<S9>/v->s' */
  real_T accspeed_CSTATE;              /* '<S6>/acc->speed' */
  real_T Wbatt_CSTATE;                 /* '<S32>/Wbatt' */
  real_T usecondstimeconstant_CSTATE;  /* '<S5>/0.1 seconds time constant' */
  real_T Wbatt_CSTATE_m;               /* '<S3>/Wbatt' */
  real_T Chargingenergy_CSTATE;        /* '<S1>/Charging energy' */
  real_T Dicharingenergy_CSTATE;       /* '<S1>/Dicharing energy' */
  real_T Dicharingenergy1_CSTATE;      /* '<S1>/Dicharing energy1' */
  real_T Integral_CSTATE_o;            /* '<S14>/Integral' */
  real_T Integral1_CSTATE_i;           /* '<S14>/Integral1' */
} StateDerivatives;

/* State disabled  */
typedef struct {
  boolean_T FuelEnergy_CSTATE;         /* '<S5>/Fuel Energy' */
  boolean_T Wbattstartsat70_CSTATE;    /* '<S12>/W batt starts at 70%' */
  boolean_T Integral_CSTATE;           /* '<S19>/Integral' */
  boolean_T Integral1_CSTATE;          /* '<S19>/Integral1' */
  boolean_T vs_CSTATE;                 /* '<S9>/v->s' */
  boolean_T accspeed_CSTATE;           /* '<S6>/acc->speed' */
  boolean_T Wbatt_CSTATE;              /* '<S32>/Wbatt' */
  boolean_T usecondstimeconstant_CSTATE;/* '<S5>/0.1 seconds time constant' */
  boolean_T Wbatt_CSTATE_m;            /* '<S3>/Wbatt' */
  boolean_T Chargingenergy_CSTATE;     /* '<S1>/Charging energy' */
  boolean_T Dicharingenergy_CSTATE;    /* '<S1>/Dicharing energy' */
  boolean_T Dicharingenergy1_CSTATE;   /* '<S1>/Dicharing energy1' */
  boolean_T Integral_CSTATE_o;         /* '<S14>/Integral' */
  boolean_T Integral1_CSTATE_i;        /* '<S14>/Integral1' */
} StateDisabled;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState Wbatt_Reset_ZCE;          /* '<S3>/Wbatt' */
  ZCSigState reftime_Trig_ZCE;         /* '<S28>/ref time' */
  ZCSigState Subsystem2_Trig_ZCE;      /* '<S25>/Subsystem2' */
  ZCSigState reftime_Trig_ZCE_k;       /* '<S26>/ref time' */
} PrevZCSigStates;

/* Parameters (auto storage) */
struct Parameters {
  real_T P_0;                          /* Expression: 0
                                        * '<S5>/Fuel Energy'
                                        */
  real_T P_1;                          /* Expression: 0.7*Wbatt
                                        * '<S12>/W batt starts at 70%'
                                        */
  real_T P_2;                          /* Expression: 1.2*Wbatt
                                        * '<S12>/W batt starts at 70%'
                                        */
  real_T P_3;                          /* Expression: 0
                                        * '<S12>/W batt starts at 70%'
                                        */
  real_T P_4;                          /* Expression: 100/Wbatt
                                        * '<S12>/Gain3'
                                        */
  real_T P_5;                          /* Expression: 100
                                        * '<S12>/Saturation'
                                        */
  real_T P_6;                          /* Expression: 0
                                        * '<S12>/Saturation'
                                        */
  real_T P_7;                          /* Expression: SOC_batt_ref_value
                                        * '<S9>/SOC+'
                                        */
  real_T P_8;                          /* Expression: Wbatt/100/0.95
                                        * '<S9>/SOC deviation equivalent fuel energy'
                                        */
  real_T P_9;                          /* Expression: 0
                                        * '<S19>/Integral'
                                        */
  real_T P_10;                         /* Expression: eps
                                        * '<S19>/Integral1'
                                        */
  real_T P_11;                         /* Expression: inf
                                        * '<S19>/Integral1'
                                        */
  real_T P_12;                         /* Expression: eps
                                        * '<S19>/Integral1'
                                        */
  real_T P_13;                         /* Expression: inf
                                        * '<S9>/Saturation'
                                        */
  real_T P_14;                         /* Expression: eps
                                        * '<S9>/Saturation'
                                        */
  real_T P_15;                         /* Expression: 1/Density
                                        * '<S9>/Ws -> Liter bensin'
                                        */
  real_T P_16;                         /* Expression: 100
                                        * '<S9>/v->s'
                                        */
  real_T P_17;                         /* Expression: inf
                                        * '<S9>/v->s'
                                        */
  real_T P_18;                         /* Expression: 100
                                        * '<S9>/v->s'
                                        */
  real_T P_19;                         /* Expression: 1/10000
                                        * '<S9>/meter -> mil'
                                        */
  real_T P_20;                         /* Expression: 10
                                        * '<S9>/Saturation1'
                                        */
  real_T P_21;                         /* Expression: eps
                                        * '<S9>/Saturation1'
                                        */
  real_T P_22;                         /* Expression: 0
                                        * '<S6>/acc->speed'
                                        */
  real_T P_23;                         /* Expression: inf
                                        * '<S6>/acc->speed'
                                        */
  real_T P_24;                         /* Expression: 0
                                        * '<S6>/acc->speed'
                                        */
  real_T P_25;                         /* Expression: 3.6
                                        * '<Root>/m//s -> km//h1'
                                        */
  real_T P_26;                         /* Expression: 1
                                        * '<S10>/Variation'
                                        */
  real_T P_27;                         /* Expression: 0
                                        * '<S10>/Flat'
                                        */
  real_T P_28;                         /* Expression: 0.50
                                        * '<S10>/Switch'
                                        */
  real_T P_29;                         /* Expression: 0
                                        * '<S32>/Wbatt'
                                        */
  real_T P_30;                         /* Expression: Pice_max
                                        * '<S23>/Saturation2'
                                        */
  real_T P_31;                         /* Expression: eps
                                        * '<S23>/Saturation2'
                                        */
  real_T P_32[21];                     /* Expression: PtoT(:,1)
                                        * '<S23>/Topt_ice'
                                        */
  real_T P_33[21];                     /* Expression: PtoT(:,2)
                                        * '<S23>/Topt_ice'
                                        */
  real_T P_34;                         /* Expression: Tice_max
                                        * '<S23>/Saturation3'
                                        */
  real_T P_35;                         /* Expression: eps
                                        * '<S23>/Saturation3'
                                        */
  real_T P_36;                         /* Expression: wice_max
                                        * '<S23>/Saturation4'
                                        */
  real_T P_37;                         /* Expression: wice_min
                                        * '<S23>/Saturation4'
                                        */
  real_T P_38;                         /* Expression: 1/rw
                                        * '<S7>/Vehicle speed to Wheel speed'
                                        */
  real_T P_39;                         /* Expression: inf
                                        * '<S21>/do not divide by zero'
                                        */
  real_T P_40;                         /* Expression: eps
                                        * '<S21>/do not divide by zero'
                                        */
  real_T P_41[7];                      /* Expression: bpData
                                        * '<S25>/PreLook-Up Index Search'
                                        */
  real_T P_42;                         /* Expression: 0.52
                                        * '<S25>/Relay'
                                        */
  real_T P_43;                         /* Expression: 0.48
                                        * '<S25>/Relay'
                                        */
  real_T P_44;                         /* Expression: 1
                                        * '<S25>/Relay'
                                        */
  real_T P_45;                         /* Expression: 0
                                        * '<S25>/Relay'
                                        */
  real_T P_46;                         /* Expression: 0
                                        * '<S26>/Memory4'
                                        */
  real_T P_47;                         /* Expression: 0
                                        * '<S26>/Memory2'
                                        */
  real_T P_48;                         /* Expression: Number_of_gears-1
                                        * '<S25>/Saturation'
                                        */
  real_T P_49;                         /* Expression: 0
                                        * '<S25>/Saturation'
                                        */
  real_T P_50[7];                      /* Expression: mxTable
                                        * '<S25>/Direct Look-Up Table (n-D)'
                                        */
  real_T P_51;                         /* Expression: DCT
                                        * '<S25>/Constant3'
                                        */
  real_T P_52;                         /* Expression: 0.6
                                        * '<S28>/Transport Delay2'
                                        */
  real_T P_53;                         /* Expression: 0
                                        * '<S28>/Transport Delay2'
                                        */
  real_T P_54;                         /* Expression: 0.5
                                        * '<S25>/ratio '
                                        */
  real_T P_55;                         /* Expression: max(Utvx_vect)
                                        * '<S25>/Saturation1'
                                        */
  real_T P_56;                         /* Expression: min(Utvx_vect)
                                        * '<S25>/Saturation1'
                                        */
  real_T P_57;                         /* Computed Parameter: A
                                        * '<S5>/0.1 seconds time constant'
                                        */
  real_T P_58;                         /* Computed Parameter: B
                                        * '<S5>/0.1 seconds time constant'
                                        */
  real_T P_59;                         /* Computed Parameter: C
                                        * '<S5>/0.1 seconds time constant'
                                        */
  real_T P_62;                         /* Expression: eps
                                        * '<S15>/Constant'
                                        */
  real_T P_63;                         /* Expression: wem_max
                                        * '<S4>/Limit w'
                                        */
  real_T P_64;                         /* Expression: 0
                                        * '<S4>/Limit w'
                                        */
  real_T P_65[21];                     /* Expression: Wem
                                        * '<S7>/Tem_max1'
                                        */
  real_T P_66[21];                     /* Expression: Tem_lim
                                        * '<S7>/Tem_max1'
                                        */
  real_T P_67;                         /* Expression: 20+eps
                                        * '<S7>/20'
                                        */
  real_T P_68;                         /* Expression: 20-eps
                                        * '<S7>/20'
                                        */
  real_T P_69;                         /* Expression: 1
                                        * '<S7>/20'
                                        */
  real_T P_70;                         /* Expression: 0
                                        * '<S7>/20'
                                        */
  real_T P_71;                         /* Expression: Kv
                                        * '<S3>/Gain2'
                                        */
  real_T P_72;                         /* Expression: 0.3
                                        * '<S3>/Stäng integralverkan vid stillastående'
                                        */
  real_T P_73;                         /* Expression: 0.3-eps
                                        * '<S3>/Stäng integralverkan vid stillastående'
                                        */
  real_T P_74;                         /* Expression: 1
                                        * '<S3>/Stäng integralverkan vid stillastående'
                                        */
  real_T P_75;                         /* Expression: 0
                                        * '<S3>/Stäng integralverkan vid stillastående'
                                        */
  real_T P_76;                         /* Expression: 0
                                        * '<S3>/Wbatt'
                                        */
  real_T P_77;                         /* Expression: 1000
                                        * '<S3>/Wbatt'
                                        */
  real_T P_78;                         /* Expression: -10000
                                        * '<S3>/Wbatt'
                                        */
  real_T P_79;                         /* Expression: rw
                                        * '<S3>/F * -> T +'
                                        */
  real_T P_80;                         /* Expression: Pmax*rw
                                        * '<S3>/Constant'
                                        */
  real_T P_81;                         /* Expression: inf
                                        * '<S3>/avoid zero speed for max torque calculation'
                                        */
  real_T P_82;                         /* Expression: 5/3.6
                                        * '<S3>/avoid zero speed for max torque calculation'
                                        */
  real_T P_83;                         /* Expression: eps
                                        * '<S3>/Relay2'
                                        */
  real_T P_84;                         /* Expression: eps
                                        * '<S3>/Relay2'
                                        */
  real_T P_85;                         /* Expression: 10
                                        * '<S3>/Relay2'
                                        */
  real_T P_86;                         /* Expression: 1
                                        * '<S3>/Relay2'
                                        */
  real_T P_87;                         /* Expression: 0
                                        * '<S3>/Switch'
                                        */
  real_T P_88[21];                     /* Expression: Wem
                                        * '<S7>/Tem_min1'
                                        */
  real_T P_89[21];                     /* Expression: -Tem_lim
                                        * '<S7>/Tem_min1'
                                        */
  real_T P_90;                         /* Expression: 90+eps
                                        * '<S7>/90'
                                        */
  real_T P_91;                         /* Expression: 90-eps
                                        * '<S7>/90'
                                        */
  real_T P_92;                         /* Expression: 0
                                        * '<S7>/90'
                                        */
  real_T P_93;                         /* Expression: 1
                                        * '<S7>/90'
                                        */
  real_T P_94;                         /* Expression: Tem_max
                                        * '<S4>/Limit T'
                                        */
  real_T P_95;                         /* Expression: -Tem_max
                                        * '<S4>/Limit T'
                                        */
  real_T P_96;                         /* Expression: Pmax
                                        * '<S15>/maxP'
                                        */
  real_T P_97;                         /* Expression: -Pmax
                                        * '<S15>/maxP'
                                        */
  real_T P_98;                         /* Expression: Tmax
                                        * '<S15>/maxT'
                                        */
  real_T P_99;                         /* Expression: -Tmax
                                        * '<S15>/maxT'
                                        */
  real_T P_100;                        /* Expression: 1/rw
                                        * '<S6>/Torque-Force2'
                                        */
  real_T P_101;                        /* Expression: 1/1000
                                        * '<Root>/m//s -> km//h2'
                                        */
  real_T P_102;                        /* Expression: rho_air
                                        * '<S8>/Constant13'
                                        */
  real_T P_103;                        /* Expression: Cd
                                        * '<S8>/Constant16'
                                        */
  real_T P_104;                        /* Expression: Av
                                        * '<S8>/Constant19'
                                        */
  real_T P_105;                        /* Expression: 1/2
                                        * '<S8>/Bromsande kraft1'
                                        */
  real_T P_106;                        /* Expression: 1
                                        * '<S8>/Saturation2'
                                        */
  real_T P_107;                        /* Expression: -1
                                        * '<S8>/Saturation2'
                                        */
  real_T P_108;                        /* Expression: Cr
                                        * '<S8>/Constant18'
                                        */
  real_T P_109;                        /* Expression: Mv
                                        * '<S8>/Constant15'
                                        */
  real_T P_110;                        /* Expression: grav
                                        * '<S8>/Constant14'
                                        */
  real_T P_111;                        /* Expression: -1
                                        * '<S8>/Bromsande kraft'
                                        */
  real_T P_112;                        /* Expression: 1/1000
                                        * '<Root>/m//s -> km//h3'
                                        */
  real_T P_113;                        /* Expression: 0
                                        * '<S2>/Endast negativa bromsmoment1'
                                        */
  real_T P_114;                        /* Expression: -inf
                                        * '<S2>/Endast negativa bromsmoment1'
                                        */
  real_T P_115;                        /* Expression: 1/rw
                                        * '<S6>/Torque-Force1'
                                        */
  real_T P_116;                        /* Expression: 1/1000
                                        * '<Root>/m//s -> km//h4'
                                        */
  real_T P_117[26];                    /* Expression: x
                                        * '<S13>/Look-Up Table (2-D)'
                                        */
  real_T P_118[21];                    /* Expression: y
                                        * '<S13>/Look-Up Table (2-D)'
                                        */
  real_T P_119[546];                   /* Expression: t
                                        * '<S13>/Look-Up Table (2-D)'
                                        */
  real_T P_120;                        /* Expression: EtaGEAR
                                        * '<S4>/Gain'
                                        */
  real_T P_121;                        /* Expression: EtaPE
                                        * '<S4>/Gain1'
                                        */
  real_T P_122;                        /* Expression: 0
                                        * '<S4>/0<T<0 ?'
                                        */
  real_T P_123;                        /* Expression: Paux
                                        * '<Root>/Constant'
                                        */
  real_T P_124;                        /* Expression: -1
                                        * '<S1>/Motoring discharges the battery'
                                        */
  real_T P_125;                        /* Expression: max(Pbatt)
                                        * '<S11>/Saturation'
                                        */
  real_T P_126;                        /* Expression: min(Pbatt)
                                        * '<S11>/Saturation'
                                        */
  real_T P_127[22];                    /* Expression: Pbatt
                                        * '<S11>/Look-Up Table'
                                        */
  real_T P_128[22];                    /* Expression: EtaBATT
                                        * '<S11>/Look-Up Table'
                                        */
  real_T P_129;                        /* Expression: 3.6
                                        * '<Root>/m//s -> km//h'
                                        */
  real_T P_130;                        /* Expression: 1
                                        * '<S1>/Charging energy'
                                        */
  real_T P_131;                        /* Expression: inf
                                        * '<S1>/Charging energy'
                                        */
  real_T P_132;                        /* Expression: 1
                                        * '<S1>/Charging energy'
                                        */
  real_T P_133;                        /* Expression: 0.7*Wbatt
                                        * '<S12>/Constant'
                                        */
  real_T P_134;                        /* Expression: -1
                                        * '<S1>/Dicharing energy'
                                        */
  real_T P_135;                        /* Expression: -1
                                        * '<S1>/Dicharing energy'
                                        */
  real_T P_136;                        /* Expression: -inf
                                        * '<S1>/Dicharing energy'
                                        */
  real_T P_137;                        /* Expression: -1
                                        * '<S1>/ '
                                        */
  real_T P_138;                        /* Expression: inf
                                        * '<S1>/Charging power'
                                        */
  real_T P_139;                        /* Expression: 0
                                        * '<S1>/Charging power'
                                        */
  real_T P_140;                        /* Expression: 0
                                        * '<S1>/Dicharing energy1'
                                        */
  real_T P_141;                        /* Expression: 0
                                        * '<S1>/Discharging power'
                                        */
  real_T P_142;                        /* Expression: -inf
                                        * '<S1>/Discharging power'
                                        */
  real_T P_143;                        /* Expression: 1/Ti
                                        * '<S3>/Gain1'
                                        */
  real_T P_144;                        /* Expression: eps
                                        * '<S3>/Relay1'
                                        */
  real_T P_145;                        /* Expression: eps
                                        * '<S3>/Relay1'
                                        */
  real_T P_146;                        /* Expression: 0
                                        * '<S3>/Relay1'
                                        */
  real_T P_147;                        /* Expression: 1
                                        * '<S3>/Relay1'
                                        */
  real_T P_148;                        /* Expression: 0
                                        * '<S14>/Integral'
                                        */
  real_T P_149;                        /* Expression: eps
                                        * '<S14>/Integral1'
                                        */
  real_T P_150;                        /* Expression: inf
                                        * '<S14>/Integral1'
                                        */
  real_T P_151;                        /* Expression: eps
                                        * '<S14>/Integral1'
                                        */
  real_T P_152;                        /* Expression: 30/pi
                                        * '<S4>/rad//s -> rpm'
                                        */
  real_T P_153;                        /* Expression: 0.05
                                        * '<S14>/Relay'
                                        */
  real_T P_154;                        /* Expression: 0.04
                                        * '<S14>/Relay'
                                        */
  real_T P_155;                        /* Expression: 1
                                        * '<S14>/Relay'
                                        */
  real_T P_156;                        /* Expression: 0
                                        * '<S14>/Relay'
                                        */
  real_T P_157[21];                    /* Expression: PtoT(:,1)
                                        * '<S23>/eta_ice'
                                        */
  real_T P_158[21];                    /* Expression: PtoT(:,3)
                                        * '<S23>/eta_ice'
                                        */
  real_T P_159;                        /* Expression: 0.31
                                        * '<S23>/NO ICE Torque @  to low efficiency'
                                        */
  real_T P_160;                        /* Expression: 0.28
                                        * '<S23>/NO ICE Torque @  to low efficiency'
                                        */
  real_T P_161;                        /* Expression: 1
                                        * '<S23>/NO ICE Torque @  to low efficiency'
                                        */
  real_T P_162;                        /* Expression: 0
                                        * '<S23>/NO ICE Torque @  to low efficiency'
                                        */
  real_T P_163;                        /* Expression: Pice_max/2*1.1
                                        * '<S23>/ICE always at high power'
                                        */
  real_T P_164;                        /* Expression: Pice_max/2*0.9
                                        * '<S23>/ICE always at high power'
                                        */
  real_T P_165;                        /* Expression: 1
                                        * '<S23>/ICE always at high power'
                                        */
  real_T P_166;                        /* Expression: 0
                                        * '<S23>/ICE always at high power'
                                        */
  real_T P_167;                        /* Expression: Tice_max
                                        * '<S7>/Saturation'
                                        */
  real_T P_168;                        /* Expression: 0
                                        * '<S7>/Saturation'
                                        */
  real_T P_169;                        /* Expression: 30/pi
                                        * '<S5>/rad//s -> rpm'
                                        */
  real_T P_170;                        /* Expression: max(max(EtaICE))
                                        * '<S5>/Constant1'
                                        */
  real_T P_171;                        /* Expression: inf
                                        * '<S5>/Do not adress tables with negative torque'
                                        */
  real_T P_172;                        /* Expression: 0
                                        * '<S5>/Do not adress tables with negative torque'
                                        */
  real_T P_173[21];                    /* Expression: x
                                        * '<S18>/Look-Up Table (2-D)'
                                        */
  real_T P_174[21];                    /* Expression: y
                                        * '<S18>/Look-Up Table (2-D)'
                                        */
  real_T P_175[441];                   /* Expression: t
                                        * '<S18>/Look-Up Table (2-D)'
                                        */
  real_T P_176;                        /* Expression: EtaGEAR
                                        * '<S5>/gear eff'
                                        */
  real_T P_177[21];                    /* Expression: x
                                        * '<S17>/Look-Up Table (2-D)'
                                        */
  real_T P_178[21];                    /* Expression: y
                                        * '<S17>/Look-Up Table (2-D)'
                                        */
  real_T P_179[441];                   /* Expression: t
                                        * '<S17>/Look-Up Table (2-D)'
                                        */
  real_T P_180;                        /* Expression: 1/EtaGEAR
                                        * '<S5>/gear eff1'
                                        */
  real_T P_181;                        /* Expression: wice_max
                                        * '<S5>/ICE'ns real speed1'
                                        */
  real_T P_182;                        /* Expression: wice_min
                                        * '<S5>/ICE'ns real speed1'
                                        */
  real_T P_183;                        /* Expression: 0.05
                                        * '<S19>/Relay'
                                        */
  real_T P_184;                        /* Expression: 0.04
                                        * '<S19>/Relay'
                                        */
  real_T P_185;                        /* Expression: 1
                                        * '<S19>/Relay'
                                        */
  real_T P_186;                        /* Expression: 0
                                        * '<S19>/Relay'
                                        */
  real_T P_187[21];                    /* Expression: Wice
                                        * '<S5>/Torque limitation'
                                        */
  real_T P_188[21];                    /* Expression: Tlim_ice
                                        * '<S5>/Torque limitation'
                                        */
  real_T P_189;                        /* Expression: Mv
                                        * '<S6>/Constant3'
                                        */
  real_T P_190;                        /* Expression: 0.5
                                        * '<S10>/Switch1'
                                        */
  real_T P_191;                        /* Expression: -9.81
                                        * '<S6>/g'
                                        */
  real_T P_192;                        /* Expression: Number_of_gears
                                        * '<S25>/Constant2'
                                        */
  real_T P_193;                        /* Expression: 0.6
                                        * '<S28>/Transport Delay1'
                                        */
  real_T P_194;                        /* Expression: 0
                                        * '<S28>/Transport Delay1'
                                        */
  real_T P_195;                        /* Expression: 0.5
                                        * '<S25>/gear '
                                        */
  real_T P_196;                        /* Expression: 0
                                        * '<S25>/Memory1'
                                        */
  real_T P_197;                        /* Expression: 1
                                        * '<S25>/Constant4'
                                        */
  real_T P_198;                        /* Expression: 1
                                        * '<S26>/Pulse Generator'
                                        */
  real_T P_199;                        /* Expression: 16
                                        * '<S26>/Pulse Generator'
                                        */
  real_T P_200;                        /* Expression: 8
                                        * '<S26>/Pulse Generator'
                                        */
  real_T P_201;                        /* Expression: wice_max*0.8
                                        * '<S26>/w>yy'
                                        */
  real_T P_202;                        /* Expression: wice_max*0.7
                                        * '<S26>/w>yy'
                                        */
  real_T P_203;                        /* Expression: 1
                                        * '<S26>/w>yy'
                                        */
  real_T P_204;                        /* Expression: 0
                                        * '<S26>/w>yy'
                                        */
  real_T P_205;                        /* Expression: wice_min*1.2
                                        * '<S26>/w<xx'
                                        */
  real_T P_206;                        /* Expression: wice_min*1.1
                                        * '<S26>/w<xx'
                                        */
  real_T P_207;                        /* Expression: 0
                                        * '<S26>/w<xx'
                                        */
  real_T P_208;                        /* Expression: 1
                                        * '<S26>/w<xx'
                                        */
  real_T P_209;                        /* Expression: 0
                                        * '<S28>/Memory3'
                                        */
  real_T P_210;                        /* Expression: 0
                                        * '<S28>/Memory2'
                                        */
  real_T P_211;                        /* Expression: 0.5
                                        * '<S28>/Switch'
                                        */
  real_T P_212;                        /* Expression: 0
                                        * '<S28>/Memory1'
                                        */
  real_T P_213[4];                     /* Expression: [0 0.2 0.8 1.0]
                                        * '<S28>/Look-Up Table'
                                        */
  real_T P_214[4];                     /* Expression: [1 0 0 1]
                                        * '<S28>/Look-Up Table'
                                        */
  real_T P_215;                        /* Expression: 0
                                        * '<S28>/Memory4'
                                        */
  real_T P_216;                        /* Expression: 0.5
                                        * '<S28>/Switch1'
                                        */
  real_T P_217;                        /* Expression: 0.5
                                        * '<S25>/Tq_int'
                                        */
  real_T P_218;                        /* Expression: 0.5
                                        * '<S25>/on//off '
                                        */
  real_T P_219;                        /* Expression: SOC_batt_ref_value
                                        * '<S22>/SOC+'
                                        */
  real_T P_220;                        /* Expression: ksoc
                                        * '<S22>/10% avvikelse ger 10 kW motoreffekt'
                                        */
  real_T P_221[31];                    /* Expression: [-15:1:15]
                                        * '<S22>/Look-Up Table'
                                        */
  real_T P_222[31];                    /* Expression: ksoc_vect
                                        * '<S22>/Look-Up Table'
                                        */
  real_T P_223;                        /* Expression: inf
                                        * '<S23>/Saturation1'
                                        */
  real_T P_224;                        /* Expression: eps
                                        * '<S23>/Saturation1'
                                        */
  real_T P_225;                        /* Expression: 1/(Tau_charge)
                                        * '<S32>/Gain2'
                                        */
  real_T P_226;                        /* Expression: 3.6
                                        * '<S10>/Gain2'
                                        */
  real_T P_227;                        /* Expression: 1/1000
                                        * '<S10>/Gain1'
                                        */
  real_T P_228;                        /* Expression: -1
                                        * '<S3>/Gain3'
                                        */
  real_T P_229[6];                     /* Expression: [0 0.2 0.2+eps 0.8-eps 0.8 1.0]
                                        * '<S28>/Look-Up Table1'
                                        */
  real_T P_230[6];                     /* Expression: [1 1 0 0 1 1]
                                        * '<S28>/Look-Up Table1'
                                        */
  real_T P_231;                        /* Expression: 1
                                        * '<S27>/Constant'
                                        */
  real_T P_232;                        /* Expression: 0
                                        * '<S27>/Memory'
                                        */
  uint8_T P_233;                       /* Expression: uint8(1)
                                        * '<S33>/Constant'
                                        */
  uint8_T P_234;                       /* Expression: uint8(0)
                                        * '<S33>/SwitchControl'
                                        */
  uint8_T P_235;                       /* Expression: uint8(1)
                                        * '<S20>/Constant'
                                        */
  uint8_T P_236;                       /* Expression: uint8(1)
                                        * '<S20>/SwitchControl'
                                        */
  uint8_T P_237;                       /* Expression: uint8(1)
                                        * '<S16>/Constant'
                                        */
  uint8_T P_238;                       /* Expression: uint8(0)
                                        * '<S16>/SwitchControl'
                                        */
  uint8_T P_239;                       /* Expression: uint8(1)
                                        * '<S31>/Constant'
                                        */
  uint8_T P_240;                       /* Expression: uint8(1)
                                        * '<S31>/SwitchControl'
                                        */
};

extern Parameters rtDefaultParameters; /* parameters */

#endif                                 /* RTW_HEADER_Parallel_acc_h_ */
