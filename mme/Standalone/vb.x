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
 
     Name:     Trillium LTE CNE - MME
  
     Type:     C include file
  
     Desc:     C structures for CNE node MME 
  
     File:     vb.x 
  
     Sid:      vb.x@@/main/1 - Mon Jun 28 19:35:34 2010
  
     Prg:      sv
  
*********************************************************************21*/
#ifndef __VBX__
#define __VBX__

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************/
/*                MME                                  */
/*******************************************************/

/*******************************************************/
/* EPS Mobility Management Module */
/*******************************************************/

/* VbEmmEvnt EPS Mobility Management Events */
typedef enum vbEmmEvnt
{
   /* EMM Specific Procedures */
   VB_EMM_EVNT_ATTACH_REQ = 0,      /*  UE Attach Request Event */
   VB_EMM_EVNT_ATTACH_ACC,          /*  UE Attach Accept Event */
   VB_EMM_EVNT_ATTACH_COMPL,        /*  UE Attach Complete Event */
   VB_EMM_EVNT_ATTACH_REJ,          /*  UE Attach Complete Event */
   VB_EMM_EVNT_UE_DETACH_REQ,       /*  UE - N/w Detach Request Event */
   VB_EMM_EVNT_NW_DETACH_REQ,       /*  Network - UE Detach Request Event */
   VB_EMM_EVNT_NW_DETACH_ACC,       /*  Network - UE Detach Accept Event */
   VB_EMM_EVNT_UE_DETACH_ACC,       /*  UE - N/w Detach Accept Event */
   VB_EMM_EVNT_TA_UPD_REQ,              /*  Tracking Area Update Events */
   VB_EMM_EVNT_TA_UPD_ACC,              /*  Tracking Area Update Events */
   VB_EMM_EVNT_TA_UPD_CMP,              /*  Tracking Area Update Events */
   VB_EMM_EVNT_TA_UPD_REJ,              /*  Tracking Area Update Events */

   /* EMM Connection Management Procedures */
   VB_EMM_EVNT_SRVCE_PROC,          /*  Service Request Procedural Events */
   VB_EMM_EVNT_PAGING,              /*  Paging Event */
   VB_EMM_EVNT_NAS_TRANPORT,        /*  UL and DL NAS Transport events */

   /* EMM Common Procedures */
   VB_EMM_EVNT_COMMON_PROC,         /*  Common Procedural events */

   /* EMM General */
   VB_EMM_EVNT_STATUS,              /*  EMM Status */
   VB_EMM_EVNT_INFO,                /*  EMM Information */
   VB_EMM_EVNT_CS_NOTIFY,   /*15*/   /*  CS Service Notification */
   VB_EMM_EVNT_MAX = 0x26,           /* KWORKS Fix To handle max Events*/
   VB_EMM_EVNT_INVALID = 0xff       /* Invalid Event */

}VbEmmEvnt;

/* VbEmmState EPS Mobility Management States */
typedef enum vbEmmState
{
   VB_EMM_UE_DEREGISTERED = 0,  /* UE Deregistered */
   VB_EMM_UE_COMN_PROC_INIT,    /* UE Common Procedure Initiated */
   VB_EMM_UE_REG_INIT,          /* UE Registration Initiated/In progress */
   VB_EMM_UE_REGISTERED,        /* UE Registered */
   VB_EMM_UE_DEREG_INIT         /* UE Deregister Initiated */
}VbEmmState;

typedef enum vbEmmS1apConState
{
   VB_EMM_UE_S1AP_INIT = 0,  /* UE Deregistered */
   VB_EMM_UE_S1AP_CONNECTED    /* UE Common Procedure Initiated */
}VbEmmS1apConState;

typedef enum vbMmeEnbState
{
   VB_MME_ENB_UP=0,
   VB_MME_ENB_DOWN
}VbMmeEnbState;

/* VbEsm of EMM common procedures */
typedef enum vbEmmCommProcType
{
   VB_EMM_COMM_PROC_IDLE = 0,  /* Common procedure initiated */
   VB_EMM_COMM_PROC_INIT        /* Common procedure complete */
}VbEmmCommProcSubState;


/* EMM Common procedure in progress */
typedef enum vbEmmCommPrcInPrg
{
   VB_EMM_COM_PRC_NONE = 0x0,
   VB_EMM_COM_PRC_GUTI_RALC_CMD = 0x1,
   VB_EMM_COM_PRC_GUTI_RALC_CMP = 0x2,
   VB_EMM_COM_PRC_AUTH_REQ = 0x4,
   VB_EMM_COM_PRC_AUTH_RSP = 0x8,
   VB_EMM_COM_PRC_AUTH_REJ = 0x10,
   VB_EMM_COM_PRC_AUTH_FAIL = 0x20,
   VB_EMM_COM_PRC_ID_REQ = 0x40,
   VB_EMM_COM_PRC_ID_RSP = 0x80,
   VB_EMM_COM_PRC_SEC_MOD_CMD = 0x100,
   VB_EMM_COM_PRC_SEC_MOD_CMP = 0x200,
   VB_EMM_COM_PRC_SEC_MOD_REJ = 0x400,
   VB_EMM_COM_PRC_TAU_ACC = 0x800
}VbEmmCommPrcInPrg;

/* vb001.101: Added ECM State */
typedef enum vbEcmState
{
   VB_ECM_IDLE = 0,          /* UE ECM IDLE */
   VB_ECM_CONNECTED          /* UE ECM_CONNECTED */
}VbEcmState;

/* EMM common procedure events */
typedef enum vbEmmCmnPrcEvt
{
   VB_EMM_EVNT_CMN_PRC_GUTI_RLC = 0,
   VB_EMM_EVNT_CMN_PRC_AUTH_NRML,
   VB_EMM_EVNT_CMN_PRC_AUTH_ABNRML,
   VB_EMM_EVNT_CMN_PRC_ID,
   VB_EMM_EVNT_CMN_PRC_SEC_MOD_NRML,
   VB_EMM_EVNT_CMN_PRC_SEC_MOD_ABNRML,
   VB_EMM_EVNT_CMN_PRC_ATTACH_ACC,
   VB_EMM_EVNT_CMN_PRC_ATTACH_CMP,
   VB_EMM_EVNT_CMN_PRC_TAU_CMP,
   VB_EMM_EVNT_CMN_PRC_INVALID
}VbEmmCmnPrcEvt;

/* emm timers at network side */
typedef enum vbEmmTmr
{
   VB_TMR_EMM_3413 = 1,
   VB_TMR_EMM_3422,
   VB_TMR_EMM_3450,
   VB_TMR_EMM_3460,
   VB_TMR_EMM_3470,
   VB_TMR_EMM_MOB_REACHABLE,
   VB_TMR_EMM_IMPLI_DTCH_TMR,
   /* +--- Patch 1.0.3 ---+ */
   VB_TMR_UE_CTX_DEL,
   /* ---- Patch 1.0.3 ---- */
   VB_TMR_INT_MGMT,
   VB_TMR_SZT_EVT,
   VB_TMR_EMM_IRAT_UE_CTXT_REL,
   VB_EMM_MAX_TIMERS  /* Should be last element */
}VbEmmTmr;

typedef enum vbEsmTmr
{
   VB_TMR_ESM_3485 = VB_EMM_MAX_TIMERS,
   VB_TMR_ESM_3486,
   VB_TMR_ESM_3489,
   VB_TMR_ESM_3495,
   VB_ESM_MAX_TIMERS,
   VB_MME_MAX_TMRS
}VbEsmTmr;


typedef enum vbMmeProc
{
   VB_MME_NULL_PROC,
   VB_MME_ATTACH_PROC,
   VB_MME_DETACH_UE_PROC,
   VB_MME_DETACH_NW_PROC,
   VB_MME_PAGING_PROC,
   VB_MME_SERVICE_REQ_PROC,
   VB_MME_CTXT_REL_PROC,
   VB_MME_TAU_PROC,
   VB_MME_IMPLICIT_DETACH_PROC,
   VB_MME_GUTI_REALLOC_PROC,
   VB_MME_UE_CTXT_MOD_PROC,
   VB_MME_ESM_PROC,

   VB_MME_PDN_CONN_PROC,
   VB_MME_PDN_DIS_CONN_PROC,
   VB_MME_ACT_DEF_BEAR_PROC,
   VB_MME_ACT_DED_BEAR_PROC,
   VB_MME_DEACT_DEF_BEAR_PROC,
   VB_MME_DEACT_DED_BEAR_PROC,
   VB_MME_MOD_BEAR_PROC,
   VB_MME_RES_ALLOC_PROC,
   VB_MME_RES_MOD_PROC,
   VB_MME_BEAR_REL_PROC,
   VB_MME_BEAR_REL_IND_PROC,
   VB_MME_HANDOVER_PROC,
   VB_MME_INVALID_PROC
}VbMmeProc;


/* VbEsmState EPS Session Management States */
typedef enum vbEsmState
{
   VB_ESM_ST_BEARER_INACTIVE     = 0,     /* Bearer Inactive */
   VB_ESM_ST_BEARER_ACT_PEND,             /* Bearer Active Pending */
   VB_ESM_ST_BEARER_ACTIVE,               /* Bearer Active */
   VB_ESM_ST_BEARER_INACT_PEND,           /* Bearer Inactive pending  */
   VB_ESM_ST_BEARER_MOD_PEND,             /* Bearer Mod Pending */
   VB_ESM_ST_PROC_TRANS_PEND,             /* Procedure Transaction Pending */
   VB_ESM_ST_PROC_TRANS_INACTIVE          /* Procedure Transaction Inactive */
}VbEsmState;

/* GTP-C Message Reponses */
typedef enum vbGtpcMsg
{
   VB_EGTPC_CREATE_SESSION_RESPONSE = 0,  /* Create Sesion Response*/
   VB_EGTPC_MODIFY_BEARER_RESPONSE,       /* Modify Bearer Response */
   VB_EGTPC_DELETE_SESSION_RESPONSE,      /* Delete Session Response*/
   /* vb001.101: Added Create Bearer Req message */
   VB_EGTPC_CREATE_BEARER_REQ,             /* Create Bearer Req*/
   VB_EGTPC_RELEASE_ACCESS_BEARER_RESPONSE,
   VB_EGTPC_DWN_LNK_DATA_NOTIFY,
   VB_EGTPC_DELETE_BEARER_REQ,
   VB_EGTPC_UPDATE_BEARER_REQ,
   VB_EGTPC_BEARER_RESOURCE_FAIL_IND
}VbGtpcMsg;

typedef enum _VbRatType
{
   VB_RAT_RESERVED      = 0,  /* reserved */
   VB_RAT_UTRAN,              /* UTRAN */               
   VB_RAT_GERAN,              /* GERAN */               
   VB_RAT_WLAN,               /* WLAN */               
   VB_RAT_GAN,                /* GAN */               
   VB_RAT_HSPAE,              /* HSPA Evolution  */               
   VB_RAT_EUTRAN,             /* EUTRAN  */               
   VB_RAN_INVALID
} VbRatType;

typedef enum _VbMmeHoMmeType
{
   VB_MME_HO_INV_MME,
   VB_MME_HO_SRC_MME,
   VB_MME_HO_TGT_MME

} VbMmeHoMmeType;
#ifdef X2_HO
typedef enum _VbMmeHoType
{
  VB_MME_S1_HO,
  VB_MME_X2_HO
}VbMmeHoType;
#endif
/* Forward Declaration */
typedef struct vbMmeEnbCb VbMmeEnbCb;
typedef struct vbMmeUeCb VbMmeUeCb;

/* ESM Module */
typedef struct vbMmeEsmCb VbMmeEsmCb;
/* vb001.101: Added ECM control block */
typedef struct vbMmeEcmCb VbMmeEcmCb;

typedef struct vbBearerIdLst VbBearerIdLst;

/* HSS Module */
/* typedef struct vbHssUeInfo VbHssUeInfo; */

/* TODO: START: Confirm if these typedefs are correct */
typedef U32 Teid;

/** MS ISDN : UE Number */
typedef U8  Msisdn[16];
/* TODO: END: Confirm if these typedefs are correct */

/* Bearer Id list */
struct vbBearerIdLst
{
   U8 nmbBearerId;
   U8 bearerId[CM_ESM_MAX_BEARER_ID];
};

/* Tracking Area Identity of the UE */
typedef struct _vbUeTai
{
   VbPlmnId             plmnId;     /* Plmn ID, BCD format */
   U16                  tac;        /* Tracking Area Code */
} VbUeTai;

/* EUTRAN CGI */
typedef struct _vbEutranCgi
{
   VbPlmnId             plmnId;     /* Plmn ID, BCD format */
   U32                  cellId;     /* Cell Identity */
} VbEutranCgi;

typedef struct vbMmeUeTrnList
{
   struct
   {
      U8          status;
      U8          trnId;
      VbMmeEsmCb  *esmCb;
   }esmInfo[CM_ESM_MAX_BEARER_ID];
}VbMmeUeTrnList;

typedef struct vbMmeDelEsmCbList
{
   U8          count;
   struct
   {
      VbMmeEsmCb  *esmCb;
   }esmInfo[CM_ESM_MAX_BEARER_ID];
}VbMmeDelEsmCbList;

