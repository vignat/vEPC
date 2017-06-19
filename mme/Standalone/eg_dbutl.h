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

    Name: Source file containing IE COdes 

    Type: C Source

    Desc: 

    File: eg_dbutl.c

    Sid:      eg_dbutl.h@@/main/1 - Fri Feb  4 18:18:09 2011

    Prg : an

************************************************************************/
#ifdef EGTP_C

/*  Interface IE Codes */

/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
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
#define EG_DETACH_TYPE_IETYPE                                        150       
#define EG_LDN_IETYPE                                                151       
#define EG_NODE_FEATR_IETYPE                                         152       
#define EG_MBMS_TIME_TO_DATA_TRANS_IETYPE                            153       
#define EG_THROTTLING_IETYPE                                         154       
#define EG_ARP_IETYPE                                                155       
#define EG_EPC_TMR_IETYPE                                            156       
#define EG_SIG_PRIOR_IND_IETYPE                                      157       
#define EG_TMGI_IETYPE                                               158       
#define EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE                              159       
#define EG_ADD_FLAG_FOR_SRVCC_IETYPE                                 160       
#define EG_MDT_CONF_IETYPE                                           162       
#define EG_APCO_IETYPE                                               163       
#define EG_CNG_TO_RPT_FLG_IETYPE                                     167       
#define EG_CAUSE_IETYPE                                              2         
#define EG_PRIVATE_EXT_IETYPE                                        255       
#define EG_RECOVERY_IETYPE                                           3         
#define EG_STN_SR_IETYPE                                             51        
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

/* Mavnir changes */


/* Message Codes */

#endif /* End of EGTP_C */
#ifdef EGTP_U

/* GTP-U Message Types */
#define EGU_ECHO_REQ          1
#define EGU_ECHO_RSP          2
#define EGU_ERROR_IND         26
#define EGU_SUPP_EXT_HDR_NTF  31
#define EGU_END_MARKER        254

/* GTP-U IEs */
#define EGU_RECOVERY_IETYPE   14 
#define EGU_TEID_IETYPE       16
#define EGU_GSN_ADDR_IETYPE   133
#define EGU_EXT_HDR_LST_IETYPE 141
#define EGU_PVT_EXTN_IETYPE   255


#endif /* End of EGTP_U */
/**********************************************************************
         End of file:     eg_dbutl.h@@/main/1 - Fri Feb  4 18:18:09 2011

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
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
            eg009.201 asaurabh     1. Added New IE types for S2A and S2B 
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

