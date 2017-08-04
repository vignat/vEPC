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

     Name:    MME

     Type:    C Include file

     Desc:    This file contains the ESM module source code

     File:    vb_esm.c

     Sid:      vb_esm.c@@/main/1 - Mon Jun 28 19:35:35 2010

     Prg:     aj
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
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "cm_emm_esm.h"

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
#include "vb.x"            /* CNE Application structures     */


/* Local typedefs */


/* Forward references */

typedef S16 (* VbEsmStTbl) ARGS((VbMmeEsmCb  *esmCb, CmNasEvnt   *evnt));

/* function prototypes */
PRIVATE S16 vbEsmHndlOutActDefBearerReq ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutPDNConnectRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutPDNDisConnectRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncActDefBearerAcc  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncActDefBearerRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncPDNConnectReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncPDNDisConnectReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlInvEvnt  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutInvEvnt  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
/* vb001.101: Added functions for dedicated bearer and bearer resource allocation messages */
PRIVATE S16 vbEsmHndlIncActDedBearerRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncDeActBearerAcc  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncModBearerCtxtAcc  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncModBearerCtxtRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutActDedBearerReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncBearResAllocReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncBearResModReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncActDedBearerAcc  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutBearResAllocRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutBearResModRej  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutDeActBearerReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutModBearerCtxtReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbMmeHndlOutMmeEsmStatus ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbMmeHndlInMmeEsmStatus ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlOutEsmInfoReq  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));
PRIVATE S16 vbEsmHndlIncEsmInfoRsp  ARGS((VbMmeEsmCb *esmCb, CmNasEvnt *evnt));

