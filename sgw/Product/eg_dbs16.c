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


/***********************************************************************

    Name: Source file for filling LM configuration

    Type: C Source

    Desc: 

    File: eg_dbs16.c

    Sid:      eg_dbs16.c@@/main/2 - Fri Feb  4 18:18:04 2011

    Prg: an

************************************************************************/


/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/* Header Include Files (.h) */
#include "envopt.h"            /* Environment Options */
#include "envdep.h"            /* Environment Dependent */
#include "envind.h"            /* Environment Independent */
#include "gen.h"               /* General */
#include "ssi.h"               /* System Services */
#include "cm_tkns.h"           /* Common Tokens */
#include "cm_hash.h"           /* Common Hash List */
#include "cm_mblk.h"           /* Common memory */
#include "cm_llist.h"          /* Common Link List */
#include "cm5.h"               /* Common Structures */
#include "cm_inet.h"           /* Common IP network */
#include "cm_tpt.h"            /* Common Transport */
#include "hit.h"               /* TUCL layer */
#include "egt.h"               /* EU layer */
#include "leg.h"               /* layer manager */
#include "eg_util.h"           /* UTIL */
#include "eg_edm.h"            /* EDM MODULE */
#include "eg_dbs16.h"
#include "eg_dbutl.h"   /* Ie codes are here */

/* Header Include Files (.x) */

#include "gen.x"               /* General */
#include "ssi.x"               /* System Services */
#include "cm_tkns.x"           /* Common Tokens */
#include "cm_hash.x"           /* Common Hash List */
#include "cm_mblk.x"           /* Common memory */
#include "cm_llist.x"          /* Common Link List */
#include "cm5.x"               /* Common Structures */
#include "cm_inet.x"           /* Common IP network */
#include "cm_tpt.x"            /* Common Transport */
#include "hit.x"               /* TUCL layer */
#include "egt.x"               /* EU layer */
#include "leg.x"               /* layer manager */
#include "eg_util.x"           /* UTIL */
#include "eg_edm.x"      /* EDM MODULE */
#include "eg_dbutl.x"          /* Application MODULE */
#include "eg_dbs16.x"


#ifdef EGTP_C


/***********************************************************************

   Func  : egMakes16CTXACKGmEntry

   Desc  : This function makes the GM Entry for CTXACK

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16CTXACKGmEntry
(
LegMsgEntry               *gmCTXACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16CTXACKGmEntry(gmCTXACKCb)
LegMsgEntry               *gmCTXACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16CTXACKGmEntry);

   /* Populate the CTXACK GM Entry */
   gmCTXACKCb->msgType = EG_S16_CTXACK_CMD_CODE;
   gmCTXACKCb->nmbIeProp = EG_S16_CTXACK_NMB_IES;
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

} /* End of egMakes16CTXACKGmEntry */


/***********************************************************************

   Func  : egMakes16CTXREQGmEntry

   Desc  : This function makes the GM Entry for CTXREQ

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16CTXREQGmEntry
(
LegMsgEntry               *gmCTXREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16CTXREQGmEntry(gmCTXREQCb)
LegMsgEntry               *gmCTXREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16CTXREQGmEntry);

   /* Populate the CTXREQ GM Entry */
   gmCTXREQCb->msgType = EG_S16_CTXREQ_CMD_CODE;
   gmCTXREQCb->nmbIeProp = EG_S16_CTXREQ_NMB_IES;
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

   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16CTXREQGmEntry */


/***********************************************************************

   Func  : egMakes16CTXRSPGmEntry

   Desc  : This function makes the GM Entry for CTXRSP

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16CTXRSPGmEntry
(
LegMsgEntry               *gmCTXRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16CTXRSPGmEntry(gmCTXRSPCb)
LegMsgEntry               *gmCTXRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes16CTXRSPGmEntry);

   /* Populate the CTXRSP GM Entry */
   gmCTXRSPCb->msgType = EG_S16_CTXRSP_CMD_CODE;
   gmCTXRSPCb->nmbIeProp = EG_S16_CTXRSP_NMB_IES;
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
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif

#ifdef EG_REL_AC0 
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_SIG_PRIOR_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;

   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCTXRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CNG_TO_RPT_FLG_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif 
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

   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16CTXRSPGmEntry */


