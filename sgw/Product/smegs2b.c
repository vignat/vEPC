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

    File: smegs2b.c

    Sid:      egcodetool.cpp@@/main/3 - Fri Feb  4 18:18:18 2011

    Prg: an

************************************************************************/

/*eg009.201:Initial Release for S2B interface*/
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
#include "smegs2b.h"

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
#include "smegs2b.x"


/***********************************************************************

   Func  : egMakeIeDict

   Desc  : This function populates the IEs in IE Dictionary

   Ret   : EG_ROK/EG_RFAILED

   File  : smegs2b.c

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

   /* Populating EG_IMSI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_IMSI_IETYPE,
                    TRUE ,EG_STRING_16 ,(NULL) ,14,15);
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
                    TRUE ,EG_GSM_KEY_TRIP ,(NULL) ,78,219);
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
                    TRUE ,EG_UMTS_KEY_QUIN ,(NULL) ,37,224);
   posIdx[EG_UMTS_KEY_QUIN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_EPS_SEC_QUAD_QUIN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_EPS_SEC_QUAD_QUIN_IETYPE,
                    TRUE ,EG_EPS_SEC_QUAD_QUIN ,(NULL) ,45,350);
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

   /* Populating EG_PDU_NUMBER_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PDU_NUMBER_IETYPE,
                    TRUE ,EG_PDUNUM ,(NULL) ,1,16);
   posIdx[EG_PDU_NUMBER_IETYPE] = ieCount+1; 
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

   /* Populating EG_UE_TIME_ZONE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UE_TIME_ZONE_IETYPE,
                    TRUE ,EG_TIME_ZONE ,(NULL) ,2,7);
   posIdx[EG_UE_TIME_ZONE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRACE_REF_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRACE_REF_IETYPE,
                    TRUE ,EG_TRC_REF ,(NULL) ,6,6);
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
                    TRUE ,EG_TRGTID ,(NULL) ,8,9);
   posIdx[EG_TARGET_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_PKT_FLOW_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_PKT_FLOW_ID_IETYPE,
                    TRUE ,EG_PKTFLOWID ,(NULL) ,9,19);
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
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRC_ID_IETYPE,
                    TRUE ,EG_SRC_ID ,(NULL) ,14,14);
   posIdx[EG_SRC_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CNG_RPT_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CNG_RPT_ACT_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_CNG_RPT_ACT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSID_IETYPE,
                    TRUE ,EG_PDN_CONN_SET_ID ,(NULL) ,16,22);
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

   /* Populating EG_MBMS_SESS_DURATION_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SESS_DURATION_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,3,3);
   posIdx[EG_MBMS_SESS_DURATION_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_SERV_AREA_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_SERV_AREA_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,513);
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
                    TRUE ,EG_MBMS_IP_MULT_DIST ,(NULL) ,12,256);
   posIdx[EG_MBMS_IP_MULT_DIST_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MBMS_DIST_ACK_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MBMS_DIST_ACK_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_MBMS_DIST_ACK_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_RFSP_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_RFSP_ID_IETYPE,
                    TRUE ,EG_UNSIGNED_16 ,(NULL) ,2,2);
   posIdx[EG_RFSP_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_UCI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_UCI_IETYPE,
                    TRUE ,EG_UCI ,(NULL) ,12,12);
   posIdx[EG_UCI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CSG_INFO_REP_ACT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CSG_INFO_REP_ACT_IETYPE,
                    TRUE ,EG_CSG_INFO_REP_ACT ,(NULL) ,1,1);
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

   /* Populating EG_SERV_IND_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SERV_IND_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SERV_IND_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TMGI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TMGI_IETYPE,
                    TRUE ,EG_STRING_32 ,(NULL) ,3,3);
   posIdx[EG_TMGI_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_LDN_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_LDN_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,1,400);
   posIdx[EG_LDN_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_APCO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_APCO_IETYPE,
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,256);
   posIdx[EG_APCO_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_CAUSE_IETYPE,
                    TRUE ,EG_CAUSE ,(NULL) ,6,6);
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

   /* Populating EG_STN_SR_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_STN_SR_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,4,8);
   posIdx[EG_STN_SR_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SRC_TRGT_TRANS_CONT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRC_TRGT_TRANS_CONT_IETYPE,
                    TRUE ,EG_SRVCC_CONTAINER ,(NULL) ,9,25);
   posIdx[EG_SRC_TRGT_TRANS_CONT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRGT_SRC_TRANS_CONT_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRGT_SRC_TRANS_CONT_IETYPE,
                    TRUE ,EG_SRVCC_CONTAINER ,(NULL) ,9,25);
   posIdx[EG_TRGT_SRC_TRANS_CONT_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MME_CTXT_EUTRAN_SRVCC_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MME_CTXT_EUTRAN_SRVCC_IETYPE,
                    TRUE ,EG_SV_MMCNTXT_EUTRAN ,(NULL) ,1,1000);
   posIdx[EG_MME_CTXT_EUTRAN_SRVCC_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_MME_CTXT_UTRAN_SRVCC_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_MME_CTXT_UTRAN_SRVCC_IETYPE,
                    TRUE ,EG_SV_MMCNTXT_UTRAN ,(NULL) ,1,1000);
   posIdx[EG_MME_CTXT_UTRAN_SRVCC_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SRVCC_CAUSE_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SRVCC_CAUSE_IETYPE,
                    TRUE ,EG_UNSIGNED_8 ,(NULL) ,1,1);
   posIdx[EG_SRVCC_CAUSE_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRGT_RNC_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRGT_RNC_ID_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,8,8);
   posIdx[EG_TRGT_RNC_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_TRGT_CELL_ID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_TRGT_CELL_ID_IETYPE,
                    TRUE ,EG_STRING_8 ,(NULL) ,8,8);
   posIdx[EG_TRGT_CELL_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SV_CP_TEID_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SV_CP_TEID_IETYPE,
                    TRUE ,EG_UNSIGNED_32 ,(NULL) ,4,4);
   posIdx[EG_SV_CP_TEID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_SV_FLAG_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_SV_FLAG_IETYPE,
                    TRUE ,EG_SV_FLAGS ,(NULL) ,1,1);
   posIdx[EG_SV_FLAG_IETYPE] = ieCount+1; 
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
                    TRUE ,EG_STRING_16 ,(NULL) ,15,16);
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
                    TRUE ,EG_STRING_MAX ,(NULL) ,3,257);
   posIdx[EG_TAD_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_ULI_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_ULI_IETYPE,
                    TRUE ,EG_ULI_DATA ,(NULL) ,8,34);
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
                    TRUE ,EG_GBL_CN_ID ,(NULL) ,7,7);
   posIdx[EG_GBL_CN_ID_IETYPE] = ieCount+1; 
   ieCount++;

   /* Populating EG_S103_PDN_FW_INFO_IETYPE */
   egMakeIeEntry(&legIeEntry[ieCount], 
                    EG_S103_PDN_FW_INFO_IETYPE,
                    TRUE ,EG_S103_PDN_FW_INFO ,(NULL) ,15,37);
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
                    TRUE ,EG_TRC_INFO ,(NULL) ,46,92);
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




