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

/********************************************************************20**
 
     Name:     Trillium LTE CNE - MME EPS Mobility Management Module
  
     Type:     C source file
  
     Desc:     C source code for EMM Module
  
     File:     ny_emm.c 
  
     Sid:      vb_emm.c@@/main/1 - Mon Jun 28 19:35:34 2010
  
     Prg:      sv
  
*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library           */
#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "cm_emm_esm.h"
/* vb005.101: Added support for Network initiated Detach (ccpu00117691)*/
#ifdef VB_PERF_MEAS
#include <sys/time.h>
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "sct.x"           /* SCT interface defines           */

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb_hss.x"
#include "vb.x"            /* CNE Application structures     */

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/* Syntax of the EPS Mobility Management Procedure handlers */
typedef S16 (*VBPF) ARGS((VbMmeEnbCb  *eNbCb,
                          VbMmeUeCb   *ueCb,
                          CmNasEvnt   *evnt,
                          VbMmeUeCtxt  *ueCtxt));


PRIVATE S16 vbMmeEmmHdlAttachReq ARGS((VbMmeEnbCb *eNbCb,
                                    VbMmeUeCb  *ueCb,
                                    CmNasEvnt  *evnt,
                                    VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlTAUInDeReg ARGS((VbMmeEnbCb *eNbCb,
                                    VbMmeUeCb  *ueCb,
                                    CmNasEvnt  *evnt,
                                    VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAttachAccpt ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAttachReject ARGS((VbMmeEnbCb *eNbCb,
                                       VbMmeUeCb  *ueCb,
                                       CmNasEvnt  *evnt,
                                       VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlTAUReq ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlTAUAcc ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlTAUCmp ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlTAURej ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlOutCommonProc ARGS((VbMmeEnbCb *eNbCb,
                                   VbMmeUeCb  *ueCb,
                                   CmNasEvnt  *evnt,
                                   VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlInCommonProc ARGS((VbMmeEnbCb *eNbCb,
                                   VbMmeUeCb  *ueCb,
                                   CmNasEvnt  *evnt,
                                   VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlMmeStatus ARGS((VbMmeEnbCb *eNbCb,
                                 VbMmeUeCb  *ueCb,
                                 CmNasEvnt  *evnt,
                                 VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlUeStatus ARGS((VbMmeEnbCb *eNbCb,
                                 VbMmeUeCb  *ueCb,
                                 CmNasEvnt  *evnt,
                                 VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAttachReqRept ARGS((VbMmeEnbCb *eNbCb,
                                        VbMmeUeCb  *ueCb,
                                        CmNasEvnt  *evnt,
                                        VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAttachAccCmnProc ARGS((VbMmeEnbCb *eNbCb,
                                           VbMmeUeCb  *ueCb,
                                           CmNasEvnt  *evnt,
                                           VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAttachComplCmnProc ARGS((VbMmeEnbCb *eNbCb,
                                             VbMmeUeCb  *ueCb,
                                             CmNasEvnt  *evnt,
                                             VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAttachCompl ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlUeDetachReq ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlNwDetachReq ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlNwDetachAccpt ARGS((VbMmeEnbCb *eNbCb,
                                       VbMmeUeCb  *ueCb,
                                       CmNasEvnt  *evnt,
                                       VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlUeDetachAccpt ARGS((VbMmeEnbCb *eNbCb,
                                       VbMmeUeCb  *ueCb,
                                       CmNasEvnt  *evnt,
                                       VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlIncSrvceProc ARGS((VbMmeEnbCb *eNbCb,
                                    VbMmeUeCb  *ueCb,
                                    CmNasEvnt  *evnt,
                                    VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlOutSrvceProc ARGS((VbMmeEnbCb *eNbCb,
                                    VbMmeUeCb  *ueCb,
                                    CmNasEvnt  *evnt,
                                    VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlOutNasTranport ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlIncNasTranport ARGS((VbMmeEnbCb *eNbCb,
                                      VbMmeUeCb  *ueCb,
                                      CmNasEvnt  *evnt,
                                      VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlEmmInfo ARGS((VbMmeEnbCb *eNbCb,
                                   VbMmeUeCb  *ueCb,
                                   CmNasEvnt  *evnt,
                                   VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlCsNotify ARGS((VbMmeEnbCb *eNbCb,
                                       VbMmeUeCb  *ueCb,
                                       CmNasEvnt  *evnt,
                                       VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlUeDetachReqCrossd ARGS((VbMmeEnbCb *eNbCb,
                                            VbMmeUeCb  *ueCb,
                                            CmNasEvnt  *evnt,
                                            VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlEvntErr ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlOutEvntErr ARGS((VbMmeEnbCb *eNbCb,
                                        VbMmeUeCb  *ueCb,
                                        CmNasEvnt  *evnt,
                                        VbMmeUeCtxt  *ueCtxt));


PRIVATE S16 vbMmeEmmHdlGutiReallocCmd ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

#ifdef VB_MME_AUTH
PRIVATE S16 vbMmeEmmHdlAuthReq ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAuthRej ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAuthRsp ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlAuthFail ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlSecModeCmd ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlSecModeCmp ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlSecModeRej ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));
#endif

PRIVATE S16 vbMmeEmmHdlIdReq ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlEvntErr ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlGutiReallocCmp ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlIdRsp ARGS((VbMmeEnbCb *eNbCb,
                                  VbMmeUeCb  *ueCb,
                                  CmNasEvnt  *evnt,
                                  VbMmeUeCtxt  *ueCtxt));

PRIVATE S16 vbMmeEmmHdlServReqInDeReg ARGS((VbMmeEnbCb *eNbCb,
                                    VbMmeUeCb  *ueCb,
                                    CmNasEvnt  *evnt,
                                    VbMmeUeCtxt  *ueCtxt));

/* EPS Mobility Management Messages - Message Types */

/* Bits  
8 7 6 5 4 3 2 1  
         
0 1 - - - - - -  EPS mobility management messages
         
0 1 0 0 0 0 0 1  Attach request
0 1 0 0 0 0 1 0  Attach accept
0 1 0 0 0 0 1 1  Attach complete
0 1 0 0 0 1 0 0  Attach reject
0 1 0 0 0 1 0 1  Detach request
0 1 0 0 0 1 1 0  Detach accept
         
0 1 0 0 1 0 0 0  Tracking area update request
0 1 0 0 1 0 0 1  Tracking area update accept
0 1 0 0 1 0 1 0  Tracking area update complete
0 1 0 0 1 0 1 1  Tracking area update reject
         
0 1 0 0 1 1 0 0  Extended service request
0 1 0 0 1 1 1 0  Service reject
         
0 1 0 1 0 0 0 0  GUTI reallocation command
0 1 0 1 0 0 0 1  GUTI reallocation complete
0 1 0 1 0 0 1 0  Authentication request
0 1 0 1 0 0 1 1  Authentication response
0 1 0 1 0 1 0 0  Authentication reject
0 1 0 1 1 1 0 0  Authentication failure
0 1 0 1 0 1 0 1  Identity request
0 1 0 1 0 1 1 0  Identity response
0 1 0 1 1 1 0 1  Security mode command
0 1 0 1 1 1 1 0  Security mode complete
0 1 0 1 1 1 1 1  Security mode reject
         
0 1 1 0 0 0 0 0  EMM status
0 1 1 0 0 0 0 1  EMM information
0 1 1 0 0 0 1 0  Downlink NAS transport
0 1 1 0 0 0 1 1  Uplink NAS transport
0 1 1 0 0 1 0 0  CS Service notification

*/

/* EPS Mobility Management Procedure Matrix for an UE intended to be
    retrieve services over an eNodeB through S1 interface. 
  * This state machine matrix is referred for the all the EPS Mobility 
  * Management incoming and outgoing messages only when this MME is
  * connected with an eNodeB for an UE in question. */

/* EMM Procedure Types
   1. EMM Specific
      1. UE Attach
      2. UE Detach
          2.1 UE Initiated Detach
          2.2 Network Initiated Detach
      3. Tracking Area Update

   2. EMM Connection Management
      1. Service Request Procedure
      2. Paging
      3. NAS Transport [SMS services]

   3. EMM Common
      1. Identification
      2. Authentication
      3. Security Mode Control
      4. GUTI Reallocation

   4. EMM General
      1. EMM Status
      2. EMM Information
      3. CS Notification

The EMM state machine provided handles EMM specific UE attach and UE detach 
to completion.

For the rest of EMM procedures, entry function hooks are provided.

The unexpected events in a state are considered with common handler.

Suitable actions are taken to notify the MME Controller either to abort
the procedure or send a failure response back to sender of the EMM 
event.

*/

/* The EMM common procedure message to event map */
PRIVATE VbEmmCmnPrcEvt VbEmmCmnProcMsgToEvntMap[CM_EMM_MAX_CMN_PROC_MSG_VALUE] =
{
   VB_EMM_EVNT_CMN_PRC_GUTI_RLC,
   VB_EMM_EVNT_CMN_PRC_GUTI_RLC,
   VB_EMM_EVNT_CMN_PRC_AUTH_NRML,
   VB_EMM_EVNT_CMN_PRC_AUTH_NRML,
   VB_EMM_EVNT_CMN_PRC_AUTH_ABNRML,
   VB_EMM_EVNT_CMN_PRC_ID,
   VB_EMM_EVNT_CMN_PRC_ID,
   VB_EMM_EVNT_CMN_PRC_INVALID,
   VB_EMM_EVNT_CMN_PRC_INVALID,
   VB_EMM_EVNT_CMN_PRC_INVALID,
   VB_EMM_EVNT_CMN_PRC_INVALID,
   VB_EMM_EVNT_CMN_PRC_INVALID,
   VB_EMM_EVNT_CMN_PRC_AUTH_ABNRML,
   VB_EMM_EVNT_CMN_PRC_SEC_MOD_NRML,
   VB_EMM_EVNT_CMN_PRC_SEC_MOD_NRML,
   VB_EMM_EVNT_CMN_PRC_SEC_MOD_ABNRML
};



/* The EMM message to event mapper array */
PRIVATE VbEmmEvnt VbEmmMsgToEvntMap[CM_EMM_MAX_MSG_VALUE] =
{
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_ATTACH_REQ,
   VB_EMM_EVNT_ATTACH_ACC,
   VB_EMM_EVNT_ATTACH_COMPL,
   VB_EMM_EVNT_ATTACH_REJ,
   VB_EMM_EVNT_UE_DETACH_REQ, /*-- VB_EMM_EVNT_NW_DETACH_REQ, --*/
   VB_EMM_EVNT_NW_DETACH_ACC, /*-- VB_EMM_EVNT_UE_DETACH_ACC, --*/

   VB_EMM_EVNT_INVALID,

   VB_EMM_EVNT_TA_UPD_REQ,
   VB_EMM_EVNT_TA_UPD_ACC,
   VB_EMM_EVNT_TA_UPD_CMP,
   VB_EMM_EVNT_TA_UPD_REJ,

   VB_EMM_EVNT_SRVCE_PROC,
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_SRVCE_PROC,

   VB_EMM_EVNT_INVALID,

   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_INVALID,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,
   VB_EMM_EVNT_COMMON_PROC,


   VB_EMM_EVNT_STATUS,
   VB_EMM_EVNT_INFO,
   VB_EMM_EVNT_NAS_TRANPORT,
   VB_EMM_EVNT_NAS_TRANPORT,
   VB_EMM_EVNT_CS_NOTIFY,
   VB_EMM_EVNT_MAX        /*KWORKS Fix - to check for MAX events that can be handled*/
};

/* Matrix to handle outgoing common procedure messages */
PRIVATE VBPF VbEmmCmnProcOutSm[CM_EMM_UE_MAX_CMN_PROC_STATES][CM_EMM_UE_MAX_CMN_PROC_EVENTS] =
{
   /* VB_EMM_COMM_PROC_IDLE */
   {
      vbMmeEmmHdlGutiReallocCmd, /* GUTI Reallocation */
#ifdef VB_MME_AUTH
      vbMmeEmmHdlAuthReq,        /* Authentication normal */
      vbMmeEmmHdlAuthRej,        /* Authentication abnormal */
#else
      vbMmeEmmHdlOutEvntErr,
      vbMmeEmmHdlOutEvntErr,
#endif
      vbMmeEmmHdlIdReq,          /* Identity */
#ifdef VB_MME_AUTH
      vbMmeEmmHdlSecModeCmd,     /* Security mode normal */
#else
      vbMmeEmmHdlOutEvntErr,
#endif
      vbMmeEmmHdlOutEvntErr,        /* Security mode abnormal */
      vbMmeEmmHdlAttachAccCmnProc, /* Attach Accept */
      vbMmeEmmHdlOutEvntErr,        /* Attach complete */
      vbMmeEmmHdlOutEvntErr        /* TAU complete */
   },
   /* VB_EMM_COMM_PROC_INIT */
   {
      vbMmeEmmHdlOutEvntErr,        /* GUTI Reallocation */
#ifdef VB_MME_AUTH 
      vbMmeEmmHdlAuthReq,        /* Authentication normal */
      vbMmeEmmHdlAuthRej,        /* Authentication abnormal */
#else
      vbMmeEmmHdlOutEvntErr,
#endif
      vbMmeEmmHdlOutEvntErr,        /* Identity */
#ifdef VB_MME_AUTH 
      vbMmeEmmHdlSecModeCmd,        /* Security mode normal */
#endif
      vbMmeEmmHdlOutEvntErr,         /* Security mode abnormal */
      vbMmeEmmHdlAttachAccCmnProc, /* Attach Accept */
      vbMmeEmmHdlOutEvntErr,        /* Attach complete */
      vbMmeEmmHdlOutEvntErr        /* TAU complete */
   }
};

/* Matrix to handle incoming common procedure messages */
PRIVATE VBPF VbEmmCmnProcInSm[CM_EMM_UE_MAX_CMN_PROC_STATES][CM_EMM_UE_MAX_CMN_PROC_EVENTS] =
{
   /* VB_EMM_COMM_PROC_IDLE */
   {
      vbMmeEmmHdlEvntErr,        /* GUTI Reallocation */
      vbMmeEmmHdlEvntErr,        /* Authentication normal*/
      vbMmeEmmHdlEvntErr,        /* Authentication abnormal */
      vbMmeEmmHdlEvntErr,        /* Identity */
      vbMmeEmmHdlEvntErr,        /* Security normal */
      vbMmeEmmHdlEvntErr,        /* Security abnormal */
      vbMmeEmmHdlEvntErr,        /* Attach Accept */
      vbMmeEmmHdlAttachComplCmnProc,     /* Attach complete */
      vbMmeEmmHdlEvntErr         /* TAU Complete */
   },
   /* VB_EMM_COMM_PROC_INIT */
   {
      vbMmeEmmHdlGutiReallocCmp, /* GUTI Reallocation */
#ifdef VB_MME_AUTH
      vbMmeEmmHdlAuthRsp,        /* Authentication normal */
      vbMmeEmmHdlAuthFail,       /* Authentication abnormal */
#else
      vbMmeEmmHdlEvntErr,
      vbMmeEmmHdlEvntErr,
#endif
      vbMmeEmmHdlIdRsp,          /* Identity */
#ifdef VB_MME_AUTH
      vbMmeEmmHdlSecModeCmp,     /* Security mode normal */
      vbMmeEmmHdlSecModeRej,     /* Security mode abnormal */
#else
      vbMmeEmmHdlEvntErr,
      vbMmeEmmHdlEvntErr,
#endif
      vbMmeEmmHdlEvntErr, /* Attach Accept */
      vbMmeEmmHdlAttachComplCmnProc,        /* Attach complete */
      vbMmeEmmHdlTAUCmp   /* TAU Complete */
   }
};

/* The EMM state machine matrix referred for all outgoing EMM messages. */
PRIVATE VBPF VbEmmOutUeSm[VB_EMM_UE_MAX_STATES][VB_EMM_UE_MAX_EVENTS] =
{
   /* VB_EMM_UE_DEREGISTERED State */
   {
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlAttachAccpt,   /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlAttachReject,  /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlTAURej,           /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlOutSrvceProc,     /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlOutCommonProc,    /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlMmeStatus,     /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

   /* VB_EMM_UE_COMN_PROC_INIT State */
   {
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlAttachAccpt,   /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlAttachReject,  /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlOutCommonProc, /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlMmeStatus,     /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

   /* VB_EMM_UE_REG_INIT State */
   {
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlOutCommonProc,    /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlMmeStatus,     /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

    /* VB_EMM_UE_REGISTERED State */
   {
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlNwDetachReq,   /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */

   vbMmeEmmHdlOutEvntErr,      /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlTAUAcc,      /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlOutEvntErr,      /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlTAURej,      /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlOutSrvceProc,     /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlOutEvntErr,        /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlOutNasTranport,   /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlOutCommonProc,    /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlMmeStatus,     /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlEmmInfo,      /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlCsNotify,  /* VB_EMM_EVNT_CS_NOTIFY */

   },


  /* VB_EMM_UE_DEREG_INIT State */
   {
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlNwDetachAccpt, /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlMmeStatus,     /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlOutEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlOutEvntErr        /* VB_EMM_EVNT_CS_NOTIFY */

   }

};


/* The EMM state machine matrix referred for all incoming EMM messages. */
PRIVATE VBPF VbEmmIncUeSm[VB_EMM_UE_MAX_STATES][VB_EMM_UE_MAX_EVENTS] =
{
   /* VB_EMM_UE_DEREGISTERED State */
   {
   vbMmeEmmHdlAttachReq,     /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */

   vbMmeEmmHdlTAUInDeReg,   /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlEvntErr,      /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlEvntErr,      /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlEvntErr,      /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlServReqInDeReg,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlInCommonProc,    /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlUeStatus,      /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

   /* VB_EMM_UE_COMN_PROC_INIT State */
   {
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlUeDetachReq,   /* VB_EMM_EVNT_UE_DETACH_REQ */
                             /* Valid if Identification is ongoing */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlInCommonProc,  /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlInCommonProc,    /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlUeStatus,      /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

   /* VB_EMM_UE_REG_INIT State */
   {
   vbMmeEmmHdlAttachReqRept, /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlAttachCompl,   /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlUeDetachReqCrossd,       /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlInCommonProc,  /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlUeStatus,      /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

  /* VB_EMM_UE_REGISTERED State */
   {
   vbMmeEmmHdlAttachReqRept,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlUeDetachReq,   /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_UE_DETACH_ACC */
   vbMmeEmmHdlTAUReq,        /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlIncSrvceProc,     /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlIncNasTranport,   /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlInCommonProc,    /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlUeStatus,      /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_CS_NOTIFY */

   },

  /* VB_EMM_UE_DEREG_INIT State */
   {
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_COMPL */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_ATTACH_REJ */
   vbMmeEmmHdlUeDetachReqCrossd,    /* VB_EMM_EVNT_UE_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NW_DETACH_ACC */
   vbMmeEmmHdlUeDetachAccpt, /* VB_EMM_EVNT_UE_DETACH_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REQ */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_ACC */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_CMP */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_TA_UPD_REJ */

   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_SRVCE_PROC  */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_PAGING */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_NAS_TRANPORT */

   vbMmeEmmHdlEvntErr,      /* VB_EMM_EVNT_COMMON_PROC */

   vbMmeEmmHdlUeStatus,      /* VB_EMM_EVNT_STATUS */
   vbMmeEmmHdlEvntErr,       /* VB_EMM_EVNT_INFO */
   vbMmeEmmHdlEvntErr        /* VB_EMM_EVNT_CS_NOTIFY */

   }

};


/*
    This function is the entry point for all the outgoing EPS mobility
    management messages. This is invoked from MME Controller to 
    process the outgoing EMM messages. 
*/

#ifdef ANSI
PUBLIC S16 vbMmeEmmHdlOutUeEvnt
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
VbMmeUeCtxt *ueCtxt,
CmNasEvnt     *evnt
)
#else
PUBLIC S16 vbMmeEmmHdlOutUeEvnt(eNbCb, ueCb, ueCtxt, evnt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
VbMmeUeCtxt *ueCtxt;
CmNasEvnt     *evnt;
#endif
{
   U32        ueCbKey;
   VbEmmEvnt  emmEvnt = VB_EMM_EVNT_INVALID;
   U8         state;
   S16        ret;

   VB_MME_TRC2(vbMmeEmmHdlOutUeEvnt)


	/* A NULLP check has been put to avoid klocwork warnings */
	if (NULLP == evnt)
	{
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                  "evnt is NULL"));
      RETVALUE(RFAILED);
	}

   if(NULLP == ueCb)
   {
      /* Retrieve the UeCb pointer from the eNodeB block, based on
         suConnId allocated by MME Controller */
      /* This suConnId can be same as eNB UE S1AP ID */
      VB_MME_GET_UEID((ueCtxt->suConnId), ueCbKey);

      vbMmeUtlFndUeCb(eNbCb, &ueCb, ueCbKey);
   }

   /*******************************************************/
   /* If UeCb is present invoke EMM FSM handler to process the message */
   /* Else No UE Cb Found - Assume the state is DEREGISTERED
    * Handle appropriately based on message */
   /*******************************************************/

   if(NULLP != ueCb)
   {
      state = ueCb->emmCb.state;
   }
   else
   {
      state = VB_EMM_UE_DEREGISTERED;
   }

   /*******************************************************/
   /* Get the mapped event type from EMM Message Type */
   /*******************************************************/
   VB_EMM_GET_EVNT(evnt->m.emmEvnt->msgId, CM_EMM_DIR_NW_TO_UE, emmEvnt);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"state (%d), emmEvnt (%d)", state, emmEvnt));
   if(emmEvnt >= VB_EMM_EVNT_ATTACH_REQ && emmEvnt <= VB_EMM_EVNT_CS_NOTIFY)
   {
      /* Invoke associated function handler for this event */
      ret = VbEmmOutUeSm[state][emmEvnt](eNbCb, ueCb, evnt, ueCtxt);
   }
   else
   {
      /* Unexpected event for EMM */
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                  "Invalid Message Id(%d)", evnt->m.emmEvnt->msgId));
      CM_FREE_NASEVNT(&evnt);
      /* Error - Inform the MME Controller */
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);

}



/*
    This function is the entry point for all the incoming EPS mobility
    management messages.This is invoked from MME Lower Interface module to 
    process the incoming EMM messages.
*/

#ifdef ANSI
PUBLIC S16 vbMmeEmmHdlIncUeEvnt
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCtxt *ueCtxt,
CmNasEvnt     *evnt
)
#else
PUBLIC S16 vbMmeEmmHdlIncUeEvnt(eNbCb, ueCtxt, evnt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCtxt *ueCtxt;
CmNasEvnt     *evnt;
#endif
{
   VbMmeUeCb         *ueCb   = NULLP;
   VbEmmEvnt         emmEvnt = VB_EMM_EVNT_INVALID;
   U8                state   = VB_EMM_UE_DEREGISTERED;
   S16               ret     = ROK;
   U8                *pImsi = NULLP;
   U8                imsiLen = 0;
 
   VB_MME_TRC2(vbMmeEmmHdlIncUeEvnt);

   /* Checks whether the UE belongs to a HOME network or a roamer
    * The function checks for only Attach Request message and Identity
    * Response for other messages
    * MME will already has the UE context details
    */
   if (((CM_EMM_MSG_ATTACH_REQ == evnt->m.emmEvnt->msgId && \
       CM_EMM_MID_TYPE_IMSI == evnt->m.emmEvnt->u.atchReq.epsMi.type) ||
       (CM_EMM_MSG_ID_RSP == evnt->m.emmEvnt->msgId && \
       CM_EMM_MID_TYPE_IMSI == evnt->m.emmEvnt->u.idRsp.msId.type)))
   {
      if (CM_EMM_MSG_ATTACH_REQ == evnt->m.emmEvnt->msgId)
      {
         pImsi = evnt->m.emmEvnt->u.atchReq.epsMi.u.imsi.id;
         imsiLen = evnt->m.emmEvnt->u.atchReq.epsMi.len;
      }
      else
      {
         pImsi = evnt->m.emmEvnt->u.idRsp.msId.u.imsi.id;
         imsiLen = evnt->m.emmEvnt->u.idRsp.msId.len;
      }

		VB_MME_DBG_INFO((VB_MME_PRNTBUF, "NAS Message received from a "\
						"UE belonging to HOME PLMN"));
		if (CM_EMM_MSG_ID_RSP == evnt->m.emmEvnt->msgId)
		{
			 vbMmeUtlFndUeCbOnEvent(eNbCb, &ueCb, ueCtxt->suConnId,\
						ueCtxt->spConnId, evnt); 
		}
   }
   else
   {
      vbMmeUtlFndUeCbOnEvent(eNbCb, &ueCb, ueCtxt->suConnId,\
            ueCtxt->spConnId, evnt); 
   }

   /*******************************************************/
   /* If UeCb is present invoke EMM FSM handler to process the message */
   /* Else No UE Cb Found - Assume the state is DEREGISTERED
    * Handle appropriately based on message */
   /*******************************************************/
   if(NULLP != ueCb)
   {
      state = ueCb->emmCb.state;
   }
   else
   {
      state = VB_EMM_UE_DEREGISTERED;
   }

   /*******************************************************/
   /* Get the mapped event type from EMM Message Type */
   /*******************************************************/
   VB_EMM_GET_EVNT(evnt->m.emmEvnt->msgId, CM_EMM_DIR_UE_TO_NW, emmEvnt);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, " state (%d), emmEvnt (%d)\n", state, emmEvnt));
   if (emmEvnt >= VB_EMM_EVNT_ATTACH_REQ && emmEvnt <= VB_EMM_EVNT_CS_NOTIFY)
   {
      ret = VbEmmIncUeSm[state][emmEvnt](eNbCb, ueCb, evnt, ueCtxt);
   }
   else
   {
      /* Unexpected event for EMM */
      VB_MME_DBGP(("Invalid Message Id(%d)", evnt->m.emmEvnt->msgId));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);

}



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachReq
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt     *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachReq(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt     *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   SztENB_UE_S1AP_ID   *eNbIdTkn = NULLP;
   S16 ret      = ROK;
   U8  emmCause = 0;
   VbMmeEmmFlrInd flrInd;
   VbMmeEsmCb   *esmCb = NULLP;

   VB_MME_TRC2(vbMmeEmmHdlAttachReq)

   /**************************************************************/
   /* No existing ueCb present in the ueCb's list for this UE */
   /**************************************************************/
   if (!ueCb)
   {
      /*******************************************************/
      /* Allocate a new UE CB for this message */
      /*******************************************************/
      ret = vbMmeUtlAddUeCb(eNbCb, ueCtxt, &ueCb);
      if (ROK == ret && NULLP != ueCb)
      {
         ueCb->ecmCb.state = VB_ECM_CONNECTED;
         ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

         /* Find and Update the eNodeB UE S1APID*/
         /* Extract the eNB S1AP ID  here */
         vbMmeUtlGetS1apIE((SztS1AP_PDU *)&(((S1apPdu *)(evnt->pdu))->pdu),\
              Sztid_eNB_UE_S1AP_ID, (TknU8 **)&eNbIdTkn);
         if (eNbIdTkn == NULLP)
         {
            ret = RFAILED;
         }
         else
         {
            ueCb->eNbUeS1apId = (U16)(eNbIdTkn->val);
         }
      }

      if (ROK != ret)
      {
         VB_MME_DBGP(("Adding UE CB Failed"));
         /* Failure either memory loss or insertion failed */
         /* Inform back to UE with attach reject */
         /* No need to inform the user app again */

         flrInd.trg = CM_EMM_MSG_ATTACH_REQ;
         flrInd.cause = CM_EMM_NW_FAILURE;
         flrInd.evntPtr = (Ptr*)evnt;

			/* This NULL check has been introduced to avoid klocworks warning */
			if (ueCb != NULLP)
			{
				ret = vbMmeEmmFailInd(ueCb, &flrInd);
			}
      
         RETVALUE(ret);
      }
   }
   else
   {
      VB_MME_DBGP(("Stopping UE Context Delete Timer\n"));
      vbMmeStopTmr((PTR)ueCb, VB_TMR_UE_CTX_DEL);
      ueCb->ecmCb.state = VB_ECM_CONNECTED;

      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460); /* Auth/Sec timer */
      ret = vbMmeGetFrmTrnList(ueCb, &esmCb,
            ueCb->emmCb.piggyAction.tId);
      if((ROK == ret) && (NULLP != esmCb))
      {
         vbMmeStopTmr((PTR)ueCb, VB_TMR_ESM_3489); /* Esm Info Timer */
         vbMmeUtlDelEsmCb(esmCb, ueCb);
      }
   }

	/* This NULL check has been put to avoid klocworks tool warning */
	if (ueCb != NULLP)
	{
		(ueCb)->suConnId = ueCtxt->suConnId;
		(ueCb)->spConnId = ueCtxt->spConnId;
	}

   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
	/* This NULL check has been put to avoid klocworks tool warning */
   if(evnt->m.emmEvnt->u.atchReq.esmEvnt && ueCb != NULLP)
   {
      /* Make sure to track that ESM is contained in EMM
         till the UE attach is complete */
      ueCb->emmCb.esmPiggyBacked = TRUE;

      ret = vbEsmHdlPiggyBackIncUeEvnt(ueCb,\
            evnt->m.emmEvnt->u.atchReq.esmEvnt);
      if(ROK == ret)
      {
         emmCause = CM_EMM_NW_FAILURE;
      }
   }
   else
   { 
      emmCause = CM_EMM_PROT_ERR_UNSP;
   }

   /* ESM message container is a mandatory parameter, reject if not present,
      Or Handling the message fails if it is present */
   if((!evnt->m.emmEvnt->u.atchReq.esmEvnt || ROK != ret) && ueCb != NULLP)
   {
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

      VB_MME_DBGP(("Handling ESM Message Container"
                    " failed, Sending Failure Indication To Application"));

      flrInd.trg = CM_EMM_MSG_ATTACH_REQ;
      flrInd.cause = emmCause;
      flrInd.evntPtr = (Ptr*)evnt;

      ret = vbMmeEmmFailInd(ueCb, &flrInd);
      
      RETVALUE(ret);
   }

   /* Dont need to change the state - remain in UE DEREGISTERED only */

   /*******************************************************/
   /* Invoke a certain function in MME Controller to send 
      it up to user */
   /*******************************************************/
	/* This NULL check has been introduced to avoid klocworks warning */
	if (ueCb != NULLP)
	{
		ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_ATTACH_REQ);
	}

   RETVALUE(ROK);

} /* vbMmeEmmHdlAttachReq */

/*******************************************************/
/* All EMM Common Procedures
   1. Identity Request - Response
   2. Authentication Request - Response
   3. Security Mode Command - Complete 
   4. GUTI Reallocation Request - Response */
/*******************************************************/
#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlGutiReallocCmd
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt     *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlGutiReallocCmd(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt     *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlGutiReallocCmd);

   /* Enter into common procedure state */
   ueCb->emmCb.state = VB_EMM_UE_COMN_PROC_INIT;

   /* Change the sub-state so that we will handle the next
      message in the same common proc handler but in 
      a different sub-state */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_INIT;

   /* set the relevant common procedure bit */
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_GUTI_RALC_CMD;

   /* start the 3450 timer counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3450] = 1;

   /* Start T3450 timer for this UE */
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3450, eNbCb->eNbCfg.t3450Tmr);

   /* Send the authentication request message down to network */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf,\
         Sztid_downlinkNASTport, TRUE);

   /* Roll back in case of a failure at lower layer */
   if (ROK != ret)
   {
      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
      
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);

      ueCb->emmCb.state = VB_EMM_UE_REGISTERED;
      ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
      ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   }

   RETVALUE(ret);

} /* vbMmeEmmHdlGutiReallocCmd */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlGutiReallocCmp
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt     *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlGutiReallocCmp(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt     *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlGutiReallocCmp);

   /* We have completed the Authentication procedure */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.state = VB_EMM_UE_REGISTERED;

   /* Stop the timer, as we received Auth Resp*/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
    
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);

   /* set the relevant common procedure bit */
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_GUTI_RALC_CMP;

   /* reset the 3450 counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3450] = 0;

   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_GUTI_RLC_CMP);

   RETVALUE(ret);

} /* vbMmeEmmHdlGutiReallocCmp */

#ifdef VB_MME_AUTH
#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAuthReq
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAuthReq(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlAuthReq);

   /* Change the sub-state so that we will handle the next
      message in the same common proc handler but in 
      a different sub-state */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_INIT;

   /* set the relevant common procedure bit */
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_AUTH_REQ;

   /* start the 3460 timer counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 1;

   /* Start T3460 timer for this UE */
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3460, eNbCb->eNbCfg.t3460Tmr);

   /* Send the authentication request message down to network */
   /* vb001.101: Added new argument nasMsgPres */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf,\
         Sztid_downlinkNASTport, TRUE);

   /* Roll back in case of a failure at lower layer */
   if (ROK != ret)
   {
      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
      /* vb006.101:ccpu00120671 Added NULLP check*/
      if(ueCb->emmCb.retrBuf.val != NULLP)
      {
         VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU);
      }
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
      ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
      ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   }

   RETVALUE(ret);

} /* vbMmeEmmHdlAuthReq */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAuthRsp
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAuthRsp(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlAuthRsp);

   /* Check whether we are in a valid state to received Auth Rsp */
   if (!((ueCb->emmCb.emmCommProcInProgress) & VB_EMM_COM_PRC_AUTH_REQ))
   {
      VB_MME_DBGP(("Unexpected Message(%d)", CM_EMM_MSG_AUTH_RSP));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   /* We have completed the Authentication procedure */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

   /* Stop the timer, as we received Auth Resp*/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);

   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_AUTH_RSP;
   /* reset the 3460 counter for use by Security Mode Command procedure */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 0;

   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_AUTH_RSP);

   RETVALUE(ret);

} /* vbMmeEmmHdlAuthRsp */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAuthFail
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAuthFail(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlAuthFail);

   /* Check whether we are in a valid state to receive Auth Failure */
   if (!((ueCb->emmCb.emmCommProcInProgress) & VB_EMM_COM_PRC_AUTH_REQ))
   {
      VB_MME_DBGP(("Unexpected Message(%d)", CM_EMM_MSG_AUTH_FAIL));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   /* Stop the timer, as we received Auth Fail */
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   /* vb006.101:ccpu00120671 passing length of  allocated buffer*/
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU);
   /* Back to the state de-registered and sub-state idle */
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   /* reset the 3460 counter for use by Security Mode Command procedure */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 0;

   /* Send the message to MME for action */
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_AUTH_FAIL);

   RETVALUE(ret);

} /* vbMmeEmmHdlAuthFail */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAuthRej
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAuthRej(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlAuthRej);

   /* Check whether we are in a valid state to send the Auth Rej */
   if (!((ueCb->emmCb.emmCommProcInProgress) & VB_EMM_COM_PRC_AUTH_RSP))
   {
      VB_MME_DBGP(("Unexpected Message(%d)", CM_EMM_MSG_AUTH_REJ));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   /* Change the sub-state so that we will handle the next
      message in the same common proc handler but in 
      a different sub-state */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   /* Send the authentication request message down to network */
   /* vb001.101: Added new argument nasMsgPres */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   RETVALUE(ret);

} /* vbMmeEmmHdlAuthRej */
#endif /* VB_MME_AUTH */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlIdReq
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlIdReq(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlIdReq);

   /* Change the sub-state so that we will handle the next
      message in the same common proc handler but in 
      a different sub-state */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_INIT;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_ID_REQ;

   /* Start T3470 timer for this UE */
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3470, eNbCb->eNbCfg.t3470Tmr);

   /* Send the Id request message down to network */
   /* vb001.101: Added new argument nasMsgPres */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   /* Roll back in case of a failure at lower layer */
   if (ROK != ret)
   {
      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);

      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
      ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
      ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   }

   RETVALUE(ret);

} /* vbMmeEmmHdlIdReq */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlIdRsp
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlIdRsp(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlIdRsp);

   /* Check whether we are in a valid state to receive Id Req */
   if (!((ueCb->emmCb.emmCommProcInProgress) & VB_EMM_COM_PRC_ID_REQ))
   {
      VB_MME_DBGP(("Unexpected Message(%d)", CM_EMM_MSG_ID_RSP));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   /* We have completed the Authentication procedure */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   /* Stop the timer, as we received Identity Resp*/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   ueCb->emmCb.retrBuf.len = 0;
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3470] = 0;

   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_ID_RSP);

   RETVALUE(ret);

} /* vbMmeEmmHdlIdRsp */

#ifdef VB_MME_AUTH
#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlSecModeCmd
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlSecModeCmd(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlSecModeCmd);

#ifdef VB_MME_NAS_SEC
   if(VB_MME_SEC_HT_INT_PRTD_NEW_SEC_CTXT != evnt->secHT)
   {
      VB_MME_DBGP(("Message Must Be Int Protected And Ciphered, "
                   "Ignore The Message"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
#endif

   /* change the sub-state to INIT */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_INIT;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_SEC_MOD_CMD;

   /* Start T3460 timer for this UE */
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3460, eNbCb->eNbCfg.t3460Tmr);

   /* Send the message down to network */
   /* vb001.101: Added new argument nasMsgPres */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf,\
         Sztid_downlinkNASTport, TRUE);

   /* start the 3460 timer counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 1;

   /* Roll back in case of a failure at lower layer */
   if (ROK != ret)
   {

      /* Stop the timer, as we are unable to send the message */
      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
      /* vb006.101:ccpu00120671 Added NULLP check*/
      if(ueCb->emmCb.retrBuf.val !=NULLP)
      {
         VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU);
      }
      /* start the 3460 timer counter */
      ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 0;
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
      ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
      ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   }

   RETVALUE(ret);

} /* vbMmeEmmHdlSecModeCmd */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlSecModeCmp
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlSecModeCmp(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlSecModeCmp);

   /* Check whether we are in a valid state to receive Sec mode cmp */
   if (!((ueCb->emmCb.emmCommProcInProgress) & VB_EMM_COM_PRC_SEC_MOD_CMD))
   {
      VB_MME_DBGP(("Unexpected Message(%d)", CM_EMM_MSG_SEC_MODE_CMP));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   /* Stop the timer, as we received security mode complete */
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);

   /* The common procedures are complete, 
      change state and give the message to MME */
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   /* reset the 3460 counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 0;

   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_SEC_MODE_CMP);

   RETVALUE(ret);

} /* vbMmeEmmHdlSecModeCmp */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlSecModeRej
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlSecModeRej(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlSecModeRej);

   /* Check whether we are in a valid state to receive Sec mode cmp */
   if (!((ueCb->emmCb.emmCommProcInProgress) & VB_EMM_COM_PRC_SEC_MOD_CMD))
   {
      VB_MME_DBGP(("Unexpected Message(%d)", CM_EMM_MSG_SEC_MODE_REJ));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   /* Stop the timer, as we received security mode reject */
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);

   /* Roll back the state and give to MME */
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   /* reset the 3460 counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] = 0;

   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_SEC_MODE_REJ);

   RETVALUE(ret);

} /* vbMmeEmmHdlSecModeRej */
#endif /* VB_MME_AUTH */



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlInCommonProc
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlInCommonProc(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;
   VbEmmCmnPrcEvt cmnProcEvnt = VB_EMM_EVNT_CMN_PRC_INVALID;

   VB_MME_TRC2(vbMmeEmmHdlInCommonProc);


   if(NULLP == evnt)
   {
      VB_MME_DBGP(("NULL Common Procedure Event"));
      RETVALUE(RFAILED);
   }

	/* This statement has been moved from above to here to 
	 * avoid klocwork warnings */ 
   VB_EMM_GET_COMMON_PROC_EVNT(evnt->m.emmEvnt->msgId, cmnProcEvnt);

   if(NULLP == ueCb)
   {
      VB_MME_DBGP(("NULL UeCb"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   if (cmnProcEvnt == VB_EMM_EVNT_CMN_PRC_INVALID)
   {
      VB_MME_DBGP(("Unexpected Message(%d)", evnt->m.emmEvnt->msgId));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   ret = VbEmmCmnProcInSm[ueCb->emmCb.emmCommProcSubState][cmnProcEvnt]
                         (eNbCb, ueCb, evnt, ueCtxt);

   RETVALUE(ret);
} /* vbMmeEmmHdlInCommonProc */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlOutCommonProc
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlOutCommonProc(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;
   VbEmmCmnPrcEvt cmnProcEvnt = VB_EMM_EVNT_CMN_PRC_INVALID;

   VB_MME_TRC2(vbMmeEmmHdlOutCommonProc);

   VB_EMM_GET_COMMON_PROC_EVNT(evnt->m.emmEvnt->msgId, cmnProcEvnt);

   if (cmnProcEvnt == VB_EMM_EVNT_CMN_PRC_INVALID)
   {
      VB_MME_DBGP(("Unexpected Message(%d)", evnt->m.emmEvnt->msgId));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   ret = VbEmmCmnProcOutSm[ueCb->emmCb.emmCommProcSubState][cmnProcEvnt]
                          (eNbCb, ueCb, evnt, ueCtxt);

   RETVALUE(ret);
} /* vbMmeEmmHdlOutCommonProc */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachAccpt
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachAccpt(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16           ret = ROK;
   VbMmeEsmCb    *esmCb = NULLP;

   VB_MME_TRC2(vbMmeEmmHdlAttachAccpt)

#ifdef VB_MME_NAS_SEC
   if(VB_MME_SEC_HT_INT_PRTD_ENC != evnt->secHT)   
   {
      VB_MME_DBGP(("Message Must Be Int Protected And Ciphered, "
                   "Ignore The Message"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
#endif

   /* Ue Cb is assumed to be present, as it has been created
      by Attach Request */
   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
   /* If Attach Request had ESM Piggy backed, its must we piggy back here too */
   /* Generate an error by aborting this procedure */
   if(ueCb->emmCb.esmPiggyBacked && !evnt->m.emmEvnt->u.atchAcc.esmEvnt)
   {
      VB_MME_DBGP(("ESM Message Not Present"));
      /* Inform the application back */
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   else if(evnt->m.emmEvnt->u.atchAcc.esmEvnt)
   {
      ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, ueCb->emmCb.piggyAction.bId, 
                                           ueCb->emmCb.piggyAction.tId);
      if(ROK != ret)
      {
      
         VB_MME_DBGP(("ESM Cb Not Present"));
         CM_FREE_NASEVNT(&evnt);
         RETVALUE(RFAILED);
      }

      /* Check for ESM control block state machine */
      ret = vbEsmHdlOutUeEvnt(esmCb, evnt->m.emmEvnt->u.atchAcc.esmEvnt);
      if(ROK != ret)
      {
         ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
         VB_MME_DBGP(("Handling ESM Message Failed"));
         CM_FREE_NASEVNT(&evnt);
         RETVALUE(RFAILED);
      }
   }

   /* Now Change the state */
   ueCb->emmCb.state = VB_EMM_UE_REG_INIT;

   /* start the 3450 timer counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3450] = 1;      

   /*******************************************************/
   /* Start T3450 timer for this UE */
   /*******************************************************/
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3450, eNbCb->eNbCfg.t3450Tmr);

   /*******************************************************/
   /* We might need to preserve a copy of this PDU to track the
    timer expiry of T3450 till 4th count */
   /* Invoke a lower interface module's common function to
      send it out to eNodeB */
   /*******************************************************/
   /* attach accept is going in init Ctxt setup req */
   /* vb001.101: Added new argument nasMsgPres */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, \
              Sztid_InitCntxtSetup, TRUE);

   /*******************************************************/
   /* Message is not sent out Roll Back */
   /*******************************************************/
   if(ROK != ret)
   {
      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);

      /* Roll back on ESM too */
      if(ueCb->emmCb.esmPiggyBacked)
      {
         vbEsmHdlRollBack(ueCb, esmCb, CM_ESM_MSG_ACTV_DEF_BEAR_REQ);
      }

      /*******************************************************/
      /* Do not remove the entry of this UE from ueCb List, 
         to help user send it again */
      /*******************************************************/
   }

   RETVALUE(ret);

} /* vbMmeEmmHdlAttachAccpt */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachReject
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachReject(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16          ret    = ROK;
   VbMmeEsmCb   *esmCb = NULLP;
   SztNAS_PDU *nasPdu = NULLP;

   VB_MME_TRC2(vbMmeEmmHdlAttachReject)

   /* Ue Cb is assumed to be present, as it has been created
      by Attach Request */
   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/

   if(evnt->m.emmEvnt->u.atchRej.esmEvnt)
   {
      ret = vbMmeGetFrmTrnList(ueCb, &esmCb, ueCb->emmCb.piggyAction.tId);
      if(ROK != ret)
      {
         ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, ueCb->emmCb.piggyAction.bId, 
                                              ueCb->emmCb.piggyAction.tId);
      }

      if(ROK != ret)
      {
         VB_MME_DBGP(("ESM Cb Not Found"));
         CM_FREE_NASEVNT(&evnt);
         RETVALUE(ret);
      }
      /* Check for ESM control block state machine */
      ret = vbEsmHdlOutUeEvnt(esmCb, evnt->m.emmEvnt->u.atchRej.esmEvnt);
   }

   /* Now Change the state */
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

   /*******************************************************/
   /* Invoke a lower interface module's common function to
      send it out to eNodeB */
   /*******************************************************/
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   /* valgrind Fix */
   nasPdu = &ueCb->emmCb.retrBuf;
   if(nasPdu->val != NULLP)
   {  
      EDM_FREE(nasPdu->val, CM_MAX_EMM_ESM_PDU /*nasPdu->len*/)
   }
   RETVALUE(ret);

} /* vbMmeEmmHdlAttachReject */



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachCompl
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachCompl(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlAttachCompl)

#ifdef VB_MME_NAS_SEC
   if(VB_MME_SEC_SUCCESS != evnt->secStatus)   
   {
      VB_MME_DBGP(("Security Validation Failed, Ignore The Message"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
#endif

   /* Ue Cb is assumed to be present, as it has been created
      by Attach Request */
   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
   /*******************************************************/
   /* If Attach Request had ESM Piggy backed, its must we 
      piggy back here too */
   /* Generate an error by aborting this procedure */
   /*******************************************************/
   if((!evnt->m.emmEvnt->u.atchCmp.esmEvnt))
   {
      /* Abort the UE attach procedure */
      /* Perform local release or S1AP Release */
      /* Inform the application back */

      VB_MME_DBGP(("ESM Message Not Present"));

#if 0
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
      vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
      ueCb->emmCb.retrBuf.len = 0;


      VbMmeEmmFlrInd flrInd;
      flrInd.trg = CM_EMM_MSG_ATTACH_CMP;
      flrInd.cause = CM_EMM_PROT_ERR_UNSP;
      frlInd.evntPtr = (Ptr*)evnt;
      flrPtr.ptr = NULLP;

      ret = vbMmeEmmFailInd(ueCb, &flrInd);
#endif
      
      /*Ignore the event, let us time out*/
      CM_FREE_NASEVNT(&evnt);

      RETVALUE(RFAILED);
   }
   else if(evnt->m.emmEvnt->u.atchCmp.esmEvnt)
   {
      /* Handle EPS Default Bearer Context Accept */
      ret = vbEsmHdlPiggyBackIncUeEvnt(ueCb, evnt->m.emmEvnt->u.atchCmp.esmEvnt);
      if(ROK != ret)
      {
         VB_MME_DBGP(("Handling ESM Pdu Failed"));

#if 0
         ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
         vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
         VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
         ueCb->emmCb.retrBuf.len = 0;

         /* Info application about failure*/

         VbMmeEmmFlrInd flrInd;
         flrInd.trg = CM_EMM_MSG_ATTACH_CMP;
         flrInd.cause = CM_EMM_PROT_ERR_UNSP;
         frlInd.evntPtr = (Ptr*)evnt;
         flrPtr.ptr = NULLP;

         ret = vbMmeEmmFailInd(ueCb, &flrInd);
#endif
  
         /*Ignore the event, let us time out*/
         CM_FREE_NASEVNT(&evnt);

         /* Inform the application back */
         RETVALUE(RFAILED);
      }
   }

   /* Update the state */
   ueCb->emmCb.state = VB_EMM_UE_REGISTERED;

   /*******************************************************/
   /* Stop T3450 timer for this UE */
   /*******************************************************/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);

   /* vb005.101: Added support for Network initiated Detach */
#ifdef VB_MME_NW_INIT_DETACH
   /*******************************************************/
   /* Start network initiated detach timer */
   /*******************************************************/
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR,\
         eNbCb->eNbCfg.tNwDetachTmr);
#endif /* end of VB_MME_NW_INIT_DETACH */


   /* Free any PDU preserved to track the retransmission
      of attach accept */
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   ueCb->emmCb.retrBuf.len = 0;

   /*******************************************************/
   /* Invoke a certain function in MME Controller to send 
      it up to user */
   /*******************************************************/
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_ATTACH_CMP);
   RETVALUE(ret);
} /* vbMmeEmmHdlAttachCompl */



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlUeDetachReq
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlUeDetachReq(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlUeDetachReq)

   ueCb->ecmCb.state = VB_ECM_CONNECTED;

   /*******************************************************/
   /* Check for the Detach type                           */
   /*******************************************************/
   if(evnt->m.emmEvnt->u.dtchReq.detchType.switchOff)
   {
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   }
   else
   {
      ueCb->emmCb.state = VB_EMM_UE_DEREG_INIT;
   }

   /*******************************************************/
   /* Invoke a certain function in MME Controller to send 
      it up to user */
   /*******************************************************/
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_DETACH_REQ);


   RETVALUE(ret);

} /* vbMmeEmmHdlUeDetachReq */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlNwDetachAccpt
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlNwDetachAccpt (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;
   VB_MME_TRC2(vbMmeEmmHdlNwDetachAccpt)

   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);
   
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   
   RETVALUE(ret); 

} /* vbMmeEmmHdlNwDetachAccpt */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlNwDetachReq
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlNwDetachReq (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;


   VB_MME_TRC2(vbMmeEmmHdlNwDetachReq)

   /* vb005.101: Added support for Network initiated Detach */
   /*******************************************************/
   /* Nw initiated Detach Request            */
   /*******************************************************/
   /* Now Change the state */
   ueCb->emmCb.state = VB_EMM_UE_DEREG_INIT;

   /* start the 3422 timer counter */
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3422] = 1;

   /*******************************************************/
   /* Start T3422 timer for this UE */
   /*******************************************************/
   vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3422, eNbCb->eNbCfg.t3422Tmr);

   /*******************************************************/
   /* Invoke a lower interface module's common function to
      send it out to eNodeB */
   /*******************************************************/
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf,\
         Sztid_downlinkNASTport, TRUE);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Detach Request Message"));

      /* Roll back */
      vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3422);
      ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3422] = 0;

      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
      ueCb->emmCb.retrBuf.len = 0;
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

      RETVALUE(ret);
   }

   RETVALUE(ret);
} /* vbMmeEmmHdlNwDetachReq */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlUeDetachAccpt
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlUeDetachAccpt (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   VB_MME_TRC2(vbMmeEmmHdlUeDetachAccpt)

   /*******************************************************/
   /* handle UE Detach Accept                             */
   /*******************************************************/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3422);
   ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3422] = 0;

   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   ueCb->emmCb.retrBuf.len = 0;

   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   RETVALUE(vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_DETACH_ACC));

} /* vbMmeEmmHdlUeDetachAccpt */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlIncSrvceProc
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlIncSrvceProc (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;
   SztENB_UE_S1AP_ID  *sztENB_UE_S1AP_ID = NULLP;
#ifdef VB_MME_NAS_SEC
   VbMmeMsg     srcMsg;
   VbMmeMsg     dstMsg;
#endif

   VB_MME_TRC2(vbMmeEmmHdlIncSrvceProc)
    
   (ueCb)->suConnId = ueCtxt->suConnId;
   (ueCb)->spConnId = ueCtxt->spConnId;
   ueCb->ecmCb.state = VB_ECM_CONNECTED;
   ueCb->proc = VB_MME_SERVICE_REQ_PROC;

   vbMmeUtlGetS1apIE(&(((S1apPdu*)evnt->pdu)->pdu), Sztid_eNB_UE_S1AP_ID,
         (TknU8 ** )&sztENB_UE_S1AP_ID);

   /* NULL check has been put to avoid klocwork warning */
	if (sztENB_UE_S1AP_ID != NULLP)
	{
		ueCb->eNbUeS1apId = sztENB_UE_S1AP_ID->val; 
	}

#ifdef VB_MME_NAS_SEC
   if (VB_MME_SEC_SERVREQ == evnt->secStatus)
   {
      evnt->secStatus = VB_MME_SEC_FAILURE;   
      if (evnt->m.emmEvnt->u.srvReq.ksiSeqNum.Ksi == \
         ueCb->ueCtxt.secCtxt.ksi)
      {
         ret = vbMmeUtlGetNasMsg(evnt, &srcMsg.val, &srcMsg.len);
         if (ROK == ret)
         {
            ret = vbMmeVldULSec(&ueCb->ueCtxt.secCtxt, &srcMsg, &dstMsg);
            if (ROK == ret)
            {
               evnt->secStatus = VB_MME_SEC_SUCCESS;   
            }
         }
      }
   } 
#endif

   /* send the message to MME for handling */
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_SERVICE_REQ);

   RETVALUE(ret);

} /* vbMmeEmmHdlIncSrvceProc */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlOutSrvceProc
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlOutSrvceProc (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlOutSrvceProc)

   /* Send the message to lower layer for sending on to network */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   RETVALUE(ret);

} /* vbMmeEmmHdlOutSrvceProc */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlIncNasTranport
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlIncNasTranport (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlIncNasTranport)

   /* send the message to MME for handling */
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_UL_NAS_TPT);

   RETVALUE(ret);

} /* vbMmeEmmHdlIncNasTranport */



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlOutNasTranport
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlOutNasTranport (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlOutNasTranport)

   /* Send the message to lower layer for sending on to network */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   RETVALUE(ret);

} /* vbMmeEmmHdlOutNasTranport */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlEmmInfo
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlEmmInfo (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlEmmInfo)

   /* Send the message to lower layer for sending on to network */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   RETVALUE(ret);

} /* vbMmeEmmHdlEmmInfo */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlCsNotify
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlCsNotify (eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlCsNotify)

   /* Send the message to lower layer for sending on to network */
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   RETVALUE(ret);

} /* vbMmeEmmHdlCsNotify */



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlUeDetachReqCrossd
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlUeDetachReqCrossd(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlUeDetachReqCrossd)

   /*******************************************************/
   /* Stop T3450 all timer */
   /*******************************************************/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);


   /* Free any PDU preserved to track the retransmission
      of attach accept */
   if(NULLP != ueCb->emmCb.retrBuf.val)
   {
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
      ueCb->emmCb.retrBuf.len = 0;
   }

   /*******************************************************/
   /* Check for the Detach type                           */
   /*******************************************************/
   if(evnt->m.emmEvnt->u.dtchReq.detchType.switchOff)
   {
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   }
   else
   {
      ueCb->emmCb.state = VB_EMM_UE_DEREG_INIT;
   }

   /*******************************************************/
   /* Invoke a certain function in MME Controller to send 
      it up to user */
   /*******************************************************/
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_DETACH_REQ);
   
   RETVALUE(ret);

} /* vbMmeEmmHdlUeDetachReqCrossd */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachReqRept
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachReqRept(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret      = ROK;
   U8  emmCause = 0;
   VbMmeEmmFlrInd flrInd;

   VB_MME_TRC2(vbMmeEmmHdlAttachReqRept)
   /* Stop T3450 Timer */
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
   VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   ueCb->emmCb.retrBuf.len = 0;

   ueCb->ecmCb.state = VB_ECM_CONNECTED;

   if(VB_EMM_UE_S1AP_CONNECTED == ueCb->ueCtxt.s1apConState)
   {
      VB_MME_DBGP(("Attach Request Not Handled In Connected State"));
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
      flrInd.trg = CM_EMM_MSG_ATTACH_REQ;
      flrInd.cause = CM_EMM_PROT_ERR_UNSP;
      flrInd.evntPtr = (Ptr*)evnt;

      ret = vbMmeEmmFailInd(ueCb, &flrInd);

      RETVALUE(ret);
   }
   /* In case of LTE-Dongles the second time attach
    * is something like new attach only. Ue states will be
    * EMM_REGISTERED and ECM_IDLE. In this state MME has to
    * maintain all the UE context, so removing of UeCb is 
    * not required here.
    */ 
#if 0
   /* Repeated Attach Request after sending Attach Accept to UE */
   /* We need to compare the previously recieved Attach Request's UE Context
      match the parameters [preserve the PDU] */
   /* Right now we dont assume this to happen */
   vbMmeUtlDelUeCbOnSTmsi(ueCb);
   vbMmeUtlDelUeCbOnImsi(ueCb);
#endif
   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
   if(evnt->m.emmEvnt->u.atchReq.esmEvnt)
   {
      /* Make sure to track that ESM is contained in EMM
         till the UE attach is complete */
      ueCb->emmCb.esmPiggyBacked = TRUE;
      ret = vbEsmHdlPiggyBackIncUeEvnt(ueCb,\
            evnt->m.emmEvnt->u.atchReq.esmEvnt);
      if(ROK == ret)
      {
         emmCause = CM_EMM_NW_FAILURE;
      }
   }
   else
   {
      emmCause = CM_EMM_PROT_ERR_UNSP;
   }
   /* ESM message container is a mandatory parameter, reject if not present,
      Or Handling the message fails if it is present */
   if(!evnt->m.emmEvnt->u.atchReq.esmEvnt || ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling ESM Message Container"
            " failed, Sending Failure Indication To Application"));
      ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
      flrInd.trg = CM_EMM_MSG_ATTACH_REQ;
      flrInd.cause = emmCause;
      flrInd.evntPtr = (Ptr*)evnt;

      ret = vbMmeEmmFailInd(ueCb, &flrInd);

      RETVALUE(ret);
   } 

   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

   RETVALUE(vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_ATTACH_REQ));

} /* vbMmeEmmHdlAttachReqRept */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachAccCmnProc
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachAccCmnProc(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   VB_MME_TRC2(vbMmeEmmHdlAttachAccCmnProc)

   /* Attach Accept when Common Procedure(s) are in progress is valid
      one */
   /* Right now we dont assume this to happen as we dont initiate 
      any common procedures identified during UE Attach */

   RETVALUE(vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_ATTACH_ACC));

} /* vbMmeEmmHdlAttachAccCmnProc */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlAttachComplCmnProc
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlAttachComplCmnProc(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   VB_MME_TRC2(vbMmeEmmHdlAttachComplCmnProc)

   /* Attach Complete when Common Procedure(s) are in progress is valid
      one */
   /* Right now we dont assume this to happen as we dont initiate 
      any common procedures identified during UE Attach */

   RETVALUE(vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_ATTACH_CMP));

} /* vbMmeEmmHdlAttachComplCmnProc */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlEvntErr
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlEvntErr(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   VB_MME_TRC2(vbMmeEmmHdlEvntErr)
   if(NULLP == evnt)
   {
      RETVALUE(RFAILED);
   }
   if(NULLP == ueCb)
   {
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   VB_MME_DBGP(("Invalid Message(%d) In State(%d)", 
                     evnt->m.emmEvnt->msgId, ueCb->state));
   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message(%d) In State(%d)", 
                             evnt->m.emmEvnt->msgId, ueCb->emmCb.state));

   /* Send Emm Status message only in S1AP Connected State */
   if(ueCb->ueCtxt.s1apConState == VB_EMM_UE_S1AP_CONNECTED)
   {
      vbMmeSndEmmStatus(ueCb, CM_EMM_INV_MSG_TYPE_IN_PROT_STATE);
   }
   
   CM_FREE_NASEVNT(&evnt);

   RETVALUE(RFAILED);

} /* vbMmeEmmHdlEvntErr */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlOutEvntErr
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlOutEvntErr(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   VB_MME_TRC2(vbMmeEmmHdlOutEvntErr)
   if(NULLP == evnt)
   {
      RETVALUE(RFAILED);
   }
   VB_MME_DBGP(("Invalid Message(%d) In State(%d)", 
                     evnt->m.emmEvnt->msgId, ueCb->state));
   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message(%d) In State(%d)", 
                             evnt->m.emmEvnt->msgId, ueCb->emmCb.state));

   CM_FREE_NASEVNT(&evnt);
   RETVALUE(RFAILED);

} /* vbMmeEmmHdlOutEvntErr */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlTAUReq
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt     *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlTAUReq(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt     *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   S16 ret      = ROK;

   VB_MME_TRC2(vbMmeEmmHdlTAUReq)

   if(!ueCb)
   {
      VB_MME_DBGP(("UE CB Not Found, Ignore The Message\n"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(ret);
   }

   ueCb->ecmCb.state = VB_ECM_CONNECTED;
   /*******************************************************/
   /* Invoke a certain function in MME Controller to send 
      it up to user */
   /*******************************************************/
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_TAU_REQ);
   RETVALUE(ROK);

} /* vbMmeEmmHdlTAUReq */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlTAUCmp
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt     *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlTAUCmp(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt     *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   S16 ret      = ROK;

   VB_MME_TRC2(vbMmeEmmHdlEvntTAUCmp)

   if(VB_EMM_COM_PRC_TAU_ACC != ueCb->emmCb.emmCommProcInProgress)
   {
      ret = vbMmeEmmHdlEvntErr( eNbCb, ueCb, evnt, ueCtxt);
      RETVALUE(ret);
   }

   ueCb->emmCb.state = VB_EMM_UE_REGISTERED;
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;

   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);

   /*******************************************************/
   /* Invoke a certain function in MME Controller to send 
      it up to user */
   /*******************************************************/
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_TAU_CMP);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlTAUAcc
(
VbMmeEnbCb   *eNbCb,
VbMmeUeCb    *ueCb,
CmNasEvnt      *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlTAUAcc(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb   *eNbCb;
VbMmeUeCb    *ueCb;
CmNasEvnt      *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16           ret = ROK;
   U8            gutiFlag = 0;

   VB_MME_TRC2(vbMmeEmmHdlTAUAcc)

   gutiFlag = evnt->m.emmEvnt->u.tauAcc.guti.pres;

   /*******************************************************/
   /* Start T3450 timer for this UE */
   /*******************************************************/
   if(gutiFlag)
   {
      /* Change the sub-state so that we will handle the next
         message in the same common proc handler but in 
         a different sub-state */
      ueCb->emmCb.state = VB_EMM_UE_COMN_PROC_INIT; 
      ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_INIT;

      /* set the relevant common procedure bit */
      ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_TAU_ACC;

      /* start the 3450 timer counter */
      ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3450] = 1;      

      /* Now Change the state */
      vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3450, eNbCb->eNbCfg.t3450Tmr);
   }

   if(TRUE != ueCb->ueCtxt.tauActv)
   {
       ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, \
              Sztid_downlinkNASTport, TRUE);
   }
   else
   {
       ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, \
              Sztid_InitCntxtSetup, TRUE);
   }

   /*******************************************************/
   /* Message is not sent out Roll Back */
   /*******************************************************/
   if(ROK != ret)
   {
      if(gutiFlag)
      {
         vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
      }
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   }
   RETVALUE(ret);

} /* vbMmeEmmHdlTAUAcc */

