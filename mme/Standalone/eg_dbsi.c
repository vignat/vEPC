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

    File: smegsi.c

    Sid:      eg_dbsi.c@@/main/1 - Fri Feb  4 18:18:07 2011

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
#include "eg_dbsi.h"
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
/*-- eg005.201 : Update with product header file --*/
#include "eg_edm.x"      /* EDM MODULE */
#include "eg_dbutl.x"      /* Application MODULE */

#include "eg_dbsi.x"



#ifdef EGTP_C
/***********************************************************************

   Func  : egMakesiCFGTFRTNLGmEntry

   Desc  : This function makes the GM Entry for CFGTFRTNL

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCFGTFRTNLGmEntry
(
LegMsgEntry               *gmCFGTFRTNLCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCFGTFRTNLGmEntry(gmCFGTFRTNLCb)
LegMsgEntry               *gmCFGTFRTNLCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiCFGTFRTNLGmEntry);

   /* Populate the CFGTFRTNL GM Entry */
   gmCFGTFRTNLCb->msgType = EG_SI_CFGTFRTNL_CMD_CODE;
   gmCFGTFRTNLCb->nmbIeProp = EG_SI_CFGTFRTNL_NMB_IES;
   gmCFGTFRTNLCb->validateIe = (EscFun)NULLP;

   /* Populate CFGTFRTNL IE Properties */

   /* Populate F_CONTAINER IE Properties */
   egMakeAppIeProp(&gmCFGTFRTNLCb->ieProperties[ieCnt],
                    EG_F_CONTAINER_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCFGTFRTNLCb->posIndex[0][EG_F_CONTAINER_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TARGET_ID IE Properties */
   egMakeAppIeProp(&gmCFGTFRTNLCb->ieProperties[ieCnt],
                    EG_TARGET_ID_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCFGTFRTNLCb->posIndex[0][EG_TARGET_ID_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCFGTFRTNLGmEntry */


/***********************************************************************

   Func  : egMakesiCRTINDDATFWDTUNREQGmEntry

   Desc  : This function makes the GM Entry for CRTINDDATFWDTUNREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCRTINDDATFWDTUNREQGmEntry
(
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCRTINDDATFWDTUNREQGmEntry(gmCRTINDDATFWDTUNREQCb)
LegMsgEntry               *gmCRTINDDATFWDTUNREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakesiCRTINDDATFWDTUNREQGmEntry);

   /* Populate the CRTINDDATFWDTUNREQ GM Entry */
   gmCRTINDDATFWDTUNREQCb->msgType = EG_SI_CRTINDDATFWDTUNREQ_CMD_CODE;
   gmCRTINDDATFWDTUNREQCb->nmbIeProp = EG_SI_CRTINDDATFWDTUNREQ_NMB_IES;
   gmCRTINDDATFWDTUNREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTINDDATFWDTUNREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
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

} /* End of egMakesiCRTINDDATFWDTUNREQGmEntry */


/***********************************************************************

   Func  : egMakesiCRTSESREQGmEntry

   Desc  : This function makes the GM Entry for CRTSESREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCRTSESREQGmEntry
(
LegMsgEntry               *gmCRTSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCRTSESREQGmEntry(gmCRTSESREQCb)
LegMsgEntry               *gmCRTSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakesiCRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_SI_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_SI_CRTSESREQ_NMB_IES;
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
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,2,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,3,FALSE,
0 );

   grpCnt++;
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   grpCnt++;
/*eg009.201 New FTEID Instances 5,6 introduced*/
#if defined(EG_S2A_SUPP) || defined(EG_REL_AC0)
   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );
   
   grpCnt++;

   egMakeAppChildIeEntry((gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );

   grpCnt++;
#endif
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
   /*eg009.201:Added CSID instance 2,3*/
#if defined(EG_S2A_SUPP) || defined(EG_REL_AC0)
   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmCRTSESREQCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
   
   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,3,FALSE);

   gmCRTSESREQCb->posIndex[3][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif   
   /* Populate UE_TIME_ZONE IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_UE_TIME_ZONE_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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

   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,1,FALSE);

   gmCRTSESREQCb->posIndex[1][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,2,FALSE);

   gmCRTSESREQCb->posIndex[2][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate SIG_PRIOR_IND IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_SIG_PRIOR_IND_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_SIG_PRIOR_IND_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APCO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_APCO_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCRTSESREQGmEntry */


/***********************************************************************

   Func  : egMakesiCTXREQGmEntry

   Desc  : This function makes the GM Entry for CTXREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiCTXREQGmEntry
(
LegMsgEntry               *gmCTXREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiCTXREQGmEntry(gmCTXREQCb)
LegMsgEntry               *gmCTXREQCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesiCTXREQGmEntry);

   /* Populate the CTXREQ GM Entry */
   gmCTXREQCb->msgType = EG_SI_CTXREQ_CMD_CODE;
   gmCTXREQCb->nmbIeProp = EG_SI_CTXREQ_NMB_IES;
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

   /* Populate GUTI IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_GUTI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_GUTI_IETYPE]  = ieCnt + 1;
   ieCnt++;

#ifdef EG_REL_AC0 
   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

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
#endif 

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCTXREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCTXREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiCTXREQGmEntry */


/***********************************************************************

   Func  : egMakesiDELPDNCONSETREQGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDELPDNCONSETREQGmEntry
(
LegMsgEntry               *gmDELPDNCONSETREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDELPDNCONSETREQGmEntry(gmDELPDNCONSETREQCb)
LegMsgEntry               *gmDELPDNCONSETREQCb;
#endif
{
   U8 ieCnt = 0;
   TRC2(egMakesiDELPDNCONSETREQGmEntry);

   /* Populate the DELPDNCONSETREQ GM Entry */
   gmDELPDNCONSETREQCb->msgType = EG_SI_DELPDNCONSETREQ_CMD_CODE;
   gmDELPDNCONSETREQCb->nmbIeProp = EG_SI_DELPDNCONSETREQ_NMB_IES;
   gmDELPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

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

#ifdef EG_REL_AC0
   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,3,FALSE);

   gmDELPDNCONSETREQCb->posIndex[3][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif


   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiDELPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakesiDELPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETRSP

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDELPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmDELPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDELPDNCONSETRSPGmEntry(gmDELPDNCONSETRSPCb)
LegMsgEntry               *gmDELPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiDELPDNCONSETRSPGmEntry);

   /* Populate the DELPDNCONSETRSP GM Entry */
   gmDELPDNCONSETRSPCb->msgType = EG_SI_DELPDNCONSETRSP_CMD_CODE;
   gmDELPDNCONSETRSPCb->nmbIeProp = EG_SI_DELPDNCONSETRSP_NMB_IES;
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

} /* End of egMakesiDELPDNCONSETRSPGmEntry */


/***********************************************************************

   Func  : egMakesiDWLNKDATNOTGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOT

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDWLNKDATNOTGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDWLNKDATNOTGmEntry(gmDWLNKDATNOTCb)
LegMsgEntry               *gmDWLNKDATNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiDWLNKDATNOTGmEntry);

   /* Populate the DWLNKDATNOT GM Entry */
   gmDWLNKDATNOTCb->msgType = EG_SI_DWLNKDATNOT_CMD_CODE;
   gmDWLNKDATNOTCb->nmbIeProp = EG_SI_DWLNKDATNOT_NMB_IES;
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

} /* End of egMakesiDWLNKDATNOTGmEntry */