/* ESM State machine for Outgoing messages */
PRIVATE VbEsmStTbl vbEsmOutStTbl[CM_ESM_NMB_CM_ESM_EVNT][CM_ESM_NMB_CM_ESM_ST] =
{
   /* VB_ESM_EVNT_ACTI_DEF_BEAR_REQ Receipt of Activate Def Bearer Req */
   {
      vbEsmHndlOutActDefBearerReq,  /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutActDefBearerReq,  /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DEF_BEAR_ACC Receipt of Activate Def Bearer Accept */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DEF_BEAR_REJ Receipt of Activate Def Bearer Reject */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DED_BEAR_REQ Receipt of Activate Dedicated Bearer Req */
   {
      /* vb001.101: Added dedicated bearer req */
      vbEsmHndlOutActDedBearerReq,  /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DED_BEAR_ACC Receipt of Activate Dedicated Bearer Acc*/
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,           /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DED_BEAR_REJ Receipt of Activate Dedicated Bearer Rej*/
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_MODI_BEAR_CTXT_REQ Receipt of Modify Bearer Context Req */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutModBearerCtxtReq, /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_MODI_BEAR_CTXT_ACC Receipt of Modify Bearer Accept */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_MODI_BEAR_CTXT_REJ Receipt of Modify Bearer Reject */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_DEACT_BEAR_CTXT_REQ Receipt of Deactivate Bearer Request */
   {
      vbEsmHndlOutDeActBearerReq,   /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutDeActBearerReq,   /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_DEACT_BEAR_CTXT_ACC Receipt of Deactivate Bearer Accept */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_EPSBEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_EPSBEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_EPSBEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_EPSBEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_EPSBEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_CONN_REQ Receipt of PDN Connectivity Request */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_CONN_REJ Receipt of PDN Connectivity Reject */
   {
      vbEsmHndlOutPDNConnectRej,    /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_DISCONN_REQ Receipt of PDN Disconnect Request */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_DISCONN_REJ Receipt of Deact Bearer Context Request */
   {
      vbEsmHndlOutPDNDisConnectRej, /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutPDNDisConnectRej, /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_ALLOC_REQ Receipt of Bearer Res Alloc Req */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_ALLOC_REJ Receipt of Bearer Res Alloc Rej */
   {
      vbEsmHndlOutBearResAllocRej,  /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_MODI_REQ Receipt of Bearer Res Mod Request */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_MODI_REJ Receipt of Bearer Resource Modify Req */
   {
      vbEsmHndlOutBearResModRej,    /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ESM_INFO_REQ Receipt of ESM information request */
   {
      vbEsmHndlOutEsmInfoReq,       /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ESM_INFO_RES Receipt of ESM information response */
   {
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlOutInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlOutInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ESM_STATUS Receipt of ESM status */
   {
      vbMmeHndlOutMmeEsmStatus,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbMmeHndlOutMmeEsmStatus,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbMmeHndlOutMmeEsmStatus,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbMmeHndlOutMmeEsmStatus,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbMmeHndlOutMmeEsmStatus,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbMmeHndlOutMmeEsmStatus,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbMmeHndlOutMmeEsmStatus           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   }
};

/* ESM State machine for incoming messages */
PUBLIC VbEsmStTbl vbEsmIncStTbl[CM_ESM_NMB_CM_ESM_EVNT][CM_ESM_NMB_CM_ESM_ST] =
{
   /* VB_ESM_EVNT_ACTI_DEF_BEAR_REQ Receipt of Activate Def Bearer Req */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DEF_BEAR_ACC Receipt of Activate Def Bearer Accept */
   {
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlIncActDefBearerAcc,/* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt            /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DEF_BEAR_REJ Receipt of Activate Def Bearer Reject */
   {
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlIncActDefBearerRej,/* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt            /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DED_BEAR_REQ Receipt of Activate Dedicated Bearer Req */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DED_BEAR_ACC Receipt of Activate Dedicated Bearer Acc */
   {
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACTIVE    */
      /* vb001.101: Added dedicated bearer acc */
      vbEsmHndlIncActDedBearerAcc,/* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt            /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ACTI_DED_BEAR_REJ Receipt of Activate Dedicated Bearer Rej */
   {
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlIncActDedBearerRej,  /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt              /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_MODI_BEAR_CTXT_REQ Receipt of Modify Bearer Context Req */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_MODI_BEAR_CTXT_ACC Receipt of Modify Bearer Accept */
   {
      vbEsmHndlInvEvnt,              /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,              /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,              /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,              /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlIncModBearerCtxtAcc,  /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,              /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt               /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_MODI_BEAR_CTXT_REJ Receipt of Modify Bearer Reject */
   {
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlIncModBearerCtxtRej,/* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt             /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_DEACT_BEAR_CTXT_REQ Receipt of Deactivate Bearer Request */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_DEACT_BEAR_CTXT_ACC Receipt of Deactivate Bearer Accept */
   {
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_EPSBEARER_INACTIVE    */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_EPSBEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_EPSBEARER_ACTIVE      */
      vbEsmHndlIncDeActBearerAcc,  /* VB_ESM_ST_EPSBEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_EPSBEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,            /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt             /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_CONN_REQ Receipt of PDN Connectivity Request */
   {
      vbEsmHndlIncPDNConnectReq,  /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt            /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_CONN_REJ Receipt of PDN Connectivity Reject */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_DISCONN_REQ Receipt of PDN Disconnect Request */
   {
      vbEsmHndlIncPDNDisConnectReq, /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlIncPDNDisConnectReq, /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,             /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt              /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_PDN_DISCONN_REJ Receipt of Deact Bearer Context Request */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_ALLOC_REQ Receipt of Bearer Res Alloc Req */
   {
      /* vb001.101: Added bearer resource alloc req*/
      vbEsmHndlIncBearResAllocReq,/* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,           /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt            /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_ALLOC_REJ Receipt of Bearer Res Alloc Rej */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_MODI_REQ Receipt of Bearer Res Mod Request */
   {
      vbEsmHndlIncBearResModReq, /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_BEAR_RES_MODI_REJ Receipt of Bearer Resource Modify Req */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ESM_INFO_REQ Receipt of ESM information request */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ESM_INFO_RES Receipt of ESM information response */
   {
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbEsmHndlInvEvnt,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbEsmHndlIncEsmInfoRsp,    /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbEsmHndlInvEvnt           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   },

   /* VB_ESM_EVNT_ESM_STATUS Receipt of ESM status */
   {
      vbMmeHndlInMmeEsmStatus,          /* VB_ESM_ST_BEARER_INACTIVE    */
      vbMmeHndlInMmeEsmStatus,          /* VB_ESM_ST_BEARER_ACT_PEND    */
      vbMmeHndlInMmeEsmStatus,          /* VB_ESM_ST_BEARER_ACTIVE      */
      vbMmeHndlInMmeEsmStatus,          /* VB_ESM_ST_BEARER_INACT_PEND  */
      vbMmeHndlInMmeEsmStatus,          /* VB_ESM_ST_BEARER_MOD_PEND  */
      vbMmeHndlInMmeEsmStatus,          /* VB_ESM_ST_PROC_TRANS_PEND    */
      vbMmeHndlInMmeEsmStatus           /* VB_ESM_ST_PROC_TRANS_INACTIVE*/
   }
};

/*
 *
 *       Fun:   vbEsmHdlPiggyBackIncUeEvnt
 *
 *       Desc:  This function will handle piggy backed incoming esm events
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbEsmHdlPiggyBackIncUeEvnt
(
VbMmeUeCb   *ueCb,
CmNasEvnt   *evnt
)
#else
PUBLIC S16 vbEsmHdlPiggyBackIncUeEvnt(ueCb, evnt)
VbMmeUeCb   *ueCb;
CmNasEvnt   *evnt;
#endif
{
   S16           ret = ROK;
   VbMmeEsmCb    *esmCb = NULLP;
   CmEsmMsg      *esmMsg;
   CmEsmMsgIndx  msgIdx;

   VB_MME_TRC2(vbEsmHdlPiggyBackIncUeEvnt)

   if(ueCb == NULLP || evnt == NULLP || evnt->m.esmEvnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid UECb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Incoming Piggy Backed UE Event"));

   esmMsg = evnt->m.esmEvnt;

   /*********************************************************/
   /* Allocate a new ESM CB for PDN Connect Message         */
   /* Else find the ESM CB                                  */
   /*********************************************************/

   if(esmMsg->msgType == CM_ESM_MSG_PDN_CONN_REQ)
   {
      ret = vbMmeUtlCrtEsmCbOnTrnId(ueCb, &esmCb, esmMsg->bearerId,\
            esmMsg->prTxnId);
      if(ROK == ret)
      {
         esmCb->esmPiggyBacked          = TRUE;
         ueCb->emmCb.esmPiggyBacked     = TRUE;
         ueCb->emmCb.piggyAction.evnt   = esmMsg->msgType;
         ueCb->emmCb.piggyAction.bId    = esmMsg->bearerId;
         ueCb->emmCb.piggyAction.tId    = esmMsg->prTxnId;     
         ueCb->emmCb.piggyAction.status = VB_MME_BEAR_STUP_ONGOING;     
      }
   }
   else
   {
      ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, esmMsg->bearerId, esmMsg->prTxnId);
   }


   if(ret != ROK)
   {
      /* Failure either memory loss or insertion failed */
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Cb Is Not Found"));
      RETVALUE(RFAILED);
   }


   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
   CM_ESM_MSGIDXMAP(esmMsg->msgType, msgIdx);

   if(msgIdx <= CM_ESM_EVNT_ESM_STATUS)
   {
      ret = vbEsmIncStTbl[msgIdx][esmCb->state](esmCb, evnt);
      if (ret != ROK)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Fsm Failed"));
         RETVALUE(RFAILED);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message"));
      /*handle the Error */
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /*vbEsmHdlPiggyBackUeEvnt  */

/*
 *
 *       Fun:   vbEsmHdlIncUeEvnt
 *
 *       Desc:  This function will handle incoming esm events
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbEsmHdlIncUeEvnt
(
VbMmeEnbCb  *eNodebCb,
VbMmeUeCtxt *ueCtxt,
CmNasEvnt   *evnt
)
#else
PUBLIC S16 vbEsmHdlIncUeEvnt(eNodebCb, ueCtxt, evnt)
VbMmeEnbCb  *eNodebCb;
VbMmeUeCtxt *ueCtxt;
CmNasEvnt   *evnt;
#endif
{
   S16 ret = ROK;
   VbMmeEsmCb    *esmCb = NULLP;
   VbMmeUeCb     *ueCb = NULLP;
   CmEsmMsg      *esmMsg;
   CmEsmMsgIndx  msgIdx;
   U32           ueCbKey;

   VB_MME_TRC2(vbEsmHdlIncUeEvnt)

   if((evnt == NULLP) || (evnt->m.esmEvnt == NULLP))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message"));
      RETVALUE(RFAILED);
   }
   if(ueCtxt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid UeCtxt"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Incoming UE ESM Event"));
   esmMsg = evnt->m.esmEvnt;
   /* Find the ueCb from the UE Cxt*/
   /* Retrieve the UE Cb Key */
   VB_MME_GET_UEID(ueCtxt->suConnId, ueCbKey);

   /* Retrieve the UeCb pointer from the eNodeB block */
   vbMmeUtlFndUeCb(eNodebCb, &ueCb, ueCbKey);

   if(ueCb == NULLP)
   {
      /*Log the Error stating the UE Could not be found*/
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "UECb Is Not Found"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   /********************************************************/
   /* Allocate a new ESM CB for UE initiated procecdure    */
   /* Else find the ESM CB                                 */
   /********************************************************/

   if((esmMsg->msgType == CM_ESM_MSG_PDN_CONN_REQ) || 
      (esmMsg->msgType == CM_ESM_MSG_BEAR_RES_ALLOC_REQ) ||
      (esmMsg->msgType == CM_ESM_MSG_PDN_DISCONN_REQ) ||
      (esmMsg->msgType == CM_ESM_MSG_BEAR_RES_MOD_REQ)) 
   {
      ret = vbMmeUtlCrtEsmCbOnTrnId(ueCb, &esmCb, esmMsg->bearerId, esmMsg->prTxnId);
   }
   else
   {
      if(esmMsg->msgType == CM_ESM_MSG_ESM_INFO_RSP)
      {
         ret = vbMmeGetFrmTrnList(ueCb, &esmCb, esmMsg->prTxnId);
      }
      else
      {
         ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, esmMsg->bearerId, esmMsg->prTxnId);
      }
   }

   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Cb Is Not Found"));
      /* Failure either memory loss or insertion failed */
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }


   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
   CM_ESM_MSGIDXMAP(esmMsg->msgType, msgIdx);
   if(msgIdx <= CM_ESM_EVNT_ESM_STATUS)
   {
      ret = vbEsmIncStTbl[msgIdx][esmCb->state](esmCb, evnt);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message"));
      CM_FREE_NASEVNT(&evnt);
      /*handle the Error */
      /* kworks fix */
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /*vbEsmHdlIncUeEvnt*/

/*
 *
 *       Fun:   vbEsmHdlOutUeEvnt
 *
 *       Desc:  This function will handle outgoing esm events
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbEsmHdlOutUeEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbEsmHdlOutUeEvnt(esmCb, evnt)
VbMmeEsmCb   *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16           ret = ROK;
   CmEsmMsg      *esmMsg;
   CmEsmMsgIndx  msgIdx;

   VB_MME_TRC2(vbEsmHdlOutUeEvnt)

   if((evnt == NULLP) || (evnt->m.esmEvnt == NULLP))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message"));
      RETVALUE(RFAILED);
   }
   if(esmCb == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid UeCtxt"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   esmMsg = evnt->m.esmEvnt;

   /*******************************************************/
   /* Handle ESM if present for this UE */
   /*******************************************************/
   CM_ESM_MSGIDXMAP(esmMsg->msgType, msgIdx);
   if(msgIdx <= CM_ESM_EVNT_ESM_STATUS)
   {
      ret = vbEsmOutStTbl[msgIdx][esmCb->state](esmCb, evnt);
      if (ret != ROK)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Fsm Failed"));
         RETVALUE(RFAILED);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message"));
      /*handle the Error */
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /*vbEsmHdlOutUeEvnt*/

/*
 *
 *       Fun:   vbEsmHdlRollBack
 *
 *       Desc:  This function will roll back to previous state
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbEsmHdlRollBack
(
VbMmeUeCb   *ueCb,
VbMmeEsmCb  *esmCb,
U16         msgType
)
#else
PUBLIC S16 vbEsmHdlRollBack(ueCb, esmCb, msgType)
VbMmeUeCb   *ueCb;
VbMmeEsmCb  *esmCb;
U16         msgType;
#endif
{

   VB_MME_TRC2(vbEsmHdlRollBack)

   if(ueCb == NULLP || esmCb == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid UeCb Or EsmCb"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Rolling Back The "
            "Event From(%d) To (%d) Event", esmCb->state, esmCb->prevstate));
   /* Revert the State to the previous State*/
   esmCb->state = esmCb->prevstate;

   RETVALUE(ROK);
} /*vbEsmHdlRollBack*/

/*
 *
 *       Fun:   vbEsmHndlOutActDefBearerReq
 *
 *       Desc:  This function is to handle outgoing active def bearer request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutActDefBearerReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutActDefBearerReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16     ret = ROK;
   VB_MME_TRC2(vbEsmHndlOutActDefBearerReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Activate Default Bearer Request"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active pending */
   esmCb->state = VB_ESM_ST_BEARER_ACT_PEND;
   /* Default EPS bearer type */
   esmCb->bearerType = CM_ESM_EPS_DEF_BEARER;
   /* reset Procedure transaction identity */
   esmCb->tId = 0;

   if(!(evnt->m.esmEvnt->u.actReq.epsQos.pres))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Mandatory IE EpsQos Is Not Present In ESM Msg"));
      RETVALUE(RFAILED);
   }
   /* The apn name sent in the NAS msg is concatenated with mcc and mnc - however the UE alsways sends
    * the APN name in any UL NAS message - so lets not update the apn in the esmCb - the reason is that
    * in NAS deactivate bearer accept we need to compare the APN sent by the UE and mark it not in use.
    */
#if 0
   if(evnt->m.esmEvnt->u.actReq.apn.pres)
   {
      /* update apn */
      cmMemcpy((U8* )&esmCb->apn, (U8* )&evnt->m.esmEvnt->u.actReq.apn,
            sizeof(CmEsmAccessPtName));
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Mandatory IE Apn Is Not Present In ESM Msg"));
      RETVALUE(RFAILED);
   }
#endif
   if( ! evnt->m.esmEvnt->u.actReq.apn.pres)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Mandatory IE Apn Is Not Present In ESM Msg"));
      RETVALUE(RFAILED);
   }
   if(evnt->m.esmEvnt->u.actReq.pktFlowId.pres)
   {
      /* update pkt flow id */
      cmMemcpy((U8* )&esmCb->pfi, (U8* )&evnt->m.esmEvnt->u.actReq.pktFlowId,
            sizeof(CmEsmPktFlowId));
   }

   if(FALSE == esmCb->esmPiggyBacked)
   {
      ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
             Sztid_E_RABSetup ,TRUE );

      if(ret != ROK)
      {
         VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
         RETVALUE(ret);
      }
      /* Start T3485 timer for this UE */
      esmCb->esmTimersCntr[VB_TMR_ESM_3485] = 1;
      vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3485, esmCb->ueCb->eNodebCb->eNbCfg.t3485Tmr);
   }

   RETVALUE(ret);
} /* vbEsmHndlOutActDefBearerReq */

/*
 *
 *       Fun:   vbEsmHndlOutActDedBearerReq
 *
 *       Desc:  This function is to handle outgoing active ded bearer request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutActDedBearerReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutActDedBearerReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16      ret ;

   VB_MME_TRC2(vbEsmHndlOutActDedBearerReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, " Sending Activate Dedicated Bearer Request"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active pending */
   esmCb->state = VB_ESM_ST_BEARER_ACT_PEND;
   /* Default EPS bearer type */
   esmCb->bearerType = CM_ESM_EPS_DED_BEARER;
   /* If activate ded bearer req is  part of the E-RAB Setup Req */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_E_RABSetup ,TRUE );

   if(ret != ROK)
   {
      VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
      RETVALUE(ret);
   }
   /* Start T3485 timer for this UE */
   esmCb->esmTimersCntr[VB_TMR_ESM_3485] = 1;
   vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3485, esmCb->ueCb->eNodebCb->eNbCfg.t3485Tmr);

   RETVALUE(ROK);
} /* vbEsmHndlOutActDedBearerReq */

/*
 *
 *       Fun:   vbEsmHndlOutModBearerCtxtReq
 *
 *       Desc:  This function is to handle outgoing Mod Bearer Ctxt Request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutModBearerCtxtReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutModBearerCtxtReq(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16      ret ;
   VB_MME_TRC2(vbEsmHndlOutModBearerCtxtReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Modify Bearer Ctxt Request"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active pending */
   esmCb->state = VB_ESM_ST_BEARER_MOD_PEND;

   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_E_RABMdfy, TRUE);

   if(ret != ROK)
   {
      VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
      RETVALUE(ret);
   }

   /* Start T3486 timer for this UE */
   esmCb->esmTimersCntr[VB_TMR_ESM_3486] = 1;
   vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3486, esmCb->ueCb->eNodebCb->eNbCfg.t3486Tmr);
   RETVALUE(ROK);
} /* vbEsmHndlOutModBearerCtxtReq */

/*
 *
 *       Fun:   vbEsmHndlOutDeActBearerReq
 *
 *       Desc:  This function is to handle outgoing Deactive ded bearer request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutDeActBearerReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutDeActBearerReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16      ret ;

   VB_MME_TRC2(vbEsmHndlOutDeActBearerReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, " Sending DeActivate Dedicated Bearer Request"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active pending */
   esmCb->state = VB_ESM_ST_BEARER_INACT_PEND;
   /* Send this NAS PDU(DeActivate Dedicated Bearer Req) msg inside 
    E-RAB Release Command */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_E_RABRls, TRUE);

   if(ret != ROK)
   {
      VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
      RETVALUE(ret);
   }

   /* Start T3495 timer for this UE */
   esmCb->esmTimersCntr[VB_TMR_ESM_3495] = 1;
   vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3495, esmCb->ueCb->eNodebCb->eNbCfg.t3495Tmr);
   RETVALUE(ROK);
} /* vbEsmHndlOutDeActBearerReq */

/*
 *
 *       Fun:   vbEsmHndlOutEsmInfoReq
 *
 *       Desc:  This function is to handle outgoing Esm Info Request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutEsmInfoReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutEsmInfoReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16      ret ;
   VB_MME_TRC2(vbEsmHndlOutEsmInfoReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, " Sending ESM Info Request"));

   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;

   /* Update ESM state to active pending */
   esmCb->state = VB_ESM_ST_PROC_TRANS_PEND;

   /* Send to lower layer */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_downlinkNASTport, TRUE);

   if(ret != ROK)
   {
      VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
      RETVALUE(ret);
   }

   /* Start T3489 timer for this UE */
   esmCb->esmTimersCntr[VB_TMR_ESM_3489] = 1;
   vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3489, esmCb->ueCb->eNodebCb->eNbCfg.t3489Tmr);

   RETVALUE(ROK);

} /* vbEsmHndlOutEsmInfoReq */
/*
 *
 *       Fun:   vbEsmHndlOutBearResAllocRej
 *
 *       Desc:  This function is to handle outgoing BearResAllocRej 
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutBearResAllocRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutBearResAllocRej(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16      ret ;
   VB_MME_TRC2(vbEsmHndlOutBearResAllocRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Bearer Resource Allocation Reject"));
   /* Sent BearResAllocRej nas pdu inside S1AP DL NAS Transport Msg */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_downlinkNASTport ,TRUE );

   RETVALUE(ret);
} /* vbEsmHndlOutBearResAllocRej */


/*
 *
 *       Fun:   vbEsmHndlOutBearResModRej
 *
 *       Desc:  This function is to handle outgoing BearResModRej 
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutBearResModRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutBearResModRej(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16      ret ;
   VB_MME_TRC2(vbEsmHndlOutBearResModRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Bearer Resource Modification Reject"));

   /* Sent BearResModRej nas pdu inside S1AP DL NAS Transport Msg */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_downlinkNASTport ,TRUE );
   RETVALUE(ret);
}