/***********************************************************************

   Func  : egMakes2bCRTBREREQGmEntry

   Desc  : This function makes the GM Entry for CRTBREREQ

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bCRTBREREQGmEntry
(
LegMsgEntry               *gmCRTBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bCRTBREREQGmEntry(gmCRTBREREQCb)
LegMsgEntry               *gmCRTBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bCRTBREREQGmEntry);

   /* Populate the CRTBREREQ GM Entry */
   gmCRTBREREQCb->msgType = EG_S2B_CRTBREREQ_CMD_CODE;
   gmCRTBREREQCb->nmbIeProp = EG_S2B_CRTBREREQ_NMB_IES;
   gmCRTBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL_OPTIONAL,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bCRTBREREQGmEntry */


/***********************************************************************

   Func  : egMakes2bCRTBRERSPGmEntry

   Desc  : This function makes the GM Entry for CRTBRERSP

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bCRTBRERSPGmEntry
(
LegMsgEntry               *gmCRTBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bCRTBRERSPGmEntry(gmCRTBRERSPCb)
LegMsgEntry               *gmCRTBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bCRTBRERSPGmEntry);

   /* Populate the CRTBRERSP GM Entry */
   gmCRTBRERSPCb->msgType = EG_S2B_CRTBRERSP_CMD_CODE;
   gmCRTBRERSPCb->nmbIeProp = EG_S2B_CRTBRERSP_NMB_IES;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,8,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,9,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmCRTBRERSPCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bCRTBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes2bCRTSESREQGmEntry

   Desc  : This function makes the GM Entry for CRTSESREQ

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bCRTSESREQGmEntry
(
LegMsgEntry               *gmCRTSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bCRTSESREQGmEntry(gmCRTSESREQCb)
LegMsgEntry               *gmCRTSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bCRTSESREQGmEntry);

   /* Populate the CRTSESREQ GM Entry */
   gmCRTSESREQCb->msgType = EG_S2B_CRTSESREQ_CMD_CODE;
   gmCRTSESREQCb->nmbIeProp = EG_S2B_CRTSESREQ_NMB_IES;
   gmCRTSESREQCb->validateIe = (EscFun)NULLP;

   /* Populate CRTSESREQ IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate MSISDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_MSISDN_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_MSISDN_IETYPE]  = ieCnt + 1;
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

   /* Populate PAA IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PAA_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PAA_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,5,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

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
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,2,FALSE);

   gmCRTSESREQCb->posIndex[2][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CHARGING_CHAR IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_CHARGING_CHAR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_CHARGING_CHAR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,2,FALSE);

   gmCRTSESREQCb->posIndex[2][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APCO IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_APCO_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_APCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bCRTSESREQGmEntry */


