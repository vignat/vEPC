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

    File: smegs11.c

    Sid:      smegs11.c@@/main/3 - Fri Feb  4 18:18:29 2011

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
#include "smegs11.h"

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
#include "smegs11.x"


/***********************************************************************

   Func  : egMakeIeDict

   Desc  : This function populates the IEs in IE Dictionary

   Ret   : EG_ROK/EG_RFAILED

   File  : smegs11.c

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakeIeDict)

   ieCount = 0;
   prevIndex = 0;

   /* Populating EG_IMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_IMSI_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,8,8);
   posIdx[EG_IMSI_IETYPE] = ieCount+1; 
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
                    TRUE ,EG_TRC_INFO ,(NULL) ,8,92);
   posIdx[EG_TRACE_INFO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_FLAGS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_FLAGS_IETYPE,
                    TRUE ,EG_BEARER_FLAG ,(NULL) ,1,1);
   posIdx[EG_BEARER_FLAGS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PAGING_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PAGING_CAUSE_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_PAGING_CAUSE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDN_TYPE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDN_TYPE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_PDN_TYPE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PROCEDURE_TID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PROCEDURE_TID_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_PROCEDURE_TID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UE_NTWRK_CAPB_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UE_NTWRK_CAPB_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,1,16);
   posIdx[EG_UE_NTWRK_CAPB_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDU_NUMBER_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDU_NUMBER_IETYPE,
                    TRUE ,EG_PDUNUM ,(NULL) ,1,16);
   posIdx[EG_PDU_NUMBER_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GSM_KEY_TRIP_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GSM_KEY_TRIP_IETYPE,
                    TRUE ,EG_GSM_KEY_TRIP ,(NULL) ,16,95);
   posIdx[EG_GSM_KEY_TRIP_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_CIPH_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_CIPH_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_CIPH_QUIN ,(NULL) ,40,153);
   posIdx[EG_UMTS_KEY_CIPH_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GSM_KEY_CIPH_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GSM_KEY_CIPH_QUIN_IETYPE,
                    TRUE ,EG_GSM_KEY_CIPH_QUIN ,(NULL) ,16,129);
   posIdx[EG_GSM_KEY_CIPH_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_QUIN ,(NULL) ,37,150);
   posIdx[EG_UMTS_KEY_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EPS_SEC_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,
                    TRUE ,EG_EPS_SEC_QUAD_QUIN ,(NULL) ,45,224);
   posIdx[EG_EPS_SEC_QUAD_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UMTS_KEY_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UMTS_KEY_QUAD_QUIN_IETYPE,
                    TRUE ,EG_UMTS_KEY_QUAD_QUIN ,(NULL) ,40,219);
   posIdx[EG_UMTS_KEY_QUAD_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PDN_CONN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDN_CONN_IETYPE,
                    TRUE ,EG_GROUPED ,(NULL) ,2,2000);
   posIdx[EG_PDN_CONN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_GRE_KEY_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_GRE_KEY_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,1,16);
   posIdx[EG_GRE_KEY_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_CMODE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_CMODE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_BEARER_CMODE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CNTXT_PRIOR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CNTXT_PRIOR_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,0,0);
   posIdx[EG_CNTXT_PRIOR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_LMA_IP_ADDR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_LMA_IP_ADDR_IETYPE,
                    TRUE ,EG_IP_ADDR ,(NULL) ,4,16);
   posIdx[EG_LMA_IP_ADDR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PTMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PTMSI_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,3);
   posIdx[EG_PTMSI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PTMSI_SIG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PTMSI_SIG_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,3);
   posIdx[EG_PTMSI_SIG_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_HOP_CNTR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_HOP_CNTR_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_HOP_CNTR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_AUTH_QUINTUP_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_AUTH_QUINTUP_IETYPE,
                    TRUE ,EG_AUTH_QUIN ,(NULL) ,16,1);
   posIdx[EG_AUTH_QUINTUP_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_AUTH_QUADRUP_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_AUTH_QUADRUP_IETYPE,
                    TRUE ,EG_AUTH_QUAD ,(NULL) ,16,1);
   posIdx[EG_AUTH_QUADRUP_IETYPE] = ieCount+1; 
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
                    TRUE ,EG_TRGTID ,(NULL) ,1,8);
   posIdx[EG_TARGET_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CELL_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CELL_ID_IETYPE,
                    TRUE ,EG_CELLID ,(NULL) ,1,2);
   posIdx[EG_CELL_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_NSAPI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_NSAPI_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_NSAPI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PKT_FLOW_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PKT_FLOW_ID_IETYPE,
                    TRUE ,EG_PKTFLOWID ,(NULL) ,9,19);
   posIdx[EG_PKT_FLOW_ID_IETYPE] = ieCount+1; 
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

   /* Populating EG_TRANS_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRANS_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_TRANS_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SEL_MODE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SEL_MODE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SEL_MODE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CNG_RPT_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CNG_RPT_ACT_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_CNG_RPT_ACT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EMB_OFF_IE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EMB_OFF_IE_IETYPE,
                    TRUE ,EG_EMB_CAUSE ,(NULL) ,6,261);
   posIdx[EG_EMB_OFF_IE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSID_IETYPE,
                    TRUE ,EG_PDN_CONN_SET_ID ,(NULL) ,16,22);
   posIdx[EG_CSID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CAUSE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
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

   /* Populating EG_APN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APN_IETYPE,
                    TRUE ,EG_STRING_132 ,(NULL) ,1,100);
   posIdx[EG_APN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_AMBR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_AMBR_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,5,5);
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
                    TRUE ,EG_STRING_16 ,(NULL) ,1,16);
   posIdx[EG_MEI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MSISDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MSISDN_IETYPE,
                    TRUE ,EG_STRING_32 ,(NULL) ,1,32);
   posIdx[EG_MSISDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_INDICATION_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_INDICATION_IETYPE,
                    TRUE ,EG_INDICATION ,(NULL) ,2,2);
   posIdx[EG_INDICATION_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PCO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PCO_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,256);
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
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,256);
   posIdx[EG_BEARER_TFT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TAD_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TAD_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,256);
   posIdx[EG_TAD_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_ULI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ULI_IETYPE,
                    TRUE ,EG_ULI_DATA ,(NULL) ,6,34);
   posIdx[EG_ULI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_FTEID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_FTEID_IETYPE,
                    TRUE ,EG_FTEID ,(NULL) ,9,22);
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
                    TRUE ,EG_GBL_CN_ID ,(NULL) ,7,7);
   posIdx[EG_GBL_CN_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_LEGACY_QOS_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_LEGACY_QOS_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,256);
   posIdx[EG_LEGACY_QOS_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S103_PDN_FW_INFO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S103_PDN_FW_INFO_IETYPE,
                    TRUE ,EG_S103_PDN_FW_INFO ,(NULL) ,6,31);
   posIdx[EG_S103_PDN_FW_INFO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S1U_DATA_FW_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S1U_DATA_FW_IETYPE,
                    TRUE ,EG_S1U_DATA_FW ,(NULL) ,9,24);
   posIdx[EG_S1U_DATA_FW_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_DELAY_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_DELAY_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_DELAY_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_ID_LIST_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_ID_LIST_IETYPE,
                    TRUE ,EG_BID_LST ,(NULL) ,1,11);
   posIdx[EG_BEARER_ID_LIST_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_BEARER_CNTX_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_BEARER_CNTX_IETYPE,
                    TRUE ,EG_GROUPED ,(NULL) ,2,2000);
   posIdx[EG_BEARER_CNTX_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S101_IP_ADDR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S101_IP_ADDR_IETYPE,
                    TRUE ,EG_IP_ADDR ,(NULL) ,4,16);
   posIdx[EG_S101_IP_ADDR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S102_IP_ADDR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S102_IP_ADDR_IETYPE,
                    TRUE ,EG_IP_ADDR ,(NULL) ,4,16);
   posIdx[EG_S102_IP_ADDR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CHARGING_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CHARGING_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_CHARGING_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Return the number of IEs inserted  */
   *nmbIe = ieCount;
   RETVALUE(ROK);
}  /* End of egMakeIeDeict() */




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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11BRERESCMDGmEntry);

   /* Populate the BRERESCMD GM Entry */
   gmBRERESCMDCb->msgType = EG_S11_BRERESCMD_CMD_CODE;
   gmBRERESCMDCb->nmbIeProp = EG_S11_BRERESCMD_NMB_IES;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11BRERESFLRINDGmEntry);

   /* Populate the BRERESFLRIND GM Entry */
   gmBRERESFLRINDCb->msgType = EG_S11_BRERESFLRIND_CMD_CODE;
   gmBRERESFLRINDCb->nmbIeProp = EG_S11_BRERESFLRIND_NMB_IES;

   /* Populate BRERESFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmBRERESFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmBRERESFLRINDCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmBRERESFLRINDCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes11BRERESFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes11CHNGNOTREQGmEntry

   Desc  : This function makes the GM Entry for CHNGNOTREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CHNGNOTREQGmEntry
(
LegMsgEntry               *gmCHNGNOTREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CHNGNOTREQGmEntry(gmCHNGNOTREQCb)
LegMsgEntry               *gmCHNGNOTREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CHNGNOTREQGmEntry);

   /* Populate the CHNGNOTREQ GM Entry */
   gmCHNGNOTREQCb->msgType = EG_S11_CHNGNOTREQ_CMD_CODE;
   gmCHNGNOTREQCb->nmbIeProp = EG_S11_CHNGNOTREQ_NMB_IES;

   /* Populate CHNGNOTREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCHNGNOTREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmCHNGNOTREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RAT_TYPE IE Properties */
   egMakeAppIeProp(&gmCHNGNOTREQCb->ieProperties[ieCnt],
                    EG_RAT_TYPE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCHNGNOTREQCb->posIndex[0][EG_RAT_TYPE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate ULI IE Properties */
   egMakeAppIeProp(&gmCHNGNOTREQCb->ieProperties[ieCnt],
                    EG_ULI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCHNGNOTREQCb->posIndex[0][EG_ULI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCHNGNOTREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCHNGNOTREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11CHNGNOTREQGmEntry */


/***********************************************************************

   Func  : egMakes11CHNGNOTRSPGmEntry

   Desc  : This function makes the GM Entry for CHNGNOTRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11CHNGNOTRSPGmEntry
(
LegMsgEntry               *gmCHNGNOTRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11CHNGNOTRSPGmEntry(gmCHNGNOTRSPCb)
LegMsgEntry               *gmCHNGNOTRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CHNGNOTRSPGmEntry);

   /* Populate the CHNGNOTRSP GM Entry */
   gmCHNGNOTRSPCb->msgType = EG_S11_CHNGNOTRSP_CMD_CODE;
   gmCHNGNOTRSPCb->nmbIeProp = EG_S11_CHNGNOTRSP_NMB_IES;

   /* Populate CHNGNOTRSP IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCHNGNOTRSPCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmCHNGNOTRSPCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCHNGNOTRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCHNGNOTRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CNG_RPT_ACT IE Properties */
   egMakeAppIeProp(&gmCHNGNOTRSPCb->ieProperties[ieCnt],
                    EG_CNG_RPT_ACT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCHNGNOTRSPCb->posIndex[0][EG_CNG_RPT_ACT_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCHNGNOTRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCHNGNOTRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11CHNGNOTRSPGmEntry */


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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTBREREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCRTBREREQCb->msgType = EG_S11_CRTBREREQ_CMD_CODE;
   gmCRTBREREQCb->nmbIeProp = EG_S11_CRTBREREQ_NMB_IES;

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

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
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
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTBRERSPGmEntry);

   /* Populate the CRTBRERSP GM Entry */
   gmCRTBRERSPCb->msgType = EG_S11_CRTBRERSP_CMD_CODE;
   gmCRTBRERSPCb->nmbIeProp = EG_S11_CRTBRERSP_NMB_IES;

   /* Populate CRTBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTBRERSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,3,
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
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmCRTBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTFWDTUNREQGmEntry);

   /* Populate the CRTFWDTUNREQ GM Entry */
   gmCRTFWDTUNREQCb->msgType = EG_S11_CRTFWDTUNREQ_CMD_CODE;
   gmCRTFWDTUNREQCb->nmbIeProp = EG_S11_CRTFWDTUNREQ_NMB_IES;

   /* Populate CRTFWDTUNREQ IE Properties */

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTFWDTUNREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,3,
0 );

   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTFWDTUNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTFWDTUNRSPGmEntry);

   /* Populate the CRTFWDTUNRSP GM Entry */
   gmCRTFWDTUNRSPCb->msgType = EG_S11_CRTFWDTUNRSP_CMD_CODE;
   gmCRTFWDTUNRSPCb->nmbIeProp = EG_S11_CRTFWDTUNRSP_NMB_IES;

   /* Populate CRTFWDTUNRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,1,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,2,
0 );

   egMakeAppChildIeEntry(&(gmCRTFWDTUNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,1,3,
0 );

   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTFWDTUNRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTINDDATFWDTUNREQGmEntry);

   /* Populate the CRTINDDATFWDTUNREQ GM Entry */
   gmCRTINDDATFWDTUNREQCb->msgType = EG_S11_CRTINDDATFWDTUNREQ_CMD_CODE;
   gmCRTINDDATFWDTUNREQCb->nmbIeProp = EG_S11_CRTINDDATFWDTUNREQ_NMB_IES;

   /* Populate CRTINDDATFWDTUNREQ IE Properties */

   /* Populate S103_PDN_FW_INFO IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNREQCb->ieProperties[ieCnt],
                    EG_S103_PDN_FW_INFO_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTINDDATFWDTUNREQCb->posIndex[0][EG_S103_PDN_FW_INFO_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTINDDATFWDTUNRSPGmEntry);

   /* Populate the CRTINDDATFWDTUNRSP GM Entry */
   gmCRTINDDATFWDTUNRSPCb->msgType = EG_S11_CRTINDDATFWDTUNRSP_CMD_CODE;
   gmCRTINDDATFWDTUNRSPCb->nmbIeProp = EG_S11_CRTINDDATFWDTUNRSP_NMB_IES;

   /* Populate CRTINDDATFWDTUNRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate S1U_DATA_FW IE Properties */
   egMakeAppIeProp(&gmCRTINDDATFWDTUNRSPCb->ieProperties[ieCnt],
                    EG_S1U_DATA_FW_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTINDDATFWDTUNRSPCb->posIndex[0][EG_S1U_DATA_FW_IETYPE]  = ieCnt + 1;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_S11_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_S11_CRTSESREQ_NMB_IES;

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
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,1,1);

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11CRTSESRSPGmEntry);

   /* Populate the CRTSESRSP GM Entry */
   gmCRTSESRSPCb->msgType = EG_S11_CRTSESRSP_CMD_CODE;
   gmCRTSESRSPCb->nmbIeProp = EG_S11_CRTSESRSP_NMB_IES;

   /* Populate CRTSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,1);

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

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,1,1);

   gmCRTSESRSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,0,1);

   gmCRTSESRSPCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DELBRECMDGmEntry);

   /* Populate the DELBRECMD GM Entry */
   gmDELBRECMDCb->msgType = EG_S11_DELBRECMD_CMD_CODE;
   gmDELBRECMDCb->nmbIeProp = EG_S11_DELBRECMD_NMB_IES;

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DELBREFLRINDGmEntry);

   /* Populate the DELBREFLRIND GM Entry */
   gmDELBREFLRINDCb->msgType = EG_S11_DELBREFLRIND_CMD_CODE;
   gmDELBREFLRINDCb->nmbIeProp = EG_S11_DELBREFLRIND_NMB_IES;

   /* Populate DELBREFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELBREFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmDELBREFLRINDCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREFLRINDCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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

   egMakeAppChildIeEntry(&(gmDELBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DELBREREQGmEntry);

   /* Populate the DELBREREQ GM Entry */
   gmDELBREREQCb->msgType = EG_S11_DELBREREQ_CMD_CODE;
   gmDELBREREQCb->nmbIeProp = EG_S11_DELBREREQ_NMB_IES;

   /* Populate DELBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,1);

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
                    EG_CSID_IETYPE,EG_IE_OPTIONAL,0,1);

   gmDELBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DELBRERSPGmEntry);

   /* Populate the DELBRERSP GM Entry */
   gmDELBRERSPCb->msgType = EG_S11_DELBRERSP_CMD_CODE;
   gmDELBRERSPCb->nmbIeProp = EG_S11_DELBRERSP_NMB_IES;

   /* Populate DELBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELBRERSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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

   egMakeAppChildIeEntry(&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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

} /* End of egMakes11DELBRERSPGmEntry */


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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DELSESREQGmEntry);

   /* Populate the DELSESREQ GM Entry */
   gmDELSESREQCb->msgType = EG_S11_DELSESREQ_CMD_CODE;
   gmDELSESREQCb->nmbIeProp = EG_S11_DELSESREQ_NMB_IES;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DELSESRSPGmEntry);

   /* Populate the DELSESRSP GM Entry */
   gmDELSESRSPCb->msgType = EG_S11_DELSESRSP_CMD_CODE;
   gmDELSESRSPCb->nmbIeProp = EG_S11_DELSESRSP_NMB_IES;

   /* Populate DELSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDELSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDELSESRSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DWLNKDATNOTGmEntry);

   /* Populate the DWLNKDATNOT GM Entry */
   gmDWLNKDATNOTCb->msgType = EG_S11_DWLNKDATNOT_CMD_CODE;
   gmDWLNKDATNOTCb->nmbIeProp = EG_S11_DWLNKDATNOT_NMB_IES;

   /* Populate DWLNKDATNOT IE Properties */

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmDWLNKDATNOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DWLNKDATNOTACKGmEntry);

   /* Populate the DWLNKDATNOTACK GM Entry */
   gmDWLNKDATNOTACKCb->msgType = EG_S11_DWLNKDATNOTACK_CMD_CODE;
   gmDWLNKDATNOTACKCb->nmbIeProp = EG_S11_DWLNKDATNOTACK_NMB_IES;

   /* Populate DWLNKDATNOTACK IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate DELAY IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_DELAY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_DELAY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_OPTIONAL,0,1);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmDWLNKDATNOTACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11DWLNKDATNOTACKGmEntry */