/*
 *
 *       Fun:   vbEsmHndlOutPDNConnectRej
 *
 *       Desc:  This function is to handle outgoing PDN conn reject
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutPDNConnectRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutPDNConnectRej (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16   ret     = ROK; 
   VB_MME_TRC2(vbEsmHndlOutPDNConnectRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending PDN Connectivity Reject"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to inactive */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

   if(FALSE == esmCb->esmPiggyBacked)
   {
      /* Sent BearResModRej nas pdu inside S1AP DL NAS Transport Msg */
      ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
               Sztid_downlinkNASTport ,TRUE );
   }

   RETVALUE(ret);
} /* vbEsmHndlOutPDNConnectRej */

/*
 *
 *       Fun:   vbEsmHndlOutPDNDisConnectRej
 *
 *       Desc:  This function is to handle outgoing PDN Disconn reject
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutPDNDisConnectRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutPDNDisConnectRej (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16   ret     = ROK; 
   VB_MME_TRC2(vbEsmHndlOutPDNDisConnectRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending PDN Dis Connectivity Reject"));

   /* Sent BearResModRej nas pdu inside S1AP DL NAS Transport Msg */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &esmCb->retrBuf, \
              Sztid_downlinkNASTport ,TRUE );
   
   RETVALUE(ret);
} /* vbEsmHndlOutPDNDisConnectRej */
/*
 *
 *       Fun:   vbEsmHndlIncActDefBearerAcc
 *
 *       Desc:  This function is to handle incoming active def bearer accept
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncActDefBearerAcc
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncActDefBearerAcc (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncActDefBearerAcc)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Receiving Activate Default Bearer Accept"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active */
   esmCb->state = VB_ESM_ST_BEARER_ACTIVE;

   /* If activate def bearer acc is not part of attach complete */
   if(!esmCb->esmPiggyBacked)
   {
      vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
      VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
      /* Stand Alone message send it to Application */
      RETVALUE(vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType));
   }

   RETVALUE(ROK);
} /* vbEsmHndlIncActDefBearerAcc */

