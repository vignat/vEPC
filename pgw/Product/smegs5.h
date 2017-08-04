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

    File: smegs5.h

    Sid:      smegs5.h@@/main/3 - Fri Feb  4 18:18:30 2011

    Prg : an

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
#define EG_DRX_PARAM_IETYPE                                          106       
#define EG_UE_NET_CAP_IETYPE                                         107       
#define EG_PDN_CONN_IETYPE                                           115       
#define EG_BEARER_CMODE_IETYPE                                       117       
#define EG_LMA_IP_ADDR_IETYPE                                        119       
#define EG_P_TMSI_IETYPE                                             120       
#define EG_P_TMSI_SIG_IETYPE                                         121       
#define EG_HOP_COUNTER_IETYPE                                        122       
#define EG_SEL_PLMN_ID_IETYPE                                        129       
#define EG_NSAPI_IETYPE                                              132       
#define EG_APN_RESTRICT_IETYPE                                       137       
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
#define EG_IP_ADDRESS_IETYPE                                         74        
#define EG_MEI_IETYPE                                                75        
#define EG_MSISDN_IETYPE                                             76        
#define EG_INDICATION_IETYPE                                         77        
#define EG_PCO_IETYPE                                                78        
#define EG_PAA_IETYPE                                                79        
#define EG_BEARER_QOS_IETYPE                                         80        
#define EG_FLOW_QOS_IETYPE                                           81        
#define EG_RAT_TYPE_IETYPE                                           82        
#define EG_SERVING_NWK_IETYPE                                        83        
#define EG_TEID_C_IETYPE                                             84        
#define EG_BEARER_TFT_IETYPE                                         85        
#define EG_TAD_IETYPE                                                86        
#define EG_ULI_IETYPE                                                87        
#define EG_FTEID_IETYPE                                              88        
#define EG_TMSI_IETYPE                                               89        
#define EG_LEGACY_QOS_IETYPE                                         91        
/* ccpu00138260 */        
#define EG_BEARER_CNTX_IETYPE                                        93 
#define EG_DELAY_IETYPE                                              94        
#define EG_BEARER_ID_LIST_IETYPE                                     95        
#define EG_S101_IP_ADD_IETYPE                                        97        
#define EG_S102_IP_ADD_IETYPE                                        98        
#define EG_CHARGING_ID_IETYPE                                        99        
#define EG_GRE_KEY_IETYPE                                            116


/* Message Codes   */
 /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */

#define EG_S5_BRERESCMD_CMD_CODE       68        
#define EG_S5_BRERESCMD_NMB_IES        5         
#define EG_S5_BRERESFLRIND_CMD_CODE    69        
#define EG_S5_BRERESFLRIND_NMB_IES     5         
#define EG_S5_CRTBREREQ_CMD_CODE       95        
#define EG_S5_CRTBREREQ_NMB_IES        9         
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 10        
#define EG_S5_CRTBRERSP_CMD_CODE       96        
#define EG_S5_CRTBRERSP_NMB_IES        4         
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 7         
#define EG_S5_CRTSESREQ_CMD_CODE       32        
#define EG_S5_CRTSESREQ_NMB_IES        23        
#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 9         
#define EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S5_CRTSESRSP_CMD_CODE       33        
#define EG_S5_CRTSESRSP_NMB_IES        16        
#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 11        
#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S5_DELBRECMD_CMD_CODE       66        
#define EG_S5_DELBRECMD_NMB_IES        2         
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 1         
#define EG_S5_DELBREFLRIND_CMD_CODE    67        
#define EG_S5_DELBREFLRIND_NMB_IES     4         
#define EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_DELBREREQ_CMD_CODE       99        
#define EG_S5_DELBREREQ_NMB_IES        8         
#define EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES 1         
#define EG_S5_DELBRERSP_CMD_CODE       100       
#define EG_S5_DELBRERSP_NMB_IES        7         
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_DELSESREQ_CMD_CODE       38        
#define EG_S5_DELSESREQ_NMB_IES        4         
#define EG_S5_DELSESRSP_CMD_CODE       39        
#define EG_S5_DELSESRSP_NMB_IES        4         
#define EG_S5_MODBRECMD_CMD_CODE       64        
#define EG_S5_MODBRECMD_NMB_IES        3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_MODBREFLRIND_CMD_CODE    65        
#define EG_S5_MODBREFLRIND_NMB_IES     4         
#define EG_MODBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_MODBREREQ_CMD_CODE       36        
#define EG_S5_MODBREREQ_NMB_IES        13        
#define EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES 9         
#define EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S5_MODBRERSP_CMD_CODE       37        
#define EG_S5_MODBRERSP_NMB_IES        9         
#define EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S5_UPDBREREQ_CMD_CODE       97        
#define EG_S5_UPDBREREQ_NMB_IES        7         
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 8         
#define EG_S5_UPDBRERSP_CMD_CODE       98        
#define EG_S5_UPDBRERSP_NMB_IES        5         
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         


/********************************************************************30**

         End of file:     smegs5.h@@/main/3 - Fri Feb  4 18:18:30 2011

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
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

