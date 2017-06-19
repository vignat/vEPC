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

    File: eg_dbsn.c

    Sid:      eg_dbsn.c@@/main/1 - Fri Feb  4 18:18:09 2011

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
#include "eg_dbsn.h"
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
#include "eg_dbsn.x"

#ifdef EGTP_C
#if defined(EG_REL_930) || defined(EG_REL_AC0)


/***********************************************************************

   Func  : egMakesnMBMSESNSTOPREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTOPREQ

   Ret   : None

   File  : smegsn.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTOPREQGmEntry
(
LegMsgEntry               *gmMBMSESNSTOPREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTOPREQGmEntry(gmMBMSESNSTOPREQCb)
LegMsgEntry               *gmMBMSESNSTOPREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesnMBMSESNSTOPREQGmEntry);

   /* Populate the MBMSESNSTOPREQ GM Entry */
   gmMBMSESNSTOPREQCb->msgType = EG_SN_MBMSESNSTOPREQ_CMD_CODE;
   gmMBMSESNSTOPREQCb->nmbIeProp = EG_SN_MBMSESNSTOPREQ_NMB_IES;
   gmMBMSESNSTOPREQCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTOPREQ IE Properties */

   /* Populate MBMS_FLOW_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPREQCb->ieProperties[ieCnt],
                    EG_MBMS_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTOPREQCb->posIndex[0][EG_MBMS_FLOW_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTOPREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakesnMBMSESNSTOPREQGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNSTOPRSPGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTOPRSP

   Ret   : None

   File  : smegsn.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTOPRSPGmEntry
(
LegMsgEntry               *gmMBMSESNSTOPRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTOPRSPGmEntry(gmMBMSESNSTOPRSPCb)
LegMsgEntry               *gmMBMSESNSTOPRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesnMBMSESNSTOPRSPGmEntry);

   /* Populate the MBMSESNSTOPRSP GM Entry */
   gmMBMSESNSTOPRSPCb->msgType = EG_SN_MBMSESNSTOPRSP_CMD_CODE;
   gmMBMSESNSTOPRSPCb->nmbIeProp = EG_SN_MBMSESNSTOPRSP_NMB_IES;
   gmMBMSESNSTOPRSPCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTOPRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTOPRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0
   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTOPRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTOPRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTOPRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakesnMBMSESNSTOPRSPGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNSTRTREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTRTREQ

   Ret   : None

   File  : smegsn.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTRTREQGmEntry
(
LegMsgEntry               *gmMBMSESNSTRTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTRTREQGmEntry(gmMBMSESNSTRTREQCb)
LegMsgEntry               *gmMBMSESNSTRTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesnMBMSESNSTRTREQGmEntry);

   /* Populate the MBMSESNSTRTREQ GM Entry */
   gmMBMSESNSTRTREQCb->msgType = EG_SN_MBMSESNSTRTREQ_CMD_CODE;
   gmMBMSESNSTRTREQCb->nmbIeProp = EG_SN_MBMSESNSTRTREQ_NMB_IES;
   gmMBMSESNSTRTREQCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTRTREQ IE Properties */

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TMGI IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_TMGI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_TMGI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_DURATION IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_DURATION_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_SESS_DURATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SERV_AREA IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_SERV_AREA_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_SERV_AREA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_SESS_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_FLOW_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_FLOW_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_QOS IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_BEARER_QOS_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_IP_MULT_DIST IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_IP_MULT_DIST_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_IP_MULT_DIST_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate MBMS_TIME_TO_DATA_TRANS IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_TIME_TO_DATA_TRANS_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_TIME_TO_DATA_TRANS_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakesnMBMSESNSTRTREQGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNSTRTRSPGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTRTRSP

   Ret   : None

   File  : smegsn.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNSTRTRSPGmEntry
(
LegMsgEntry               *gmMBMSESNSTRTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNSTRTRSPGmEntry(gmMBMSESNSTRTRSPCb)
LegMsgEntry               *gmMBMSESNSTRTRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesnMBMSESNSTRTRSPGmEntry);

   /* Populate the MBMSESNSTRTRSP GM Entry */
   gmMBMSESNSTRTRSPCb->msgType = EG_SN_MBMSESNSTRTRSP_CMD_CODE;
   gmMBMSESNSTRTRSPCb->nmbIeProp = EG_SN_MBMSESNSTRTRSP_NMB_IES;
   gmMBMSESNSTRTRSPCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNSTRTRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_DIST_ACK IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_MBMS_DIST_ACK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_MBMS_DIST_ACK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[1][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTRTRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakesnMBMSESNSTRTRSPGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNUPDREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNUPDREQ

   Ret   : None

   File  : smegsn.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNUPDREQGmEntry
(
LegMsgEntry               *gmMBMSESNUPDREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNUPDREQGmEntry(gmMBMSESNUPDREQCb)
LegMsgEntry               *gmMBMSESNUPDREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesnMBMSESNUPDREQGmEntry);

   /* Populate the MBMSESNUPDREQ GM Entry */
   gmMBMSESNUPDREQCb->msgType = EG_SN_MBMSESNUPDREQ_CMD_CODE;
   gmMBMSESNUPDREQCb->nmbIeProp = EG_SN_MBMSESNUPDREQ_NMB_IES;
   gmMBMSESNUPDREQCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNUPDREQ IE Properties */

   /* Populate MBMS_SERV_AREA IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_SERV_AREA_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_SERV_AREA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TMGI IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_TMGI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_TMGI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_DURATION IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_DURATION_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_SESS_DURATION_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_QOS IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_BEARER_QOS_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_SESS_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_SESS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_SESS_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_FLOW_ID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_FLOW_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_FLOW_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
   /* Populate MBMS_TIME_TO_DATA_TRANS IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_MBMS_TIME_TO_DATA_TRANS_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_MBMS_TIME_TO_DATA_TRANS_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNUPDREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakesnMBMSESNUPDREQGmEntry */


/***********************************************************************

   Func  : egMakesnMBMSESNUPDRSPGmEntry

   Desc  : This function makes the GM Entry for MBMSESNUPDRSP

   Ret   : None

   File  : smegsn.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesnMBMSESNUPDRSPGmEntry
(
LegMsgEntry               *gmMBMSESNUPDRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesnMBMSESNUPDRSPGmEntry(gmMBMSESNUPDRSPCb)
LegMsgEntry               *gmMBMSESNUPDRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesnMBMSESNUPDRSPGmEntry);

   /* Populate the MBMSESNUPDRSP GM Entry */
   gmMBMSESNUPDRSPCb->msgType = EG_SN_MBMSESNUPDRSP_CMD_CODE;
   gmMBMSESNUPDRSPCb->nmbIeProp = EG_SN_MBMSESNUPDRSP_NMB_IES;
   gmMBMSESNUPDRSPCb->validateIe = (EscFun)NULLP;

   /* Populate MBMSESNUPDRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MBMS_DIST_ACK IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_MBMS_DIST_ACK_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_MBMS_DIST_ACK_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_FTEID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNUPDRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNUPDRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

   RETVOID;

} /* End of egMakesnMBMSESNUPDRSPGmEntry */
/***********************************************************************

   Func  : egMakesnGmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegsn.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakesnGmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakesnGmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakesnGmDict);

   /* Populating MBMSESNSTOPREQ GM Entry */
   egMakesnMBMSESNSTOPREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNSTOPRSP GM Entry */
   egMakesnMBMSESNSTOPRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNSTRTREQ GM Entry */
   egMakesnMBMSESNSTRTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNSTRTRSP GM Entry */
   egMakesnMBMSESNSTRTRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNUPDREQ GM Entry */
   egMakesnMBMSESNUPDREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MBMSESNUPDRSP GM Entry */
   egMakesnMBMSESNUPDRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakesnGmDict */




#endif /* EG_REL_930 */
#endif /* EGTP_C */

/********************************************************************30**

         End of file:     eg_dbsn.c@@/main/1 - Fri Feb  4 18:18:09 2011

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
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