/***********************************************************************

   Func  : egMakes11DWLNKDATNOTFLRGmEntry

   Desc  : This function makes the GM Entry for DWLNKDATNOTFLR

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11DWLNKDATNOTFLRGmEntry
(
LegMsgEntry               *gmDWLNKDATNOTFLRCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11DWLNKDATNOTFLRGmEntry(gmDWLNKDATNOTFLRCb)
LegMsgEntry               *gmDWLNKDATNOTFLRCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11DWLNKDATNOTFLRGmEntry);

   /* Populate the DWLNKDATNOTFLR GM Entry */
   gmDWLNKDATNOTFLRCb->msgType = EG_S11_DWLNKDATNOTFLR_CMD_CODE;
   gmDWLNKDATNOTFLRCb->nmbIeProp = EG_S11_DWLNKDATNOTFLR_NMB_IES;

   /* Populate DWLNKDATNOTFLR IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmDWLNKDATNOTFLRCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmDWLNKDATNOTFLRCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDWLNKDATNOTFLRCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmDWLNKDATNOTFLRCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11DWLNKDATNOTFLRGmEntry */


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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11MODBRECMDGmEntry);

   /* Populate the MODBRECMD GM Entry */
   gmMODBRECMDCb->msgType = EG_S11_MODBRECMD_CMD_CODE;
   gmMODBRECMDCb->nmbIeProp = EG_S11_MODBRECMD_NMB_IES;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11MODBREFLRINDGmEntry);

   /* Populate the MODBREFLRIND GM Entry */
   gmMODBREFLRINDCb->msgType = EG_S11_MODBREFLRIND_CMD_CODE;
   gmMODBREFLRINDCb->nmbIeProp = EG_S11_MODBREFLRIND_NMB_IES;

   /* Populate MODBREFLRIND IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBREFLRINDCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmMODBREFLRINDCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBREFLRINDCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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

   egMakeAppChildIeEntry(&(gmMODBREFLRINDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11MODBREREQGmEntry);

   /* Populate the MODBREREQ GM Entry */
   gmMODBREREQCb->msgType = EG_S11_MODBREREQ_CMD_CODE;
   gmMODBREREQCb->nmbIeProp = EG_S11_MODBREREQ_NMB_IES;

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
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,1);

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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11MODBRERSPGmEntry);

   /* Populate the MODBRERSP GM Entry */
   gmMODBRERSPCb->msgType = EG_S11_MODBRERSP_CMD_CODE;
   gmMODBRERSPCb->nmbIeProp = EG_S11_MODBRERSP_NMB_IES;

   /* Populate MODBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmMODBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBRERSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmMODBRERSPCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate FTEID IE Properties */
   egMakeAppIeProp(&gmMODBRERSPCb->ieProperties[ieCnt],
                    EG_FTEID_IETYPE,EG_IE_MANDATORY,0,1);

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

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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

   egMakeAppChildIeEntry(&(gmMODBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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

} /* End of egMakes11MODBRERSPGmEntry */


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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11RESUMEACKGmEntry);

   /* Populate the RESUMEACK GM Entry */
   gmRESUMEACKCb->msgType = EG_S11_RESUMEACK_CMD_CODE;
   gmRESUMEACKCb->nmbIeProp = EG_S11_RESUMEACK_NMB_IES;

   /* Populate RESUMEACK IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRESUMEACKCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmRESUMEACKCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRESUMEACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmRESUMEACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11RESUMENOTGmEntry);

   /* Populate the RESUMENOT GM Entry */
   gmRESUMENOTCb->msgType = EG_S11_RESUMENOT_CMD_CODE;
   gmRESUMENOTCb->nmbIeProp = EG_S11_RESUMENOT_NMB_IES;

   /* Populate RESUMENOT IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmRESUMENOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmRESUMENOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmRESUMENOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11RESUMENOTGmEntry */


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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11SUSPENDACKGmEntry);

   /* Populate the SUSPENDACK GM Entry */
   gmSUSPENDACKCb->msgType = EG_S11_SUSPENDACK_CMD_CODE;
   gmSUSPENDACKCb->nmbIeProp = EG_S11_SUSPENDACK_NMB_IES;

   /* Populate SUSPENDACK IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmSUSPENDACKCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmSUSPENDACKCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPENDACKCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmSUSPENDACKCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

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
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11SUSPENDNOTGmEntry);

   /* Populate the SUSPENDNOT GM Entry */
   gmSUSPENDNOTCb->msgType = EG_S11_SUSPENDNOT_CMD_CODE;
   gmSUSPENDNOTCb->nmbIeProp = EG_S11_SUSPENDNOT_NMB_IES;

   /* Populate SUSPENDNOT IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_MANDATORY,0,1);

   gmSUSPENDNOTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmSUSPENDNOTCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmSUSPENDNOTCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11SUSPENDNOTGmEntry */