#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlTAURej
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlTAURej(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16          ret    = ROK;

   VB_MME_TRC2(vbMmeEmmHdlTAURej)

   /* Now Change the state */
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;

   /*******************************************************/
   /* Invoke a lower interface module's common function to
      send it out to eNodeB */
   /*******************************************************/
   ret = vbMmeLimSndToLower(ueCb, evnt, &ueCb->emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);

   RETVALUE(ret);

} /* vbMmeEmmHdlTAURej */



#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlUeStatus
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlUeStatus(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16 ret = ROK;

   VB_MME_TRC2(vbMmeEmmHdlUeStatus)

   /* Give the message to MME, no need to change state */
   ret = vbMmeRcvEmmMsg(ueCb, evnt, CM_EMM_MSG_EMM_STS);

   RETVALUE(ret);

} /* vbMmeEmmHdlUeStatus */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlMmeStatus
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt  *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlMmeStatus(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt  *ueCtxt;
#endif
{
   S16         ret  = ROK;
   SztNAS_PDU  pdu;

   VB_MME_TRC2(vbMmeEmmHdlMmeStatus)

   cmMemset((U8*)&pdu, 0, sizeof(SztNAS_PDU));

   /* Send the message to network */
   ret = vbMmeLimSndToLower(ueCb, evnt, &pdu, Sztid_downlinkNASTport, TRUE);

   VB_MME_FREE(pdu.val, pdu.len);

   RETVALUE(ret);

} /* vbMmeEmmHdlMmeStatus */



/**
 * @brief This function handles the EPS mobility management related timers'
 * expiry. 
 * 
 * @details The timers handled are:
        - VB_TMR_EMM_3450,
        - VB_TMR_EMM_3460,
        - VB_TMR_EMM_3470
        - VB_TMR_EMM_MOB_REACHABLE
        - VB_TMR_EMM_IMPLI_DTCH_TMR
        - VB_TMR_EMM_3413
        - VB_TMR_EMM_3422
 * After expiry of T3450 timer Attach Accept is sent upto a maximum of four
 * expiries, and after the 5th expiry the procedure is aborted.
 * After expiry of T3460 timer Auth Request is sent upto a maximum of four
 * times, and after the 5th expiry the procedure is aborted.
 * After expiry of T3470 timer Identity Request is sent upto a maximum of four
 * times, and after the 5th expiry the procedure is aborted.
 * @param[in] ueCb     An UECb pointer from this UE.
 * @param[in] tmrEvnt  A Timer event which triggered the timer expiry.
 *  
 * @return 
 * -# ROK on success. 
 * -# Suitable error code otherwise.
 *
 * 
 */
#ifdef ANSI
PUBLIC Void vbMmeEmmHdlTmrEvnt
(
VbMmeUeCb *ueCb,
S16       tmrEvnt
)
#else
PUBLIC Void vbMmeEmmHdlTmrEvnt(ueCb, tmrEvnt)
VbMmeUeCb *ueCb;
S16       tmrEvnt;
#endif
{
   S16              ret          = ROK;
   SztLclErrInd     sztLclErrInd;

   VB_MME_TRC2(vbMmeEmmHdlTmrEvnt);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Timer Event(%d) Expired", tmrEvnt));

   switch(tmrEvnt)
   {
      case VB_TMR_EMM_3413:
      {
         vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3413);
         break;
      }
      case VB_TMR_EMM_3422:
      {
         /* vb005.101: Added support for Network initiated Detach */
         if (ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3422] < VB_TMR_EMM_MAX_DTCH_REQ_ATMPS)
         {
            /* Send the stored buffer and increment the attempts counter */
            ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3422]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3422, ueCb->eNodebCb->eNbCfg.t3422Tmr);

            ret = vbMmeLimSndStoredBuf(ueCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored Detach Request"));
               vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3422);
               
               VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
               vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3422);
               /* Inform the application back */
               RETVOID;
            }
         }
         else
         {
            /* We tried for maximum number of times allowed as per the spec without a
               response so we will abort the procedure now and perform 
               local release and S1AP Release */
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                              "Of Detach Request Reached"));

            /* call handler after max retransmission reached */
            vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3422);
            RETVOID;
         }
         break;
      }
      case VB_TMR_EMM_3450: /* Attach Accept, TAU Acc and GUTI Reallocation Cmd*/
      {
         if (ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3450] < VB_TMR_EMM_MAX_ATCH_ACC_ATMPS)
         {
            /* Send the stored buffer and increment the attempts counter */
            ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3450]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3450, ueCb->eNodebCb->eNbCfg.t3450Tmr);

            ret = vbMmeLimSndStoredBuf(ueCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored Attach Accept"));
               vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
               VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
               vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3450);
               /* Inform the application back */
               RETVOID;
            }
         }
         else
         {
            /* We tried for maximum number of times allowed as per the spec without a
               response so we will abort the procedure now and perform 
               local release and S1AP Release */
            /* vb001.101: Modified function name */
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Maximum Number Of Retransmission "
                                              "Of Attach Accept Reached"));
            /* call handler after max retransmission reached */
            vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3450);
            /* Inform the application back */
            RETVOID;

         }
         break;
      }
      case VB_TMR_EMM_3460: /* Auth Req/Sec Mode Cmd */
      {
         if (ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460] < VB_TMR_EMM_MAX_AUTH_REQ_ATMPS)
         {
            /* Send the stored buffer and increment the attempts counter */
            ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3460]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3460, ueCb->eNodebCb->eNbCfg.t3460Tmr);

            ret = vbMmeLimSndStoredBuf(ueCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored Auth Req/Sec Mode Cmd"));
               vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
               VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
               vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3460);
               /* Inform the application back */
               RETVOID;

            }
         }
         else
         {
            /* We tried for maximum number of times allowed as per the spec without a
               response so we will abort the procedure now and perform 
               local release and S1AP Release */
            /* vb001.101: Modified function name */
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                              "Of Authentication/Security Mode Reached"));
            /* call handler after max retransmission reached */
            vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3460);
            /* Inform the application back */
            RETVOID;

         }
         break;
      }
      case VB_TMR_EMM_3470: /* Id Req */
      {
         if (ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3470] < VB_TMR_EMM_MAX_ID_REQ_ATMPS)
         {
            /* Send the stored buffer and increment the attempts counter */
            ueCb->emmCb.emmTimersCntr[VB_TMR_EMM_3470]++;
            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_3470, ueCb->eNodebCb->eNbCfg.t3470Tmr);
            ret = vbMmeLimSndStoredBuf(ueCb, Sztid_downlinkNASTport);
            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored Auth Req/Sec Mode Cmd"));
               vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);
               VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
               vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3470);
               /* Inform the application back */
               RETVOID;
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                    "Of Identity Request Reached"));
            /* call handler after max retransmission reached */
            vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_3470);
            /* Inform the application back */
            RETVOID;
         }
         break;
      }
      case VB_TMR_EMM_MOB_REACHABLE:
      {
#ifdef ALIGN_64BIT
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Mobile Reachable Timer Expired For UE(%d)", 
                                         ueCb->ueCbKey));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Mobile Reachable Timer Expired For UE(%ld)", 
                                         ueCb->ueCbKey));
