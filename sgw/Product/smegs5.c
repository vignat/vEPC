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

    File: smegs5.c

    Sid:      smegs5.c@@/main/3 - Fri Feb  4 18:18:30 2011

    Prg : an

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
#include "smegs5.h"

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
#include "smeg_apputl.x"      /* Application MODULE */
#include "smegs5.x"


 /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */

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
   U8 grpCnt = 0;
   /*-- eg006.102 changes --*/
   EG_TRC2(egMakes5BRERESCMDGmEntry);

   /* Populate the BRERESCMD GM Entry */
   gmBRERESCMDCb->msgType = EG_S5_BRERESCMD_CMD_CODE;
   gmBRERESCMDCb->nmbIeProp = EG_S5_BRERESCMD_NMB_IES;

   /* Populate BRERESCMD IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESCMDCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESCMDCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FLOW_QOS IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_FLOW_QOS_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmBRERESCMDCb->posIndex[0][EG_FLOW_QOS_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TAD IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_TAD_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESCMDCb->posIndex[0][EG_TAD_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmBRERESCMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmBRERESCMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5BRERESFLRINDGmEntry);

   /* Populate the BRERESFLRIND GM Entry */
   gmBRERESFLRINDCb->msgType = EG_S5_BRERESFLRIND_CMD_CODE;
   gmBRERESFLRINDCb->nmbIeProp = EG_S5_BRERESFLRIND_NMB_IES;

   /* Populate BRERESFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESFLRINDCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESFLRINDCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_OPTIONAL,0,1);

   gmBRERESFLRINDCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmBRERESFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes5BRERESFLRINDGmEntry */


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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5CRTBREREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCRTBREREQCb->msgType = EG_S5_CRTBREREQ_CMD_CODE;
   gmCRTBREREQCb->nmbIeProp = EG_S5_CRTBREREQ_NMB_IES;

   /* Populate CRTBREREQ IE Properties */

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,2,1);

   gmCRTBREREQCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,1,1);

   gmCRTBREREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCRTBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCRTBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5CRTBRERSPGmEntry);

   /* Populate the CRTBRERSP GM Entry */
   gmCRTBRERSPCb->msgType = EG_S5_CRTBRERSP_CMD_CODE;
   gmCRTBRERSPCb->nmbIeProp = EG_S5_CRTBRERSP_NMB_IES;

   /* Populate CRTBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,1,3,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,4,
0 );

   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,1,1);

   gmCRTBRERSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5CRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_S5_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_S5_CRTSESREQ_NMB_IES;

   /* Populate CRTSESREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmCRTSESREQCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APN_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_APN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SEL_MODE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SEL_MODE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_SEL_MODE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PDN_TYPE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PDN_TYPE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_PDN_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PAA IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PAA_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_PAA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN_RESTRICT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APN_RESTRICT_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_APN_RESTRICT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmCRTSESREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTSESREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,1,1);

   gmCRTSESREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCRTSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5CRTSESRSPGmEntry);

   /* Populate the CRTSESRSP GM Entry */
   gmCRTSESRSPCb->msgType = EG_S5_CRTSESRSP_CMD_CODE;
   gmCRTSESRSPCb->nmbIeProp = EG_S5_CRTSESRSP_NMB_IES;

   /* Populate CRTSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CMODE IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CMODE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_BEARER_CMODE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,1,1);

   gmCRTSESRSPCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PAA IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PAA_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_PAA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APN_RESTRICT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_APN_RESTRICT_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_APN_RESTRICT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,3,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmCRTSESRSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,2,1);

   gmCRTSESRSPCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,1,1);

   gmCRTSESRSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCRTSESRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5DELBRECMDGmEntry);

   /* Populate the DELBRECMD GM Entry */
   gmDELBRECMDCb->msgType = EG_S5_DELBRECMD_CMD_CODE;
   gmDELBRECMDCb->nmbIeProp = EG_S5_DELBRECMD_NMB_IES;

   /* Populate DELBRECMD IE Properties */

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBRECMDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELBRECMDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmDELBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES);

   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBRECMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmDELBRECMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5DELBREFLRINDGmEntry);

   /* Populate the DELBREFLRIND GM Entry */
   gmDELBREFLRINDCb->msgType = EG_S5_DELBREFLRIND_CMD_CODE;
   gmDELBREFLRINDCb->nmbIeProp = EG_S5_DELBREFLRIND_NMB_IES;

   /* Populate DELBREFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELBREFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELBREFLRINDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREFLRINDCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmDELBREFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5DELBREREQGmEntry);

   /* Populate the DELBREREQ GM Entry */
   gmDELBREREQCb->msgType = EG_S5_DELBREREQ_CMD_CODE;
   gmDELBREREQCb->nmbIeProp = EG_S5_DELBREREQ_NMB_IES;

   /* Populate DELBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE, EG_IE_OPTIONAL, 0, 1);

   gmDELBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmDELBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,2,1);

   gmDELBREREQCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,1,1);

   gmDELBREREQCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmDELBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5DELBRERSPGmEntry);

   /* Populate the DELBRERSP GM Entry */
   gmDELBRERSPCb->msgType = EG_S5_DELBRERSP_CMD_CODE;
   gmDELBRERSPCb->nmbIeProp = EG_S5_DELBRERSP_NMB_IES;

   /* Populate DELBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBRERSPCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,0,1);

   gmDELBRERSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,1,1);

   gmDELBRERSPCb->posIndex[1][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmDELBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes5DELBRERSPGmEntry */


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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5DELSESREQGmEntry);

   /* Populate the DELSESREQ GM Entry */
   gmDELSESREQCb->msgType = EG_S5_DELSESREQ_CMD_CODE;
   gmDELSESREQCb->nmbIeProp = EG_S5_DELSESREQ_NMB_IES;

   /* Populate DELSESREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELSESREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELSESREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELSESREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmDELSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5DELSESRSPGmEntry);

   /* Populate the DELSESRSP GM Entry */
   gmDELSESRSPCb->msgType = EG_S5_DELSESRSP_CMD_CODE;
   gmDELSESRSPCb->nmbIeProp = EG_S5_DELSESRSP_NMB_IES;

   /* Populate DELSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELSESRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELSESRSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmDELSESRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5MODBRECMDGmEntry);

   /* Populate the MODBRECMD GM Entry */
   gmMODBRECMDCb->msgType = EG_S5_MODBRECMD_CMD_CODE;
   gmMODBRECMDCb->nmbIeProp = EG_S5_MODBRECMD_NMB_IES;

   /* Populate MODBRECMD IE Properties */

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBRECMDCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBRECMDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmMODBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmMODBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmMODBRECMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5MODBREFLRINDGmEntry);

   /* Populate the MODBREFLRIND GM Entry */
   gmMODBREFLRINDCb->msgType = EG_S5_MODBREFLRIND_CMD_CODE;
   gmMODBREFLRINDCb->nmbIeProp = EG_S5_MODBREFLRIND_NMB_IES;

   /* Populate MODBREFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBREFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBREFLRINDCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmMODBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_MODBREFLRIND_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmMODBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREFLRINDCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmMODBREFLRINDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5MODBREREQGmEntry);

   /* Populate the MODBREREQ GM Entry */
   gmMODBREREQCb->msgType = EG_S5_MODBREREQ_CMD_CODE;
   gmMODBREREQCb->nmbIeProp = EG_S5_MODBREREQ_NMB_IES;

   /* Populate MODBREREQ IE Properties */

   /* Populate MEI IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_MEI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_MEI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_OPTIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SERVING_NWK IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_SERVING_NWK_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_SERVING_NWK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate INDICATION IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_INDICATION_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBREREQCb->posIndex[0][EG_INDICATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmMODBREREQCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate DELAY IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_DELAY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_DELAY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_NSAPI_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,3,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmMODBREREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmMODBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES);

   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmMODBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5MODBRERSPGmEntry);

   /* Populate the MODBRERSP GM Entry */
   gmMODBRERSPCb->msgType = EG_S5_MODBRERSP_CMD_CODE;
   gmMODBRERSPCb->nmbIeProp = EG_S5_MODBRERSP_NMB_IES;

   /* Populate MODBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBRERSPCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBRERSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,1,1);

   gmMODBRERSPCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmMODBRERSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmMODBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes5MODBRERSPGmEntry */


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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5UPDBREREQGmEntry);

   /* Populate the UPDBREREQ GM Entry */
   gmUPDBREREQCb->msgType = EG_S5_UPDBREREQ_CMD_CODE;
   gmUPDBREREQCb->nmbIeProp = EG_S5_UPDBREREQ_NMB_IES;

   /* Populate UPDBREREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBREREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBREREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_LEGACY_QOS_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_FLAGS_IETYPE,EG_IE_OPTIONAL,1,0,
0 );

   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBREREQCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBREREQCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmUPDBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5UPDBRERSPGmEntry);

   /* Populate the UPDBRERSP GM Entry */
   gmUPDBRERSPCb->msgType = EG_S5_UPDBRERSP_CMD_CODE;
   gmUPDBRERSPCb->nmbIeProp = EG_S5_UPDBRERSP_NMB_IES;

   /* Populate UPDBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate PCO IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_PCO_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_PCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmUPDBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5UPDPDNCONSETREQGmEntry);

   /* Populate the DELPDNCONSETREQ GM Entry */
   gmUPDPDNCONSETREQCb->msgType = EG_S5_UPDPDNCONSETREQ_CMD_CODE;
   gmUPDPDNCONSETREQCb->nmbIeProp = EG_S5_UPDPDNCONSETREQ_NMB_IES;
   gmUPDPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,4,FALSE);

   gmUPDPDNCONSETREQCb->posIndex[4][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes5UPDPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakes5UPDPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETRSP

   Ret   : None

   File  : smegs5.c

************************************************************************/

#ifdef ANSI
PRIVATE void egMakes5UPDPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmUPDPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes5UPDPDNCONSETRSPGmEntry(gmDELPDNCONSETRSPCb)
LegMsgEntry               *gmUPDPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes5UPDPDNCONSETRSPGmEntry);

   /* Populate the UPDPDNCONSETRSP GM Entry */
   gmUPDPDNCONSETRSPCb->msgType = EG_S5_UPDPDNCONSETRSP_CMD_CODE;
   gmUPDPDNCONSETRSPCb->nmbIeProp = EG_S5_UPDPDNCONSETRSP_NMB_IES;
   gmUPDPDNCONSETRSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmUPDPDNCONSETRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDPDNCONSETRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDPDNCONSETRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes5GmDict);

   /* Populating BRERESCMD GM Entry */
   egMakes5BRERESCMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating BRERESFLRIND GM Entry */
   egMakes5BRERESFLRINDGmEntry(&gmEntry[gmMsgCount]);
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

   /* Populating UPDBREREQ GM Entry */
   egMakes5UPDBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   egMakes5DELPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   egMakes5DELPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   egMakes5UPDPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   egMakes5UPDPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBRERSP GM Entry */
   egMakes5UPDBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes5GmDict */


/********************************************************************30**

         End of file:     smegs5.c@@/main/3 - Fri Feb  4 18:18:30 2011

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
/main/2      ---      akulkarni          1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3     eg003.201 psingh            1. Merged code from eg006.102 to Fix 
                                           TRACE5 macro related issue
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

