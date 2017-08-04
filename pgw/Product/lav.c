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
 
        File:     lav.c

        Sid:      lav.c@@/main/1 - Tue Aug 17 18:55:16 2010
  
        Prg:      ve
  
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

#include <pcap.h>

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

#include "leg.h"
#include "egt.h"

#include "lav.h"
#include "av.h"

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

#include "leg.x"
#include "egt.x"

#include "lav.x"
#include "av.x"            /* CNE Application structures     */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#if (defined(LCLAV) || defined(LWLCLAV))

/* Function prototypes */
PRIVATE S16 cmPkAvMngmt ARGS((AvMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmUnpkAvMngmt ARGS((AvMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmPkCmInetNetAddr ARGS((CmInetNetAddr *param, Buffer *mBuf));
PRIVATE S16 cmUnpkCmInetNetAddr ARGS((CmInetNetAddr *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwExGwCfg ARGS((AvPgwExGwCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwExGwCfg ARGS((AvPgwExGwCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwAddrCfg ARGS((AvPgwAddrCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwAddrCfg ARGS((AvPgwAddrCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwSgwCfg ARGS((AvPgwSgwCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwSgwCfg ARGS((AvPgwSgwCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwPeerCfg ARGS((AvPgwPeerCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwPeerCfg ARGS((AvPgwPeerCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwLSapCfg ARGS((AvPgwLSapCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwLSapCfg ARGS((AvPgwLSapCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwGenCfg ARGS((AvPgwGenCfg *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwGenCfg ARGS((AvPgwGenCfg *param, Buffer *mBuf));
PRIVATE S16 cmPkAvCfgPgw ARGS((AvCfg *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmPkAvCfg ARGS((AvCfg *param, Elmnt elmnt,
         Buffer *mBuf));
PRIVATE S16 cmUnpkAvCfgPgw ARGS((AvCfg *param, Elmnt elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkAvCfg ARGS((AvCfg *param, Elmnt elmnt,
         Buffer *mBuf));
#ifdef DEBUGP
PRIVATE S16 cmPkAvDbgCntrl ARGS((AvDbgCntrl *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvDbgCntrl ARGS((AvDbgCntrl *param, Buffer *mBuf));
#endif /* DEBUGP */
PRIVATE S16 cmPkAvSapCntrl ARGS((AvSapCntrl *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvSapCntrl ARGS((AvSapCntrl *param, Buffer *mBuf));
PRIVATE S16 cmPkAvPgwCntrl ARGS((AvPgwCntrl *param, Elmnt  elmnt,
         Buffer *mBuf));
PRIVATE S16 cmUnpkAvPgwCntrl ARGS((AvPgwCntrl *param, Elmnt  elmnt,
         Buffer *mBuf));
PRIVATE S16 cmPkAvCntrl ARGS((AvCntrl *param, Elmnt   elmnt, Buffer *mBuf));
PRIVATE S16 cmUnpkAvCntrl ARGS((AvCntrl *param, Elmnt   elmnt, Buffer *mBuf));
PRIVATE S16 cmPkAvUstaDgn ARGS((AvUstaDgn *param, Buffer *mBuf));
PRIVATE S16 cmUnpkAvUstaDgn ARGS((AvUstaDgn *param, Buffer *mBuf));
PRIVATE S16 cmPkAvMngmt ARGS((AvMngmt *param, S16 eventType, Buffer *mBuf));
PRIVATE S16 cmUnpkAvMngmt ARGS((AvMngmt *param, S16 eventType, Buffer *mBuf));


#ifdef ANSI
PUBLIC S16 cmPkLavCfgReq
(
Pst     * pst,
AvMngmt * cfg
)
#else
PUBLIC S16 cmPkLavCfgReq(pst, cfg)
Pst     * pst;
AvMngmt * cfg;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(cfg, EVTLAVCFGREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)cfg, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfg,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavCfgReq
(
LavCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavCfgReq(func, pst, mBuf)
LavCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *cfg;

   TRC3(cmUnpkLavCfgReq)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfg,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfg, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC) 
      if (cmUnpkAvMngmt(cfg, EVTLAVCFGREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfg));
}

#ifdef ANSI
PUBLIC S16 cmPkLavCntrlReq
(
Pst     * pst,
AvMngmt * cntrl
)
#else
PUBLIC S16 cmPkLavCntrlReq(pst, cntrl)
Pst     * pst;
AvMngmt * cntrl;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavCntrlReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(cntrl, EVTLAVCNTRLREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)cntrl, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cntrl,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVCNTRLREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavCntrlReq
(
LavCntrlReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavCntrlReq(func, pst, mBuf)
LavCntrlReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *cntrl;
   
   TRC3(cmUnpkLavCntrlReq)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cntrl,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cntrl, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC) 
      if (cmUnpkAvMngmt(cntrl, EVTLAVCNTRLREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cntrl));
}

#ifdef ANSI
PUBLIC S16 cmPkLavStaReq
(
Pst     * pst,
AvMngmt * sta
)
#else
PUBLIC S16 cmPkLavStaReq(pst, sta)
Pst     * pst;
AvMngmt * sta;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavStaReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(sta, EVTLAVSTAREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)sta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)sta,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVSTAREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavStaReq
(
LavStaReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavStaReq(func, pst, mBuf)
LavStaReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *sta;

   TRC3(cmUnpkLavStaReq)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&sta,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&sta, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(sta, EVTLAVSTAREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)sta, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, sta));
}

#ifdef ANSI
PUBLIC S16 cmPkLavStsReq
(
Pst     * pst,
Action action,
AvMngmt * sts
)
#else
PUBLIC S16 cmPkLavStsReq(pst, action, sts)
Pst     * pst;
Action action;
AvMngmt * sts;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavStsReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(sts, EVTLAVSTSREQ, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)sts, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (cmPkAction(action, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(AvMngmt));
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)sts,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVSTSREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavStsReq
(
LavStsReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavStsReq(func, pst, mBuf)
LavStsReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   Action action;
   AvMngmt *sts;

   TRC3(cmUnpkLavStsReq)

   if (cmUnpkAction(&action, mBuf) != ROK) {
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&sts,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&sts, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(sts, EVTLAVSTSREQ, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)sts, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, action, sts));
}

#ifdef ANSI
PUBLIC S16 cmPkLavCfgCfm
(
Pst     * pst,
AvMngmt * cfm
)
#else
PUBLIC S16 cmPkLavCfgCfm(pst, cfm)
Pst     * pst;
AvMngmt * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavCfgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(cfm, EVTLAVCFGCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVCFGCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavCfgCfm
(
LavCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavCfgCfm(func, pst, mBuf)
LavCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *cfm;

   TRC3(cmUnpkLavCfgCfm)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC) 
      if (cmUnpkAvMngmt(cfm, EVTLAVCFGCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLavStsCfm
(
Pst     * pst,
AvMngmt * cfm
)
#else
PUBLIC S16 cmPkLavStsCfm(pst, cfm)
Pst     * pst;
AvMngmt * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavStsCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(cfm, EVTLAVSTSCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVSTSCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavStsCfm
(
LavStsCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavStsCfm(func, pst, mBuf)
LavStsCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *cfm;

   TRC3(cmUnpkLavStsCfm)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(cfm, EVTLAVSTSCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLavStaCfm
(
Pst      * pst,
AvMngmt  * cfm
)
#else
PUBLIC S16 cmPkLavStaCfm(pst, cfm)
Pst      * pst;
AvMngmt  * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavStaCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(cfm, EVTLAVSTACFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVSTACFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavStaCfm
(
LavStaCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavStaCfm(func, pst, mBuf)
LavStaCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *cfm;

   TRC3(cmUnpkLavStaCfm)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(cfm, EVTLAVSTACFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLavStaInd
(
Pst      * pst,
AvMngmt  * usta
)
#else
PUBLIC S16 cmPkLavStaInd(pst, usta)
Pst      * pst;
AvMngmt  * usta;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavStaInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(usta, EVTLAVSTAIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)usta, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)usta,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVSTAIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavStaInd
(
LavStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavStaInd(func, pst, mBuf)
LavStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *usta;

   TRC3(cmUnpkLavStaInd)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&usta,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&usta, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(usta, EVTLAVSTAIND, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, usta));
}

#ifdef ANSI
PUBLIC S16 cmPkLavCntrlCfm
(
Pst      * pst,
AvMngmt  * cfm
)
#else
PUBLIC S16 cmPkLavCntrlCfm(pst, cfm)
Pst      * pst;
AvMngmt  * cfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkLavCntrlCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(cfm, EVTLAVCNTRLCFM, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)cfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)cfm,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVCNTRLCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavCntrlCfm
(
LavCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavCntrlCfm(func, pst, mBuf)
LavCntrlCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *cfm;

   TRC3(cmUnpkLavCntrlCfm)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&cfm,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&cfm, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(cfm, EVTLAVCNTRLCFM, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, cfm));
}

#ifdef ANSI
PUBLIC S16 cmPkLavTrcInd
(
Pst      * pst,
AvMngmt  * trc,
Buffer   * trcBuf
)
#else
PUBLIC S16 cmPkLavTrcInd(pst, trc, trcBuf)
Pst      * pst;
AvMngmt  * trc;
Buffer   * trcBuf;
#endif
{
   Buffer *mBuf = NULLP;
   MsgLen msgLen = 0;

   TRC3(cmPkLavTrcInd)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      if (SFndLenMsg(trcBuf, &msgLen) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(AvMngmt));
         SPutMsg(trcBuf);
         RETVALUE(RFAILED);
      }
      if (SCatMsg(mBuf, trcBuf, M1M2) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(AvMngmt));
         SPutMsg(trcBuf);
         RETVALUE(RFAILED);
      }
      SPutMsg(trcBuf);
      CMCHKPK(cmPkMsgLen, msgLen, mBuf);

   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)trcBuf, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(AvMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LC) {
      if (cmPkAvMngmt(trc, EVTLAVTRCIND, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(AvMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmPkPtr((PTR)trc, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(AvMngmt));
         SPutMsg(trcBuf);
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else {
      if (SPutSBuf(pst->region, pst->pool, (Data *)trc,
               sizeof(AvMngmt)) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   pst->event = (Event) EVTLAVTRCIND;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkLavTrcInd
(
LavTrcInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLavTrcInd(func, pst, mBuf)
LavTrcInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
   AvMngmt *trc;
   /*Added a Klockworks Check : Initialising trcBuf*/
   Buffer *trcBuf = NULLP;

   TRC3(cmUnpkLavTrcInd)

   if (pst->selector != LAV_SEL_LWLC) {
      if ((SGetSBuf(pst->region, pst->pool, (Data **)&trc,
                  sizeof(AvMngmt))) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&trc, mBuf) != ROK) {
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
   }
   else if (pst->selector == LAV_SEL_LC)
      if (cmUnpkAvMngmt(trc, EVTLAVTRCIND, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)trc, sizeof(AvMngmt));
      SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EAVXXX, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (pst->selector == LAV_SEL_LC) {
      MsgLen msgLen, totalMsgLen;
      CMCHKUNPK(cmUnpkMsgLen, &msgLen, mBuf);
      if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
         RETVALUE(RFAILED);
      if (SSegMsg(mBuf, totalMsgLen-msgLen, &trcBuf) != ROK)
         RETVALUE(RFAILED);
   }
   else if (pst->selector == LAV_SEL_LWLC) {
      if (cmUnpkPtr((PTR *)&trcBuf, mBuf) != ROK) {
         RETVALUE(RFAILED);
      }
   }
   SPutMsg(mBuf);
   RETVALUE((*func)(pst, trc, trcBuf));
}







#ifdef ANSI
PRIVATE S16 cmPkCmInetNetAddr
(
CmInetNetAddr *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkCmInetNetAddr(param, mBuf)
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
PRIVATE S16 cmUnpkCmInetNetAddr
(
CmInetNetAddr *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkCmInetNetAddr(param, mBuf)
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
PRIVATE S16 cmPkAvPgwExGwCfg
(
AvPgwExGwCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwExGwCfg(param, mBuf)
AvPgwExGwCfg *param;
Buffer *mBuf;
#endif
{
#if 0
   S32 i;
#endif
   TRC3(cmPkAvPgwExGwCfg)

#if 1
   CMCHKPK(cmPkCmInetNetAddr, param, mBuf);
#else
   for (i=AV_PGW_ROUT_IP_ADDR_LEN-1; i >= 0; i--)
   {
      CMCHKPK(SPkU8, param->addr[i], mBuf);
   }
#endif
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkAvPgwExGwCfg
(
AvPgwExGwCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwExGwCfg(param, mBuf)
AvPgwExGwCfg *param;
Buffer *mBuf;
#endif
{
#if 0
   S32 i;
#endif
   TRC3(cmUnpkAvPgwExGwCfg)

#if 1
   CMCHKUNPK(cmUnpkCmInetNetAddr, param, mBuf);
#else
   for (i=0; i<AV_PGW_ROUT_IP_ADDR_LEN; i++)
   {
      CMCHKUNPK(SUnpkU8, &param->addr[i], mBuf);
   }
#endif
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkAvPgwAddrCfg
(
AvPgwAddrCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwAddrCfg(param, mBuf)
AvPgwAddrCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvPgwAddrCfg)

   CMCHKPK(cmPkCmInetNetAddr, &param->pgwNetAddr.startAddr, mBuf);
   CMCHKPK(SPkU8, param->pgwNetAddr.nmb, mBuf);
   CMCHKPK(cmPkCmTptAddr, &param->selfAddr, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvPgwAddrCfg
(
AvPgwAddrCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwAddrCfg(param, mBuf)
AvPgwAddrCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvPgwAddrCfg)

   CMCHKUNPK(cmUnpkCmTptAddr, &param->selfAddr, mBuf);
   CMCHKUNPK(SUnpkU8, &param->pgwNetAddr.nmb, mBuf);
   CMCHKUNPK(cmUnpkCmInetNetAddr, &param->pgwNetAddr.startAddr, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkAvPgwSgwCfg
(
AvPgwSgwCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwSgwCfg(param, mBuf)
AvPgwSgwCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvPgwSgwCfg)

   CMCHKPK(cmPkCmTptAddr, &param->tptAddr, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvPgwSgwCfg
(
AvPgwSgwCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwSgwCfg(param, mBuf)
AvPgwSgwCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvPgwSgwCfg)

   CMCHKUNPK(cmUnpkCmTptAddr, &param->tptAddr, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkAvPgwPeerCfg
(
AvPgwPeerCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwPeerCfg(param, mBuf)
AvPgwPeerCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvPgwPeerCfg)

   CMCHKPK(cmPkAvPgwSgwCfg, &param->sgwCfg, mBuf);
   CMCHKPK(cmPkAvPgwExGwCfg, &param->exGwCfg, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvPgwPeerCfg
(
AvPgwPeerCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwPeerCfg(param, mBuf)
AvPgwPeerCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvPgwPeerCfg)

   CMCHKUNPK(cmUnpkAvPgwExGwCfg, &param->exGwCfg, mBuf);
   CMCHKUNPK(cmUnpkAvPgwSgwCfg, &param->sgwCfg, mBuf);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkAvPgwLSapCfg
(
AvPgwLSapCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwLSapCfg(param, mBuf)
AvPgwLSapCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvPgwLSapCfg)

   CMCHKPK(SPkU32, param->nmbConn, mBuf);
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
PRIVATE S16 cmUnpkAvPgwLSapCfg
(
AvPgwLSapCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwLSapCfg(param, mBuf)
AvPgwLSapCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvPgwLSapCfg)

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
   CMCHKUNPK(SUnpkU32, &param->nmbConn, mBuf);


   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkAvPgwGenCfg
(
AvPgwGenCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwGenCfg(param, mBuf)
AvPgwGenCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvPgwGenCfg)

   CMCHKPK(cmPkAvPgwAddrCfg, &param->pgwAddrCfg, mBuf);
   CMCHKPK(SPkU16, param->maxNmbUes, mBuf);
   CMCHKPK(SPkU16, param->maxNmbBearers, mBuf);
   CMCHKPK(SPkU16, param->maxNmbTunls, mBuf);
   CMCHKPK(SPkU16, param->maxNmbEgtSaps, mBuf);
   CMCHKPK(SPkU16, param->tmrRes, mBuf);
   CMCHKPK(cmPkMemoryId, &param->mem, mBuf);
   CMCHKPK(cmPkPst, &param->lmPst, mBuf);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkAvPgwGenCfg
(
AvPgwGenCfg *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwGenCfg(param, mBuf)
AvPgwGenCfg *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvPgwGenCfg)

   CMCHKUNPK(cmUnpkPst, &param->lmPst, mBuf);
   CMCHKUNPK(cmUnpkMemoryId, &param->mem, mBuf);
   CMCHKUNPK(SUnpkU16, &param->tmrRes, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbEgtSaps, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbTunls, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbBearers, mBuf);
   CMCHKUNPK(SUnpkU16, &param->maxNmbUes, mBuf);
   CMCHKUNPK(cmUnpkAvPgwAddrCfg, &param->pgwAddrCfg, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkAvCfgPgw
(
AvCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvCfgPgw(param, elmnt, mBuf)
AvCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvCfgPgw)

   switch(elmnt)
   {
      case STAVGEN:
         CMCHKPK(cmPkAvPgwGenCfg, &param->s.pgwGenCfg, mBuf);
         break;
      case STAVPEER:
         CMCHKPK(cmPkAvPgwPeerCfg, &param->s.peerCfg, mBuf);
         break;
      case STAVEGTPCSAP:
      case STAVEGTPUSAP:
      case STAVECHOSAP:
         CMCHKPK(cmPkAvPgwLSapCfg, &param->s.pgwLSapCfg, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkAvCfg
(
AvCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvCfg(param, elmnt, mBuf)
AvCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkAvCfg)

   ret1 = cmPkAvCfgPgw(param, elmnt, mBuf);
   RETVALUE(ret1);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvCfgPgw
(
AvCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvCfgPgw(param, elmnt, mBuf)
AvCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvCfgPgw)

   switch(elmnt)
   {
      case STAVGEN:
         CMCHKUNPK(cmUnpkAvPgwGenCfg, &param->s.pgwGenCfg, mBuf);
         break;
      case STAVPEER:
         CMCHKUNPK(cmUnpkAvPgwPeerCfg, &param->s.peerCfg, mBuf);
         break;
      case STAVEGTPCSAP:
      case STAVEGTPUSAP:
      case STAVECHOSAP:
         CMCHKUNPK(cmUnpkAvPgwLSapCfg, &param->s.pgwLSapCfg, mBuf);
         break;
      default :
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvCfg
(
AvCfg *param,
Elmnt elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvCfg(param, elmnt, mBuf)
AvCfg *param;
Elmnt elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkAvCfg)

   ret1 = cmUnpkAvCfgPgw(param, elmnt, mBuf);
   RETVALUE(ret1);
}

#ifdef DEBUGP

#ifdef ANSI
PRIVATE S16 cmPkAvDbgCntrl
(
AvDbgCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvDbgCntrl(param, mBuf)
AvDbgCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvDbgCntrl)

   CMCHKPK(SPkU32, param->dbgMask, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkAvDbgCntrl
(
AvDbgCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvDbgCntrl(param, mBuf)
AvDbgCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvDbgCntrl)

   CMCHKUNPK(SUnpkU32, &param->dbgMask, mBuf);
   RETVALUE(ROK);
}
#endif

#ifdef ANSI
PRIVATE S16 cmPkAvSapCntrl
(
AvSapCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvSapCntrl(param, mBuf)
AvSapCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvSapCntrl)

   CMCHKPK(SPkS16, param->id, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkAvSapCntrl
(
AvSapCntrl *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvSapCntrl(param, mBuf)
AvSapCntrl *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvSapCntrl)

   CMCHKUNPK(SUnpkS16, &param->id, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkAvPgwCntrl
(
AvPgwCntrl *param,
Elmnt  elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvPgwCntrl(param, elmnt, mBuf)
AvPgwCntrl *param;
Elmnt  elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvPgwCntrl)

   CMCHKPK(cmPkAvSapCntrl, &param->u.sap, mBuf);
   CMCHKPK(SPkU8, param->subAction, mBuf);
   CMCHKPK(SPkU8, param->action, mBuf);
   CMCHKPK(cmPkDateTime, &param->dt, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkAvPgwCntrl
(
AvPgwCntrl *param,
Elmnt  elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvPgwCntrl(param, elmnt, mBuf)
AvPgwCntrl *param;
Elmnt  elmnt;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvPgwCntrl)

   CMCHKUNPK(cmUnpkDateTime, &param->dt, mBuf);
   CMCHKUNPK(SUnpkU8, &param->action, mBuf);
   CMCHKUNPK(SUnpkU8, &param->subAction, mBuf);
   CMCHKUNPK(cmUnpkAvSapCntrl, &param->u.sap, mBuf);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkAvCntrl
(
AvCntrl *param,
Elmnt   elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvCntrl(param, elmnt, mBuf)
AvCntrl *param;
Elmnt   elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkAvCntrl)

   ret1 = cmPkAvPgwCntrl(&param->u.pgwCntrl, elmnt, mBuf);
   if(ret1 != ROK)
      RETVALUE(RFAILED);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmUnpkAvCntrl
(
AvCntrl *param,
Elmnt   elmnt,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvCntrl(param, elmnt, mBuf)
AvCntrl *param;
Elmnt   elmnt;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkAvCntrl)

   ret1 = cmUnpkAvPgwCntrl(&param->u.pgwCntrl, elmnt, mBuf);
   if(ret1 != ROK)
      RETVALUE(ret1);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmPkAvUstaDgn
(
AvUstaDgn *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvUstaDgn(param, mBuf)
AvUstaDgn *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkAvUstaDgn)

   CMCHKPK(cmPkCmAlarm, &param->alarm, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvUstaDgn
(
AvUstaDgn *param,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvUstaDgn(param, mBuf)
AvUstaDgn *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkAvUstaDgn)

   CMCHKUNPK(cmUnpkCmAlarm, &param->alarm, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 cmPkAvMngmt
(
AvMngmt *param,
S16 eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmPkAvMngmt(param, S16 eventType, mBuf)
AvMngmt *param;
S16 eventType;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmPkAvMngmt)

   switch(eventType)
   {
      case EVTLAVSTAIND:
         CMCHKPK(cmPkAvUstaDgn, &param->u.usta, mBuf);
         break;
      case EVTLAVCNTRLREQ:
         ret1 = cmPkAvCntrl(&param->u.cntrl, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLAVCFGREQ:
         ret1 = cmPkAvCfg(&param->u.cfg,
               param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLAVSTACFM:
      case EVTLAVCNTRLCFM:
      case EVTLAVSTSCFM:
      case EVTLAVCFGCFM:
         CMCHKPK(cmPkCmStatus, &param->cfm, mBuf);
         break;
      case EVTLAVTRCIND:
      case EVTLAVSTSREQ:
      case EVTLAVSTAREQ:
      default :
         break;
   }
   if(ret1 != ROK)
      RETVALUE(ret1);

   CMCHKPK(cmPkHeader, &param->hdr, mBuf);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 cmUnpkAvMngmt
(
AvMngmt *param,
S16 eventType,
Buffer *mBuf
)
#else
PRIVATE S16 cmUnpkAvMngmt(param, mBuf)
AvMngmt *param;
S16 eventType;
Buffer *mBuf;
#endif
{
   S16 ret1 = ROK;
   TRC3(cmUnpkAvMngmt)

   CMCHKUNPK(cmUnpkHeader, &param->hdr, mBuf);

   switch(eventType)
   {
      case EVTLAVCFGREQ:
         ret1 = cmUnpkAvCfg(&param->u.cfg, 
               param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLAVCNTRLREQ:
         ret1 = cmUnpkAvCntrl(&param->u.cntrl, param->hdr.elmId.elmnt, mBuf);
         break;
      case EVTLAVSTAIND:
         ret1 = cmUnpkAvUstaDgn(&param->u.usta, mBuf);
         break;
      case EVTLAVCFGCFM:
      case EVTLAVSTSCFM:
      case EVTLAVCNTRLCFM:
      case EVTLAVSTACFM:
         CMCHKUNPK(cmUnpkCmStatus, &param->cfm, mBuf);
         break;
      case EVTLAVSTSREQ:
      case EVTLAVSTAREQ:
      case EVTLAVTRCIND:
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
  
         End of file:     lav.c@@/main/1 - Tue Aug 17 18:55:16 2010
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
/main/1      ---      ve               1. LTE-CNE Initial Release.
*********************************************************************91*/
