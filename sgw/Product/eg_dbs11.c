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

    File: eg_dbs11.c

    Sid:      eg_dbs11.c@@/main/3 - Fri Feb  4 18:18:03 2011

    Prg : an

************************************************************************/


/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/* Header Include Files (.h) */
#include "envopt.h"           /* Environment Options */
#include "envdep.h"           /* Environment Dependent */
#include "envind.h"           /* Environment Independent */
#include "gen.h"              /* General */
#include "ssi.h"              /* System Services */
#include "cm_tkns.h"          /* Common Tokens */
#include "cm_hash.h"          /* Common Hash List */
#include "cm_mblk.h"          /* Common memory */
#include "cm_llist.h"         /* Common Link List */
#include "cm5.h"              /* Common Structures */
#include "cm_inet.h"          /* Common IP network */
#include "cm_tpt.h"           /* Common Transport */
#include "hit.h"              /* TUCL layer */
#include "egt.h"              /* EU layer */
#include "leg.h"              /* layer manager */
#include "eg_util.h"          /* UTIL */
#include "eg_edm.h"           /* EDM MODULE */
#include "eg_dbs11.h"
#include "eg_dbutl.h"   /* Ie codes are here */

/* Header Include Files (.x) */

#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* Common Tokens */
#include "cm_hash.x"       /* Common Hash List */
#include "cm_mblk.x"       /* Common memory */
#include "cm_llist.x"      /* Common Link List */
#include "cm5.x"           /* Common Structures */
#include "cm_inet.x"       /* Common IP network */
#include "cm_tpt.x"        /* Common Transport */
#include "hit.x"           /* TUCL layer */
#include "egt.x"           /* EU layer */
#include "leg.x"           /* layer manager */
#include "eg_util.x"       /* UTIL */
#include "eg_edm.x"      /* Application MODULE */
#include "eg_dbutl.x"      /* Application MODULE */
#include "eg_dbs11.x"

#ifdef EGTP_C


/***********************************************************************

   Func  : egMakes11BRERESCMDGmEntry

   Desc  : This function makes the GM Entry for BRERESCMD

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11BRERESCMDGmEntry
(
LegMsgEntry               *gmBRERESCMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11BRERESCMDGmEntry(gmBRERESCMDCb)
LegMsgEntry               *gmBRERESCMDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11BRERESCMDGmEntry);

   /* Populate the BRERESCMD GM Entry */
   gmBRERESCMDCb->msgType = EG_S11_BRERESCMD_CMD_CODE;
   gmBRERESCMDCb->nmbIeProp = EG_S11_BRERESCMD_NMB_IES;
   gmBRERESCMDCb->validateIe = (EscFun)NULLP;

   /* Populate BRERESCMD IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FLOW_QOS IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_FLOW_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_FLOW_QOS_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TAD IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_TAD_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_TAD_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmBRERESCMDCb->posIndex[1][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate SIG_PRIOR_IND IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_SIG_PRIOR_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_SIG_PRIOR_IND_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11BRERESCMDGmEntry */


/***********************************************************************

   Func  : egMakes11BRERESFLRINDGmEntry

   Desc  : This function makes the GM Entry for BRERESFLRIND

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11BRERESFLRINDGmEntry
(
LegMsgEntry               *gmBRERESFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11BRERESFLRINDGmEntry(gmBRERESFLRINDCb)
LegMsgEntry               *gmBRERESFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11BRERESFLRINDGmEntry);

   /* Populate the BRERESFLRIND GM Entry */
   gmBRERESFLRINDCb->msgType = EG_S11_BRERESFLRIND_CMD_CODE;
   gmBRERESFLRINDCb->nmbIeProp = EG_S11_BRERESFLRIND_NMB_IES;
   gmBRERESFLRINDCb->validateIe = (EscFun)NULLP;

   /* Populate BRERESFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmBRERESFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmBRERESFLRINDCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmBRERESFLRINDCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESFLRINDCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmBRERESFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11BRERESFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes11CHGNTFREQGmEntry

   Desc  : This function makes the GM Entry for CHGNTFREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CHGNTFREQGmEntry
(
LegMsgEntry               *gmCHGNTFREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CHGNTFREQGmEntry(gmCHGNTFREQCb)
LegMsgEntry               *gmCHGNTFREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11CHGNTFREQGmEntry);

   /* Populate the CHGNTFREQ GM Entry */
   gmCHGNTFREQCb->msgType = EG_S11_CHGNTFREQ_CMD_CODE;
   gmCHGNTFREQCb->nmbIeProp = EG_S11_CHGNTFREQ_NMB_IES;
   gmCHGNTFREQCb->validateIe = (EscFun)NULLP;

   /* Populate CHGNTFREQ IE Properties */

#ifdef EG_REL_AC0
   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif

   gmCHGNTFREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCHGNTFREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCHGNTFREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CHGNTFREQGmEntry */


/***********************************************************************

   Func  : egMakes11CHGNTFRSPGmEntry

   Desc  : This function makes the GM Entry for CHGNTFRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CHGNTFRSPGmEntry
(
LegMsgEntry               *gmCHGNTFRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CHGNTFRSPGmEntry(gmCHGNTFRSPCb)
LegMsgEntry               *gmCHGNTFRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11CHGNTFRSPGmEntry);

   /* Populate the CHGNTFRSP GM Entry */
   gmCHGNTFRSPCb->msgType = EG_S11_CHGNTFRSP_CMD_CODE;
   gmCHGNTFRSPCb->nmbIeProp = EG_S11_CHGNTFRSP_NMB_IES;
   gmCHGNTFRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CHGNTFRSP IE Properties */

#ifdef EG_REL_AC0
   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif

   gmCHGNTFRSPCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFRSPCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCHGNTFRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFRSPCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate CSG_INFO_REP_ACT IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCHGNTFRSPCb->posIndex[0][EG_CSG_INFO_REP_ACT_IETYPE]  = ieCnt + 1;
#endif
   RETVOID;

} /* End of egMakes11CHGNTFRSPGmEntry */


/***********************************************************************

   Func  : egMakes11CRTBREREQGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTBREREQGmEntry
(
LegMsgEntry               *gmCRTBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTBREREQGmEntry(gmCRTBREREQCb)
LegMsgEntry               *gmCRTBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11CRTBREREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCRTBREREQCb->msgType = EG_S11_CRTBREREQ_CMD_CODE;
   gmCRTBREREQCb->nmbIeProp = EG_S11_CRTBREREQ_NMB_IES;
   gmCRTBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTBREREQ IE Properties */

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_TFT_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
#endif
}

}
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTBREREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate CSG_INFO_REP_ACT IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_CSG_INFO_REP_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTBREREQGmEntry */


/***********************************************************************

   Func  : egMakes11CRTBRERSPGmEntry

   Desc  : This function makes the GM Entry for CRTBRERSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTBRERSPGmEntry
(
LegMsgEntry               *gmCRTBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTBRERSPGmEntry(gmCRTBRERSPCb)
LegMsgEntry               *gmCRTBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11CRTBRERSPGmEntry);

   /* Populate the CRTBRERSP GM Entry */
   gmCRTBRERSPCb->msgType = EG_S11_CRTBRERSP_CMD_CODE;
   gmCRTBRERSPCb->nmbIeProp = EG_S11_CRTBRERSP_NMB_IES;
   gmCRTBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate CRTBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTBRERSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes11CRTFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for CRTFWDTUNREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTFWDTUNREQGmEntry
