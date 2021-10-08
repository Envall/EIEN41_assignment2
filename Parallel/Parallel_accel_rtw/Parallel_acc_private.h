/*
 * Parallel_acc_private.h
 *
 * Real-Time Workshop code generation for Simulink model "Parallel_acc.mdl".
 *
 * Model Version              : 1.359
 * Real-Time Workshop version : 7.0  (R2007b)  02-Aug-2007
 * C source code generated on : Wed Feb 10 08:25:54 2010
 */
#ifndef RTW_HEADER_Parallel_acc_private_h_
#define RTW_HEADER_Parallel_acc_private_h_
#include "rtwtypes.h"
#ifndef RTW_COMMON_DEFINES_
# define RTW_COMMON_DEFINES_
#define rt_VALIDATE_MEMORY(S, ptr)     if(!(ptr)) {\
 ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
 return;\
 }
#if !defined(_WIN32)
#define rt_FREE(ptr)                   if((ptr) != NULL) {\
 free((ptr));\
 (ptr) = NULL;\
 }
#else

/* Visual and other windows compilers delcare free without const! */
#define rt_FREE(ptr)                   if((ptr) != NULL) {\
 free((void *)(ptr));\
 (ptr) = NULL;\
 }
#endif
#endif                                 /* RTW_COMMON_DEFINES_ */

/* Used by FromWorkspace Block: '<S10>/B_7_5' */
#ifndef rtInterpolate
# define rtInterpolate(v1,v2,f1,f2)    (((v1)==(v2))?((double)(v1)): (((f1)*((double)(v1)))+((f2)*((double)(v2)))))
#endif

#ifndef rtRound
# define rtRound(v)                    ( ((v) >= 0) ? muDoubleScalarFloor((v) + 0.5) : muDoubleScalarCeil((v) - 0.5) )
#endif

#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#else

/* Check for inclusion of an incorrect version of rtwtypes.h */
#ifndef RTWTYPES_ID_C08S16I32L32N32F1
#error This code was generated with a different "rtwtypes.h" than the file included
#endif                                 /* RTWTYPES_ID_C08S16I32L32N32F1 */
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

#ifndef __RTW_UTFREE__

extern void * utMalloc(size_t);
extern void utFree(void *);

#endif

boolean_T Parallel_acc_rt_TDelayUpdateTailOrGrowBuf(
  int_T *bufSzPtr,                     /* in/out - circular buffer size                 */
  int_T *tailPtr,                      /* in/out - tail of circular buffer              */
  int_T *headPtr,                      /* in/out - head of circular buffer              */
  int_T *lastPtr,                      /* in/out - same logical 'last' referenced index */
  real_T tMinusDelay,                  /* in     - last point we are looking at   */
  real_T **tBufPtr,                    /* in/out - larger buffer for time         */
  real_T **uBufPtr,                    /* in/out - larger buffer for input        */
  real_T **xBufPtr,                    /* in/out - larger buffer for state        */
  boolean_T isfixedbuf,                /* in     - fixed buffer size enable       */
  boolean_T istransportdelay,          /* in     - block acts as transport dela y */
  int_T *maxNewBufSzPtr);
real_T Parallel_acc_rt_TDelayInterpolate(
  real_T tMinusDelay,                  /* tMinusDelay = currentSimTime - delay */
  real_T tStart,
  real_T *tBuf,
  real_T *uBuf,
  int_T bufSz,
  int_T *lastIdx,
  int_T oldestIdx,
  int_T newIdx,
  real_T initOutput,
  boolean_T discrete,
  boolean_T minorStepAndTAtLastMajorOutput);
void Parallel_acc_BINARYSEARCH_real_T( uint32_T *piLeft, uint32_T *piRght,
  real_T u, const real_T *pData, uint32_T iHi);
void Parallel_acc_LookUp_real_T_real_T_SAT( real_T *pY, const real_T *pYData,
  real_T u, const real_T *pUData, uint32_T iHi);
uint32_T plook_binx(real_T u, const real_T *const bp, uint32_T maxIndex, real_T *
                    fraction);
uint32_T binsearch_u32d(real_T u, const real_T *const bp, uint32_T startIndex,
  uint32_T maxIndex);

#endif                                 /* RTW_HEADER_Parallel_acc_private_h_ */