/***********************************************************************

   Func  : egMakes2bCRTSESRSPGmEntry

   Desc  : This function makes the GM Entry for CRTSESRSP

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bCRTSESRSPGmEntry
(
LegMsgEntry               *gmCRTSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bCRTSESRSPGmEntry(gmCRTSESRSPCb)
LegMsgEntry               *gmCRTSESRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bCRTSESRSPGmEntry);

   /* Populate the CRTSESRSP GM Entry */
   gmCRTSESRSPCb->msgType = EG_S2B_CRTSESRSP_CMD_CODE;
   gmCRTSESRSPCb->nmbIeProp = EG_S2B_CRTSESRSP_NMB_IES;
   gmCRTSESRSPCb->validateIe = (EscFun)NULLP;

   /* Populate CRTSESRSP IE Properties */

   /* Populate CAUSE IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_CAUSE_IETYPE]  = ieCnt + 1;
   ieCnt++;

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

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_OPTIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_FTEID_IETYPE,EG_IE_CONDITIONAL,4,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CHARGING_ID_IETYPE,EG_IE_CONDITIONAL_OPTIONAL,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmCRTSESRSPCb->posIndex[1][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES )));
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmCRTSESRSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

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

   /* Populate LDN IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_LDN_IETYPE,EG_IE_OPTIONAL,1,FALSE);

   gmCRTSESRSPCb->posIndex[1][EG_LDN_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate APCO IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_APCO_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_APCO_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmCRTSESRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmCRTSESRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bCRTSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes2bDELBREREQGmEntry

   Desc  : This function makes the GM Entry for DELBREREQ

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bDELBREREQGmEntry
(
LegMsgEntry               *gmDELBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bDELBREREQGmEntry(gmDELBREREQCb)
LegMsgEntry               *gmDELBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bDELBREREQGmEntry);

   /* Populate the DELBREREQ GM Entry */
   gmDELBREREQCb->msgType = EG_S2B_DELBREREQ_CMD_CODE;
   gmDELBREREQCb->nmbIeProp = EG_S2B_DELBREREQ_NMB_IES;
   gmDELBREREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELBREREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,1,FALSE);

   gmDELBREREQCb->posIndex[1][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
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

} /* End of egMakes2bDELBREREQGmEntry */


