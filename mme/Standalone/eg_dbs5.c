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

    File: eg_dbs5.c

    Sid:      eg_dbs5.c@@/main/3 - Fri Feb  4 18:18:06 2011

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
#include "eg_dbs5.h"
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
#include "eg_dbs5.x"

#ifdef EGTP_C


/***********************************************************************

   Func  : egMakes5BRERESCMDGmEntry

   Desc  : This function makes the GM Entry for BRERESCMD

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5BRERESCMDGmEntry
(
LegMsgEntry               *gmBRERESCMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5BRERESCMDGmEntry(gmBRERESCMDCb)
LegMsgEntry               *gmBRERESCMDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5BRERESCMDGmEntry);

   /* Populate the BRERESCMD GM Entry */
   gmBRERESCMDCb->msgType = EG_S5_BRERESCMD_CMD_CODE;
   gmBRERESCMDCb->nmbIeProp = EG_S5_BRERESCMD_NMB_IES;
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

} /* End of egMakes5BRERESCMDGmEntry */


/***********************************************************************

   Func  : egMakes5BRERESFLRINDGmEntry

   Desc  : This function makes the GM Entry for BRERESFLRIND

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5BRERESFLRINDGmEntry
(
LegMsgEntry               *gmBRERESFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5BRERESFLRINDGmEntry(gmBRERESFLRINDCb)
LegMsgEntry               *gmBRERESFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5BRERESFLRINDGmEntry);

   /* Populate the BRERESFLRIND GM Entry */
   gmBRERESFLRINDCb->msgType = EG_S5_BRERESFLRIND_CMD_CODE;
   gmBRERESFLRINDCb->nmbIeProp = EG_S5_BRERESFLRIND_NMB_IES;
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

} /* End of egMakes5BRERESFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes5CHGNTFREQGmEntry

   Desc  : This function makes the GM Entry for CHGNTFREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5CHGNTFREQGmEntry
(
LegMsgEntry               *gmCHGNTFREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5CHGNTFREQGmEntry(gmCHGNTFREQCb)
LegMsgEntry               *gmCHGNTFREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5CHGNTFREQGmEntry);

   /* Populate the CHGNTFREQ GM Entry */
   gmCHGNTFREQCb->msgType = EG_S5_CHGNTFREQ_CMD_CODE;
   gmCHGNTFREQCb->nmbIeProp = EG_S5_CHGNTFREQ_NMB_IES;
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

} /* End of egMakes5CHGNTFREQGmEntry */


/***********************************************************************

   Func  : egMakes5CHGNTFRSPGmEntry

   Desc  : This function makes the GM Entry for CHGNTFRSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5CHGNTFRSPGmEntry
(
LegMsgEntry               *gmCHGNTFRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5CHGNTFRSPGmEntry(gmCHGNTFRSPCb)
LegMsgEntry               *gmCHGNTFRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5CHGNTFRSPGmEntry);

   /* Populate the CHGNTFRSP GM Entry */
   gmCHGNTFRSPCb->msgType = EG_S5_CHGNTFRSP_CMD_CODE;
   gmCHGNTFRSPCb->nmbIeProp = EG_S5_CHGNTFRSP_NMB_IES;
   gmCHGNTFRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CHGNTFRSP IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCHGNTFRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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

} /* End of egMakes5CHGNTFRSPGmEntry */


/***********************************************************************

   Func  : egMakes5CRTBREREQGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5CRTBREREQGmEntry
(
LegMsgEntry               *gmCRTBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5CRTBREREQGmEntry(gmCRTBREREQCb)
LegMsgEntry               *gmCRTBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;

   TRC2(egMakes5CRTBREREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCRTBREREQCb->msgType = EG_S5_CRTBREREQ_CMD_CODE;
   gmCRTBREREQCb->nmbIeProp = EG_S5_CRTBREREQ_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
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

} /* End of egMakes5CRTBREREQGmEntry */


