/*
 * This file is not available for use in any application other than as a
 * MATLAB(R) MEX file for use with the Simulink(R) Accelerator product.
 */

/*
 * Parallel_acc.c
 *
 * Real-Time Workshop code generation for Simulink model "Parallel_acc.mdl".
 *
 * Model Version              : 1.359
 * Real-Time Workshop version : 7.0  (R2007b)  02-Aug-2007
 * C source code generated on : Wed Feb 10 08:25:54 2010
 */
#include <math.h>
#include "Parallel_acc.h"
#include "Parallel_acc_private.h"
#include <stdio.h>
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat                     S-Function
#define AccDefine1                     Accelerator_S-Function
#ifndef __RTW_UTFREE__

extern void * utMalloc(size_t);
extern void utFree(void *);

#endif

/*********************************************************************
 * Function Parallel_acc_rt_TDelayUpdateTailOrGrowBuf
 *
 * Abstract:
 * Buffer management routine for variable delay block
 *
 */
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
  int_T *maxNewBufSzPtr)
{
  int_T testIdx;
  int_T tail = *tailPtr;
  int_T bufSz = *bufSzPtr;
  real_T *tBuf = *tBufPtr;
  real_T *xBuf = NULL;
  int_T numBuffer = 2;
  if (istransportdelay) {
    numBuffer = 3 ;
    xBuf= *xBufPtr;
  }

  /*    Get testIdx, the index of the second oldest data point and
   *    see if this is older than current sim time minus applied delay,
   *    used to see if we can move tail forward
   */
  testIdx = (tail < (bufSz - 1)) ? (tail + 1) : 0;
  if ((tMinusDelay <= tBuf[testIdx]) && !isfixedbuf) {
    int_T j;
    real_T *tempT;
    real_T *tempU;
    real_T *tempX = NULL;
    real_T *uBuf = *uBufPtr;
    int_T newBufSz = bufSz + 1024;
    if (newBufSz > *maxNewBufSzPtr) {
      *maxNewBufSzPtr = newBufSz;      /* save for warning*/
    }

    tempU = (real_T*)utMalloc(numBuffer*newBufSz*sizeof(real_T));
    if (tempU == NULL) {
      return (false);
    }

    tempT = tempU + newBufSz;
    if (istransportdelay)
      tempX = tempT + newBufSz;
    for (j = tail; j < bufSz; j++) {
      tempT[j - tail] = tBuf[j];
      tempU[j - tail] = uBuf[j];
      if (istransportdelay)
        tempX[j - tail] = xBuf[j];
    }

    for (j = 0; j < tail; j++) {
      tempT[j + bufSz - tail] = tBuf[j];
      tempU[j + bufSz - tail] = uBuf[j];
      if (istransportdelay)
        tempX[j + bufSz - tail] = xBuf[j];
    }

    if (*lastPtr> tail) {
      *lastPtr -= tail;
    } else {
      *lastPtr += (bufSz - tail);
    }

    *tailPtr= 0;
    *headPtr = bufSz;
    utFree(uBuf);
    *bufSzPtr = newBufSz;
    *tBufPtr = tempT;
    *uBufPtr = tempU;
    if (istransportdelay)
      *xBufPtr = tempX;
  } else {
    *tailPtr = testIdx;                /* move tail forward */
  }

  return(true);
}

/* end function Parallel_acc_rt_TDelayUpdateTailOrGrowBuf
 *********************************************************************/

/*********************************************************************
 * Function Parallel_acc_rt_TDelayInterpolate
 *
 * Abstract:
 * Time delay interpolation routine
 *
 * The linear interpolation is performed using the formula:
 *
 *          (t2 - tMinusDelay)         (tMinusDelay - t1)
 * u(t)  =  ----------------- * u1  +  ------------------- * u2
 *              (t2 - t1)                  (t2 - t1)
 */
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
  boolean_T minorStepAndTAtLastMajorOutput)
{
  int_T i;
  real_T yout, t1, t2, u1, u2;

  /*
   * If tMinusDelay is less than zero, should output initial value
   */
  if (tMinusDelay <= tStart)
    return initOutput;

  /* For fixed buffer extrapolation:
   * if tMinusDelay is small than the time at oldestIdx, if discrete, output
   * tailptr value,  else use tailptr and tailptr+1 value to extrapolate
   * It is also for fixed buffer. Note: The same condition can happen for transport delay block where
   * use tStart and and t[tail] other than using t[tail] and t[tail+1].
   * See below
   */
  if ((tMinusDelay <= tBuf[oldestIdx] ) ) {
    if (discrete) {
      return(uBuf[oldestIdx]);
    } else {
      int_T tempIdx= oldestIdx + 1;
      if (oldestIdx == bufSz-1)
        tempIdx = 0;
      t1= tBuf[oldestIdx];
      t2= tBuf[tempIdx];
      u1= uBuf[oldestIdx];
      u2= uBuf[tempIdx];
      if (t2 == t1) {
        if (tMinusDelay >= t2) {
          yout = u2;
        } else {
          yout = u1;
        }
      } else {
        real_T f1 = (t2-tMinusDelay) / (t2-t1);
        real_T f2 = 1.0 - f1;

        /*
         * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
         */
        yout = f1*u1 + f2*u2;
      }

      return yout;
    }
  }

  /*
   * When block does not have direct feedthrough, we use the table of
   * values to extrapolate off the end of the table for delays that are less
   * than 0 (less then step size).  This is not completely accurate.  The
   * chain of events is as follows for a given time t.  Major output - look
   * in table.  Update - add entry to table.  Now, if we call the output at
   * time t again, there is a new entry in the table. For very small delays,
   * this means that we will have a different answer from the previous call
   * to the output fcn at the same time t.  The following code prevents this
   * from happening.
   */
  if (minorStepAndTAtLastMajorOutput) {
    /* pretend that the new entry has not been added to table */
    if (newIdx != 0) {
      if (*lastIdx == newIdx) {
        (*lastIdx)--;
      }

      newIdx--;
    } else {
      if (*lastIdx == newIdx) {
        *lastIdx = bufSz-1;
      }

      newIdx = bufSz - 1;
    }
  }

  i = *lastIdx;
  if (tBuf[i] < tMinusDelay) {
    /* Look forward starting at last index */
    while (tBuf[i] < tMinusDelay) {
      /* May occur if the delay is less than step-size - extrapolate */
      if (i == newIdx)
        break;
      i = ( i < (bufSz-1) ) ? (i+1) : 0;/* move through buffer */
    }
  } else {
    /*
     * Look backwards starting at last index which can happen when the
     * delay time increases.
     */
    while (tBuf[i] >= tMinusDelay) {
      /*
       * Due to the entry condition at top of function, we
       * should never hit the end.
       */
      i = (i > 0) ? i-1 : (bufSz-1);   /* move through buffer */
    }

    i = ( i < (bufSz-1) ) ? (i+1) : 0;
  }

  *lastIdx = i;
  if (discrete) {
    /*
     * tempEps = 128 * eps;
     * localEps = max(tempEps, tempEps*fabs(tBuf[i]))/2;
     */
    double tempEps = DBL_EPSILON * 128.0;
    double localEps = tempEps * fabs(tBuf[i]);
    if (tempEps > localEps) {
      localEps = tempEps;
    }

    localEps = localEps / 2.0;
    if (tMinusDelay >= (tBuf[i] - localEps)) {
      yout = uBuf[i];
    } else {
      if (i == 0) {
        yout = uBuf[bufSz-1];
      } else {
        yout = uBuf[i-1];
      }
    }
  } else {
    if (i == 0) {
      t1 = tBuf[bufSz-1];
      u1 = uBuf[bufSz-1];
    } else {
      t1 = tBuf[i-1];
      u1 = uBuf[i-1];
    }

    t2 = tBuf[i];
    u2 = uBuf[i];
    if (t2 == t1) {
      if (tMinusDelay >= t2) {
        yout = u2;
      } else {
        yout = u1;
      }
    } else {
      real_T f1 = (t2-tMinusDelay) / (t2-t1);
      real_T f2 = 1.0 - f1;

      /*
       * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
       */
      yout = f1*u1 + f2*u2;
    }
  }

  return(yout);
}

/* end function Parallel_acc_rt_TDelayInterpolate
 *********************************************************************/

/*********************************************************************
 * Lookup Binary Search Utility Parallel_acc_BINARYSEARCH_real_T
 */
void Parallel_acc_BINARYSEARCH_real_T( uint32_T *piLeft, uint32_T *piRght,
  real_T u, const real_T *pData, uint32_T iHi)
{
  /* Find the location of current input value in the data table. */
  *piLeft = 0;
  *piRght = iHi;
  if (u <= pData[0] ) {
    /* Less than or equal to the smallest point in the table. */
    *piRght = 0;
  } else if (u >= pData[iHi] ) {
    /* Greater than or equal to the largest point in the table. */
    *piLeft = iHi;
  } else {
    uint32_T i;

    /* Do a binary search. */
    while (( *piRght - *piLeft ) > 1 ) {
      /* Get the average of the left and right indices using to Floor rounding. */
      i = (*piLeft + *piRght) >> 1;

      /* Move either the right index or the left index so that */
      /*  LeftDataPoint <= CurrentValue < RightDataPoint */
      if (u < pData[i] ) {
        *piRght = i;
      } else {
        *piLeft = i;
      }
    }
  }
}

/* end macro Parallel_acc_BINARYSEARCH_real_T
 *********************************************************************/

