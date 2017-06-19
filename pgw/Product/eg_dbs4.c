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

    File: eg_dbs4.c

    Sid:      eg_dbs4.c@@/main/2 - Fri Feb  4 18:18:05 2011

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
#include "eg_dbs4.h"
#include "eg_dbutl.h"   /* Ie codes are here */

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
#include "eg_dbs4.x"


#ifdef EGTP_C


/***********************************************************************

   Func  : egMakes4BRERESCMDGmEntry

   Desc  : This function makes the GM Entry for BRERESCMD

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4BRERESCMDGmEntry
(
LegMsgEntry               *gmBRERESCMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4BRERESCMDGmEntry(gmBRERESCMDCb)
LegMsgEntry               *gmBRERESCMDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4BRERESCMDGmEntry);

   /* Populate the BRERESCMD GM Entry */
   gmBRERESCMDCb->msgType = EG_S4_BRERESCMD_CMD_CODE;
   gmBRERESCMDCb->nmbIeProp = EG_S4_BRERESCMD_NMB_IES;
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
                    EG_TAD_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmBRERESCMDCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmBRERESCMDCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes4BRERESCMDGmEntry */


/***********************************************************************

   Func  : egMakes4BRERESFLRINDGmEntry

   Desc  : This function makes the GM Entry for BRERESFLRIND

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4BRERESFLRINDGmEntry
(
LegMsgEntry               *gmBRERESFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4BRERESFLRINDGmEntry(gmBRERESFLRINDCb)
LegMsgEntry               *gmBRERESFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4BRERESFLRINDGmEntry);

   /* Populate the BRERESFLRIND GM Entry */
   gmBRERESFLRINDCb->msgType = EG_S4_BRERESFLRIND_CMD_CODE;
   gmBRERESFLRINDCb->nmbIeProp = EG_S4_BRERESFLRIND_NMB_IES;
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

} /* End of egMakes4BRERESFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes4CHGNTFREQGmEntry

   Desc  : This function makes the GM Entry for CHGNTFREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CHGNTFREQGmEntry
(
LegMsgEntry               *gmCHGNTFREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CHGNTFREQGmEntry(gmCHGNTFREQCb)
LegMsgEntry               *gmCHGNTFREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4CHGNTFREQGmEntry);

   /* Populate the CHGNTFREQ GM Entry */
   gmCHGNTFREQCb->msgType = EG_S4_CHGNTFREQ_CMD_CODE;
   gmCHGNTFREQCb->nmbIeProp = EG_S4_CHGNTFREQ_NMB_IES;
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

   RETVOID;

} /* End of egMakes4CHGNTFREQGmEntry */


/***********************************************************************

   Func  : egMakes4CHGNTFRSPGmEntry

   Desc  : This function makes the GM Entry for CHGNTFRSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CHGNTFRSPGmEntry
(
LegMsgEntry               *gmCHGNTFRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CHGNTFRSPGmEntry(gmCHGNTFRSPCb)
LegMsgEntry               *gmCHGNTFRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4CHGNTFRSPGmEntry);

   /* Populate the CHGNTFRSP GM Entry */
   gmCHGNTFRSPCb->msgType = EG_S4_CHGNTFRSP_CMD_CODE;
   gmCHGNTFRSPCb->nmbIeProp = EG_S4_CHGNTFRSP_NMB_IES;
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
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCHGNTFRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes4CHGNTFRSPGmEntry */


/***********************************************************************

   Func  : egMakes4CRTBREREQGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CRTBREREQGmEntry
(
LegMsgEntry               *gmCRTBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CRTBREREQGmEntry(gmCRTBREREQCb)
LegMsgEntry               *gmCRTBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4CRTBREREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCRTBREREQCb->msgType = EG_S4_CRTBREREQ_CMD_CODE;
   gmCRTBREREQCb->nmbIeProp = EG_S4_CRTBREREQ_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_ID_IETYPE,EG_IE_OPTIONAL,0,FALSE,
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

} /* End of egMakes4CRTBREREQGmEntry */