#endif
         /* Stop paging */
         ueCb->ueCtxt.suspendPaging = TRUE;
         vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR, 
                                   ueCb->eNodebCb->eNbCfg.t3412Tmr);
         break;
      }
      case VB_TMR_EMM_IMPLI_DTCH_TMR:
      {
#ifdef ALIGN_64BIT
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Implicit Detach Timer Expired For UE(%d)", 
                                         ueCb->ueCbKey));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Implicit Detach Timer Expired For UE(%ld)", 
                                         ueCb->ueCbKey));
#endif
         vbMmeEmmMaxTmrExpInd(ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
         break;
      }
      case VB_TMR_UE_CTX_DEL:
      {
         ret = vbMmeUtlDelUeCb(ueCb);
         if (ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting of UE CB after"
                              "expiry of UE Ctx Delete Timer, SUCCESS"));
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Deleting of UE CB after"
                              "expiry of UE Ctx Delete Timer"));
         }

         break;
      }
      case VB_TMR_INT_MGMT:
      {
         sztLclErrInd.causeType = ueCb->errIndCauseType;
         vbMmeS1apErrInd(ueCb, &sztLclErrInd);
         break;
      }
      case VB_TMR_EMM_IRAT_UE_CTXT_REL:
      {
         vbMmeSndUeCtxRlsCmdDuringiRATHO(ueCb);
         break;
      }
      default:
      {
         break;
      }
   }
} /* vbMmeEmmHdlTmrEvnt */


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlTAUInDeReg
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlTAUInDeReg(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   CmNasEvnt            *tauRejEvnt = NULLP;
   S16                  ret         = ROK;
   VbMmeUeCb            tmpUeCb; /* Temporary ue cb just to send TAU
                                  * reject to UE */
   SztENB_UE_S1AP_ID    *sztENB_UE_S1AP_ID = NULLP;
                                       
   VB_MME_TRC2(vbMmeEmmHdlTAUInDeReg);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TAU received in DEREGISTERED State"));
   
   if ((NULLP != ueCb) && \
      VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_TAU_FOR_HO_REQD))
   {
      VB_MME_UNSET_MASK(ueCb->bitmask, VB_EMM_TAU_FOR_HO_REQD);
      vbMmeEmmHdlTAUReq(eNbCb, ueCb, evnt, ueCtxt);
   }
   else
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending TAU Reject"));

      cmMemset((U8*)&tmpUeCb, 0, sizeof(VbMmeUeCb));
      tmpUeCb.ecmCb.state = VB_ECM_CONNECTED;
      tmpUeCb.emmCb.ueCb = &tmpUeCb;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building TAU Reject"));
      ret = vbMmeUtlBldTAURej(&tmpUeCb.emmCb, &tauRejEvnt,\
            CM_EMM_UE_ID_NOT_DERIVED);
      if (ROK == ret)
      {
         tmpUeCb.eNodebCb            = eNbCb;
         tmpUeCb.ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;
         tmpUeCb.mMeUeS1apId         = ueCtxt->spConnId;
         tmpUeCb.suConnId            = ueCtxt->suConnId;
         tmpUeCb.spConnId            = ueCtxt->spConnId;

         vbMmeUtlGetS1apIE(&(((S1apPdu*)evnt->pdu)->pdu),\
               Sztid_eNB_UE_S1AP_ID, (TknU8 ** )&sztENB_UE_S1AP_ID);
         /* NULL check has been introduced to avoid klocwork warning */
         if (sztENB_UE_S1AP_ID != NULLP)
         {
            tmpUeCb.eNbUeS1apId = sztENB_UE_S1AP_ID->val;
         }

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending TAU Reject to S1AP "\
                  "Layer"));
         ret = vbMmeLimSndToLower(&tmpUeCb, tauRejEvnt,\
               &tmpUeCb.emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);
         if (ROK != ret)
         {
            /* valgrind Fix */
            CM_FREE_NASEVNT(&evnt); 
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending TAU Reject message Failed "));
            VB_MME_FREE(tmpUeCb.emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*tmpUeCb.emmCb.retrBuf.len*/);
         }
         else
         {
            /* valgrind Fix */
            CM_FREE_NASEVNT(&evnt); 
            VB_MME_FREE(tmpUeCb.emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*tmpUeCb.emmCb.retrBuf.len*/);
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building of TAU Reject"));
      }
      if(evnt != NULLP)
      {
         CM_FREE_NASEVNT(&evnt); 
      }

      /*  The cause SztCauseRadioNwsuccessful_handoverEnum 
       *  is chosen to avoid starting a timer in the 
       *  function vbMmeInitCtxtRelProc. 
       *  */
      vbMmeInitCtxtRelProc(&tmpUeCb, CAUSE_RADIONW/*CAUSE_NAS*/, SztCauseRadioNwsuccessful_handoverEnum
            /*SztCauseNasunspecifiedEnum*/, 0);
   }

   RETVALUE(ROK);

} /* vbMmeEmmHdlTAUInDeReg */