/***********************************************************************

   Func  : egMakes2bDELBRERSPGmEntry

   Desc  : This function makes the GM Entry for DELBRERSP

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bDELBRERSPGmEntry
(
LegMsgEntry               *gmDELBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bDELBRERSPGmEntry(gmDELBRERSPCb)
LegMsgEntry               *gmDELBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bDELBRERSPGmEntry);

   /* Populate the DELBRERSP GM Entry */
   gmDELBRERSPCb->msgType = EG_S2B_DELBRERSP_CMD_CODE;
   gmDELBRERSPCb->nmbIeProp = EG_S2B_DELBRERSP_NMB_IES;
   gmDELBRERSPCb->validateIe = (EscFun)NULLP;

   /* Populate DELBRERSP IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate BEARER_CNTX IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_BEARER_CNTX_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_BEARER_CNTX_IETYPE]  = ieCnt + 1;

   grpCnt = 0;
   ret = SGetSBuf(DFLT_REGION,DFLT_POOL,(Data **)&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe) ,sizeof(LegGroupedIe));
   if( ret == ROK && (gmDELBRERSPCb->ieProperties[ieCnt].groupedIe) != NULLP)
   {

      ieProperties = &gmDELBRERSPCb->ieProperties[ieCnt].groupedIe->ieProperties;
      ret = SGetSBuf(DFLT_REGION,DFLT_POOL ,(Data **)(ieProperties),(sizeof(LegIeProperties)*(EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES )));
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmDELBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

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
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,3,FALSE);

   gmDELBRERSPCb->posIndex[3][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bDELBRERSPGmEntry */


/***********************************************************************

   Func  : egMakes2bDELPDNCONSETREQGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETREQ

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bDELPDNCONSETREQGmEntry
(
LegMsgEntry               *gmDELPDNCONSETREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bDELPDNCONSETREQGmEntry(gmDELPDNCONSETREQCb)
LegMsgEntry               *gmDELPDNCONSETREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bDELPDNCONSETREQGmEntry);

   /* Populate the DELPDNCONSETREQ GM Entry */
   gmDELPDNCONSETREQCb->msgType = EG_S2B_DELPDNCONSETREQ_CMD_CODE;
   gmDELPDNCONSETREQCb->nmbIeProp = EG_S2B_DELPDNCONSETREQ_NMB_IES;
   gmDELPDNCONSETREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELPDNCONSETREQ IE Properties */

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,3,FALSE);

   gmDELPDNCONSETREQCb->posIndex[3][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELPDNCONSETREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELPDNCONSETREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bDELPDNCONSETREQGmEntry */


/***********************************************************************

   Func  : egMakes2bDELPDNCONSETRSPGmEntry

   Desc  : This function makes the GM Entry for DELPDNCONSETRSP

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bDELPDNCONSETRSPGmEntry
(
LegMsgEntry               *gmDELPDNCONSETRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bDELPDNCONSETRSPGmEntry(gmDELPDNCONSETRSPCb)
LegMsgEntry               *gmDELPDNCONSETRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bDELPDNCONSETRSPGmEntry);

   /* Populate the DELPDNCONSETRSP GM Entry */
   gmDELPDNCONSETRSPCb->msgType = EG_S2B_DELPDNCONSETRSP_CMD_CODE;
   gmDELPDNCONSETRSPCb->nmbIeProp = EG_S2B_DELPDNCONSETRSP_NMB_IES;
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

} /* End of egMakes2bDELPDNCONSETRSPGmEntry */


