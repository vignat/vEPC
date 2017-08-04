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
  
        Name:     LTE CNE Layer management interface
    
        Type:     C include file
  
        Desc:     This file contains defines required by LTE CNE layer 
                  management interface
 
        File:     lvb.c

        Sid:      lvb.c@@/main/nodal_main/2 - Fri Jan  3 14:56:05 2014
  
        Prg:      rk
  
*********************************************************************21*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      
 *     
 */

/* header include files -- defines (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common pasn define              */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "lvb.h"
#include "vb_hss.h"        /* CNE Application defines        */
#include "vb.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* common NAS library              */
#include "cm_esm.h"        /* common NAS library              */

/* header/extern include files (.x) */
/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common pasn library             */
#include "sct.x"           /* S1AP Upper Interface */
/*lvb_c_001.main_4: s1ap inclusive order has chnged */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */
#include "egt.x"           /* EGTP Upper Interface */
#include "lvb.x"
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#if (defined(LCLVB) || defined(LWLCLVB))

/* Function prototypes */
PRIVATE S16 cmPkVbMngmt ARGS((VbMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmUnpkVbMngmt ARGS((VbMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmPkVbEnbCbCfg ARGS((VbEnbCbCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbEnbCbCfg ARGS((VbEnbCbCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkVbNwId ARGS((VbNwId *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbNwId ARGS((VbNwId *param, Buffer *mBuf));
PRIVATE S16 cmPkVbGUMMEI ARGS((VbGUMMEI *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbGUMMEI ARGS((VbGUMMEI *param, Buffer *mBuf));
PRIVATE S16 cmPkVbSrvdGLst ARGS((VbSrvdGLst *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbSrvdGLst ARGS((VbSrvdGLst *param,Buffer *mBuf));
PRIVATE S16 cmPkVbPlmnId ARGS((VbPlmnId *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbPlmnId ARGS((VbPlmnId *param, Buffer *mBuf));
PRIVATE S16 cmPkVbPlmnLst ARGS((VbPlmnLst *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbPlmnLst ARGS((VbPlmnLst *param, Buffer *mBuf));
PRIVATE S16 cmPkMmeName ARGS((VbMmeName  *mmeName, Buffer *mBuf));
PRIVATE S16 cmUnpkMmeName ARGS((VbMmeName  *mmeName, Buffer *mBuf));
PRIVATE S16 cmPkVbTai ARGS((VbTai *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbTai ARGS((VbTai *param, Buffer *mBuf));
PRIVATE S16 cmPkVbTaiLst ARGS((VbTaiLst *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbTaiLst ARGS((VbTaiLst *param, Buffer *mBuf));
PRIVATE S16 cmPkVbMmeGenCfg ARGS((VbMmeGenCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbMmeGenCfg ARGS((VbMmeGenCfg *param, Buffer *mBuf));
/*warning fix - this function is not used anywhere */
/*PRIVATE S16 cmPkCmInetNetAddr ARGS((CmInetNetAddr *param, Buffer *mBuf));*/
/*PRIVATE S16 cmUnpkCmInetNetAddr ARGS((CmInetNetAddr *param, Buffer *mBuf));*/
PRIVATE S16 cmPkVbMmeLSapCfg ARGS((VbMmeLSapCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbMmeLSapCfg ARGS((VbMmeLSapCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkVbCfgMme ARGS((VbCfg *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkVbCfg ARGS((VbCfg *param, Elmnt elmnt,  Buffer *mBuf));
PRIVATE S16 cmUnpkVbCfgMme ARGS((VbCfg *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkVbCfg ARGS((VbCfg *param, Elmnt elmnt, Buffer *mBuf));
#ifdef DEBUGP
PRIVATE S16 cmPkVbDbgCntrl ARGS((VbDbgCntrl *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbDbgCntrl ARGS((VbDbgCntrl *param, Buffer *mBuf));
#endif /* DEBUGP */
PRIVATE S16 cmPkVbSapCntrl ARGS((VbSapCntrl *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbSapCntrl ARGS((VbSapCntrl *param, Buffer *mBuf));
PRIVATE S16 cmPkVbMmeCntrl ARGS((VbMmeCntrl *param, Elmnt  elmnt,
         Buffer *mBuf));
PRIVATE S16 cmUnpkVbMmeCntrl ARGS((VbMmeCntrl *param, Elmnt  elmnt,
         Buffer *mBuf));
PRIVATE S16 cmPkVbCntrl ARGS((VbCntrl *param, Elmnt   elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkVbCntrl ARGS((VbCntrl *param, Elmnt   elmnt, Buffer *mBuf));
PRIVATE S16 cmPkVbUstaDgn ARGS((VbUstaDgn *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbUstaDgn ARGS((VbUstaDgn *param, Buffer *mBuf));
PRIVATE S16 cmPkVbMngmt ARGS((VbMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmUnpkVbMngmt ARGS((VbMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmPkVbMmeCodeLst ARGS((VbMmeCodeLst *param, Buffer *mBuf));
PRIVATE S16 cmPkVbGrpIdLst ARGS((VbGrpIdLst *param, Buffer *mBuf));
PRIVATE S16 cmUnpkVbMmeCodeLst ARGS((VbMmeCodeLst *param,Buffer *mBuf));
PRIVATE S16 cmUnpkVbGrpIdLst ARGS((VbGrpIdLst *param,Buffer *mBuf));


#ifdef ANSI
PUBLIC S16 cmPkLvbCfgReq
(
Pst     * pst,
VbMngmt * cfg
)
#else
PUBLIC S16 cmPkLvbCfgReq(pst, cfg)
Pst     * pst;
VbMngmt * cfg;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB001, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(cfg, EVTLVBCFGREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB002, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)cfg, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB003, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfg,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB004, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbCfgReq
(
LvbCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbCfgReq(func, pst, mBuf)
LvbCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *cfg;

   TRC3(cmUnpkLvbCfgReq)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfg,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB005, (ErrVal)0, "Unpacking failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfg, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC) 
      if (cmUnpkVbMngmt(cfg, EVTLVBCFGREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB006, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfg));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbCntrlReq
(
Pst     * pst,
VbMngmt * cntrl
)
#else
PUBLIC S16 cmPkLvbCntrlReq(pst, cntrl)
Pst     * pst;
VbMngmt * cntrl;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbCntrlReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB007, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(cntrl, EVTLVBCNTRLREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB008, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)cntrl, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB009, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cntrl,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB010, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBCNTRLREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbCntrlReq
(
LvbCntrlReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbCntrlReq(func, pst, mBuf)
LvbCntrlReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *cntrl;
   
   TRC3(cmUnpkLvbCntrlReq)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cntrl,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB011, (ErrVal)0, "Unpacking failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cntrl, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC) 
      if (cmUnpkVbMngmt(cntrl, EVTLVBCNTRLREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB012, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cntrl));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbStaReq
(
Pst     * pst,
VbMngmt * sta
)
#else
PUBLIC S16 cmPkLvbStaReq(pst, sta)
Pst     * pst;
VbMngmt * sta;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbStaReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB013, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(sta, EVTLVBSTAREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB014, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)sta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB015, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)sta,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB016, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBSTAREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbStaReq
(
LvbStaReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbStaReq(func, pst, mBuf)
LvbStaReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *sta;

   TRC3(cmUnpkLvbStaReq)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&sta,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB017, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&sta, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(sta, EVTLVBSTAREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB018, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, sta));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbStsReq
(
Pst     * pst,
Action action,
VbMngmt * sts
)
#else
PUBLIC S16 cmPkLvbStsReq(pst, action, sts)
Pst     * pst;
Action action;
VbMngmt * sts;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbStsReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB019, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(sts, EVTLVBSTSREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB020, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)sts, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB021, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (cmPkAction(action, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB022, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(VbMngmt));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)sts,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB023, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBSTSREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbStsReq
(
LvbStsReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbStsReq(func, pst, mBuf)
LvbStsReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   Action action;
   VbMngmt *sts;

   TRC3(cmUnpkLvbStsReq)

   if (cmUnpkAction(&action, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB024, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&sts,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB025, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&sts, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(sts, EVTLVBSTSREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB026, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, action, sts));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbCfgCfm
(
Pst     * pst,
VbMngmt * cfm
)
#else
PUBLIC S16 cmPkLvbCfgCfm(pst, cfm)
Pst     * pst;
VbMngmt * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbCfgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB027, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(cfm, EVTLVBCFGCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB028, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB029, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB030, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBCFGCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbCfgCfm
(
LvbCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbCfgCfm(func, pst, mBuf)
LvbCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *cfm;

   TRC3(cmUnpkLvbCfgCfm)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB031, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC) 
      if (cmUnpkVbMngmt(cfm, EVTLVBCFGCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB032, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbStsCfm
(
Pst     * pst,
VbMngmt * cfm
)
#else
PUBLIC S16 cmPkLvbStsCfm(pst, cfm)
Pst     * pst;
VbMngmt * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbStsCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB033, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(cfm, EVTLVBSTSCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB034, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB035, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB036, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBSTSCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbStsCfm
(
LvbStsCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbStsCfm(func, pst, mBuf)
LvbStsCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *cfm;

   TRC3(cmUnpkLvbStsCfm)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB037, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(cfm, EVTLVBSTSCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB038, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbStaCfm
(
Pst      * pst,
VbMngmt  * cfm
)
#else
PUBLIC S16 cmPkLvbStaCfm(pst, cfm)
Pst      * pst;
VbMngmt  * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbStaCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB039, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(cfm, EVTLVBSTACFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB040, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB041, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB042, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBSTACFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbStaCfm
(
LvbStaCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbStaCfm(func, pst, mBuf)
LvbStaCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *cfm;

   TRC3(cmUnpkLvbStaCfm)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB043, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(cfm, EVTLVBSTACFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB044, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbStaInd
(
Pst      * pst,
VbMngmt  * usta
)
#else
PUBLIC S16 cmPkLvbStaInd(pst, usta)
Pst      * pst;
VbMngmt  * usta;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbStaInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB045, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(usta, EVTLVBSTAIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB046, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)usta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB047, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)usta,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB048, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBSTAIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbStaInd
(
LvbStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbStaInd(func, pst, mBuf)
LvbStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *usta;

   TRC3(cmUnpkLvbStaInd)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&usta,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB049, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&usta, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(usta, EVTLVBSTAIND, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB050, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, usta));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbCntrlCfm
(
Pst      * pst,
VbMngmt  * cfm
)
#else
PUBLIC S16 cmPkLvbCntrlCfm(pst, cfm)
Pst      * pst;
VbMngmt  * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLvbCntrlCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB051, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(cfm, EVTLVBCNTRLCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB052, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB053, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB054, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBCNTRLCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbCntrlCfm
(
LvbCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbCntrlCfm(func, pst, mBuf)
LvbCntrlCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *cfm;

   TRC3(cmUnpkLvbCntrlCfm)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB055, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(cfm, EVTLVBCNTRLCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB056, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLvbTrcInd
(
Pst      * pst,
VbMngmt  * trc,
Buffer   * trcBuf
)
#else
PUBLIC S16 cmPkLvbTrcInd(pst, trc, trcBuf)
Pst      * pst;
VbMngmt  * trc;
Buffer   * trcBuf;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;

   TRC3(cmPkLvbTrcInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB057, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      if (SFndLenMsg(trcBuf, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB058, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(VbMngmt));
         SPutMsg(trcBuf);
         RETVALUE(RFAILED);
      }
      if (SCatMsg(mBuf, trcBuf, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB059, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(VbMngmt));
         SPutMsg(trcBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(trcBuf);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)trcBuf, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB060, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(VbMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LC) {
      if (cmPkVbMngmt(trc, EVTLVBTRCIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB061, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(VbMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmPkPtr((PTR)trc, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB062, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(VbMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)trc,
               sizeof(VbMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB063, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLVBTRCIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLvbTrcInd
(
LvbTrcInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLvbTrcInd(func, pst, mBuf)
LvbTrcInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   VbMngmt *trc = NULLP;
   Buffer *trcBuf = NULLP;

   TRC3(cmUnpkLvbTrcInd)

   if (pst->selector != LVB_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&trc,
                  sizeof(VbMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELVB064, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&trc, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LVB_SEL_LC)
      if (cmUnpkVbMngmt(trc, EVTLVBTRCIND, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(VbMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ELVB065, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LVB_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &trcBuf) != ROK)
         RETVALUE(RFAILED);
   }
   else if (pst->selector == LVB_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&trcBuf, mBuf) != ROK) {
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, trc, trcBuf));
}

#ifdef ANSI
PRIVATE S16 cmPkVbEnbCbCfg
(
VbEnbCbCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbEnbCbCfg(param, mBuf)
VbEnbCbCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbEnbCbCfg)

   CMCHKPK(cmPkCmTptAddr, &param->eNbAddr, mBuf);
   CMCHKPK(SPkU16, param->maxUeNum, mBuf);
   /* lvb_c_001.main_2: Added Nw Detach timer */
   CMCHKPK(SPkU32, param->tNwDetachTmr, mBuf);
   /* lvb_c_001.main_1: Added 3485 timer */
   CMCHKPK(SPkU8, param->t3485Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3486Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3489Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3495Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3470Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3460Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3450Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3412Tmr, mBuf);
   CMCHKPK(SPkU8, param->t3413Tmr, mBuf);
   /* lvb_c_001.main_2: Added 3422 timer */
   CMCHKPK(SPkU8, param->t3422Tmr, mBuf);
   CMCHKPK(SPkU16, param->t3402Tmr, mBuf);
   CMCHKPK(SPkU32, param->rac, mBuf);
   CMCHKPK(SPkU16, param->lac, mBuf);
   CMCHKPK(SPkU32, param->peerId, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkVbEnbCbCfg
(
VbEnbCbCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbEnbCbCfg(param, mBuf)
VbEnbCbCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbEnbCbCfg)

   CMCHKUNPK(SUnpkU32, &param->peerId, mBuf);
   CMCHKUNPK(SUnpkU16, &param->lac, mBuf);
   CMCHKUNPK(SUnpkU32, &param->rac, mBuf);
   CMCHKUNPK(SUnpkU16, &param->t3402Tmr, mBuf);
   /* lvb_c_001.main_2: Added 3422 timer */
   CMCHKUNPK(SUnpkU8, &param->t3422Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3413Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3412Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3450Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3460Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3470Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3495Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3489Tmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->t3486Tmr, mBuf);
   /* lvb_c_001.main_1: Added 3485 timer */
   CMCHKUNPK(SUnpkU8, &param->t3485Tmr, mBuf);
   /* lvb_c_001.main_2: Added Nw Detach timer */
   CMCHKUNPK(SUnpkU32, &param->tNwDetachTmr, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxUeNum, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->eNbAddr, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkVbNwId
(
VbNwId *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbNwId(param, mBuf)
VbNwId *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbNwId)

   for (i=LVB_MAX_MNC_LEN-1; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->mnc[i], mBuf);
   }
   for (i=LVB_MAX_MCC_LEN-1; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->mcc[i], mBuf);
   }
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbNwId
(
VbNwId *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbNwId(param, mBuf)
VbNwId *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbNwId)

   for (i=0; i<LVB_MAX_MCC_LEN; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->mcc[i], mBuf);
   }
   for (i=0; i<LVB_MAX_MNC_LEN; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->mnc[i], mBuf);
   }
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkVbGUMMEI
(
VbGUMMEI *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbGUMMEI(param, mBuf)
VbGUMMEI *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbGUMMEI)

   CMCHKPK(cmPkVbMmeCodeLst, &param->codeLst, mBuf);   
   CMCHKPK(cmPkVbGrpIdLst, &param->grpIdLst, mBuf);   
   CMCHKPK(cmPkVbPlmnLst, &param->pLst, mBuf);   
   CMCHKPK(cmPkVbNwId, &param->nwId, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbGUMMEI
(
VbGUMMEI *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbGUMMEI(param, mBuf)
VbGUMMEI *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbGUMMEI)

   CMCHKUNPK(cmUnpkVbNwId, &param->nwId, mBuf);
   CMCHKUNPK(cmUnpkVbPlmnLst, &param->pLst, mBuf);
   CMCHKUNPK(cmUnpkVbGrpIdLst, &param->grpIdLst, mBuf);
   CMCHKUNPK(cmUnpkVbMmeCodeLst, &param->codeLst, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkVbSrvdGLst
*
*
*     Desc : Served GUMMEI List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmPkVbSrvdGLst
(
VbSrvdGLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbSrvdGLst(param, mBuf)
VbSrvdGLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbSrvdGLst)

   for (i=param->noOfGummeis-1; i >= 0; i--)
   {
      CMCHKPK(cmPkVbGUMMEI, &param->gummeiLst[i], mBuf);
   }
   CMCHKPK(SPkU8, param->noOfGummeis, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkVbSrvdGLst
*
*
*     Desc : Served GUMMEI List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmUnpkVbSrvdGLst
(
VbSrvdGLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbSrvdGLst(param, mBuf)
VbSrvdGLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbSrvdGLst)

   CMCHKUNPK(SUnpkU8, &param->noOfGummeis, mBuf);
   for (i=0; i<param->noOfGummeis; i++)
   {
      CMCHKUNPK(cmUnpkVbGUMMEI, &param->gummeiLst[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkVbMmeCodeLst
*
*
*     Desc : Served MME CodeList
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmPkVbMmeCodeLst
(
VbMmeCodeLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbMmeCodeLst(param, mBuf)
VbMmeCodeLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbMmeCodeLst)

   for (i=param->noOfCodes-1; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->mmeCode[i], mBuf);
   }
   CMCHKPK(SPkU16, param->noOfCodes, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkVbMmeCodeLst
*
*
*     Desc : Served MME Code List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmUnpkVbMmeCodeLst
(
VbMmeCodeLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbMmeCodeLst(param, mBuf)
VbMmeCodeLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbMmeCodeLst)

   CMCHKUNPK(SUnpkU16, &param->noOfCodes, mBuf);
   for (i=0; i<param->noOfCodes; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->mmeCode[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkVbGrpIdLst
*
*
*     Desc : Served MME GroupId List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmPkVbGrpIdLst
(
VbGrpIdLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbGrpIdLst(param, mBuf)
VbGrpIdLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbGrpIdLst)

   for (i=param->noOfGrpIds-1; i >= 0; i--)
   {
      CMCHKPK(SPkU16, param->grpId[i], mBuf);
   }
   CMCHKPK(SPkU16, param->noOfGrpIds, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkVbGrpIdLst
*
*
*     Desc : Served MME GroupId List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmUnpkVbGrpIdLst
(
VbGrpIdLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbGrpIdLst(param, mBuf)
VbGrpIdLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbGrpIdLst)

   CMCHKUNPK(SUnpkU16, &param->noOfGrpIds, mBuf);
   for (i=0; i<param->noOfGrpIds; i++)
   {
      CMCHKUNPK(SUnpkU16, &param->grpId[i], mBuf);
   }
   RETVALUE(ROK);
}



#ifdef ANSI
PRIVATE S16 cmPkVbPlmnId
(
VbPlmnId *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbPlmnId(param, mBuf)
VbPlmnId *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbPlmnId)

   for (i=2; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->plmnId[i], mBuf);
   }
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbPlmnId
(
VbPlmnId *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbPlmnId(param, mBuf)
VbPlmnId *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbPlmnId)

   for (i=0; i<=2; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->plmnId[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkVbPlmnLst
*
*
*     Desc : PLMN List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmPkVbPlmnLst
(
VbPlmnLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbPlmnLst(param, mBuf)
VbPlmnLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbPlmnLst)

   for (i=param->noOfPlmns-1; i >= 0; i--)
   {
      CMCHKPK(cmPkVbPlmnId, &param->plmnLst[i], mBuf);
   }
   CMCHKPK(SPkU8, param->noOfPlmns, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkVbPlmnLst
*
*
*     Desc : PLMN List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmUnpkVbPlmnLst
(
VbPlmnLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbPlmnLst(param, mBuf)
VbPlmnLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbPlmnLst)

   CMCHKUNPK(SUnpkU8, &param->noOfPlmns, mBuf);
   for (i=0; i<param->noOfPlmns; i++)
   {
      CMCHKUNPK(cmUnpkVbPlmnId, &param->plmnLst[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkVbTai
*
*
*     Desc : Tracking Area Identity List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmPkVbTai
(
VbTai *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbTai(param, mBuf)
VbTai *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbTai)

   CMCHKPK(SPkU16, param->tac, mBuf);
   CMCHKPK(cmPkVbNwId, &param->nwId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkVbTai
*
*
*     Desc : Tracking Area Identity List
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 cmUnpkVbTai
(
VbTai *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbTai(param, mBuf)
VbTai *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbTai)

   CMCHKUNPK(cmUnpkVbNwId, &param->nwId, mBuf);
   CMCHKUNPK(SUnpkU16, &param->tac, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkVbTaiLst
(
VbTaiLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbTaiLst(param, mBuf)
VbTaiLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbTaiLst)

   for (i=LVB_MAX_TAI_LIST-1; i >= 0; i--)
   {
      CMCHKPK(cmPkVbTai, &param->taiLst[i], mBuf);
   }
   CMCHKPK(SPkU8, param->noOfTais, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbTaiLst
(
VbTaiLst *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbTaiLst(param, mBuf)
VbTaiLst *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbTaiLst)

   CMCHKUNPK(SUnpkU8, &param->noOfTais, mBuf);
   for (i=0; i<LVB_MAX_TAI_LIST; i++)
   {
      CMCHKUNPK(cmUnpkVbTai, &param->taiLst[i], mBuf);
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkMmeName
(
VbMmeName   *mmeName,     /* the string to be packed */
Buffer      *mBuf        /* to hold the packed buffer */
)
#else
PRIVATE S16 cmPkMmeName(mmeName, mBuf)
VbMmeName   *mmeName;     /* the string to be packed */
Buffer      *mBuf;       /* to hold the packed buffer */
#endif
{
   U16   i;
   TRC3(cmPkMmeName)

   /* pack the buffer of the string first */
   for (i = 0; (i < (U16) mmeName->length) && (i < LVB_MME_MAX_NAME); i++)
   {
      CMCHKPK(SPkU8, mmeName->name[i], mBuf);
   }

 /* pack the length */
   CMCHKPK(SPkU16,  mmeName->length, mBuf);

   /* return success */
   RETVALUE(ROK);
} /* end of cmPkMmeName */



#ifdef ANSI
PRIVATE S16 cmPkVbMmeGenCfg
(
VbMmeGenCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbMmeGenCfg(param, mBuf)
VbMmeGenCfg *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmPkVbMmeGenCfg)

   CMCHKPK(cmPkCmTptAddr, &param->sgsnIpAddr, mBuf);
   for (i = LVB_MAX_PLMN_ID_LEN - 1; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->sgsnPlmnId[i], mBuf);
   }

   CMCHKPK(SPkU32, param->t3412, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->tgtMmeIpAddr, mBuf);
   for (i = LVB_MAX_PLMN_ID_LEN - 1; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->tgtMmePlmnId[i], mBuf);
   }
   for (i=LVB_MAX_ENBS-1; i >= 0; i--)
   {
      CMCHKPK(cmPkVbEnbCbCfg, &param->eNbCfg[i], mBuf);
   }
   CMCHKPK(cmPkVbTaiLst, &param->tLst, mBuf);
   CMCHKPK(cmPkMmeName, &param->mmeName, mBuf);
   CMCHKPK(cmPkVbSrvdGLst, &param->srvGLst, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->pgwAddr, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->sgwAddr, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->sgsnAddr, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->s3selfAddr, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->s10selfAddr, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->selfAddr, mBuf);
   CMCHKPK(SPkU16, param->tmrRes, mBuf);
   CMCHKPK(SPkU16, param->maxNmbUes, mBuf);
   CMCHKPK(SPkU16, param->maxNmbEnbs, mBuf);
   CMCHKPK(SPkU16, param->maxNmbEgtSaps, mBuf);
   CMCHKPK(SPkU16, param->maxNmbSztSaps, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(cmPkPst, &param->lmPst, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkMmeName
(
VbMmeName   *mmeName,   
Buffer      *mBuf      
)
#else
PRIVATE S16 cmUnpkMmeName(mmeName, mBuf)
VbMmeName   *mmeName; 
Buffer      *mBuf;   
#endif
{
   S16   i = 0;
   TRC3(cmUnpkMmeName)

   /* pack the length */
   CMCHKUNPK(SUnpkU16,  &mmeName->length, mBuf);


   /* Unpack the buffer of the string first */
   for (i = mmeName->length-1; i >= 0; i--)
   {
      CMCHKUNPK(SUnpkU8, &mmeName->name[i], mBuf);
   }

   mmeName->name[mmeName->length] = '\0';

   /* return success */
   RETVALUE(ROK);
} /* end of cmUnpkMmeName */



#ifdef ANSI
PRIVATE S16 cmUnpkVbMmeGenCfg
(
VbMmeGenCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbMmeGenCfg(param, mBuf)
VbMmeGenCfg *param;
Buffer *mBuf;
#endif
{
   S32 i;
   TRC3(cmUnpkVbMmeGenCfg)

   CMCHKUNPK(cmUnpkPst, &param->lmPst, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbSztSaps, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbEgtSaps, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbEnbs, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbUes, mBuf);
   CMCHKUNPK(SUnpkU16, &param->tmrRes, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->selfAddr, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->s10selfAddr, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->s3selfAddr, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->sgsnAddr, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->sgwAddr, mBuf);
   CMCHKUNPK(cmUnpkCmTptAddr, &param->pgwAddr, mBuf);
   CMCHKUNPK(cmUnpkVbSrvdGLst, &param->srvGLst, mBuf);
   CMCHKUNPK(cmUnpkMmeName, &param->mmeName, mBuf);
   CMCHKUNPK(cmUnpkVbTaiLst, &param->tLst, mBuf);
   for (i=0; i<LVB_MAX_ENBS; i++)
   {
      CMCHKUNPK(cmUnpkVbEnbCbCfg, &param->eNbCfg[i], mBuf);
   }
   for (i = 0; i < LVB_MAX_PLMN_ID_LEN; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->tgtMmePlmnId[i], mBuf);
   }
   CMCHKUNPK(cmUnpkCmTptAddr, &param->tgtMmeIpAddr, mBuf);
   CMCHKUNPK(SUnpkU32, &param->t3412, mBuf);

   for (i = 0; i < LVB_MAX_PLMN_ID_LEN; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->sgsnPlmnId[i], mBuf);
   }
   CMCHKUNPK(cmUnpkCmTptAddr, &param->sgsnIpAddr, mBuf);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkVbMmeLSapCfg
(
VbMmeLSapCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbMmeLSapCfg(param, mBuf)
VbMmeLSapCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbMmeLSapCfg)

   CMCHKPK(SPkU8, param->type, mBuf);
   CMCHKPK(cmPkTmrCfg, &param->tBndTmr, mBuf);
   CMCHKPK(SPkU8, param->maxBndRetry, mBuf);
   CMCHKPK(SPkU8, param->dstInstId, mBuf);
   CMCHKPK(SPkU8, param->dstEntId, mBuf);
   CMCHKPK(SPkU16, param->dstProcId, mBuf);
   CMCHKPK(SPkU8, param->route, mBuf);
   CMCHKPK(SPkU8, param->prior, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(SPkU8, param->selector, mBuf);
   CMCHKPK(SPkS16, param->spId, mBuf);
   CMCHKPK(SPkS16, param->suId, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbMmeLSapCfg
(
VbMmeLSapCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbMmeLSapCfg(param, mBuf)
VbMmeLSapCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbMmeLSapCfg)

   CMCHKUNPK(SUnpkS16, &param->suId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->spId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->selector, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(SUnpkU8, &param->prior, mBuf);
   CMCHKUNPK(SUnpkU8, &param->route, mBuf);
   CMCHKUNPK(SUnpkU16, &param->dstProcId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dstEntId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dstInstId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->maxBndRetry, mBuf);
   CMCHKUNPK(cmUnpkTmrCfg, &param->tBndTmr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->type, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkVbCfgMme
(
VbCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbCfgMme(param, elmnt, mBuf)
VbCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbCfgMme)

   switch(elmnt)
   {
      case STVBGEN:
         CMCHKPK(cmPkVbMmeGenCfg, &param->s.mmeGenCfg, mBuf);
         break;
      case STVBSZTSAP:
         CMCHKPK(cmPkVbMmeLSapCfg, &param->s.mmeLSapCfg, mBuf);
         break;
      case STVBEGTSAP:
        CMCHKPK(cmPkVbMmeLSapCfg, &param->s.mmeLSapCfg, mBuf);
        break;
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkVbCfg
(
VbCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbCfg(param, elmnt, mBuf)
VbCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkVbCfg)

   ret1 = cmPkVbCfgMme(param, elmnt, mBuf);
   RETVALUE(ret1);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbCfgMme
(
VbCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbCfgMme(param, elmnt, mBuf)
VbCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbCfgMme)

   switch(elmnt)
   {
      case STVBGEN:
         CMCHKPK(cmUnpkVbMmeGenCfg, &param->s.mmeGenCfg, mBuf);
         break;
      case STVBSZTSAP:
         CMCHKPK(cmUnpkVbMmeLSapCfg, &param->s.mmeLSapCfg, mBuf);
         break;
      case STVBEGTSAP:
         CMCHKPK(cmUnpkVbMmeLSapCfg, &param->s.mmeLSapCfg, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
#ifdef ANSI
PRIVATE S16 cmUnpkVbCfg
(
VbCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbCfg(param, elmnt, mBuf)
VbCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkVbCfg)
   ret1 = cmUnpkVbCfgMme(param, elmnt, mBuf);
   RETVALUE(ret1);
}

#ifdef DEBUGP

#ifdef ANSI
PRIVATE S16 cmPkVbDbgCntrl
(
VbDbgCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbDbgCntrl(param, mBuf)
VbDbgCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbDbgCntrl)

   CMCHKPK(SPkU32, param->dbgMask, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkVbDbgCntrl
(
VbDbgCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbDbgCntrl(param, mBuf)
VbDbgCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbDbgCntrl)

   CMCHKUNPK(SUnpkU32, &param->dbgMask, mBuf);
   RETVALUE(ROK);
}
#endif

#ifdef ANSI
PRIVATE S16 cmPkVbSapCntrl
(
VbSapCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbSapCntrl(param, mBuf)
VbSapCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbSapCntrl)

   CMCHKPK(SPkS16, param->id, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkVbSapCntrl
(
VbSapCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbSapCntrl(param, mBuf)
VbSapCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbSapCntrl)

   CMCHKUNPK(SUnpkS16, &param->id, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkVbMmeCntrl
(
VbMmeCntrl *param,
Elmnt  elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbMmeCntrl(param, elmnt, mBuf)
VbMmeCntrl *param;
Elmnt  elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbMmeCntrl)

   switch(elmnt)
   {
      case STVBGEN:
      {
         switch(param->action)
         {
            case AENA:
            {
               switch (param->subAction)
               {
#ifdef DEBUGP
                  case SADBG:
                     CMCHKPK(cmPkVbDbgCntrl, &param->u.dbg, mBuf);
                     break;
#endif
                  default :
                     RETVALUE(RFAILED);
               }
               break;
            }
            case ADISIMM:
            {
               switch (param->subAction)
               {
#ifdef DEBUGP
                  case SADBG:
                     CMCHKPK(cmPkVbDbgCntrl, &param->u.dbg, mBuf);
                     break;
#endif
                  default :
                     RETVALUE(RFAILED);
               }
               break;
            }
            default:
            RETVALUE(RFAILED);

         }
         break;
      }
      case STVBSZTSAP:
      {
         CMCHKPK(cmPkVbSapCntrl, &param->u.sap, mBuf);
         break;
      }
      case STVBEGTSAP:
      {
         CMCHKPK(cmPkVbSapCntrl, &param->u.sap, mBuf);
         break;
      }
      default :
         RETVALUE(RFAILED);
   }

   CMCHKPK(SPkU8, param->subAction, mBuf);
   CMCHKPK(SPkU8, param->action, mBuf);
   CMCHKPK(cmPkDateTime, &param->dt, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkVbMmeCntrl
(
VbMmeCntrl *param,
Elmnt  elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbMmeCntrl(param, elmnt, mBuf)
VbMmeCntrl *param;
Elmnt  elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbMmeCntrl)

   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(SUnpkU8, &param->action, mBuf);
   CMCHKUNPK(SUnpkU8, &param->subAction, mBuf);

   switch(elmnt)
   {
      case STVBGEN:
      {
         switch(param->action)
         {
            case AENA:
            {
               switch (param->subAction)
               {
#ifdef DEBUGP
                  case SADBG:
                     CMCHKUNPK(cmUnpkVbDbgCntrl, &param->u.dbg, mBuf);
                     break;
#endif
                  default :
                     RETVALUE(RFAILED);
               }
               break;
            }
            case ADISIMM:
            {
               switch (param->subAction)
               {
#ifdef DEBUGP
                  case SADBG:
                     CMCHKUNPK(cmUnpkVbDbgCntrl, &param->u.dbg, mBuf);
                     break;
#endif
                  default :
                     RETVALUE(RFAILED);
               }
               break;
            }
            default:
            RETVALUE(RFAILED);

         }
         break;
      }
      case STVBSZTSAP:
      {
         CMCHKUNPK(cmUnpkVbSapCntrl, &param->u.sap, mBuf);
         break;
      }
      case STVBEGTSAP:
      {
         CMCHKUNPK(cmUnpkVbSapCntrl, &param->u.sap, mBuf);
         break;
      }
      default :
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}



#ifdef ANSI
PRIVATE S16 cmPkVbCntrl
(
VbCntrl *param,
Elmnt   elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbCntrl(param, elmnt, mBuf)
VbCntrl *param;
Elmnt   elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkVbCntrl)

   ret1 = cmPkVbMmeCntrl(&param->mmeCntrl, elmnt, mBuf);
   if(ret1 != ROK)
      RETVALUE(RFAILED);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkVbCntrl
(
VbCntrl *param,
Elmnt   elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbCntrl(param, elmnt, mBuf)
VbCntrl *param;
Elmnt   elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkVbCntrl)

   ret1 = cmUnpkVbMmeCntrl(&param->mmeCntrl, elmnt, mBuf);
   if(ret1 != ROK)
      RETVALUE(ret1);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkVbUstaDgn
(
VbUstaDgn *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbUstaDgn(param, mBuf)
VbUstaDgn *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkVbUstaDgn)

   CMCHKPK(cmPkCmAlarm, &param->alarm, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbUstaDgn
(
VbUstaDgn *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbUstaDgn(param, mBuf)
VbUstaDgn *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkVbUstaDgn)

   CMCHKUNPK(cmUnpkCmAlarm, &param->alarm, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkVbMngmt
(
VbMngmt *param,
S16 eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkVbMngmt(param, S16 eventType, mBuf)
VbMngmt *param;
S16 eventType;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkVbMngmt)

   switch(eventType)
   {
      case EVTLVBSTAIND:
         CMCHKPK(cmPkVbUstaDgn, &param->u.usta, mBuf);
         break;
      case EVTLVBCNTRLREQ:
      case EVTLVBCNTRLCFM:
         ret1 = cmPkVbCntrl(&param->u.cntrl, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLVBCFGREQ:
         ret1 = cmPkVbCfg(&param->u.cfg, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLVBSTACFM:
      case EVTLVBSTSCFM:
      case EVTLVBCFGCFM:
         CMCHKPK(cmPkCmStatus, &param->cfm, mBuf);
         break;
      case EVTLVBTRCIND:
      case EVTLVBSTSREQ:
      case EVTLVBSTAREQ:
      default :
         break;
   }
   if(ret1 != ROK)
      RETVALUE(ret1);

   CMCHKPK(cmPkHeader, &param->hdr, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkVbMngmt
(
VbMngmt *param,
S16 eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkVbMngmt(param, mBuf)
VbMngmt *param;
S16 eventType;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkVbMngmt)

   CMCHKUNPK(cmUnpkHeader, &param->hdr, mBuf);

   switch(eventType)
   {
      case EVTLVBCFGREQ:
         ret1 = cmUnpkVbCfg(&param->u.cfg, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLVBCNTRLREQ:
      case EVTLVBCNTRLCFM:
         ret1 = cmUnpkVbCntrl(&param->u.cntrl, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLVBSTAIND:
         ret1 = cmUnpkVbUstaDgn(&param->u.usta, mBuf);
         break;
      case EVTLVBCFGCFM:
      case EVTLVBSTSCFM:
      case EVTLVBSTACFM:
         CMCHKUNPK(cmUnpkCmStatus, &param->cfm, mBuf);
         break;
      case EVTLVBSTSREQ:
      case EVTLVBSTAREQ:
      case EVTLVBTRCIND:
      default :
         break;
   }
   if(ret1 != ROK)
      RETVALUE(ret1);
   RETVALUE(ROK);
}

#endif

#ifdef __cplusplus
}
#endif


/********************************************************************30**
  
         End of file:     lvb.c@@/main/nodal_main/2 - Fri Jan  3 14:56:05 2014
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
/main/2      ---      lvb_c_001.main_1  rk          1. Added 3485 timer packing and unpacking
/main/3     lvb_c_001.main_2  rk        1. Added 3422 and Nw Detach timer packing and unpacking
/main/4     lvb_c_001.main_3  ragrawal  1. Added packing and unpacking for 3495 timer.
/main/5      ---      lvb_c_001.main_4 akaranth   1. s1ap inclusive order has chnged
*********************************************************************91*/
