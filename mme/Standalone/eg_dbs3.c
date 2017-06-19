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


/***********************************************************************

    Name: Source file for filling LM configuration

    Type: C Source

    Desc: 

    File: eg_dbs3.c

    Sid:      eg_dbs3.c@@/main/2 - Fri Feb  4 18:18:04 2011

    Prg: an

************************************************************************/


/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/* Header Include Files (.h) */
#include "envopt.h"            /* Environment Options */
#include "envdep.h"            /* Environment Dependent */
#include "envind.h"            /* Environment Independent */
#include "gen.h"            /* General */
#include "ssi.h"            /* System Services */
#include "cm_tkns.h"         /* Common Tokens */
#include "cm_hash.h"         /* Common Hash List */
#include "cm_mblk.h"         /* Common memory */
#include "cm_llist.h"           /* Common Link List */
#include "cm5.h"               /* Common Structures */
#include "cm_inet.h"           /* Common IP network */
#include "cm_tpt.h"           /* Common Transport */
#include "hit.h"            /* TUCL layer */
#include "egt.h"            /* EU layer */
#include "leg.h"      /* layer manager */
#include "eg_util.h"      /* UTIL */
#include "eg_edm.h"      /* EDM MODULE */
#include "eg_dbutl.h"   /* IE Types are here */
#include "eg_dbs3.h"   /* S3 interface message codes are here */

/* Header Include Files (.x) */

#include "gen.x"            /* General */
#include "ssi.x"            /* System Services */
#include "cm_tkns.x"         /* Common Tokens */
#include "cm_hash.x"         /* Common Hash List */
#include "cm_mblk.x"         /* Common memory */
#include "cm_llist.x"           /* Common Link List */
#include "cm5.x"               /* Common Structures */
#include "cm_inet.x"           /* Common IP network */
#include "cm_tpt.x"           /* Common Transport */
#include "hit.x"            /* TUCL layer */
#include "egt.x"            /* EU layer */
#include "leg.x"      /* layer manager */
#include "eg_util.x"      /* UTIL */
#include "eg_edm.x"      /* EDM MODULE */
#include "eg_dbutl.x"      /* Application MODULE */
#include "eg_dbs3.x"


#ifdef EGTP_C


#if defined(EG_REL_AC0) || defined(EG_REL_930)
/***********************************************************************

   Func  : egMakes3ALRTMMEACKGmEntry

   Desc  : This function makes the GM Entry for ALRTMMEACK

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3ALRTMMEACKGmEntry
(
LegMsgEntry               *gmALRTMMEACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3ALRTMMEACKGmEntry(gmALRTMMEACKCb)
LegMsgEntry               *gmALRTMMEACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3ALRTMMEACKGmEntry);

   /* Populate the ALRTMMEACK GM Entry */
   gmALRTMMEACKCb->msgType = EG_S3_ALRTMMEACK_CMD_CODE;
   gmALRTMMEACKCb->nmbIeProp = EG_S3_ALRTMMEACK_NMB_IES;
   gmALRTMMEACKCb->validateIe = (EscFun)NULLP;

   /* Populate ALRTMMEACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmALRTMMEACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmALRTMMEACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmALRTMMEACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmALRTMMEACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3ALRTMMEACKGmEntry */


/***********************************************************************

   Func  : egMakes3ALRTMMENOTFNGmEntry

   Desc  : This function makes the GM Entry for ALRTMMENOTFN

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3ALRTMMENOTFNGmEntry
(
LegMsgEntry               *gmALRTMMENOTFNCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3ALRTMMENOTFNGmEntry(gmALRTMMENOTFNCb)
LegMsgEntry               *gmALRTMMENOTFNCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3ALRTMMENOTFNGmEntry);

   /* Populate the ALRTMMENOTFN GM Entry */
   gmALRTMMENOTFNCb->msgType = EG_S3_ALRTMMENOTFN_CMD_CODE;
   gmALRTMMENOTFNCb->nmbIeProp = EG_S3_ALRTMMENOTFN_NMB_IES;
   gmALRTMMENOTFNCb->validateIe = (EscFun)NULLP;

   /* Populate ALRTMMENOTFN IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmALRTMMENOTFNCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmALRTMMENOTFNCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3ALRTMMENOTFNGmEntry */