typedef struct vbMmeEsmPiggyAct
{
   U8      evnt;
   U8      bId;
   U8      tId;
   U8      cause;  /* cause for reject */
   U8      status; /* bearer setup tatus */ 
}VbMmeEsmPiggyAct;

typedef struct vbMmeEmmFlrInd
{
   U8     trg;
   U8     cause;
   Ptr    *evntPtr;
}VbMmeEmmFlrInd;

typedef struct vbMmeSpndEsmProcInfo
{
   VbMmeEsmCb   *esmCb;
   CmNasEvnt    *evnt;
   U8           msgType;
}VbMmeSpndEsmProcInfo;
typedef struct vbMmeSpndEmmProcInfo
{
   VbMmeUeCb    *ueCb;
   CmNasEvnt    *evnt;
   U8           msgType;
}VbMmeSpndEmmProcInfo;

typedef struct VbMmeS1apEvnt
{
   union 
   {
      SztDatEvntInd pdu;
   }evnt;
}VbMmeS1apEvnt; 
typedef struct vbMmeSpndS1apProcInfo
{
   VbMmeUeCb      *ueCb;
   VbMmeS1apEvnt  s1apEvnt;
   S1apPdu        *s1apPdu;
   U8             msgType;
}VbMmeSpndS1apProcInfo;

typedef struct vbMmeSpndProc
{
   U8     choice;
   union  _proc
   {
      VbMmeSpndEsmProcInfo  esmProc;
      VbMmeSpndEmmProcInfo  emmProc;
      VbMmeSpndS1apProcInfo s1apProc; 
   }proc;
}VbMmeSpndProc;


/*******************************************************/
/* End Of EPS Mobility Management Module */
/*******************************************************/

/*******************************************************/
/* MME Controller Module */
/*******************************************************/

/* S1AP SAP control block */

typedef struct vbMmeSztSapCb        /* VB Lower SAP Control Block */
{
   Pst           pst;               /* Service user post structure */
   State         state;             /* State of this SAP */
   SuId          suId;              /* Service User SAP id */
   SpId          spId;              /* Service Provider Id */
   U8            maxBndRetry;       /* maximum Bind Retry counter */
   U8            bndRetryCnt;       /* Bind retry count */
   TmrCfg        tBndTmr;           /* Bind Timer */
   CmTimer       timers[VB_MME_MAX_TMRS];/* Interface timer control blocks
                                        one for each running timer */
}VbMmeSztSapCb;


/* The following structure represents a control tunnel on MME for an
   UE on S11 interface with S-GW. An UE can map to a EGTP-C tunnel */
typedef struct vbMmeUeTunnelCb
{
   CmHashListEnt ueTunHashEnt;  /* Hashlist manipulator */
   Teid          localTeIdKey;  /* Local Tunnel ID for this tunnel */
   Teid          remoteTeId;    /* Remote Tunnel ID mapped to this tunnel */
   TknU32        transId;

   CmTptAddr     locAddr;      /* Local Tunnel Trasnport Address details */
   CmTptAddr     remAddr;      /* Remote Tunnel Trasnport Address details */

   U8            bearerId;     /* Linked EPS Bearer ID */
   U32           seqNum;       /* Sequence number for this tunnel */ 
   CmInetNetAddr ipAddr;       /* IP Address allocated to this UE */

   VbMmeUeCb     *ueCb;        /* Reference to UE Cb to which this tunnel
                                  belongs to. */
}VbMmeUeTunnelCb;

/* eGTPC - SAP control block */
typedef struct vbMmeEgtSapCb
{
   Pst           pst;            /* Service user post structure */
   State         state;          /* State of this SAP */
   SuId          suId;           /* Service user SAP id */
   SpId          spId;           /* Service Provider Id */
   U32           nmbConn;        /* Number of Bearers through this SAP */
   U8            maxBndRetry;       /* maximum Bind Retry counter */
   U8            bndRetryCnt;       /* Bind retry count */
   TmrCfg        tBndTmr;           /* Bind Timer */
   CmTimer       timers[VB_MME_MAX_TMRS];/* Interface timer control blocks */
   CmHashListCp  ueTunnelCbHlCp; /* Hash List of EGTP control tunnel blocks on
                                    S11 interface blocks for the UE involved.*/
   CmHashListCp  tunnPendCbHlCp; /* Hash List of EGTP control tunnel temporary on
                                    control block.*/
}VbMmeEgtSapCb;


/*  This structure represents EMM Context Control block for an UE */
typedef struct vbMmeEmmCb
{

   VbEmmState             state;       /* State of this UE involved in EMM
                                          procedure. */

   VbEmmCommProcSubState  emmCommProcSubState;
                                /* Sub-State of this UE within an EMM 
                                   common procedure. */

   VbEmmCommPrcInPrg      emmCommProcInProgress; 
                                 /* The bit array to indicate the common proc 
                                    that is going on. Each bit represents one
                                    message of the EMM common procedures */

   SztNAS_PDU             retrBuf;   /* The buffer to retransmit in case timer
                                        expires */

   CmTimer                emmTimers[VB_EMM_MAX_TIMERS];      /* EMM Timers */
   U8                     emmTimersCntr[VB_EMM_MAX_TIMERS];  
                                 /* EMM Timers Counters to track number of
                                    expiries. */
   Bool                   esmPiggyBacked;  /* Flag to indicate if Default EPS
                                       bearer needs to be established with ESM
                                       piggybacked on EMM message */

   VbMmeEsmPiggyAct       piggyAction; /* Piggy back info */
  
   VbMmeUeCb              *ueCb;     /* UE CB reference for this UE involved
                                        in EMM procedure. */
   U32                    cause;
   U32                    typeOfId;

}VbMmeEmmCb;

/* vb001.101: Added ECM control block */
struct vbMmeEcmCb
{
   VbEcmState  state;
};

typedef struct vbMmePdnCb
{
   U8            id;
   VbMmeEsmCb    *esmCb;
   Teid          s5PgwTeId;     /* GTP-Teid used by PGW on S5 interface
                                 * This is required during HO/TAU with
                                 * SGW change and Multiple PDN Connections */

   CmInetNetAddr s5PgwAddr;     /* Transport Layer Address used by PGW */
} VbMmePdnCb;

/*  This structure represents ESM Context Control block for an UE 
 ESM Context can be for default EPS bearer or dedicated EPS Bearer */
struct vbMmeEsmCb
{
   CmEsmBearerId     bId;        /* EPS bearer identity */
   CmEsmProcTxnId    tId;        /* Procedure transaction identity */
   VbEsmState        state;      /* State of this UE involved in ESM 
                                    procedure. */
   VbEsmState        prevstate;  /* State of this UE involved in ESM 
                                    procedure. */
   U8            bearerType;     /* EPS Bearer Type: Default OR Dedicated */
   U8            pdnType;        /* type of pdn IPV4/IPV6/IPV4V6 */
   CmEsmAccessPtName apn;        /* Access Point Name for the bearer */
   CmEsmProtCfgOpt   pco;        /* Protocol configuration option */
   CmEsmPktFlowId    pfi;        /* Packet Flow Identifier for the bearer */
   CmEsmTft          tft;        /* vb001.101: TFT Information recvd*/
   CmEsmEpsQos       epsQos;     /* vb001.101: Eps Qos as received */

   CmTimer       esmTimers[VB_ESM_MAX_TIMERS];  /* ESM Timers */
   U8            esmTimersCntr[VB_ESM_MAX_TIMERS];  /* ESM Timers Counters to 
                                                       track number of 
                                                       expiries. */
   Bool          esmPiggyBacked; /* Flag to indicate if EPS
                                    bearer is to be established with ESM
                                    piggybacked on EMM message */

   SztNAS_PDU             retrBuf;   /* vb001.101: The buffer to retransmit in case timer
                                        expires */

   CmInetNetAddr s1SgwAddr;     /* Transport Layer Address used by S-GW on
                                   S1-U interface for this E-RAB/EPS Bearer
                                   Fill this from Bearer Context IE received
                                   from Create Session Response from EGTPC */

   Teid          s1SgwTeId;     /* GTP-Teid used by S-GW on S1-U interface
                                   for this E-RAB / EPS bearer.
                                   Fill this from Bearer Context IE received
                                   from Create Session Response from EGTP */

   CmInetNetAddr s1EnbAddr;     /* Transport Layer Address used by eNodeB on
                                   S1-U interface for this E-RAB/EPS Bearer*/

   Teid          s1EnbTeId;     /* GTP-Teid used by eNodeB on S1-U interface
                                   for this E-RAB / EPS bearer */

   CmEsmBearerId  linkBearId;   /* vb001.101: Linked Bearer ID of the Default Bearer */
   VbMmeUeCb     *ueCb;         /* UE reference for this ESM procedure. */

   VbMmePdnCb     *pdnCb;       /* PDN Control block in case of multiple
                                 * PDN connections */
   U8             proc;
   TknU32         tranId;
   Bool           pendingRsp;
   Bool           infoTransFlg;

   Teid           s5PgwUTeid;
   CmInetNetAddr  s5PgwUAddr;

   Teid           s4SgwUTeid;
   CmInetNetAddr  s4SgwUAddr;
   Teid           rncDlTeId;     /* RNC DL GTP-Teid used by eNodeB on i-RAT HO */
   CmInetNetAddr  rncIpAddr;     /* RNC Transport Layer Address */
};



typedef struct vbMmeUeCtxt
{
   UConnId suConnId;
   UConnId spConnId;
}VbMmeUeCtxt;

#if 0
#ifdef VB_MME_NAS_SEC
/* MME Authentication Vector */
typedef struct vbMmeAuthVec
{
   U8 rndNmb[VB_RAND_SIZE];
   U8 autn[VB_AUTN_SIZE];
   U8 xres[VB_RES_SIZE];
   U8 kasme[VB_KASME_SIZE];
} VbMmeAuthVec;
#endif /* VB_MME_NAS_SEC */
#endif

typedef struct vbMmeUeInfo
{

   U8            ueImsi[CM_EMM_MAX_MOBILE_ID_DIGS];    /* IMSI of UE */
   U8            ueImsiLen;
   GUTI          ueGuti;        /* Guti allocated to this UE */
   Msisdn        ueIsdn;        /* Msisdn of UE. Retrieved from HSS */
   SztCSG_Id     csgId;         /* Cell Global identity of UE */
   CmTimer       cellIdElpsdTmr;   /* Cell Identity Age since last acquired */
   U32           cellIdCntr;    /* Cell Identity Age tracker */
   CmEmmTaiLst   taList;        /* Tracking Area List for this UE */
   CmEmmUeRadCapInfoUpdNdd ueRadiocap; /* Radio capability of UE */
   CmEmmUeNwCap  ueNwCap;      /* Network capability of UE */
   CmEmmUeSecCap ueSecCap;     /* UE security capability */
   CmInetNetAddr selfAddr;     /* MME IP address for S11 for this UE. */
   VbUeTai       tai;          /* Tracking Area Identity of the UE */ 
   VbEutranCgi   eCgi;         /* eutran cgi of the UE. req to form the uli
                                * along with tai */
   U8            srvPlmn[3];
                  
   /* To Fill PDN details 
   1. APN in Use
   2. The subscribed APN received from HSS 
   3. PDN-GW IP address currently in use */
   VbHssUeInfo   ueHssCtxt;   /* All UE information from HSS */
   VbEmmS1apConState s1apConState; /* S1AP Connection State*/

#ifdef VB_MME_NAS_SEC
   U8              numOfQuin;
   VbMmeAuthVec    authVec;
   VbHssUeAuthInfo authInfo;
   VbMmeSecCtxt    secCtxt;
   VbMmeSecCtxt    mappedSecCtxt;
   Bool            resync;
   CmEmmNasKsi     nasKsi;
#endif

   /*Tracking area type*/
   U8              tauActv;
   U8              tauType;
   U8              gutiAlloc;
   U8              suspendPaging;

}VbMmeUeInfo;

typedef struct _VbMmeGlbEnbId
{
   SztENB_ID            enbId;
   VbPlmnId             plmnId;
} VbMmeGlbEnbId;

typedef struct _VbMmeTgtRncId
{
   SztLAI               laiId;
   SztRAC               rac;
   SztRNC_ID            rncId;
   SztExtendedRNC_ID    extndRncId;
} VbMmeTgtRncId;

/* Handover Target details */
typedef struct _VbMmeHoTgt
{
   VbMmeGlbEnbId  gEnbId;        /**< Target eNB ID         */
   CmTptAddr      mmeTptAddr;    /**< Target MME IP, Port */
   VbMmeTgtRncId  tgtRncId;      /**< Target RNC ID        */
   CmTptAddr      sgsnTptAddr;    /**< Target SGSN IP, Port */

} VbMmeHoTgt;

/**< E-UTRAN Transparent Containter */
typedef struct _VbMmeEUTransCont
{
   U32         len;     /**< Length of the buffer  */
   U8          *pVal;   /**< Buffer pointer        */
} VbMmeEUTransCont;

