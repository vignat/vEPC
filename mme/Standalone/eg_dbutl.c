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

    File: eg_dbutl.c

    Sid:      eg_dbutl.c@@/main/3 - Fri Feb  4 18:18:09 2011

    Prg : an

************************************************************************/

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
#include "eg_edm.h"           /* EDM MODULE */
#include "eg_dbutl.h"         /* Application MODULE */
#include "eg_util.h"          /* Application MODULE */

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
#include "eg_edm.x"        /* EDM MODULE */
#include "eg_dbutl.x"      /* Application MODULE */
#include "eg_util.x"       /* Application MODULE */


#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

#ifdef EGTP_C
/************************************************************************
*
*  Func  : egMakeIeEntry
*
*  Desc  : This function populates the IE Properties in IE
           Dictionary.
*
*  Return: No Return value
*
*  File  : eg_dbutl.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC Void egMakeIeEntry
(
LegIeEntry      *ieEntry,
U8             ieCode,
Bool           valid,
U8             dataType,
Ptr            fncPtr,
U16             minLen,
U16             maxLen
)
#else
PUBLIC Void egMakeIeEntry(ieEntry,ieCode,valid, dataType, fncPtr,minLen, maxLen)
LegIeEntry    *ieEntry;
U8             ieCode;
Bool           valid;
U8             dataType;
Ptr            fncPtr;
U16             minLen;
U16             maxLen;
#endif
{
   TRC2(egMakeIeEntry)

   /* Populate the IE Entry */
   ieEntry->ieType  = ieCode;
   ieEntry->dataType = dataType;
   ieEntry->minLen = minLen;
   ieEntry->maxLen = maxLen;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egMakeIeEntry */

/************************************************************************
*
*  Func  : egMakeAppChildIeEntry
*
*  Desc  : This function populates the Application IEs Properties
*          of a grouped IE
*
*  Return: No Return value
*
*  File  : eg_dbutl.c
*
*************************************************************************/

#ifdef ANSI
PUBLIC Void egMakeAppChildIeEntry
(
LegGroupedIe *grpIe,
U8             count,
U8            ieCode,
U8            ieClass,
U8            ieInst,
U8            ieList,
U8             nmbGrpIe
)
#else
PUBLIC Void egMakeAppChildIeEntry(grpIe,count,ieCode,ieClass,
                               ieInst, ieList, nmbGrpIe)