(
LegMsgEntry               *gmCRTFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTFWDTUNREQGmEntry(gmCRTFWDTUNREQCb)
LegMsgEntry               *gmCRTFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11CRTFWDTUNREQGmEntry);

   /* Populate the CRTFWDTUNREQ GM Entry */
   gmCRTFWDTUNREQCb->msgType = EG_S11_CRTFWDTUNREQ_CMD_CODE;
   gmCRTFWDTUNREQCb->nmbIeProp = EG_S11_CRTFWDTUNREQ_NMB_IES;
   gmCRTFWDTUNREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTFWDTUNREQ IE Properties */

   /* Populate S103_PDN_FW_INFO IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNREQCb->ieProperties[ieCnt],
                    EG_S103_PDN_FW_INFO_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTFWDTUNREQCb->posIndex[0][EG_S103_PDN_FW_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTFWDTUNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakes11CRTFWDTUNRSPGmEntry

   Desc  : This function makes the GM Entry for CRTFWDTUNRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTFWDTUNRSPGmEntry
(
LegMsgEntry               *gmCRTFWDTUNRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTFWDTUNRSPGmEntry(gmCRTFWDTUNRSPCb)
LegMsgEntry               *gmCRTFWDTUNRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11CRTFWDTUNRSPGmEntry);

   /* Populate the CRTFWDTUNRSP GM Entry */
   gmCRTFWDTUNRSPCb->msgType = EG_S11_CRTFWDTUNRSP_CMD_CODE;
   gmCRTFWDTUNRSPCb->nmbIeProp = EG_S11_CRTFWDTUNRSP_NMB_IES;
   gmCRTFWDTUNRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CRTFWDTUNRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate S1U_DATA_FW IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_S1U_DATA_FW_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_S1U_DATA_FW_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTFWDTUNRSPGmEntry */


/***********************************************************************

   Func  : egMakes11CRTINDDATFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for CRTINDDATFWDTUNREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTINDDATFWDTUNREQGmEntry
(
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTINDDATFWDTUNREQGmEntry(gmCRTINDDATFWDTUNREQCb)
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11CRTINDDATFWDTUNREQGmEntry);

   /* Populate the CRTINDDATFWDTUNREQ GM Entry */
   gmCRTINDDATFWDTUNREQCb->msgType = EG_S11_CRTINDDATFWDTUNREQ_CMD_CODE;
   gmCRTINDDATFWDTUNREQCb->nmbIeProp = EG_S11_CRTINDDATFWDTUNREQ_NMB_IES;
   gmCRTINDDATFWDTUNREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTINDDATFWDTUNREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTINDDATFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakes11CRTINDDATFWDTUNRSPGmEntry

   Desc  : This function makes the GM Entry for CRTINDDATFWDTUNRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTINDDATFWDTUNRSPGmEntry
(
LegMsgEntry               *gmCRTINDDATFWDTUNRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTINDDATFWDTUNRSPGmEntry(gmCRTINDDATFWDTUNRSPCb)
LegMsgEntry               *gmCRTINDDATFWDTUNRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11CRTINDDATFWDTUNRSPGmEntry);

   /* Populate the CRTINDDATFWDTUNRSP GM Entry */
   gmCRTINDDATFWDTUNRSPCb->msgType = EG_S11_CRTINDDATFWDTUNRSP_CMD_CODE;
   gmCRTINDDATFWDTUNRSPCb->nmbIeProp = EG_S11_CRTINDDATFWDTUNRSP_NMB_IES;
   gmCRTINDDATFWDTUNRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CRTINDDATFWDTUNRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTINDDATFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTINDDATFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTINDDATFWDTUNRSPGmEntry */


/***********************************************************************

   Func  : egMakes11CRTSESREQGmEntry

   Desc  : This function makes the GM Entry for CRTSESREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTSESREQGmEntry
(
LegMsgEntry               *gmCRTSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTSESREQGmEntry(gmCRTSESREQCb)
LegMsgEntry               *gmCRTSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11CRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_S11_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_S11_CRTSESREQ_NMB_IES;
   gmCRTSESREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTSESREQ IE Properties */

#if defined(EG_REL_930) || defined(EG_REL_AC0) 
   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif
   gmCRTSESREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APN_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_APN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SEL_MODE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SEL_MODE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_SEL_MODE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDN_TYPE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PDN_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PDN_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PAA IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PAA_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PAA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN_RESTRICT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_APN_RESTRICT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTSESREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmCRTSESREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
}

}
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_OPTIONAL,0,FALSE);
#endif
   gmCRTSESREQCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif
   /* Populate CHARGING_CHAR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_CHARGING_CHAR_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SIG_PRIOR_IND IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SIG_PRIOR_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_SIG_PRIOR_IND_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTSESREQGmEntry */


/***********************************************************************

   Func  : egMakes11CRTSESRSPGmEntry

   Desc  : This function makes the GM Entry for CRTSESRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CRTSESRSPGmEntry
(
LegMsgEntry               *gmCRTSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CRTSESRSPGmEntry(gmCRTSESRSPCb)
LegMsgEntry               *gmCRTSESRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11CRTSESRSPGmEntry);

   /* Populate the CRTSESRSP GM Entry */
   gmCRTSESRSPCb->msgType = EG_S11_CRTSESRSP_CMD_CODE;
   gmCRTSESRSPCb->nmbIeProp = EG_S11_CRTSESRSP_NMB_IES;
   gmCRTSESRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CRTSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate CSG_INFO_REP_ACT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_CSG_INFO_REP_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESRSPCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PAA IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PAA_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_PAA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN_RESTRICT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_APN_RESTRICT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmCRTSESRSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
  
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;

   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmCRTSESRSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESRSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPC_TMR IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_EPC_TMR_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_EPC_TMR_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11CRTSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes11DELBRECMDGmEntry

   Desc  : This function makes the GM Entry for DELBRECMD

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELBRECMDGmEntry
(
LegMsgEntry               *gmDELBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELBRECMDGmEntry(gmDELBRECMDCb)
LegMsgEntry               *gmDELBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11DELBRECMDGmEntry);

   /* Populate the DELBRECMD GM Entry */
   gmDELBRECMDCb->msgType = EG_S11_DELBRECMD_CMD_CODE;
   gmDELBRECMDCb->nmbIeProp = EG_S11_DELBRECMD_NMB_IES;
   gmDELBRECMDCb->validateIe = (EscFun)NULLP;

   /* Populate DELBRECMD IE Properties */

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBRECMDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmDELBRECMDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmDELBRECMDCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmDELBRECMDCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmDELBRECMDCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmDELBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
#if defined(EG_REL_930) || defined(EG_REL_AC0) 
   egMakeAppChildIeEntry((gmDELBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif
}

}
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBRECMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBRECMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes11DELBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for DELBREFLRIND

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELBREFLRINDGmEntry
(
LegMsgEntry               *gmDELBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELBREFLRINDGmEntry(gmDELBREFLRINDCb)
LegMsgEntry               *gmDELBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11DELBREFLRINDGmEntry);

   /* Populate the DELBREFLRIND GM Entry */
   gmDELBREFLRINDCb->msgType = EG_S11_DELBREFLRIND_CMD_CODE;
   gmDELBREFLRINDCb->nmbIeProp = EG_S11_DELBREFLRIND_NMB_IES;
   gmDELBREFLRINDCb->validateIe = (EscFun)NULLP;

   /* Populate DELBREFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELBREFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELBREFLRINDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,TRUE,
EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREFLRINDCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBREFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes11DELBREREQGmEntry

   Desc  : This function makes the GM Entry for DELBREREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELBREREQGmEntry
(
LegMsgEntry               *gmDELBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELBREREQGmEntry(gmDELBREREQCb)
LegMsgEntry               *gmDELBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11DELBREREQGmEntry);

   /* Populate the DELBREREQ GM Entry */
   gmDELBREREQCb->msgType = EG_S11_DELBREREQ_CMD_CODE;
   gmDELBREREQCb->nmbIeProp = EG_S11_DELBREREQ_NMB_IES;
   gmDELBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmDELBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmDELBREREQCb->posIndex[1][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_OPTIONAL,0,TRUE);

   gmDELBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmDELBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmDELBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmDELBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmDELBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,TRUE,
EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmDELBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   
   egMakeAppChildIeEntry((gmDELBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmDELBREREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELBREREQGmEntry */


/***********************************************************************

   Func  : egMakes11DELBRERSPGmEntry

   Desc  : This function makes the GM Entry for DELBRERSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELBRERSPGmEntry
(
LegMsgEntry               *gmDELBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELBRERSPGmEntry(gmDELBRERSPCb)
LegMsgEntry               *gmDELBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11DELBRERSPGmEntry);

   /* Populate the DELBRERSP GM Entry */
   gmDELBRERSPCb->msgType = EG_S11_DELBRERSP_CMD_CODE;
   gmDELBRERSPCb->nmbIeProp = EG_S11_DELBRERSP_NMB_IES;
   gmDELBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmDELBRERSPCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmDELBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmDELBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmDELBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;

   egMakeAppChildIeEntry((gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes11DELINDDATFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for DELINDDATFWDTUNREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELINDDATFWDTUNREQGmEntry
(
LegMsgEntry               *gmDELINDDATFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELINDDATFWDTUNREQGmEntry(gmDELINDDATFWDTUNREQCb)
LegMsgEntry               *gmDELINDDATFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DELINDDATFWDTUNREQGmEntry);

   /* Populate the DELINDDATFWDTUNREQ GM Entry */
   gmDELINDDATFWDTUNREQCb->msgType = EG_S11_DELINDDATFWDTUNREQ_CMD_CODE;
   gmDELINDDATFWDTUNREQCb->nmbIeProp = EG_S11_DELINDDATFWDTUNREQ_NMB_IES;
   gmDELINDDATFWDTUNREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELINDDATFWDTUNREQ IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELINDDATFWDTUNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELINDDATFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakes11DELINDDATFWDTUNRSPGmEntry

   Desc  : This function makes the GM Entry for DELINDDATFWDTUNRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELINDDATFWDTUNRSPGmEntry
(
LegMsgEntry               *gmDELINDDATFWDTUNRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELINDDATFWDTUNRSPGmEntry(gmDELINDDATFWDTUNRSPCb)
LegMsgEntry               *gmDELINDDATFWDTUNRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DELINDDATFWDTUNRSPGmEntry);

   /* Populate the DELINDDATFWDTUNRSP GM Entry */
   gmDELINDDATFWDTUNRSPCb->msgType = EG_S11_DELINDDATFWDTUNRSP_CMD_CODE;
   gmDELINDDATFWDTUNRSPCb->nmbIeProp = EG_S11_DELINDDATFWDTUNRSP_NMB_IES;
   gmDELINDDATFWDTUNRSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELINDDATFWDTUNRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELINDDATFWDTUNRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELINDDATFWDTUNRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELINDDATFWDTUNRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELINDDATFWDTUNRSPGmEntry */


/***********************************************************************

   Func  : egMakes11DELPDNCONSETREQGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELPDNCONSETREQGmEntry
(
LegMsgEntry               *gmDELPDNCONSETREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELPDNCONSETREQGmEntry(gmDELPDNCONSETREQCb)
LegMsgEntry               *gmDELPDNCONSETREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DELPDNCONSETREQGmEntry);

   /* Populate the DELPDNCONSETREQ GM Entry */
   gmDELPDNCONSETREQCb->msgType = EG_S11_DELPDNCONSETREQ_CMD_CODE;
   gmDELPDNCONSETREQCb->nmbIeProp = EG_S11_DELPDNCONSETREQ_NMB_IES;
   gmDELPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
   /*ccpu00137353 */
#if 0   
   Populate CSID IE Properties 
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmDELPDNCONSETREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif
   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakes11DELPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmDELPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELPDNCONSETRSPGmEntry(gmDELPDNCONSETRSPCb)
LegMsgEntry               *gmDELPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DELPDNCONSETRSPGmEntry);

   /* Populate the DELPDNCONSETRSP GM Entry */
   gmDELPDNCONSETRSPCb->msgType = EG_S11_DELPDNCONSETRSP_CMD_CODE;
   gmDELPDNCONSETRSPCb->nmbIeProp = EG_S11_DELPDNCONSETRSP_NMB_IES;
   gmDELPDNCONSETRSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELPDNCONSETRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELPDNCONSETRSPGmEntry */


/***********************************************************************

   Func  : egMakes11DELSESREQGmEntry

   Desc  : This function makes the GM Entry for DELSESREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELSESREQGmEntry
(
LegMsgEntry               *gmDELSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELSESREQGmEntry(gmDELSESREQCb)
LegMsgEntry               *gmDELSESREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DELSESREQGmEntry);

   /* Populate the DELSESREQ GM Entry */
   gmDELSESREQCb->msgType = EG_S11_DELSESREQ_CMD_CODE;
   gmDELSESREQCb->nmbIeProp = EG_S11_DELSESREQ_NMB_IES;
   gmDELSESREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELSESREQ IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate NODE_TYPE IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_NODE_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_NODE_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELSESREQGmEntry */


/***********************************************************************

   Func  : egMakes11DELSESRSPGmEntry

   Desc  : This function makes the GM Entry for DELSESRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DELSESRSPGmEntry
(
LegMsgEntry               *gmDELSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DELSESRSPGmEntry(gmDELSESRSPCb)
LegMsgEntry               *gmDELSESRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DELSESRSPGmEntry);

   /* Populate the DELSESRSP GM Entry */
   gmDELSESRSPCb->msgType = EG_S11_DELSESRSP_CMD_CODE;
   gmDELSESRSPCb->nmbIeProp = EG_S11_DELSESRSP_NMB_IES;
   gmDELSESRSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESRSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELSESRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DELSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes11DWLNKDATNOTGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOT

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DWLNKDATNOTGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DWLNKDATNOTGmEntry(gmDWLNKDATNOTCb)
LegMsgEntry               *gmDWLNKDATNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DWLNKDATNOTGmEntry);

   /* Populate the DWLNKDATNOT GM Entry */
   gmDWLNKDATNOTCb->msgType = EG_S11_DWLNKDATNOT_CMD_CODE;
   gmDWLNKDATNOTCb->nmbIeProp = EG_S11_DWLNKDATNOT_NMB_IES;
   gmDWLNKDATNOTCb->validateIe = (EscFun)NULLP;

   /* Populate DWLNKDATNOT IE Properties */

#ifdef EG_REL_AC0
   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_OPTIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#endif

   gmDWLNKDATNOTCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmDWLNKDATNOTCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ARP IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_ARP_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTCb->posIndex[0][EG_ARP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDWLNKDATNOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DWLNKDATNOTGmEntry */


/***********************************************************************

   Func  : egMakes11DWLNKDATNOTACKGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTACK

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DWLNKDATNOTACKGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DWLNKDATNOTACKGmEntry(gmDWLNKDATNOTACKCb)
LegMsgEntry               *gmDWLNKDATNOTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DWLNKDATNOTACKGmEntry);

   /* Populate the DWLNKDATNOTACK GM Entry */
   gmDWLNKDATNOTACKCb->msgType = EG_S11_DWLNKDATNOTACK_CMD_CODE;
   gmDWLNKDATNOTACKCb->nmbIeProp = EG_S11_DWLNKDATNOTACK_NMB_IES;
   gmDWLNKDATNOTACKCb->validateIe = (EscFun)NULLP;

   /* Populate DWLNKDATNOTACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate DELAY IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_DELAY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_DELAY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate THROTTLING IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_THROTTLING_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_THROTTLING_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DWLNKDATNOTACKGmEntry */


/***********************************************************************

   Func  : egMakes11DWLNKDATNOTFLRINDGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTFLRIND

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DWLNKDATNOTFLRINDGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DWLNKDATNOTFLRINDGmEntry(gmDWLNKDATNOTFLRINDCb)
LegMsgEntry               *gmDWLNKDATNOTFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11DWLNKDATNOTFLRINDGmEntry);

   /* Populate the DWLNKDATNOTFLRIND GM Entry */
   gmDWLNKDATNOTFLRINDCb->msgType = EG_S11_DWLNKDATNOTFLRIND_CMD_CODE;
   gmDWLNKDATNOTFLRINDCb->nmbIeProp = EG_S11_DWLNKDATNOTFLRIND_NMB_IES;
   gmDWLNKDATNOTFLRINDCb->validateIe = (EscFun)NULLP;

   /* Populate DWLNKDATNOTFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDWLNKDATNOTFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate NODE_TYPE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRINDCb->ieProperties[ieCnt],
                    EG_NODE_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTFLRINDCb->posIndex[0][EG_NODE_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRINDCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDWLNKDATNOTFLRINDCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif
   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDWLNKDATNOTFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11DWLNKDATNOTFLRINDGmEntry */

#ifdef EG_REL_AC0
/***********************************************************************

   Func  : egMakes11MODACCBREREQGmEntry

   Desc  : This function makes the GM Entry for MODACCBREREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11MODACCBREREQGmEntry
(
LegMsgEntry               *gmMODACCBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11MODACCBREREQGmEntry(gmMODACCBREREQCb)
LegMsgEntry               *gmMODACCBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11MODACCBREREQGmEntry);

   /* Populate the MODACCBREREQ GM Entry */
   gmMODACCBREREQCb->msgType = EG_S11_MODACCBREREQ_CMD_CODE;
   gmMODACCBREREQCb->nmbIeProp = EG_S11_MODACCBREREQ_NMB_IES;
   gmMODACCBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate MODACCBREREQ IE Properties */

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODACCBREREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODACCBREREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate DELAY IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_DELAY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODACCBREREQCb->posIndex[0][EG_DELAY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmMODACCBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODACCBREREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODACCBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmMODACCBREREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODACCBREREQ_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODACCBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODACCBREREQ_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODACCBREREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODACCBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODACCBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11MODACCBREREQGmEntry */


/***********************************************************************

   Func  : egMakes11MODACCBRERSPGmEntry

   Desc  : This function makes the GM Entry for MODACCBRERSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11MODACCBRERSPGmEntry
(
LegMsgEntry               *gmMODACCBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11MODACCBRERSPGmEntry(gmMODACCBRERSPCb)
LegMsgEntry               *gmMODACCBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11MODACCBRERSPGmEntry);

   /* Populate the MODACCBRERSP GM Entry */
   gmMODACCBRERSPCb->msgType = EG_S11_MODACCBRERSP_CMD_CODE;
   gmMODACCBRERSPCb->nmbIeProp = EG_S11_MODACCBRERSP_NMB_IES;
   gmMODACCBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate MODACCBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODACCBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMODACCBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODACCBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmMODACCBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODACCBRERSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODACCBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODACCBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmMODACCBRERSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODACCBRERSP_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODACCBRERSP_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODACCBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODACCBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODACCBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODACCBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODACCBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11MODACCBRERSPGmEntry */
#endif

/***********************************************************************

   Func  : egMakes11MODBRECMDGmEntry

   Desc  : This function makes the GM Entry for MODBRECMD

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11MODBRECMDGmEntry
(
LegMsgEntry               *gmMODBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11MODBRECMDGmEntry(gmMODBRECMDCb)
LegMsgEntry               *gmMODBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11MODBRECMDGmEntry);

   /* Populate the MODBRECMD GM Entry */
   gmMODBRECMDCb->msgType = EG_S11_MODBRECMD_CMD_CODE;
   gmMODBRECMDCb->nmbIeProp = EG_S11_MODBRECMD_NMB_IES;
   gmMODBRECMDCb->validateIe = (EscFun)NULLP;

   /* Populate MODBRECMD IE Properties */

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMODBRECMDCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMODBRECMDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODBRECMDCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODBRECMDCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODBRECMDCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties) ,(sizeof(LegIeProperties)*(EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBRECMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11MODBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes11MODBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for MODBREFLRIND

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11MODBREFLRINDGmEntry
(
LegMsgEntry               *gmMODBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11MODBREFLRINDGmEntry(gmMODBREFLRINDCb)
LegMsgEntry               *gmMODBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11MODBREFLRINDGmEntry);

   /* Populate the MODBREFLRIND GM Entry */
   gmMODBREFLRINDCb->msgType = EG_S11_MODBREFLRIND_CMD_CODE;
   gmMODBREFLRINDCb->nmbIeProp = EG_S11_MODBREFLRIND_NMB_IES;
   gmMODBREFLRINDCb->validateIe = (EscFun)NULLP;

   /* Populate MODBREFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMODBREFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREFLRINDCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBREFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11MODBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes11MODBREREQGmEntry

   Desc  : This function makes the GM Entry for MODBREREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11MODBREREQGmEntry
(
LegMsgEntry               *gmMODBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11MODBREREQGmEntry(gmMODBREREQCb)
LegMsgEntry               *gmMODBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11MODBREREQGmEntry);

   /* Populate the MODBREREQ GM Entry */
   gmMODBREREQCb->msgType = EG_S11_MODBREREQ_CMD_CODE;
   gmMODBREREQCb->nmbIeProp = EG_S11_MODBREREQ_NMB_IES;
   gmMODBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate MODBREREQ IE Properties */

   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate DELAY IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_DELAY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_DELAY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmMODBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmMODBREREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_OPTIONAL,0,FALSE);
#endif

   gmMODBREREQCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate UCI IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_UCI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_UCI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11MODBREREQGmEntry */


/***********************************************************************

   Func  : egMakes11MODBRERSPGmEntry

   Desc  : This function makes the GM Entry for MODBRERSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11MODBRERSPGmEntry
(
LegMsgEntry               *gmMODBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11MODBRERSPGmEntry(gmMODBRERSPCb)
LegMsgEntry               *gmMODBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11MODBRERSPGmEntry);

   /* Populate the MODBRERSP GM Entry */
   gmMODBRERSPCb->msgType = EG_S11_MODBRERSP_CMD_CODE;
   gmMODBRERSPCb->nmbIeProp = EG_S11_MODBRERSP_NMB_IES;
   gmMODBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate MODBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN_RESTRICT IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_APN_RESTRICT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_APN_RESTRICT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmMODBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,TRUE);

   gmMODBRERSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmMODBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmMODBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
}

}
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate CSG_INFO_REP_ACT IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_CSG_INFO_REP_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmMODBRERSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11MODBRERSPGmEntry */


#ifdef EG_REL_AC0
/***********************************************************************

   Func  : egMakes11PGWRSTNOTGmEntry

   Desc  : This function makes the GM Entry for PGWRSTNOT

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11PGWRSTNOTGmEntry
(
LegMsgEntry               *gmPGWRSTNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11PGWRSTNOTGmEntry(gmPGWRSTNOTCb)
LegMsgEntry               *gmPGWRSTNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11PGWRSTNOTGmEntry);

   /* Populate the PGWRSTNOT GM Entry */
   gmPGWRSTNOTCb->msgType = EG_S11_PGWRSTNOT_CMD_CODE;
   gmPGWRSTNOTCb->nmbIeProp = EG_S11_PGWRSTNOT_NMB_IES;
   gmPGWRSTNOTCb->validateIe = (EscFun)NULLP;

   /* Populate PGWRSTNOT IE Properties */

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmPGWRSTNOTCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmPGWRSTNOTCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmPGWRSTNOTCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_MANDATORY,1,FALSE);

   gmPGWRSTNOTCb->posIndex[1][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmPGWRSTNOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmPGWRSTNOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11PGWRSTNOTGmEntry */


/***********************************************************************

   Func  : egMakes11PGWRSTNOTACKGmEntry

   Desc  : This function makes the GM Entry for PGWRSTNOTACK

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11PGWRSTNOTACKGmEntry
(
LegMsgEntry               *gmPGWRSTNOTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11PGWRSTNOTACKGmEntry(gmPGWRSTNOTACKCb)
LegMsgEntry               *gmPGWRSTNOTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11PGWRSTNOTACKGmEntry);

   /* Populate the PGWRSTNOTACK GM Entry */
   gmPGWRSTNOTACKCb->msgType = EG_S11_PGWRSTNOTACK_CMD_CODE;
   gmPGWRSTNOTACKCb->nmbIeProp = EG_S11_PGWRSTNOTACK_NMB_IES;
   gmPGWRSTNOTACKCb->validateIe = (EscFun)NULLP;

   /* Populate PGWRSTNOTACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmPGWRSTNOTACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmPGWRSTNOTACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmPGWRSTNOTACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmPGWRSTNOTACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11PGWRSTNOTACKGmEntry */
#endif


/***********************************************************************

   Func  : egMakes11RELACCBREREQGmEntry

   Desc  : This function makes the GM Entry for RELACCBREREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11RELACCBREREQGmEntry
(
LegMsgEntry               *gmRELACCBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11RELACCBREREQGmEntry(gmRELACCBREREQCb)
LegMsgEntry               *gmRELACCBREREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11RELACCBREREQGmEntry);

   /* Populate the RELACCBREREQ GM Entry */
   gmRELACCBREREQCb->msgType = EG_S11_RELACCBREREQ_CMD_CODE;
   gmRELACCBREREQCb->nmbIeProp = EG_S11_RELACCBREREQ_NMB_IES;
   gmRELACCBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate RELACCBREREQ IE Properties */

#ifdef EG_REL_AC0
   /* Populate NODE_TYPE IE Properties */
   egMakeAppIeProp(&gmRELACCBREREQCb->ieProperties[ieCnt],
                    EG_NODE_TYPE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRELACCBREREQCb->posIndex[0][EG_NODE_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRELACCBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRELACCBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11RELACCBREREQGmEntry */


/***********************************************************************

   Func  : egMakes11RELACCBRERSPGmEntry

   Desc  : This function makes the GM Entry for RELACCBRERSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11RELACCBRERSPGmEntry
(
LegMsgEntry               *gmRELACCBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11RELACCBRERSPGmEntry(gmRELACCBRERSPCb)
LegMsgEntry               *gmRELACCBRERSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11RELACCBRERSPGmEntry);

   /* Populate the RELACCBRERSP GM Entry */
   gmRELACCBRERSPCb->msgType = EG_S11_RELACCBRERSP_CMD_CODE;
   gmRELACCBRERSPCb->nmbIeProp = EG_S11_RELACCBRERSP_NMB_IES;
   gmRELACCBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate RELACCBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmRELACCBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRELACCBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmRELACCBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRELACCBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRELACCBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRELACCBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11RELACCBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes11RESUMEACKGmEntry

   Desc  : This function makes the GM Entry for RESUMEACK

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11RESUMEACKGmEntry
(
LegMsgEntry               *gmRESUMEACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11RESUMEACKGmEntry(gmRESUMEACKCb)
LegMsgEntry               *gmRESUMEACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11RESUMEACKGmEntry);

   /* Populate the RESUMEACK GM Entry */
   gmRESUMEACKCb->msgType = EG_S11_RESUMEACK_CMD_CODE;
   gmRESUMEACKCb->nmbIeProp = EG_S11_RESUMEACK_NMB_IES;
   gmRESUMEACKCb->validateIe = (EscFun)NULLP;

   /* Populate RESUMEACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmRESUMEACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRESUMEACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRESUMEACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRESUMEACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11RESUMEACKGmEntry */


/***********************************************************************

   Func  : egMakes11RESUMENOTGmEntry

   Desc  : This function makes the GM Entry for RESUMENOT

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11RESUMENOTGmEntry
(
LegMsgEntry               *gmRESUMENOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11RESUMENOTGmEntry(gmRESUMENOTCb)
LegMsgEntry               *gmRESUMENOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11RESUMENOTGmEntry);

   /* Populate the RESUMENOT GM Entry */
   gmRESUMENOTCb->msgType = EG_S11_RESUMENOT_CMD_CODE;
   gmRESUMENOTCb->nmbIeProp = EG_S11_RESUMENOT_NMB_IES;
   gmRESUMENOTCb->validateIe = (EscFun)NULLP;

   /* Populate RESUMENOT IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRESUMENOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmRESUMENOTCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRESUMENOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11RESUMENOTGmEntry */


/***********************************************************************

   Func  : egMakes11STOPPAGINGINDGmEntry

   Desc  : This function makes the GM Entry for STOPPAGINGIND

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11STOPPAGINGINDGmEntry
(
LegMsgEntry               *gmSTOPPAGINGINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11STOPPAGINGINDGmEntry(gmSTOPPAGINGINDCb)
LegMsgEntry               *gmSTOPPAGINGINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11STOPPAGINGINDGmEntry);

   /* Populate the STOPPAGINGIND GM Entry */
   gmSTOPPAGINGINDCb->msgType = EG_S11_STOPPAGINGIND_CMD_CODE;
   gmSTOPPAGINGINDCb->nmbIeProp = EG_S11_STOPPAGINGIND_NMB_IES;
   gmSTOPPAGINGINDCb->validateIe = (EscFun)NULLP;

   /* Populate STOPPAGINGIND IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSTOPPAGINGINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSTOPPAGINGINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSTOPPAGINGINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSTOPPAGINGINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11STOPPAGINGINDGmEntry */


/***********************************************************************

   Func  : egMakes11SUSPENDACKGmEntry

   Desc  : This function makes the GM Entry for SUSPENDACK

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11SUSPENDACKGmEntry
(
LegMsgEntry               *gmSUSPENDACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11SUSPENDACKGmEntry(gmSUSPENDACKCb)
LegMsgEntry               *gmSUSPENDACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11SUSPENDACKGmEntry);

   /* Populate the SUSPENDACK GM Entry */
   gmSUSPENDACKCb->msgType = EG_S11_SUSPENDACK_CMD_CODE;
   gmSUSPENDACKCb->nmbIeProp = EG_S11_SUSPENDACK_NMB_IES;
   gmSUSPENDACKCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPENDACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmSUSPENDACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmSUSPENDACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPENDACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPENDACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11SUSPENDACKGmEntry */


/***********************************************************************

   Func  : egMakes11SUSPENDNOTGmEntry

   Desc  : This function makes the GM Entry for SUSPENDNOT

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11SUSPENDNOTGmEntry
(
LegMsgEntry               *gmSUSPENDNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11SUSPENDNOTGmEntry(gmSUSPENDNOTCb)
LegMsgEntry               *gmSUSPENDNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11SUSPENDNOTGmEntry);

   /* Populate the SUSPENDNOT GM Entry */
   gmSUSPENDNOTCb->msgType = EG_S11_SUSPENDNOT_CMD_CODE;
   gmSUSPENDNOTCb->nmbIeProp = EG_S11_SUSPENDNOT_NMB_IES;
   gmSUSPENDNOTCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPENDNOT IE Properties */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif

   gmSUSPENDNOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmSUSPENDNOTCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#endif
   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPENDNOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11SUSPENDNOTGmEntry */


/***********************************************************************

   Func  : egMakes11TRACESESSACTVGmEntry

   Desc  : This function makes the GM Entry for TRACESESSACTV

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11TRACESESSACTVGmEntry
(
LegMsgEntry               *gmTRACESESSACTVCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11TRACESESSACTVGmEntry(gmTRACESESSACTVCb)
LegMsgEntry               *gmTRACESESSACTVCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11TRACESESSACTVGmEntry);

   /* Populate the TRACESESSACTV GM Entry */
   gmTRACESESSACTVCb->msgType = EG_S11_TRACESESSACTV_CMD_CODE;
   gmTRACESESSACTVCb->nmbIeProp = EG_S11_TRACESESSACTV_NMB_IES;
   gmTRACESESSACTVCb->validateIe = (EscFun)NULLP;

   /* Populate TRACESESSACTV IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmTRACESESSACTVCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmTRACESESSACTVCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmTRACESESSACTVCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmTRACESESSACTVCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmTRACESESSACTVCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmTRACESESSACTVCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11TRACESESSACTVGmEntry */


/***********************************************************************

   Func  : egMakes11TRACESESSDEACTVGmEntry

   Desc  : This function makes the GM Entry for TRACESESSDEACTV

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11TRACESESSDEACTVGmEntry
(
LegMsgEntry               *gmTRACESESSDEACTVCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11TRACESESSDEACTVGmEntry(gmTRACESESSDEACTVCb)
LegMsgEntry               *gmTRACESESSDEACTVCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes11TRACESESSDEACTVGmEntry);

   /* Populate the TRACESESSDEACTV GM Entry */
   gmTRACESESSDEACTVCb->msgType = EG_S11_TRACESESSDEACTV_CMD_CODE;
   gmTRACESESSDEACTVCb->nmbIeProp = EG_S11_TRACESESSDEACTV_NMB_IES;
   gmTRACESESSDEACTVCb->validateIe = (EscFun)NULLP;

   /* Populate TRACESESSDEACTV IE Properties */

   /* Populate TRACE_REF IE Properties */
   egMakeAppIeProp(&gmTRACESESSDEACTVCb->ieProperties[ieCnt],
                    EG_TRACE_REF_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmTRACESESSDEACTVCb->posIndex[0][EG_TRACE_REF_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11TRACESESSDEACTVGmEntry */


/***********************************************************************

   Func  : egMakes11UPDBREREQGmEntry

   Desc  : This function makes the GM Entry for UPDBREREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11UPDBREREQGmEntry
(
LegMsgEntry               *gmUPDBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11UPDBREREQGmEntry(gmUPDBREREQCb)
LegMsgEntry               *gmUPDBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11UPDBREREQGmEntry);

   /* Populate the UPDBREREQ GM Entry */
   gmUPDBREREQCb->msgType = EG_S11_UPDBREREQ_CMD_CODE;
   gmUPDBREREQCb->nmbIeProp = EG_S11_UPDBREREQ_NMB_IES;
   gmUPDBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate UPDBREREQ IE Properties */

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmUPDBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmUPDBREREQCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmUPDBREREQCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif
}

}
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate CSG_INFO_REP_ACT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_CSG_INFO_REP_ACT_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_CSG_INFO_REP_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmUPDBREREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11UPDBREREQGmEntry */


/***********************************************************************

   Func  : egMakes11UPDBRERSPGmEntry

   Desc  : This function makes the GM Entry for UPDBRERSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11UPDBRERSPGmEntry
(
LegMsgEntry               *gmUPDBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11UPDBRERSPGmEntry(gmUPDBRERSPCb)
LegMsgEntry               *gmUPDBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes11UPDBRERSPGmEntry);

   /* Populate the UPDBRERSP GM Entry */
   gmUPDBRERSPCb->msgType = EG_S11_UPDBRERSP_CMD_CODE;
   gmUPDBRERSPCb->nmbIeProp = EG_S11_UPDBRERSP_NMB_IES;
   gmUPDBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate UPDBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,TRUE);

   gmUPDBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && *ieProperties != NULLP)
      {

   egMakeAppChildIeEntry((gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   grpCnt++;
   egMakeAppChildIeEntry((gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif
}

}
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmUPDBRERSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes11UPDBRERSPGmEntry */
/***********************************************************************

   Func  : egMakes11GmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegs11.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakes11GmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakes11GmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakes11GmDict);

   /* Populating BRERESCMD GM Entry */
   egMakes11BRERESCMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating BRERESFLRIND GM Entry */
   egMakes11BRERESFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHGNTFREQ GM Entry */
   egMakes11CHGNTFREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHGNTFRSP GM Entry */
   egMakes11CHGNTFRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBREREQ GM Entry */
   egMakes11CRTBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBRERSP GM Entry */
   egMakes11CRTBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTFWDTUNREQ GM Entry */
   egMakes11CRTFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTFWDTUNRSP GM Entry */
   egMakes11CRTFWDTUNRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTINDDATFWDTUNREQ GM Entry */
   egMakes11CRTINDDATFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTINDDATFWDTUNRSP GM Entry */
   egMakes11CRTINDDATFWDTUNRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESREQ GM Entry */
   egMakes11CRTSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESRSP GM Entry */
   egMakes11CRTSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRECMD GM Entry */
   egMakes11DELBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREFLRIND GM Entry */
   egMakes11DELBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREREQ GM Entry */
   egMakes11DELBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRERSP GM Entry */
   egMakes11DELBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELINDDATFWDTUNREQ GM Entry */
   egMakes11DELINDDATFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELINDDATFWDTUNRSP GM Entry */
   egMakes11DELINDDATFWDTUNRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETREQ GM Entry */
   egMakes11DELPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETRSP GM Entry */
   egMakes11DELPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESREQ GM Entry */
   egMakes11DELSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESRSP GM Entry */
   egMakes11DELSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOT GM Entry */
   egMakes11DWLNKDATNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOTACK GM Entry */
   egMakes11DWLNKDATNOTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOTFLRIND GM Entry */
   egMakes11DWLNKDATNOTFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   /* Populating MODACCBREREQ GM Entry */
   egMakes11MODACCBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODACCBRERSP GM Entry */
   egMakes11MODACCBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif

   /* Populating MODBRECMD GM Entry */
   egMakes11MODBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREFLRIND GM Entry */
   egMakes11MODBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREREQ GM Entry */
   egMakes11MODBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBRERSP GM Entry */
   egMakes11MODBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   /* Populating PGWRSTNOT GM Entry */
   egMakes11PGWRSTNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating PGWRSTNOTACK GM Entry */
   egMakes11PGWRSTNOTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif

   /* Populating RELACCBREREQ GM Entry */
   egMakes11RELACCBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELACCBRERSP GM Entry */
   egMakes11RELACCBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMEACK GM Entry */
   egMakes11RESUMEACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMENOT GM Entry */
   egMakes11RESUMENOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating STOPPAGINGIND GM Entry */
   egMakes11STOPPAGINGINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDACK GM Entry */
   egMakes11SUSPENDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDNOT GM Entry */
   egMakes11SUSPENDNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRACESESSACTV GM Entry */
   egMakes11TRACESESSACTVGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRACESESSDEACTV GM Entry */
   egMakes11TRACESESSDEACTVGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBREREQ GM Entry */
   egMakes11UPDBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBRERSP GM Entry */
   egMakes11UPDBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes11GmDict */




#endif /* End of EGTP_C */

#ifdef EGTP_U
/***********************************************************************

   Func  : egUMakeIeDict

   Desc  : This function populates the IEs in IE Dictionary of GTP-U

   Ret   : EG_ROK/EG_RFAILED

   File  : eg_dbs11.c

************************************************************************/
#ifdef ANSI
PUBLIC S16 egUMakeIeDict
(
EgIeEntry *ieEntry,
U16      *posIdx
)
#else
PUBLIC S16 egUMakeIeDict(ieEntry,posIdx)
EgIeEntry *ieEntry;
U16      *posIdx;
#endif
{
   U8  ieCount = 1;            /* Number of IEs inserted */

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   TRC2(egUMakeIeDict)
   cmMemset((U8*)posIdx, 0 , EGU_MAX_IE);

   /********************************************************
    * First Entry of ieEntry is not used i.e ieEntry[0]    *
    ********************************************************/
   /* Populating EGU_RECOVERY_IETYPE */
   egUMakeIeEntry(&ieEntry[ieCount], 
                    EGU_RECOVERY_IETYPE,
                    TRUE , EG_UNSIGNED_8, 1, 1);
   posIdx[EGU_RECOVERY_IETYPE] = ieCount; 
   ieCount++;

   /* Populating EGU_TEID_IETYPE */
   egUMakeIeEntry(&ieEntry[ieCount], 
                    EGU_TEID_IETYPE,
                    TRUE , EG_UNSIGNED_32, 4, 4);
   posIdx[EGU_TEID_IETYPE] = ieCount; 
   ieCount++;

   /* Populating EGU_GSN_ADDR_IETYPE */
   egUMakeIeEntry(&ieEntry[ieCount], 
                    EGU_GSN_ADDR_IETYPE,
                    FALSE , EG_IP_ADDR, 4, 16);
   posIdx[EGU_GSN_ADDR_IETYPE] = ieCount; 
   ieCount++;

   /* Populating EGU_EXT_HDR_LST_IETYPE */
   egUMakeIeEntry(&ieEntry[ieCount], 
                    EGU_EXT_HDR_LST_IETYPE,
                    FALSE , EG_EXT_HDR_TYPE_LST, 1, 2);
   posIdx[EGU_EXT_HDR_LST_IETYPE] = ieCount; 
   ieCount++;

   /* Populating EGU_PVT_EXTN_IETYPE */
   egUMakeIeEntry(&ieEntry[ieCount], 
                    EGU_PVT_EXTN_IETYPE,
                    FALSE , EG_PVT_EXTN, 3, 134);
   posIdx[EGU_PVT_EXTN_IETYPE] = ieCount; 
   ieCount++;

   if(ieCount <= EGU_SUP_IES)
      RETVALUE(ROK);
   else
      RETVALUE(RFAILED);
}  /* End of egUMakeIeDeict() */


/***********************************************************************

   Func  : egUMakeGmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : eg_dbs11.c

************************************************************************/

#ifdef ANSI
PUBLIC S16 egUMakeGmDict
(
EgUMsgEntry *gmEntry,
U16       *posIdx
)
#else
PUBLIC S16 egUMakeGmDict(gmEntry, posIdx)
EgUMsgEntry *gmEntry;
U16       *posIdx;
#endif
{
   /********************************************************
    * First Entry of gmEntry is not used i.e gmEntry[0]    *
    ********************************************************/
   U8  gmMsgCount = 1;            /* Number of eGTP Messages inserted */

   TRC2(egUMakeGmDict);

   /* Populating ECHO-REQ GM Entry */
   egUMakeECHOREQGmEntry(&gmEntry[gmMsgCount]);
   posIdx[EGU_ECHO_REQ] = gmMsgCount; 
   gmMsgCount++;

   /* Populating ECHO-RSP GM Entry */
   egUMakeECHORSPGmEntry(&gmEntry[gmMsgCount]);
   posIdx[EGU_ECHO_RSP] = gmMsgCount; 
   gmMsgCount++;

   /* Populating ERROR IND GM Entry */
   egUMakeERRORINDGmEntry(&gmEntry[gmMsgCount]);
   posIdx[EGU_ERROR_IND] = gmMsgCount; 
   gmMsgCount++;

   /* Populating SUPP_EXT_HDR_NTF GM Entry */
   egUMakeSUPPEXTHDRNTFGmEntry(&gmEntry[gmMsgCount]);
   posIdx[EGU_SUPP_EXT_HDR_NTF] = gmMsgCount; 
   gmMsgCount++;

   /* Populating END-MARKER GM Entry */
   egUMakeENDMARKGmEntry(&gmEntry[gmMsgCount]);
   posIdx[EGU_END_MARKER] = gmMsgCount; 
   gmMsgCount++;

   if(gmMsgCount <= EGU_SUP_MSGS)
      RETVALUE(ROK);
   else
      RETVALUE(RFAILED);
} /* End of geMakes11GmDict */

/***********************************************************************

   Func  : egUMakeECHOREQGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : eg_dbs11.c

************************************************************************/
#ifdef ANSI
PUBLIC void egUMakeECHOREQGmEntry
(
EgUMsgEntry               *gmCb                         /* GM Control Block */
)
#else
PUBLIC void egUMakeECHOREQGmEntry(gmCb)
EgUMsgEntry               *gmCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egUMakeECHOREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCb->msgType = EGU_ECHO_REQ;
   gmCb->nmbMandIe = 0;

   /* Populate PRIVATE_EXT IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_PVT_EXTN_IETYPE, EG_IE_OPTIONAL);
   ieCnt++;

   gmCb->nmbIe = ieCnt;
   /*-- eg003.201 : merge changes from eg006.102 --*/
   RETVOID;
   
} /* End of egUMakeECHOREQGmEntry */

/***********************************************************************

   Func  : egUMakeECHORSPGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : eg_dbs11.c

************************************************************************/
#ifdef ANSI
PUBLIC void egUMakeECHORSPGmEntry
(
EgUMsgEntry               *gmCb                         /* GM Control Block */
)
#else
PUBLIC void egUMakeECHORSPGmEntry(gmCb)
EgUMsgEntry               *gmCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egUMakeECHORSPGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCb->msgType = EGU_ECHO_RSP;
   gmCb->nmbMandIe = 0;

   /* Populate PRIVATE_EXT IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_RECOVERY_IETYPE, EG_IE_MANDATORY);
   ieCnt++;
   gmCb->nmbMandIe ++;

   /* Populate PRIVATE_EXT IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_PVT_EXTN_IETYPE, EG_IE_OPTIONAL);
   ieCnt++;

   gmCb->nmbIe = ieCnt;
   /*-- eg003.201 : merge changes from eg006.102 --*/
   RETVOID;
   
} /* End of egUMakeECHORSPGmEntry */

/***********************************************************************

   Func  : egUMakeERRORINDGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : eg_dbs11.c

************************************************************************/
#ifdef ANSI
PUBLIC void egUMakeERRORINDGmEntry
(
EgUMsgEntry               *gmCb                         /* GM Control Block */
)
#else
PUBLIC void egUMakeERRORINDGmEntry(gmCb)
EgUMsgEntry               *gmCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egUMakeERRORINDGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCb->msgType = EGU_ERROR_IND;
   gmCb->nmbMandIe = 0;

   /* Populate TEID IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_TEID_IETYPE, EG_IE_MANDATORY);
   ieCnt++;
   gmCb->nmbMandIe ++;

   /* Populate GSN Peer Addr IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_GSN_ADDR_IETYPE, EG_IE_MANDATORY);
   ieCnt++;
   gmCb->nmbMandIe ++;

   /* Populate PRIVATE_EXT IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_PVT_EXTN_IETYPE, EG_IE_OPTIONAL);
   ieCnt++;

   gmCb->nmbIe = ieCnt;
   /*-- eg003.201 : merge changes from eg006.102 --*/
   RETVOID;
   
} /* End of egUMakeERRORINDGmEntry */

/***********************************************************************

   Func  : egUMakeSUPPEXTHDRNTFGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : eg_dbs11.c

************************************************************************/
#ifdef ANSI
PUBLIC void egUMakeSUPPEXTHDRNTFGmEntry
(
EgUMsgEntry               *gmCb                         /* GM Control Block */
)
#else
PUBLIC void egUMakeSUPPEXTHDRNTFGmEntry(gmCb)
EgUMsgEntry               *gmCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egUMakeSUPPEXTHDRNTFGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCb->msgType = EGU_SUPP_EXT_HDR_NTF;
   gmCb->nmbMandIe = 0;

   /* Populate TEID IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_EXT_HDR_LST_IETYPE, EG_IE_MANDATORY);
   ieCnt++;
   gmCb->nmbMandIe ++;

   gmCb->nmbIe = ieCnt;
   /*-- eg003.201 : merge changes from eg006.102 --*/
   RETVOID;
   
} /* End of egUMakeSUPPEXTHDRNTFGmEntry */

