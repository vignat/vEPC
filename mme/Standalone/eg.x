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
 
    Name:   EGTP layer
 
    Type:   C include file
 
    Desc:   Structures, variables and typedefs required by EGTP
 
    File:   eg.x
 
    Sid:      eg.x@@/main/3 - Fri Feb  4 18:18:02 2011
 
    Prg:    sn
 
*********************************************************************21*/

#ifndef __EGX__
#define __EGX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* eg005.201: PSF flag to avoid eNodeB compile error --*/
#ifdef HW
#include "sht.h"
#include "sht.x"
#endif


/** @file eg.x
    @brief EG Include File (eg.x)
*/
#ifdef TIME_MES
#ifdef SS_MT_CB
struct euTime
{
   U32 call;
   struct timespec time[MAX_MSG];
};
struct euTime entryTx[MAX_THREAD];
struct euTime exitTx[MAX_THREAD];
struct euTime entryRx[MAX_THREAD];
struct euTime exitRx[MAX_THREAD];
#else
struct timespec entryTx1[MAX_MSG];
struct timespec exitTx1[MAX_MSG];
struct timespec entryTx2[MAX_MSG];
struct timespec exitTx2[MAX_MSG];
struct timespec entryRx1[MAX_MSG];
struct timespec exitRx1[MAX_MSG];
struct timespec entryRx2[MAX_MSG];
struct timespec exitRx2[MAX_MSG];
#endif
#endif

typedef struct egTSapCb EgTSapCb;
typedef struct egUSapCb EgUSapCb;
typedef struct egTunnCb EgTunnCb;
typedef struct egTptSrvCb EgTptSrvCb;


/*----------- EGTP Timer Control Block ---------------*/
typedef struct egTimer
{
   CmTimer        tmr;         /* common timer control block */
   PTR            egCbPtr;     /* pointer to Cb which ownes this EgTimer */
} EgTimer;



#ifdef EGTP_U

typedef struct _teIdCb EgTeIdCb;
typedef struct _egUThreadCb EgUThreadCb;

typedef struct _seqInfo
{
   Bool reOrder;         /**< Reording is currently going or not */
   U32  expSeqNmb;       /**< Expecting sequence number */
   U32  curLastSeq;      
   U32 lastRcvdSeq;
   U32  numRcvdPdus;     /**< Algo1 Number of received gpdus */
   U32  winStart;        /**< Algo1 Reording window, start sequence numb */
   EgtUEvnt **pduArray;   /**< Reordering buffer, list of egtUiMsg pointers */   
}SeqInfo;

#ifdef EGTP_U_MULTI_THREADED
struct _egUThreadCb
{
   SSTskId stskId;         /**< STaskId of this thread */
   Inst    thrId;
   Region region;
   Pool pool;
   U32       nmbReOrdTnl;
   EgTeIdCb *sTunCbList;  /**< Start pointer of list of Tunnels assigned to this thread */
   EgTeIdCb *eTunCbList;  /**< End pointer of list of Tunnels assigned to this threade  */
   Cntr       sendMsg;
   Cntr       recvMsg;
   CmTqCp     thrdTqCp;          /**< Reorder TimerQ contol pointer */
   CmTqType   thrdTq[EG_TQSIZE]; /**< Reorder Tunnel Timer Queue */
   EgUThreadCb *next;     /**< Pointer to Next ThreadCb in ThrdsInfo list */
   EgErrSts  errSts;
};

typedef struct _egThrdsInfo
{
   EgUThreadCb *egThrdCbList;  /**< List of worker thread control blocks */
   EgUThreadCb *curThrdCb;     /**< Worker-ThreadCb to be assigned in Round Robin Fashion */
   EgTimer    ldDistTmr;      /**< Load distribution timer */
}EgThrdsInfo;
#endif /* EGTP_U_MULTI_THREADED */

struct _teIdCb
{
   CmHashListEnt        destTeIdEnt;         /**< Key for teId entity in the teIdLst hashlist *
                                               *  maintained in destAddrCb */
#ifdef EGTP_U_MULTI_THREADED 
   U32         inMsgCnt;         /**< Number of incoming Messages for the tunnel */ 
   U32         outMsgCnt;          /**< Number of out going message */
   EgUThreadCb          *egThreadCb;      /**< Pointer to the assigned Worker ThreadCb */
   EgTeIdCb             *next;        
   EgTeIdCb             *prev;
   EgTimer              tnlDelTimer;        /**< Tunnel delete timer */
   Bool                 tnlPendOp;
   U8                   nmbReqToWrkThr;
   U8                   nmbCfmFrmWrkThr;
   U8             notCmpltOp;
#endif /* EGTP_U_MULTI_THREADED  */
/* eg001.201 Adding parameter for determining tunnelType */
#ifdef EGTP_U_REL_9
   U8         tunnelType;      /**<Indicates if tunnel is forwarding or Normal data tunnel */
#endif 
   U8         reordAlgo;
   EgTimer    reOrderTimer;            /**< Packet Reorder Timer */
   U32         teId;      /**< Tunnel Id established with the destination*/
   SeqInfo              *seqInfo;         /**< Sequence numbers information */
   Bool                  endMarker;
   Bool         reOrdering;  /**< This flag represent whether reordering is required or not */
   Cntr                 totalMsgsTx;    /**< Total number of messages transmitted */
   Cntr                 totalMsgsRx;    /**< Total number of messages received */
   EgUSapCb             *uSapCb;     /**< Associated Upper SAP Cb */
};

typedef struct _destAddrCb
{
   CmHashListEnt        destEnt;             /**< Key for destAddrCbLst */
   U32                  echoReqSent;          /**< Cnt about number of echo req sent for this destination */
   U8                   intfType;            /**< Interface Type--*/
/* eg001.201 Adding parameter for keeping stats for Fwd & Normal data tunnel */
   #ifdef EGTP_U_REL_9                           
   U32 forwardingTunnCnt;                    /**< Number of forwarding tunnels*/
   U32 normaldataTunnCnt;                    /**< Number of normal data tunnels*/
   #endif
   CmTptAddr            destIpAddr;             /**< destination IpAddr */
   Bool                 destReachable;          /*--Destination reachability
                                                
                                                  info set by Echo Request*/
   Bool  multicastAddr; /*Multicast Address or not*/
   CmHashListCp         teIdLst;              /**< Hash list containing the list of TEID CB*/
   U16                  maxTPduSize;           /** < Maximum TPDU size for each Ip */
}EgDestAddrCb;

typedef struct _ipCb
{
   CmHashListEnt        ipCbOnUSapEnt;       /**< Key for IpCb Hash list */
   EgIpInfo             ipAddrInfo;          /**< Info about Ip Address */
   CmHashListCp         tptServCbLst;          /**< Hash list of transport contol block */
   EgIpSts               ipStats;            /**< Src Ip statistics */
   U16                  status;                 /**< Info about serOpenReq */
   CmHashListCp          destAddrCbLst;       /**< Hash List about destination  control block */
   Bool                 dfltPort;
   Bool                 encEchoAvail;        /**< Tells whether the decoded buffer is available or not */
#ifdef EGTP_U_MULTI_THREADED
   U8               nmbReqToWrkthr;    
   U8               nmbCfmFrmWrkthr;    
   U8             pendOp;
   U8             notCmpltOp;
#endif /* EGTP_U_MULTI_THREADED */
}EgIpCb;

typedef struct _egUEchoReqCb
{
   CmHashListEnt   echoEnt;       /**< Key for IpCb Hash list */
   U32 seqNo;
   SpId   uSapId;
   Buffer *echoReqBuff;
   EgTptSrvCb *tptServerCb;
   CmTptAddr srcAddr;
   CmTptAddr destAddr;
}EgUEchoReqCb;

typedef struct egTptStat
{
   Cntr         totalMsgsTx;    /* Total number of messages transmitted */
   Cntr         totalMsgsRx;    /* Total number of messages received */
}EgTptStat;


typedef struct _egUcb
{
   CmHashListCp   echoReqHashList;            /**< Hash list maintained to store the echCb */
#ifdef EGTP_U_MULTI_THREADED
   EgThrdsInfo    egThrdsInfo; 
   U32            lastUsedThrId;                /**< Last used thread */
   Bool            pendOp;                     /**< Global pending operation */
   U16            noOfCntrlReq;                /**< Number of control req are in progress */
   U8               nmbReqToWrkthr;             /**< Number of Req to worker threads */
   U8               nmbCfmFrmWrkthr;            /**< Number of confirmations from worker threads */
   U8             notCmpltOp;                 /**< All the request to worker threds are sent succesfully or not */
   /* Added Lock for multi threaded changes */
#ifdef SS_MT_CB
   SLockInfo     threadLock;       
#endif /* SS_MT_CB */
#endif /* EGTP_U_MULTI_THREADED */
   CmTimer        echoTimers;                  /**< Echo timer */
   CmTimer        n3t3Timers;                  /**< Echo timer */
   EgUEdmCb       edmUCb;                     /**< Encoder-Decoder Cb */
   Bool           echoTmrStarted;             /**< Falg representing the echo timer is started
                                                  * or not */
   U8             echoReqCntr;                /**< Number of Echo Re-transmission */
}EgUcb;