/*********************************************************************
 * Lookup Utility Parallel_acc_LookUp_real_T_real_T_SAT
 */
void Parallel_acc_LookUp_real_T_real_T_SAT( real_T *pY, const real_T *pYData,
  real_T u, const real_T *pUData, uint32_T iHi)
{
  uint32_T iLeft;
  uint32_T iRght;
  Parallel_acc_BINARYSEARCH_real_T( &(iLeft), &(iRght), u, pUData, iHi);

  {
    real_T lambda;
    if (pUData[iRght] > pUData[iLeft] ) {
      real_T num;
      real_T den;
      den = pUData[iRght];
      den = den - pUData[iLeft];
      num = u;
      num = num - pUData[iLeft];
      lambda = num / den;
    } else {
      lambda = 0;
    }

    {
      real_T yLeftCast;
      real_T yRghtCast;
      yLeftCast = pYData[iLeft];
      yRghtCast = pYData[iRght];
      yLeftCast += lambda * ( yRghtCast - yLeftCast );
      (*pY) = yLeftCast;
    }
  }
}

/* end function Parallel_acc_LookUp_real_T_real_T_SAT
 *********************************************************************/
uint32_T plook_binx(real_T u, const real_T *const bp, uint32_T maxIndex, real_T *
                    fraction)
{
  uint32_T bpIndex;
  real_T bpLeftCast;
  real_T bpRghtCast;

  /* Prelookup Index and Fraction
     Search method: 'binary'
     Process out of range input: 'Linear extrapolation'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
   */
  if (u < bp[0U]) {
    bpIndex = 0U;
    *fraction = (u - bp[0U]) / (bp[1U] - bp[0U]);
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d(u, bp, (maxIndex + 1U) >> 1U, maxIndex);
    bpLeftCast = bp[bpIndex];
    bpRghtCast = bp[bpIndex + 1U];
    *fraction = (u - bpLeftCast) / (bpRghtCast - bpLeftCast);
  } else {
    bpIndex = maxIndex - 1U;
    *fraction = (u - bp[maxIndex - 1U]) / (bp[maxIndex] - bp[maxIndex - 1U]);
  }

  return bpIndex;
}

uint32_T binsearch_u32d(real_T u, const real_T *const bp, uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIndex;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  /* Binary Search */
  bpIdx = startIndex;
  iLeft = 0U;
  iRght = maxIndex;
  while (iRght - iLeft > 1U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      iLeft = bpIdx;
    }

    bpIdx = (iRght + iLeft) >> 1U;
  }

  bpIndex = iLeft;
  return bpIndex;
}

