/*
 * Copyright 2017-present Open Networking Laboratory
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/********************************************************************20**
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:     C source code for LM Module
  
     File:     lqo.x 
  
     Sid:      lqo.x@@/main/1 - Thu Jul  8 21:25:40 2010
  
     Prg:      ap
  
*********************************************************************21*/
#ifndef __LQOX__
#define __LQOX__

#ifdef __cplusplus
EXTERN "C" {
#endif

/***********************************************************************
                   SGW General Configuration 
***********************************************************************/

typedef struct _qoGenCfg
{
   Pst         lmPst;             /* Post structure for layer manager */
   Mem         mem;               /* Memory pool and region */
   U16         tmrRes;            /* timer resolution */
   U16         maxNmbEgtSaps;     /* Maximum Number of Lower (GTU)SAPs */
   U16         maxNmbBearers;      /* Maximum number of bearers */
   U16         maxNmbTunls;       /* Maximum number of e-gtpu tunnels */
}QoGenCfg;


/***********************************************************************
                   SGW Lower SAP Configuration 
***********************************************************************/

typedef struct _qoLSapCfg
{
   SuId                 suId;               /* E-GTPU  Sap ID */
   SpId                 spId;               /* Service Provider SAP Id */
   Selector             selector;           /* Coupling */
   MemoryId             mem;                /* Memory pool */
   Priority             prior;              /* Priority */
   Route                route;              /* Route */
   ProcId               dstProcId;          /* Destination processor id */
   Ent                  dstEntId;           /* Destination entity Id */
   Inst                 dstInstId;          /* Destination instance id */
   U8                   maxBndRetry;        /* Maximum number of bind retries allowed */
   TmrCfg               tBndTmr;            /* Bind timer GTU sap  */
   CmTptAddr            localAddr;          /* Self Address of the S-GW */
   U32                  nmbTunn;            /* Number of bearers through this SAP */
}QoLSapCfg;


/***********************************************************************
                   Configuration Structures 
***********************************************************************/

typedef struct _qoCfg
{
   union
   {
      QoGenCfg   sgwGenCfg;   /* SGW General config */
      QoLSapCfg  sgwLSapCfg;  /* SGW Lower sap config */
   }s;
} QoCfg;

/**********************************************************************
 Control Structure Definition
**********************************************************************/


/***********************************************************************
                   Control Structures 
***********************************************************************/

typedef struct _qoSapCntrl
{
   SpId    id;   /**< SAP identifier. */
} QoSapCntrl;

#ifdef DEBUGP
typedef struct _qoDbgCntrl
{
   U32 dbgMask;
} QoDbgCntrl;
#endif /* DEBUGP */
typedef struct _qoCntrl
{
   DateTime     dt;              /**< Date */
   U8           action;          /**< Action to be performed  Bind, Unbind or shutdown*/
   U8           subAction;       /**< subaction to be performed, SADBGB and SAUSTA */
   union
   {
#ifdef DEBUGP
      QoDbgCntrl   dbg;
#endif /* DEBUGP */
      QoSapCntrl        sap;     /**< The layer manager can selectively
                                      bind/unbind the different SAPs. */
   }u;
}QoCntrl;

typedef struct _qoUstaDgn
{
   CmAlarm   alarm;
   SpId      sapId;
}QoUstaDgn;
/**********************************************************************
 General Structure Definition
**********************************************************************/

typedef struct _qoMngmt
{
   Header      hdr;           /* header structure */
   CmStatus    cfm;           /* primitive's status confirmation */

   union
   {
      QoCfg     cfg;           /* configuration */
      QoCntrl   cntrl;         /* control */
      QoUstaDgn usta;          /* Alarm */
   } u;
} QoMngmt; 

/**********************************************************************
 Interface Primitives
**********************************************************************/

typedef S16 (*LqoCfgReq)   ARGS((Pst     *pst,
                                 QoMngmt *cfg));


typedef S16 (*LqoCntrlReq)   ARGS((Pst     *pst,
                                   QoMngmt *cntrl));


typedef S16 (*LqoStaReq)   ARGS((Pst     *pst,
                                 QoMngmt *sta));

typedef S16 (*LqoStsReq)   ARGS((Pst     *pst,
                                 Action   action,
                                 QoMngmt *sts));

typedef S16 (*LqoCfgCfm) ARGS((Pst     *pst,
                               QoMngmt *cfm));


typedef S16 (*LqoStsCfm) ARGS((Pst     *pst,
                               QoMngmt *cfm));


typedef S16 (*LqoStaCfm) ARGS((Pst      *pst,
                               QoMngmt  *cfm));


typedef S16 (*LqoStaInd) ARGS((Pst      *pst,
                               QoMngmt  *usta));


typedef S16 (*LqoCntrlCfm) ARGS((Pst      *pst,
                                 QoMngmt  *cfm));


typedef S16 (*LqoTrcInd) ARGS((Pst      *pst,
                               QoMngmt  *trc,
                               Buffer   *trcBuf));


#ifdef SM
EXTERN S16 SmMiLqoCfgReq ARGS((
   Pst *                pst,
   QoMngmt *            cfg
));

EXTERN S16 SmMiLqoCfgCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 SmMiLqoStsReq ARGS((
   Pst *                pst,
   Action               action,
   QoMngmt *            sts
));


EXTERN S16 SmMiLqoStsCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));

