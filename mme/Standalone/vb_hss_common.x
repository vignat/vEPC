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
 
    Name:   HSS
 
    Type:   C source file
 
    Desc:   Data Structures required by the HSS and MME.
 
    File:   vb_hss_common.x
 
    Sid:      vb_hss_common.x@@/main/1 - Mon Jun 28 19:35:37 2010
 
    Prg:    ad
 
*********************************************************************21*/
 
#ifndef __HSSCOMMONX__
#define __HSSCOMMONX__


/******************************************************************************
 * Common Structures for HSS and MME
 *****************************************************************************/

/** International Mobile Subscriber Identity */
typedef struct vbHssImsi
{
  U8     val[VB_HSS_IMSI_LEN];
  U8     len;  /*Number of digits */
}VbHssImsi;

/*typedef U8   VbHssImsi[VB_HSS_IMSI_LEN];*/

/** MS ISDN : UE Number */
typedef U8  VbHssMsisdn[VB_HSS_MSISDN_LEN];

/** International Mobile Equipment Identity */
typedef U8   VbHssImei[VB_HSS_IMEI_LEN];

/** IMEI and Software Version Number */
typedef U8   VbHssImeisv[VB_HSS_IMEISV_LEN];

/** MME IP Address */
typedef CmInetNetAddr VbHssMmeAddr;

/** Aggregated Max Bit Rate (AMBR) */
typedef struct _hssAMBR
{
   U32 dl;      /* AMBR for downlink */
   U32 ul;      /* AMBR for uplink */
   U32 dlExt;   /* AMBR for downlink (extended) */
   U32 ulExt;   /* AMBR for uplink (extended) */
   U32 dlExt2;  /* AMBR for downlink (extended-2) */
   U32 ulExt2;  /* AMBR for uplink (extended-2) */

}VbHssAMBR;

/** Subscriber UE AMBR */
typedef VbHssAMBR VbHssSubUeAMBR;

/** Context ID*/
typedef U16 VbHssCntxtId;

/** PDN Address List*/
typedef struct _hssPdnAddrLst
{
   U8             nmb;                        /* Number of Addresses */
   CmInetNetAddr  nAddr[VB_HSS_MAX_PDN_NET_ADDRS];   /* List of Addresses */
} VbHssPdnAddrLst;

/** PDN Address Type ( IPv4, IPv6, IPv4v6)*/
typedef U32 VbHssPdnAddrType;

/** Access Point Name (FQDN)*/
typedef S8 VbHssAPN[VB_HSS_APN_LEN];


/** Allocation and Retention Priority */
typedef struct _hssARP
{
   U32 priLevel;              /* Priority Level*/
   U32 preCapbFlg;            /* Pre-emption capability (flag)*/
   U32 preVlnbFlg;            /* Pre-emption vulnerability (flag)*/ 
}VbHssARP;

/** EPS Subscriber QOS Profile */
typedef struct _hssEPSSubQOSPrf
{
   U8    qci;            /* QoS Class Identifier  */
   VbHssARP arp;            /* Allocation and Retention Priority */
   U32 maxBitRateUL;        /* Maximum bit rate for uplink */
   U32 maxBitRateDL;        /* Maximum bit rate for downlink */
   U32 guaraBitRateUL;      /* Guaranteed bit rate for uplink */
   U32 guaraBitRateDL;      /* Guaranteed bit rate for downlink */
   U32 maxBitRateULExt;     /* Maximum bit rate for uplink (extended) */
   U32 maxBitRateDLExt;     /* Maximum bit rate for downlink (extended) */
   U32 guaraBitRateULExt;   /* Guaranteed bit rate for uplink (extended) */
   U32 guaraBitRateDLExt;   /* Guaranteed bit rate for downlink (extended) */

}VbHssEPSSubQOSPrf;


/** Subscriber APN AMBR */
typedef VbHssAMBR VbHssSubApnAMBR;

/** PDN Gateway ID: IP is consider only for Phase 1*/
typedef CmInetNetAddr VbHssPdnGwAddr;

/** PDN GW Allocation Type (Static or Dynamic) */
typedef U32 VbHssPdnGwAllocType;

/** UE Status : UE engaged/available */
typedef U32  VbHssUEStatus;

/** PDN Subscription Context */
typedef struct _hssPdnSubCntxt
{
  VbHssCntxtId         cntxtId;     /* Context Id */
  VbHssPdnAddrLst      pdnAddrLst;  /* PDN Address List*/
  VbHssPdnAddrType     pdnAddrType; /* PDN Type */
  VbHssAPN             apn;         /* APN */
  VbHssEPSSubQOSPrf    qos;         /* QOS Profile */
  VbHssSubApnAMBR      ambr;        /* AMBR */
  VbHssPdnGwAddr       pdnGwAddr;     /* PDN Gateway ID*/
  VbHssPdnGwAllocType  allocType;   /* PDN GW Allocation Type */
  Bool                 subCntxtinUse; /* Indiacates APN allocated or Not */
  CmInetNetAddr        ipAddr;        /*IP address allocated to this PDN.*/
  CmInetNetAddr        pDnsAddr;      /*Primary DNS IP address.*/
  CmInetNetAddr        sDnsAddr;      /*Secondary DNS IP address.*/
  CmInetNetAddr        pcscfAddr;      /*Proxy call session control function IP address.*/
}VbHssPdnSubCntxt;