#endif /* EGTP_U */


/************* ENUMS ************************************************/

typedef enum _egtMsgCat
{
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_NOTFN,
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_COMMAND,
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_FAILIND,
   EG_GTPC_MSGCAT_RSP_ACK,
   EG_GTPC_MSGCAT_OTHER
}EgtMsgCat;


/* EGTP Modules Name */
typedef enum _egModuleName
{
   EG_MOD_PCM,
   EG_MOD_PCM_USAP,
   EG_MOD_TPT,
   EG_MOD_EDM_GM,
   EG_MOD_EDM_IE,
   EG_MOD_CPM,
   EG_MOD_INVALID
}EgModuleName;

/* Actions posssible on transport server (in fn egTptProcessTsapServers) */
typedef enum _egTptSrvAct
{
   EG_TPT_TPTSRV_INVLD,
   EG_TPT_TPTSRV_CLOSE,
   EG_TPT_TPTSRV_OPEN,
   EG_TPT_TPTSRV_DISABLE,
   EG_TPT_TPTSRV_FREE
}EgTptSrvAct;



/* Upper SAP Control Block */
struct egUSapCb
{
   State         state;          /* SAP state */
   SuId          suId;           /* service user SAP id */
   Pst           pst;            /* post structure */
   EgUSapCfg     cfg;            /* configuration for this SAP */
   EgSapSts      sts;            /* Statistics */
   EgTSapCb      *tSapCb;        /* Associated TSAP */ 
#ifdef EGTP_C
   CmHashListCp incReqCp;        /* used in a special usap for incoming echo
                                  * for saving source details used while
                                  * sending a response. */
   EgTptSrvCb    *tptSrvCb;      /* Associated TPT SRV: all USAPs will  be mapped onto same tpt for single server */
   Bool          isDfltUSap;      /* ccpu00135369 : This needs to be used while freeing the Usap. If this is not set and the associated 
                                    TPT server is default server, then dont free the tpt server */
#endif /* EGTP_C */
#ifdef EGTP_U
   CmHashListCp  ipCbHashList;          /* List of IpCbs */
#ifdef EGTP_U_MULTI_THREADED
   U8               nmbReqToWrkthr;    
   U8               nmbCfmFrmWrkthr;    
   U8             pendOp;
   U8             notCmpltOp;
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */
/*eg004.201 Modified for PSF RUG: New variables introduced
 *remIntfValid - TRUE/FALSE, if TRUE, remote interface version number stored within pst is valid
 *verContEnt - interface version controlling entity 
 */
#ifdef EG_RUG
   Bool remIntfValid;          /* TRUE/FALSE, if TRUE, remote interface
                          version number stored within pst is valid */
   Ent verContEnt;             /* interface version controlling entity */
#endif /* EG_RUG */
#ifdef HW
   CmPFthaRsetMap egHwCb;
#endif
};

/*-- eg006.201: Parameters , connTunnCnt, indicates number of connected Tunnels per destination  --*/
#ifdef EGTP_C
typedef struct _destCb
{
   CmHashListEnt        destEnt;             /**< Key for destAddrCbLst */
   U32                  echoReqSent;          /**< Cnt about number of echo req sent for this destination */
   U8                   intfType;            /**< Interface Type--*/
   CmTptAddr            destIpAddr;             /**< destination IpAddr */
   Bool                 destReachable;          /*--Destination reachability
                                                  info set by Echo Request*/
   U32                  connTunnCnt;         /**< Number of connected tunnels on a path--*/ 
   U32                  numTunn;
}EgDestCb;
#endif


/*********************************************************************
* Transport Module structures                                        *
**********************************************************************/
/**
    @ingroup TPTMOD
    @struct egTSapCb
    @brief Control Block for Transport SAP 
 */
/*eg007.201: TSapCb updated to accomodate list of destination as dstCbLst
             and IP addr indicator to the tSapCb */
struct egTSapCb         
{
   Pst           pst;                 /**< Post structure */       
   Ent           contEnt;             /**< Controlling entity */
   Bool          cfgDone;             /**< Config done */
   SuId          suId;                /**< Service user SAP id */
   SpId          spId;                /**< Service provider id */
   State         state;               /**< State of this SAP */
   EgTSapCfg     cfg;                 /**< Transport SAP configuration   */
   EgSapSts      sts;                 /**< Statistics */
   EgTSapReCfg   reCfg;               /**< Transport SAP reconfiguration */
   U8            bndRetryCnt;         /**< Current bind retry count */
   U8            numSrvr;             /**< Number of active servers */
   UConnId       lastSuConnId;        /**< Last suConnId generated on this SAP */
   EgTptSrvCb    **tptSrvCbLst;       /**< Tpt Server CB List - key SuConnId */
   CmTimer       timers[EG_MAX_TMRS]; /**< Max simultenous timers */
   EgTimer        bndTmr;             /**< Bind timer control block*/
   U32            trcMask;            /**< Trace mask */
   S16            trcLen;             /**< Trace length */
   Pst            cfmPst;             /**< Post structure used for LM confirms       */
   Header         ctrlHdr;            /**< Header from pending control request       */
   CmTptAddr     tSapIpAddr;          /**< IP addr identifier for tSap */
   Bool          ipCfgDone;           /**< Indicator for IP existing in tSap */
#ifdef EGTP_C
   CmHashListCp    dstCbList;          /* List of destination control blocks */
#endif
#ifdef EGTP_C
   Bool           isDfltTptSrvPsnt;   /**< True if dfltTptSrv conatins valid Pointer*/
   EgTptSrvCb     *dftlTptSrv;         /**< To store the pointer of Default TPT SRV*/
#endif /* EGTP_C */
#ifdef EGTP_U_MULTI_THREADED
   U8               nmbReqToWrkthr;    
   U8               nmbCfmFrmWrkthr;    
   U8             pendOp;
   U8             notCmpltOp;
#endif /* EGTP_U_MULTI_THREADED */
/*eg004.201 Modified for PSF RUG : TRUE/FALSE, if TRUE, remote interface
 * version number stored within pst is valid
 */
#ifdef EG_RUG
   Bool remIntfValid;         /* TRUE/FALSE, if TRUE, remote interface
                                 version number stored within pst is valid */
#endif /* EG_RUG */
#ifdef HW
   CmPFthaRsetMap egHwCb;
#endif

};


/* eg014.201:  ccpu00139510 Egtpc release 10 upgrade  */
/**
  @ingroup TPTMOD
  @struct egTptSrvCb
  @brief Control Block for Transport Server */
struct egTptSrvCb
{
#ifdef EGTP_U
   CmHashListEnt  transportEnt;  /* Key to get the transport Cb in transCbHashList *
                                  * of IpCb */
   EgIpCb         *ipCbPtr;              /* Ip Control block Pointer */
   EgTptStat      stats;                /* Statistics of transCb */
   U16            portNum;
   U32            uSapId;               /* upper Sap Id */
   Bool          portStatus; /*Set by echo Request*/
   Bool          isdftlTptSrv; /* Is this default server? */
#ifdef EGTP_U_MULTI_THREADED
   U8               nmbReqToWrkthr;    
   U8               nmbCfmFrmWrkthr;    
   U8             pendOp;
   U8             notCmpltOp;
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */
   UConnId        suConnId;      /**< Server connection id */
   UConnId        spConnId;      /**< Server connection id */
   U8             state;         /**< Transport state */
   TknStrOSXL     hostName;     /**< Server's host/domain name(host.domain)*/
   EgTptSrvCfg    cfg;           /**< Server configuration */
   EgUSapCb       *uSapCb;       /**< Associated USAP : will be the default USAP */
   EgTSapCb       *tSapCb;       /**< Associated TSAP : will be the default USAP */
#ifdef EGTP_C
   CmHashListCp   msgHlCp;       /**< Message hash list control point: key 
                                    seq number, to queue UDP signalling 
                                    messages only */
   CmHashListCp   teidMsgHlCp;   /**< Message hash list control point: key 
                                    TEID, to queue Command messages only.
                                  * Also this will be populated for Default
                                  * TPT server only */
#endif /* EGTP_C */
   EgTimer        opnSrvTmrNode; /**< Open Server Timer control block*/
   S16            opnSrvCurRetryCnt; /**< Tpt Srv open current retry     *
                                       *  attempt                       */
/*eg004.201 Modified for PSF :PSF Control block added*/
#ifdef HW
   CmPFthaRsetMap  egHwCb;
#endif
};