/***********************************************************************

   Func  : egMakes2bDELSESREQGmEntry

   Desc  : This function makes the GM Entry for DELSESREQ

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bDELSESREQGmEntry
(
LegMsgEntry               *gmDELSESREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bDELSESREQGmEntry(gmDELSESREQCb)
LegMsgEntry               *gmDELSESREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bDELSESREQGmEntry);

   /* Populate the DELSESREQ GM Entry */
   gmDELSESREQCb->msgType = EG_S2B_DELSESREQ_CMD_CODE;
   gmDELSESREQCb->nmbIeProp = EG_S2B_DELSESREQ_NMB_IES;
   gmDELSESREQCb->validateIe = (EscFun)NULLP;

   /* Populate DELSESREQ IE Properties */

   /* Populate EBI IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_EBI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_EBI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELSESREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELSESREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bDELSESREQGmEntry */


/***********************************************************************

   Func  : egMakes2bDELSESRSPGmEntry

   Desc  : This function makes the GM Entry for DELSESRSP

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bDELSESRSPGmEntry
(
LegMsgEntry               *gmDELSESRSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bDELSESRSPGmEntry(gmDELSESRSPCb)
LegMsgEntry               *gmDELSESRSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bDELSESRSPGmEntry);

   /* Populate the DELSESRSP GM Entry */
   gmDELSESRSPCb->msgType = EG_S2B_DELSESRSP_CMD_CODE;
   gmDELSESRSPCb->nmbIeProp = EG_S2B_DELSESRSP_NMB_IES;
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

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmDELSESRSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmDELSESRSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bDELSESRSPGmEntry */


/***********************************************************************

   Func  : egMakes2bMODBRECMDGmEntry

   Desc  : This function makes the GM Entry for MODBRECMD

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bMODBRECMDGmEntry
(
LegMsgEntry               *gmMODBRECMDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bMODBRECMDGmEntry(gmMODBRECMDCb)
LegMsgEntry               *gmMODBRECMDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bMODBRECMDGmEntry);

   /* Populate the MODBRECMD GM Entry */
   gmMODBRECMDCb->msgType = EG_S2B_MODBRECMD_CMD_CODE;
   gmMODBRECMDCb->nmbIeProp = EG_S2B_MODBRECMD_NMB_IES;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmMODBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmMODBRECMDCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmMODBRECMDCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmMODBRECMDCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bMODBRECMDGmEntry */


/***********************************************************************

   Func  : egMakes2bMODBREFLRINDGmEntry

   Desc  : This function makes the GM Entry for MODBREFLRIND

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bMODBREFLRINDGmEntry
(
LegMsgEntry               *gmMODBREFLRINDCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bMODBREFLRINDGmEntry(gmMODBREFLRINDCb)
LegMsgEntry               *gmMODBREFLRINDCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bMODBREFLRINDGmEntry);

   /* Populate the MODBREFLRIND GM Entry */
   gmMODBREFLRINDCb->msgType = EG_S2B_MODBREFLRIND_CMD_CODE;
   gmMODBREFLRINDCb->nmbIeProp = EG_S2B_MODBREFLRIND_NMB_IES;
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

} /* End of egMakes2bMODBREFLRINDGmEntry */