/**< E-UTRAN Transparent Containter */
typedef struct _VbMmeHoCause
{
   U8      causeType;    /**< Cause Type */
   U32     cause;        /**< Cause - */
} VbMmeHoCause;



/* Used to store S1 Handover related data
 * The data is recived either from Forward Relocation Request if MME is
 * relocated during handover or Handover Perparation message is MME is not
 * relocated
 */
typedef struct vbMmeHoData
{
   U32               bitmask;       /**< Events during Handover         */
   VbMmeEUTransCont  tgtToSrcTCont; /**< E-UTRAN Transparent Container  */
   VbMmeEUTransCont  srcToTgtTCont; /**< E-UTRAN Transparent Container  */
   VbMmeEUTransCont  staTfrTCont;   /**< eNB Status Transfer Trans      
                                         Container */

   VbMmeUeCb         *pNewUeCb;
   VbMmeUeCb         *pOldUeCb;
   VbMmeEnbCb        *pTgtEnbCb;    /**< Target eNodeB CB               */
   VbMmeEnbCb        *pSrcEnbCb;
   UConnId           oldsuConnId;   /**< Service User Connection Identifier
                                     * with S1AP, used in HO            */

   UConnId           oldspConnId;   /**< Service Provider Connection 
                                     * Identifier with S1AP, used in HO */

   VbMmeHoTgt        hoTgt;         /**< HO target details              */

   VbMmeHoMmeType    hoMmeType;     /**< Tells whether this MME is source
                                     * or target MME in case of HO with
                                     * MME relocation                   */

   U32               mmeUeS1apId;   /**< MME UE S1AP ID                 */
   U32               enbUeS1apId;   /**< eNodeB UE S1AP ID              */

   CmInetNetAddr     ueAddr;        /**< IP Address allocated for the UE*/

   CmInetNetAddr     dlTransportLyrAddr;
   U32               dlGtpTeid;
   CmInetNetAddr     ulTransportLyrAddr;
   U32               ulGtpTeid;
   VbMmeHoCause      hoCause;
   U32               hoType;
   
   Teid              sgwTeId;    /* S-GW Tunnel ID mapped to this tunnel */
   CmTptAddr         sgwAddr;    /* S-GW Tunnel Trasnport Address details */

} VbMmeHoData;

/*  This structure represents UE Control block in eNodeB's scope on
    S1 Interface.*/
struct vbMmeUeCb
{
   CmHashListEnt ueHashEnt;     /* Hashlist manipulator */
   CmHashListEnt ueSTmsiHashEnt;/* vb001.101: Hashlist manipulator */
   CmHashListEnt ueImsiHashEnt; 
   U32           bitmask;       /* Contains the details of TAU/HO 
                                 * MME/SGW change  */
   U32           ueCbKey;       /* UE control block key */
   U8            ueSTmsiCbKey[5];  /* vb001.101: UE control block key */
   U32           eNbUeS1apId;   /* eNB S1AP UE Identifier from eNodeB for
                                   this UE. */
   U32           mMeUeS1apId;   /* MME S1AP UE Identifier allocated from MME  
                                   for this UE. */
   UConnId       suConnId;      /* Service User Connection Identifier
                                   with S1AP */
   UConnId       spConnId;      /* Service Provider Connection Identifier 
                                   with S1AP */

   VbMmeEnbCb   *eNodebCb;      /* eNodeB reference for this UE involved
                                   in EMM-ESM procedure. */

   VbMmeUeInfo   ueCtxt;        /* Addtional UE Information for this UE */
   VbMmeEmmCb    emmCb;         /* EMM Context for this UE */
   VbMmeEcmCb    ecmCb;         /* vb001.101: ECM Context for this UE */
   VbMmeUeTunnelCb *ueTunnelCb; /* UE control tunnel Cb reference */

   VbMmeEsmCb    *esmList[CM_ESM_MAX_BEARER_ID];/* List of esm Cbs */
   VbBearerIdLst  creBid;   /* Bearer ids to create session */

/* vb003.101: Added UeRa Capability Oct Str */
#ifdef UE_RAD_CAP
   TknStrOSXL    ueCapbInfo; /* Ue Capability info */
#endif

   VbMmeProc         proc;
   SztCause          relCause;
   VbMmeUeTrnList    esmTrnList;
   U8                lstMsgHdrType;
   VbMmeDelEsmCbList esmDelList;

   VbMmeSpndProc     spndProc[VB_MME_MAX_SPND_PROC];
   U32               transCntr;
   VbMmePdnCb        *pdnList[VB_MME_MAX_PDN_CONN];
   U8                errIndCauseType;

   /* Temporary solution gtp stack issue*/
   TknU32               transId;

   VbMmeUeTunnelCb   *pUeS10TunCb;  /**< UEs' S10 tunnel CB             */
  
   VbMmeUeTunnelCb   *pUeS3TunCb;   /* UE's S3 Tunnel CB */
   
   VbMmeHoData       *pHoData;      /**< S1 Handover related data       */
   Bool              rxd_erab_rsp; /* this flag is to track the reception of erab setup rsp as part of mPDN */
   /* this flag is to keep track of the reception of nas actvt dflt beare accp for mPDN - the reason for maintaining
      the above 2 flags is becuase the transmissoin of modify from the MME is depenedant on the reception 
      of both the messages where in one is coming from eNodeB and the other comes from the UE. These messages are not
      sequential and can come at any order as seen - the modify shall be sent only on reception of both for optimisation
      of not needing to send delete towards SGW */
   Bool              rxd_dflt_bearer_acc;
#ifdef X2_HO
   VbMmeHoType       hoType;
   UConnId           pthSwthSpConnId;  /* Service Provider Connection Identifier
                                          with S1AP - temporary suconid stored
                                          when recieving path switch for the UE.
                                          Updates the spConnId after resception of 
                                          successful Modify Bearer Response from SGW.
                                         */
   Bool           pthSwtchReq;
#endif

};

typedef struct _enbName
{
   TknPres         pres;
   U8              len;
   U8              name[VB_ENB_NAME_LEN];
}EnbName;



typedef struct _supportedTAItem
{
    U16             tac;
    U8              nmbOfPlmn;
    U8              plmnId[VB_PLMN_LIST_LEN][VB_PLMN_ID_LEN];
}SupportedTAItem;

typedef struct _supportedTAs
{
    U8               nmbOfTAs;
    SupportedTAItem  supportedTAItem[VB_TAC_LIST_LEN];
}SupportedTAs;

typedef struct _enbStupInfo
{
    VbMmeGlbEnbId  gEnbId;
    EnbName      enbName;
    SupportedTAs supportedTAs;
    U32          dfltPgingDRX;
}EnbStupInfo;


/*  This structure represents a eNodeB on S1 Interface for this MME. 
    MME can have multiple eNodeBs connected. The maximum number is
    configured from layer manager. */
struct vbMmeEnbCb
{
   CmHashListEnt  eNbHashEnt;   /* eNodeB Hashlist Entry */
   U32            peerId;       /* An unique eNb identification with S1AP. */
                                /* An unique eNb identification-Global eNB ID */
   VbMmeEnbState  state;        /* State of this eNodeB on S1 Interface */
   VbEnbCbCfg     eNbCfg;       /* All configuration related to this eNodeB. */
   VbMmeSztSapCb *sztSapCb;     /* Upper S1AP SAP reference for this eNodeB. */
   CmHashListCp   ueCbHlCp;     /* Hash List of UE Control Context involved on
                                   S1 with this eNodeB. */
   U32            ueConnIdCntr; /* Global UE service user connection Id 
                                 * allocated
                                 * for every UE to map to S1AP's conCb;*/
		
   EnbStupInfo   enbStupInfo;
   U32           globeNbId;
   Bool          isSecUeAllowed; /* If TRUE, secondary UEs will be allowed
                                  * on this eNB. Default value = FALSE
                                  */
};

/* VB MME Global Control Block */
typedef struct _vbMmeCb
{
   TskInit         init;               /* Task initialization structure */
   VbMmeGenCfg     mmeCfg;             /* MME's configuration */
   VbMmeSztSapCb   *sztSapCb;          /* Upper S1AP SAP with this MME for
                                          S1 interface */
   VbMmeEgtSapCb   *egtSapCb;          /* Upper EGTPC SAP with this MME
                                          for S11 interface - NonDefault */
   VbMmeEgtSapCb   *egtEchoSapCb;      /* Upper EGTPC SAP with this MME for 
                                          S11 interface - Default */
   VbMmeEgtSapCb   *egtS10SapCb;       /* EGT Sap for S10 interface */
   VbMmeEgtSapCb   *egtS3SapCb;        /* EGT Sap for S3 interface */
   CmHashListCp    eNbLst;             /* Hash list of eNodeB control blocks */
   CmTqCp          vbTqCp;             /* Timing queue control point */
   CmTqType        vbTq[VB_MME_TQSIZE]; /* Timing queue */
   U32             transIdCntr;         /*counter for sending the TunnelReq*/
   U32             gutiCntr;            /*counter for GUTI */
   U32             debugLevel;          /* Debug Level */
   CmHashListCp   ueSTmsiCbHlCp; 
   CmHashListCp   ueImsiCbHlCp; 
   U32            s10TransIdCntr;
   U32            s3TransIdCntr;
}VbMmeCb;

/**< Used for Update Location Request and Response message to HSS */
typedef struct _VbMmeHssUsrData
{
   VbMmeEnbCb     *pEnbCb;
   VbMmeUeCtxt    ueCtx;
   CmNasEvnt      *pNasEvnt;
} VbMmeHssUsrData;

EXTERN VbMmeCb vbMmeCb;

/*******************************************************/
/* End of MME Controller Module */
/*******************************************************/


/* MME Event Structure --> Moved to EMM/ESM event as NAS event */

/*******************************************************/
/*               End of MME                            */
/*******************************************************/

/*******************************************************/
/* MME Function declarations */
/*******************************************************/

/* vb006.101: Test Code for OverLoading Feature */
#ifdef VB_TEST_CODE
EXTERN S16 vbMmeUtlBldMmeCfgUpd ARGS((VbMmeEnbCb  *eNbCb,
                                      S1apPdu     **mmeCfgUpdPduPtr));

EXTERN S16 vbMmeFillRstMsg ARGS((U32         conId,
                                 S1apPdu     **rstPdu));

EXTERN S16 vbMmeFillOvldStopMsg ARGS((VbMmeEnbCb  *eNbCb,
                                      S1apPdu     **ovldPdu));

EXTERN S16 vbMmeTstSndRst ARGS((VbMmeEnbCb   *eNbCb)); 
#endif
EXTERN S16 vbMmeHdleNBUpd ARGS((SztUDatEvnt    *uDatEvnt));
EXTERN S16 vbMmeSndPathSwtchAck ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeInit ARGS((U16 nmbSztSaps));

EXTERN S16 vbMmeSndUeCtxtRelCmd ARGS (( VbMmeUeCb   *ueCb,
                                         U8           choice,
                                         U8           cause));
EXTERN S16 vbMmeSndUeCtxtModReq ARGS (( VbMmeUeCb     *ueCb,
                                       CmNasEvnt     *vbEvnt,
                                       SztNAS_PDU    *nasPdu,
                                       Bool     nasPduPres));



/* MME Controller's protoypes */
EXTERN S16 vbMmeHdlIncSetup ARGS((SztUDatEvnt    *uDatEvnt));
EXTERN S16 vbMmeHdlIncReset ARGS((SztUDatEvnt    *uDatEvnt));
EXTERN S16 vbMmeHdlIncResetAck ARGS((SztUDatEvnt    *uDatEvnt));

EXTERN S16 vbMmeHdlInitialCtxtSetupRsp ARGS((VbMmeUeCb      *ueCb,
                                             SztDatEvntInd  *datEvnt));

EXTERN S16 vbMmeHdlUeCtxtModRsp ARGS((VbMmeUeCb      *ueCb,
                                             SztDatEvntInd  *datEvnt));
/* vb001.101: Added functions */
EXTERN S16 vbMmeHdlERABSetupRsp ARGS((VbMmeUeCb      *ueCb,
                                      SztDatEvntInd  *datEvnt));
EXTERN S16 vbMmeHdlERABModRsp ARGS((VbMmeUeCb      *ueCb,
                                      SztDatEvntInd  *datEvnt));
EXTERN S16 vbMmeHdlERABRlsRsp ARGS((VbMmeUeCb      *ueCb,
                                      SztDatEvntInd  *datEvnt));
EXTERN S16 vbMmeHdlERABRelInd ARGS((VbMmeUeCb      *ueCb,
                                      SztDatEvntInd  *datEvnt));
EXTERN S16 vbMmeHdlIncErrIndMsg ARGS((VbMmeUeCb      *ueCb,
                                      Ptr            *ptr,
                                      U8             msgType));
EXTERN S16 vbMmeHdlIncNASNonDlvryIndMsg ARGS((VbMmeUeCb      *ueCb,
                                              Ptr            *ptr,
                                              U8             msgType));
/* vb003.101: Added functions */
#ifdef UE_RAD_CAP
EXTERN S16 vbMmeHdlUeCapInfoInd ARGS((VbMmeUeCb      *ueCb,
                                      SztDatEvntInd  *datEvnt));