/***********************************************************************

   Func  : egMakesiDWLNKDATNOTACKGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTACK

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDWLNKDATNOTACKGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDWLNKDATNOTACKGmEntry(gmDWLNKDATNOTACKCb)
LegMsgEntry               *gmDWLNKDATNOTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiDWLNKDATNOTACKGmEntry);

   /* Populate the DWLNKDATNOTACK GM Entry */
   gmDWLNKDATNOTACKCb->msgType = EG_SI_DWLNKDATNOTACK_CMD_CODE;
   gmDWLNKDATNOTACKCb->nmbIeProp = EG_SI_DWLNKDATNOTACK_NMB_IES;
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

} /* End of egMakesiDWLNKDATNOTACKGmEntry */


/***********************************************************************

   Func  : egMakesiDWLNKDATNOTFLRINDGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTFLRIND

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiDWLNKDATNOTFLRINDGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiDWLNKDATNOTFLRINDGmEntry(gmDWLNKDATNOTFLRINDCb)
LegMsgEntry               *gmDWLNKDATNOTFLRINDCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiDWLNKDATNOTFLRINDGmEntry);

   /* Populate the DWLNKDATNOTFLRIND GM Entry */
   gmDWLNKDATNOTFLRINDCb->msgType = EG_SI_DWLNKDATNOTFLRIND_CMD_CODE;
   gmDWLNKDATNOTFLRINDCb->nmbIeProp = EG_SI_DWLNKDATNOTFLRIND_NMB_IES;
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

} /* End of egMakesiDWLNKDATNOTFLRINDGmEntry */