/***********************************************************************

   Func  : egMakes2bTRCSESACTGmEntry

   Desc  : This function makes the GM Entry for TRCSESACT

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bTRCSESACTGmEntry
(
LegMsgEntry               *gmTRCSESACTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bTRCSESACTGmEntry(gmTRCSESACTCb)
LegMsgEntry               *gmTRCSESACTCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bTRCSESACTGmEntry);

   /* Populate the TRCSESACT GM Entry */
   gmTRCSESACTCb->msgType = EG_S2B_TRCSESACT_CMD_CODE;
   gmTRCSESACTCb->nmbIeProp = EG_S2B_TRCSESACT_NMB_IES;
   gmTRCSESACTCb->validateIe = (EscFun)NULLP;

   /* Populate TRCSESACT IE Properties */

   /* Populate IMSI IE Properties */
   egMakeAppIeProp(&gmTRCSESACTCb->ieProperties[ieCnt],
                    EG_IMSI_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmTRCSESACTCb->posIndex[0][EG_IMSI_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate TRACE_INFO IE Properties */
   egMakeAppIeProp(&gmTRCSESACTCb->ieProperties[ieCnt],
                    EG_TRACE_INFO_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmTRCSESACTCb->posIndex[0][EG_TRACE_INFO_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bTRCSESACTGmEntry */


/***********************************************************************

   Func  : egMakes2bTRCSESDEACTGmEntry

   Desc  : This function makes the GM Entry for TRCSESDEACT

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bTRCSESDEACTGmEntry
(
LegMsgEntry               *gmTRCSESDEACTCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bTRCSESDEACTGmEntry(gmTRCSESDEACTCb)
LegMsgEntry               *gmTRCSESDEACTCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bTRCSESDEACTGmEntry);

   /* Populate the TRCSESDEACT GM Entry */
   gmTRCSESDEACTCb->msgType = EG_S2B_TRCSESDEACT_CMD_CODE;
   gmTRCSESDEACTCb->nmbIeProp = EG_S2B_TRCSESDEACT_NMB_IES;
   gmTRCSESDEACTCb->validateIe = (EscFun)NULLP;

   /* Populate TRCSESDEACT IE Properties */

   /* Populate TRACE_REF IE Properties */
   egMakeAppIeProp(&gmTRCSESDEACTCb->ieProperties[ieCnt],
                    EG_TRACE_REF_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmTRCSESDEACTCb->posIndex[0][EG_TRACE_REF_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bTRCSESDEACTGmEntry */


/***********************************************************************

   Func  : egMakes2bUPDBREREQGmEntry

   Desc  : This function makes the GM Entry for UPDBREREQ

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bUPDBREREQGmEntry
(
LegMsgEntry               *gmUPDBREREQCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bUPDBREREQGmEntry(gmUPDBREREQCb)
LegMsgEntry               *gmUPDBREREQCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bUPDBREREQGmEntry);

   /* Populate the UPDBREREQ GM Entry */
   gmUPDBREREQCb->msgType = EG_S2B_UPDBREREQ_CMD_CODE;
   gmUPDBREREQCb->nmbIeProp = EG_S2B_UPDBREREQ_NMB_IES;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_TFT_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

   egMakeAppChildIeEntry(&(gmUPDBREREQCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_BEARER_QOS_IETYPE,EG_IE_CONDITIONAL,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate PROCEDURE_TID IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PROCEDURE_TID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PROCEDURE_TID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate AMBR IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_AMBR_IETYPE,EG_IE_MANDATORY,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_AMBR_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBREREQCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBREREQCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bUPDBREREQGmEntry */


/***********************************************************************

   Func  : egMakes2bUPDBRERSPGmEntry

   Desc  : This function makes the GM Entry for UPDBRERSP

   Ret   : None

   File  : smegs2b.c

************************************************************************/




#ifdef ANSI
PRIVATE void egMakes2bUPDBRERSPGmEntry
(
LegMsgEntry               *gmUPDBRERSPCb                         /* GM Control Block */
)
#else
PRIVATE void egMakes2bUPDBRERSPGmEntry(gmUPDBRERSPCb)
LegMsgEntry               *gmUPDBRERSPCb;
#endif
{
   U8 ieCnt = 0;
   U8 grpCnt = 0;
   U8 grpCnt1 = 0;
   U8 ret = RFAILED;
   LegIeProperties **ieProperties;
   TRC2(egMakes2bUPDBRERSPGmEntry);

   /* Populate the UPDBRERSP GM Entry */
   gmUPDBRERSPCb->msgType = EG_S2B_UPDBRERSP_CMD_CODE;
   gmUPDBRERSPCb->nmbIeProp = EG_S2B_UPDBRERSP_NMB_IES;
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
      if( ret == ROK && ieProperties != NULLP)
      {

   egMakeAppChildIeEntry(&(gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_EBI_IETYPE,EG_IE_MANDATORY,0,FALSE,
EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES);

   egMakeAppChildIeEntry(&(gmUPDBRERSPCb->ieProperties[ieCnt].groupedIe), grpCnt++, EG_CAUSE_IETYPE,EG_IE_MANDATORY,0,FALSE,
0 );

}

}
   ieCnt++;

   /* Populate RECOVERY IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_RECOVERY_IETYPE,EG_IE_CONDITIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_RECOVERY_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate CSID IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_CSID_IETYPE,EG_IE_CONDITIONAL,3,FALSE);

   gmUPDBRERSPCb->posIndex[3][EG_CSID_IETYPE]  = ieCnt + 1;
   ieCnt++;

   /* Populate PRIVATE_EXT IE Properties */
   egMakeAppIeProp(&gmUPDBRERSPCb->ieProperties[ieCnt],
                    EG_PRIVATE_EXT_IETYPE,EG_IE_OPTIONAL,0,FALSE);

   gmUPDBRERSPCb->posIndex[0][EG_PRIVATE_EXT_IETYPE]  = ieCnt + 1;

} /* End of egMakes2bUPDBRERSPGmEntry */
/***********************************************************************

   Func  : egMakes2bGmDict

   Desc  : This functions populates eGTP Messages in GM Dictionary

   Ret   : EG_ROK

   File  : smegs2b.c

************************************************************************/


#ifdef ANSI
PUBLIC S16 egMakes2bGmDict
(
LegMsgEntry *gmEntry,
U8        *nmbGmMsgs
)
#else
PUBLIC S16 egMakes2bGmDict(gmEntry,nmbGmMsgs)
LegMsgEntry *gmEntry;
U8        *nmbGmMsgs;
#endif
{
   U8  gmMsgCount;                  /* Number of eGTP Messages inserted */
   gmMsgCount = 0;

   TRC2(egMakes2bGmDict);

   /* Populating CRTBREREQ GM Entry */
   egMakes2bCRTBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTBRERSP GM Entry */
   egMakes2bCRTBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESREQ GM Entry */
   egMakes2bCRTSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating CRTSESRSP GM Entry */
   egMakes2bCRTSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBREREQ GM Entry */
   egMakes2bDELBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELBRERSP GM Entry */
   egMakes2bDELBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETREQ GM Entry */
   egMakes2bDELPDNCONSETREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELPDNCONSETRSP GM Entry */
   egMakes2bDELPDNCONSETRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESREQ GM Entry */
   egMakes2bDELSESREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating DELSESRSP GM Entry */
   egMakes2bDELSESRSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBRECMD GM Entry */
   egMakes2bMODBRECMDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating MODBREFLRIND GM Entry */
   egMakes2bMODBREFLRINDGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRCSESACT GM Entry */
   egMakes2bTRCSESACTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating TRCSESDEACT GM Entry */
   egMakes2bTRCSESDEACTGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBREREQ GM Entry */
   egMakes2bUPDBREREQGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;

   /* Populating UPDBRERSP GM Entry */
   egMakes2bUPDBRERSPGmEntry(&gmEntry[gmMsgCount]);
   gmMsgCount++;
   *nmbGmMsgs = gmMsgCount;
   RETVALUE(ROK);
} /* End of geMakes2bGmDict */


/********************************************************************30**

         End of file:     egcodetool.cpp@@/main/3 - Fri Feb  4 18:18:18 2011

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
/main/1      eg009.201  asaurabh    1. Initial Release for S2B interface 
*********************************************************************91*/

