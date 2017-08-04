/*
 * Copyright 2017-present Open Networking Foundation
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
  
     File:     lvb.x 
  
     Sid:      lvb.x@@/main/nodal_main/3 - Fri Jan  3 15:04:43 2014
  
     Prg:      ap
  
*********************************************************************21*/
#ifndef __LVBX__
#define __LVBX__

#ifdef __cplusplus
extern "C" {
#endif

#define EVBXXX          1     /* Error Code */

/***********************************************************************
                   MME General Configuration 
***********************************************************************/
/***********************************************************************
                   eNodeB onfiguration 
***********************************************************************/
typedef struct vbEnbCbCfg
{
   U32  peerId;       /* An unique eNb identification with S1AP. */
   U16  lac;          /* LAC */
   U32  rac;          /* RAC */
   U16  t3402Tmr;     /* Timer with UE attach accept - 
                         Can go upto 12 minutes */
   U8   t3412Tmr;     /* Timer for MME UE attach accept */
   U8   t3413Tmr;     /* Page gaurd Timer */
   /* lvb_x_001.main_2: Added 3422 timer */
   U8   t3422Tmr;     /* Timer for MME NW detach request */
   U8   t3450Tmr;     /* Timer for MME UE attach */
   U8   t3460Tmr;     /* Timer for MME UE Authentication */
   U8   t3470Tmr;     /* Timer for MME UE Identification */
   /* lvb_x_001.main_1: Added 3485 timer */
   U8   t3485Tmr;     /* Timer for MME UE Identification */
   U8   t3486Tmr;     /* Timer for MME Modify Bearer Ctxt Req */
/* lvb_x_001.main_4 : Added support for security feature */
   U8   t3489Tmr;     /* Timer for MME ESM Information request */
   /* lvb_x_001.main_2: Added nw initiated detach timer */
   U8   t3495Tmr;    /* Timer for DeActEpsBearCtxtReq  */
   U32  tNwDetachTmr;     /* Timer for Network Detach */ 
   U16  maxUeNum;     /* Maximum no of UEs through this eNodeB */ 
   CmTptAddr eNbAddr; /* Address details of eNodeB */
}VbEnbCbCfg;


typedef struct vbNwId
{
   U8   mcc[LVB_MAX_MCC_LEN];  /* MCC */
   U8   mnc[LVB_MAX_MNC_LEN];  /* MNC */
}VbNwId;

/* MME Group ID  List */
typedef struct vbGrpIdLst
{
   U16      noOfGrpIds; /* Number of GroupIds */
   U16      grpId[LVB_MAX_GRPID_LIST];
}VbGrpIdLst;

/* MME CODE  List */
typedef struct vbMmeCodeLst
{
   U16       noOfCodes; /* Number of MME Codes */
   U8       mmeCode[LVB_MAX_CODE_LIST];
}VbMmeCodeLst;

typedef struct VbPlmnId
{
   U8  plmnId[3];
}VbPlmnId;

/* PLMN List */
typedef struct VbPlmnLst
{
   U8       noOfPlmns; /* Number of plmns */
   VbPlmnId plmnLst[LVB_MAX_PLMN_LIST];
}VbPlmnLst;
typedef struct vbGUMMEI
{
   VbNwId         nwId;           /* Serving Network with PLMN ID */ 
   VbPlmnLst      pLst;           /* PLMN Information */
   VbGrpIdLst     grpIdLst;       /* MME Group ID */ 
   VbMmeCodeLst   codeLst;        /* MME Code */
}VbGUMMEI;


/* Served GUMMEI List */
typedef struct vbSrvdGLst
{
   U8       noOfGummeis; /* Number of GUMMEIs */
   VbGUMMEI gummeiLst[LVB_MAX_SERVED_GUMMEI_LIST];
}VbSrvdGLst;


/* Tracking Area Identity List */
typedef struct vbTai
{
   VbNwId nwId;   /* Network Identification */
   U16    tac;    /* Tracking Area Code */
}VbTai;

typedef struct vbTaiLst
{
   U8    noOfTais;  /* Number of TAIs */
   VbTai taiLst[LVB_MAX_TAI_LIST];
}VbTaiLst;

typedef struct vbMmeName
{
   U16        length;
   Data       name[LVB_MME_MAX_NAME];
}VbMmeName;

typedef enum _vbMmeType
{
   VB_MME_TYPE_INV,
   VB_MME_TYPE_PS,
   VB_MME_TYPE_COM
} VbMmeType;


typedef struct _vbMmeGenCfg
{
   Pst        lmPst;             /* Post structure for layer manager */
   Mem        mem;               /* Memory pool and region */
   U16        maxNmbSztSaps;     /* Maximum Number of Lower(SZT)SAPs */
   U16        maxNmbEgtSaps;     /* Maximum Number of Lower(EGT)SAPs */
   U16        maxNmbEnbs;        /* Maximum number of Enodebs */
   U16        maxNmbUes;         /* Maximum number of Ues */
   U16        tmrRes;            /* Timer resolution */

   CmTptAddr  selfAddr;          /* Transport Address of the self */
   CmTptAddr  s10selfAddr;       /* Transport Address of the self, S10 */
   CmTptAddr  s3selfAddr;        /* Transport Address of the self, S3 */
   CmTptAddr  sgsnAddr;          /* Transport Address of the SGSN */
   CmTptAddr  sgwAddr;           /* Transport Address of the SGW Addr */
   CmTptAddr  pgwAddr;           /* Transport Address of the PGW Addr */

   VbSrvdGLst srvGLst;          /* Served GUMMEI List */

   VbMmeName  mmeName;

   VbTaiLst   tLst;         /* Tracking Area Information to send to UE */

   VbEnbCbCfg eNbCfg[LVB_MAX_ENBS];/* Config for set of eNodeb's */

   U8         tgtMmePlmnId[LVB_MAX_PLMN_ID_LEN]; /* Target MME PLMN ID (HO) */
   CmTptAddr  tgtMmeIpAddr;      /* Target MME IP, Port (HO)*/
   U32        t3412;
 
   U8         sgsnPlmnId[LVB_MAX_PLMN_ID_LEN]; /* Target SGSN PLMN ID (HO) */
   CmTptAddr  sgsnIpAddr;      /* SGSN IP Addr*/
}VbMmeGenCfg;

/***********************************************************************
                   MME Lower SAP Configuration 
***********************************************************************/

typedef struct vbMmeLSapCfg
{
   SuId                 suId;               /* S1AP Sap ID */
   SpId                 spId;               /* Service Provider SAP Id */
   Selector             selector;           /* Coupling */
   MemoryId             mem;                /* Memory pool */
   Priority             prior;              /* Priority */
   Route                route;              /* Route */
   ProcId               dstProcId;          /* Destination processor id */
   Ent                  dstEntId;           /* Destination entity Id */
   Inst                 dstInstId;          /* Destination instance id */
   U8                   maxBndRetry;        /* Maximum number of bind retries allowed */
   TmrCfg               tBndTmr;            /* Bind timer for ap */
   U8                   type;               /* SAP Type */
}VbMmeLSapCfg;


/***********************************************************************
                   Configuration Structures 
***********************************************************************/

typedef struct vbCfg
{
   union
   {
      VbMmeGenCfg   mmeGenCfg;   /* MME General config */
      VbMmeLSapCfg  mmeLSapCfg;  /* MME Lower sap config */
   }s;
} VbCfg;

/**********************************************************************
 Control Structure Definition
**********************************************************************/


/***********************************************************************
                   Control Structures 
***********************************************************************/

typedef struct vbSapCntrl
{
   SpId    id;   /**< SAP identifier. */
} VbSapCntrl;

#ifdef DEBUGP
typedef struct _vbDbgCntrl
{
   U32 dbgMask;
}VbDbgCntrl;
#endif /* DEBUGP */
typedef struct vbMmeCntrl
{
   DateTime     dt;              /**< Date */
   U8           action;          /**< Action to be performed  Bind, Unbind or shutdown*/
   U8           subAction;       /**< subaction to be performed, SADBGB and SAUSTA */
   union
   {
#ifdef DEBUGP
      VbDbgCntrl   dbg;
#endif /* DEBUGP */
      VbSapCntrl        sap;     /**< The layer manager can selectively
                                      bind/unbind the different SAPs. */
   }u;
}VbMmeCntrl;

typedef struct vbCntrl
{
   VbMmeCntrl     mmeCntrl;
} VbCntrl;

typedef struct _vbUstaDgn
{
   CmAlarm       alarm;
}VbUstaDgn;
/**********************************************************************
 General Structure Definition
**********************************************************************/

typedef struct vbMngmt
{
   Header      hdr;           /* header structure */
   CmStatus    cfm;           /* primitive's status confirmation */

   union
   {
      VbCfg     cfg;           /* configuration */
      VbCntrl   cntrl;         /* control */
      VbUstaDgn usta;          /* Alarm */
   } u;
} VbMngmt; 

/**********************************************************************
 Interface Primitives
**********************************************************************/

typedef S16 (*LvbCfgReq)   ARGS((Pst     *pst,
                                 VbMngmt *cfg));


typedef S16 (*LvbCntrlReq)   ARGS((Pst     *pst,
                                   VbMngmt *cntrl));


typedef S16 (*LvbStaReq)   ARGS((Pst     *pst,
                                 VbMngmt *sta));

typedef S16 (*LvbStsReq)   ARGS((Pst     *pst,
                                 Action   action,
                                 VbMngmt *sts));

typedef S16 (*LvbCfgCfm) ARGS((Pst     *pst,
                               VbMngmt *cfm));


typedef S16 (*LvbStsCfm) ARGS((Pst     *pst,
                               VbMngmt *cfm));


typedef S16 (*LvbStaCfm) ARGS((Pst      *pst,
                               VbMngmt  *cfm));


typedef S16 (*LvbStaInd) ARGS((Pst      *pst,
                               VbMngmt  *usta));


typedef S16 (*LvbCntrlCfm) ARGS((Pst      *pst,
                                 VbMngmt  *cfm));


typedef S16 (*LvbTrcInd) ARGS((Pst      *pst,
                               VbMngmt  *trc,
                               Buffer   *trcBuf));


#ifdef SM
EXTERN S16 SmMiLvbCfgReq ARGS((
   Pst *                pst,
   VbMngmt *            cfg
));

EXTERN S16 SmMiLvbCfgCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 SmMiLvbStsReq ARGS((
   Pst *                pst,
   Action               action,
   VbMngmt *            sts
));


EXTERN S16 SmMiLvbStsCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));