/*eg004.201 Modified for PSF RUG:EgVerInfoCb structure to control Rolling Upgrade
 *introduced with interfaceInfo and PSF control block object
 */
#ifdef TDS_ROLL_UPGRADE_SUPPORT
#ifdef EG_RUG
typedef struct _egVerInfoCb
{
   ShtVerInfo intfInfo;
#ifdef HW
   CmPFthaRsetMap egHwCb;
#endif /* HW */
}EgVerInfoCb;
#endif /* EG_RUG */
#endif /* TDS_ROLL_UPGRADE_SUPPORT */

#ifdef EGTP_C
/*********************************************************************
* Tunnel Management structures                                       *
**********************************************************************/
/**
  @defgroup TUNNMNGMT The Tunnel Management
  @brief The Tunnel Management module handles the tunnel
                management for GTP layer.
                It creates, maintains and destroys tunnel contexts.
*/
/**
  @ingroup TUNNMNGMT
    @struct egIncReq
    @brief Node for the list of incoming requests on a tunnel to save its 
           source details so that the response is sent to the same address
 */
typedef struct egIncReq
{
   CmHashListEnt  egIncReqHlEnt; /**< Entity for Hash List manipulation */
   U32        seqNum;     /**< Sequence number of the incoming request */
   CmTptAddr  locAddr;    /**< dest IP-address and port of the
                            incoming message */
   CmTptAddr  remAddr;    /**< source IP-address and port of the
                            incoming message */

   U8         reqType;    /**< GTP-C Message Type of the incoming request */
/*eg004.201 Modified for PSF:local teid added to associate the respective egIncReq
 * with the teid;egHwCb added as the PSF control block
 */
#ifdef HW
   U32        localTeid;  /**< Associtated Teid */
   CmPFthaRsetMap  egHwCb;
#endif
}EgIncReq;


/* eg014.201:  ccpu00139510 Egtpc release 10 upgrade  */
#ifdef EGTP_C
/**@struct _egEvnt
   @brief Event Structure for eGTP */
typedef struct _egEvnt
{
   U8          msgType;    /**< Msg Type */
   U32         seqNumber;  /**< Sequence Number */
   TknU32      lclTeid;    /**< Local Tunnel Identifier */
   TknU32      transId;    /**< Transaction Identifier */
   TknU8       crtFlag;    /**< Create tunnel or not */
   CmTptAddr   localAddr; /**< Local node address */
   CmTptAddr   remAddr;   /**< Remote node address */
   EgTptSrvCb  *srvCb;
   TmrCfg      t3;         /**< Request timer expiry */
   U8          eventType;  /**< Event Type */
   U8     intfType;   /**< This info will be used by EDM for getting the message DB */
   S16  resultType;         /**< Stores the result for sending to application
                             * can be an error also. */
   U8  failedIeInst;       /**< stores the failed IE Inst */
   U8  failedIeType;       /**< stores the failed IE type */
   U8  failedMsgType;      /**< stores the failed msg type */
   Bool isitPbMsg;         /**< is it a piggy backed message? */
   union
   {
      Buffer *mBuf;        /**< Msg Buffer -- For Decoding */
      EgMsg  *egMsg;       /**< Msg -- For Encoding */
   }u;
} EgEvnt;
#endif /* EGTP_C */

/** 
  @ingroup TUNNMNGMT
    @struct egTECCb
    @brief Control Block for Tunnel endpoint
 */
typedef struct egTECCb
{
   EgtTunnState state;    /**< State of the tunnel*/
   EgUSapCb     *usap;    /**< entity, interface, source port (srvCb)*/
   EgtIntf      intfType; /**< EUTRAN interface type of the tunnel */
   U32          locTeid;  /**< Local Teid of the tunnel */
   U32          transId;  /**< temporary ID given by user while creating the
                           * tunnel */
   CmTptAddr    destIpAddr;  /**< remote IP-address and port */
   /*eg010.201:Buffer to store curr msg which will piggyback next msg*/
#ifdef EG_PIGGYBACK_SUPP
   EgEvnt        *tempPiggyEvnt;
   U32            piggyBackStatus;  /**< stores status for piggybacking*/
   /* eg012.201: Enhanced the piggy backing feature by adding  *
    * the Timer for waiting for the Piggibacked SigReq Message */ 
   EgTimer       tmrDisPbSigRsp;    /**<timer for piggibacking */
   U16           timeToWaitPbTmr;
#endif
#ifdef PHASE2
   CmTptAddr    locAddr;  /**< Local IP-address and port */
   TmrCfg       retrans;  /**< retransmission timer details */
#endif /* PHASE2 */
   CmHashListCp incReqCp; /**< sequence number*/
/*eg004.201 Modified for PSF:PSF Control block object introduced in EgTECCb*/
#ifdef HW
   CmPFthaRsetMap  egHwCb;
#endif
}EgTECCb;

/**
  @ingroup TUNNMNGMT
    @struct egTunnCb
    @brief Control block for array Tunnel endpoint and parameters to handle 
        TEID generation.
 */
struct egTunnCb
{
   CmLListCp freeTeidLst;    /**< Array to manage free tunnel enpoint ID list */
   EgTECCb   ***teCCbArr;      /**< 2 D Array with index as local TEID */
   U32       freeArrIdx;     /**< maintain free TEIDs */
   U32       arrNum;         /**< Number of arrays maintains for TEIDs */
};

#endif /* EGTP_C */
/******* EGTP Global Control Block *******/
/**
    @struct _egCb
    @brief Control block for EGTP global control block
 */
/* eg006.201: Parameter, echoSeqNum, indictes Echo Sequence Number */
/*eg007.201: egCb updated to accomodate list of tSaps as tSapLst
             dstCbList, and echoSap ptr is removed --*/
typedef struct _egCb
{
   TskInit         init;               /**< Task initialization info. */
   EgGenCfg        genCfg;             /**< general configuration */
   EgGenSts        genSts;             /**< general statistics */
   EgGenSta        genSta;             /**< General status */
   EgUSapCb        **egtSapLst;        /**< Array of pointers to upper SAPs */
#ifdef EG_MULTI_IP
   EgTSapCb       **tSapLst;         /**< Pointer to Transport SAP Control Block */
#else
   EgTSapCb        *tSapCb;            /**< Pointer to Transport SAP Control Block */
#endif
#ifdef EGTP_C
#ifndef EG_MULTI_IP
   EgUSapCb        *echoSap;           /**< Special Usap to receive incoming echo
                                        * requests */
#endif
   U32             numTnl;             /**< Number of active tunnels */
   EgTunnCb        tnlCb;              /**< Array of pointers to Tunnel CBs */
#ifndef EG_MULTI_IP 
   CmHashListCp    dstCbList;          /* List of destination control blocks */
#endif

#ifdef EGTP_C
   Bool            echoTmrStarted;
   U32             echoSeqNum;      /**< Echo Request Sequence Number Indicator */
   CmTimer         pathTimers;         /**< Path timer */
#endif
#ifdef SS_MULTIPLE_PROCS
   EdmCb         edmCb;               /* ENCODER-DECODER CONTROL BLOCK */
#endif
#endif /* EGTP_C */
#ifdef SS_MULTIPLE_PROCS
   U8            used;
#endif
   CmTqCp          egTqCp;             /**< timing queue control point */
   CmTqType        egTq[EG_TQSIZE];    /**< timing queue */
   U8              outOfRsrc;          /**< out of resources */
   Bool            shutdown;           /**< Layer is shutting down             */
   Bool            ieCfgDone;          /**< IE Config is done or not? */
#ifdef EGTP_U
  EgUcb          egUCb;                /**< EGTP-U Control Block */
#endif /* EGTP_U */
/*eg004.201 Added for PSF RUG- New variables introduced
 *numIntfInfo - Number of stored interface information
 *egVerInfoCb - Interface Version information store
 */
#ifdef EG_RUG
   U16           numIntfInfo;    /* Number of stored interface information */
   EgVerInfoCb   *egVerInfoCb;   /* Interface Version information store */
#endif /* EG_RUG */
#ifdef HW
   CmPFthaRsetMap  egHwCb;
#ifdef SS_MULTIPLE_PROCS
   Void *psfCb;
#endif
#endif
} EgCb;



#ifdef EGTP_U
/* In case of multi threading this structure is used by main threaed to  *
 * send information to worker threaded */