#endif
EXTERN S16 vbMmeHdlIncUeConMsg ARGS((SztConInd    *conEvnt,
                                     U8           msgType));

/* MME EMM-ESM Encoder Decoder Module */
EXTERN S16 vbMmeEdmDecode ARGS((SztNAS_PDU  *nasPdu,
                                CmNasEvnt **mmeEvnt));

EXTERN S16 vbMmeEdmEncode ARGS((CmNasEvnt *mmeEvnt,
                                SztNAS_PDU *nasPdu));

/* EMM Module Prototypes */


EXTERN S16 vbMmeEmmHdlOutUeEvnt ARGS((VbMmeEnbCb *enbCb,
                                      VbMmeUeCb   *ueCb,
                                      VbMmeUeCtxt *ueCtxt,
                                      CmNasEvnt   *evnt));

EXTERN S16 vbMmeEmmHdlIncUeEvnt ARGS((VbMmeEnbCb *enbCb,
                                      VbMmeUeCtxt *ueCtxt,
                                      CmNasEvnt   *evnt));

EXTERN S16 vbEmmHdlUeTmrEvnt ARGS((VbMmeUeCb *enbCb,
                                   U8        tmrEvntType));

/* MME LI Module Prototypes */
EXTERN S16 vbMmeLimSndToLower ARGS((VbMmeUeCb     *ueCb,
                                    CmNasEvnt     *vbEvnt,
                                    SztNAS_PDU    *nasPdu,
                                    U8             msgType, 
                                    Bool           nasMsgPres));

EXTERN S16 vbMmeLimSndStoredBuf ARGS((VbMmeUeCb     *ueCb,
                                    U8             msgType));

EXTERN S16 vbMmeLimSndEsmStoredBuf ARGS((VbMmeEsmCb     *esmCb,
                                    U8             msgType));
/* ESM Module Prototypes */

EXTERN S16 vbEsmHdlPiggyBackIncUeEvnt ARGS((VbMmeUeCb   *ueCb,
                                            CmNasEvnt   *evnt));

EXTERN S16 vbEsmHdlRollBack ARGS((VbMmeUeCb   *ueCb, 
                                  VbMmeEsmCb   *esmCb,
                                  U16          msgType));

EXTERN S16 vbEsmHdlIncUeEvnt ARGS((VbMmeEnbCb  *eNodebCb, 
                                   VbMmeUeCtxt *ueCtxt,
                                   CmNasEvnt   *evnt ));

EXTERN S16 vbEsmHdlOutUeEvnt ARGS((VbMmeEsmCb  *ueCb, 
                                   CmNasEvnt   *evnt ));

EXTERN S16 vbEsmHdlUeEvnt ARGS((VbMmeUeCb   *ueCb,
                                CmNasEvnt   *evnt,
                                U8          esmMsgType,
                                U8          evntDirectn));
EXTERN S16 vbMmeIncDwnLnkDatNot  ARGS
((
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
));
EXTERN S16 vbMmeHdlOutPagingReq ARGS
((
VbMmeUeCb  *ueCb
));

/* vb005.101: Fix for ccpu00117480 */
EXTERN S16 vbMmeUtlChar2BCD ARGS ((U8 *imsi, U8 imsiLen, U32 *imsiInt));



/* ESM Encoder Function */
EXTERN S16 vbEsmEncMsg ARGS((CmNasEvnt *evnt, U8 **buf, U16 *totLen));

/* ESM Decoder Function */
EXTERN S16 vbEsmDecMsg ARGS((U8 *buf, CmNasEvnt **evnt, U32 totLen));


/* S1AP PDU building Prototypes */
EXTERN S16 vbMmeUtlBldS1SetupRsp ARGS ((VbMmeEnbCb *eNbCb, S1apPdu **pdu));

EXTERN S16 vbMmeUtlBldS1SetupFail ARGS ((VbMmeEnbCb *eNbCb,
                                         S1apPdu **pdu,
                                         U8 choice,
                                         U8 cause));

EXTERN S16 vbMmeUtlBldS1ResetAck ARGS ((U8 flag,SztUDatEvnt *uDatEvnt, S1apPdu **pdu));
EXTERN S16 vbMmeUtlBldS1Reset ARGS ((S1apPdu **pdu, SztCause *cause, U8 flag));
/*changed the signature to include nas pdu */
EXTERN S16 vbMmeUtlBldInitCxtSetupReq ARGS ((VbMmeUeCb *ueCb,
                                             SztNAS_PDU *sztNAS_PDU,
                                             S1apPdu **pdu));
EXTERN S16 vbMmeUtlBldUeCxtModReq ARGS ((VbMmeUeCb *ueCb,
                                             SztNAS_PDU *sztNAS_PDU,
                                             S1apPdu **pdu));
EXTERN S16 vbMmeUtlBldERABSetReq  ARGS ((VbMmeEsmCb *esmCb,
                                             SztNAS_PDU *sztNAS_PDU,
                                             S1apPdu **pdu));

EXTERN S16 vbMmeUtlBldERABModReq  ARGS ((VbMmeEsmCb *esmCb,
                                             SztNAS_PDU *sztNAS_PDU,
                                             S1apPdu **pdu));
EXTERN S16 vbMmeUtlBldERABRlsCmd  ARGS ((VbMmeEsmCb *esmCb,
                                             SztNAS_PDU *sztNAS_PDU,
                                             S1apPdu **pdu));

EXTERN S16 vbMmeUtlBldDlNasTransport ARGS ((VbMmeUeCb *ueCb,
                                            SztNAS_PDU *sztNAS_PDU,
                                            S1apPdu **pdu));


EXTERN S16 vbMmeUtlBldUeCxtRlsCmd ARGS ((VbMmeUeCb *ueCb,
                                         S1apPdu **pdu,
                                         U8 choice,
                                         U8 cause));

EXTERN S16 vbMmeUtlGetS1apIE ARGS((SztS1AP_PDU *pdu, U32 id, TknU8   **ie));

EXTERN S16 vbMmeAppInit ARGS(( Void ));

EXTERN S16 vbMmeRcvSgwMsg ARGS((VbMmeEgtSapCb *egtSapCb, 
                                VbMmeUeCb     *ueCb,
                                VbGtpcMsg      msgType,
                                EgMsg         *egMsg,
                                TknU32        *transId));

EXTERN S16 vbMmeEmmMaxTmrExpInd ARGS((VbMmeUeCb *ueCb,
                                      S16       tmrEvnt));

EXTERN S16 vbMmeEsmMaxTmrExpInd ARGS((VbMmeEsmCb *ueCb,
                                      S16       tmrEvnt));

EXTERN S16 vbMmeEgtpErrInd ARGS((VbMmeUeCb *ueCb,EgErrEvnt *egErrEvnt));
EXTERN S16 vbMmeLimSndS1APMsg ARGS((VbMmeEnbCb  *eNbCb, 
                                    VbMmeUeCb   *ueCb,
                                    S1apPdu     *spdu,
                                    U8           msgType ));

EXTERN S16 vbMmeHdlIncUeRelMsg ARGS((VbMmeUeCb *ueCb,
                                     Ptr       *ptr,
                                     U8        msgType));

EXTERN S16 vbMmeRcvEmmMsg ARGS(( VbMmeUeCb  *ueCb, 
                                 CmNasEvnt    *evnt,
                                 U8          emmMsgType ));

EXTERN S16 vbMmeRcvEsmMsg ARGS((VbMmeEsmCb  *esmCb,
                                CmNasEvnt   *evnt,
                                U8          esmMsgType));

EXTERN S16 vbMmeUtlBldAttachAccept ARGS ((VbMmeUeCb *ueCb, CmNasEvnt **evnt));

EXTERN S16 vbMmeUtlBldAttachReject ARGS ((VbMmeUeCb *ueCb,
                                          CmNasEvnt  **evnt,
                                          U8         emmCause,
                                          VbMmeEsmCb *esmCb));

/* Moving auth code under VB_MME_AUTH flag */
#ifdef VB_MME_AUTH
EXTERN S16 vbMmeUtlBldAuthRequest ARGS ((VbMmeEmmCb *ueEmmCb, 
                                         CmNasEvnt **evnt));

/* VB_MME_AUTH Authentication feature added 
 * Function to build Authentication Reject message added*/
EXTERN S16 vbMmeUtlBldAuthReject ARGS((
         VbMmeEmmCb  *ueEmmCb,
         CmNasEvnt   **evnt
         ));
#endif /* VB_MME_AUTH */

EXTERN S16 vbMmeUtlBldIdentityReq ARGS(( VbMmeEmmCb  *ueEmmCb, 
                                         CmNasEvnt   **evnt,
                                         U8          type));

EXTERN S16 vbMmeUtlBldDetachAcc ARGS(( VbMmeEmmCb  *ueEmmCb, 
                                         CmNasEvnt   **evnt));

EXTERN S16 vbMmeUtlBldDetachReq ARGS((VbMmeEmmCb  *ueEmmCb,    
                                      CmNasEvnt  **evnt, 
                                      U8         detachType,
                                      U16        emmCause));

/* ESM Messages building Prototypes */
EXTERN S16 vbMmeUtlBldDfltActvBearerReq ARGS ((VbMmeEsmCb *esmCb,
                                               CmNasEvnt  **evnt));

EXTERN S16 vbMmeUtlBldPdnConnReject ARGS ((VbMmeEsmCb  *ueCb,
                                           CmNasEvnt   **evnt,
                                           U8          cause));
EXTERN S16 vbMmeUtlBldPdnDisConnReject ARGS ((VbMmeEsmCb  *ueCb,
                                           CmNasEvnt   **evnt,
                                           U8          cause));

EXTERN S16 vbMmeUtlBldEgtCrtBearRsp ARGS (( VbMmeUeTunnelCb *ueTunlCb, 
                                            VbMmeEsmCb     *esmCb, 
                                            EgMsg           **egMsg,
                                            U8              status,
                                            U8              cause));
EXTERN S16 vbMmeUtlBldEgtDelBearRsp ARGS (( VbMmeUeTunnelCb *ueTunlCb, 
                                            VbMmeEsmCb     *esmCb, 
                                            EgMsg           **egMsg,
                                            U8              status,
                                            U8              cause));
EXTERN S16 vbMmeUtlBldEgtUpdBearRsp ARGS (( VbMmeUeTunnelCb *ueTunlCb, 
                                            VbMmeEsmCb     *esmCb, 
                                            EgMsg           **egMsg,
                                            U8              status,
                                            U8              cause));

/* EGTPC Messages building Prototypes */

/* MME to S-GW */
EXTERN S16 vbMmeCreateTunnelReq ARGS((VbMmeUeCb    *ueCb));/* UMAPATHI*/

EXTERN S16 vbMmeUtlBldCSReq ARGS ((VbMmeUeTunnelCb *ueTunlCb,
                                          VbMmeEsmCb      *esmCb,
                                          EgMsg **egMsg));

EXTERN S16 vbMmeUtlBldDSReq ARGS ((VbMmeUeTunnelCb *ueTunlCb,
                                          VbMmeEsmCb       *esmCb,
                                          EgMsg           **egMsg, 
                                          U8               cause));

EXTERN S16 vbMmeUtlBldMBReq  ARGS ((VbMmeUeTunnelCb  *ueTunlCb,
                                              VbMmeEsmCb       *esmCb,
                                              EgMsg            **egMsg,
                                              Bool             isRelease));

EXTERN S16  vbMmeUtlBldActvDedBearerReq ARGS (( VbMmeEsmCb  *ueEsmCb,
                                                CmNasEvnt   **evnt
                                              ));
EXTERN S16  vbMmeUtlBldDeActvDedBearerReq ARGS ((VbMmeEsmCb  *ueEsmCb,
                                                 CmNasEvnt   **evnt,
                                                 CmEsmCause   cause));
EXTERN S16  vbMmeUtlBldDeActvDefBearerReq ARGS ((VbMmeEsmCb  *ueEsmCb,
                                                 CmNasEvnt   **evnt,
                                                 CmEsmCause   cause));
EXTERN S16  vbMmeUtlBldModBearerCtxtReq ARGS ((VbMmeEsmCb  *ueEsmCb,
                                               CmNasEvnt   **evnt,
                                               CmEsmCause   cause));
EXTERN S16 vbMmeUtlBldEgtBearResCmd ARGS
((
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg,
Bool            isRelease));

EXTERN S16 vbMmeUtlBldEgtModifyBearCmd ARGS
((
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg));

EXTERN S16 vbMmeUtlBldEgtDeleteBearCmd ARGS
((
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg));

/*MME ESM GTP-C Module*/
EXTERN S16 vbMmeIncCSRsp  ARGS ((CmTptAddr   *localAddr,
                                         CmTptAddr   *remAddr, 
                                         TknU32       teid,
                                         TknU32       transId,
                                         EgMsg       *egMsg));

EXTERN S16 vbMmeIncMBRsp  ARGS ((CmTptAddr   *localAddr,
                                           CmTptAddr   *remAddr, 
                                           TknU32       teid,
                                           TknU32       transId, 
                                           EgMsg       *egMsg));

