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



/************************************************************************

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for timer functions

     File:     sq_tmr.c

     Sid:      sq_tmr.c@@/main/nodal_main/3 - Fri Nov 22 18:01:34 2013

     Prg:      sn

************************************************************************/
/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "cm_pasn.h"

#include "sct.h"           /* SCTP Layer */
#include "szt.h"           /* S1AP Upper Interface */
#include "szt_asn.h"
#ifdef SZ_FTHA
#include "cm_psfft.h"
#include "cm_ftha.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#include "lyt.h"
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP LM Interface */
#include "sz.h"            /* S1AP Layer */
#ifdef YT
#include "yt.h"            /* S1AP-PSF Layer */
#endif /* YT */
#include "sz_err.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_pasn.x"
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_pftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif /* SZ_FTHA */

#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCTP Layer */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "sht.x"           /* System Agent */
#include "lyt.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP LM Interface */
#ifdef YT
#include "yt.x"
#endif /* YT */
#include "sz.x"            /* S1AP Layer */

/* local function definition */

PRIVATE Void szBndTmrExpiry ARGS((PTR cb));

/* public variable declarations */

/* control variables for testing */

/* private variable declarations */

/*
 *
 *       Fun:   szActvTmr
 *
 *       Desc:  Invoked by system services to activate a task with
 *              a timer tick.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  sz_tmr.c
 *
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 szActvTmr
(
ProcId proc,
Ent    ent,
Inst   inst
)
#else /* ANSI */

PUBLIC S16 szActvTmr(proc, ent, inst)
ProcId proc;
Ent    ent;
Inst   inst;
#endif /* ANSI */
#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI
PUBLIC S16 szActvTmr
(
void
)
#else /* ANSI */
PUBLIC S16 szActvTmr()
#endif /* ANSI */
#endif /* SS_MULTIPLE_PROCS */
{
#ifdef SS_MULTIPLE_PROCS
   S16 ret;
#endif /*SS_MULTIPLE_PROCS */
   TRC3(szActvTmr);
#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(proc, ent, inst, (Void **) &szCbPtr);
#if (ERRCLASS & ERRCLS_DEBUG)
   if ((ret !=ROK)||
       (szCbPtr == NULLP))
   {
      SLOGERROR(ent, inst, proc, __FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ170,(ErrVal)0,
                "szActvTmr() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         proc, ent, inst));
#endif  /* DEBUGP */
#endif /* ERRCLASS & ERRCLS_DEBUG */
#endif /* SS_MULTIPLE_PROCS */

   cmPrcTmr(&szCb.szTqCp, szCb.szTq, (PFV) szTmrEvnt);

   RETVALUE(ROK);

} /* end of szActvTmr */

/*
 *
 *       Fun:   szRegInitTmr
 *
 *       Desc:  Register and initialize timer queue structures in 
 *              S1AP during general config.
 *              
 *       Ret:   ROK - ok; 
 *              RFAILED - failed;
 *
 *       Notes: none
 *
 *       File:  sz_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 szRegInitTmr
(
Void
)
#else /* ANSI */
PUBLIC S16 szRegInitTmr()
#endif /* ANSI */
{
   U16 idx;
   S16 ret;

   TRC2(szRegInitTmr);

   SZDBGP(LSZ_DBGMASK_TMR, (szCb.init.prntBuf, "szRegInitTmr: tmrRes(%d) \n",
         szCb.genCfg.timeRes));

   /* register general timer here */
#ifdef SS_MULTIPLE_PROCS
   ret = SRegTmr(szCb.init.proc,szCb.init.ent, szCb.init.inst,
         szCb.genCfg.timeRes, (PAIFTMRS16) szActvTmr); 
#else /* SS_MULTIPLE_PROCS */
   ret = SRegTmr(szCb.init.ent, szCb.init.inst, szCb.genCfg.timeRes, szActvTmr);
#endif /* SS_MULTIPLE_PROCS */

#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      SZLOGERROR(ERRCLS_DEBUG,ESZ171,(ErrVal) 0,
                 "szRegInitTmr:  szActvTmr failed");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */

   /* initialize timing queues */
   szCb.szTqCp.tmrLen      = SZ_TQSIZE;

   for (idx = 0; idx < SZ_TQSIZE; idx++)
   {
      szCb.szTq[idx].first    = NULLP;
      szCb.szTq[idx].tail     = NULLP;
   }

   RETVALUE(ROK);

} /* szRegInitTmr */


/*
*
*       Fun:    szStartTmr
*
*       Desc:   start a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   sz_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void szStartTmr
(
CmTimer            *tmr,                /* Common Timer Structure */
PTR                 cbPtr,                 /* Parent Control Block */
U8                  event,              /* Event */
TmrCfg             *wait                /* Timeout Waiting Period */
)
#else /* ANSI */
PUBLIC Void szStartTmr (tmr, cbPtr, event, wait)
CmTimer            *tmr;                /* Common Timer Structure */
PTR                 cbPtr;                 /* Parent Control Block */
U8                  event;              /* Event */
TmrCfg             *wait;               /* Timeout Waiting Period */
#endif /* ANSI */
{
   CmTmrArg         arg;                /* Timer Arguments to be passed */

   TRC3(szStartTmr)
#ifdef DEBUGP
      SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
               "szStartTmr(event(%d), wait(%d))\n", event, wait->val));
