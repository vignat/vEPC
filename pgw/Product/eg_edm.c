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

     Name:     eGTP Encoder/Decoder & Msg validator

     Type:     C include file

     Desc:     Encoder/Decoder and Msg validation functions

     File:     eg_edm.c

     Sid:      pg_edm.c@@/main/3 - Fri Feb  4 18:18:15 2011

     Prg:      pchebolu

*********************************************************************21*/


/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment dependent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common header 1 */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common header 3 */
#include "cm_err.h"        /* common error */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tpt.h"        /* common transport */
#include "cm_dns.h"        /* common transport */
#include "leg.h"            /* eGTP Layer */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"
#endif
#include "eg.h"            /* eGTP Layer */
#include "eg_util.h"        /* eGTP layer */
#include "eg_dbutl.h"
#include "eg_edm.h"            /* eGTP Layer */
#include "eg_err.h"        /* eGTP Error Codes */

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common header 3 */
#include "cm_inet.x"       /* common tokens  */
#include "cm_tpt.x"        /* common transport */
#include "cm_dns.x"        /* common transport */

#include "egt.x"           /* eGTP layer */
#include "leg.x"            /* eGTP layer */
#include "eg_edm.x"        /* eGTP layer */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "eg.x"            /* eGTP layer */

#include "eg_tpt.x"            /* EGTP TPT module defines */
#include "eg_util.x"        /* eGTP layer */

#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif

/* fix for  ccpu00136924 */
#ifdef EGTP_C
PRIVATE S16 egValidateInftType(U8 intfType,U8 IntfEnt);         /* Event message stores the decoded hdr */
/********************************************************************
 * L O C A L     E X T E R N S                                      *
 *******************************************************************/
PUBLIC EgIe* msgIeIdx[EG_MAX_INST][EG_MAX_IE];
/* fix for ccpu00136098 */
EXTERN S16 egUtilDropIeList ARGS( (CmLListCp *ieList));

 /********************************************
  * Encode Decode Mapping function structure *
  ********************************************/
typedef struct parse_cmd {
S16 (*encParseFn) ARGS((EgIe* ,Buffer*));
S16 (*decParseFn) ARGS((EgIe* ,Buffer*,U16));
}PARSE_CMD;

/********************************************************************
 * L O C A L     F U N C T I O N    D E C L A R A T I O N S        *
 *******************************************************************/


/*******************************************************************
*        ENCODER            RELATED                  FUNCTIONS     *
*******************************************************************/
PRIVATE S16 egEncodeGmHdr ARGS(( EgEvnt *evntMsg, Buffer *mBuf));

PRIVATE  S16 egEncIe ARGS(( EgIe *ie, Buffer *mBuf));

PRIVATE S16 egEncIeData ARGS(( Buffer *mBuf, EgIe *egIe));

PRIVATE S16 egEncPthHdr ARGS(( EgEvnt *evntMsg, Buffer *mBuf));

PRIVATE  S16 egEncPthIe ARGS(( EgIe *ie, Buffer *mBuf));

PRIVATE S16 egEncVerIndMsg ARGS
((
 EgEvnt *evntMsg,         /* Event message stores the decoded hdr */
 Buffer     *mBuf,        /* Buffer to decode the header */
 Bool    cntrlMsg         /* Tells whether it is a cntrl msg */
));

/********************************************************
 * Encode Parser Functions for eGTP Message IE Encoding *
 ********************************************************/
PRIVATE S16 egEncEG_UNSIGNED_8 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UNSIGNED_16 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UNSIGNED_32 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_STRING_8 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_STRING_16 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_STRING_32 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_STRING_132 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_STRING_400 ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_STRING_MAX ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_SNETWORK ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_INDICATION ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_PVT_EXTN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_CAUSE ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_TRC_INFO ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_BEARER_FLAG ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_FTEID ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_ULI_DATA ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_FQOS ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_BQOS_IEDATA ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_PAA_IEDATA ARGS((EgIe* ,Buffer*));
/* PRIVATE S16 egEncEG_BID_LST ARGS((EgIe* ,Buffer*)); */
PRIVATE S16 egEncEG_GLOBAL_CNID ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_AUTH_QUIN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_AUTH_QUAD ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_CMP_REQ ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_GUTI ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_F_CONT ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_TRGTID ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_PKTFLOWID ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_SRCID ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_PDN_CONN_SET_ID ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_IP_ADDR ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_GSM_KEY_TRIP ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UMTS_KEY_CIPH_QUIN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_GSM_KEY_CIPH_QUIN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UMTS_KEY_QUIN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_EPS_SEC_QUAD_QUIN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UMTS_KEY_QUAD_QUIN ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_F_CAUSE ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_S103_PDN_FW_INFO ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_S1U_DATA_FW ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_SRC_RNC_PDCP_CTX ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_PDUNUM ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UE_TIME_ZONE ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_TRACE_REF ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_RAB_CONTEXT ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_AMBR ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_GLOBAL_CN_ID ARGS((EgIe* ,Buffer*));
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
PRIVATE S16 egEncEG_MBMS_IPMCAST_DIST ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_UCI ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_CSG_INFO_REP_ACT ARGS((EgIe* ,Buffer*));
#endif
/*fix for ccpu00135846 :added bcd encoding handler for IEs */
PRIVATE S16 egEncEG_BCD ARGS((EgIe* ,Buffer*)); 
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
PRIVATE S16 egEncEGGeneric_BCD ARGS((U8*,U16*, Buffer*)); 
PRIVATE S16 egEncEG_ARP ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_EPC_TMR ARGS((EgIe* ,Buffer*));
PRIVATE S16 egEncEG_CNG_TO_RPT_FLG ARGS((EgIe* ,Buffer *));
PRIVATE S16 egEncEG_MDT_CONF ARGS((EgIe* , Buffer *));
PRIVATE S16 egEncEG_ADD_MM_CNTXT_SRVCC ARGS((EgIe* ,Buffer *));
PRIVATE S16 egEncEG_STNSR ARGS((EgIe* , Buffer *));
PRIVATE S16 egEncEG_THROTTLING ARGS((EgIe* ,Buffer*));
#endif 




/*******************************************************************
*        DECODER            RELATED                  FUNCTIONS     *
*******************************************************************/

PRIVATE S16 egDecodeGmHdr ARGS(( EgEvnt *evntMsg, Buffer *mBuf,EgMsgEntry  **gmEntry));

PRIVATE S16 egGetInfType ARGS(( EgEvnt *evntMsg));

PRIVATE S16 egDecIe ARGS(( EgEvnt *evntMsg, Buffer *mBuf,EgIeLevelInfo *ieLvlInfo, 
                            U16 *outIeLen, U8 *posIndex,EgIeProperties *ieProperties, EgMsgEntry  *gmEntry)); 
 
PRIVATE S16 egDecIeData ARGS(( EgEvnt *evntMsg, Buffer *mBuf, 
                               EgIe *egIe, U16 ieLen));

PRIVATE S16 egDecGrpIe ARGS(( EgEvnt *evntMsg, Buffer *mBuf, EgIeLevelInfo *ieLvlInfo, U16 inIeLen, 
                                      U8 *posIndex,EgIeProperties *ieProperties,EgMsgEntry *gmEntry));
                              

PRIVATE S16 egDecPthHdr ARGS(( EgEvnt *evntMsg, Buffer *mBuf));

PRIVATE S16 egDecPthIe ARGS(( EgEvnt *evntMsg, Buffer *mBuf, 
                           EgIeLevelInfo *ieLvlInfo, U16 *outIeLen));

/********************************************************
 * Decode Parser Functions for eGTP Message IE Encoding *
 ********************************************************/
PRIVATE S16 egDecEG_UNSIGNED_8 ARGS((EgIe* ,Buffer*,U16 ));
PRIVATE S16 egDecEG_UNSIGNED_16 ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_UNSIGNED_32 ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_STRING_8 ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_STRING_16 ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_STRING_32 ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_STRING_132 ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_STRING_MAX ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_STRING_400 ARGS((EgIe* egIe, Buffer *mBuf, U16 ieLen));
PRIVATE S16 egDecEG_SNETWORK ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_INDICATION ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_CAUSE ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_PVT_EXTN ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_TRC_INFO ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_BEARER_FLAG ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_FTEID ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_ULI_DATA ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_FQOS ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_BQOS_IEDATA ARGS((EgIe* ,Buffer*,U16));
PRIVATE S16 egDecEG_PAA_IEDATA ARGS((EgIe* ,Buffer*,U16));
/* PRIVATE S16 egDecEG_BID_LST ARGS((EgIe* ,Buffer*, U16)); */
PRIVATE S16 egDecEG_GLOBAL_CNID ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_AUTH_QUIN ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_AUTH_QUAD ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_CMP_REQ ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_GUTI ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_F_CONT ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_TRGTID ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_PKTFLOWID ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_SRCID ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_PDN_CONN_SET_ID ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_IP_ADDR ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_GSM_KEY_TRIP ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_UMTS_KEY_CIPH_QUIN ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_GSM_KEY_CIPH_QUIN ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_UMTS_KEY_QUIN ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_EPS_SEC_QUAD_QUIN ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_UMTS_KEY_QUAD_QUIN ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_F_CAUSE ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_S103_PDN_FW_INFO ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_S1U_DATA_FW ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_SRC_RNC_PDCP_CTX ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_PDUNUM ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_UE_TIME_ZONE ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_TRACE_REF ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_RAB_CONTEXT ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_AMBR ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_GLOBAL_CN_ID ARGS((EgIe* ,Buffer*, U16));
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
PRIVATE S16 egDecEG_MBMS_IPMCAST_DIST ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_UCI ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_CSG_INFO_REP_ACT ARGS((EgIe* ,Buffer*, U16));
#endif
#ifdef EG_REL_AC0 
PRIVATE S16 egDecEG_ARP ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_EPC_TMR ARGS((EgIe* ,Buffer*, U16));
PRIVATE S16 egDecEG_CNG_TO_RPT_FLG ARGS((EgIe* , Buffer *, U16));
PRIVATE S16 egDecEG_MDT_CONF ARGS((EgIe* ,Buffer *, U16));
PRIVATE S16 egDecEG_ADD_MM_CNTXT_SRVCC ARGS((EgIe* ,Buffer *, U16));
PRIVATE S16 egDecEG_STNSR ARGS((EgIe* , Buffer *, U16));
PRIVATE S16 egDecEGGeneric_BCD ARGS((U8*, Buffer*, U16, U16*));
PRIVATE S16 egDecEG_THROTTLING ARGS((EgIe* ,Buffer*, U16));
#endif 

/* fix for ccpu00135846:added bcd decoing handler for IEs */
PRIVATE S16 egDecEG_BCD ARGS((EgIe* ,Buffer*, U16)); 

/******************************************
 * Encode Decode Mapping function Metrics *
 ******************************************/
PARSE_CMD Parse_function[] =
{
  { NULLP, NULLP },
  MAPFUNCTION(EG_FTEID), /* 1 */
  MAPFUNCTION(EG_UNSIGNED_8) ,
  MAPFUNCTION(EG_UNSIGNED_16),
  MAPFUNCTION(EG_UNSIGNED_32),
  MAPFUNCTION(EG_CAUSE),
  MAPFUNCTION(EG_PVT_EXTN),
  MAPFUNCTION(EG_TRC_INFO),
  MAPFUNCTION(EG_ULI_DATA),
  MAPFUNCTION(EG_FQOS),
  MAPFUNCTION(EG_BQOS_IEDATA), /* 10 */
  MAPFUNCTION(EG_PAA_IEDATA),
  MAPFUNCTION(EG_GLOBAL_CNID),
  MAPFUNCTION(EG_STRING_8),
  MAPFUNCTION(EG_STRING_16),
  MAPFUNCTION(EG_STRING_32),
  MAPFUNCTION(EG_STRING_132),
  MAPFUNCTION(EG_STRING_MAX),
  MAPFUNCTION(EG_SNETWORK),
  MAPFUNCTION(EG_INDICATION),
  MAPFUNCTION(EG_BEARER_FLAG), /* 20 */
  {NULLP , NULLP}, 
  MAPFUNCTION(EG_IP_ADDR),
  MAPFUNCTION(EG_S103_PDN_FW_INFO),
  MAPFUNCTION(EG_S1U_DATA_FW),
  MAPFUNCTION(EG_PDUNUM),
  MAPFUNCTION(EG_UE_TIME_ZONE),
  MAPFUNCTION(EG_TRACE_REF),
  MAPFUNCTION(EG_RAB_CONTEXT),
  MAPFUNCTION(EG_GSM_KEY_TRIP),
  MAPFUNCTION(EG_UMTS_KEY_CIPH_QUIN), /* 30 */
  MAPFUNCTION(EG_GSM_KEY_CIPH_QUIN),
  MAPFUNCTION(EG_UMTS_KEY_QUIN),
  MAPFUNCTION(EG_EPS_SEC_QUAD_QUIN),
  MAPFUNCTION(EG_UMTS_KEY_QUAD_QUIN),
  MAPFUNCTION(EG_AUTH_QUIN),
  MAPFUNCTION(EG_AUTH_QUAD),
  MAPFUNCTION(EG_CMP_REQ),
  MAPFUNCTION(EG_GUTI),
  MAPFUNCTION(EG_F_CONT), 
  MAPFUNCTION(EG_F_CAUSE), /* 40 */
  MAPFUNCTION(EG_TRGTID),
  MAPFUNCTION(EG_SRCID),
  MAPFUNCTION(EG_PKTFLOWID),
  MAPFUNCTION(EG_SRC_RNC_PDCP_CTX),
  MAPFUNCTION(EG_PDN_CONN_SET_ID), /* 45 */
  MAPFUNCTION(EG_AMBR), /* 46 */
  MAPFUNCTION(EG_GLOBAL_CN_ID), /* 47 */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
  MAPFUNCTION(EG_MBMS_IPMCAST_DIST), /* MBMS IP Multicast distribution */
  MAPFUNCTION(EG_UCI),               /* user CSG Information */
  MAPFUNCTION(EG_CSG_INFO_REP_ACT), /* 50 */ /* CSG info reporting action */
#endif
  {NULLP , NULLP},
  MAPFUNCTION(EG_BCD),  /* fix for ccpu00135094 */
  MAPFUNCTION(EG_STRING_400),  /* fix for ccpu00135094 */
#ifdef EG_REL_AC0 
  MAPFUNCTION(EG_ARP),
  MAPFUNCTION(EG_EPC_TMR),  /* 55 EPC timer */
  MAPFUNCTION(EG_MDT_CONF),
  MAPFUNCTION(EG_CNG_TO_RPT_FLG),
  MAPFUNCTION(EG_ADD_MM_CNTXT_SRVCC),
  MAPFUNCTION(EG_STNSR),
  MAPFUNCTION(EG_THROTTLING),
#endif
  {NULLP , NULLP}
};
/*******************************************************************
*        VALIDATION         RELATED                  FUNCTIONS     *
*******************************************************************/
 

PRIVATE S16 egValidateGmHdr ARGS(( EgEvnt *evntMsg));

#ifdef EG_GROUPED_CHK
PRIVATE S16 egValidateGrpIe ARGS((EgMsgEntry *,EgGrouped *grpIe, 
                                   EgGroupedIe *grpIeEntry));             
#endif
PRIVATE S16 egValidateGmMsgIEs ARGS(( EgEvnt *evntMsg,EgMsgEntry *gmEntry));

PRIVATE S16 egValidateIeLen ARGS(( EgIe *egIe, EgIeEntry *ieEntry, U8 *posIndex, 
                                   EgIeProperties *ieProperties,U16 ieLen));

PRIVATE S16 egValidatePthHdr ARGS(( EgEvnt *evntMsg));

PRIVATE S16 egValidatePthMsgIEs ARGS(( EgEvnt *evntMsg ));

PRIVATE S16 egMakeGrpIeEntry ARGS((Region region, Pool pool, EgGroupedIe  *groupedIe,
                                   LegIeProperties   *ieEntry));

#ifdef EG_MT
PRIVATE S16 egEdmSendMsgToCm ARGS((Pst *pst, EgEvnt *evntMsg));
#endif /* EG_MT */
/* fix for ccpu00136098 */
/* fix for ccpu00136924 */
PRIVATE S16  egTunnErrIncOutRsp(EgEvnt *event,U8 failedIeType,U8 failedIeInst); 

PRIVATE S16 egEdmRemoveExtraIeData ARGS((Buffer *mBuf, U16 nmbBytes));

#endif /* End of EGTP_C */


#ifdef EGTP_U


/*******************************************************************
*        ENCODER            RELATED                  FUNCTIONS     *
*******************************************************************/
PRIVATE S16 egUEncodeGmHdr ARGS((EgUEvnt *evntMsg, Buffer *mBuf));

PRIVATE S16 egUEncIeData   ARGS((Buffer *mBuf, EgUIe *egIe));

PRIVATE S16 egUEncIe    ARGS((EgUIe *ie, Buffer *mBuf));

/*******************************************************************
*        ENCODER            RELATED                  FUNCTIONS     *
*******************************************************************/
PRIVATE S16 egUDecodeGmHdr ARGS((EgUEvnt *evntMsg, Buffer *mBuf));

PRIVATE S16 egUDecIeData ARGS((EgUEvnt *evntMsg, Buffer *mBuf, EgUIe *egIe, U16 ieLen));

PRIVATE S16 egUDecIe ARGS((EgUEvnt *evntMsg, Buffer *mBuf,Bool *iePres,U8 *ieType));

/*******************************************************************
*        VALIDATION         RELATED                  FUNCTIONS     *
*******************************************************************/

PRIVATE S16 egUValidateGmHdr ARGS((EgUEvnt *evntMsg));

PRIVATE S16 egUValidateGmMsgIEs ARGS((EgUEvnt *evntMsg, Bool *iePres));

PRIVATE S16 egUValidateIeLen ARGS((EgIeEntry *ieEntry, U8 msgType, U16 ieLen));

PRIVATE S16 egUGetCRType ARGS ((U8 extHdrType, U8 extHdrLen, Buffer *mBuf));

#endif /* End of EGTP_U */

#ifdef __cplusplus
}
#endif

#ifdef EGTP_C

#ifdef SS_MULTIPLE_PROCS
#define edmCb egCb.edmCb
#else
PUBLIC EdmCb edmCb;
#endif

#endif /* End of EGTP_U */


#ifdef EGTP_C
/***********************************************************************
 *   Fun:   egEdmRemoveExtraIeData
 *   Desc:  Removes the extra content of the IE
 *          It will be useful during backword compatibity
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEdmRemoveExtraIeData
(
Buffer *mBuf,      /* Holds the Data for decoding */
U16 nmbBytes          /* Ie Length to decode */
)
#else
PRIVATE S16 egEdmRemoveExtraIeData(mBuf,nmbBytes)
Buffer *mBuf;      /* Holds the Data for decoding */
U16 nmbBytes;         /* Ie Length to decode */
#endif
{
   S16      retVal = ROK; /* Return value used to know the successful parsing */
   Data     dumBytes[EG_MAX_BYTES];

   retVal = SRemPreMsgMult(dumBytes, nmbBytes, mBuf);
   
   EG_DBG_WARN(0,0, (egp,
         "egEdmRemoveExtraIeData(Skipped Bytes(%d)) \n",
          nmbBytes));
   RETVALUE(retVal);
}





/************************************************************************
 *   Fun:  egGetIeEntryFrmDb
 *
 *   Desc:  This Functions returns the Ie Entry based 
 *          on the IE Type.
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ***********************************************************************/

#ifdef ANSI
PUBLIC EgIeEntry* egGetIeEntryFrmDb
(
U8   ieType           /* refers to the type of the IE to get the entry */
)
#else
PUBLIC EgIeEntry* egGetIeEntryFrmDb(ieType)
U8   ieType;          /* refers to the type of the IE to get the entry */
#endif
{
   U16  posIdx=0; 
   
   EG_TRC2(egGetIeEntryFrmDb)
   /****************************************
    * Get the actual Position Index of the *
    * Ie Type from the EDM Control Block   *
    ****************************************/
   posIdx = edmCb.egIeCb.posIdx[ieType]; 
   if( posIdx != 0)
   {
      EG_RETVALUE( &edmCb.egIeCb.ieEntry[posIdx-1]); 
   }
   else
   {
      EG_RETVALUE(NULLP);
   }
}  /* End of egGetIeEntryFrmDb() */



/************************************************************************
 *   Fun:  egDbGetMsgFrmGmDict
 *
 *   Desc:  This Functions returns the Msg Entry based 
 *          on the interface Type and msg Type.
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ***********************************************************************/
#ifdef ANSI
PUBLIC EgMsgEntry* egDbGetMsgFrmGmDict
(
U8 msgType,      /* refers to the type of the message to get the entry */
U8 intfType    /* refers to the type of the interface to get the entry */
)
#else
PUBLIC EgMsgEntry* egDbGetMsgFrmGmDict(msgType, intfType)
U8 msgType;      /* refers to the type of the message to get the entry */
U8 intfType;   /* refers to the type of the interface to get the entry */
#endif
{

   EgGmCb      *gmCb = NULLP;       /* Variable to store the DM Entry */
   EgIntfCb    *intfCb = NULLP;         /* Variable to get the Cb IDX */
   S16          ret = ROK;     /* Variable to store the return values */
   S16          lstIdx = 0;              /* To iter thru the intf lst */
   
   
   EG_TRC2(egDbGetMsgFrmGmDict)

   /*****************
    * Debug trace   *
    ****************/
   EG_DBG_INFO(0, 0, (egp,
          "egDbGetMsgFrmGmDict(Getting GM Entry for Msg Type = (%d)) \n",
           msgType));
 
    /***************************
     * Calculate the hash key  *
     **************************/
    for ( lstIdx =0;intfType && lstIdx < egCb.genCfg.nmbIntfs; lstIdx++)
    {
       if( PRSNT_NODEF == edmCb.egIdxToIntfMap[lstIdx].pres)
       {
          if( intfType == edmCb.egIdxToIntfMap[lstIdx].val)
          {
             intfCb =  edmCb.intfCbLst[lstIdx];
             break;
          }
       }
    }
    if( intfCb == NULLP)
    {
       EGLOGERROR(ERRCLS_DEBUG, EEG079, 0,
                    "egDbGetMsgFrmGmDict() Failed Intf is Invalid");
       EG_RETVALUE(NULLP);
    }
    
    /*********************************************
     * Search the Gm Entry for msg GM Dictionary *
     ********************************************/
   if(ROK == (ret = cmHashListFind(&intfCb->gmDictHlCp, (U8*)&msgType,
                                   sizeof(U8), 0, (PTR *)&gmCb)))
   {
      EG_RETVALUE(&gmCb->gmEntry);
   }
   else
   {
    /*********************************************
     * Failed to retrieve from GM Dictionary     *
     ********************************************/
      EGLOGERROR(ERRCLS_DEBUG, EEG080, 0,
         "egDbGetMsgFrmGmDict()-Failed to retrieve msg from the GM Dictionary");
      EG_RETVALUE(NULLP);
   }
} /* end of egDbGetMsgFrmGmDict () */


/************************************************************************
 *   Fun:  egEncodeMsgHdr
 *   
 *   Desc:  This will Encode the message Header.
 *   
 *   Ret:   ROK  - for Success
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncodeGmHdr
(
EgEvnt *evntMsg,      /* Event message having the MsgHdr to encode hdr */
Buffer     *mBuf                /* Mbuf which holds the encoded buffer */
)
#else
PRIVATE S16 egEncodeGmHdr( evntMsg, mBuf)
EgEvnt *evntMsg;     /* Event message having the MsgHdr to encode hdr */
Buffer     *mBuf;              /* Mbuf which holds the encoded buffer */
#endif
{
   S16         retValue = ROK;              /* Holds the return value */
   EgMsgHdr    header;       /* Temp stores the header struct for enc */
   Data        dumMsgLen[EG_LEN_THREE];        /* for length encoding */
   U8          tmpByte = 0;        /* stores one byte of data for enc */

   EG_TRC2(egEncodeGmHdr)

   /********************************************
    * Clearing of message lenth and spare byte *
    *******************************************/
   cmMemset((U8*)dumMsgLen, EG_ZERO, EG_LEN_THREE);

   /********************************************************
    * Copy the eGTP message header to a local buffer       *
    *******************************************************/
   cmMemset((U8* )&header, 0, sizeof(EgMsgHdr));
   cmMemcpy( (U8* )&header, (U8* )&evntMsg->u.egMsg->msgHdr,
           sizeof(EgMsgHdr));

   /********************************************************
    * Encode the version  number, shift 5 times to store   *
    * the version in the last 3bits                        *
    *******************************************************/
   tmpByte = header.version << EG_SHIFT_5;
   /********************************************************
    * Encode the piggybackbit bit, shift 5 times to store  *
    * the version in the last 1bit                         *
    * ******************************************************/
   /* eg010.201: encode piggyback bit*/
#ifdef EG_PIGGYBACK_SUPP
   if (evntMsg->u.egMsg->msgHdr.pMsgPres)
      tmpByte |= EG_MASK_BIT5; 
#endif

   /********************************************************
    * Encode the TEID value if present, add to the bit 4   *
    *******************************************************/
   if(header.teidPres)
      tmpByte |=  EG_MASK_BIT4;

   SAddPstMsg(tmpByte, mBuf);

   /*********************************************************
    * Encode the Message Type                             *
    ********************************************************/ 
   SAddPstMsg(header.msgType, mBuf);

   /*********************************************************
    * Encode the message length with zeroes                 *
    * At the end of the encoding all the IE's, the final    *
    * Length will be calculated and updated the mBuf        *
    ********************************************************/
   SAddPstMsgMult(dumMsgLen, EG_LEN_TWO, mBuf);

   /*********************************************************
    * Encode the Teid if T-bit is set                       *
    ********************************************************/
   if(header.teidPres) 
      EG_ENC_U32(header.teid, mBuf);

   /*********************************************************
    * Encode the sequence number along with spare byte     *
    ********************************************************/
   header.seqNumber &= 0x00FFFFFF;
   header.seqNumber <<= EG_SHIFT_8;
   header.seqNumber &= 0xFFFFFF00;
   EG_ENC_U32(header.seqNumber, mBuf);

   EG_RETVALUE(retValue);
} /*  end of egEncodeGmHdr() */


/***********************************************************************
 *   Fun:  egEncMsgReq
 *   
 *   Desc:  This will Encode the message receieved from the application
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c  
 **********************************************************************/
#ifdef EG_MT
#ifdef ANSI
PUBLIC S16 egEncMsgReq
(
 Pst  *pst,            /* which holds the pst strucuture for MT case */
 EgEvnt *evntMsg       /* Event message having the Msg to encode msg */
)
#else
PUBLIC S16 egEncMsgReq(pst, evntMsg)
Pst  *pst;             /* which holds the pst strucuture for MT case */
EgEvnt *evntMsg;       /* Event message having the Msg to encode msg */
#endif
#else
#ifdef ANSI
PUBLIC S16 egEncMsgReq
(
 EgEvnt *evntMsg       /* Event message having the Msg to encode msg */
)
#else
PUBLIC S16 egEncMsgReq(evntMsg)
EgEvnt *evntMsg;       /* Event message having the Msg to encode msg */
#endif
#endif /* EG_MT */
{
   S16               retVal = ROK;         /* Holds the return value */
   Buffer            *mBuf = NULLP;     /* Stores the Encoded Buffer */
   CmLList           *tmpNode = NULLP;         /* Stores the Ie Node */
   EgUstaDgn         dgn;    /* diagnostics for sending alaram to LM */
   Mem               mem;   /* For allocating memory using reg, pool */
   U16               cnt    = 0;    /* For iterating thru Node Lists */
   /*eg009.201:Klocwork fix-Unused variable removed*/
   U32               ieCnt  = 0;   /* For storing the nmb ie in list */
   U16               tmpCnt  = 0;  /* For storing the nmb ie in list */
   U16               tmpLen = 0;            /* Stores the ieLen temp */
   MsgLen            encLen = 0;   /* Temp stores the encoded length */
   U16               instCnt= 0;  /* For iterating thru the inst cnt */
   EgIeLevelInfo     ieLvlInfo;   /* Used to fill the IE Level Info */
   EgIe              *tmpIe = NULLP;            /* Holds the IE NODE */
   EgIeEntry         *ieEntry = NULLP; /* Holds IE Entry for each IE */
   EgMsgEntry        *gmEntry = NULLP;        /* Holds the Message Entry */
   EgIeProperties    *ieProp = NULLP;
   U8                retValue = RFAILED;
   U8                idx;


#ifdef LEG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16       nmbLines = 0;
#endif
#endif

   EG_TRC2(egEncMsgReq);
 
   /**************************************************
    * Initialize the diagnostic and memory structure *
    **************************************************/
    cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
    cmMemset((U8*)&mem, EG_ZERO, sizeof(Mem));

   /**************************************************
    * Clear the IE level information                 *
    **************************************************/
    cmMemset((U8*)&ieLvlInfo, EG_ZERO, sizeof(EgIeLevelInfo));

   /**************************************************
    * Check for event message being proper           *
    **************************************************/
    if (!evntMsg)
    {
       EG_DBG_INFO(0, 0, (egp,
            "egEncMsgReq: evntMsg is NULLP \n"));
       EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
    }

   /******************************************************
    * Check for eGTP message to be encoded being proper  *
    ******************************************************/ 
   if (!evntMsg->u.egMsg)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_DBG_INFO(0, 0, (egp,
            "egEncMsgReq: egMsg is NULLP \n"));

      /****************************************************
       * Fill and send error event message to main thread *
       ****************************************************/ 
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 0);
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }
 
   /********************************************************
   * Check if the version is 2, if not report INV version  *
   ********************************************************/
   if(evntMsg->u.egMsg->msgHdr.version != EG_MSG_VERSION)
   {
      retVal = EGT_ERR_MSG_INV_VERSION;
      EG_DBG_INFO(0,0, (egp,
             "EgEncMsgReq(Invalid Msg Version(%d), \
             resultType(%d) \n", evntMsg->u.egMsg->msgHdr.version, retVal));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   } 
   /******************************************************
    * Initialise the position matrix pointers to NULL    *
    ******************************************************/
   for (instCnt=0; instCnt < EG_MAX_INST; instCnt++)
   {
      for (cnt=0; cnt < EG_MAX_IE; cnt++)
      {
      /************************************
       * Initialise the msgIeIdx to NULL  *
       ***********************************/
       msgIeIdx[instCnt][cnt] = (EgIe *)NULLP;
      }
   }

   /*************************
    * Copy region and pool  *
    ************************/
#ifdef EG_MT
   mem.region = pst->region;
   mem.pool   = pst->pool;
#else
   mem.region = egCb.init.region;
   mem.pool   = egCb.init.pool;
#endif /* EG_MT */
   /******************************************************
    * Check whether interface type is present            *
    *******************************************************/
   if ( ! evntMsg->intfType)
   {
     retVal = EGT_ERR_INV_INTF_TYPE;
     EG_DBG_INFO(0, 0, (egp,
           "egEncMsgReq: evntMsg interface is not present\n"));

     /****************************************************
      * Fill and send error event message to main thread *
      *****************************************************/
     EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
         evntMsg->u.egMsg->msgHdr.msgType);
#ifdef EG_MT
     egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
     EG_RETVALUE(EGT_ERR_INV_INTF_TYPE);
   }

#ifdef _EG_PRINT_BUF_
   EgUtilGmDump(evntMsg->u.egMsg
#ifdef LEG_FILE_LOG
                , egCb.dbgFp, &nmbLines
#endif /* end of LEG_FILE_LOG */
                );
#endif
   /******************************************************
    * Validate the eGTP message header                   *
    ******************************************************/
   retVal = egValidateGmHdr(evntMsg);   
   if(retVal != ROK)
   {
      /****************************************************
       * Fill and send error event message to main thread *
       ****************************************************/ 
      EG_DBG_INFO(0, 0, (egp,
            "egEncMsgReq: egValidateGmHdr failed \n"));
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 
                           evntMsg->u.egMsg->msgHdr.msgType);
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */

      EG_RETVALUE(retVal);
   }
    /*******************************************************
    *Validate whether the message came on valid interface *
    ******************************************************/
   gmEntry = egDbGetMsgFrmGmDict(evntMsg->u.egMsg->msgHdr.msgType,
       evntMsg->intfType); 
   if (gmEntry == NULLP)
   {
     retValue = EGT_ERR_MSG_INV_TYPE;
     EG_DBG_INFO(0,0, (egp,
           "egEncMsgreq (Invalid Message Type(%d) resultType(%d) \n",
           evntMsg->u.egMsg->msgHdr.msgType, retValue));
     EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, 
         evntMsg->u.egMsg->msgHdr.msgType);
     EG_RETVALUE(retValue);
   }

  
   /***************************************************
    * Frame the msg Index which is req for validation *
    * Itearate through the list of nodes in the egMsg *
    * and store the ptr of the node in the msgIeIdx   *
    ***************************************************/
   CM_LLIST_FIRST_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
   ieCnt = evntMsg->u.egMsg->egIeList.count;
   for(cnt = 0; ((cnt < ieCnt) && (tmpNode));
       cnt++)
   {
      tmpIe   = (EgIe*)tmpNode->node;
      idx = gmEntry->posIndex[tmpIe->ieInst][tmpIe->ieType]; 
      /*******************************************
      * Get the IE entry from the IE Dictionary  *
      * Check if the ie Data type is matching?   *
      *******************************************/
      ieEntry = egGetIeEntryFrmDb(tmpIe->ieType);
      if ( (ieEntry) && (ieEntry->dataType != tmpIe->dataType) && (idx == 0 ||
            idx > EG_MAX_IES_PER_MSG))
      {
         retVal = EGT_ERR_INV_IE_TYPE;
         /****************************************************
         * Fill and send error event message to main thread *
         ****************************************************/ 
         EG_DBG_INFO(0,0, (egp,
           "egEncMsgreq (Invalid IE Type(%d) on Message Type(%d) resultType(%d) \n",
           tmpIe->ieType, evntMsg->u.egMsg->msgHdr.msgType, retValue));

         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe->ieType,
              tmpIe->ieInst, evntMsg->u.egMsg->msgHdr.msgType);
         EG_RETVALUE(retVal);
      }
      ieProp = &gmEntry->ieProperties[idx-1];
      /**********************************************************
       * In decoding we are framing this message index only for *
       * Level 0 IEs only, not for rest of the levels           *
       * So, please get the IE level and skip the following if  *
       * not at Level 0                                         *
       * ********************************************************/
      /* This can be a repeated IE */
      if(msgIeIdx[tmpIe->ieInst][tmpIe->ieType] == NULLP || ieProp->ieList)
      {
          msgIeIdx[tmpIe->ieInst][tmpIe->ieType] = tmpIe;
      }
      else
      {
         /* ccpu00137636 : Repeated IEs needs to be ignored */
         switch (tmpIe->ieType)
         {
            case EG_BEARER_CNTX_IETYPE:
            case EG_PDN_CONN_IETYPE:
            case EG_PDU_NUMBER_IETYPE:
            case EG_RAB_CONTEXT_IETYPE:
               break;
            default:
            {
               /**************************************************
          * Case of repeated IE : Skip this IE and continue *
          * further processing                              *
               **************************************************/
          EG_FILL_IE_LVL_INFO(&ieLvlInfo, ieLvlInfo.level,
                            tmpIe->ieType, tmpIe->ieInst);
          /****************************************************
          * The Second occurance IE should be deleted. so fill*
          * the occurance pres to TRUE and occr as 2          *
          ****************************************************/
          ieLvlInfo.ieInfo[ieLvlInfo.level].occrPres = TRUE;
          ieLvlInfo.ieInfo[ieLvlInfo.level].occr = 2;
          /* eg012.201 : Added NULL check */ 
          if ( (ieEntry) && (ieEntry->dataType == EG_GROUPED))
          {
             EgUtilGmDelGrpIe(evntMsg->u.egMsg, &ieLvlInfo);
          }
          else
          {
             EgUtilGmDelIe(evntMsg->u.egMsg, &ieLvlInfo, FALSE);
          }
         /***************************************************
          * Iterate thru the list till the position of the  *
          * ie we have deleted, so that the outer loop can  *
          * be continued smoothly.                          *
          ***************************************************/
          for ( tmpCnt=0; tmpCnt < cnt; tmpCnt++)
          {
             CM_LLIST_NEXT_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
          }
      }
         }
      }

      CM_LLIST_NEXT_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
   }
   
   /*********************************************
    * validate the incoming encoded IE's        *
    *********************************************/
   retVal = egValidateGmMsgIEs(evntMsg,gmEntry);
   if(retVal != ROK)
   {
      /******************************************************
       * Fill and send error event message to main          *
       * thread and  free buffer                            *
       ******************************************************/ 
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
      EG_RETVALUE(retVal);
   }
   /*********************************************************
    * Validation is successful, do the encoding             * 
    * Allocate memory for the buffer                        *
    *********************************************************/ 
#ifdef EG_MT
   retVal = SGetMsg(pst->region, pst->pool, &mBuf);
#else
   retVal = SGetMsg(egCb.init.region, egCb.init.pool, &mBuf);
#endif /* EG_MT */ 
   if(retVal != ROK)
   {
     /********************************************************
      * Memory allocation failed, send alarm to LM and error *
      * event message to main thread along with result code  * 
      ********************************************************/ 
      EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);

      retVal = EGT_ERR_MEM_ALLOC_FAILED;
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 
                           evntMsg->u.egMsg->msgHdr.msgType); 
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
      EG_RETVALUE(retVal);
   }

   /*****************************
    * Encode the message header *
    ****************************/
   egEncodeGmHdr(evntMsg, mBuf);

   /*********************************************************
    * Encode the individual IE' and grouped IE's            *
    *********************************************************/
   CM_LLIST_FIRST_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
   for(cnt = 0; 
       ((cnt < evntMsg->u.egMsg->egIeList.count) && (tmpNode)); 
       cnt++)
   {
      /*****************************
       * Get the IE entry          *
       ****************************/
      tmpIe = (EgIe*)tmpNode->node;

      /*****************************
       * Encode the IE             *
       ****************************/
      retVal = egEncIe(tmpIe, mBuf);
      if(retVal != ROK)
      {
        /********************************************************
         * Fill and send error event message to main thread and * 
         * free buffer                                          *
         ********************************************************/ 
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe->ieType, 
                              tmpIe->ieInst, 
                              evntMsg->u.egMsg->msgHdr.msgType); 
         SPutMsg(mBuf);
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
         EG_RETVALUE(retVal);
      }
      else
      {
         /*********************************
          * Get the next IE from the list *
          ********************************/
         CM_LLIST_NEXT_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
      }
   }

   /************************************************************
    * Fill the message length, length starts from index 2      *
    * calculate total Buffer length and decrement 4bytes which *
    * is to encode the mandatory hdr elements                  *
    ************************************************************/
   SFndLenMsg(mBuf, &encLen);
   encLen -= EG_LEN_FOUR;

   /************************************************************
    * Fill the message length, length starts from index 2      *
    ************************************************************/
   tmpLen = EG_LEN_TWO;

   EG_ENC_LEN(encLen, mBuf, tmpLen);

   /**************************************
    * Free the event structure received  *       
    **************************************/
   cmFreeMem(&evntMsg->u.egMsg->memCp);
   evntMsg->u.egMsg = NULLP;

   /****************************************
    * Assign the mBuf in the event message *     
    ***************************************/
   evntMsg->u.mBuf = mBuf;

   /************************************************************
    * Send the message to main thread with SUCCESS result code *
    ************************************************************/
   evntMsg->resultType = EG_SUCCESS;
   retVal = egTptProcessMsg(evntMsg);
#ifdef EG_MT
   egEdmSendMsgToCm(pst, evntMsg);
#endif 

   EG_RETVALUE(retVal);
} /* End of egEncMsgReq() */



/***********************************************************************
*   Fun:  egEncIeData
*
*   Desc:  Encode the GTP Msg IE Data part
*
*   Ret:   ROK  - for Success
*          RFAILED - for Failure
*
*   Notes: None
*
*   File:  eg_edm.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncIeData
(
Buffer    *mBuf,                    /* Buffer to hold the Encoded msg */
EgIe      *egIe                      /* IE to encode the data portion */
) 
#else
PRIVATE S16 egEncIeData(mBuf, egIe)
Buffer    *mBuf;                    /* Buffer to hold the Encoded msg */
EgIe      *egIe;                     /* IE to encode the data portion */
#endif
{
   S16         retVal = ROK;                /* Holds the return value */
   Data        dumBytes[EG_MAX_BYTES];  /* tmp holds the data for Enc */

   EG_TRC2(egEncIeData);

   cmMemset((U8*)dumBytes, EG_ZERO, EG_LEN_BYTES);
   EG_DBG_INFO(0,0, (egp,
         "egEncIeData(IE Type(%d), IE Type(%d)) \n",
          egIe->ieType, egIe->dataType));

   /*****************************************************
   *  Check if the data type is out of index    (OR)    *
   *  The index of the function is not valid one        *
   ******************************************************/
   if((egIe->dataType >= EG_MAX_DATATYPE) ||
      !(Parse_function[egIe->dataType].encParseFn))
   {
       retVal = EGT_ERR_INV_IE_TYPE;
   }
   else
   {
     /**************************************************
      * Handling of encoding  of different data types  *
      **************************************************/
      retVal = Parse_function[egIe->dataType].encParseFn(egIe,mBuf);
   }

    EG_RETVALUE(retVal);    
} /* End of egEncIeData() */



/***********************************************************************
 *   Fun:  egEncIe
 *   
 *   Desc:  Encodes the IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncIe
(
EgIe   *ie,                                          /* IE to encode */
Buffer *mBuf                          /* Buffer holds the encoded IE */
)
#else
PRIVATE S16 egEncIe(ie, mBuf)
EgIe   *ie;                                          /* IE to encode */
Buffer *mBuf;                         /* Buffer holds the encoded IE */
#endif
{
   S16      retVal = ROK;                  /* Holds the return value */
   MsgLen   initLen = 0;  /* Initial mBuf Length for Grp IE encoding */
   MsgLen   finLen = 0;     /* final mBuf Length for Grp IE encoding */
   MsgLen   ieLen = 0;          /* actual length of IE being encoded */
   Data     dumBytes[EG_LEN_BYTES];   /* Tmp stores the data for Enc */
   CmLList *tmpNode = NULLP;                 /* Tmp stores the Node  */
   EgIe    *tmpIe = NULLP;                      /* Tmp Stores the ie */
   U16      cnt = 0;          /* Used to itreate thru the list of IE */
   U8       tmpByte = 0;                /*Holds the one byte for Enc */

   EG_TRC2(egEncIe);

   cmMemset((U8*)dumBytes, EG_ZERO, EG_LEN_BYTES);

   /*************************
    * Check for IE validity *
    ************************/
   if(ie == NULLP)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_RETVALUE(retVal);
   }

   /*********************
    * Debug information *
    ********************/
   EG_DBG_INFO(0,0, (egp,
         "egEncIe(IE Type(%d), instance(%d)) \n", 
          ie->ieType, ie->ieInst));

   /****************************************
    * Store the index to encode the length *
    ***************************************/
   SFndLenMsg(mBuf, &initLen);

   /**********************
    * Encode the IE Type *
    **********************/
   SAddPstMsg(ie->ieType, mBuf);

   /******************************************
    * Encode dummy bytes in the length field *
    ******************************************/
   SAddPstMsgMult((U8*)dumBytes, EG_LEN_TWO, mBuf);

   /**************************
    * Encode the IE Instance *
    **************************/
   tmpByte = (ie->ieInst & EG_MASK_INST);
   SAddPstMsg(tmpByte , mBuf);
   
   /**************************************
    * check whether this is a grouped IE *
    **************************************/
   if(ie->dataType == EG_GROUPED)
   {
      /**********************************
       * Call the encode IE recursively *
       **********************************/
      CM_LLIST_FIRST_NODE(&ie->t.grouped.gIeList, tmpNode);
      while((cnt < ie->t.grouped.gIeList.count) && (tmpNode))
      {
         tmpIe = (EgIe*)tmpNode->node;
         retVal = egEncIe(tmpIe, mBuf);
 
         /********************************
          * Encoding of IE is successful *
          ********************************/
         if(retVal == ROK)
         {
            cnt++;
            CM_LLIST_NEXT_NODE(&ie->t.grouped.gIeList, tmpNode);
         }
         else
         {
            EG_RETVALUE(retVal);
         }
      }
   }
   else
   {
      /**********************
      * Encode the IE data  *
      ***********************/ 
      retVal = egEncIeData(mBuf, ie);
      if(retVal != ROK)
         EG_RETVALUE(retVal);
   }

   /********************************************
    * Encode the IE length for this grouped IE *
    ********************************************/
    SFndLenMsg(mBuf, &finLen);

   /***********************************
    * Decrement 4bytes for the ie Hdr *
    ***********************************/
    ieLen = finLen - initLen - EG_LEN_FOUR;
   
   /***********************************************************
    * Increment the index to point the actual length location *
    ***********************************************************/
   initLen += EG_IE_LEN_OFFSET;

   /********************************************
    * Encode the offset b/w finLen and initLen *
    ********************************************/
   EG_ENC_LEN(ieLen, mBuf, initLen); 

   EG_RETVALUE(retVal);
} /* End of egEncIe() */



/***********************************************************************
 *   Fun:  egDecMsgReq
 *   
 *   Desc:  Decodes the GTP Msg and then validate it.
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef EG_MT
#ifdef ANSI
PUBLIC S16 egDecMsgReq
(
 Pst    *pst,           /* which holds the pst strucuture for MT case */
 EgEvnt *evntMsg        /* Event message having the Msg to decode msg */
)
#else
PUBLIC S16 egDecMsgReq(pst, evntMsg)
Pst     *pst;           /* which holds the pst strucuture for MT case */
EgEvnt  *evntMsg;       /* Event message having the Msg to decode msg */
#endif
#else
#ifdef ANSI
PUBLIC S16 egDecMsgReq
(
 EgEvnt  *evntMsg       /* Event message having the Msg to decode msg */
)
#else
PUBLIC S16 egDecMsgReq( evntMsg)
EgEvnt   *evntMsg;      /* Event message having the Msg to decode msg */
#endif
#endif /* EG_MT */
{
   S16            retVal = ROK;            /* stores the return value */
   Buffer         *mBuf = NULLP;/* stores buffer present in event msg */
   EgUstaDgn      dgn;        /* diagnostics for sending alaram to LM */
   MsgLen         msgLen = 0; /* Stores Message length after decoding */
   Mem            mem;                  /* Memory used for allocating */
   EgIeLevelInfo  ieLvlInfo;              /* Stores the IE Level Info */
   U16            ieLen = 0;                  /* Stores the IE Length */
   U16            instCnt = 0;           /* Loop to iterate thru inst */
   U16            cnt    = 0;        /* For iterating thru Node Lists */
   EgIeProperties ieProp;             /* Tmp stores the IE properties */
   EgMsgEntry   *gmEntry;        /* Holds the Message Entry */
   /*fix for ccpu00135658 */
   EgtMsgCat msgCat; 
   EgIe   *ie = NULLP;
   /* fix for ccpu00136924 */ 
   U8  failedIeInst;       /**< stores the failed IE Inst */ 
   U8  failedIeType;       /**< stores the failed IE type */
   S16  resultType;
#ifdef LEG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16      nmbLines = 0;
#endif
#endif 

   EG_TRC2(egDecMsgReq);

   /*********************************************************
    * Verify the NULL check for event message               *
    ********************************************************/
   if (!evntMsg)
   {
      EG_DBG_INFO(0, 0, (egp,
            "egDecMsgReq: evntMsg is NULLP \n"));
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /*********************************************************
    * Initialize memory region and pool                     *
    *********************************************************/
#ifdef EG_MT
   mem.region = pst->region;
   mem.pool   = pst->pool;
#else
   mem.region = evntMsg->srvCb->uSapCb->pst.region;
   mem.pool   = evntMsg->srvCb->uSapCb->pst.pool;
#endif /* EG_MT */

   /**************************************************
    * Initialize the diagnostic and memory structure *
    **************************************************/
    cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));

   /*********************************************************
    * Clear the IE level information                        *
    *********************************************************/
   cmMemset((U8*)&ieLvlInfo, EG_ZERO, sizeof(EgIeLevelInfo));
   cmMemset((U8*)&ieProp, EG_ZERO, sizeof(EgIeProperties));

   /******************************************************
    * Initialise the position matrix pointers to NULL    *
    ******************************************************/
   for (instCnt=0; instCnt < EG_MAX_INST; instCnt++)
   {
      for (cnt=0; cnt < EG_MAX_IE; cnt++)
      {
         /************************************
         * Initialise the msgIeIdx to NULL  *
         ***********************************/
         msgIeIdx[instCnt][cnt] = (EgIe *)NULLP;
      }
   }
   /*********************************************************
    * Fill event message result code if buffer is invalid   *
    *********************************************************/
   if (!evntMsg->u.mBuf)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_DBG_INFO(0, 0, (egp,
            "egDecMsgReq: mBuf is NULLP \n"));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 0);

      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }
   /*********************************************************
    * Assign the buffer to the local pointer and clear the  *
    * event structure buffer                                *
    *********************************************************/
   mBuf = evntMsg->u.mBuf;
   evntMsg->u.mBuf = NULLP;

   /*********************************************************
    * Allocate the memory for the event structure           *
    *********************************************************/
   if(cmAllocEvnt(sizeof(EgMsg), EG_MEM_SDU_SIZE, 
      &mem, (Ptr*)&evntMsg->u.egMsg) != ROK)
   {
      /* eg006.201: Update Event for retVal --*/
      retVal=EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 0);

      EG_DBG_INFO(0, 0, (egp,
            "egDecMsgReq: Memory allocation failed for egMsg \n"));

      /******************************************************
       * Fill and send error event message to main thread   *
       * and free memory associated with mBuf.              * 
       ******************************************************/ 
      EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);
      /* free the memory associated with mBuf */
      SPutMsg(mBuf);
      /* validation failed. send Msg to Main Thread */
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /**************************************************
    * Find the message length for tracking purpose   *
    *************************************************/
    SFndLenMsg(mBuf, &msgLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egDecMsgReq(MsgLen(%ld)) \n", msgLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egDecMsgReq(MsgLen(%d)) \n", msgLen));
#endif
#endif

   /**************************************************
    * Decode the GTP message header                  *
    **************************************************/
   gmEntry = NULLP;
   retVal = egDecodeGmHdr(evntMsg, mBuf, &gmEntry);
   if(retVal != ROK)
   {

      /***********************************************
      *  If the error is INV version then frame the  *
      *  VER NOT SUPP INDI and send the msg back to  *
      *  Originator                                  *
      ************************************************/
      if( retVal == EGT_ERR_MSG_INV_VERSION)
      {
         /********************************************
         * Prepare the header for the message        *
         * Set the last parameter to true, to        *
         * indicate the message as control msg       *
         *********************************************/
         retVal = egEncVerIndMsg(evntMsg, mBuf, TRUE);
      }
      /* ccpu00137168 */
      else if ((retVal == EGT_ERR_MSG_INV_LEN) && 
               (EG_GTPC_MSGCAT_REQUEST == egGetMsgCategory((EgtMsgType)evntMsg->u.egMsg->msgHdr.msgType)))
      {
         evntMsg->resultType = retVal;
         evntMsg->u.egMsg->msgHdr.teid = EG_ZERO;
         evntMsg->u.egMsg->msgHdr.teidPres = TRUE;
         egTunnErrIncOutRsp(evntMsg,0,0);
      }
      /*******************
       * free the memory *
       ******************/
      SPutMsg(mBuf);
      evntMsg->resultType = retVal;
      EG_RETVALUE(retVal);
   }

   /**********************************************
    *  Loop through to Decode the GTP IE's       *
    *  Decrement the length of the Hdr in the msg*
    **********************************************/
   msgLen -= EG_LEN_TWELVE;
   while (msgLen)
   {
      ieLvlInfo.level = 0;
      retVal = egDecIe(evntMsg, mBuf, &ieLvlInfo, 
                       &ieLen,NULLP,NULLP,gmEntry);
      /*******************************************
       * If decoding of IE fails then free the   *
       * the message buffer and retrun from here *
       *******************************************/
      /*fix for ccpu00137063 */
      /*fix for ccpu00136925 */
      if (retVal == EGT_ERR_MSG_MLFRMD_PKT)
      {
         evntMsg->resultType = ROK;
         SPutMsg(mBuf);
         EG_FREE_GM_MSG(evntMsg->u.egMsg);
         EG_RETVALUE(RFAILED); 
      }
      else if((retVal != ROK) && ((EgtMsgType)evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_REQ))
      {
         msgCat = egGetMsgCategory ((EgtMsgType)evntMsg->u.egMsg->msgHdr.msgType);  
         resultType=evntMsg->resultType; 
         failedIeInst=evntMsg->failedIeInst;       /**< stores the failed IE Inst */
         failedIeType=evntMsg->failedIeType;       /**< stores the failed IE type */
         if((evntMsg->lclTeid.pres != PRSNT_NODEF) || (evntMsg->lclTeid.val == 0))
         {
            while(msgLen)
            {   
               if(((ie = msgIeIdx[EG_IE_INST_ZERO][EG_IE_FTEID]) != NULLP))
               {
                  evntMsg->u.egMsg->msgHdr.teid=ie->t.fTeid.teid;
                  evntMsg->u.egMsg->msgHdr.teidPres=TRUE;
                  egTunnErrIncOutRsp(evntMsg,failedIeType,failedIeInst);
                  /* fix for ccpu00136924 */
                  failedIeInst = evntMsg->failedIeInst;       /**< stores the failed IE Inst */
                  failedIeType = evntMsg->failedIeType;       /**< stores the failed IE type */
                  evntMsg->resultType=resultType;
                  SPutMsg(mBuf);
                  EG_RETVALUE(evntMsg->resultType);
               }
               ieLvlInfo.level = 0;
               if ( ROK != (retVal = egDecIe(evntMsg, mBuf, &ieLvlInfo,&ieLen,NULLP,NULLP,gmEntry)))
               {
                  if (retVal == EGT_ERR_MSG_MLFRMD_PKT)
                  {

                     evntMsg->resultType = ROK;
                     SPutMsg(mBuf);
                     EG_RETVALUE(ROK); 
                  }
                  evntMsg->failedIeInst = failedIeInst;
                  evntMsg->failedIeType = failedIeType;
               }
               SFndLenMsg(mBuf, &msgLen);
            }
            /* ccpu00137353 */

            evntMsg->u.egMsg->msgHdr.teidPres=FALSE;
            egTunnErrIncOutRsp(evntMsg,failedIeType,failedIeInst);
            SPutMsg(mBuf);
            EG_RETVALUE(retVal);
         }
         SPutMsg(mBuf);
         /* fix for ccpu00136924  */ 
         evntMsg->failedIeInst=failedIeInst;       /**< stores the failed IE Inst */
         evntMsg->failedIeType=failedIeType;       /**< stores the failed IE type */
         evntMsg->u.egMsg->msgHdr.teid=0; 
         EG_RETVALUE(retVal); 
      }
      else if(retVal != ROK)
      {
         SPutMsg(mBuf);
         evntMsg->resultType = retVal;
         EG_RETVALUE(retVal);
      }
      SFndLenMsg(mBuf, &msgLen);
   } 

   /*****************************
    * Validate the decoded IE's *
    ****************************/
   retVal = egValidateGmMsgIEs(evntMsg,gmEntry);
   if(retVal != ROK)
   {
      /*********************************************************
       * Fill and send error event message to main thread and  *
       * free memory associated with mBuf.                     * 
       *********************************************************/ 
      /* fix for ccpu00136098 */  
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
      /*start fix for ccpu00135658*/
      msgCat = egGetMsgCategory ((EgtMsgType)evntMsg->u.egMsg->msgHdr.msgType);      
      if((EgtMsgType)evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_REQ)
      {   
         if((evntMsg->lclTeid.pres != PRSNT_NODEF) || (evntMsg->lclTeid.val == 0))
         {
            
            if(((ie = msgIeIdx[EG_IE_INST_ZERO][EG_IE_FTEID]) != NULLP)||
               ((ie = msgIeIdx[EG_IE_INST_ONE][EG_IE_FTEID]) != NULLP))
            {
               evntMsg->u.egMsg->msgHdr.teid=ie->t.fTeid.teid;
               evntMsg->u.egMsg->msgHdr.teidPres=TRUE;
               /* fix for ccpu00136098 */
               /* fix for ccpu00136924 */
               egTunnErrIncOutRsp(evntMsg,evntMsg->failedIeType,evntMsg->failedIeInst);
               SPutMsg(mBuf);
               EG_RETVALUE(retVal);
            }
            else
            {  /* ccpu00137353  */
               evntMsg->u.egMsg->msgHdr.teidPres=FALSE;
               egTunnErrIncOutRsp(evntMsg,evntMsg->failedIeType,evntMsg->failedIeInst);
               SPutMsg(mBuf); 
               evntMsg->u.egMsg->msgHdr.teid=0; 
               EG_RETVALUE(retVal); 
            }
         }

      } 
      /*end  fix for ccpu00135658 */
      else if(evntMsg->u.egMsg->msgHdr.msgType==EGT_GTPC_MSG_CS_RSP)
      {
         ie = msgIeIdx[EG_IE_INST_ZERO][EG_IE_CAUSE];
         if(( ie == NULLP) ||  !((ie->t.cause.causeVal == EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING )
              ||(ie->t.cause.causeVal == EGT_GTPC_CAUSE_INVALIED_LEN )
              || (ie->t.cause.causeVal == EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR )
              ||(ie->t.cause.causeVal == EGT_GTPC_CAUSE_REJ_INV_MSG_FMT )))   
         {    
            SPutMsg(mBuf); 
            evntMsg->resultType = retVal;
            EG_RETVALUE(retVal);
         }
      }
      else
      {
         SPutMsg(mBuf); 
         evntMsg->resultType = retVal;
         EG_RETVALUE(retVal);
      }
   }

   /*******************************************************************
   * If it the first message received, then the TEID in the message   *
   * Header will be 0. To get the message DB for the interface,       *
   * retreive the interface type from the Senders F-TEID or PGW       *
   * S5/S8 F-TEID [Control] from the received message.For the rest of *
   * the messages, Tunnel module will provide the Interface Type to   *
   * get the message DB.                                              *
   ********************************************************************
   * Get interface type either from the Senders FTEID or PGW address  *
   ********************************************************************/

   /*************************************************************
    * Check validity of the interface Id and see whether it is  *
    * request message Type or Response Message Type.            *
    *************************************************************/
   /* ccpu00137353 */
   switch(evntMsg->u.egMsg->msgHdr.msgType)
   {
       case EGT_GTPC_MSG_DPDN_CON_SET_REQ:
       case EGT_GTPC_MSG_DPDN_CON_SET_RSP:
         /* fix for ccpu00139013 */
      case  EGT_GTPC_MSG_ID_REQ:
      case  EGT_GTPC_MSG_ID_RSP:
#ifdef EG_REL_AC0
       case EGT_GTPC_MSG_PGW_RST_NOTFN:
       case EGT_GTPC_MSG_PGW_RST_NOTFN_ACK:
       case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
       case EGT_GTPC_MSG_DWN_DATA_NOTFN:
       case EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK:
       case EGT_GTPC_MSG_SSPND_NOTFN:
       case EGT_GTPC_MSG_SSPND_ACK:
#endif
          {
             if(evntMsg->u.egMsg->msgHdr.teid == 0)
   {
      evntMsg->intfType = EGT_GTP_INT_UNKNWN;
   }
          }
          break;
       default:
          break;
   }
   if (evntMsg->intfType == EGT_GTP_INT_SI)
   {
      evntMsg->intfType = EGT_GTP_INT_UNKNWN;
      retVal = egGetInfType(evntMsg);

      if(retVal != ROK)
      {
         EG_DBG_INFO(0, 0, (egp,"\n Could not get interface type"));
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 
               ieProp.ieType, ieProp.ieInst,
               evntMsg->u.egMsg->msgHdr.msgType);
         evntMsg->resultType = retVal;
         /* fix for  ccpu00136924 */
         if((evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_REQ)&&(retVal == EGT_ERR_MSG_INCORR_IE)
             &&((evntMsg->lclTeid.pres != PRSNT_NODEF) || (evntMsg->lclTeid.val == 0)))
         {
            /* fix for ccpu00136924  */ 
            if(((ie = msgIeIdx[EG_IE_INST_ZERO][EG_IE_FTEID]) != NULLP))
            {
               evntMsg->u.egMsg->msgHdr.teid=ie->t.fTeid.teid;
               evntMsg->u.egMsg->msgHdr.teidPres=TRUE;
               egTunnErrIncOutRsp(evntMsg,evntMsg->failedIeType,evntMsg->failedIeInst);
               SPutMsg(mBuf);
               evntMsg->u.egMsg->msgHdr.teid = evntMsg->lclTeid.val;
            }   
         }
         EG_RETVALUE(retVal);
      }
      /********************************************
       * If Interface Type is Invalid then report *
       * error and return from here               *
       ********************************************/
      if(!evntMsg->intfType)
      {
         
         retVal = EGT_ERR_INV_INTF_TYPE;
         EG_DBG_INFO(0, 0, (egp,"\n Invalid interface type "));
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 
                              evntMsg->u.egMsg->msgHdr.msgType);
         evntMsg->resultType = retVal;
         EG_RETVALUE(retVal);
      }

   }

   /****************************************************************
    * Decoding is successful, free the memory associated with mBuf *
    * and send the message to main thread with return as SUCCESS   *
    ***************************************************************/
   SPutMsg(mBuf);

   evntMsg->resultType = EG_SUCCESS;

#ifdef _EG_PRINT_BUF_
   EgUtilGmDump(evntMsg->u.egMsg
#ifdef LEG_FILE_LOG
                  , egCb.dbgFp, &nmbLines
#endif /* end of LEG_FILE_LOG */
                );
#endif /* end of _EG_PRINT_BUF_ */
#ifdef EG_MT
   /************************
    * Send Message to CPM  *
    ************************/
   egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
   retVal = egTunReceive(evntMsg);

   EG_RETVALUE(retVal);
}  /* End of egDecMsgReq() */


/***********************************************************************
 *   Fun:  egGetInfType
 *   
 *   Desc:  Updates interface type
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egGetInfType
(
 EgEvnt    *evntMsg         /* Event message stores the decoded hdr */
)
#else
PRIVATE S16 egGetInfType(evntMsg)
EgEvnt     *evntMsg;         /* Event message stores the decoded hdr */
#endif
{
 
   S16      retVal = ROK;                  /* Holds the return value */
   EgIe     *ie = NULLP;                /* Pointer to the IE CB */

   EG_TRC2(egGetInfType);

   if ((evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_REQ) ||
       (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_RSP)
#if defined(EG_REL_930) || defined(EG_REL_AC0)
       || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_MBMS_SESS_START_REQ)
#endif /* EG_REL_930 */
      )
   {
     /**********************************************************
       * In create session response message, Senders F-TEID is  *
       * mandatory on S11 and on S5 it is optional, so in this  *
       * case PGW F-TEID is required.                           *
       **********************************************************
       * Determine Interface type while processing Instance 0   *
       **********************************************************/
      if ((ie = msgIeIdx[EG_IE_INST_ZERO][EG_IE_FTEID]) != NULLP)
      {
         if((ie->t.fTeid.intfType == EGT_INT_S5_S8_SGW_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S5_S8_PGW_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S5_S8;
         }
         else if((ie->t.fTeid.intfType == EGT_INT_S11_MME_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S11_S4_SGW_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S11;
         }
         else if((ie->t.fTeid.intfType == EGT_INT_S4_SGSN_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S4;
         }
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         else if((ie->t.fTeid.intfType == EGT_INT_SM_MBMSGW_GTP_C) ||
                     (ie->t.fTeid.intfType == EGT_INT_SM_MME_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_SM;
         }
         else if((ie->t.fTeid.intfType == EGT_INT_SN_MBMSGW_GTP_C) || 
                     (ie->t.fTeid.intfType == EGT_INT_SN_SGSN_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_SN;
         }
#endif /* EG_REL_930 */
         /****************************
          * eg009.201 -Added for S2B *
          ****************************/
#ifdef EG_REL_AC0
         else if((ie->t.fTeid.intfType == EGT_INT_S2B_PGW_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S2B_EPDG_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S2B;
         }
#endif /* EG_REL_AC0 */
         /****************************
          * eg009.201 -Added for S2A *
          ****************************/
#ifdef EG_S2A_SUPP
         else if((ie->t.fTeid.intfType == EGT_INT_S2A_PGW_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S2A_TWAN_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S2A;
         }
#endif /* EG_S2A_SUPP */
         else
         {
            evntMsg->intfType = EGT_GTP_INT_UNKNWN;
         }

      }
      /**********************************************************
       * Determine Interface type while processing Instance 1   *
       **********************************************************/
      else if((ie =  msgIeIdx[EG_IE_INST_ONE][EG_IE_FTEID]) != NULLP)
      {
         if( (ie->t.fTeid.intfType == EGT_INT_S5_S8_SGW_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S5_S8_PGW_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S5_S8;
         }
         else if((ie->t.fTeid.intfType == EGT_INT_S11_MME_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S11_S4_SGW_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S11;
         }
         else if (ie->t.fTeid.intfType == EGT_INT_S4_SGSN_GTP_C)
         {
            evntMsg->intfType = EGT_GTP_INT_S4;
         }
         else
         {
            evntMsg->intfType = EGT_GTP_INT_UNKNWN;
         }
      }
      else
      {
         /**********************************************************
          * if both elements are not present, then we can drop the *
          * message, Drop the event message and then log the error *
          **********************************************************/
         retVal = EGT_ERR_MSG_MISS_MAND_IE;
         EG_RETVALUE(retVal);
      }
   }
   else if ((evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_FR_REQ) ||
         (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_FR_RSP) ||
         (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CNTXT_REQ))
   {
      /**********************************************************
       * In Forward Relocation response message, Senders F-TEID is  *
       * mandatory on S11 and on S5 it is optional, so in this  *
       * case PGW F-TEID is required.                           *
       **********************************************************
       * Determine Interface type while processing Instance 0   *
       **********************************************************/
      if ((ie =  msgIeIdx[EG_IE_INST_ZERO][EG_IE_FTEID])!= NULLP)
      {
         if (ie->t.fTeid.intfType == EGT_INT_S10_MME_GTP_C)
         {
            evntMsg->intfType = EGT_GTP_INT_S10;
         }
         else if((ie->t.fTeid.intfType == EGT_INT_S3_MME_GTP_C) || 
               (ie->t.fTeid.intfType == EGT_INT_S3_SGSN_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S3;
         }
         else if((ie->t.fTeid.intfType == EGT_INT_S16_SGSN_GTP_C))
         {
            evntMsg->intfType = EGT_GTP_INT_S16;
         }
         else
         {
            evntMsg->intfType = EGT_GTP_INT_UNKNWN;
         }
      }
      else
      {
         /**********************************************************
          * if both elements are not present, then we can drop the *
          * message, Drop the event message and then log the error *
          **********************************************************/
         retVal = EGT_ERR_MSG_MISS_MAND_IE;
         EG_RETVALUE(retVal);
      }
   }
   /*fix for ccpu00136924*/
   if((evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_REQ))
   {
      retVal = egValidateInftType(evntMsg->intfType,ie->t.fTeid.intfType);
   }
   EG_RETVALUE(retVal);
}


/***********************************************************************
 *   Fun:  egDecodeGmHdr
 *   
 *   Desc:  Decodes the GTP Msg Header
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecodeGmHdr
(
 EgEvnt    *evntMsg,         /* Event message stores the decoded hdr */
 Buffer    *mBuf,                      /* Buffer to decode the header */
 EgMsgEntry   **gmEntry        /* Holds the Message Entry */
)
#else
PRIVATE S16 egDecodeGmHdr(evntMsg, mBuf,gmEntry)
EgEvnt     *evntMsg;         /* Event message stores the decoded hdr */
Buffer     *mBuf;                     /* Buffer to decode the header */
EgMsgEntry    **gmEntry;        /* Holds the Message Entry */
#endif
{
   S16      retVal = ROK;                  /* Holds the return value */
   MsgLen   msgLen = 0;             /* Holds the msgLen from the Hdr */
   MsgLen   bufLen = 0;             /* Holds the total buffer length */
   U8       tmpByte = 0;         /* Holds one byte of data after Dec */
   S16      retValue = ROK;                  /* Holds the return value */

   EG_TRC2(egDecodeGmHdr);

   /*********************************************
    * Check the length of buffer to be decoded  *
    *********************************************/
   SFndLenMsg(mBuf, &bufLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egDecodeGmHdr(MsgLen(%ld)) \n", bufLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egDecodeGmHdr(MsgLen(%d)) \n", bufLen));
#endif
#endif

   /********************************************************
    * Msg header is 8 bytes in case of ECHO REQ/RSP , and  *
    * 12 bytes in case of all control messages atleast msg *
    * should be of this much length                        *
    * Since the Echo request or response will not this fn  *
    * the minimum length should be 12bytes                 *
    *******************************************************/
   if(bufLen <  EG_MIN_MSG_LEN)
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
            "egDecodeGmHdr(MsgLen(%ld), resultType(%d)) -- Invalid \n",
       bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
            "egDecodeGmHdr(MsgLen(%d), resultType(%d)) -- Invalid \n",
       bufLen, retVal));
#endif
#endif
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal); 
   }
   
   /******************************************
    * decode the version & Right shift       *
    * 5 times to store in the version field  *
    ******************************************/
   SRemPreMsg(&tmpByte, mBuf);
   evntMsg->u.egMsg->msgHdr.version = tmpByte >> EG_SHIFT_5;

   /*eg010.201:Decode Piggyback bit*/
#ifdef EG_PIGGYBACK_SUPP
   evntMsg->u.egMsg->msgHdr.pMsgPres = (tmpByte & EG_MASK_BIT5) >> EG_SHIFT_4;
#endif

   /******************************
    * decode the message Type    *
    ******************************/
   SRemPreMsg(&(evntMsg->u.egMsg->msgHdr.msgType), mBuf);

   /*********************************************************
    * Check if the version is 2, if not report INV version  *
    *********************************************************/
   if(evntMsg->u.egMsg->msgHdr.version != EG_MSG_VERSION)
   {
      retVal = EGT_ERR_MSG_INV_VERSION;
      EG_DBG_INFO(0,0, (egp,
             "egDecodeGmHdr(Invalid Msg Version(%d), \
              resultType(%d) \n", \
              evntMsg->u.egMsg->msgHdr.version, retVal));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }

   /******************************
    * decode the message length  *
    *****************************/
    EG_DEC_U16(msgLen, mBuf);
   
   /*******************************************************************
    * The message length should not have the first 4 mandatory octets *
    * Check whether the message lenght is less than the 4 octets on   *
    * the total buffer length                                         *
    ******************************************************************/
   if(msgLen != bufLen - EG_LEN_FOUR)
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
            "egDecodeGmHdr(Decoded MsgLen(%ld), actual MsgLen(%ld), \
            resultType(%d)) -- Invalid Length \n", 
            msgLen, bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
            "egDecodeGmHdr(Decoded MsgLen(%d), actual MsgLen(%d), \
             resultType(%d)) -- Invalid Length \n", 
            msgLen, bufLen, retVal));
#endif            
#endif            
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }
   bufLen -= EG_LEN_FOUR;

   /***********************************
    * decode the Teid if T-bit is set *
    ***********************************/
   if ( tmpByte & EG_MASK_BIT4 )
   {
      evntMsg->u.egMsg->msgHdr.teidPres = TRUE;
      EG_DEC_U32(evntMsg->u.egMsg->msgHdr.teid, mBuf);
      bufLen -= EG_LEN_FOUR;
   }
   
   /**********************************************
    * decode the seq no(3octets) and spare byte  *
    **********************************************/
    EG_DEC_U32(evntMsg->u.egMsg->msgHdr.seqNumber, mBuf);
    evntMsg->u.egMsg->msgHdr.seqNumber >>= EG_SHIFT_8;
    evntMsg->u.egMsg->msgHdr.seqNumber &= 0x00FFFFFF; 
    bufLen -= EG_LEN_FOUR;
  
   /******************************************************************
    * check for msgLen error, if no error continue, otherwise return *
    ******************************************************************/
   if(retVal == ROK)
   {
      /***************************************
       * Call to validate the GTP Msg Header *
       **************************************/
      retVal = egValidateGmHdr(evntMsg);
   }
   if(retVal == ROK)
   {
     /******************************************************************
      * Validate the message Type, Get the message entry by taking the *
      * interface Type. If invalide message entry then log the error   *
      ******************************************************************/
     retValue = EGT_ERR_MSG_INV_TYPE;
     /* ccpu00137353  */
     if(!evntMsg->u.egMsg->msgHdr.teid && 
        ((evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CS_REQ) 
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_FR_REQ)
#if defined(EG_REL_930) || defined(EG_REL_AC0)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_MBMS_SESS_START_REQ)
#endif /* EG_REL_930 */
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_DPDN_CON_SET_REQ)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_DPDN_CON_SET_RSP)
#ifdef EG_REL_AC0
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_PGW_RST_NOTFN)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_PGW_RST_NOTFN_ACK)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_DWN_DATA_NOTFN)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_DD_NOTFN_FAIL_IND)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_SSPND_NOTFN)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_SSPND_ACK)
#endif
        /*fix for ccpu00139013 */
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_ID_REQ)
        || (evntMsg->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_ID_RSP)
        ))
     {
       evntMsg->intfType = EGT_GTP_INT_SI;
     }

     if(evntMsg->intfType != EGT_GTP_INT_UNKNWN)
     {
        retValue = ROK;
        *gmEntry = (EgMsgEntry *)NULLP;
        *gmEntry = egDbGetMsgFrmGmDict(evntMsg->u.egMsg->msgHdr.msgType,
           evntMsg->intfType); 
        /* eg012:201 : Fixed NULL check */
        if (NULLP == *gmEntry)
        {
           retValue = EGT_ERR_MSG_INV_TYPE;
        }
     }
     if(retValue == EGT_ERR_MSG_INV_TYPE)
     {
       EG_DBG_ERR(0,0, (egp,
             "egValidateGmHdr(Invalid Message Type(%d) resultType(%d) \n",
             evntMsg->u.egMsg->msgHdr.msgType, retValue));
       EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, 
           evntMsg->u.egMsg->msgHdr.msgType);
       EG_RETVALUE(retValue);
     }
   }
   EG_RETVALUE(retVal); 
} /* End of egDecodeGmHdr () */




/***********************************************************************
 *   Fun:  egValidateGmHdr
 *   
 *   Desc:  To Validate GTP Msg header as per specs.
 *   
 *   Ret:   ROK  - for Success
 *          EGT_ERR_INV_HDR -   Invalid GTP Header
 *          EGT_ERR_UNSUPP_VER - VERSION not supported
 *          EGT_ERR_MSG_INV_TYPE - Invalid Message Type
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egValidateGmHdr
(
EgEvnt    *evntMsg                      /* Event message to validate */
)
#else
PRIVATE S16 egValidateGmHdr( evntMsg)
EgEvnt *evntMsg;                        /* Event message to validate */
#endif
{
   S16           retValue = ROK;           /* Holds the return value */
   EgtMsgCat     msgCat;

   /*************************************************************
    *  Trace the function only if debug mask is enabled for EDM *
    *************************************************************/
   EG_TRC2(egValidateGmHdr)


   EG_DBG_INFO(0,0, (egp,
            "egValidateGmHdr( Message Type (%d) \n",
             evntMsg->u.egMsg->msgHdr.msgType));

   /********************************************************
    * Check for T- BIT. If the T-bit is not set in case of *
    * the all control messages then its a error            *
    ********************************************************/
    if (!evntMsg->u.egMsg->msgHdr.teidPres)
    {
      /*************************************************************
       * If the message is other than the ECHO REQ/RSP, then its a *
       * error TEID should be present in all the other messages    *
       *************************************************************/
       if((evntMsg->u.egMsg->msgHdr.msgType != EGT_GTPC_MSG_ECHO_REQ) && 
         ( evntMsg->u.egMsg->msgHdr.msgType != EGT_GTPC_MSG_ECHO_RSP) &&
         ( evntMsg->u.egMsg->msgHdr.msgType != EGT_GTPC_MSG_VER_N_SUPP_IND))
       {
          /*************************************
           *  Report invalid message Format    *
           *************************************/
          retValue = EGT_ERR_MSG_INV_HDR;
          EG_DBG_INFO(0,0, (egp,
                "egValidateGmHdr(Invalid message header: resultType(%d) \n",
                   retValue));

          EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, evntMsg->u.egMsg->msgHdr.msgType);
          EG_RETVALUE(retValue);
       } 
    }
    /****************************************************
    * If the message is CSREQ/CSRSP the TEID will be  0 *
    * In this case fill the interface is not known at   *
    * this time. So skipping the validation of the Msg  *
    * Entry. After validation of GM HDR, the intf will  *
    * be filled from the Senders/PWG F-TEID IE value.   *
    * If the TEID is not ZERO, then we will retireve    *
    * from the saved previously using the data base.    *
    ****************************************************/
    if(evntMsg->u.egMsg->msgHdr.teid)
    {
       if(evntMsg->lclTeid.pres == PRSNT_NODEF)
       {
          evntMsg->intfType = egGetIntfFrmTeid(evntMsg->lclTeid.val,(evntMsg->crtFlag.pres?evntMsg->crtFlag.val:0));
          /* ccpu00137922, ccpu00137318 : No Response will be sent from stack */
          if (evntMsg->intfType == EGT_GTP_INT_UNKNWN)
       {
             retValue = EGT_ERR_INV_TEID_RCVD;
          }
          /* fix for ccpu00137318 */
       }
       else
       {
          /* fix for ccpu00136098,ccpu137318 */
          msgCat = egGetMsgCategory((EgtMsgType)evntMsg->u.egMsg->msgHdr.msgType);
          if((msgCat == EG_GTPC_MSGCAT_RESPONSE) || (msgCat == EG_GTPC_MSGCAT_FAILIND))
          {
             EG_RETVALUE(retValue);
          }

           retValue = EGT_ERR_INV_TEID;
#ifdef ALIGN_64BIT
           EG_DBG_INFO(0,0, (egp,
                   "egValidateGmHdr(Invalid teid(%d) \n", evntMsg->lclTeid.val));
#else
           EG_DBG_INFO(0,0, (egp,
                   "egValidateGmHdr(Invalid teid(%ld) \n", evntMsg->lclTeid.val));
#endif

           EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, 
                               evntMsg->u.egMsg->msgHdr.msgType);
           EG_RETVALUE(retValue);
       }
     }

   EG_RETVALUE (retValue);
}  /* end of egValidateGmHdr() */


/**********************************************************************
 *   Fun:  egDecIe
 *   
 *   Desc:  Decodes the GTP IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecIe
(
EgEvnt         *evntMsg,  /* Event message to stores the decoded ie */
Buffer         *mBuf,                 /* Buffer which has to decode */
EgIeLevelInfo  *ieLvlInfo,  /* Level info to store in the event msg */
U16            *outIeLen,          /* Contains the Ie len to decode */
U8              *posIndex,      /* IE Properties Position Idx*/
EgIeProperties  *ieProperties,     /* IE Properties Array*/
EgMsgEntry     *gmEntry         /* msg db in dictionary */
)
#else
PRIVATE S16 egDecIe(evntMsg, mBuf, ieLvlInfo, outIeLen,posIndex,ieProperties,gmEntry)
EgEvnt     *evntMsg;       /* Event message to stores the decoded ie */
Buffer         *mBuf;                  /* Buffer which has to decode */
EgIeLevelInfo *ieLvlInfo;    /* Level info to store in the event msg */
U16            *outIeLen;           /* Contains the Ie len to decode */
U8              *posIndex;      /* IE Properties Position Idx*/
EgIeProperties  *ieProperties;     /* IE Properties Array*/
EgMsgEntry     *gmEntry;         /* msg db in dictionary */
#endif
{
   S16              retVal = ROK;              /* Holds return value */
   EgIe             tmpIe;                   /* tmp Holds Decoded IE */
   EgIe             *egIe = NULLP;      /* Used to store PTR of Node */
   EgIeEntry      *ieEntry = NULLP;                /* Holds IE Entry */
   MsgLen           msgLen = 0;           /* Holds Rcd Buffer length */
   U16              tmpIeLen = 0;             /* Holds the IE Length */
   EgUstaDgn        dgn;             /* Used for sending the alarms  */
   EgIeLevelInfo    tmpLvlInfo;      /* Stores the Level Info for IE */
   U16              cnt = 0;         /* Used to iterate thru IE list */
   U16              idx = 0;          /* Used to Get IE index in DB  */
   /*eg009.201:Warning fixed,unused variable grpOccCnt*/
   U8               tmpByte =0;
   EgIeProperties   ieProp;             /* Holds IE prop for rcd msg */
   U8              skipIe[EG_MAX_BYTES];  /* To Skip IE from the buf */
   MsgLen           curBufLen = 0;     /* Holds curent Buffer length */
   EgIe             *tmpIeNode;
   U16              tmpIeLen2 = 0;     /* Holds the IE Length, to take care of BCD */

   EG_TRC2(egDecIe);
   
   cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
   cmMemset((U8*)&tmpIe, EG_ZERO, sizeof(EgIe));
   cmMemset((U8*)skipIe, EG_ZERO, EG_MAX_BYTES);
   cmMemset((U8*)&tmpLvlInfo, EG_ZERO, sizeof(tmpLvlInfo));

   /*****************************************
    * find the length of the encoded buffer *
    *****************************************/
   SFndLenMsg(mBuf, &msgLen);

   /* ccpu00136925 : Fix added to avoid crash */
   if(msgLen <= 4)
   {
      EG_RETVALUE(EGT_ERR_MSG_MLFRMD_PKT);
   }

   /*************************
    * decode the IE Type    *
    ************************/
   SRemPreMsg(&tmpIe.ieType, mBuf);
   msgLen--;

   /************************
    * decode the IE length *
    ***********************/
   EG_DEC_U16(tmpIeLen, mBuf);
   msgLen -= EG_LEN_TWO;
   *outIeLen = tmpIeLen;
   /* ccpu00137362 : Checking the value received in IE Length */
   if ((tmpIeLen == 0) || (tmpIeLen >= msgLen))
   {
      EG_RETVALUE(EGT_ERR_MSG_MLFRMD_PKT);
   }

   /***************************
    * decode the IE Instance  *
    ***************************/
   SRemPreMsg(&tmpByte, mBuf);
   msgLen--;

   /********************************************************
    * Get the instance from the tmp Byte as the 4th octet  *
    * is Instance. Instance is present in 0,1,2,3 bits pos,*
    * 5th to 7th bit is spare.                             *
    ********************************************************/
   tmpIe.ieInst = (tmpByte & EG_MASK_INST); /* Mask value is 0x0F */

   /*******************************************
    * get the IE entry from the IE Dictionary *
    ******************************************/
   ieEntry = egGetIeEntryFrmDb(tmpIe.ieType);
  
    if( ieEntry == NULLP)
    {
      /*****************************************
       * Reject the message and send the error *
       * indication to the application         *
       *****************************************/
      EG_DBG_INFO(0,0, (egp,
            "egDecIe(IE(%d), \
        resultType(%d) not configured in Dict) \n",
            tmpIe.ieType,  retVal));
      /* ccpu00137579 : 29.274 Section 7 
         Unknown IEs can be ignored for backward compatibilty */
      SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
      msgLen -= tmpIeLen;
      EG_RETVALUE(retVal);
    }
    else
    {
        EG_DBG_INFO(0,0, (egp,
         "egDecIe:IEType(%d) \n",tmpIe.ieType));

      /***********************************************************
       * Position Index zero means, we have passed base level IE *
       * *********************************************************/
       if( posIndex == NULLP || ieProperties == NULLP)
      {
        posIndex = (U8 *)&(gmEntry->posIndex);
        ieProperties = (EgIeProperties *)&(gmEntry->ieProperties);
      }
      /**********************************************************************
       * check for the ie flags, if optional and ie not found in the Dict   *
       * skip the IE pass position idx to get its properties                *
       *********************************************************************/
      /* fix for ccpu00137063 */
      if(ieEntry->dataType == EG_BCD)
      {
         Data bcdLsb = 0;
         SExamMsg(&bcdLsb, mBuf, tmpIeLen-1); 
         if ((bcdLsb & 0xF0) == 0xF0)
         {
            tmpIeLen2 = 2*tmpIeLen -1;
         }
         else
         {
            tmpIeLen2 = 2*tmpIeLen;
         }
      }
      else
      {
         tmpIeLen2 = tmpIeLen;
      }

      retVal = egValidateIeLen(&tmpIe, ieEntry, posIndex, ieProperties, tmpIeLen2); 
      /* fix for ccpu00137579  */   
      if ((retVal == EGT_ERR_IE_INV_LEN) || (retVal == EGT_ERR_MSG_MISS_MAND_IE))
      {
        /************************************
         * Invalid length, report error     *
         ************************************/
        EG_DBG_INFO(0,0, (egp,
              "egDecIe(IE(%d), resultType(%d)) \n",
              tmpIe.ieType,  retVal));
        /*********************************************************************
        * eg006.201 : Replaced hard coded values with the varaible instance  * 
        *********************************************************************/
        EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, tmpIe.ieInst,
            evntMsg->u.egMsg->msgHdr.msgType);
        {   
           SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
           msgLen -= tmpIeLen;
        }
        EG_RETVALUE(retVal);
      }
      else if( retVal == EG_SKIP_OPT_IE)
      {
        /*********************************************************
         * Report invalid length error in case invalid IE length *
         ********************************************************/
        if (tmpIeLen > msgLen)
        {

          /************************************
           * Invalid length, report error     *
           ************************************/
          retVal = EGT_ERR_IE_INV_LEN;
          EG_DBG_INFO(0,0, (egp,
                "egDecIe(IE(%d), resultType(%d)) \n",
                tmpIe.ieType,  retVal));
          /*********************************************************************
          * eg006.201 : Replaced hard coded values with the varaible instance * 
          *********************************************************************/
          EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, tmpIe.ieInst,
              evntMsg->u.egMsg->msgHdr.msgType);
          EG_RETVALUE(retVal);

        }

        /***************************************************
         * skip this IE and continue further processing    *
         **************************************************/
        /* fix for ccpu00137063 */
        SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
        msgLen -= tmpIeLen;
        EG_RETVALUE(ROK);
      }
    }
    /******************************************************
     * idx validation is already done in egValidateIeLen  *
     * ***************************************************/
     idx = *(posIndex+(EG_MAX_IE * tmpIe.ieInst)+tmpIe.ieType);
     ieProp = ieProperties[idx-1];
    /********************************************
     * retreive the data type from the ie entry *
     *******************************************/
    tmpIe.dataType = ieEntry->dataType;

    /*******************************************
     * check whether this IE is grouped IE     *
     ******************************************/
   if(tmpIe.dataType == EG_GROUPED)
   {
      /***********************************
       * Fill the IE Level Info          *
       **********************************/
      EG_FILL_IE_LVL_INFO(ieLvlInfo, ieLvlInfo->level,
                          tmpIe.ieType, tmpIe.ieInst);
 
      /***********************************
       * Append this IE into the IE List *
       **********************************/
      retVal = EgUtilGmAppendIe(evntMsg->u.egMsg, ieLvlInfo, &tmpIe);

      /***********************************************
       * This macro checks for the memory allocation *
       * Fails or not. If fails send the report to   *
       * LM and then returns RFAILED                 *
       **********************************************/
      EG_CHK_MEMFAIL(evntMsg, tmpIe, mBuf, retVal);

      /**********************************
      *  Frame the msgIeIdx for this    *
      *  Grouped IE for validation      *
      **********************************/
      /**************************************************
       * Here we are going to do these validations only *
       * for Level 0, rest of the levels we are not     *
       * taking care, let's fix that too                *
       * ***********************************************/
      if(!ieLvlInfo->level)
      {
   
         egIe = (EgIe*)(evntMsg->u.egMsg->egIeList.crnt->node);
         /* This can be a repeated IE */
         /* 
          * if we are inserting msgIeIdx for all IEs and if an IE inside a 
          * grouped IE is same as one already present then are we skipping
          * those IEs
          */
         if(msgIeIdx[egIe->ieInst][egIe->ieType] == NULLP || ieProp.ieList)
         {
            /*eg012.201: fix for two bearer contexts with same instance for CreateIndirectForward request, fix for ccpu00130004*/
            if( msgIeIdx[egIe->ieInst][egIe->ieType] != NULLP)
           {
              if(( EgUtilGmGetIeOccrCnt( evntMsg->u.egMsg, ieLvlInfo, &(ieLvlInfo->ieInfo[ieLvlInfo->level].occr))) == ROK)
                 {
                     ieLvlInfo->ieInfo[ieLvlInfo->level].occrPres = TRUE;
                 }
             }

            msgIeIdx[egIe->ieInst][egIe->ieType] = egIe;
         }
        else
         {
            /****************************************************
             * Repeated information element, delete this IE and *
             * continue further processing, return the success  *
             ****************************************************/
            EG_FILL_IE_LVL_INFO(ieLvlInfo, ieLvlInfo->level,
                                egIe->ieType, egIe->ieInst);
            /****************************************************
            * The Second occurance IE should be deleted. so fill*
            * the occurance pres to TRUE and occr as 2          *
            ****************************************************/
            ieLvlInfo->ieInfo[ieLvlInfo->level].occrPres = TRUE;
            ieLvlInfo->ieInfo[ieLvlInfo->level].occr = 2;

            /***************************************************
            * Since this is the gouped IE, we need to delete   *
            * parent and all the child IE's belong to this IE  *
            ***************************************************/
            EgUtilGmDelGrpIe(evntMsg->u.egMsg, ieLvlInfo);
  
            /****************************************************
             * Since this is the grouped IE, All the Child IE's *
             * are also has to be deleted, so remove the length *
             * Bytes present in this IE from the mBuf           *
             ****************************************************/
            SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
            EG_RETVALUE(ROK);
         }
      }
      ieLvlInfo->level++;

      /********************************************
       * Call this function recursively to decode *
       * the IEs in the Group                     *
       ********************************************/
      retVal = egDecGrpIe(evntMsg, mBuf, ieLvlInfo,tmpIeLen,
                                 (U8 *)&ieProp.groupedIe->posIndex,ieProp.groupedIe->ieProperties, gmEntry);
      if(retVal != ROK)
      {
         EG_RETVALUE(retVal);
      }

      /***********************************************
       * Restore the level of parent IE, so that the *
       * remaining IE's can be appended correctly    *
       ***********************************************/
      ieLvlInfo->level--;

      EG_FILL_IE_LVL_INFO(ieLvlInfo, ieLvlInfo->level, 
                          tmpIe.ieType, tmpIe.ieInst);
   }
   else
   /******************
    * non-grouped IE *
    ******************/
   {
     /*************************************************
      * Check if the data type is out of index (OR)   *
      * The index of the function is not valid one    *
      *************************************************/
     if((tmpIe.dataType >=EG_MAX_DATATYPE) ||
         !(Parse_function[tmpIe.dataType].decParseFn))
     {
       retVal = EGT_ERR_INV_IE_TYPE;
     }
     else
     {
       retVal = Parse_function[tmpIe.dataType].decParseFn(&tmpIe,mBuf,tmpIeLen);
     }
     if(retVal != ROK)
     {
       EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,
           tmpIe.ieInst, evntMsg->u.egMsg->msgHdr.msgType);
       EG_RETVALUE(retVal);
     }

     /**********************
      * if decoding fails  *
      **********************/
     if(retVal != ROK)
     {
       if( ieProp.ieClass != EG_IE_MANDATORY )
       {
         EG_DBG_INFO(0,0, (egp,
               "egDecIe(Optional IE(%d), messageType(%d)\
           resultType(%d): data part is incorreting. Skipping it) \n",
               tmpIe.ieType,evntMsg->u.egMsg->msgHdr.msgType,retVal));
         /***************************************************** 
          * skip this IE and continue further processing      *
          * Till this point we might not decoded the complete *
          * IE data, so find the left over data part and      *
          * remove it from the buffer                         *
          *****************************************************/
         SFndLenMsg(mBuf, &curBufLen);
         if(curBufLen < msgLen)
         {
           tmpIeLen -= (msgLen - curBufLen);
           SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
           msgLen -= tmpIeLen;
         }
         EG_RETVALUE(ROK);
       }
       else
       {
         EG_DBG_INFO(0,0, (egp,
               "egDecIe(Mandatory IE(%d), messageType(%d) \
           resultType(%d): data part is incorrect in Msg) \n",
               tmpIe.ieType,evntMsg->u.egMsg->msgHdr.msgType, retVal));

         /*************************************
          * Raise an alarm and return FAILURE *
          ************************************/
         dgn.dgnVal[0].type = LEG_PAR_IECODE;
         egSendLmAlarm(LCM_CATEGORY_LYR_SPECIFIC,
             LCM_EVENT_LYR_SPECIFIC,
             LCM_CAUSE_DECODE_ERR, &dgn); 

         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 
             tmpIe.ieType, tmpIe.ieInst, 
             evntMsg->u.egMsg->msgHdr.msgType); 
         EG_RETVALUE(retVal);
       }
     }

     /******************************************************************
      * check whether it is a grouped IE, if it is check whether the   *
      * parent IE of this IE is already present, and use this count to *
      * append the IE                                                  *
      ******************************************************************/
     if(ieLvlInfo->level)
     {
       for(cnt = 0; cnt < ieLvlInfo->level; cnt++)
       { 
         tmpLvlInfo.ieInfo[cnt].ieTypePres = ieLvlInfo->ieInfo[cnt].ieTypePres;
         tmpLvlInfo.ieInfo[cnt].ieType = ieLvlInfo->ieInfo[cnt].ieType;
         tmpLvlInfo.ieInfo[cnt].ieInst = ieLvlInfo->ieInfo[cnt].ieInst;
       }
       tmpLvlInfo.level =  ieLvlInfo->level - 1;
       /*eg009.201:Klockwork fix-variable not usedafter assignment*/
     }

     /**********************************************************
      * Frame the posIndex array for all the level 0 IE's only *
      **********************************************************/
     retVal = EgUtilGmAppendIe(evntMsg->u.egMsg, ieLvlInfo, &tmpIe);

     /***********************************************
      * This macro checks for the memory allocation *
      * Fails or not. If fails send the report to   *
      * LM and then returns RFAILED                 *
      **********************************************/
     EG_CHK_MEMFAIL(evntMsg, tmpIe, mBuf, retVal);

     if(!ieLvlInfo->level)
     {
       tmpIeNode = (EgIe*)(evntMsg->u.egMsg->egIeList.crnt->node);
       if(msgIeIdx[tmpIeNode->ieInst][tmpIeNode->ieType] == NULLP || ieProp.ieList)
       {
         msgIeIdx[tmpIeNode->ieInst][tmpIeNode->ieType] = tmpIeNode;
       }
       else
       {
         /****************************************************
          * Repeated information element, delete this IE and *
          * continue further processing, return the success  *
          ****************************************************/
         EG_FILL_IE_LVL_INFO(ieLvlInfo, ieLvlInfo->level,
             tmpIeNode->ieType, tmpIeNode->ieInst);
         /****************************************************
          * The Second occurance IE should be deleted. so fill*
          * the occurance pres to TRUE and occr as 2          *
          ****************************************************/
         ieLvlInfo->ieInfo[ieLvlInfo->level].occrPres = TRUE;
         ieLvlInfo->ieInfo[ieLvlInfo->level].occr = 2;

         EgUtilGmDelIe(evntMsg->u.egMsg, ieLvlInfo, FALSE);

         EG_RETVALUE(ROK);
       }
     }
   } 
   
   EG_RETVALUE(retVal); 
} /*End of egDecIe() */


/**********************************************************************
 *   Fun:  egDecIeData
 *   
 *   Desc:  Decodes the GTP Msg IE Data part 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecIeData
(
EgEvnt     *evntMsg,       /* Event message to stores the decoded ie */
Buffer     *mBuf,                      /* Buffer which has to decode */
EgIe       *egIe,                 /* Ie to store the decoded IE data */
U16        ieLen                        /* IE Length psnt in the Hdr */
)
#else
PRIVATE S16 egDecIeData(evntMsg, mBuf, egIe, ieLen)
EgEvnt     *evntMsg;       /* Event message to stores the decoded ie */
Buffer     *mBuf;                      /* Buffer which has to decode */
EgIe       *egIe;                 /* Ie to store the decoded IE data */
U16        ieLen;                 /* Total Ie Length excluding hdr   */
#endif
{
   S16      retVal;                       /* Holds the return value */
   Data     dumBytes[EG_MAX_BYTES];  /* Holds the Data for decoding */
   /*eg009.201: Klockwork fix -Variable not used*/

   EG_TRC2(egDecIeData);

   retVal = ROK;
   
   cmMemset((U8*)dumBytes, EG_ZERO, EG_MAX_BYTES);

   EG_DBG_INFO(0,0, (egp,
         "egDecIeData(IE Type(%d), IE Type(%d)) \n",
         egIe->ieType, egIe->dataType));

   /**************************************************
    *  Check if the data type is out of index (OR)   *
    *  The index of the function is not valid one    *
    **************************************************/
   if((egIe->dataType >=EG_MAX_DATATYPE) || 
      !(Parse_function[egIe->dataType].decParseFn))
   {
       retVal = EGT_ERR_INV_IE_TYPE;
   }
   else
   {
       retVal = Parse_function[egIe->dataType].decParseFn(egIe,mBuf,ieLen);
   }
   if(retVal == EGT_ERR_IE_INV_LEN)
   {
       EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, egIe->ieType,
                      egIe->ieInst, evntMsg->u.egMsg->msgHdr.msgType);
   }

   EG_RETVALUE(retVal);
} /* end of egDecIeData() */



/**********************************************************************
 *   Fun:  egDecGrpIe
 *   
 *   Desc:  Decodes the GTP Grouped IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecGrpIe
(
EgEvnt         *evntMsg,                   /* Holds the decoded IE's */
Buffer         *mBuf,                    /* Holds the data to decode */
EgIeLevelInfo  *ieLvlInfo,          /* The Level of the IE to decode */
U16            inIeLen,                     /* the Decoded IE Length */
U8             *posIndex,         /* IE's position idx in the msg db */
EgIeProperties *ieProperties,     /* IE properties in the msg db */
EgMsgEntry     *gmEntry           /* Msg db in dictionary */
)
#else
PRIVATE S16 egDecGrpIe(evntMsg, mBuf, ieLvlInfo, inIeLen,posIndex,ieProperties,gmEntry)
EgEvnt          *evntMsg;                  /* Holds the decoded IE's */
Buffer          *mBuf;                   /* Holds the data to decode */
EgIeLevelInfo   *ieLvlInfo;         /* The Level of the IE to decode */
U16             inIeLen;                    /* the Decoded IE Length */
U8             *posIndex;         /* IE's position idx in the msg db */
EgIeProperties *ieProperties;         /* IE properties in the msg db */
EgMsgEntry     *gmEntry;           /* Msg db in dictionary */
#endif
{
   S16        retVal = ROK;         /* Holds the return value  */
   U16        tmpIeLen = 0;         /* Tmp stores the IE Len   */
   U16        grpIeLen = inIeLen;   /* Contains the GRP IE Len */

   EG_TRC2(egDecGrpIe);

   EG_DBG_INFO(0,0, (egp,
         "egDecGrpIe(GrpIE Type(%d), GrpIeLen(%d)) \n",
         ieLvlInfo->ieInfo[ieLvlInfo->level-1].ieType, grpIeLen));

  /**********************************************************
   * Loop until all IE's in the grouped IE has been exposed *
   **********************************************************/
   while(grpIeLen)
   {
      /***************************************
       * check the return value of the decIe *
       ***************************************/
      if ( (retVal = egDecIe(evntMsg, mBuf, ieLvlInfo,
                            &tmpIeLen,posIndex,ieProperties,gmEntry)) != ROK)
      {
         /******************************************************
          * Decoding of chield IE failed, Log error and return *
          ******************************************************/
         EGLOGERROR(ERRCLS_DEBUG, EEG081, 0,
                     "egDecGrpIe: decoding child ie failed");
         EG_RETVALUE(retVal);
      } /* end of if decoding child ie fails */
      grpIeLen -= tmpIeLen + EG_LEN_FOUR;
   }

   EG_RETVALUE(retVal);
} /* end of egDecGrpIe() */




#ifdef EG_GROUPED_CHK
/**********************************************************************
 *   Fun:  egValidateGrpIe
 *   
 *   Desc:  To Validate the GTP Grouped IEs. 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egValidateGrpIe
(
 EgMsgEntry      *gmEntry,                   /* Message entry in db */
 EgGrouped      *grpIe,                     /* groupe Ie to validate */
 EgGroupedIe     *groupedIe                /* grouped IE pointer */
)
#else
PRIVATE S16 egValidateGrpIe(gmEntry,grpIe, groupedIe)
EgMsgEntry      *gmEntry;                   /* Message entry in db */
EgGrouped        *grpIe;                     /* groupe Ie to validate */
EgGroupedIe     *groupedIe;                /* grouped IE pointer */
#endif
{
   S16           retVal = ROK;              /* Holds the return value */
   CmLList       *tmpNode = NULLP;      /* Used to store the List PTR */
   EgIe          *tmpIe = NULLP;                     /* Stores the IE */
   U16           cnt = 0;                  /* Used for iter thru list */
   EgIeEntry         *ieEntry = NULLP;  /* Holds IE Entry for each IE */
   U8              *posIndex;                  /* holds the position index */
   EgIeProperties  *ieProperties;               /* holds the ie properties */
   EgIeProperties  *ieProp = NULLP;               /* holds the ie properties */
   U8 idx;

   EG_TRC2(egValidateGrpIe);

   /*************************************
    * Check for validity for Grouped IE *
    ************************************/
    if(!grpIe || !groupedIe) 
    {
       retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
       EG_RETVALUE(retVal);
    }

#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
         "egValidateGrpIe(IE Count(%ld) \n", grpIe->gIeList.count));
#else
   EG_DBG_INFO(0,0, (egp,
         "egValidateGrpIe(IE Count(%d) \n", grpIe->gIeList.count));
#endif
   posIndex = (U8 *)&groupedIe->posIndex;
   ieProperties = groupedIe->ieProperties;
   if(!posIndex || !ieProperties) 
   {
     retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
     EG_RETVALUE(retVal);
   }
   /********************************************
    * Loop to validate each IE in a grouped IE *
    ********************************************/
    /********************************************
     * Note: Right now we are not verifying the *
     * duplicate IEs in a grouped IE, please do *
     * so,u can go through the IEs instead of   *
     * maintaining msgIeIdx kind of array       *
     * use visitedIe[] array and find repeated  *
     * and undesired IEs                        *
     * *****************************************/
   CM_LLIST_FIRST_NODE(&grpIe->gIeList, tmpNode);

   for(cnt = 0; ((cnt < grpIe->gIeList.count) && (tmpNode)); cnt++)
   {
     tmpIe = (EgIe*)tmpNode->node;
     ieEntry = egGetIeEntryFrmDb(tmpIe->ieType);
     if ((ieEntry) && (ieEntry->dataType != tmpIe->dataType))
     {
       EG_RETVALUE(EGT_ERR_INV_IE_TYPE);
     }

     /****************************************************** 
      * Check whether the IE matches with our local DB     *
      ******************************************************/
     idx = *(posIndex+(EG_MAX_IE * tmpIe->ieInst)+tmpIe->ieType); 
     if(idx != 0 && (ieProp = &ieProperties[idx-1]) && 
         (tmpIe->ieType == ieProp->ieType) &&
         (tmpIe->ieInst == ieProp->ieInst))
     {
       /******************************************
        * check whether this IE is of grouped IE *
        ******************************************/
       if(tmpIe->dataType == EG_GROUPED)
       {
         /***********************************************
          * If Chield Entry is valid with valid group IE *
          * then validate the chiled IE.                *
          ***********************************************/
         if(ieProp->groupedIe)
         {
           retVal = egValidateGrpIe(gmEntry,&tmpIe->t.grouped,ieProp->groupedIe);
         } 
         else
           /****************************************************
            * The Chield IE is invalid so log error and return *
            ****************************************************/
         {
           retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
           EG_DBG_INFO(0,0, (egp,
                 "egValidateGrpIe(Grouped IE(%d) not found in Dict,\
             resultType(%d)) \n",
                 tmpIe->ieType, retVal));

           EG_RETVALUE(retVal);
         }
         if(retVal != ROK)
         {
           EG_RETVALUE(retVal);
         }
       } 
       else
       {
         /* Pradeep: if the IE is CAUSE, lets validate it */
         if (tmpIe->ieType == EG_IE_CAUSE && gmEntry->validateIe != NULLP) 
         {
           retVal = gmEntry->validateIe(tmpIe,tmpIe->ieType); 
           if( retVal == EGT_CAUSE_INVALID)
           {
             EG_DBG_INFO(0,0, (egp,"egValidateGrpIe(IE(%d), \
               is invalid ) \n",tmpIe->ieType));
             EG_RETVALUE(retVal);
           }
         }
       }
     } /* If the IE is matching */
     else
     {
       /* eg012.201 : Added NULL check */
       if((ieProp) && (ieProp->ieClass == EG_IE_MANDATORY))
       {
         /************************************************
          * Send missing mandatory IE to the application *
          ************************************************/
         retVal = EGT_ERR_MSG_MISS_MAND_IE;

         EG_DBG_INFO(0,0, (egp,
               "egValidateGrpIEs(Mandatory IE Type(%d), IE Instance(%d), \
           mandatory element is missing resulType(%d),))\n", 
               ieProp->ieType, ieProp->ieInst,retVal));

         EG_RETVALUE(retVal);
       }
       /* Remove this IE from list and continue processing */
       else
       {
         retVal = ROK;
         EG_DBG_INFO(0,0, (egp,
               "egValidateGrpIe(Conditional/Optional IE(%d), \
           resultType(%d) not configured in Dict) \n",
               tmpIe->ieType,  retVal));

         egUtilDelIe(&grpIe->gIeList, tmpIe, FALSE);
       }
     }

     CM_LLIST_NEXT_NODE(&grpIe->gIeList, tmpNode);

   } /* end of iterating thru the Ie's in the rcd message */

      EG_RETVALUE(retVal);
} /* end of egValidateGrpIe() */

#endif /* EG_GROUPED_CHK */


/**********************************************************************
 *   Fun:  egValidateGmMsgIEs
 *   
 *   Desc:  To Validate the Msg IEs. 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egValidateGmMsgIEs
(
EgEvnt     *evntMsg,                 /* Contains EGMSG for validation */
EgMsgEntry      *gmEntry             /* Holds the message Entry */
)
#else
PRIVATE S16 egValidateGmMsgIEs(evntMsg,gmEntry)
EgEvnt     *evntMsg;             /* Contains the EGMSG for validation */
EgMsgEntry      *gmEntry;       /* Holds the message Entry */
#endif
{
   S16              retVal = ROK;           /* Holds the return value */
   EgIeProperties  *ieProp;                      /* Holds the Ie Prop */
   CmLList         *tmpNode;                    /* Holds the Node Ptr */
   EgIe            *ie = NULLP;                       /* Holds the IE */
   U16              cnt;                   /* Used for iter thru list */
   S16              loopCnt;                    /* Used for the loops */
   EgIeLevelInfo    ieLvlInfo;               /* Holds the Ie Lvl Info */
   U8               ieInst = 0;        /* Gets the Idx from the Array */
   U8               ieType = 0;        /* Gets the Idx from the Array */
   U8               ieIdx = 0;         /* Gets the Idx from the Array */
   EgIeProperties   *tmpIeProp = NULLP;

   EG_TRC2(egValidateGmMsgIEs)

   /*********************************
    * initialize the local variable *
    ********************************/
   ieProp  = NULLP;
   tmpNode = NULLP;
   cnt     =     0;
   loopCnt =     0;
   cmMemset( (U8* )&ieLvlInfo, 0, sizeof(EgIeLevelInfo));

   EG_DBG_INFO(0,0, (egp,
            "egValidateGmMsgIEs(Msg Type(%d) \n",    
             evntMsg->u.egMsg->msgHdr.msgType));

/*****************************************************************
 * If there are no mandatory IEs, that means we can get a message *
 * with no IEs. Anyway we are going to validate the message for   *
 * MandatoryIEs, removal of this check will allow the above case  *
   *****************************************************************/
   /*******************************************************
    * validate the Gm Msg properties with the Data Base   *
    * to check whether any Mandatory IE is missing or not *
    *******************************************************/
   for(loopCnt = 0; loopCnt <  gmEntry->nmbIeProp; loopCnt++)
   {
      ieProp = &(gmEntry->ieProperties[loopCnt]);
      if (ieProp->ieClass == EG_IE_MANDATORY)
      {
        /*****************************************
         * search in the stored array to get the *
         * IE Index  for the received message    *
         *****************************************/
        ie = msgIeIdx[ieProp->ieInst][ieProp->ieType];
        /***************************************************
         * Case : IE not found in GM Dict as entry is NULL *
         ***************************************************/
        if(ie == NULLP)
        {
          /*************************************************
           * If control comes here means, the Mandatory IE *
           * is missing.Check if the IE missing is CauseIE *
           *************************************************/
            /************************************************
             * Send missing mandatory IE to the application *
             ************************************************/
            /* ccpu00137585 : If there are more than one Mandatory IE, send Inv Msg Format */
            retVal = EGT_ERR_MSG_MISS_MAND_IE;
            while (++loopCnt < gmEntry->nmbIeProp)
            {
               tmpIeProp = &(gmEntry->ieProperties[loopCnt]); 
               if ((tmpIeProp->ieClass == EG_IE_MANDATORY) && 
                   (msgIeIdx[tmpIeProp->ieInst][tmpIeProp->ieType] == NULLP))
               {
                  /* Found 2nd Mandatory IE */
                  retVal = EGT_ERR_INV_MSG_FMT;
                  break;
               }
            }
            EG_DBG_INFO(0,0, (egp,
                  "egValidateGmMsgIEs(IE Type(%d), IE Instance(%d), \
              Msg Type (%d), mandatory element is missing, resulType(%d)) \n",
                  ieProp->ieType, ieProp->ieInst, evntMsg->u.egMsg->msgHdr.msgType, retVal));
            EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 
                ieProp->ieType, ieProp->ieInst,  
                evntMsg->u.egMsg->msgHdr.msgType);
            EG_RETVALUE(retVal);
        } 
         /* ccpu00137045,ccpu00137844 : break if cause is for reject */
        else if ((ieProp->ieType == EG_IE_CAUSE) && (ie->t.cause.causeVal >= 64))
        {
           break;
        }
   }
      /* ccpu00137045 : break if cause is for reject */
      if (ie == NULLP)
      {
         ie = msgIeIdx[ieProp->ieInst][ieProp->ieType];
         if ((ie != NULLP) && (ieProp->ieType == EG_IE_CAUSE) && (ie->t.cause.causeVal >= 64))
         {
            break;
         }
      }
   }
   if (retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
                  "egValidateGmMsgIEs(IE Type(%d), IE Instance(%d), \
              Msg Type (%d), IE value is invalid, resulType(%d)) \n",
                  ieProp->ieType, ieProp->ieInst, evntMsg->u.egMsg->msgHdr.msgType, retVal)); 
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal,
                ieProp->ieType, ieProp->ieInst,
                evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }
  /********************************************************************
   * Check whether they are any unknown IE's present in received msg  *
   * If there are any unknown IE's with CR is set will be rejected    *
   * and  an IE with unexpected Instance the shall be skipped in msg  *
   ********************************************************************/
   CM_LLIST_FIRST_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);

   /*********************************************************
    * Loop through the list of IE's in the message received *
    *********************************************************/
   for(cnt = 0; ((cnt < evntMsg->u.egMsg->egIeList.count) && (tmpNode));
       cnt++)
   {                   
      ieProp = NULLP;
      ie = (EgIe*)tmpNode->node;
      ieType = ie->ieType;
      ieInst = ie->ieInst;

      /******************************************************************
       * Check whether this IE is present in the message DB, if it is   * 
       * not present in the DB, then check whether instance is greater  *
       * than 0, means the IE with type known but unexpected instance.  *
       * In this case delete the IE from the event msg. If the instance *
       * is 0 means unknown IE type.                                    *
       ******************************************************************/
       ieIdx = gmEntry->posIndex[ieInst][ieType];
       if ( ieIdx != 0)
       {
          ieProp = &gmEntry->ieProperties[ieIdx-1];
       }
       /*******************************
        * Check if the entry is Valid *
        *******************************/
       if((ieIdx != 0) ||  (ieProp != NULLP))
       {
#ifdef EG_GROUPED_CHK
         /********************************
          * Process if the IE is grouped *
          ********************************/
          if(ie->dataType == EG_GROUPED)
          {     
              EG_FILL_IE_LVL_INFO(&ieLvlInfo, ieLvlInfo.level,
                                   ieType, ieInst);
              ieLvlInfo.level++;

              retVal = egValidateGrpIe(gmEntry,&ie->t.grouped,ieProp->groupedIe);
              if (retVal != ROK)
              {
                 EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, ieType, ieInst, 
                                      evntMsg->u.egMsg->msgHdr.msgType);
                 EG_RETVALUE(retVal);
              }
          }
#endif /* EG_GROUPED_CHK */
         /* Pradeep: if its CAUSE lets validate it */
          if (ieProp->ieType == EG_IE_CAUSE && gmEntry->validateIe != NULLP) 
          {
            retVal = gmEntry->validateIe(ie,ieProp->ieType); 
            if( retVal == EGT_CAUSE_INVALID)
            {
              EG_DBG_INFO(0,0, (egp,
                    "egValidateGmMsgIEs(IE(%d), \
                is invalid ) \n",ie->ieType));

              EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, ieType, ieInst,
                  evntMsg->u.egMsg->msgHdr.msgType);
              EG_RETVALUE(retVal);
            }
            /* ccpu00137844 : If response in reject, no need to validate other mandatory IEs */
            if (ie->t.cause.causeVal >= 64)
            {
               break;
            }
         }
         if (ie->ieInValidCause != 0)
         {
            retVal = ie->ieInValidCause;
            EG_DBG_INFO(0,0, (egp,
                  "egValidateGmMsgIEs(IE Type(%d), IE Instance(%d), \
                  Msg Type (%d), Invalid IE Value, resulType(%d)) \n",
                  ieProp->ieType, ieProp->ieInst, evntMsg->u.egMsg->msgHdr.msgType, retVal));
            EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, ieType, ieInst,
                  evntMsg->u.egMsg->msgHdr.msgType);
            EG_RETVALUE(retVal);
          }
      } 
      else 
      /***********************************
       * The IE is not present in the DB *
       ***********************************/
      {
          ieLvlInfo.level = 0;
         /********************************************************
          * Discard the IE as it is unexpected for this message. *
          * Delete the IE from the framed event message          *
          ********************************************************/
          EG_FILL_IE_LVL_INFO(&ieLvlInfo, ieLvlInfo.level, ieType, ieInst);

          EgUtilGmDelIe(evntMsg->u.egMsg, &ieLvlInfo, FALSE);
      }
      CM_LLIST_NEXT_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
   }

   EG_RETVALUE(retVal);
} /* End of egValidateGmMsgIEs() */



/*************************************************************************
 *   Fun:  egValidateIeLen
 *
 *   Desc:  validate the Ie len is correct or not
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ************************************************************************/
#ifdef ANSI
PRIVATE S16 egValidateIeLen
(
EgIe            *egIe,   /* IE to validate for the len     */
EgIeEntry    *ieEntry,   /* Holds the min and max length   */
U8           *posIndex,    /* IE Postion Index in msg db */
EgIeProperties *ieProperties, /* Holds IE Properties */
U16           ieLen      /* actual IE length to validate   */
)
#else
PRIVATE S16 egValidateIeLen(egIe, ieEntry, posIndex,ieProperties, ieLen)
EgIe            *egIe;   /* IE to validate for the len     */
EgIeEntry    *ieEntry;   /* Holds the min and max length   */
U8           *posIndex;    /* IE Postion Index in msg db */
EgIeProperties *ieProperties; /* Holds IE Properties */
U16           ieLen;     /* actual IE length to validate   */
#endif
{
  EgIeProperties    *ieProp;    /* IE Prop to validate                    */
   U8                idx;    /* Index to get the pos of IE in the list */
   S16               retVal;
   S16               ieInst;

   retVal = ROK;

  EG_TRC2(egValidateIeLen);

   /********************************************************
     * Get the ieProp and then check whether this ie is      *
     * mandatory or not                                      *
     ********************************************************/
   if (egIe->ieInst > EG_MAX_INST)
   {
      /* Wrong Instance Value of the IE */
      EG_RETVALUE(EGT_ERR_MSG_INV_IE_INST);
   }
     idx = *(posIndex+(EG_MAX_IE * egIe->ieInst)+egIe->ieType);
   ieProp = &ieProperties[idx-1];
 
   /* ccpu00137579 : IE Instance Validation */
   if (ieProp->ieType == 0)
   {
      /* Wrong Instance Value of the IE */
      retVal = EG_SKIP_OPT_IE;
      /* Get the IE Type */
      ieInst = 0;
      while (ieInst < EG_MAX_INST)
    {
         idx = *(posIndex+(EG_MAX_IE * ieInst)+egIe->ieType);
      ieProp = &ieProperties[idx-1];
         if (ieProp->ieType != 0)
      {
            if (ieProp->ieClass != EG_IE_MANDATORY)
            {
               retVal = EG_SKIP_OPT_IE;
            }
            else
            {
               retVal = EGT_ERR_MSG_MISS_MAND_IE;
            }
            break;
         }
         ieInst++;
      }
   }
   else if (ieLen < ieEntry->minLen)
   {
      /* fix for ccpu00137063 */
      if((ieProp->ieClass == EG_IE_MANDATORY) || (ieEntry->dataType == EG_BCD))
      {
         /*******************************
         * Invalid length, report error *
         *******************************/
         retVal = EGT_ERR_IE_INV_LEN;
      }
      else
      {
        /************************************************
         * skip this IE and continue further processing *
         ***********************************************/
         retVal = EG_SKIP_OPT_IE;
      } /* if it is optional or conditional */
  } /* End of if the ie Length is invalid */
   EG_RETVALUE(retVal);
}



/*********************************************************************/
/******************  DB INITIALISATION  ******************************/
/*********************************************************************/


/*
*
*   Func  : egGetIntfMem
*
*   Desc  : This function returns the memory required for intfCbLst
*
*    Return: memSize - size of the intfCbLst
*
*  File  : eg_edm.c
*
*/

#ifdef ANSI
PUBLIC Size  egGetIntfMem
(
Void
)   
#else
PUBLIC Size egGetIntfMem(Void)
#endif
{
   Size memSize=0;      /* To store the total memory reu for the IntfCbLst */
   
   /*******************************************************
   *  Calculate the memory required for the INTF CB       *
   *******************************************************/
   memSize = (sizeof(EgIntfCb) + sizeof(EgIntfCb* ) + 
             SBUFSIZE(EG_INTF_GMDICT_BIN_SIZE));

   RETVALUE(memSize);
}

 
/*
*
*   Func  : egAllocIntfLst
*
*   Desc  : This function allocates memory for the IntfCb Lst
*
*    Return: ROK - If the allocation is success
*            RFAILED - If the allocation is failed.
*
*  File  : eg_edm.c
*
*/

#ifdef ANSI
PUBLIC S16  egAllocIntfLst
(
U8   nmbIntf          /* Contains the number of interfaces         */
)   
#else
PUBLIC S16 egAllocIntfLst(nmbIntf)
U8   nmbIntf;         /* Contains the number of interfaces         */
#endif
{
   U8  idx = 0;     /* To iterate thru the list of interfaces     */
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egAllocIntfLst)

   /*******************************************************
   *  Allocate memory for the intfCb Lst for the nmbIntf  *
   *******************************************************/
   if ( (edmCb.intfCbLst = (EgIntfCb** ) egAlloc (
                           (Size) (nmbIntf * sizeof(PTR)))) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   for( idx=0; idx < nmbIntf; idx++)
   {

      edmCb.intfCbLst[idx] = (EgIntfCb* ) NULLP;
   }

   RETVALUE(ROK);
   
}


/*
*
*   Func  : egEdmDeInit
*
*   Desc  This function drops the complete GM Dictionary for 
*         all the interfaces.
*
*    Return: ROK - GM Dictionary is deleted successfully
*           RFAILED - Failed to delete the complete GM Dictionary
*
*  File  : eg_edm.c
*
*/

#ifdef ANSI
PUBLIC S16  egEdmDeInit
(
Void
)   
#else
PUBLIC S16 egEdmDeInit()
#endif
{
   S16  ret    = ROK;    /* Variable to store the return value    */
   U8   lstIdx = 0;      /* Idx to iterate thru the list of intf  */

   EG_TRC2(egEdmDeInit)

   /*******************************************************
   *        Iterate through the list of the intfLst       *
   *        And then Drop the msg DB for that intface     *
   *******************************************************/
   for ( lstIdx =0; lstIdx < LEG_MAX_INTFTYPE; lstIdx++)
   {
      if( PRSNT_NODEF == edmCb.egIdxToIntfMap[lstIdx].pres)
      {
         if( (ret = egDbDropGmDict(edmCb.egIdxToIntfMap[lstIdx].val)) != ROK)
         {
            EG_RETVALUE(ret);
         }
         edmCb.egIdxToIntfMap[lstIdx].pres = NOTPRSNT;
      }
      else
      { 
         /********************************************************
         *    As the Interfaces are stored from the index 0      *
         *    till the pres is PRSNT_NODEF the indexes are valid *
         ********************************************************/
         break;
      }
   }
   /******************************************
    * Free the interface list control blocks *
    ******************************************/
   for( lstIdx=0; lstIdx < egCb.genCfg.nmbIntfs; lstIdx++)
   {
     if(edmCb.intfCbLst[lstIdx] == NULLP)
        continue;
     else
     {
        /*****************************************************************
         * eg007.201: Free Hashlist,gmDictHlCp, associated with intfCbLst*
         *****************************************************************/
        /*****************************************************************
         * eg008.201: Data Type of nextGmCb modified from PTR to EgGmCb  *
         *****************************************************************/
        EgGmCb *nextGmCb = NULLP;               /* pointer to next GM entry */
        for (;((ret = cmHashListGetNext(&(edmCb.intfCbLst[lstIdx]->gmDictHlCp), (PTR)NULLP, (PTR*)nextGmCb)) == ROK);)
        {
           cmHashListDelete(&(edmCb.intfCbLst[lstIdx]->gmDictHlCp), (PTR)nextGmCb);
           EG_FREE(nextGmCb, sizeof(EgGmCb));
           nextGmCb = NULLP;
        }
        cmHashListDeinit(&(edmCb.intfCbLst[lstIdx]->gmDictHlCp));

        ret = egFree((Data *)edmCb.intfCbLst[lstIdx], sizeof(EgIntfCb));
        edmCb.intfCbLst[lstIdx] = NULLP;
     }
   }
   /*****************
    * Free the list *
    *****************/
   ret = egFree((Data *)edmCb.intfCbLst, (Size) (egCb.genCfg.nmbIntfs * sizeof(PTR)));
   
   EG_RETVALUE(ROK);
}


/*
*
*   Func  : egDbDropGmDict
*
*   Desc  : This function drops the complete GM Dictionary specified
*            by the Interface Type
*
*    Return: ROK - GM Dictionary is deleted successfully
*           RFAILED - Failed to delete the complete GM Dictionary
*
*  File  : eg_edm.c
*
*/

#ifdef ANSI
PUBLIC S16   egDbDropGmDict
(
U8     intfType         /* Interface type to the CP */
)   
#else
PUBLIC S16 egDbDropGmDict(intfType)
U8     intfType;         /* Interface type to the CP */
#endif
{
   S16  ret = ROK;                           /* Variable to store the return value */
   U8 nmbIe    =  0;                         /* To iterate thru the Ie's*/
   U8 nmbIe1    =  0;                         /* To iterate thru the Ie's*/
   EgIeEntry  *ieEntry=NULLP;                /* To get the ieEntry      */
   PTR  prevGmCb = NULLP;                      /* Pointer to previous control block */
   PTR  nextGmCb = NULLP;                      /* Pointer to next control block */
   CmHashListCp *hashListCp = NULLP;           /* Variable to store the HL control pointer
                                        of the GM dictionary which has to be dropped */
   Region region = 0;                      /* Variable to store the Region from which the 
                                       memory has to be freed */
   S16    lstIdx=0;                 /* Used to iterate thru the list of intf array*/
   EgIntfCb  *intfCb=NULLP;         /* Used to get the control ptr */
   Pool pool = 0;                          /* Variable to store the Pool from which the 
                                       memory has to be freed */   

   EG_TRC2(egDbDropGmDict)

   EG_DBG_INFO(0, 0, (egp,
          "egDbDropGmDict(Deleting GM Dictionary of intfType (%d))\n",intfType)); 

   /**************************************************
   * Retrieve the Hash list control pointer and      *
   *  memory region and pool associate with intfType *
   * Drop the application specific GM Dictionary     *
   * Retrieve the Idx from intf Type                 *
   **************************************************/

   for ( lstIdx =0; lstIdx < LEG_MAX_INTFTYPE; lstIdx++)
   {
     if( PRSNT_NODEF == edmCb.egIdxToIntfMap[lstIdx].pres)
     {
        if( intfType == edmCb.egIdxToIntfMap[lstIdx].val)
        {
           intfCb =  edmCb.intfCbLst[lstIdx];
           break;
        }
     }
   }
   if( intfCb == NULLP)
   {
      EGLOGERROR(ERRCLS_DEBUG, EEG083, 0,
                 "egDbDropGmDict() Failed Intf is Invalid");
      EG_RETVALUE(EGT_ERR_INV_INTF_TYPE);
   }
   hashListCp = &intfCb->gmDictHlCp;
   region = egCb.init.region;
   pool = egCb.init.pool;   
   prevGmCb = (PTR)NULLP;
   ret = cmHashListGetNext(hashListCp, prevGmCb, &nextGmCb);
   for(; ;)
   {
      if(ROK != ret)
      {
         /***************************************
         * Iterated through complete Hash List  *
         ***************************************/
         break;
      }
      else
      {
         prevGmCb = nextGmCb;
         /***************************************
         * Check whether there is an IE of GRP  *
         * Then delete all the child IE's       *
         * Since there is no 2nd level grouped  *
         * IE, there is no function called      *
         * again to check for the grped type.   *
         ****************************************/
         for (nmbIe=0; nmbIe < ((EgGmCb *)prevGmCb)->gmEntry.nmbIeProp;nmbIe++)
         {

            ieEntry = egGetIeEntryFrmDb
                (((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].ieType );
            if ( ieEntry && ieEntry->dataType == EG_GROUPED && 
                 ((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].groupedIe )
            {
               /*************************************************************
               *  Free all the child IE's present in the grouped structure  *
               *  Then free the grouped structure allocated during cfg time *
               *************************************************************/
              /*****************************************
               * If there is a second level grouped IE *
               ** ***************************************/
              for(nmbIe1=0;nmbIe1< ((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].groupedIe->nmbGrpIe;nmbIe1++)
              {
                if( ((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].groupedIe->ieProperties[nmbIe1].groupedIe != NULLP)
                {
                  SPutSBuf(region, pool,
                      (Data *)((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].groupedIe->ieProperties[nmbIe1].groupedIe->ieProperties,
                      (sizeof(LegIeProperties) *
                     ((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].groupedIe->ieProperties[nmbIe1].groupedIe->nmbGrpIe));

                  SPutSBuf(region, pool,
                      (Data *) ((EgGmCb *)prevGmCb)->gmEntry.ieProperties[nmbIe].groupedIe->ieProperties[nmbIe1].groupedIe,
                      (sizeof(LegGroupedIe)));

                }
              }


               SPutSBuf(region, pool,
                       (Data *)((EgGmCb *)prevGmCb)->gmEntry.
                        ieProperties[nmbIe].groupedIe->ieProperties,
                        (sizeof(EgIeProperties) *
                        ((EgGmCb *)prevGmCb)->gmEntry.
                        ieProperties[nmbIe].groupedIe->nmbGrpIe));

               ((EgGmCb *)prevGmCb)->gmEntry.
                        ieProperties[nmbIe].groupedIe->ieProperties= NULLP;
 
                             
               SPutSBuf(region, pool,
                       (Data *)((EgGmCb *)prevGmCb)->gmEntry.
                        ieProperties[nmbIe].groupedIe,
                        sizeof(EgGroupedIe));

               ((EgGmCb *)prevGmCb)->gmEntry.
                        ieProperties[nmbIe].groupedIe = NULLP;

            }
         }
         /***************************************
         * Delete the Gm entry retrieved        *
         ***************************************/
         if( ROK == cmHashListDelete(hashListCp, (PTR)prevGmCb))
         {
            /***************************************
            * DeAllocate the GM Entry              *
            ***************************************/
            SPutSBuf(region,pool,(Data *)prevGmCb, sizeof(EgGmCb));
            prevGmCb = NULLP;
         }
         else
         {
            /***************************************
            * Failed to delete the GM Entry        *
            ***************************************/
            EGLOGERROR(ERRCLS_DEBUG, EEG084, 0,
                       "egDbDropGmDict() failed,Failed to delete GM \
                        control block from GM Dictionary");
            EG_RETVALUE(LEG_REASON_UNKNOWN);
         }
         /***************************************
         *         Move to next GmCb            *
         ***************************************/
         ret = cmHashListGetNext(hashListCp, prevGmCb, &nextGmCb);
      }
   } 

   ret = cmHashListDeinit(hashListCp);
   
   EG_RETVALUE(ROK);

} 



/*
*
*   Func  : egDbCfgIeDict
*
*   Desc  : This function inserts all the IE's in the appropriate Ie dict
*
*    Return: ROK : Inserts the entries successfully
*           EGT_ERR_MEM_ALLOC_FAILED: Fail to allocate memory
*
*  File  : eg_cdb.c
*
*/

#ifdef ANSI
PUBLIC Void egDbCfgIeDict
(
U8            nmbIe,             /* Number of Ie entries    */
U16           *posIdx, /* position of each IE     */
LegIeEntry   *ieEntry            /* IE Entry to be inserted */
)
#else
PUBLIC Void egDbCfgIeDict(nmbIe, posIdx, ieEntry)
U8            nmbIe;             /* Number of Ie entries    */
U16           *posIdx; /* position of each IE     */
LegIeEntry   *ieEntry;           /* IE Entry to be inserted */
#endif
{

   S16  ieCnt=0;                 /* To Iterate thru the ie Lst */

   EG_TRC2(egDbCfgIeDict)
   
   /*************************************************
   *           INITIALISATION OF THE IE DICT        *
   **************************************************/
   for( ieCnt =0;  ieCnt < EG_MAX_IES_ALL_MSGS; ieCnt++)
   {
      cmMemset((U8 *)&edmCb.egIeCb.ieEntry[ieCnt], 0, sizeof(EgIeEntry));
   }
   cmMemset((U8 *)edmCb.egIeCb.posIdx, 0, EG_MAX_IE);


   /*************************************************
   *  Copy the position index array, which contains *
   *  the position of the IE in the EgIeEntry.      *
   *************************************************/
   for( ieCnt =0;  ieCnt < EG_MAX_IE; ieCnt++)
   {
      edmCb.egIeCb.posIdx[ieCnt] = posIdx[ieCnt];
   }
   for( ieCnt=0; ieCnt < nmbIe; ieCnt++)
   { 
      /************************************************
       * Copy the ieEntry idexed by the iecnt         *
       ***********************************************/
       cmMemcpy((U8*)&edmCb.egIeCb.ieEntry[ieCnt],(U8*)&ieEntry[ieCnt], sizeof(EgIeEntry));  
   }
   /*-- eg003.201 : changes from eg006.102: Added RETVOID--*/
   RETVOID;
} 



/*
*
*   Func  : egMakeGrpIeEntry
*
*   Desc  : This function creates the IE Entries with in the group IE  
*
*    Return: None
*
*  File  : Eg_cdb.c
*
*/

#ifdef ANSI
PRIVATE S16 egMakeGrpIeEntry
(
Region        region,     /* Region from where the memory has to be allocated */
Pool          pool,      /* Pool from where the memory has to be allocated */
EgGroupedIe  *groupedIe,         /* IE Control block */
LegIeProperties   *ieEntry            /* IE Entry */
)
#else
PRIVATE S16 egMakeGrpIeEntry(region ,pool,groupedIe,ieEntry)
Region        region;     /* Region from where the memory has to be allocated */
Pool          pool;      /* Pool from where the memory has to be allocated */
EgGroupedIe  *groupedIe;      /* IE Control Block  */
LegIeProperties   *ieEntry;         /* IE Entry */
#endif
{
   U8 ieCnt;                  /* Variable to store the ie count */
   S16 ret;                     /* Variable to store the return value */
   U8 grpIeCnt;               /* Variable to store the grp ie count */
   U8 dataType;             /* Stores the data type of the IE from the entry */
   U8   ieType;
   U8   ieInst;
   EgIeEntry *tmpIeEntry = NULLP; /* stores the IE Entry */
   
   EG_TRC2(egMakeGrpIeEntry)

   /* Variable initialization */
   ieCnt = 0;
   ret = ROK;
   grpIeCnt = 0;
   grpIeCnt = groupedIe->nmbGrpIe;

   /* Allocate the memory for array of EgIeEntry with in EgGroupedIe */
   /* Check if the region is same as EdmCb.region */
   /*eg009.201:Klockwork fix,check groupedIe ptr for NULL*/
   /*eg012.201: klocwork fix, removed groupedIe pt for NULL */
   if(grpIeCnt)
   {
      ret = SGetSBuf(region, pool, (Data **)&groupedIe->ieProperties,
            (grpIeCnt*(sizeof(EgIeProperties))));
      /* eg012.201: klockwork fix:removed memset from here, added if ROK */

      if((ROK == ret) && (NULLP != groupedIe->ieProperties))
      {
         /* eg012.201: klockwork fix:Added memset for ieProperties here */
         cmMemset((U8 *)groupedIe->ieProperties, 0 , (grpIeCnt*(sizeof(EgIeProperties))));
         for(ieCnt = 0; ieCnt < grpIeCnt; ieCnt ++)
         {
            /* Populate the IE  control block */
            groupedIe->ieProperties[ieCnt].ieType = ieType = ieEntry->groupedIe->ieProperties[ieCnt].ieType;
            groupedIe->ieProperties[ieCnt].ieInst = ieInst = ieEntry->groupedIe->ieProperties[ieCnt].ieInst;
            groupedIe->ieProperties[ieCnt].ieClass = ieEntry->groupedIe->ieProperties[ieCnt].ieClass;
            groupedIe->ieProperties[ieCnt].ieList = ieEntry->groupedIe->ieProperties[ieCnt].ieList;
            groupedIe->posIndex[ieInst][ieType]  = ieEntry->groupedIe->posIndex[ieInst][ieType];

            /***************************************************
             *   Get the IE Entry from the Db and then check if *
             *   the Entry is valid or not. If valid then cont  *
             *   else return Error from here                    *
             ***************************************************/
            if( (tmpIeEntry = egGetIeEntryFrmDb(ieType)) != NULLP)
            {
               dataType = tmpIeEntry->dataType;
            }
            else
            {
               EGLOGERROR(ERRCLS_ADD_RES, EEG085, 0,
                     "IE Entry NOT found, frm the ie");
               EG_RETVALUE(RFAILED);
            }
#ifdef EG_PHASE2
            if(dataType == EG_GROUPED)
            {
               /***********************************
                * allocate memory for EgGroupedIe *
                ***********************************/
               groupedIe->ieProperties[ieCnt].groupedIe = (EgGroupedIe *)egAlloc(sizeof(EgGroupedIe));
               if(NULLP != groupedIe->ieProperties[ieCnt].groupedIe)
               {
                  /* Populate the groupIe structure */
                  groupedIe->ieProperties[ieCnt].groupedIe->nmbGrpIe = ieEntry->groupedIe->ieProperties[ieCnt].groupedIe->nmbGrpIe;
                  ret = egMakeGrpIeEntry(region,pool,groupedIe->ieProperties[ieCnt].groupedIe,&ieEntry->groupedIe->ieProperties[ieCnt]);
                  /**********************************
                   *  If it fails the return error   *
                   **********************************/
                  if (ret != ROK)
                  {
                     EG_RETVALUE(RFAILED);
                  }
               }
               else
               {
                  /* Memory allocation fails */
                  EGLOGERROR(ERRCLS_DEBUG, EEG086, 0, "egMakeGrpIeEntry failed,Memory \
                        allocation failed");
                  EG_RETVALUE(RFAILED);
               }
            }
            else
            {
               groupedIe->ieProperties[ieCnt].groupedIe = NULLP;
            }
#endif /* EG_PHASE2 */
         }/* End of for loop */
      }
   }
   EG_RETVALUE(ROK);
}


/*
*
*   Func  : egDbCfgGmDict
*
*   Desc  : This function inserts the application specific GM Messages into 
*          the appropriate GM Dictionary
*
*    Return: ROK : Inserts the Diameter commands successfully
*           EGT_ERR_MEM_ALLOC_FAILED: Fail to allocate memory
*
*  File  : eg_cdb.c
*
*/

#ifdef ANSI
PUBLIC S16 egDbCfgGmDict
(
U8          intfType,            /* Interface Type */
U16          nmbMsg,            /* Number of Messages to be inserted */
LegMsgEntry   *gmEntry            /* GM Entry to be inserted */
)
#else
PUBLIC S16 egDbCfgGmDict(intfType, nmbMsg, gmEntry)
U8          intfType;            /* Interface Type */
U16          nmbMsg;            /* Number of Messages to be inserted */
LegMsgEntry   *gmEntry;           /* GM Entry to be inserted */
#endif
{
   S16 ret;                     /* Variable to store the return value */
   U8  msgCnt;                  /* Variable to store the count of GM command */
   U8  ieCnt;                  /* Variable to store the count of IE */
   U8  instCnt;                  /* Variable to store the count of IE inst*/
   U8 dataType;             /* Stores the data type of the IE from the entry */
   EgGmCb   *gmMsgCb;              /* GM Control block */   
   EgIntfCb *intfCb;        /* Holds the Msg CB based on the intf key */
   Region     region;               /* Variable to store the region of the mem */
   Pool        pool;            /* Variable to store the pool of the mem */
   CmHashListCp *hashListCp;            
   Mem mem;
   EgUstaDgn dgn;               /* Variable to store the diagnostic values of alarm */
   PTR prevGmCb;               /* Variable to store the pointer to prev GM Cb */
   PTR nextGmCb;               /* Variable to store the pointer to next GM entry */
   U8     hashKey;             /* To store the hash Key */
   S16     lstIdx;          /* Used to store the intf in the index array */
   EgIeEntry *tmpIeEntry = NULLP; /* stores the IE Entry */
   
   EG_TRC2(egDbCfgGmDict)
   
   /* Variable initialization */
   ret = ROK;
   msgCnt = 0;
   ieCnt = 0;
   dataType = 0;
   instCnt = 0;
   intfCb = NULLP;
   gmMsgCb = NULLP;
   prevGmCb = NULLP;
   nextGmCb = NULLP;
   region = 0;
   pool = 0;
   hashListCp = NULLP;
   hashKey = 0;
   cmMemset((U8 *)&mem, 0 ,sizeof(Mem));
   cmMemset((U8 *)&dgn, 0 ,sizeof(EgUstaDgn));

   EG_DBG_INFO(0, 0, (egp,
           "egDbCfgGmDict(Configuring Gm Dictionary for intfType = (%d))\n", 
           intfType));

   /************************************************
   * Check if the number of commands is more then  *
   * the maximum allowed value then return failure *
   ************************************************/
   if(LEG_MAX_NUM_OF_MSGS < nmbMsg || nmbMsg == 0)
   {
      EG_DBG_INFO(0, 0, (egp,
           "egDbCfgGmDict(Number of GM to be configured = (%d))\n", 
           nmbMsg));
      EGLOGERROR(ERRCLS_DEBUG, EEG088, 0,  "egDbCfgGmDict() failed,Number of GM \
                 to be configured is invalid");
      EG_RETVALUE(LEG_REASON_INV_NMB_MSGS);
   }

   /************************************************
   * All the configured interfaces will be stored  *
   * in the array which is to index the CP         *
   * If the Idx is not filled, then the intf type  *
   * will be stored in that idx and if the Idx is  *
   * already filled and the value is equal to      *
   * interface type value then we will return that *
   * Index value to access the CONTROL PTR         *
   ************************************************/
   for( lstIdx=0; lstIdx < egCb.genCfg.nmbIntfs; lstIdx++)
   {
      if( edmCb.egIdxToIntfMap[lstIdx].pres != PRSNT_NODEF)
      {
         edmCb.egIdxToIntfMap[lstIdx].pres = PRSNT_NODEF;
         edmCb.egIdxToIntfMap[lstIdx].val = intfType;
         
         /************************************************
         *  Allocate the memory for the intfCbLst.       *
         ************************************************/
         if ( (edmCb.intfCbLst[lstIdx] = (EgIntfCb *)egAlloc(sizeof(EgIntfCb))) == NULLP)
         {
            /*****************************************************
            * Memory allocation for gm control block fails       *
            *****************************************************/
            EGLOGERROR(ERRCLS_ADD_RES, EEG089, 0,
                        "egDbCfgGmDict() failed, Memory allocation \
                         Fails for INTF control block");
            EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, ret);
            EG_RETVALUE(LEG_REASON_UNKNOWN);
         }

         /*************************************************
         * Initialise the CONTROL POINTER for this index  *
         *************************************************/
         
         intfCb = edmCb.intfCbLst[lstIdx];
         if(ROK != (ret = cmHashListInit(&intfCb->gmDictHlCp,  /* Hash List control pointer */
                           EG_INTF_GMDICT_BIN_SIZE,            /* Hash list bin size */
                           0,                                  /* Offset of HL entry */
                           FALSE,                              /* Allow Duplicate Key */
                           CM_HASH_KEYTYPE_CONID,              /* Hash Function */
                           egCb.init.region,                     /* Memory Region */
                           egCb.init.pool)))                     /* Memory Pool */
         {
            /**********************************************
            *  Initialisation failed return from here     *
            **********************************************/
            EGLOGERROR(ERRCLS_DEBUG, EEG090, 0,
                       "INIT FOR THE INTF CB FAILED\n");
            EG_RETVALUE(RFAILED);
         }

         break;
      }
      else
      {
         if(edmCb.egIdxToIntfMap[lstIdx].val == intfType)
         {
            intfCb = edmCb.intfCbLst[lstIdx];
            break;
         }
      }
   }

   /************************************************
   *      Check if the GM Dictionary already       *
   *      exists for the interface                 *
   ************************************************/
   if(ROK == (ret = (cmHashListGetNext(&intfCb->gmDictHlCp, prevGmCb, &nextGmCb))))
   {
      /*************************************************
      * GM Dictionary already exists for the interface *
      *          Drop the GM Dictionary                *
      *************************************************/
      egDbDropGmDict(intfType);

      if(ROK != (ret = cmHashListInit(&intfCb->gmDictHlCp,      /* Hash List control pointer */
                           EG_INTF_GMDICT_BIN_SIZE,            /* Hash list bin size */
                           0,                                    /* Offset of HL entry */
                           FALSE,                              /* Allow Duplicate Key? */
                           CM_HASH_KEYTYPE_CONID,              /* Hash Function */
                           egCb.init.region,                     /* Memory Region */
                           egCb.init.pool)))                     /* Memory Pool */
      {
         EGLOGERROR(ERRCLS_DEBUG, EEG091, 0,  "egDbCfgGmDict, failed to re-initialize \
                    the hash list for IE dictionary");
         EG_RETVALUE(LEG_REASON_UNKNOWN);
      }
   }

   /************************************************
   *   Copy the hashList cp and region and pool    *
   *   from where the memory has to be allocated   *
   *   for the GM Entries                          *
   ************************************************/
   hashListCp = &intfCb->gmDictHlCp;
   region = egCb.init.region;
   pool = egCb.init.pool;
   /************************************************
   *          Populate the mem structure           *
   ************************************************/
   mem.region = region;
   mem.pool = pool;

   /************************************************
   *          Populate the GM Dictionary           *
   ************************************************/
   for(msgCnt = 0; msgCnt < nmbMsg && gmEntry[msgCnt].msgType; msgCnt++)
   {
      /************************************************
      *   Prepare the hash key: intfType + msgType    *
      ************************************************/
      hashKey = gmEntry[msgCnt].msgType;

      /******************************************************
      * Check if the gm command exists in the GM Dictionary *
      ******************************************************/
      ret = cmHashListFind(hashListCp, (U8 *)&hashKey, sizeof(U8), 0,
                           (PTR*)&gmMsgCb);
      if(ret != ROK)
      {
         /*****************************************************
         * Its a new entry.Allocate the memory for GM Command *
         *****************************************************/
         if(ROK != (ret = SGetSBuf(region, pool, (Data **)&gmMsgCb,
                                   sizeof(EgGmCb))))
         {
            /****************************************************
            *  This Macro sends alarm ind and then drops the    *
            *  Msg entry based on the intf and then returns     *
            ****************************************************/
            EG_CFG_MEMFAIL(intfType, TRUE, ret,
                ("Memory allocation Fail for GM control block"));
         }
         /*****************************************************
         *       Populate the GM command control block        *
         *****************************************************/
         cmMemset((U8 *)gmMsgCb,0, sizeof(EgGmCb));
         gmMsgCb->gmEntry.msgType = gmEntry[msgCnt].msgType;
          
         /*****************************************************
         *       copy the position index array                *
         *****************************************************/
         for( instCnt=0; instCnt < LEG_MAX_INSTANCES; instCnt++)
         {
            (Void) cmMemcpy((U8 *)gmMsgCb->gmEntry.posIndex[instCnt], (U8 *)gmEntry[msgCnt].posIndex[instCnt], LEG_MAX_IES);
         }    
         gmMsgCb->gmEntry.nmbIeProp = gmEntry[msgCnt].nmbIeProp;

         /*****************************************************
         *            Populate the IE Properties              *
         *****************************************************/
         for(ieCnt = 0; ieCnt < gmMsgCb->gmEntry.nmbIeProp; ieCnt++)
         {
            gmMsgCb->gmEntry.ieProperties[ieCnt].ieType = gmEntry[msgCnt].
                                               ieProperties[ieCnt].ieType;

            gmMsgCb->gmEntry.ieProperties[ieCnt].ieClass = gmEntry[msgCnt].
                                               ieProperties[ieCnt].ieClass;

            gmMsgCb->gmEntry.ieProperties[ieCnt].ieList = gmEntry[msgCnt].
                                               ieProperties[ieCnt].ieList;

            gmMsgCb->gmEntry.ieProperties[ieCnt].ieInst = gmEntry[msgCnt].
                                               ieProperties[ieCnt].ieInst;
            /***************************************************
            *   Get the IE Entry from the Db and then check if *
            *   the Entry is valid or not. If valid then cont  *
            *   else return Error from here                    *
            ***************************************************/
            if( (tmpIeEntry = egGetIeEntryFrmDb(gmEntry[msgCnt].
                             ieProperties[ieCnt].ieType)) != NULLP)
            {
               dataType = tmpIeEntry->dataType;
            }
            else
            {
               /****************************************************
               *  This Macro sends alarm ind and then drops the    *
               *  Msg entry based on the intf and then returns     *
               ****************************************************/
               EG_CFG_MEMFAIL(intfType, FALSE, ret,
                ("IE Entry NOT found, frm the ie"));
            }
            if( dataType == EG_GROUPED)
            {
               ret = SGetSBuf(egCb.init.region, egCb.init.pool, 
                              (Data **)&(gmMsgCb->gmEntry.ieProperties[ieCnt].groupedIe), 
                              sizeof(EgGroupedIe));

               if(NULLP != gmMsgCb->gmEntry.ieProperties[ieCnt].groupedIe)
               {
                  /* KW_FIX */
                  cmMemset((U8 *)gmMsgCb->gmEntry.ieProperties[ieCnt].groupedIe ,0,sizeof(EgGroupedIe));
                  gmMsgCb->gmEntry.ieProperties[ieCnt].groupedIe->nmbGrpIe = gmEntry[msgCnt].ieProperties[ieCnt].groupedIe->nmbGrpIe;
                  ret = egMakeGrpIeEntry(egCb.init.region,egCb.init.pool,gmMsgCb->gmEntry.ieProperties[ieCnt].groupedIe,
                                                                                        &gmEntry[msgCnt].ieProperties[ieCnt]);
                  if (ret != ROK)
                  {
                     EG_RETVALUE(ret);
                  }
               }
               else
               {

                  /****************************************************
                  *  This Macro sends alarm ind and then drops the    *
                  *  Msg entry based on the intf and then returns     *
                  ****************************************************/
                  EG_CFG_MEMFAIL(intfType, TRUE, ret,
                              ("Memory allocation failed for the child Ie's"));
               } 
            }
            else
            {
               gmMsgCb->gmEntry.ieProperties[ieCnt].groupedIe = NULLP;
            }
         }
         /*****************************************************
         * Insert the GM control block into the GM Dictionary *
         *****************************************************/
         gmMsgCb->key = hashKey;
         EG_DBG_INFO(0, 0, (egp,
                 "egDbCfgGmDict(Inserting GM Entry of Msg Code= (%d))\n",
                 gmMsgCb->gmEntry.msgType));
         ret = cmHashListInsert(hashListCp, (PTR)gmMsgCb, (U8*)&gmMsgCb->key,
                                sizeof(U8));
         if(ROK != ret)
         {
            /*****************************************************
            *    GM Entry Insertion fails - Free the memory      *
            *     allocated for GM control block                 * 
            *****************************************************/
            SPutSBuf(region, pool ,(Data *)gmMsgCb ,sizeof(EgGmCb));

            /****************************************************
            *  This Macro sends alarm ind and then drops the    *
            *  Msg entry based on the intf and then returns     *
            ****************************************************/
            EG_CFG_MEMFAIL(intfType, FALSE, ret,
                             ("Fail to insert the GM Entry"));
         }
      }
      else
      {
         /****************************************************
         *  This Macro sends alarm ind and then drops the    *
         *  Msg entry based on the intf and then returns     *
         ****************************************************/
         EG_CFG_MEMFAIL(intfType, FALSE, ret,
                             (" Duplicate GM Entry"));
      }
   } 

   EG_RETVALUE(ret);
} 




#ifdef EG_MT
 /* **********************************************************************
 *   As of now, we are calling one common function to send the message   *
 *   back to the sender. Later once the modules are finalised, the       *
 *   corresponding module primitive will be called.                      *
 *   The code is written for both multi threaded and single threaded     *
 *   But while integrating we will be removing/guarding under one flag.  *
 *   This is written just to show that how the multi-threaded case will  *
 *   be handled by the EDM module.                                       *
 ************************************************************************/

/*************************************************************************
 *  
 *   Fun:  egEdmSendMsgToCm
 *   
 *   Desc:  send the encoded/Decodede msg to the main thread
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 ************************************************************************/
#ifdef ANSI
PRIVATE S16 egEdmSendMsgToCm
(
 Pst  *pst,
 EgEvnt *evntMsg 
)
#else
PRIVATE S16 egEdmSendMsgToCm(pst, evntMsg)
Pst  *pst;
EgEvnt *evntMsg; 
#endif
{
   S16         retVal = ROK;
   Pst         rPst;           /* holds the Pst being sent to Main thread */
   Buffer     *msgBuf = NULLP; /* used for packing */
   EgUstaDgn   dgn;    /* diagnostics for while sending alaram to LM */
   Mem         mem;
   Pst        *rPstPtr;

   EG_TRC2(egEdmSendMsgToCm);

   mem.region = pst->region;
   mem.pool  = pst->pool;

   if(evntMsg == NULLP)
   {
      EG_DBG_INFO(0,0, (egp,
            "egEdmSendMsgToCm: evntMsg is NULLP \n"));
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }
   EG_DBG_INFO(0,0, (egp,
          "egEdmSendMsgToCm: InstId(%d), Selector(%d) \n",
          pst->dstInst, pst->selector));

   /**************************************************
    * Initialize the diagnostic and memory structure *
    **************************************************/
    cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));

   /************************************************************
    * check the pst->event and set it to appropriate Cfm event *
    ***********************************************************/
   switch(pst->event)
   {
      case EVTEGENCREQ:
         rPst.event = EVTEGENCCFM;
         break;

      case EVTEGDECREQ:
         rPst.event = EVTEGDECCFM;
         break;

      default:
         retVal = RFAILED;
         /********************************************
          * print debug for invalid event & return   *
          *******************************************/
          EG_DBG_INFO(0,0, (egp,
                 "egEdmSendMsgToCm: InstId(%d), Selector(%d) \n",
                 pst->dstInst, pst->selector));
         EG_RETVALUE(retVal);
   }

   /*********************************
    * Fill the reply post structure *
    ********************************/
   rPst.srcEnt = pst->dstEnt;
   rPst.srcInst = pst->dstInst;
   rPst.srcProcId = pst->dstProcId;;
   rPst.dstEnt = pst->srcEnt;
   rPst.dstInst = pst->srcInst;
   rPst.dstProcId = pst->srcProcId;
   Pst.selector = pst->selector;
   rPst.prior = pst->prior;
   rPst.route = pst->route;
   rPst.region = pst->region;
   rPst.pool = pst->pool;
   
   /***********************************
    * check the selector for coupling *
    **********************************/
   /**********************
    * For Tight Coupling *
    *********************/
   if(rPst.selector == EG_EDM_TC) 
   {
      EG_RETVALUE(retVal); 
   }
   /**********************
    * For Loose Coupling *
    *********************/
   else
   {
      /*******************************************
       * allocate the Buffer to hold the packing *
       ******************************************/
      retVal = SGetMsg(rPst.region, rPst.pool, &msgBuf);
      if(retVal != ROK)
      {
         /******************************************************
          * memory allocation failed, send alarm to LM         *
          *****************************************************/
         EG_DBG_INFO(0,0, (egp,
            "egEdmSendMsgToCm: memory allocation failed for msgBuf \n"));

         EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);

         retVal = EGT_ERR_MEM_ALLOC_FAILED;
         EG_RETVALUE(retVal);
      }
      else
      {
         /***************************************************************
          * pack the evntMsg pointer and post it to main thread via SSI *
          **************************************************************/
         rPstPtr = &rPst;
         CMCHKPKLOG(cmPkPtr, (PTR)evntMsg, msgBuf, EEG092, rPstPtr);
         EG_RETVALUE(SPstTsk(&rPst,msgBuf));
      }
   }
}
#endif /* EG_MT */


/**********************************************************
*                   PATH MANAGEMENT MESSAGES              *
**********************************************************/



/***********************************************************************
 *  
 *   Fun:  egEncPthReq
 *   
 *   Desc:  This will Encode the path management msg's
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c  
 *   
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egEncPthReq
(
 EgEvnt *evntMsg           /* Event message having the Msg to encode msg */
)
#else
PUBLIC S16 egEncPthReq(evntMsg)
EgEvnt *evntMsg;           /* Event message having the Msg to encode msg */
#endif
{
   S16                  retVal = ROK;    /* Holds the return value */
   Buffer              *mBuf = NULLP;    /* Stores the Encoded Buffer */
   CmLList             *tmpNode = NULLP; /* Stores the Ie Node */
   EgUstaDgn            dgn;             /* diagnostics for while sending alaram to LM */
   Mem                  mem;             /* Used for allocating memory using reg and pool */
   U16                  cnt    = 0;      /* Used for iterating thru the list of nodes */
   U16                  tmpLen = 0;      /* Stores the ieLen temp */
   MsgLen               encLen = 0;      /* Temp stores the encoded length */
   EgIe                *tmpIe = NULLP;   /* Holds the IE NODE */

#ifdef LEG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16       nmbLines = 0;
#endif
#endif 

   EG_TRC2(egEncPthReq);
 

   cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
   cmMemset((U8*)&mem, EG_ZERO, sizeof(Mem));
   /***********************************************************
    * Check for event message being proper                    *
    **********************************************************/
   if (!evntMsg)
   {
      EG_DBG_INFO(0, 0, (egp,
            "egEncPthReq: evntMsg is NULLP \n"));

   /**************************************************************
    * eg006.201 : Send out Alarm case Mem alloc fails for Evntmsg*
    **************************************************************/
      dgn.dgnVal[0].type = LEG_PAR_MEM;
      egSendLmAlarm(LCM_CATEGORY_LYR_SPECIFIC,
                    LCM_EVENT_LYR_SPECIFIC,
                    LCM_CAUSE_DECODE_ERR, &dgn); 

      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /***********************************************************
    * Check for eGTP message to be encoded being proper       *
    **********************************************************/ 
   if (!evntMsg->u.egMsg)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_DBG_INFO(0, 0, (egp,
            "egEncPthReq: egMsg is NULLP \n"));

   
   /***********************************************************
    * Fill and send error event message to main thread        *
    **********************************************************/ 
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 0);
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }
  
   /*************************
    * Copy region and pool  *
    ************************/
   mem.region = egCb.init.region;
   mem.pool   = egCb.init.pool;

#ifdef _EG_PRINT_BUF_
   EgUtilGmDump(evntMsg->u.egMsg
#ifdef LEG_FILE_LOG
                                , egCb.dbgFp, &nmbLines
#endif /* end of LEG_FILE_LOG */
                );
#endif

   
   /**********************************************************
    * Validate the eGTP message header                       *
    *********************************************************/
   retVal = egValidatePthHdr(evntMsg);   
   if(retVal != ROK)
   {
      /***********************************************************
       * Fill and send error event message to main thread        *
       **********************************************************/ 
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 
                           evntMsg->u.egMsg->msgHdr.msgType);

      EG_RETVALUE(retVal);
   }
   
   /**************************************
    * validate the incoming encoded IE's *
    *************************************/
   retVal = egValidatePthMsgIEs(evntMsg);
   if(retVal != ROK)
   {
      /***********************************************************
       * Fill and send error event message to main thread and    * 
       * free buffer                                             *
       **********************************************************/ 
      EG_RETVALUE(retVal);
   }

   /***********************************************************
    * Validation is successful, do the encoding               * 
    * Allocate memory for the buffer                          *
    **********************************************************/ 
   retVal = SGetMsg(egCb.init.region, egCb.init.pool, &mBuf);
   if(retVal != ROK)
   {
      /***********************************************************
       * Memory allocation failed, send alarm to LM and error    *
       * event message to main thread along with result code     * 
       **********************************************************/ 
      EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);

      retVal = EGT_ERR_MEM_ALLOC_FAILED;
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, evntMsg->u.egMsg->msgHdr.msgType); 
      EG_RETVALUE(retVal);
   }

   /*****************************
    * Encode the message header *
    ****************************/
   egEncPthHdr(evntMsg, mBuf);

   /**************************************************************
    *                Encode the individual IE'                   *
    *************************************************************/
   CM_LLIST_FIRST_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
   for(cnt = 0; 
       ((cnt < evntMsg->u.egMsg->egIeList.count) && (tmpNode)); 
       cnt++)
   {
      /*****************************
       * Get the IE entry          *
       ****************************/
      tmpIe = (EgIe*)tmpNode->node;

      /*****************************
       * Encode the IE             *
       ****************************/
      retVal = egEncPthIe(tmpIe, mBuf);
      if(retVal != ROK)
      {
         /***********************************************************
          * Fill and send error event message to main thread and    * 
          * free buffer                                             *
          **********************************************************/ 
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe->ieType, 
                              tmpIe->ieInst, evntMsg->u.egMsg->msgHdr.msgType); 
         SPutMsg(mBuf);
         EG_RETVALUE(retVal);
      }
      else
      {
         /*********************************
          * Get the next IE from the list *
          ********************************/
         CM_LLIST_NEXT_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
      }
   }

   /**************************************************************
    * Fill the message length, length starts from index 2        *
    * calculate total Buffer length and decrement 4bytes which   *
    * is to encode the mandatory hdr elements                    *
    *************************************************************/
   SFndLenMsg(mBuf, &encLen);
   encLen -= EG_LEN_FOUR;

   /**************************************************************
    * Fill the message length, length starts from index 2        *
    *************************************************************/
   tmpLen = EG_LEN_TWO;

   EG_ENC_LEN(encLen, mBuf, tmpLen);

   /**************************************
    * Free the event structure received  *       
    *************************************/
   cmFreeMem(&evntMsg->u.egMsg->memCp);
   evntMsg->u.egMsg = NULLP;

   /****************************************
    * Assign the mBuf in the event message *     
    ***************************************/
   evntMsg->u.mBuf = mBuf;

   /****************************************************************
    * Send the message to the main thread with SUCCESS result code *
    ***************************************************************/
   evntMsg->resultType = EG_SUCCESS;
   egTptProcessMsg(evntMsg);

   EG_RETVALUE(retVal);
}

   
/*************************************************************************
 *  
 *   Fun:  egEncodeMsgHdr
 *   
 *   Desc:  This will Encode the message Header.
 *   
 *   Ret:   ROK  - for Success
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 ************************************************************************/
#ifdef ANSI
PRIVATE S16 egEncPthHdr
(
EgEvnt *evntMsg,      /* Event message having the MsgHdr to encode hdr */
Buffer     *mBuf         /*  Mbuf which holds the encoded buffer          */
)
#else
PRIVATE S16 egEncPthHdr( evntMsg, mBuf)
EgEvnt *evntMsg;     /* Event message having the MsgHdr to encode hdr */
Buffer     *mBuf;        /*  Mbuf which holds the encoded buffer          */
#endif
{
   S16              retValue = ROK;          /* Holds the return value */
   EgMsgHdr         header;                  /* Temp stores the header struct for enc */
   Data             dumMsgLen[EG_LEN_THREE]; /* for length encoding */
   U8               tmpByte = 0;             /* Tmp stores the one byte of data for enc*/

   EG_TRC2(egEncPthHdr)

   /********************************************
    * Clearing of message lenth and spare byte *
    *******************************************/
   cmMemset((U8*)dumMsgLen, EG_ZERO, EG_LEN_THREE);

   /********************************************************
    * Copy the eGTP message header to a local buffer       *
    *******************************************************/
   cmMemset((U8* )&header, 0, sizeof(EgMsgHdr));
   cmMemcpy( (U8* )&header, (U8* )&evntMsg->u.egMsg->msgHdr,
           sizeof(EgMsgHdr));

   /********************************************************
    * Encode the version  number, shift 5 times to store   *
    * the version in the last 3bits                        *
    *******************************************************/
   tmpByte = header.version << EG_SHIFT_5;

   SAddPstMsg(tmpByte, mBuf);

   /*********************************************************
    * Encode the Message Type                             *
    ********************************************************/ 
   SAddPstMsg(header.msgType, mBuf);

   /*********************************************************
    * Encode the message length with zeroes                 *
    * At the end of the encoding all the IE's, the final    *
    * Length will be calculated and updated the mBuf        *
    ********************************************************/
   SAddPstMsgMult(dumMsgLen, EG_LEN_TWO, mBuf);

   /*********************************************************
    * Encode the sequence number                            *
    ********************************************************/
   header.seqNumber &= 0x00FFFFFF;
   header.seqNumber <<= EG_SHIFT_8;
   header.seqNumber &= 0xFFFFFF00;
   EG_ENC_U32(header.seqNumber, mBuf);

   EG_RETVALUE(retValue);

}



/***********************************************************************
 *  
 *   Fun:  egEncPthIe
 *   
 *   Desc:  Encodes the IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncPthIe
(
EgIe  *ie,            /* IE to encode */
Buffer *mBuf          /* Buffer holds the encoded IE */
)
#else
PRIVATE S16 egEncPthIe(ie, mBuf)
EgIe   *ie;          /* IE to encode */
Buffer *mBuf;        /* Buffer holds the encoded IE */
#endif
{
   S16      retVal = ROK;           /* Holds the return value */
   MsgLen   initLen = 0;            /* Initial mBuf Length for Grp IE length encoding */
   MsgLen   finLen = 0;             /* final mBuf Length for Grp IE length encoding */
   MsgLen   ieLen = 0;              /* actual length of IE being encoded */
   Data     dumBytes[EG_LEN_BYTES]; /* Tmp stores the data for Enc */
   U8       tmpByte = 0;            /*Holds the one byte for Enc */

   EG_TRC2(egEncPthIe);

   cmMemset((U8*)dumBytes, EG_ZERO, EG_LEN_BYTES);

   /*************************
    * Check for IE validity *
    ************************/
   if(ie == NULLP)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_RETVALUE(retVal);
   }

   /*********************
    * Debug information *
    ********************/
   EG_DBG_INFO(0,0, (egp,
         "egEncPthIe(IE Type(%d), instance(%d)) \n", 
          ie->ieType, ie->ieInst));

   /****************************************
    * Store the index to encode the length *
    ***************************************/
   SFndLenMsg(mBuf, &initLen);

   /**********************
    * Encode the IE Type *
    *********************/
   SAddPstMsg(ie->ieType, mBuf);

   /*****************************************
    * Encode dummy bytes in the length field *
    ****************************************/
   SAddPstMsgMult((U8*)dumBytes, EG_LEN_TWO, mBuf);

   /**************************
    * Encode the IE Instance *
    **************************/
   tmpByte = ((tmpByte) | (ie->ieInst & EG_MASK_INST));
   SAddPstMsg(tmpByte, mBuf);
   
   /**********************
    * Encode the IE data *
    *********************/ 
   {
      retVal = egEncIeData(mBuf, ie);
      if(retVal != ROK)
         EG_RETVALUE(retVal);
   }
   /********************************************
    * Encode the IE length for this grouped IE *
    *******************************************/
   SFndLenMsg(mBuf, &finLen);

   /***********************************
    * Decrement 4bytes for the ie Hdr *
    **********************************/
   ieLen = finLen - initLen - EG_LEN_FOUR;
   
   /***********************************************************
    * Increment the index to point the actual length location *
    **********************************************************/
   initLen += EG_IE_LEN_OFFSET;

   /********************************************
    * Encode the offset b/w finLen and initLen *
    *******************************************/
   EG_ENC_LEN(ieLen, mBuf, initLen); 

   EG_RETVALUE(retVal);
}


/************************************************************************
 *  
 *   Fun:  egValidatePthHdr
 *   
 *   Desc:  To Validate Path Msg header as per specs.
 *   
 *   Ret:   ROK  - for Success
 *          EGT_ERR_INV_HDR -   Invalid GTP Header
 *          EGT_ERR_UNSUPP_VER - VERSION not supported
 *          EGT_ERR_MSG_INV_TYPE - Invalid Message Type
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egValidatePthHdr
(
 EgEvnt *evntMsg  /* Event message to validate */
)
#else
PRIVATE S16 egValidatePthHdr( evntMsg)
EgEvnt *evntMsg;   /* Event message to validate */
#endif
{
   S16           retValue = ROK;  /* Holds the return value  */
   EgMsgHdr      hdr;             /* Holds the Message Hdr   */

   /*************************************************************
    *  Trace the function only if debug mask is enabled for EDM *
    ************************************************************/

   EG_TRC2(egValidatePthHdr)

   cmMemset( (U8* )&hdr, 0, sizeof(EgMsgHdr));

   cmMemcpy( (U8* )&hdr, (U8* )&evntMsg->u.egMsg->msgHdr, sizeof(EgMsgHdr));

   EG_DBG_INFO(0,0, (egp,
            "egValidatePthHdr( Message Type (%d) \n",
             hdr.msgType));

   /****************************
    * validate the version     *
    ***************************/
   if(hdr.version != EG_MSG_VERSION)
   {
      retValue = EGT_ERR_MSG_INV_VERSION;
      EG_DBG_INFO(0,0, (egp,
             "egValidatePthHdr(Invalid Msg Version(%d), \
             resultType(%d) \n", hdr.version, retValue));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, hdr.msgType);
      EG_RETVALUE(retValue);
   }
   /********************************************************************
    * If the Message type is other that the path message, Then         *
    * its a error, invalid msg type                                    *
    *******************************************************************/
   if( (hdr.msgType != EGT_GTPC_MSG_ECHO_REQ) &&
       (hdr.msgType != EGT_GTPC_MSG_ECHO_RSP) &&
       (hdr.msgType != EGT_GTPC_MSG_VER_N_SUPP_IND))
   {

      retValue = EGT_ERR_MSG_INV_TYPE;
      EG_DBG_INFO(0,0, (egp,
               "egValidatePthHdr(Invalid Message Type(%d) resultType(%d) \n",
                hdr.msgType, retValue));
      EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, hdr.msgType);
      EG_RETVALUE(retValue);


   }
   /********************************************************************
    * Check for T- BIT. If the T-bit is set in case of the all         *
    * Path messages then its a error                                   *
    *******************************************************************/
   if (evntMsg->u.egMsg->msgHdr.teidPres)
   {
      /*******************************************************************
       * If the message is other than the ECHO REQ/RSP, then its a error *
       * TEID should be present in all the other messages                *
       ******************************************************************/
      /*************************************
       *  Report invalid message Format    *
       ************************************/
      retValue = EGT_ERR_MSG_INV_HDR;
      EG_DBG_INFO(0,0, (egp,
             "egValidatePthHdr(Invalid message header: resultType(%d) \n",
                retValue));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 0, 
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retValue);
   } 
   
   EG_RETVALUE (retValue);
} 

/****************************************************************************
 *  
 *   Fun:  egValidatePthMsgIEs
 *   
 *   Desc:  To Validate the Diameter Msg IEs. 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 ***************************************************************************/
#ifdef ANSI
PRIVATE S16 egValidatePthMsgIEs
(
EgEvnt *evntMsg      /* Contains the EGMSG for validation */
)
#else
PRIVATE S16 egValidatePthMsgIEs(evntMsg)
EgEvnt *evntMsg;      /* Contains the EGMSG for validation */
#endif
{
   S16              retVal = ROK;     /* Holds the return value       */
   EgMsgHdr         hdr;              /* Holds the messge Hds         */
   CmLList          *tmpNode;         /* Holds the Node Ptr           */
   EgIe            *ie = NULLP;       /* Holds the IE                 */
   U16              cnt;              /* Used for iter thru list      */
   Bool              search = FALSE;  /* Tells that the IE is found   */

   EG_TRC2(egValidatePthMsgIEs)

   /*********************************
    * initialize the local variable *
    ********************************/
   tmpNode = NULLP;
   cnt     =     0;
   cmMemset( (U8* )&hdr, 0, sizeof(EgMsgHdr));

   /************************************
    * Retrieve the Msg Type properties *
    ***********************************/
   cmMemcpy( (U8* )&hdr, (U8* )&evntMsg->u.egMsg->msgHdr, sizeof(EgMsgHdr));

   EG_DBG_INFO(0,0, (egp,
            "egValidatePthMsgIEs(Msg Type(%d) \n",    
             hdr.msgType));

   CM_LLIST_FIRST_NODE(&evntMsg->u.egMsg->egIeList, tmpNode);
   if ( (tmpNode == NULLP) && 
        (evntMsg->u.egMsg->msgHdr.msgType != EGT_GTPC_MSG_VER_N_SUPP_IND))
   {

      retVal = EGT_ERR_MSG_INV_LEN;
      EGLOGERROR(ERRCLS_DEBUG, EEG093, 0,
                 "egValidatePthMsgIEs() IE's are not present\n");
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);

   }
   for(cnt = 0; ((cnt < evntMsg->u.egMsg->egIeList.count) && (tmpNode)); cnt++)
   {                   
      ie = (EgIe*)tmpNode->node;
      /********************************************************
      *   Check if the ieType is Recovery IE and instance is  *
      *   Zero. If it is not present then return mandatory    *
      *   IE is missing. Othe IE will not be checked as       *
      *   Optional IE is not required                         *
      ********************************************************/
      if( ie->ieType == EG_IE_RECOVERY && ie->ieInst == EG_IE_INST_ZERO)
      {
         search = TRUE;
         break;
      }
   } 
   /* eg012.201 : Added NULL check */
   if( (!search) && (ie))
   {
     
      retVal = EGT_ERR_MSG_MISS_MAND_IE;
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 
                           ie->ieType, ie->ieInst,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }

   EG_RETVALUE(retVal);
} 



/**********************************************************
 *  
 *   Fun:  egDecPthReq
 *   
 *   Desc:  Decodes the GTP Path Msg and then validate it.
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 *********************************************************/
#ifdef ANSI
PUBLIC S16 egDecPthReq
(
 EgEvnt *evntMsg            /* Event message having the Msg to decode msg */
)
#else
PUBLIC S16 egDecPthReq( evntMsg)
EgEvnt *evntMsg;            /* Event message having the Msg to decode msg */
#endif
{
   S16            retVal = ROK; /* stores the return value */
   Buffer        *mBuf = NULLP; /* temp stores the buffer present in the
                                    event msg */
   EgUstaDgn      dgn;         /* diagnostics for while sending alaram to LM */
   MsgLen         msgLen = 0;  /* Stores the Message length after decoding */
   Mem            mem;         /* Memory used for allocating */
   EgIeLevelInfo  ieLvlInfo;   /* Stores the IE Level Info */
   U16            ieLen = 0;   /* Stores the IE Length */

#ifdef LEG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16      nmbLines = 0;
#endif
#endif 
   EG_TRC2(egDecPthReq);

   /*********************************************************
    * Verify the NULL check for event message               *
    ********************************************************/
   if (!evntMsg)
   {
      EG_DBG_INFO(0, 0, (egp,
            "egDecPthReq: evntMsg is NULLP \n"));
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /*********************************************************
    * Initialize memory region and pool                     *
    ********************************************************/
   mem.region = evntMsg->srvCb->uSapCb->pst.region;
   mem.pool   = evntMsg->srvCb->uSapCb->pst.pool;
   cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
   cmMemset((U8*)&ieLvlInfo, EG_ZERO, sizeof(EgIeLevelInfo));

   /***********************************************************
    * Fill the event message result code if buffer is invalid *
    **********************************************************/
   if (!evntMsg->u.mBuf)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_DBG_INFO(0, 0, (egp,
            "egDecPthReq: mBuf is NULLP \n"));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, 0);

      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /***********************************************************
    * Assign the buffer to the local pointer and clear the    *
    * event structure buffer                                  *
    **********************************************************/
   mBuf = evntMsg->u.mBuf;
   evntMsg->u.mBuf = NULLP;

   /***********************************************************
    * Allocate the memory for the event structure             *
    **********************************************************/
   if(cmAllocEvnt(sizeof(EgMsg), EG_MEM_SDU_SIZE, 
      &mem, (Ptr*)&evntMsg->u.egMsg) != ROK)
   {
      EG_DBG_INFO(0, 0, (egp,
            "egDecPthReq: Memory allocation failed for egMsg \n"));

      /***********************************************************
       * Fill and send error event message to main thread and    *
       * free memory associated with mBuf.                       * 
       **********************************************************/ 
      EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);
      /* free the memory associated with mBuf */
      SPutMsg(mBuf);
      /* validation failed. send Msg to Main Thread */
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /**************************************************
    * Find the message length for tracking purpose   *
    *************************************************/
   SFndLenMsg(mBuf, &msgLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egDecPthReq(MsgLen(%ld)) \n", msgLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egDecPthReq(MsgLen(%d)) \n", msgLen));
#endif
#endif

   /**************************************************
    * Decode the GTP message header                  *
    *************************************************/
   retVal = egDecPthHdr(evntMsg, mBuf);
   if(retVal != ROK)
   {

      /************************************************
      *  If the error is INV version then frame the   *
      *  VER NOT SUPP INDI and send the msg back to   *
      *  Originator                                   *
      ************************************************/
      if( retVal == EGT_ERR_MSG_INV_VERSION)
      {
         /********************************************
         * Prepare the header for the message        *
         ********************************************/
         retVal = egEncVerIndMsg(evntMsg, mBuf, FALSE);
      }
      /*******************
      * free the memory  *
      *******************/
      SPutMsg(mBuf);
      EG_RETVALUE(retVal);
   } 

   /**************************************************
    * Decode the GTP IE's                            *
    *************************************************/
   msgLen -= EG_LEN_EIGHT;
   while (msgLen)
   {
      ieLvlInfo.level = 0;
      retVal = egDecPthIe(evntMsg, mBuf, &ieLvlInfo, &ieLen);
      if (retVal != ROK)
      {
         SPutMsg(mBuf);
         EG_RETVALUE(retVal);
      } 

      SFndLenMsg(mBuf, &msgLen);
   } 

   /********************************
    * Validate the decoded IE's    *
    * only in case of Echo REQ/RSP *
    *******************************/
   if ( evntMsg->u.egMsg->msgHdr.msgType != EGT_GTPC_MSG_VER_N_SUPP_IND)
   {
      retVal = egValidatePthMsgIEs(evntMsg);
      if(retVal != ROK)
      {
         /***********************************************************
          * Fill and send error event message to main thread and    *
          * free memory associated with mBuf.                       * 
          **********************************************************/ 
         SPutMsg(mBuf);   
         EG_RETVALUE(retVal);
      }
   }
   /****************************************************************
    * Decoding is successful, free the memory associated with mBuf *
    * and send the message to main thread with return as SUCCESS   *
    ***************************************************************/
   SPutMsg(mBuf);

   evntMsg->resultType = EG_SUCCESS;

#ifdef _EG_PRINT_BUF_
   EgUtilGmDump(evntMsg->u.egMsg
#ifdef LEG_FILE_LOG
                  , egCb.dbgFp, &nmbLines
#endif /* end of LEG_FILE_LOG */
                );
#endif /* end of _EG_PRINT_BUF_ */
   retVal = egTunReceive(evntMsg);

   EG_RETVALUE(retVal);
} 


/***********************************************************************
 *  
 *   Fun:  egEncVerIndMsg
 *   
 *   Desc:  Decodes the GTP Path Msg Header
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncVerIndMsg
(
 EgEvnt *evntMsg,         /* Event message stores the decoded hdr */
 Buffer     *mBuf,        /* Buffer to decode the header */
 Bool    cntrlMsg         /* Tells whether it is a cntrl msg */
)
#else
PRIVATE S16 egEncVerIndMsg(evntMsg, mBuf, cntrlMsg)
EgEvnt *evntMsg;          /* Event message stores the decoded hdr */
Buffer     *mBuf;         /* Buffer to decode the header */
Bool    cntrlMsg;         /* Tells whether it is a cntrl msg */
#endif
{
   S16      retVal = ROK;      /* Holds the return value */
   U8  tmpByte = EGT_GTPC_MSG_VER_N_SUPP_IND; /* Holds the one byte of data after Dec */
   EgUstaDgn        dgn;            /* Used for sending the alarms         */
   Mem                  mem;             /* Used for allocating memory using reg and pool */
   MsgLen   bufLen = 0;        /* Holds the total buffer length */
   U32      seqNo=0;           /* Holds the seq to encode */
   U16         count= 6;                   /* To count number of bytes */

   EG_TRC2(egEncVerIndMsg);

   cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
   cmMemset((U8*)&mem, EG_ZERO, sizeof(Mem));


   SFndLenMsg(mBuf, &bufLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egEncVerIndMsg(MsgLen(%ld)) \n", bufLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egEncVerIndMsg(MsgLen(%d)) \n", bufLen));
#endif
#endif

   /***************************************************
   * Check the length of buffer to be decoded         *
   * Before calling this function already first two   *
   * were decoded. So has to check the length of the  *
   * msg Header as should not be less than 4bytes for *
   * path msg and 8bytes for all other msgs          *
   ***************************************************/
   if( (cntrlMsg && (bufLen <  8)) || (!cntrlMsg && (bufLen <  4)))
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
            "egEncVerIndMsg(MsgLen(%ld), resultType(%d)) -- Invalid \n",
       bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
            "egEncVerIndMsg(MsgLen(%d), resultType(%d)) -- Invalid \n",
       bufLen, retVal));
#endif
#endif
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal); 
   }
 
   /***************************************************
   * Free the egMsg and start framing the version ind *
   ***************************************************/

   EG_FREE_GM_MSG(evntMsg->u.egMsg);
   retVal = SGetMsg(egCb.init.region, egCb.init.pool, &evntMsg->u.mBuf);
   if(retVal != ROK)
   {
      /***********************************************************
       * Memory allocation failed, send alarm to LM and error    *
       * event message to main thread along with result code     * 
       **********************************************************/ 
      mem.region = egCb.init.region;
      mem.pool = egCb.init.pool;
      EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);
      retVal = EGT_ERR_MEM_ALLOC_FAILED;
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, tmpByte);
      EG_RETVALUE(retVal);
   }
  
   /**********************************************
   *           Encode 1st byte as version2       *
   *           Encode 2nd byte as version ind    *
   *           Encode 3rd & 4th byte as 0bytes   *
   *           Encode 5th & 6th seqno bytes      *
   *           Copy from the recevide message    *
   **********************************************/
   tmpByte = EG_MSG_VERSION << EG_SHIFT_5;
   SAddPstMsg(tmpByte, evntMsg->u.mBuf);
   tmpByte = EGT_GTPC_MSG_VER_N_SUPP_IND; 
   SAddPstMsg(tmpByte, evntMsg->u.mBuf);
   EG_ENC_U16( EG_LEN_FOUR, evntMsg->u.mBuf);
   
   /**********************************************
   * If the message is contol msg then 8Bytes    *
   * need to be retreived from the rcd buffer    *
   * If the message is path management then      *
   * copy 4bytes from the Rcd  buffer            *
   **********************************************/
   if( cntrlMsg )
   {
      /*******************************************
      * Get the Seq No from the mBuf and then    *
      * Copy the 6 & 7 bytes to the new Mbuf     *
      * This is to copy the seq no in to mBuf    *
      *******************************************/
      EG_EXAM_U32(seqNo, mBuf, count);
      EG_ENC_U32(seqNo, evntMsg->u.mBuf);
   }
   else
   {
      /*******************************************
      * Get the Seq No from the mBuf and then    *
      * Copy the 2 & 3 bytes to the new Mbuf     *
      * This is to copy the seq no in to mBuf    *
      *******************************************/
      count = 4;
      EG_EXAM_U32(seqNo, mBuf, count);
      EG_ENC_U32(seqNo, evntMsg->u.mBuf);
   }

   /****************************************************
   * Return EGT_ERR_MSG_INV_VERSION, since TPT will expect *
   * this error to send the message to peer            *
   ****************************************************/
   EG_RETVALUE(EGT_ERR_MSG_INV_VERSION);

}
/***********************************************************************
 *  
 *   Fun:  egDecPthHdr
 *   
 *   Desc:  Decodes the GTP Path Msg Header
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecPthHdr
(
 EgEvnt *evntMsg,         /* Event message stores the decoded hdr */
 Buffer     *mBuf             /* Buffer to decode the header */
)
#else
PRIVATE S16 egDecPthHdr(evntMsg, mBuf)
EgEvnt *evntMsg;          /* Event message stores the decoded hdr */
Buffer     *mBuf;             /* Buffer to decode the header */
#endif
{
   S16      retVal = ROK;      /* Holds the return value */
   MsgLen   msgLen = 0;        /* Holds the msgLen from the Hdr */
   MsgLen   bufLen = 0;        /* Holds the total buffer length */
   U8       tmpByte = 0;       /* Holds the one byte of data after Dec */

   EG_TRC2(egDecPthHdr);

   /****************************************************
    * Check the length of buffer to be decoded         *
    ***************************************************/
   SFndLenMsg(mBuf, &bufLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egDecPthHdr(MsgLen(%ld)) \n", bufLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egDecPthHdr(MsgLen(%d)) \n", bufLen));
#endif
#endif

   /********************************************************
    * Msg header is 8 bytes in case of ECHO REQ/RSP , and  *
    * 12 bytes in case of all control messages atleast msg *
    * should be of this much length                        *
    * Since the Echo request or response will not this fn  *
    * the minimum length should be 12bytes                 *
    *******************************************************/
   if(bufLen <  EG_MIN_PATH_MSG_LEN)
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
            "egDecPthHdr(MsgLen(%ld), resultType(%d)) -- Invalid \n",
       bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
            "egDecPthHdr(MsgLen(%d), resultType(%d)) -- Invalid \n",
       bufLen, retVal));
#endif
#endif
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal); 
   }
   
   /******************************************
    * decode the version & Right shift       *
    * 5 times to store in the version field  *
    *****************************************/
   SRemPreMsg(&tmpByte, mBuf);
   evntMsg->u.egMsg->msgHdr.version = tmpByte >> EG_SHIFT_5;

   /******************************
    * decode the message Type    *
    *****************************/
   SRemPreMsg(&(evntMsg->u.egMsg->msgHdr.msgType), mBuf);

   /********************************************************
   * Check if the version is 2, if not report INV version  *
   ********************************************************/
   if(evntMsg->u.egMsg->msgHdr.version != EG_MSG_VERSION)
   {
      retVal = EGT_ERR_MSG_INV_VERSION;
      EG_DBG_INFO(0,0, (egp,
             "egValidateGmHdr(Invalid Msg Version(%d), \
             resultType(%d) \n", evntMsg->u.egMsg->msgHdr.version, retVal));

      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0, evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }

   /******************************
    * decode the message length  *
    *****************************/
   EG_DEC_U16(msgLen, mBuf);
   
   /*******************************************************************
    * The message length should not have the first 4 mandatory octets *
    * Check whether the message lenght is less than the 4 octets on   *
    * the total buffer length                                         *
    ******************************************************************/
   if(msgLen != bufLen - EG_LEN_FOUR)
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
            "egDecPthHdr(Decoded MsgLen(%ld), actual MsgLen(%ld), \
            resultType(%d)) -- Invalid Length \n", 
            msgLen, bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
            "egDecPthHdr(Decoded MsgLen(%d), actual MsgLen(%d), \
             resultType(%d)) -- Invalid Length \n", 
            msgLen, bufLen, retVal));
#endif            
#endif            
      EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }
   bufLen -= EG_LEN_FOUR;

   /***********************************
    * decode the Teid if T-bit is set *
    **********************************/
   if ( tmpByte & EG_MASK_BIT4 )
   {
      evntMsg->u.egMsg->msgHdr.teidPres = TRUE;
      EG_DEC_U32(evntMsg->u.egMsg->msgHdr.teid, mBuf);
      bufLen -= EG_LEN_FOUR;
   }
   
   /***********************
    * decode the seq no   *
    **********************/
   EG_DEC_U32(evntMsg->u.egMsg->msgHdr.seqNumber, mBuf);
   evntMsg->u.egMsg->msgHdr.seqNumber >>= EG_SHIFT_8;
   evntMsg->u.egMsg->msgHdr.seqNumber &= 0x00FFFFFF;
   bufLen -= EG_LEN_FOUR;

   
   /********************************************************************
    * check for msgLen error, if no error continue, otherwise return   *
    *******************************************************************/
   if(retVal == ROK)
   {
      /***************************************
       * Call to validate the GTP Msg Header *
       **************************************/
      retVal = egValidatePthHdr(evntMsg);
   }

   EG_RETVALUE(retVal); 
}



/***************************************************************************
 *  
 *   Fun:  egDecPthIe
 *   
 *   Desc:  Decodes the GTP IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 *   
 **************************************************************************/
#ifdef ANSI
PRIVATE S16 egDecPthIe
(
 EgEvnt     *evntMsg,  /* Event message to stores the decoded ie */
 Buffer         *mBuf,     /* Buffer which has to decode */
 EgIeLevelInfo *ieLvlInfo, /* Contains the level info to store in the event msg */
 U16            *outIeLen /* Contains the Ie len to decode */
)
#else
PRIVATE S16 egDecPthIe(evntMsg, mBuf, ieLvlInfo, outIeLen)
EgEvnt     *evntMsg;   /* Event message to stores the decoded ie */
Buffer         *mBuf;      /* Buffer which has to decode */
EgIeLevelInfo *ieLvlInfo;  /* Contains the level info to store in the event msg */
U16            *outIeLen;  /* Contains the Ie len to decode */
#endif
{
   S16              retVal = ROK;   /* Holds the return value              */
   EgIe             tmpIe;          /* tmp Holds the Decoded IE            */
   EgIeEntry       *ieEntry= NULLP; /* Holds the IE Entry                  */
   MsgLen           msgLen = 0;     /* Holds the Rcd Buffer length         */
   U16              tmpIeLen = 0;   /* Holds the IE Length                 */
   EgUstaDgn        dgn;            /* Used for sending the alarms         */
   U8               tmpByte = 0;    /* Holds the 1byte of data after Dec   */
   U8               skipIe[EG_MAX_BYTES]; /* To Skip IE from the buf       */

   EG_TRC2(egDecPthIe);

   cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
   cmMemset((U8*)&tmpIe, EG_ZERO, sizeof(EgIe));
   cmMemset((U8*)skipIe, EG_ZERO, EG_MAX_BYTES);

   /*****************************************
    * find the length of the encoded buffer *
    ****************************************/
   SFndLenMsg(mBuf, &msgLen);

   /*************************
    * decode the IE Type    *
    ************************/
   SRemPreMsg(&tmpIe.ieType, mBuf);
   msgLen--;

   /************************
    * decode the IE length *
    ***********************/
   EG_DEC_U16(tmpIeLen, mBuf);
   msgLen -= EG_LEN_TWO;
   *outIeLen = tmpIeLen;

   /***************************
    * decode the IE Instance  *
    ***************************/
   SRemPreMsg(&tmpByte, mBuf);
   msgLen--;

   /********************************************************************
    * Get the instance from the tmp Byte as the 4th octet is Spare and *
    * Instance. Instance is present in 0,1,2,3 bits pos, 4th bit to    *
    * 7th bit are spares.                                              *
    ********************************************************************/
   tmpIe.ieInst = (tmpByte & EG_MASK_INST); /* Mask value is 0x0F */

   /*******************************************
    * get the IE entry from the IE Dictionary *
    ******************************************/
   ieEntry = egGetIeEntryFrmDb(tmpIe.ieType);

   if(!ieEntry)
   {
         EG_DBG_ERR(0,0, (egp,
               "egDecPthIe(IE(%d) does not exists in IE Db) \n", tmpIe.ieType));

         /*********************************************************
          * Report invalid length error in case invalid IE length *
          ********************************************************/
         if (tmpIeLen > msgLen)
         {
            /************************************
            * Invalid length, report error     *
            ************************************/
            retVal = EGT_ERR_IE_INV_LEN;

            EG_DBG_INFO(0,0, (egp,
                  "egDecIe(IE(%d), resultType(%d)) \n",
                  tmpIe.ieType,  retVal));
            EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
            EG_RETVALUE(retVal);
         }

         /***************************************************
          * skip this IE and continue further processing    *
          **************************************************/
         SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
         msgLen -= tmpIeLen;
         EG_RETVALUE(ROK);
   }
  
   /**********************************************************************
    * Check whether the IE 
    * skip the IE                                                        *
    *********************************************************************/
   if( (tmpIe.ieType == EG_IE_RECOVERY) && ( tmpIe.ieInst == EG_IE_INST_ZERO))
       
   {
      if( tmpIeLen < ieEntry->minLen || tmpIeLen > ieEntry->maxLen)
      {
         retVal =  EGT_ERR_IE_INV_LEN;
         EG_DBG_INFO(0,0, (egp,
               "egDecPthIe(IE(%d), resultType(%d)) \n",
               tmpIe.ieType,  retVal));
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 0,
                              evntMsg->u.egMsg->msgHdr.msgType);
         EG_RETVALUE(retVal);
      }
   }
#ifdef EG_REL_AC0
   else if( (tmpIe.ieType == EG_IE_NODE_FEATR) && ( tmpIe.ieInst == EG_IE_INST_ZERO))
       
   {
      if( tmpIeLen < ieEntry->minLen || tmpIeLen > ieEntry->maxLen)
      {
         retVal =  EGT_ERR_IE_INV_LEN;
         EG_DBG_INFO(0,0, (egp,
               "egDecPthIe(IE(%d), resultType(%d)) \n",
               tmpIe.ieType,  retVal));
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 0,
                              evntMsg->u.egMsg->msgHdr.msgType);
         EG_RETVALUE(retVal);
      }
   }
#endif
   else if(tmpIe.ieType == EG_IE_PVT_EXTN)
   {
      if( tmpIeLen < ieEntry->minLen || tmpIeLen > ieEntry->maxLen)
      {
         /*********************************************************
          * Report invalid length error in case invalid IE length *
          ********************************************************/
         if (tmpIeLen > msgLen)
         {
            /************************************
            * Invalid length, report error     *
            ************************************/
            retVal = EGT_ERR_IE_INV_LEN;

            EG_DBG_INFO(0,0, (egp,
                  "egDecIe(IE(%d), resultType(%d)) \n",
                  tmpIe.ieType,  retVal));
            EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
            EG_RETVALUE(retVal);
         }

         /***************************************************
          * skip this IE and continue further processing    *
          **************************************************/
         SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
         msgLen -= tmpIeLen;
         EG_RETVALUE(ROK);
      }
   }
   else
   {
      /*********************************************************
       * Report invalid length error in case invalid IE length *
       ********************************************************/
      if (tmpIeLen > msgLen)
      {
         /************************************
         * Invalid length, report error     *
         ************************************/
         retVal = EGT_ERR_IE_INV_LEN;

         EG_DBG_INFO(0,0, (egp,
               "egDecIe(IE(%d), resultType(%d)) \n",
               tmpIe.ieType,  retVal));
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 0,
                           evntMsg->u.egMsg->msgHdr.msgType);
         EG_RETVALUE(retVal);
      }

      /***************************************************
       * skip this IE and continue further processing    *
       **************************************************/
      SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
      msgLen -= tmpIeLen;
      EG_RETVALUE(ROK);
   }

   /********************************************
    * retreive the data type from the ie entry *
    *******************************************/
   tmpIe.dataType = ieEntry->dataType;

   /**********************
    * decode the IE Data *
    *********************/
   retVal = egDecIeData(evntMsg, mBuf, &tmpIe, tmpIeLen );

   /*********************************************************************
    * if decoding fails, check whether for this IE CR is set or not     *
    ********************************************************************/
   if(retVal != ROK)
   {
      if( tmpIe.ieType != EG_IE_RECOVERY) 
      {
         /*****************
         * skip the IE   *
         ****************/
         EG_DBG_INFO(0,0, (egp,
               "egDecPthIe(Optional IE(%d) not present in Msg) \n",
               tmpIe.ieType));
         retVal = ROK;
      }  
      else
      {
         EG_DBG_INFO(0,0, (egp,
                "egDecPthIe(Mandatory IE(%d), \
                 resultType(%d): data part is incorrect in Msg) \n",
                 tmpIe.ieType, retVal));

         /*************************************
         * raise an alarm and return FAILURE *
         ************************************/
         dgn.dgnVal[0].type = LEG_PAR_IECODE;
         egSendLmAlarm(LCM_CATEGORY_LYR_SPECIFIC,
                       LCM_EVENT_LYR_SPECIFIC,
                       LCM_CAUSE_DECODE_ERR, &dgn); 
  
         EG_FILL_ERR_EVNT_MSG(evntMsg, retVal, 
                              tmpIe.ieType, tmpIe.ieInst, 
                              evntMsg->u.egMsg->msgHdr.msgType); 
         EG_RETVALUE(retVal);
      }
   }
   retVal = EgUtilGmAppendIe(evntMsg->u.egMsg, ieLvlInfo, &tmpIe);   
   
   /***********************************************
    * This macro checks for the memory allocation *
    * Fails or not. If fails send the report to   *
    * LM and then returns RFAILED                 *
    **********************************************/
   EG_CHK_MEMFAIL(evntMsg, tmpIe, mBuf, retVal);

   EG_RETVALUE(retVal); 
}
/* Ie Data Encoding and decoding functions */
/***********************************************************************
 *   Fun:   egEncEG_UNSIGNED_8
 *   Desc:  Encodes the IE Data of type U8
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UNSIGNED_8
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UNSIGNED_8(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
  S16 retValue = 0; /* retrun value used for success or failure */
  /*-- eg003.201 : TRC changes from eg006.102--*/
  EG_TRC2(egEncEG_UNSIGNED_8)

  retValue= SAddPstMsg(egIe->t.valUnSgnd8, mBuf);
  RETVALUE(retValue);
} 
/***********************************************************************
 *   Fun:   egEncEG_UNSIGNED_16
 *   Desc:  Encodes the IE
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UNSIGNED_16
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UNSIGNED_16(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
    S16 retValue = 0; /* retrun value used for success or failure */
    /*-- eg003.201 : TRC changes from eg006.102--*/
    EG_TRC2(egEncEG_UNSIGNED_16)

    EG_ENC_U16(egIe->t.valUnSgnd16, mBuf);
    RETVALUE(retValue);
} 
/***********************************************************************
 *   Fun:   egEncEG_UNSIGNED_32
 *   Desc:  Encodes the IE data of type U32
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UNSIGNED_32
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UNSIGNED_32(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = 0; /* retrun value used for success or failure */
    /*-- eg003.201 : TRC changes from eg006.102--*/
    EG_TRC2(egEncEG_UNSIGNED_32)

   EG_ENC_U32(egIe->t.valUnSgnd32, mBuf);
   RETVALUE(retValue);
}
/***********************************************************************
 *   Fun:   egEncEG_STRING_8
 *   Desc:  Encodes the IE Data type STRING_8
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16  egEncEG_STRING_8
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16  egEncEG_STRING_8(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
    /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egEncEG_STRING_8)

   EG_ENC_STR(mBuf, egIe->t.valStr8.val, egIe->t.valStr8.length, EG_LEN_EIGHT, retVal);
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_STRING_16
 *   Desc:  Encodes the IE Data type STRING_16
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_STRING_16
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_STRING_16(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
    /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egEncEG_STRING_16)

   EG_ENC_STR(mBuf, egIe->t.valStr16.val, egIe->t.valStr16.length, EG_LEN_SIXTEEN, retVal);
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_STRING_32
 *   Desc:  Encodes the IE data type STRING_32
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_STRING_32
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_STRING_32(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
  S16 retVal = ROK; /* Return value used to know the successful parsing */
    /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egEncEG_STRING_32)

  EG_ENC_STR(mBuf, egIe->t.valStr32.val, egIe->t.valStr32.length, EG_LEN_32, retVal);
  RETVALUE(retVal);
} 
/***********************************************************************
 *   Fun:   egEncEG_STRING_132
 *   Desc:  Encodes the IE DATA Of type STRING_132
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_STRING_132
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_STRING_132(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_ENC_STR(mBuf, egIe->t.valStr132.val, egIe->t.valStr132.length, EG_LEN_132, retVal);
   RETVALUE(retVal);
} 

/***********************************************************************
 *   Fun:   egEncEG_STRING_400
 *   Desc:  Encodes the IE DATA Of type STRING_400
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_STRING_400
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_STRING_400(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_ENC_STR(mBuf, egIe->t.valStr400.val, egIe->t.valStr400.length, EG_LEN_400, retVal);
   RETVALUE(retVal);
} 

/***********************************************************************
 *   Fun:   egEncEG_STRING_MAX
 *   Desc:  Encodes the IE data type STRING_MAX
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_STRING_MAX
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_STRING_MAX(egIe, mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_ENC_STR(mBuf, egIe->t.valStr.val, egIe->t.valStr.length, EG_MAX_STR_LEN, retVal);
   RETVALUE(retVal);
} 
/***********************************************************************
 *   Fun:   egEncEG_SNETWORK
 *   Desc:  Encodes the IE Data type SNETWORK
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_SNETWORK
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_SNETWORK(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_ENC_NETID(egIe->t.sNwk,mBuf);
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_INDICATION
 *   Desc:  Encodes the IE Data type INDICATION
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_INDICATION
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_INDICATION(egIe, mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   Data     dumBytes[EG_MAX_BYTES]; /* Holds the Data for decoding */
   dumBytes[0]  = ((egIe->t.indication.isSGWCIPres * EG_MASK_BIT1) |
                    (egIe->t.indication.isISRAIPres * EG_MASK_BIT2) |
                    (egIe->t.indication.isISRSIPres * EG_MASK_BIT3) |
                    (egIe->t.indication.isOIPres * EG_MASK_BIT4)    |
                    (egIe->t.indication.isDFIPres * EG_MASK_BIT5)   |
                    (egIe->t.indication.isHIPres * EG_MASK_BIT6)    |
                    (egIe->t.indication.isDTFPres * EG_MASK_BIT7)   |
                    (egIe->t.indication.isDAFPres * EG_MASK_BIT8));

   dumBytes[1]  = ((egIe->t.indication.isMSVPres * EG_MASK_BIT1)   |
                    (egIe->t.indication.isSIPres * EG_MASK_BIT2)    |
                    (egIe->t.indication.isPTPres * EG_MASK_BIT3)   |
                    (egIe->t.indication.isPPres * EG_MASK_BIT4)   |
                    (egIe->t.indication.isCRSIPres * EG_MASK_BIT5)   |
                    (egIe->t.indication.isCFSIPres * EG_MASK_BIT6));
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   dumBytes[1] = (dumBytes[1] | (egIe->t.indication.isUIMSIPres * EG_MASK_BIT7));
#endif

#ifdef EG_REL_AC0
   dumBytes[1] = (dumBytes[1] | (egIe->t.indication.isSQCIPres * EG_MASK_BIT8));
   dumBytes[2]  = ((egIe->t.indication.isCCRSIPres * EG_MASK_BIT1)   |
                    (egIe->t.indication.isISRAUPres * EG_MASK_BIT2)    |
                    (egIe->t.indication.isMBMDTPres * EG_MASK_BIT3)   |
                    (egIe->t.indication.isS4AFPres * EG_MASK_BIT4)   |
                    (egIe->t.indication.isS6AFPres * EG_MASK_BIT5));
#endif

#ifndef EG_REL_AC0
   retVal= SAddPstMsgMult(dumBytes, EG_LEN_TWO, mBuf);
#else
   retVal= SAddPstMsgMult(dumBytes, EG_LEN_THREE, mBuf);
#endif
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egEncEG_PVT_EXTN
 *   Desc:  Encodes the IE data type PVT EXTN
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_PVT_EXTN
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_PVT_EXTN(egIe, mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_ENC_U16(egIe->t.pvtExt.entId, mBuf);

   /************************************************
    * For encoding the remaining bytes to prop val *
    ***********************************************/
   if(egIe->t.pvtExt.propVal.length)
   {
   EG_ENC_STR( mBuf, egIe->t.pvtExt.propVal.val,
                egIe->t.pvtExt.propVal.length, EG_LEN_132, retVal);
   }

   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_CAUSE
 *   Desc:  Encodes the IE Data type and offedning IE info 
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_CAUSE
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_CAUSE(egIe, mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16  retVal = ROK; /* Holds the IE parsing result   */
   U8   pceBceCs = 0;

    /* Encode the cause value */
    SAddPstMsg(egIe->t.cause.causeVal, mBuf);

   /* Encode the cause source,bce & pce bits */
   pceBceCs = (egIe->t.cause.ieCS * EG_MASK_BIT1);
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   pceBceCs = (pceBceCs | (egIe->t.cause.bce * EG_MASK_BIT2));
   pceBceCs = (pceBceCs | (egIe->t.cause.pce * EG_MASK_BIT3));
#endif
   SAddPstMsg(pceBceCs, mBuf);

   /* ccpu00135385 : offending IE is not mandatory */
   if (egIe->t.cause.ieType != EG_ZERO)
   {
    /* Encode the offending IE Type */
    SAddPstMsg(egIe->t.cause.ieType, mBuf);

    /* Encode the offending IE length */
    EG_ENC_U16(egIe->t.cause.ieLength, mBuf);

    /* Encode the offending IE instance */
    SAddPstMsg(egIe->t.cause.ieInst & 0x0F, mBuf);
   }

   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egEncEG_TRC_INFO
 *   Desc:  Encodes the IE data type TRC INFO
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_TRC_INFO
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_TRC_INFO(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */

   EG_ENC_NETID(egIe->t.trcInfo.sNwk,mBuf);
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
   EG_ENC_U24(egIe->t.trcInfo.trcId, mBuf);
   SAddPstMsgMult(egIe->t.trcInfo.trigEvnts,EG_LEN_NINE, mBuf);
   EG_ENC_U16(egIe->t.trcInfo.lstNETypes, mBuf);
   EG_ENC_U8(egIe->t.trcInfo.sesnTrcDep, mBuf);
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
   SAddPstMsgMult(egIe->t.trcInfo.lstOfIntfs,EG_LEN_TWELVE, mBuf);

   switch (egIe->t.trcInfo.trcColEntAddr.type)
   {
     case CM_TPTADDR_IPV4:
       {
         EG_ENC_U32(egIe->t.trcInfo.trcColEntAddr.u.ipv4, mBuf);
         break;
       } /* end of case */
#ifdef EG_IPV6_SUPPORTED
     case CM_TPTADDR_IPV6:
       {
         SAddPstMsgMult(egIe->t.trcInfo.trcColEntAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         break;
       } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
     default:
       {
          RETVALUE(RFAILED);
       } /* end of case */
   }
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_BEARER_FLAG
 *   Desc:  Encodes the IE data type BEARER FLAG
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_BEARER_FLAG
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_BEARER_FLAG(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
  S16  retVal = ROK; /* Return value used to know the successful parsing */
  U8  tmpByte = 0;      /* Holds 1 byte of data for decoding  */

  tmpByte =  ((egIe->t.bearFlags.isPPCPres * EG_MASK_BIT1) | 
              (egIe->t.bearFlags.isVBPres * EG_MASK_BIT2) 
#ifdef EG_REL_AC0
         | (egIe->t.bearFlags.isASIPres * EG_MASK_BIT4) /* Added as part of Release 10 */
#endif
     );
  SAddPstMsg(tmpByte, mBuf);

  RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_FTEID
 *   Desc:  Encodes the IE data type FTEID
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_FTEID
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_FTEID(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16  retVal = ROK; /* Return value used to know the successful parsing */

   /***************************************
    * F-TEID  Ie is having the fields:    *
    * one octet: 1-5bits intfType         *
    * one octet: 6th bit EBIF pres        *
    * one octet: 7th bit V6 pres          *
    * one octet: 8th bit V4 pres          *
    * four octets of TEID value           *
    * if 7th bit is set then ipv6 addr    *
    * else 8th bit is set then ipv4 addr  *
    **************************************/
   
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
#if defined(EG_S2A_SUPP)
   egIe->t.fTeid.intfType &= EG_MASK_FTEID_INTFTYPE_REL11;
#else
#if defined(EG_REL_AC0)
   egIe->t.fTeid.intfType &= EG_MASK_FTEID_INTFTYPE_REL10; /* added for release 10 */
#else
   egIe->t.fTeid.intfType &= EG_MASK_FTEID_INTFTYPE;
#endif
#endif
   /* ccpu00137940 : Both IPv6 & IPv4 Addr has to be encoded */
   if(egIe->t.fTeid.isIPv4Pres)
   {
      egIe->t.fTeid.intfType |= EG_MASK_BIT8;
   }
   if(egIe->t.fTeid.isIPv6Pres)
   {
      egIe->t.fTeid.intfType |= EG_MASK_BIT7;
   }
   SAddPstMsg(egIe->t.fTeid.intfType, mBuf);

   EG_ENC_U32(egIe->t.fTeid.teid, mBuf);
   if(egIe->t.fTeid.isIPv4Pres)
   {
     /***************************
      * Encode the IPV4 address *
      **************************/
     SAddPstMsgMult(egIe->t.fTeid.ip4Addr, EG_LEN_FOUR, mBuf);
   }
   /* ccpu00137940 */
   if (egIe->t.fTeid.isIPv6Pres)
   {
     /***************************
      * Encode the IPV6 address *
      **************************/
     SAddPstMsgMult(egIe->t.fTeid.ipv6Address,
         EG_LEN_SIXTEEN, mBuf);
   }

   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_ULI_DATA
 *   Desc:  Encodes the IE Data type ULI
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_ULI_DATA
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_ULI_DATA(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16  retVal = ROK; /* Return value used to know the successful parsing */
    U8  tmpByte;      /* Holds 1 byte of data for decoding  */

   /*********************************************
    * Prepare the first byte which tells        *
    * the presence of the CGI, SAI,RAI,TAI,ECGI *
    *********************************************/
    tmpByte =((egIe->t.uli.isCGIPres * EG_MASK_BIT1) |
              (egIe->t.uli.isSAIPres * EG_MASK_BIT2)  |
              (egIe->t.uli.isRAIPres * EG_MASK_BIT3)  |
              (egIe->t.uli.isTAIPres * EG_MASK_BIT4)  |
              (egIe->t.uli.isECGIPres * EG_MASK_BIT5) 
#ifdef EG_REL_AC0
         | (egIe->t.uli.isLAIPres * EG_MASK_BIT6) /* added as per release 10 */
#endif
        );
    SAddPstMsg(tmpByte,mBuf);

   /************************************
    * Check if the CGI presence is set *
    ***********************************/
   if(egIe->t.uli.isCGIPres)
   {
      /************************************
       * Encode the MCC,MNC,LAC,CI fields *
       ***********************************/
      EG_ENC_NETID(egIe->t.uli.cgi.sNwk, mBuf);

      EG_ENC_U16(egIe->t.uli.cgi.lac, mBuf);
      EG_ENC_U16(egIe->t.uli.cgi.ci, mBuf);
   }

  /************************************
    * Check if the SAI presence is set *
    ***********************************/
   if(egIe->t.uli.isSAIPres)
   {
      /*************************************
       * Encode the MCC,MNC,LAC,SAC fields *
       *************************************/
       EG_ENC_NETID(egIe->t.uli.sai.sNwk, mBuf);
       EG_ENC_U16(egIe->t.uli.sai.lac, mBuf);
       EG_ENC_U16(egIe->t.uli.sai.sac, mBuf);
   }

   /************************************
    * Check if the RAI presence is set *
    ***********************************/
   if(egIe->t.uli.isRAIPres)
   {
      /*************************************
       * Encode the MCC,MNC,LAC,RAC fields *
       ************************************/
      EG_ENC_NETID(egIe->t.uli.rai.sNwk, mBuf);
      EG_ENC_U16(egIe->t.uli.rai.lac, mBuf);
      EG_ENC_U16(egIe->t.uli.rai.rac, mBuf);
   }

   /************************************
    * Check if the TAI presence is set *
    ***********************************/
  if(egIe->t.uli.isTAIPres)
   {
      /*************************************
       * Encode the MCC,MNC,LAC,TAC fields *
       *************************************/
       EG_ENC_NETID(egIe->t.uli.tai.sNwk, mBuf);
       EG_ENC_U16(egIe->t.uli.tai.tac, mBuf);
   }

   /*************************************
    * Check if the ECGI presence is set *
    ************************************/
   if(egIe->t.uli.isECGIPres)
   {
      /**************************************
       * Encode the MCC,MNC,LAC,ECGI fields *
       **************************************/
       EG_ENC_NETID(egIe->t.uli.ecgi.sNwk, mBuf);

      /****************************************
       * Mask the high 4bits of the ECI value *
       * And then Encode the ECI value        *
       ****************************************/
       egIe->t.uli.ecgi.ecId &= EG_MASK_ECI;
       EG_ENC_U32(egIe->t.uli.ecgi.ecId, mBuf);

   }
#ifdef EG_REL_AC0
   /************************************
    * Check if the LAI presence is set * 
    ***********************************/
  if(egIe->t.uli.isLAIPres)
   {
      /*************************************
       * Encode the MCC,MNC,LAC,TAC fields *
       *************************************/
       EG_ENC_NETID(egIe->t.uli.lai.sNwk, mBuf);
       EG_ENC_U16(egIe->t.uli.lai.lac, mBuf);
   }
#endif
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_FQOS
 *   Desc:  Encodes the IE Data type FQOS
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_FQOS
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_FQOS(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */

   EG_ENC_FQOS(egIe->t.fQos, mBuf);
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egEncEG_BQOS_IEDATA
 *   Desc:  Encodes the IE data type BQOS
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_BQOS_IEDATA
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_BQOS_IEDATA(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
  S16 retVal = ROK; /* Return value used to know the successful parsing */
  U8  tmpByte = 0;

  tmpByte =  egIe->t.bQos.prityLvl << EG_SHIFT_2;
  tmpByte |= (egIe->t.bQos.isPVIPres * EG_MASK_BIT1);
  /* ccpu00137026 : PCI updated */
  tmpByte |= (egIe->t.bQos.isPCIPres * EG_MASK_BIT7);

  SAddPstMsg(tmpByte,mBuf);
  SAddPstMsg(egIe->t.bQos.qci,mBuf);
  SAddPstMsgMult(egIe->t.bQos.upMBR.val, EG_LEN_FIVE, mBuf);
  SAddPstMsgMult(egIe->t.bQos.dnMBR.val, EG_LEN_FIVE, mBuf);
  SAddPstMsgMult(egIe->t.bQos.upGBR.val, EG_LEN_FIVE, mBuf);
  SAddPstMsgMult(egIe->t.bQos.dnGBR.val, EG_LEN_FIVE, mBuf);

  RETVALUE(retVal);
} 
/***********************************************************************
 *   Fun:   egEncEG_PAA_IEDATA
 *   Desc:  Encodes the IE data type PAA
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_PAA_IEDATA
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_PAA_IEDATA(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
    S16 retVal = ROK; /* Return value used to know the successful parsing */

    egIe->t.paa.pdnType &= EG_MASK_LOW3;
    SAddPstMsg(egIe->t.paa.pdnType, mBuf);
    switch(egIe->t.paa.pdnType)
    {
       /*-- eg006.201: Replace hard coded numeral with their respective Macros --*/
        case EG_PAA_IPV4 :
        {
           /**************************************************************
            * if the PDN type is 1, then the pdn address is of type Ipv4 *
            * decode the ipv4 address which is of 4 octets length        *
            **************************************************************/
            SAddPstMsgMult(egIe->t.paa.u.pdnIpv4Addr, EG_LEN_FOUR, mBuf);
            break;
        }
       /*-- eg006.201: Replace hard coded numeral with their respective Macros --*/
        case EG_PAA_IPV6 :
        {
           /***********************************************************
            * If the pdnType is 2, then first octet will tell the pdn *
            * prifix next 16octets will tell the ipv6 address         *
            ***********************************************************/
            SAddPstMsg(egIe->t.paa.pdnPrefix,mBuf);
            SAddPstMsgMult(egIe->t.paa.u.pdnIpv6Addr, EG_LEN_SIXTEEN, mBuf);
            break;
        }
       /*-- eg006.201: Replace hard coded numeral with their respective Macros --*/
        case EG_PAA_BOTH :
        {
           /************************************************************
            * If the pdnType is 3, then first octet will tell the pdn  *
            * prifix next 16octets will be the ipv6 address next 4     *
            * octets will be the ipv4 address                          *
            ************************************************************/
           SAddPstMsg(egIe->t.paa.pdnPrefix,mBuf);
           SAddPstMsgMult(egIe->t.paa.u.pdnBoth.pdnIpv6Addr,
                           EG_LEN_SIXTEEN, mBuf);
           SAddPstMsgMult(egIe->t.paa.u.pdnBoth.pdnIpv4Addr, EG_LEN_FOUR, mBuf);
           break;
        }
        default:
        {
          /******************
           * Invalid length *
           ******************/
           retVal = EGT_ERR_IE_INV_LEN;
           break;
        }
     }
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egEncEG_AUTH_QUIN
 *   Desc:  Encodes the IE data type Authentication Quintuplet
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_AUTH_QUIN
(
EgIe       *egIe,
Buffer     *mBuf
)
#else
PRIVATE S16 egEncEG_AUTH_QUIN(egIe, mBuf)
EgIe       *egIe;
Buffer     *mBuf;
#endif
{
   S16 retValue = ROK;

   /* mandate the length as 16 octets */
   egIe->t.authQt.rand.length = EG_LEN_SIXTEEN;

   /* encode the RAND which is of 16 octets*/
   EG_ENC_STR(mBuf, egIe->t.authQt.rand.val, egIe->t.authQt.rand.length, EG_LEN_SIXTEEN, retValue);
   if( retValue != ROK)
   {
      RETVALUE(retValue);
   }

   /* encode the XRES length in single octet and XRES which can be of 16 octets */
   SAddPstMsg(egIe->t.authQt.xresLen, mBuf);
   EG_ENC_STR(mBuf, egIe->t.authQt.xres.val, egIe->t.authQt.xresLen, EG_LEN_SIXTEEN, retValue);
   if( retValue != ROK)
   {
      RETVALUE(retValue);
   }

   /* mandate the length as 16 octets */
   egIe->t.authQt.ck.length = EG_LEN_SIXTEEN;

   /* encode the CK, IK each is of 16 octets length */
   EG_ENC_STR(mBuf, egIe->t.authQt.ck.val, egIe->t.authQt.ck.length, EG_LEN_SIXTEEN, retValue);
   if( retValue != ROK)
   {
      RETVALUE(retValue);
   }

   /* mandate the length as 16 octets */
   egIe->t.authQt.ik.length = EG_LEN_SIXTEEN;

   EG_ENC_STR(mBuf, egIe->t.authQt.ik.val, egIe->t.authQt.ik.length, EG_LEN_SIXTEEN, retValue);
   if( retValue != ROK)
   {
      RETVALUE(retValue);
   }

   /* encode the AUTN */
   SAddPstMsg(egIe->t.authQt.autnLen, mBuf);
   EG_ENC_STR(mBuf, egIe->t.authQt.autn.val, egIe->t.authQt.autn.length, EG_LEN_SIXTEEN, retValue);
   if( retValue != ROK)
   {
      RETVALUE(retValue);
   }

   RETVALUE(ROK);
} /* end of egEncEG_AUTH_QUIN */


/***********************************************************************
 *   Fun:   egEncEG_AUTH_QUAD
 *   Desc:  Encodes the IE data type Authentication Quadruplet
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_AUTH_QUAD
(
EgIe               *egIe,
Buffer             *mBuf
)
#else
PRIVATE S16 egEncEG_AUTH_QUAD(egIe, mBuf)
EgIe               *egIe;
Buffer             *mBuf;
#endif
{
   S16 retValue = ROK;

   /* mandate the length as 16 octets */
   egIe->t.quadruplet.rand.length = EG_LEN_SIXTEEN;

   /* encode the RAND which is of 16 octets*/
   EG_ENC_STR(mBuf, egIe->t.quadruplet.rand.val, egIe->t.quadruplet.rand.length, EG_LEN_SIXTEEN, retValue);

   /* encode the XRES length in single octet and XRES which can be of 16 octets */
   SAddPstMsg(egIe->t.quadruplet.xresLen, mBuf);
   EG_ENC_STR(mBuf, egIe->t.quadruplet.xres.val, egIe->t.quadruplet.xresLen, EG_LEN_SIXTEEN, retValue);

   /* encode the AUTN length in single octet and AUTH which can be of 16 octets */
   SAddPstMsg(egIe->t.quadruplet.autnLen, mBuf);
   EG_ENC_STR(mBuf, egIe->t.quadruplet.autn.val, egIe->t.quadruplet.autnLen, EG_LEN_SIXTEEN, retValue);
   /* encode the Kasme which is of 32 octets */
   egIe->t.quadruplet.kasme.length = EG_KASME_LEN;
   EG_ENC_STR(mBuf, egIe->t.quadruplet.kasme.val, egIe->t.quadruplet.kasme.length, EG_LEN_32, retValue);

   RETVALUE(ROK);
} /* end of egEncEG_AUTH_QUAD */


/***********************************************************************
 *   Fun:   egEncEG_CMP_REQ
 *   Desc:  Encodes the IE data type Complete Request Message
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_CMP_REQ
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_CMP_REQ(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode the CRM Type */
   SAddPstMsg(egIe->t.crm.crmType, mBuf);

   /* encode the CRM Value */
   EG_ENC_STR(mBuf, egIe->t.crm.crm.val, egIe->t.crm.crm.length, EG_LEN_400, retValue);

   RETVALUE(retValue);
} /* end of egEncEG_CMP_REQ */


/***********************************************************************
 *   Fun:   egEncEG_GUTI
 *   Desc:  Encodes the IE data type GUTI
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_GUTI
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_GUTI(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   /* encode the NETID */
   EG_ENC_NETID(egIe->t.guti.sNwk,mBuf);
   EG_ENC_U16(egIe->t.guti.mmeGrpId, mBuf);
   SAddPstMsg(egIe->t.guti.mmeCode, mBuf);
   EG_ENC_U32(egIe->t.guti.mTmsi, mBuf);

   RETVALUE(ROK);
} /* end of egEncEG_GUTI */


/***********************************************************************
 *   Fun:   egEncEG_F_CONT
 *   Desc:  Encodes the IE data type F-Container
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_F_CONT
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_F_CONT(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode the container type - mask the msb 4 bits */
   SAddPstMsg(egIe->t.fContainer.containerType & 0x0F, mBuf);
   EG_ENC_STR(mBuf, egIe->t.fContainer.fContainer.val, egIe->t.fContainer.fContainer.length, EG_LEN_400, retValue);

   RETVALUE(retValue);
} /* end of egEncEG_F_CONT */


/***********************************************************************
 *   Fun:   egEncEG_TRGTID
 *   Desc:  Encodes the IE data type Target Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_TRGTID
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_TRGTID(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode the target type */
   SAddPstMsg(egIe->t.targetId.targetType, mBuf);
   EG_ENC_STR(mBuf, egIe->t.targetId.targetId.val, egIe->t.targetId.targetId.length, EG_LEN_SIXTEEN, retValue);

   RETVALUE(retValue);
} /* end of egEncEG_TRGTID */


/***********************************************************************
 *   Fun:   egEncEG_PKTFLOWID
 *   Desc:  Encodes the IE data type Packet Flow Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_PKTFLOWID
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_PKTFLOWID(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   /* encode the ebi - mask the msb 4 bits */
   SAddPstMsg((egIe->t.pktFlowId.ebi & 0x0F), mBuf);
   SAddPstMsg(egIe->t.pktFlowId.pktFlowId, mBuf);

   RETVALUE(ROK);
} /* end of egEncEG_PKTFLOWID */


/***********************************************************************
 *   Fun:   egEncEG_SRCID
 *   Desc:  Encodes the IE data type Cell Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_SRCID
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_SRCID(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   EG_ENC_STR(mBuf, egIe->t.srcId.targetCellId.val, egIe->t.srcId.targetCellId.length, EG_LEN_EIGHT, retValue);
   SAddPstMsg(egIe->t.srcId.sourceType, mBuf);
   EG_ENC_STR(mBuf, egIe->t.srcId.sourceId.val, egIe->t.srcId.sourceId.length, EG_LEN_SIXTEEN, retValue);

   RETVALUE(retValue);
} /* end of egEncEG_SRCID */

/***********************************************************************
 *   Fun:   egEncEG_GLOBAL_CNID
 *   Desc:  Encodes the IE data type Global CN Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_GLOBAL_CNID
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_GLOBAL_CNID(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   EG_ENC_NETID(egIe->t.gblCnId.sNwk,mBuf);
   EG_ENC_STR(mBuf, egIe->t.gblCnId.cnId.val, egIe->t.gblCnId.cnId.length, EG_LEN_FOUR, retValue);

   RETVALUE(retValue);
} /* end of egEncEG_GLOBAL_CNID */


/***********************************************************************
 *   Fun:   egEncEG_PDN_CONN_SET_ID
 *   Desc:  Encodes the IE data type PDN Connection Set Identifier
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_PDN_CONN_SET_ID
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_PDN_CONN_SET_ID(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   /*ccpu00137353 */
   S16 idx=0;
   U8  tmpByte = 0;

   egIe->t.csid.nodeIdType = ((egIe->t.csid.nodeIdType << EG_SHIFT_4) & 0xF0);
   tmpByte = (egIe->t.csid.nodeIdType) | (egIe->t.csid.noOfCSIDs & 0x0F);

   SAddPstMsg(tmpByte , mBuf);

   EG_ENC_STR(mBuf, egIe->t.csid.nodeId.val, egIe->t.csid.nodeId.length, EG_LEN_SIXTEEN, retValue);
   for(idx=0; idx < egIe->t.csid.noOfCSIDs; idx++)
   {   
      EG_ENC_STR(mBuf, egIe->t.csid.csid[idx].val, egIe->t.csid.csid[idx].length, EG_LEN_SIXTEEN, retValue);
   }   

   RETVALUE(retValue);
} /* end of egEncEG_PDN_CONN_SET_ID */


/***********************************************************************
 *   Fun:   egEncEG_IP_ADDR
 *   Desc:  Encodes the IE data type IP Address
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_IP_ADDR
(
EgIe       *egIe,
Buffer     *mBuf
)
#else
PRIVATE S16 egEncEG_IP_ADDR(egIe, mBuf)
EgIe       *egIe;
Buffer     *mBuf;
#endif
{
   switch (egIe->t.ipAddr.type)
   {
      case CM_TPTADDR_IPV4:
      {
         EG_ENC_U32(egIe->t.ipAddr.u.ipv4, mBuf);
         break;
      } /* end of case */
#ifdef EG_IPV6_SUPPORTED
      case CM_TPTADDR_IPV6:
      {
         SAddPstMsgMult(egIe->t.ipAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         break;
      } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
      default:
      {
         RETVALUE(RFAILED);
      } /* end of case */
   }

   RETVALUE(ROK);
} /* end of egEncEG_IP_ADDR */


/***********************************************************************
 *   Fun:   egEncEG_GSM_KEY_TRIP
 *   Desc:  Encodes the IE data type GSM Key Triplet
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_GSM_KEY_TRIP
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_GSM_KEY_TRIP(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 cnt = 0;
   S16 retValue = ROK;
   U8  tmpByte = 0;

   tmpByte = (((egIe->t.gsmKeyT.secMode << EG_SHIFT_5)&0xE0) | 
             (egIe->t.gsmKeyT.isDRXIPres * EG_MASK_BIT4) | (egIe->t.gsmKeyT.cksn & 0x07));
   SAddPstMsg(tmpByte , mBuf);

   tmpByte = 0;
   tmpByte = (egIe->t.gsmKeyT.nmbTrip << EG_SHIFT_5) & 0xE0;
   tmpByte |= (egIe->t.gsmKeyT.isUAMBRIPres * EG_MASK_BIT2);
   tmpByte |= (egIe->t.gsmKeyT.isSAMBRIPres * EG_MASK_BIT1);
   SAddPstMsg(tmpByte , mBuf);

   SAddPstMsg( (egIe->t.gsmKeyT.usedCipher & 0x07), mBuf);

   /* encode Kc in eight octets */
   SAddPstMsgMult(egIe->t.gsmKeyT.kc, EG_LEN_EIGHT, mBuf);

   for (cnt = 0; cnt < egIe->t.gsmKeyT.nmbTrip; cnt++)
   {
     SAddPstMsgMult(egIe->t.gsmKeyT.authTrip[cnt].rand, 16, mBuf);
     SAddPstMsgMult(egIe->t.gsmKeyT.authTrip[cnt].sres, 4, mBuf);
     SAddPstMsgMult(egIe->t.gsmKeyT.authTrip[cnt].kc, 8, mBuf);
   } /* end of loop */

   if(egIe->t.gsmKeyT.isDRXIPres)
   {
   /* encode drx parameter - is of 5 octets */
   SAddPstMsgMult(egIe->t.gsmKeyT.drxParam, EG_LEN_TWO, mBuf);
   }

   if(egIe->t.gsmKeyT.isSAMBRIPres)
   {
   /* encode upLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyT.upSubsUEAMBR, mBuf);

   /* encode downLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyT.dnSubsUEAMBR, mBuf);
   }

   if(egIe->t.gsmKeyT.isUAMBRIPres)
   {
   /* encode upLink used UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyT.upUsedUEAMBR, mBuf);

   /* encode downLink used UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyT.dnUsedUEAMBR, mBuf);
   }

   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   SAddPstMsg(egIe->t.gsmKeyT.ueNtwrkCapb.length , mBuf);
   /* encode UE Network Capability - can be max of 15 octets */
   EG_ENC_STR(mBuf, egIe->t.gsmKeyT.ueNtwrkCapb.val, egIe->t.gsmKeyT.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

  /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   SAddPstMsg(egIe->t.gsmKeyT.msNtwrkCapb.length , mBuf);
   /* encode MS Network Capability - can be max of 10 octets */
   EG_ENC_STR(mBuf, egIe->t.gsmKeyT.msNtwrkCapb.val, egIe->t.gsmKeyT.msNtwrkCapb.length,EG_LEN_SIXTEEN, retValue);

   /* here we are mandating the length of MEID will be of 16 octets */
   SAddPstMsg(egIe->t.gsmKeyT.meId.length , mBuf);
   /* encode MEID - is of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.gsmKeyT.meId.val, egIe->t.gsmKeyT.meId.length, EG_LEN_SIXTEEN, retValue);

   tmpByte = 0;

   tmpByte = egIe->t.gsmKeyT.isUNA * EG_MASK_BIT1;
   tmpByte |= egIe->t.gsmKeyT.isGENA * EG_MASK_BIT2;
   tmpByte |= egIe->t.gsmKeyT.isGANA * EG_MASK_BIT3;
   tmpByte |= egIe->t.gsmKeyT.isINA * EG_MASK_BIT4;
   tmpByte |= egIe->t.gsmKeyT.isENA * EG_MASK_BIT5;
   tmpByte |= egIe->t.gsmKeyT.isHNNA * EG_MASK_BIT6;

   SAddPstMsg(tmpByte , mBuf);
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* encode Voice Domain Preference and UE's usage Setting length */
   SAddPstMsg( egIe->t.gsmKeyT.vDomPrefUEUsageLen, mBuf);
   
   /* encode UE's usage setting (1 bit field) Bit-3
    * encode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.gsmKeyT.vDomPrefUEUsageLen)
   {
     tmpByte = 0;
     tmpByte = egIe->t.gsmKeyT.ueUsageSett * EG_MASK_BIT1;
     tmpByte <<= EG_SHIFT_2;
     tmpByte |= egIe->t.gsmKeyT.vDomPref &  0x03;

     SAddPstMsg(tmpByte , mBuf);
   }
#endif /* EG_REL_930 */


   RETVALUE(retValue);
} /* end of egEncEG_GSM_KEY_TRIP */


/***********************************************************************
 *   Fun:   egEncEG_UMTS_KEY_CIPH_QUIN
 *   Desc:  Encodes the IE data type UMTS Key, Used Cipher and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UMTS_KEY_CIPH_QUIN
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UMTS_KEY_CIPH_QUIN(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 cnt = 0;
   S16 retValue = ROK;
   U8  tmpByte = 0;

   tmpByte = (((egIe->t.umtsKeyUCQt.secMode << EG_SHIFT_5)&0xE0) | 
             (egIe->t.umtsKeyUCQt.isDRXIPres * EG_MASK_BIT4) | (egIe->t.umtsKeyUCQt.cksn & 0x07));
   SAddPstMsg(tmpByte , mBuf);

   tmpByte = 0;
   tmpByte = (egIe->t.umtsKeyUCQt.nmbQuin << EG_SHIFT_5) & 0xE0;
   tmpByte |= (egIe->t.umtsKeyUCQt.isUAMBRIPres * EG_MASK_BIT2);
   tmpByte |= (egIe->t.umtsKeyUCQt.isSAMBRIPres * EG_MASK_BIT1);
   SAddPstMsg(tmpByte , mBuf);

   SAddPstMsg( (egIe->t.umtsKeyUCQt.usedCipher & 0x07), mBuf);
   /* mandate the length as 16 octets */
   egIe->t.umtsKeyUCQt.ck.length = EG_LEN_SIXTEEN;
   egIe->t.umtsKeyUCQt.ik.length = EG_LEN_SIXTEEN;

   /* encode CK, IK each of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.ck.val, egIe->t.umtsKeyUCQt.ck.length, EG_LEN_SIXTEEN, retValue);
   EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.ik.val, egIe->t.umtsKeyUCQt.ik.length, EG_LEN_SIXTEEN, retValue);
   for(cnt = 0;cnt < egIe->t.umtsKeyUCQt.nmbQuin;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.umtsKeyUCQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].rand.val, egIe->t.umtsKeyUCQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.umtsKeyUCQt.authQuin[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].xres.val, egIe->t.umtsKeyUCQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* mandate the length as 16 octets */
     egIe->t.umtsKeyUCQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.umtsKeyUCQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;

     /* encode the CK, IK each is of 16 octets length */
     EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].ck.val, egIe->t.umtsKeyUCQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].ik.val, egIe->t.umtsKeyUCQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the AUTN */
     SAddPstMsg(egIe->t.umtsKeyUCQt.authQuin[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].autn.val, egIe->t.umtsKeyUCQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }
   }

   if(egIe->t.umtsKeyUCQt.isDRXIPres)
   {
   /* encode drx parameter - is of 2 octets */
   SAddPstMsgMult(egIe->t.umtsKeyUCQt.drxParam, EG_LEN_TWO, mBuf);
   }

   if(egIe->t.umtsKeyUCQt.isSAMBRIPres)
   {
   /* encode upLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyUCQt.upSubsUEAMBR, mBuf);

   /* encode downLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyUCQt.dnSubsUEAMBR, mBuf);
   }

   if(egIe->t.umtsKeyUCQt.isUAMBRIPres)
   {
   /* encode upLink used UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyUCQt.upUsedUEAMBR, mBuf);

   /* encode downLink used UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyUCQt.dnUsedUEAMBR, mBuf);
   }

   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   SAddPstMsg(egIe->t.umtsKeyUCQt.ueNtwrkCapb.length , mBuf);
   /* encode UE Network Capability - can be max of 15 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.ueNtwrkCapb.val, egIe->t.umtsKeyUCQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

  /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   SAddPstMsg(egIe->t.umtsKeyUCQt.msNtwrkCapb.length , mBuf);
   /* encode MS Network Capability - can be max of 10 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.msNtwrkCapb.val, egIe->t.umtsKeyUCQt.msNtwrkCapb.length,EG_LEN_SIXTEEN, retValue);

   /* here we are mandating the length of MEID will be of 16 octets */
   SAddPstMsg(egIe->t.umtsKeyUCQt.meId.length , mBuf);
   /* encode MEID - is of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyUCQt.meId.val, egIe->t.umtsKeyUCQt.meId.length, EG_LEN_SIXTEEN, retValue);

   tmpByte = 0;

   tmpByte = egIe->t.umtsKeyUCQt.isUNA * EG_MASK_BIT1;
   tmpByte |= egIe->t.umtsKeyUCQt.isGENA * EG_MASK_BIT2;
   tmpByte |= egIe->t.umtsKeyUCQt.isGANA * EG_MASK_BIT3;
   tmpByte |= egIe->t.umtsKeyUCQt.isINA * EG_MASK_BIT4;
   tmpByte |= egIe->t.umtsKeyUCQt.isENA * EG_MASK_BIT5;
   tmpByte |= egIe->t.umtsKeyUCQt.isHNNA * EG_MASK_BIT6;

   SAddPstMsg(tmpByte, mBuf);
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* encode Voice Domain Preference and UE's usage Setting length */
   SAddPstMsg( egIe->t.umtsKeyUCQt.vDomPrefUEUsageLen, mBuf);
   
   /* encode UE's usage setting (1 bit field) Bit-3
    * encode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.umtsKeyUCQt.vDomPrefUEUsageLen)
   {
     tmpByte = 0;
     tmpByte = egIe->t.umtsKeyUCQt.ueUsageSett * EG_MASK_BIT1;
     tmpByte <<= EG_SHIFT_2;
     tmpByte |= egIe->t.umtsKeyUCQt.vDomPref & 0x03;

     SAddPstMsg(tmpByte , mBuf);
   }
#endif

#ifdef EG_REL_AC0
/* encode HighBitRateThan16MbpsLen and HighBitRateThan16Mbps */
   SAddPstMsg( egIe->t.umtsKeyUCQt.HighBitRateThan16MbpsLen, mBuf);
   SAddPstMsg( egIe->t.umtsKeyUCQt.HighBitRateThan16Mbps, mBuf);
#endif

   RETVALUE(retValue);
} /* end of egEncEG_UMTS_KEY_CIPH_QUIN */


/***********************************************************************
 *   Fun:   egEncEG_GSM_KEY_CIPH_QUIN
 *   Desc:  Encodes the IE data type GSM Key, Used Cipher and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_GSM_KEY_CIPH_QUIN
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_GSM_KEY_CIPH_QUIN(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 cnt = 0;
   S16 retValue = ROK;
   U8  tmpByte = 0;

   tmpByte = (((egIe->t.gsmKeyUCQt.secMode << EG_SHIFT_5)&0xE0) | 
             (egIe->t.gsmKeyUCQt.isDRXIPres * EG_MASK_BIT4) | (egIe->t.gsmKeyUCQt.cksn & 0x07));
   SAddPstMsg(tmpByte , mBuf);

   tmpByte = 0;
   tmpByte = (egIe->t.gsmKeyUCQt.nmbQuin << EG_SHIFT_5) & 0xE0;
   tmpByte |= (egIe->t.gsmKeyUCQt.isUAMBRIPres * EG_MASK_BIT2);
   tmpByte |= (egIe->t.gsmKeyUCQt.isSAMBRIPres * EG_MASK_BIT1);
   SAddPstMsg(tmpByte , mBuf);

   SAddPstMsg( (egIe->t.gsmKeyUCQt.usedCipher & 0x07), mBuf);

   /* encode KC of 8 octets */
   SAddPstMsgMult(egIe->t.gsmKeyUCQt.kc, EG_LEN_EIGHT, mBuf);

   for(cnt = 0;cnt < egIe->t.gsmKeyUCQt.nmbQuin;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.gsmKeyUCQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].rand.val, egIe->t.gsmKeyUCQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.gsmKeyUCQt.authQuin[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].xres.val, egIe->t.gsmKeyUCQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* mandate the length as 16 octets */
     egIe->t.gsmKeyUCQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.gsmKeyUCQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;

     /* encode the CK, IK each is of 16 octets length */
     EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].ck.val, egIe->t.gsmKeyUCQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].ik.val, egIe->t.gsmKeyUCQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the AUTN */
     SAddPstMsg(egIe->t.gsmKeyUCQt.authQuin[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].autn.val, egIe->t.gsmKeyUCQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }
   }

   if(egIe->t.gsmKeyUCQt.isDRXIPres)
   {
   /* encode drx parameter - is of 2 octets */
   SAddPstMsgMult(egIe->t.gsmKeyUCQt.drxParam, EG_LEN_TWO, mBuf);
   }

   if(egIe->t.gsmKeyUCQt.isSAMBRIPres)
   {
   /* encode upLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyUCQt.upSubsUEAMBR, mBuf);

   /* encode downLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyUCQt.dnSubsUEAMBR, mBuf);
   }

   if(egIe->t.gsmKeyUCQt.isUAMBRIPres)
   {
   /* encode upLink used UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyUCQt.upUsedUEAMBR, mBuf);

   /* encode downLink used UE AMBR */
   EG_ENC_U32(egIe->t.gsmKeyUCQt.dnUsedUEAMBR, mBuf);
   }

   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   SAddPstMsg(egIe->t.gsmKeyUCQt.ueNtwrkCapb.length , mBuf);
   /* encode UE Network Capability - can be max of 15 octets */
   EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.ueNtwrkCapb.val, egIe->t.gsmKeyUCQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

  /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   SAddPstMsg(egIe->t.gsmKeyUCQt.msNtwrkCapb.length , mBuf);
   /* encode MS Network Capability - can be max of 10 octets */
   EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.msNtwrkCapb.val, egIe->t.gsmKeyUCQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

   /* here we are mandating the length of MEID will be of 16 octets */
   SAddPstMsg(egIe->t.gsmKeyUCQt.meId.length , mBuf);
   /* encode MEID - is of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.gsmKeyUCQt.meId.val, egIe->t.gsmKeyUCQt.meId.length, EG_LEN_SIXTEEN, retValue);

   tmpByte = 0;

   tmpByte = egIe->t.gsmKeyUCQt.isUNA * EG_MASK_BIT1;
   tmpByte |= egIe->t.gsmKeyUCQt.isGENA * EG_MASK_BIT2;
   tmpByte |= egIe->t.gsmKeyUCQt.isGANA * EG_MASK_BIT3;
   tmpByte |= egIe->t.gsmKeyUCQt.isINA * EG_MASK_BIT4;
   tmpByte |= egIe->t.gsmKeyUCQt.isENA * EG_MASK_BIT5;
   tmpByte |= egIe->t.gsmKeyUCQt.isHNNA * EG_MASK_BIT6;

   SAddPstMsg(tmpByte, mBuf);

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* encode Voice Domain Preference and UE's usage Setting length */
   SAddPstMsg( egIe->t.gsmKeyUCQt.vDomPrefUEUsageLen, mBuf);
   
   /* encode UE's usage setting (1 bit field) Bit-3
    * encode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.gsmKeyUCQt.vDomPrefUEUsageLen)
   {
     tmpByte = 0;
     tmpByte = egIe->t.gsmKeyUCQt.ueUsageSett * EG_MASK_BIT1;
     tmpByte <<= EG_SHIFT_2;
     tmpByte |= egIe->t.gsmKeyUCQt.vDomPref & 0x03;

     SAddPstMsg(tmpByte , mBuf);
   }
#endif
   
#ifdef EG_REL_AC0
/* encode of HighBitRateThan16MbpsLen and HighBitRateThan16Mbps as per release 10*/
   SAddPstMsg( egIe->t.gsmKeyUCQt.HighBitRateThan16MbpsLen, mBuf);
   SAddPstMsg( egIe->t.gsmKeyUCQt.HighBitRateThan16Mbps, mBuf);
#endif

   RETVALUE(retValue);
} /* end of egEncEG_GSM_KEY_CIPH_QUIN */


/***********************************************************************
 *   Fun:   egEncEG_UMTS_KEY_QUIN
 *   Desc:  Encodes the IE data type UMTS Key, and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UMTS_KEY_QUIN
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UMTS_KEY_QUIN(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 cnt = 0;
   S16 retValue = ROK;
   U8  tmpByte = 0;

   tmpByte = (((egIe->t.umtsKeyQt.secMode << EG_SHIFT_5)&0xE0) | 
             (egIe->t.umtsKeyQt.isDRXIPres * EG_MASK_BIT4) | (egIe->t.umtsKeyQt.ksi & 0x07));
   SAddPstMsg(tmpByte , mBuf);

   tmpByte = 0;
   tmpByte = (egIe->t.umtsKeyQt.nmbQuin<< EG_SHIFT_5) & 0xE0;
   tmpByte |= (egIe->t.umtsKeyQt.isUAMBRIPres * EG_MASK_BIT2);
   tmpByte |= (egIe->t.umtsKeyQt.isSAMBRIPres * EG_MASK_BIT1);
   SAddPstMsg(tmpByte , mBuf);
   /* 7th one is  spare octect */
   tmpByte = 0;
   SAddPstMsg( tmpByte, mBuf);

   /* mandate the length as 16 octets */
   egIe->t.umtsKeyQt.ck.length = EG_LEN_SIXTEEN;
   egIe->t.umtsKeyQt.ik.length = EG_LEN_SIXTEEN;

   /* encode CK, IK each of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.ck.val, egIe->t.umtsKeyQt.ck.length, EG_LEN_SIXTEEN, retValue);
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.ik.val, egIe->t.umtsKeyQt.ik.length, EG_LEN_SIXTEEN, retValue);
   for(cnt = 0;cnt < egIe->t.umtsKeyQt.nmbQuin;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.umtsKeyQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].rand.val, egIe->t.umtsKeyQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.umtsKeyQt.authQuin[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].xres.val, egIe->t.umtsKeyQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* mandate the length as 16 octets */
     egIe->t.umtsKeyQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.umtsKeyQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;

     /* encode the CK, IK each is of 16 octets length */
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].ck.val, egIe->t.umtsKeyQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].ik.val, egIe->t.umtsKeyQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the AUTN */
     SAddPstMsg(egIe->t.umtsKeyQt.authQuin[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].autn.val, egIe->t.umtsKeyQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }
   }

   if(egIe->t.umtsKeyQt.isDRXIPres)
   {
   /* encode drx parameter - is of 2 octets */
   SAddPstMsgMult(egIe->t.umtsKeyQt.drxParam, EG_LEN_TWO, mBuf);
   }

   if(egIe->t.umtsKeyQt.isSAMBRIPres)
   {
   /* encode upLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQt.upSubsUEAMBR, mBuf);

   /* encode downLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQt.dnSubsUEAMBR, mBuf);
   }

   if(egIe->t.umtsKeyQt.isUAMBRIPres)
   {
   /* encode upLink used UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQt.upUsedUEAMBR, mBuf);

   /* encode downLink used UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQt.dnUsedUEAMBR, mBuf);
   }
 
  if(egIe->t.umtsKeyQt.ueNtwrkCapb.length )
  { 
   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   SAddPstMsg(egIe->t.umtsKeyQt.ueNtwrkCapb.length , mBuf);
   /* encode UE Network Capability - can be max of 15 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.ueNtwrkCapb.val, egIe->t.umtsKeyQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
  }
  else
  {
     SAddPstMsg(0, mBuf);
  }
   
  if( egIe->t.umtsKeyQt.msNtwrkCapb.length )
  {
      /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   SAddPstMsg(egIe->t.umtsKeyQt.msNtwrkCapb.length , mBuf);
   /* encode MS Network Capability - can be max of 10 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.msNtwrkCapb.val, egIe->t.umtsKeyQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
  }
  else
  {
     SAddPstMsg(0, mBuf); 
  }
   
  if( egIe->t.umtsKeyQt.meId.length )
  { 
   /* here we are mandating the length of MEID will be of 16 octets */
   SAddPstMsg(egIe->t.umtsKeyQt.meId.length , mBuf);
   /* encode MEID - is of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQt.meId.val, egIe->t.umtsKeyQt.meId.length, EG_LEN_SIXTEEN, retValue);
  }
  else
  {
     SAddPstMsg(0, mBuf);
  }
  
   tmpByte = 0;

   tmpByte = egIe->t.umtsKeyQt.isUNA * EG_MASK_BIT1;
   tmpByte |= egIe->t.umtsKeyQt.isGENA * EG_MASK_BIT2;
   tmpByte |= egIe->t.umtsKeyQt.isGANA * EG_MASK_BIT3;
   tmpByte |= egIe->t.umtsKeyQt.isINA * EG_MASK_BIT4;
   tmpByte |= egIe->t.umtsKeyQt.isENA * EG_MASK_BIT5;
   tmpByte |= egIe->t.umtsKeyQt.isHNNA * EG_MASK_BIT6;

   SAddPstMsg(tmpByte, mBuf);

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* encode Voice Domain Preference and UE's usage Setting length */
   SAddPstMsg( egIe->t.umtsKeyQt.vDomPrefUEUsageLen, mBuf);
   
   /* encode UE's usage setting (1 bit field) Bit-3
    * encode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.umtsKeyQt.vDomPrefUEUsageLen)
   {
     tmpByte = 0;
     tmpByte = egIe->t.umtsKeyQt.ueUsageSett * EG_MASK_BIT1;
     tmpByte <<= EG_SHIFT_2;
     tmpByte |= egIe->t.umtsKeyQt.vDomPref & 0x03;

     SAddPstMsg(tmpByte , mBuf);
   }
#endif

#ifdef EG_REL_AC0
   SAddPstMsg( egIe->t.umtsKeyQt.HighBitRateThan16MbpsLen, mBuf);
   SAddPstMsg( egIe->t.umtsKeyQt.HighBitRateThan16Mbps, mBuf);
#endif
   
   RETVALUE(retValue);
} /* end of egEncEG_UMTS_KEY_QUIN */


/***********************************************************************
 *   Fun:   egEncEG_EPS_SEC_QUAD_QUIN
 *   Desc:  Encodes the IE data type EPS Security Context, Quadruplets and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_EPS_SEC_QUAD_QUIN
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_EPS_SEC_QUAD_QUIN(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 cnt = 0;
   S16 retValue = ROK;
   U8  tmpByte = 0x00;

   tmpByte = (((egIe->t.eSecCxtQdQt.secMode & 0x07) << EG_SHIFT_5)) | 
       (egIe->t.eSecCxtQdQt.isNHIPres * EG_MASK_BIT5) |
       (egIe->t.eSecCxtQdQt.isDRXIPres * EG_MASK_BIT4) | (egIe->t.eSecCxtQdQt.ksi & 0x07);
     SAddPstMsg(tmpByte, mBuf);

   tmpByte = 0x00;
   tmpByte = ((egIe->t.eSecCxtQdQt.nmbQuin & 0x07) << EG_SHIFT_5) |
       ((egIe->t.eSecCxtQdQt.nmbQuadru & 0x07) << EG_SHIFT_2) |
       (egIe->t.eSecCxtQdQt.isUAMBRIPres * EG_MASK_BIT2) |
       (egIe->t.eSecCxtQdQt.isOCSIPres  * EG_MASK_BIT1);
   SAddPstMsg(tmpByte, mBuf);

   tmpByte = egIe->t.eSecCxtQdQt.isSAMBRIPres * EG_MASK_BIT8;
   tmpByte |= ( (((egIe->t.eSecCxtQdQt.nasProtAlgo & 0x07) << EG_SHIFT_4))| 
          (egIe->t.eSecCxtQdQt.usedNasCipher & 0x0F));
   SAddPstMsg(tmpByte , mBuf);

   /* encode NAS Downlink Count and NAS Uplink Count - each of 3 octets long */
   EG_ENC_U24(egIe->t.eSecCxtQdQt.nasDlCount, mBuf);
   EG_ENC_U24(egIe->t.eSecCxtQdQt.nasUlCount, mBuf);

   /* encode Kasme of 32 octets */
   egIe->t.eSecCxtQdQt.kasme.length = EG_KASME_LEN;
   EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.kasme.val, egIe->t.eSecCxtQdQt.kasme.length, EG_LEN_32, retValue);

   for(cnt=0;cnt < egIe->t.eSecCxtQdQt.nmbQuadru;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.eSecCxtQdQt.authQuadru[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].rand.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
      if( retValue != ROK)
      {
         RETVALUE(retValue);
      }

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.eSecCxtQdQt.authQuadru[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].xres.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
      if( retValue != ROK)
      {
         RETVALUE(retValue);
      }

     /* encode the AUTN length in single octet and AUTH which can be of 16 octets */
     SAddPstMsg(egIe->t.eSecCxtQdQt.authQuadru[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].autn.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
      if( retValue != ROK)
      {
         RETVALUE(retValue);
      }

     /* encode the Kasme which is of 31 octets */
     egIe->t.eSecCxtQdQt.authQuadru[cnt].kasme.length = EG_KASME_LEN;
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].kasme.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].kasme.length, EG_LEN_32, retValue);
      if( retValue != ROK)
      {
         RETVALUE(retValue);
      }

   }
   for(cnt=0;cnt<egIe->t.eSecCxtQdQt.nmbQuin;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.eSecCxtQdQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].rand.val, egIe->t.eSecCxtQdQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.eSecCxtQdQt.authQuin[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].xres.val, egIe->t.eSecCxtQdQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* mandate the length as 16 octets */
     egIe->t.eSecCxtQdQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.eSecCxtQdQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;

     /* encode the CK, IK each is of 16 octets length */
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].ck.val, egIe->t.eSecCxtQdQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].ik.val, egIe->t.eSecCxtQdQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the AUTN */
     SAddPstMsg(egIe->t.eSecCxtQdQt.authQuin[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].autn.val, egIe->t.eSecCxtQdQt.authQuin[cnt].autn.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }
   }
   if(egIe->t.eSecCxtQdQt.isDRXIPres)
   {
   /* encode drx parameter - is of 2 octets */
   SAddPstMsgMult(egIe->t.eSecCxtQdQt.drxParam, EG_LEN_TWO, mBuf);
   }

   if(egIe->t.eSecCxtQdQt.isNHIPres)
   {
   /* encode next hop parameter - is of 32 octets */
   SAddPstMsgMult(egIe->t.eSecCxtQdQt.nxtHop, 32, mBuf);
   /* encode ncc parameter - is of 3 bits*/
   SAddPstMsg((egIe->t.eSecCxtQdQt.ncc)&0x07,mBuf);
   }

   if(egIe->t.eSecCxtQdQt.isSAMBRIPres)
   {
     /* encode upLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.eSecCxtQdQt.upSubsUEAMBR, mBuf);
   /* encode downLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.eSecCxtQdQt.dnSubsUEAMBR, mBuf);
   }

   if(egIe->t.eSecCxtQdQt.isUAMBRIPres)
   {
   /* encode upLink used UE AMBR */
   EG_ENC_U32(egIe->t.eSecCxtQdQt.upUsedUEAMBR, mBuf);
   /* encode downLink used UE AMBR */
   EG_ENC_U32(egIe->t.eSecCxtQdQt.dnUsedUEAMBR, mBuf);
   }

   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   SAddPstMsg(egIe->t.eSecCxtQdQt.ueNtwrkCapb.length , mBuf);
   /* encode UE Network Capability - can be max of 15 octets */
   EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.ueNtwrkCapb.val, egIe->t.eSecCxtQdQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

  /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   SAddPstMsg(egIe->t.eSecCxtQdQt.msNtwrkCapb.length , mBuf);
   /* encode MS Network Capability - can be max of 10 octets */
   EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.msNtwrkCapb.val, egIe->t.eSecCxtQdQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

   /* here we are mandating the length of MEID will be of 16 octets */
   SAddPstMsg(egIe->t.eSecCxtQdQt.meId.length , mBuf);
   /* encode MEID - is of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.meId.val, egIe->t.eSecCxtQdQt.meId.length, EG_LEN_SIXTEEN, retValue);

   tmpByte = 0;

   tmpByte = egIe->t.eSecCxtQdQt.isUNA * EG_MASK_BIT1;
   tmpByte |= egIe->t.eSecCxtQdQt.isGENA * EG_MASK_BIT2;
   tmpByte |= egIe->t.eSecCxtQdQt.isGANA * EG_MASK_BIT3;
   tmpByte |= egIe->t.eSecCxtQdQt.isINA * EG_MASK_BIT4;
   tmpByte |= egIe->t.eSecCxtQdQt.isENA * EG_MASK_BIT5;
   tmpByte |= egIe->t.eSecCxtQdQt.isHNNA * EG_MASK_BIT6;
   SAddPstMsg(tmpByte, mBuf);

   tmpByte = egIe->t.eSecCxtQdQt.nccOld & 0x07;
   tmpByte |= (egIe->t.eSecCxtQdQt.ksiOld & 0x07)<< EG_SHIFT_2;
   tmpByte |= (egIe->t.eSecCxtQdQt.isNHIOldPres * EG_MASK_BIT8);
   SAddPstMsg(tmpByte, mBuf);

  /* encode Kasme_old of 32 octets */
   egIe->t.eSecCxtQdQt.kasmeOld.length = EG_KASME_LEN;
   EG_ENC_STR(mBuf, egIe->t.eSecCxtQdQt.kasmeOld.val, egIe->t.eSecCxtQdQt.kasmeOld.length, EG_LEN_32, retValue);

   if(egIe->t.eSecCxtQdQt.isNHIOldPres)
   {
   /* encode next hop_old parameter - is of 32 octets */
   SAddPstMsgMult(egIe->t.eSecCxtQdQt.nxtHopOld, 32, mBuf);
   }

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* encode Voice Domain Preference and UE's usage Setting length */
   SAddPstMsg( egIe->t.eSecCxtQdQt.vDomPrefUEUsageLen, mBuf);
   
   /* encode UE's usage setting (1 bit field) Bit-3
    * encode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.eSecCxtQdQt.vDomPrefUEUsageLen)
   {
     tmpByte = 0;
     tmpByte = egIe->t.eSecCxtQdQt.ueUsageSett * EG_MASK_BIT1;
     tmpByte <<= EG_SHIFT_2;
     tmpByte |= egIe->t.eSecCxtQdQt.vDomPref & 0x03;

     SAddPstMsg(tmpByte , mBuf);
   }
#endif

   RETVALUE(retValue);
} /* end of egEncEG_EPS_SEC_QUAD_QUIN */


/***********************************************************************
 *   Fun:   egEncEG_UMTS_KEY_QUAD_QUIN
 *   Desc:  Encodes the IE data type UMTS Key, Quadruplets and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UMTS_KEY_QUAD_QUIN
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UMTS_KEY_QUAD_QUIN(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 cnt = 0;
   S16 retValue = ROK;
   U8  tmpByte = 0x00;

   tmpByte = ((egIe->t.umtsKeyQdQt.secMode & 0x07) << EG_SHIFT_5) | 
       (egIe->t.umtsKeyQdQt.isDRXIPres * EG_MASK_BIT4) | (egIe->t.umtsKeyQdQt.ksi & 0x07);
   SAddPstMsg(tmpByte, mBuf);

   tmpByte = 0x00;
   tmpByte = (((egIe->t.umtsKeyQdQt.nmbQuin & 0x07) << EG_SHIFT_5) |
       ((egIe->t.umtsKeyQdQt.nmbQuadru & 0x03) << EG_SHIFT_2) |
       (egIe->t.umtsKeyQdQt.isUAMBRIPres * EG_MASK_BIT2) |
       (egIe->t.umtsKeyQdQt.isSAMBRIPres * EG_MASK_BIT1));
   SAddPstMsg(tmpByte, mBuf);

  /* 7th one is spare octect */
   tmpByte = 0x00;
   SAddPstMsg(tmpByte , mBuf);

   /* mandate the length as 16 octets */
   egIe->t.umtsKeyQdQt.ck.length = EG_LEN_SIXTEEN;
   egIe->t.umtsKeyQdQt.ik.length = EG_LEN_SIXTEEN;

   /* encode CK, IK each of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.ck.val, egIe->t.umtsKeyQdQt.ck.length, EG_LEN_SIXTEEN, retValue);
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.ik.val, egIe->t.umtsKeyQdQt.ik.length, EG_LEN_SIXTEEN, retValue);

   for(cnt=0;cnt < egIe->t.umtsKeyQdQt.nmbQuadru;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.umtsKeyQdQt.authQuadru[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].rand.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].rand.length, EG_LEN_SIXTEEN, retValue);

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.umtsKeyQdQt.authQuadru[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].xres.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].xresLen, EG_LEN_SIXTEEN, retValue);

     /* encode the AUTN length in single octet and AUTH which can be of 16 octets */
     SAddPstMsg(egIe->t.umtsKeyQdQt.authQuadru[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].autn.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].autnLen, EG_LEN_SIXTEEN, retValue);

     /* encode the Kasme which is of 31 octets */
     egIe->t.umtsKeyQdQt.authQuadru[cnt].kasme.length = EG_KASME_LEN;
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].kasme.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].kasme.length, EG_LEN_32, retValue);
   }
   for(cnt=0;cnt<egIe->t.umtsKeyQdQt.nmbQuin;cnt++)
   {
     /* mandate the length as 16 octets */
     egIe->t.umtsKeyQdQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;

     /* encode the RAND which is of 16 octets*/
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].rand.val, egIe->t.umtsKeyQdQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the XRES length in single octet and XRES which can be of 16 octets */
     SAddPstMsg(egIe->t.umtsKeyQdQt.authQuin[cnt].xresLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].xres.val, egIe->t.umtsKeyQdQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* mandate the length as 16 octets */
     egIe->t.umtsKeyQdQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.umtsKeyQdQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;

     /* encode the CK, IK each is of 16 octets length */
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].ck.val, egIe->t.umtsKeyQdQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].ik.val, egIe->t.umtsKeyQdQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }

     /* encode the AUTN */
     SAddPstMsg(egIe->t.umtsKeyQdQt.authQuin[cnt].autnLen, mBuf);
     EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].autn.val, egIe->t.umtsKeyQdQt.authQuin[cnt].autn.length, EG_LEN_SIXTEEN, retValue);
     if( retValue != ROK)
     {
       RETVALUE(retValue);
     }
   }
   if(egIe->t.umtsKeyQdQt.isDRXIPres)
   {
   /* encode drx parameter - is of 2 octets */
   SAddPstMsgMult(egIe->t.umtsKeyQdQt.drxParam, EG_LEN_TWO, mBuf);
   }

   if(egIe->t.umtsKeyQdQt.isSAMBRIPres)
   {
     /* encode upLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQdQt.upSubsUEAMBR, mBuf);

   /* encode downLink subscribed UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQdQt.dnSubsUEAMBR, mBuf);
   }

   if(egIe->t.umtsKeyQdQt.isUAMBRIPres)
   {
   /* encode upLink used UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQdQt.upUsedUEAMBR, mBuf);

   /* encode downLink used UE AMBR */
   EG_ENC_U32(egIe->t.umtsKeyQdQt.dnUsedUEAMBR, mBuf);
   }

   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   SAddPstMsg(egIe->t.umtsKeyQdQt.ueNtwrkCapb.length , mBuf);
   /* encode UE Network Capability - can be max of 15 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.ueNtwrkCapb.val, egIe->t.umtsKeyQdQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

  /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   SAddPstMsg(egIe->t.umtsKeyQdQt.msNtwrkCapb.length , mBuf);
   /* encode MS Network Capability - can be max of 10 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.msNtwrkCapb.val, egIe->t.umtsKeyQdQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

   /* here we are mandating the length of MEID will be of 16 octets */
   SAddPstMsg(egIe->t.umtsKeyQdQt.meId.length , mBuf);
   /* encode MEID - is of 16 octets */
   EG_ENC_STR(mBuf, egIe->t.umtsKeyQdQt.meId.val, egIe->t.umtsKeyQdQt.meId.length, EG_LEN_SIXTEEN, retValue);

   tmpByte = 0;

   tmpByte = egIe->t.umtsKeyQdQt.isUNA * EG_MASK_BIT1;
   tmpByte |= egIe->t.umtsKeyQdQt.isGENA * EG_MASK_BIT2;
   tmpByte |= egIe->t.umtsKeyQdQt.isGANA * EG_MASK_BIT3;
   tmpByte |= egIe->t.umtsKeyQdQt.isINA * EG_MASK_BIT4;
   tmpByte |= egIe->t.umtsKeyQdQt.isENA * EG_MASK_BIT5;
   tmpByte |= egIe->t.umtsKeyQdQt.isHNNA * EG_MASK_BIT6;
   SAddPstMsg(tmpByte, mBuf);

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* encode Voice Domain Preference and UE's usage Setting length */
   SAddPstMsg( egIe->t.umtsKeyQdQt.vDomPrefUEUsageLen, mBuf);
   
   /* encode UE's usage setting (1 bit field) Bit-3
    * encode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.umtsKeyQdQt.vDomPrefUEUsageLen)
   {
     tmpByte = 0;
     tmpByte = egIe->t.umtsKeyQdQt.ueUsageSett * EG_MASK_BIT1;
     tmpByte <<= EG_SHIFT_2;
     tmpByte |= egIe->t.umtsKeyQdQt.vDomPref & 0x03;

     SAddPstMsg(tmpByte , mBuf);
   }
#endif

   RETVALUE(retValue);
} /* end of egEncEG_UMTS_KEY_QUAD_QUIN */


/***********************************************************************
 *   Fun:   egEncEG_F_CAUSE
 *   Desc:  Encodes the IE data type F-Cause
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_F_CAUSE
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_F_CAUSE(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode the FCause Type */
   egIe->t.fCause.causeType = egIe->t.fCause.causeType & 0x0F;
   SAddPstMsg(egIe->t.fCause.causeType, mBuf);

   /* encode the FCause Value - CR 135356 fix */
   if( egIe->t.fCause.cause > 255 )
   {
   EG_ENC_U16(egIe->t.fCause.cause, mBuf);
   }
   else
   {
      SAddPstMsg((U8)egIe->t.fCause.cause, mBuf);
   }

   RETVALUE(retValue);
} /* end of egEncEG_F_CAUSE */


/***********************************************************************
 *   Fun:   egEncEG_S103_PDN_FW_INFO
 *   Desc:  Encodes the IE data type S103 PDN Data Forwarding Info
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_S103_PDN_FW_INFO
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_S103_PDN_FW_INFO(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8  cnt;

   /* encode the PSDN Address length */
   switch (egIe->t.s103PDF.hsgwAddr.type)
   {
     case CM_TPTADDR_IPV4:
       {
         /* in case of IPv4 encode length as 4 */
         egIe->t.s103PDF.hsgwAddrLen = 0x04;
         SAddPstMsg(egIe->t.s103PDF.hsgwAddrLen, mBuf);
         EG_ENC_U32(egIe->t.s103PDF.hsgwAddr.u.ipv4, mBuf);
         break;
       } /* end of case */
     case CM_TPTADDR_IPV6:
       {
         /* in case of IPv6 encode length as 16 */
         egIe->t.s103PDF.hsgwAddrLen = 0x10;
         SAddPstMsg(egIe->t.s103PDF.hsgwAddrLen, mBuf);
         SAddPstMsgMult(egIe->t.s103PDF.hsgwAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         break;
       } /* end of case */
     default:
       {
          RETVALUE(RFAILED);
       } /* end of case */
   } /* end of switch */

   /* encode the GRE Key */
   EG_ENC_U32(egIe->t.s103PDF.greKey, mBuf);

   /* encode the EBI Number */
   SAddPstMsg(egIe->t.s103PDF.ebiNumber, mBuf);

   /* encode the Bearer Id - can be max of 10 octets */
   for(cnt = 0; cnt < egIe->t.s103PDF.ebiNumber && cnt < EG_MAX_NMB_BEARER;cnt++)
   {
     SAddPstMsg(egIe->t.s103PDF.ebi[cnt] & 0x0F, mBuf);
   }

   RETVALUE(retValue);
} /* end of egEncEG_S103_PDN_FW_INFO */


/***********************************************************************
 *   Fun:   egEncEG_S1U_DATA_FW
 *   Desc:  Encodes the IE data type S1-U Data Forwarding
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_S1U_DATA_FW
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_S1U_DATA_FW(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   SAddPstMsg(egIe->t.s1UDF.ebi & 0x0F, mBuf);

   /* encode the SGW Address length */
   switch (egIe->t.s1UDF.sgwAddr.type)
   {
      case CM_TPTADDR_IPV4:
      {
         /* in case of IPv4 encode length as 4 */
         egIe->t.s1UDF.sgwAddrLen = 0x04;
         SAddPstMsg(egIe->t.s1UDF.sgwAddrLen , mBuf);
         EG_ENC_U32(egIe->t.s1UDF.sgwAddr.u.ipv4, mBuf);
         break;
      } /* end of case */
#ifdef EG_IPV6_SUPPORTED
      case CM_TPTADDR_IPV6:
      {
         /* in case of IPv6 encode length as 16 */
         egIe->t.s1UDF.sgwAddrLen = 0x10;
         SAddPstMsg(egIe->t.s1UDF.sgwAddrLen , mBuf);
         SAddPstMsgMult(egIe->t.s1UDF.sgwAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         break;
      } /* end of case */
#endif /* end of CM_TPTADDR_IPV6 */
      default:
      {
         RETVALUE(RFAILED);
      } /* end of case */
   } /* end of switch */

   /* encode the S1U-TEID */
   EG_ENC_U32(egIe->t.s1UDF.sgwS1UTEID, mBuf);

   RETVALUE(retValue);
} /* end of egEncEG_S1U_DATA_FW */


/***********************************************************************
 *   Fun:   egEncEG_SRC_RNC_PDCP_CTX
 *   Desc:  Encodes the IE data type Source RNC PDCP Context
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_SRC_RNC_PDCP_CTX
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_SRC_RNC_PDCP_CTX(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   EG_ENC_STR(mBuf, egIe->t.valStr.val, egIe->t.valStr.length, EG_MAX_STR_LEN, retValue);

   RETVALUE(retValue);
} /* end of egEncEG_SRC_RNC_PDCP_CTX */


/***********************************************************************
 *   Fun:   egEncEG_PDUNUM
 *   Desc:  Encodes the IE data type PDU Number
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_PDUNUM
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_PDUNUM(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode NSAPI */
   SAddPstMsg(egIe->t.pduNum.nsapi, mBuf);

   /* encode the DownLink GTP-U Sequence Number */
   EG_ENC_U16(egIe->t.pduNum.dnLinkSeqNo, mBuf);

   /* encode the Uplink GTP-U Sequence Number */
   EG_ENC_U16(egIe->t.pduNum.upLinkSeqNo, mBuf);

   /* encode the Send N-PDU Number */
   EG_ENC_U16(egIe->t.pduNum.SNPduNo, mBuf);

   /* encode the Receive N-PDU Number */
   EG_ENC_U16(egIe->t.pduNum.RNPduNo, mBuf);

   RETVALUE(retValue);
} /* end of egEncEG_PDUNUM */

/***********************************************************************
 *   Fun:   egEncEG_UE_TIME_ZONE
 *   Desc:  Encodes the IE data type Time zone
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UE_TIME_ZONE
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UE_TIME_ZONE(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode time zone */
   SAddPstMsg(egIe->t.timeZone.timezone, mBuf);
   /* encode day light savings time */
   egIe->t.timeZone.dayLiteSavTime = egIe->t.timeZone.dayLiteSavTime & 0x03;
   SAddPstMsg(egIe->t.timeZone.dayLiteSavTime, mBuf);
   RETVALUE(retValue);
}
/***********************************************************************
 *   Fun:   egEncEG_TRACE_REF
 *   Desc:  Encodes the IE data type trace reference
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_TRACE_REF
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_TRACE_REF(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode snkw */
   EG_ENC_NETID(egIe->t.trcRef.sNwk,mBuf);
   /* encode trace id */
   
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
   EG_ENC_U24(egIe->t.trcRef.trcId, mBuf);

   RETVALUE(retValue);
}
/***********************************************************************
 *   Fun:   egEncEG_RAB_CONTEXT
 *   Desc:  Encodes the IE data type RAB Context
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_RAB_CONTEXT
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_RAB_CONTEXT(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode nsapi */
   SAddPstMsg(egIe->t.rabCntxt.nsapi & 0x0F,mBuf);
   /* encode Downlink GTP-U Sequence Number */
   EG_ENC_U16(egIe->t.rabCntxt.dnlEgUSeqNo ,mBuf);
   /* encode Uplink GTP-U Sequence Number */
   EG_ENC_U16(egIe->t.rabCntxt.uplEgUSeqNo ,mBuf);
   /* Downlink GTP-U Sequence Number */
   EG_ENC_U16(egIe->t.rabCntxt.dnlPdcpSeqNo ,mBuf);
   /*  Uplink GTP-U Sequence Number */
   EG_ENC_U16(egIe->t.rabCntxt.uplPdcpSeqNo ,mBuf);

   RETVALUE(retValue);
}
/***********************************************************************
 *   Fun:   egEncEG_AMBR
 *   Desc:  Encodes the IE data type Aggregate Maximum Bit Rate
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_AMBR
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_AMBR(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /*encode APN-AMBR for uplink */
   EG_ENC_U32(egIe->t.ambr.uplAPNAMBR, mBuf);
   /* encode APN-AMBR for downlink */
   EG_ENC_U32(egIe->t.ambr.dnlAPNAMBR, mBuf);

   RETVALUE(retValue);
}
/***********************************************************************
 *   Fun:   egEncEG_GLOBAL_CN_ID
 *   Desc:  Encodes the IE data type Global CN Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_GLOBAL_CN_ID
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_GLOBAL_CN_ID(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* Encode sNwk */
   EG_ENC_NETID(egIe->t.gblCnId.sNwk,mBuf);
   /* Encode cnId */
   EG_ENC_STR(mBuf, egIe->t.gblCnId.cnId.val, egIe->t.gblCnId.cnId.length, EG_LEN_FOUR, retValue);

   RETVALUE(retValue);
} /* egEncEG_GLOBAL_CN_ID */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
/***********************************************************************
 *   Fun:   egEncEG_MBMS_IPMCAST_DIST
 *   Desc:  Encodes the IE data type MBMS Multicast IP Distribution
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_MBMS_IPMCAST_DIST
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_MBMS_IPMCAST_DIST(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 addressType = 0;

   /* encode Common Tunnel Endpoint Identifier Data */
   EG_ENC_U32(egIe->t.mbmsIPMCastDist.cteidDat, mBuf);

   /* encode IP Multicast Distribution Address Type */
   EG_ENC_U8(egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type, mBuf);
#ifdef EG_REL_AC0
   addressType = egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type & 0x3F;
#else
   addressType = egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type;
#endif /* EG_REL_AC0 */
   /* encode IP Multicast Distribution Address */
   switch (addressType)
   {
     case CM_TPTADDR_IPV4:
       {
         EG_ENC_U32(egIe->t.mbmsIPMCastDist.ipMCastDistAddr.u.ipv4, mBuf);
         break;
       } /* end of case */
#ifdef EG_IPV6_SUPPORTED
     case CM_TPTADDR_IPV6:
       {
         SAddPstMsgMult(egIe->t.mbmsIPMCastDist.ipMCastDistAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         break;
       } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
     default:
       {
          RETVALUE(RFAILED);
       } /* end of case */
   }

   /* encode  IP Multicast Source Address Type */
   EG_ENC_U8(egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type, mBuf);
#ifdef EG_REL_AC0
   addressType = egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type & 0x3F;
#else
   addressType = egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type;
#endif /* EG_REL_AC0 */
 
   /* encode  IP Multicast Source Address */
   switch (egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type)
   {
     case CM_TPTADDR_IPV4:
       {
         EG_ENC_U32(egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.u.ipv4, mBuf);
         break;
       } /* end of case */
#ifdef EG_IPV6_SUPPORTED
     case CM_TPTADDR_IPV6:
       {
         SAddPstMsgMult(egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         break;
       } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
     default:
       {
          RETVALUE(RFAILED);
       } /* end of case */
   }

   /* encode MBMS HC Indicator */
   EG_ENC_U8(egIe->t.mbmsIPMCastDist.mbmsHCInd, mBuf);

   RETVALUE(retValue);
}

/***********************************************************************
 *   Fun:   egEncEG_UCI 
 *   Desc:  Encodes the IE data type user CSG information
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_UCI 
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_UCI(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;

   /* encode serving network */
   EG_ENC_NETID(egIe->t.uci.sNwk,mBuf);
   
   /* encode CSG Id values */
   /* CSG Id consists of 4 octets. Bit 3 of Octect 8 is the most significant bit and
    * bit 1 of Octet 11 is the least significant bit
    */
   egIe->t.uci.csgId &= 0x07FFFFFF;
   EG_ENC_U32(egIe->t.uci.csgId, mBuf);

   /* encode access mode */
   tmpByte = ( egIe->t.uci.accMode && 0x03) << 0x06;
   /* encode CMI */
   tmpByte |= egIe->t.uci.isCMIPres * EG_MASK_BIT1;
   /* encode LCSG */
   tmpByte |= egIe->t.uci.isLCSGPres * EG_MASK_BIT2;

   EG_ENC_U8(tmpByte, mBuf);

   RETVALUE(retValue);
}/* egEncEG_UCI */

/***********************************************************************
 *   Fun:   egEncEG_CSG_INFO_REP_ACT 
 *   Desc:  Encodes the IE data type CSG information reproting action
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_CSG_INFO_REP_ACT 
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_CSG_INFO_REP_ACT(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;

   /* encode UCICSG */
   tmpByte |= egIe->t.csgInfoReqAct.isUCICSGPres * EG_MASK_BIT1;
   /* encode UCISHC */
   tmpByte |= egIe->t.csgInfoReqAct.isUCISHCPres * EG_MASK_BIT2;
   /* encode UCIUHC */
   tmpByte |= egIe->t.csgInfoReqAct.isUCIUHCPres * EG_MASK_BIT3;

   EG_ENC_U8(tmpByte, mBuf);

   RETVALUE(retValue);
}/* egEncEG_CSG_INFO_REP_ACT */

#endif /* EG_REL_930 */

#ifdef EG_REL_AC0
/***********************************************************************
 *   Fun:   egEncEG_ARP
 *   Desc:  Encodes the IE data type ARP
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_ARP
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_ARP(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   /* encode PL */
   tmpByte = ( egIe->t.arp.PL & EG_MASK_LOW4) << EG_SHIFT_2;
   /* encode PVI */
   tmpByte |= (egIe->t.arp.isPVIPres * EG_MASK_BIT1);
   /* encode PCI */
   tmpByte |= (egIe->t.arp.isPCIPres * EG_MASK_BIT7);

   EG_ENC_U8(tmpByte, mBuf);

   RETVALUE(retValue);
}/* egEncEG_ARP */


/***********************************************************************
 *   Fun:   egEncEG_STNSR
 *   Desc:  Encodes the IE data type STNSR
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_STNSR
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_STNSR(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   /* encode extension */
   tmpByte = ( egIe->t.stnsr.ext) << EG_SHIFT_7;
   /* encode nature of address */
   tmpByte |= egIe->t.stnsr.NatOfAdd << EG_SHIFT_4;
/* encode numbering plan indicator */
   tmpByte |= egIe->t.stnsr.NPI;

   EG_ENC_U8(tmpByte, mBuf);
   egEncEGGeneric_BCD(egIe->t.stnsr.val,&egIe->t.stnsr.length, mBuf); /* encode STN-SR val */
   RETVALUE(retValue);
}/* egEncEG_EPCTMR */


/***********************************************************************
 *   Fun:   egEncEG_THROTTLING
 *   Desc:  Encodes the IE data type THROTTLING
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_THROTTLING
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_THROTTLING(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   /* encode timer unit */
   tmpByte = ( egIe->t.egThrot.thrDelayUnit & EG_MASK_LOW4) << EG_SHIFT_4;
   /* encode timer value */
   tmpByte |= egIe->t.egThrot.thrDelayValue;

   EG_ENC_U8(tmpByte, mBuf);

   EG_ENC_U8(egIe->t.egThrot.thrFactor, mBuf);

   RETVALUE(retValue);
}/* egEncEG_THROTTLING */





/***********************************************************************
 *   Fun:   egEncEG_EPCTMR
 *   Desc:  Encodes the IE data type EPC TMR
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_EPC_TMR
(
EgIe   *egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_EPC_TMR(egIe, mBuf)
EgIe   *egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   /* encode timer unit */
   tmpByte = ( egIe->t.epcTmr.tmrUnit & EG_MASK_LOW3) << EG_SHIFT_5;
   /* encode timer value */
   tmpByte |= egIe->t.epcTmr.tmrVal;

   EG_ENC_U8(tmpByte, mBuf);

   RETVALUE(retValue);
}/* egEncEG_EPCTMR */

/***********************************************************************
 *   Fun:   egEncEG_CNG_TO_RPT_FLG
 *   Desc:  Encodes the IE data type CNG_TO_RPT_FLG
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_CNG_TO_RPT_FLG
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_CNG_TO_RPT_FLG(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16  retVal = ROK; /* Return value used to know the successful parsing */
   U8   cngRptFlag = 0;

   /********************************************
    * Change to report flag IE is 1 octet      *
    *                                          *
    * 1 Bit -. SNCR(Serving Network Change     *
    *               To Report)                 *
    * 2 Bit - TZCR(Time Zone Change To Report  *
    * 3 -8 Bits - SPARE
    **************************************/
   
   /******************************************
    * eg013.201 - Release 10 upgrade changes *
    ******************************************/
/* encode SNCR */
   cngRptFlag |= egIe->t.cngRptFlg.isSNCRPres * EG_MASK_BIT1;
   /* encode TZCR */
   cngRptFlag |= egIe->t.cngRptFlg.isTZCRPres * EG_MASK_BIT2;

   retVal = SAddPstMsg(cngRptFlag, mBuf);


   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egEncEG_MDT_CONF
 *   Desc:  Encodes the IE data type MDT configuration
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_MDT_CONF
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_MDT_CONF(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retValue = ROK;

   /* encode Job Type */
   EG_ENC_U8(egIe->t.mdtConf.jobType, mBuf);

   /* Enocde List of Measurments */
   SAddPstMsgMult(egIe->t.mdtConf.listOfMsmt, EG_LEN_FOUR, mBuf);

   /* Enocde Reporting Trigger */
   EG_ENC_U8(egIe->t.mdtConf.rptTrigger, mBuf);
   /* Enocde Report Interval */
   EG_ENC_U8(egIe->t.mdtConf.rptInterval, mBuf);

   /* Encode Report Amount */
   EG_ENC_U8(egIe->t.mdtConf.rptAmount, mBuf);

   /* Encode Event Threshold for RSRP */
   EG_ENC_U8(egIe->t.mdtConf.evtThrRsrp, mBuf);

   /* Encode Event Threshold for RSRQ */
   EG_ENC_U8(egIe->t.mdtConf.evtThrRsrq, mBuf);

   /* Encode length of Area Scope */
   EG_ENC_U8(egIe->t.mdtConf.areaScope.length, mBuf);

   /* Encode Area Scope */
   EG_ENC_STR(mBuf, egIe->t.mdtConf.areaScope.val, 
                 egIe->t.mdtConf.areaScope.length, 
                 EGT_MAX_STR_LEN, retValue);

   RETVALUE(retValue);
}

/***********************************************************************
 *   Fun:   egEncEG_ADD_MM_CNTXT_SRVCC
 *   Desc:  Encodes the IE data type EgMMCntxtForSrvcc
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_ADD_MM_CNTXT_SRVCC
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_ADD_MM_CNTXT_SRVCC(egIe,mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16 retVal = ROK;
   U8  supportedCodecLen = 0;
   U8  count = 0;

   /* encode Class Mark2 len */
   EG_ENC_U8(egIe->t.mmCntxtForSrvcc.classMk2Len, mBuf);

   /* encode Class Mark2 */
   EG_ENC_STR(mBuf, egIe->t.mmCntxtForSrvcc.classMk2, 
                 egIe->t.mmCntxtForSrvcc.classMk2Len, 
                 EG_LEN_THREE, retVal);

   /* encode Class Mark3 len */
   EG_ENC_U8(egIe->t.mmCntxtForSrvcc.classMk3Len, mBuf);

   /* encode Class Mark3 */
   EG_ENC_STR(mBuf, egIe->t.mmCntxtForSrvcc.classMk3, 
                 egIe->t.mmCntxtForSrvcc.classMk3Len, 
                 EG_LEN_34, retVal);


   for(count = 0; 
         count < egIe->t.mmCntxtForSrvcc.numOfcodecList; 
         count++)
   {
      /* Here only bit map values length is considered */
      supportedCodecLen += 
         egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen;
   }
   
   /* Here the sys Id(U8) and bitmap length(U8) field is also considered */
   supportedCodecLen += (egIe->t.mmCntxtForSrvcc.numOfcodecList * 2); 

   EG_ENC_U8(supportedCodecLen, mBuf);

   for(count = 0; 
         count < egIe->t.mmCntxtForSrvcc.numOfcodecList; 
         count++)
   {
      /* Encode System identification */
      EG_ENC_U8(egIe->t.mmCntxtForSrvcc.codec[count].sysId, mBuf);
      /* Encode Bit Map Length */
      EG_ENC_U8(egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen, mBuf);

      /* Encode Bit Map value */
      EG_ENC_STR(mBuf, egIe->t.mmCntxtForSrvcc.codec[count].bitMap, 
            egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen, 
            EG_LEN_TWO, retVal);
   }
 
   RETVALUE(retVal);
} /* End of egEncEG_ADD_MM_CNTXT_SRVCC */

#endif /* EG_REL_AC0 */


/***********************************************************************
 *   Fun:   egDecEG_UNSIGNED_8
 *   Desc:  Decodes the IE data type U8
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UNSIGNED_8
(
EgIe* egIe,
Buffer *mBuf,
U16 ieLen
)
#else
PRIVATE S16 egDecEG_UNSIGNED_8(egIe,mBuf,ieLen)
EgIe* egIe;
Buffer *mBuf;
U16 ieLen;
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */

   SRemPreMsg(&egIe->t.valUnSgnd8, mBuf);
   RETVALUE(retVal);
} 
/***********************************************************************
 *   Fun:   egDecEG_UNSIGNED_16
 *   Desc:  Decodes the IE data type U16
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UNSIGNED_16
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_UNSIGNED_16(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_DEC_U16(egIe->t.valUnSgnd16, mBuf);
   RETVALUE(retVal);
} 
/***********************************************************************
 *   Fun:   egDecEG_UNSIGNED_32
 *   Desc:  Decodes the IE data type U32
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UNSIGNED_32
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_UNSIGNED_32(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_DEC_U32(egIe->t.valUnSgnd32, mBuf);
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_STRING_8
 *   Desc:  Decodes the IE data type string_8
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STRING_8
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_STRING_8(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   egIe->t.valStr8.length = ieLen;
   EG_DEC_STR(mBuf, egIe->t.valStr8.val, egIe->t.valStr8.length, EG_LEN_EIGHT, retVal);
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_STRING_16
 *   Desc:  Decodes the IE data type STRING_16
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STRING_16
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_STRING_16(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
    egIe->t.valStr16.length = ieLen;
    EG_DEC_STR(mBuf, egIe->t.valStr16.val, egIe->t.valStr16.length, EG_LEN_SIXTEEN, retVal);
    RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_STRING_32
 *   Desc:  Decodes the IE  data type STRING_32
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STRING_32
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_STRING_32(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   egIe->t.valStr32.length = ieLen;
   EG_DEC_STR(mBuf, egIe->t.valStr32.val, egIe->t.valStr32.length, EG_LEN_32, retVal);
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_STRING_132
 *   Desc:  Decodes the IE data type STRING_132
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STRING_132
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_STRING_132(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   egIe->t.valStr132.length = ieLen;
   EG_DEC_STR(mBuf, egIe->t.valStr132.val, egIe->t.valStr132.length, EG_LEN_132, retVal);
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_STRING_400
 *   Desc:  Decodes the IE data type STRING_400
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STRING_400
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_STRING_400(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   egIe->t.valStr400.length = ieLen;
   EG_DEC_STR(mBuf, egIe->t.valStr400.val, egIe->t.valStr400.length, EG_LEN_400, retVal);
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_STRING_MAX
 *   Desc:  Decodes the IE data type STRING_MAX
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STRING_MAX
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_STRING_MAX(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   egIe->t.valStr.length = ieLen;
   EG_DEC_STR(mBuf, egIe->t.valStr.val, egIe->t.valStr.length, EG_MAX_STR_LEN, retVal);
   RETVALUE(retVal);
}


/***********************************************************************
 *   Fun:   egDecEG_SNETWORK
 *   Desc:  Decodes the IE data type SNETWORK
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_SNETWORK
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_SNETWORK(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_DEC_NETID(egIe->t.sNwk,mBuf);
   ieLen -= EG_LEN_THREE;

   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_INDICATION
 *   Desc:  Decodes the IE data type INDICATION
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_INDICATION
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_INDICATION(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   Data     dumBytes[EG_MAX_BYTES];

   SRemPreMsgMult(dumBytes, ieLen, mBuf);

   egIe->t.indication.isSGWCIPres = ((dumBytes[0] & EG_MASK_BIT1) > 0)?1:0;
   egIe->t.indication.isISRAIPres = ((dumBytes[0] & EG_MASK_BIT2) > 0)?1:0;
   egIe->t.indication.isISRSIPres = ((dumBytes[0] & EG_MASK_BIT3) > 0)?1:0;
   egIe->t.indication.isOIPres = ((dumBytes[0] & EG_MASK_BIT4) > 0)?1:0;
   egIe->t.indication.isDFIPres = ((dumBytes[0] & EG_MASK_BIT5) > 0)?1:0;
   egIe->t.indication.isHIPres = ((dumBytes[0] & EG_MASK_BIT6) > 0)?1:0;
   egIe->t.indication.isDTFPres = ((dumBytes[0] & EG_MASK_BIT7) > 0)?1:0;
   egIe->t.indication.isDAFPres = ((dumBytes[0] & EG_MASK_BIT8) > 0)?1:0;

   egIe->t.indication.isMSVPres = ((dumBytes[1] & EG_MASK_BIT1) > 0)?1:0;
   egIe->t.indication.isSIPres = ((dumBytes[1] & EG_MASK_BIT2) > 0)?1:0;
   egIe->t.indication.isPTPres = ((dumBytes[1] & EG_MASK_BIT3) > 0)?1:0;
   egIe->t.indication.isPPres = ((dumBytes[1] & EG_MASK_BIT4) > 0)?1:0;
   egIe->t.indication.isCRSIPres = ((dumBytes[1] & EG_MASK_BIT5) > 0)?1:0;
   egIe->t.indication.isCFSIPres = ((dumBytes[1] & EG_MASK_BIT6) > 0)?1:0;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egIe->t.indication.isUIMSIPres = ((dumBytes[1] & EG_MASK_BIT7) > 0)?1:0;
#endif

#ifdef EG_REL_AC0
   if(ieLen == EG_LEN_THREE)
   {
      egIe->t.indication.isSQCIPres = ((dumBytes[1] & EG_MASK_BIT8) > 0)?1:0;
      egIe->t.indication.isCCRSIPres = ((dumBytes[2] & EG_MASK_BIT1) > 0)?1:0;
      egIe->t.indication.isISRAUPres = ((dumBytes[2] & EG_MASK_BIT2) > 0)?1:0;
      egIe->t.indication.isMBMDTPres = ((dumBytes[2] & EG_MASK_BIT3) > 0)?1:0;
      egIe->t.indication.isS4AFPres = ((dumBytes[2] & EG_MASK_BIT4) > 0)?1:0;
      egIe->t.indication.isS6AFPres = ((dumBytes[2] & EG_MASK_BIT5) > 0)?1:0;
   }
#endif

   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_CAUSE   
 *   Desc:  Decodes the IE data type EMB CAUSE
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_CAUSE
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_CAUSE(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   /* ccpu00137627 */
   U8  pceBceCs = 0;

   /* decode cause value */
   SRemPreMsg(&egIe->t.cause.causeVal, mBuf);
   ieLen--;
   /* decode cause source,bce,pce */
   /* ccpu00137627 */
   SRemPreMsg(&pceBceCs, mBuf);
   ieLen--;

   egIe->t.cause.ieCS = (pceBceCs & EG_MASK_BIT1)?1:0;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egIe->t.cause.bce  = (pceBceCs & EG_MASK_BIT2)?1:0;
   egIe->t.cause.pce  = (pceBceCs & EG_MASK_BIT3)?1:0;
#endif

   /* ccpu00135385 : offending IE is not mandatory */
   if (EG_LEN_FOUR == ieLen)
   {
  /* decode offending IE type */
   SRemPreMsg(&egIe->t.cause.ieType, mBuf);

  /* decode offending IE legth */
   EG_DEC_U16(egIe->t.cause.ieLength, mBuf);

  /* decode offending IE legth */
   SRemPreMsg(&egIe->t.cause.ieInst, mBuf);
   egIe->t.cause.ieInst = egIe->t.cause.ieInst & EG_MASK_LOW4;
      ieLen-= EG_LEN_FOUR;
   }
   else if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retVal);

}
/***********************************************************************
 *   Fun:   egDecEG_PVT_EXTN
 *   Desc:  Decodes the IE data type PVT EXTN
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_PVT_EXTN
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_PVT_EXTN(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   EG_DEC_U16(egIe->t.pvtExt.entId, mBuf);
   /* call the EgStr132 for decoding the remaining bytes to prop val */
   egIe->t.pvtExt.propVal.length = (ieLen - EG_LEN_TWO);

   if(egIe->t.pvtExt.propVal.length)
   {
   EG_DEC_STR( mBuf, egIe->t.pvtExt.propVal.val,  egIe->t.pvtExt.propVal.length, EG_LEN_132, retVal);
   }
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_TRC_INFO
 *   Desc:  Decodes the IE data type TRC INFO
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_TRC_INFO
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_TRC_INFO(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{

   S16 retVal = ROK; /* Return value used to know the successful parsing */

   EG_DEC_NETID(egIe->t.trcInfo.sNwk,mBuf);
   /* eg009.201 - Magic number replaced with Macro */
   ieLen -= EG_LEN_THREE;
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
   EG_DEC_U24(egIe->t.trcInfo.trcId, mBuf); /* removed S2B and S2A support flags */
   ieLen -= EG_LEN_THREE;

   SRemPreMsgMult(egIe->t.trcInfo.trigEvnts,EG_LEN_NINE, mBuf);
   ieLen -= EG_LEN_NINE;
   EG_DEC_U16(egIe->t.trcInfo.lstNETypes, mBuf);
   ieLen -= EG_LEN_TWO;
   EG_DEC_U8(egIe->t.trcInfo.sesnTrcDep, mBuf);
   ieLen -= EG_LEN_ONE;
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
   SRemPreMsgMult(egIe->t.trcInfo.lstOfIntfs,EG_LEN_TWELVE, mBuf);
   ieLen -= EG_LEN_TWELVE;

  egIe->t.trcInfo.trcColEntAddr.type = (ieLen == CM_INET_IPV4ADDR_SIZE) ? CM_TPTADDR_IPV4 : CM_TPTADDR_IPV6;

   switch (egIe->t.trcInfo.trcColEntAddr.type)
   {
      case CM_TPTADDR_IPV4:
      {
         EG_DEC_U32(egIe->t.trcInfo.trcColEntAddr.u.ipv4, mBuf);
         ieLen -= EG_LEN_FOUR;
         break;
      } /* end of case */
#ifdef EG_IPV6_SUPPORTED
      case CM_TPTADDR_IPV6:
      {
         SRemPreMsgMult(egIe->t.trcInfo.trcColEntAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         ieLen -= EG_LEN_SIXTEEN;
         break;
      } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
      default:
      {
         RETVALUE(RFAILED);
      } /* end of case */
   }

   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_BEARER_FLAG
 *   Desc:  Decodes the IE data type BEARER FLAG
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_BEARER_FLAG
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_BEARER_FLAG(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U8       tmpByte = 0;         /* Holds 1 byte of data for decoding  */

   SRemPreMsg(&tmpByte, mBuf);
   egIe->t.bearFlags.isPPCPres = tmpByte & EG_MASK_BIT1;
   egIe->t.bearFlags.isVBPres = ((tmpByte & EG_MASK_BIT2) >> 1);
#ifdef EG_REL_AC0
   egIe->t.bearFlags.isASIPres = ((tmpByte & EG_MASK_BIT4) >> 3); /* Added as part of Release 10 */
#endif
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_FTEID
 *   Desc:  Decodes the IE data type FTEID
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/

#ifdef ANSI
PRIVATE S16 egDecEG_FTEID
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_FTEID(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{

   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U8       tmpByte = 0;         /* Holds 1 byte of data for decoding  */
   Data     dumBytes[EG_MAX_BYTES]; /* Holds the Data for decoding */

   /*************************************
   * F-TEID  Ie is having the fields:    *
   * one octet: 1-5bits intfType         *
   * one octet: 6th bit EBIF pres        *
   * one octet: 7th bit V6 pres          *
   * one octet: 8th bit V4 pres          *
   * four octets of TEID value           *
   * if 7th bit is set then ipv6 addr    *
   * else 8th bit is set then ipv4 addr  *
   * ************************************/
   SRemPreMsg(&tmpByte,mBuf);
   ieLen -= EG_LEN_ONE;
   
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
#if defined(EG_S2A_SUPP)
   egIe->t.fTeid.intfType = tmpByte & EG_MASK_FTEID_INTFTYPE_REL11;
#else
#ifdef EG_REL_AC0
   egIe->t.fTeid.intfType = tmpByte & EG_MASK_FTEID_INTFTYPE_REL10; /* added for release 10 */
#else
   egIe->t.fTeid.intfType = tmpByte & EG_MASK_FTEID_INTFTYPE;
#endif
#endif

   EG_DEC_U32(egIe->t.fTeid.teid, mBuf);
   ieLen -= 4;
   /* ccpu00137940 */
   if(tmpByte & EG_MASK_BIT8)
   {
      /* decode the ipv4 address */
      egIe->t.fTeid.isIPv4Pres = TRUE;
      SRemPreMsgMult(egIe->t.fTeid.ip4Addr, EG_LEN_FOUR, mBuf);
      ieLen -= 4;   /* IPV4 is of 4bytes and 1byte is for 1st octet */
   }
   if(tmpByte & EG_MASK_BIT7)
   {
      /* decode the IPV6 address */
      egIe->t.fTeid.isIPv6Pres = TRUE;
      SRemPreMsgMult(egIe->t.fTeid.ipv6Address, EG_LEN_SIXTEEN, mBuf);
      ieLen -= 16;   /* IPV6 is of 16bytes and 1Dece is for 1st octet */
   }
   /***********************************************
    * Check if there are any spare bits remove it *
    ***********************************************/
   if( ieLen)
   {
      SRemPreMsgMult(dumBytes, ieLen, mBuf);
   }
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_ULI_DATA 
 *   Desc:  Decodes the IE data type ULI
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_ULI_DATA
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_ULI_DATA(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U8  tmpByte = 0;         /* Holds 1 byte of data for decoding  */

   SRemPreMsg(&tmpByte,mBuf);
   ieLen--;
   /* Check if the CGI presence is there or not */
   if((ieLen >= EG_LEN_SEVEN) && (tmpByte & EG_MASK_BIT1))
   {
      /* decode the MCC,MNC,LAC,CI fields */
      EG_DEC_NETID(egIe->t.uli.cgi.sNwk,mBuf);
      EG_DEC_U16(egIe->t.uli.cgi.lac,mBuf);
      EG_DEC_U16(egIe->t.uli.cgi.ci,mBuf);
      egIe->t.uli.isCGIPres = TRUE;
      ieLen -= EG_LEN_SEVEN;
   }
   /* Check if the SAI presence is there or not */
   if((ieLen >= EG_LEN_SEVEN) && (tmpByte & EG_MASK_BIT2))
   {
      /* decode the MCC,MNC,LAC,SAI fields */
      EG_DEC_NETID(egIe->t.uli.sai.sNwk,mBuf);
      EG_DEC_U16(egIe->t.uli.sai.lac,mBuf);
      EG_DEC_U16(egIe->t.uli.sai.sac,mBuf);
      egIe->t.uli.isSAIPres = TRUE;
      ieLen -= EG_LEN_SEVEN;
   }
   /* Check if the RAI presence is there or not */
   if((ieLen >= EG_LEN_SEVEN) && (tmpByte & EG_MASK_BIT3))
   {
      /* decode the MCC,MNC,LAC,RAI fields */
      EG_DEC_NETID(egIe->t.uli.rai.sNwk,mBuf);
      EG_DEC_U16(egIe->t.uli.rai.lac,mBuf);
      EG_DEC_U16(egIe->t.uli.rai.rac,mBuf);
      egIe->t.uli.isRAIPres = TRUE;
      ieLen -= EG_LEN_SEVEN;
   }
   /* Check if the TAI presence is there or not */
   if((ieLen >= EG_LEN_FIVE) && (tmpByte & EG_MASK_BIT4))
   {
      /* decode the MCC,MNC,TAC fields */
      EG_DEC_NETID(egIe->t.uli.tai.sNwk,mBuf);
      EG_DEC_U16(egIe->t.uli.tai.tac,mBuf);
      egIe->t.uli.isTAIPres = TRUE;
      ieLen -= EG_LEN_FIVE;
   }
   /**********************************************
    * Check if the ECGI presence is there or not *
    **********************************************/
   if((ieLen >= EG_LEN_SEVEN) && (tmpByte & EG_MASK_BIT5))
   {
      /* decode the MCC,MNC,ECGI fields */
      EG_DEC_NETID(egIe->t.uli.ecgi.sNwk,mBuf);
      EG_DEC_U32(egIe->t.uli.ecgi.ecId, mBuf);
      /**************************************
      * EG_MASK_ECI:mask value  0x0fffffff  *
      ***************************************/
      egIe->t.uli.ecgi.ecId &=  EG_MASK_ECI;
      egIe->t.uli.isECGIPres = TRUE;
      ieLen -= EG_LEN_SEVEN;
   }
#ifdef EG_REL_AC0
   /**********************************************
    * Check if the LAI presence is there or not * // added as per release 10
    **********************************************/
   if((ieLen >= EG_LEN_FIVE) && (tmpByte & EG_MASK_BIT6))
   {
      /* decode the MCC,MNC,LAI fields */
      EG_DEC_NETID(egIe->t.uli.lai.sNwk,mBuf);
      EG_DEC_U16(egIe->t.uli.lai.lac,mBuf);
      egIe->t.uli.isLAIPres = TRUE;
      ieLen -= EG_LEN_FIVE;
   }
#endif
   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_FQOS
 *   Desc:  Decodes the IE data type FQOS
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_FQOS
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_FQOS(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   Data     dumBytes[EG_MAX_BYTES]; /* Holds the Data for decoding */
   EG_DEC_FQOS((&egIe->t.fQos), mBuf);
   /* calculate the remaining IE */
   ieLen -= EG_QOS_LEN;
   if(ieLen)
   {
      /* remove the remaining spare bits */
      SRemPreMsgMult(dumBytes,ieLen, mBuf);
   }
   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_BQOS_IEDATA
 *   Desc:  Decodes the IE data type BQOS 
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/

#ifdef ANSI
PRIVATE S16 egDecEG_BQOS_IEDATA
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_BQOS_IEDATA(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U8  tmpByte = 0;
   Data dumBytes[EG_MAX_BYTES]; /* Holds the Data for decoding */


   SRemPreMsg(&tmpByte,mBuf);

  /* ccpu00137026 : PCI/PVI updated */
   egIe->t.bQos.isPVIPres = (tmpByte & EG_MASK_BIT1)?1:0;
   egIe->t.bQos.isPCIPres = (tmpByte & EG_MASK_BIT7)?1:0;

   tmpByte >>= EG_SHIFT_2; /* Right shift twice */
   egIe->t.bQos.prityLvl = tmpByte & 0x0F;
    
   /* eg013.201:the length of the egDatStr8 needs to be filled explicitily */
   SRemPreMsg(&egIe->t.bQos.qci,mBuf);
   SRemPreMsgMult(egIe->t.bQos.upMBR.val, EG_LEN_FIVE, mBuf);
   egIe->t.bQos.upMBR.length = EG_LEN_FIVE;
   SRemPreMsgMult(egIe->t.bQos.dnMBR.val, EG_LEN_FIVE, mBuf);
   egIe->t.bQos.dnMBR.length = EG_LEN_FIVE;
   SRemPreMsgMult(egIe->t.bQos.upGBR.val, EG_LEN_FIVE, mBuf);
   egIe->t.bQos.upGBR.length = EG_LEN_FIVE;
   SRemPreMsgMult(egIe->t.bQos.dnGBR.val, EG_LEN_FIVE, mBuf);
   egIe->t.bQos.dnGBR.length = EG_LEN_FIVE;
   ieLen -= EG_LEN_22; /* 22 */
   if(ieLen)
   {
      /* remove the remaining spare bits */
      SRemPreMsgMult(dumBytes,ieLen, mBuf);
   }

   RETVALUE(retVal);
}
/***********************************************************************
 *   Fun:   egDecEG_PAA_IEDATA
 *   Desc:  Decodes the IE data type PAA
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_PAA_IEDATA
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_PAA_IEDATA(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   SRemPreMsg(&egIe->t.paa.pdnType, mBuf);
   ieLen--;
   /* mask the high 4 bits as pdn type 4bits */
   egIe->t.paa.pdnType = egIe->t.paa.pdnType & EG_MASK_LOW4;
   switch(egIe->t.paa.pdnType)
   {
      case 1:
      {
        /**************************************************************
         * if the pdn type is 1, then the pdn address is of type Ipv4 *
         * decode the ipv4 address which is of 4 octets length        *
         **************************************************************/
         egIe->t.paa.pdnPrefix =0;
         SRemPreMsgMult(egIe->t.paa.u.pdnIpv4Addr, EG_LEN_FOUR, mBuf);
         ieLen -= EG_LEN_FOUR;
         break;
      }
      case 2:
      {
        /******************************************************************
         * If the pdnType is 2, then first octet will tell the pdn prifix *
         * next 16octets will tell the ipv6 address                       *
         ******************************************************************/
         SRemPreMsg(&egIe->t.paa.pdnPrefix,mBuf);
         SRemPreMsgMult(egIe->t.paa.u.pdnIpv6Addr, EG_LEN_SIXTEEN, mBuf);
         ieLen -= (EG_LEN_SIXTEEN + EG_LEN_ONE);
         break;
      }
      case 3:
      {
        /******************************************************************
         * If the pdnType is 3, then first octet will tell the pdn prifix *
         * next 16octets will be the ipv6 address                         *
         * next 4 octets will be the ipv4 address                         *
         ******************************************************************/
         SRemPreMsg(&egIe->t.paa.pdnPrefix,mBuf);
         SRemPreMsgMult(egIe->t.paa.u.pdnBoth.pdnIpv6Addr, EG_LEN_SIXTEEN, mBuf);
         SRemPreMsgMult(egIe->t.paa.u.pdnBoth.pdnIpv4Addr, EG_LEN_FOUR, mBuf);
         ieLen -= (EG_LEN_SIXTEEN + EG_LEN_FOUR + EG_LEN_ONE);
         break;
      }
      default:
     {
         /* Invalid Value */
         egIe->ieInValidCause = EGT_ERR_MSG_INV_PDN_TYPE;
         retVal = EGT_ERR_MSG_INV_PDN_TYPE;
         break;
      }
   }

   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retVal);
}


/***********************************************************************
 *   Fun:   egDecEG_AUTH_QUIN
 *   Desc:  Decodes the IE data type Authentication Quintuplet
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_AUTH_QUIN
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_AUTH_QUIN(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;

   /* Decode the RAND which is of 16 octets*/
   egIe->t.authQt.rand.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.authQt.rand.val, egIe->t.authQt.rand.length, EG_LEN_SIXTEEN, retValue);

   /* decode the XRES length in single octet and XRES which can be of 16 octets */
   SRemPreMsg(&egIe->t.authQt.xresLen, mBuf);
   EG_DEC_STR(mBuf, egIe->t.authQt.xres.val, egIe->t.authQt.xresLen, EG_LEN_SIXTEEN, retValue);

   /* decode the CK, IK each is of 16 octets length */
   egIe->t.authQt.ck.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.authQt.ck.val, egIe->t.authQt.ck.length, EG_LEN_SIXTEEN, retValue);
   egIe->t.authQt.ik.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.authQt.ik.val, egIe->t.authQt.ik.length, EG_LEN_SIXTEEN, retValue);

   /* decode the AUTN */
   SRemPreMsg(&egIe->t.authQt.autnLen, mBuf);
   EG_DEC_STR(mBuf, egIe->t.authQt.autn.val, egIe->t.authQt.autnLen, EG_LEN_SIXTEEN, retValue);

   RETVALUE(ROK);
} /* end of egDecEG_AUTH_QUIN */


/***********************************************************************
 *   Fun:   egDecEG_AUTH_QUAD
 *   Desc:  Decodes the IE data type Authentication Quadruplet
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_AUTH_QUAD
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_AUTH_QUAD(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode the RAND which is of 16 octets*/
   egIe->t.quadruplet.rand.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.quadruplet.rand.val, egIe->t.quadruplet.rand.length, EG_LEN_SIXTEEN, retValue);

   /* decode the XRES length in single octet and XRES which can be of 16 octets */
   SRemPreMsg(&egIe->t.quadruplet.xresLen, mBuf);
   EG_DEC_STR(mBuf, egIe->t.quadruplet.xres.val, egIe->t.quadruplet.xresLen, EG_LEN_SIXTEEN, retValue);

   /* decode the AUTN length in single octet and AUTH which can be of 16 octets */
   SRemPreMsg(&egIe->t.quadruplet.autnLen, mBuf);
   EG_DEC_STR(mBuf, egIe->t.quadruplet.autn.val, egIe->t.quadruplet.autnLen, EG_LEN_SIXTEEN, retValue);

   /* decode the Kasme which is of 31 octets */
   egIe->t.quadruplet.kasme.length = EG_KASME_LEN;
   EG_DEC_STR(mBuf, egIe->t.quadruplet.kasme.val, egIe->t.quadruplet.kasme.length, EG_LEN_32, retValue);

   RETVALUE(ROK);
} /* end of egDecEG_AUTH_QUAD */


/***********************************************************************
 *   Fun:   egDecEG_CMP_REQ
 *   Desc:  Decodes the IE data type Complete Request Message
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_CMP_REQ
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_CMP_REQ(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode the CRM Type */
   SRemPreMsg(&egIe->t.crm.crmType, mBuf);

   /* decode the CRM Value */
   egIe->t.crm.crm.length = ieLen - 1;
   EG_DEC_STR(mBuf, egIe->t.crm.crm.val, egIe->t.crm.crm.length, EG_LEN_400, retValue);

   RETVALUE(retValue);
} /* end of egDecEG_CMP_REQ */


/***********************************************************************
 *   Fun:   egDecEG_GUTI
 *   Desc:  Decodes the IE data type GUTI
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_GUTI
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_GUTI(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16     retVal = ROK;
   /* decode the NETID */
   EG_DEC_NETID(egIe->t.guti.sNwk,mBuf);
   EG_DEC_U16(egIe->t.guti.mmeGrpId, mBuf);
   SRemPreMsg(&egIe->t.guti.mmeCode, mBuf);
   ieLen -= EG_LEN_SIX;

   if(ieLen >= EG_LEN_FOUR)
   {
   EG_DEC_U32(egIe->t.guti.mTmsi, mBuf);
      ieLen -= EG_LEN_FOUR;
   }
   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retVal);
} /* end of egDecEG_GUTI */


/***********************************************************************
 *   Fun:   egDecEG_F_CONT
 *   Desc:  Decodes the IE data type F-Container
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_F_CONT
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_F_CONT(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode the container type */
   SRemPreMsg(&egIe->t.fContainer.containerType, mBuf);

   egIe->t.fContainer.fContainer.length = ieLen - 1;
   EG_DEC_STR(mBuf, egIe->t.fContainer.fContainer.val, egIe->t.fContainer.fContainer.length, EG_LEN_400, retValue);

   RETVALUE(retValue);
} /* end of egDecEG_F_CONT */


/***********************************************************************
 *   Fun:   egDecEG_TRGTID
 *   Desc:  Decodes the IE data type Target Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_TRGTID
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_TRGTID(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode the target type */
   SRemPreMsg(&egIe->t.targetId.targetType, mBuf);

   egIe->t.targetId.targetId.length = ieLen - 1;
   EG_DEC_STR(mBuf, egIe->t.targetId.targetId.val, egIe->t.targetId.targetId.length, EG_LEN_SIXTEEN, retValue);

   RETVALUE(retValue);
} /* end of egDecEG_TRGTID */


/***********************************************************************
 *   Fun:   egDecEG_PKTFLOWID
 *   Desc:  Decodes the IE data type Packet Flow Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_PKTFLOWID
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_PKTFLOWID(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retVal = ROK;

   /* decode the EBI */
   SRemPreMsg(&egIe->t.pktFlowId.ebi, mBuf);
   ieLen--;
   egIe->t.pktFlowId.ebi &= 0x0F;
   SRemPreMsg(&egIe->t.pktFlowId.pktFlowId, mBuf);
   ieLen--;

   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retVal);
} /* end of egDecEG_PKTFLOWID */


/***********************************************************************
 *   Fun:   egDecEG_SRCID
 *   Desc:  Decodes the IE data type Cell Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_SRCID
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_SRCID(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   egIe->t.srcId.targetCellId.length = EG_LEN_EIGHT;
   EG_DEC_STR(mBuf, egIe->t.srcId.targetCellId.val, egIe->t.srcId.targetCellId.length, EG_LEN_EIGHT, retValue);

   SRemPreMsg(&egIe->t.srcId.sourceType, mBuf);

   ieLen -= (EG_LEN_EIGHT + EG_LEN_ONE);

   egIe->t.srcId.sourceId.length = ieLen;
   EG_DEC_STR(mBuf, egIe->t.srcId.sourceId.val, egIe->t.srcId.sourceId.length, EG_LEN_SIXTEEN, retValue);

   ieLen -= egIe->t.srcId.sourceId.length;

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* end of egDecEG_SRCID */

/***********************************************************************
 *   Fun:   egDecEG_GLOBAL_CNID
 *   Desc:  Decodes the IE data type Global CN Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_GLOBAL_CNID
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_GLOBAL_CNID(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;

   EG_DEC_NETID(egIe->t.gblCnId.sNwk,mBuf);
   egIe->t.gblCnId.cnId.length = EG_LEN_FOUR;
   EG_DEC_STR(mBuf, egIe->t.gblCnId.cnId.val, egIe->t.gblCnId.cnId.length, EG_LEN_FOUR, retValue);
   ieLen -= EG_LEN_SEVEN;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* end of egDecEG_GLOBAL_CNID */


/***********************************************************************
 *   Fun:   egDecEG_PDN_CONN_SET_ID
 *   Desc:  Decodes the IE data type PDN Connection Set Identifier
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_PDN_CONN_SET_ID
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_PDN_CONN_SET_ID(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   Data dumBytes[EG_MAX_BYTES];
   /* ccpu00137353  */
   S16 idx;
   SRemPreMsg(&dumBytes[0], mBuf);
   ieLen -= 1;
   egIe->t.csid.noOfCSIDs = dumBytes[0] & 0x0F;
   egIe->t.csid.nodeIdType = ((dumBytes[0] & 0xF0) >> EG_SHIFT_4);

   switch(egIe->t.csid.nodeIdType)
   {
     case 0:
       {
         egIe->t.csid.nodeId.length = EG_LEN_FOUR;
         EG_DEC_STR(mBuf, egIe->t.csid.nodeId.val, egIe->t.csid.nodeId.length,EG_LEN_SIXTEEN , retValue);
         ieLen -= EG_LEN_FOUR;
       }
       break;
     case 1:
       {
         egIe->t.csid.nodeId.length = EG_LEN_SIXTEEN;
         EG_DEC_STR(mBuf, egIe->t.csid.nodeId.val, egIe->t.csid.nodeId.length, EG_LEN_SIXTEEN, retValue);
         ieLen -= EG_LEN_SIXTEEN;
       }
       break;
     case 2:
       {
         egIe->t.csid.nodeId.length = EG_LEN_FOUR;
         EG_DEC_STR(mBuf, egIe->t.csid.nodeId.val, egIe->t.csid.nodeId.length, EG_LEN_SIXTEEN, retValue);
         ieLen -= EG_LEN_FOUR;
       }
       break;
     default:
       retValue = RFAILED;
       EG_RETVALUE(retValue);
   }

   /* ccpu00137353 */
   for(idx = 0; idx < egIe->t.csid.noOfCSIDs; idx++)
   {   
      egIe->t.csid.csid[idx].length=0x02;
      EG_DEC_STR(mBuf, egIe->t.csid.csid[idx].val, egIe->t.csid.csid[idx].length, EG_LEN_SIXTEEN, retValue);
   } 
   /* ccpu00137353 */ 
   ieLen-=( egIe->t.csid.noOfCSIDs * 0x02);
   if(ieLen)
   {
     /* remove the remaining spare bits */
     SRemPreMsgMult(dumBytes,ieLen, mBuf);
   }
   RETVALUE(retValue);
} /* end of egDecEG_PDN_CONN_SET_ID */


/***********************************************************************
 *   Fun:   egDecEG_IP_ADDR
 *   Desc:  Decodes the IE data type IP Address
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_IP_ADDR
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_IP_ADDR(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16      retValue = ROK;
   egIe->t.ipAddr.type = (ieLen == 4) ? CM_TPTADDR_IPV4 : CM_TPTADDR_IPV6;

   switch (egIe->t.ipAddr.type)
   {
      case CM_TPTADDR_IPV4:
      {
         EG_DEC_U32(egIe->t.ipAddr.u.ipv4, mBuf);
         ieLen -= EG_LEN_FOUR;
         break;
      } /* end of case */
#ifdef EG_IPV6_SUPPORTED
      case CM_TPTADDR_IPV6:
      {
         SRemPreMsgMult(egIe->t.ipAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         ieLen -= EG_LEN_SIXTEEN;
         break;
      } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
      default:
      {
         RETVALUE(RFAILED);
      } /* end of case */
   }

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retValue);
} /* end of egDecEG_IP_ADDR */


/***********************************************************************
 *   Fun:   egDecEG_GSM_KEY_TRIP
 *   Desc:  Decodes the IE data type GSM Key Triplet
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_GSM_KEY_TRIP
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_GSM_KEY_TRIP(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   S16 cnt = 0;
   U8  tempOctet = 0x00;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyT.secMode  = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.gsmKeyT.isDRXIPres = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.gsmKeyT.cksn     = (tempOctet & 0x07);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyT.nmbTrip = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.gsmKeyT.isUAMBRIPres = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.gsmKeyT.isSAMBRIPres = (tempOctet & EG_MASK_BIT1)?1:0;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyT.usedCipher = tempOctet & 0x07;

   /* decode Kc in eight octets */
   SRemPreMsgMult(egIe->t.gsmKeyT.kc, EG_LEN_EIGHT, mBuf);

   /* The Bypts from 1 to 11 should be mandatory */
   ieLen -= (EG_LEN_EIGHT + EG_LEN_THREE);

   for (cnt = 0; (cnt < egIe->t.gsmKeyT.nmbTrip) && (ieLen >= EG_LEN_28); cnt++)
   {
     SRemPreMsgMult(egIe->t.gsmKeyT.authTrip[cnt].rand, EG_LEN_SIXTEEN, mBuf);
     SRemPreMsgMult(egIe->t.gsmKeyT.authTrip[cnt].sres, EG_LEN_FOUR, mBuf);
     SRemPreMsgMult(egIe->t.gsmKeyT.authTrip[cnt].kc, EG_LEN_EIGHT, mBuf);
     ieLen -= EG_LEN_28;
   } /* end of loop */

   if(egIe->t.gsmKeyT.isDRXIPres && ieLen >= EG_LEN_TWO)
   {
   /* decode drx parameter - is of 2 octets */
   SRemPreMsgMult(egIe->t.gsmKeyT.drxParam, EG_LEN_TWO, mBuf);
      ieLen -= EG_LEN_TWO;
   }

   if(egIe->t.gsmKeyT.isSAMBRIPres && ieLen >= EG_LEN_EIGHT)
   {
   /* decode upLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyT.upSubsUEAMBR, mBuf);
   /* decode downLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyT.dnSubsUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   if(egIe->t.gsmKeyT.isUAMBRIPres && ieLen >= EG_LEN_EIGHT)
   {
   /* decode upLink used UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyT.upUsedUEAMBR, mBuf);

   /* decode downLink used UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyT.dnUsedUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   SRemPreMsg((Data*)&egIe->t.gsmKeyT.ueNtwrkCapb.length, mBuf);
   /* decode UE Network Capability - can be max of 15 octets */
   EG_DEC_STR(mBuf, egIe->t.gsmKeyT.ueNtwrkCapb.val, egIe->t.gsmKeyT.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);

   ieLen -= (egIe->t.gsmKeyT.ueNtwrkCapb.length + 1);
   /* here we are mandating the length of UE Ntwrk will be of 15 octets */

   SRemPreMsg((Data*)&egIe->t.gsmKeyT.msNtwrkCapb.length, mBuf);
   /* decode MS Network Capability - can be max of 10 octets */
   EG_DEC_STR(mBuf, egIe->t.gsmKeyT.msNtwrkCapb.val, egIe->t.gsmKeyT.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   ieLen -= (egIe->t.gsmKeyT.msNtwrkCapb.length + 1);

   SRemPreMsg((Data*)&egIe->t.gsmKeyT.meId.length, mBuf);
   /* decode MEID - is of 16 octets */
   EG_DEC_STR(mBuf, egIe->t.gsmKeyT.meId.val, egIe->t.gsmKeyT.meId.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MEID will be of 16 octets */
   ieLen -= (egIe->t.gsmKeyT.meId.length + 1); 

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyT.isUNA = (tempOctet & EG_MASK_BIT1)?1:0;
   egIe->t.gsmKeyT.isGENA = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.gsmKeyT.isGANA = (tempOctet & EG_MASK_BIT3)?1:0;
   egIe->t.gsmKeyT.isINA = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.gsmKeyT.isENA = (tempOctet & EG_MASK_BIT5)?1:0;
   egIe->t.gsmKeyT.isHNNA = (tempOctet & EG_MASK_BIT6)?1:0;
   ieLen--;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   if(ieLen)
   {
   /* decode Voice Domain Preference and UE's usage Setting length */
   SRemPreMsg(&egIe->t.gsmKeyT.vDomPrefUEUsageLen, mBuf);
      ieLen--;

   /* decode UE's usage setting (1 bit field) Bit-3
    * decode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.gsmKeyT.vDomPrefUEUsageLen)
   {
     SRemPreMsg(&tempOctet, mBuf);
     egIe->t.gsmKeyT.ueUsageSett = (tempOctet & EG_MASK_BIT3)?1:0;

         egIe->t.gsmKeyT.vDomPref = tempOctet & 0x03;
         ieLen--;
      }
   }
#endif

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* end of egDecEG_GSM_KEY_TRIP */


/***********************************************************************
 *   Fun:   egDecEG_UMTS_KEY_CIPH_QUIN
 *   Desc:  Decodes the IE data type UMTS Key, Used Cipher and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UMTS_KEY_CIPH_QUIN
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_UMTS_KEY_CIPH_QUIN(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;
   S16 cnt = 0;
   U8  tempOctet = 0x00;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyUCQt.secMode  = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.umtsKeyUCQt.isDRXIPres = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.umtsKeyUCQt.cksn     = (tempOctet & 0x07);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyUCQt.nmbQuin = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.umtsKeyUCQt.isUAMBRIPres = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.umtsKeyUCQt.isSAMBRIPres = (tempOctet & EG_MASK_BIT1)?1:0;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyUCQt.usedCipher = tempOctet & 0x07;

/* decode CK, IK each of 16 octets */
   egIe->t.umtsKeyUCQt.ck.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.ck.val, egIe->t.umtsKeyUCQt.ck.length, EG_LEN_SIXTEEN, retValue);

   egIe->t.umtsKeyUCQt.ik.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.ik.val, egIe->t.umtsKeyUCQt.ik.length, EG_LEN_SIXTEEN, retValue);

   ieLen -= (EG_LEN_SIXTEEN + EG_LEN_SIXTEEN + EG_LEN_THREE);

   for (cnt = 0; cnt < egIe->t.umtsKeyUCQt.nmbQuin; cnt++)
   {
     /* Decode the RAND which is of 16 octets*/
     egIe->t.umtsKeyUCQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].rand.val, egIe->t.umtsKeyUCQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.umtsKeyUCQt.authQuin[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].xres.val, egIe->t.umtsKeyUCQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyUCQt.authQuin[cnt].xresLen + 1);

     /* decode the CK, IK each is of 16 octets length */
     egIe->t.umtsKeyUCQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.umtsKeyUCQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].ck.val, egIe->t.umtsKeyUCQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].ik.val, egIe->t.umtsKeyUCQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_32;

     /* decode the AUTN */
     SRemPreMsg(&egIe->t.umtsKeyUCQt.authQuin[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.authQuin[cnt].autn.val, egIe->t.umtsKeyUCQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyUCQt.authQuin[cnt].autnLen + 1);

   }

   if(egIe->t.umtsKeyUCQt.isDRXIPres)
   {
      /* decode drx parameter - is of 2 octets */
      SRemPreMsgMult(egIe->t.umtsKeyUCQt.drxParam, EG_LEN_TWO, mBuf);
      ieLen -= EG_LEN_TWO;
   }

   if(egIe->t.umtsKeyUCQt.isSAMBRIPres)
   {
      /* decode upLink subscribed UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyUCQt.upSubsUEAMBR, mBuf);

      /* decode downLink subscribed UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyUCQt.dnSubsUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   if(egIe->t.umtsKeyUCQt.isUAMBRIPres)
   {
      /* decode upLink used UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyUCQt.upUsedUEAMBR, mBuf);

      /* decode downLink used UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyUCQt.dnUsedUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }


 /* decode UE Network Capability - can be max of 15 octets */
   SRemPreMsg((Data*)&egIe->t.umtsKeyUCQt.ueNtwrkCapb.length, mBuf);
   EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.ueNtwrkCapb.val, egIe->t.umtsKeyUCQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   ieLen -= (egIe->t.umtsKeyUCQt.ueNtwrkCapb.length + 1);

   SRemPreMsg((Data*)&egIe->t.umtsKeyUCQt.msNtwrkCapb.length, mBuf);
   /* decode MS Network Capability - can be max of 10 octets */
   EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.msNtwrkCapb.val, egIe->t.umtsKeyUCQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   ieLen -= (egIe->t.umtsKeyUCQt.msNtwrkCapb.length + 1);

   SRemPreMsg((Data*)&egIe->t.umtsKeyUCQt.meId.length, mBuf);
   /* decode MEID - is of 16 octets */
   EG_DEC_STR(mBuf, egIe->t.umtsKeyUCQt.meId.val, egIe->t.umtsKeyUCQt.meId.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MEID will be of 16 octets */
   ieLen -= (egIe->t.umtsKeyUCQt.meId.length + 1);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyUCQt.isUNA = (tempOctet & EG_MASK_BIT1)?1:0;
   egIe->t.umtsKeyUCQt.isGENA = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.umtsKeyUCQt.isGANA = (tempOctet & EG_MASK_BIT3)?1:0;
   egIe->t.umtsKeyUCQt.isINA = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.umtsKeyUCQt.isENA = (tempOctet & EG_MASK_BIT5)?1:0;
   egIe->t.umtsKeyUCQt.isHNNA = (tempOctet & EG_MASK_BIT6)?1:0;
   ieLen--;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   if(ieLen)
   {
   /* decode Voice Domain Preference and UE's usage Setting length */
   SRemPreMsg(&egIe->t.umtsKeyUCQt.vDomPrefUEUsageLen, mBuf);

      ieLen--;
   /* decode UE's usage setting (1 bit field) Bit-3
    * decode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.umtsKeyUCQt.vDomPrefUEUsageLen)
   {
     SRemPreMsg(&tempOctet, mBuf);
     egIe->t.umtsKeyUCQt.ueUsageSett = (tempOctet & EG_MASK_BIT3)?1:0;

         egIe->t.umtsKeyUCQt.vDomPref = tempOctet & 0x03;
         ieLen--;
      }
   }
#endif

#ifdef EG_REL_AC0
   if(ieLen)
   {
      /* decode HighBitRateThan16MbpsLen and HighBitRateThan16Mbps */
      SRemPreMsg(&egIe->t.umtsKeyUCQt.HighBitRateThan16MbpsLen, mBuf);
      SRemPreMsg(&egIe->t.umtsKeyUCQt.HighBitRateThan16Mbps, mBuf);
      ieLen -= EG_LEN_TWO;
   }
#endif

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }  
   RETVALUE(retValue);
} /* end of egDecEG_UMTS_KEY_CIPH_QUIN */


/***********************************************************************
 *   Fun:   egDecEG_GSM_KEY_CIPH_QUIN
 *   Desc:  Decodes the IE data type GSM Key, Used Cipher and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_GSM_KEY_CIPH_QUIN
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_GSM_KEY_CIPH_QUIN(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   S16 cnt = 0;
   U8  tempOctet = 0x00;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyUCQt.secMode  = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.gsmKeyUCQt.isDRXIPres = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.gsmKeyUCQt.cksn     = (tempOctet & 0x07);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyUCQt.nmbQuin = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.gsmKeyUCQt.isUAMBRIPres = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.gsmKeyUCQt.isSAMBRIPres = (tempOctet & EG_MASK_BIT1)?1:0;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyUCQt.usedCipher = tempOctet & 0x07;

   /* decode Kc in eight octets */
   SRemPreMsgMult(egIe->t.gsmKeyUCQt.kc, EG_LEN_EIGHT, mBuf);
   ieLen -= (EG_LEN_EIGHT + EG_LEN_THREE);

     for (cnt = 0; cnt < egIe->t.gsmKeyUCQt.nmbQuin; cnt++)
   {
     /* Decode the RAND which is of 16 octets*/
     egIe->t.gsmKeyUCQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].rand.val, egIe->t.gsmKeyUCQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.gsmKeyUCQt.authQuin[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].xres.val, egIe->t.gsmKeyUCQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.gsmKeyUCQt.authQuin[cnt].xresLen + 1);;

     /* decode the CK, IK each is of 16 octets length */
     egIe->t.gsmKeyUCQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.gsmKeyUCQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].ck.val, egIe->t.gsmKeyUCQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].ik.val, egIe->t.gsmKeyUCQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_32;

     /* decode the AUTN */
     SRemPreMsg(&egIe->t.gsmKeyUCQt.authQuin[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.authQuin[cnt].autn.val, egIe->t.gsmKeyUCQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.gsmKeyUCQt.authQuin[cnt].autnLen + 1);
   }

   if(egIe->t.gsmKeyUCQt.isDRXIPres)
   {
   /* decode drx parameter - is of 2 octets */
   SRemPreMsgMult(egIe->t.gsmKeyUCQt.drxParam, EG_LEN_TWO, mBuf);
      ieLen -= EG_LEN_TWO;
   }

   if(egIe->t.gsmKeyUCQt.isSAMBRIPres)
   {
   /* decode upLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyUCQt.upSubsUEAMBR, mBuf);

   /* decode downLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyUCQt.dnSubsUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   if(egIe->t.gsmKeyUCQt.isUAMBRIPres)
   {
   /* decode upLink used UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyUCQt.upUsedUEAMBR, mBuf);

   /* decode downLink used UE AMBR */
   EG_DEC_U32(egIe->t.gsmKeyUCQt.dnUsedUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   SRemPreMsg((Data*)&egIe->t.gsmKeyUCQt.ueNtwrkCapb.length, mBuf);
   /* decode UE Network Capability - can be max of 15 octets */
   EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.ueNtwrkCapb.val, egIe->t.gsmKeyUCQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   ieLen -= (egIe->t.gsmKeyUCQt.ueNtwrkCapb.length + 1); 

   SRemPreMsg((Data*)&egIe->t.gsmKeyUCQt.msNtwrkCapb.length, mBuf);
   /* decode MS Network Capability - can be max of 10 octets */
   EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.msNtwrkCapb.val, egIe->t.gsmKeyUCQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   ieLen -= (egIe->t.gsmKeyUCQt.msNtwrkCapb.length + 1);

   SRemPreMsg((Data*)&egIe->t.gsmKeyUCQt.meId.length, mBuf);
   /* decode MEID - is of 16 octets */
   EG_DEC_STR(mBuf, egIe->t.gsmKeyUCQt.meId.val, egIe->t.gsmKeyUCQt.meId.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MEID will be of 16 octets */
   ieLen -= (egIe->t.gsmKeyUCQt.meId.length + 1); 


   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.gsmKeyUCQt.isUNA = (tempOctet & EG_MASK_BIT1)?1:0;
   egIe->t.gsmKeyUCQt.isGENA = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.gsmKeyUCQt.isGANA = (tempOctet & EG_MASK_BIT3)?1:0;
   egIe->t.gsmKeyUCQt.isINA = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.gsmKeyUCQt.isENA = (tempOctet & EG_MASK_BIT5)?1:0;
   egIe->t.gsmKeyUCQt.isHNNA = (tempOctet & EG_MASK_BIT6)?1:0;
   ieLen--;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   if(ieLen)
   {
   /* decode Voice Domain Preference and UE's usage Setting length */
   SRemPreMsg(&egIe->t.gsmKeyUCQt.vDomPrefUEUsageLen, mBuf);
      ieLen--;

   /* decode UE's usage setting (1 bit field) Bit-3
    * decode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.gsmKeyUCQt.vDomPrefUEUsageLen)
   {
     SRemPreMsg(&tempOctet, mBuf);
     egIe->t.gsmKeyUCQt.ueUsageSett = (tempOctet & EG_MASK_BIT3)?1:0;

         egIe->t.gsmKeyUCQt.vDomPref = tempOctet & 0x03;
         ieLen--;
      }
   }
#endif
#ifdef EG_REL_AC0
   if(ieLen)
   {
      /* decode of HighBitRateThan16MbpsLen and HighBitRateThan16Mbps as per release 10 */
      SRemPreMsg(&egIe->t.gsmKeyUCQt.HighBitRateThan16MbpsLen, mBuf);
      SRemPreMsg(&egIe->t.gsmKeyUCQt.HighBitRateThan16Mbps, mBuf);
      ieLen -= EG_LEN_TWO;
   }
#endif
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }  
   RETVALUE(retValue);
} /* end of egDecEG_GSM_KEY_CIPH_QUIN */


/***********************************************************************
 *   Fun:   egDecEG_UMTS_KEY_QUIN
 *   Desc:  Decodes the IE data type UMTS Key, and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UMTS_KEY_QUIN
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_UMTS_KEY_QUIN(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;
   S16 cnt = 0;
   U8  tempOctet = 0x00;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyQt.secMode  = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.umtsKeyQt.isDRXIPres = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.umtsKeyQt.ksi = (tempOctet & 0x07);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyQt.nmbQuin = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.umtsKeyQt.isUAMBRIPres = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.umtsKeyQt.isSAMBRIPres = (tempOctet & EG_MASK_BIT1)?1:0;

  /* 7th one is spare octect */
   SRemPreMsg(&tempOctet, mBuf);
   tempOctet = 0x00;

/* decode CK, IK each of 16 octets */
   egIe->t.umtsKeyQt.ck.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.ck.val, egIe->t.umtsKeyQt.ck.length, EG_LEN_SIXTEEN, retValue);

   egIe->t.umtsKeyQt.ik.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.ik.val, egIe->t.umtsKeyQt.ik.length, EG_LEN_SIXTEEN, retValue);

   ieLen -= (EG_LEN_SIXTEEN + EG_LEN_SIXTEEN + EG_LEN_THREE);
   for (cnt = 0; cnt < egIe->t.umtsKeyQt.nmbQuin; cnt++)
   {
     /* Decode the RAND which is of 16 octets*/
     egIe->t.umtsKeyQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].rand.val, egIe->t.umtsKeyQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.umtsKeyQt.authQuin[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].xres.val, egIe->t.umtsKeyQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQt.authQuin[cnt].xresLen + 1);

     /* decode the CK, IK each is of 16 octets length */
     egIe->t.umtsKeyQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.umtsKeyQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].ck.val, egIe->t.umtsKeyQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].ik.val, egIe->t.umtsKeyQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_32;

     /* decode the AUTN */
     SRemPreMsg(&egIe->t.umtsKeyQt.authQuin[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.authQuin[cnt].autn.val, egIe->t.umtsKeyQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQt.authQuin[cnt].autnLen + 1);
   }

   if(egIe->t.umtsKeyQt.isDRXIPres)
   {
      /* decode drx parameter - is of 2 octets */
      SRemPreMsgMult(egIe->t.umtsKeyQt.drxParam, EG_LEN_TWO, mBuf);
      ieLen -= EG_LEN_TWO;
   }

   if(egIe->t.umtsKeyQt.isSAMBRIPres)
   {
      /* decode upLink subscribed UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyQt.upSubsUEAMBR, mBuf);

      /* decode downLink subscribed UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyQt.dnSubsUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   if(egIe->t.umtsKeyQt.isUAMBRIPres)
   {
      /* decode upLink used UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyQt.upUsedUEAMBR, mBuf);

      /* decode downLink used UE AMBR */
      EG_DEC_U32(egIe->t.umtsKeyQt.dnUsedUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

 /* decode UE Network Capability - can be max of 15 octets */
     SRemPreMsg((Data*)&egIe->t.umtsKeyQt.ueNtwrkCapb.length, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.ueNtwrkCapb.val, egIe->t.umtsKeyQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQt.ueNtwrkCapb.length + 1);

     /* decode MS Network Capability - can be max of 10 octets */
     SRemPreMsg((Data*)&egIe->t.umtsKeyQt.msNtwrkCapb.length, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.msNtwrkCapb.val, egIe->t.umtsKeyQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
     /* here we are mandating the length of MS Ntwrk will be of 10 octets */
     ieLen -= (egIe->t.umtsKeyQt.msNtwrkCapb.length + 1);

     /* decode MEID - is of 16 octets */
     SRemPreMsg((Data*)&egIe->t.umtsKeyQt.meId.length, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQt.meId.val, egIe->t.umtsKeyQt.meId.length, EG_LEN_SIXTEEN, retValue);
     /* here we are mandating the length of MEID will be of 16 octets */
     ieLen -= (egIe->t.umtsKeyQt.meId.length + 1);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyQt.isUNA = (tempOctet & EG_MASK_BIT1)?1:0;
   egIe->t.umtsKeyQt.isGENA = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.umtsKeyQt.isGANA = (tempOctet & EG_MASK_BIT3)?1:0;
   egIe->t.umtsKeyQt.isINA = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.umtsKeyQt.isENA = (tempOctet & EG_MASK_BIT5)?1:0;
   egIe->t.umtsKeyQt.isHNNA = (tempOctet & EG_MASK_BIT6)?1:0;
   ieLen--;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   if(ieLen)
   {
   /* decode Voice Domain Preference and UE's usage Setting length */
   SRemPreMsg(&egIe->t.umtsKeyQt.vDomPrefUEUsageLen, mBuf);
      ieLen--;

   /* decode UE's usage setting (1 bit field) Bit-3
    * decode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.umtsKeyQt.vDomPrefUEUsageLen)
   {
     SRemPreMsg(&tempOctet, mBuf);
     egIe->t.umtsKeyQt.ueUsageSett = (tempOctet & EG_MASK_BIT3)?1:0;

         egIe->t.umtsKeyQt.vDomPref = tempOctet & 0x03;
         ieLen--;
      }
   }
#endif
#ifdef EG_REL_AC0
   if(ieLen)
   {
      /* decode of HighBitRateThan16MbpsLen and HighBitRateThan16Mbps as per release 10 */
      SRemPreMsg(&egIe->t.umtsKeyQt.HighBitRateThan16MbpsLen, mBuf);
      SRemPreMsg(&egIe->t.umtsKeyQt.HighBitRateThan16Mbps, mBuf);
      ieLen -= EG_LEN_TWO;
   }
#endif
  if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }  
   RETVALUE(retValue);
} /* end of egDecEG_UMTS_KEY_QUIN */


/***********************************************************************
 *   Fun:   egDecEG_EPS_SEC_QUAD_QUIN
 *   Desc:  Decodes the IE data type EPS Security Context, Quadruplets and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_EPS_SEC_QUAD_QUIN
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_EPS_SEC_QUAD_QUIN(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8  tempOctet = 0x00;
   U8  cnt;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.eSecCxtQdQt.secMode = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.eSecCxtQdQt.ksi = (tempOctet & 0x07);
   egIe->t.eSecCxtQdQt.isNHIPres = (tempOctet & 0x10)?1:0;
   egIe->t.eSecCxtQdQt.isDRXIPres = (tempOctet & 0x08)?1:0;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.eSecCxtQdQt.nmbQuin = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.eSecCxtQdQt.nmbQuadru = (tempOctet & 0x1C) >> EG_SHIFT_2;
   egIe->t.eSecCxtQdQt.isUAMBRIPres = (tempOctet & 0x02)?1:0;
   egIe->t.eSecCxtQdQt.isOCSIPres = (tempOctet & 0x01)?1:0;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.eSecCxtQdQt.isSAMBRIPres = (tempOctet & 0x80)?1:0;
   egIe->t.eSecCxtQdQt.nasProtAlgo = (tempOctet&0x70) >> EG_SHIFT_4;
   egIe->t.eSecCxtQdQt.usedNasCipher = (tempOctet & 0x0F);

   /* decode NAS Downlink Count and NAS Uplink Count - each of 3 octets long */
   EG_DEC_U24(egIe->t.eSecCxtQdQt.nasDlCount, mBuf);
   EG_DEC_U24(egIe->t.eSecCxtQdQt.nasUlCount, mBuf);

   /* decode Kasme of 32 octets */
   egIe->t.eSecCxtQdQt.kasme.length = EG_KASME_LEN;
   EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.kasme.val, egIe->t.eSecCxtQdQt.kasme.length, EG_LEN_32, retValue);
   ieLen -= (EG_KASME_LEN + EG_LEN_NINE);

   for(cnt=0; cnt < egIe->t.eSecCxtQdQt.nmbQuadru; cnt++)
   {
     /* decode the RAND which is of 16 octets*/
     egIe->t.eSecCxtQdQt.authQuadru[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].rand.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.eSecCxtQdQt.authQuadru[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].xres.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.eSecCxtQdQt.authQuadru[cnt].xresLen + 1);

     /* decode the AUTN length in single octet and AUTH which can be of 16 octets */
     SRemPreMsg(&egIe->t.eSecCxtQdQt.authQuadru[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].autn.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.eSecCxtQdQt.authQuadru[cnt].autnLen + 1);

     /* decode the Kasme which is of 31 octets */
     egIe->t.eSecCxtQdQt.authQuadru[cnt].kasme.length = EG_KASME_LEN;
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuadru[cnt].kasme.val, egIe->t.eSecCxtQdQt.authQuadru[cnt].kasme.length, EG_LEN_32, retValue);
     ieLen -= EG_KASME_LEN;
   }

   for(cnt=0; cnt < egIe->t.eSecCxtQdQt.nmbQuin; cnt++)
   {
     /* Decode the RAND which is of 16 octets*/
     egIe->t.eSecCxtQdQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].rand.val, egIe->t.eSecCxtQdQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.eSecCxtQdQt.authQuin[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].xres.val, egIe->t.eSecCxtQdQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.eSecCxtQdQt.authQuin[cnt].xresLen + 1);

     /* decode the CK, IK each is of 16 octets length */
     egIe->t.eSecCxtQdQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].ck.val, egIe->t.eSecCxtQdQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     egIe->t.eSecCxtQdQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].ik.val, egIe->t.eSecCxtQdQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_32;

     /* decode the AUTN */
     SRemPreMsg(&egIe->t.eSecCxtQdQt.authQuin[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.authQuin[cnt].autn.val, egIe->t.eSecCxtQdQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.eSecCxtQdQt.authQuin[cnt].autnLen + 1);
   }

   if(egIe->t.eSecCxtQdQt.isDRXIPres)
   {
   /* decode drx parameter - is of 2 octets */
   SRemPreMsgMult(egIe->t.eSecCxtQdQt.drxParam, EG_LEN_TWO, mBuf);
   ieLen -= EG_LEN_TWO;
   }

   if(egIe->t.eSecCxtQdQt.isNHIPres)
   {
   /* decode nh  parameter - is of 32 octets */
   SRemPreMsgMult(egIe->t.eSecCxtQdQt.nxtHop, EG_LEN_32, mBuf);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.eSecCxtQdQt.ncc = (tempOctet & 0x07);
   ieLen -= (EG_LEN_32 + EG_LEN_ONE);
   }

   if(egIe->t.eSecCxtQdQt.isSAMBRIPres)
   {
   /* decode upLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.eSecCxtQdQt.upSubsUEAMBR, mBuf);

   /* decode downLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.eSecCxtQdQt.dnSubsUEAMBR, mBuf);
   ieLen -= EG_LEN_EIGHT;
   }

   if(egIe->t.eSecCxtQdQt.isUAMBRIPres)
   {
   /* decode upLink used UE AMBR */
   EG_DEC_U32(egIe->t.eSecCxtQdQt.upUsedUEAMBR, mBuf);

   /* decode downLink used UE AMBR */
   EG_DEC_U32(egIe->t.eSecCxtQdQt.dnUsedUEAMBR, mBuf);
   ieLen -= EG_LEN_EIGHT;
   }

 /* decode UE Network Capability - can be max of 15 octets */
   SRemPreMsg((Data*)&egIe->t.eSecCxtQdQt.ueNtwrkCapb.length, mBuf);
   EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.ueNtwrkCapb.val, egIe->t.eSecCxtQdQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   ieLen -= (egIe->t.eSecCxtQdQt.ueNtwrkCapb.length + 1);

   SRemPreMsg((Data*)&egIe->t.eSecCxtQdQt.msNtwrkCapb.length, mBuf);
   /* decode MS Network Capability - can be max of 10 octets */
   EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.msNtwrkCapb.val, egIe->t.eSecCxtQdQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   ieLen -= (egIe->t.eSecCxtQdQt.msNtwrkCapb.length + 1);

   SRemPreMsg((Data*)&egIe->t.eSecCxtQdQt.meId.length, mBuf);
   /* decode MEID - is of 16 octets */
   EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.meId.val, egIe->t.eSecCxtQdQt.meId.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MEID will be of 16 octets */
   ieLen -= (egIe->t.eSecCxtQdQt.meId.length + 1);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.eSecCxtQdQt.isUNA = (tempOctet & EG_MASK_BIT1)?1:0;
   egIe->t.eSecCxtQdQt.isGENA = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.eSecCxtQdQt.isGANA = (tempOctet & EG_MASK_BIT3)?1:0;
   egIe->t.eSecCxtQdQt.isINA = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.eSecCxtQdQt.isENA = (tempOctet & EG_MASK_BIT5)?1:0;
   egIe->t.eSecCxtQdQt.isHNNA = (tempOctet & EG_MASK_BIT6)?1:0;
   ieLen--;


   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.eSecCxtQdQt.nccOld = (tempOctet & 0x07);
   egIe->t.eSecCxtQdQt.ksiOld = (tempOctet & 0x38) >> EG_SHIFT_3;
   egIe->t.eSecCxtQdQt.isNHIOldPres = (tempOctet & 0x80)?1:0;
   ieLen--;

 /* decode Kasme of 32 octets */
   egIe->t.eSecCxtQdQt.kasmeOld.length = EG_KASME_LEN;
   EG_DEC_STR(mBuf, egIe->t.eSecCxtQdQt.kasmeOld.val, egIe->t.eSecCxtQdQt.kasmeOld.length, EG_LEN_32, retValue);
   ieLen -= EG_KASME_LEN;

   if(egIe->t.eSecCxtQdQt.isNHIOldPres)
   {
   /* decode nh old  parameter - is of 32 octets */
   SRemPreMsgMult(egIe->t.eSecCxtQdQt.nxtHopOld, EG_LEN_32, mBuf);
      ieLen -= EG_LEN_32;
   }

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   if(ieLen)
   {
   /* decode Voice Domain Preference and UE's usage Setting length */
   SRemPreMsg(&egIe->t.eSecCxtQdQt.vDomPrefUEUsageLen, mBuf);
      ieLen--;

   /* decode UE's usage setting (1 bit field) Bit-3
    * decode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.eSecCxtQdQt.vDomPrefUEUsageLen)
   {
     SRemPreMsg(&tempOctet, mBuf);
     egIe->t.eSecCxtQdQt.ueUsageSett = (tempOctet & EG_MASK_BIT3)?1:0;

         egIe->t.eSecCxtQdQt.vDomPref = tempOctet & 0x03;
         ieLen--;
      }
   }
#endif
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retValue);
} /* end of egDecEG_EPS_SEC_QUAD_QUIN */


/***********************************************************************
 *   Fun:   egDecEG_UMTS_KEY_QUAD_QUIN
 *   Desc:  Decodes the IE data type UMTS Key, Quadruplets and Quintuplets
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UMTS_KEY_QUAD_QUIN
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_UMTS_KEY_QUAD_QUIN(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;
   S16 cnt = 0;
   U8  tempOctet = 0x00;

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyQdQt.secMode  = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.umtsKeyQdQt.isDRXIPres = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.umtsKeyQdQt.ksi = (tempOctet & 0x07);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyQdQt.nmbQuin = (tempOctet & 0xE0) >> EG_SHIFT_5;
   egIe->t.umtsKeyQdQt.nmbQuadru = (tempOctet & 0x1C) >> EG_SHIFT_2;
   egIe->t.umtsKeyQdQt.isUAMBRIPres = (tempOctet & 0x02)?1:0;
   egIe->t.umtsKeyQdQt.isSAMBRIPres = (tempOctet & 0x01)?1:0;

  /* 7th one is spare octect */
   SRemPreMsg(&tempOctet, mBuf);
   tempOctet = 0x00;

/* decode CK, IK each of 16 octets */
   egIe->t.umtsKeyQdQt.ck.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.ck.val, egIe->t.umtsKeyQdQt.ck.length, EG_LEN_SIXTEEN, retValue);

   egIe->t.umtsKeyQdQt.ik.length = EG_LEN_SIXTEEN;
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.ik.val, egIe->t.umtsKeyQdQt.ik.length, EG_LEN_SIXTEEN, retValue);

   ieLen -= (EG_LEN_SIXTEEN + EG_LEN_SIXTEEN + EG_LEN_THREE);

   for(cnt=0; cnt < egIe->t.umtsKeyQdQt.nmbQuadru; cnt++)
   {
     /* decode the RAND which is of 16 octets*/
     egIe->t.umtsKeyQdQt.authQuadru[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].rand.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.umtsKeyQdQt.authQuadru[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].xres.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQdQt.authQuadru[cnt].xresLen + 1);

     /* decode the AUTN length in single octet and AUTH which can be of 16 octets */
     SRemPreMsg(&egIe->t.umtsKeyQdQt.authQuadru[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].autn.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQdQt.authQuadru[cnt].autnLen + 1);

     /* decode the Kasme which is of 31 octets */
     egIe->t.umtsKeyQdQt.authQuadru[cnt].kasme.length = EG_KASME_LEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuadru[cnt].kasme.val, egIe->t.umtsKeyQdQt.authQuadru[cnt].kasme.length, EG_LEN_32, retValue);
     ieLen -= EG_KASME_LEN;
   }

   for (cnt = 0; cnt < egIe->t.umtsKeyQdQt.nmbQuin; cnt++)
   {
     /* Decode the RAND which is of 16 octets*/
     egIe->t.umtsKeyQdQt.authQuin[cnt].rand.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].rand.val, egIe->t.umtsKeyQdQt.authQuin[cnt].rand.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= EG_LEN_SIXTEEN;

     /* decode the XRES length in single octet and XRES which can be of 16 octets */
     SRemPreMsg(&egIe->t.umtsKeyQdQt.authQuin[cnt].xresLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].xres.val, egIe->t.umtsKeyQdQt.authQuin[cnt].xresLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQdQt.authQuin[cnt].xresLen + 1);

     /* decode the CK, IK each is of 16 octets length */
     egIe->t.umtsKeyQdQt.authQuin[cnt].ck.length = EG_LEN_SIXTEEN;
     egIe->t.umtsKeyQdQt.authQuin[cnt].ik.length = EG_LEN_SIXTEEN;
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].ck.val, egIe->t.umtsKeyQdQt.authQuin[cnt].ck.length, EG_LEN_SIXTEEN, retValue);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].ik.val, egIe->t.umtsKeyQdQt.authQuin[cnt].ik.length, EG_LEN_SIXTEEN, retValue);
     ieLen -= (EG_LEN_SIXTEEN + EG_LEN_SIXTEEN);

     /* decode the AUTN */
     SRemPreMsg(&egIe->t.umtsKeyQdQt.authQuin[cnt].autnLen, mBuf);
     EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.authQuin[cnt].autn.val, egIe->t.umtsKeyQdQt.authQuin[cnt].autnLen, EG_LEN_SIXTEEN, retValue);
     ieLen -= (egIe->t.umtsKeyQdQt.authQuin[cnt].autnLen + 1);
   }

   if(egIe->t.umtsKeyQdQt.isDRXIPres)
   {
   /* decode drx parameter - is of 2 octets */
   SRemPreMsgMult(egIe->t.umtsKeyQdQt.drxParam, EG_LEN_TWO, mBuf);
      ieLen -= EG_LEN_TWO;
   }

   if(egIe->t.umtsKeyQdQt.isSAMBRIPres)
   {
   /* decode upLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.umtsKeyQdQt.upSubsUEAMBR, mBuf);

   /* decode downLink subscribed UE AMBR */
   EG_DEC_U32(egIe->t.umtsKeyQdQt.dnSubsUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

   if(egIe->t.umtsKeyQdQt.isUAMBRIPres)
   {
   /* decode upLink used UE AMBR */
   EG_DEC_U32(egIe->t.umtsKeyQdQt.upUsedUEAMBR, mBuf);

   /* decode downLink used UE AMBR */
   EG_DEC_U32(egIe->t.umtsKeyQdQt.dnUsedUEAMBR, mBuf);
      ieLen -= EG_LEN_EIGHT;
   }

 /* decode UE Network Capability - can be max of 15 octets */
   SRemPreMsg((Data*)&egIe->t.umtsKeyQdQt.ueNtwrkCapb.length, mBuf);
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.ueNtwrkCapb.val, egIe->t.umtsKeyQdQt.ueNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of UE Ntwrk will be of 15 octets */
   ieLen -= (egIe->t.umtsKeyQdQt.ueNtwrkCapb.length + 1);

   /* decode MS Network Capability - can be max of 10 octets */
   SRemPreMsg((Data*)&egIe->t.umtsKeyQdQt.msNtwrkCapb.length, mBuf);
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.msNtwrkCapb.val, egIe->t.umtsKeyQdQt.msNtwrkCapb.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MS Ntwrk will be of 10 octets */
   ieLen -= (egIe->t.umtsKeyQdQt.msNtwrkCapb.length + 1);

   /* decode MEID - is of 16 octets */
   SRemPreMsg((Data*)&egIe->t.umtsKeyQdQt.meId.length, mBuf);
   EG_DEC_STR(mBuf, egIe->t.umtsKeyQdQt.meId.val, egIe->t.umtsKeyQdQt.meId.length, EG_LEN_SIXTEEN, retValue);
   /* here we are mandating the length of MEID will be of 16 octets */
   ieLen -= (egIe->t.umtsKeyQdQt.meId.length + 1);

   SRemPreMsg(&tempOctet, mBuf);
   egIe->t.umtsKeyQdQt.isUNA = (tempOctet & EG_MASK_BIT1)?1:0;
   egIe->t.umtsKeyQdQt.isGENA = (tempOctet & EG_MASK_BIT2)?1:0;
   egIe->t.umtsKeyQdQt.isGANA = (tempOctet & EG_MASK_BIT3)?1:0;
   egIe->t.umtsKeyQdQt.isINA = (tempOctet & EG_MASK_BIT4)?1:0;
   egIe->t.umtsKeyQdQt.isENA = (tempOctet & EG_MASK_BIT5)?1:0;
   egIe->t.umtsKeyQdQt.isHNNA = (tempOctet & EG_MASK_BIT6)?1:0;
   ieLen--;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   if(ieLen)
   {
   /* decode Voice Domain Preference and UE's usage Setting length */
   SRemPreMsg(&egIe->t.umtsKeyQdQt.vDomPrefUEUsageLen, mBuf);
      ieLen--;

   /* decode UE's usage setting (1 bit field) Bit-3
    * decode Voice  domain prefernce for E-UTRAN(2 bit field) Bit 2 and 1 */
   if(egIe->t.umtsKeyQdQt.vDomPrefUEUsageLen)
   {
     SRemPreMsg(&tempOctet, mBuf);
     egIe->t.umtsKeyQdQt.ueUsageSett = (tempOctet & EG_MASK_BIT3)?1:0;

         egIe->t.umtsKeyQdQt.vDomPref = tempOctet & 0x03;
         ieLen--;
      }
   }
#endif
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retValue);
} /* end of egDecEG_UMTS_KEY_QUAD_QUIN */


/***********************************************************************
 *   Fun:   egDecEG_F_CAUSE
 *   Desc:  Decodes the IE data type F-Cause
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_F_CAUSE
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_F_CAUSE(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode the FCause Type */
   SRemPreMsg(&egIe->t.fCause.causeType, mBuf);
   ieLen--;
   egIe->t.fCause.causeType = egIe->t.fCause.causeType & 0x0F;

   /* decode the FCause Value */
   if(ieLen >= EG_LEN_TWO)
   {
   EG_DEC_U16(egIe->t.fCause.cause, mBuf);
      ieLen -= EG_LEN_TWO;
   }

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }


   RETVALUE(retValue);
} /* end of egDecEG_F_CAUSE */


/***********************************************************************
 *   Fun:   egDecEG_S103_PDN_FW_INFO
 *   Desc:  Decodes the IE data type S103 PDN Data Forwarding Info
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_S103_PDN_FW_INFO
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_S103_PDN_FW_INFO(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8  cnt;
   SRemPreMsg(&egIe->t.s103PDF.hsgwAddrLen, mBuf);
   if (egIe->t.s103PDF.hsgwAddrLen == 0x04)
   {
     egIe->t.s103PDF.hsgwAddr.type = CM_TPTADDR_IPV4;
     EG_DEC_U32(egIe->t.s103PDF.hsgwAddr.u.ipv4, mBuf);
     ieLen -= (EG_LEN_FOUR + EG_LEN_ONE);
   } /* end of if */
   else if (egIe->t.s103PDF.hsgwAddrLen == 0x10)
   {
     egIe->t.s103PDF.hsgwAddr.type = CM_TPTADDR_IPV6;
     SRemPreMsgMult(egIe->t.s103PDF.hsgwAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
     ieLen -= (EG_LEN_SIXTEEN + EG_LEN_ONE);
   } /* end of case */
   else
   {
      RETVALUE(RFAILED);
   }

   /* decode the GRE Key */
   EG_DEC_U32(egIe->t.s103PDF.greKey, mBuf);

   /* decode the EBI Number */
   SRemPreMsg(&egIe->t.s103PDF.ebiNumber, mBuf);

   ieLen -= (EG_LEN_FOUR + EG_LEN_ONE);
   for(cnt=0;((cnt < egIe->t.s103PDF.ebiNumber) 
                   && (cnt < EG_MAX_NMB_BEARER));cnt++)
   {
     SRemPreMsg(&egIe->t.s103PDF.ebi[cnt], mBuf);
     ieLen--;
   }

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
   RETVALUE(retValue);
} /* end of egDecEG_S103_PDN_FW_INFO */


/***********************************************************************
 *   Fun:   egDecEG_S1U_DATA_FW
 *   Desc:  Decodes the IE data type S1-U Data Forwarding
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_S1U_DATA_FW
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_S1U_DATA_FW(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;


   SRemPreMsg(&egIe->t.s1UDF.ebi, mBuf);
   egIe->t.s1UDF.ebi = (egIe->t.s1UDF.ebi & 0x0F);

   /* decode the SGW Address length */
   SRemPreMsg(&egIe->t.s1UDF.sgwAddrLen, mBuf);

   if(egIe->t.s1UDF.sgwAddrLen == 0x04)
   {
      egIe->t.s1UDF.sgwAddr.type = CM_TPTADDR_IPV4;
      EG_DEC_U32(egIe->t.s1UDF.sgwAddr.u.ipv4, mBuf);
      ieLen -= (EG_LEN_FOUR + EG_LEN_TWO);
   } /* end of if */
#ifdef EG_IPV6_SUPPORTED
   else if (egIe->t.s1UDF.sgwAddrLen == 0x10)
   {
     egIe->t.s1UDF.sgwAddr.type = CM_TPTADDR_IPV6;
     SRemPreMsgMult(egIe->t.s1UDF.sgwAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
      ieLen -= (EG_LEN_SIXTEEN + EG_LEN_TWO);
   } /* end of case */
#endif /* end of CM_TPTADDR_IPV6 */
   else if(egIe->t.s1UDF.sgwAddrLen == 0)
   {
      /* Dont do anything */
   } /* end of if */
   else
   {
      RETVALUE(RFAILED);
   }

   /* decode the S1U-TEID */
   EG_DEC_U32(egIe->t.s1UDF.sgwS1UTEID, mBuf);
   ieLen -= EG_LEN_FOUR;

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* end of egDecEG_S1U_DATA_FW */


/***********************************************************************
 *   Fun:   egDecEG_SRC_RNC_PDCP_CTX
 *   Desc:  Decodes the IE data type Source RNC PDCP Context
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_SRC_RNC_PDCP_CTX
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_SRC_RNC_PDCP_CTX(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;

   EG_DEC_STR(mBuf, egIe->t.valStr.val, egIe->t.valStr.length, EG_MAX_STR_LEN, retValue);

   RETVALUE(retValue);
} /* end of egDecEG_SRC_RNC_PDCP_CTX */


/***********************************************************************
 *   Fun:   egDecEG_PDUNUM
 *   Desc:  Decodes the IE data type PDU Number
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_PDUNUM
(
EgIe       *egIe,
Buffer     *mBuf,
U16        ieLen
)
#else
PRIVATE S16 egDecEG_PDUNUM(egIe, mBuf, ieLen)
EgIe       *egIe;
Buffer     *mBuf;
U16        ieLen;
#endif
{
   S16 retValue = ROK;

   /* Decode NSAPI */
   SRemPreMsg(&egIe->t.pduNum.nsapi, mBuf);

   /* Decode the DownLink GTP-U Sequence Number */
   EG_DEC_U16(egIe->t.pduNum.dnLinkSeqNo, mBuf);

   /* Decode the Uplink GTP-U Sequence Number */
   EG_DEC_U16(egIe->t.pduNum.upLinkSeqNo, mBuf);

   /* Decode the Send N-PDU Number */
   EG_DEC_U16(egIe->t.pduNum.SNPduNo, mBuf);

   /* Decode the Receive N-PDU Number */
   EG_DEC_U16(egIe->t.pduNum.RNPduNo, mBuf);

   ieLen -= EG_LEN_NINE;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* end of egDecEG_PDUNUM */
/***********************************************************************
 *   Fun:   egDecEG_UE_TIME_ZONE
 *   Desc:  Decodes the IE data type Time zone
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UE_TIME_ZONE
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_UE_TIME_ZONE(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode time zone */
   SRemPreMsg(&egIe->t.timeZone.timezone, mBuf);
   /* decode day light savings time */
   SRemPreMsg(&egIe->t.timeZone.dayLiteSavTime, mBuf);
   egIe->t.timeZone.dayLiteSavTime = egIe->t.timeZone.dayLiteSavTime & 0x03;

   ieLen -= EG_LEN_TWO;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}
/***********************************************************************
 *   Fun:   egDecEG_TRACE_REF
 *   Desc:  Decodes the IE data type trace reference
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_TRACE_REF
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_TRACE_REF(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode snkw */
   EG_DEC_NETID(egIe->t.trcRef.sNwk,mBuf);
   /* decode trace id */
   
   /************************************
    * eg009.201 -Added for S2B and S2A *
    ************************************/
   EG_DEC_U24(egIe->t.trcRef.trcId, mBuf);
   ieLen -= EG_LEN_SIX;

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}/* egDecEG_TRACE_REF */
/***********************************************************************
 *   Fun:   egDecEG_RAB_CONTEXT
 *   Desc:  Decodes the IE data type RAB Context
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_RAB_CONTEXT
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_RAB_CONTEXT(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode nsapi */
   SRemPreMsg(&egIe->t.rabCntxt.nsapi,mBuf);
   egIe->t.rabCntxt.nsapi = egIe->t.rabCntxt.nsapi & 0x0F;
   /* decode Downlink GTP-U Sequence Number */
   EG_DEC_U16(egIe->t.rabCntxt.dnlEgUSeqNo ,mBuf);
   /* decode Uplink GTP-U Sequence Number */
   EG_DEC_U16(egIe->t.rabCntxt.uplEgUSeqNo ,mBuf);
   /*decode Downlink GTP-U Sequence Number */
   EG_DEC_U16(egIe->t.rabCntxt.dnlPdcpSeqNo ,mBuf);
   /* decode Uplink GTP-U Sequence Number */
   EG_DEC_U16(egIe->t.rabCntxt.uplPdcpSeqNo ,mBuf);

   ieLen -= EG_LEN_NINE;

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* egDecEG_RAB_CONTEXT */
/***********************************************************************
 *   Fun:   egDeSEVENGLOBAL_CN_ID
 *   Desc:  Decodes the IE data type Global CN Id
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_GLOBAL_CN_ID
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_GLOBAL_CN_ID(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /* decode sNwk */
   EG_DEC_NETID(egIe->t.gblCnId.sNwk,mBuf);
   /* decode cnId */
   egIe->t.gblCnId.cnId.length = EG_LEN_FOUR;
   EG_DEC_STR(mBuf, egIe->t.gblCnId.cnId.val, egIe->t.gblCnId.cnId.length, EG_LEN_FOUR, retValue);

   ieLen -= EG_LEN_SEVEN;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* egDecEG_GLOBAL_CN_ID */

/***********************************************************************
 *   Fun:   egDecEG_AMBR_
 *   Desc:  Decodes the IE data type Aggregate Maximum Bit Rate
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_AMBR
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_AMBR(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;

   /*decode APN-AMBR for uplink */
   EG_DEC_U32(egIe->t.ambr.uplAPNAMBR, mBuf);
   /* decode APN-AMBR for downlink */
   EG_DEC_U32(egIe->t.ambr.dnlAPNAMBR, mBuf);
   ieLen -= EG_LEN_EIGHT;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
} /* egDecEG_AMBR */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
/***********************************************************************
 *   Fun:   egDecEG_MBMS_IPMCAST_DIST
 *   Desc:  Decodes the IE data type RAB Context
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_MBMS_IPMCAST_DIST
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_MBMS_IPMCAST_DIST(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8 addressType = 0;

   /* decode Common Tunnel Endpoint Identifier Data */
   EG_DEC_U32(egIe->t.mbmsIPMCastDist.cteidDat, mBuf);

   ieLen -= EG_LEN_FOUR;
   /* decode IP Multicast Distribution Address Type */
   EG_DEC_U8(egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type, mBuf);
#ifdef EG_REL_AC0
   addressType = egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type & 0x3F;
#else
   addressType = egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type;
#endif /* EG_REL_AC0 */
   /* decode IP Multicast Distribution Address */
   switch (addressType)
   {
      case CM_TPTADDR_IPV4:
      {
         EG_DEC_U32(egIe->t.mbmsIPMCastDist.ipMCastDistAddr.u.ipv4, mBuf);
         ieLen -= (EG_LEN_FOUR + EG_LEN_ONE);
         break;
      } /* end of case */
#ifdef EG_IPV6_SUPPORTED
      case CM_TPTADDR_IPV6:
      {
         SRemPreMsgMult(egIe->t.mbmsIPMCastDist.ipMCastDistAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         ieLen -= (EG_LEN_SIXTEEN + EG_LEN_ONE);
         break;
      } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
      default:
      {
         RETVALUE(RFAILED);
      } /* end of case */
   }

  /* decode IP Multicast Source Address Type */
   EG_DEC_U8(egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type, mBuf);
#ifdef EG_REL_AC0
   addressType = egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type & 0x3F;
#else
   addressType = egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type;
#endif /* EG_REL_AC0 */
 
   /* decode IP Multicast Source Address */
   switch (addressType)
   {
      case CM_TPTADDR_IPV4:
      {
         EG_DEC_U32(egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.u.ipv4, mBuf);
         ieLen -= (EG_LEN_FOUR + EG_LEN_ONE);
         break;
      } /* end of case */
#ifdef EG_IPV6_SUPPORTED
      case CM_TPTADDR_IPV6:
      {
         SRemPreMsgMult(egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         ieLen -= (EG_LEN_SIXTEEN + EG_LEN_ONE);
         break;
      } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
      default:
      {
         RETVALUE(RFAILED);
      } /* end of case */
   }

   /* decode MBMS HC Indicator  */
   EG_DEC_U8(egIe->t.mbmsIPMCastDist.mbmsHCInd, mBuf);
   ieLen --;

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}

/***********************************************************************
 *   Fun:   egDecEG_UCI 
 *   Desc:  Decodes the IE data type user CSG information
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_UCI 
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_UCI(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;

   /* decode serving network */
   EG_DEC_NETID(egIe->t.uci.sNwk,mBuf);
   
   /* decode CSG Id values */
   /* CSG Id consists of 4 octets. Bit 3 of Octect 8 is the most significant bit and
    * bit 1 of Octet 11 is the least significant bit
    */
   EG_DEC_U32(egIe->t.uci.csgId, mBuf);
   egIe->t.uci.csgId &= 0x07FFFFFF;

   EG_DEC_U8(tmpByte, mBuf);
   /* decode access mode */
   egIe->t.uci.accMode =  (tmpByte >> 0x06) && 0x03;
   /* decode LCSG */
   egIe->t.uci.isLCSGPres = ((tmpByte & EG_MASK_BIT2) > 0)?1:0;
   /* decode CMI */
   egIe->t.uci.isCMIPres = ((tmpByte & EG_MASK_BIT1) > 0)?1:0;
   ieLen -= EG_LEN_EIGHT;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}/* egDecEG_UCI */


/***********************************************************************
 *   Fun:   egDecEG_CSG_INFO_REP_ACT 
 *   Desc:  Decodes the IE data type CSG information reproting action
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_CSG_INFO_REP_ACT 
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_CSG_INFO_REP_ACT(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16     ieLen;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;

   EG_DEC_U8(tmpByte, mBuf);
   /* decode UCICSG */
   egIe->t.csgInfoReqAct.isUCICSGPres = ((tmpByte & EG_MASK_BIT1) > 0)?1:0;
   /* decode UCISHC */
   egIe->t.csgInfoReqAct.isUCISHCPres = ((tmpByte & EG_MASK_BIT2) > 0)?1:0;
   /* decode UCIUHC */
   egIe->t.csgInfoReqAct.isUCIUHCPres = ((tmpByte & EG_MASK_BIT3) > 0)?1:0;

   ieLen --;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}/* egDecEG_CSG_INFO_REP_ACT */


#endif /* EG_REL_930 */

#ifdef EG_REL_AC0
/***********************************************************************
 *   Fun:   egDecEG_ARP
 *   Desc:  Decodes the IE data type ARP
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_ARP
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_ARP(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   EG_DEC_U8(tmpByte, mBuf);
   /* decode PL */
   egIe->t.arp.PL =  (tmpByte >> EG_SHIFT_2) & EG_MASK_LOW4;
   /* decode PCI */
   egIe->t.arp.isPCIPres = ((tmpByte & EG_MASK_BIT7) > 0)?1:0;
   /* decode PVI*/
   egIe->t.arp.isPVIPres = ((tmpByte & EG_MASK_BIT1) > 0)?1:0;

   ieLen --;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}/* egDecEG_ARP */


/***********************************************************************
 *   Fun:   egDecEG_STNSR
 *   Desc:  decodes the IE data type STNSR
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_STNSR
(
EgIe   *egIe,
Buffer *mBuf,
U16 ieLen
)
#else
PRIVATE S16 egDecEG_STNSR(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16 ieLen;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;

   EG_DEC_U8(tmpByte, mBuf);

   egIe->t.stnsr.ext =  (tmpByte >> 0x07) & 0x01;
   egIe->t.stnsr.NatOfAdd=  (tmpByte >> 0x04) & 0x07;
   egIe->t.stnsr.NPI=  (tmpByte & 0x0f);
   ieLen--;

   retValue = egDecEGGeneric_BCD(egIe->t.stnsr.val,mBuf, ieLen, &egIe->t.stnsr.length);

   RETVALUE(retValue);
}/* egDecEG_STNSR*/

/***********************************************************************
 *   Fun:   egDecEG_THROTTLING
 *   Desc:  Decodes the IE data type THROTTLING
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_THROTTLING
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_THROTTLING(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   EG_DEC_U8(tmpByte, mBuf);
   /* decode Timer unit*/
   egIe->t.egThrot.thrDelayUnit =  (tmpByte >> 0x04) & 0x0F;
   /* decode Timer val */
   egIe->t.egThrot.thrDelayValue = ((tmpByte & 0x0F));

   EG_DEC_U8(egIe->t.egThrot.thrFactor, mBuf);
   ieLen -= EG_LEN_TWO;

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}/* egDecEG_EPCTMR */





/***********************************************************************
 *   Fun:   egDecEG_EPC_TMR
 *   Desc:  Decodes the IE data type EPCTMR
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_EPC_TMR
(
EgIe   *egIe,
Buffer *mBuf,
U16    ieLen
)
#else
PRIVATE S16 egDecEG_EPC_TMR(egIe, mBuf, ieLen)
EgIe   *egIe;
Buffer *mBuf;
U16    ieLen;
#endif
{
   S16 retValue = ROK;
   U8 tmpByte = 0x00;


   EG_DEC_U8(tmpByte, mBuf);
   /* decode Timer unit*/
   egIe->t.epcTmr.tmrUnit =  (tmpByte >> 0x05) & 0x07;
   /* decode Timer val */
   egIe->t.epcTmr.tmrVal = ((tmpByte & 0x1f));

   ieLen --;
   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retValue);
}/* egDecEG_EPCTMR */


/***********************************************************************
 *   Fun:   egDecEG_CNG_TO_RPT_FLG
 *   Desc:  Decodes the IE data type Change to report Action
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/

#ifdef ANSI
PRIVATE S16 egDecEG_CNG_TO_RPT_FLG
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_CNG_TO_RPT_FLG(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
#endif
{

   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U8       tmpByte = 0;         /* Holds 1 byte of data for decoding  */

   /********************************************
    * Change to report flag IE is 1 octet      *
    *                                          *
    * 1 Bit -. SNCR(Serving Network Change     *
    *               To Report)                 *
    * 2 Bit - TZCR(Time Zone Change To Report  *
    * 3 -8 Bits - SPARE
    **************************************/
   
   /******************************************
    * eg013.201 - Release 10 upgrade changes *
    ******************************************/


   EG_DEC_U8(tmpByte, mBuf);

   egIe->t.cngRptFlg.isSNCRPres = tmpByte & EG_MASK_BIT1;
   egIe->t.cngRptFlg.isTZCRPres = tmpByte & EG_MASK_BIT2;

   ieLen --;
   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }

   RETVALUE(retVal);
}

/***********************************************************************
 *   Fun:   egDecEG_MDT_CONF
 *   Desc:  Decode the IE data type MDT Configuration
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_MDT_CONF
(
EgIe* egIe,
Buffer *mBuf,
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_MDT_CONF(egIe,mBuf,ieLen)
EgIe* egIe;
Buffer *mBuf;
U16 ieLen;          /* Ie Length to decode */
#endif
{
   S16 retValue = ROK;

   /* encode Job Type */
   EG_DEC_U8(egIe->t.mdtConf.jobType, mBuf);

   /* Enocde List of Measurments */
   SRemPreMsgMult(egIe->t.mdtConf.listOfMsmt, EG_LEN_FOUR, mBuf);

   /* Enocde Reporting Trigger */
   EG_DEC_U8(egIe->t.mdtConf.rptTrigger, mBuf);
   /* Enocde Report Interval */
   EG_DEC_U8(egIe->t.mdtConf.rptInterval, mBuf);

   /* Encode Report Amount */
   EG_DEC_U8(egIe->t.mdtConf.rptAmount, mBuf);

   /* Encode Event Threshold for RSRP */
   EG_DEC_U8(egIe->t.mdtConf.evtThrRsrp, mBuf);

   /* Encode Event Threshold for RSRQ */
   EG_DEC_U8(egIe->t.mdtConf.evtThrRsrq, mBuf);

   /* Encode length of Area Scope */
   SRemPreMsg((U8*)&(egIe->t.mdtConf.areaScope.length), mBuf);

   /* Encode Area Scope */
   EG_DEC_STR(mBuf, egIe->t.mdtConf.areaScope.val, 
                 egIe->t.mdtConf.areaScope.length, 
                 EGT_MAX_STR_LEN, retValue);

   ieLen -= (EG_LEN_ELEVEN + egIe->t.mdtConf.areaScope.length);

   if(ieLen)
   {
      retValue = egEdmRemoveExtraIeData(mBuf,ieLen);
   }


   RETVALUE(retValue);
}

/***********************************************************************
 *   Fun:   egDecEG_ADD_MM_CNTXT_SRVCC
 *   Desc:  Encodes the IE data type EgMMCntxtForSrvcc
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_ADD_MM_CNTXT_SRVCC
(
EgIe* egIe,
Buffer *mBuf,
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_ADD_MM_CNTXT_SRVCC(egIe,mBuf,ieLen)
EgIe* egIe;
Buffer *mBuf;
U16 ieLen;          /* Ie Length to decode */
#endif
{
   S16 retVal = ROK;
   U8  supportedCodecLen = 0;
   U8  count = 0;

   /* encode Class Mark2 len */
   EG_DEC_U8(egIe->t.mmCntxtForSrvcc.classMk2Len, mBuf);

   /* encode Class Mark2 */
   EG_DEC_STR(mBuf, egIe->t.mmCntxtForSrvcc.classMk2, 
                 egIe->t.mmCntxtForSrvcc.classMk2Len, 
                 EG_LEN_THREE, retVal);

   /* encode Class Mark3 len */
   EG_DEC_U8(egIe->t.mmCntxtForSrvcc.classMk3Len, mBuf);

   /* encode Class Mark3 */
   EG_DEC_STR(mBuf, egIe->t.mmCntxtForSrvcc.classMk3, 
                 egIe->t.mmCntxtForSrvcc.classMk3Len, 
                 EG_LEN_34, retVal);

   EG_DEC_U8(supportedCodecLen, mBuf);

   count = 0;
   while(supportedCodecLen)
   {
      /* To-Do Introduce new macro for 10 */
      if(count < EG_MAX_SUPP_CODEC)
      {
         /* Encode System identification */
         EG_DEC_U8(egIe->t.mmCntxtForSrvcc.codec[count].sysId, mBuf);
         /* Encode Bit Map Length */
         EG_DEC_U8(egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen, mBuf);

         /* Encode Bit Map value */
         EG_DEC_STR(mBuf, egIe->t.mmCntxtForSrvcc.codec[count].bitMap, 
               egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen, 
               EG_LEN_TWO, retVal);
         supportedCodecLen -= (egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen + 2);
         count++;
      }
      else
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG023, ERRZERO,
               "Maximum support codec list is 10.....\n");
      }

   }
   egIe->t.mmCntxtForSrvcc.numOfcodecList = count;

   ieLen -= (egIe->t.mmCntxtForSrvcc.classMk2Len + 
              egIe->t.mmCntxtForSrvcc.classMk3Len +
               supportedCodecLen + EG_LEN_THREE);               
   if(ieLen)
   {
      retVal = egEdmRemoveExtraIeData(mBuf,ieLen);
   }
 
   RETVALUE(retVal);
} /* End of egDecEG_ADD_MM_CNTXT_SRVCC */
#endif


/*****fix for ccpu00135846:bcd encoding and decoding handler for IEs */
/***********************************************************************
 *   Fun:   egEncEG_BCD
 *   Desc:  Encodes the IE Data
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEG_BCD
(
EgIe* egIe,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEG_BCD(egIe, mBuf)
EgIe* egIe;
Buffer *mBuf;
#endif
{
   S16  retVal = ROK; /* Holds the IE parsing result   */
   Data str_enc[EG_MAX_BYTES]; /*encoded string */
   U8 len=0;     /*length of string to be encoded */
   U16 i,j=0;
   U16 idx1;

   len = cmStrlen(egIe->t.valStr32.val);
   cmMemset((U8 *)str_enc,0xFF,16);
   if(len%2)
   {
  
      egIe->t.valStr16.length=(len/2)+1;
   }
   else
   {
      egIe->t.valStr16.length=(len/2);
   }
      
   idx1=len;
   for(i=0;i<len;i++)
   { 
     if(i%2==0)
     {
        str_enc[j]=(egIe->t.valStr32.val[i])&0x0f;
     }
     else
     {
        str_enc[j]= (str_enc[j]|((egIe->t.valStr32.val[i]<<4)&0xf0));
        j++;
     }
   }
   str_enc[j]=(i%2)?((str_enc[j])|0xf0):str_enc[j];
   cmMemcpy(egIe->t.valStr16.val,str_enc,EG_LEN_SIXTEEN);
   EG_ENC_STR(mBuf, egIe->t.valStr16.val, egIe->t.valStr16.length, EG_LEN_SIXTEEN, retVal);
   RETVALUE(retVal);
}   

/***********************************************************************
 *   Fun:   egDecEG_BCD
 *   Desc:  Decodes the IE data type string_8
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEG_BCD
(
EgIe* egIe,        /* Ie to store the decoded IE data */
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEG_BCD(egIe,mBuf,ieLen)
EgIe* egIe;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
EgIeEntry    *ieEntry,   /* Holds the min and max length   */
#endif
{
   U16 idx1=0,idx3=0; /* index values */
   U8 str_dec[32]; /* decoded string */
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U16 loopCnt;
   
   cmMemset((U8*)str_dec,'\0',32);
  
   EG_DEC_STR(mBuf, egIe->t.valStr32.val, ieLen, EG_LEN_SIXTEEN, retVal); 
   if(((egIe->t.valStr32.val[ieLen-1])& 0xF0) == 0xF0)
   {
      idx1 = ieLen-1;
      str_dec[ieLen*2-2]=(((egIe->t.valStr32.val[ieLen-1]) & 0x0F)+'0');
      str_dec[ieLen*2-1]='\0';
      egIe->t.valStr32.length = ieLen*2-1;
   }
   else
   {
     idx1 = ieLen;
     egIe->t.valStr32.length = ieLen*2;
   }

   /***********************
    * IE decoding from BCD*
   ************************/
   for(loopCnt = 0; loopCnt < idx1; loopCnt++)
   {
      str_dec[idx3++] = ((egIe->t.valStr32.val[loopCnt])& 0x0F)+'0';
      str_dec[idx3++] = (((egIe->t.valStr32.val[loopCnt])& 0xF0) >> 4)+'0';
   }

   egIe->dataType= EG_STRING_32;
   cmMemcpy(egIe->t.valStr32.val,str_dec,egIe->t.valStr32.length);
   RETVALUE(retVal);
}

#ifdef EG_REL_AC0
/***********************************************************************
 *   Fun:   egEncEGGeneric_BCD
 *   Desc:  Encodes the IE Data
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egEncEGGeneric_BCD
(
U8 *val,
U16 *length,
Buffer *mBuf
)
#else
PRIVATE S16 egEncEGGeneric_BCD(val, length, mBuf)
U8 *val;
U16 *length;
Buffer *mBuf;
#endif
{
   S16  retVal = ROK; /* Holds the IE parsing result   */
   Data str_enc[EG_MAX_BYTES]; /*encoded string */
   U8 len = 0;
   U16 i,j=0;
   U16 idx1;

   len = cmStrlen(val);
   cmMemset((U8 *)str_enc,0xFF,16);
   if(len%2)
   {
  
      *length = (len/2)+1;
   }
   else
   {
      *length = (len/2);
   }
      
   idx1=len;
   for(i=0;i<len;i++)
   { 
     if(i%2==0)
     {
        str_enc[j]=(val[i])&0x0f;
     }
     else
     {
        str_enc[j]= (str_enc[j]|((val[i]<<4)&0xf0));
        j++;
     }
   }
   str_enc[j]=(i%2)?((str_enc[j])|0xf0):str_enc[j];
   /* cmMemcpy(egIe->t.valStr16.val,str_enc,EG_LEN_SIXTEEN);*/ 
   EG_ENC_STR(mBuf, str_enc, *length, EG_LEN_SIXTEEN, retVal);
   RETVALUE(retVal);
}   

/***********************************************************************
 *   Fun:   egDecEGGeneric_BCD
 *   Desc:  Decodes the IE data type string_8
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   Notes: None
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egDecEGGeneric_BCD
(
U8 *val, 
Buffer *mBuf,      /* Holds the Data for decoding */
U16 ieLen,          /* Ie Length to decode */
U16 *len          /* Ie Length to decode */
)
#else
PRIVATE S16 egDecEGGeneric_BCD(val,mBuf,ieLen,len)
U8 *val;        /* Ie to store the decoded IE data */
Buffer *mBuf;      /* Holds the Data for decoding */
U16 ieLen;         /* Ie Length to decode */
U16 *len;          /* Ie Length to decode */
EgIeEntry    *ieEntry,   /* Holds the min and max length   */
#endif
{
   U16 idx1=0,idx3=0; /* index values */
   U8 str_dec[EG_MAX_BYTES]; /* decoded string */
   S16 retVal = ROK; /* Return value used to know the successful parsing */
   U16 loopCnt;
   U16 length = 0;
   
   cmMemset((U8*)str_dec,'\0',32);
  
   EG_DEC_STR(mBuf, val, ieLen, EG_LEN_SIXTEEN, retVal); 
   if(((val[ieLen-1])& 0xF0) == 0xF0)
   {
      idx1 = ieLen-1;
      str_dec[ieLen*2-2]=(((val[ieLen-1]) & 0x0F)+'0');
      str_dec[ieLen*2-1]='\0';
      length = ieLen*2-1;
   }
   else
   {
     idx1 = ieLen;
     length = ieLen*2;
   }

   /***********************
    * IE decoding from BCD*
   ************************/
   for(loopCnt = 0; loopCnt < idx1; loopCnt++)
   {
      str_dec[idx3++] = ((val[loopCnt])& 0x0F)+'0';
      str_dec[idx3++] = (((val[loopCnt])& 0xF0) >> 4)+'0';
   }
  *len = length;

   /* egIe->dataType= EG_STRING_32;*/ 
   cmMemcpy(val,str_dec,length);
   RETVALUE(retVal);
}
#endif

/*********************fix for ccpu00136098****************************************/
/*************************************************************************
 *      Fun:   egTunnErrIncOutRsp
 *
 *      Desc:  The function is handler for event - send Session Response for the manatory ie 
 *             missing  in the state - INCSESSREQ.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnErrIncOutRsp 
(
EgEvnt *event,           /* outgoing event */
/*fix for ccpu00136924 */
U8 failedIeType,      /**< stores the failed IE type */
U8 failedIeInst       /**< stores the failed IE Inst */
)
#else
PRIVATE S16 egTunnErrIncOutRsp (event,failedIeInst,failedIeType)
EgEvnt *event;           /* outgoing event */
/* fxi for ccpu00136924 */
U8 failedIeType;     /**< stores the failed IE type */
U8 failedIeInst;       /**< stores the failed IE Inst */
#endif
{
   Buffer         *mBuf=NULLP;
   U8             cnt=0;
   CmLList        *tmpNode = NULLP;
   EgEvnt         *outEvntMsg;
   EgMsg          *outEgMsg;
   EgIe           egIe;
   EgIeLevelInfo  egLvlInfo;
   EgIe           *tmpIe = NULLP;
   S16            retVal=ROK;
   U16            tmpLen = 0;            /* Stores the ieLen temp */
   MsgLen         encLen = 0;   /* Temp stores the encoded length */
   Pst            tmpPst;
   EgUstaDgn      dgn;    /* diagnostics for sending alaram to LM */
   Mem            mem;   /* For allocating memory using reg, pool */

   EG_TRC2(egTunnErrIncOutRsp);
 
   /**************************************************
    * Initialize the diagnostic and memory structure *
    **************************************************/
   cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
   cmMemset((U8*)&mem, EG_ZERO, sizeof(Mem));

   mem.region = egCb.init.region;
   mem.pool   = egCb.init.pool;
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   
   /*******************************
   * PREPARE THE RESPONSE MESSAGE *
   ********************************/
   EG_ALLOCEVNT (&outEvntMsg);
   if(EgUtilAllocGmMsg(&outEvntMsg->u.egMsg, &mem) != ROK)
   {
      EG_DBG_ERR (0, 0, (egp, "\n egSendEchoReq:Memory allocation \
                            for egMsg failed"));
      EG_RETVALUE (RFAILED);
   }
   outEgMsg = outEvntMsg->u.egMsg;
   cmMemcpy((U8 *)&outEgMsg->msgHdr,(U8 *)&event->u.egMsg->msgHdr, sizeof(EgMsgHdr)); 
   /* ccpu00137168 : Get the associated response message */
   outEgMsg->msgHdr.msgType = (U8)egGetAssocMsg((EgtMsgType) event->u.egMsg->msgHdr.msgType);
   /* fix for ccpu00137318 */
#ifdef EG_PIGGYBACK_SUPP
   outEgMsg->msgHdr.pMsgPres=FALSE;
#endif
   /**********************
   * PREPARE THE CAUSE IE*
   **********************/
   egIe.ieType = 2;
   egIe.ieInst = 0;
   egIe.dataType = EG_CAUSE;
   /* fix for ccpu00136924 */
   egIe.t.cause.ieType = failedIeType;
   egIe.t.cause.ieInst = failedIeInst;
   /*fix for ccpu00137063 */
   switch (event->resultType)
   {
      case EGT_ERR_IE_INV_LEN:
      case EGT_ERR_MSG_INV_LEN:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_INVALIED_LEN;
         break;
      case EGT_ERR_MSG_MISS_MAND_IE:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         break;
      /*fix for ccpu00136924 */
      case EGT_ERR_MSG_INCORR_IE:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
         egIe.t.cause.ieType = 87; /* EG_FTEID_IETYPE */
         egIe.t.cause.ieInst = 0;
         break;
      /* fix for ccpu00136925 */
      case EGT_ERR_IE_UNKNOWN:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_UNEXPD_IE;
         break;
      /* ccpu00137045 */
      case EGT_ERR_MSG_INV_PDN_TYPE:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_PDN_NOT_SUPP;
         break;
      /* fix for ccpu00137362 */   
      case EGT_ERR_INV_TEID_RCVD:
         /* ccpu00137802 */
         outEgMsg->msgHdr.teid = EG_ZERO;
         outEgMsg->msgHdr.teidPres = TRUE;
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_REJ_CXT_NON_EXISTENT;
         break;
      case EGT_ERR_MSG_MLFRMD_PKT:
      case EGT_ERR_INV_MSG_FMT:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_REJ_INV_MSG_FMT;
         break;
      default:
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_INVALIED_LEN;
         break;
   }

   /* fix for ccpu00137063 */
   egIe.t.cause.ieLength = 0;
   egIe.t.cause.ieCS = FALSE;

   /*******************************************************
    * DROP THE OLD IELIST associated with request message *
    ******************************************************/
   if ( (retVal = egUtilDropIeList(&(event->u.egMsg)->egIeList)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG023, ERRZERO,
               "EgUtilDeAllocGmMsg: De-allocating memory for the IE \
               list failed and continuing with dropping the message.\n");
   }

  /************************************************************
   * ADD the new ie list associated with new response message *
   *************************************************************/
  retVal = EgUtilGmAppendIe(outEgMsg, &egLvlInfo, &egIe);
  if(retVal!= ROK)
  {
     EG_RETVALUE(retVal);
  }
  /*********************
   *ENCODE THE MESSAGE *
   *********************/
#ifdef EG_MT
   retVal = SGetMsg(pst->region, pst->pool, &mBuf);
#else
   retVal = SGetMsg(egCb.init.region, egCb.init.pool, &mBuf);
#endif /* EG_MT */ 
   if(retVal != ROK)
   {
     /********************************************************
      * Memory allocation failed, send alarm to LM and error *
      * event message to main thread along with result code  * 
      ********************************************************/ 
      EG_SND_ALARM_MEM_ALLOC_FAIL(mem, dgn, retVal);

      retVal = EGT_ERR_MEM_ALLOC_FAILED;
      EG_FILL_ERR_EVNT_MSG(event, retVal, 0, 0, 
                           event->u.egMsg->msgHdr.msgType); 
#ifdef EG_MT
      egEdmSendMsgToCm(pst, evntMsg);
#endif /* EG_MT */
      EG_RETVALUE(retVal);
   }
   /***********************
   * Encode the gtp header *
   ************************/
   egEncodeGmHdr(outEvntMsg, mBuf);

  /**********************
   * Encode the cause IE *
   ***********************/
   CM_LLIST_FIRST_NODE(&outEgMsg->egIeList, tmpNode);
   for(cnt = 0; 
   ((cnt < outEgMsg->egIeList.count) && (tmpNode)); 
       cnt++)
   {
      /*****************************
      * Get the IE entry          *
      ****************************/
      tmpIe = (EgIe*)tmpNode->node;


      /*****************************
       * Encode the IE             *
       ****************************/
      retVal = egEncIe(tmpIe, mBuf);
      if(retVal != ROK)
      {
        /********************************************************
         * Fill and send error event message to main thread and * 
         * free buffer                                          *
         ********************************************************/ 
         EG_FILL_ERR_EVNT_MSG(event, retVal, tmpIe->ieType, 
                              tmpIe->ieInst, 
                              event->u.egMsg->msgHdr.msgType); 
         SPutMsg(mBuf);

         EG_RETVALUE(retVal);
      }
      else
      {
         /*********************************
          * Get the next IE from the list *
          ********************************/
         CM_LLIST_NEXT_NODE(&outEgMsg->egIeList, tmpNode);
      }
   }


   /************************************************************
    * Fill the message length, length starts from index 2      *
    * calculate total Buffer length and decrement 4bytes which *
    * is to encode the mandatory hdr elements                  *
    ************************************************************/
   SFndLenMsg(mBuf, &encLen);
   encLen -= EG_LEN_FOUR;

   /************************************************************
    * Fill the message length, length starts from index 2      *
    ************************************************************/
   tmpLen = EG_LEN_TWO;

   EG_ENC_LEN(encLen, mBuf, tmpLen);

#ifdef EG_MULTI_IP
   tmpPst.srcInst = event->srvCb->tSapCb->pst.srcInst;
#else
#ifndef SS_MULTIPLE_PROCS
   tmpPst.srcInst = egCb.tSapCb->pst.srcInst;
#else
   tmpPst.srcInst = egCbPtr->tSapCb->pst.srcInst;
#endif /*SS_MULTIPLE_PROCS*/
#endif /* MULTI_IP */
   retVal = egTptSendMsg(event->srvCb,&(event->remAddr), mBuf, &tmpPst);
   if (retVal != ROK)
   {
       event->resultType = EGT_ERR_MSG_SNDING_FAIL;
       EG_DBG_ERR(0, 0, (egp,"\n Sending of Message Failed"));
   }   
   SPutMsg(mBuf);
   EG_FREE_GM_MSG(outEvntMsg->u.egMsg);
   EG_FREEEVNT(outEvntMsg);
   EG_RETVALUE(retVal);

} /* End of egTunnErrIncOutRsp () */

#endif /* End of EGTP_C */
#ifdef EGTP_U


/************************************************************************
 *   Fun:   egUEncodeGmHdr
 *   
 *   Desc:  This will Encode the GTP-U message Header.
 *   
 *   Ret:   ROK  - for Success
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUEncodeGmHdr
(
   EgUEvnt    *evntMsg,   /* Event message having the MsgHdr to encode hdr */
   Buffer     *mBuf       /* Mbuf which holds the encoded buffer */
 )
#else
PRIVATE S16 egUEncodeGmHdr( evntMsg, mBuf)
   EgUEvnt    *evntMsg;   /* Event message having the MsgHdr to encode hdr */
   Buffer     *mBuf;      /* Mbuf which holds the encoded buffer */
#endif
{

   S16        retVal  = ROK;        /* Holds the return value */
   EgUMsgHdr  *header = NULLP;      /* Temp stores the header struct for enc */
   U8         tmpByte = 0;          /* stores one byte of data for enc */
   U8         hdr[EGU_MSG_MAX_HDR_LEN];      /* stores one byte of data for enc */
   U8         cnt     = EGU_MSG_MAX_HDR_LEN; /* Stores the position */
   MsgLen     msgLen  = 0;          /* Holds message length, in case of G-PDU */ 
   /* eg006.201: Flag indicating presence of any one Seq Num, Ext Hdr, or PDCP  --*/
   Bool       extPres = FALSE;      /* Flag for indication of S, E or P presense flag */

   EG_TRC2(egUEncodeGPduHdr)

   header = &(evntMsg->egMsg->msgHdr);

   /* Version */
   tmpByte |= EG_MASK_BIT6;

   /* Protocol Type */
   tmpByte |= EG_MASK_BIT5;

   /*************************************************
    * Encode the Extension Header Flag, if present  *
    *************************************************/
   if(header->extHdr.udpPort.pres || header->extHdr.pdcpNmb.pres)
   {
      tmpByte |=  EG_MASK_BIT3;
   }

   /**************************************************
    * Encode the Sequence Number Flag, if present    *
    **************************************************/
   if(header->seqNumber.pres)
   {
      tmpByte |=  EG_MASK_BIT2;
   }

   /**************************************************
    * Encode the N-PDU Number Flag, if present       *
    *************************************************/
   if(header->nPduNmb.pres)
   {
      tmpByte |=  EG_MASK_BIT1;
   }

   /*-- eg006.201: Set extPres Flag if either one of the Bit fields(S,P,E) is set. If this
    * flag is set, all the optional fields for Seq No, Pdcp & Ext Hdr will be encoded into mBuf */

   if((tmpByte & EG_MASK_BIT1) || (tmpByte & EG_MASK_BIT2)||(tmpByte & EG_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /*****************************************************
    * Fill the header in the reversing order as finally *
    * we will be doing SAddPreMsgMult                   *
    *****************************************************/
   hdr[--cnt] = tmpByte;

   /*********************************************
    * Encode the Message Type                   *
    *********************************************/ 
   hdr[--cnt] = header->msgType;

   /*********************************************************
    * Encode the message length with zeroes                 *
    * At the end of the encoding all the IE's, the final    *
    * Length will be calculated and updated the mBuf        *
    ********************************************************/
   hdr[--cnt] = 0;
   hdr[--cnt] = 0;

   /**********************
    * Encode the Teid    *
    **********************/
   EG_GET_U32(header->teId, hdr, cnt);

   /*********************************************
    * Encode the sequence number,if present     *
    *********************************************/
   if(header->seqNumber.pres)
   {
      hdr[--cnt] = GetHiByte(header->seqNumber.val);
      hdr[--cnt] = GetLoByte(header->seqNumber.val);
   }

   /******************************************************************
    *   eg006.201 : If ExtPres is true, but Seq Num is not present, 
    * implies, either of E or P bit is set, so according to the spec 
    * the Byte fields will be padded with 0 for Seq Num */
   /****************************************************************
    * Spec says: This field is present if any of Seq Num, Ext Hdr  *
    * PN Present Flag is present, So we fill the field with Zeroes *
    * in case where Seq Num is not preset but other flags are set  *
    ****************************************************************/
   else if(extPres)
   {
      hdr[--cnt] = EG_ZERO;
      hdr[--cnt] = EG_ZERO;
   }

   /*********************************************
    * Encode the N-PDU Number, if present       *
    *********************************************/
   if(header->nPduNmb.pres)
   {
      hdr[--cnt] = header->nPduNmb.val;
   }

   /******************************************************************
    *   eg006.201 : If ExtPres is true, but Pdu Num is not present, 
    * implies, either of E or S bit is set, so according to the spec 
    * the Byte fields will be padded with 0 for Pdu Num */
   /****************************************************************
    * Spec says: This field is present if any of Seq Num, Ext Hdr  *
    * PN Present Flag is present, So we fill the field with Zeroes *
    * in case where Seq Num is not preset but other flags are set  *
    ****************************************************************/
   else  if(extPres)
   {
      hdr[--cnt] = EG_ZERO;
   }

   /*********************************************
    * Encode extension headers, if present      *
    *********************************************/
   if(header->extHdr.udpPort.pres)
   {
      hdr[--cnt] = EGU_EXT_HDR_UDP_TYPE;
      hdr[--cnt] = EG_ONE;
      hdr[--cnt] = GetHiByte(header->extHdr.udpPort.val);
      hdr[--cnt] = GetLoByte(header->extHdr.udpPort.val);
   }

   if(header->extHdr.pdcpNmb.pres)
   {
      hdr[--cnt] = EGU_EXT_HDR_PDCP_TYPE;
      hdr[--cnt] = EG_ONE;
      hdr[--cnt] = GetHiByte(header->extHdr.pdcpNmb.val);
      hdr[--cnt] = GetLoByte(header->extHdr.pdcpNmb.val);
   }

   if(tmpByte & EG_MASK_BIT3)
   {
      /****************************************************************
       * Add End of Extension Headers                                    *
       *******************************************************************/
      hdr[--cnt] = EG_ZERO;
   }

   /******************************************************************
    * eg006.201 : If ExtPres is true, but Ext hdr is not present, 
    * implies, either of P or S bit is set, so according to the spec 
    * the Byte fields will be padded with 0 for Pdu Num */
   /****************************************************************
    * Spec says: This field is present if any of Seq Num, Ext Hdr  *
    * PN Present Flag is present, So we fill the field with Zeroes *
    * in case where Seq Num is not preset but other flags are set  *
      ****************************************************************/
   else if(extPres)
   {
      hdr[--cnt] = EG_ZERO;
   }
    
   /***********************************************************
    * In case of G-PDU, calculate and update the final length *
    **********************************************************/
   if(header->msgType == EGT_GTPU_MSG_GPDU)
   {
      /***********************************************************
       * Calculate total Buffer length and decrement 8bytes, to  *
       * get the value tobe filled in the length field of the    *
       * message header                                          *
       ***********************************************************/
      msgLen = (evntMsg->tPduSize) + (EGU_MSG_MAX_HDR_LEN - cnt) - EG_LEN_EIGHT;

      /***********************************************
       * Fill the length field of the message header *
       ***********************************************/
      hdr[EGU_MSG_MAX_HDR_LEN - 3] = (U8)GetHiByte(msgLen);
      hdr[EGU_MSG_MAX_HDR_LEN - 4] = (U8)GetLoByte(msgLen);
   }

   /*****************************************
    * Add the header to the existing mBuf.  *
    *****************************************/
   /***************************************************************************
    * In case of non G-PDU messages, existing mBuf will be empty              *
    *                                                                         *
    * In case of G-PDUs existing mBuf will have T-PDU, i.e mBuf coming from   *
    * application as T-PDU                                                    *
    ***************************************************************************/
   SAddPreMsgMult(&hdr[cnt], (EGU_MSG_MAX_HDR_LEN - cnt), mBuf);
   EG_RETVALUE(retVal);

} /* End of egUEncodeGmHdr() */


/***********************************************************************
 *   Fun:  egUEncMsgReq
 *   
 *   Desc:  This will Encode the GTP-U message receieved from 
 *          the application
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c  
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egUEncMsgReq
(
   EgUEvnt *evntMsg        /* Event message having the Msg to encode msg */
)
#else
PUBLIC S16 egUEncMsgReq(evntMsg)
   EgUEvnt *evntMsg;       /* Event message having the Msg to encode msg */
#endif
{
   S16               retVal = ROK;         /* Holds the return value */
   /* eg012.201 : Changed datatype for wrap around condition */
   U16               msgType = 0;          /* For storing the msg Type */
   U16               cnt    = 0;           /* For iterating thru Node Lists */
   /*eg009.201:Klockwork fix datatype modified*/
   U32               ieCnt  = 0;           /* For storing the nmb ie in list */
   U16               tmpCnt = 0;           /* For storing the nmb ie in list */
   Bool              expIe  = TRUE;        /* expected IE or NOT */
   Bool              iePres[EGU_MAX_IE]; /* Holds whether IE is present or not */
   Buffer            *mBuf  = NULLP;       /* Stores the Encoded Buffer */
   CmLList           *tmpNode = NULLP;     /* Stores the Ie Node */
   CmLList           *tmpList = NULLP;     /* Stores Ie List */
   EgUstaDgn         dgn;      /* diagnostics for sending alaram to LM */
   Mem               mem;      /* For allocating memory using reg, pool */
   MsgLen            encLen = 0;           /* Temp stores the encoded length */
   EgUIe             *tmpIe = NULLP;       /* Holds the IE NODE */
   EgIeEntry         *ieEntry = NULLP;     /* Holds IE Entry for each IE */
   EgUMsgEntry       *gmEntry = NULLP;     /* Holds the message Entry */
   MsgLen            tPduSize = 0;         /* T-PDU Size */
   U16               gmPosIdx = 0;         /* Message Poiton Idx */
   U16               iePosIdx = 0;         /* Ie Poiton Idx */
   U16               tempLen = 0;         /* tmp length */
#ifdef EG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16               nmbLines = 0;         /* No. of lines */
#endif
#endif

   EG_TRC2(egUEncMsgReq);

   /**************************************************
    * Check for event message being proper           *
    **************************************************/
   if (!evntMsg)
   {
      EG_DBG_INFO(0, 0, (egp,
               "egUEncMsgReq: evntMsg is NULLP \n"));
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /******************************************************
    * Check for eGTP message to be encoded being proper  *
    ******************************************************/ 
   if (!evntMsg->egMsg)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_DBG_INFO(0, 0, (egp,
               "egUEncMsgReq: egMsg is NULLP \n"));

      /*************************************
       * Fill and send error event message *
       *************************************/ 
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0);
      EG_RETVALUE(retVal);
   }

   /*************************
    * Copy region and pool  *
    ************************/
   mem.region = evntMsg->region;
   mem.pool   = evntMsg->pool;

#ifdef _EG_PRINT_BUF_
   EgUUtilGmDump(evntMsg->egMsg
#ifdef EG_FILE_LOG
         , egCb.dbgFp, &nmbLines
#endif /* end of EG_FILE_LOG */
         );
#endif

   /*************************
    * Update Message Type   *
    *************************/
   msgType = evntMsg->egMsg->msgHdr.msgType;

   if(msgType == EGT_GTPU_MSG_GPDU)
   {
      /***************************** 
       * eg009.201 - Fix TEID zero *
       *****************************/
      /**************************************************************
       * Validate T-PDU size, based on configured max size of T-PDU *
       **************************************************************/
      SFndLenMsg(evntMsg->egMsg->u.mBuf, &tPduSize);
      if((evntMsg->maxTPduSize) && (tPduSize > evntMsg->maxTPduSize))
      {

         /*-- eg003.201: use format specifier according to datatype --*/
#ifdef LONG_MSG
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: T-PDU size: %ld exceeds the maximum configured value: %d \n",
                  tPduSize, evntMsg->maxTPduSize));
#else
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: T-PDU size: %hd exceeds the maximum configured value: %d \n",
                  tPduSize, evntMsg->maxTPduSize));
#endif

         retVal = EGT_ERR_MAX_TDPU_SIZE_EXCED;
         /****************************************************
          * Fill and send error event message                *
          ****************************************************/ 
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType);
         EG_RETVALUE(retVal);
      }

      /*************************************
       * Update the T-PDU size to evntMsg  *
       *************************************/
      evntMsg->tPduSize = tPduSize;

      /*****************************
       * Encode the message header *
       ****************************/      
      egUEncodeGmHdr(evntMsg, evntMsg->egMsg->u.mBuf);

      /*****************************************
       * Update the buffer pointer to evntMsg  *
       *****************************************/
      evntMsg->mBuf = evntMsg->egMsg->u.mBuf;

      /**************************
       * Return Success         *
       **************************/
      evntMsg->resultType = EG_SUCCESS;
      evntMsg->egMsg->u.mBuf = NULLP;

      EgUUtilDeAllocGmMsg(&(evntMsg->egMsg));
      /* eg012.201 : updated return value */
      EG_RETVALUE(ROK);
   } /* End of G-PDU */

   /********************************************************
    * Echo Request / Response dont need any validation as  *
    * they are generated by GTP-U layer itself             *
    * ******************************************************/
   /*************************************************************
    * Error Indication message, Supported Extension header      *
    * Notification messages generated by GTP-U layer dont need  *
    * any validation                                            *
    * ***********************************************************/
   else if((msgType != EGT_GTPU_MSG_ECHO_REQ) && (msgType != EGT_GTPU_MSG_ECHO_RSP) &&
         !(msgType == EGT_GTPU_MSG_ERROR_IND && evntMsg->errInd == TRUE) && 
         !(msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF && evntMsg->suppExtHdrNtf == TRUE))
   {
      /***********************************
       * Initialize the iePres structure *
       ***********************************/
      cmMemset((Bool *)iePres, EG_ZERO, sizeof(iePres));

      evntMsg->isEnc = TRUE;

      /******************************************************
       * Validate the eGTP message header                   *
       ******************************************************/
      retVal = egUValidateGmHdr(evntMsg);   
      if(retVal != ROK)
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: Message validation is failed \n"));
         /****************************************************
          * Fill and send error event message                *
          ****************************************************/ 
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType);
         EG_RETVALUE(retVal);
      }
      /***************************************************
       *  Load the message entry to gmEntry              *
       ***************************************************/
      /* eg012.201 : Added array out of bound checks */
      if ((msgType >= (U16)EGU_MAX_MSG) || (egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType] >= (U16)EGU_SUP_MSGS))
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: Message decoding  has failed \n"));
         /****************************************************
          * Fill and send error event message                *
          ****************************************************/
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType);
         EG_RETVALUE(retVal);
      }
      gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
      gmEntry =  &(egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx]);

      CM_LLIST_FIRST_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      ieCnt = evntMsg->egMsg->u.egIeList.count;
      /* eg012.201 : Added check for ieCnt */
      if ((NULLP == tmpNode) && (ieCnt > EG_ZERO))
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: IeList is NULLP, Message decoding  has failed \n"));
         /****************************************************
          * Fill and send error event message                *
          ****************************************************/
         EGU_FILL_ERR_EVNT_MSG(evntMsg, EGT_ERR_WRONG_EXT_HDR, EGU_IE_TEID, msgType);
         EG_RETVALUE(retVal);
      }

      /***************************************************
       * Itearate through the list of nodes in the egMsg *
       * for doing the validation of IEs                 *
       ***************************************************/
      for(cnt = 0; cnt < ieCnt; cnt++)
      {
         tmpIe = (EgUIe*)tmpNode->node;

         /********************************************
          * Get the IE entry from the IE Dictionary  *
          ********************************************/
         /* eg012.201 : Added verification for null pointer and array out of bounds verification */
         if((tmpIe) && (iePosIdx = egCb.egUCb.edmUCb.egIeCb.posIdx[tmpIe->ieType]) )
         {
            ieEntry = &(egCb.egUCb.edmUCb.egIeCb.ieEntry[iePosIdx]);
         }
         else
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************
             * Delete the Unknown IE               *
             * *************************************/
            /* eg012.201 : Added NULLP check here */
            if (tmpIe)
            {
               EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
               EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the Unknown IE(%d)\n",
                           tmpIe->ieType));
            }

            /***************************
             * Resoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /***********************************************
          * Verify whether the ie Data type is matching *
          * with the expected datatype                  *
          * *********************************************/
         /* eg012.201: klocwork fixes added */
         if ( ieEntry->dataType != tmpIe->dataType)
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************
             * Delete IE with wrong DataType       *
             ***************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the IE(%d) with wrong datatype\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /******************************************************
          * For IEs of type EG_IP_ADDR, address type should be *
          * Ipv4 or IPv6                                       *
          ******************************************************/
         if((tmpIe->dataType == EG_IP_ADDR) &&
               ((tmpIe->t.ipAddr.addrType != CM_TPTADDR_IPV4) && 
                (tmpIe->t.ipAddr.addrType != CM_TPTADDR_IPV6)))
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************
             * Delete IE with wrong Data           *
             ***************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the IE(%d) with wrong data\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /******************************************************************
          * Store ie presence value in iePres, while avoiding duplicates. *
          * Duplicates are allowed for Private Extension IE                *
          ******************************************************************/
         if((iePres[tmpIe->ieType] == FALSE) || 
               (tmpIe->ieType == EGU_IE_PVT_EXTN))
         {
            iePres[tmpIe->ieType] = TRUE;
         }
         else
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /***************************************************
             * Case of repeated IE : Skip this IE and continue *
             * further processing                              *
             ***************************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_TWO);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the duplicate IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }

         /******************************************
          * Initializing expected IE to FALSE      *
          * ****************************************/
         expIe = FALSE;
         /*************************************************
          * Iterate through the message dict. to identify *
          * Unexpected IEs                                *
          *************************************************/ 
         for ( tmpCnt=0; tmpCnt < gmEntry->nmbIe; tmpCnt++)
         {
            if (gmEntry->ieProperties[tmpCnt].ieType == tmpIe->ieType)
            {
               expIe = TRUE;
               break;
            }   
         }  

         if(!expIe)
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;


            /*************************************
             * Delete Unexpected IE              *
             *************************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUEncIe(Deleted the Unexpected IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Restoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;

            CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
            continue;
         }
         CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      }

      /*****************************************
       * Do more validation on the IE's        *
       *****************************************/
      retVal = egUValidateGmMsgIEs(evntMsg, iePres);
      if(retVal != ROK)
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: IE validation is failed \n"));
         EG_RETVALUE(retVal);
      }
   }
   /*********************************************************
    * Validation is successful, do the encoding             * 
    * Allocate memory for the buffer                        *
    *********************************************************/ 
   retVal = SGetMsg(mem.region, mem.pool, &mBuf);
   if(retVal != ROK)
   {
      /********************************************************
       * Memory allocation failed, send alarm to LM and send  *
       * error event message                                  * 
       ********************************************************/ 
      cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
      EGU_SND_ALARM_MEM_ALLOC_FAIL(mem.region, mem.pool, dgn);

      retVal = EGT_ERR_MEM_ALLOC_FAILED;
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, msgType); 
      EG_RETVALUE(retVal);
   }

   /*****************************
    * Encode the message header *
    ****************************/
   egUEncodeGmHdr(evntMsg, mBuf);

   /*********************************
    * Encode the IEs one by one     *
    *********************************/
   CM_LLIST_FIRST_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
   for(cnt = 0; cnt < evntMsg->egMsg->u.egIeList.count; cnt++)
   {
      /*****************************
       * Get the IE entry          *
       ****************************/
      tmpIe = (EgUIe*)tmpNode->node;

      /*****************************
       * Encode the IE             *
       ****************************/
      retVal = egUEncIe(tmpIe, mBuf);
      if(retVal != ROK)
      {
         EG_DBG_INFO(0,0, (egp,
                  "egUEncMsgReq: IE encoding is failed \n"));

         if(retVal == EGT_ERR_INV_IE_VAL)
         {
            EG_DBG_INFO(0,0, (egp, "egUEncIe(IE(%d), resultType(%d)) \n",
                     tmpIe->ieType,  retVal));
         }
         /**************************************
          * Fill and send error event message  * 
          * free buffer                        *
          **************************************/ 
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe->ieType, msgType); 
         SPutMsg(mBuf);
         EG_RETVALUE(retVal);
      }
      else
      {
         /*********************************
          * Get the next IE from the list *
          ********************************/
         CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      }
   }

   /************************************************************
    * Calculate total Buffer length and decrement 8bytes, to   *
    * get the value tobe filled in the length field of the     *
    * message header                                           *
    ************************************************************/
   SFndLenMsg(mBuf, &encLen);
   encLen -= EG_LEN_EIGHT;

   /************************************************************
    * Fill the message length, length starts from index 2      *
    ************************************************************/
   tempLen = EG_LEN_TWO;
   EG_ENC_LEN(encLen, mBuf, tempLen);

   /**************************************
    * Free the event structure received  *       
    **************************************/
   if(msgType != EGT_GTPU_MSG_ECHO_REQ)
   {
      /* eg009.201: egMsg & memory allocated to IE inside egMsg
       * is freed with EgUUtilDeAllocGmMsg */
      EgUUtilDeAllocGmMsg(&(evntMsg->egMsg));      
   }
   /****************************************
    * Assign the mBuf in the event message *     
    ***************************************/
   evntMsg->mBuf = mBuf;

   /************************************************************
    * Send the message to main thread with SUCCESS result code *
    ************************************************************/
   evntMsg->resultType = EG_SUCCESS;

   EG_RETVALUE(retVal);
} /* End of egUEncMsgReq() */



/***********************************************************************
 *   Fun:  egUEncIeData
 *
 *   Desc:  Encode the GTP-U Msg IE Data part
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUEncIeData
(
   Buffer    *mBuf,                    /* Buffer to hold the Encoded msg */
   EgUIe     *egIe                     /* IE to encode the data portion */
 ) 
#else
PRIVATE S16 egUEncIeData(mBuf, egIe)
   Buffer    *mBuf;                    /* Buffer to hold the Encoded msg */
   EgUIe     *egIe;                    /* IE to encode the data portion */
#endif
{
   S16         retVal = ROK;           /* Holds the return value */

   EG_TRC2(egUEncIeData);

   EG_DBG_INFO(0,0, (egp,
            "egUEncIeData(IE Type(%d), IE Data Type(%d)) \n",
            egIe->ieType, egIe->dataType));

   /*********************************************
    * Decode the IE based on DataType of the IE *
    *********************************************/
   switch (egIe->dataType)
   {
      case EG_UNSIGNED_8:
         SAddPstMsg(egIe->t.valUnSgnd8, mBuf);
         break;

      case EG_UNSIGNED_32:
         EG_ENC_U32(egIe->t.valUnSgnd32, mBuf);
         break;

      case EG_PVT_EXTN:
         EG_ENC_U16(egIe->t.pvtExt.entId, mBuf);
         SAddPstMsgMult(egIe->t.pvtExt.propVal.val,
               egIe->t.pvtExt.propVal.length, mBuf);
         break;

      case EG_IP_ADDR:
         if(egIe->t.ipAddr.addrType == CM_TPTADDR_IPV4)
         {
            EG_ENC_U32(egIe->t.ipAddr.u.ipv4, mBuf);
         }
#ifdef EG_IPV6_SUPPORTED
         else if (egIe->t.ipAddr.addrType == CM_TPTADDR_IPV6)
         {
            SAddPstMsgMult(egIe->t.ipAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         }
#endif /* EG_IPV6_SUPPORTED */
         break;

      case EG_EXT_HDR_TYPE_LST:
         {
            U16 cnt = 0;
            if(egIe->t.extHdrTypeLst.noOfExtHdrs > EGT_U_MAX_EXT_TYPES)
            {
               retVal = EGT_ERR_INV_IE_VAL;
            }
            for(cnt = 0; cnt < egIe->t.extHdrTypeLst.noOfExtHdrs; cnt++)
               SAddPstMsg(egIe->t.extHdrTypeLst.extType[cnt], mBuf);
            break;
         }

#ifdef ERRCHK
         /********************************************************
          * Default is not required as datatype validation is    *
          * already done in egUEncMsgReq                         *
          ********************************************************/
      default:
         EG_DBG_INFO(0,0, (egp,
                  "egUEncIeData(Invalied IE Data Type: %d)\n", egIe->dataType));
         retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
#endif
   }

   EG_RETVALUE(retVal);    
} /* End of egUEncIeData() */



/***********************************************************************
 *   Fun:  egUEncIe
 *   
 *   Desc:  Encodes the IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUEncIe
(
   EgUIe   *ie,                          /* IE to encode */
   Buffer  *mBuf                         /* Buffer holds the encoded IE */
 )
#else
PRIVATE S16 egUEncIe(ie, mBuf)
   EgUIe   *ie;                          /* IE to encode */
   Buffer  *mBuf;                        /* Buffer holds the encoded IE */
#endif
{
   S16      retVal = ROK;                /* Holds the return value */
   MsgLen   initLen = 0;    /* Initial mBuf Length for Grp IE encoding */
   MsgLen   finLen = 0;     /* final mBuf Length for Grp IE encoding */
   MsgLen   ieLen = 0;                  /* actual length of IE being encoded */
   Data     dumBytes[EG_LEN_TWO];     /* Tmp stores the data for Enc */
   Data     dumBytes1[EG_LEN_ONE];     /* Tmp stores the data for Enc */
   EgIeEntry  *ieEntry = NULLP;     /* Holds IE Entry for the IE */
   U16         iePosIdx = 0;         /* Ie Poiton Idx */

   EG_TRC2(egUEncIe);

   cmMemset((U8*)dumBytes, EG_ZERO, EG_LEN_TWO);
   cmMemset((U8*)dumBytes1, EG_ZERO, EG_LEN_ONE);

   /*************************
    * Check for IE validity *
    ************************/
   if(ie == NULLP)
   {
      retVal = EGT_ERR_MSG_UNABLE_TO_COMPLY;
      EG_RETVALUE(retVal);
   }

   /*********************
    * Debug information *
    ********************/
   EG_DBG_INFO(0,0, (egp,
            "egUEncIe(IE Type(%d) \n",ie->ieType));

   /********************************************
    * Get the IE entry from the IE Dictionary  *
    ********************************************/
   iePosIdx = egCb.egUCb.edmUCb.egIeCb.posIdx[ie->ieType];
   ieEntry = &(egCb.egUCb.edmUCb.egIeCb.ieEntry[iePosIdx]);

   /***************************************
    * Store the index to encode IE length *
    ***************************************/
   SFndLenMsg(mBuf, &initLen);

   /**********************
    * Encode the IE Type *
    **********************/
   SAddPstMsg(ie->ieType, mBuf);

   /************************************************************
    * Encode dummy bytes in the length field, for TLV type IEs *
    ************************************************************/
   if(!ieEntry->tvType)
   {
      if(ie->ieType != EGU_IE_EXT_HDR_LST)
      {
         SAddPstMsgMult((U8*)dumBytes, EG_LEN_TWO, mBuf);
      }
      else
      {
         SAddPstMsgMult((U8*)dumBytes1, EG_LEN_ONE, mBuf);
      }
   }

   /**********************
    * Encode the IE data  *
    ***********************/ 
   retVal = egUEncIeData(mBuf, ie);
   if(retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUEncMsgReq: IE Data Encoding is failed \n"));
      EG_RETVALUE(retVal);
   }

   /*********************************************
    * Insert Correct IE length for TLV type IEs *
    *********************************************/
   if(!ieEntry->tvType)
   {
      /********************************************
       * Calculate final length of the buffer     *
       ********************************************/
      SFndLenMsg(mBuf, &finLen);

      /***********************************
       * Decrement 3bytes for the ie Hdr *
       ***********************************/
      if(ie->ieType != EGU_IE_EXT_HDR_LST)
      {
         ieLen = finLen - initLen - EG_LEN_THREE;
      }
      else
      {
         ieLen = finLen - initLen - EG_LEN_TWO;
      }

      /***********************************************************
       * Increment the index to point the actual length location *
       ***********************************************************/
      initLen += EG_IE_LEN_OFFSET;

      /********************************************
       * Encode the offset b/w finLen and initLen *
       ********************************************/
      if(ie->ieType != EGU_IE_EXT_HDR_LST)
      {
         EG_ENC_LEN(ieLen, mBuf, initLen);
      }
      else
      {
         SRepMsg(ieLen, mBuf, initLen);
      }
   } 

   EG_RETVALUE(retVal);
} /* End of egUEncIe() */



/***********************************************************************
 *   Fun:  egUDecMsgReq
 *   
 *   Desc:  Decodes the GTP-U Msg and then validate it.
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egUDecMsgReq
(
   EgUEvnt  *evntMsg        /* Event message having the Msg to decode msg */
)
#else
PUBLIC S16 egUDecMsgReq( evntMsg)
   EgUEvnt  *evntMsg;       /* Event message having the Msg to decode msg */
#endif
{

   /***************************************************
    * Local variables required for non G-PDU message  *
    ***************************************************/

   S16            retVal  = ROK;       /* stores the return value */
   Buffer         *mBuf   = NULLP;     /* stores buffer present in event msg */
   Mem            mem;                 /* Memory used for allocating */
   EgUstaDgn      dgn;               /* diagnostics for sending alaram to LM */
   U16            cnt     = 0;         /* For iterating thru Node Lists */
   /*eg009.201:Klockwork fix Modified datatype*/
   U32            ieCnt   = 0;         /* Loop to iterate thru IE's */
   MsgLen         msgLen  = 0;         /* Stores Message len. after decoding */
   EgUMsgEntry    *gmEntry = NULLP;    /* Pointer to Message entry */
   U8             lastDecIeType = 0;   /* Last Decoded IE type */
   Bool           iePres[EGU_MAX_IE];    /* Holds IE Presence value */
   U8             msgType = 0;          /* Message Type */
   U8             ieType = 0;           /* Ie Type */
   U16            gmPosIdx = 0;         /* Message Position Idx */
   CmLList        *tmpNode = NULLP;     /* Stores the Ie Node */
   CmLList        *tmpList = NULLP;     /* Ie list */
   EgUIe          *tmpIe = NULLP;       /* tmp IE */
   Bool           expIe  = TRUE;        /* expected IE or NOT */
   U16            tmpCnt     = 0;         /* For iterating thru Node Lists */

#ifdef EG_FILE_LOG 
#ifdef _EG_PRINT_BUF_
   U16             nmbLines = 0;        /* number of Lines */ 
#endif
#endif 

   EG_TRC2(egUDecMsgReq);

   /* eg012.201: klocwork fix : removed check, NULL check already done in egUCpmHdlEvt */

   /*********************************************************
    * Initialize memory region and pool                     *
    *********************************************************/
   mem.region = evntMsg->region;
   mem.pool   = evntMsg->pool;

   /* eg012.201: klocwork fix : removed check, NULL check already done in EgActvTsk */

   /*********************************************************
    * Assign the buffer to the local pointer and clear the  *
    * event structure buffer                                *
    *********************************************************/
   mBuf = evntMsg->mBuf;
   evntMsg->mBuf = NULLP;

   /*********************************************************
    * Allocate the memory for the event structure           *
    *********************************************************/
   if(cmAllocEvnt(sizeof(EgUMsg), EGU_MEM_SDU_SIZE, 
            &mem, (Ptr*)&evntMsg->egMsg) != ROK)
   {
      EG_DBG_INFO(0, 0, (egp,
               "egUDecMsgReq: Memory allocation failed for egMsg \n"));

      /******************************************************
       * Fill and send error event message                  *
       * Rree memory associated with mBuf    .              * 
       ******************************************************/ 
      cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
      EGU_SND_ALARM_MEM_ALLOC_FAIL(mem.region, mem.pool, dgn);
      SPutMsg(mBuf);
      /* eg012.201: klocwork fix : removed since returning here */
      EG_RETVALUE(EGT_ERR_MSG_UNABLE_TO_COMPLY);
   }

   /**************************************************
    * Decode the GTP-U message header                *
    **************************************************/
   retVal = egUDecodeGmHdr(evntMsg, mBuf);
   if(retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUDecMsgReq: Message header decoding is failed \n"));
      /*******************
       * free the memory *
       ******************/
      SPutMsg(mBuf);
      /*eg012.201: klocwork fix : removed since returning here */
      evntMsg->resultType = retVal;
      EG_RETVALUE(retVal);
   }

   /**************************************************
    *  Decode the IEs, if message type is not GPDU   *
    **************************************************/
   if(evntMsg->egMsg->msgHdr.msgType != EGT_GTPU_MSG_GPDU)
   {
      /**************************************************
       * Initialize iePres array                        *
       **************************************************/
      cmMemset((Bool *)iePres, EG_ZERO, sizeof(iePres));


      msgType = evntMsg->egMsg->msgHdr.msgType;

      /**********************************************
       *  Loop through to Decode the GTP IE's       *
       **********************************************/
      SFndLenMsg(mBuf, &msgLen);
      while (msgLen)
      {
         /************************
          *  Decode the IE       *
          ************************/
         retVal = egUDecIe(evntMsg, mBuf, iePres, &ieType);
         /*******************************************
          * If decoding of IE fails then free the   *
          * the message buffer and retrun from here *
          *******************************************/
         /* eg012.201: klocwork fix: combining both the error checks together */
			/* Compromise for performance in case of error is OK */
         if ((retVal != ROK) || (ieType < lastDecIeType))
         {
            if (retVal != ROK)
            {
               EG_DBG_INFO(0,0, (egp,
                     "egUDecMsgReq: IE decoding is failed \n"));
            }
				/****************************************************************
				* If decodded IE type is less than the last decoded IE type,    *
				* i.e IEs are not received in ascending order, free mBuf & ret  *
				****************************************************************/
				else
				{
					retVal = EGT_ERR_IE_NOT_ASCD_ORDER;
				}
            SPutMsg(mBuf);
			evntMsg->resultType = retVal;
            EG_RETVALUE(retVal);
         }
		 /* eg012.201: klocwork fix: removed check here */

         /***************************************
          * Update the last decoded IE type     *
          ***************************************/
         lastDecIeType = ieType;

         SFndLenMsg(mBuf, &msgLen);
      } 

      gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
      gmEntry =  &egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx];

      /********************************************************************
       * Itearate through the list of IEs in egMsg, while comparing with  *
       * the Message dict. to find out Unexpected IEs                     *
       ********************************************************************/
      CM_LLIST_FIRST_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      ieCnt = evntMsg->egMsg->u.egIeList.count;
      for(cnt = 0; cnt < ieCnt; cnt++)
      {
         tmpIe = (EgUIe*)tmpNode->node;
         expIe = FALSE;
         for ( tmpCnt=0; tmpCnt < gmEntry->nmbIe; tmpCnt++)
         {
            if (gmEntry->ieProperties[tmpCnt].ieType == tmpIe->ieType)
            {
               /********************
                * Expected IE      *
                ********************/
               expIe = TRUE;
               break;
            }
         }
         if(!expIe)
         {
            /****************************************
             * Store the previous element details   *
             ****************************************/
            tmpList = tmpNode->prev;

            /*******************************
             * Delete the unexpected IEs   *
             *******************************/
            EgUUtilGmDelIe(evntMsg->egMsg, tmpIe->ieType, EG_ONE);
            EG_DBG_INFO(0,0, (egp, "egUDecIe(Deleted the unexpected IE(%d)\n",
                     tmpIe->ieType));

            /***************************
             * Resoring the crnt value *
             ***************************/
            evntMsg->egMsg->u.egIeList.crnt = tmpList;
         }

         CM_LLIST_NEXT_NODE(&evntMsg->egMsg->u.egIeList, tmpNode);
      }

      if(gmEntry->nmbMandIe)
      {
         /************************************************************
          * Validate the IE's, if the message has some mandatory IEs *
          ************************************************************/
         retVal = egUValidateGmMsgIEs(evntMsg, iePres);
         if(retVal != ROK)
         {
            EG_DBG_INFO(0,0, (egp,
                     "egUDecMsgReq: Message validation is failed \n"));
            /******************************************
             * Fill and send error event message and  *
             * free memory associated with mBuf.      * 
             ******************************************/ 
            SPutMsg(mBuf);   
            /* eg012.201 : klocwork fix : removed since returning here */
            evntMsg->resultType = retVal;
            EG_RETVALUE(retVal);
         }
      }

      /****************************************************************
       * Decoding is successful, free the memory associated with mBuf *
       ****************************************************************/
      SPutMsg(mBuf);
      /* eg012.201: klocwork fix : removed since returning after this code */
   }
   else
   {
      /****************************************************
       * Fill egMsg with the remaining buffer, for GPDUs  *
       ***************************************************/
      evntMsg->egMsg->u.mBuf = mBuf;
   }

   /**********************
    * Return as SUCCESS  *
    **********************/
   evntMsg->resultType = EG_SUCCESS;

#ifdef _EG_PRINT_BUF_
   EgUUtilGmDump(evntMsg->egMsg
#ifdef EG_FILE_LOG
         , egCb.dbgFp, &nmbLines
#endif /* end of EG_FILE_LOG */
         );
#endif /* end of _EG_PRINT_BUF_ */

   EG_RETVALUE(retVal);
} /* End of egUDecMsgReq() */



/***********************************************************************
 *   Fun:  egUDecodeGmHdr
 *   
 *   Desc:  Decodes the GTP-U Msg Header
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUDecodeGmHdr
(
   EgUEvnt    *evntMsg,         /* Event message stores the decoded hdr */
   Buffer     *mBuf             /* Buffer to decode the header */
 )
#else
PRIVATE S16 egUDecodeGmHdr(evntMsg, mBuf)
   EgUEvnt     *evntMsg;         /* Event message stores the decoded hdr */
   Buffer      *mBuf;            /* Buffer to decode the header */
#endif
{
   S16      retVal  = ROK;       /* Holds the return value */
   U8       tmpByte = 0;         /* Holds one byte of data after Dec */
   U8       version = 0;         /* Holds the version type, decoded */
   MsgLen   msgLen  = 0;         /* Holds the msgLen from the Hdr */
   MsgLen   bufLen  = 0;         /* Holds the total buffer length */
   U8      extHdrType = 0;       /* Holds the Extension hdr type */
   U8      extHdrLen = 0;        /* Extension hdr length */
   Bool    unKnownExtHdr = 0;    /* Is Unknown Extension Hdr? */
   /* eg006.201: Flag indicating presence of any one Seq Num, Ext Hdr, or PDCP  --*/
   Bool       extPres = FALSE;      /* Flag for indication of S, E or P presense flag */

   EG_TRC2(egUDecodeGmHdr);


   /*********************************************
    * Check the length of buffer to be decoded  *
    *********************************************/
   SFndLenMsg(mBuf, &bufLen);

#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp,
            "egUDecodeGmHdr(MsgLen(%ld)) \n", bufLen));
#else            
   EG_DBG_INFO(0,0, (egp,
            "egUDecodeGmHdr(MsgLen(%d)) \n", bufLen));
#endif
#endif

   /*************************************************
    * Store the first byte in tmpByte, Right shift  *
    * 5 times to get version                        *
    *************************************************/
   SRemPreMsg(&tmpByte, mBuf);
   version = tmpByte >> EG_SHIFT_5;

   /*********************************************************
    * Check if the version is 1, if not report INV version  *
    *********************************************************/
   if(version != EGU_MSG_VERSION)
   {
      retVal = EGT_ERR_MSG_INV_VERSION;
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Invalid Msg Version(%d), \
         resultType(%d) \n",version, retVal));

      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0);
      EG_RETVALUE(retVal);
   }

   /**************************************************************
    * Decode the Protocol Type, and if it is not 1, return error *
    **************************************************************/
   if (!(tmpByte & EG_MASK_BIT5))
   {
      retVal = EGT_ERR_MSG_INV_PTTYPE;
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Invalid Protocol Type(%d), \
         resultType(%d) \n",0, retVal));

      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0, 0);
      EG_RETVALUE(retVal);
   }

   /******************************
    * decode the message Type    *
    ******************************/
   SRemPreMsg((Data*)&(evntMsg->egMsg->msgHdr.msgType), mBuf);

   /******************************
    * decode the message length  *
    *****************************/
   EG_DEC_U16(msgLen, mBuf);


   /*-- eg006.201: Aligning Decoder code to spec --*/
   if((tmpByte & EG_MASK_BIT1) || (tmpByte & EG_MASK_BIT2)||(tmpByte & EG_MASK_BIT3))
   {
      extPres = TRUE;
   }

   /*******************************************************************
    * The message length should not have the first 8 mandatory octets *
    * Check whether the message lenght is 8 octets less than the      *
    * total buffer length                                             *
    ******************************************************************/
   if(msgLen != bufLen - EG_LEN_EIGHT)
   {
      retVal = EGT_ERR_MSG_INV_LEN;
#ifdef LONG_MSG   
#ifndef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Decoded MsgLen(%ld), actual MsgLen(%ld), \
         resultType(%d)) -- Invalid Length \n", 
               msgLen, bufLen, retVal));
#else            
      EG_DBG_INFO(0,0, (egp,
               "egUDecodeGmHdr(Decoded MsgLen(%d), actual MsgLen(%d), \
         resultType(%d)) -- Invalid Length \n", 
               msgLen, bufLen, retVal));
#endif            
#endif            
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, 0,
            evntMsg->egMsg->msgHdr.msgType);
      EG_RETVALUE(retVal);
   }

   EG_DEC_U32(evntMsg->egMsg->msgHdr.teId, mBuf);

   /***********************************************
    * Decode the Sequence Number, if S-bit is set *
    ***********************************************/
   if ( tmpByte & EG_MASK_BIT2 )
   {
      evntMsg->egMsg->msgHdr.seqNumber.pres = PRSNT_NODEF;
      EG_DEC_U16(evntMsg->egMsg->msgHdr.seqNumber.val, mBuf);
   }

   /******************************************************************
    * eg006.201 : If ExtPres is true, but Seq NUm bit is not set, 
    * implies, either of P or S bit was set during Encode, so accordingly
    * extract Byte fields from mBuf will be extracted with 0 for Seq Num
   ******************************************************************/
   else  if(extPres)
   {
      evntMsg->egMsg->msgHdr.seqNumber.pres = 0;
      EG_DEC_U16(evntMsg->egMsg->msgHdr.seqNumber.val, mBuf);
   }

   /*********************************************
    * Decode the N-PDU Number, if PN-bit is set *
    *********************************************/
   if ( tmpByte & EG_MASK_BIT1 )
   {
      evntMsg->egMsg->msgHdr.nPduNmb.pres = PRSNT_NODEF;
      SRemPreMsg(&(evntMsg->egMsg->msgHdr.nPduNmb.val), mBuf);
   }
   /******************************************************************
    * eg006.201 : If ExtPres is true, but Pdu NUm bit is not set, 
    * implies, either of E or S bit was set during Encode  so accordingly 
    * extract  Byte fields will be extracted form Mbuf with 0 for Pdu Num
   ******************************************************************/
   else  if(extPres)
   {
      evntMsg->egMsg->msgHdr.nPduNmb.pres = 0;
      SRemPreMsg(&(evntMsg->egMsg->msgHdr.nPduNmb.val), mBuf);
   }

   /************************************************
    * Decode the Extension Header, if E-bit is set *
    ************************************************/
   if(tmpByte & EG_MASK_BIT3)
   {
      SRemPreMsg(&extHdrType, mBuf);
      while(extHdrType)
      {
         retVal = ROK;
         unKnownExtHdr = FALSE;

         switch (extHdrType)
         {
            /*********************************************
             * Decode Extension Header of type UDP Port  *
             *********************************************/
            case EGU_EXT_HDR_UDP_TYPE:
               SRemPreMsg(&extHdrLen, mBuf);
               if(extHdrLen == EG_LEN_ONE)
               {
                  evntMsg->egMsg->msgHdr.extHdr.udpPort.pres = PRSNT_NODEF;
                  EG_DEC_U16(evntMsg->egMsg->msgHdr.extHdr.udpPort.val, mBuf);
               }
               else
               {
                  /********************************************************
                   * If the Ext. Header length is wrong, get the CR type  *
                   ********************************************************/
                  retVal = egUGetCRType(extHdrType, extHdrLen, mBuf);
                  EG_DBG_INFO(0,0, (egp,
                           "egUDecodeGmHdr(Received Ext. Hdr :%d with wrong \
                     length : %d -- Actual length :%d\n", 
                           extHdrType, extHdrLen, EG_LEN_ONE));
               }
               break;

               /****************************************************
                * Decode Extension Header of type PDCP PDU Number  *
                ****************************************************/
            case EGU_EXT_HDR_PDCP_TYPE:
               SRemPreMsg(&extHdrLen, mBuf);
               if(extHdrLen == EG_LEN_ONE)
               {
                  evntMsg->egMsg->msgHdr.extHdr.pdcpNmb.pres = PRSNT_NODEF;
                  EG_DEC_U16(evntMsg->egMsg->msgHdr.extHdr.pdcpNmb.val, mBuf);
               }
               else
               {
                  /********************************************************
                   * If the Ext. Header length is wrong, get the CR type  *
                   ********************************************************/
                  retVal = egUGetCRType(extHdrType, extHdrLen, mBuf);
                  EG_DBG_INFO(0,0, (egp,
                           "egUDecodeGmHdr(Received Ext. Hdr :%d with wrong \
                     length : %d -- Actual length :%d\n", 
                           extHdrType, extHdrLen, EG_LEN_ONE));
               }
               break;

            default:
               /**********************************************************
                * If the Ext. Header length is Unknown, get the CR type  *
                **********************************************************/
               SRemPreMsg(&extHdrLen, mBuf);
               retVal = egUGetCRType(extHdrType, extHdrLen, mBuf);
               unKnownExtHdr = TRUE;
         }

         if((retVal == ROK || retVal == EG_CH_NOT_REQ) && (unKnownExtHdr == FALSE))
         {
            /**************************
             * Get Next Ext. Hdr Type *
             **************************/
            SRemPreMsg(&extHdrType, mBuf);
            retVal = ROK;
         }

         else if(retVal == EG_CH_REQ )
         {
            if(unKnownExtHdr == TRUE)
            {
               EG_DBG_INFO(0,0, (egp,
                        "egUDecodeGmHdr(Received UnSupported Ext. Hdr :%d with \
                  Comprehension Required\n", extHdrType));

               /***************************************************************
                * If sequence number is present, update in the evntMsg,       *
                * so that same can be used for sending Supported Extension    *
                * Header Notification message                                 *
                * *************************************************************/
               if(evntMsg->egMsg->msgHdr.seqNumber.pres)
               {
                  evntMsg->seqNumber = evntMsg->egMsg->msgHdr.seqNumber.val;
               }
               else
               {
                  evntMsg->seqNumber = 0;
               }

               retVal = EGT_ERR_UNSUPP_EXT_HDR;
			   /* eg012.201: klocwork fix: removed return */
			   break; /* while loop */
	    }
            else
            {
               EG_DBG_INFO(0,0, (egp,
                        "egUDecodeGmHdr(Received Supported Ext. Hdr :%d with \
                  Comprehension Required with Wrong length field\n",
                        extHdrType));
               retVal = EGT_ERR_WRONG_EXT_HDR;/* klocwork fix: removed return */
			   break; /* while loop */
            }
         }
         else if ((retVal == EG_CH_NOT_REQ) && (unKnownExtHdr == TRUE))
         {
            /**************************
             * Get Next Ext. Hdr Type *
             **************************/
            SRemPreMsg(&extHdrType, mBuf);
            retVal = ROK;
         }
      }
   }
   /******************************************************************
    * eg006.201 : If ExtPres is true, but Pdu NUm bit is not set, 
    * implies, either of E or S bit was set during Encode  so accordingly 
    * extract  Byte fields will be extracted form Mbuf with 0 for Ext Hdr 
   ******************************************************************/
   else  if(extPres)
   {
      SRemPreMsg(&extHdrType, mBuf);
   }
   /******************************************************************
    * check for msgLen error, if no error continue, otherwise return *
    ******************************************************************/
   if((retVal == ROK) && (evntMsg->egMsg->msgHdr.msgType != EGT_GTPU_MSG_GPDU))
   {
      /***************************************
       * Call to validate the GTP Msg Header *
       **************************************/
      retVal = egUValidateGmHdr(evntMsg);
   }

   EG_RETVALUE(retVal); 
} /* End of egUDecodeGmHdr() */ 



/***********************************************************************
 *   Fun:  egUValidateGmHdr
 *   
 *   Desc:  To Validate GTP-U Msg header as per specs.
 *   
 *   Ret:   ROK  - for Success
 *          EGT_ERR_INV_HDR -   Invalid GTP Header
 *          EGT_ERR_UNSUPP_VER - VERSION not supported
 *          EGT_ERR_MSG_INV_TYPE - Invalid Message Type
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUValidateGmHdr
(
   EgUEvnt  *evntMsg                      /* Event message to validate */
 )
#else
PRIVATE S16 egUValidateGmHdr( evntMsg)
   EgUEvnt  *evntMsg;                     /* Event message to validate */
#endif
{
   S16           retValue = ROK;          /* Holds the return value */
   EgUMsgHdr     *hdr = NULLP;            /* Holds the Message Hdr */
   U16           gmPosIdx = 0;            /* Message Position Idx */

   /*************************************************************
    *  Trace the function only if debug mask is enabled for EDM *
    *************************************************************/
   EG_TRC2(egUValidateGmHdr)

      hdr = &evntMsg->egMsg->msgHdr;

   EG_DBG_INFO(0,0, (egp,
            "egUValidateGmHdr( Message Type (%d) \n",
            hdr->msgType));

   gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[hdr->msgType];
   if(!gmPosIdx)
   {

      /*******************************************
       * Return Error for Unknown Message Type   *
       *******************************************/
      retValue = EGT_ERR_MSG_INV_TYPE;
      EG_DBG_INFO(0,0, (egp,
               "egUValidateGmHdr(Invalid Message Type(%d) resultType(%d) \n",
               hdr->msgType, retValue));
      EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
            evntMsg->egMsg->msgHdr.msgType);
      EG_RETVALUE(retValue);
   }

   /***********************************************************
    * TEID should be Zero for the following message types *
    ***********************************************************/
   if(((hdr->msgType == EGT_GTPU_MSG_ECHO_REQ) || 
            (hdr->msgType == EGT_GTPU_MSG_ECHO_RSP) || 
            (hdr->msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF) || 
            (hdr->msgType == EGT_GTPU_MSG_ERROR_IND)) && (hdr->teId))
   {
      /*************************************
       * Return Error if TEID is non-ZERO  *
       *************************************/
      retValue = EGT_ERR_INV_TEID;
#ifdef ALIGN_64BIT
      EG_DBG_INFO(0,0, (egp,
               "egUValidateGmHdr(Invalid teid(%d) for this message \n", hdr->teId));
#else
      EG_DBG_INFO(0,0, (egp,
               "egUValidateGmHdr(Invalid teid(%ld) for this message\n", hdr->teId));
#endif

      EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
            evntMsg->egMsg->msgHdr.msgType);
      EG_RETVALUE(retValue);

   }

   /*************************************
    * Encoding side Validation          *
    *************************************/
   if(evntMsg->isEnc)
   {
      /******************************************************************
       * TEID should be non-Zero for the following message types.       *
       * On Decoding side the same validation will be taken care by CPM *
       ******************************************************************/
      if((hdr->msgType == EGT_GTPU_MSG_END_MARKER) && (!hdr->teId))
      {
         /*********************************
          * Return Error if TEID is ZERO  *
          *********************************/
         retValue = EGT_ERR_INV_TEID;
#ifdef ALIGN_64BIT
         EG_DBG_INFO(0,0, (egp,
                  "egUValidateGmHdr(Invalid teid(%d) for this message \n", hdr->teId));
#else
         EG_DBG_INFO(0,0, (egp,
                  "egUValidateGmHdr(Invalid teid(%ld) for this message\n", hdr->teId));
#endif

         EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
               evntMsg->egMsg->msgHdr.msgType);
         EG_RETVALUE(retValue);
      }

      /*********************************************************
       * Sequence Number should be present for these messages  *
       *********************************************************/
      /********************************************************************
       * Sequence Number should be present for Echo also, but the control * 
       * never comes here for Echo, so no check is added for Echo         *
       ********************************************************************/
      if((hdr->msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF) || (hdr->msgType == EGT_GTPU_MSG_ERROR_IND))
      {
         if(!hdr->seqNumber.pres)
         {
            retValue = EGT_ERR_NO_SEQ; 
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateGmHdr(Sequence number not present for message %d\n", hdr->msgType));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
                  evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retValue);
         }
      }

      /*********************************************************
       * End Marker should be sent, only from few interfaces   *
       *********************************************************/
      if(hdr->msgType == EGT_GTPU_MSG_END_MARKER )
      {
         /*-- eg003.201: S1_U interface added for END Marker msg --*/
         /* eg001.201 End marker to be Sent on X2/S1-U/S4/S12 interfaces*/
#ifndef EGTP_U_REL_9
        if((evntMsg->intfType != EGT_INT_S1_U_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S1_U_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_DL_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_UL_ENB_GTP_U))
#else
         /********************************************************
          * Release 9 :End Marker: End marker can be received    *
          * from X2/S1-U/S4/S12 interfaces and hence checks are  *
          * added to ensure the same.                            *
          ****************************************************** */
        if((evntMsg->intfType != EGT_INT_S1_U_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S1_U_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S12_RNC_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S12_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S4_SGSN_GTP_U)&&
           (evntMsg->intfType != EGT_INT_S4_SGW_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_DL_ENB_GTP_U)&&
           (evntMsg->intfType != EGT_INT_X2_UL_ENB_GTP_U))
#endif 
         {
            retValue = EGT_ERR_MSG_INV_TYPE;
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateGmHdr(Invalid Message(%d) on this node \n",
                     hdr->msgType));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
                  evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retValue);
         }  
      }
   }
   /*************************************
    * Decoding side Validation          *
    *************************************/
   else
   {
      /*********************************************************
       * Sequence Number should be present for these messages  *
       *********************************************************/
      if((hdr->msgType == EGT_GTPU_MSG_ECHO_REQ) || (hdr->msgType == EGT_GTPU_MSG_ECHO_RSP))
      {
         if(!hdr->seqNumber.pres)
         {
            retValue = EGT_ERR_NO_SEQ; 
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateGmHdr(Sequence number not present for message %d\n", hdr->msgType));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue,  0, 
                  evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retValue);
         }

      }
/* eg001.201 End Marker reception not be dropped for X2 interface */
#ifndef EGTP_U_REL_9
      /**************************************************************
       * End Marker reception and Echo request reception on X2 is   *
       * not Valid in eGTP-U R8.Drop the received message.However   *
       * the same messages are valid in case of R9 hence no dropping*
       **************************************************************/
      if(((hdr->msgType == EGT_GTPU_MSG_END_MARKER) &&
          (evntMsg->intfType != EGT_INT_X2_DL_ENB_GTP_U && 
           evntMsg->intfType != EGT_INT_X2_UL_ENB_GTP_U)) ||
         ((hdr->msgType == EGT_GTPU_MSG_ECHO_REQ) && 
          (evntMsg->intfType == EGT_INT_X2_DL_ENB_GTP_U || 
           evntMsg->intfType == EGT_INT_X2_UL_ENB_GTP_U)))
      {
         retValue = EGT_ERR_MSG_INV_TYPE;
         EG_DBG_INFO(0,0, (egp,
                  "egUValidateGmHdr(Invalid Message(%d) on this node \n",
                  hdr->msgType));
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retValue, 0, 
               evntMsg->egMsg->msgHdr.msgType);
         EG_RETVALUE(retValue);
      }
#endif
   } 

   EG_RETVALUE (retValue);
}  /* End of egUValidateGmHdr() */


/**********************************************************************
 *   Fun:  egUDecIe
 *   
 *   Desc:  Decodes the GTP-U IE 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUDecIe
(
   EgUEvnt         *evntMsg,   /* Event message to stores the decoded ie */
   Buffer          *mBuf,       /* Buffer which has to decode */
   Bool            *iePres,
   U8              *ieType      /* Decoded IE type */
 )
#else
PRIVATE S16 egUDecIe(evntMsg, mBuf, iePres, ieType)
   EgUEvnt        *evntMsg;       /* Event message to stores the decoded ie */
   Buffer         *mBuf;          /* Buffer which has to decode */
   Bool           *iePres;
   U8              *ieType;       /* Decoded IE type */
#endif
{
   S16              retVal   = ROK;        /* Holds return value */
   U16              tmpIeLen = 0;          /* Holds the IE Length */
   EgIeEntry        *ieEntry = NULLP;      /* Holds IE Entry */
   MsgLen           msgLen   = 0;          /* Holds Rcd Buffer length */
   EgUIe            tmpIe;                 /* tmp Holds Decoded IE */
   EgUstaDgn        dgn;                   /* Used for sending the alarms  */
   Bool             isTvType = FALSE;       /* is this IE of TV type ? */
   U8               skipIe[EG_MAX_BYTES];  /* To Skip IE from the buf */
   U16              iePosIdx = 0;          /* IE position Idx */

   EG_TRC2(egUDecIe);

   /******************************************
    * Initialization for all the structures  *
    ******************************************/
   cmMemset((U8*)&tmpIe, EG_ZERO, sizeof(EgUIe));
   cmMemset((U8*)skipIe, EG_ZERO, EG_MAX_BYTES);

   /*****************************************
    * find the length of the encoded buffer *
    *****************************************/
   SFndLenMsg(mBuf, &msgLen);

   if(msgLen == 0)
   {
      EG_RETVALUE(EGT_ERR_MSG_INV_LEN);
   }

   /*************************
    * Decode the IE Type    *
    ************************/
   SRemPreMsg(&tmpIe.ieType, mBuf);
   *ieType = tmpIe.ieType;
   msgLen--;

   isTvType = EGU_IE_IS_TV(tmpIe.ieType);
   if(!isTvType)
   {
      /************************
       * Decode the IE length *
       ***********************/
      if(tmpIe.ieType != EGU_IE_EXT_HDR_LST)
      {
         EG_DEC_U16(tmpIeLen, mBuf);
         msgLen -= EG_LEN_TWO;
      }
      else
      {
         SRemPreMsg((U8 *)&tmpIeLen, mBuf);
         msgLen--;
      }
   }

   /*******************************************
    * Get the IE entry from the IE Dictionary *
    ******************************************/
   if((iePosIdx = egCb.egUCb.edmUCb.egIeCb.posIdx[tmpIe.ieType]))
   {
      ieEntry = &egCb.egUCb.edmUCb.egIeCb.ieEntry[iePosIdx];
   }
   else
   {
      /**************************************************
       * Unknown IE, handle based on the Type (TV/TLV)  *
       **************************************************/
      if(isTvType)
      {
         /********************************************
          * Unknown IE of TV type - Return Error     *
          ********************************************/
         retVal = EGT_ERR_UNKNOWN_TV;
         EG_DBG_INFO(0,0, (egp,
                  "egUDecIe(IE(%d), resultType(%d)) \n",
                  tmpIe.ieType,  retVal));
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,
               evntMsg->egMsg->msgHdr.msgType);
         /* eg012.201: : removed ret, klocwork fix */
      }
      else
      {
         /***************************************************
          * Unknown IE of TLV type -                        *
          * Skip this IE and continue further processing    *
          **************************************************/
         SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
	 /* eg012.201: removed, return klocwork fix */
         retVal = ROK;
      }
	  EG_RETVALUE(retVal);
   }

   /**************************************************
    * Get the IE length from IE dict. for TV type IE *
    **************************************************/
   if(isTvType)
   {
      tmpIeLen = ieEntry->minLen;
   }

   if((iePres[tmpIe.ieType] == TRUE) && (tmpIe.ieType != EGU_IE_PVT_EXTN))
   {
      /*************************************************************
       * Skip this Duplicate IE and continue further processing    *
       * Duplicates are allowed for Private Extension IEs          *
       *************************************************************/
      SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
      EG_RETVALUE(ROK);
   }

   /*******************************************
    * Do the IE length Validation for TLV IEs *
    *******************************************/
   if(!isTvType)
   {
      /********************************
       * Do the IE length Validation  *
       ********************************/
      retVal = egUValidateIeLen(ieEntry,evntMsg->egMsg->msgHdr.msgType,
            tmpIeLen);

      if (retVal == EGT_ERR_MAN_IE_INV_LEN)
      {
         /**************************************************************
          * Mandatory IE is received with Invalid length, report error *
          **************************************************************/
         EG_DBG_INFO(0,0, (egp,
                  "egUDecIe(IE(%d), resultType(%d)) \n",
                  tmpIe.ieType,  retVal));
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType, 
               evntMsg->egMsg->msgHdr.msgType);
         EG_RETVALUE(retVal);
      }
      /**********************************************************
       * Non-Mandatory IE received with Invalid length *
       **********************************************************/
      else if( retVal == EGU_SKIP_IE)
      {
         if (tmpIeLen > msgLen)
         {
            /************************************
             * Invalid length, report error     *
             ************************************/
            retVal = EGT_ERR_IE_INV_LEN;
            EG_DBG_INFO(0,0, (egp,
                     "egUDecIe(IE(%d), resultType(%d)) \n",
                     tmpIe.ieType,  retVal));
            EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,
                  evntMsg->egMsg->msgHdr.msgType);
            /* eg012.201: removed, return klocwork fix */
         }
         /* eg012.201: Klocwork fix */
	 else
         {
            /***************************************************
	    * skip this IE and continue further processing    *
	    **************************************************/
	    SRemPreMsgMult(skipIe, tmpIeLen, mBuf);
            retVal = ROK;
         }
         EG_RETVALUE(retVal);
      }
   }

   /********************************************
    * Retreive the data type from the ie entry *
    *******************************************/
   tmpIe.dataType = ieEntry->dataType;

   /**********************
    * Decode the IE Data *
    *********************/
   retVal = egUDecIeData(evntMsg, mBuf, &tmpIe, tmpIeLen );
   if( retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUDecIe(Unable to Decode IE: %d)\n",tmpIe.ieType));
      EG_RETVALUE(retVal);
   }

   /******************************
    * Append the IE to the egMsg *
    ******************************/
   retVal = EgUUtilGmAppendIe(evntMsg->egMsg, &tmpIe);
   if(retVal != ROK)
   {
      EG_DBG_INFO(0,0, (egp,
               "egUDecIe(Unable to Append IE :%d) \n",tmpIe.ieType));

      /**************************************************
       * If memory allocation fails send the report to   *
       * LM and then return                              *
       ****************************************************/
      if(retVal == EG_UTIL_ERR_MEM_ALLOC_FAILED)
      {
         cmMemset((U8*)&dgn, EG_ZERO, sizeof(EgUstaDgn));
         dgn.dgnVal[0].type = LEG_PAR_MEM;
         dgn.dgnVal[0].t.mem.region = evntMsg->region;
         dgn.dgnVal[0].t.mem.pool   = evntMsg->pool;
         egSendLmAlarm (LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL, 
               LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
         EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal, tmpIe.ieType,evntMsg->egMsg->msgHdr.msgType);
         /* eg012.201: klocwork fix,removed return */
      }
      else
      {
	 /* eg012.201: removed, return klocwork fix */
         retVal = ROK;
      }
      /* eg012.201 : Updated retVal */
      EG_RETVALUE(retVal);
   }

   /****************************
    * Update the iePres array  *
    ****************************/
   iePres[tmpIe.ieType] = TRUE;

   EG_RETVALUE(retVal); 
} /* End of egUDecIe() */



/**********************************************************************
 *   Fun:  egUDecIeData
 *   
 *   Desc:  Decodes the GTP-U Msg IE Data part 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUDecIeData
(
 EgUEvnt     *evntMsg,       /* Event message to stores the decoded ie */
 Buffer      *mBuf,          /* Buffer which has to decode */
 EgUIe       *egIe,          /* Ie to store the decoded IE data */
 U16         ieLen           /* IE Length psnt in the Hdr */
 )
#else
PRIVATE S16 egUDecIeData(evntMsg, mBuf, egIe, ieLen)
   EgUEvnt    *evntMsg;       /* Event message to stores the decoded ie */
   Buffer     *mBuf;          /* Buffer which has to decode */
   EgUIe      *egIe;          /* Ie to store the decoded IE data */
   U16        ieLen;          /* Total Ie Length excluding hdr   */
#endif
{
   S16      retVal = ROK;         /* Holds the return value */
   U8       skipIe[EG_MAX_BYTES];  /* To Skip IE from the buf */

   EG_TRC2(egUDecIeData);

   EG_DBG_INFO(0,0, (egp,
            "egUDecIeData(IE Type(%d), IE Data Type(%d)) \n",
            egIe->ieType, egIe->dataType));

   /**************************************************
    * Decodes the IE based on the IE data type       *
    **************************************************/
   switch (egIe->dataType)
   {
      case EG_UNSIGNED_8:
         SRemPreMsg(&egIe->t.valUnSgnd8, mBuf);
         break;

      case EG_UNSIGNED_32:
         EG_DEC_U32(egIe->t.valUnSgnd32, mBuf);
         break;

      case EG_PVT_EXTN:
         {
            /*eg009.201:Klockwork fix,modified U8 correctly to U16*/
            U16 propValLen = 0;
            EG_DEC_U16(egIe->t.pvtExt.entId, mBuf);

            propValLen = ieLen - EG_LEN_TWO;
            egIe->t.pvtExt.propVal.length = propValLen;
            SRemPreMsgMult(egIe->t.pvtExt.propVal.val, propValLen, mBuf);
            break;
         }

      case EG_IP_ADDR:
         /***************************************
          * Ip address is either IPv4 or IPv6   *
          ***************************************/
         if(ieLen == EG_LEN_FOUR)
         {
            egIe->t.ipAddr.addrType = EG_LEN_FOUR;
            EG_DEC_U32(egIe->t.ipAddr.u.ipv4, mBuf);
         }
#ifdef EG_IPV6_SUPPORTED
         else
         {
            egIe->t.ipAddr.addrType = EG_LEN_SIXTEEN;
            SRemPreMsgMult(&egIe->t.ipAddr.u.ipv6, EG_LEN_SIXTEEN, mBuf);
         }
#endif /* EG_IPV6_SUPPORTED */
         break;

      case EG_EXT_HDR_TYPE_LST:
         {
            U16 cnt = 0;

            egIe->t.extHdrTypeLst.noOfExtHdrs = ieLen;
            for(cnt = 0; cnt < ieLen; cnt ++)
            {
               SRemPreMsg(&egIe->t.extHdrTypeLst.extType[cnt], mBuf);
            }
            break;
         }

      default:
         SRemPreMsgMult((Data*)skipIe, ieLen, mBuf);
         EG_RETVALUE(EG_IE_INV_DATA_TYPE);
   }

   EG_RETVALUE(retVal);
} /* End of egUDecIeData() */



/**********************************************************************
 *   Fun:  egUValidateGmMsgIEs
 *   
 *   Desc:  To Validate the GTP-U Msg IEs. 
 *   
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUValidateGmMsgIEs
(
   EgUEvnt     *evntMsg,              /* Contains EGMSG for validation */
   Bool        *iePres
 )
#else
PRIVATE S16 egUValidateGmMsgIEs(evntMsg, iePres)
   EgUEvnt     *evntMsg;             /* Contains the EGMSG for validation */
   Bool        *iePres;
#endif
{
   S16              retVal = ROK;           /* Holds the return value */
   EgUMsgEntry      *gmEntry = NULLP;       /* Holds the message Entry */
   S16              loopCnt = 0;                /* Used for the loops */
   U8               msgType = 0;           /* Message type */
   CmLListCp        *ieList = NULLP;      /* Ie List tobe sorted*/
   CmLList          *tmpNode = NULLP;      /* Node for outer loop of sorting */
   CmLList          *node    = NULLP;      /* Node for inner loop of sorting */
   EgUIe            *tmpEgIe = NULLP;      /* IE for outer loop of sorting */
   EgUIe            *egIe    = NULLP;      /* IE for inner loop of sorting*/
   CmLList          *lclCrnt = NULLP;      /* List used for inner loop */
   CmLList          *tmpList = NULLP;     /* Stores the crnt List */
   U16              gmPosIdx = 0;         /* Message Position Idx */

   EG_TRC2(egUValidateGmMsgIEs)

      msgType = evntMsg->egMsg->msgHdr.msgType;
   gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
   gmEntry =  &egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx];

   /*****************************************************
    * Iterate through the message dict. to find out the *
    * Missing Mandatory IEs                             *
    *****************************************************/
   if(gmEntry->nmbMandIe)
   {
      for ( loopCnt=0; loopCnt < gmEntry->nmbIe; loopCnt++)
      {
         if((gmEntry->ieProperties[loopCnt].ieClass == EG_IE_MANDATORY) &&
               (! iePres[gmEntry->ieProperties[loopCnt].ieType]))
         {
            /************************************************
             * Send missing mandatory IE to the application *
             ************************************************/
            retVal = EGT_ERR_MSG_MISS_MAND_IE;
            EG_DBG_INFO(0,0, (egp,
                     "egValidateGmMsgIEs(IE Type(%d), Msg Type (%d) - \
               mandatory element is missing, resulType(%d)) \n",
                     gmEntry->ieProperties[loopCnt].ieType, msgType, retVal));

            EGU_FILL_ERR_EVNT_MSG(evntMsg, retVal,
                  gmEntry->ieProperties[loopCnt].ieType, evntMsg->egMsg->msgHdr.msgType);
            EG_RETVALUE(retVal);
         }
         
      }
   }

   if((evntMsg->isEnc) && (evntMsg->egMsg->u.egIeList.count > 1))
   {
      ieList = &evntMsg->egMsg->u.egIeList;
      /***************************************
       * Sort the IEs in the Ascending order *
       ***************************************/
      tmpEgIe = (EgUIe* ) CM_LLIST_FIRST_NODE(ieList, tmpNode);

      while(tmpEgIe != NULLP)
      {
         lclCrnt = ieList->crnt->next;
         if(lclCrnt == NULLP)
            break;
         else
            egIe = (EgUIe* )(lclCrnt->node);

         while(egIe != NULLP)
         {
            if(tmpEgIe->ieType > egIe->ieType)
            {
               tmpList = ieList->crnt;
               node = lclCrnt;

               /*********************************
                * Remove the Node from the List *
                *********************************/
               cmLListDelFrm(ieList, node);

               /***************************************************
                * Travel till the position where this node needs  *
                * tobe inserted                                   *
                ***************************************************/
               ieList->crnt = tmpList;

               /***********************************
                * Insert the removed node         *
                ***********************************/
               cmLListInsCrnt(ieList, node);

               tmpNode = node;
               tmpEgIe = (EgUIe* ) tmpNode;
            }
            lclCrnt = lclCrnt->next;
            if(lclCrnt == NULLP)
               break;
            else   
               egIe = (EgUIe* )(lclCrnt->node);
         }

         tmpEgIe = (EgUIe* ) CM_LLIST_NEXT_NODE(ieList, tmpNode);
      }
   }

   EG_RETVALUE(retVal);
} /* End of egUValidateGmMsgIEs() */


/*************************************************************************
 *   Fun:  egUValidateIeLen
 *
 *   Desc:  Validate the Ie Len for a GTP-U IE
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_edm.c
 ************************************************************************/
#ifdef ANSI
PRIVATE S16 egUValidateIeLen
(
 EgIeEntry    *ieEntry,   /* Holds the min and max length   */
 U8            msgType,   /* To get the msg entry           */
 U16           ieLen      /* actual IE length to validate   */
 )
#else
PRIVATE S16 egUValidateIeLen(ieEntry, msgType, ieLen)
   EgIeEntry    *ieEntry;   /* Holds the min and max length   */
   U8            msgType;   /* To get the msg entry           */
   U16           ieLen;     /* actual IE length to validate   */
#endif
{
   EgUMsgEntry       *gmEntry = NULLP;   /* Holds the msg Entry */
   U8                tmpCnt = 0;   /* Index to get the pos of IE in the list */
   U16               gmPosIdx = 0; /* Position Idx */ 

   EG_TRC2(egUValidateIeLen);

   /*********************
    * Invalid IE length *
    *********************/
   /*************************************************************************
    * For IEs of type EG_IP_ADDR the length should match with exact length  *
    *************************************************************************/
   if ((ieLen < ieEntry->minLen || ieLen > ieEntry->maxLen) ||
         ((ieEntry->dataType == EG_IP_ADDR) && 
          ((ieLen != ieEntry->minLen) && (ieLen != ieEntry->maxLen))))
   {
      /*********************************************************
       * Get the ieProp and then check whether this ie is      *
       * mandatory or not                                      *
       ********************************************************/
      gmPosIdx = egCb.egUCb.edmUCb.egMsgCb.posIdx[msgType];
      gmEntry = &egCb.egUCb.edmUCb.egMsgCb.gmEntry[gmPosIdx];

      for(tmpCnt=0; tmpCnt < gmEntry->nmbIe; tmpCnt++)
      {
         if ((gmEntry->ieProperties[tmpCnt].ieType == ieEntry->ieCode) &&
               (gmEntry->ieProperties[tmpCnt].ieClass == EG_IE_MANDATORY))
         {
            EG_DBG_INFO(0,0, (egp,
                     "egUValidateIeLen: :Length validation for Mandatory IE(%d) is failed) \n",
                     ieEntry->ieCode));
            /*****************************************
             * Invalied Mandatory IE - Return Error  *
             *****************************************/
            EG_RETVALUE (EGT_ERR_MAN_IE_INV_LEN);
         }
      }

      /******************************************
       * For non-Mandatory IEs - Skip this IE   *
       * and continue further processing        *
       ******************************************/
      EG_RETVALUE(EGU_SKIP_IE);
   } /* End of getting the idx */

   EG_RETVALUE(ROK);
} /* End of egUValidateIeLen() */


/***********************************************************************
 *   Fun:  egUGetCRType
 *   
 *   Desc: Calculates the CR value of an IE
 *   
 *   Ret:   EG_CH_REQ     - Comprehension Required
 *          EG_CH_NOT_REQ - Comprehension Not Required
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 **********************************************************************/
#ifdef ANSI
PRIVATE S16 egUGetCRType
(
U8         extHdrType,             /* Ext. Hdr type */
U8         extHdrLen,               /* Ext. Hdr length */
Buffer      *mBuf                 /* Buffer */
)
#else
PRIVATE S16 egUGetCRType(extHdrType, extHdrLen, mBuf)
U8         extHdrType;            /* Ext. Hdr type */
U8         extHdrLen;             /* Ext. Hdr length */
Buffer      *mBuf;                 /* Buffer */
#endif
{
   U8 chValue = 0; /* Comprehension value */
   U8 extHdrValLen = 0; /* Length of Ext. Hdr Value field */
   U8 temp[100];   /* temp. variable */

   EG_TRC2(egUGetCRType);

   /************************************************************
    * Get Comprehension value, by shifting extHdrType 6 times, *
    * to get the the last 2bits                                *
    ************************************************************/
   chValue = extHdrType >> EG_SHIFT_6;

   /******************************
    * Comprehension Required     *
    ******************************/
   if(chValue == EG_THREE)
   {
      EG_RETVALUE(EG_CH_REQ);
   }
   else
   {
      /******************************
      * Comprehension Not Required *
      ******************************/
      /************************************************************************
       * Length field of Ext. Hdr is in terms of 4 bytes.                     *
       * Ext. Hdr Value field is of size 2 bytes (length, Next Ext. Hdr type) *
       * less than the total length of the Ext. Hdr                           *
       ************************************************************************/
      extHdrValLen = (EG_LEN_FOUR * extHdrLen) - EG_LEN_TWO;

      /***********************************************
       * Delete the Extension header Value Feild     *
       ***********************************************/
      SRemPreMsgMult(temp, extHdrValLen, mBuf);
      EG_DBG_INFO(0,0, (egp,
               "egUGetCRType Deleted Extension Header :%d", extHdrType));
      EG_RETVALUE(EG_CH_NOT_REQ);
   }

} /* end of egUGetCRType() */

#endif /* End of EGTP_U */

#ifdef EGTP_C
/* fix for  ccpu00136924 */
 /***********************************************************************
 *   Fun:  egValidateInftType
 *   
 *   Desc: validate the interfacetype for endpoint 
 *   
 *   Ret:   ROK
 *          EGT_ERR_MSG_INCORR_MAND_IE
 *   
 *   Notes: None
 *   
 *   File:  eg_edm.c
 ***********************************************************************/
PRIVATE S16 egValidateInftType(U8 intfType,U8 IntfEnt)
{
   
   S16 retVal=ROK;

   switch(intfType)
   {
      case EGT_GTP_INT_S11:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_MME)&&(IntfEnt == EGT_INT_S11_S4_SGW_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_SGW )&&(IntfEnt == EGT_INT_S11_MME_GTP_C))))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break;        
      case EGT_GTP_INT_S5_S8:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_PGW)&&(IntfEnt == EGT_INT_S5_S8_SGW_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_SGW )&&(IntfEnt == EGT_INT_S5_S8_PGW_GTP_C))))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break;
             
      case EGT_GTP_INT_S4:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_SGW)&&(IntfEnt == EGT_INT_S4_SGSN_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_SGSN )&&(IntfEnt == EGT_INT_S11_S4_SGW_GTP_C))))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break;
#if defined(EG_REL_930) || defined(EG_REL_AC0)

      case EGT_GTP_INT_SM:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_MME)&&(IntfEnt == EGT_INT_SM_MBMSGW_GTP_C ))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_MBMSGW )&&(IntfEnt == EGT_INT_SM_MME_GTP_C ))))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break;        
      case EGT_GTP_INT_SN:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_SGSN)&&(IntfEnt == EGT_INT_SN_MBMSGW_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_MBMSGW)&&(IntfEnt == EGT_INT_SN_SGSN_GTP_C ))))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break; 
#endif
      case EGT_GTP_INT_S10:
              if(!((egCb.genCfg.nodeType == LEG_GTP_ENT_MME)&&(IntfEnt == EGT_INT_S10_MME_GTP_C)))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break;        
      case EGT_GTP_INT_S3:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_SGSN)&&(IntfEnt == EGT_INT_S3_MME_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_MME )&&(IntfEnt == EGT_INT_S3_SGSN_GTP_C ))))
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break; 
      /* fix for ccpu00137514 */       
      case EGT_GTP_INT_S16:
              if(!((egCb.genCfg.nodeType == LEG_GTP_ENT_SGSN)&&(IntfEnt == EGT_INT_S16_SGSN_GTP_C)))
                   
              {   
                 retVal=EGT_ERR_MSG_INCORR_IE; 
              }          
              break;
      /* fix for ccpu00137514 */
#ifdef EG_REL_AC0
      case EGT_GTP_INT_S2B:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_EPDG )&&(IntfEnt == EGT_INT_S2B_PGW_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_PGW )&&(IntfEnt == EGT_INT_S2B_EPDG_GTP_C ))))
              {
                 retVal=EGT_ERR_MSG_INCORR_IE;
              }
              break;
#endif
#ifdef EG_S2A_SUPP
      case EGT_GTP_INT_S2A:
              if(!(((egCb.genCfg.nodeType == LEG_GTP_ENT_TWAN )&&(IntfEnt == EGT_INT_S2A_PGW_GTP_C))||
                    ((egCb.genCfg.nodeType == LEG_GTP_ENT_PGW )&&(IntfEnt == EGT_INT_S2A_TWAN_GTP_C ))))
              {
                 retVal=EGT_ERR_MSG_INCORR_IE;
              }
              break;
#endif              


       default:
              retVal=EGT_ERR_MSG_INCORR_IE;  

   }
   EG_RETVALUE(retVal);
}/* end of egValidateInftType() */
#endif 

/********************************************************************30**

         End of file:     pg_edm.c@@/main/3 - Fri Feb  4 18:18:15 2011

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
/main/1      ---          ad             1. Created for Initial Release 1.1
/main/2      ---          kchaitanya     1. Initial for eGTP 1.2 Release
/main/2      eg001.102    rss            1. Memory leak fix.
/main/3      ---          pmacharla         Initial for eGTP release 2.1
/main/3      eg001.201    asaurabh       1. End marker can be received from 
                                            X2/S1-U/S4/S12 interfaces
					 2. Endmarker reception not be dropped 
                                            for X2 interface 
/main/3      eg003.201    psingh         1. Merged code from eg006.102 to Fix 
                                            TRACE5 macro related issue
                                         2. Merged code from eg006.102 to Fix 
                                            the encoding and decoding of optional
                                            fields(Sequence Number, N-PDU
                                            Number and Next Extension Header Type)
                                         3. Fix for IOT issue
                                         4. Updated valid interfaces S1_U &X2 for 
                                            END_MARKER msg
                                         5. Init the ptr ieProp with NULL to avoid
                                            warnings
/main/3      eg004.201   magnihotri      1.Header files added for eGTP-C PSF
/main/3      eg006.201    psingh         1.Updated EvntMsg to reflect the error
                                            return type
                                         2.Removed EGTPU_HDR_ALWAYS_PRSNT flag 
                                         3.Aligned GTP-U Encode/Decode proc with Spec
/main/3      eg007.201    psingh         1. Free Hashlist,gmDictHlCp, associated with intfCbLst
/main/3      eg008.201    asaurabh       1. Data Type of nextGmCb modified from PTR to EgGmCb
/main/3      eg009.201    asaurabh       1. Klocwork fix-Unused variable removed
                                         2. Added for S2B
                                         3. Added for S2A
                                         4. Warning fixed,unused variable grpOccCnt
                                         5. Klockwork fix-variable not usedafter assignment
                                         6. Klockwork fix -Variable not used
                                         7. Klockwork fix,check groupedIe ptr for NULL
                                         8. Magic number replaced with Macro
                                         9. Added for S2B and S2A
                                         10.Klockwork fix datatype modified
                                         11.Fix TEID zero 
                                         12.egMsg & memory allocated to IE inside egMsg 
                                            is freed with EgUUtilDeAllocGmMsg
                                         13.Klockwork fix Modified datatype
                                         14.Klockwork fix,modified U8 correctly to U16
/main/3      eg010.201    asaurabh       1. Fixed encoding piggyBack bit
                                         2. Fixed decoding piggyback bit
/main/3      eg012.201    shpandey       1. klocwork fixes added
                                         2. fix for two bearer contexts with same instance 
                                            for CreateIndirectForward request,fix for ccpu00130004
                                         3. In function egUEncMsgReq:Added check for ieCnt	
/main/3      eg013.201    shpandey       1. The length of the egDatStr8 filled explicitly in 
                                            egDecEG_BQOS_IEDATA
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