typedef struct _eguEvnt
{
   U8           msgType;    /**< Msg Type */
   U16          seqNumber;  /**< Sequence Number */
   U32          teid;    /**< Local Tunnel Identifier */
   CmTptAddr    localAddr; /**< Local node address */
   CmTptAddr    remAddr;   /**< Remote node address */
   EgTptSrvCb   *srvCb;
   U8           intfType;   /**< This info will be used by EDM */
   S16          resultType; /**< Stores the result for sending to application */
   Region       region;
   Pool         pool;
   U8           failedIeInst;       /**< stores the failed IE Inst */
   U8           failedIeType;       /**< stores the failed IE type */
   U8           failedMsgType;      /**< stores the failed msg type */
   U16          maxTPduSize;       
   U16          tPduSize;           
   Bool         isEnc;
   U8           eventType;  /**< Event Type */
   EgTeIdCb     *teIdCb;
   EgDestAddrCb *destAddrCb;   
   EgIpCb       *ipCb;   
   EgUSapCb     *usap;
   EgTSapCb     *tsap;
   EgMngmt      *lmInfo;    /**< confirm structure */
   Bool         errInd;   /*<Flag to tell if Error Indication needs to be
                           generated>*/
   Bool         suppExtHdrNtf; /*<Flag to tell whether Supp Ext Hdr Not message is
                                 generated by GTP-U later or Application>*/
#ifdef EGTP_U_MULTI_THREADED
   EgUThreadCb  *threadCb;
#endif
   Buffer    *mBuf;        /**< Msg Buffer -- For Decoding */
   EgUMsg    *egMsg;       /**< Msg -- For Encoding */
}EgUEvnt;

#endif /* EGTP_U */

#ifdef SS_MULTIPLE_PROCS
EXTERN EgCb  egCbLst[EG_MAX_INSTANCES];
EXTERN EgCb  *egCbPtr;
#else
EXTERN EgCb  egCb;
#endif /* SS_MULTIPLE_PROCS */


#ifdef EGTP_C
/*********************************************************************
 **************************** Transport(TPT) Module ******************
 ********************************************************************/
   /**
    * @defgroup TPTMOD The Transport Module
    */
    /**
    * @ingroup TPTMOD
    * @brief This function receive the EGTP Message from Lower layer
    * @details This function is called by Lower Layer when ever a new
    * EGTP messages is received from TUCL. 
    *
    * @param[in] serverCb Pointer to the Transport Server Control Block. 
    *
    * @param[in] srcAddr  Pointer to the Source Address from where 
    *                     Msg is received. 
    *
    * @param[in] mBuf Pointer to received Message from Peer. 
    *
    * @param[in] pbVal Bool Flag to indicate whether PiggyBack check to be done. 

    * @return  ROK - Success, RFAILED - Failure 
    */
EXTERN S16 egTptGetMsg ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           CmTptAddr       *srcAddr, 
                           Buffer          *mBuf,
                           Bool            pbVal));

   /**
    * @ingroup TPTMOD
    * @brief This function process the EGTP Message received from EDM.
    * @details Ths function will process the EGTP Message to be sent to Peer.
    * This function is called by the Encoder/Decoder Module for 
    * sending the Encoded EGTP Message to be sent to Peer. 
    *
    * @param[in] egEvnt  Pointer to the encoded Message to be sent to Peer
    *
    * @return  ROK - Success, RFAILED - Failure 
    */
#ifndef EG_MT
EXTERN S16  egTptProcessMsg ARGS (( EgEvnt *egEvnt));
#else
EXTERN S16  egTptProcessMsg ARGS (( Pst  *pst, EgEvnt *egEvnt));
#endif /* EG_MT */

/**
    * @ingroup TPTMOD
    * @brief This function handles the Local tunnel Delete Request
    * @details This function handles the Local tunnel Delete Request 
    *
    * @param[in] serverCb Pointer to the Transport Server Control Block. 
    *
    * @param[in] teid Local Tunnel Id of Message 
    *
    * @return  ROK - Success, RFAILED - Failure 
    */

EXTERN S16  egTptProcessLclDel ARGS (( EgTptSrvCb  *serverCb, U32   teid));

#endif /* EGTP_C */
/**
    * @ingroup TPTMOD
    * @brief This function initializse the Transport Module of EGTP
    * @details This function initializse the Transport Module of EGTP
    *
    * @return  ROK - Success, RFAILED - Failure 
    */

EXTERN S16  egTptInit ARGS (( Void ));
/**
    * @ingroup TPTMOD
    * @brief This function deinitializse the Transport Module of EGTP
    * @details This function deinitializse the Transport Module of EGTP 
    *
    * @return  ROK - Success, RFAILED - Failure 
    */

EXTERN Void egTptDeInit ARGS (( Void ));
/**
    * @ingroup TPTMOD
    * @brief This function receive the EGTP Message from Lower layer
    * @details This function is called by Lower Layer when ever a new
    * EGTP messages is received from TUCL. 
    *
    * @param[in] localAddr Pointer to the Transport Server Address. 
    *
    * @param[out] state  Pointer to the State of Tranport Server 
    *
    * @return  ROK - Success, RFAILED - Failure 
    */

EXTERN S16  egTptState ARGS (( CmTptAddr  localAddr, TknU8 *state));

/**
    * @ingroup TPTMOD
    * @brief This function perform the given action on Transport Servers
    * @details This function perform the given action on Transport Servers
    *           in  Given Tranport SAP.
    *
    * @param[in] tsapCb Pointer to the Transport SAP Control Block. 
    *
    * @param[in] srvAction  Action to be performed on Transport Servers
    *
    * @return  ROK - Success, RFAILED - Failure 
    */

EXTERN S16  egTptProcessTsapServers ARGS ((
                             EgTSapCb    *tsapCb,
                             EgTptSrvAct srvAction));

/**
    * @ingroup TPTMOD
    * @brief This function handles the opening of Transport Server
    * @details This function handles the opening of Transport Servert
    *
    * @param[in] serverCb Pointer to the Transport Server Control Block. 
    *
    * @param[in] reason  Pointer to the reason
    *
    * @return  ROK - Success, RFAILED - Failure 
    */

EXTERN S16  egTptOpenServer ARGS ((
                             EgTptSrvCb    *serverCb,
                             U16           *reason));

/**
    * @ingroup TPTMOD
    * @brief This function handles closing of Transport Server
    * @details This function handles closing of Transport Server 
    *
    * @param[in] serverCb Pointer to the Transport Server Control Block. 
    *
    * @return  Void
    */

EXTERN Void  egTptCloseServer ARGS ((
                             EgTptSrvCb    *serverCb ));

/**
    * @ingroup TPTMOD
    * @brief This function frees the Transport Server
    * @details This function frees the Transport Server 
    *
    * @param[in] serverCb Pointer to the Transport Server Control Block. 
    *
    * @return  Void 
    */

EXTERN Void  egTptFreeServer ARGS ((
                             EgTptSrvCb    *serverCb ));

/*********************************************************************
 **************************** MI Module ***************************
 ********************************************************************/
/**
  @brief The indication of transport server open is handled.
                                                                                                 
  @details This Function handles the sending of unsolicited status alarm towards layer 
manager indicating the success of server open event.

  @param[in] srvCb The transport server control block. <br>
  It is an IN parameter for this method.
                                                                                                 
  @return   void <br>

  */

EXTERN Void egMiTptSrvOpenInd ARGS((
               EgTptSrvCb   *srvCb
               ));

/**
  @brief The indication of transport server close is handled.
                                                                                                 
  @details This Function handles the sending of unsolicited status alarm towards layer
manager indicating the success of server close event.
                                                                                                 
  @param[in] srvCb The transport server control block. <br>
  It is an IN parameter for this method.
                                                                                                 
  @return   void <br>
                                                                                                 
  */

EXTERN Void egMiTptSrvCloseInd  ARGS ((
                                   EgTptSrvCb   *serverCb,
                                   Bool            fromLMI  ));

/**
  @brief The memory for the requested size is allocated.
                                                                                                 
  @details This Function handles the allocation and initialization of memory from the 
reserved memory for the layer. The memory avalability is checked before allocation.

  @param[in] size The number of bytes to be allocated. <br>
  It is an IN parameter for this method.
                                                                                                 
  @return   Pointer - Static buffer pointer <br>
            NULLP - Memory allocation failure <br>
                                                                                                 
  */

EXTERN Data *egAlloc ARGS
((
Size size                                  /* size of the buffer required */
));