/***********************************************************************

   Func  : egMakes5CRTBRERSPGmEntry

   Desc  : This function makes the GM Entry for CRTBRERSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5CRTBRERSPGmEntry
(
LegMsgEntry               *gmCRTBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5CRTBRERSPGmEntry(gmCRTBRERSPCb)
LegMsgEntry               *gmCRTBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5CRTBRERSPGmEntry);

   /* Populate the CRTBRERSP GM Entry */
   gmCRTBRERSPCb->msgType = EG_S5_CRTBRERSP_CMD_CODE;
   gmCRTBRERSPCb->nmbIeProp = EG_S5_CRTBRERSP_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
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
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5CRTBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes5CRTSESREQGmEntry

   Desc  : This function makes the GM Entry for CRTSESREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5CRTSESREQGmEntry
(
LegMsgEntry               *gmCRTSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5CRTSESREQGmEntry(gmCRTSESREQCb)
LegMsgEntry               *gmCRTSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5CRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_S5_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_S5_CRTSESREQ_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
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

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
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
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_LDN_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes5CRTSESREQGmEntry */


/***********************************************************************

   Func  : egMakes5CRTSESRSPGmEntry

   Desc  : This function makes the GM Entry for CRTSESRSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5CRTSESRSPGmEntry
(
LegMsgEntry               *gmCRTSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5CRTSESRSPGmEntry(gmCRTSESRSPCb)
LegMsgEntry               *gmCRTSESRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5CRTSESRSPGmEntry);

   /* Populate the CRTSESRSP GM Entry */
   gmCRTSESRSPCb->msgType = EG_S5_CRTSESRSP_CMD_CODE;
   gmCRTSESRSPCb->nmbIeProp = EG_S5_CRTSESRSP_NMB_IES;
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

#ifndef EG_REL_AC0 
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt++;
#endif

   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
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

   /* Populate FQDN IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_FQDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_FQDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate IPADDR IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_IPADDR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_IPADDR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,1,FALSE);

   gmCRTSESRSPCb->posIndex[1][EG_LDN_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes5CRTSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes5DELBRECMDGmEntry

   Desc  : This function makes the GM Entry for DELBRECMD

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELBRECMDGmEntry
(
LegMsgEntry               *gmDELBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELBRECMDGmEntry(gmDELBRECMDCb)
LegMsgEntry               *gmDELBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5DELBRECMDGmEntry);

   /* Populate the DELBRECMD GM Entry */
   gmDELBRECMDCb->msgType = EG_S5_DELBRECMD_CMD_CODE;
   gmDELBRECMDCb->nmbIeProp = EG_S5_DELBRECMD_NMB_IES;
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

} /* End of egMakes5DELBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes5DELBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for DELBREFLRIND

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELBREFLRINDGmEntry
(
LegMsgEntry               *gmDELBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELBREFLRINDGmEntry(gmDELBREFLRINDCb)
LegMsgEntry               *gmDELBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5DELBREFLRINDGmEntry);

   /* Populate the DELBREFLRIND GM Entry */
   gmDELBREFLRINDCb->msgType = EG_S5_DELBREFLRIND_CMD_CODE;
   gmDELBREFLRINDCb->nmbIeProp = EG_S5_DELBREFLRIND_NMB_IES;
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

} /* End of egMakes5DELBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes5DELBREREQGmEntry

   Desc  : This function makes the GM Entry for DELBREREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELBREREQGmEntry
(
LegMsgEntry               *gmDELBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELBREREQGmEntry(gmDELBREREQCb)
LegMsgEntry               *gmDELBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5DELBREREQGmEntry);

   /* Populate the DELBREREQ GM Entry */
   gmDELBREREQCb->msgType = EG_S5_DELBREREQ_CMD_CODE;
   gmDELBREREQCb->nmbIeProp = EG_S5_DELBREREQ_NMB_IES;
   gmDELBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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

#ifdef EG_REL_AC0
   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_OPTIONAL,0,FALSE);
#endif

   gmDELBREREQCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5DELBREREQGmEntry */


/***********************************************************************

   Func  : egMakes5DELBRERSPGmEntry

   Desc  : This function makes the GM Entry for DELBRERSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELBRERSPGmEntry
(
LegMsgEntry               *gmDELBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELBRERSPGmEntry(gmDELBRERSPCb)
LegMsgEntry               *gmDELBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5DELBRERSPGmEntry);

   /* Populate the DELBRERSP GM Entry */
   gmDELBRERSPCb->msgType = EG_S5_DELBRERSP_CMD_CODE;
   gmDELBRERSPCb->nmbIeProp = EG_S5_DELBRERSP_NMB_IES;
   gmDELBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmDELBRERSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5DELBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes5DELPDNCONSETREQGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELPDNCONSETREQGmEntry
(
LegMsgEntry               *gmDELPDNCONSETREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELPDNCONSETREQGmEntry(gmDELPDNCONSETREQCb)
LegMsgEntry               *gmDELPDNCONSETREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5DELPDNCONSETREQGmEntry);

   /* Populate the DELPDNCONSETREQ GM Entry */
   gmDELPDNCONSETREQCb->msgType = EG_S5_DELPDNCONSETREQ_CMD_CODE;
   gmDELPDNCONSETREQCb->nmbIeProp = EG_S5_DELPDNCONSETREQ_NMB_IES;
   gmDELPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmDELPDNCONSETREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmDELPDNCONSETREQCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5DELPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakes5DELPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETRSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmDELPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELPDNCONSETRSPGmEntry(gmDELPDNCONSETRSPCb)
LegMsgEntry               *gmDELPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5DELPDNCONSETRSPGmEntry);

   /* Populate the DELPDNCONSETRSP GM Entry */
   gmDELPDNCONSETRSPCb->msgType = EG_S5_DELPDNCONSETRSP_CMD_CODE;
   gmDELPDNCONSETRSPCb->nmbIeProp = EG_S5_DELPDNCONSETRSP_NMB_IES;
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

} /* End of egMakes5DELPDNCONSETRSPGmEntry */


/***********************************************************************

   Func  : egMakes5DELSESREQGmEntry

   Desc  : This function makes the GM Entry for DELSESREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELSESREQGmEntry
(
LegMsgEntry               *gmDELSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELSESREQGmEntry(gmDELSESREQCb)
LegMsgEntry               *gmDELSESREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5DELSESREQGmEntry);

   /* Populate the DELSESREQ GM Entry */
   gmDELSESREQCb->msgType = EG_S5_DELSESREQ_CMD_CODE;
   gmDELSESREQCb->nmbIeProp = EG_S5_DELSESREQ_NMB_IES;
   gmDELSESREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELSESREQ IE Properties */

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

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
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

} /* End of egMakes5DELSESREQGmEntry */