EXTERN S16 vbMmeIncDSRsp  ARGS ((CmTptAddr   *localAddr,
                                         CmTptAddr   *remAddr, 
                                         TknU32       teid,
                                         TknU32       transId,
                                         EgMsg       *egMsg));

EXTERN S16 vbMmeSndCSReq  ARGS ((VbMmeUeCb    *ueCb,
                                             VbMmeEsmCb   *esmCb));


EXTERN S16 vbMmeSndS1Reset ARGS((VbMmeEnbCb *enbCb,
                                 SztCause    *cause,
                                 U8          flag));


EXTERN S16 vbMmeSendCreateBearerCmd  ARGS ((VbMmeEsmCb    *esmCb,
                                            Bool          isRelease));
EXTERN S16 vbMmeSendModifyBearerCmd  ARGS ((VbMmeEsmCb    *esmCb));
EXTERN S16 vbMmeSendDeleteBearerCmd  ARGS ((VbMmeEsmCb    *esmCb));
EXTERN S16 vbMmeSndCBRsp  ARGS ((VbMmeEsmCb    *esmCb,
                                            U8            status,
                                            U8            cause ));
EXTERN S16 vbMmeSendDeleteBearerRsp  ARGS ((VbMmeEsmCb    *esmCb,
                                            U8            status,
                                            U8            cause ));

EXTERN S16 vbMmeSndMBReq  ARGS ((VbMmeUeCb    *ueCb,
                                            VbMmeEsmCb   *esmCb,
                                            Bool isRelease));
EXTERN S16 vbMmeSndMBReqiRAT  ARGS ((VbMmeUeCb    *ueCb,
                                            VbMmeEsmCb   *esmCb,
                                            Bool isRelease));

EXTERN S16 vbMmeSndDSReq ARGS ((VbMmeUeCb    *ueCb, 
                                            VbMmeEsmCb   *esmCb,
                                            U8   cause));
EXTERN S16 vbMmeSndRABReq ARGS ((VbMmeUeCb* ueCb));
EXTERN S16 vbMmeUtlDelPdnConn ARGS((VbMmeUeCb  *ueCb,
                                       U8         cause));
EXTERN S16 vbMmeUtlModPdnConn ARGS((VbMmeUeCb  *ueCb,
                                       U8      lastPdnId));

EXTERN S16 vbMmeUtlRelBearer ARGS((VbMmeUeCb  *ueCb,
                                   U8         bId,
                                   TknU32     *transId));

EXTERN S16 vbMmeIncCreateBearerReq   ARGS ((CmTptAddr   *localAddr, 
                                            CmTptAddr   *remAddr,
                                            TknU32       teid,
                                            TknU32       transId,
                                            EgMsg       *egMsg));

EXTERN S16 vbMmeIncDeleteBearerReq   ARGS ((CmTptAddr   *localAddr, 
                                            CmTptAddr   *remAddr,
                                            TknU32       teid,
                                            TknU32       transId,
                                            EgMsg       *egMsg));

/* Timer Related Function */
EXTERN S16  vbMmeActvTmr ARGS((Void));
EXTERN Void vbMmeTmrEvnt ARGS((PTR cb, S16 event));
EXTERN S16  vbMmeRegInitTmr ARGS((Void));
EXTERN Void vbMmeStartTmr ARGS((PTR cb, S16 event, U32 delay));
EXTERN Void vbMmeStopTmr ARGS((PTR cb, S16 event));
EXTERN Void vbMmeEmmHdlTmrEvnt ARGS((VbMmeUeCb *ueCb, S16 event));
EXTERN Void vbMmeEsmHdlTmrEvnt ARGS((VbMmeEsmCb *esmCb, S16 event));
EXTERN Void vbMmeStopTmr ARGS((PTR cb, S16 event));
/* EXTERN S16  vbMmeParseRdConQ ARGS((Data data)); */

/*******************************************************/
/* End of MME Function declarations                    */
/*******************************************************/

/* MME-HSS Function Prototypes */
EXTERN S16 vbMmeHssULReqCmd ARGS((VbMmeHssULRCmd *ulrCmd, 
                                  VbHssUeInfo    *ueInfo));

/* vb005.101: Added support for multiple Attach and Detach ccpu00117537
 */
EXTERN S16 vbMmeHssReset ARGS((VbMmeHssULRCmd *ulrCmd));

/*******************************************************/
/* All Utilities                                       */
/*******************************************************/
PUBLIC S16 vbUtlEgtAddUli ARGS((EgMsg            *egMsg,
                                EgIeLevelInfo    *egLvlInfo,
                                U8               inst,
                                VbUeTai          *tai,
                                VbEutranCgi      *eCgi));

PUBLIC S16 vbUtlGetS1APTaId ARGS((SztS1AP_PDU       *pS1apPdu,
                                  U8                msgType,
                                  VbUeTai             *tai));

PUBLIC S16 vbUtlGetS1APeCgi ARGS((SztS1AP_PDU       *pS1apPdu,
                                  U8                msgType,
                                  VbEutranCgi       *eCgi));

PUBLIC S16 VbUtlAddIndType ARGS((EgMsg            *egMsg,
                                 EgIeLevelInfo    *egLvlInfo,
                                 U8               inst));


EXTERN S16 vbUtlIpv4InetToArr ARGS((U8           *addrArr,   
                                    CmInetIpAddr *address));

EXTERN S16 vbUtlProtoCfgOptToArr ARGS((U8               *protCfgArr,     
                                       U8               *len,
                                       CmEsmProtCfgOpt  *protCfgOpt));

EXTERN S16 vbUtlArrToProtoCfgOpt ARGS((U8               *protCfgArr,     
                                       U8               len,
                                       CmEsmProtCfgOpt  *protCfgOpt));


EXTERN S16 vbMmeUtlInitUeTnlCbs ARGS((VbMmeEgtSapCb *egtSapCb,
                                      U16         nmbBins,
                                      U16         offset,
                                      Bool        dupFlg,
                                      U16         keyType));

EXTERN S16 vbMmeUtlInitENodeBs ARGS((U16          nmbBins,
                                     U16          offset,
                                     Bool         dupFlg,
                                     U16          keyType));

EXTERN S16 vbMmeUtlInitUeCbs ARGS((VbMmeEnbCb   *enbCb, 
                                   U16          nmbBins,
                                   U16          offset,
                                   Bool         dupFlg,
                                   U16          keyType));

EXTERN S16 vbMmeUtlInitSTmsiUeCbs ARGS((U16          nmbBins,
                                   U16          offset,
                                   Bool         dupFlg,
                                   U16          keyType));


EXTERN S16 vbMmeUtlAddEnbCb ARGS((U32           peerId,
                                  VbMmeEnbCb    **enbCb));

EXTERN S16 vbMmeUtlFndEnbCb ARGS((VbMmeEnbCb    **enbCb,
                                  U32           peerId));

EXTERN S16 vbMmeUtlDelEnbCb ARGS((VbMmeEnbCb    *enbCb));


EXTERN S16 vbMmeUtlAllocGuti ARGS((GUTI *guti));


EXTERN S16 vbMmeUtlAddUeCb ARGS((VbMmeEnbCb    *enbCb, 
                                 VbMmeUeCtxt   *ueCtxt,
                                 VbMmeUeCb     **ueCb));

EXTERN S16 vbMmeUtlFndUeCb ARGS((VbMmeEnbCb   *enbCb, 
                                 VbMmeUeCb    **ueCb,
                                 U32           ueId));

EXTERN S16 vbMmeUtlFndSTmsiUeCb ARGS((VbMmeEnbCb   *enbCb, 
                                 VbMmeUeCb    **ueCb,
                                 U32           STmsiId));


EXTERN S16 vbMmeUtlDelUeCb ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeUtlDelPndTrns ARGS((VbMmeUeCb    *ueCb));


EXTERN S16 vbMmeUtlInitImsiUeCbs ARGS((U16          nmbBins,
                                       U16          offset,
                                       Bool         dupFlg,
                                       U16          keyType));

EXTERN S16 vbMmeUtlAddUeCbOnConnId ARGS((U32           connId,
                                         VbMmeEnbCb    *enbCb,
                                         VbMmeUeCb     *ueCb));

EXTERN S16 vbMmeUtlAddUeCbOnSTmsi ARGS((U8            *stmsi,
                                        VbMmeUeCb     *ueCb));

EXTERN S16 vbMmeUtlAddUeCbOnImsi  ARGS((U8            *imsi,
                                        U8            imsiLen,
                                        VbMmeUeCb     *ueCb));


EXTERN S16 vbMmeUtlFndUeCbOnEvent ARGS((VbMmeEnbCb    *enbCb,
                                        VbMmeUeCb     **ueCb,
                                        U32           suConnId,
                                        U32           spConnId,
                                        CmNasEvnt     *evnt));

EXTERN S16 vbMmeUtlFndUeCbOnConnId ARGS((VbMmeEnbCb    *enbCb,
                                         VbMmeUeCb    **ueCb,
                                         U32            ueId));

EXTERN S16 vbMmeUtlFndUeCbOnSTmsi ARGS((U8           *stmsi,
                                        VbMmeUeCb    **ueCb));

EXTERN S16 vbMmeUtlFndUeCbOnImsi ARGS((U8           *imsi,
                                       U8           len,
                                       VbMmeUeCb    **ueCb));


EXTERN S16 vbMmeUtlDelUeCbOnConnId ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeUtlDelUeCbOnSTmsi ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeUtlDelUeCbOnImsi ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeUtlVldS1SetupPlmn ARGS((SupportedTAs *tAs));



EXTERN S16 vbMmeUtlAddEsmCb ARGS((VbMmeUeCb    *ueCb,
                                  VbMmeEsmCb   **esmCb,
                                  U8           ueEsmBId,
                                  U8           ueEsmTId));


EXTERN S16 vbMmeUtlFndEsmCb ARGS((VbMmeUeCb    *ueCb,
                                  VbMmeEsmCb   **esmCb,
                                  U8           ueEsmBId,
                                  U8           ueEsmTId));

