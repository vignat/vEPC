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
  
        Name:     LTE CNE Layer management interface
    
        Type:     C include file
  
        Desc:     This file contains defines required by LTE CNE layer 
                  management interface
 
        File:     lqo.c

        Sid:      lqo.c@@/main/1 - Thu Jul  8 21:25:40 2010
  
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
#include "lqo.h"

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
#include "lqo.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#if (defined(LCLQO) || defined(LWLCLQO))

/* Function prototypes */
PRIVATE S16 cmPkLqoMngmt ARGS((QoMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoMngmt ARGS((QoMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmPkLqoGenCfg ARGS((QoGenCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoGenCfg ARGS((QoGenCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkLqoLSapCfg ARGS((QoLSapCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoLSapCfg ARGS((QoLSapCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkLqoCfgSgw ARGS((QoCfg *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkLqoCfg ARGS((QoCfg *param, Elmnt elmnt,
         Buffer *mBuf));
PRIVATE S16 cmUnpkLqoCfgSgw ARGS((QoCfg *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoCfg ARGS((QoCfg *param, Elmnt elmnt,
         Buffer *mBuf));
#ifdef DEBUGP
PRIVATE S16 cmPkLqoDbgCntrl ARGS((QoDbgCntrl *param, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoDbgCntrl ARGS((QoDbgCntrl *param, Buffer *mBuf));
#endif /* DEBUGP */
PRIVATE S16 cmPkLqoSapCntrl ARGS((QoSapCntrl *param, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoSapCntrl ARGS((QoSapCntrl *param, Buffer *mBuf));
PRIVATE S16 cmPkLqoCntrl ARGS((QoCntrl *param, Elmnt   elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoCntrl ARGS((QoCntrl *param, Elmnt   elmnt, Buffer *mBuf));
PRIVATE S16 cmPkLqoUstaDgn ARGS((QoUstaDgn *param, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoUstaDgn ARGS((QoUstaDgn *param, Buffer *mBuf));
PRIVATE S16 cmPkLqoMngmt ARGS((QoMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmUnpkLqoMngmt ARGS((QoMngmt *param, S16 eventType, Buffer *mBuf));


#ifdef ANSI
PUBLIC S16 cmPkLqoCfgReq
(
Pst     * pst,
QoMngmt * cfg
)
#else
PUBLIC S16 cmPkLqoCfgReq(pst, cfg)
Pst     * pst;
QoMngmt * cfg;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(cfg, EVTLQOCFGREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)cfg, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfg,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoCfgReq
(
LqoCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoCfgReq(func, pst, mBuf)
LqoCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *cfg;

   TRC3(cmUnpkLqoCfgReq)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfg,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfg, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC) 
      if (cmUnpkLqoMngmt(cfg, EVTLQOCFGREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfg));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoCntrlReq
(
Pst     * pst,
QoMngmt * cntrl
)
#else
PUBLIC S16 cmPkLqoCntrlReq(pst, cntrl)
Pst     * pst;
QoMngmt * cntrl;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoCntrlReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(cntrl, EVTLQOCNTRLREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)cntrl, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cntrl,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOCNTRLREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoCntrlReq
(
LqoCntrlReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoCntrlReq(func, pst, mBuf)
LqoCntrlReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *cntrl;
   
   TRC3(cmUnpkLqoCntrlReq)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cntrl,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cntrl, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC) 
      if (cmUnpkLqoMngmt(cntrl, EVTLQOCNTRLREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cntrl));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoStaReq
(
Pst     * pst,
QoMngmt * sta
)
#else
PUBLIC S16 cmPkLqoStaReq(pst, sta)
Pst     * pst;
QoMngmt * sta;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoStaReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(sta, EVTLQOSTAREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)sta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)sta,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOSTAREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoStaReq
(
LqoStaReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoStaReq(func, pst, mBuf)
LqoStaReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *sta;

   TRC3(cmUnpkLqoStaReq)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&sta,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&sta, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(sta, EVTLQOSTAREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, sta));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoStsReq
(
Pst     * pst,
Action action,
QoMngmt * sts
)
#else
PUBLIC S16 cmPkLqoStsReq(pst, action, sts)
Pst     * pst;
Action action;
QoMngmt * sts;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoStsReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(sts, EVTLQOSTSREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)sts, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (cmPkAction(action, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(QoMngmt));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)sts,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOSTSREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoStsReq
(
LqoStsReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoStsReq(func, pst, mBuf)
LqoStsReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   Action action;
   QoMngmt *sts;

   TRC3(cmUnpkLqoStsReq)

   if (cmUnpkAction(&action, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&sts,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&sts, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(sts, EVTLQOSTSREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, action, sts));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoCfgCfm
(
Pst     * pst,
QoMngmt * cfm
)
#else
PUBLIC S16 cmPkLqoCfgCfm(pst, cfm)
Pst     * pst;
QoMngmt * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoCfgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(cfm, EVTLQOCFGCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOCFGCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoCfgCfm
(
LqoCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoCfgCfm(func, pst, mBuf)
LqoCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *cfm;

   TRC3(cmUnpkLqoCfgCfm)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC) 
      if (cmUnpkLqoMngmt(cfm, EVTLQOCFGCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoStsCfm
(
Pst     * pst,
QoMngmt * cfm
)
#else
PUBLIC S16 cmPkLqoStsCfm(pst, cfm)
Pst     * pst;
QoMngmt * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoStsCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(cfm, EVTLQOSTSCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOSTSCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoStsCfm
(
LqoStsCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoStsCfm(func, pst, mBuf)
LqoStsCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *cfm;

   TRC3(cmUnpkLqoStsCfm)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(cfm, EVTLQOSTSCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoStaCfm
(
Pst      * pst,
QoMngmt  * cfm
)
#else
PUBLIC S16 cmPkLqoStaCfm(pst, cfm)
Pst      * pst;
QoMngmt  * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoStaCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(cfm, EVTLQOSTACFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOSTACFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoStaCfm
(
LqoStaCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoStaCfm(func, pst, mBuf)
LqoStaCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *cfm;

   TRC3(cmUnpkLqoStaCfm)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(cfm, EVTLQOSTACFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoStaInd
(
Pst      * pst,
QoMngmt  * usta
)
#else
PUBLIC S16 cmPkLqoStaInd(pst, usta)
Pst      * pst;
QoMngmt  * usta;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoStaInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(usta, EVTLQOSTAIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)usta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)usta,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOSTAIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoStaInd
(
LqoStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoStaInd(func, pst, mBuf)
LqoStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *usta;

   TRC3(cmUnpkLqoStaInd)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&usta,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&usta, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(usta, EVTLQOSTAIND, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, usta));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoCntrlCfm
(
Pst      * pst,
QoMngmt  * cfm
)
#else
PUBLIC S16 cmPkLqoCntrlCfm(pst, cfm)
Pst      * pst;
QoMngmt  * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLqoCntrlCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(cfm, EVTLQOCNTRLCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOCNTRLCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoCntrlCfm
(
LqoCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoCntrlCfm(func, pst, mBuf)
LqoCntrlCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *cfm;

   TRC3(cmUnpkLqoCntrlCfm)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(cfm, EVTLQOCNTRLCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLqoTrcInd
(
Pst      * pst,
QoMngmt  * trc,
Buffer   * trcBuf
)
#else
PUBLIC S16 cmPkLqoTrcInd(pst, trc, trcBuf)
Pst      * pst;
QoMngmt  * trc;
Buffer   * trcBuf;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;

   TRC3(cmPkLqoTrcInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      if (SFndLenMsg(trcBuf, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(QoMngmt));
         SPutMsg(trcBuf);
         RETVALUE(RFAILED);
      }
      if (SCatMsg(mBuf, trcBuf, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(QoMngmt));
         SPutMsg(trcBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(trcBuf);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)trcBuf, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(QoMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LC) {
      if (cmPkLqoMngmt(trc, EVTLQOTRCIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(QoMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmPkPtr((PTR)trc, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(QoMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)trc,
               sizeof(QoMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLQOTRCIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLqoTrcInd
(
LqoTrcInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLqoTrcInd(func, pst, mBuf)
LqoTrcInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   QoMngmt *trc;
   Buffer *trcBuf = NULLP;

   TRC3(cmUnpkLqoTrcInd)

   if (pst->selector != LQO_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&trc,
                  sizeof(QoMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&trc, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LQO_SEL_LC)
      if (cmUnpkLqoMngmt(trc, EVTLQOTRCIND, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(QoMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EQOXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LQO_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &trcBuf) != ROK)
         RETVALUE(RFAILED);
   }
   else if (pst->selector == LQO_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&trcBuf, mBuf) != ROK) {
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, trc, trcBuf));
}





#ifdef ANSI
PRIVATE S16 cmPkLqoGenCfg
(
QoGenCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoGenCfg(param, mBuf)
QoGenCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoGenCfg)

   CMCHKPK(SPkU16, param->maxNmbTunls, mBuf);
   CMCHKPK(SPkU16, param->maxNmbBearers, mBuf);
   CMCHKPK(SPkU16, param->maxNmbEgtSaps, mBuf);
   CMCHKPK(SPkU16, param->tmrRes, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(cmPkPst, &param->lmPst, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkLqoGenCfg
(
QoGenCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoGenCfg(param, mBuf)
QoGenCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoGenCfg)

   CMCHKUNPK(cmUnpkPst, &param->lmPst, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(SUnpkU16, &param->tmrRes, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbEgtSaps, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbBearers, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbTunls, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkCmInetNetAddr
(
CmInetNetAddr *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkCmInetNetAddr(param, mBuf)
CmInetNetAddr *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkCmInetNetAddr)

   switch(param->type)
   {
      case CM_INET_IPV4ADDR_TYPE:
         cmPkCmInetIpAddr(param->u.ipv4NetAddr, mBuf);
         break;
#ifdef IPV6_SUPPORTED
      case CM_INET_IPV6ADDR_TYPE:
         for (i=15; i >= 0; i--)
         {
            CMCHKPK(SPkU8, param->ipv6NetAddr[i], mBuf);
         }

         break;
#endif
      default:
         RETVALUE(RFAILED);
   }
   CMCHKPK(SPkU8, param->type, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkCmInetNetAddr
(
CmInetNetAddr *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkCmInetNetAddr(param, mBuf)
CmInetNetAddr *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkCmInetNetAddr)

   CMCHKUNPK(SUnpkU8, &param->type, mBuf);

   switch(param->type)
   {
      case CM_INET_IPV4ADDR_TYPE:
         CMCHKUNPK(cmUnpkCmInetIpAddr, &param->u.ipv4NetAddr, mBuf);
         break;
#ifdef IPV6_SUPPORTED
      case CM_INET_IPV6ADDR_TYPE:
         for (i=0; i<16; i++)
         {
            CMCHKUNPK(SUnpkU8, &param->ipv6NetAddr[i], mBuf);
         }

         break;
#endif
      default:
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}






#ifdef ANSI
PRIVATE S16 cmPkLqoLSapCfg
(
QoLSapCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoLSapCfg(param, mBuf)
QoLSapCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoLSapCfg)

   CMCHKPK(SPkU32, param->nmbTunn, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->localAddr, mBuf);
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
PRIVATE S16 cmUnpkLqoLSapCfg
(
QoLSapCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoLSapCfg(param, mBuf)
QoLSapCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoLSapCfg)

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
   CMCHKUNPK(cmUnpkCmTptAddr, &param->localAddr, mBuf);
   CMCHKUNPK(SUnpkU32, &param->nmbTunn, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkLqoCfgSgw
(
QoCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoCfgSgw(param, elmnt, mBuf)
QoCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoCfgSgw)

   switch(elmnt)
   {
      case STQOGEN:
         CMCHKPK(cmPkLqoGenCfg, &param->s.sgwGenCfg, mBuf);
         break;
      case STQOEGUSAP:
      case STQOPGWCSAP:
      case STQOMMECSAP:
      case STQOECHOCSAP:
      case STQOSGSNCSAP:
         CMCHKPK(cmPkLqoLSapCfg, &param->s.sgwLSapCfg, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}




#ifdef ANSI
PRIVATE S16 cmPkLqoCfg
(
QoCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoCfg(param, elmnt, mBuf)
QoCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkLqoCfg)

   ret1 = cmPkLqoCfgSgw(param, elmnt, mBuf);
   RETVALUE(ret1);
}


#ifdef ANSI
PRIVATE S16 cmUnpkLqoCfgSgw
(
QoCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoCfgSgw(param, elmnt, mBuf)
QoCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoCfgSgw)

   switch(elmnt)
   {
      case STQOGEN:
         CMCHKPK(cmUnpkLqoGenCfg, &param->s.sgwGenCfg, mBuf);
         break;
      case STQOEGUSAP:
      case STQOPGWCSAP:
      case STQOMMECSAP:
      case STQOECHOCSAP:
      case STQOSGSNCSAP:
         CMCHKPK(cmUnpkLqoLSapCfg, &param->s.sgwLSapCfg, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}




#ifdef ANSI
PRIVATE S16 cmUnpkLqoCfg
(
QoCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoCfg(param, elmnt, mBuf)
QoCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkLqoCfg)

   ret1 = cmUnpkLqoCfgSgw(param, elmnt, mBuf);
   RETVALUE(ret1);
}

#ifdef DEBUGP

#ifdef ANSI
PRIVATE S16 cmPkLqoDbgCntrl
(
QoDbgCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoDbgCntrl(param, mBuf)
QoDbgCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoDbgCntrl)

   CMCHKPK(SPkU32, param->dbgMask, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkLqoDbgCntrl
(
QoDbgCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoDbgCntrl(param, mBuf)
QoDbgCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoDbgCntrl)

   CMCHKUNPK(SUnpkU32, &param->dbgMask, mBuf);
   RETVALUE(ROK);
}
#endif

#ifdef ANSI
PRIVATE S16 cmPkLqoSapCntrl
(
QoSapCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoSapCntrl(param, mBuf)
QoSapCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoSapCntrl)

   CMCHKPK(SPkS16, param->id, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkLqoSapCntrl
(
QoSapCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoSapCntrl(param, mBuf)
QoSapCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoSapCntrl)

   CMCHKUNPK(SUnpkS16, &param->id, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkLqoCntrl
(
QoCntrl *param,
Elmnt  elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoCntrl(param, elmnt, mBuf)
QoCntrl *param;
Elmnt  elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoCntrl)

   switch(elmnt)
   {
      case STQOGEN:
      {
         switch(param->action)
         {
            case AENA:
            {
               switch (param->subAction)
               {
#ifdef DEBUGP
                  case SADBG:
                     CMCHKPK(cmPkLqoDbgCntrl, &param->u.dbg, mBuf);
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
                     CMCHKPK(cmPkLqoDbgCntrl, &param->u.dbg, mBuf);
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
      case STQOEGUSAP:
      {
         CMCHKPK(cmPkLqoSapCntrl, &param->u.sap, mBuf);
         break;
      }
      case STQOPGWCSAP:
      {
         CMCHKPK(cmPkLqoSapCntrl, &param->u.sap, mBuf);
         break;
      }
      case STQOMMECSAP:
      case STQOECHOCSAP:
      case STQOSGSNCSAP:
      {
         CMCHKPK(cmPkLqoSapCntrl, &param->u.sap, mBuf);
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
PRIVATE S16 cmUnpkLqoCntrl
(
QoCntrl *param,
Elmnt  elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoCntrl(param, elmnt, mBuf)
QoCntrl *param;
Elmnt  elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoCntrl)

   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(SUnpkU8, &param->action, mBuf);
   CMCHKUNPK(SUnpkU8, &param->subAction, mBuf);

   switch(elmnt)
   {
      case STQOGEN:
      {
         switch(param->action)
         {
            case AENA:
            {
               switch (param->subAction)
               {
#ifdef DEBUGP
                  case SADBG:
                     CMCHKUNPK(cmUnpkLqoDbgCntrl, &param->u.dbg, mBuf);
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
                     CMCHKUNPK(cmUnpkLqoDbgCntrl, &param->u.dbg, mBuf);
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
      case STQOEGUSAP:
      case STQOPGWCSAP:
      case STQOMMECSAP:
      case STQOECHOCSAP:
      case STQOSGSNCSAP:
      {
         CMCHKUNPK(cmUnpkLqoSapCntrl, &param->u.sap, mBuf);
         break;
      }
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkLqoUstaDgn
(
QoUstaDgn *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoUstaDgn(param, mBuf)
QoUstaDgn *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkLqoUstaDgn)

   CMCHKPK(cmPkCmAlarm, &param->alarm, mBuf);
   CMCHKPK(SPkS16, param->sapId, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkLqoUstaDgn
(
QoUstaDgn *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoUstaDgn(param, mBuf)
QoUstaDgn *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkLqoUstaDgn)

   CMCHKUNPK(SUnpkS16, &param->sapId, mBuf);
   CMCHKUNPK(cmUnpkCmAlarm, &param->alarm, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkLqoMngmt
(
QoMngmt *param,
S16 eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkLqoMngmt(param, S16 eventType, mBuf)
QoMngmt *param;
S16 eventType;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkLqoMngmt)

   switch(eventType)
   {
      case EVTLQOSTAIND:
         CMCHKPK(cmPkLqoUstaDgn, &param->u.usta, mBuf);
         break;
      case EVTLQOCNTRLREQ:
         ret1 = cmPkLqoCntrl(&param->u.cntrl, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLQOCFGREQ:
         ret1 = cmPkLqoCfg(&param->u.cfg,
               param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLQOSTACFM:
      case EVTLQOCNTRLCFM:
      case EVTLQOSTSCFM:
      case EVTLQOCFGCFM:
         CMCHKPK(cmPkCmStatus, &param->cfm, mBuf);
         break;
      case EVTLQOTRCIND:
      case EVTLQOSTSREQ:
      case EVTLQOSTAREQ:
      default :
         break;
   }
   if(ret1 != ROK)
      RETVALUE(ret1);

   CMCHKPK(cmPkHeader, &param->hdr, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkLqoMngmt
(
QoMngmt *param,
S16 eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkLqoMngmt(param, mBuf)
QoMngmt *param;
S16 eventType;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkLqoMngmt)

   CMCHKUNPK(cmUnpkHeader, &param->hdr, mBuf);

   switch(eventType)
   {
      case EVTLQOCFGREQ:
         ret1 = cmUnpkLqoCfg(&param->u.cfg,
               param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLQOCNTRLREQ:
         ret1 = cmUnpkLqoCntrl(&param->u.cntrl, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLQOSTAIND:
         ret1 = cmUnpkLqoUstaDgn(&param->u.usta, mBuf);
         break;
      case EVTLQOCFGCFM:
      case EVTLQOSTSCFM:
      case EVTLQOCNTRLCFM:
      case EVTLQOSTACFM:
         CMCHKUNPK(cmUnpkCmStatus, &param->cfm, mBuf);
         break;
      case EVTLQOSTSREQ:
      case EVTLQOSTAREQ:
      case EVTLQOTRCIND:
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
  
         End of file:     lqo.c@@/main/1 - Thu Jul  8 21:25:40 2010
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
*********************************************************************91*/