LegGroupedIe *grpIe;
U8             count;
U8            ieCode;
U8            ieClass;
U8            ieInst;
U8            ieList;
U8             nmbGrpIe;
#endif
{
   TRC2(egMakeAppChildIeEntry)

   /* If it is a grouped IE the value of nmbGrpIe is greater than 0 */
   if(nmbGrpIe > 0)
   {
      grpIe->nmbGrpIe   =   nmbGrpIe;
      cmMemset((U8* )grpIe->ieProperties, 0,nmbGrpIe*sizeof(LegIeProperties)); 
   } /* End of if statement */

   /* Populate the IE Entry */
   if( count < grpIe->nmbGrpIe )
   {
      grpIe->ieProperties[count].ieType  = ieCode;
      grpIe->ieProperties[count].ieClass  = ieClass;
      grpIe->ieProperties[count].ieInst  = ieInst;
      grpIe->ieProperties[count].ieList   = ieList;
      grpIe->posIndex[ieInst][ieCode] = count+1;
   }
   else
   {
      /* Trying to access array out of bounds */
   }
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egMakeAppChildIeEntry */



/**********************************************************************************
*
*     Func  : egMakeAppIeProp
*
*     Desc  : This function populates the IE Properties within the eGTP message
*
*     Return: None
*
*     File  : eg_dbutl.c
*
**************************************************************************/


#ifdef ANSI
PUBLIC Void egMakeAppIeProp
(
LegIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass,     /* Mandatory/Conditional/Optional */
U8             instance,     /* Instance  */
U8             ieList     /* Comprehension Required */
)
#else
PUBLIC Void egMakeAppIeProp(ieProp, ieCode, ieClass , instance, ieList)
LegIeProperties *ieProp;
U8             ieCode;         /*  IE Code  */
U8             ieClass;        /* Mandatory/Conditional/Optional */
U8             instance;       /* Instance  */
U8           ieList;             /* Comprehension Required */
#endif
{
   TRC2(egMakeAppIeProp)

   /* Populate the IE Properties */
   ieProp->ieType   =  ieCode;
   ieProp->ieClass  =  ieClass;
   ieProp->ieList   =  ieList;
   ieProp->ieInst =  instance;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of aqMakeAppIeProp */

/***********************************************************************

   Func  : egMakeIeDict

   Desc  : This function populates the IEs in IE Dictionary

   Ret   : EG_ROK/EG_RFAILED

   File  : smegsm.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakeIeDict
(
LegIeEntry *legIeEntry,
U8      *nmbIe,
U16      *posIdx
)
#else
PUBLIC S16 egMakeIeDict(legIeEntry,nmbIe, posIdx)
LegIeEntry *legIeEntry;
U8      *nmbIe;
U16      *posIdx;
#endif
{
   U8  ieCount;                  /* Number of IEs inserted */
   U8  prevIndex;                            /* Previous Index  */

   ieCount = 0;
   prevIndex = 0;
   TRC2(egMakeIeDict); 
   /*-- eg009.201: Corrected Min IE length value for IE to 7
    *              EG_IMSI_IETYPE --*/
   /* Populating EG_IMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_IMSI_IETYPE,
                    TRUE ,EG_BCD ,(NULL) ,14,15); /*fix for ccpu00135846 */
   posIdx[EG_IMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PROCEDURE_TID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PROCEDURE_TID_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_PROCEDURE_TID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_DRX_PARAM_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_DRX_PARAM_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_DRX_PARAM_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UE_NTWRK_CAPB_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UE_NTWRK_CAPB_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,1,16);
   posIdx[EG_UE_NTWRK_CAPB_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GSM_KEY_TRIP_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GSM_KEY_TRIP_IETYPE,
                    TRUE ,EG_GSM_KEY_TRIP ,(NULL) ,78,248);
   posIdx[EG_GSM_KEY_TRIP_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_CIPH_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_CIPH_QUIN ,(NULL) ,40,660);
   posIdx[EG_UMTS_KEY_CIPH_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GSM_KEY_CIPH_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,
                    TRUE ,EG_GSM_KEY_CIPH_QUIN ,(NULL) ,16,628);
   posIdx[EG_GSM_KEY_CIPH_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_QUIN ,(NULL) ,37,656);
   posIdx[EG_UMTS_KEY_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EPS_SEC_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,
                    TRUE ,EG_EPS_SEC_QUAD_QUIN ,(NULL) ,45,1272);
   posIdx[EG_EPS_SEC_QUAD_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_QUAD_QUIN ,(NULL) ,40,1160);
   posIdx[EG_UMTS_KEY_QUAD_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDN_CONN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDN_CONN_IETYPE,
                    TRUE ,EG_GROUPED ,(NULL) ,2,2000);
   posIdx[EG_PDN_CONN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDU_NUMBER_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDU_NUMBER_IETYPE,
                    TRUE ,EG_PDUNUM ,(NULL) ,1,16);
   posIdx[EG_PDU_NUMBER_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PTMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PTMSI_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,4);
   posIdx[EG_PTMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PTMSI_SIG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PTMSI_SIG_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,4);
   posIdx[EG_PTMSI_SIG_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_HOP_CNTR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_HOP_CNTR_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_HOP_CNTR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UE_TIME_ZONE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UE_TIME_ZONE_IETYPE,
                    TRUE ,EG_UE_TIME_ZONE,(NULL) ,2,7);
   posIdx[EG_UE_TIME_ZONE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRACE_REF_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRACE_REF_IETYPE,
                    TRUE ,EG_TRACE_REF,(NULL) ,6,6);
   posIdx[EG_TRACE_REF_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_COMP_REQ_MSG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_COMP_REQ_MSG_IETYPE,
                    TRUE ,EG_CMP_REQ ,(NULL) ,1,1000);
   posIdx[EG_COMP_REQ_MSG_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GUTI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GUTI_IETYPE,
                    TRUE ,EG_GUTI ,(NULL) ,6,10);
   posIdx[EG_GUTI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_F_CONTAINER_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_F_CONTAINER_IETYPE,
                    TRUE ,EG_F_CONT ,(NULL) ,1,1000);
   posIdx[EG_F_CONTAINER_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_F_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_F_CAUSE_IETYPE,
                    TRUE ,EG_F_CAUSE ,(NULL) ,1,1000);
   posIdx[EG_F_CAUSE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SEL_PLMN_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SEL_PLMN_ID_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,5,6);
   posIdx[EG_SEL_PLMN_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TARGET_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TARGET_ID_IETYPE,
                    TRUE ,EG_TRGTID ,(NULL) ,8,10);
   posIdx[EG_TARGET_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PKT_FLOW_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PKT_FLOW_ID_IETYPE,
                    TRUE ,EG_PKTFLOWID ,(NULL) ,2,19);
   posIdx[EG_PKT_FLOW_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RAB_CONTEXT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RAB_CONTEXT_IETYPE,
                    TRUE ,EG_RAB_CONTEXT ,(NULL) ,9,19);
   posIdx[EG_RAB_CONTEXT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SRC_RNC_PDCP_CTX_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRC_RNC_PDCP_CTX_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,2000);
   posIdx[EG_SRC_RNC_PDCP_CTX_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PORT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PORT_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_PORT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_APN_RESTRICT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APN_RESTRICT_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_APN_RESTRICT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SEL_MODE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SEL_MODE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SEL_MODE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SRC_ID_IETYPE */
   /* ccpu00134880: Fix IE length */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRC_ID_IETYPE,
                    TRUE ,EG_SRCID,(NULL) ,13,14);
   posIdx[EG_SRC_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CNG_RPT_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CNG_RPT_ACT_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_CNG_RPT_ACT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSID_IETYPE */
   /* ccpu00134880: Fix IE length */
   /* ccpu00137353: changes minimum length from 16 to 7 */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSID_IETYPE,
                    TRUE ,EG_PDN_CONN_SET_ID ,(NULL) ,7,22);
   posIdx[EG_CSID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHNL_NEEDED_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHNL_NEEDED_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CHNL_NEEDED_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EMLPP_PRIOR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EMLPP_PRIOR_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_EMLPP_PRIOR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_NODE_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_NODE_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_NODE_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FQDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FQDN_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,256);
   posIdx[EG_FQDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRANS_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRANS_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_TRANS_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RFSP_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RFSP_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_RFSP_ID_IETYPE] = ieCount+1; 
   ieCount++;

    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined EG_REL_930 || defined EG_REL_AC0

   /* Populating EG_TMGI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TMGI_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,5,8);
   posIdx[EG_TMGI_IETYPE] = ieCount+1; 
   ieCount++;


   /* Populating EG_MBMS_SESS_DURATION_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SESS_DURATION_IETYPE,
                    TRUE ,EG_UNSIGNED_32,(NULL) ,3,4);
   posIdx[EG_MBMS_SESS_DURATION_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_SERV_AREA_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SERV_AREA_IETYPE,
                    TRUE ,EG_STRING_MAX,(NULL) ,1,513);
   posIdx[EG_MBMS_SERV_AREA_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_SESS_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SESS_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_MBMS_SESS_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_FLOW_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_FLOW_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_MBMS_FLOW_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_IP_MULT_DIST_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_IP_MULT_DIST_IETYPE,
                    TRUE ,EG_MBMS_IPMCAST_DIST ,(NULL) ,13,256);
   posIdx[EG_MBMS_IP_MULT_DIST_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_DIST_ACK_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_DIST_ACK_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_MBMS_DIST_ACK_IETYPE] = ieCount+1; 
   ieCount++;


   /* Populating EG_UCI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UCI_IETYPE,
                    TRUE ,EG_UCI ,(NULL) ,8,12);
   posIdx[EG_UCI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSG_INFO_REP_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSG_INFO_REP_ACT_IETYPE,
                    TRUE ,EG_CSG_INFO_REP_ACT,(NULL) ,1,1);
   posIdx[EG_CSG_INFO_REP_ACT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSG_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSG_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CSG_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CMI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CMI_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_CMI_IETYPE] = ieCount+1; 
   ieCount++;
#endif  /* EG_REL_930 */
   /* Populating EG_SERV_IND_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SERV_IND_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SERV_IND_IETYPE] = ieCount+1; 
   ieCount++;

    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
   /* Populating EG_DETACH_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_DETACH_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_DETACH_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_LDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_LDN_IETYPE,
                    TRUE ,EG_STRING_400 ,(NULL) ,1,400);
   posIdx[EG_LDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_NODE_FEATR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_NODE_FEATR_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_NODE_FEATR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_TIME_TO_DATA_TRANS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_TIME_TO_DATA_TRANS_IETYPE,
                    TRUE ,EG_UNSIGNED_8,(NULL) ,1, 1);
   posIdx[EG_MBMS_TIME_TO_DATA_TRANS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_THROTTLING_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_THROTTLING_IETYPE,
                    TRUE ,EG_THROTTLING,(NULL) ,2,2);
   posIdx[EG_THROTTLING_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_ARP_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ARP_IETYPE,
                    TRUE ,EG_ARP ,(NULL) ,1,1);
   posIdx[EG_ARP_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EPC_TMR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EPC_TMR_IETYPE,
                    TRUE ,EG_EPC_TMR ,(NULL) ,1,1);
   posIdx[EG_EPC_TMR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SIG_PRIOR_IND_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SIG_PRIOR_IND_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SIG_PRIOR_IND_IETYPE] = ieCount+1; 
   ieCount++;
 
   /* Populating EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE,
                    TRUE ,EG_ADD_MM_CNTXT_SRVCC,(NULL) ,3,80);
   posIdx[EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_ADD_FLAG_FOR_SRVCC_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ADD_FLAG_FOR_SRVCC_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_ADD_FLAG_FOR_SRVCC_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MDT_CONF_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MDT_CONF_IETYPE,
                    TRUE ,EG_MDT_CONF ,(NULL) ,11,273);
   posIdx[EG_MDT_CONF_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_APCO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APCO_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,256);
   posIdx[EG_APCO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CNG_TO_RPT_FLG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CNG_TO_RPT_FLG_IETYPE,
                    TRUE ,EG_CNG_TO_RPT_FLG ,(NULL) ,1,1);
   posIdx[EG_CNG_TO_RPT_FLG_IETYPE] = ieCount+1; 
   ieCount++;
#endif 

   /* Populating EG_CAUSE_IETYPE */
   /* ccpu00135385 : fix for min Length */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CAUSE_IETYPE,
                    TRUE ,EG_CAUSE ,(NULL) ,2,6);
   posIdx[EG_CAUSE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PRIVATE_EXT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PRIVATE_EXT_IETYPE,
                    TRUE ,EG_PVT_EXTN ,(NULL) ,3,134);
   posIdx[EG_PRIVATE_EXT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RECOVERY_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RECOVERY_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_RECOVERY_IETYPE] = ieCount+1; 
   ieCount++;

    /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   /* Populating EG_STN_SR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_STN_SR_IETYPE,
                    TRUE ,EG_STN_SR ,(NULL) ,8,9);
   posIdx[EG_STN_SR_IETYPE] = ieCount+1; 
   ieCount++;
#endif

   /* Populating EG_APN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APN_IETYPE,
                    TRUE ,EG_STRING_132 ,(NULL) ,1,100);
   posIdx[EG_APN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_AMBR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_AMBR_IETYPE,
                    TRUE ,EG_AMBR ,(NULL) ,8,8);
   posIdx[EG_AMBR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EBI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EBI_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_EBI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_IPADDR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_IPADDR_IETYPE,
                    TRUE ,EG_IP_ADDR ,(NULL) ,4,16);
   posIdx[EG_IPADDR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MEI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MEI_IETYPE,
                    TRUE ,EG_BCD ,(NULL) ,15,16); /* fix for ccpu00135094*/
   posIdx[EG_MEI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MSISDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MSISDN_IETYPE,
                    TRUE ,EG_BCD ,(NULL) ,1,30); /* fix for ccpu00135846 */
   posIdx[EG_MSISDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_INDICATION_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_INDICATION_IETYPE,
                    TRUE ,EG_INDICATION ,(NULL) ,2,3);
   posIdx[EG_INDICATION_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PCO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PCO_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,256);
   posIdx[EG_PCO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PAA_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PAA_IETYPE,
                    TRUE ,EG_PAA_IEDATA ,(NULL) ,5,22);
   posIdx[EG_PAA_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_QOS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_QOS_IETYPE,
                    TRUE ,EG_BQOS_IEDATA ,(NULL) ,22,22);
   posIdx[EG_BEARER_QOS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FLOW_QOS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FLOW_QOS_IETYPE,
                    TRUE ,EG_FQOS ,(NULL) ,21,21);
   posIdx[EG_FLOW_QOS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RAT_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RAT_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_RAT_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SERVING_NWK_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SERVING_NWK_IETYPE,
                    TRUE ,EG_SNETWORK ,(NULL) ,3,3);
   posIdx[EG_SERVING_NWK_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_TFT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_TFT_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,257);
   posIdx[EG_BEARER_TFT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TAD_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TAD_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,257);
   posIdx[EG_TAD_IETYPE] = ieCount+1; 
   ieCount++;

   /* ccpu00135095 : Updating the DB or ULI */
   /* Populating EG_ULI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ULI_IETYPE,
                    TRUE ,EG_ULI_DATA ,(NULL) ,8,39);
   posIdx[EG_ULI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FTEID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FTEID_IETYPE,
                    TRUE ,EG_FTEID ,(NULL) ,9,25);
   posIdx[EG_FTEID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TMSI_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_TMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GBL_CN_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GBL_CN_ID_IETYPE,
                    TRUE ,EG_GLOBAL_CN_ID ,(NULL) ,7,7);
   posIdx[EG_GBL_CN_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S103_PDN_FW_INFO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S103_PDN_FW_INFO_IETYPE,
                    TRUE ,EG_S103_PDN_FW_INFO ,(NULL) ,11,32);
   posIdx[EG_S103_PDN_FW_INFO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S1U_DATA_FW_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S1U_DATA_FW_IETYPE,
                    TRUE ,EG_S1U_DATA_FW ,(NULL) ,10,22);
   posIdx[EG_S1U_DATA_FW_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_DELAY_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_DELAY_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_DELAY_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_CNTX_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_CNTX_IETYPE,
                    TRUE ,EG_GROUPED ,(NULL) ,2,2000);
   posIdx[EG_BEARER_CNTX_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHARGING_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHARGING_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CHARGING_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHARGING_CHAR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHARGING_CHAR_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_CHARGING_CHAR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRACE_INFO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRACE_INFO_IETYPE,
                    TRUE ,EG_TRC_INFO ,(NULL) ,34,86);
   posIdx[EG_TRACE_INFO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_FLAGS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_FLAGS_IETYPE,
                    TRUE ,EG_BEARER_FLAG ,(NULL) ,1,1);
   posIdx[EG_BEARER_FLAGS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDN_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDN_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_PDN_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Return the number of IEs inserted  */
   *nmbIe = ieCount;
   RETVALUE(ROK);
}  /* End of egMakeIeDeict() */