/***********************************************************************

   Func  : egMakes16FWDACCSCTXACKGmEntry

   Desc  : This function makes the GM Entry for FWDACCSCTXACK

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16FWDACCSCTXACKGmEntry
(
LegMsgEntry               *gmFWDACCSCTXACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16FWDACCSCTXACKGmEntry(gmFWDACCSCTXACKCb)
LegMsgEntry               *gmFWDACCSCTXACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16FWDACCSCTXACKGmEntry);

   /* Populate the FWDACCSCTXACK GM Entry */
   gmFWDACCSCTXACKCb->msgType = EG_S16_FWDACCSCTXACK_CMD_CODE;
   gmFWDACCSCTXACKCb->nmbIeProp = EG_S16_FWDACCSCTXACK_NMB_IES;
   gmFWDACCSCTXACKCb->validateIe = (EscFun)NULLP;

   /* Populate FWDACCSCTXACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmFWDACCSCTXACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDACCSCTXACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDACCSCTXACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDACCSCTXACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16FWDACCSCTXACKGmEntry */


/***********************************************************************

   Func  : egMakes16FWDACCSCTXNTFGmEntry

   Desc  : This function makes the GM Entry for FWDACCSCTXNTF

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16FWDACCSCTXNTFGmEntry
(
LegMsgEntry               *gmFWDACCSCTXNTFCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16FWDACCSCTXNTFGmEntry(gmFWDACCSCTXNTFCb)
LegMsgEntry               *gmFWDACCSCTXNTFCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16FWDACCSCTXNTFGmEntry);

   /* Populate the FWDACCSCTXNTF GM Entry */
   gmFWDACCSCTXNTFCb->msgType = EG_S16_FWDACCSCTXNTF_CMD_CODE;
   gmFWDACCSCTXNTFCb->nmbIeProp = EG_S16_FWDACCSCTXNTF_NMB_IES;
   gmFWDACCSCTXNTFCb->validateIe = (EscFun)NULLP;

   /* Populate FWDACCSCTXNTF IE Properties */

   /* Populate RAB_CONTEXT IE Properties */
   egMakeAppIeProp(&gmFWDACCSCTXNTFCb->ieProperties[ieCnt],
                    EG_RAB_CONTEXT_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmFWDACCSCTXNTFCb->posIndex[0][EG_RAB_CONTEXT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SRC_RNC_PDCP_CTX IE Properties */
   egMakeAppIeProp(&gmFWDACCSCTXNTFCb->ieProperties[ieCnt],
                    EG_SRC_RNC_PDCP_CTX_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDACCSCTXNTFCb->posIndex[0][EG_SRC_RNC_PDCP_CTX_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDU_NUMBER IE Properties */
   egMakeAppIeProp(&gmFWDACCSCTXNTFCb->ieProperties[ieCnt],
                    EG_PDU_NUMBER_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDACCSCTXNTFCb->posIndex[0][EG_PDU_NUMBER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDACCSCTXNTFCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDACCSCTXNTFCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16FWDACCSCTXNTFGmEntry */


/***********************************************************************

   Func  : egMakes16FWDRELCMPACKGmEntry

   Desc  : This function makes the GM Entry for FWDRELCMPACK

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16FWDRELCMPACKGmEntry
(
LegMsgEntry               *gmFWDRELCMPACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16FWDRELCMPACKGmEntry(gmFWDRELCMPACKCb)
LegMsgEntry               *gmFWDRELCMPACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16FWDRELCMPACKGmEntry);

   /* Populate the FWDRELCMPACK GM Entry */
   gmFWDRELCMPACKCb->msgType = EG_S16_FWDRELCMPACK_CMD_CODE;
   gmFWDRELCMPACKCb->nmbIeProp = EG_S16_FWDRELCMPACK_NMB_IES;
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

} /* End of egMakes16FWDRELCMPACKGmEntry */


/***********************************************************************

   Func  : egMakes16FWDRELCMPNTFGmEntry

   Desc  : This function makes the GM Entry for FWDRELCMPNTF

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16FWDRELCMPNTFGmEntry
(
LegMsgEntry               *gmFWDRELCMPNTFCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16FWDRELCMPNTFGmEntry(gmFWDRELCMPNTFCb)
LegMsgEntry               *gmFWDRELCMPNTFCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16FWDRELCMPNTFGmEntry);

   /* Populate the FWDRELCMPNTF GM Entry */
   gmFWDRELCMPNTFCb->msgType = EG_S16_FWDRELCMPNTF_CMD_CODE;
   gmFWDRELCMPNTFCb->nmbIeProp = EG_S16_FWDRELCMPNTF_NMB_IES;
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

} /* End of egMakes16FWDRELCMPNTFGmEntry */


/***********************************************************************

   Func  : egMakes16FWDRELREQGmEntry

   Desc  : This function makes the GM Entry for FWDRELREQ

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16FWDRELREQGmEntry
(
LegMsgEntry               *gmFWDRELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16FWDRELREQGmEntry(gmFWDRELREQCb)
LegMsgEntry               *gmFWDRELREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes16FWDRELREQGmEntry);

   /* Populate the FWDRELREQ GM Entry */
   gmFWDRELREQCb->msgType = EG_S16_FWDRELREQ_CMD_CODE;
   gmFWDRELREQCb->nmbIeProp = EG_S16_FWDRELREQ_NMB_IES;
   gmFWDRELREQCb->validateIe = (EscFun)NULLP;

   /* Populate FWDRELREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1++, EG_F_CONTAINER_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

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
#endif
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
#endif 
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

   /* Populate GSM_KEY_TRIP IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,EG_IE_MANDATORY,0,FALSE);

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

   /* Populate SRC_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_SRC_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_SRC_ID_IETYPE]  = ieCnt + 1;
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
#endif
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

   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16FWDRELREQGmEntry */


/***********************************************************************

   Func  : egMakes16FWDRELRSPGmEntry

   Desc  : This function makes the GM Entry for FWDRELRSP

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16FWDRELRSPGmEntry
(
LegMsgEntry               *gmFWDRELRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16FWDRELRSPGmEntry(gmFWDRELRSPCb)
LegMsgEntry               *gmFWDRELRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes16FWDRELRSPGmEntry);

   /* Populate the FWDRELRSP GM Entry */
   gmFWDRELRSPCb->msgType = EG_S16_FWDRELRSP_CMD_CODE;
   gmFWDRELRSPCb->nmbIeProp = EG_S16_FWDRELRSP_NMB_IES;
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
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16FWDRELRSPGmEntry */


/***********************************************************************

   Func  : egMakes16IDENTREQGmEntry

   Desc  : This function makes the GM Entry for IDENTREQ

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16IDENTREQGmEntry
(
LegMsgEntry               *gmIDENTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16IDENTREQGmEntry(gmIDENTREQCb)
LegMsgEntry               *gmIDENTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16IDENTREQGmEntry);

   /* Populate the IDENTREQ GM Entry */
   gmIDENTREQCb->msgType = EG_S16_IDENTREQ_CMD_CODE;
   gmIDENTREQCb->nmbIeProp = EG_S16_IDENTREQ_NMB_IES;
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

} /* End of egMakes16IDENTREQGmEntry */


/***********************************************************************

   Func  : egMakes16IDENTRSPGmEntry

   Desc  : This function makes the GM Entry for IDENTRSP

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16IDENTRSPGmEntry
(
LegMsgEntry               *gmIDENTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16IDENTRSPGmEntry(gmIDENTRSPCb)
LegMsgEntry               *gmIDENTRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16IDENTRSPGmEntry);

   /* Populate the IDENTRSP GM Entry */
   gmIDENTRSPCb->msgType = EG_S16_IDENTRSP_CMD_CODE;
   gmIDENTRSPCb->nmbIeProp = EG_S16_IDENTRSP_NMB_IES;
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

} /* End of egMakes16IDENTRSPGmEntry */


/***********************************************************************

   Func  : egMakes16RANINFORLYGmEntry

   Desc  : This function makes the GM Entry for RANINFORLY

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16RANINFORLYGmEntry
(
LegMsgEntry               *gmRANINFORLYCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16RANINFORLYGmEntry(gmRANINFORLYCb)
LegMsgEntry               *gmRANINFORLYCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16RANINFORLYGmEntry);

   /* Populate the RANINFORLY GM Entry */
   gmRANINFORLYCb->msgType = EG_S16_RANINFORLY_CMD_CODE;
   gmRANINFORLYCb->nmbIeProp = EG_S16_RANINFORLY_NMB_IES;
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

} /* End of egMakes16RANINFORLYGmEntry */


/***********************************************************************

   Func  : egMakes16RELCANCELREQGmEntry

   Desc  : This function makes the GM Entry for RELCANCELREQ

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16RELCANCELREQGmEntry
(
LegMsgEntry               *gmRELCANCELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16RELCANCELREQGmEntry(gmRELCANCELREQCb)
LegMsgEntry               *gmRELCANCELREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16RELCANCELREQGmEntry);

   /* Populate the RELCANCELREQ GM Entry */
   gmRELCANCELREQCb->msgType = EG_S16_RELCANCELREQ_CMD_CODE;
   gmRELCANCELREQCb->nmbIeProp = EG_S16_RELCANCELREQ_NMB_IES;
   gmRELCANCELREQCb->validateIe = (EscFun)NULLP;

   /* Populate RELCANCELREQ IE Properties */

#ifdef EG_REL_AC0
   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmRELCANCELREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif

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

} /* End of egMakes16RELCANCELREQGmEntry */