/*
 *
 *       Fun:   vbEsmHndlIncActDedBearerAcc
 *
 *       Desc:  This function is to handle incoming active ded bearer accept
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncActDedBearerAcc
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncActDedBearerAcc (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{

   VB_MME_TRC2(vbEsmHndlIncActDedBearerAcc)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Act Ded Bearer Accept"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active */
   esmCb->state = VB_ESM_ST_BEARER_ACTIVE;
   /* 1. stop the timer T3485 */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
   VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
   /* Check whether it is a poggyback Message or Stand Alone Message 
    * based on that send the message to the Application */
   /* Stand Alone message send it to Application */
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType );

   RETVALUE(ROK);
} /* vbEsmHndlIncActDedBearerAcc */

/*
 *
 *       Fun:   vbEsmHndlIncActDedBearerRej
 *
 *       Desc:  This function is to handle incoming active ded bearer reject
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncActDedBearerRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncActDedBearerRej (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncActDedBearerRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Act Ded Bearer Rej"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;
   /* 1. stop the timer T3485 */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
   VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
   /* Check whether it is a poggyback Message or Stand Alone Message 
    * based on that send the message to the Application */
   /* Stand Alone message send it to Application */
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType );

   RETVALUE(ROK);
} /* vbEsmHndlIncActDedBearerRej */