/***********************************************************************

   Func  : egMakes11UPDBEARERCMPGmEntry

   Desc  : This function makes the GM Entry for UPDBEARERCMP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11UPDBEARERCMPGmEntry
(
LegMsgEntry               *gmUPDBEARERCMPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11UPDBEARERCMPGmEntry(gmUPDBEARERCMPCb)
LegMsgEntry               *gmUPDBEARERCMPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11UPDBEARERCMPGmEntry);

   /* Populate the UPDBEARERCMP GM Entry */
   gmUPDBEARERCMPCb->msgType = EG_S11_UPDBEARERCMP_CMD_CODE;
   gmUPDBEARERCMPCb->nmbIeProp = EG_S11_UPDBEARERCMP_NMB_IES;

   /* Populate UPDBEARERCMP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDBEARERCMPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBEARERCMPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDBEARERCMPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBEARERCMPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBEARERCMPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmUPDBEARERCMPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11UPDBEARERCMPGmEntry */


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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11UPDBREREQGmEntry);

   /* Populate the UPDBREREQ GM Entry */
   gmUPDBREREQCb->msgType = EG_S11_UPDBREREQ_CMD_CODE;
   gmUPDBREREQCb->nmbIeProp = EG_S11_UPDBREREQ_NMB_IES;

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

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL,1,0,
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

   /* Populate GRE_KEY IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_GRE_KEY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBREREQCb->posIndex[0][EG_GRE_KEY_IETYPE]  = ieCnt + 1;
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
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11UPDBRERSPGmEntry);

   /* Populate the UPDBRERSP GM Entry */
   gmUPDBRERSPCb->msgType = EG_S11_UPDBRERSP_CMD_CODE;
   gmUPDBRERSPCb->nmbIeProp = EG_S11_UPDBRERSP_NMB_IES;

   /* Populate UPDBRERSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
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

   egMakeAppChildIeEntry(&(gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
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
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,1);

   gmUPDBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11UPDBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes11UPDUPLNREQGmEntry

   Desc  : This function makes the GM Entry for UPDUPLNREQ

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11UPDUPLNREQGmEntry
(
LegMsgEntry               *gmUPDUPLNREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11UPDUPLNREQGmEntry(gmUPDUPLNREQCb)
LegMsgEntry               *gmUPDUPLNREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11UPDUPLNREQGmEntry);

   /* Populate the UPDUPLNREQ GM Entry */
   gmUPDUPLNREQCb->msgType = EG_S11_UPDUPLNREQ_CMD_CODE;
   gmUPDUPLNREQCb->nmbIeProp = EG_S11_UPDUPLNREQ_NMB_IES;

   /* Populate UPDUPLNREQ IE Properties */

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDUPLNREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDUPLNREQCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmUPDUPLNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_UPDUPLNREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDUPLNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDUPLNREQCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmUPDUPLNREQCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmUPDUPLNREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_UPDUPLNREQ_BEARER_CNTX1_GRP_NMB_IES);

   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmUPDUPLNREQCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDUPLNREQCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDUPLNREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmUPDUPLNREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11UPDUPLNREQGmEntry */