EXTERN S16 vbMmeUtlDelEsmCb ARGS((VbMmeEsmCb   *esmCb,
                                  VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeUtlAllocConId ARGS((VbMmeEnbCb  *eNbCb,
                                    UConnId     *newSuConnId));

EXTERN S16 vbMmeUtlFreeConId ARGS((VbMmeEnbCb  *eNbCb));

EXTERN S16 vbMmeUtlBldEgtDwnLnkNotAck ARGS((VbMmeUeTunnelCb *ueTunlCb,
                                            EgMsg           **egMsg,
                                            U8              casue));
EXTERN S16 vbMmeUtlBldEgtDwnLnkNotFailInd ARGS((VbMmeUeTunnelCb *ueTunlCb,
                                                EgMsg           **egMsg,
                                                U8              cause));

EXTERN S16 vbMmeUtlBldS1PagingReq ARGS((VbMmeEnbCb  *eNbCb,
                                        VbMmeUeCb   *ueCb,
                                        Bool        sTmsiBased,
                                        S1apPdu     **s1PagingReqPduPtr));

EXTERN S16 VbMmeLiEgtBndCfm ARGS((Pst    *pst,
                                  SuId    suId,
                                  U8      status));

EXTERN S16 VbMmeLiEgtLclDelCfm ARGS ((Pst         *pst,      
                                      SuId         suId,     
                                      U32          transId, 
                                      U32          teid,   
                                      CmStatus     status));

EXTERN S16 VbMmeLiEgtSigCfm ARGS ((Pst         *pst,         /* post structure */
                                   SuId         suId,        /* Use values from EgwNodeSapId */
                                   CmTptAddr   *localAddr,   /* Local node address */
                                   CmTptAddr   *remAddr,     /* Remote node address */
                                   TknU32       teid,        /* */
                                   TknU32       transId,     /* */
                                   EgMsg       *egMsg));     /* structure for carrying eGTP MSG info */

EXTERN S16 VbMmeLiEgtSigInd ARGS ((Pst         *pst,        /* post structure */   
                                   SuId         suId,       /* Use values from EgwNodeSapId */
                                   CmTptAddr   *localAddr,  /* Local node address */
                                   CmTptAddr   *remAddr,    /* Remote node address */
                                   TknU32       teid,       /* */
                                   EgMsg       *egMsg));    /* structure for carrying eGTP MSG info */



EXTERN S16 vbUtlFillTknStrOSXL ARGS((TknStrOSXL *ptr,     /* Pointer to the structure */ 
                                     U16         len,     /* length to be filled */
                                     Data       *val,     /* byte to be repetitively filled in */
                                     S1apPdu    **pdu));

EXTERN S16 vbUtlFillTknStrOSXL2 ARGS((TknStrOSXL *ptr,     /* Pointer to the structure */ 
                                     U16         len,     /* length to be filled */
                                     Data       *val,     /* byte to be repetitively filled in */
                                     S1apPdu    **pdu));

                                     
EXTERN S16 vbUtlFillTknStrOSXL1 ARGS((
TknStrOSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val,     /* value to be repetitively filled in */
S1apPdu    **pdu
));

EXTERN S16 vbUtlFillTknStrBSXL ARGS((TknStrBSXL *ptr,     /* Pointer to the structure */
                                     U16         len,     /* length to be filled */
                                     Data       *val,     /* byte to be repetitively filled in */
                                     S1apPdu    **pdu));

EXTERN S16 vbUtlFillTknStr4 ARGS((TknStr4 *ptr,    /* Pointer to the structure */
                                  U8      len,     /* length to be filled */
                                  U8      val));   /* byte to be repetitively filled in */

EXTERN S16 vbUtlFill16TknStr4 ARGS((TknStr4 *ptr,    /* Pointer to the structure */
                                  U8      len,     /* length to be filled */
                                  U16     val));   /* byte to be repetitively filled in */

EXTERN S16  vbUtlFillTknU8  ARGS ((TknU8  *encParam,
                                   U8     val));


EXTERN S16  vbUtlFillTknU16  ARGS ((TknU16  *encParam,
                                    U16     val));

EXTERN S16  vbUtlFillTknU32  ARGS ((TknU32  *encParam,
                                    U32     val));

EXTERN S16 vbUtlFillTknBStr32 ARGS ((TknBStr32  *ptr,     /* Pointer to the structure */
                                     U16         len,     /* length to be filled */
                                     Data       *val));   /* value to be filled in */

EXTERN S16 vbUtlFillTknBStr32WithVal ARGS ((TknBStr32  *ptr,     /* Pointer to the structure */
                                     U16         len,     /* length to be filled */
                                     U32         val));   /* value to be filled in */

EXTERN S16 vbUtlItoa ARGS((U8 *str,U32 num));
EXTERN S16 vbUtlMccMncToPlmn ARGS((U8 *str, CmEmmTai *ptai));

EXTERN S16 vbUtlIpv4InetToArr ARGS((U8           *addrArr,   /* ascii address representation */
                                    CmInetIpAddr *address)); /* 4 byte interent address */

EXTERN S16 vbUtlIpv4ArrToInet ARGS((U8           *addrArr,   /* ascii address representation */
                                    CmInetIpAddr *address)); /* 4 byte interent address */

EXTERN S16 vbUtlEgtAddPdnType ARGS((EgMsg         *egMsg, 
                                    EgIeLevelInfo *egLvlInfo,
                                    U8             inst));

EXTERN S16 vbUtlEgtAddSelMode ARGS((EgMsg         *egMsg,
                                    EgIeLevelInfo *egLvlInfo,
                                    U8             inst));

EXTERN S16 vbUtlEgtAddApn ARGS((EgMsg         *egMsg, 
                                EgIeLevelInfo *egLvlInfo,
                                U8            *apn,
                                U8             inst));

EXTERN S16 vbUtlEgtAddIndType ARGS((EgMsg *egMsg, 
                                    EgIeLevelInfo *egLvlInfo,
                                    U8 inst));

EXTERN S16 vbUtlEgtAddRatType ARGS((EgMsg *egMsg, 
                                    EgIeLevelInfo *egLvlInfo,
                                    U8 ratType, U8 inst));

EXTERN S16 vbUtlEgtAddSrvNwk ARGS((EgMsg         *egMsg, 
                                   EgIeLevelInfo *egLvlInfo,
                                   U8             inst));

EXTERN S16 vbUtlEgtAddMsisdn ARGS((EgMsg         *egMsg,
                                   EgIeLevelInfo *egLvlInfo,
                                   U8            *msIsdn, 
                                   U8             inst));

EXTERN S16 vbUtlEgtAddImsi ARGS((EgMsg         *egMsg, 
                                 EgIeLevelInfo *egLvlInfo,
                                 U8            *imsi,
                                 U8            imsiLen,
                                 U8             inst));

EXTERN S16 vbUtlEgtAddPAA ARGS((EgMsg         *egMsg, 
                                EgIeLevelInfo *egLvlInfo,
                                U8            *ipAddr,
                                U8             inst));

/* All prototypes for lower interface with S1AP */
EXTERN S16 VbLiSztBndReq  ARGS((Pst *pst, SuId suId, SpId spId));
EXTERN S16 VbLiSztUbndReq ARGS((Pst *pst, SpId spId, Reason reason));
EXTERN S16 VbLiSztUDatReq ARGS((Pst *pst, SpId spId, SztUDatEvnt *uDatEvnt));
EXTERN S16 VbLiSztConReq  ARGS((Pst *pst, SpId spId, SztConReq *conReq));
EXTERN S16 VbLiSztConRsp  ARGS((Pst *pst, SpId spId, SztConRsp *conRsp));
EXTERN S16 VbLiSztRelReq  ARGS((Pst *pst, SpId spId, SztRelReq *relReq));
EXTERN S16 VbLiSztRelRsp  ARGS((Pst *pst, SpId spId, SztRelRsp *relRsp));
EXTERN S16 VbLiSztDatReq  ARGS((Pst *pst, SpId spId, SztDatEvntReq *datEvnt));
EXTERN S16 VbLiSztEncDecReq ARGS((Pst *pst, SpId spId, SztEncDecEvnt
                                  *sztEncDecEvnt));
EXTERN S16 VbLiSztAudReq   ARGS((Pst *pst, SpId spId, SztAudEvnt *audEvnt));

/* All prototypes for lower interface with EGTP */
EXTERN S16 VbLiEgtBndReq  ARGS((Pst *pst, SuId suId, SpId spId));

EXTERN S16 VbLiEgtUbndReq ARGS((Pst *pst, SpId spId, Reason reason));

EXTERN S16 VbLiEgtSigReq ARGS((Pst         *pst,
                               SpId        spId,
                               CmTptAddr   *localAddr, /* Local node address */
                               CmTptAddr   *remAddr,   /* Remote node address */
                               TknU32      lclTeid,    /* Local Tunnel Identifier */
                               TknU32      transId,    /* Transaction Identifier */
                               TknU8       crtFlag,    /* Create tunnel or not */
                               TmrCfg      T3,         /* Request timer expiry */
                               EgMsg       *egMsg));

EXTERN S16 VbLiEgtSigRsp ARGS((Pst     *pst,
                               SpId     spId,
                               TknU32   lclTeid, 
                               EgMsg   *egMsg));

EXTERN S16 VbLiEgtStaReq ARGS((Pst         *pst,
                               SpId         spId,
                               U8           eventType, 
                               EgPathInfo  *pathInfo));

EXTERN S16 VbLiEgtLclDelReq ARGS((Pst *pst,
                                  SpId spId,
                                  U32 transId, 
                                  U32 teid));

EXTERN S16 vbMmeSndInitialCtxtSetup ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeSndDwnLnkNotAck ARGS((VbMmeUeCb    *ueCb,
                                      U8           casue));

EXTERN S16 vbMmeSndDwnLnkNotFailInd ARGS((VbMmeUeCb    *ueCb,
                                          U8           casue));
  
/* Borrowed from EGTP */
EXTERN S16 EgUtilDeAllocGmMsg ARGS ((EgMsg **egMsg));
EXTERN S16 EgUtilGmGetIeOccrCnt ARGS((EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, 
           U16 *ieOccrCnt));
EXTERN S16 EgUtilGmGetIe ARGS((EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, 
                    EgIe **egIe, U16 *iePos));


/* Declare all EMM-Procedure handler here */
EXTERN S16 vbMmeHndlAttachOnNasEvnt ARGS ((VbMmeUeCb    *ueCb,
                                             CmNasEvnt    *evnt,
                                             U8           emmMsgType));
EXTERN S16 vbMmeHndlUeDetachOnNasEvnt ARGS ((VbMmeUeCb    *ueCb,
                                               CmNasEvnt    *evnt,
                                               U8           emmMsgType));

EXTERN S16 vbMmeHndlNwDetachOnNasEvnt ARGS ((VbMmeUeCb    *ueCb,
                                               CmNasEvnt    *evnt,
                                               U8           emmMsgType));

EXTERN S16 vbMmeHndlServReqOnNasEvnt ARGS ((VbMmeUeCb    *ueCb,
                                                 CmNasEvnt    *evnt,
                                                 U8           emmMsgType));

EXTERN S16 vbMmeHndlTauOnNasEvnt ARGS ((VbMmeUeCb    *ueCb,
                                          CmNasEvnt    *evnt,
                                          U8           emmMsgType));

EXTERN S16 vbMmeHndlGutiReallocOnNasEvnt ARGS ((VbMmeUeCb    *ueCb,
                                          CmNasEvnt    *evnt,
                                          U8           emmMsgType));

EXTERN S16  vbMmeHndlEmmOnUeEvnt ARGS ((VbMmeUeCb    *ueCb,
                                        CmNasEvnt    *evnt,
                                        U8           emmMsgType));

EXTERN S16  vbMmeHndlEmmOnEsmUeEvnt ARGS ((VbMmeUeCb    *ueCb,
                                          CmNasEvnt    *evnt,
                                          U8           emmMsgType));



/* Declare all ESM-Procedure handler here */
EXTERN S16 vbMmeHndlActDedBearOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                                CmNasEvnt    *evnt,
                                                U8           esmMsgType));

EXTERN S16 vbMmeHndlDeactDedBearOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                                CmNasEvnt    *evnt,
                                                U8           esmMsgType));

EXTERN S16 vbMmeHndlActDefBearOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                                CmNasEvnt    *evnt,
                                                U8           esmMsgType));

EXTERN S16 vbMmeHndlDeactDefBearOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                                CmNasEvnt    *evnt,
                                                U8           esmMsgType));

EXTERN S16 vbMmeHndlModDedBearOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                                CmNasEvnt    *evnt,
                                                U8           esmMsgType));

EXTERN S16 vbMmeHndlResAllocOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                              CmNasEvnt    *evnt,
                                              U8           esmMsgType));

EXTERN S16 vbMmeHndlResModOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                            CmNasEvnt    *evnt,
                                            U8           esmMsgType));

EXTERN S16 vbMmeHndlEsmOnUeEvnt ARGS((VbMmeEsmCb   *esmCb,
                                            CmNasEvnt    *evnt,
                                            U8           esmMsgType));

EXTERN S16 vbMmeHndlPdnConnOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                           CmNasEvnt    *evnt,
                                           U8           esmMsgType));

EXTERN S16 vbMmeHndlPdnDisConnOnNasEvnt ARGS((VbMmeEsmCb   *esmCb,
                                           CmNasEvnt    *evnt,
                                           U8           esmMsgType));

/* Declare all S1ap-Procedure handler here */

EXTERN S16 vbMmeRcvS1apMsg ARGS((U32           suConnId,
                                 Ptr            *evnt,
                                 U8             msgType)); 

EXTERN S16 vbMmeHndlAttachOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                               Ptr            *evnt,
                                               U8             msgType));

EXTERN S16 vbMmeHndlUeDetachOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                                 Ptr            *evnt,
                                                 U8             msgType));

EXTERN S16 vbMmeHndlNwDetachOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                                 Ptr            *evnt,
                                                 U8             msgType));

EXTERN S16 vbMmeHndlServReqOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                                   Ptr            *evnt,
                                                   U8             msgType));

EXTERN S16 vbMmeHndlPagingOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                               Ptr            *evnt,
                                               U8             msgType));

EXTERN S16 vbMmeHndlTAUOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                               Ptr            *evnt,
                                               U8             msgType));

EXTERN S16 vbMmeHndlCtxtRelOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                                Ptr            *evnt,
                                                U8             msgType));
EXTERN S16 vbMmeHndlEsmProcOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                                Ptr            *evnt,
                                                U8             msgType));

EXTERN S16 vbMmeHndlUeCtxtModOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                               Ptr            *evnt,
                                               U8             msgType));

EXTERN S16 vbMmeHndlBearRelIndOnS1apEvnt ARGS((VbMmeUeCb      *ueCb,
                                               Ptr            *evnt,
                                               U8             msgType));
EXTERN S16 vbMmeHndlS1apOnEnbEvnt ARGS((VbMmeUeCb      *ueCb,
                                        Ptr            *evnt,
                                        U8             msgType));

/* Declare all Sgw-Procedure handler here */

EXTERN S16 vbMmeHndlAttachOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                              VbMmeUeCb     *ueCb,
                                              VbGtpcMsg      msgType,
                                              EgMsg         *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlUeDetachOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                                VbMmeUeCb     *ueCb,
                                                VbGtpcMsg      msgType,
                                                EgMsg         *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlNwDetachOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                                VbMmeUeCb     *ueCb,
                                                VbGtpcMsg      msgType,
                                                EgMsg         *egMsg,
                                              TknU32        *transId));


EXTERN S16 vbMmeHndlPagingOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                              VbMmeUeCb     *ueCb,
                                              VbGtpcMsg      msgType,
                                              EgMsg         *egMsg,
                                              TknU32        *transId));


