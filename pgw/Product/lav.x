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
  
     File:     lav.x 
  
     Sid:      lav.x@@/main/1 - Tue Aug 17 18:55:17 2010
  
     Prg:      ve
  
*********************************************************************21*/
#ifndef __LAVX__
#define __LAVX__

#ifdef __cplusplus
EXTERN "C" {
#endif



/***********************************************************************
                   PGW General Configuration 
***********************************************************************/


/* PDN-GW's External Gateway Configuration */
/* No need of Transport Address as we use Ethernet interface directly */
typedef CmInetNetAddr AvPgwExGwCfg; 

/* PDN-GW Address Configuration */
/* Can add port too if required */ 
typedef struct avPgwAddrCfg
{
   CmTptAddr    selfAddr;      /* Transport Address of the self */
   
  /* IP Addresses range for UE allocation */
  /* nmb in this structure cannot exceed the the number of UEs 
    given in general config info */
   struct avPgwIpAddrLst 
   {

      U8            nmb;          /* Number of UEs to serve with these 
                                  set of network Addresses. startAddress + nmb
                                  will give the endAddress for PDN-GW to
                                  allocate. */
      CmInetNetAddr startAddr;    /* Starting network address to allocate to UE 
                                    from PDN-GW */
   }pgwNetAddr;

}AvPgwAddrCfg;


/* S-GW Address Configuration */
/* Can remove this later if maintained run time on bearer table per UE */
/* This might be required for path management with S-GW from PDN-GW */

typedef struct avPgwSgwCfg
{
   CmTptAddr    tptAddr;      /* Transport Address of the S-GW */
}AvPgwSgwCfg;

typedef struct
{
   AvPgwExGwCfg exGwCfg;   /* External Gateway details */
   AvPgwSgwCfg  sgwCfg;    /* S-GW details */
}AvPgwPeerCfg;

typedef struct
{
   Pst  lmPst;             /* Post structure for layer manager */
   Mem  mem;               /* Memory pool and region */
   U16  tmrRes;            /* timer resolution */
   U16  maxNmbEgtSaps;     /* Maximum Number of Lower (GTU)SAPs */
   U16  maxNmbTunls;       /* Maximum number of e-gtpu tunnels */
   U16  maxNmbBearers;      /* Maximum number of bearers per UE */
   U16  maxNmbUes;         /* Maximum number of UEs */
   AvPgwAddrCfg pgwAddrCfg;    /* PDN-GW's self details */
}AvPgwGenCfg;

typedef struct avPgwLSapCfg
{
   SuId          suId;           /* E-GTPU  Sap ID */
   SpId          spId;           /* Service Provider SAP Id */
   Selector      selector;       /* Coupling */
   MemoryId      mem;            /* Memory pool */
   Priority      prior;          /* Priority */
   Route         route;          /* Route */
   ProcId        dstProcId;      /* Destination processor id */
   Ent           dstEntId;       /* Destination entity Id */
   Inst          dstInstId;      /* Destination instance id */
   U8            maxBndRetry;    /* Maximum number of bind retries allowed */
   TmrCfg        tBndTmr;        /* Bind timer GTU sap  */
   U32           nmbConn;        /* Number of Bearers through this SAP */
}AvPgwLSapCfg;


/***********************************************************************
                   End of PGW General Configuration 
***********************************************************************/

/***********************************************************************
                   Configuration Structures 
***********************************************************************/

typedef struct avCfg
{
   union
   {
      AvPgwGenCfg   pgwGenCfg;   /* PGW General config */
      AvPgwPeerCfg  peerCfg;     /* All peer details for this PDN-GW */
      AvPgwLSapCfg  pgwLSapCfg;
   }s;
} AvCfg;

/**********************************************************************
 Control Structure Definition
**********************************************************************/


/***********************************************************************
                   Control Structures 
***********************************************************************/

typedef struct avSapCntrl
{
   SpId    id;   /**< SAP identifier. */
} AvSapCntrl;

#ifdef DEBUGP
typedef struct _avDbgCntrl
{
   U32 dbgMask;
}AvDbgCntrl;
#endif /* DEBUGP */
typedef struct avPgwCntrl
{
   DateTime     dt;              /**< Date */
   U8           action;          /**< Action to be performed  Bind, Unbind or shutdown*/
   U8           subAction;       /**< subaction to be performed, SADBGB and SAUSTA */
   union
   {
#ifdef DEBUGP
      AvDbgCntrl   dbg;
#endif /* DEBUGP */
      AvSapCntrl        sap;     /**< The layer manager can selectively
                                      bind/unbind the different SAPs. */
   }u;
}AvPgwCntrl;

typedef struct avCntrl
{
   union
   {
      AvPgwCntrl     pgwCntrl;
   }u;
} AvCntrl;

typedef struct _avUstaDgn
{
   CmAlarm       alarm;
}AvUstaDgn;
/**********************************************************************
 General Structure Definition
**********************************************************************/

typedef struct avMngmt
{
   Header      hdr;           /* header structure */
   CmStatus    cfm;           /* primitive's status confirmation */

   union
   {
      AvCfg     cfg;           /* configuration */
      AvCntrl   cntrl;         /* control */
      AvUstaDgn usta;          /* Alarm */
   } u;
} AvMngmt; 

/**********************************************************************
 Interface Primitives
**********************************************************************/

typedef S16 (*LavCfgReq)   ARGS((Pst     *pst,
                                 AvMngmt *cfg));


typedef S16 (*LavCntrlReq)   ARGS((Pst     *pst,
                                   AvMngmt *cntrl));


typedef S16 (*LavStaReq)   ARGS((Pst     *pst,
                                 AvMngmt *sta));

typedef S16 (*LavStsReq)   ARGS((Pst     *pst,
                                 Action   action,
                                 AvMngmt *sts));

typedef S16 (*LavCfgCfm) ARGS((Pst     *pst,
                               AvMngmt *cfm));


typedef S16 (*LavStsCfm) ARGS((Pst     *pst,
                               AvMngmt *cfm));


typedef S16 (*LavStaCfm) ARGS((Pst      *pst,
                               AvMngmt  *cfm));


typedef S16 (*LavStaInd) ARGS((Pst      *pst,
                               AvMngmt  *usta));


typedef S16 (*LavCntrlCfm) ARGS((Pst      *pst,
                                 AvMngmt  *cfm));


typedef S16 (*LavTrcInd) ARGS((Pst      *pst,
                               AvMngmt  *trc,
                               Buffer   *trcBuf));


#ifdef SM
EXTERN S16 SmMiLavCfgReq ARGS((
   Pst *                pst,
   AvMngmt *            cfg
));

EXTERN S16 SmMiLavCfgCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 SmMiLavStsReq ARGS((
   Pst *                pst,
   Action               action,
   AvMngmt *            sts
));


EXTERN S16 SmMiLavStsCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));