/***********************************************************************

   Func  : egMakes4CRTBRERSPGmEntry

   Desc  : This function makes the GM Entry for CRTBRERSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CRTBRERSPGmEntry
(
LegMsgEntry               *gmCRTBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CRTBRERSPGmEntry(gmCRTBRERSPCb)
LegMsgEntry               *gmCRTBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4CRTBRERSPGmEntry);

   /* Populate the CRTBRERSP GM Entry */
   gmCRTBRERSPCb->msgType = EG_S4_CRTBRERSP_CMD_CODE;
   gmCRTBRERSPCb->nmbIeProp = EG_S4_CRTBRERSP_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,6,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,7,FALSE,
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

} /* End of egMakes4CRTBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes4CRTINDDATFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for CRTINDDATFWDTUNREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CRTINDDATFWDTUNREQGmEntry
(
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CRTINDDATFWDTUNREQGmEntry(gmCRTINDDATFWDTUNREQCb)
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4CRTINDDATFWDTUNREQGmEntry);

   /* Populate the CRTINDDATFWDTUNREQ GM Entry */
   gmCRTINDDATFWDTUNREQCb->msgType = EG_S4_CRTINDDATFWDTUNREQ_CMD_CODE;
   gmCRTINDDATFWDTUNREQCb->nmbIeProp = EG_S4_CRTINDDATFWDTUNREQ_NMB_IES;
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

} /* End of egMakes4CRTINDDATFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakes4CRTINDDATFWDTUNRSPGmEntry

   Desc  : This function makes the GM Entry for CRTINDDATFWDTUNRSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CRTINDDATFWDTUNRSPGmEntry
(
LegMsgEntry               *gmCRTINDDATFWDTUNRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CRTINDDATFWDTUNRSPGmEntry(gmCRTINDDATFWDTUNRSPCb)
LegMsgEntry               *gmCRTINDDATFWDTUNRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4CRTINDDATFWDTUNRSPGmEntry);

   /* Populate the CRTINDDATFWDTUNRSP GM Entry */
   gmCRTINDDATFWDTUNRSPCb->msgType = EG_S4_CRTINDDATFWDTUNRSP_CMD_CODE;
   gmCRTINDDATFWDTUNRSPCb->nmbIeProp = EG_S4_CRTINDDATFWDTUNRSP_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
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

} /* End of egMakes4CRTINDDATFWDTUNRSPGmEntry */


/***********************************************************************

   Func  : egMakes4CRTSESREQGmEntry

   Desc  : This function makes the GM Entry for CRTSESREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CRTSESREQGmEntry
(
LegMsgEntry               *gmCRTSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CRTSESREQGmEntry(gmCRTSESREQCb)
LegMsgEntry               *gmCRTSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4CRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_S4_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_S4_CRTSESREQ_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
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
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

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

} /* End of egMakes4CRTSESREQGmEntry */