/**************************************************************************//**
 *
 * @brief
 *    Callback from HSS
 *
 * @param pHssUsrData
 *    MME data given to HSS
 *
 * @return 
 *    Void
 *
 *****************************************************************************/
PUBLIC Void vbMmeUpdLocRspCbk
(
Void        *pMmeUsrData
)
{
   VbMmeUeCb            *pUeCb = NULLP;
   VbMmeHssUsrData      *pUsrData = NULLP;
   VbEmmEvnt            emmEvnt = VB_EMM_EVNT_INVALID;
   U8                   state   = VB_EMM_UE_DEREGISTERED;
   CmNasEvnt            *pNasEvnt  = NULLP;

   VB_TRC2(vbMmeUpdLocRspCbk);

   pUsrData = (VbMmeHssUsrData *)pMmeUsrData;
   if (NULLP != pUsrData)
   {
      pNasEvnt = pUsrData->pNasEvnt;
      vbMmeUtlFndUeCbOnEvent(pUsrData->pEnbCb, &pUeCb,\
            pUsrData->ueCtx.suConnId, pUsrData->ueCtx.spConnId,\
            pNasEvnt); 

      /*******************************************************/
      /* If UeCb is present invoke EMM FSM handler to process the message */
      /* Else No UE Cb Found - Assume the state is DEREGISTERED
       * Handle appropriately based on message */
      /*******************************************************/
      if (NULLP != pUeCb)
      {
         state = pUeCb->emmCb.state;
      }

      /*******************************************************/
      /* Get the mapped event type from EMM Message Type */
      /*******************************************************/
      VB_EMM_GET_EVNT(pNasEvnt->m.emmEvnt->msgId, CM_EMM_DIR_UE_TO_NW, emmEvnt);
      if (emmEvnt >= VB_EMM_EVNT_ATTACH_REQ && emmEvnt <= VB_EMM_EVNT_CS_NOTIFY)
      {
         VbEmmIncUeSm[state][emmEvnt](pUsrData->pEnbCb,\
               pUeCb, pNasEvnt, &(pUsrData->ueCtx));
      }
      else
      {
         /* Unexpected event for EMM */
         VB_MME_DBGP(("Invalid Message Id(%d)", pNasEvnt->m.emmEvnt->msgId));
         CM_FREE_NASEVNT(&pNasEvnt);
      }

      VB_MME_FREE(pUsrData, sizeof(VbMmeHssUsrData));
   }

   RETVOID;
}