/***********************************************************************

   Func  : egMakesiFWDRELREQGmEntry

   Desc  : This function makes the GM Entry for FWDRELREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiFWDRELREQGmEntry
(
LegMsgEntry               *gmFWDRELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiFWDRELREQGmEntry(gmFWDRELREQCb)
LegMsgEntry               *gmFWDRELREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   LegIeProperties **ieProperties1;
   TRC2(egMakesiFWDRELREQGmEntry);

   /* Populate the FWDRELREQ GM Entry */
   gmFWDRELREQCb->msgType = EG_SI_FWDRELREQ_CMD_CODE;
   gmFWDRELREQCb->nmbIeProp = EG_SI_FWDRELREQ_NMB_IES;
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
         ieProperties1 = &gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe->ieProperties;
         ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties1),(sizeof(LegIeProperties)*(EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES)));
         if( ret == ROK && *ieProperties1 != NULLP)
         {

   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES);

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_FTEID_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,FALSE,
0 );

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_F_CONTAINER_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_TRANS_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt1++;
   egMakeAppChildIeEntry((gmFWDRELREQCb->ieProperties[ieCnt].groupedIe->ieProperties[grpCnt].groupedIe), grpCnt1, EG_BEARER_FLAGS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   grpCnt1++;
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
   /* CR 135348 fix- this should be CONDITIONAL not mandatory */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_TRIP_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_TRIP_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_CIPH_QUIN IE Properties */ 
   /* CR 135348 fix same as above */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate GSM_KEY_CIPH_QUIN IE Properties */ 
   /* CR 135348 fix - same as above */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_GSM_KEY_CIPH_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUIN IE Properties */ 
   /* CR 135348 fix - same as above */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_UMTS_KEY_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_UMTS_KEY_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EPS_SEC_QUAD_QUIN IE Properties */ 
   /* CR 135348 fix - same as above */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_EPS_SEC_QUAD_QUIN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate UMTS_KEY_QUAD_QUIN IE Properties */
   /* CR 135348 fix same as above */
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
   /* Populate CMI IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_CMI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_CMI_IETYPE]  = ieCnt + 1;
   ieCnt++;
   /* Populate CSG_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_CSG_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_CSG_ID_IETYPE]  = ieCnt + 1;
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

   /* Populate MDT_CONF IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_MDT_CONF_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_MDT_CONF_IETYPE]  = ieCnt + 1;
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
#endif

   /* Populate SRC_ID IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_SRC_ID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_SRC_ID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmFWDRELREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmFWDRELREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiFWDRELREQGmEntry */


/***********************************************************************

   Func  : egMakesiIDENTREQGmEntry

   Desc  : This function makes the GM Entry for IDENTREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiIDENTREQGmEntry
(
LegMsgEntry               *gmIDENTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiIDENTREQGmEntry(gmIDENTREQCb)
LegMsgEntry               *gmIDENTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiIDENTREQGmEntry);

   /* Populate the IDENTREQ GM Entry */
   gmIDENTREQCb->msgType = EG_SI_IDENTREQ_CMD_CODE;
   gmIDENTREQCb->nmbIeProp = EG_SI_IDENTREQ_NMB_IES;
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

   /* Populate GUTI IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_GUTI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_GUTI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate COMP_REQ_MSG IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_COMP_REQ_MSG_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_COMP_REQ_MSG_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmIDENTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmIDENTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiIDENTREQGmEntry */


/***********************************************************************

   Func  : egMakesiIDENTRSPGmEntry

   Desc  : This function makes the GM Entry for IDENTRSP

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiIDENTRSPGmEntry
(
LegMsgEntry               *gmIDENTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiIDENTRSPGmEntry(gmIDENTRSPCb)
LegMsgEntry               *gmIDENTRSPCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiIDENTRSPGmEntry);

   /* Populate the IDENTRSP GM Entry */
   gmIDENTRSPCb->msgType = EG_SI_IDENTRSP_CMD_CODE;
   gmIDENTRSPCb->nmbIeProp = EG_SI_IDENTRSP_NMB_IES;
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

} /* End of egMakesiIDENTRSPGmEntry */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
/***********************************************************************

   Func  : egMakesiMBMSESNSTRTREQGmEntry

   Desc  : This function makes the GM Entry for MBMSESNSTRTREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiMBMSESNSTRTREQGmEntry
(
LegMsgEntry               *gmMBMSESNSTRTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiMBMSESNSTRTREQGmEntry(gmMBMSESNSTRTREQCb)
LegMsgEntry               *gmMBMSESNSTRTREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiMBMSESNSTRTREQGmEntry);

   /* Populate the MBMSESNSTRTREQ GM Entry */
   gmMBMSESNSTRTREQCb->msgType = EG_SI_MBMSESNSTRTREQ_CMD_CODE;
   gmMBMSESNSTRTREQCb->nmbIeProp = EG_SI_MBMSESNSTRTREQ_NMB_IES;
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