/* Outputs for root system: '<Root>' */
static void mdlOutputs(SimStruct *S, int_T tid)
{
  /* local block i/o variables */
  real_T B_7_13_0;
  real_T B_7_68_0;
  real_T B_7_239_0;
  real_T B_3_0_0;
  real_T B_7_38_0[2];
  real_T B_7_138_0;
  real_T B_7_6_0;
  real_T B_7_29_0;
  real_T B_7_15_0;
  real_T B_7_35_0;
  real_T B_7_228_0;
  real_T B_7_139_0;
  real_T B_7_56_0;
  real_T B_7_47_0;
  real_T B_7_148_0;
  real_T B_7_257_0;
  real_T B_7_170_0;
  real_T B_7_241_0;
  real_T B_7_230_0;
  real_T B_7_247_0;
  boolean_T B_7_78_0;

  {
    uint32_T scalarIndex;
    real_T scalarFraction;
    B_7_170_0 = ((ContinuousStates *) ssGetContStates(S))->FuelEnergy_CSTATE;

    /* Integrator: '<S12>/W batt starts at 70%' */
    if (((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE >=
        ((Parameters *) ssGetDefaultParam(S))->P_2 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_2) {
        ((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_2;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE
               <= ((Parameters *) ssGetDefaultParam(S))->P_3 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_3) {
        ((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_3;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_138_0 = ((ContinuousStates *) ssGetContStates(S))
      ->Wbattstartsat70_CSTATE;
    B_7_6_0 = ((Parameters *) ssGetDefaultParam(S))->P_4 * B_7_138_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_3_0 = rt_SATURATE(B_7_6_0, ((Parameters *)
      ssGetDefaultParam(S))->P_6, ((Parameters *) ssGetDefaultParam(S))->P_5);
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_4_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_7;
    }

    B_7_6_0 = (((BlockIO *) _ssGetBlockIO(S))->B_7_3_0 - ((BlockIO *)
                _ssGetBlockIO(S))->B_7_4_0) * ((Parameters *) ssGetDefaultParam
      (S))->P_8;
    B_7_29_0 = ((ContinuousStates *) ssGetContStates(S))->Integral_CSTATE;

    /* Integrator: '<S19>/Integral1' */
    if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE >=
        ((Parameters *) ssGetDefaultParam(S))->P_11 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_11) {
        ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_11;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE <=
               ((Parameters *) ssGetDefaultParam(S))->P_12 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_12) {
        ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_12;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_15_0 = ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_9_0 = B_7_29_0 / B_7_15_0;
    B_7_13_0 = (B_7_170_0 - B_7_6_0 / rt_SATURATE(((BlockIO *) _ssGetBlockIO(S)
      )->B_7_9_0, ((Parameters *) ssGetDefaultParam(S))->P_14, ((Parameters *)
      ssGetDefaultParam(S))->P_13)) * ((Parameters *) ssGetDefaultParam(S))
      ->P_15;

    /* Integrator: '<S9>/v->s' */
    if (((ContinuousStates *) ssGetContStates(S))->vs_CSTATE >= ((Parameters *)
         ssGetDefaultParam(S))->P_17 ) {
      if (((ContinuousStates *) ssGetContStates(S))->vs_CSTATE != ((Parameters *)
           ssGetDefaultParam(S))->P_17) {
        ((ContinuousStates *) ssGetContStates(S))->vs_CSTATE = ((Parameters *)
          ssGetDefaultParam(S))->P_17;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->vs_CSTATE <=
               ((Parameters *) ssGetDefaultParam(S))->P_18 ) {
      if (((ContinuousStates *) ssGetContStates(S))->vs_CSTATE != ((Parameters *)
           ssGetDefaultParam(S))->P_18) {
        ((ContinuousStates *) ssGetContStates(S))->vs_CSTATE = ((Parameters *)
          ssGetDefaultParam(S))->P_18;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_15_0 = ((ContinuousStates *) ssGetContStates(S))->vs_CSTATE;
    B_7_15_0 *= ((Parameters *) ssGetDefaultParam(S))->P_19;
    B_7_29_0 = B_7_13_0 / B_7_15_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_17_0 = rt_SATURATE(B_7_29_0,
      ((Parameters *) ssGetDefaultParam(S))->P_21, ((Parameters *)
      ssGetDefaultParam(S))->P_20);
    if (ssIsSampleHit(S, 1, 0)) {
      ssCallAccelRunBlock(S, 7, 18, SS_CALL_MDL_OUTPUTS);

      /* Scope: '<Root>/Fuel Consumption1' */

      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 19, SS_CALL_MDL_OUTPUTS);
    }

    /* Integrator: '<S6>/acc->speed' */
    if (((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE >=
        ((Parameters *) ssGetDefaultParam(S))->P_23 ) {
      if (((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_23) {
        ((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_23;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE <=
               ((Parameters *) ssGetDefaultParam(S))->P_24 ) {
      if (((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_24) {
        ((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_24;
        ssSetSolverNeedsReset(S);
      }
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_20_0 = ((ContinuousStates *)
      ssGetContStates(S))->accspeed_CSTATE;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_21_0 = ((Parameters *) ssGetDefaultParam
      (S))->P_25 * ((BlockIO *) _ssGetBlockIO(S))->B_7_20_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* FromWorkspace: '<S10>/v-buss2' */
      /* Call into Simulink for From Workspace */
      ssCallAccelRunBlock(S, 7, 22, SS_CALL_MDL_OUTPUTS);
      ((BlockIO *) _ssGetBlockIO(S))->B_7_23_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_26;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_24_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_233;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_25_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_24_0;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_26_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_27;
      ssCallAccelRunBlock(S, 7, 27, SS_CALL_MDL_OUTPUTS);
    }

    if (((BlockIO *) _ssGetBlockIO(S))->B_7_27_0 >= ((Parameters *)
         ssGetDefaultParam(S))->P_28) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_28_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_22_0[2];
    } else {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_28_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_26_0;
    }

    B_7_29_0 = ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE;
    B_7_170_0 = rt_SATURATE(B_7_29_0, ((Parameters *) ssGetDefaultParam(S))
      ->P_31, ((Parameters *) ssGetDefaultParam(S))->P_30);
    B_7_35_0 = rt_Lookup(&((Parameters *) ssGetDefaultParam(S))->P_32[0], 21,
                         B_7_170_0, &((Parameters *) ssGetDefaultParam(S))->
                         P_33[0]);
    B_7_35_0 = 1.0 / rt_SATURATE(B_7_35_0, ((Parameters *) ssGetDefaultParam(S)
      )->P_35, ((Parameters *) ssGetDefaultParam(S))->P_34) * B_7_170_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_34_0 = rt_SATURATE(B_7_35_0,
      ((Parameters *) ssGetDefaultParam(S))->P_37, ((Parameters *)
      ssGetDefaultParam(S))->P_36);
    B_7_35_0 = ((Parameters *) ssGetDefaultParam(S))->P_38 * ((BlockIO *)
      _ssGetBlockIO(S))->B_7_20_0;
    B_7_139_0 = rt_SATURATE(B_7_35_0, ((Parameters *) ssGetDefaultParam(S))
      ->P_40, ((Parameters *) ssGetDefaultParam(S))->P_39);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_37_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_34_0 / B_7_139_0;
    scalarIndex = plook_binx(((BlockIO *) _ssGetBlockIO(S))->B_7_37_0,
      &((Parameters *) ssGetDefaultParam(S))->P_41[0], 6U, &scalarFraction);
    B_7_38_0[0U] = (real_T)scalarIndex;
    B_7_38_0[1U] = scalarFraction;

    /* Relay: '<S25>/Relay'
     *
     * Regarding '<S25>/Relay':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (B_7_38_0[1] >= ((Parameters *) ssGetDefaultParam(S))->P_42 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay_Mode = TRUE;
      } else if (B_7_38_0[1] <= ((Parameters *) ssGetDefaultParam(S))->P_43 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay_Mode = FALSE;
      }
    }

    B_7_56_0 = ((D_Work *) ssGetRootDWork(S))->Relay_Mode ? ((Parameters *)
      ssGetDefaultParam(S))->P_44 : ((Parameters *) ssGetDefaultParam(S))->P_45;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_40_0 = B_7_38_0[0] + B_7_56_0;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_41_0 = ((D_Work *) ssGetRootDWork(S)
        )->Memory4_PreviousInput;
      B_7_47_0 = ((D_Work *) ssGetRootDWork(S))->Memory2_PreviousInput;
      B_7_47_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_41_0 - B_7_47_0;
      B_7_47_0 = muDoubleScalarAbs(B_7_47_0);
      if (ssIsMajorTimeStep(S) && (rt_ZCFcn(RISING_ZERO_CROSSING,
            &((PrevZCSigStates *) _ssGetPrevZCSigState(S))->reftime_Trig_ZCE_k,
            (B_7_47_0)) != NO_ZCEVENT)) {
        ((BlockIO *) _ssGetBlockIO(S))->B_4_0_0 = ((BlockIO *) _ssGetBlockIO(S)
          )->B_7_40_0;
        ((D_Work *) ssGetRootDWork(S))->reftime_SubsysRanBC = 4;
      }

      B_7_47_0 = rt_SATURATE(((BlockIO *) _ssGetBlockIO(S))->B_4_0_0,
        ((Parameters *) ssGetDefaultParam(S))->P_49, ((Parameters *)
        ssGetDefaultParam(S))->P_48);

      /* S-Function (sfun_nddirectlook): '<S25>/Direct Look-Up Table (n-D)' */
      /* LookupNDDirect: '<S25>/B_7_9' */
      /* 1-dimensional Direct Look-Up returning a Scalar */
      {
        int_T rt_uClip = (int_T)B_7_47_0;
        rt_uClip = rt_SATURATE(rt_uClip,0,
          6);
        ((BlockIO *) _ssGetBlockIO(S))->B_7_48_0 = ((Parameters *)
          ssGetDefaultParam(S))->P_50[rt_uClip];
      }

      ((BlockIO *) _ssGetBlockIO(S))->B_7_49_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_51;
    }

    /* TransportDelay Block: '<S28>/B_7_10' */
    {
      real_T **uBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
        ->TransportDelay2_PWORK.TUbufferPtrs[0];
      real_T **tBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
        ->TransportDelay2_PWORK.TUbufferPtrs[1];
      real_T simTime = ssGetT(S);
      real_T tMinusDelay = simTime - ((Parameters *) ssGetDefaultParam(S))->P_52;
      B_7_56_0 = Parallel_acc_rt_TDelayInterpolate(
        tMinusDelay,
        0.0,
        *tBuffer,
        *uBuffer,
        ((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.CircularBufSize,
        &((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Last,
        ((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Tail,
        ((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Head,
        ((Parameters *) ssGetDefaultParam(S))->P_53,
        1,
        (boolean_T) (ssIsMinorTimeStep(S) && (ssGetTimeOfLastOutput(S) == ssGetT
        (S))));
    }

    if (((BlockIO *) _ssGetBlockIO(S))->B_7_49_0 >= ((Parameters *)
         ssGetDefaultParam(S))->P_54) {
      B_7_56_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_48_0;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_52_0 = rt_SATURATE(B_7_56_0,
      ((Parameters *) ssGetDefaultParam(S))->P_56, ((Parameters *)
      ssGetDefaultParam(S))->P_55);

    /* TransferFcn Block: '<S5>/B_7_11' */
    ((BlockIO *) _ssGetBlockIO(S))->B_7_53_0 = ((Parameters *) ssGetDefaultParam
      (S))->P_59*((ContinuousStates *) ssGetContStates(S))
      ->usecondstimeconstant_CSTATE;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_54_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_62;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_55_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_52_0 * B_7_139_0;
    B_7_56_0 = rt_SATURATE(((BlockIO *) _ssGetBlockIO(S))->B_7_55_0,
      ((Parameters *) ssGetDefaultParam(S))->P_64, ((Parameters *)
      ssGetDefaultParam(S))->P_63);
    B_7_139_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_54_0 + muDoubleScalarAbs
      (B_7_56_0);
    B_7_13_0 = rt_Lookup(&((Parameters *) ssGetDefaultParam(S))->P_65[0], 21,
                         ((BlockIO *) _ssGetBlockIO(S))->B_7_55_0, &((Parameters
      *) ssGetDefaultParam(S))->P_66[0]);

    /* Relay: '<S7>/20'
     *
     * Regarding '<S7>/20':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (((BlockIO *) _ssGetBlockIO(S))->B_7_3_0 >= ((Parameters *)
           ssGetDefaultParam(S))->P_67 ) {
        ((D_Work *) ssGetRootDWork(S))->u_Mode = TRUE;
      } else if (((BlockIO *) _ssGetBlockIO(S))->B_7_3_0 <= ((Parameters *)
                  ssGetDefaultParam(S))->P_68 ) {
        ((D_Work *) ssGetRootDWork(S))->u_Mode = FALSE;
      }
    }

    B_7_148_0 = ((D_Work *) ssGetRootDWork(S))->u_Mode ? ((Parameters *)
      ssGetDefaultParam(S))->P_69 : ((Parameters *) ssGetDefaultParam(S))->P_70;
    B_7_148_0 *= B_7_13_0;
    B_7_228_0 = (((BlockIO *) _ssGetBlockIO(S))->B_7_22_0[0] - ((BlockIO *)
      _ssGetBlockIO(S))->B_7_20_0) * ((Parameters *) ssGetDefaultParam(S))->P_71;
    B_7_247_0 = muDoubleScalarAbs(((BlockIO *) _ssGetBlockIO(S))->B_7_20_0);

    /* Relay: '<S3>/Stäng integralverkan vid stillastående'
     *
     * Regarding '<S3>/Stäng integralverkan vid stillastående':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (B_7_247_0 >= ((Parameters *) ssGetDefaultParam(S))->P_72 ) {
        ((D_Work *) ssGetRootDWork(S))->Stngintegralverkanvidstillasten = TRUE;
      } else if (B_7_247_0 <= ((Parameters *) ssGetDefaultParam(S))->P_73 ) {
        ((D_Work *) ssGetRootDWork(S))->Stngintegralverkanvidstillasten = FALSE;
      }
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_65_0 = ((D_Work *) ssGetRootDWork(S))
      ->Stngintegralverkanvidstillasten ? ((Parameters *) ssGetDefaultParam(S)
      )->P_74 : ((Parameters *) ssGetDefaultParam(S))->P_75;

    /* Integrator: '<S3>/Wbatt' */
    if (ssIsMajorTimeStep(S)) {
      ZCEventType zcEvent;
      boolean_T resetIntg = FALSE;
      zcEvent = rt_ZCFcn(FALLING_ZERO_CROSSING,&((PrevZCSigStates *)
        _ssGetPrevZCSigState(S))->Wbatt_Reset_ZCE,
                         ((BlockIO *) _ssGetBlockIO(S))->B_7_65_0);

      /* evaluate zero-crossings */
      if (zcEvent) {
        resetIntg = TRUE;
        ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m = ((Parameters
          *) ssGetDefaultParam(S))->P_76;
      }

      if (resetIntg) {
        ssSetSolverNeedsReset(S);
      }
    }

    if (((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m >=
        ((Parameters *) ssGetDefaultParam(S))->P_77 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m !=
          ((Parameters *) ssGetDefaultParam(S))->P_77) {
        ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m = ((Parameters
          *) ssGetDefaultParam(S))->P_77;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m <=
               ((Parameters *) ssGetDefaultParam(S))->P_78 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m !=
          ((Parameters *) ssGetDefaultParam(S))->P_78) {
        ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m = ((Parameters
          *) ssGetDefaultParam(S))->P_78;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_257_0 = ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m;
    B_7_68_0 = (B_7_228_0 + B_7_257_0) * ((Parameters *) ssGetDefaultParam(S))
      ->P_79;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_69_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_80;
    }

    B_7_13_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_69_0 / rt_SATURATE(B_7_247_0,
      ((Parameters *) ssGetDefaultParam(S))->P_82, ((Parameters *)
      ssGetDefaultParam(S))->P_81);

    /* Relay: '<S3>/Relay2'
     *
     * Regarding '<S3>/Relay2':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (B_7_68_0 >= ((Parameters *) ssGetDefaultParam(S))->P_83 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay2_Mode = TRUE;
      } else if (B_7_68_0 <= ((Parameters *) ssGetDefaultParam(S))->P_84 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay2_Mode = FALSE;
      }
    }

    B_7_257_0 = ((D_Work *) ssGetRootDWork(S))->Relay2_Mode ? ((Parameters *)
      ssGetDefaultParam(S))->P_85 : ((Parameters *) ssGetDefaultParam(S))->P_86;
    if (B_7_257_0 >= ((Parameters *) ssGetDefaultParam(S))->P_87) {
      B_7_257_0 = muDoubleScalarMin(B_7_68_0, B_7_13_0);
    } else {
      B_7_257_0 = muDoubleScalarMax(B_7_68_0, ((Parameters *) ssGetDefaultParam
        (S))->P_228 * B_7_13_0);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_77_0 = 1.0 / ((BlockIO *) _ssGetBlockIO
      (S))->B_7_52_0 * B_7_257_0 - ((BlockIO *) _ssGetBlockIO(S))->B_7_53_0;
    B_7_78_0 = (((BlockIO *) _ssGetBlockIO(S))->B_7_77_0 > B_7_148_0);
    B_7_13_0 = rt_Lookup(&((Parameters *) ssGetDefaultParam(S))->P_88[0], 21,
                         ((BlockIO *) _ssGetBlockIO(S))->B_7_55_0, &((Parameters
      *) ssGetDefaultParam(S))->P_89[0]);

    /* Relay: '<S7>/90'
     *
     * Regarding '<S7>/90':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (((BlockIO *) _ssGetBlockIO(S))->B_7_3_0 >= ((Parameters *)
           ssGetDefaultParam(S))->P_90 ) {
        ((D_Work *) ssGetRootDWork(S))->u_Mode_n = TRUE;
      } else if (((BlockIO *) _ssGetBlockIO(S))->B_7_3_0 <= ((Parameters *)
                  ssGetDefaultParam(S))->P_91 ) {
        ((D_Work *) ssGetRootDWork(S))->u_Mode_n = FALSE;
      }
    }

    B_7_247_0 = ((D_Work *) ssGetRootDWork(S))->u_Mode_n ? ((Parameters *)
      ssGetDefaultParam(S))->P_92 : ((Parameters *) ssGetDefaultParam(S))->P_93;
    B_7_247_0 *= B_7_13_0;
    if (((BlockIO *) _ssGetBlockIO(S))->B_7_77_0 < B_7_247_0) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_83_0 = B_7_247_0;
    } else {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_83_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_77_0;
    }

    if (B_7_78_0) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_84_0 = B_7_148_0;
    } else {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_84_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_83_0;
    }

    B_7_247_0 = B_7_139_0 * rt_SATURATE(((BlockIO *) _ssGetBlockIO(S))->B_7_84_0,
                                        ((Parameters *) ssGetDefaultParam(S))
      ->P_95, ((Parameters *) ssGetDefaultParam(S))->P_94);
    B_7_139_0 = 1.0 / B_7_139_0 * rt_SATURATE(B_7_247_0, ((Parameters *)
      ssGetDefaultParam(S))->P_97, ((Parameters *) ssGetDefaultParam(S))->P_96);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_89_0 = rt_SATURATE(B_7_139_0,
      ((Parameters *) ssGetDefaultParam(S))->P_99, ((Parameters *)
      ssGetDefaultParam(S))->P_98);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_92_0 = (((BlockIO *) _ssGetBlockIO(S))
      ->B_7_53_0 + ((BlockIO *) _ssGetBlockIO(S))->B_7_89_0) * ((BlockIO *)
      _ssGetBlockIO(S))->B_7_52_0 * ((Parameters *) ssGetDefaultParam(S))->P_100;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_94_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_92_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_20_0 * ((Parameters *)
      ssGetDefaultParam(S))->P_101;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_96_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_102;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_97_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_103;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_98_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_104;
    }

    if (ssIsSampleHit(S, 1, 0)) {
      B_7_241_0 = ((Parameters *) ssGetDefaultParam(S))->P_109;
      B_7_230_0 = ((Parameters *) ssGetDefaultParam(S))->P_110;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_105_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_108 * ((Parameters *) ssGetDefaultParam(S))
        ->P_109 * ((Parameters *) ssGetDefaultParam(S))->P_110;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_108_0 = (muDoubleScalarAbs(((BlockIO *)
      _ssGetBlockIO(S))->B_7_20_0) * ((BlockIO *) _ssGetBlockIO(S))->B_7_20_0 *
      ((BlockIO *) _ssGetBlockIO(S))->B_7_96_0 * ((BlockIO *) _ssGetBlockIO(S)
      )->B_7_97_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_98_0 * ((Parameters *)
      ssGetDefaultParam(S))->P_105 + rt_SATURATE(((BlockIO *) _ssGetBlockIO(S)
      )->B_7_20_0, ((Parameters *) ssGetDefaultParam(S))->P_107, ((Parameters *)
      ssGetDefaultParam(S))->P_106) * ((BlockIO *) _ssGetBlockIO(S))->B_7_105_0)
      * ((Parameters *) ssGetDefaultParam(S))->P_111;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_110_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_108_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_20_0 * ((Parameters *)
      ssGetDefaultParam(S))->P_112;
    B_7_247_0 = B_7_257_0 - (((BlockIO *) _ssGetBlockIO(S))->B_7_89_0 +
      ((BlockIO *) _ssGetBlockIO(S))->B_7_53_0) * ((BlockIO *) _ssGetBlockIO(S)
      )->B_7_52_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_115_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_115 * rt_SATURATE(B_7_247_0, ((Parameters *)
      ssGetDefaultParam(S))->P_114, ((Parameters *) ssGetDefaultParam(S))->P_113);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_117_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_20_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_115_0 * ((Parameters *)
      ssGetDefaultParam(S))->P_116;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<Root>/Powers' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 118, SS_CALL_MDL_OUTPUTS);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_122_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_120 * rt_Lookup2D_Normal(&((Parameters *)
      ssGetDefaultParam(S))->P_117[0], 26, &((Parameters *) ssGetDefaultParam(S))
      ->P_118[0], 21, &((Parameters *) ssGetDefaultParam(S))->P_119[0],
      muDoubleScalarAbs(((BlockIO *) _ssGetBlockIO(S))->B_7_89_0), B_7_56_0) *
      ((Parameters *) ssGetDefaultParam(S))->P_121;
    if (muDoubleScalarSign(((BlockIO *) _ssGetBlockIO(S))->B_7_89_0) >=
        ((Parameters *) ssGetDefaultParam(S))->P_122) {
      B_7_247_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_122_0;
    } else {
      B_7_247_0 = 1.0 / ((BlockIO *) _ssGetBlockIO(S))->B_7_122_0;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_126_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_89_0 * B_7_56_0 / B_7_247_0;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_127_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_123;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_128_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_126_0 + ((BlockIO *) _ssGetBlockIO(S))->B_7_127_0;
    B_7_247_0 = ((Parameters *) ssGetDefaultParam(S))->P_124 * ((BlockIO *)
      _ssGetBlockIO(S))->B_7_128_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_132_0 = B_7_247_0 * rt_Lookup
      (&((Parameters *) ssGetDefaultParam(S))->P_127[0], 22, rt_SATURATE
       (B_7_247_0, ((Parameters *) ssGetDefaultParam(S))->P_126, ((Parameters *)
         ssGetDefaultParam(S))->P_125), &((Parameters *) ssGetDefaultParam(S))
       ->P_128[0]);
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<Root>/SOC' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 133, SS_CALL_MDL_OUTPUTS);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_134_0[0] = ((Parameters *)
      ssGetDefaultParam(S))->P_129 * ((BlockIO *) _ssGetBlockIO(S))->B_7_22_0[0];
    ((BlockIO *) _ssGetBlockIO(S))->B_7_134_0[1] = ((Parameters *)
      ssGetDefaultParam(S))->P_129 * ((BlockIO *) _ssGetBlockIO(S))->B_7_20_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<Root>/v* & v' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 135, SS_CALL_MDL_OUTPUTS);
    }

    /* Integrator: '<S1>/Charging energy' */
    if (((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE >=
        ((Parameters *) ssGetDefaultParam(S))->P_131 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_131) {
        ((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_131;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE <=
               ((Parameters *) ssGetDefaultParam(S))->P_132 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_132) {
        ((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_132;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_148_0 = ((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_137_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_133;
    }

    B_7_138_0 -= ((BlockIO *) _ssGetBlockIO(S))->B_7_137_0;

    /* Integrator: '<S1>/Dicharing energy' */
    if (((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE >=
        ((Parameters *) ssGetDefaultParam(S))->P_135 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_135) {
        ((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_135;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE
               <= ((Parameters *) ssGetDefaultParam(S))->P_136 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE !=
          ((Parameters *) ssGetDefaultParam(S))->P_136) {
        ((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE =
          ((Parameters *) ssGetDefaultParam(S))->P_136;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_139_0 = ((ContinuousStates *) ssGetContStates(S))
      ->Dicharingenergy_CSTATE;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_142_0 = (((Parameters *)
      ssGetDefaultParam(S))->P_137 * B_7_139_0 + B_7_138_0) * (1.0 / B_7_148_0);
    if (ssIsSampleHit(S, 1, 0)) {
      ssCallAccelRunBlock(S, 7, 143, SS_CALL_MDL_OUTPUTS);

      /* Scope: '<S1>/Scope' */

      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 144, SS_CALL_MDL_OUTPUTS);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_145_0 = B_7_138_0 * B_7_138_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* ToWorkspace: '<S1>/To Workspace1' */
      /* Call into Simulink for To Workspace */
      ssCallAccelRunBlock(S, 7, 146, SS_CALL_MDL_OUTPUTS);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_147_0 = rt_SATURATE(B_7_247_0,
      ((Parameters *) ssGetDefaultParam(S))->P_139, ((Parameters *)
      ssGetDefaultParam(S))->P_138);
    B_7_148_0 = ((ContinuousStates *) ssGetContStates(S))
      ->Dicharingenergy1_CSTATE;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_149_0 = rt_SATURATE(B_7_247_0,
      ((Parameters *) ssGetDefaultParam(S))->P_142, ((Parameters *)
      ssGetDefaultParam(S))->P_141);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_150_0 = B_7_247_0 - ((BlockIO *)
      _ssGetBlockIO(S))->B_7_132_0;
    B_7_247_0 = B_7_68_0 - B_7_257_0;
    B_7_247_0 = muDoubleScalarAbs(B_7_247_0);
    B_7_13_0 = ((Parameters *) ssGetDefaultParam(S))->P_143 * B_7_228_0;

    /* Relay: '<S3>/Relay1'
     *
     * Regarding '<S3>/Relay1':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (B_7_247_0 >= ((Parameters *) ssGetDefaultParam(S))->P_144 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay1_Mode = TRUE;
      } else if (B_7_247_0 <= ((Parameters *) ssGetDefaultParam(S))->P_145 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay1_Mode = FALSE;
      }
    }

    B_7_247_0 = ((D_Work *) ssGetRootDWork(S))->Relay1_Mode ? ((Parameters *)
      ssGetDefaultParam(S))->P_146 : ((Parameters *) ssGetDefaultParam(S))
      ->P_147;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_155_0 = B_7_247_0 * B_7_13_0 * ((BlockIO
      *) _ssGetBlockIO(S))->B_7_65_0;
    B_7_247_0 = ((ContinuousStates *) ssGetContStates(S))->Integral_CSTATE_o;

    /* Integrator: '<S14>/Integral1' */
    if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i >=
        ((Parameters *) ssGetDefaultParam(S))->P_150 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i !=
          ((Parameters *) ssGetDefaultParam(S))->P_150) {
        ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i =
          ((Parameters *) ssGetDefaultParam(S))->P_150;
        ssSetSolverNeedsReset(S);
      }
    } else if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i <=
               ((Parameters *) ssGetDefaultParam(S))->P_151 ) {
      if (((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i !=
          ((Parameters *) ssGetDefaultParam(S))->P_151) {
        ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i =
          ((Parameters *) ssGetDefaultParam(S))->P_151;
        ssSetSolverNeedsReset(S);
      }
    }

    B_7_228_0 = ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_158_0 = B_7_247_0 / B_7_228_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_159_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_89_0 * B_7_56_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S4>/EM' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 160, SS_CALL_MDL_OUTPUTS);
    }

    /* Clock: '<S4>/Clock' */
    ((BlockIO *) _ssGetBlockIO(S))->B_7_161_0 = ssGetT(S);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_162_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_152 * ((BlockIO *) _ssGetBlockIO(S))->B_7_55_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* ToWorkspace: '<S4>/To Workspace' */
      /* Call into Simulink for To Workspace */
      ssCallAccelRunBlock(S, 7, 163, SS_CALL_MDL_OUTPUTS);
    }

    /* Relay: '<S14>/Relay'
     *
     * Regarding '<S14>/Relay':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (((BlockIO *) _ssGetBlockIO(S))->B_7_122_0 >= ((Parameters *)
           ssGetDefaultParam(S))->P_153 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay_Mode_n = TRUE;
      } else if (((BlockIO *) _ssGetBlockIO(S))->B_7_122_0 <= ((Parameters *)
                  ssGetDefaultParam(S))->P_154 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay_Mode_n = FALSE;
      }
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_164_0 = ((D_Work *) ssGetRootDWork(S))
      ->Relay_Mode_n ? ((Parameters *) ssGetDefaultParam(S))->P_155 :
      ((Parameters *) ssGetDefaultParam(S))->P_156;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_165_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_122_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_164_0;
    B_7_13_0 = 1.0 / ((BlockIO *) _ssGetBlockIO(S))->B_7_34_0 * B_7_170_0;
    B_7_170_0 = rt_Lookup(&((Parameters *) ssGetDefaultParam(S))->P_157[0], 21,
                          B_7_170_0, &((Parameters *) ssGetDefaultParam(S))
                          ->P_158[0]);

    /* Relay: '<S23>/NO ICE Torque @  to low efficiency'
     *
     * Regarding '<S23>/NO ICE Torque @  to low efficiency':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (B_7_170_0 >= ((Parameters *) ssGetDefaultParam(S))->P_159 ) {
        ((D_Work *) ssGetRootDWork(S))->NOICETorquetolowefficiency_Mode = TRUE;
      } else if (B_7_170_0 <= ((Parameters *) ssGetDefaultParam(S))->P_160 ) {
        ((D_Work *) ssGetRootDWork(S))->NOICETorquetolowefficiency_Mode = FALSE;
      }
    }

    B_7_170_0 = ((D_Work *) ssGetRootDWork(S))->NOICETorquetolowefficiency_Mode ?
      ((Parameters *) ssGetDefaultParam(S))->P_161 : ((Parameters *)
      ssGetDefaultParam(S))->P_162;

    /* Relay: '<S23>/ICE always at high power'
     *
     * Regarding '<S23>/ICE always at high power':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (B_7_29_0 >= ((Parameters *) ssGetDefaultParam(S))->P_163 ) {
        ((D_Work *) ssGetRootDWork(S))->ICEalwaysathighpower_Mode = TRUE;
      } else if (B_7_29_0 <= ((Parameters *) ssGetDefaultParam(S))->P_164 ) {
        ((D_Work *) ssGetRootDWork(S))->ICEalwaysathighpower_Mode = FALSE;
      }
    }

    B_7_247_0 = ((D_Work *) ssGetRootDWork(S))->ICEalwaysathighpower_Mode ?
      ((Parameters *) ssGetDefaultParam(S))->P_165 : ((Parameters *)
      ssGetDefaultParam(S))->P_166;
    B_7_170_0 = (real_T)((B_7_170_0 != 0.0) || (B_7_247_0 != 0.0));
    ((BlockIO *) _ssGetBlockIO(S))->B_7_171_0 = B_7_13_0 * B_7_170_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_172_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_171_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_34_0 / ((BlockIO *)
      _ssGetBlockIO(S))->B_7_55_0;
    B_7_247_0 = (((BlockIO *) _ssGetBlockIO(S))->B_7_77_0 - ((BlockIO *)
      _ssGetBlockIO(S))->B_7_89_0) + ((BlockIO *) _ssGetBlockIO(S))->B_7_172_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_176_0 = rt_SATURATE(B_7_247_0,
      ((Parameters *) ssGetDefaultParam(S))->P_168, ((Parameters *)
      ssGetDefaultParam(S))->P_167) * B_7_170_0;
    B_7_247_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_55_0 * B_7_170_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_178_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_169 * B_7_247_0;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_179_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_170;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_180_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_235;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_181_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_180_0;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_182_0 = rt_SATURATE(((BlockIO *)
      _ssGetBlockIO(S))->B_7_53_0, ((Parameters *) ssGetDefaultParam(S))->P_172,
      ((Parameters *) ssGetDefaultParam(S))->P_171);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_184_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_176 * rt_Lookup2D_Normal(&((Parameters *)
      ssGetDefaultParam(S))->P_173[0], 21, &((Parameters *) ssGetDefaultParam(S))
      ->P_174[0], 21, &((Parameters *) ssGetDefaultParam(S))->P_175[0],
      ((BlockIO *) _ssGetBlockIO(S))->B_7_182_0, B_7_247_0);
    ssCallAccelRunBlock(S, 7, 185, SS_CALL_MDL_OUTPUTS);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_186_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_182_0 * B_7_247_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S5>/ICE' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 187, SS_CALL_MDL_OUTPUTS);
    }

    /* Clock: '<S5>/Clock' */
    ((BlockIO *) _ssGetBlockIO(S))->B_7_188_0 = ssGetT(S);
    if (ssIsSampleHit(S, 1, 0)) {
      /* ToWorkspace: '<S5>/To Workspace' */
      /* Call into Simulink for To Workspace */
      ssCallAccelRunBlock(S, 7, 189, SS_CALL_MDL_OUTPUTS);
      ((BlockIO *) _ssGetBlockIO(S))->B_7_190_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_237;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_191_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_190_0;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_193_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_180 * rt_Lookup2D_Normal(&((Parameters *)
      ssGetDefaultParam(S))->P_177[0], 21, &((Parameters *) ssGetDefaultParam(S))
      ->P_178[0], 21, &((Parameters *) ssGetDefaultParam(S))->P_179[0],
      ((BlockIO *) _ssGetBlockIO(S))->B_7_182_0, B_7_247_0);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_194_0 = 1.0 / ((BlockIO *) _ssGetBlockIO
      (S))->B_7_185_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_186_0;
    ssCallAccelRunBlock(S, 7, 195, SS_CALL_MDL_OUTPUTS);
    B_7_13_0 = rt_SATURATE(B_7_247_0, ((Parameters *) ssGetDefaultParam(S))
      ->P_182, ((Parameters *) ssGetDefaultParam(S))->P_181);
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S19>/Scope' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 197, SS_CALL_MDL_OUTPUTS);
    }

    /* Relay: '<S19>/Relay'
     *
     * Regarding '<S19>/Relay':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (((BlockIO *) _ssGetBlockIO(S))->B_7_185_0 >= ((Parameters *)
           ssGetDefaultParam(S))->P_183 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay_Mode_d = TRUE;
      } else if (((BlockIO *) _ssGetBlockIO(S))->B_7_185_0 <= ((Parameters *)
                  ssGetDefaultParam(S))->P_184 ) {
        ((D_Work *) ssGetRootDWork(S))->Relay_Mode_d = FALSE;
      }
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_198_0 = ((D_Work *) ssGetRootDWork(S))
      ->Relay_Mode_d ? ((Parameters *) ssGetDefaultParam(S))->P_185 :
      ((Parameters *) ssGetDefaultParam(S))->P_186;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_199_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_185_0 * ((BlockIO *) _ssGetBlockIO(S))->B_7_198_0;
    B_7_247_0 = rt_Lookup(&((Parameters *) ssGetDefaultParam(S))->P_187[0], 21,
                          B_7_13_0, &((Parameters *) ssGetDefaultParam(S))
                          ->P_188[0]);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_201_0 = muDoubleScalarMin(((BlockIO *)
      _ssGetBlockIO(S))->B_7_176_0, B_7_247_0);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_202_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_255_0 * B_7_170_0;
    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_203_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_189;
    }

    if (((BlockIO *) _ssGetBlockIO(S))->B_7_27_0 >= ((Parameters *)
         ssGetDefaultParam(S))->P_190) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_204_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_22_0[3];
    } else {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_204_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_26_0;
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_207_0 = ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_203_0 * muDoubleScalarSin(((BlockIO *) _ssGetBlockIO(S))->B_7_204_0)
      * ((Parameters *) ssGetDefaultParam(S))->P_191;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S6>/Forces' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 208, SS_CALL_MDL_OUTPUTS);

      /* Scope: '<S6>/Scope' */

      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 209, SS_CALL_MDL_OUTPUTS);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_212_0 = (((((BlockIO *) _ssGetBlockIO(S))
      ->B_7_92_0 + ((BlockIO *) _ssGetBlockIO(S))->B_7_115_0) + ((BlockIO *)
      _ssGetBlockIO(S))->B_7_108_0) + ((BlockIO *) _ssGetBlockIO(S))->B_7_207_0)
      / ((BlockIO *) _ssGetBlockIO(S))->B_7_203_0;
    if (ssIsSampleHit(S, 1, 0)) {
      B_7_230_0 = ((Parameters *) ssGetDefaultParam(S))->P_192;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_214_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_192 - B_7_47_0;
    }

    /* TransportDelay Block: '<S28>/B_7_44' */
    {
      real_T **uBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
        ->TransportDelay1_PWORK.TUbufferPtrs[0];
      real_T **tBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
        ->TransportDelay1_PWORK.TUbufferPtrs[1];
      real_T simTime = ssGetT(S);
      real_T tMinusDelay = simTime - ((Parameters *) ssGetDefaultParam(S))
        ->P_193;
      B_7_247_0 = Parallel_acc_rt_TDelayInterpolate(
        tMinusDelay,
        0.0,
        *tBuffer,
        *uBuffer,
        ((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.CircularBufSize,
        &((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Last,
        ((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Tail,
        ((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Head,
        ((Parameters *) ssGetDefaultParam(S))->P_194,
        1,
        (boolean_T) (ssIsMinorTimeStep(S) && (ssGetTimeOfLastOutput(S) == ssGetT
        (S))));
    }

    if (((BlockIO *) _ssGetBlockIO(S))->B_7_49_0 >= ((Parameters *)
         ssGetDefaultParam(S))->P_195) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_216_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_214_0;
    } else {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_216_0 = B_7_247_0;
    }

    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S21>/Växellåda' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 217, SS_CALL_MDL_OUTPUTS);
      B_7_230_0 = muDoubleScalarAbs(((BlockIO *) _ssGetBlockIO(S))->B_4_0_0 -
        ((D_Work *) ssGetRootDWork(S))->Memory1_PreviousInput);
      if (ssIsMajorTimeStep(S) && (rt_ZCFcn(ANY_ZERO_CROSSING,&((PrevZCSigStates
              *) _ssGetPrevZCSigState(S))->Subsystem2_Trig_ZCE,
            (B_7_230_0)) != NO_ZCEVENT)) {
        ((BlockIO *) _ssGetBlockIO(S))->B_5_2_0 = ((Parameters *)
          ssGetDefaultParam(S))->P_231 + ((D_Work *) ssGetRootDWork(S))
          ->Memory_PreviousInput;
        ((D_Work *) ssGetRootDWork(S))->Memory_PreviousInput = ((BlockIO *)
          _ssGetBlockIO(S))->B_5_2_0;
        ((D_Work *) ssGetRootDWork(S))->Subsystem2_SubsysRanBC = 4;
      }

      ssCallAccelRunBlock(S, 7, 223, SS_CALL_MDL_OUTPUTS);

      /* ToWorkspace: '<S25>/To Workspace' */

      /* Call into Simulink for To Workspace */
      ssCallAccelRunBlock(S, 7, 224, SS_CALL_MDL_OUTPUTS);
      ((BlockIO *) _ssGetBlockIO(S))->B_7_225_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_197;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_226_0 = ((real_T)((D_Work *)
        ssGetRootDWork(S))->clockTickCounter < ((Parameters *) ssGetDefaultParam
        (S))->P_200) && (((D_Work *) ssGetRootDWork(S))->clockTickCounter >= 0) ?
        ((Parameters *) ssGetDefaultParam(S))->P_198 : 0.0;
      if ((real_T)((D_Work *) ssGetRootDWork(S))->clockTickCounter >=
          ((Parameters *) ssGetDefaultParam(S))->P_199 - 1.0) {
        ((D_Work *) ssGetRootDWork(S))->clockTickCounter = 0;
      } else {
        ((D_Work *) ssGetRootDWork(S))->clockTickCounter = ((D_Work *)
          ssGetRootDWork(S))->clockTickCounter + 1;
      }
    }

    /* Relay: '<S26>/w>yy'
     *
     * Regarding '<S26>/w>yy':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (((BlockIO *) _ssGetBlockIO(S))->B_7_55_0 >= ((Parameters *)
           ssGetDefaultParam(S))->P_201 ) {
        ((D_Work *) ssGetRootDWork(S))->wyy_Mode = TRUE;
      } else if (((BlockIO *) _ssGetBlockIO(S))->B_7_55_0 <= ((Parameters *)
                  ssGetDefaultParam(S))->P_202 ) {
        ((D_Work *) ssGetRootDWork(S))->wyy_Mode = FALSE;
      }
    }

    B_7_247_0 = ((D_Work *) ssGetRootDWork(S))->wyy_Mode ? ((Parameters *)
      ssGetDefaultParam(S))->P_203 : ((Parameters *) ssGetDefaultParam(S))
      ->P_204;

    /* Relay: '<S26>/w<xx'
     *
     * Regarding '<S26>/w<xx':
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * On  Points Value parameter uses the same data type and scaling as Input0
     * Off Points Value parameter uses the same data type and scaling as Input0
     * On  Output Value parameter uses the same data type and scaling as Output0
     * Off Output Value parameter uses the same data type and scaling as Output0
     */
    if (ssIsMajorTimeStep(S)) {
      if (((BlockIO *) _ssGetBlockIO(S))->B_7_55_0 >= ((Parameters *)
           ssGetDefaultParam(S))->P_205 ) {
        ((D_Work *) ssGetRootDWork(S))->wxx_Mode = TRUE;
      } else if (((BlockIO *) _ssGetBlockIO(S))->B_7_55_0 <= ((Parameters *)
                  ssGetDefaultParam(S))->P_206 ) {
        ((D_Work *) ssGetRootDWork(S))->wxx_Mode = FALSE;
      }
    }

    B_7_228_0 = ((D_Work *) ssGetRootDWork(S))->wxx_Mode ? ((Parameters *)
      ssGetDefaultParam(S))->P_207 : ((Parameters *) ssGetDefaultParam(S))
      ->P_208;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_229_0 = (((BlockIO *) _ssGetBlockIO(S)
      )->B_7_226_0 + B_7_247_0) + B_7_228_0;
    if (ssIsSampleHit(S, 1, 0)) {
      B_7_230_0 = ((D_Work *) ssGetRootDWork(S))->Memory3_PreviousInput;
      if (((D_Work *) ssGetRootDWork(S))->Memory3_PreviousInput >= ((Parameters *)
           ssGetDefaultParam(S))->P_211) {
        ((BlockIO *) _ssGetBlockIO(S))->B_7_232_0 = ((BlockIO *) _ssGetBlockIO(S))
          ->B_7_214_0;
      } else {
        ((BlockIO *) _ssGetBlockIO(S))->B_7_232_0 = ((D_Work *) ssGetRootDWork(S))
          ->Memory2_PreviousInput_m;
      }

      B_7_241_0 = muDoubleScalarAbs(((BlockIO *) _ssGetBlockIO(S))->B_7_232_0 -
                                    ((D_Work *) ssGetRootDWork(S))
        ->Memory1_PreviousInput_o);
    }

    /* Clock: '<S28>/Clock' */
    ((BlockIO *) _ssGetBlockIO(S))->B_7_236_0 = ssGetT(S);
    if (ssIsSampleHit(S, 1, 0)) {
      if (ssIsMajorTimeStep(S) && (rt_ZCFcn(RISING_ZERO_CROSSING,
            &((PrevZCSigStates *) _ssGetPrevZCSigState(S))->reftime_Trig_ZCE,
            (B_7_241_0)) != NO_ZCEVENT)) {
        ((BlockIO *) _ssGetBlockIO(S))->B_6_0_0 = ((BlockIO *) _ssGetBlockIO(S)
          )->B_7_236_0;
        ((D_Work *) ssGetRootDWork(S))->reftime_SubsysRanBC_e = 4;
      }
    }

    B_7_239_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_236_0 - ((BlockIO *)
      _ssGetBlockIO(S))->B_6_0_0;

    /* Lookup Block: '<S28>/Look-Up Table'
     * About '<S28>/Look-Up Table :'
     * Lookup Block: '<S28>/Look-Up Table'
     * Input0  Data Type:  Floating Point real_T
     * Output0 Data Type:  Floating Point real_T
     * Saturation Mode: Saturate
     * Lookup Method: Linear_Endpoint
     *
     * XData parameter uses the same data type and scaling as Input0
     * YData parameter uses the same data type and scaling as Output0


     */
    Parallel_acc_LookUp_real_T_real_T_SAT( &(((BlockIO *) _ssGetBlockIO(S))
      ->B_7_240_0), ((Parameters *) ssGetDefaultParam(S))->P_214, B_7_239_0,
      ((Parameters *) ssGetDefaultParam(S))->P_213, 3U);
    if (ssIsSampleHit(S, 1, 0)) {
      if (B_7_230_0 >= ((Parameters *) ssGetDefaultParam(S))->P_216) {
        ((BlockIO *) _ssGetBlockIO(S))->B_7_242_0 = ((BlockIO *) _ssGetBlockIO(S))
          ->B_7_48_0;
      } else {
        ((BlockIO *) _ssGetBlockIO(S))->B_7_242_0 = ((D_Work *) ssGetRootDWork(S))
          ->Memory4_PreviousInput_d;
      }
    }

    if (((BlockIO *) _ssGetBlockIO(S))->B_7_49_0 >= ((Parameters *)
         ssGetDefaultParam(S))->P_217) {
      B_7_247_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_225_0;
    } else {
      B_7_247_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_240_0;
    }

    if (((BlockIO *) _ssGetBlockIO(S))->B_7_49_0 >= ((Parameters *)
         ssGetDefaultParam(S))->P_218) {
      B_7_247_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_225_0;
    } else {
      /* Lookup Block: '<S28>/Look-Up Table1'
       * About '<S28>/Look-Up Table1 :'
       * Lookup Block: '<S28>/Look-Up Table1'
       * Input0  Data Type:  Floating Point real_T
       * Output0 Data Type:  Floating Point real_T
       * Saturation Mode: Saturate
       * Lookup Method: Linear_Endpoint
       *
       * XData parameter uses the same data type and scaling as Input0
       * YData parameter uses the same data type and scaling as Output0


       */
      Parallel_acc_LookUp_real_T_real_T_SAT( &(B_3_0_0), ((Parameters *)
        ssGetDefaultParam(S))->P_230, B_7_239_0, ((Parameters *)
        ssGetDefaultParam(S))->P_229, 5U);
      B_7_247_0 = B_3_0_0;
    }

    if (ssIsSampleHit(S, 1, 0)) {
      ((BlockIO *) _ssGetBlockIO(S))->B_7_246_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_219;
    }

    B_7_247_0 = ((BlockIO *) _ssGetBlockIO(S))->B_7_246_0 - ((BlockIO *)
      _ssGetBlockIO(S))->B_7_3_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_248_0 = ((Parameters *)
      ssGetDefaultParam(S))->P_220 * B_7_247_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_250_0 = muDoubleScalarAbs(B_7_247_0);
    ((BlockIO *) _ssGetBlockIO(S))->B_7_251_0 = rt_Lookup(&((Parameters *)
      ssGetDefaultParam(S))->P_221[0], 31, B_7_247_0, &((Parameters *)
      ssGetDefaultParam(S))->P_222[0]) * ((BlockIO *) _ssGetBlockIO(S))
      ->B_7_250_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S22>/Scope' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 252, SS_CALL_MDL_OUTPUTS);
      ((BlockIO *) _ssGetBlockIO(S))->B_7_253_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_239;
      ((BlockIO *) _ssGetBlockIO(S))->B_7_254_0 = ((BlockIO *) _ssGetBlockIO(S)
        )->B_7_253_0;
    }

    ssCallAccelRunBlock(S, 7, 255, SS_CALL_MDL_OUTPUTS);
    B_7_257_0 = B_7_35_0 * B_7_257_0 + ((BlockIO *) _ssGetBlockIO(S))->B_7_255_0;
    ((BlockIO *) _ssGetBlockIO(S))->B_7_260_0 = (rt_SATURATE(B_7_257_0,
      ((Parameters *) ssGetDefaultParam(S))->P_224, ((Parameters *)
      ssGetDefaultParam(S))->P_223) - B_7_29_0) * ((Parameters *)
      ssGetDefaultParam(S))->P_225;
    if (ssIsSampleHit(S, 1, 0)) {
      /* ToWorkspace: '<S9>/To Workspace' */
      /* Call into Simulink for To Workspace */
      ssCallAccelRunBlock(S, 7, 263, SS_CALL_MDL_OUTPUTS);
    }

    ((BlockIO *) _ssGetBlockIO(S))->B_7_264_0 = B_7_6_0 / B_7_15_0;
    if (ssIsSampleHit(S, 1, 0)) {
      /* Scope: '<S9>/Ws//mil' */
      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 265, SS_CALL_MDL_OUTPUTS);
      ((BlockIO *) _ssGetBlockIO(S))->B_7_266_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_226 * ((BlockIO *) _ssGetBlockIO(S))->B_7_22_0
        [0];
      ((BlockIO *) _ssGetBlockIO(S))->B_7_267_0 = ((Parameters *)
        ssGetDefaultParam(S))->P_227 * ((BlockIO *) _ssGetBlockIO(S))->B_7_22_0
        [1];

      /* Scope: '<S10>/Cycle' */

      /* Call into Simulink for Scope */
      ssCallAccelRunBlock(S, 7, 268, SS_CALL_MDL_OUTPUTS);
    }
  }

  /* tid is required for a uniform function interface. This system
   * is single rate, and in this case, tid is not accessed. */
  UNUSED_PARAMETER(tid);
}

/* Update for root system: '<Root>' */
#define MDL_UPDATE

static void mdlUpdate(SimStruct *S, int_T tid)
{
  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
    ((D_Work *) ssGetRootDWork(S))->Memory4_PreviousInput = ((BlockIO *)
      _ssGetBlockIO(S))->B_7_229_0;
    ((D_Work *) ssGetRootDWork(S))->Memory2_PreviousInput = ((BlockIO *)
      _ssGetBlockIO(S))->B_7_41_0;
  }

  /* TransportDelay Block: '<S28>/B_7_10' */
  {
    real_T **uBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
      ->TransportDelay2_PWORK.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
      ->TransportDelay2_PWORK.TUbufferPtrs[1];
    real_T simTime = ssGetT(S);
    ((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Head = ((((D_Work *)
      ssGetRootDWork(S))->TransportDelay2_IWORK.Head < (((D_Work *)
      ssGetRootDWork(S))->TransportDelay2_IWORK.CircularBufSize-1)) ? (((D_Work *)
      ssGetRootDWork(S))->TransportDelay2_IWORK.Head+1) : 0);
    if (((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Head == ((D_Work *)
         ssGetRootDWork(S))->TransportDelay2_IWORK.Tail) {
      if (!Parallel_acc_rt_TDelayUpdateTailOrGrowBuf( &((D_Work *)
            ssGetRootDWork(S))->TransportDelay2_IWORK.CircularBufSize, &((D_Work
             *) ssGetRootDWork(S))->TransportDelay2_IWORK.Tail, &((D_Work *)
            ssGetRootDWork(S))->TransportDelay2_IWORK.Head, &((D_Work *)
            ssGetRootDWork(S))->TransportDelay2_IWORK.Last, simTime -
           ((Parameters *) ssGetDefaultParam(S))->P_52, tBuffer, uBuffer, NULL,
           (boolean_T)0, FALSE, &((D_Work *) ssGetRootDWork(S))
           ->TransportDelay2_IWORK.MaxNewBufSize)) {
        ssSetErrorStatus(S, "tdelay memory allocation error");
      }
    }

    (*tBuffer)[((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Head] =
      simTime;
    (*uBuffer)[((D_Work *) ssGetRootDWork(S))->TransportDelay2_IWORK.Head] =
      ((BlockIO *) _ssGetBlockIO(S))->B_7_242_0;
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  /* TransportDelay Block: '<S28>/B_7_44' */
  {
    real_T **uBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
      ->TransportDelay1_PWORK.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)&((D_Work *) ssGetRootDWork(S))
      ->TransportDelay1_PWORK.TUbufferPtrs[1];
    real_T simTime = ssGetT(S);
    ((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Head = ((((D_Work *)
      ssGetRootDWork(S))->TransportDelay1_IWORK.Head < (((D_Work *)
      ssGetRootDWork(S))->TransportDelay1_IWORK.CircularBufSize-1)) ? (((D_Work *)
      ssGetRootDWork(S))->TransportDelay1_IWORK.Head+1) : 0);
    if (((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Head == ((D_Work *)
         ssGetRootDWork(S))->TransportDelay1_IWORK.Tail) {
      if (!Parallel_acc_rt_TDelayUpdateTailOrGrowBuf( &((D_Work *)
            ssGetRootDWork(S))->TransportDelay1_IWORK.CircularBufSize, &((D_Work
             *) ssGetRootDWork(S))->TransportDelay1_IWORK.Tail, &((D_Work *)
            ssGetRootDWork(S))->TransportDelay1_IWORK.Head, &((D_Work *)
            ssGetRootDWork(S))->TransportDelay1_IWORK.Last, simTime -
           ((Parameters *) ssGetDefaultParam(S))->P_193, tBuffer, uBuffer, NULL,
           (boolean_T)0, FALSE, &((D_Work *) ssGetRootDWork(S))
           ->TransportDelay1_IWORK.MaxNewBufSize)) {
        ssSetErrorStatus(S, "tdelay memory allocation error");
      }
    }

    (*tBuffer)[((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Head] =
      simTime;
    (*uBuffer)[((D_Work *) ssGetRootDWork(S))->TransportDelay1_IWORK.Head] =
      ((BlockIO *) _ssGetBlockIO(S))->B_7_232_0;
  }

  if (ssIsSampleHit(S, 1, 0)) {
    ((D_Work *) ssGetRootDWork(S))->Memory1_PreviousInput = ((BlockIO *)
      _ssGetBlockIO(S))->B_4_0_0;
  }

  if (ssIsSampleHit(S, 1, 0)) {
    ((D_Work *) ssGetRootDWork(S))->Memory3_PreviousInput = ((BlockIO *)
      _ssGetBlockIO(S))->B_7_240_0;
    ((D_Work *) ssGetRootDWork(S))->Memory2_PreviousInput_m = ((BlockIO *)
      _ssGetBlockIO(S))->B_7_232_0;
    ((D_Work *) ssGetRootDWork(S))->Memory1_PreviousInput_o = ((BlockIO *)
      _ssGetBlockIO(S))->B_7_232_0;
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
    ((D_Work *) ssGetRootDWork(S))->Memory4_PreviousInput_d = ((BlockIO *)
      _ssGetBlockIO(S))->B_7_242_0;
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  if (ssIsSampleHit(S, 1, 0)) {
  }

  /* tid is required for a uniform function interface. This system
   * is single rate, and in this case, tid is not accessed. */
  UNUSED_PARAMETER(tid);
}

/* Derivatives for root system: '<Root>' */
#define MDL_DERIVATIVES

static void mdlDerivatives(SimStruct *S)
{
  ((StateDerivatives *) ssGetdX(S))->FuelEnergy_CSTATE = ((BlockIO *)
    _ssGetBlockIO(S))->B_7_202_0;

  /* Limited Integrator Block: '<S12>/B_7_0' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE <=
            ((Parameters *) ssGetDefaultParam(S))->P_3 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->Wbattstartsat70_CSTATE >=
            ((Parameters *) ssGetDefaultParam(S))->P_2 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_132_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_132_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->Wbattstartsat70_CSTATE = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_132_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->Wbattstartsat70_CSTATE = 0.0;
    }
  }

  ((StateDerivatives *) ssGetdX(S))->Integral_CSTATE = ((BlockIO *)
    _ssGetBlockIO(S))->B_7_199_0;

  /* Limited Integrator Block: '<S19>/B_7_1' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE <=
            ((Parameters *) ssGetDefaultParam(S))->P_12 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE >=
            ((Parameters *) ssGetDefaultParam(S))->P_11 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_198_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_198_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->Integral1_CSTATE = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_198_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->Integral1_CSTATE = 0.0;
    }
  }

  /* Limited Integrator Block: '<S9>/B_7_2' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->vs_CSTATE <=
            ((Parameters *) ssGetDefaultParam(S))->P_18 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->vs_CSTATE >=
            ((Parameters *) ssGetDefaultParam(S))->P_17 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_20_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_20_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->vs_CSTATE = ((BlockIO *) _ssGetBlockIO
        (S))->B_7_20_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->vs_CSTATE = 0.0;
    }
  }

  /* Limited Integrator Block: '<S6>/B_7_4' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE <=
            ((Parameters *) ssGetDefaultParam(S))->P_24 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->accspeed_CSTATE >=
            ((Parameters *) ssGetDefaultParam(S))->P_23 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_212_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_212_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->accspeed_CSTATE = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_212_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->accspeed_CSTATE = 0.0;
    }
  }

  ((StateDerivatives *) ssGetdX(S))->Wbatt_CSTATE = ((BlockIO *) _ssGetBlockIO(S))
    ->B_7_260_0;

  /* TransferFcn Block: '<S5>/B_7_11' */
  {
    ((StateDerivatives *) ssGetdX(S))->usecondstimeconstant_CSTATE =
      ((Parameters *) ssGetDefaultParam(S))->P_58*((BlockIO *) _ssGetBlockIO(S)
      )->B_7_201_0;
    ((StateDerivatives *) ssGetdX(S))->usecondstimeconstant_CSTATE +=
      (((Parameters *) ssGetDefaultParam(S))->P_57)*((ContinuousStates *)
      ssGetContStates(S))->usecondstimeconstant_CSTATE;
  }

  /* Limited Integrator Block: '<S3>/B_7_14' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m <=
            ((Parameters *) ssGetDefaultParam(S))->P_78 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->Wbatt_CSTATE_m >=
            ((Parameters *) ssGetDefaultParam(S))->P_77 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_155_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_155_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->Wbatt_CSTATE_m = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_155_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->Wbatt_CSTATE_m = 0.0;
    }
  }

  /* Limited Integrator Block: '<S1>/B_7_23' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE <=
            ((Parameters *) ssGetDefaultParam(S))->P_132 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->Chargingenergy_CSTATE >=
            ((Parameters *) ssGetDefaultParam(S))->P_131 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_147_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_147_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->Chargingenergy_CSTATE = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_147_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->Chargingenergy_CSTATE = 0.0;
    }
  }

  /* Limited Integrator Block: '<S1>/B_7_24' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE <=
            ((Parameters *) ssGetDefaultParam(S))->P_136 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->Dicharingenergy_CSTATE >=
            ((Parameters *) ssGetDefaultParam(S))->P_135 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_149_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_149_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->Dicharingenergy_CSTATE = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_149_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->Dicharingenergy_CSTATE = 0.0;
    }
  }

  ((StateDerivatives *) ssGetdX(S))->Dicharingenergy1_CSTATE = ((BlockIO *)
    _ssGetBlockIO(S))->B_7_150_0;
  ((StateDerivatives *) ssGetdX(S))->Integral_CSTATE_o = ((BlockIO *)
    _ssGetBlockIO(S))->B_7_165_0;

  /* Limited Integrator Block: '<S14>/B_7_28' */
  {
    boolean_T lsat;
    boolean_T usat;
    lsat = ( ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i <=
            ((Parameters *) ssGetDefaultParam(S))->P_151 );
    usat = ( ((ContinuousStates *) ssGetContStates(S))->Integral1_CSTATE_i >=
            ((Parameters *) ssGetDefaultParam(S))->P_150 );
    if ((!lsat && !usat) ||
        (lsat && (((BlockIO *) _ssGetBlockIO(S))->B_7_164_0 > 0)) ||
        (usat && (((BlockIO *) _ssGetBlockIO(S))->B_7_164_0 < 0)) ) {
      ((StateDerivatives *) ssGetdX(S))->Integral1_CSTATE_i = ((BlockIO *)
        _ssGetBlockIO(S))->B_7_164_0;
    } else {
      /* in saturation */
      ((StateDerivatives *) ssGetdX(S))->Integral1_CSTATE_i = 0.0;
    }
  }
}

/* Function to initialize sizes */
static void mdlInitializeSizes(SimStruct *S)
{
  /* checksum */
  ssSetChecksumVal(S, 0, 10077064U);
  ssSetChecksumVal(S, 1, 2731240081U);
  ssSetChecksumVal(S, 2, 3987332660U);
  ssSetChecksumVal(S, 3, 414844016U);

  /* options */
  ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);

  /* Accelerator check memory map size match for DWork */
  if (ssGetSizeofDWork(S) != sizeof(D_Work)) {
    ssSetErrorStatus(S,"Unexpected error: Internal DWork sizes do "
                     "not match for accelerator mex file.");
  }

  /* Accelerator check memory map size match for BlockIO */
  if (ssGetSizeofGlobalBlockIO(S) != sizeof(BlockIO)) {
    ssSetErrorStatus(S,"Unexpected error: Internal BlockIO sizes do "
                     "not match for accelerator mex file.");
  }

  /* model parameters */
  _ssSetDefaultParam(S, (real_T *) &rtDefaultParameters);

  /* non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  ((Parameters *) ssGetDefaultParam(S))->P_11 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_13 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_17 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_23 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_39 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_41[6] = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_50[6] = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_55 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_81 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_114 = rtMinusInf;
  ((Parameters *) ssGetDefaultParam(S))->P_131 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_136 = rtMinusInf;
  ((Parameters *) ssGetDefaultParam(S))->P_138 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_142 = rtMinusInf;
  ((Parameters *) ssGetDefaultParam(S))->P_150 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_171 = rtInf;
  ((Parameters *) ssGetDefaultParam(S))->P_223 = rtInf;
}

/* Empty mdlInitializeSampleTimes function (never called) */
static void mdlInitializeSampleTimes(SimStruct *S)
{
}

/* Empty mdlTerminate function (never called) */
static void mdlTerminate(SimStruct *S)
{
}

/* MATLAB MEX Glue */
#include "simulink.c"