/**
  @brief The memory for the requested size is deallocated.
                                                                                                 
  @details This Function handles the deallocation of memory from the specified memory
location.

  @param[in] pCb The start of the memory location. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] size The number of bytes to be deallocated. <br>
  It is an IN parameter for this method.
                                                                                                 
  @return   ROK - Deallocation success. <br>

  */

EXTERN S16 egFree ARGS
((
Data *pCb,                                 /* pointer to the buffer to be deallocated */
Size size                                  /* size of the buffer */
));

/**
  @brief The unsolicited status indications are sent to layer manager.
                                                                                                 
  @details This Function handles the sending of unsolicited status towards layer manager.
The function is called only when unsolicited status is enabled for the layer.
                                                                                                 
  @param[in] category The category of unsolicited event. <br>
  It is an IN parameter for this method.

  @param[in] event The unsolicited event occurred. <br>
  It is an IN parameter for this method.

  @param[in] cause The cause for the unsolicited event. <br>
  It is an IN parameter for this method.

  @param[in] dgn The diagnostic parameters for the alarm. <br>
  It is an IN parameter for this method.

  @return   ROK - Alarm sending success. <br>

  */

EXTERN S16 egSendLmAlarm ARGS
((
U16           category,                    /**< category */
U16           event,                       /**< event */
U16           cause,                       /**< cause */
EgUstaDgn     *dgn                         /**< diagnostics structure */
));

/**
  @brief The confirms for the layer manager requests are sent.
                                                                                                 
  @details This Function handles the sending of solicited confirms towards layer manager.
It sends the confirms for configuration, control, status, statistics requests sent from
layer manager.

  @param[in] pst The layer manager post structure. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] hdr The header for the confirm. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] type The type of confirm. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] cfm The confirmation parameters. <br>
  It is an IN parameter for this method.
                                                                                                 
  @return   ROK - Alarm sending success. <br>
            RFAILED - Alarm sending failure. <br>
                                                                                                 
  */

EXTERN S16 egSendLmCfm ARGS
((
Pst              *pst,                     /* post structure */
Header           *hdr,                     /* header */
U8               type,                     /* type of confirm */
EgMngmt          *cfm                      /* confirm structure */
));

/**
  @brief The control confirm for the layer manager requests is sent.
                                                                                                 
  @details This Function handles the sending of solicited control confirm towards layer 
manager.

  @param[in] pst The layer manager post structure. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] status The status of control request event for the confirm. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] reason The reason of control request event for the confirm. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] hdr The header for the control confirm. <br>
  It is an IN parameter for this method.
                                                                                                 
  @return   void <br>
                                                                                                 
  */

EXTERN Void egSendLmCntrlCfm  ARGS((Pst      *pst,
                                    U16      status,
                                    U16      reason,
                                    Header   *hdr));

/**
  @brief The message trace indications are sent towards the layer manager.
                                                                                                 
  @details This Function handles the sending of message trace indications towards layer
manager. It also provides the event information and the source and remote addresses.
                                                                                                 
  @param[in] tsapCb Pointer to the Transport SAP Control Block. 

  @param[in] elmnt The type of element. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] srcAddr The source address. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] destAddr The destination address. <br>
  It is an IN parameter for this method.
                                                                                                 
  @param[in] evnt The trace event type indication. <br>
  It is an IN parameter for this method.

  @param[in] mBuf The message sent or received. <br>
  It is an IN parameter for this method.

  @return   void <br>
                                                                                                 
  */

/* eg009.201: Modified proc to accept TSAP cb as argument */
EXTERN Void egGenTrcInd ARGS
((
EgTSapCb *tSapCb,                          /* TSap associated          */
S16      elmnt,                            /* type of element to trace */
CmTptAddr *srcAddr,                        /* Destination IP-address/port */
CmTptAddr *destAddr,                       /* Destination IP-address/port */
U16      evnt,                             /* trace event type */
Buffer   *mBuf                             /* message being traced */
));


/*********************************************************************
 **************************** TIMER Module ***************************
 ********************************************************************/
EXTERN S16   egSchedTmr       ARGS ((Ptr egHandle,
                                     S16 tmrEvnt,
                                     Action action, U32 tmrVal));

EXTERN Void   egTmrEvnt       ARGS ((Ptr cb,
                                     S16 event));

EXTERN Void egCmInitTimer          ARGS((EgTimer *tmr));

EXTERN S16 egRegInitTmr          ARGS((Void));

EXTERN Void egRmvTmr ARGS (( PTR      cbp,
                             CmTimer  *pTimerStruct,
                             U8       tmrNum,      
                             U8       maxNmbTmrs));

/*********************************************************************
 **************************** Debug Prints ***************************
 ********************************************************************/
EXTERN Void egCmPrntDbg ARGS(( U8    ptrType,
                               void  *ptr,
                               S8    *name
                               ));


#ifdef EGTP_C
EXTERN S16 egCmCreateEvent ARGS((EgEvnt **newEvent ));

EXTERN S16 egCmFreeEvent   ARGS((EgEvnt *event ));

EXTERN Void   egPrintEventStructure ARGS ((EgEvnt    *evnt ));
#endif /* EGTP_C */

/*********************************************************************
*             ENCODER     AND       DECODER       FUNTIONS           *
 ********************************************************************/

EXTERN S16  egEdmDeInit ARGS
((
Void
));

EXTERN S16  egAllocIntfLst ARGS
((
U8   nmbIntf          /* Contains the number of interfaces         */
));


EXTERN Size  egGetIntfMem ARGS
((
Void
));

#ifdef EGTP_C
#ifdef EG_MT
EXTERN  S16 egEncMsgReq ARGS(( Pst  *pst, EgEvnt *evntMsg));

EXTERN  S16 egDecMsgReq ARGS(( Pst  *pst, EgEvnt *evntMsg));
#else
EXTERN  S16 egEncMsgReq ARGS(( EgEvnt *evntMsg));

EXTERN  S16 egDecMsgReq ARGS(( EgEvnt *evntMsg));
#endif /* EG_MT */

/*eg004.201 Added for PSF RUG Support:New function to get the interface version*/
#ifdef EG_RUG
EXTERN S16 egGetVer ARGS((ShtGetVerCfm *getVerCfmInfo));
EXTERN S16 egSetVer ARGS((ShtVerInfo *shtVerInfo, CmStatus *status,
                                 U16 *index));
#endif /* EG_RUG */

EXTERN  S16 egEncPthReq ARGS(( EgEvnt *evntMsg));

EXTERN  S16 egDecPthReq ARGS(( EgEvnt *evntMsg));

/*********************************************************************
 *             Tunnel Managaement Module                             *
 *********************************************************************/ 
/**
  @ingroup TUNNMNGMT
  @brief The function egChkFailInd matches the command messages with
         the corressponding failure messages. This is called after
         egGetAssocMsg fails to find the match.
  @param[in] msgType The msgtype is the one of the message type to be matched.
  @param[in] pairMsgType The pairMsgtype is the one of the message type to be matched.
  @return
  - ROK - If the procedure was successful
  - ROUTRES - If the procedure failed due to memory failure.
  - RFAILED - If the procedure failed due to internal reasons.
 */
EXTERN S16 egChkFailInd ARGS((
U8   msgType,
U8   pairMsgType
));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunnInit initializes the global array of teids.
  @return
  - ROK - If the procedure was successful
  - ROUTRES - If the procedure failed due to memory failure.
  - RFAILED - If the procedure failed due to internal reasons.
 */
EXTERN S16 egTunnInit ARGS((Void));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunnDeInit un-initializes the global array of teids.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egTunnDeInit ARGS((Void));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunSend sends a GTP-C message through the tunnel
          FSM to the transport. For a fresh Create Session Request it 
          creates a tunnel. Echo Request and Responses are also handled
          independent of tunnel FSM.
  @param[in] usap The usap is the pointer to the usap control block representing
              the interface on which message is sent. 
  @param[in] event The event is the event representing the GTP-C message the 
                user requested to send.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egTunSend ARGS((
                        EgUSapCb *usap,
                        EgEvnt *msg
                        ));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunDelete is called by the user whenever the User wants
  destroy a tunnel context locally.
  @param[in] usap The usap is the control block of the SAP of the interface to which
              the tunnel belongs to.
  @param[in] localTeid The localTeid is the local TEID of the tunnel to be deleted.
  @param[in] transId The transId is the transaction ID given by the the user to 
                while creating the tunnel. This is used when the user deletes
                the tunnel even before the tunnel ID is given.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egTunDelete ARGS((
                          EgUSapCb *usap,
                          U32 localTeid,
                          U32 transId
                          ));