#endif /* defined(EG_REL_AC0) || defined(EG_REL_930) */


/***********************************************************************

   Func  : egMakes3CSPAGEINDGmEntry

   Desc  : This function makes the GM Entry for CSPAGEIND

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3CSPAGEINDGmEntry
(
LegMsgEntry               *gmCSPAGEINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3CSPAGEINDGmEntry(gmCSPAGEINDCb)
LegMsgEntry               *gmCSPAGEINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3CSPAGEINDGmEntry);

   /* Populate the CSPAGEIND GM Entry */
   gmCSPAGEINDCb->msgType = EG_S3_CSPAGEIND_CMD_CODE;
   gmCSPAGEINDCb->nmbIeProp = EG_S3_CSPAGEIND_NMB_IES;
   gmCSPAGEINDCb->validateIe = (EscFun)NULLP;

   /* Populate CSPAGEIND IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TMSI IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_TMSI_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_TMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GBL_CN_ID IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_GBL_CN_ID_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_GBL_CN_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CHNL_NEEDED IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_CHNL_NEEDED_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_CHNL_NEEDED_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMLPP_PRIOR IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_EMLPP_PRIOR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_EMLPP_PRIOR_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_AC0) || defined(EG_REL_930)
   /* Populate SERV_IND IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_SERV_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_SERV_IND_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCSPAGEINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCSPAGEINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3CSPAGEINDGmEntry */


/***********************************************************************

   Func  : egMakes3CTXACKGmEntry

   Desc  : This function makes the GM Entry for CTXACK

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3CTXACKGmEntry
(
LegMsgEntry               *gmCTXACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3CTXACKGmEntry(gmCTXACKCb)
LegMsgEntry               *gmCTXACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3CTXACKGmEntry);

   /* Populate the CTXACK GM Entry */
   gmCTXACKCb->msgType = EG_S3_CTXACK_CMD_CODE;
   gmCTXACKCb->nmbIeProp = EG_S3_CTXACK_NMB_IES;
   gmCTXACKCb->validateIe = (EscFun)NULLP;

   /* Populate CTXACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCTXACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCTXACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCTXACKCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXACKCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3CTXACKGmEntry */


/***********************************************************************

   Func  : egMakes3CTXREQGmEntry

   Desc  : This function makes the GM Entry for CTXREQ

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3CTXREQGmEntry
(
LegMsgEntry               *gmCTXREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3CTXREQGmEntry(gmCTXREQCb)
LegMsgEntry               *gmCTXREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3CTXREQGmEntry);

   /* Populate the CTXREQ GM Entry */
   gmCTXREQCb->msgType = EG_S3_CTXREQ_CMD_CODE;
   gmCTXREQCb->nmbIeProp = EG_S3_CTXREQ_NMB_IES;
   gmCTXREQCb->validateIe = (EscFun)NULLP;

   /* Populate CTXREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PTMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PTMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI_SIG IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PTMSI_SIG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PTMSI_SIG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate COMP_REQ_MSG IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_COMP_REQ_MSG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_COMP_REQ_MSG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PORT IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PORT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PORT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate HOP_CNTR IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_HOP_CNTR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_HOP_CNTR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCTXREQCb->posIndex[1][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif /* EG_REL_AC0 */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3CTXREQGmEntry */


/***********************************************************************

   Func  : egMakes3CTXRSPGmEntry

   Desc  : This function makes the GM Entry for CTXRSP

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3CTXRSPGmEntry
(
LegMsgEntry               *gmCTXRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3CTXRSPGmEntry(gmCTXRSPCb)
LegMsgEntry               *gmCTXRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes3CTXRSPGmEntry);

   /* Populate the CTXRSP GM Entry */
   gmCTXRSPCb->msgType = EG_S3_CTXRSP_CMD_CODE;
   gmCTXRSPCb->nmbIeProp = EG_S3_CTXRSP_NMB_IES;
   gmCTXRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CTXRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_TRIP IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_UMTS_KEY_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDN_CONN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_PDN_CONN_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmCTXRSPCb->posIndex[0][EG_PDN_CONN_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCTXRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCTXRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CTXRSP_PDN_CONN0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_APN_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CTXRSP_PDN_CONN0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_SEL_MODE_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE,
0 );


      grpCnt1 = 0;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe) ,sizeof(LegGroupedIe));
      if( ret == ROK && (gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe)!= NULLP)
      {

         ieProperties = &gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe->ieProperties;
         ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CTXRSP_BEARER_CNTX00_GRP_NMB_IES)));
         if( ret == ROK && *ieProperties != NULLP)
         {

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CTXRSP_BEARER_CNTX00_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_TRANS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

      }
   }
   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_AMBR_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );
   
   grpCnt++;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif /* (EG_REL_930 || EG_REL_AC0) */
