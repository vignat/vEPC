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

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: smegs11.h

    Sid:      smegs11.h@@/main/3 - Fri Feb  4 18:18:29 2011

    Prg: an

************************************************************************/


/***********************************************************************
    This file contains automatically generated code
************************************************************************/
/*  Interface IE Codes */

#define EG_IMSI_IETYPE                                               1         
#define EG_CHARGING_CHAR_IETYPE                                      100       
#define EG_TRACE_INFO_IETYPE                                         101       
#define EG_BEARER_FLAGS_IETYPE                                       102       
#define EG_PAGING_CAUSE_IETYPE                                       103       
#define EG_PDN_TYPE_IETYPE                                           104       
#define EG_PROCEDURE_TID_IETYPE                                      105       
#define EG_UE_NTWRK_CAPB_IETYPE                                      107       
#define EG_PDU_NUMBER_IETYPE                                         108       
#define EG_GSM_KEY_TRIP_IETYPE                                       109       
#define EG_UMTS_KEY_CIPH_QUIN_IETYPE                                 110       
#define EG_GSM_KEY_CIPH_QUIN_IETYPE                                  111       
#define EG_UMTS_KEY_QUIN_IETYPE                                      112       
#define EG_EPS_SEC_QUAD_QUIN_IETYPE                                  113       
#define EG_UMTS_KEY_QUAD_QUIN_IETYPE                                 114       
#define EG_PDN_CONN_IETYPE                                           115       
#define EG_GRE_KEY_IETYPE                                            116       
#define EG_BEARER_CMODE_IETYPE                                       117       
#define EG_CNTXT_PRIOR_IETYPE                                        118       
#define EG_LMA_IP_ADDR_IETYPE                                        119       
#define EG_PTMSI_IETYPE                                              120       
#define EG_PTMSI_SIG_IETYPE                                          121       
#define EG_HOP_CNTR_IETYPE                                           122       
#define EG_AUTH_QUINTUP_IETYPE                                       123       
#define EG_AUTH_QUADRUP_IETYPE                                       124       
#define EG_COMP_REQ_MSG_IETYPE                                       125       
#define EG_GUTI_IETYPE                                               126       
#define EG_F_CONTAINER_IETYPE                                        127       
#define EG_F_CAUSE_IETYPE                                            128       
#define EG_SEL_PLMN_ID_IETYPE                                        129       
#define EG_TARGET_ID_IETYPE                                          130       
#define EG_CELL_ID_IETYPE                                            131       
#define EG_NSAPI_IETYPE                                              132       
#define EG_PKT_FLOW_ID_IETYPE                                        133       
#define EG_SRC_RNC_PDCP_CTX_IETYPE                                   135       
#define EG_PORT_IETYPE                                               136       
#define EG_APN_RESTRICT_IETYPE                                       137       
#define EG_TRANS_ID_IETYPE                                           137       
#define EG_SEL_MODE_IETYPE                                           138       
#define EG_CNG_RPT_ACT_IETYPE                                        139       
#define EG_EMB_OFF_IE_IETYPE                                         140       
#define EG_CSID_IETYPE                                               141       
#define EG_CAUSE_IETYPE                                              2         
#define EG_PRIVATE_EXT_IETYPE                                        255       
#define EG_RECOVERY_IETYPE                                           3         
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
#define EG_BEARER_TFT_IETYPE                                         85        
#define EG_TAD_IETYPE                                                86        
#define EG_ULI_IETYPE                                                87        
#define EG_FTEID_IETYPE                                              88        
#define EG_TMSI_IETYPE                                               89        
#define EG_GBL_CN_ID_IETYPE                                          90        
#define EG_LEGACY_QOS_IETYPE                                         91        
#define EG_S103_PDN_FW_INFO_IETYPE                                   92        
#define EG_S1U_DATA_FW_IETYPE                                        93        
#define EG_DELAY_IETYPE                                              94        
#define EG_BEARER_ID_LIST_IETYPE                                     95        
#define EG_BEARER_CNTX_IETYPE                                        96        
#define EG_S101_IP_ADDR_IETYPE                                       97        
#define EG_S102_IP_ADDR_IETYPE                                       98        
#define EG_CHARGING_ID_IETYPE                                        99        


/* Message Codes   */