/**
  @ingroup TUNNMNGMT
  @brief The function egTunModify is called by the user whenever the User wants
  modify a tunnel context locally.
  @param[in] usap The usap is the control block of the SAP of the interface to which
              the tunnel belongs to.
  @param[in] localTeid The localTeid is the local TEID of the tunnel to be deleted.
  @param[in] transId The transId is the transaction ID given by the the user to 
                while creating the tunnel. This is used when the user deletes
                the tunnel even before the tunnel ID is given.
  @param[in] oldDstIpAddr Old Destination IP Address
  @param[in] newDstIpAddr New Destination IP Address
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egTunModify ARGS((
                          EgUSapCb *usap,
                          U32 localTeid,
                          U32 transId,
                          CmTptAddr oldDstIpAddr,
                          CmTptAddr newDstIpAddr
                          ));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunIndicate is called by the transport or 
        encoder/decoder modules whenever the PCM has to be indicated with any
        failure events.
  @param[in] usap The usap is the control block of the SAP representing the 
                interface to which the tunnel belongs to.
  @param[in] localTEID The localTEID is the tunnel ID for which the indication is sent.
  @param[in] event The event contains the details about the failure.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egTunIndicate ARGS((
                            EgUSapCb *usap,
                            U32 localTEID,
                            EgEvnt *evnt
                            ));

/**
  @ingroup TUNNMNGMT
  @brief The function egTunnState returns the state of a tunnel given the tunnel
        ID.
  @param[in] teid The teid is the tunnel whose state is queried.
  @param[out] state The state is the state of the tunnel returned.
              -   EGT_TUN_STATE_IDLE
              -   EGT_TUN_STATE_INCSESSREQ
              -   EGT_TUN_STATE_OUTSESSREQ
              -   EGT_TUN_STATE_CONNECTED
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egTunnState ARGS((
                         U32 teid,
                         EgtTunnState *state
                         ));

/**
  @ingroup TUNNMNGMT
  @brief The function egInitEchoUsap initializes the hash list in the usap
         meant for Echo mechanism.
  @param[in] usap The usap is the pointer to the usap control block meant for Echo
                mechanism.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egInitEchoUsap ARGS((
                            EgUSapCb *usap
                            ));

/**
  @ingroup TUNNMNGMT
  @brief The function egDeInitEchoUsap un-initializes the hash list in the usap
         meant for Echo mechanism.
  @param[in] usap The usap is the pointer to the usap control block meant for Echo
                mechanism.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 */
EXTERN S16 egDeInitEchoUsap ARGS((
                               EgUSapCb *usap
                               ));

/**
  @ingroup TUNNMNGMT
  @brief The function  egTunReceive is called by transport when ever a
        GTPC message is received.
  @param[in] event The event is the event representing the incoming message.
  @return 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
*/
EXTERN S16 egTunReceive ARGS((
                                EgEvnt *msg
                                ));

/**
  @ingroup TUNNMNGMT
  @brief The function egGetIntfFrmTeid is a utility to get the interface
        type of a tunnel given its TEID.
  @param[in] The teid is the TEID of the tunnel whose interface is to be returned.
  @return -
       - EGT_GTP_INT_UNKNWN
       - EGT_GTP_INT_S5_S8
       - EGT_GTP_INT_S11
*/
EXTERN EgtIntf egGetIntfFrmTeid ARGS((
                                U32 teid,
                                Bool crtFlag
                                ));

/**
  @ingroup TUNNMNGMT
  @brief The function egSendErrInd is an utility to send an error 
         indication to the user.
  @param[in] event The event isthe pointer to the event for which the error
                has occured.
  @param[in] usap The usap is the pointer to the SAp on which the error has
                 occured.
*/
EXTERN Void egSendErrInd ARGS ((
                                EgEvnt *event,
                                EgUSapCb *usap
                                ));

/**
  @ingroup TUNNMNGMT
  @brief The function egGetUsapFrmIntf returns a pointer to the usap control
        block given its EUTRAN interface type.
  @param[in] intfType The intfType is the GTP-C interface type.
 */
EXTERN EgUSapCb *egGetUsapFrmIntf ARGS((
                                     EgtIntf intfType
                                     ));

/**
  @ingroup TUNNMNGMT
  @brief The function egGetMsgCategory is an utility to get the category
         of a GTP-C message, namely, request, response, command,
         notification, acknowledge and indication.
  @param[in] msgType The msgType is the GTP-C message type.
  @return The function returns the enumerated value of the message category.
   - EG_GTPC_MSGCAT_REQUEST,
   - EG_GTPC_MSGCAT_RESPONSE,
   - EG_GTPC_MSGCAT_NOTFN,
   - EG_GTPC_MSGCAT_ACKN,
   - EG_GTPC_MSGCAT_COMMAND,
   - EG_GTPC_MSGCAT_IND,
   - EG_GTPC_MSGCAT_OTHER

*/
EXTERN EgtMsgCat egGetMsgCategory ARGS((
                                        EgtMsgType msgType
                                        ));

/**
  @ingroup TUNNMNGMT
  @brief The function egGetAssocMsg is an utility to get the
         corresponsding GTP-C message type for one type.
  @param[in] msgType The msgType is the type of the message whose associated
                message has to be returned.
  @return The enumerated message type.
        - ZERO                  - If the input is invalid message
        - EGT_GTPC_MSG_ECHO_REQ - if input is EGT_GTPC_MSG_ECHO_RSP.
        - EGT_GTPC_MSG_MB_CMND  - if input is EGT_GTPC_MSG_UB_REQ.
        - so on...
*/
EXTERN EgtMsgType egGetAssocMsg ARGS((
                                   EgtMsgType msgType
                                   ));

/**
  @ingroup TUNNMNGMT
  @brief The function egValidateMsgInt is a utility to evaluate if a 
        message type is valid on a given interface, originating entity
        and terminating entity.
  @param[in] msgType - The msgType is the GTP-C message Type.
  @param[in] origEnt - The origEnt is the originating entity type.
  @param[in] termEnt - The termEnt is the terminating entity type.
  @return -
        - TRUE  - If the message is valid.
        - FALSE - If the message is valid.
*/
EXTERN Bool egValidateMsgInt ARGS((
                                EgtMsgType msgType,
                                LegEntity origEnt,
                                LegEntity termEnt,
                                EgtIntf intf
                                ));


/*********************************************************************
 *             CPM MODULES                                           *
 *********************************************************************/ 
/* EXTERN S16 egActvInit ARGS((Ent ent, Inst inst, Region region, Reason reason)); */

EXTERN S16 egCpmSigHdl ARGS((EgUSapCb *uSapCb, EgEvnt *egEvnt));
EXTERN S16 egCpmStaReq ARGS((EgUSapCb *uSapCb,U8 eventType,EgPathInfo *pathInfo));
EXTERN S16 egCpmLclDelReq ARGS((EgUSapCb *uSapCb,U32 transId,U32 teid));
EXTERN S16 egCpmLclModReq ARGS((EgUSapCb *uSapCb,U32 transId,U32 teid, \
                               CmTptAddr oldDstIpAddr, CmTptAddr newDstIpAddr));
#ifdef EGTP_C_PTH_MGT
EXTERN S16 egSendEchoReq ARGS((Void));
EXTERN S16 egSendPathRsp ARGS
((
 EgEvnt *event
));

#endif
/*******************************************
 Function to fill Evt Structure for SigReq *
 *******************************************/

EXTERN S16 egUiFillEvntStrSigReq ARGS
((
EgEvnt      *egEvnt,       /* Event Structure */ 
CmTptAddr   *localAddr,    /* Local node address */
CmTptAddr   *remAddr,      /* Remote node address */
TknU32      lclTeid,       /* Local Tunnel Identifier */
TknU32      transId,       /* Transaction Identifier */
TknU8       crtFlag,       /* Create tunnel or not */
TmrCfg      t3,            /* Request timer expiry */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
));

/*eg004.201 Modified for eGTP-C PSF Upgrade:Function Prototype qualifier changed from PUBLIC to EXTERN*/
EXTERN S16 egTptQueueMsgRmvWthMsb ARGS
((
EgTptSrvCb     *dfltSrvCb,  /* Server connection of received message */
U32            teid,              /* Local TEID for Received message */
U32            seqNo,                /* Seq. Num of Received message */
Bool           isSendInd      /* To send the Error Indication to PCM */
));


#ifdef EGTP_C_PTH_MGT
EXTERN S16 egTptQueueMsgRmv ARGS
((
EgTptSrvCb     *serverCb, /* Server connection that received message */
U32             seqNo,                /* Seq No for Received message */
Bool            isSendInd,    /* To send the Error Indication to PCM */
CmTptAddr      *destAddr
));
#else
EXTERN S16 egTptQueueMsgRmv ARGS
((
EgTptSrvCb     *serverCb, /* Server connection that received message */
U32             seqNo,                /* Seq No for Received message */
Bool            isSendInd     /* To send the Error Indication to PCM */
));
#endif