/*
 *
 *       Fun:   vbEsmHndlIncDeActBearerAcc
 *
 *       Desc:  This function is to handle incoming deactive ded bearer accept
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncDeActBearerAcc
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncDeActBearerAcc(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncDeActBearerAcc)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received DeAct Ded Bearer Accept"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;
   /* 1. stop the timer T3495 */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3495);
   VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
   /* Check whether it is a poggyback Message or Stand Alone Message 
    * based on that send the message to the Application */
   /* Stand Alone message send it to Application */
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType );

   RETVALUE(ROK);
} /* vbEsmHndlIncDeActBearerAcc */

/*
 *
 *       Fun:   vbEsmHndlIncModBearerCtxtAcc
 *
 *       Desc:  This function is to handle incoming Modify bearer Ctxt accept
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncModBearerCtxtAcc
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncModBearerCtxtAcc(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncModBearerCtxtAcc)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Mod Bearer Ctxt Accept"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to active */
   esmCb->state = VB_ESM_ST_BEARER_ACTIVE;

   /* 1. stop the timer T3486 */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3486);
   VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
   /* Check whether it is a poggyback Message or Stand Alone Message 
    * based on that send the message to the Application */
   /* Stand Alone message send it to Application */
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType);

   RETVALUE(ROK);
} /* vbEsmHndlIncModBearerCtxtAcc */