EXTERN S16 SmMiLvbStaReq ARGS((
   Pst *                pst,
   VbMngmt *            sta
));


EXTERN S16 SmMiLvbStaCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 SmMiLvbStaInd ARGS((
   Pst *                pst,
   VbMngmt *            usta
));


EXTERN S16 SmMiLvbCntrlReq ARGS((
   Pst *                pst,
   VbMngmt *            cntrl
));


EXTERN S16 SmMiLvbCntrlCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 SmMiLvbTrcInd ARGS((
   Pst *                pst,
   VbMngmt *            trc,
   Buffer *             trcBuf
));
#endif


EXTERN S16 smVbActvTsk ARGS ((Pst    *pst,
                              Buffer *mBuf
                              ));

EXTERN S16 VbMiLvbCfgReq ARGS((Pst     *pst,
                               VbMngmt *cfg));

EXTERN S16 VbMiLvbCntrlReq ARGS((Pst     *pst,
                                 VbMngmt *cntrl));

EXTERN S16 VbMiLvbStsReq ARGS((Pst     *pst,
                               Action   action,              
                               VbMngmt *sts));

EXTERN S16 VbMiLvbStaReq ARGS((Pst     *pst,
                               VbMngmt *sta));


EXTERN S16 vbMmeActvInit ARGS((Ent entity, 
                               Inst inst,  
                               Region region, 
                               Reason reason));