/***********************************************************************

   Func  : egMakes4CRTSESRSPGmEntry

   Desc  : This function makes the GM Entry for CRTSESRSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4CRTSESRSPGmEntry
(
LegMsgEntry               *gmCRTSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4CRTSESRSPGmEntry(gmCRTSESRSPCb)
LegMsgEntry               *gmCRTSESRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4CRTSESRSPGmEntry);

   /* Populate the CRTSESRSP GM Entry */
   gmCRTSESRSPCb->msgType = EG_S4_CRTSESRSP_CMD_CODE;
   gmCRTSESRSPCb->nmbIeProp = EG_S4_CRTSESRSP_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_ID_IETYPE,EG_IE_OPTIONAL,0,FALSE,
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

} /* End of egMakes4CRTSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes4DELBRECMDGmEntry

   Desc  : This function makes the GM Entry for DELBRECMD

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELBRECMDGmEntry
(
LegMsgEntry               *gmDELBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELBRECMDGmEntry(gmDELBRECMDCb)
LegMsgEntry               *gmDELBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4DELBRECMDGmEntry);

   /* Populate the DELBRECMD GM Entry */
   gmDELBRECMDCb->msgType = EG_S4_DELBRECMD_CMD_CODE;
   gmDELBRECMDCb->nmbIeProp = EG_S4_DELBRECMD_NMB_IES;
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

} /* End of egMakes4DELBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes4DELBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for DELBREFLRIND

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELBREFLRINDGmEntry
(
LegMsgEntry               *gmDELBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELBREFLRINDGmEntry(gmDELBREFLRINDCb)
LegMsgEntry               *gmDELBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4DELBREFLRINDGmEntry);

   /* Populate the DELBREFLRIND GM Entry */
   gmDELBREFLRINDCb->msgType = EG_S4_DELBREFLRIND_CMD_CODE;
   gmDELBREFLRINDCb->nmbIeProp = EG_S4_DELBREFLRIND_NMB_IES;
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

   egMakeAppChildIeEntry((gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
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

} /* End of egMakes4DELBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes4DELBREREQGmEntry

   Desc  : This function makes the GM Entry for DELBREREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELBREREQGmEntry
(
LegMsgEntry               *gmDELBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELBREREQGmEntry(gmDELBREREQCb)
LegMsgEntry               *gmDELBREREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DELBREREQGmEntry);

   /* Populate the DELBREREQ GM Entry */
   gmDELBREREQCb->msgType = EG_S4_DELBREREQ_CMD_CODE;
   gmDELBREREQCb->nmbIeProp = EG_S4_DELBREREQ_NMB_IES;
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

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes4DELBREREQGmEntry */


/***********************************************************************

   Func  : egMakes4DELBRERSPGmEntry

   Desc  : This function makes the GM Entry for DELBRERSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELBRERSPGmEntry
(
LegMsgEntry               *gmDELBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELBRERSPGmEntry(gmDELBRERSPCb)
LegMsgEntry               *gmDELBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4DELBRERSPGmEntry);

   /* Populate the DELBRERSP GM Entry */
   gmDELBRERSPCb->msgType = EG_S4_DELBRERSP_CMD_CODE;
   gmDELBRERSPCb->nmbIeProp = EG_S4_DELBRERSP_NMB_IES;
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
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
#endif
}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes4DELBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes4DELINDDATFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for DELINDDATFWDTUNREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELINDDATFWDTUNREQGmEntry
(
LegMsgEntry               *gmDELINDDATFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELINDDATFWDTUNREQGmEntry(gmDELINDDATFWDTUNREQCb)
LegMsgEntry               *gmDELINDDATFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DELINDDATFWDTUNREQGmEntry);

   /* Populate the DELINDDATFWDTUNREQ GM Entry */
   gmDELINDDATFWDTUNREQCb->msgType = EG_S4_DELINDDATFWDTUNREQ_CMD_CODE;
   gmDELINDDATFWDTUNREQCb->nmbIeProp = EG_S4_DELINDDATFWDTUNREQ_NMB_IES;
   gmDELINDDATFWDTUNREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELINDDATFWDTUNREQ IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELINDDATFWDTUNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes4DELINDDATFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakes4DELINDDATFWDTUNRSPGmEntry

   Desc  : This function makes the GM Entry for DELINDDATFWDTUNRSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELINDDATFWDTUNRSPGmEntry
(
LegMsgEntry               *gmDELINDDATFWDTUNRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELINDDATFWDTUNRSPGmEntry(gmDELINDDATFWDTUNRSPCb)
LegMsgEntry               *gmDELINDDATFWDTUNRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DELINDDATFWDTUNRSPGmEntry);

   /* Populate the DELINDDATFWDTUNRSP GM Entry */
   gmDELINDDATFWDTUNRSPCb->msgType = EG_S4_DELINDDATFWDTUNRSP_CMD_CODE;
   gmDELINDDATFWDTUNRSPCb->nmbIeProp = EG_S4_DELINDDATFWDTUNRSP_NMB_IES;
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

} /* End of egMakes4DELINDDATFWDTUNRSPGmEntry */