/*
 *
 *       Fun:   vbEsmHndlIncModBearerCtxtRej
 *
 *       Desc:  This function is to handle incoming Modify Bearer Ctxt reject
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncModBearerCtxtRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncModBearerCtxtRej(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncModBearerCtxtRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Modify Bearer Ctxt Reject"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to inactive */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

   /* 1. stop the timer T3486 */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3486);
   VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType);

   RETVALUE(ROK);
} /* vbEsmHndlIncModBearerCtxtRej */

/*
 *
 *       Fun:   vbEsmHndlIncActDefBearerRej
 *
 *       Desc:  This function is to handle incoming active def bearer reject
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncActDefBearerRej
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncActDefBearerRej (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncActDefBearerRej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Activate Default Dearer Reject"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to inactive */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

   /* If activate def bearer acc is not part of attach procudure */
   if(!esmCb->esmPiggyBacked)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Stopping T3485 Timer"));
      vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
      VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /* esmCb->retrBuf.len*/);
      vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType);
   }

   RETVALUE(ROK);
} /* vbEsmHndlIncActDefBearerRej */

/*
 *
 *       Fun:   vbEsmHndlIncPDNConnectReq
 *
 *       Desc:  This function is to handle incoming PDN conn request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncPDNConnectReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncPDNConnectReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   U8       i = 0;
   S16      ret = RFAILED;
   U8       cause = VB_ESM_NETWORK_FAILURE;

   VB_MME_TRC2(vbEsmHndlIncPDNConnectReq)


   VB_MME_DBG_INFO((VB_MME_PRNTBUF, " Received PDN Connectivity Request"));
   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;
   /* Update ESM state to inactive */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

   /* copy Access point name */
   if(evnt->m.esmEvnt->u.conReq.apn.pres)
   {
      for(i = 0; i < VB_HSS_MAX_PDN_SUB_CTXT; i++)
      {  
         /* 
          * indexing apn[1], becasue apn[0] is the length of the apn name.
          */
         if((ret = (cmMemcmp((U8 *)&evnt->m.esmEvnt->u.conReq.apn.apn[1], \
                     (U8 *)&esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].apn, \
                     evnt->m.esmEvnt->u.conReq.apn.len))) == 0)
         {
            if(esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].subCntxtinUse == FALSE)
            {
               /*KWORKS Fix*/
#if 0
               cmMemcpy((U8* )&esmCb->apn.apn, (U8* )&esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].apn,
            sizeof(CmEsmAccessPtName));
#else
               cmMemcpy((U8* )&esmCb->apn.apn, (U8* )&esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].apn,CM_ESM_MAX_LEN_ACCESS_PTNAME);
#endif

               /* Mark this APN as Used */
               esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].subCntxtinUse = TRUE;

               esmCb->apn.pres = TRUE;
               esmCb->apn.len = evnt->m.esmEvnt->u.conReq.apn.len;
               break;
            }
            /*
             * if UE is requesting for the already assigned APN,
             * than sending the PDN connectivity reject with the 
             * cause VB_ESM_MULT_PDN_CONN_FOR_APN.
             */
            else
            {
               cause = VB_ESM_MULT_PDN_CONN_FOR_APN;
               ret = vbMmeSndPdnConnRej(esmCb, cause);
               /* Clean up esm cb */
               VB_MME_END_ESM_PROC(esmCb);
               /* Remove esmCb from transaction list */
               vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
               /* Release esmCb memory */
               VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
               RETVALUE(ret);
            }
         }
      }

      /* 
       * if APN does not match with the APN List in the DB
       * sending pdn connectivity reject.
       */

      if(esmCb->apn.pres == FALSE)
      {
         cause = VB_ESM_UNKNOWN_OR_MISSING_APN;
         ret = vbMmeSndPdnConnRej(esmCb, cause);
         /* Clean up esm cb */
         VB_MME_END_ESM_PROC(esmCb);
         /* Remove esmCb from transaction list */
         vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
         /* Release esmCb memory */
         VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
         RETVALUE(ret);
      }
   }

   /* update pdn type */
   /* Spec: 24.301 8.2 version: 
   3 bits -> 1 0 0 -unused; 
   shall be interpreted as "IPv6" if received by the network */
   if(evnt->m.esmEvnt->u.conReq.pdnType.val == 4) 
   {
      esmCb->pdnType = CM_ESM_PDN_IPV6;
   }
   else
   {
      esmCb->pdnType = evnt->m.esmEvnt->u.conReq.pdnType.val;
   }

   if(evnt->m.esmEvnt->u.conReq.infoTransFlg.pres)
   {
      esmCb->infoTransFlg = evnt->m.esmEvnt->u.conReq.infoTransFlg.val;
   }
   else
   {
      esmCb->infoTransFlg = FALSE;
   }

   esmCb->bearerType = CM_ESM_EPS_DEF_BEARER;

   /* esmCb->esmPiggyBacked will be set only if it piggybacked in the EMM
    * message. For the stand alone PDN conn Req message esmCb->esmPiggyBacked
    * parameter will be set to FALSE */
   if(TRUE != esmCb->esmPiggyBacked)
   {
      RETVALUE(vbMmeRcvEsmMsg(esmCb, evnt, CM_ESM_MSG_PDN_CONN_REQ));
   }

   RETVALUE(ROK);
} /* vbEsmHndlIncPDNConnectReq */

/*
 *
 *       Fun:   vbEsmHndlIncPDNDisConnectReq
 *
 *       Desc:  This function is to handle incoming PDN Disconn request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncPDNDisConnectReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncPDNDisConnectReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncPDNDisConnectReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, " Received PDN Disconnectivity Request"));

   esmCb->linkBearId = evnt->m.esmEvnt->u.disConReq.bearerId;

   RETVALUE(vbMmeRcvEsmMsg(esmCb, evnt, CM_ESM_MSG_PDN_DISCONN_REQ));

   RETVALUE(ROK);
} /* vbEsmHndlIncPDNDisConnectReq */