/***********************************************************************

   Func  : egMakes11UPDUPLNRSPGmEntry

   Desc  : This function makes the GM Entry for UPDUPLNRSP

   Ret   : None

   File  : smegs11.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes11UPDUPLNRSPGmEntry
(
LegMsgEntry               *gmUPDUPLNRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes11UPDUPLNRSPGmEntry(gmUPDUPLNRSPCb)
LegMsgEntry               *gmUPDUPLNRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11UPDUPLNRSPGmEntry);

   /* Populate the UPDUPLNRSP GM Entry */
   gmUPDUPLNRSPCb->msgType = EG_S11_UPDUPLNRSP_CMD_CODE;
   gmUPDUPLNRSPCb->nmbIeProp = EG_S11_UPDUPLNRSP_NMB_IES;

   /* Populate UPDUPLNRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmUPDUPLNRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDUPLNRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EMB_OFF_IE IE Properties */
   egMakeAppIeProp(&gmUPDUPLNRSPCb->ieProperties[ieCnt],
                    EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,0,1);

   gmUPDUPLNRSPCb->posIndex[0][EG_EMB_OFF_IE_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDUPLNRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_MANDATORY,0,1);

   gmUPDUPLNRSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_UPDUPLNRSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmUPDUPLNRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,1);

   gmUPDUPLNRSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,1,0,