/* eg010:201 : egTptHdlOutPBMsg prototype added */
#ifdef EG_PIGGYBACK_SUPP
EXTERN S16 egTptHdlOutPBMsg ARGS 
((
EgTECCb     **teCCb,        /* Tunnel Control Block */
EgEvnt       *egCurrEvnt    /* current event structure */ 
));
#endif /* EG_PIGGYBACK_SUPP */

#endif /* EGTP_C */

/**
 @brief The function egCpyCmTptAddr is the utility to copy the CmTptAddr
        from source to destination.
 @param[in] dst The pointer to the destination CmTptAddr.
 @param[in] src The pointer to the source CmTptAddr.
 @return 
          - ROK      - If the procedure was successful.
          - ROUTRES  - If the procedure failed due to resource failure.
          - RFAILED  - If the procedure failed due to internal reasons.
*/
EXTERN S16 egCpyCmTptAddr ARGS((
                                CmTptAddr *dst,
                                CmTptAddr *src
                                ));

 /**
    * @ingroup TPTMOD
    * @brief This function compares two Transport Addresses 
    * @details This function compares two Transport Addresses
    *
    * @param[in] one Pointer to the Transport Address
    * @param[in] two Pointer to the Transport Address
    * @param[out] addressMatch Pointer to result indicating whether addresses
    *                          matches or not 
    *
    * @return  Void
    */

EXTERN S16 egCmpTptAddr            ARGS((CmTptAddr  *one,
                                         CmTptAddr  *two,
                                         Bool       *addressMatch));

/*********************************************************************
                  Timer Functions
**********************************************************************/

#ifndef SS_MULTIPLE_PROCS
EXTERN S16 egActvTmr ARGS((Void));
#else
EXTERN S16 egActvTmr ARGS((ProcId proc, Ent ent, Inst inst));
#endif

EXTERN S16 egTptSendMsg ARGS ((EgTptSrvCb *serverCb, CmTptAddr *destAddr, Buffer *mBuf, Pst *pst));

EXTERN S16 egCpmInit ARGS((EgMngmt *cfgReq, EgModuleName module));

EXTERN S16 egCpmDeinit ARGS((Void));

EXTERN S16 egInitExt ARGS((Void));

EXTERN S16 egGetSId ARGS((SystemId *sysId));
/**
  @ingroup TUNNMNGMT
  @brief The function egUpdateTxStatistics is an utility to update the
        SAP statistics for transmitted message.
  @param[in] msgType The msgType is the type of the sent messasge.
  @param[in] sts The sts is the pointer to the Sap statistics on which the 
                message was sent.
*/
EXTERN Void egUpdateTxStatistics ARGS ((
                                   EgtMsgType msgType,
                                   EgSapSts *sts
                                   ));

/**
  @ingroup TUNNMNGMT
  @brief The function egUpdateRxStatistics is an utility to update the
        SAP statistics for received message.
  @param[in] msgType The msgType is the type of the received messasge.
  @param[in] sts The sts is the pointer to the Sap statistics on which the 
                message was received.
*/
EXTERN Void egUpdateRxStatistics ARGS ((
                                        EgtMsgType msgType,
                                        EgSapSts *sts
                                        ));


EXTERN S16 egTptUdpServerDatReq ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           CmTptAddr       *destAddr,
                           Buffer          *mBuf,
                           Pst             *pst));

#ifdef EGTP_U
   /**
    * @defgroup EDMMOD Encoder-Decoder Module
    */
    /**
    * @ingroup EDMMOD
    * @brief This function encodes the EGTP-U Message
    * @details  This function is entry function for EDM module on 
    *           Encoding side. It is called by CPM module for encoding
    *           a message received from the application. 
    *
    * @param[in] evntMsg Event structure containing the EGTP-U message
    *                    to be encoded
    *
    * @return  ROK - Success, Error Code for Failure 
    */

EXTERN S16 egUEncMsgReq ARGS((EgUEvnt *evntMsg));

    /**
    * @ingroup EDMMOD
    * @brief This function decodes the EGTP-U Message
    * @details  This function is entry function for EDM module on 
    *           Decoding side. It is called by CPM module for decoding
    *           a message received from the lower layer. 
    *
    * @param[in] evntMsg Event structure containing the mBuf to be encoded
    *
    * @return  ROK - Success, Error Code for Failure 
    */
EXTERN S16 egUDecMsgReq ARGS((EgUEvnt *evntMsg));

EXTERN S16 egUTptGetMsg ARGS((EgTptSrvCb *serverCb, CmTptAddr *srcAddr, Buffer *mBuf, Pst *pst));
EXTERN S16 egUCmCreateEvent ARGS((EgUEvnt **newEvent)); 
EXTERN S16 egTunProcEguDatReq ARGS((EgtUEvnt *egtUEvnt, EgUSapCb *usap, Pst *pst));
EXTERN S16 egTunProcEguTnlMgmtReq ARGS((EgtUEvnt *egtUEvnt, EgUSapCb *usap));
EXTERN S16 egUTunnTeidMod ARGS ((EgtUEvnt *egtUEvnt, EgIpCb *ipCb));
EXTERN S16 egUTunnTeidDel ARGS((EgtUEvnt *egtUEvnt, EgIpCb *ipCb, EgUSapCb *usap));
EXTERN S16 egUTunnTeidReordEna ARGS((EgtUEvnt *egtUEvnt, EgIpCb *ipCb));
EXTERN S16 egUTunnTeidReordDis ARGS((EgtUEvnt *egtUEvnt, EgIpCb *ipCb, EgUSapCb *usap));
EXTERN S16 egUTunnTeidAdd ARGS((EgtUEvnt *egtUEvnt, EgIpCb *ipCb));
EXTERN S16 egTunProcEguStaReq ARGS((EgtUEvnt *egtUEvnt, EgUSapCb *usap));
EXTERN S16 egUFreeUiEvnt ARGS((EgtUEvnt *egtUEvnt));
EXTERN S16 egUCpmHdlEvt ARGS((EgUEvnt *eguEvnt, Pst *pst));
EXTERN S16 egTnlFreeReOrderQReq ARGS((Inst  threadId));
EXTERN S16 egUTunnDeleteTunnel ARGS((EgUEvnt *eguEvnt));
EXTERN S16 egUCmFreeEvent ARGS ((EgUEvnt *event));


EXTERN S16 egReordRcvdEguMsgs ARGS((EgTeIdCb *teidCb, EgtUEvnt *egtuEvt));

EXTERN Void egUCpmProcMsg ARGS((EgUEvnt *eguEvnt, Pst *pst));

EXTERN S16 egUCpmHdlTeid ARGS((EgUEvnt *eguEvnt));

EXTERN S16 egGetThrdId ARGS((EgTeIdCb **egTeidCb, S16 event, EgUThreadCb **pThrCb));

EXTERN S16 egAssignThrd ARGS((EgTeIdCb  *egTeidCb));

EXTERN S16 egLdDistribute ARGS((Void));

EXTERN S16 egFlushReordRcvdGtpuPdus ARGS((EgTeIdCb *teidCb));
EXTERN S16 egFlushAllPackets ARGS((EgTeIdCb *teidCb));
EXTERN S16 egFlushTillHole ARGS((EgTeIdCb *teidCb));

EXTERN S16 egDeleteTun ARGS((EgTeIdCb *teidCb));
EXTERN S16 egSchedreOrdTmr ARGS((Ptr egHandle, S16 tmrEvnt, Action action, U32 tmrVal));
EXTERN S16 egUTunnReordCfm ARGS((EgUEvnt *eguEvnt));
EXTERN S16 egUHndlT3N3TmrExpiry ARGS((Void));
/* eg012.201 : Modified prototype of egUSendEchoReqOnLclPort,removed EchoReqCb */
EXTERN S16 egUSendEchoReqOnLclPort ARGS((EgIpCb *ipCb, EgUEvnt *echoEvnt, 
                                    U16 *echoSeqNo));
EXTERN S16 egUSendEchoReq ARGS((Void));
EXTERN S16 egMiProcIpCb ARGS((EgIpCb *ipCb, SpId spId));
EXTERN Void egUCpmProcEguDatReq ARGS((EgtUEvnt *egtUEvnt, EgUSapCb *usap, Pst *pst));
EXTERN Void egUCpmProcEguStaReq ARGS((EgtUEvnt *egtUEvnt, EgUSapCb *usap));
EXTERN Void egUCpmProcEguTnlMgmtReq ARGS((EgtUEvnt *egtUEvnt, EgUSapCb *usap ));
EXTERN S16 EgMiHdlCntrlCfmFrmWrkr ARGS((EgUEvnt *eguEvnt));
EXTERN S16 egMiRmvEchoCb ARGS((EgUSapCb *uSapCb, EgIpCb *ipCb, EgTptSrvCb *srvCb, 
                                                         EgTSapCb *tSapCb, U8 type));