EXTERN S16 SmMiLqoStaReq ARGS((
   Pst *                pst,
   QoMngmt *            sta
));


EXTERN S16 SmMiLqoStaCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 SmMiLqoStaInd ARGS((
   Pst *                pst,
   QoMngmt *            usta
));


EXTERN S16 SmMiLqoCntrlReq ARGS((
   Pst *                pst,
   QoMngmt *            cntrl
));


EXTERN S16 SmMiLqoCntrlCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 SmMiLqoTrcInd ARGS((
   Pst *                pst,
   QoMngmt *            trc,
   Buffer *             trcBuf
));
#endif


EXTERN S16 smLqoActvTsk ARGS ((Pst    *pst,
                              Buffer *mBuf
                              ));

EXTERN S16 QoMiLqoCfgReq ARGS((Pst     *pst,
                               QoMngmt *cfg));

EXTERN S16 QoMiLqoCntrlReq ARGS((Pst     *pst,
                                 QoMngmt *cntrl));

EXTERN S16 QoMiLqoStsReq ARGS((Pst     *pst,
                               Action   action,              
                               QoMngmt *sts));

EXTERN S16 QoMiLqoStaReq ARGS((Pst     *pst,
                               QoMngmt *sta));



#ifdef QO_SGW
EXTERN S16 QoActvInit  ARGS((Ent entity,
                                Inst inst, 
                                Region region, 
                                Reason reason));

EXTERN S16 QoActvTsk ARGS((Pst      *pst,
                              Buffer   *mBuf));
#endif


EXTERN Void qoSendLmAlarm ARGS((U16 category,
                                U16 event,
                                U16 cause, 
                                QoUstaDgn *dgn));


#if (defined(LCLQO) || defined(LWLCLQO))


EXTERN S16 cmPkLqoCfgReq ARGS((
   Pst *                pst,
   QoMngmt *            cfg
));


EXTERN S16 cmUnpkLqoCfgReq ARGS((
   LqoCfgReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoCfgCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 cmUnpkLqoCfgCfm ARGS((
   LqoCfgCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoStsReq ARGS((
   Pst *                pst,
   Action               action,
   QoMngmt *            sts
));


EXTERN S16 cmUnpkLqoStsReq ARGS((
   LqoStsReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoStsCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 cmUnpkLqoStsCfm ARGS((
   LqoStsCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoStaReq ARGS((
   Pst *                pst,
   QoMngmt *            sta
));


EXTERN S16 cmUnpkLqoStaReq ARGS((
   LqoStaReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoStaCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 cmUnpkLqoStaCfm ARGS((
   LqoStaCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoStaInd ARGS((
   Pst *                pst,
   QoMngmt *            usta
));


EXTERN S16 cmUnpkLqoStaInd ARGS((
   LqoStaInd            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoCntrlReq ARGS((
   Pst *                pst,
   QoMngmt *            cntrl
));


EXTERN S16 cmUnpkLqoCntrlReq ARGS((
   LqoCntrlReq          func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoCntrlCfm ARGS((
   Pst *                pst,
   QoMngmt *            cfm
));


EXTERN S16 cmUnpkLqoCntrlCfm ARGS((
   LqoCntrlCfm          func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLqoTrcInd ARGS((
   Pst *                pst,
   QoMngmt *            trc,
   Buffer *             trcBuf
));


EXTERN S16 cmUnpkLqoTrcInd ARGS((
   LqoTrcInd            func,
   Pst *                pst,
   Buffer               *mBuf
));

#endif

EXTERN S16 smQoActvTsk ARGS ((Pst    *pst,Buffer *mBuf ));
EXTERN S16 cmPkCmInetNetAddr ARGS ((
CmInetNetAddr *param,
Buffer *mBuf
));
EXTERN S16 cmUnpkCmInetNetAddr ARGS((
CmInetNetAddr *param,
Buffer *mBuf
));


#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __LQOX__ */

/**********************************************************************

         End of file:     lqo.x@@/main/1 - Thu Jul  8 21:25:40 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rk              1. LTE-CNE Initial Release.
*********************************************************************91*/