/* vb001.101: Added function */
/*
 *
 *       Fun:   vbEsmHndlIncBearResAllocReq
 *
 *       Desc:  This function is to handle incoming  Bearer Res Alloc request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncBearResAllocReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncBearResAllocReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncBearResAllocReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Bearer Resource Allocation Request"));
   /* Update the Linked BearerID of the Default Bearer */
   esmCb->linkBearId = evnt->m.esmEvnt->u.bearAllocReq.bearerId;

   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;

   /* Update ESM state to inactive */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

   /* copy ESM TFT Info */
   if(evnt->m.esmEvnt->u.bearAllocReq.tft.pres)
   {
      cmMemcpy((U8* )&esmCb->tft,
            (U8* )&evnt->m.esmEvnt->u.bearAllocReq.tft,
            sizeof(CmEsmTft));
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received TFT in Bearer Res Allocation Request"));
   }

   /* copy epsQos */
   if(evnt->m.esmEvnt->u.bearAllocReq.epsQos.pres)
   {
      cmMemcpy((U8* )&esmCb->epsQos, (U8* )&evnt->m.esmEvnt->u.bearAllocReq.epsQos,
            sizeof(CmEsmEpsQos));
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received QOS As Part Of" 
                                       "The Bearer Res Alloc Request"));
   }
   /* Check whether it is a poggyback Message or Stand Alone Message 
    * based on that send the message to the Application */
   /* Stand Alone message send it to Application */
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType );

   RETVALUE(ROK);
} /* vbEsmHndlIncBearResAllocReq */

/*
 *
 *       Fun:   vbEsmHndlIncBearResModReq
 *
 *       Desc:  This function is to handle incoming Bearer Res Mod request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncBearResModReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncBearResModReq(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncBearResModReq)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Bearer Resource Modification Request"));

   /* fill the received EPS bearer Id inside esmCb */
   esmCb->bId = evnt->m.esmEvnt->bearerId;
   
   /* Update the Linked BearerID of the Default Bearer */
   esmCb->linkBearId = evnt->m.esmEvnt->u.bearModReq.bearerId;

   /* Update ESM prev state */
   esmCb->prevstate = esmCb->state;

   /* Update ESM state to inactive */
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

   /* copy ESM TFT Info */
   if(evnt->m.esmEvnt->u.bearModReq.tft.pres)
   {
      cmMemcpy((U8* )&esmCb->tft,
            (U8* )&evnt->m.esmEvnt->u.bearModReq.tft,
            sizeof(CmEsmTft));
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received TFT in Bearer Res Modification Request"));
   }

   /* copy epsQos */
   if(evnt->m.esmEvnt->u.bearModReq.epsQos.pres)
   {
      cmMemcpy((U8* )&esmCb->epsQos, (U8* )&evnt->m.esmEvnt->u.bearModReq.epsQos,
            sizeof(CmEsmEpsQos));
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received QOS As Part Of" 
                                       "The Bearer Res Modification Request"));
   }

   /* Check whether it is a poggyback Message or Stand Alone Message 
    * based on that send the message to the Application */
   /* Stand Alone message send it to Application */
   vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType);

   RETVALUE(ROK);
} /* vbEsmHndlIncBearResAllocReq */

#ifdef ANSI
PRIVATE S16 vbMmeHndlOutMmeEsmStatus
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbMmeHndlOutMmeEsmStatus(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   S16         ret  = ROK;
   SztNAS_PDU  pdu;

   VB_MME_TRC2(vbMmeHndlOutMmeEsmStatus)

   cmMemset((U8*)&pdu, 0, sizeof(SztNAS_PDU));

   /* Send the message to network */
   ret = vbMmeLimSndToLower(esmCb->ueCb, evnt, &pdu, Sztid_downlinkNASTport, TRUE);

   if(ROK == ret)
   {
      VB_MME_FREE(pdu.val, pdu.len);
   }

   RETVALUE(ret);

} /* vbMmeHndlOutMmeEsmStatus */


#ifdef ANSI
PRIVATE S16 vbMmeHndlInMmeEsmStatus
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbMmeHndlInMmeEsmStatus(esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbMmeHndlInMmeEsmStatus)
   if(NULLP != evnt)
   {
      CM_FREE_NASEVNT(&evnt);
   }
   if(NULLP != esmCb)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,"ESM Status Message Received For Bearer (%d)", esmCb->bId));
   }
   else
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "ESM Status Message Received"));
   }
   RETVALUE(ROK);

} /* vbMmeHndlInMmeEsmStatus */

/*
 *
 *       Fun:   vbEsmHndlIncEsmInfoRsp
 *
 *       Desc:  This function is to handle incoming esm info response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlIncEsmInfoRsp
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlIncEsmInfoRsp (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlIncEsmInfoRsp)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Receiving Esm Info Response"));

   /* Update ESM state to previous state */
   esmCb->state = esmCb->prevstate;

   /* Update ESM prev state */
   esmCb->prevstate = VB_ESM_ST_PROC_TRANS_PEND;

   /* Stop T3489 timer */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3489);
   VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);

   /* Stand Alone message send it to Application */
   RETVALUE(vbMmeRcvEsmMsg(esmCb, evnt, evnt->m.esmEvnt->msgType));

} /* vbEsmHndlIncEsmInfoRsp */

/*
 *
 *       Fun:   vbEsmHndlInvEvnt
 *
 *       Desc:  This function is for error events
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlInvEvnt
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlInvEvnt (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlInvEvnt)

   if(NULLP == evnt)
   {
      RETVALUE(RFAILED);
   }

   if(NULLP == esmCb)
   {
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   /* Invalid message in the state, returing RFAILED */
   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message(%d) In The State(%d)",
            evnt->m.esmEvnt->msgType, esmCb->state));
   
   vbMmeSndEsmStatus(esmCb->ueCb, esmCb, evnt->m.esmEvnt->prTxnId, 
                     evnt->m.esmEvnt->bearerId, 
                     VB_ESM_MSG_INCOMP_WITH_PROTO_STATE);

   CM_FREE_NASEVNT(&evnt);

   RETVALUE(RFAILED);
} /* vbEsmHndlInvEvnt */