/***********************************************************************

   Func  : egMakes4DELSESREQGmEntry

   Desc  : This function makes the GM Entry for DELSESREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELSESREQGmEntry
(
LegMsgEntry               *gmDELSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELSESREQGmEntry(gmDELSESREQCb)
LegMsgEntry               *gmDELSESREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DELSESREQGmEntry);

   /* Populate the DELSESREQ GM Entry */
   gmDELSESREQCb->msgType = EG_S4_DELSESREQ_CMD_CODE;
   gmDELSESREQCb->nmbIeProp = EG_S4_DELSESREQ_NMB_IES;
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

} /* End of egMakes4DELSESREQGmEntry */


/***********************************************************************

   Func  : egMakes4DELSESRSPGmEntry

   Desc  : This function makes the GM Entry for DELSESRSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DELSESRSPGmEntry
(
LegMsgEntry               *gmDELSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DELSESRSPGmEntry(gmDELSESRSPCb)
LegMsgEntry               *gmDELSESRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DELSESRSPGmEntry);

   /* Populate the DELSESRSP GM Entry */
   gmDELSESRSPCb->msgType = EG_S4_DELSESRSP_CMD_CODE;
   gmDELSESRSPCb->nmbIeProp = EG_S4_DELSESRSP_NMB_IES;
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

} /* End of egMakes4DELSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes4DWLNKDATNOTGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOT

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DWLNKDATNOTGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DWLNKDATNOTGmEntry(gmDWLNKDATNOTCb)
LegMsgEntry               *gmDWLNKDATNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DWLNKDATNOTGmEntry);

   /* Populate the DWLNKDATNOT GM Entry */
   gmDWLNKDATNOTCb->msgType = EG_S4_DWLNKDATNOT_CMD_CODE;
   gmDWLNKDATNOTCb->nmbIeProp = EG_S4_DWLNKDATNOT_NMB_IES;
   gmDWLNKDATNOTCb->validateIe = (EscFun)NULLP;

   /* Populate DWLNKDATNOT IE Properties */

#ifdef EG_REL_AC0
   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_OPTIONAL,0,FALSE);
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

} /* End of egMakes4DWLNKDATNOTGmEntry */


/***********************************************************************

   Func  : egMakes4DWLNKDATNOTACKGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTACK

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DWLNKDATNOTACKGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DWLNKDATNOTACKGmEntry(gmDWLNKDATNOTACKCb)
LegMsgEntry               *gmDWLNKDATNOTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DWLNKDATNOTACKGmEntry);

   /* Populate the DWLNKDATNOTACK GM Entry */
   gmDWLNKDATNOTACKCb->msgType = EG_S4_DWLNKDATNOTACK_CMD_CODE;
   gmDWLNKDATNOTACKCb->nmbIeProp = EG_S4_DWLNKDATNOTACK_NMB_IES;
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

} /* End of egMakes4DWLNKDATNOTACKGmEntry */