EXTERN S16 vbMmeHndlServReqOnSgwEvnt ARGS ((VbMmeEgtSapCb *egtSapCb,
                                            VbMmeUeCb     *ueCb,
                                            VbGtpcMsg      msgType,
                                            EgMsg         *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlTAUOnSgwEvnt ARGS ((VbMmeEgtSapCb *egtSapCb,
                                            VbMmeUeCb     *ueCb,
                                            VbGtpcMsg      msgType,
                                            EgMsg         *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlImplicitDetachOnSgwEvnt ARGS ((VbMmeEgtSapCb *egtSapCb,
                                                   VbMmeUeCb     *ueCb,
                                                   VbGtpcMsg      msgType,
                                                   EgMsg         *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlNwTrigSgwEvnt ARGS ((VbMmeEgtSapCb *egtSapCb,
                                         VbMmeUeCb     *ueCb,
                                         VbGtpcMsg      msgType,
                                         EgMsg          *egMsg,
                                              TknU32        *transId));
EXTERN S16 vbMmeHndlUeCtxRelOnSgwEvnt ARGS ((
                                             VbMmeEgtSapCb *egtSapCb,
                                             VbMmeUeCb     *ueCb,
                                             VbGtpcMsg      msgType,
                                             EgMsg          *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlEsmProcOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                      VbMmeUeCb     *ueCb,
                                      VbGtpcMsg      msgType,
                                      EgMsg          *egMsg,
                                              TknU32        *transId));
EXTERN S16 vbMmeHndlBrRelIndProcOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                      VbMmeUeCb     *ueCb,
                                      VbGtpcMsg      msgType,
                                      EgMsg          *egMsg,
                                              TknU32        *transId));

EXTERN S16 vbMmeHndlResAllocOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                            VbMmeUeCb     *ueCb,
                                            VbGtpcMsg     msgType,
                                            EgMsg         *egMsg,
                                            VbMmeEsmCb    *esmCb));
EXTERN S16 vbMmeHndlResModOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                            VbMmeUeCb     *ueCb,
                                            VbGtpcMsg     msgType,
                                            EgMsg         *egMsg,
                                            VbMmeEsmCb    *esmCb));

EXTERN S16 vbMmeHndlBearRelOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                            VbMmeUeCb     *ueCb,
                                            VbGtpcMsg     msgType,
                                            EgMsg         *egMsg,
                                            VbMmeEsmCb    *esmCb));

EXTERN S16 vbMmeHndlPdnConnOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                           VbMmeUeCb     *ueCb,
                                           VbGtpcMsg     msgType,
                                           EgMsg         *egMsg,
                                           VbMmeEsmCb    *esmCb));

EXTERN S16 vbMmeHndlPdnDisConnOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                           VbMmeUeCb     *ueCb,
                                           VbGtpcMsg     msgType,
                                           EgMsg         *egMsg,
                                           VbMmeEsmCb    *esmCb));

EXTERN S16 vbMmeHndlActDefBearOnSgwEvnt ARGS((VbMmeEgtSapCb *egtSapCb,
                                           VbMmeUeCb     *ueCb,
                                           VbGtpcMsg     msgType,
                                           EgMsg         *egMsg,
                                           VbMmeEsmCb    *esmCb));

/**************************************************************************//**
 *
 * @brief 
 *    Handles Create Session Response recieved as a part of S1 Handover
 *    Procedure
 *             
 * @param[in]  pEgtSapCb
 *    Pointer to EGT Sap control block
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] msgType
 *    GTP message type
 * @param[out] pEgMsg
 *    Pointer to Egtp message structure
 * @param[out] pTransId
 *    Pointer to transaction ID
 * 
 * @return
 *    SUCCESS if Create Session Response is handled Successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 vbMmeHndlHandoverOnSgwEvnt
(
VbMmeEgtSapCb  *pEgtSapCb,
VbMmeUeCb      *pUeCb,
VbGtpcMsg      msgType,
EgMsg          *pEgMsg,
TknU32         *pTransId
);

/* All utility functions */

/* nas utils*/
EXTERN S16 vbMmeEmmFailInd ARGS((VbMmeUeCb       *ueCb,
                                 VbMmeEmmFlrInd  *iflrInd));

EXTERN S16 vbMmeHndlAttachReq ARGS((VbMmeUeCb    *ueCb,
                                    CmNasEvnt    *evnt));

EXTERN S16 vbMmeHndlIdentityResp ARGS((VbMmeUeCb    *ueCb,
                                       CmNasEvnt    *evnt));

EXTERN S16 vbMmeHndlAuthResp ARGS((VbMmeUeCb    *ueCb,
                                   CmNasEvnt    *evnt));
#ifdef ANSI
EXTERN S16 vbMmeHndlAuthFail ARGS((VbMmeUeCb    *ueCb,
                                   CmNasEvnt    *evnt));

EXTERN S16 vbMmeHndlSecModeCmp ARGS((VbMmeUeCb    *ueCb,
                                     CmNasEvnt    *evnt));

EXTERN S16 vbMmeHndlSecModeRej ARGS((VbMmeUeCb    *ueCb,
                                     CmNasEvnt    *evnt));
#endif

EXTERN S16 vbMmeHndlAttachCmp ARGS((VbMmeUeCb    *ueCb,
                                    CmNasEvnt    *evnt));

EXTERN S16 vbMmeHandlDetachReq ARGS((VbMmeUeCb    *ueCb,
                                CmNasEvnt    *evnt));

EXTERN S16 vbMmeSndAttachReject ARGS ((VbMmeUeCb  *ueCb,
                                        U16         emmCause,
                                        VbMmeEsmCb *esmCb));

EXTERN S16 vbMmeSndIdentityRequest ARGS((VbMmeUeCb  *ueCb,
                                         U8         type));