/***********************************************************************

   Func  : egMakes16RELCANCELRSPGmEntry

   Desc  : This function makes the GM Entry for RELCANCELRSP

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16RELCANCELRSPGmEntry
(
LegMsgEntry               *gmRELCANCELRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16RELCANCELRSPGmEntry(gmRELCANCELRSPCb)
LegMsgEntry               *gmRELCANCELRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16RELCANCELRSPGmEntry);

   /* Populate the RELCANCELRSP GM Entry */
   gmRELCANCELRSPCb->msgType = EG_S16_RELCANCELRSP_CMD_CODE;
   gmRELCANCELRSPCb->nmbIeProp = EG_S16_RELCANCELRSP_NMB_IES;
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

} /* End of egMakes16RELCANCELRSPGmEntry */


/***********************************************************************

   Func  : egMakes16SUSPNDACKGmEntry

   Desc  : This function makes the GM Entry for SUSPNDACK

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16SUSPNDACKGmEntry
(
LegMsgEntry               *gmSUSPNDACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16SUSPNDACKGmEntry(gmSUSPNDACKCb)
LegMsgEntry               *gmSUSPNDACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16SUSPNDACKGmEntry);

   /* Populate the SUSPNDACK GM Entry */
   gmSUSPNDACKCb->msgType = EG_S16_SUSPNDACK_CMD_CODE;
   gmSUSPNDACKCb->nmbIeProp = EG_S16_SUSPNDACK_NMB_IES;
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

} /* End of egMakes16SUSPNDACKGmEntry */