/***********************************************************************

   Func  : egMakes4DWLNKDATNOTFLRINDGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTFLRIND

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4DWLNKDATNOTFLRINDGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4DWLNKDATNOTFLRINDGmEntry(gmDWLNKDATNOTFLRINDCb)
LegMsgEntry               *gmDWLNKDATNOTFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4DWLNKDATNOTFLRINDGmEntry);

   /* Populate the DWLNKDATNOTFLRIND GM Entry */
   gmDWLNKDATNOTFLRINDCb->msgType = EG_S4_DWLNKDATNOTFLRIND_CMD_CODE;
   gmDWLNKDATNOTFLRINDCb->nmbIeProp = EG_S4_DWLNKDATNOTFLRIND_NMB_IES;
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

} /* End of egMakes4DWLNKDATNOTFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes4MODBRECMDGmEntry

   Desc  : This function makes the GM Entry for MODBRECMD

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4MODBRECMDGmEntry
(
LegMsgEntry               *gmMODBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4MODBRECMDGmEntry(gmMODBRECMDCb)
LegMsgEntry               *gmMODBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4MODBRECMDGmEntry);

   /* Populate the MODBRECMD GM Entry */
   gmMODBRECMDCb->msgType = EG_S4_MODBRECMD_CMD_CODE;
   gmMODBRECMDCb->nmbIeProp = EG_S4_MODBRECMD_NMB_IES;
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
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES )));
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

} /* End of egMakes4MODBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes4MODBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for MODBREFLRIND

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4MODBREFLRINDGmEntry
(
LegMsgEntry               *gmMODBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4MODBREFLRINDGmEntry(gmMODBREFLRINDCb)
LegMsgEntry               *gmMODBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4MODBREFLRINDGmEntry);

   /* Populate the MODBREFLRIND GM Entry */
   gmMODBREFLRINDCb->msgType = EG_S4_MODBREFLRIND_CMD_CODE;
   gmMODBREFLRINDCb->nmbIeProp = EG_S4_MODBREFLRIND_NMB_IES;
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

} /* End of egMakes4MODBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes4MODBREREQGmEntry

   Desc  : This function makes the GM Entry for MODBREREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4MODBREREQGmEntry
(
LegMsgEntry               *gmMODBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4MODBREREQGmEntry(gmMODBREREQCb)
LegMsgEntry               *gmMODBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4MODBREREQGmEntry);

   /* Populate the MODBREREQ GM Entry */
   gmMODBREREQCb->msgType = EG_S4_MODBREREQ_CMD_CODE;
   gmMODBREREQCb->nmbIeProp = EG_S4_MODBREREQ_NMB_IES;
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

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
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
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
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

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes4MODBREREQGmEntry */


/***********************************************************************

   Func  : egMakes4MODBRERSPGmEntry

   Desc  : This function makes the GM Entry for MODBRERSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4MODBRERSPGmEntry
(
LegMsgEntry               *gmMODBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4MODBRERSPGmEntry(gmMODBRERSPCb)
LegMsgEntry               *gmMODBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4MODBRERSPGmEntry);

   /* Populate the MODBRERSP GM Entry */
   gmMODBRERSPCb->msgType = EG_S4_MODBRERSP_CMD_CODE;
   gmMODBRERSPCb->nmbIeProp = EG_S4_MODBRERSP_NMB_IES;
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
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_ID_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
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

} /* End of egMakes4MODBRERSPGmEntry */


#ifdef EG_REL_AC0
/***********************************************************************

   Func  : egMakes4PGWRSTNOTGmEntry

   Desc  : This function makes the GM Entry for PGWRSTNOT

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4PGWRSTNOTGmEntry
(
LegMsgEntry               *gmPGWRSTNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4PGWRSTNOTGmEntry(gmPGWRSTNOTCb)
LegMsgEntry               *gmPGWRSTNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4PGWRSTNOTGmEntry);

   /* Populate the PGWRSTNOT GM Entry */
   gmPGWRSTNOTCb->msgType = EG_S4_PGWRSTNOT_CMD_CODE;
   gmPGWRSTNOTCb->nmbIeProp = EG_S4_PGWRSTNOT_NMB_IES;
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

} /* End of egMakes4PGWRSTNOTGmEntry */


/***********************************************************************

   Func  : egMakes4PGWRSTNOTACKGmEntry

   Desc  : This function makes the GM Entry for PGWRSTNOTACK

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4PGWRSTNOTACKGmEntry
(
LegMsgEntry               *gmPGWRSTNOTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4PGWRSTNOTACKGmEntry(gmPGWRSTNOTACKCb)
LegMsgEntry               *gmPGWRSTNOTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4PGWRSTNOTACKGmEntry);

   /* Populate the PGWRSTNOTACK GM Entry */
   gmPGWRSTNOTACKCb->msgType = EG_S4_PGWRSTNOTACK_CMD_CODE;
   gmPGWRSTNOTACKCb->nmbIeProp = EG_S4_PGWRSTNOTACK_NMB_IES;
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

} /* End of egMakes4PGWRSTNOTACKGmEntry */
#endif