#ifdef VB_MME_AUTH
EXTERN S16 vbMmeSndAuthReject ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeSndAuthRequest ARGS ((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeVerifyAuthResp ARGS((VbMmeUeCb    *ueCb, 
                                      CmNasEvnt    *evnt));
#endif /* VB_MME_AUTH */

#ifdef VB_MME_NAS_SEC
EXTERN S16 vbMmeSndSecModCmd ARGS ((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeUtlBldSecModCmd ARGS(( VbMmeEmmCb  *ueEmmCb, 
                                             CmNasEvnt   **evnt ));
PUBLIC S16 vbMmeUtlGetNasAlg ARGS
((
VbMmeUeCb      *ueCb,
VbMmeEncAlg    *encAlg,
VbMmeIntAlg    *intAlg,
VbMmeAuthAlg   *authAlg  
));


#endif /* VB_MME_NAS_SEC */


EXTERN S16 vbMmeSndActDedBearerReq ARGS ((VbMmeEsmCb    *esmCb));
EXTERN S16 vbMmeSndActDefBearerReq ARGS ((VbMmeEsmCb    *esmCb));
EXTERN S16 vbMmeSndDeActDedBearerReq ARGS ((VbMmeEsmCb    *esmCb,
                                          CmEsmCause     cause));
EXTERN S16 vbMmeSndDeActDefBearerReq ARGS ((VbMmeEsmCb    *esmCb,
                                          CmEsmCause     cause));
EXTERN S16 vbMmeSndModBearerCtxtReq ARGS ((VbMmeEsmCb    *esmCb,
                                          CmEsmCause     cause));


EXTERN S16 vbMmeSndAttachAccept ARGS ((VbMmeUeCb *ueCb));
EXTERN S16 vbMmeSndDetachReq ARGS ((VbMmeUeCb    *ueCb,
                                    U8           detachType,
                                    U16          emmCause));

EXTERN S16 vbMmeSndDetachAcc ARGS ((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeSndServiceRej ARGS ((VbMmeUeCb    *ueCb,
                                     U8            cause));

EXTERN S16 vbMmeSndResAllocRej ARGS((VbMmeEsmCb    *esmCb,
                               U8            esmCause));
EXTERN S16 vbMmeSndResModRej ARGS((VbMmeEsmCb    *esmCb,
                               U8            esmCause));

EXTERN S16 vbMmeSndPdnConnRej ARGS((VbMmeEsmCb    *esmCb,
                               U8            esmCause));
EXTERN S16 vbMmeSndPdnDisConnRej ARGS((VbMmeEsmCb    *esmCb,
                                       U8            esmCause));

EXTERN S16 vbMmeUtlBldResAllocReject ARGS ((VbMmeEsmCb *esmCb,
                                          CmNasEvnt  **evnt,
                                          U8         esmCause));
EXTERN S16 vbMmeUtlBldResModReject ARGS ((VbMmeEsmCb *esmCb,
                                          CmNasEvnt  **evnt,
                                          U8         esmCause));

EXTERN S16 vbMmeHandlServiceReq ARGS((VbMmeUeCb    *ueCb,
                                CmNasEvnt    *evnt));
EXTERN S16 vbMmeUtlBldServiceReject ARGS ((VbMmeUeCb *ueCb,
                                          CmNasEvnt  **evnt,
                                          U8         emmCause));

EXTERN S16 vbMmeUtlBldGutiReallocCmd ARGS ((VbMmeUeCb *ueCb,
                                          CmNasEvnt  **evnt));
EXTERN S16 vbMmeSndGutiReallocCmd ARGS((VbMmeUeCb    *ueCb));


EXTERN S16 vbMmeHndlTAUReq ARGS((VbMmeUeCb    *ueCb,
                                  CmNasEvnt    *evnt));
EXTERN S16 vbMmeHndlTAUCmp ARGS((VbMmeUeCb    *ueCb,
                                  CmNasEvnt    *evnt));
/* s1ap utils */

EXTERN S16 vbMmeHdlInitialCtxtSetupFail ARGS ((VbMmeUeCb     *ueCb,
                                               SztDatEvntInd *evnt));

EXTERN S16 vbMmeHdlUeCtxtModFail ARGS ((VbMmeUeCb     *ueCb,
                                               SztDatEvntInd *evnt));
/*sgw/gtp utils */

EXTERN S16 vbMmeVldNStrCSRsp ARGS((VbMmeUeCb   *ueCb,
                                     EgMsg       *egMsg,
                                     U8          *esmCause));

EXTERN S16 vbMmeVldNStrMBRsp ARGS((VbMmeUeCb   *ueCb,
                                   EgMsg       *egMsg,
                                   U8         *esmCause));

EXTERN S16 vbMmeVldNStrRABRsp ARGS ((VbMmeUeCb   *ueCb,
                                     EgMsg       *egMsg,
                                     U8          *esmCause));

EXTERN S16 vbMmeVldNStrBearResFailInd ARGS ((VbMmeUeCb   *ueCb,
                                             VbMmeEsmCb  *esmCb,
                                             EgMsg       *egMsg,
                                             U8     *esmCause));

EXTERN S16 vbMmeVldNStrCrtBrReq ARGS ((VbMmeUeCb   *ueCb, 
                                       VbMmeEsmCb  *esmCb,
                                       EgMsg       *egMsg,
                                       U8          *cause));

EXTERN S16 vbMmeVldNStrDelBrReq ARGS ((VbMmeUeCb   *ueCb, 
                                       VbMmeEsmCb  *esmCb,
                                       EgMsg       *egMsg,
                                       U8          *cause));

EXTERN S16 vbMmeVldNStrUpdBrReq ARGS ((VbMmeUeCb   *ueCb, 
                                       VbMmeEsmCb  *esmCb,
                                       EgMsg       *egMsg,
                                       U8          *cause));

EXTERN S16 vbMmeUtlBldEgtRABReq ARGS ((VbMmeUeTunnelCb  *ueTunlCb,
                                       EgMsg            **egMsg));

EXTERN S16 vbMmeIncRlsAccBearerRsp ARGS ((CmTptAddr   *localAddr,
                                          CmTptAddr   *remAddr,
                                          TknU32       teid,
                                          TknU32       transId,
                                          EgMsg       *egMsg));

EXTERN S16 vbMmeIncBrResFailInd ARGS ((CmTptAddr   *localAddr,
                                          CmTptAddr   *remAddr,
                                          TknU32       teid,
                                          TknU32       transId,
                                          EgMsg       *egMsg));

EXTERN S16 vbMmeSndTAUAcc ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeSndTAURej ARGS((VbMmeUeCb    *ueCb,
                                U8           cause));

EXTERN S16 vbMmeUtlBldTAUAcc ARGS((VbMmeEmmCb   *ueEmmCb,    
                                   CmNasEvnt    **evnt));

EXTERN S16 vbMmeUtlBldTAURej ARGS((VbMmeEmmCb   *ueEmmCb,    
                                   CmNasEvnt    **evnt,
                                   U8           cause));

EXTERN S16 vbMmeSndEmmStatus ARGS((VbMmeUeCb    *ueCb,
                                U8           cause));
EXTERN S16 vbMmeUtlBldEmmStatus ARGS((VbMmeEmmCb   *emmCb,    
                                   CmNasEvnt    **evnt,
                                   U8           cause));
EXTERN S16 vbMmeSndEsmStatus ARGS((VbMmeUeCb    *ueCb,
                                   VbMmeEsmCb   *esmCb,
                                   U8           pti,
                                   U8           brId,
                                   U8           cause));
EXTERN S16 vbMmeSndEsmInfoReq ARGS((VbMmeEsmCb    *esmCb));
EXTERN S16 vbMmeUtlBldEsmStatus ARGS((VbMmeUeCb   *ueUeCb,    
                                      CmNasEvnt    **evnt,
                                      U8           pti,
                                      U8           brId,
                                      U8           cause));
EXTERN S16 vbMmeUtlBldEsmInfoReq ARGS((VbMmeEsmCb   *esmCb,    
                                       CmNasEvnt    **evnt));
EXTERN S16 vbMmeHndlEsmInfoRsp ARGS((VbMmeEsmCb    *esmCb,
                                     CmNasEvnt     *evnt));

PUBLIC S16 vbMmeUtlGetNasMsg ARGS
((
CmNasEvnt *evnt, 
U8        **val, 
U16       *len
));

/* Other */
#ifdef VB_PERF_MEAS
EXTERN Void vbMmePerfMeasEnd ARGS((VbMmeUeCb *ueCb));
#endif

EXTERN S16 vbMmeUtlAllocBId ARGS((VbMmeUeCb    *ueCb,
                                  VbMmeEsmCb   **esmCb,
                                  U8           *ueEsmBId,
                                  U8           memAllocReq));
EXTERN S16 vbMmeUtlCrtEsmCbOnTrnId ARGS((VbMmeUeCb    *ueCb,
                                   VbMmeEsmCb   **esmCb,
                                   U8           ueEsmBId,
                                   U8           ueEsmTId));

EXTERN S16 vbMmeAddToTrnList ARGS((VbMmeUeCb    *ueCb,
                                   VbMmeEsmCb   *esmCb,
                                   U8           ueEsmTId));

EXTERN S16 vbMmeRmvFrmTrnList ARGS((VbMmeUeCb    *ueCb,
                                    U8           ueEsmTId));

EXTERN S16 vbMmeMoveToEsmCbDelList ARGS((VbMmeUeCb    *ueCb,
                                         VbMmeEsmCb   *esmCb));

EXTERN S16 vbMmeDelAllFrmDelList ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeGetFrmTrnList ARGS((VbMmeUeCb    *ueCb,
                                    VbMmeEsmCb   **esmCb,
                                    U8           ueEsmTId));

EXTERN S16 vbMmeAddToUETaiLst ARGS((VbMmeUeCb *ueCb,
                                    VbUeTai   *tai));

EXTERN S16 vbMmeUtlMapGtpToNasCause ARGS((U8   gtpCasue,
                                         U8   *nasCuase));

EXTERN S16 vbMmeInitCtxtRelProc ARGS((VbMmeUeCb  *ueCb,
                                      U8         choice,
                                      U8         cause,
                                      U8         rabReq));

EXTERN S16 vbMmeUtlLocDel ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeS1apErrInd ARGS((VbMmeUeCb         *ueCb,
                                 SztLclErrInd      *sztLclErrInd));

EXTERN S16 vbMmeRelLocS1Con ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeAbortTAUProc ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeAbortEsmProc ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeAbortAttachProc ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeAbortServProc ARGS((VbMmeUeCb    *ueCb));
EXTERN S16 vbMmeAbortUeCtxtRelProc ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeAbortBrMgmtProc ARGS((VbMmeUeCb    *ueCb,
                                      U8           proc,
                                      Bool         isRel));

EXTERN S16 vbMmeUtlSpndEsmProc ARGS((VbMmeEsmCb   *esmCb,
                                      CmNasEvnt    *evnt,
                                      U8           msgType));

EXTERN S16 vbMmeUtlSpndEmmProc ARGS((VbMmeUeCb   *ueCb,
                                      CmNasEvnt    *evnt,
                                      U8           msgType));

EXTERN S16 vbMmeUtlSpndS1apProc ARGS((VbMmeUeCb     *ueCb,
                                      VbMmeS1apEvnt *s1apEvnt,
                                      S1apPdu       *s1apPdu,
                                      U8            msgType));

EXTERN S16 vbMmeUtlResumeProc ARGS((VbMmeUeCb    *ueCb));

EXTERN S16 vbMmeUtlAddPdnCb ARGS((VbMmeUeCb    *ueCb,
                                        VbMmeEsmCb   *esmCb,
                                        VbMmePdnCb   **pdnCb));
EXTERN S16 vbMmeUtlDelPdnCb ARGS((VbMmeUeCb    *ueCb,
                                  VbMmeEsmCb   *esmCb,
                                  VbMmePdnCb   *pdnCb));

EXTERN S16 vbMmeUtlGetEsmCbOnS11TranId ARGS((VbMmeUeCb      *ueCb,
                                             VbMmeEsmCb     **esmCb,
                                             TknU32         *transId));
EXTERN S16 vbMmeUtlIsPlmnSupported ARGS((VbMmeUeCb *ueCb));

#ifdef X2_HO
EXTERN S16 vbMmeUtlFndUeCbMmeS1apId ARGS((VbMmeUeCb    **ueCb,
                                         U32            ueId));

EXTERN S16 vbMmeHdlPathSwRqst ARGS ((VbMmeEnbCb    *enbCb,
                                      SztConInd     *datEvnt));
EXTERN S16 vbMmeUtlBldPathSwRqstAckg( VbMmeUeCb  *ueCb,
                                      S1apPdu    **pdu
                                      );

#endif

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Preparation Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOPrep
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles eNB status transfer message 
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for enb status transfer is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if eNB status transfer is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndleNBStatusTfr
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Resource Allocation Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOResAlloc
(
VbMmeUeCb      *pUeCb,
SztConCfm      *pConCfm
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Notification Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHONotif
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Related S1AP Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * @param[in]  msgType
 *    Type of message received on S1-MME interface
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverOnS1apEvnt
(
VbMmeUeCb      *pUeCb,
Ptr            *pEvnt,
U8             msgType
);

/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Request message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pTgtEnbCb
 *    Target eNodeB cb
 * 
 * @return
 *    SUCCESS if Hanodver Request is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOReq
(
VbMmeUeCb      *pUeCb,
VbMmeEnbCb     *pTgtEnbCb
);


/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Command message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Hanodver Command is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOCmd
(
VbMmeUeCb      *pUeCb,
VbMmeEnbCb     *pEnbCb
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Delete Session Response received as a part of Handover
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pTransId
 *    Pointer to Transaction ID
 * 
 * @return
 *    SUCCESS if Delete Session Request is handled successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHODSRsp
(
VbMmeUeCb      *pUeCb,
EgMsg          *pEgMsg,
TknU32         *pTransId
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Modify Bearer Response received as a part of Handover
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pTransId
 *    Pointer to Transaction ID
 * 
 * @return
 *    SUCCESS if Modify Bearer Response is handled successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOMBrsp
(
VbMmeUeCb      *pUeCb,
EgMsg          *pEgMsg,
TknU32         *pTransId
);

/**************************************************************************//**
 *
 * @brief 
 *    Validates and stores the Handover Required Message as a part
 *    of Hanodver Preparation Procedure
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pPdu
 *    Pointer to S1AP Pdu with HO Required Message
 * 
 * @return
 *    SUCCESS if Hanodver Required message is validate and information is
 *    stored successfully otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreHOReqd
(
VbMmeUeCb      *pUeCb,
SztS1AP_PDU    *pPdu
);

/**************************************************************************//**
 *
 * @brief 
 *    Finds eNodeB CB based on the Home eNB ID
 *             
 * @param[in]  pEnbId
 *    Pointer to Home eNB ID
 * @param[out] pEnbCb
 *    Pointer to eNodeB CB
 * 
 * @return
 *    SUCCESS if enode cb is found otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlGetTgtEnbCb
(
VbMmeGlbEnbId     *pEnbId,
VbMmeEnbCb        **pEnbCb
);

/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Preparation Failure message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Hanodver Preparation is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOPrepFail
(
VbMmeUeCb      *pUeCb
);

/**************************************************************************//**
 *
 * @brief 
 *    Searches in HSS DB for UE details
 *             
 * @param[in]  imsi
 *    Pointer to imsi buffer
 * @param[in]  imsiLen
 *    Length of imsi buffer
 * 
 * @return
 *    TRUE if the UE details are present in DB therwise FALSE
 *
 *****************************************************************************/
PUBLIC Bool vbMmeHssIsRoamingUe
(
U8       *pImsi,
U32      imsiLen
);

/**************************************************************************//**
 *
 * @brief 
 *    Gets MCC and MNC from PLMN ID
 *             
 * @param[in]  pPlmnId
 *    Pointer to plmn id buffer
 * @param[out] mcc
 *    Pointer to mcc buffer
 * @param[out] mnc
 *    Pointer to mnc buffer
 * 
 * @return
 *    No return values
 *
 *****************************************************************************/
PUBLIC Void vbMmeUtlGetTgtMccMnc
(
U8       *pPlmnId,
U8       *pMcc,
U8       *pMnc
);

/**************************************************************************//**
 *
 * @brief
 *    Callback from HSS
 *
 * @param pHssUsrData
 *    MME data given to HSS
 *
 * @return 
 *    Void
 *
 *****************************************************************************/
PUBLIC Void vbMmeUpdLocRspCbk
(
Void        *pMmeUsrData
);

/**************************************************************************//**
 *
 * @brief 
 *    Sends eNB Status Transfer message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pTgtEnbCb
 *    Target eNodeB cb
 * 
 * @return
 *    SUCCESS if eNB Status Transfer is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndMmeStatusTfr
(
VbMmeUeCb      *pUeCb,
VbMmeEnbCb     *pTgtEnbCb
);


/**************************************************************************//**
 *
 * @brief 
 *    Copies HO Data from src UE cb to Dst UE cb
 *             
 * @param[in]  pDstUeCb
 *    Pointer to UE Control block
 * @param[in]  pSrcUeCb
 *    Pointer to UE Control block
 * 
 * @return
 *    SUCCESS if values are copied otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlCopyUeCbData
(
VbMmeUeCb   *pTgtUeCb,
VbMmeUeCb   *pSrcUeCb
);

PUBLIC Void vbMmeUtlDelHoData
(
VbMmeUeCb   *pUeCb
);

PUBLIC Void vbMmeUtlFndNDelUeCb
(
U8       *pImsi,
U32      imsiLen
);

/**************************************************************************//**
 *
 * @brief 
 *    Aborts Handover Procedure.
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * 
 * @return
 *    No return Values
 *
 *****************************************************************************/
PUBLIC Void vbMmeAbortHOProc
(
VbMmeUeCb      *pUeCb
);

/**************************************************************************//**
 *
 * @brief 
 *    Nas message received during Handover
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block 
 * @param[in]  pEvnt
 *    Pointer to Emm NAS Event
 * @param[in]  msgType
 *    Type of NAS message received
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverOnNasEvnt
(
VbMmeUeCb      *pUeCb,
CmNasEvnt      *pEvnt,
U8             emmMsgType 
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles UE Context Release request received Midway of Handover
 *    Procedure
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for enb status transfer is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if UE Context Release Request is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlUeCtxRlsReqDuringHO
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles UE Context Release request received in interRAT Handover
 *    Procedure
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for enb status transfer is received
 * 
 * @return
 *    SUCCESS if UE Context Release comand is send successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/

PUBLIC S16 vbMmeSndUeCtxRlsCmdDuringiRATHO
(
VbMmeUeCb      *pUeCb
);


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Cancel Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover Cancel is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Handover Cancel is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOCancel
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
);

/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Cancel Ack message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Hanodver Cancel Ack is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOCancelAck
(
VbMmeUeCb      *pUeCb
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Failure S1AP Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * @param[in]  msgType
 *    Type of message received on S1-MME interface
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverFailOnS1apEvnt
(
VbMmeUeCb      *pUeCb,
Ptr            *pEvnt,
U8             msgType
);

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Failure Message received from eNodeB
 *    Sends Delete Session Request to SGW
 *    Sends Forward Relocation Response with failure cause to MME
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Handover Failure is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOFailure
(
VbMmeUeCb      *pUeCb
);

/**************************************************************************//**
 *
 * @brief 
 *    S1AP Release Indication during Handover
 *             
 * @param[in] suConnId
 * @param[in] evnt
 *    S1AP Release indication event
 * @param[in] msgType
 *    Message type
 *    
 * @return
 *    SUCCESS/FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeRcvS1apRelInd
(
U32            suConnId,
Ptr            *evnt,
U8             msgType
);

/**************************************************************************//**
 *
 * @brief 
 *    Deletes the Old UE Cb, after a successfull S1 Handover without
 *    MME relocation. Deallocated memory allocated for UE Cb, 
 *    removes from Connection ID hash list
 *             
 * @param[in]  pUeCb
 *    Pointer to UE CB
 * 
 * @return
 *    Void
 *
 *****************************************************************************/
PUBLIC Void vbMmeUtlDelHOUeCb
(
VbMmeUeCb   *pUeCb
);

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __VBX__ */

/**********************************************************************

         End of file:     vb.x@@/main/1 - Mon Jun 28 19:35:34 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -       ad               1. LTE-CNE Initial Release.
/main/1  vb001.101   rk               1. Added ECM control block & ECM State.
                                      2. Added EGTPC Create bearer messages.
                                      3. Added utility functions.
/main/1  vb003.101   chekar           1. Added Ue Rad Capability support.
/main/1  vb005.101   rk               1. Added support for Network initiated Detach
                                      2. Added support for multiple Attach and Detach 
						pmacharla			  1. eGTP Rel 9 upgrade changes under EG_REL_930
*********************************************************************91*/