/* NAS Security Algorithms:  */
typedef struct vbHssEmmNasSecAlgo
{
   U8 pres;
   U8 intProtAlgo;
   U8 ciphAlgo;
}VbHssEmmNasSecAlgo;

/* NAS Key Set Identifier */
typedef struct vbHssEmmNasKsi
{
   U8 pres;
   U8 id;
   U8 tsc;
}VbHssEmmNasKsi;

/** Authentication Parameters */
/* VB_MME_AUTH Authentication feature added
 * Length field for each auth parameter removed since the lengths are fixed */
typedef struct hssUeSecInfo
{
   U8                   pres;

   U8                   rand[CM_EMM_MAX_RAND];
   U8                   autn[CM_EMM_MAX_AUTN];
   U8                   ck[CM_EMM_MAX_CK];
   U8                   ik[CM_EMM_MAX_IK];
   U8                   xres[CM_EMM_MAX_RES];
   VbHssEmmNasSecAlgo   secAlgo;
   VbHssEmmNasKsi       nasKsi;
   U8                   sqnMS[6];
   Bool                 resync;

#ifdef VB_MME_AUTH
   U8                   authKey[VB_HSS_SHARED_KEY_SIZE];
   U8                   opKey[VB_HSS_OP_KEY_SIZE];
   U8                   sqnHE[VB_HSS_SEQ_NMB_SIZE];
   VbMmeAuthAlg         authType;
   VbMmeIntAlg          intType;
   VbMmeEncAlg          ciphType;
#endif 
} VbHssUeSecInfo;

typedef struct _VbHssXaTransCb
{
   CmLList  lnk;        /* Link for the linked list */
   U32      transId;    /* Transaction ID */
   Void     *pUsrData;  /* User Data to be returned to the caller */

} VbHssXaTransCb;

/** HSS UE Information */
typedef struct vbHssUeInfo
{
   U32               pres;               /* Presence Variable */
   U32               ueEntry;            /* UE Entry Index */
   U32               numIMSIDigit;       /* Number of IMSI Digits*/
   VbHssImsi         imsi;               /* IMSI */
   VbHssMsisdn       msisdn;             /* UE number : MSISDN */
   VbHssImei         imei;               /* IMEI */
   VbHssImeisv       imeisv;             /* IMEISV */
   VbHssMmeAddr      mmeAddr;            /* MME Address */
   Bool              isMsPsPurgd;        /* Flag to indicate if MS is Purged*/
   VbHssSubUeAMBR    subUeAMBR;          /* Subscription UE AMBR */
   U32               numPdnSubCntxt;     /* Number of PDN Sub Context */
   VbHssPdnSubCntxt  subCntxt[VB_HSS_MAX_PDN_SUB_CTXT]; /* Subscription 
                                          Profile*/
   VbHssUEStatus     ueStatus;           /* UE status */
   VbHssUeSecInfo    ueSecInfo;          /* UE authentication parameters */

} VbHssUeInfo;

/** Update Location Request Command Structure */
typedef struct _vbMmeHssULRCmd
{
   VbHssImsi imsi;       /* IMSI */
   U32       ulrFlgs;    /* ULR Flags */
   U32       ratType;    /* Radio access technology-Type*/
} VbMmeHssULRCmd;

/*VB_TODO_SECOND_PHASE:  For the second phase*/
/** Update Location Answer Command Structure*/
typedef struct _vbMmeHssULACmd
{
   VbHssUeInfo  ueInfo;       /* UE Information */
   U32          ulaFlgs;      /* ULA Flags */
   U32          result;       /* Result */
}VbMmeHssULACmd;


/* MME-HSS Function Prototypes */
/*
EXTERN S16 vbMmeHssULReqCmd ARGS((VbMmeHssULRCmd *ulrCmd, vbHssUeInfo     *ueInfo));
*/

/*
EXTERN S16 vbHssMmeULAnsCmd ARGS((VbHssMmeULACmd *ulaCmd));
*/

#endif /*  __HSSCOMMONX__ */


/********************************************************************30**
         End of file:     vb_hss_common.x@@/main/1 - Mon Jun 28 19:35:37 2010
*********************************************************************31*/


/********************************************************************40**
        Notes:
*********************************************************************41*/


/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ad          1. Initial version for LTE-CNE Phase 1.
*********************************************************************91*/