/***********************************************************************

   Func  : egMakes4RELACCBREREQGmEntry

   Desc  : This function makes the GM Entry for RELACCBREREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4RELACCBREREQGmEntry
(
LegMsgEntry               *gmRELACCBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4RELACCBREREQGmEntry(gmRELACCBREREQCb)
LegMsgEntry               *gmRELACCBREREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4RELACCBREREQGmEntry);

   /* Populate the RELACCBREREQ GM Entry */
   gmRELACCBREREQCb->msgType = EG_S4_RELACCBREREQ_CMD_CODE;
   gmRELACCBREREQCb->nmbIeProp = EG_S4_RELACCBREREQ_NMB_IES;
   gmRELACCBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate RELACCBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmRELACCBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,TRUE);

   gmRELACCBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

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

} /* End of egMakes4RELACCBREREQGmEntry */


/***********************************************************************

   Func  : egMakes4RELACCBRERSPGmEntry

   Desc  : This function makes the GM Entry for RELACCBRERSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4RELACCBRERSPGmEntry
(
LegMsgEntry               *gmRELACCBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4RELACCBRERSPGmEntry(gmRELACCBRERSPCb)
LegMsgEntry               *gmRELACCBRERSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4RELACCBRERSPGmEntry);

   /* Populate the RELACCBRERSP GM Entry */
   gmRELACCBRERSPCb->msgType = EG_S4_RELACCBRERSP_CMD_CODE;
   gmRELACCBRERSPCb->nmbIeProp = EG_S4_RELACCBRERSP_NMB_IES;
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

} /* End of egMakes4RELACCBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes4RESUMEACKGmEntry

   Desc  : This function makes the GM Entry for RESUMEACK

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4RESUMEACKGmEntry
(
LegMsgEntry               *gmRESUMEACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4RESUMEACKGmEntry(gmRESUMEACKCb)
LegMsgEntry               *gmRESUMEACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4RESUMEACKGmEntry);

   /* Populate the RESUMEACK GM Entry */
   gmRESUMEACKCb->msgType = EG_S4_RESUMEACK_CMD_CODE;
   gmRESUMEACKCb->nmbIeProp = EG_S4_RESUMEACK_NMB_IES;
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

} /* End of egMakes4RESUMEACKGmEntry */


/***********************************************************************

   Func  : egMakes4RESUMENOTGmEntry

   Desc  : This function makes the GM Entry for RESUMENOT

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4RESUMENOTGmEntry
(
LegMsgEntry               *gmRESUMENOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4RESUMENOTGmEntry(gmRESUMENOTCb)
LegMsgEntry               *gmRESUMENOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4RESUMENOTGmEntry);

   /* Populate the RESUMENOT GM Entry */
   gmRESUMENOTCb->msgType = EG_S4_RESUMENOT_CMD_CODE;
   gmRESUMENOTCb->nmbIeProp = EG_S4_RESUMENOT_NMB_IES;
   gmRESUMENOTCb->validateIe = (EscFun)NULLP;

   /* Populate RESUMENOT IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRESUMENOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
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

} /* End of egMakes4RESUMENOTGmEntry */


/***********************************************************************

   Func  : egMakes4STOPPAGINGINDGmEntry

   Desc  : This function makes the GM Entry for STOPPAGINGIND

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4STOPPAGINGINDGmEntry
(
LegMsgEntry               *gmSTOPPAGINGINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4STOPPAGINGINDGmEntry(gmSTOPPAGINGINDCb)
LegMsgEntry               *gmSTOPPAGINGINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4STOPPAGINGINDGmEntry);

   /* Populate the STOPPAGINGIND GM Entry */
   gmSTOPPAGINGINDCb->msgType = EG_S4_STOPPAGINGIND_CMD_CODE;
   gmSTOPPAGINGINDCb->nmbIeProp = EG_S4_STOPPAGINGIND_NMB_IES;
   gmSTOPPAGINGINDCb->validateIe = (EscFun)NULLP;

   /* Populate STOPPAGINGIND IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSTOPPAGINGINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSTOPPAGINGINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes4STOPPAGINGINDGmEntry */