EXTERN S16 egMiGetIpCb ARGS((EgUSapCb *uSapCb, EgMngmt *cntrlReq, U8 eventType, U8 *nmbReq));
EXTERN S16 egMiIpCbDeInit ARGS((CmHashListCp *IpCbHashList, Bool del)); 
#ifdef EGTP_U_MULTI_THREADED
EXTERN S16 egUPackSendMTMsg ARGS((EgUEvnt *eguEvnt, U32 thrId, U8 event));
EXTERN S16 egWrkrActvTsk ARGS((Pst *pst, Buffer *mBuf));
EXTERN S16 EgMiProcCntrlReq ARGS((EgMngmt *cntrlReq));
EXTERN S16 egUiProcUbndReq ARGS((EgUEvnt *eguEvnt));
#ifdef SS_MULTIPLE_PROCS
EXTERN S16 egActvThrdTmr ARGS((ProcId proc, Ent ent, Inst inst));
#else /* SS_MULTIPLE_PROCS */
#ifdef SS_MT_TMR
EXTERN S16 egActvThrdTmr ARGS((Ent ent, Inst inst));
#else
EXTERN S16 egActvThrdTmr ARGS((Void));
#endif /* SS_MT_TMR */
#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_MULTIPLE_PROCS
EXTERN S16  egWrkrActvInit    ARGS((ProcId procId, Ent ent, Inst inst,
                                    Region region, Reason reason, Void **xxCb));
#else
EXTERN S16  egWrkrActvInit    ARGS((Ent ent, Inst inst,
                                    Region region, Reason reason));
#endif
EXTERN S16 egRegThrdTmr ARGS((EgUThreadCb *egThreadCb, U8 thrCnt));
EXTERN Void egthrTmrEvnt ARGS((Ptr cb, S16 event));
EXTERN S16 egCpmHdlEguCbMsg ARGS((Pst    *pst, Buffer *mBuf));
EXTERN S16 egCpmHdlHitCbMsg ARGS((Pst    *pst, Buffer *mBuf));

#ifdef SS_MT_CB
EXTERN S16 egCbTsk ARGS((Pst    *pst, Buffer *mBuf));
#endif /* SS_MT_CB */
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */

#ifdef EGTP_C

/*eg004.201 Modified for eGTP-C PSF Upgrade:Function Prototype qualifier changed from PUBLIC to EXTERN*/
/**
  @ingroup TUNNMNGMT
   @brief The function egTunnDelTECCb is a utility to free the tunnel
   control block context.
   @param[in] teCCb The pointer to the control block that has to be freed.
*/
EXTERN Void egTunnDelTECCb ARGS ((EgTECCb *teCCb));

/*eg004.201 Modified for eGTP-C PSF Upgrade:Function Prototype qualifier changed from PUBLIC to EXTERN*/
/**
  @ingroup TUNNMNGMT
  @brief egGetTunnelCb is function to get the tunnel Cb given the teid.
                 first get the array into which the teid falls into.
                 get the index ofthe teid in that array.
                 return the tunnel Cb.
  @param[in] teid The teid is the TEID of the tunnel.
  @return This function returns the pointer to the tunnel endpoint
                 control block.
*/
EXTERN EgTECCb** egGetTunnelCb ARGS((
                                U32 teid
                                ));

/**
   @ingroup TUNNMNGMT
  @brief The function egReleaseTeid is a local function to release a teid in use. 
  @param[in] teid The teid is the teid to be released.
  @return -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
*/
EXTERN S16 egReleaseTeid ARGS ((U32 teid));


#ifdef HW
EXTERN S16 EgMiShtCntrlReq ARGS((Pst *pst, ShtCntrlReqEvnt *reqInfo));
EXTERN S16  EgMiShtCntrlCfm ARGS((Pst *pst, ShtCntrlCfmEvnt *cfmInfo));
#endif

/**
  @ingroup TUNNMNGMT
   @brief The function egTunnDelTECCb is a utility to free the tunnel
   control block context.
   @param[in] teCCb The pointer to the control block that has to be freed.
*/
EXTERN Void egTunnDelTECCb ARGS ((
                              EgTECCb *teCCb
                              ));

/************************************************
 *               TEID generators                *
*************************************************/
/**
  @ingroup TUNNMNGMT
 @brief The function egInitFreeTeidList initializes an array of teids. The array
        of free teids is initialized.
 @param[in] tunnIdx The tunnel Cb whose teid array has to be initialize
 @return 
   - ROK      - If the procedure was successful.
   - ROUTRES  - If the procedure failed due to resource failure.
   - RFAILED  - If the procedure failed due to internal reasons.
 */

EXTERN S16 egInitFreeTeidList ARGS ((
                                U32 tunnIdx
                                ));

#endif
/************************************************
 * eg007.201: TsapCb reference                  *
*************************************************/
/**
  @ingroup TPTMOD
  @brief The function egTptGetTsapFrmAddr provides tSapCb from the IP address
         assocciated
 @param[in] tptSrvAddr The Ip address for which associated TSapCb needs to be found
 @param[in] tSapCb     The TSapCb ptr which will be updated with tSapCb associated to 
                       given IP
 @return 
   - ROK      - If the procedure was successful.
   - RFAILED  - If the procedure failed due to internal reasons.
 */


/*eg007.201: dstCbList, and echoSap ptr is removed --*/
PUBLIC S16 egTptGetTsapFrmAddr  ARGS (( CmTptAddr   tptSrvAddr, EgTSapCb **tSapCb));

/*****************************************
 *  * This method is used to duplicate an ie*
 *   *****************************************/
PUBLIC S16 egUtilDupIe ARGS( (CmMemListCp *memCp, EgIe *oldIe, EgIe **dupIe));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EGX__ */


/**********************************************************************
 
         End of file:     eg.x@@/main/3 - Fri Feb  4 18:18:02 2011
 
**********************************************************************/

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/

/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sn                1. Created for Initial Release 1.1
/main/2      ---      rss               1. Initial for eGTP 1.2 Release
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss               2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3      eg001.201    asaurabh      1.  Adding parameter for determining tunnelType
                                        2.  Adding parameter for keeping stats for Fwd & Normal data tunnel
/main/3      eg004.201    magnihotri  1.Modified for PSF RUG: New variables introduced 
                                        remIntfValid - TRUE/FALSE, if TRUE, remote interface version number stored 
                                        within pst is valid
                                        verContEnt - interface version controlling entity
                                      2.Modified for PSF RUG: New variables introduced
                                        remIntfValid - TRUE/FALSE, if TRUE, remote interface version number store 
                                      3.Modified for PSF:PSF Control block added 
                                      4.Modified for PSF RUG:EgVerInfoCb structure to control Rolling Upgrade
                                        introduced with interfaceInfo and PSF control block object
                                      5.Modified for PSF RUG:local teid added to associate the respective egIncReq
                                        with the teid;egHwCb added as the PSF control block
                                      6.Modified for PSF:PSF Control block object introduced in EgTECCb
                                      7.Added for PSF RUG:New function to get the interface version
                                      8.Added for PSF - RUG: New variables introduced
                                        numIntfInfo - Number of stored interface information
                                        egVerInfoCb - Interface Version information store
                                      9.Function Prototype qualifier changed from PUBLIC to EXTERN
/main/3      eg005.201    psingh       1.PSF flag added to avoid eNodeB build error
/main/3      eg006.201    psingh       1. Parameter connTunnCnt introduced to keep track of
                                          number of connected tunnels per destination
                                       2. Parameter to keep the count of  echo request sent
/main/3      eg007.201    psingh       1. dstCblist moved from egCB to egTSapCb
                                       2. USapCb ptr, echoSap, inside egCb removed
                                       3. egTSapCb updated with tSapIPAddress & ipConfig flag
/main/3      eg009.201    asaurabh     1. Modified proc to accept TSAP cb as argument 
/main/3      eg010.201    asaurabh     1. Buffer to store message to be piggybacked
/main/3      eg012.201    shpandey     1. Modified prototype of egUSendEchoReqOnLclPort,removed EchoReqCb
                                       2. Enhanced the piggy backing feature by adding the Timer
/main/3      eg014.201    rss          1. Egtpc release 10 upgrade
*********************************************************************91*/