/***********************************************************************

   Func  : egUMakeENDMARKGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : eg_dbs11.c

************************************************************************/
#ifdef ANSI
PUBLIC void egUMakeENDMARKGmEntry
(
EgUMsgEntry               *gmCb                         /* GM Control Block */
)
#else
PUBLIC void egUMakeENDMARKGmEntry(gmCb)
EgUMsgEntry               *gmCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egUMakeENDMARKGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCb->msgType = EGU_END_MARKER;
   gmCb->nmbMandIe = 0;

   /* Populate PRIVATE_EXT IE Properties */
   egUMakeAppIeProp(&gmCb->ieProperties[ieCnt],
                    EGU_PVT_EXTN_IETYPE, EG_IE_OPTIONAL);
   ieCnt++;

   gmCb->nmbIe = ieCnt;
   /*-- eg003.201 : merge changes from eg006.102 --*/
   RETVOID;
   
} /* End of egUMakeENDMARKGmEntry */


#endif  /* End of EGTP_U */


/********************************************************************30**

         End of file:     eg_dbs11.c@@/main/3 - Fri Feb  4 18:18:03 2011

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
/main/1      ---      an                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya        1. Initial for eGTP 1.2 Release
            eg004.102 rrm               1. BEARER_CNTX inst 1 is optional
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg003.201 psingh            1. Merged code from eg006.102
/main/3     eg012.201 shpandey          1. Modify bearer response fix  for multiple bearer contexts with 
                                           Same instance, ccpu00130004
                                        2. Multipe bearer context IEs with instance-1 should be 
                   allowed in modify bearer request message,fix for ccpu00130411 
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