#endif /* End of EGTP_C */

#ifdef EGTP_U
/************************************************************************
*
*  Func  : egUMakeIeEntry
*
*  Desc  : This function populates the IE Properties in IE
           Dictionary for GTP-U
*
*  Return: No Return value
*
*  File  : eg_dbutl.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC Void egUMakeIeEntry
(
EgIeEntry      *ieEntry,
U8             ieCode,
Bool           tvType,
U8             dataType,
U16             minLen,
U16             maxLen
)
#else
PUBLIC Void egUMakeIeEntry(ieEntry, ieCode, tvType, dataType, minLen, maxLen)
EgIeEntry    *ieEntry;
U8             ieCode;
Bool           tvType;
U8             dataType;
U16             minLen;
U16             maxLen;
#endif
{
   TRC2(egUMakeIeEntry)

   /* Populate the IE Entry */
   ieEntry->ieCode  = ieCode;
   ieEntry->tvType  = tvType;
   ieEntry->dataType = dataType;
   ieEntry->minLen = minLen;
   ieEntry->maxLen = maxLen;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egUMakeIeEntry */

/**********************************************************************************
*
*     Func  : egUMakeAppIeProp
*
*     Desc  : This function populates the IE Properties within the eGTPi-U message
*
*     Return: None
*
*     File  : eg_dbutl.c
*
**************************************************************************/