#endif /* DEBUGP */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tmr->tmrEvnt != TMR_NONE)
   {
      /* Timer is currently in use - reset */
      (Void) szStopTmr(tmr);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   arg.tq      = szCb.szTq;
   arg.tqCp    = &szCb.szTqCp;
   arg.timers  = tmr;
   arg.cb      = cbPtr;
   arg.evnt    = (U8)event;
   arg.wait    = wait->val;
   arg.tNum    = NOTUSED;
   arg.max     = 1;
   cmPlcCbTq(&arg);

   /* Send the runtime update to peer to start the timer */
#ifdef YT
   if (event == SZ_TMR_WAIT_BNDCFM)
   {
      YT_RTUPD_MOD(YT_UPD_SCT_TMR_CB, (SzSctSapCb *)cbPtr);
   }
#endif /* YT */

   RETVOID;
} /* end of szStartTmr() */


/*
*
*       Fun:    szStopTmr
*
*       Desc:   stop a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   sz_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void szStopTmr
(
CmTimer            *tmr                 /* Common Timer Structure */
)
#else /* ANSI */
PUBLIC Void szStopTmr (tmr)
CmTimer            *tmr;                /* Common Timer Structure */
#endif /* ANSI */
{
   CmTmrArg         arg;                /* Timer Arguments to be passed */

   TRC3(szStopTmr)

#ifdef DEBUGP
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
          "szStopTmr(event(%d))\n", tmr->tmrEvnt));
#endif /* DEBUGP */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tmr->tmrEvnt != TMR_NONE)
   {
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      /* Send the runtime update to peer to stop the timer */
#ifdef YT
      if (tmr->tmrEvnt == SZ_TMR_WAIT_BNDCFM)
      {
         YT_RTUPD_MOD(YT_UPD_SCT_TMR_CB, (SzSctSapCb *)tmr->cb);
      }
#endif /* YT */
      arg.tq      = szCb.szTq;
      arg.tqCp    = &szCb.szTqCp;
      arg.timers  = tmr;
      arg.cb      = (PTR)NULLP;
      arg.evnt    = NOTUSED;
      arg.wait    = NOTUSED;
      arg.tNum    = 0;
      arg.max     = 1;
      cmRmvCbTq(&arg);
#if (ERRCLASS & ERRCLS_INT_PAR)
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   RETVOID;
} /* end of szStopTmr() */

/*
 *
 *       Fun:   szTmrEvnt
 *
 *       Desc:  Process the S1AP timer event. 
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  sz_tmr.c
 *
 */
#ifdef ANSI
PUBLIC Void szTmrEvnt
(
PTR       cb,
S16       event
)
#else /* ANSI */
PUBLIC Void szTmrEvnt(cb, event)
PTR       cb;
S16       event;
#endif /* ANSI */
{
   TRC2(szTmrEvnt)
#ifdef DEBUGP
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,"szTmrEvnt(event(%d))\n", event));
#endif  /* DEBUGP */

   switch (event)
   {
      case SZ_TMR_WAIT_BNDCFM:
      {
         szBndTmrExpiry(cb);
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SZLOGERROR_DEBUG(ESZ172, (ErrVal) event,
               "szTmrEvnt(): timer event for an unknown timer\n");
#endif /* ERRCLASS & ERRCLS_DEBUG */
         break;
      }
   }
   RETVOID;
} /* end of szTmrEvnt */

/*
 *
 *       Fun:   szBndTmrExpiry
 *
 *       Desc:  Process the S1AP bind timer expiry. If the number of retry is
 *              less than the maximum retry counter, the bind request is sent
 *              again, else an alarm is raised to the layer manager.
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  sz_tmr.c
 *
 */
#ifdef ANSI
PRIVATE Void szBndTmrExpiry
(
PTR       cb
)
#else /* ANSI */
PRIVATE Void szBndTmrExpiry(cb)
PTR       cb;
#endif /* ANSI */
{
   SzSctSapCb *sctSapCb;
   SzUstaDgn dgn;

   TRC2(szBndTmrExpiry);

   /* Ignoe the timer event if it is received at standBy node */
#ifdef YT
   if (YT_IS_ACTV() == FALSE)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,"Standby Node\n"));
#endif /* DEBUGP */
      RETVOID;
   }
#endif /* YT */

   sctSapCb = (SzSctSapCb *) cb;
   if (sctSapCb->state == LSZ_SAP_BINDING)
   {
      if (sctSapCb->retryCnt < sctSapCb->maxBndRetry)
      {
         sctSapCb->retryCnt++;
         szStartTmr(sctSapCb->timers,(PTR)sctSapCb,
                      SZ_TMR_WAIT_BNDCFM,&sctSapCb->tBndTmr);
         (Void)SzLiSctBndReq(&sctSapCb->pst, sctSapCb->suId, sctSapCb->spId);
      }
      else
      {
         sctSapCb->retryCnt = 0;
         sctSapCb->state = LSZ_SAP_CFG;
         dgn.type = LSZ_USTA_DGNVAL_SAPID;
         dgn.u.sapId = sctSapCb->suId;
         szSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                     LCM_CAUSE_UNKNOWN, &dgn);
      }
   }
   RETVOID;
} /* end of szBndTmrExpiry */

/**********************************************************************
 
         End of file:     sq_tmr.c@@/main/nodal_main/3 - Fri Nov 22 18:01:34 2013
 
**********************************************************************/

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
------------ -------- ---- ----------------------------------------------
/main/4      ---      va    1. Updated for S1AP release 4.1
*********************************************************************91*/