/***********************************************************************

   Func  : egMakes4SUSPENDACKGmEntry

   Desc  : This function makes the GM Entry for SUSPENDACK

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4SUSPENDACKGmEntry
(
LegMsgEntry               *gmSUSPENDACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4SUSPENDACKGmEntry(gmSUSPENDACKCb)
LegMsgEntry               *gmSUSPENDACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4SUSPENDACKGmEntry);

   /* Populate the SUSPENDACK GM Entry */
   gmSUSPENDACKCb->msgType = EG_S4_SUSPENDACK_CMD_CODE;
   gmSUSPENDACKCb->nmbIeProp = EG_S4_SUSPENDACK_NMB_IES;
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

} /* End of egMakes4SUSPENDACKGmEntry */


/***********************************************************************

   Func  : egMakes4SUSPENDNOTGmEntry

   Desc  : This function makes the GM Entry for SUSPENDNOT

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4SUSPENDNOTGmEntry
(
LegMsgEntry               *gmSUSPENDNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4SUSPENDNOTGmEntry(gmSUSPENDNOTCb)
LegMsgEntry               *gmSUSPENDNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4SUSPENDNOTGmEntry);

   /* Populate the SUSPENDNOT GM Entry */
   gmSUSPENDNOTCb->msgType = EG_S4_SUSPENDNOT_CMD_CODE;
   gmSUSPENDNOTCb->nmbIeProp = EG_S4_SUSPENDNOT_NMB_IES;
   gmSUSPENDNOTCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPENDNOT IE Properties */

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

} /* End of egMakes4SUSPENDNOTGmEntry */


/***********************************************************************

   Func  : egMakes4TRACESESSACTVGmEntry

   Desc  : This function makes the GM Entry for TRACESESSACTV

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4TRACESESSACTVGmEntry
(
LegMsgEntry               *gmTRACESESSACTVCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4TRACESESSACTVGmEntry(gmTRACESESSACTVCb)
LegMsgEntry               *gmTRACESESSACTVCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4TRACESESSACTVGmEntry);

   /* Populate the TRACESESSACTV GM Entry */
   gmTRACESESSACTVCb->msgType = EG_S4_TRACESESSACTV_CMD_CODE;
   gmTRACESESSACTVCb->nmbIeProp = EG_S4_TRACESESSACTV_NMB_IES;
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

} /* End of egMakes4TRACESESSACTVGmEntry */