#ifdef ANSI
PRIVATE S16 vbMmeEmmHdlServReqInDeReg
(
VbMmeEnbCb  *eNbCb,
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt,
VbMmeUeCtxt *ueCtxt
)
#else
PRIVATE S16 vbMmeEmmHdlServReqInDeReg(eNbCb, ueCb, evnt, ueCtxt)
VbMmeEnbCb  *eNbCb;
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
VbMmeUeCtxt *ueCtxt;
#endif
{
   CmNasEvnt            *ServRejEvnt = NULLP;
   S16                  ret         = ROK;
   VbMmeUeCb            tmpUeCb; /* Temporary ue just to send Service
                                  * reject to UE */
   SztENB_UE_S1AP_ID    *sztENB_UE_S1AP_ID = NULLP;
                                       
   VB_MME_TRC2(vbMmeEmmHdlServReqInDeReg);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Service Request received in DEREGISTERED State"));
   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Service Reject"));

   cmMemset((U8*)&tmpUeCb, 0, sizeof(VbMmeUeCb));
   tmpUeCb.ecmCb.state = VB_ECM_CONNECTED;
   tmpUeCb.emmCb.ueCb = &tmpUeCb;

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Service Reject"));
   ret = vbMmeUtlBldServiceReject(&tmpUeCb, &ServRejEvnt,\
            CM_EMM_IMPLICITLY_DETACHED);
   if (ROK == ret)
   {
      tmpUeCb.eNodebCb            = eNbCb;
      tmpUeCb.ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;
      tmpUeCb.mMeUeS1apId         = ueCtxt->spConnId;
      tmpUeCb.suConnId            = ueCtxt->suConnId;
      tmpUeCb.spConnId            = ueCtxt->spConnId;

      vbMmeUtlGetS1apIE(&(((S1apPdu*)evnt->pdu)->pdu),\
            Sztid_eNB_UE_S1AP_ID, (TknU8 ** )&sztENB_UE_S1AP_ID);
      /* NULL check has been introduced to avoid klocwork warning */
      if (sztENB_UE_S1AP_ID != NULLP)
      {
         tmpUeCb.eNbUeS1apId = sztENB_UE_S1AP_ID->val;
      }

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Service Reject to S1AP "\
               "Layer"));
      ret = vbMmeLimSndToLower(&tmpUeCb, ServRejEvnt,\
            &tmpUeCb.emmCb.retrBuf, Sztid_downlinkNASTport, TRUE);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Service Reject message Failed "));
      }
      else
      {
         /* valgrind Fix */
         CM_FREE_NASEVNT(&evnt); 
         VB_MME_FREE(tmpUeCb.emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*tmpUeCb.emmCb.retrBuf.len*/);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building of Service Reject"));
   }

   /*  The cause SztCauseRadioNwsuccessful_handoverEnum 
    *  is chosen to avoid starting a timer in the 
    *  function vbMmeInitCtxtRelProc. 
    *  */
   vbMmeInitCtxtRelProc(&tmpUeCb, CAUSE_RADIONW/*CAUSE_NAS*/, SztCauseRadioNwsuccessful_handoverEnum
            /*SztCauseNasunspecifiedEnum*/, 0);

   RETVALUE(ROK);

} /* vbMmeEmmHdlServReqInDeReg  */


#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/**********************************************************************
  
         End of file:     vb_emm.c@@/main/1 - Mon Jun 28 19:35:34 2010
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sv               1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1  vb001.101    rk              1. Retrive the UE from STMSI based list.
                                      2. FIX FOR PAGING CASE.
                                      3. Added new argument nasMsgPres in 
                                         vbMmeLimSndToLower
/main/1  vb005.101    rk              1. Added support for Network initiated Detach 
                      antriksh        1. Workaround to handle receiving GUTI instead of IMSI
*********************************************************************91*/