/***********************************************************************

   Func  : egMakes5DELSESRSPGmEntry

   Desc  : This function makes the GM Entry for DELSESRSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5DELSESRSPGmEntry
(
LegMsgEntry               *gmDELSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5DELSESRSPGmEntry(gmDELSESRSPCb)
LegMsgEntry               *gmDELSESRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5DELSESRSPGmEntry);

   /* Populate the DELSESRSP GM Entry */
   gmDELSESRSPCb->msgType = EG_S5_DELSESRSP_CMD_CODE;
   gmDELSESRSPCb->nmbIeProp = EG_S5_DELSESRSP_NMB_IES;
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

} /* End of egMakes5DELSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes5MODBRECMDGmEntry

   Desc  : This function makes the GM Entry for MODBRECMD

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5MODBRECMDGmEntry
(
LegMsgEntry               *gmMODBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5MODBRECMDGmEntry(gmMODBRECMDCb)
LegMsgEntry               *gmMODBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5MODBRECMDGmEntry);

   /* Populate the MODBRECMD GM Entry */
   gmMODBRECMDCb->msgType = EG_S5_MODBRECMD_CMD_CODE;
   gmMODBRECMDCb->nmbIeProp = EG_S5_MODBRECMD_NMB_IES;
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

} /* End of egMakes5MODBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes5MODBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for MODBREFLRIND

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5MODBREFLRINDGmEntry
(
LegMsgEntry               *gmMODBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5MODBREFLRINDGmEntry(gmMODBREFLRINDCb)
LegMsgEntry               *gmMODBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5MODBREFLRINDGmEntry);

   /* Populate the MODBREFLRIND GM Entry */
   gmMODBREFLRINDCb->msgType = EG_S5_MODBREFLRIND_CMD_CODE;
   gmMODBREFLRINDCb->nmbIeProp = EG_S5_MODBREFLRIND_NMB_IES;
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

} /* End of egMakes5MODBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes5MODBREREQGmEntry

   Desc  : This function makes the GM Entry for MODBREREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5MODBREREQGmEntry
(
LegMsgEntry               *gmMODBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5MODBREREQGmEntry(gmMODBREREQCb)
LegMsgEntry               *gmMODBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5MODBREREQGmEntry);

   /* Populate the MODBREREQ GM Entry */
   gmMODBREREQCb->msgType = EG_S5_MODBREREQ_CMD_CODE;
   gmMODBREREQCb->nmbIeProp = EG_S5_MODBREREQ_NMB_IES;
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
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

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

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmMODBREREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
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
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,1,FALSE);

   gmMODBREREQCb->posIndex[1][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5MODBREREQGmEntry */


/***********************************************************************

   Func  : egMakes5MODBRERSPGmEntry

   Desc  : This function makes the GM Entry for MODBRERSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5MODBRERSPGmEntry
(
LegMsgEntry               *gmMODBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5MODBRERSPGmEntry(gmMODBRERSPCb)
LegMsgEntry               *gmMODBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5MODBRERSPGmEntry);

   /* Populate the MODBRERSP GM Entry */
   gmMODBRERSPCb->msgType = EG_S5_MODBRERSP_CMD_CODE;
   gmMODBRERSPCb->nmbIeProp = EG_S5_MODBRERSP_NMB_IES;
   gmMODBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate MODBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
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
   egMakeAppChildIeEntry((gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
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

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,1,FALSE);

   gmMODBRERSPCb->posIndex[1][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5MODBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes5RESUMEACKGmEntry

   Desc  : This function makes the GM Entry for RESUMEACK

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5RESUMEACKGmEntry
(
LegMsgEntry               *gmRESUMEACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5RESUMEACKGmEntry(gmRESUMEACKCb)
LegMsgEntry               *gmRESUMEACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5RESUMEACKGmEntry);

   /* Populate the RESUMEACK GM Entry */
   gmRESUMEACKCb->msgType = EG_S5_RESUMEACK_CMD_CODE;
   gmRESUMEACKCb->nmbIeProp = EG_S5_RESUMEACK_NMB_IES;
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

} /* End of egMakes5RESUMEACKGmEntry */


/***********************************************************************

   Func  : egMakes5RESUMENOTGmEntry

   Desc  : This function makes the GM Entry for RESUMENOT

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5RESUMENOTGmEntry
(
LegMsgEntry               *gmRESUMENOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5RESUMENOTGmEntry(gmRESUMENOTCb)
LegMsgEntry               *gmRESUMENOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5RESUMENOTGmEntry);

   /* Populate the RESUMENOT GM Entry */
   gmRESUMENOTCb->msgType = EG_S5_RESUMENOT_CMD_CODE;
   gmRESUMENOTCb->nmbIeProp = EG_S5_RESUMENOT_NMB_IES;
   gmRESUMENOTCb->validateIe = (EscFun)NULLP;

   /* Populate RESUMENOT IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmRESUMENOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmRESUMENOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5RESUMENOTGmEntry */


/***********************************************************************

   Func  : egMakes5SUSPENDACKGmEntry

   Desc  : This function makes the GM Entry for SUSPENDACK

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5SUSPENDACKGmEntry
(
LegMsgEntry               *gmSUSPENDACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5SUSPENDACKGmEntry(gmSUSPENDACKCb)
LegMsgEntry               *gmSUSPENDACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5SUSPENDACKGmEntry);

   /* Populate the SUSPENDACK GM Entry */
   gmSUSPENDACKCb->msgType = EG_S5_SUSPENDACK_CMD_CODE;
   gmSUSPENDACKCb->nmbIeProp = EG_S5_SUSPENDACK_NMB_IES;
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

} /* End of egMakes5SUSPENDACKGmEntry */


/***********************************************************************

   Func  : egMakes5SUSPENDNOTGmEntry

   Desc  : This function makes the GM Entry for SUSPENDNOT

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5SUSPENDNOTGmEntry
(
LegMsgEntry               *gmSUSPENDNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5SUSPENDNOTGmEntry(gmSUSPENDNOTCb)
LegMsgEntry               *gmSUSPENDNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5SUSPENDNOTGmEntry);

   /* Populate the SUSPENDNOT GM Entry */
   gmSUSPENDNOTCb->msgType = EG_S5_SUSPENDNOT_CMD_CODE;
   gmSUSPENDNOTCb->nmbIeProp = EG_S5_SUSPENDNOT_NMB_IES;
   gmSUSPENDNOTCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPENDNOT IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPENDNOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5SUSPENDNOTGmEntry */


/***********************************************************************

   Func  : egMakes5TRACESESSACTVGmEntry

   Desc  : This function makes the GM Entry for TRACESESSACTV

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5TRACESESSACTVGmEntry
(
LegMsgEntry               *gmTRACESESSACTVCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5TRACESESSACTVGmEntry(gmTRACESESSACTVCb)
LegMsgEntry               *gmTRACESESSACTVCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5TRACESESSACTVGmEntry);

   /* Populate the TRACESESSACTV GM Entry */
   gmTRACESESSACTVCb->msgType = EG_S5_TRACESESSACTV_CMD_CODE;
   gmTRACESESSACTVCb->nmbIeProp = EG_S5_TRACESESSACTV_NMB_IES;
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

} /* End of egMakes5TRACESESSACTVGmEntry */