/***********************************************************************

   Func  : egMakes4TRACESESSDEACTVGmEntry

   Desc  : This function makes the GM Entry for TRACESESSDEACTV

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4TRACESESSDEACTVGmEntry
(
LegMsgEntry               *gmTRACESESSDEACTVCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4TRACESESSDEACTVGmEntry(gmTRACESESSDEACTVCb)
LegMsgEntry               *gmTRACESESSDEACTVCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes4TRACESESSDEACTVGmEntry);

   /* Populate the TRACESESSDEACTV GM Entry */
   gmTRACESESSDEACTVCb->msgType = EG_S4_TRACESESSDEACTV_CMD_CODE;
   gmTRACESESSDEACTVCb->nmbIeProp = EG_S4_TRACESESSDEACTV_NMB_IES;
   gmTRACESESSDEACTVCb->validateIe = (EscFun)NULLP;

   /* Populate TRACESESSDEACTV IE Properties */

   /* Populate TRACE_REF IE Properties */
   egMakeAppIeProp(&gmTRACESESSDEACTVCb->ieProperties[ieCnt],
                    EG_TRACE_REF_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmTRACESESSDEACTVCb->posIndex[0][EG_TRACE_REF_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes4TRACESESSDEACTVGmEntry */


/***********************************************************************

   Func  : egMakes4UPDBREREQGmEntry

   Desc  : This function makes the GM Entry for UPDBREREQ

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4UPDBREREQGmEntry
(
LegMsgEntry               *gmUPDBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4UPDBREREQGmEntry(gmUPDBREREQCb)
LegMsgEntry               *gmUPDBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4UPDBREREQGmEntry);

   /* Populate the UPDBREREQ GM Entry */
   gmUPDBREREQCb->msgType = EG_S4_UPDBREREQ_CMD_CODE;
   gmUPDBREREQCb->nmbIeProp = EG_S4_UPDBREREQ_NMB_IES;
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

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes4UPDBREREQGmEntry */


/***********************************************************************

   Func  : egMakes4UPDBRERSPGmEntry

   Desc  : This function makes the GM Entry for UPDBRERSP

   Ret   : None

   File  : smegs4.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes4UPDBRERSPGmEntry
(
LegMsgEntry               *gmUPDBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes4UPDBRERSPGmEntry(gmUPDBRERSPCb)
LegMsgEntry               *gmUPDBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes4UPDBRERSPGmEntry);

   /* Populate the UPDBRERSP GM Entry */
   gmUPDBRERSPCb->msgType = EG_S4_UPDBRERSP_CMD_CODE;
   gmUPDBRERSPCb->nmbIeProp = EG_S4_UPDBRERSP_NMB_IES;
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
   egMakeAppChildIeEntry((gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
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

} /* End of egMakes4UPDBRERSPGmEntry */
/***********************************************************************

   Func  : egMakes4GmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegs4.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakes4GmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakes4GmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakes4GmDict);

   /* Populating BRERESCMD GM Entry */
   egMakes4BRERESCMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating BRERESFLRIND GM Entry */
   egMakes4BRERESFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHGNTFREQ GM Entry */
   egMakes4CHGNTFREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHGNTFRSP GM Entry */
   egMakes4CHGNTFRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBREREQ GM Entry */
   egMakes4CRTBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBRERSP GM Entry */
   egMakes4CRTBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTINDDATFWDTUNREQ GM Entry */
   egMakes4CRTINDDATFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTINDDATFWDTUNRSP GM Entry */
   egMakes4CRTINDDATFWDTUNRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESREQ GM Entry */
   egMakes4CRTSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESRSP GM Entry */
   egMakes4CRTSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRECMD GM Entry */
   egMakes4DELBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREFLRIND GM Entry */
   egMakes4DELBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREREQ GM Entry */
   egMakes4DELBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRERSP GM Entry */
   egMakes4DELBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELINDDATFWDTUNREQ GM Entry */
   egMakes4DELINDDATFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELINDDATFWDTUNRSP GM Entry */
   egMakes4DELINDDATFWDTUNRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESREQ GM Entry */
   egMakes4DELSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESRSP GM Entry */
   egMakes4DELSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOT GM Entry */
   egMakes4DWLNKDATNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOTACK GM Entry */
   egMakes4DWLNKDATNOTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOTFLRIND GM Entry */
   egMakes4DWLNKDATNOTFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBRECMD GM Entry */
   egMakes4MODBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREFLRIND GM Entry */
   egMakes4MODBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREREQ GM Entry */
   egMakes4MODBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBRERSP GM Entry */
   egMakes4MODBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   /* Populating PGWRSTNOT GM Entry */
   egMakes4PGWRSTNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating PGWRSTNOTACK GM Entry */
   egMakes4PGWRSTNOTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif

   /* Populating RELACCBREREQ GM Entry */
   egMakes4RELACCBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELACCBRERSP GM Entry */
   egMakes4RELACCBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMEACK GM Entry */
   egMakes4RESUMEACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMENOT GM Entry */
   egMakes4RESUMENOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating STOPPAGINGIND GM Entry */
   egMakes4STOPPAGINGINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDACK GM Entry */
   egMakes4SUSPENDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDNOT GM Entry */
   egMakes4SUSPENDNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRACESESSACTV GM Entry */
   egMakes4TRACESESSACTVGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRACESESSDEACTV GM Entry */
   egMakes4TRACESESSDEACTVGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBREREQ GM Entry */
   egMakes4UPDBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBRERSP GM Entry */
   egMakes4UPDBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes4GmDict */


#endif
/********************************************************************30**

         End of file:     eg_dbs4.c@@/main/2 - Fri Feb  4 18:18:05 2011

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
/main/2      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/2     eg003.201 psingh            1. Merged code from eg006.102 to Fix 
                                           TRACE5 macro related issue
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