EXTERN S16 vbMmeActvTsk ARGS((Pst      *pst,
                              Buffer   *mBuf));

EXTERN S16  vbMmeParseRdConQ ARGS((Data data));


EXTERN Void vbSendLmAlarm ARGS((U16 category,
                                U16 event,
                                U16 cause)); 

#if (defined(LCLVB) || defined(LWLCLVB))


EXTERN S16 cmPkLvbCfgReq ARGS((
   Pst *                pst,
   VbMngmt *            cfg
));


EXTERN S16 cmUnpkLvbCfgReq ARGS((
   LvbCfgReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbCfgCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 cmUnpkLvbCfgCfm ARGS((
   LvbCfgCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbStsReq ARGS((
   Pst *                pst,
   Action               action,
   VbMngmt *            sts
));


EXTERN S16 cmUnpkLvbStsReq ARGS((
   LvbStsReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbStsCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 cmUnpkLvbStsCfm ARGS((
   LvbStsCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbStaReq ARGS((
   Pst *                pst,
   VbMngmt *            sta
));


EXTERN S16 cmUnpkLvbStaReq ARGS((
   LvbStaReq            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbStaCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 cmUnpkLvbStaCfm ARGS((
   LvbStaCfm            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbStaInd ARGS((
   Pst *                pst,
   VbMngmt *            usta
));


EXTERN S16 cmUnpkLvbStaInd ARGS((
   LvbStaInd            func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbCntrlReq ARGS((
   Pst *                pst,
   VbMngmt *            cntrl
));


EXTERN S16 cmUnpkLvbCntrlReq ARGS((
   LvbCntrlReq          func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbCntrlCfm ARGS((
   Pst *                pst,
   VbMngmt *            cfm
));


EXTERN S16 cmUnpkLvbCntrlCfm ARGS((
   LvbCntrlCfm          func,
   Pst *                pst,
   Buffer               *mBuf
));


EXTERN S16 cmPkLvbTrcInd ARGS((
   Pst *                pst,
   VbMngmt *            trc,
   Buffer *             trcBuf
));


EXTERN S16 cmUnpkLvbTrcInd ARGS((
   LvbTrcInd            func,
   Pst *                pst,
   Buffer               *mBuf
));

#endif



#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __LVBX__ */

/**********************************************************************

         End of file:     lvb.x@@/main/nodal_main/3 - Fri Jan  3 15:04:43 2014

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
    /main/1  ---      rk   1. Updated for release
/main/2      ---      lvb_x_001.main_1  rk   1. Added 3485 timer
/main/3  lvb_x_001.main_2  rk   1. Added 3422 and nwDetach timer
/main/4  lvb_x_001.main_3 ragrawal 1. Added 3495 timer.
/main/5      ---      lvb_x_001.main_4 akaranth 1. Added support for security feature. 
*********************************************************************91*/