/***********************************************************************

   Func  : egMakes16SUSPNDNOTFNGmEntry

   Desc  : This function makes the GM Entry for SUSPNDNOTFN

   Ret   : None

   File  : smegs16.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes16SUSPNDNOTFNGmEntry
(
LegMsgEntry               *gmSUSPNDNOTFNCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes16SUSPNDNOTFNGmEntry(gmSUSPNDNOTFNCb)
LegMsgEntry               *gmSUSPNDNOTFNCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes16SUSPNDNOTFNGmEntry);

   /* Populate the SUSPNDNOTFN GM Entry */
   gmSUSPNDNOTFNCb->msgType = EG_S16_SUSPNDNOTFN_CMD_CODE;
   gmSUSPNDNOTFNCb->nmbIeProp = EG_S16_SUSPNDNOTFN_NMB_IES;
   gmSUSPNDNOTFNCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPNDNOTFN IE Properties */
   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
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

#endif
   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPNDNOTFNCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes16SUSPNDNOTFNGmEntry */
/***********************************************************************

   Func  : egMakes16GmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegs16.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakes16GmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakes16GmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakes16GmDict);

   /* Populating CTXACK GM Entry */
   egMakes16CTXACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXREQ GM Entry */
   egMakes16CTXREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXRSP GM Entry */
   egMakes16CTXRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDACCSCTXACK GM Entry */
   egMakes16FWDACCSCTXACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDACCSCTXNTF GM Entry */
   egMakes16FWDACCSCTXNTFGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELCMPACK GM Entry */
   egMakes16FWDRELCMPACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELCMPNTF GM Entry */
   egMakes16FWDRELCMPNTFGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELREQ GM Entry */
   egMakes16FWDRELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELRSP GM Entry */
   egMakes16FWDRELRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTREQ GM Entry */
   egMakes16IDENTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTRSP GM Entry */
   egMakes16IDENTRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RANINFORLY GM Entry */
   egMakes16RANINFORLYGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELCANCELREQ GM Entry */
   egMakes16RELCANCELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELCANCELRSP GM Entry */
   egMakes16RELCANCELRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPNDACK GM Entry */
   egMakes16SUSPNDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPNDNOTFN GM Entry */
   egMakes16SUSPNDNOTFNGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes16GmDict */


#endif

/********************************************************************30**

         End of file:     eg_dbs16.c@@/main/2 - Fri Feb  4 18:18:04 2011

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
/main/2     eg003.201 psingh            1. Merged code from eg006.201 to 
                                           Fix compilation warnings
/main/2     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/