/***********************************************************************

   Func  : egMakes5TRACESESSDEACTVGmEntry

   Desc  : This function makes the GM Entry for TRACESESSDEACTV

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5TRACESESSDEACTVGmEntry
(
LegMsgEntry               *gmTRACESESSDEACTVCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5TRACESESSDEACTVGmEntry(gmTRACESESSDEACTVCb)
LegMsgEntry               *gmTRACESESSDEACTVCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5TRACESESSDEACTVGmEntry);

   /* Populate the TRACESESSDEACTV GM Entry */
   gmTRACESESSDEACTVCb->msgType = EG_S5_TRACESESSDEACTV_CMD_CODE;
   gmTRACESESSDEACTVCb->nmbIeProp = EG_S5_TRACESESSDEACTV_NMB_IES;
   gmTRACESESSDEACTVCb->validateIe = (EscFun)NULLP;

   /* Populate TRACESESSDEACTV IE Properties */

   /* Populate TRACE_REF IE Properties */
   egMakeAppIeProp(&gmTRACESESSDEACTVCb->ieProperties[ieCnt],
                    EG_TRACE_REF_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmTRACESESSDEACTVCb->posIndex[0][EG_TRACE_REF_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5TRACESESSDEACTVGmEntry */


/***********************************************************************

   Func  : egMakes5UPDBREREQGmEntry

   Desc  : This function makes the GM Entry for UPDBREREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5UPDBREREQGmEntry
(
LegMsgEntry               *gmUPDBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5UPDBREREQGmEntry(gmUPDBREREQCb)
LegMsgEntry               *gmUPDBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;

   TRC2(egMakes5UPDBREREQGmEntry);

   /* Populate the UPDBREREQ GM Entry */
   gmUPDBREREQCb->msgType = EG_S5_UPDBREREQ_CMD_CODE;
   gmUPDBREREQCb->nmbIeProp = EG_S5_UPDBREREQ_NMB_IES;
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

    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
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

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5UPDBREREQGmEntry */


/***********************************************************************

   Func  : egMakes5UPDBRERSPGmEntry

   Desc  : This function makes the GM Entry for UPDBRERSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5UPDBRERSPGmEntry
(
LegMsgEntry               *gmUPDBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5UPDBRERSPGmEntry(gmUPDBRERSPCb)
LegMsgEntry               *gmUPDBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5UPDBRERSPGmEntry);

   /* Populate the UPDBRERSP GM Entry */
   gmUPDBRERSPCb->msgType = EG_S5_UPDBRERSP_CMD_CODE;
   gmUPDBRERSPCb->nmbIeProp = EG_S5_UPDBRERSP_NMB_IES;
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

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_UE_TIME_ZONE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5UPDBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes5UPDPDNCONSETREQGmEntry

   Desc  : This function makes the GM Entry for UPDPDNCONSETREQ

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5UPDPDNCONSETREQGmEntry
(
LegMsgEntry               *gmUPDPDNCONSETREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5UPDPDNCONSETREQGmEntry(gmUPDPDNCONSETREQCb)
LegMsgEntry               *gmUPDPDNCONSETREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5UPDPDNCONSETREQGmEntry);

   /* Populate the UPDPDNCONSETREQ GM Entry */
   gmUPDPDNCONSETREQCb->msgType = EG_S5_UPDPDNCONSETREQ_CMD_CODE;
   gmUPDPDNCONSETREQCb->nmbIeProp = EG_S5_UPDPDNCONSETREQ_NMB_IES;
   gmUPDPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate UPDPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDPDNCONSETREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmUPDPDNCONSETREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5UPDPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakes5UPDPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for UPDPDNCONSETRSP

   Ret   : None

   File  : smegs5.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes5UPDPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmUPDPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5UPDPDNCONSETRSPGmEntry(gmUPDPDNCONSETRSPCb)
LegMsgEntry               *gmUPDPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakes5UPDPDNCONSETRSPGmEntry);

   /* Populate the UPDPDNCONSETRSP GM Entry */
   gmUPDPDNCONSETRSPCb->msgType = EG_S5_UPDPDNCONSETRSP_CMD_CODE;
   gmUPDPDNCONSETRSPCb->nmbIeProp = EG_S5_UPDPDNCONSETRSP_NMB_IES;
   gmUPDPDNCONSETRSPCb->validateIe = (EscFun)NULLP;

   /* Populate UPDPDNCONSETRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmUPDPDNCONSETRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDPDNCONSETRSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDPDNCONSETRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakes5UPDPDNCONSETRSPGmEntry */
/***********************************************************************

   Func  : egMakes5GmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegs5.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakes5GmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakes5GmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakes5GmDict);

   /* Populating BRERESCMD GM Entry */
   egMakes5BRERESCMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating BRERESFLRIND GM Entry */
   egMakes5BRERESFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHGNTFREQ GM Entry */
   egMakes5CHGNTFREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHGNTFRSP GM Entry */
   egMakes5CHGNTFRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBREREQ GM Entry */
   egMakes5CRTBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBRERSP GM Entry */
   egMakes5CRTBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESREQ GM Entry */
   egMakes5CRTSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESRSP GM Entry */
   egMakes5CRTSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRECMD GM Entry */
   egMakes5DELBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREFLRIND GM Entry */
   egMakes5DELBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREREQ GM Entry */
   egMakes5DELBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRERSP GM Entry */
   egMakes5DELBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETREQ GM Entry */
   egMakes5DELPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETRSP GM Entry */
   egMakes5DELPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESREQ GM Entry */
   egMakes5DELSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESRSP GM Entry */
   egMakes5DELSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBRECMD GM Entry */
   egMakes5MODBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREFLRIND GM Entry */
   egMakes5MODBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREREQ GM Entry */
   egMakes5MODBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBRERSP GM Entry */
   egMakes5MODBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMEACK GM Entry */
   egMakes5RESUMEACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMENOT GM Entry */
   egMakes5RESUMENOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDACK GM Entry */
   egMakes5SUSPENDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDNOT GM Entry */
   egMakes5SUSPENDNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRACESESSACTV GM Entry */
   egMakes5TRACESESSACTVGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRACESESSDEACTV GM Entry */
   egMakes5TRACESESSDEACTVGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBREREQ GM Entry */
   egMakes5UPDBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBRERSP GM Entry */
   egMakes5UPDBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDPDNCONSETREQ GM Entry */
   egMakes5UPDPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDPDNCONSETRSP GM Entry */
   egMakes5UPDPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes5GmDict */



#endif /* EGTP_C */

/********************************************************************30**

         End of file:     eg_dbs5.c@@/main/3 - Fri Feb  4 18:18:06 2011

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
            eg004.102 rrm               1. FTEID IE in create sess req/rsp made conditional
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg003.201 psingh            1. Merged code from eg006.102
/main/3     eg012.201 shpandey          1. Recovery IE_TYPE fix for ccpu00128781
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