#ifdef ANSI
PUBLIC Void egUMakeAppIeProp
(
EgUIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass     /* Mandatory/Conditional/Optional */
)
#else
PUBLIC Void egUMakeAppIeProp(ieProp, ieCode, ieClass)
EgUIeProperties *ieProp;
U8             ieCode;         /*  IE Code  */
U8             ieClass;        /* Mandatory/Conditional/Optional */
#endif
{
   TRC2(egUMakeAppIeProp)

   /* Populate the IE Properties */
   ieProp->ieType   =  ieCode;
   ieProp->ieClass  =  ieClass;
   /*-- eg003.201: changes from eg006.102: Added RETVOID--*/
   RETVOID;

} /* End of egUMakeAppIeProp */





#endif /* End of EGTP_U */



#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/**********************************************************************
         End of file:     eg_dbutl.c@@/main/3 - Fri Feb  4 18:18:09 2011

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
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg003.201 psingh            1. Merged code from eg006.102 to Fix 
                                           TRACE5 macro related issue
/main/3     eg005.201 psingh            1.Corrected Max, Min IE length value
                                          for ie EG_IMSI_IETYPE
/main/3     eg009.201 asaurabh          1. For EG_IMSI_IETYPE updated Min IE 
                                           length value to 7
                                        2. Added for S2B and S2A .
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