#ifdef EG_REL_AC0 
   /* Populate MBMS_TIME_TO_DATA_TRANS IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_MBMS_TIME_TO_DATA_TRANS_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_MBMS_TIME_TO_DATA_TRANS_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif 

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMBMSESNSTRTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMBMSESNSTRTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiMBMSESNSTRTREQGmEntry */
#endif

#ifdef EG_REL_AC0
/***********************************************************************

   Func  : egMakesiPGWRSTNOTGmEntry

   Desc  : This function makes the GM Entry for PGWRSTNOT

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiPGWRSTNOTGmEntry
(
LegMsgEntry               *gmPGWRSTNOTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiPGWRSTNOTGmEntry(gmPGWRSTNOTCb)
LegMsgEntry               *gmPGWRSTNOTCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiPGWRSTNOTGmEntry);

   /* Populate the PGWRSTNOT GM Entry */
   gmPGWRSTNOTCb->msgType = EG_SI_PGWRSTNOT_CMD_CODE;
   gmPGWRSTNOTCb->nmbIeProp = EG_SI_PGWRSTNOT_NMB_IES;
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

} /* End of egMakesiPGWRSTNOTGmEntry */


/***********************************************************************

   Func  : egMakesiPGWRSTNOTACKGmEntry

   Desc  : This function makes the GM Entry for PGWRSTNOTACK

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiPGWRSTNOTACKGmEntry
(
LegMsgEntry               *gmPGWRSTNOTACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiPGWRSTNOTACKGmEntry(gmPGWRSTNOTACKCb)
LegMsgEntry               *gmPGWRSTNOTACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiPGWRSTNOTACKGmEntry);

   /* Populate the PGWRSTNOTACK GM Entry */
   gmPGWRSTNOTACKCb->msgType = EG_SI_PGWRSTNOTACK_CMD_CODE;
   gmPGWRSTNOTACKCb->nmbIeProp = EG_SI_PGWRSTNOTACK_NMB_IES;
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

} /* End of egMakesiPGWRSTNOTACKGmEntry */
#endif


/***********************************************************************

   Func  : egMakesiRANINFORLYGmEntry

   Desc  : This function makes the GM Entry for RANINFORLY

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiRANINFORLYGmEntry
(
LegMsgEntry               *gmRANINFORLYCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiRANINFORLYGmEntry(gmRANINFORLYCb)
LegMsgEntry               *gmRANINFORLYCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiRANINFORLYGmEntry);

   /* Populate the RANINFORLY GM Entry */
   gmRANINFORLYCb->msgType = EG_SI_RANINFORLY_CMD_CODE;
   gmRANINFORLYCb->nmbIeProp = EG_SI_RANINFORLY_NMB_IES;
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

} /* End of egMakesiRANINFORLYGmEntry */


/***********************************************************************

   Func  : egMakesiRELCANCELREQGmEntry

   Desc  : This function makes the GM Entry for RELCANCELREQ

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiRELCANCELREQGmEntry
(
LegMsgEntry               *gmRELCANCELREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiRELCANCELREQGmEntry(gmRELCANCELREQCb)
LegMsgEntry               *gmRELCANCELREQCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiRELCANCELREQGmEntry);

   /* Populate the RELCANCELREQ GM Entry */
   gmRELCANCELREQCb->msgType = EG_SI_RELCANCELREQ_CMD_CODE;
   gmRELCANCELREQCb->nmbIeProp = EG_SI_RELCANCELREQ_NMB_IES;
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

} /* End of egMakesiRELCANCELREQGmEntry */


