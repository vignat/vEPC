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
/********************************************************************20**
  
     Name:     e-GTP layer
  
     Type:     Non-code file
  
     Desc:     This file contains the Quality Metrics Report of eGTP Code.
  
     File:     eGTP.c
  
     Sid:      eGTP.c@@/main/3 - Fri Feb  4 18:18:18 2011
     
     Prg:      
  
*********************************************************************21*/

/******************************************************************************
                Code Complexity of the eGTP protocol software.
                ==============================================
--------------+---------------------------------------------------------------+
McCabe Metrics|       Number of Functions in the Complexity Range             |
[Complexity]  |-------------------+------------------+------------------------|
              | After Coding      |After Code Review | At the Time of Release |
--------------+-------------------+------------------+------------------------+
1-10  [GOOD]  |168        |  251             |   276                  |
--------------|-------------------|------------------|------------------------|
11-15 [FAIR]  | 19            |  24              |   23                   |
--------------|-------------------|------------------|------------------------|
16-30         | 13 [EDM=4,PCM=3]  |  18[TPT=3, MI=3] |   22 [MI= 4,  PCM=2]   | 
[COMPLEX]     |    [MI=2,UTIL=2]  |    [EDM=6,UTIL=2]|      [TMR= 1, TPT=4]   | 
              |    [TMR=1,TPT=1]  |    [PCM=1 TMR=1] |      [UTIL=3, EDM=6]   | 
         |                   |    [CPM=1, ACT=1]|      [CPM=1, ACT=1 ]   |
--------------|-------------------|------------------|------------------------|
31-42         | 03 [EDM=2, TPT=1] |  01[UTIL= 1]     |                        |
[UNMANAGEABLE]|                   |                  |                        | 
------------------------------------------------------------------------------+
Please see below the McCabe Report to know the complexity number of a function.
******************************************************************************
         ********************************************
         * McCabe Report at the time of Release 1.1 *
         ********************************************
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
pg_cpm.c   PUBLIC S16 egInitExt                         1       0
pg_cpm.c   PUBLIC S16 egCpmDeinit                       1       0
pg_edm.c   PRIVATE S16 egDecEG_STRING_132               1       0
pg_edm.c   PRIVATE S16 egDecEG_STRING_8                 1       0
pg_edm.c   PRIVATE S16 egEncPthHdr                      1       0
pg_edm.c   PRIVATE S16 egEncEG_TRC_INFO                 1       0
pg_edm.c   PRIVATE S16  egEncEG_STRING_8                1       0
pg_edm.c   PRIVATE S16 egDecEG_UNSIGNED_32              1       0
pg_edm.c   PRIVATE S16 egEncEG_EMB_CAUSE                1       0
pg_edm.c   PRIVATE S16 egEncEG_SNETWORK                 1       0
pg_edm.c   PUBLIC Size egGetIntfMem                     1       0
pg_edm.c   PRIVATE S16 egDecEG_STRING_32                1       0
pg_edm.c   PRIVATE S16 egDecEG_BQOS_IEDATA              1       0
pg_edm.c   PRIVATE S16 egDecEG_BEARER_FLAG              1       0
pg_edm.c   PRIVATE S16 egDecEG_UNSIGNED_16              1       0
pg_edm.c   PRIVATE S16 egEncEG_INDICATION               1       0
pg_edm.c   PRIVATE S16 egEncEG_STRING_32                1       0
pg_edm.c   PRIVATE S16 egEncEG_UNSIGNED_8               1       0
pg_edm.c   PRIVATE S16 egDecEG_EMB_CAUSE                1       0
pg_edm.c   PRIVATE S16 egDecEG_STRING_16                1       0
pg_edm.c   PRIVATE S16 egEncEG_UNSIGNED_32              1       0
pg_edm.c   PRIVATE S16 egEncEG_STRING_16                1       0
pg_edm.c   PRIVATE S16 egEncEG_BQOS_IEDATA              1       0
pg_edm.c   PRIVATE S16 egEncEG_FQOS                     1       0
pg_edm.c   PRIVATE S16 egEncEG_PVT_EXTN                 1       0
pg_edm.c   PRIVATE S16 egEncEG_STRING_MAX               1       0
pg_edm.c   PRIVATE S16 egEncEG_UNSIGNED_16              1       0
pg_edm.c   PRIVATE S16 egDecEG_PVT_EXTN                 1       0
pg_edm.c   PRIVATE S16 egDecEG_STRING_MAX               1       0
pg_edm.c   PRIVATE S16 egDecEG_UNSIGNED_8               1       0
pg_edm.c   PRIVATE S16 egEncEG_STRING_132               1       0
pg_mi.c    PRIVATE Void egMiFillResponsePost            1       0
pg_mi.c    PUBLIC Void egSendLmCntrlCfm                 1       0
pg_mi.c    PRIVATE Void egInitEgCb                      1       0
pg_mi.c    PUBLIC Void egMiTptSrvOpenInd                1       0
pg_tpt.c   PUBLIC S16 egTptInit                         1       0
pg_tpt.c   PUBLIC Void egTptFreeServer                  1       0
pg_tunn.c  PRIVATE S16 egTunnFsmConErr ARGS             1       0
pg_tunn.c  PRIVATE S16 egTunnFsmIncErr ARGS             1       0
pg_tunn.c  PUBLIC S16 egTunnInit ARGS                   1       0
pg_tunn.c  PRIVATE S16 egTunnFsmInvUsrEvnt ARGS         1       0
pg_tunn.c  PRIVATE S16 egTunnFsmOutIncRsp ARGS          1       0
pg_tunn.c  PRIVATE S16 egHandleIncEchoRsp ARGS          1       0
pg_tunn.c  PRIVATE S16 egTunnFsmOutErr ARGS             1       0
pg_tunn.c  PRIVATE S16 egTunnFsmInvNetEvnt ARGS         1       0
eg_dbs11.c PRIVATE void egMakes11MODBREFLRINDGmEntry    1       0
eg_dbs11.c PRIVATE void egMakes11CRTSESREQGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11UPDBRERSPGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11DELSESRSPGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11BRERESFLRINDGmEntry    1       0
eg_dbs11.c PRIVATE void egMakes11CRTFWDTUNREQGmEntry    1       0
eg_dbs11.c PRIVATE void egMakes11DELBREREQGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11DELBRECMDGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11CRTSESRSPGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11MODBREREQGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11MODBRECMDGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11CRTFWDTUNRSPGmEntry    1       0
eg_dbs11.c PRIVATE void egMakes11CRTBREREQGmEntry       1       0
eg_dbs11.c PUBLIC S16 egMakes11GmDict                   1       0
eg_dbs11.c PRIVATE void egMakes11UPDUPLNREQGmEntry      1       0
eg_dbs11.c PRIVATE void egMakes11DELBRERSPGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11BRERESCMDGmEntry       1       0
eg_dbs11.c PUBLIC S16 egMakeIeDict                      1       0
eg_dbs11.c PRIVATE void egMakes11DELBREFLRINDGmEntry    1       0
eg_dbs11.c PRIVATE void egMakes11UPDBREREQGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11MODBRERSPGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11DELSESREQGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11CRTBRERSPGmEntry       1       0
eg_dbs11.c PRIVATE void egMakes11UPDUPLNRSPGmEntry      1       0
eg_dbs11.c PRIVATE void egMakes11DWLNKDATNOTGmEntry     1       0
eg_dbs5.c  PRIVATE void egMakes5MODBREFLRINDGmEntry     1       0
eg_dbs5.c  PRIVATE void egMakes5BRERESFLRINDGmEntry     1       0
eg_dbs5.c  PRIVATE void egMakes5MODBRERSPGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5DELSESREQGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5CRTBREREQGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5DELBRERSPGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5UPDBREREQGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5DELSESRSPGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5CRTBRERSPGmEntry        1       0
eg_dbs5.c  PUBLIC S16 egMakes5GmDict                    1       0
eg_dbs5.c  PRIVATE void egMakes5DELBREFLRINDGmEntry     1       0
eg_dbs5.c  PRIVATE void egMakes5CRTSESRSPGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5CRTSESREQGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5UPDBRERSPGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5MODBREREQGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5MODBRECMDGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5DELBREREQGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5DELBRECMDGmEntry        1       0
eg_dbs5.c  PRIVATE void egMakes5BRERESCMDGmEntry        1       0
eg_dbutl.c PUBLIC Void egMakeAppIeProp                  1       0
eg_dbutl.c PUBLIC Void egMakeIeEntry                    1       0
eg_id.c    PUBLIC S16 egGetSId                          1       0
eg_ptli.c  PUBLIC S16 PtLiHitDiscReq                    1       0
eg_ptli.c  PUBLIC S16 PtLiHitUbndReq                    1       0
eg_ptli.c  PUBLIC S16 EgLiHitBndReq                     1       0
eg_ptli.c  PRIVATE S16 PtLiHitBndReq                    1       0
eg_ptli.c  PUBLIC S16 EgLiHitDiscReq                    1       0
eg_ptli.c  PUBLIC S16 EgLiHitUbndReq                    1       0
eg_ptmi.c  PUBLIC S16 PtMiLegStsCfm                     1       0
eg_ptmi.c  PUBLIC S16 PtMiLegStaCfm                     1       0
eg_ptmi.c  PUBLIC S16 EgMiLegStsCfm                     1       0
eg_ptmi.c  PUBLIC S16 EgMiLegTrcInd                     1       0
eg_ptmi.c  PUBLIC S16 EgMiLegStaCfm                     1       0
eg_ptmi.c  PUBLIC S16 PtMiLegTrcInd                     1       0
eg_ptmi.c  PUBLIC S16 PtMiLegCfgCfm                     1       0
eg_ptmi.c  PUBLIC S16 EgMiLegCntrlCfm                   1       0
eg_ptmi.c  PUBLIC S16 EgMiLegCfgCfm                     1       0
eg_ptmi.c  PUBLIC S16 PtMiLegStaInd                     1       0
eg_ptmi.c  PUBLIC S16 PtMiLegCntrlCfm                   1       0
eg_ptmi.c  PUBLIC S16 EgMiLegStaInd                     1       0
eg_ptui.c  PUBLIC S16 PtUiEgtSigInd                     1       0
eg_ptui.c  PUBLIC S16 EgUiEgtSigInd                     1       0
eg_ptui.c  PUBLIC S16 PtUiEgtLclDelCfm                  1       0
eg_ptui.c  PUBLIC S16 PtUiEgtErrInd                     1       0
eg_ptui.c  PUBLIC S16 PtUiEgtSigCfm                     1       0
eg_ptui.c  PUBLIC S16 EgUiEgtBndCfm                     1       0
eg_ptui.c  PUBLIC S16 PtUiEgtStaInd                     1       0
eg_ptui.c  PUBLIC S16 PtUiEgtBndCfm                     1       0
eg_ptui.c  PUBLIC S16 EgUiEgtLclDelCfm                  1       0
eg_ptui.c  PUBLIC S16 EgUiEgtErrInd                     1       0
eg_ptui.c  PUBLIC S16 EgUiEgtSigCfm                     1       0
eg_ptui.c  PUBLIC S16 EgUiEgtStaInd                     1       0
eg_ptui.c  PUBLIC S16 EgUiEgtStaCfm                     1       0
eg_ptui.c  PUBLIC S16 PtUiEgtStaCfm                     1       0
eg_tmr.c   PUBLIC Void egRmvTmr                         1       0
eg_tmr.c   PUBLIC Void egCmInitTimer                    1       0
eg_dbs11.c PRIVATE void egMakes11DWLNKDATNOTACKGmEnt    1       0
eg_dbs11.c PRIVATE void egMakes11DWLNKDATNOTFLRGmEnt    1       0
eg_tst.c   int main                                     1       1
eg_ui.c    PUBLIC S16 egUiFillEvntStrSigRsp             1       0
pg_cpm.c   PUBLIC Void egBcdToAsc                       2       0
pg_cpm.c   PUBLIC S16 egCmFreeEvent                     2       0
pg_cpm.c   PUBLIC S16 egCpmSigHdl                       2       0
pg_edm.c   PRIVATE S16 egDecEG_TRC_INFO                 2       0
pg_edm.c   PRIVATE S16 egDecEG_SNETWORK                 2       0
pg_edm.c   PRIVATE S16 egEncEG_BEARER_FLAG              2       0
pg_edm.c   PRIVATE S16 egDecEG_FQOS                     2       0
pg_mi.c    PUBLIC S16 egSendLmAlarm                     2       0
pg_mi.c    PRIVATE S16 egTSapStaReq                     2       0
pg_mi.c    PUBLIC S16 egFree                            2       0
pg_mi.c    PRIVATE S16 egGenStsReq                      2       0
pg_tpt.c   PUBLIC Void egTptDeInit                      2       0
pg_tpt.c   PUBLIC Void egTptIssueDiscReq                2       0
pg_tunn.c  PRIVATE EgTECCb** egGetTunnelCb ARGS         2       0
pg_tunn.c  PRIVATE S16 egReleaseTeid ARGS               2       0
pg_tunn.c  PUBLIC EgtMsgCat egGetMsgCategory ARGS       2       0
pg_tunn.c  PUBLIC S16 egInitEchoUsap ARGS               2       0
pg_tunn.c  PUBLIC S16 egDeInitEchoUsap ARGS             2       0
pg_tunn.c  PUBLIC Void egUpdateRxStatistics ARGS        2       0
pg_tunn.c  PUBLIC Void egUpdateTxStatistics ARGS        2       0
eg_li.c    PUBLIC S16 EgLiHitFlcInd                     2       0
eg_tmr.c   PUBLIC S16 egActvTmr                         2       0
eg_util.c  PUBLIC S16 EgUtilGetGtpHdr                   2       0
eg_util.c  PRIVATE S16 egUtilDupSNwk                    2       0
eg_util.c  PUBLIC S16 EgUtilSetGtpHdr                   2       0
eg_util.c  PRIVATE S16 egUtilDumpSNwk                   2       0
pg_cpm.c   PUBLIC S16 egCpmLclDelReq                    3       0
pg_edm.c   PUBLIC S16 egAllocIntfLst                    3       0
pg_edm.c   PRIVATE S16 egDecGrpIe                       3       0
pg_edm.c   PRIVATE S16 egEncPthIe                       3       0
pg_edm.c   PRIVATE S16 egEncodeGmHdr                    3       0
pg_edm.c   PUBLIC EgIeEntry* egGetIeEntryFrmDb          3       0
pg_edm.c   PRIVATE S16 egEncIeData                      3       0
pg_mi.c    PRIVATE S16 egUSapStaReq                     3       0
pg_mi.c    PRIVATE S16 egGenStaReq                      3       0
pg_tpt.c   PUBLIC S16 egUtlDelTknStrOSXL                3       0
pg_tpt.c   PRIVATE S16 egTptRmvTunnFrmLstWthMSB         3       0
pg_tpt.c   PRIVATE S16 egTptSendMsg                     3       0
pg_tunn.c  PRIVATE S16 egHandleIncEchoReq ARGS          3       1
pg_tunn.c  PUBLIC Void egSendErrInd ARGS                3       0
pg_tunn.c  PUBLIC S16 egTunnState ARGS                  3       0
pg_tunn.c  PUBLIC EgtIntf egGetIntfFrmTeid ARGS         3       0
pg_tunn.c  PRIVATE Void egTunnDelTECCb ARGS             3       0
pg_tunn.c  PRIVATE S16 egTunnFsmIdlISesReq ARGS         3       1
eg_tmr.c   PUBLIC S16 egRegInitTmr                      3       0
eg_util.c  PUBLIC U8 egGetDataFrmIeType                 3       0
eg_util.c  PRIVATE S16 egUtilDupIpv4Addr                3       0
eg_util.c  PUBLIC S16 EgUtilGmGetIeCnt                  3       0
eg_util.c  PRIVATE S16 egUtilDupIpv6Addr                3       0
eg_util.c  PUBLIC S16 EgUtilGmDelGrpIe                  3       0
pg_cpm.c   PUBLIC S16 egCmCreateEvent                   4       0
pg_cpm.c   PUBLIC Void egCmGetPortAndAscAddr            4       0
pg_cpm.c   PUBLIC Void egPrintEventStructure            4       0
pg_edm.c   PRIVATE S16 egValidatePthHdr                 4       0
pg_edm.c   PRIVATE S16 egDecIeData                      4       0
pg_edm.c   PUBLIC Void egDbCfgIeDict                    4       0
pg_mi.c    PRIVATE S16 egUnbindNDisableTSap             4       0
pg_mi.c    PRIVATE S16 egMiDelUsap                      4       0
pg_mi.c    PRIVATE S16 egUSapStsReq                     4       0
pg_tpt.c   PRIVATE S16 egTptUdpServerDatReq             4       0
pg_tpt.c   PRIVATE S16 egTptRmvTunnFrmLst               4       0
pg_tunn.c  PRIVATE S16 egHandleOutEchoRsp ARGS          4       0
pg_tunn.c  PUBLIC EgUSapCb *egGetUsapFrmIntf ARGS       4       0
pg_tunn.c  PUBLIC S16 egTunnDeInit ARGS                 4       0
pg_tunn.c  PUBLIC S16 egCpyCmTptAddr ARGS               4       0
pg_tunn.c  PUBLIC S16 egTunIndicate ARGS                4       0
pg_tunn.c  PRIVATE S16 egTunnFsmIncOutRsp ARGS          4       0
eg_tmr.c   PRIVATE Void egBndTmrExpiry                  4       0
eg_util.c  PUBLIC S16 EgUtilDeAllocGmMsg                4       0
eg_util.c  PUBLIC S16 EgUtilGmGetIe                     4       0
eg_util.c  PUBLIC S16 EgUtilAllocGmMsg                  4       0
pg_edm.c   PRIVATE S16 egEncVerIndMsg                   5       0
pg_edm.c   PRIVATE S16 egDecEG_FTEID                    5       0
pg_edm.c   PRIVATE S16 egValidatePthMsgIEs              5       0
pg_edm.c   PRIVATE S16 egEncEG_PAA_IEDATA               5       0
pg_edm.c   PRIVATE S16 egDecEG_PAA_IEDATA               5       0
pg_mi.c    PRIVATE S16 egMiDisableUsap                  5       0
pg_mi.c    PRIVATE S16 egMiDropGmCfg                    5       0
pg_mi.c    PRIVATE S16 egTSapStsReq                     5       0
pg_tpt.c   PUBLIC S16 egTptValIncMsgWthMsb              5       0
pg_tunn.c  PRIVATE S16 egSaveMsgRemDetails ARGS         5       0
pg_tunn.c  PRIVATE S16 egInitFreeTeidList ARGS          5       0
eg_li.c    PUBLIC S16 EgLiHitBndCfm                     5       0
eg_li.c    PRIVATE S16 egLiValidatePrimitive            5       0
eg_tmr.c   PRIVATE S8* egTmrGetEvntStr                  5       0
eg_ui.c    PUBLIC S16 EgUiEgtLclDelReq                  5       0
eg_ui.c    PUBLIC S16 EgUiEgtUbndReq                    5       0
eg_ui.c    PUBLIC S16 EgUiEgtBndReq                     5       0
eg_util.c  PUBLIC S16 EgUtilGmDelIe                     5       0
eg_util.c  PUBLIC S16 egUtilDropIeList                  5       0
eg_util.c  PRIVATE S16 egUtilDumpIpv4Addr               5       0
eg_util.c  PRIVATE S16 egUtilDupPaa                     5       0
eg_util.c  PRIVATE S16 egUtilDumpIpv6Addr               5       0
eg_util.c  PUBLIC S16 EgUtilGmDup                       5       0
pg_edm.c   PRIVATE S16 egDecPthHdr                      6       0
pg_edm.c   PRIVATE S16 egDecodeGmHdr                    6       0
pg_edm.c   PRIVATE S16 egEncEG_ULI_DATA                 6       0
pg_edm.c   PRIVATE S16 egDecEG_ULI_DATA                 6       0
pg_mi.c    PRIVATE S16 egTSapCfg                        6       0
pg_mi.c    PRIVATE S16 egValidateTSapCfg                6       0
pg_mi.c    PUBLIC S16 egSendLmCfm                       6       0
pg_tpt.c   PUBLIC S16 egTptQueueMsg                     6       0
pg_tpt.c   PUBLIC S16 egTptProcessLclDel                6       0
pg_tunn.c  PRIVATE S16 egDeInitTeidArr ARGS             6       0
pg_tunn.c  PRIVATE S16 egInitTeidArr ARGS               6       0
pg_tunn.c  PRIVATE S16 egTunnFsmIdlOSesReq ARGS         6       1
eg_tmr.c   PUBLIC Void egTmrEvnt                        6       0
eg_util.c  PUBLIC S16 egUtilCompareIeValue              6       0
eg_util.c  PUBLIC S16 EgUtilGmGetIeOccrCnt              6       0
eg_util.c  PUBLIC S16 egUtilDupList                     6       0
pg_cpm.c   PUBLIC S16 egCpmStaReq                       7       0
pg_edm.c   PRIVATE S16 egValidateGmHdr                  7       0
pg_edm.c   PRIVATE S16 egEncEG_FTEID                    7       0
pg_mi.c    PUBLIC Data *egAlloc                         7       0
pg_tpt.c   PUBLIC S16 egTptQueueMsgRmv                  7       0
pg_tpt.c   PUBLIC S16 egTptState                        7       0
pg_tpt.c   PUBLIC Void egTptCloseServer                 7       0
pg_tpt.c   PRIVATE S16 egTptValIncMsg                   7       0
pg_tpt.c   PUBLIC Void egTptDelAllServerContext         7       0
pg_tunn.c  PRIVATE S16 egTunnFsmConIncMsg ARGS          7       0
eg_tmr.c   PRIVATE Void egTptOpenSrvTmrExpiry           7       0
eg_util.c  PRIVATE S16 egUtilDupUliData                 7       0
pg_edm.c   PUBLIC EgMsgEntry* egDbGetMsgFrmGmDict       8       0
pg_edm.c   PRIVATE S16 egEncIe                          8       0
pg_edm.c   PUBLIC S16 egEdmDeInit                       8       0
pg_mi.c    PUBLIC S16 egShutdown                        8       0
pg_mi.c    PUBLIC Void egGenTrcInd                      8       0
pg_tpt.c   PRIVATE S16 egTptPartialReadMsg              8       0
pg_tunn.c  PRIVATE S16 egTunnFsmConOutMsg ARGS          8       1
eg_ui.c    PUBLIC S16 EgUiEgtStaReq                     8       0
eg_ui.c    PUBLIC S16 EgUiEgtSigRsp                     8       0
eg_util.c  PUBLIC S16 EgUtilGmAppendIe                  8       0
pg_edm.c   PUBLIC S16 egEncPthReq                       9       0
pg_mi.c    PRIVATE S16 egTSapCntrl                      9       0
pg_mi.c    PUBLIC S16 EgMiLegStsReq                     9       0
pg_mi.c    PRIVATE S16 egUSapCntrl                      9       0
pg_tpt.c   PUBLIC S16 egTptOpenServer                   9       0
pg_tpt.c   PUBLIC S16 egCmpTptAddr                      9       0
pg_tpt.c   PRIVATE S16 egTptQueueMsgRmvWthMsb           9       0
pg_tunn.c  PRIVATE S16 egObtainFreeTeid ARGS            9       0
pg_edm.c   PRIVATE S16 egEdmSendMsgToCm                10       0
pg_mi.c    PUBLIC Void egMiTptSrvCloseInd              10       0
pg_mi.c    PUBLIC S16 egActvInit                       10       0
pg_mi.c    PRIVATE S16 egValidateUSapCfg               10       0
pg_tunn.c  PUBLIC S16 egChkFailInd ARGS                10       0
eg_li.c    PUBLIC S16 EgLiHitDiscCfm                   10       0
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
pg_cpm.c   PUBLIC S16 egCpmInit                        11       0
pg_edm.c   PRIVATE S16 egValidateIeLen                 11       0
pg_edm.c   PUBLIC S16 egDecPthReq                      11       0
pg_mi.c    PUBLIC S16 EgMiLegStaReq                    11       0
pg_tunn.c  PRIVATE S16 egTunCreateReceive ARGS         11       0
pg_tunn.c  PRIVATE S16 egTunCreateSend ARGS            11       0
pg_tunn.c  PUBLIC S16 egTunReceive ARGS                11       0
pg_edm.c   PUBLIC S16 egDbDropGmDict                   12       0
pg_edm.c   PRIVATE S16 egMakeGrpIeEntry                12       0
pg_mi.c    PRIVATE S16 egGenCfg                        12       0
pg_mi.c    PRIVATE S16 egValidateGenCfg                12       0
pg_mi.c    PUBLIC S16 EgMiLegCntrlReq                  12       0
pg_tpt.c   PUBLIC S16 egTptProcessTsapServers          12       0
pg_tunn.c  PUBLIC S16 egTunSend ARGS                   12       0
eg_li.c    PUBLIC S16 EgLiHitDiscInd                   12       0
eg_util.c  PUBLIC S16 egUtilDelIe                      12       0
pg_edm.c   PRIVATE S16 egDecPthIe                      13       0
pg_tunn.c  PRIVATE Void egSetDefPortAddr ARGS          13       0
eg_li.c    PUBLIC S16 EgLiHitConCfm                    13       0
eg_util.c  PUBLIC S16 egUtilGetTrgtList                13       0
eg_li.c    PRIVATE S16 egLiPrcBndCfm                   14       0
pg_mi.c    PRIVATE S16 egUSapCfg                       15       0
pg_tpt.c   PUBLIC Void egTptMsgRetxTmrExpiry           15       0
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
pg_mi.c    PRIVATE S16 egGenCntrl                      16       0
pg_tunn.c  PUBLIC S16 egTunDelete ARGS                 16       0
eg_tmr.c   PUBLIC S16 egSchedTmr                       16       0
pg_mi.c    PRIVATE S16 egTServerCntrl                  17       0
pg_tpt.c   PRIVATE S16 egTptQueueMsgWthMsb             17       0
pg_tpt.c   PRIVATE S16 egTptCheckQReqrd                17       0
eg_util.c  PRIVATE S16 egUtilDelGrpIe                  17       0
pg_cpm.c   PUBLIC Void egCmPrntDbg                     18       0
pg_edm.c   PUBLIC S16 egEncMsgReq                      18       0
pg_mi.c    PUBLIC S16 EgMiLegCfgReq                    19       0
pg_edm.c   PRIVATE S16 egValidateGmMsgIEs              20       0
pg_tpt.c   PUBLIC S16 egTptProcessMsg                  20       0
pg_edm.c   PUBLIC S16 egDecMsgReq                      24       0
eg_util.c  PUBLIC S16 egUtilGetTrgtIe                  24       0
pg_edm.c   PRIVATE S16 egValidateGrpIe                 25       0
pg_ex_ms.c PUBLIC S16 egActvTsk                        26       0
pg_tunn.c  PUBLIC EgtMsgType egGetAssocMsg ARGS        27       0
eg_util.c  PUBLIC S16 egUtilDupIe                      27       0
pg_tpt.c   PUBLIC S16 egTptGetMsg                      28       0
pg_edm.c   PUBLIC S16 egDbCfgGmDict                    29       0
pg_edm.c   PRIVATE S16 egDecIe                         29       0
pg_mi.c    PRIVATE S16 egTServerCfg                    30       0
---------------------------------------------------------------------------

         *************************************************************
         * McCabe Report after the Completion of Product Code Review *
         ************************************************************* 
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
eg_dbs11.c PRIVATE void egMakes11MODBREFLRINDGmEntry   1          0
eg_dbs11.c PRIVATE void egMakes11CRTSESREQGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11UPDBRERSPGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11DELSESRSPGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11BRERESFLRINDGmEntry   1          0
eg_dbs11.c PRIVATE void egMakes11CRTFWDTUNREQGmEntry   1          0
eg_dbs11.c PRIVATE void egMakes11DELBREREQGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11DELBRECMDGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11CRTSESRSPGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11DWLNKDATNOTACKGmEntry 1          0
eg_dbs11.c PRIVATE void egMakes11MODBREREQGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11MODBRECMDGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11CRTFWDTUNRSPGmEntry   1          0
eg_dbs11.c PRIVATE void egMakes11CRTBREREQGmEntry      1          0
eg_dbs11.c PUBLIC S16 egMakes11GmDict                  1          0
eg_dbs11.c PRIVATE void egMakes11UPDUPLNREQGmEntry     1          0
eg_dbs11.c PRIVATE void egMakes11DELBRERSPGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11BRERESCMDGmEntry      1          0
eg_dbs11.c PUBLIC S16 egMakeIeDict                     1          0
eg_dbs11.c PRIVATE void egMakes11DELBREFLRINDGmEntry   1          0
eg_dbs11.c PRIVATE void egMakes11UPDBREREQGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11MODBRERSPGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11DWLNKDATNOTFLRGmEntry 1          0
eg_dbs11.c PRIVATE void egMakes11DELSESREQGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11CRTBRERSPGmEntry      1          0
eg_dbs11.c PRIVATE void egMakes11UPDUPLNRSPGmEntry     1          0
eg_dbs11.c PRIVATE void egMakes11DWLNKDATNOTGmEntry    1          0
eg_dbutl.c PUBLIC Void egMakeAppIeProp                 1          0
eg_dbutl.c PUBLIC Void egMakeIeEntry                   1          0
eg_edm.c   PRIVATE S16 egDecEG_STRING_132              1          0
eg_cpm.c   PUBLIC S16 egCpmDeinit                      1          0
eg_cpm.c   PUBLIC S16 egInitExt                        1          0
eg_edm.c   PRIVATE S16 egDecEG_STRING_8                1          0
eg_edm.c   PRIVATE S16 egEncPthHdr                     1          0
eg_edm.c   PRIVATE S16 egEncEG_TRC_INFO                1          0
eg_edm.c   PRIVATE S16  egEncEG_STRING_8               1          0
eg_edm.c   PRIVATE S16 egEncEG_STRING_16               1          0
eg_edm.c   PRIVATE S16 egEncEG_BQOS_IEDATA             1          0
eg_edm.c   PRIVATE S16 egEncEG_FQOS                    1          0
eg_edm.c   PRIVATE S16 egDecEG_UNSIGNED_32             1          0
eg_edm.c   PRIVATE S16 egEncEG_EMB_CAUSE               1          0
eg_edm.c   PRIVATE S16 egEncEG_SNETWORK                1          0
eg_edm.c   PUBLIC Size egGetIntfMem                    1          0
eg_edm.c   PRIVATE S16 egDecEG_STRING_32               1          0
eg_edm.c   PRIVATE S16 egDecEG_BQOS_IEDATA             1          0
eg_edm.c   PRIVATE S16 egDecEG_BEARER_FLAG             1          0
eg_edm.c   PRIVATE S16 egDecEG_UNSIGNED_16             1          0
eg_edm.c   PRIVATE S16 egEncEG_INDICATION              1          0
eg_edm.c   PRIVATE S16 egEncEG_STRING_32               1          0
eg_edm.c   PRIVATE S16 egEncEG_UNSIGNED_8              1          0
eg_edm.c   PRIVATE S16 egDecEG_EMB_CAUSE               1          0
eg_edm.c   PRIVATE S16 egDecEG_STRING_16               1          0
eg_edm.c   PRIVATE S16 egEncEG_UNSIGNED_32             1          0
eg_edm.c   PRIVATE S16 egEncEG_PVT_EXTN                1          0
eg_edm.c   PRIVATE S16 egEncEG_STRING_MAX              1          0
eg_edm.c   PRIVATE S16 egEncEG_UNSIGNED_16             1          0
eg_edm.c   PRIVATE S16 egDecEG_PVT_EXTN                1          0
eg_edm.c   PRIVATE S16 egDecEG_STRING_MAX              1          0
eg_edm.c   PRIVATE S16 egEncEG_STRING_132              1          0
eg_edm.c   PRIVATE S16 egDecEG_UNSIGNED_8              1          0
eg_ptli.c  PUBLIC S16 PtLiHitDiscReq                   1          0
eg_ptli.c  PUBLIC S16 PtLiHitUbndReq                   1          0
eg_ptli.c  PUBLIC S16 EgLiHitBndReq                    1          0
eg_ptli.c  PRIVATE S16 PtLiHitBndReq                   1          0
eg_ptli.c  PUBLIC S16 EgLiHitDiscReq                   1          0
eg_ptli.c  PUBLIC S16 EgLiHitUbndReq                   1          0
eg_ptmi.c  PUBLIC S16 PtMiLegStsCfm                    1          0
eg_ptmi.c  PUBLIC S16 PtMiLegStaCfm                    1          0
eg_ptmi.c  PUBLIC S16 EgMiLegStsCfm                    1          0
eg_ptmi.c  PUBLIC S16 EgMiLegTrcInd                    1          0
eg_ptmi.c  PUBLIC S16 EgMiLegStaCfm                    1          0
eg_ptmi.c  PUBLIC S16 PtMiLegTrcInd                    1          0
eg_ptmi.c  PUBLIC S16 PtMiLegCfgCfm                    1          0
eg_ptmi.c  PUBLIC S16 EgMiLegCntrlCfm                  1          0
eg_ptmi.c  PUBLIC S16 EgMiLegCfgCfm                    1          0
eg_ptmi.c  PUBLIC S16 PtMiLegStaInd                    1          0
eg_ptmi.c  PUBLIC S16 PtMiLegCntrlCfm                  1          0
eg_ptmi.c  PUBLIC S16 EgMiLegStaInd                    1          0
eg_ptui.c  PUBLIC S16 PtUiEgtSigInd                    1          0
eg_ptui.c  PUBLIC S16 EgUiEgtSigInd                    1          0
eg_ptui.c  PUBLIC S16 PtUiEgtLclDelCfm                 1          0
eg_ptui.c  PUBLIC S16 PtUiEgtErrInd                    1          0
eg_ptui.c  PUBLIC S16 PtUiEgtSigCfm                    1          0
eg_ptui.c  PUBLIC S16 EgUiEgtBndCfm                    1          0
eg_ptui.c  PUBLIC S16 PtUiEgtStaInd                    1          0
eg_ptui.c  PUBLIC S16 PtUiEgtBndCfm                    1          0
eg_ptui.c  PUBLIC S16 EgUiEgtLclDelCfm                 1          0
eg_ptui.c  PUBLIC S16 EgUiEgtErrInd                    1          0
eg_ptui.c  PUBLIC S16 EgUiEgtSigCfm                    1          0
eg_ptui.c  PUBLIC S16 EgUiEgtStaInd                    1          0
eg_ptui.c  PUBLIC S16 EgUiEgtStaCfm                    1          0
eg_ptui.c  PUBLIC S16 PtUiEgtStaCfm                    1          0
eg_id.c    PUBLIC S16 egGetSId                         1          0
eg_mi.c    PRIVATE Void egMiFillResponsePost           1          0
eg_mi.c    PUBLIC Void egSendLmCntrlCfm                1          0
eg_mi.c    PRIVATE Void egInitEgCb                     1          0
eg_tmr.c   PUBLIC Void egCmInitTimer                   1          0
eg_tpt.c   PUBLIC Void egTptFreeServer                 1          0
eg_tst.c   int main                                    1          1
eg_tunn.c  PRIVATE S16 egTunnFsmConErr ARGS            1          0
eg_tunn.c  PRIVATE S16 egTunnFsmIncErr ARGS            1          0
eg_tunn.c  PUBLIC S16 egTunnInit ARGS                  1          0
eg_tunn.c  PRIVATE S16 egReleaseTeid ARGS              1          0
eg_tunn.c  PRIVATE S16 egTunnFsmInvUsrEvnt ARGS        1          0
eg_tunn.c  PUBLIC S16 egTunnDeInit ARGS                1          0
eg_tunn.c  PRIVATE S16 egTunnFsmOutIncRsp ARGS         1          0
eg_tunn.c  PUBLIC S16 egDeInitEchoUsap ARGS            1          0
eg_tunn.c  PRIVATE S16 egHandleIncEchoRsp ARGS         1          0
eg_ui.c    PUBLIC S16 egUiFillEvntStrSigRsp            1          0
eg_tunn.c  PRIVATE S16 egTunnFsmOutErr ARGS            1          0
eg_tunn.c  PRIVATE S16 egTunnFsmInvNetEvnt ARGS        1          0
eg_tmr.c   PUBLIC Void egRmvTmr                        1          0
eg_mi.c    PUBLIC Void egMiTptSrvOpenInd               1          0
eg_tpt.c   PUBLIC S16 egTptInit                        1          0
eg_li.c    PUBLIC S16 EgLiHitFlcInd                    2          0
eg_edm.c   PRIVATE S16 egDecEG_SNETWORK                2          0
eg_edm.c   PRIVATE S16 egDecEG_FQOS                    2          0
eg_edm.c   PRIVATE S16 egEncEG_BEARER_FLAG             2          0
eg_mi.c    PUBLIC S16 egSendLmAlarm                    2          0
eg_mi.c    PRIVATE S16 egTSapStaReq                    2          0
eg_cpm.c   PUBLIC S16 egCmFreeEvent                    2          0
eg_cpm.c   PUBLIC S16 egCpmSigHdl                      2          0
eg_edm.c   PRIVATE S16 egDecEG_TRC_INFO                2          0
eg_mi.c    PUBLIC S16 egFree                           2          0
eg_tmr.c   PUBLIC S16 egActvTmr                        2          0
eg_mi.c    PRIVATE S16 egGenStsReq                     2          0
eg_mi.c    PRIVATE S16 egGenStaReq                     3          0
eg_tmr.c   PUBLIC S16 egRegInitTmr                     3          0
eg_tmr.c   PRIVATE S8* egTmrGetEvntStr                 5          0
eg_cpm.c   PUBLIC S16 egCpmLclDelReq                   3          0
eg_edm.c   PUBLIC S16 egAllocIntfLst                   3          0
eg_edm.c   PRIVATE S16 egDecGrpIe                      3          0
eg_edm.c   PRIVATE S16 egEncPthIe                      3          0
eg_edm.c   PRIVATE S16 egEncodeGmHdr                   3          0
eg_edm.c   PUBLIC EgIeEntry* egGetIeEntryFrmDb         3          0
eg_edm.c   PRIVATE S16 egEncIeData                     3          0
eg_mi.c    PRIVATE S16 egUSapStaReq                    3          0
eg_mi.c    PRIVATE S16 egMiDelUsap                     4          0
eg_mi.c    PRIVATE S16 egUnbindNDisableTSap            4          0
eg_mi.c    PRIVATE S16 egUSapStsReq                    4          0
eg_mi.c    PRIVATE S16 egTSapStsReq                    4          0
eg_tmr.c   PRIVATE Void egBndTmrExpiry                 4          0
eg_cpm.c   PUBLIC S16 egCmCreateEvent                  4          0
eg_edm.c   PUBLIC Void egDbCfgIeDict                   4          0
eg_cpm.c   PUBLIC Void egPrintEventStructure           4          0
eg_edm.c   PRIVATE S16 egValidatePthHdr                4          0
eg_edm.c   PRIVATE S16 egDecIeData                     4          0
eg_edm.c   PRIVATE S16 egEncEG_PAA_IEDATA              5          0
eg_edm.c   PRIVATE S16 egDecEG_PAA_IEDATA              5          0
eg_edm.c   PRIVATE S16 egEncVerIndMsg                  5          0
eg_edm.c   PRIVATE S16 egDecEG_FTEID                   5          0
eg_edm.c   PRIVATE S16 egValidatePthMsgIEs             5          0
eg_mi.c    PRIVATE S16 egMiDisableUsap                 5          0
eg_li.c    PUBLIC S16 EgLiHitBndCfm                    5          0
eg_li.c    PRIVATE S16 egLiValidatePrimitive           5          0
eg_mi.c    PRIVATE S16 egMiDropGmCfg                   5          0
eg_tpt.c   PUBLIC S16 egTptQueueMsg                    5          0
eg_tpt.c   PRIVATE S16 egTptUdpServerDatReq            3          0
eg_tpt.c   PUBLIC S16 egUtlDelTknStrOSXL               3          0
eg_tpt.c   PUBLIC S16 egTptState                       6          0
eg_tpt.c   PUBLIC Void egTptDeInit                     2          0
eg_mi.c    PRIVATE S16 egValidateTSapCfg               6          0
eg_mi.c    PUBLIC S16 egSendLmCfm                      6          0
eg_tmr.c   PUBLIC Void egTmrEvnt                       6          0
eg_mi.c    PRIVATE S16 egTSapCfg                       6          0
eg_edm.c   PRIVATE S16 egDecPthHdr                     6          0
eg_edm.c   PRIVATE S16 egDecEG_ULI_DATA                6          0
eg_edm.c   PRIVATE S16 egValidateGmHdr                 5          0
eg_edm.c   PRIVATE S16 egDecodeGmHdr                   6          0
eg_edm.c   PRIVATE S16 egEncEG_ULI_DATA                6          0
eg_cpm.c   PUBLIC S16 egCpmStaReq                      7          0
eg_edm.c   PRIVATE S16 egEncEG_FTEID                   7          0
eg_mi.c    PUBLIC Data *egAlloc                        7          0
eg_tpt.c   PUBLIC S16 egTptValIncMsgWthMsb             7          0
eg_tmr.c   PRIVATE Void egTptOpenSrvTmrExpiry          7          0
eg_tpt.c   PUBLIC Void egTptCloseServer                7          0
eg_tpt.c   PRIVATE S16 egTptValIncMsg                  7          0
eg_tpt.c   PUBLIC Void egTptDelAllServerContext        7          0
eg_tpt.c   PRIVATE S16 egTptPartialReadMsg             8          0
eg_tpt.c   PUBLIC S16 egTptOpenServer                  9          0
eg_edm.c   PUBLIC EgMsgEntry* egDbGetMsgFrmGmDict      8          0
eg_edm.c   PRIVATE S16 egEncIe                         8          0
eg_edm.c   PUBLIC S16 egEncPthReq                      9          0
eg_edm.c   PUBLIC S16 egEdmDeInit                      8          0
eg_mi.c    PUBLIC S16 egShutdown                       8          0
eg_mi.c    PRIVATE S16 egTSapCntrl                     9          0
eg_mi.c    PUBLIC S16 EgMiLegStsReq                    9          0
eg_mi.c    PRIVATE S16 egUSapCntrl                     9          0
eg_mi.c    PUBLIC Void egGenTrcInd                     9          0
eg_mi.c    PRIVATE S16 egValidateUSapCfg              10          0
eg_mi.c    PUBLIC S16 EgMiLegStaReq                   10          0
eg_mi.c    PUBLIC Void egMiTptSrvCloseInd             10          0
eg_mi.c    PUBLIC S16 egActvInit                      10          0
eg_edm.c   PRIVATE S16 egDecPthIe                     10          0
eg_edm.c   PUBLIC S16 egDbDropGmDict                  10          0
eg_edm.c   PRIVATE S16 egEdmSendMsgToCm               10          0
eg_tpt.c   PUBLIC Void egTptIssueDiscReq               2          0
eg_tpt.c   PUBLIC S16 egTptQueueMsgRmv                 6          0
eg_tpt.c   PUBLIC S16 egCmpTptAddr                     9          0
eg_tpt.c   PRIVATE S16 egTptQueueMsgRmvWthMsb          8          0
eg_tpt.c   PRIVATE S16 egTptSendMsg                    3          0
eg_tunn.c  PRIVATE S16 egDeInitTeidArr ARGS            5          0
eg_tunn.c  PRIVATE S16 egHandleOutEchoRsp ARGS         3          1
eg_tunn.c  PUBLIC EgUSapCb *egGetUsapFrmIntf ARGS      4          0
eg_tunn.c  PUBLIC S16 egCpyCmTptAddr ARGS              4          0
eg_tunn.c  PRIVATE EgTECCb** egGetTunnelCb ARGS        2          0
eg_tunn.c  PUBLIC EgtMsgCat egGetMsgCategory ARGS      2          0
eg_tunn.c  PUBLIC S16 egInitEchoUsap ARGS              2          0
eg_tunn.c  PRIVATE S16 egTunnFsmDel ARGS               2          0
eg_tunn.c  PRIVATE S16 egObtainFreeTeid ARGS           8          0
eg_tunn.c  PRIVATE S16 egTunnFsmConOutMsg ARGS         6          1
eg_tunn.c  PUBLIC S16 egTunIndicate ARGS               3          0
eg_tunn.c  PUBLIC Void egUpdateRxStatistics ARGS       2          0
eg_tunn.c  PRIVATE S16 egInitFreeTeidList ARGS         4          0
eg_tunn.c  PRIVATE S16 egHandleIncEchoReq ARGS         3          1
eg_tunn.c  PRIVATE S16 egInitTeidArr ARGS              8          0
eg_tunn.c  PRIVATE S16 egTunnFsmIncOutRsp ARGS         4          1
eg_tunn.c  PUBLIC Void egSendErrInd ARGS               3          0
eg_tunn.c  PRIVATE S16 egTunnFsmConIncMsg ARGS         7          1
eg_tunn.c  PRIVATE S16 egTunnFsmIdlOSesReq ARGS        4          0
eg_tunn.c  PUBLIC S16 egTunnState ARGS                 3          0
eg_tunn.c  PUBLIC EgtIntf egGetIntfFrmTeid ARGS        3          0
eg_tunn.c  PRIVATE Void egTunnDelTECCb ARGS            2          0
eg_tunn.c  PRIVATE S16 egTunnFsmIdlISesReq ARGS        3          1
eg_tunn.c  PUBLIC Void egUpdateTxStatistics ARGS       2          0
eg_ui.c    PUBLIC S16 EgUiEgtLclDelReq                 5          0
eg_ui.c    PUBLIC S16 EgUiEgtUbndReq                   5          0
eg_ui.c    PUBLIC S16 EgUiEgtStaReq                    8          0
eg_ui.c    PUBLIC S16 EgUiEgtSigRsp                    8          0
eg_ui.c    PUBLIC S16 EgUiEgtBndReq                    5          0
eg_util.c  PUBLIC S16 EgUtilGmDelIe                    5          0
eg_util.c  PUBLIC S16 egUtilDropIeList                 5          0
eg_util.c  PUBLIC S16 egUtilDumpIpv6Addr               5          0
eg_util.c  PUBLIC S16 egUtilCompareIeValue             6          0
eg_util.c  PUBLIC S16 egUtilDumpSNwk                   2          0
eg_util.c  PUBLIC S16 egUtilDumpIpv4Addr               5          0
eg_util.c  PUBLIC S16 EgUtilDeAllocGmMsg               4          0
eg_util.c  PUBLIC U8 egGetDataFrmIeType                3          0
eg_util.c  PUBLIC S16 EgUtilGetGtpHdr                  2          0
eg_util.c  PUBLIC S16 egUtilDupIpv4Addr                3          0
eg_util.c  PUBLIC S16 EgUtilGmGetIeCnt                 3          0
eg_util.c  PUBLIC S16 EgUtilGmGetIe                    4          0
eg_util.c  PUBLIC S16 egUtilDupSNwk                    2          0
eg_util.c  PUBLIC S16 EgUtilGmGetIeOccrCnt             6          0
eg_util.c  PUBLIC S16 EgUtilGmAppendIe                 8          0
eg_util.c  PUBLIC S16 egUtilDupIpv6Addr                3          0
eg_util.c  PUBLIC S16 EgUtilGmDelGrpIe                 3          0
eg_util.c  PUBLIC S16 EgUtilSetGtpHdr                  2          0
eg_util.c  PUBLIC S16 EgUtilAllocGmMsg                 4          0
eg_util.c  PUBLIC S16 egUtilDupList                    6          0
eg_util.c  PUBLIC S16 EgUtilGmDup                      5          0
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
eg_edm.c   PUBLIC S16 egDecPthReq                     11          0
eg_cpm.c   PUBLIC S16 egCpmInit                       11          0
eg_edm.c   PRIVATE S16 egValidateIeLen                11          0
eg_li.c    PUBLIC S16 EgLiHitDiscCfm                  11          0
eg_tpt.c   PUBLIC S16 egTptProcessTsapServers         11          0
eg_edm.c   PRIVATE S16 egMakeGrpIeEntry               12          0
eg_mi.c    PUBLIC S16 EgMiLegCfgReq                   12          0
eg_tpt.c   PUBLIC Void egTptMsgRetxTmrExpiry          15          0
eg_li.c    PUBLIC S16 EgLiHitDiscInd                  13          0
eg_li.c    PUBLIC S16 EgLiHitConCfm                   13          0
eg_tunn.c  PRIVATE Void egSetDefPortAddr ARGS         13          0
eg_tunn.c  PRIVATE S16 egTunCreateReceive ARGS        14          0
eg_li.c    PRIVATE S16 egLiPrcBndCfm                  14          0
eg_mi.c    PRIVATE S16 egUSapCfg                      15          0
eg_mi.c    PRIVATE S16 egGenCfg                       12          0
eg_mi.c    PRIVATE S16 egValidateGenCfg               12          0
eg_mi.c    PUBLIC S16 EgMiLegCntrlReq                 12          0
eg_tunn.c  PRIVATE S16 egTunCreateSend ARGS           13          0
eg_tpt.c   PRIVATE S16 egTptQueueMsgWthMsb            14          0
eg_tunn.c  PUBLIC S16 egTunReceive ARGS               11          0
eg_tunn.c  PUBLIC S16 egTunDelete ARGS                12          0
eg_tunn.c  PUBLIC S16 egTunSend ARGS                  12          0
eg_util.c  PUBLIC S16 egUtilDelIe                     12          0
eg_util.c  PUBLIC S16 egUtilGetTrgtList               13          0
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
eg_tpt.c   PRIVATE S16 egTptCheckQReqrd               16          0
eg_cpm.c   PUBLIC Void egCmPrntDbg                    17          0
eg_edm.c   PUBLIC S16 egEncMsgReq                     18          0
eg_edm.c   PUBLIC S16 egDecMsgReq                     24          0
eg_edm.c   PRIVATE S16 egValidateGrpIe                25          0
eg_edm.c   PRIVATE S16 egDecIe                        27          0
eg_edm.c   PRIVATE S16 egValidateGmMsgIEs             20          0
eg_ex_ms.c PUBLIC S16 egActvTsk                       26          0
eg_edm.c   PUBLIC S16 egDbCfgGmDict                   29          0
eg_mi.c    PRIVATE S16 egTServerCfg                   25          0
eg_mi.c    PRIVATE S16 egTServerCntrl                 16          0
eg_mi.c    PRIVATE S16 egGenCntrl                     16          0
eg_tmr.c   PUBLIC S16 egSchedTmr                      16          0
eg_tpt.c   PUBLIC S16 egTptProcessMsg                 24          0
eg_tpt.c   PUBLIC S16 egTptGetMsg                     26          0
eg_tunn.c  PUBLIC EgtMsgType egGetAssocMsg ARGS       27          0
eg_util.c  PRIVATE S16 egUtilDelGrpIe                 17          0
eg_util.c  PUBLIC S16 egUtilGetTrgtIe                 24          0
---------------------------------------------------------------------------
File            Name                           Complexity     No.of returns
---------------------------------------------------------------------------
eg_util.c  PUBLIC S16 egUtilDupIe                     35          0


         *************************************************************
         *    McCabe Report after the Completion of Product Coding   *
         ************************************************************* 
---------------------------------------------------------------------------
File             Name                   Complexity   No. of returns
---------------------------------------------------------------------------
eg_util.c PUBLIC S16 egStrcpy                    1            0
eg_edm.c PRIVATE S16 egEncVerIndMsg            5            0
eg_edm.c PRIVATE S16 egValidatePthHdr            4            0
eg_edm.c PRIVATE S16 egEncPthHdr            2            0
eg_edm.c PRIVATE S16 egValidatePthMsgIEs         4            0
eg_edm.c PRIVATE S16 egEncIe                    8            0
eg_edm.c PRIVATE S16 egDecPthHdr            6            0
eg_edm.c PRIVATE S16 egDecGrpIe                    3            0
eg_edm.c PRIVATE S16 egValidateGmHdr            4            0
eg_edm.c PRIVATE S16 egDecodeGmHdr            6            0
eg_edm.c PRIVATE S16 egEncPthIe                    3            0
eg_edm.c PRIVATE S16 egEncodeGmHdr            4            0
eg_edm.c PRIVATE EgMsgEntry* egDbGetMsgFrmGmDict 8            0
eg_edm.c PRIVATE S16 egDecPthIe                    9            0
eg_edm.c PUBLIC S16 egDecPthReq                   10            0
eg_edm.c PUBLIC S16 egEncPthReq                   10            0
eg_edm.c PUBLIC S16 egDbDropGmDict           10            0
eg_edm.c PRIVATE S16 egEdmSendMsgToCm           10            0
eg_edm.c PRIVATE S16 egMakeGrpIeEntry            9            0
eg_edm.c PUBLIC Void egDbCfgIeDict            3            0
eg_cpm.c PUBLIC Void egCmPrntDbg             1            0
eg_cpm.c PUBLIC S16 egCmCreateEvent             1            0
eg_cpm.c PUBLIC S16 egCpmInit                    9            0
eg_cpm.c PUBLIC S16 egCpmLclDelReq            3            0
eg_cpm.c PUBLIC S16 egCpmDeinit                    1            0
eg_cpm.c PUBLIC S16 egCpmSigHdl                2            0
eg_cpm.c PUBLIC S16 egCpmStaReq                7            0
eg_ex_ms.c PUBLIC S16 egActvTsk                1            0
eg_ex_ms.c PUBLIC S16 egActvInit            1            0
eg_li.c PUBLIC S16 EgLiHitBndCfm            4            0
eg_li.c PUBLIC S16 EgLiHitDiscCfm            8            0
eg_li.c PRIVATE S16 egLiValidatePrimitive    5            0
eg_li.c PUBLIC S16 EgLiHitFlcInd            1            0
eg_mi.c PRIVATE S16 egUSapStaReq            3            0
eg_mi.c PUBLIC S16 egGetSysId                 1            0
eg_mi.c PUBLIC S16 egSendLmAlarm            2            0
eg_mi.c PRIVATE S16 egTSapStaReq            2            0
eg_mi.c PUBLIC S16 EgMiLegStaReq           10            0
eg_mi.c PRIVATE S16 egTSapCfg                    6            0
eg_mi.c PRIVATE S16 egMiDisableUsap            4            0
eg_mi.c PUBLIC S16 egGetFreeTptConnId             1            0
eg_mi.c PUBLIC Data *egAlloc                     7            0
eg_mi.c PRIVATE S16 egUSapCfg                    6            0
eg_mi.c PUBLIC Void egSendLmCntrlCfm            1            0
eg_mi.c PRIVATE S16 egDelServer                    2            0
eg_mi.c PRIVATE S16 egServConReq            3            0
eg_mi.c PRIVATE S16 egUnbindNDisableTSap    5            0
eg_mi.c PRIVATE S16 egMiDelUsap                    4            0
eg_mi.c PUBLIC S16 egFree                     1            0
eg_mi.c PRIVATE S16 egValidateTSapCfg            6            0
eg_mi.c PUBLIC S16 egSendLmCfm                    6            0
eg_mi.c PRIVATE S16 egUSapStsReq            4            0
eg_mi.c PUBLIC S16 EgMiLegCntrlReq           10            0
eg_mi.c PRIVATE Void egInitEgCb                    1            0
eg_mi.c PRIVATE S16 egTSapStsReq            4            0
eg_mi.c PUBLIC S16 EgMiLegStsReq            9            0
eg_mi.c PRIVATE S16 egUSapCntrl                    9            0
eg_mi.c PRIVATE S16 egValidateUSapCfg            4            0
eg_mi.c PUBLIC Void egGenTrcInd                    9            0
eg_mi.c PRIVATE S16 egGenStsReq                    2            0
eg_mi.c PRIVATE S16 egGenStaReq                    3            0
eg_ptli.c PUBLIC S16 PtLiHitDiscReq            1            0
eg_ptli.c PUBLIC S16 PtLiHitUbndReq            1            0
eg_ptli.c PUBLIC S16 EgLiHitBndReq            1            0
eg_ptli.c PRIVATE S16 PtLiHitBndReq            1            0
eg_ptli.c PUBLIC S16 EgLiHitDiscReq            1            0
eg_ptli.c PUBLIC S16 EgLiHitUbndReq            1            0
eg_ptmi.c PUBLIC S16 PtMiLegStsCfm             1            0
eg_ptmi.c PUBLIC S16 PtMiLegStaCfm             1            0
eg_ptmi.c PUBLIC S16 EgMiLegStsCfm             1            0
eg_ptmi.c PUBLIC S16 EgMiLegTrcInd             1            0
eg_ptmi.c PUBLIC S16 EgMiLegStaCfm             1            0
eg_ptmi.c PUBLIC S16 PtMiLegTrcInd             1            0
eg_ptmi.c PUBLIC S16 PtMiLegCfgCfm             1            0
eg_ptmi.c PUBLIC S16 EgMiLegCntrlCfm             1            0
eg_ptmi.c PUBLIC S16 EgMiLegCfgCfm             1            0
eg_ptmi.c PUBLIC S16 PtMiLegStaInd             1            0
eg_ptmi.c PUBLIC S16 PtMiLegCntrlCfm             1            0
eg_ptmi.c PUBLIC S16 EgMiLegStaInd             1            0
eg_ptui.c PUBLIC S16 PtUiEgtSigInd             1            0
eg_ptui.c PUBLIC S16 EgUiEgtSigInd             1            0
eg_ptui.c PUBLIC S16 PtUiEgtLclDelCfm             1            0
eg_ptui.c PUBLIC S16 PtUiEgtErrInd             1            0
eg_ptui.c PUBLIC S16 PtUiEgtSigCfm             1            0
eg_ptui.c PUBLIC S16 EgUiEgtBndCfm             1            0
eg_ptui.c PUBLIC S16 PtUiEgtStaInd             1            0
eg_ptui.c PUBLIC S16 PtUiEgtBndCfm             1            0
eg_ptui.c PUBLIC S16 EgUiEgtLclDelCfm             1            0
eg_ptui.c PUBLIC S16 EgUiEgtErrInd             1            0
eg_ptui.c PUBLIC S16 EgUiEgtSigCfm             1            0
eg_ptui.c PUBLIC S16 EgUiEgtStaInd             1            0
eg_ptui.c PUBLIC S16 EgUiEgtStaCfm             1            0
eg_ptui.c PUBLIC S16 PtUiEgtStaCfm             1            0
eg_tmr.c PRIVATE S8* egTmrGetEvntStr            5            0
eg_tmr.c PUBLIC S16 egActvTmr                    2            0
eg_tmr.c PUBLIC S16 egRegInitTmr            3            0
eg_tmr.c PRIVATE Void egBndTmrExpiry            4            0
eg_tmr.c PUBLIC Void egTmrEvnt                    4            0
eg_tpt.c PRIVATE S16 egTptPartialReadMsg     7            0
eg_tpt.c PUBLIC S16 egTptValIncMsgWthMsb     5            0
eg_tpt.c PUBLIC S16 egTptQueueMsgRmv             4            0
eg_tpt.c PUBLIC S16 egTptQueueMsg             4            0
eg_tpt.c PRIVATE S16 egTptUdpServerDatReq     3            0
eg_tpt.c PUBLIC S16 egUtlDelTknStrOSXL            3            0
eg_tpt.c PUBLIC S16 egTptState                     4            0
eg_tpt.c PUBLIC Void egTptDeInit             1            0
eg_tpt.c PUBLIC Void egMiTptSrvCloseInd    10            0
eg_tpt.c PUBLIC S16 egTptInit                     1            0
eg_tpt.c PUBLIC Void egTptCloseServer             7            0
eg_tpt.c PRIVATE S16 egTptValIncMsg             4            0
eg_tpt.c PUBLIC Void egTptDelAllServerContext     7            0
eg_tpt.c PUBLIC Void egTptFreeServer             1            0
eg_tpt.c PUBLIC Void egTptIssueDiscReq             2            0
eg_tpt.c PUBLIC S16 egCmpTptAddr            9            0
eg_tpt.c PRIVATE S16 egTptQueueMsgRmvWthMsb     5            0
eg_tpt.c PRIVATE S16 egTptSendMsg             3            0
eg_tunn.c PRIVATE S16 egTunnFsmConErr ARGS     1            1
eg_tunn.c PRIVATE S16 egTunnFsmIncErr ARGS     1            1
eg_tunn.c PUBLIC S16 egTunnInit ARGS            1            1
eg_tunn.c PUBLIC S16 egInitEchoUsap ARGS    2            0
eg_tunn.c PUBLIC S16 cpyCmTptAddr ARGS             3            1
eg_tunn.c PUBLIC EgtMsgCat egGetMsgCategory      8
eg_tunn.c PUBLIC S16 egTunnDeInit ARGS            1            1
eg_tunn.c PRIVATE S16 egTunnFsmInvUsrEvnt ARGS   1            1
eg_tunn.c PRIVATE S16 releaseTeid ARGS             2            1
eg_tunn.c PUBLIC S16 egDeInitEchoUsap ARGS    1            0
eg_tunn.c PRIVATE S16 egTunnFsmOutIncRsp ARGS     1            1
eg_tunn.c PRIVATE S16 egTunnFsmDel ARGS     2            1
eg_tunn.c PRIVATE S16 egTunCreateReceive ARGS   10            5
eg_tunn.c PRIVATE S16 egTunnFsmConOutMsg ARGS     5            4
eg_tunn.c PRIVATE S16 egTunCreateSend ARGS   10            5
eg_tunn.c PUBLIC S16 egTunIndicate ARGS            2            1
eg_tunn.c PRIVATE S16 egTunnFsmOutErr ARGS     1            1
eg_tunn.c PRIVATE S16 egTunnFsmIdlErr ARGS     1            1
eg_tunn.c PRIVATE S16 egTunnFsmInvNetEvnt ARGS     1            1
eg_tunn.c PRIVATE S16 deInitTeidArr ARGS    4            1
eg_tunn.c PUBLIC S16 egTunDelete ARGS            3            1
eg_tunn.c PRIVATE EgTECCb* getTunnelCb ARGS    2            2
eg_tunn.c PRIVATE S16 egTunnFsmIncOutRsp ARGS     4            3
eg_tunn.c PUBLIC EgUSapCb *getUsapFrmIntf ARGS    4            2
eg_tunn.c PUBLIC S16 egTunnState ARGS            3            0
eg_tunn.c PUBLIC EgtIntf egGetIntfFrmTeid ARGS    3            2
eg_tunn.c PRIVATE S16 egTunnFsmConIncMsg ARGS     6            3
eg_tunn.c PRIVATE S16 egTunnFsmIdlOSesReq ARGS     3            2
eg_tunn.c PRIVATE Void egTunnDelTECCb ARGS     1            0
eg_tunn.c PRIVATE S16 egTunnFsmIdlISesReq ARGS     5            3
eg_tunn.c PRIVATE Void initFreeTeidList ARGS     2            0
eg_ui.c PUBLIC S16 EgUiEgtLclDelReq            7            0
eg_ui.c PUBLIC S16 EgUiEgtUbndReq            4            0
eg_ui.c PUBLIC S16 EgUiEgtStaReq            7            0
eg_ui.c PUBLIC S16 EgUiEgtSigRsp           10            0
eg_ui.c PUBLIC S16 EgUiEgtBndReq            4            0
eg_util.c PUBLIC S16 EgUtilGmDelIe            5            0
eg_util.c PUBLIC S16 egUtilDropIeList            5            0
eg_util.c PUBLIC S8* egStrSearch            6            0
eg_util.c PUBLIC S16 egUtilDupIe            8            0
eg_util.c PUBLIC S16 egUtilCompareIeValue    7            0
eg_util.c PUBLIC S16 EgUtilDeAllocGmMsg            4            0
eg_util.c PUBLIC S16 EgUtilGetGtpHdr            2            0
eg_util.c PUBLIC S16 egStrcat                    3            0
eg_util.c PUBLIC S16 EgUtilGmGetIeCnt            3            0
eg_util.c PUBLIC S16 EgUtilGmGetIe            4            0
eg_util.c PUBLIC S16 EgUtilGmAppendIe            8            0
eg_util.c PUBLIC S16 EgUtilGmDelGrpIe            3            0
eg_util.c PUBLIC S16 EgUtilSetGtpHdr            2            0
eg_util.c PUBLIC S16 EgUtilAllocGmMsg            4            0
eg_util.c PUBLIC S16 egUtilDupList            6            0
eg_util.c PUBLIC S16 EgUtilGmDup            5            0
---------------------------------------------------------------------------
File             Name                   Complexity   No. of returns
---------------------------------------------------------------------------
eg_tunn.c PUBLIC S16 egTunSend ARGS           13            9
eg_tpt.c PUBLIC S16 egTptProcessTsapServers    11            0
eg_tpt.c PUBLIC S16 egTptOpenServer            12            0
eg_tpt.c PRIVATE S16 egTptQueueMsgWthMsb    13            0
eg_mi.c PRIVATE S16 egTServerCfg           14            0
eg_mi.c PUBLIC S16 egShutdown                   11            0
eg_mi.c PRIVATE S16 egTSapCntrl                  11            0
eg_mi.c PRIVATE S16 egGenCfg                   12            0
eg_mi.c PRIVATE S16 egTServerCntrl           12            0
eg_mi.c PRIVATE S16 egValidateGenCfg           12            0
eg_edm.c PRIVATE S16 egValidateIeLen           11            0
eg_edm.c PRIVATE S16 egValidateGrpIe           14            0
eg_edm.c PRIVATE S16 egValidateGmMsgIEs           13            0
eg_util.c PUBLIC S16 egUtilGetTrgtList           13            0
eg_util.c PUBLIC S16 egUtilDelIe           12            0
eg_li.c PUBLIC S16 EgLiHitUDatInd           11            0
eg_tunn.c PRIVATE S16 initTeidArr ARGS           15            6
eg_li.c PRIVATE S16 egLiPrcBndCfm           14            0
eg_li.c PUBLIC S16 EgLiHitDiscInd           12            0
---------------------------------------------------------------------------
File             Name                   Complexity   No. of returns
---------------------------------------------------------------------------
eg_edm.c PUBLIC S16 egDbCfgGmDict           23            0
eg_edm.c PUBLIC S16 egEncMsgReq                   23            0
eg_edm.c PUBLIC S16 egDecMsgReq                   24            0
eg_edm.c PRIVATE S16 egDecIe                   22            0
eg_mi.c PRIVATE S16 egGenCntrl                   16            0
eg_mi.c PUBLIC S16 EgMiLegCfgReq           18            0
eg_tmr.c PUBLIC S16 egSchedTmr                    16            0
eg_tpt.c PUBLIC S16 egTptGetMsg            21            0
eg_tunn.c PRIVATE S16 ObtainFreeTeid ARGS   18            5
eg_tunn.c PUBLIC S16 egTunReceive ARGS           21           10
eg_tunn.c PUBLIC EgtMsgType egGetAssocMsg ARGS   24           24
eg_util.c PUBLIC S16 egUtilGetTrgtIe           24            0
eg_util.c PRIVATE S16 egUtilDelGrpIe           17            0
---------------------------------------------------------------------------
File             Name                   Complexity   No. of returns
---------------------------------------------------------------------------
eg_edm.c PRIVATE S16 egEncIeData           41            0
eg_edm.c PRIVATE S16 egDecIeData           42            0
eg_tpt.c PUBLIC S16 egTptProcessMsg            31            0
---------------------------------------------------------------------------
******************************************************************************/


/*************************************************************************30**

         End of file:     eGTP.c@@/main/3 - Fri Feb  4 18:18:18 2011

**************************************************************************31*/

^L
/*************************************************************************40**

        Notes:

**************************************************************************41*/

/*************************************************************************50**

**************************************************************************51*/

^L
/*************************************************************************60**

        Revision history:


**************************************************************************61*/

/*************************************************************************80**

**************************************************************************81*/

/*************************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---    skv                1. Initial Release for EGTP
/main/2      ---    skv                1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
*********************************************************************91*/
