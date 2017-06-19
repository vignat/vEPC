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

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: smegs2a.h

    Sid:      egcodetool.cpp@@/main/3 - Fri Feb  4 18:18:18 2011

    Prg: an

************************************************************************/

/*eg009.201:Initial Release for S2A*/
/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/*  Interface IE Codes */

#define EG_IMSI_IETYPE                                               1         
#define EG_PROCEDURE_TID_IETYPE                                      100       
#define EG_DRX_PARAM_IETYPE                                          101       
#define EG_UE_NTWRK_CAPB_IETYPE                                      102       
#define EG_GSM_KEY_TRIP_IETYPE                                       103       
#define EG_UMTS_KEY_CIPH_QUIN_IETYPE                                 104       
#define EG_GSM_KEY_CIPH_QUIN_IETYPE                                  105       
#define EG_UMTS_KEY_QUIN_IETYPE                                      106       
#define EG_EPS_SEC_QUAD_QUIN_IETYPE                                  107       
#define EG_UMTS_KEY_QUAD_QUIN_IETYPE                                 108       
#define EG_PDN_CONN_IETYPE                                           109       
#define EG_PDU_NUMBER_IETYPE                                         110       
#define EG_PTMSI_IETYPE                                              111       
#define EG_PTMSI_SIG_IETYPE                                          112       
#define EG_HOP_CNTR_IETYPE                                           113       
#define EG_UE_TIME_ZONE_IETYPE                                       114       
#define EG_TRACE_REF_IETYPE                                          115       
#define EG_COMP_REQ_MSG_IETYPE                                       116       
#define EG_GUTI_IETYPE                                               117       
#define EG_F_CONTAINER_IETYPE                                        118       
#define EG_F_CAUSE_IETYPE                                            119       
#define EG_SEL_PLMN_ID_IETYPE                                        120       
#define EG_TARGET_ID_IETYPE                                          121       
#define EG_PKT_FLOW_ID_IETYPE                                        123       
#define EG_RAB_CONTEXT_IETYPE                                        124       
#define EG_SRC_RNC_PDCP_CTX_IETYPE                                   125       
#define EG_PORT_IETYPE                                               126       
#define EG_APN_RESTRICT_IETYPE                                       127       
#define EG_SEL_MODE_IETYPE                                           128       
#define EG_SRC_ID_IETYPE                                             129       
#define EG_CNG_RPT_ACT_IETYPE                                        131       
#define EG_CSID_IETYPE                                               132       
#define EG_CHNL_NEEDED_IETYPE                                        133       
#define EG_EMLPP_PRIOR_IETYPE                                        134       
#define EG_NODE_TYPE_IETYPE                                          135       
#define EG_FQDN_IETYPE                                               136       
#define EG_TRANS_ID_IETYPE                                           137       
#define EG_MBMS_SESS_DURATION_IETYPE                                 138       
#define EG_MBMS_SERV_AREA_IETYPE                                     139       
#define EG_MBMS_SESS_ID_IETYPE                                       140       
#define EG_MBMS_FLOW_ID_IETYPE                                       141       
#define EG_MBMS_IP_MULT_DIST_IETYPE                                  142       
#define EG_MBMS_DIST_ACK_IETYPE                                      143       
#define EG_RFSP_ID_IETYPE                                            144       
#define EG_UCI_IETYPE                                                145       
#define EG_CSG_INFO_REP_ACT_IETYPE                                   146       
#define EG_CSG_ID_IETYPE                                             147       
#define EG_CMI_IETYPE                                                148       
#define EG_SERV_IND_IETYPE                                           149       
#define EG_TMGI_IETYPE                                               150       
#define EG_LDN_IETYPE                                                151       
#define EG_APCO_IETYPE                                               163       
#define EG_CAUSE_IETYPE                                              2         
#define EG_PRIVATE_EXT_IETYPE                                        255       
#define EG_RECOVERY_IETYPE                                           3         
#define EG_STN_SR_IETYPE                                             51        
#define EG_SRC_TRGT_TRANS_CONT_IETYPE                                52        
#define EG_TRGT_SRC_TRANS_CONT_IETYPE                                53        
#define EG_MME_CTXT_EUTRAN_SRVCC_IETYPE                              54        
#define EG_MME_CTXT_UTRAN_SRVCC_IETYPE                               55        
#define EG_SRVCC_CAUSE_IETYPE                                        56        
#define EG_TRGT_RNC_ID_IETYPE                                        57        
#define EG_TRGT_CELL_ID_IETYPE                                       58        
#define EG_SV_CP_TEID_IETYPE                                         59        
#define EG_SV_FLAG_IETYPE                                            60        
#define EG_APN_IETYPE                                                71        
#define EG_AMBR_IETYPE                                               72        
#define EG_EBI_IETYPE                                                73        
#define EG_IPADDR_IETYPE                                             74        
#define EG_MEI_IETYPE                                                75        
#define EG_MSISDN_IETYPE                                             76        
#define EG_INDICATION_IETYPE                                         77        
#define EG_PCO_IETYPE                                                78        
#define EG_PAA_IETYPE                                                79        
#define EG_BEARER_QOS_IETYPE                                         80        
#define EG_FLOW_QOS_IETYPE                                           81        
#define EG_RAT_TYPE_IETYPE                                           82        
#define EG_SERVING_NWK_IETYPE                                        83        
#define EG_BEARER_TFT_IETYPE                                         84        
#define EG_TAD_IETYPE                                                85        
#define EG_ULI_IETYPE                                                86        
#define EG_FTEID_IETYPE                                              87        
#define EG_TMSI_IETYPE                                               88        
#define EG_GBL_CN_ID_IETYPE                                          89        
#define EG_S103_PDN_FW_INFO_IETYPE                                   90        
#define EG_S1U_DATA_FW_IETYPE                                        91        
#define EG_DELAY_IETYPE                                              92        
#define EG_BEARER_CNTX_IETYPE                                        93        
#define EG_CHARGING_ID_IETYPE                                        94        
#define EG_CHARGING_CHAR_IETYPE                                      95        
#define EG_TRACE_INFO_IETYPE                                         96        
#define EG_BEARER_FLAGS_IETYPE                                       97        
#define EG_PDN_TYPE_IETYPE                                           99        