/*
 *
 *       Fun:   vbEsmHndlOutInvEvnt
 *
 *       Desc:  This function is for error events
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PRIVATE S16 vbEsmHndlOutInvEvnt
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   *evnt
)
#else
PRIVATE S16 vbEsmHndlOutInvEvnt (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt   *evnt;
#endif
{
   VB_MME_TRC2(vbEsmHndlOutInvEvnt)

   if(NULLP == evnt)
   {
      RETVALUE(RFAILED);
   }

   if(NULLP == esmCb)
   {
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(RFAILED);
   }

   /* Invalid message in the state, returing RFAILED */
   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message(%d) In The State(%d)",
            evnt->m.esmEvnt->msgType, esmCb->state));
   
   CM_FREE_NASEVNT(&evnt);

   RETVALUE(RFAILED);
}

/*
 *
 *       Fun:   vbMmeEsmHdlTmrEvnt
 *
 *       Desc:  This function is for expiration of ESM3495 timer
 *
 *       Ret:   void
 *
 *       Notes: none
 *
 *       File:  vb_esm.c
 *
 */
#ifdef ANSI
PUBLIC Void vbMmeEsmHdlTmrEvnt
(
VbMmeEsmCb  *esmCb,
S16         tmrEvnt
)
#else
PUBLIC Void vbMmeEsmHdlTmrEvnt (esmCb, tmrEvnt)
VbMmeEsmCb  *esmCb;
S16         tmrEvnt;
#endif
{
   S16           ret = ROK;

   VB_MME_TRC2(vbMmeEsmHdlTmrEvnt)
   if(esmCb == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid EsmCb"));
      RETVOID;
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling ESM Timers:Timer Event(%d) Expired", tmrEvnt));
   switch(tmrEvnt)
   {
      case VB_TMR_ESM_3485:
      {
         if (esmCb->esmTimersCntr[VB_TMR_ESM_3485] < VB_TMR_ESM_MAX_ACT_DED_BEAR_CTXT_REQ)
         {
            /* Send the stored buffer and increment the attempts counter */
            esmCb->esmTimersCntr[VB_TMR_ESM_3485]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3485, esmCb->ueCb->eNodebCb->eNbCfg.t3485Tmr);

            ret = vbMmeLimSndEsmStoredBuf(esmCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Act Def or Ded Bearer Request"));
               vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
               VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
               vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3485);
               RETVOID;
            }
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                            "of Act Def or Ded Bearer Request Reached"));
            
            /* call handler after max retransmission reached */
            vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3485);
         }
         break;
      }
      case VB_TMR_ESM_3486:
      {
         if (esmCb->esmTimersCntr[VB_TMR_ESM_3486] < VB_TMR_ESM_MAX_MOD_BEAR_CTXT_REQ)
         {
            /* Send the stored buffer and increment the attempts counter */
            esmCb->esmTimersCntr[VB_TMR_ESM_3486]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3486, esmCb->ueCb->eNodebCb->eNbCfg.t3486Tmr);

            ret = vbMmeLimSndEsmStoredBuf(esmCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored  ModEpsBearerCtxtReq"));
               vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3486);
               VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
               vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3486);
               RETVOID;
            }
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                            "of ModEpsBearerCtxtReq Reached"));
            
            /* call handler after max retransmission reached */
            vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3486);
         }
         break;
      }
      case VB_TMR_ESM_3489:
      {
         if (esmCb->esmTimersCntr[VB_TMR_ESM_3489] < VB_TMR_ESM_MAX_ESM_INFO_REQ)
         {
            /* Send the stored buffer and increment the attempts counter */
            esmCb->esmTimersCntr[VB_TMR_ESM_3489]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3489, esmCb->ueCb->eNodebCb->eNbCfg.t3489Tmr);

            ret = vbMmeLimSndEsmStoredBuf(esmCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored Esm Info Request"));
               vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3489);
               VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
               vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3489);
               RETVOID;
            }
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                            "of Esm Info Req Reached"));
            
            /* call handler after max retransmission reached */
            vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3489);
         }
         break;
      }
      case VB_TMR_ESM_3495:
      {
         if (esmCb->esmTimersCntr[VB_TMR_ESM_3495] < VB_TMR_ESM_MAX_DEACT_BEAR_CTXT_REQ)
         {
            /* Send the stored buffer and increment the attempts counter */
            esmCb->esmTimersCntr[VB_TMR_ESM_3495]++;

            /* Restart the timer as we are sending the msg again */
            vbMmeStartTmr((PTR) esmCb, VB_TMR_ESM_3495, esmCb->ueCb->eNodebCb->eNbCfg.t3495Tmr);

            ret = vbMmeLimSndEsmStoredBuf(esmCb, Sztid_downlinkNASTport);

            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send Stored  DeActEpsBearerCtxtReq"));
               vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3495);
               VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);
               vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3495);
               RETVOID;
            }
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Maximum Number Of Retransmission"
                                            "of DeActEpsBearerCtxtReq Reached"));
            
            /* call handler after max retransmission reached */
            vbMmeEsmMaxTmrExpInd(esmCb, VB_TMR_ESM_3495);
         }
         break;
      }
      default:
      {
         /* Invalid Timer */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Timer"));
         break;
      }
   }
} /* vbMmeEsmHdlTmrEvnt */


/********************************************************************30**

         End of file:     vb_esm.c@@/main/1 - Mon Jun 28 19:35:35 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rk              1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1  vb001.101    rk              1. Added functions for dedicated bearer and 
                                         bearer resource allocation messages. 
*********************************************************************91*/