EG_UPDUPLNRSP_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,1,0,
0 );

   egMakeAppChildIeEntry(&(gmUPDUPLNRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EMB_OFF_IE_IETYPE,EG_IE_CONDITIONAL,1,0,
0 );

   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDUPLNRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,0);

   gmUPDUPLNRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes11UPDUPLNRSPGmEntry */
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakes11GmDict);

   /* Populating BRERESCMD GM Entry */
   egMakes11BRERESCMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating BRERESFLRIND GM Entry */
   egMakes11BRERESFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHNGNOTREQ GM Entry */
   egMakes11CHNGNOTREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CHNGNOTRSP GM Entry */
   egMakes11CHNGNOTRSPGmEntry(&gmEntry[gmMsgCount]);
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

   /* Populating DWLNKDATNOTFLR GM Entry */
   egMakes11DWLNKDATNOTFLRGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

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

   /* Populating RESUMEACK GM Entry */
   egMakes11RESUMEACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating RESUMENOT GM Entry */
   egMakes11RESUMENOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDACK GM Entry */
   egMakes11SUSPENDACKGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating SUSPENDNOT GM Entry */
   egMakes11SUSPENDNOTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBEARERCMP GM Entry */
   egMakes11UPDBEARERCMPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBREREQ GM Entry */
   egMakes11UPDBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBRERSP GM Entry */
   egMakes11UPDBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDUPLNREQ GM Entry */
   egMakes11UPDUPLNREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDUPLNRSP GM Entry */
   egMakes11UPDUPLNRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes11GmDict */


/********************************************************************30**

         End of file:     smegs11.c@@/main/3 - Fri Feb  4 18:18:29 2011

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
/main/3     eg003.201 psingh             1. Fixed TRACE5 macro related issue from eg006.102 
*********************************************************************91*/