/* Message Codes   */

#define EG_S2A_CRTBREREQ_CMD_CODE      95        
#define EG_S2A_CRTBREREQ_NMB_IES       4         
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_S2A_CRTBRERSP_CMD_CODE      96        
#define EG_S2A_CRTBRERSP_NMB_IES       5         
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_S2A_CRTSESREQ_CMD_CODE      32        
#define EG_S2A_CRTSESREQ_NMB_IES       18        
#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S2A_CRTSESRSP_CMD_CODE      33        
#define EG_S2A_CRTSESRSP_NMB_IES       12        
#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S2A_DELBREREQ_CMD_CODE      99        
#define EG_S2A_DELBREREQ_NMB_IES       5         
#define EG_S2A_DELBRERSP_CMD_CODE      100       
#define EG_S2A_DELBRERSP_NMB_IES       5         
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S2A_DELPDNCONSETREQ_CMD_CODE 101       
#define EG_S2A_DELPDNCONSETREQ_NMB_IES 2         
#define EG_S2A_DELPDNCONSETRSP_CMD_CODE 102       
#define EG_S2A_DELPDNCONSETRSP_NMB_IES 2         
#define EG_S2A_DELSESREQ_CMD_CODE      36        
#define EG_S2A_DELSESREQ_NMB_IES       2         
#define EG_S2A_DELSESRSP_CMD_CODE      37        
#define EG_S2A_DELSESRSP_NMB_IES       3         
#define EG_S2A_MODBRECMD_CMD_CODE      64        
#define EG_S2A_MODBRECMD_NMB_IES       3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S2A_MODBREFLRIND_CMD_CODE   65        
#define EG_S2A_MODBREFLRIND_NMB_IES    3         
#define EG_S2A_TRCSESACT_CMD_CODE      71        
#define EG_S2A_TRCSESACT_NMB_IES       2         
#define EG_S2A_TRCSESDEACT_CMD_CODE    72        
#define EG_S2A_TRCSESDEACT_NMB_IES     1         
#define EG_S2A_UPDBREREQ_CMD_CODE      97        
#define EG_S2A_UPDBREREQ_NMB_IES       5         
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S2A_UPDBRERSP_CMD_CODE      98        
#define EG_S2A_UPDBRERSP_NMB_IES       5         
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         


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
/main/1     eg009.201   asaurabh        1. Initial Release for S2A
*********************************************************************91*/