#ifdef EG_REL_AC0
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_SIG_PRIOR_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CNG_TO_RPT_FLG_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif /* EG_REL_AC0 */
}

}
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCTXRSPCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCTXRSPCb->posIndex[1][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RFSP_ID IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_RFSP_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_RFSP_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RFSP_ID IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_RFSP_ID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCTXRSPCb->posIndex[1][EG_RFSP_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCTXRSPCb->posIndex[1][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmCTXRSPCb->posIndex[2][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif /* EG_REL_AC0 */


   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3CTXRSPGmEntry */


/***********************************************************************

   Func  : egMakes3DETACKGmEntry

   Desc  : This function makes the GM Entry for DETACK

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3DETACKGmEntry
(
LegMsgEntry               *gmDETACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3DETACKGmEntry(gmDETACKCb)
LegMsgEntry               *gmDETACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3DETACKGmEntry);

   /* Populate the DETACK GM Entry */
   gmDETACKCb->msgType = EG_S3_DETACK_CMD_CODE;
   gmDETACKCb->nmbIeProp = EG_S3_DETACK_NMB_IES;
   gmDETACKCb->validateIe = (EscFun)NULLP;

   /* Populate DETACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDETACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDETACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDETACKCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDETACKCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDETACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDETACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3DETACKGmEntry */


/***********************************************************************

   Func  : egMakes3DETNTFGmEntry

   Desc  : This function makes the GM Entry for DETNTF

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3DETNTFGmEntry
(
LegMsgEntry               *gmDETNTFCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3DETNTFGmEntry(gmDETNTFCb)
LegMsgEntry               *gmDETNTFCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3DETNTFGmEntry);

   /* Populate the DETNTF GM Entry */
   gmDETNTFCb->msgType = EG_S3_DETNTF_CMD_CODE;
   gmDETNTFCb->nmbIeProp = EG_S3_DETNTF_NMB_IES;
   gmDETNTFCb->validateIe = (EscFun)NULLP;

   /* Populate DETNTF IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDETNTFCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDETNTFCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate DETACH_TYPE IE Properties */
   egMakeAppIeProp(&gmDETNTFCb->ieProperties[ieCnt],
                    EG_DETACH_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDETNTFCb->posIndex[0][EG_DETACH_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif /* EG_REL_AC0 */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDETNTFCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDETNTFCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3DETNTFGmEntry */


/***********************************************************************

   Func  : egMakes3FWDRELCMPACKGmEntry

   Desc  : This function makes the GM Entry for FWDRELCMPACK

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3FWDRELCMPACKGmEntry
(
LegMsgEntry               *gmFWDRELCMPACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3FWDRELCMPACKGmEntry(gmFWDRELCMPACKCb)
LegMsgEntry               *gmFWDRELCMPACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3FWDRELCMPACKGmEntry);

   /* Populate the FWDRELCMPACK GM Entry */
   gmFWDRELCMPACKCb->msgType = EG_S3_FWDRELCMPACK_CMD_CODE;
   gmFWDRELCMPACKCb->nmbIeProp = EG_S3_FWDRELCMPACK_NMB_IES;
   gmFWDRELCMPACKCb->validateIe = (EscFun)NULLP;

   /* Populate FWDRELCMPACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELCMPACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELCMPACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmFWDRELCMPACKCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELCMPACKCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELCMPACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELCMPACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3FWDRELCMPACKGmEntry */


/***********************************************************************

   Func  : egMakes3FWDRELCMPNTFGmEntry

   Desc  : This function makes the GM Entry for FWDRELCMPNTF

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3FWDRELCMPNTFGmEntry
(
LegMsgEntry               *gmFWDRELCMPNTFCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3FWDRELCMPNTFGmEntry(gmFWDRELCMPNTFCb)
LegMsgEntry               *gmFWDRELCMPNTFCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3FWDRELCMPNTFGmEntry);

   /* Populate the FWDRELCMPNTF GM Entry */
   gmFWDRELCMPNTFCb->msgType = EG_S3_FWDRELCMPNTF_CMD_CODE;
   gmFWDRELCMPNTFCb->nmbIeProp = EG_S3_FWDRELCMPNTF_NMB_IES;
   gmFWDRELCMPNTFCb->validateIe = (EscFun)NULLP;

   /* Populate FWDRELCMPNTF IE Properties */

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmFWDRELCMPNTFCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELCMPNTFCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELCMPNTFCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELCMPNTFCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3FWDRELCMPNTFGmEntry */


/***********************************************************************

   Func  : egMakes3FWDRELREQGmEntry

   Desc  : This function makes the GM Entry for FWDRELREQ

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3FWDRELREQGmEntry
(
LegMsgEntry               *gmFWDRELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3FWDRELREQGmEntry(gmFWDRELREQCb)
LegMsgEntry               *gmFWDRELREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes3FWDRELREQGmEntry);

   /* Populate the FWDRELREQ GM Entry */
   gmFWDRELREQCb->msgType = EG_S3_FWDRELREQ_CMD_CODE;
   gmFWDRELREQCb->nmbIeProp = EG_S3_FWDRELREQ_NMB_IES;
   gmFWDRELREQCb->validateIe = (EscFun)NULLP;

   /* Populate FWDRELREQ IE Properties */

   /* Populate IMSI IE Properties */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif

   gmFWDRELREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDN_CONN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_PDN_CONN_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmFWDRELREQCb->posIndex[0][EG_PDN_CONN_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmFWDRELREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmFWDRELREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_APN_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_SEL_MODE_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE,
0 );


      grpCnt1 = 0;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe) ,sizeof(LegGroupedIe));
      if( ret == ROK && (gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe)!= NULLP)
      {

         ieProperties = &gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe->ieProperties;
         ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES)));
         if( ret == ROK && *ieProperties != NULLP)
         {

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES);

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );
#else /* defined(EG_REL_930) || defined(EG_REL_AC0) */
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_F_CONTAINER_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );
#endif /* defined(EG_REL_930) || defined(EG_REL_AC0) */

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_TRANS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_BEARER_FLAGS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

      }
   }
   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_AMBR_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif /* defined(EG_REL_930) || defined(EG_REL_AC0) */