#define EG_S11_BRERESCMD_CMD_CODE      68        
#define EG_S11_BRERESCMD_NMB_IES       5         
#define EG_S11_BRERESFLRIND_CMD_CODE   69        
#define EG_S11_BRERESFLRIND_NMB_IES    6         
#define EG_S11_CHNGNOTREQ_CMD_CODE     40        
#define EG_S11_CHNGNOTREQ_NMB_IES      4         
#define EG_S11_CHNGNOTRSP_CMD_CODE     41        
#define EG_S11_CHNGNOTRSP_NMB_IES      4         
#define EG_S11_CRTBREREQ_CMD_CODE      95        
#define EG_S11_CRTBREREQ_NMB_IES       8         
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 10        
#define EG_S11_CRTBRERSP_CMD_CODE      96        
#define EG_S11_CRTBRERSP_NMB_IES       6         
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 8         
#define EG_S11_CRTFWDTUNREQ_CMD_CODE   160       
#define EG_S11_CRTFWDTUNREQ_NMB_IES    2         
#define EG_CRTFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_S11_CRTFWDTUNRSP_CMD_CODE   161       
#define EG_S11_CRTFWDTUNRSP_NMB_IES    4         
#define EG_CRTFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES 7         
#define EG_S11_CRTINDDATFWDTUNREQ_CMD_CODE 166       
#define EG_S11_CRTINDDATFWDTUNREQ_NMB_IES 1         
#define EG_S11_CRTINDDATFWDTUNRSP_CMD_CODE 167       
#define EG_S11_CRTINDDATFWDTUNRSP_NMB_IES 2         
#define EG_S11_CRTSESREQ_CMD_CODE      32        
#define EG_S11_CRTSESREQ_NMB_IES       23        
#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 9         
#define EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S11_CRTSESRSP_CMD_CODE      33        
#define EG_S11_CRTSESRSP_NMB_IES       17        
#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 12        
#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 3         
#define EG_S11_DELBRECMD_CMD_CODE      66        
#define EG_S11_DELBRECMD_NMB_IES       2         
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 1         
#define EG_S11_DELBREFLRIND_CMD_CODE   67        
#define EG_S11_DELBREFLRIND_NMB_IES    5         
#define EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S11_DELBREREQ_CMD_CODE      99        
#define EG_S11_DELBREREQ_NMB_IES       8         
#define EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES 1         
#define EG_S11_DELBRERSP_CMD_CODE      100       
#define EG_S11_DELBRERSP_NMB_IES       8         
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S11_DELSESREQ_CMD_CODE      38        
#define EG_S11_DELSESREQ_NMB_IES       4         
#define EG_S11_DELSESRSP_CMD_CODE      39        
#define EG_S11_DELSESRSP_NMB_IES       5         
#define EG_S11_DWLNKDATNOT_CMD_CODE    176       
#define EG_S11_DWLNKDATNOT_NMB_IES     1         
#define EG_S11_DWLNKDATNOTACK_CMD_CODE 177       
#define EG_S11_DWLNKDATNOTACK_NMB_IES  5         
#define EG_S11_DWLNKDATNOTFLR_CMD_CODE 70        
#define EG_S11_DWLNKDATNOTFLR_NMB_IES  3         
#define EG_S11_MODBRECMD_CMD_CODE      64        
#define EG_S11_MODBRECMD_NMB_IES       3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S11_MODBREFLRIND_CMD_CODE   65        
#define EG_S11_MODBREFLRIND_NMB_IES    5         
#define EG_MODBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S11_MODBREREQ_CMD_CODE      36        
#define EG_S11_MODBREREQ_NMB_IES       13        
#define EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES 8         
#define EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S11_MODBRERSP_CMD_CODE      37        
#define EG_S11_MODBRERSP_NMB_IES       10        
#define EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES 3         
#define EG_S11_RESUMEACK_CMD_CODE      165       
#define EG_S11_RESUMEACK_NMB_IES       2         
#define EG_S11_RESUMENOT_CMD_CODE      164       
#define EG_S11_RESUMENOT_NMB_IES       2         
#define EG_S11_SUSPENDACK_CMD_CODE     163       
#define EG_S11_SUSPENDACK_NMB_IES      2         
#define EG_S11_SUSPENDNOT_CMD_CODE     162       
#define EG_S11_SUSPENDNOT_NMB_IES      2         
#define EG_S11_UPDBEARERCMP_CMD_CODE   178       
#define EG_S11_UPDBEARERCMP_NMB_IES    3         
#define EG_UPDBEARERCMP_BEARER_CNTX_GRP_NMB_IES 2         
#define EG_S11_UPDBREREQ_CMD_CODE      97        
#define EG_S11_UPDBREREQ_NMB_IES       8         
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_S11_UPDBRERSP_CMD_CODE      98        
#define EG_S11_UPDBRERSP_NMB_IES       6         
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S11_UPDUPLNREQ_CMD_CODE     34        
#define EG_S11_UPDUPLNREQ_NMB_IES      4         
#define EG_UPDUPLNREQ_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_UPDUPLNREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S11_UPDUPLNRSP_CMD_CODE     35        
#define EG_S11_UPDUPLNRSP_NMB_IES      5         
#define EG_UPDUPLNRSP_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_UPDUPLNRSP_BEARER_CNTX1_GRP_NMB_IES 3         


/********************************************************************30**

         End of file:     smegs11.h@@/main/3 - Fri Feb  4 18:18:29 2011

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
/main/2      ---      sv                1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
*********************************************************************91*/