EXTERN S16 SmMiLavStaReq ARGS((
   Pst *                pst,
   AvMngmt *            sta
));


EXTERN S16 SmMiLavStaCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 SmMiLavStaInd ARGS((
   Pst *                pst,
   AvMngmt *            usta
));


EXTERN S16 SmMiLavCntrlReq ARGS((
   Pst *                pst,
   AvMngmt *            cntrl
));


EXTERN S16 SmMiLavCntrlCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 SmMiLavTrcInd ARGS((
   Pst *                pst,
   AvMngmt *            trc,
   Buffer *             trcBuf
));
#endif


EXTERN S16 smAvActvTsk ARGS ((Pst    *pst,
                              Buffer *mBuf
                              ));

EXTERN S16 AvMiLavCfgReq ARGS((Pst     *pst,
                               AvMngmt *cfg));

EXTERN S16 AvMiLavCntrlReq ARGS((Pst     *pst,
                                 AvMngmt *cntrl));

EXTERN S16 AvMiLavStsReq ARGS((Pst     *pst,
                               Action   action,              
                               AvMngmt *sts));

EXTERN S16 AvMiLavStaReq ARGS((Pst     *pst,
                               AvMngmt *sta));

EXTERN S16 avPgwSndActvInit ARGS((Ent entity,
                                  Inst inst, 
                                  Region region,
                                  Reason reason));

EXTERN S16 avPgwSndActvTsk ARGS((Pst     *pst,
                                 Buffer  *mBuf));


EXTERN S16 avPgwRcvActvTsk ARGS((Pst     *pst,
                                 Buffer  *mBuf));

EXTERN Void avSendLmAlarm ARGS((U16 category,
                                U16 event,
                                U16 cause));


#if (defined(LCLAV) || defined(LWLCLAV))


EXTERN S16 cmPkLavCfgReq ARGS((
   Pst *                pst,
   AvMngmt *            cfg
));


EXTERN S16 cmUnpkLavCfgReq ARGS((
   LavCfgReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavCfgCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 cmUnpkLavCfgCfm ARGS((
   LavCfgCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavStsReq ARGS((
   Pst *                pst,
   Action               action,
   AvMngmt *            sts
));


EXTERN S16 cmUnpkLavStsReq ARGS((
   LavStsReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavStsCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 cmUnpkLavStsCfm ARGS((
   LavStsCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavStaReq ARGS((
   Pst *                pst,
   AvMngmt *            sta
));


EXTERN S16 cmUnpkLavStaReq ARGS((
   LavStaReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavStaCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 cmUnpkLavStaCfm ARGS((
   LavStaCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavStaInd ARGS((
   Pst *                pst,
   AvMngmt *            usta
));


EXTERN S16 cmUnpkLavStaInd ARGS((
   LavStaInd            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavCntrlReq ARGS((
   Pst *                pst,
   AvMngmt *            cntrl
));


EXTERN S16 cmUnpkLavCntrlReq ARGS((
   LavCntrlReq          func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavCntrlCfm ARGS((
   Pst *                pst,
   AvMngmt *            cfm
));


EXTERN S16 cmUnpkLavCntrlCfm ARGS((
   LavCntrlCfm          func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLavTrcInd ARGS((
   Pst *                pst,
   AvMngmt *            trc,
   Buffer *             trcBuf
));


EXTERN S16 cmUnpkLavTrcInd ARGS((
   LavTrcInd            func,
   Pst *                pst,
   Buffer               *mBuf
));

#endif



#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __LAVX__ */

/**********************************************************************

         End of file:     lav.x@@/main/1 - Tue Aug 17 18:55:17 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      ve              1. LTE-CNE Initial Release.
*********************************************************************91*/