#ifdef EG_REL_AC0
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_SIG_PRIOR_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CNG_TO_RPT_FLG_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif /* EG_REL_AC0 */
}

}
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* CR 135352 fix starts*/
   /* Populate GSM_KEY_TRIP IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   /* CR 135352 fix endss*/

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELREQCb->posIndex[2][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TARGET_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_TARGET_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_TARGET_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELREQCb->posIndex[2][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SEL_PLMN_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_SEL_PLMN_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_SEL_PLMN_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RFSP_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_RFSP_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_RFSP_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RFSP_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_RFSP_ID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_RFSP_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate CSG_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_CSG_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_CSG_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CMI IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_CMI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_CMI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif /* defined(EG_REL_930) || defined(EG_REL_AC0) */

#ifdef EG_REL_AC0

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ADD_MM_CTXT_FOR_SRVCC IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ADD_FLAG_FOR_SRVCC IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_ADD_FLAG_FOR_SRVCC_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_ADD_FLAG_FOR_SRVCC_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate STN_SR IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_STN_SR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_STN_SR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELREQCb->posIndex[1][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELREQCb->posIndex[2][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif /* EG_REL_AC0 */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3FWDRELREQGmEntry */


/***********************************************************************

   Func  : egMakes3FWDRELRSPGmEntry

   Desc  : This function makes the GM Entry for FWDRELRSP

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3FWDRELRSPGmEntry
(
LegMsgEntry               *gmFWDRELRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3FWDRELRSPGmEntry(gmFWDRELRSPCb)
LegMsgEntry               *gmFWDRELRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes3FWDRELRSPGmEntry);

   /* Populate the FWDRELRSP GM Entry */
   gmFWDRELRSPCb->msgType = EG_S3_FWDRELRSP_CMD_CODE;
   gmFWDRELRSPCb->nmbIeProp = EG_S3_FWDRELRSP_NMB_IES;
   gmFWDRELRSPCb->validateIe = (EscFun)NULLP;

   /* Populate FWDRELRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmFWDRELRSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_FWDRELRSP_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
EG_FWDRELRSP_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PKT_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_OPTIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_OPTIONAL,5,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_OPTIONAL,2,TRUE);

   gmFWDRELRSPCb->posIndex[2][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_FWDRELRSP_BEARER_CNTX2_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
EG_FWDRELRSP_BEARER_CNTX2_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PKT_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_OPTIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmFWDRELRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_OPTIONAL,5,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELRSPCb->posIndex[1][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELRSPCb->posIndex[2][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELRSPCb->posIndex[1][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmFWDRELRSPCb->posIndex[2][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmFWDRELRSPCb->posIndex[1][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif /* EG_REL_AC0 */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3FWDRELRSPGmEntry */


/***********************************************************************

   Func  : egMakes3IDENTREQGmEntry

   Desc  : This function makes the GM Entry for IDENTREQ

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3IDENTREQGmEntry
(
LegMsgEntry               *gmIDENTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3IDENTREQGmEntry(gmIDENTREQCb)
LegMsgEntry               *gmIDENTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3IDENTREQGmEntry);

   /* Populate the IDENTREQ GM Entry */
   gmIDENTREQCb->msgType = EG_S3_IDENTREQ_CMD_CODE;
   gmIDENTREQCb->nmbIeProp = EG_S3_IDENTREQ_NMB_IES;
   gmIDENTREQCb->validateIe = (EscFun)NULLP;

   /* Populate IDENTREQ IE Properties */

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PTMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PTMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI_SIG IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PTMSI_SIG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PTMSI_SIG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PORT IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PORT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PORT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate HOP_CNTR IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_HOP_CNTR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_HOP_CNTR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3IDENTREQGmEntry */


/***********************************************************************

   Func  : egMakes3IDENTRSPGmEntry

   Desc  : This function makes the GM Entry for IDENTRSP

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3IDENTRSPGmEntry
(
LegMsgEntry               *gmIDENTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3IDENTRSPGmEntry(gmIDENTRSPCb)
LegMsgEntry               *gmIDENTRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3IDENTRSPGmEntry);

   /* Populate the IDENTRSP GM Entry */
   gmIDENTRSPCb->msgType = EG_S3_IDENTRSP_CMD_CODE;
   gmIDENTRSPCb->nmbIeProp = EG_S3_IDENTRSP_NMB_IES;
   gmIDENTRSPCb->validateIe = (EscFun)NULLP;

   /* Populate IDENTRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_TRIP IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_UMTS_KEY_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmIDENTRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3IDENTRSPGmEntry */


/***********************************************************************

   Func  : egMakes3RANINFORLYGmEntry

   Desc  : This function makes the GM Entry for RANINFORLY

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3RANINFORLYGmEntry
(
LegMsgEntry               *gmRANINFORLYCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3RANINFORLYGmEntry(gmRANINFORLYCb)
LegMsgEntry               *gmRANINFORLYCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3RANINFORLYGmEntry);

   /* Populate the RANINFORLY GM Entry */
   gmRANINFORLYCb->msgType = EG_S3_RANINFORLY_CMD_CODE;
   gmRANINFORLYCb->nmbIeProp = EG_S3_RANINFORLY_NMB_IES;
   gmRANINFORLYCb->validateIe = (EscFun)NULLP;

   /* Populate RANINFORLY IE Properties */

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmRANINFORLYCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRANINFORLYCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TARGET_ID IE Properties */
   egMakeAppIeProp(&gmRANINFORLYCb->ieProperties[ieCnt],
                    EG_TARGET_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRANINFORLYCb->posIndex[0][EG_TARGET_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRANINFORLYCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRANINFORLYCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3RANINFORLYGmEntry */


/***********************************************************************

   Func  : egMakes3RELCANCELREQGmEntry

   Desc  : This function makes the GM Entry for RELCANCELREQ

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3RELCANCELREQGmEntry
(
LegMsgEntry               *gmRELCANCELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3RELCANCELREQGmEntry(gmRELCANCELREQCb)
LegMsgEntry               *gmRELCANCELREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3RELCANCELREQGmEntry);

   /* Populate the RELCANCELREQ GM Entry */
   gmRELCANCELREQCb->msgType = EG_S3_RELCANCELREQ_CMD_CODE;
   gmRELCANCELREQCb->nmbIeProp = EG_S3_RELCANCELREQ_NMB_IES;
   gmRELCANCELREQCb->validateIe = (EscFun)NULLP;

   /* Populate RELCANCELREQ IE Properties */

   /* Populate IMSI IE Properties */
#ifdef EG_REL_AC0
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif /* EG_REL_AC0 */

   gmRELCANCELREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate F_CAUSE IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_F_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_F_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRELCANCELREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3RELCANCELREQGmEntry */


/***********************************************************************

   Func  : egMakes3RELCANCELRSPGmEntry

   Desc  : This function makes the GM Entry for RELCANCELRSP

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3RELCANCELRSPGmEntry
(
LegMsgEntry               *gmRELCANCELRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3RELCANCELRSPGmEntry(gmRELCANCELRSPCb)
LegMsgEntry               *gmRELCANCELRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3RELCANCELRSPGmEntry);

   /* Populate the RELCANCELRSP GM Entry */
   gmRELCANCELRSPCb->msgType = EG_S3_RELCANCELRSP_CMD_CODE;
   gmRELCANCELRSPCb->nmbIeProp = EG_S3_RELCANCELRSP_NMB_IES;
   gmRELCANCELRSPCb->validateIe = (EscFun)NULLP;

   /* Populate RELCANCELRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmRELCANCELRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRELCANCELRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRELCANCELRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRELCANCELRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3RELCANCELRSPGmEntry */


/***********************************************************************

   Func  : egMakes3SUSPNDACKGmEntry

   Desc  : This function makes the GM Entry for SUSPNDACK

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3SUSPNDACKGmEntry
(
LegMsgEntry               *gmSUSPNDACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3SUSPNDACKGmEntry(gmSUSPNDACKCb)
LegMsgEntry               *gmSUSPNDACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3SUSPNDACKGmEntry);

   /* Populate the SUSPNDACK GM Entry */
   gmSUSPNDACKCb->msgType = EG_S3_SUSPNDACK_CMD_CODE;
   gmSUSPNDACKCb->nmbIeProp = EG_S3_SUSPNDACK_NMB_IES;
   gmSUSPNDACKCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPNDACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmSUSPNDACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmSUSPNDACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPNDACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPNDACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3SUSPNDACKGmEntry */


/***********************************************************************

   Func  : egMakes3SUSPNDNOTFNGmEntry

   Desc  : This function makes the GM Entry for SUSPNDNOTFN

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3SUSPNDNOTFNGmEntry
(
LegMsgEntry               *gmSUSPNDNOTFNCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3SUSPNDNOTFNGmEntry(gmSUSPNDNOTFNCb)
LegMsgEntry               *gmSUSPNDNOTFNCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3SUSPNDNOTFNGmEntry);

   /* Populate the SUSPNDNOTFN GM Entry */
   gmSUSPNDNOTFNCb->msgType = EG_S3_SUSPNDNOTFN_CMD_CODE;
   gmSUSPNDNOTFNCb->nmbIeProp = EG_S3_SUSPNDNOTFN_NMB_IES;
   gmSUSPNDNOTFNCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPNDNOTFN IE Properties */

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmSUSPNDNOTFNCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PTMSI IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_PTMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmSUSPNDNOTFNCb->posIndex[0][EG_PTMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPNDNOTFNCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3SUSPNDNOTFNGmEntry */


#if defined(EG_REL_AC0) || defined(EG_REL_930)
/***********************************************************************

   Func  : egMakes3UEACTACKGmEntry

   Desc  : This function makes the GM Entry for UEACTACK

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3UEACTACKGmEntry
(
LegMsgEntry               *gmUEACTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3UEACTACKGmEntry(gmUEACTACKCb)
LegMsgEntry               *gmUEACTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3UEACTACKGmEntry);

   /* Populate the UEACTACK GM Entry */
   gmUEACTACKCb->msgType = EG_S3_UEACTACK_CMD_CODE;
   gmUEACTACKCb->nmbIeProp = EG_S3_UEACTACK_NMB_IES;
   gmUEACTACKCb->validateIe = (EscFun)NULLP;

   /* Populate UEACTACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUEACTACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmUEACTACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUEACTACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUEACTACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3UEACTACKGmEntry */


/***********************************************************************

   Func  : egMakes3UEACTNOTFNGmEntry

   Desc  : This function makes the GM Entry for UEACTNOTFN

   Ret   : None

   File  : smegs3.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes3UEACTNOTFNGmEntry
(
LegMsgEntry               *gmUEACTNOTFNCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes3UEACTNOTFNGmEntry(gmUEACTNOTFNCb)
LegMsgEntry               *gmUEACTNOTFNCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes3UEACTNOTFNGmEntry);

   /* Populate the UEACTNOTFN GM Entry */
   gmUEACTNOTFNCb->msgType = EG_S3_UEACTNOTFN_CMD_CODE;
   gmUEACTNOTFNCb->nmbIeProp = EG_S3_UEACTNOTFN_NMB_IES;
   gmUEACTNOTFNCb->validateIe = (EscFun)NULLP;

   /* Populate UEACTNOTFN IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUEACTNOTFNCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUEACTNOTFNCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes3UEACTNOTFNGmEntry */
#endif /* #if defined(EG_REL_AC0) || defined(EG_REL_930) */

/***********************************************************************

   Func  : egMakes3GmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegs3.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakes3GmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakes3GmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakes3GmDict);

   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_AC0) || defined(EG_REL_930)
   /* Populating ALRTMMEACK GM Entry */
   egMakes3ALRTMMEACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating ALRTMMENOTFN GM Entry */
   egMakes3ALRTMMENOTFNGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif /* defined(EG_REL_AC0) || defined(EG_REL_930) */

   /* Populating CSPAGEIND GM Entry */
   egMakes3CSPAGEINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXACK GM Entry */
   egMakes3CTXACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXREQ GM Entry */
   egMakes3CTXREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXRSP GM Entry */
   egMakes3CTXRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DETACK GM Entry */
   egMakes3DETACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DETNTF GM Entry */
   egMakes3DETNTFGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELCMPACK GM Entry */
   egMakes3FWDRELCMPACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELCMPNTF GM Entry */
   egMakes3FWDRELCMPNTFGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELREQ GM Entry */
   egMakes3FWDRELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELRSP GM Entry */
   egMakes3FWDRELRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTREQ GM Entry */
   egMakes3IDENTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTRSP GM Entry */
   egMakes3IDENTRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RANINFORLY GM Entry */
   egMakes3RANINFORLYGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELCANCELREQ GM Entry */
   egMakes3RELCANCELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELCANCELRSP GM Entry */
   egMakes3RELCANCELRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPNDACK GM Entry */
   egMakes3SUSPNDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPNDNOTFN GM Entry */
   egMakes3SUSPNDNOTFNGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_AC0) || defined(EG_REL_930)
   /* Populating UEACTACK GM Entry */
   egMakes3UEACTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UEACTNOTFN GM Entry */
   egMakes3UEACTNOTFNGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif/* defined(EG_REL_AC0) || defined(EG_REL_930) */
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes3GmDict */


#endif
/********************************************************************30**

         End of file:     eg_dbs3.c@@/main/2 - Fri Feb  4 18:18:04 2011

*********************************************************************31*/

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
----------- -------- ---- -----------------------------------------------
/main/1      ---      an                1. Initial Release
/main/1      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/2      ---      svenkat           1. Initial for eGTP 2.1 Release
/main/2     eg007.201 psingh            1. Fixed TRACE5 macro related issue
/main/2     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/