/***********************************************************************

   Func  : egMakesiSUSPNDACKGmEntry

   Desc  : This function makes the GM Entry for SUSPNDACK

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiSUSPNDACKGmEntry
(
LegMsgEntry               *gmSUSPNDACKCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiSUSPNDACKGmEntry(gmSUSPNDACKCb)
LegMsgEntry               *gmSUSPNDACKCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiSUSPNDACKGmEntry);

   /* Populate the SUSPNDACK GM Entry */
   gmSUSPNDACKCb->msgType = EG_SI_SUSPNDACK_CMD_CODE;
   gmSUSPNDACKCb->nmbIeProp = EG_SI_SUSPNDACK_NMB_IES;
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

} /* End of egMakesiSUSPNDACKGmEntry */


/***********************************************************************

   Func  : egMakesiSUSPNDNOTFNGmEntry

   Desc  : This function makes the GM Entry for SUSPNDNOTFN

   Ret   : None

   File  : smegsi.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakesiSUSPNDNOTFNGmEntry
(
LegMsgEntry               *gmSUSPNDNOTFNCb                         /* GM Control Block */
)
#else
PRIVATE void egMakesiSUSPNDNOTFNGmEntry(gmSUSPNDNOTFNCb)
LegMsgEntry               *gmSUSPNDNOTFNCb;
#endif
{
   U8 ieCnt = 0;

   TRC2(egMakesiSUSPNDNOTFNGmEntry);

   /* Populate the SUSPNDNOTFN GM Entry */
   gmSUSPNDNOTFNCb->msgType = EG_SI_SUSPNDNOTFN_CMD_CODE;
   gmSUSPNDNOTFNCb->nmbIeProp = EG_SI_SUSPNDNOTFN_NMB_IES;
   gmSUSPNDNOTFNCb->validateIe = (EscFun)NULLP;

   /* Populate SUSPNDNOTFN IE Properties */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);
#else
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,FALSE);
#endif

   gmSUSPNDNOTFNCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

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

#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmSUSPNDNOTFNCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;
#endif

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPNDNOTFNCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmSUSPNDNOTFNCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakesiSUSPNDNOTFNGmEntry */
/***********************************************************************

   Func  : egMakesiGmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegsi.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakesiGmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakesiGmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakesiGmDict);

   /* Populating CFGTFRTNL GM Entry */
   egMakesiCFGTFRTNLGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTINDDATFWDTUNREQ GM Entry */
   egMakesiCRTINDDATFWDTUNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESREQ GM Entry */
   egMakesiCRTSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CTXREQ GM Entry */
   egMakesiCTXREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETREQ GM Entry */
   egMakesiDELPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETRSP GM Entry */
   egMakesiDELPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOT GM Entry */
   egMakesiDWLNKDATNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOTACK GM Entry */
   egMakesiDWLNKDATNOTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DWLNKDATNOTFLRIND GM Entry */
   egMakesiDWLNKDATNOTFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating FWDRELREQ GM Entry */
   egMakesiFWDRELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTREQ GM Entry */
   egMakesiIDENTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating IDENTRSP GM Entry */
   egMakesiIDENTRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   /* Populating MBMSESNSTRTREQ GM Entry */
   egMakesiMBMSESNSTRTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif
   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   /* Populating PGWRSTNOT GM Entry */
   egMakesiPGWRSTNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating PGWRSTNOTACK GM Entry */
   egMakesiPGWRSTNOTACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
#endif

   /* Populating RANINFORLY GM Entry */
   egMakesiRANINFORLYGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RELCANCELREQ GM Entry */
   egMakesiRELCANCELREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPNDACK GM Entry */
   egMakesiSUSPNDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPNDNOTFN GM Entry */
   egMakesiSUSPNDNOTFNGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakesiGmDict */
#endif /* EGTP_C */

/********************************************************************30**

         End of file:     eg_dbsi.c@@/main/1 - Fri Feb  4 18:18:07 2011

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
/main/2      ---      kchitanya         1. initial for egtp release 1.2
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/1     eg005.201 psingh            1. Updated headers to product file
                                           eg_edm.x & eg_dbutl.x instead of
                                           sample header file smeg_apputl.x
/main/1     eg009.201 asaurabh          1. New FTEID Instances 5,6 introduced
                                        2. Added CSID instance 2,3 
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

