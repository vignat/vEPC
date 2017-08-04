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
  
     Name:     eGTP Timer Module
  
     Type:     C source file
  
     Desc:     Timer Module
  
     File:     eg_tmr.c
  
     Sid:      pg_tmr.c@@/main/3 - Fri Feb  4 18:18:16 2011
  
     Prg:      sn
  
*********************************************************************21*/

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
#include "hit.h"           /* TUCL Layer */
#include "egt.h"           /* EGTP Upper Interface */
#include "leg.h"           /* EGTP LM Interface */
/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"
#include "hw.h"
#endif
#include "eg.h"            /* EGTP Layer */
#include "eg_err.h"
#include "eg_tpt.h"        /* EGTP TPT Module  */
#include "eg_edm.h"        /* EDM Module structures            */

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
#include "hit.x"           /* TUCL Layer */
#include "egt.x"           /* EGTP Upper Interface */
#include "leg.x"           /* EGTP LM Interface */
#include "eg_edm.x"        /* EDM Module structures            */
/* eg004.201 Header files added for eGTP-C PSF*/
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "eg.x"            /* EGTP Layer */
#include "eg_tpt.x"        /* EGTP TPT Module  */

#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

/***************************************************************************
********************************* Local function definitons ****************
*****************************************************************************/
PRIVATE Void egBndTmrExpiry ARGS((EgTSapCb       *tSapCb));
PRIVATE Void egTptOpenSrvTmrExpiry ARGS((EgTptSrvCb    *serverCb));

/* eg012.201: Enhanced the piggy backing feature by adding the Timer. */
#ifdef EGTP_C
#ifdef EG_PIGGYBACK_SUPP
PRIVATE Void egSigRspTmrExpiry ARGS ((EgTECCb *teCCb)); 
#endif
#endif

#ifdef EGTP_U
PRIVATE Void egReordTmrExpiry ARGS((EgTeIdCb *egTeidCb));
#ifdef EGTP_U_MULTI_THREADED
PRIVATE S16 egtnlDelTmrExpiry ARGS((EgUEvnt     *eguEvntMsg));
#endif /* EGTP_U_MULTI_THREADED */
#endif /* End of EGTP_U */


#ifdef DEBUGP
PRIVATE S8* egTmrGetEvntStr ARGS((S16    tmrEvnt));
#endif 


/*
 *
 *       Fun:   Activate Task - timers
 *
 *       Desc:  Invoked by system services to activate a task with
 *              a timer tick.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 *
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 egActvTmr
(
ProcId proc,
Ent    ent,
Inst   inst
)
#else

PUBLIC S16 egActvTmr(proc, ent, inst)
ProcId proc;
Ent    ent;
Inst   inst;
#endif
#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI
PUBLIC S16 egActvTmr
(
void
)
#else
PUBLIC S16 egActvTmr()
#endif
#endif /* SS_MULTIPLE_PROCS */
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(egActvTmr);
#ifdef SS_MULTIPLE_PROCS
   if ((SGetXxCb(proc, ent, egCb.init.inst, (Void **) &egCbPtr) !=ROK))
   {
      SLogError(ent, egCb.init.inst, proc,(Txt *) __FILE__, __LINE__,\
                ERRCLS_DEBUG,EEG325,(ErrVal)0,
                (Txt *)"egActvTmr() failed, cannot derive egCb");
      RETVALUE(RFAILED);
   }
#ifdef NOT_USED
   EGDBGP(DBGMASK_UI,(egCb.init.prntBuf,
         "--------EGTP-----(proc(%d),ent(%d),inst(%d))-------\n",
         proc, ent, egCb.init.inst));
#endif

#endif /* SS_MULTIPLE_PROCS */

   cmPrcTmr(&egCb.egTqCp, egCb.egTq, (PFV) egTmrEvnt);

   RETVALUE(ROK);

} /* end of egActvTmr */

#ifdef EGTP_U_MULTI_THREADED

/*
 *
 *       Fun:   Activate Task - timers
 *
 *       Desc:  Invoked by system services to activate a reordering timer task with
 *              a timer tick.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 egActvThrdTmr
(
ProcId proc,
Ent    ent,
Inst   inst
)
#else

PUBLIC S16 egActvThrdTmr(proc, ent, inst)
ProcId proc;
Ent    ent;
Inst   inst;
#endif
#else /* SS_MULTIPLE_PROCS */
#ifdef SS_MT_TMR
#ifdef ANSI
PUBLIC S16 egActvThrdTmr
(
Ent    ent,
Inst   inst
)
#else
PUBLIC S16 egActvThrdTmr(ent,inst)
Ent    ent;
Inst   inst;
#endif
#else
#ifdef ANSI
PUBLIC S16 egActvThrdTmr
(
)
#else
PUBLIC S16 egActvThrdTmr()
#endif
#endif /* SS_MT_TMR */
#endif /* SS_MULTIPLE_PROCS */
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(egActvThrdTmr);

#ifdef SS_MT_TMR
   cmPrcTmr(&(egCb.egUCb.egThrdsInfo.egThrdCbList[inst-1].thrdTqCp), egCb.egUCb.egThrdsInfo.egThrdCbList[inst-1].thrdTq, (PFV) egthrTmrEvnt);
#else
   EGLOGERROR(ERRCLS_DEBUG,EEG326,(ErrVal) 0,
      "egActvThrdTmr:  Enable SS_MT_TMR flag");
#endif

   RETVALUE(ROK);

} /* end of egActvThrdTmr */
#endif /* EGTP_U_MULTI_THREADED */
/*
 *
 *       Fun:   egRegInitTmr
 *
 *       Desc:  Register and initialize timer queue structures in 
 *              EGTP during general config.
 *              
 *       Ret:   ROK - ok; 
 *              RFAILED - failed;
 *
 *       Notes: none
 *
 *       File:  eg_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 egRegInitTmr
(
Void
)
#else
PUBLIC S16 egRegInitTmr()
#endif
{
   U16 idx;
   S16 ret;

   EG_TRC2(egRegInitTmr);

   EG_DBG_INFO (0,0, (egCb.init.prntBuf, "Timer Resolution (%d) \n",egCb.genCfg.timerRes ));

#ifndef EGTP_U_MULTI_THREADED
   /* register general timer here */
#ifdef SS_MULTIPLE_PROCS
   ret = SRegTmr(egCb.init.proc,egCb.init.ent, egCb.init.inst, egCb.genCfg.timerRes, (PAIFTMRS16) egActvTmr);
#else
   ret = SRegTmr(egCb.init.ent, egCb.init.inst, egCb.genCfg.timerRes, egActvTmr);
#endif
#else
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
   ret = SRegTmrMt(egCb.init.ent, egCb.init.inst, egCb.genCfg.timerRes, (PAIFTMRS16)egActvTmr);
#else
EGLOGERROR(ERRCLS_DEBUG,EEG327,(ErrVal) 0,
               "EgInitThrds:  Enable SS_MT_TMR flag");
   RETVALUE(RFAILED);
#endif 
#else
   ret = SRegTmr(egCb.init.procId, ENTEG, egCb.init.inst, egCb.genCfg.timerRes, (PAIFTMRS16)egActvTmr);
#endif /* SS_MULTIPLE_PROCS */
#endif /* EGTP_U_MULTI_THREADED */

   if (ret != ROK)
   {
      EGLOGERROR(ERRCLS_DEBUG,EEG328,(ErrVal) 0,
                 "egRegInitTmr:  egActvTmr failed");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   egCb.egTqCp.tmrLen      = EG_TQNUMENT;

   for (idx = 0; idx < EG_TQSIZE; idx++)
   {
      egCb.egTq[idx].first    = NULLP;
      egCb.egTq[idx].tail     = NULLP;
   }

   EG_RETVALUE(ROK);

} /* egRegInitTmr */

#ifdef EGTP_U_MULTI_THREADED
/* 
 * ***  FUNCTION  **********************************************************************
 *
 *         Name:  egRegThrdTmr                               
 *
 *  Description:  Register and initialise timer queue structures of reordering timer
 *
 *       Return: ROK/RFAILED
 *
 * *************************************************************************************
 */
#ifdef ANSI
PUBLIC S16 egRegThrdTmr
(
EgUThreadCb *egThreadCb,
U8          thrCnt
)
#else
PUBLIC S16 egRegThrdTmr(egThreadCb,thrCnt)
EgUThreadCb *egThreadCb;
U8          thrCnt;
#endif
{

        U16        idx = 0;
  S16        ret = ROK;
        EG_TRC2(egRegThrdTmr);
        /* initialize timing queues */
        egThreadCb->thrdTqCp.tmrLen      = EG_TQNUMENT;

        for (idx = 0; idx < EG_TQSIZE; idx++)
        {
                egThreadCb->thrdTq[idx].first    = NULLP;
                egThreadCb->thrdTq[idx].tail     = NULLP;
        }
#ifndef SS_MULTIPLE_PROCS
#ifdef SS_MT_TMR
          ret = SRegTmrMt(ENTEG, thrCnt, egCb.genCfg.timerRes, (PAIFTMRS16)egActvThrdTmr);
#else
       EGLOGERROR(ERRCLS_DEBUG,EEG329,(ErrVal) 0,
               "EgInitThrds:  Enable SS_MT_TMR flag");
          RETVALUE(RFAILED);
#endif 
#else
          ret = SRegTmr(egCb.init.procId, ENTEG, thrCnt, egCb.genCfg.timerRes, (PAIFTMRS16)egActvThrdTmr);
#endif /* SS_MULTIPLE_PROCS */
         if (ret != ROK)
          {
                EGLOGERROR(ERRCLS_DEBUG,EEG330,(ErrVal) 0,
                 "EgInitThrds:  egRegThrdTmr failed");
                RETVALUE(RFAILED);
          }

          EG_RETVALUE(ret);
}
#endif /* EGTP_U_MULTI_THREADED*/


/*****************************************************************************
*
*       Fun:   egCmInitTimer
*
*       Desc:  Initialize the timer control blocks
*
*       Ret:   EG_RETVOID
*
*       Notes: None
*
*       File:  eg_tmr.c
*
*****************************************************************************/

#ifdef ANSI
PUBLIC Void egCmInitTimer
(
EgTimer      *tmr         /* timer control block array */
)
#else
PUBLIC Void egCmInitTimer (tmr)
EgTimer      *tmr;         /* timer control block array */
#endif
{
   EG_TRC2(egCmInitTimer)

   cmInitTimers ((CmTimer *)&tmr->tmr, 1);

   tmr->egCbPtr    = 0;

   EG_RETVOID;
} /* egCmInitTimer */


/**************************************************************************
*
*       Fun:   egSchedTmr
*
*       Desc:  Schedule/Deschedule a timer
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: tmrVal is in units of 500ms
*
*       File:  eg_tmr.c
*
****************************************************************************/
#ifdef ANSI
PUBLIC S16 egSchedTmr
(
Ptr          egHandle,     /* Control block */
S16          tmrEvnt,      /* timer event */
Action       action,       /* action -- start/stop/restart */
U32          tmrVal        /* timer value */
)
#else
PUBLIC S16 egSchedTmr (egHandle, tmrEvnt, action, tmrVal)
Ptr          egHandle;     /* Control block */
S16          tmrEvnt;      /* timer event */
Action       action;       /* action -- start/stop/restart */
U32          tmrVal;       /* timer value */
#endif
{
   CmTimer   *tmr;         /* timers array */
   CmTmrArg  arg;          /* timer arguments */
   EgTimer   *egTmr = NULLP;       /* Actual EgTimer inside control block */

   EG_TRC2(egSchedTmr);

#ifdef ALIGN_64BIT
   EG_DBG_PARAM(0, 0,(egp, "Timer Event: %s, Action %d, Value %d",
         egTmrGetEvntStr(tmrEvnt), action, tmrVal) );
#else
   EG_DBG_PARAM(0, 0,(egp, "Timer Event: %s, Action %d, Value %ld",
         egTmrGetEvntStr(tmrEvnt), action, tmrVal) );
#endif

   if ((action != TMR_STOP) && (tmrVal == 0))
   {
      RETVALUE(RFAILED);
   }

   /*
    * None of the bits from EGTMR_EVNT_IDXOFFSET to
    * 16 (maximum number of bits in tmrEvnt) should be
    * set in tmrEvnt
    */
   if (tmrEvnt & EGTMR_EVNT_IDXMASK)
   {
      RETVALUE(RFAILED);
   }

   switch (tmrEvnt)
   {
      /*------------- Transport Module Timers ------------*/
      case EG_TMR_TPT_BND:
 {
    egTmr = &((EgTSapCb *)egHandle)->bndTmr;
    egTmr->egCbPtr  = (PTR)egHandle;
    tmr = &egTmr->tmr;
    break;
 } /* End of case EG_TMR_TPT_BND */
      case EG_TMR_TPT_OPEN_SRV: /*-- Open Server Timer --*/
 {
    egTmr = &((EgTptSrvCb *)egHandle)->opnSrvTmrNode;
    egTmr->egCbPtr  = (PTR)egHandle;
    tmr = &egTmr->tmr;
    break;
 } /* End of case EG_TMR_TPT_OPEN_SRV */
#ifdef EGTP_C
      case EG_TMR_TPT_MSG_RETX: /*-- Re-Trans Timer --*/
 {
    egTmr = &((EgTptReTmrCb *)egHandle)->reTransTmrNode;
    egTmr->egCbPtr  = (PTR)egHandle;
    tmr = &egTmr->tmr;
    break;
 } /*End of case EG_TMR_TPT_MSG_RETX */
#endif /* EGTP_C */
#ifdef EGTP_C_PTH_MGT
      case EG_TMR_PATH_GLOBAL:
      {
         tmr = &(egCb.pathTimers);
         break;
      }
#endif
#ifdef EGTP_U
      case EG_TMR_ECHO_GLOBAL: /*-- iEcho req transmit Timer --*/
 {
    tmr = &(egCb.egUCb.echoTimers);
    break;
 } /*End of case EG_TMR_TPT_MSG_RETX */
      case EG_TMR_N3T3_REQ: /*-- N3T3 retransmossoin timer --*/
 {
    tmr = &(egCb.egUCb.n3t3Timers);
    break;
 } /*End of case EG_TMR_TPT_MSG_RETX */

#ifndef EGTP_U_MULTI_THREADED
      case EG_TMR_REORD_EXP:
    {
       egTmr = &((EgTeIdCb *)egHandle)->reOrderTimer;
       egTmr->egCbPtr  = (PTR)egHandle;
       tmr = &egTmr->tmr;
       break;
    } /* End of case EG_TMR_REORD_EXP */
#endif /* End of EGTP_U_MULTI_THREADED */
#endif /* End of EGTP_U */

#ifdef EGTP_U_MULTI_THREADED
      case EG_TMR_LOAD_DISTRIBUTE: /*-- Load-distribution Timer --*/
 {
    tmr = (CmTimer*)&(egCb.egUCb.egThrdsInfo.ldDistTmr);
    break;
 } /* end of case EG_TMR_LOAD_DISTRIBUTE */
#endif /* EGTP_U_MULTI_THREADED */
#ifdef EGTP_C
/* eg012.201: Enhanced the piggy backing feature by adding the Timer*/
#ifdef EG_PIGGYBACK_SUPP
      case EG_TMR_PIGGY_RSP:
         {
            egTmr = &((EgTECCb *)egHandle)->tmrDisPbSigRsp;
            egTmr->egCbPtr = (PTR)egHandle;
            tmr = &egTmr->tmr;
            break;
         }
#endif
#endif
      default:
 RETVALUE(RFAILED);
 break;
   }


   if (action == TMR_STOP)
   {
      /* return ROK if timer has already been stopped.*/     
      if (tmr->tmrEvnt == TMR_NONE)
         RETVALUE(ROK);

      if (tmrEvnt == (tmr->tmrEvnt & ~EGTMR_EVNT_IDXMASK))
      {
         arg.tqCp   = &egCb.egTqCp;
         arg.tq     = (CmTqType *) &egCb.egTq; 

         arg.timers = tmr;
         arg.cb     = (PTR)egTmr;
         arg.evnt   = NOTUSED;
         arg.wait   = NOTUSED;
         arg.tNum   = 0;
         arg.max    = 1;

         cmRmvCbTq (&arg);
         RETVALUE(ROK);
      }
   }
   else if (action == TMR_START)
   {
      if (tmr->tmrEvnt == TMR_NONE)
      {
         arg.tqCp   = &egCb.egTqCp;
         arg.tq     = (CmTqType *) &egCb.egTq;
         arg.timers = tmr;
         arg.cb     = (PTR)egTmr;
         arg.evnt   = tmrEvnt;
         arg.wait   = tmrVal;
         arg.tNum   = NOTUSED;
         arg.max    = 1;

         cmPlcCbTq (&arg);
         RETVALUE (ROK);
      }
      else
      {
         RETVALUE (RFAILED);
      }
   }
#ifdef EG_PHASE2
   else if (action == TMR_RESTART)
   {
      if ((tmrEvnt == (tmr->tmrEvnt & ~EGTMR_EVNT_IDXMASK)) ||
          (tmr->tmrEvnt == TMR_NONE))
      {
         if (tmr->tmrEvnt != TMR_NONE)
         {
            arg.tqCp   = &egCb.egTqCp;
            arg.tq     = (CmTqType *)&egCb.egTq; 
            arg.timers = tmr;
            arg.cb     = (PTR)egTmr;
            arg.evnt   = NOTUSED;
            arg.wait   = NOTUSED;
            arg.tNum   = 0;
            arg.max    = 1;

            cmRmvCbTq (&arg);
         }

         arg.tqCp   = &egCb.egTqCp;
         arg.tq     = (CmTqType *)&egCb.egTq; 

         arg.timers = tmr;
         arg.cb     = (PTR)egTmr;
         arg.evnt   = tmrEvnt;
         arg.wait   = tmrVal;
         arg.tNum   = NOTUSED;
         arg.max    = 1;

         cmPlcCbTq (&arg);
         RETVALUE(ROK);
      }
   }
#endif /* EG_PHASE2 */

   RETVALUE(RFAILED);
} /* end of egSchedTmr */

/*
*
*       Fun:    egRmvTmr
*
*       Desc:   Removes control block from Timing Queue
*
*       Ret:    RETVOID
*
*       Notes:  None
*
*       File:   eg_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void egRmvTmr
(
PTR      cbp,              /* control block pointer       */
CmTimer  *pTimerStruct,    /* pointer to timer structure */
U8       tmrNum,           /* timer number               */
U8       maxNmbTmrs        /* maximum nmb of timers for this control block */
)
#else
PUBLIC Void egRmvTmr(cbp, pTimerStruct, tmrNum, maxNmbTmrs)
PTR      cbp;              /* ontrol block pointer       */
CmTimer  *pTimerStruct;    /* pointer to timer structure */
U8       tmrNum;           /* timer number               */
U8       maxNmbTmrs;       /* maximum nmb of timers for this control block */
#endif
{
   CmTmrArg arg;           /* timer argument structure                       */

   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egRmvTmr)

   arg.tq = egCb.egTq;
   arg.tqCp = &egCb.egTqCp;
   arg.timers = pTimerStruct;
   arg.cb = cbp;
   arg.evnt = pTimerStruct[tmrNum].tmrEvnt;
   arg.wait = 0;
   arg.tNum = tmrNum;
   arg.max = maxNmbTmrs;
   cmRmvCbTq(&arg);

   RETVOID;
}

/*
 *
 *       Fun:   egTmrEvnt
 *
 *       Desc:  Process the EGTP timer event. 
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 *
 */
#ifdef ANSI
PUBLIC Void egTmrEvnt
(
Ptr       cb,
S16       event
)
#else
PUBLIC Void egTmrEvnt(cb, event)
Ptr       cb;
S16       event;
#endif
{
   EgTimer     *egTmr;        /* Timer which expired */

   EG_TRC2(egTmrEvnt);

   egTmr = (EgTimer *)cb;

   /*
      For Default path,  a timer is started. Also, when a request is sent to the 
      peer, re-transmission timer is started. When  either timer expires, we are 
      required to send Echo Request/Retransmission Request. This is only valid 
      for Active Resource set. In order to avoid any errorenous message being sent 
      out of by Standby copy, this check is required.
    */

   /*Process timer event only if resource set is in Active state.*/
   switch (event)
   {
      case EG_TMR_TPT_BND:
      {
         egBndTmrExpiry((EgTSapCb *)egTmr->egCbPtr);
         break;
      }
      case EG_TMR_TPT_OPEN_SRV:
      {
         egTptOpenSrvTmrExpiry((EgTptSrvCb *)egTmr->egCbPtr);

         break;
      }
#ifdef EGTP_C
      case EG_TMR_TPT_MSG_RETX:
      {
         egTptMsgRetxTmrExpiry((EgTptReTmrCb*)egTmr->egCbPtr);

         break;
      }
#endif /* EGTP_C */
#ifdef EGTP_U
#ifndef EGTP_U_MULTI_THREADED
      case EG_TMR_REORD_EXP:
      {
         /* Re-oredring timer expiry */
         egReordTmrExpiry((EgTeIdCb *)egTmr->egCbPtr);
         break;
      }
#endif  /* EGTP_U_MULTI_THREADED */
#ifdef EGTP_U_MULTI_THREADED
      case EG_TMR_LOAD_DISTRIBUTE:
      {
#ifdef SS_MT_CB
         EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

         /* Load-distribution timer expiry */
         egLdDistribute();

#ifdef SS_MT_CB
    EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

         break;
      }
#endif  /* EGTP_U_MULTI_THREADED */
      case EG_TMR_ECHO_GLOBAL:
      {
         /* Echo timer expiry */
         egUSendEchoReq();
         break;
      }
      case EG_TMR_N3T3_REQ:
      {
         /* iN3T3 timer expiry */
         egUHndlT3N3TmrExpiry(); 
         break;
      }
#endif
#ifdef EGTP_C_PTH_MGT
      case EG_TMR_PATH_GLOBAL:
      {
/* eg004.201 Check added Send ECHO Request only from ACTIVE PSF node*/
#ifdef HW
            if(HW_IS_ACTV())
            {
#endif
               egSendEchoReq();
#ifdef HW
            }
            else
            {
               /********************************************
                * Re-Start echo timer at Standby           *
                *******************************************/
               if (egSchedTmr(NULLP, EG_TMR_PATH_GLOBAL, TMR_START,
                        egCb.genCfg.pathTmr.val) != ROK)
               {
                  EG_DBG_ERR(0, 0, (egp,"\n  egSendEchoReq:Starting of Echo Timer Failed"));
               }
            }
#endif
         break;
      }
      break;
#endif

/* eg012.201: Enhanced the piggy backing feature by adding the Timer*/
#ifdef EGTP_C
#ifdef EG_PIGGYBACK_SUPP
      case  EG_TMR_PIGGY_RSP:
            egSigRspTmrExpiry((EgTECCb *)egTmr->egCbPtr);
            break;   
#endif
#endif
      default:
      {
         EG_DBG_ERR(0, 0,(egp, "\nTimer event for an unknown timer\n"));
         break;
         }/*Default*/
   }/*Switch (event)*/

   EG_RETVOID;
} /* end of egTmrEvnt */

#ifdef EGTP_U
#ifdef EGTP_U_MULTI_THREADED
/* 
 * ***  FUNCTION  **********************************************************************
 *
 *         Name:  egthrTmrEvnt                               
 *
 *  Description:  Process the reOdering timer expiry events
 *
 *       Return:  RVOID
 *
 * *************************************************************************************
 */
#ifdef ANSI
PUBLIC Void egthrTmrEvnt
(
Ptr       cb,
S16       event
)
#else
PUBLIC Void egthrTmrEvnt(cb, event)
Ptr       cb;
S16       event;
#endif
{
   EgTimer     *egTmr = NULLP;        /* Timer which expired */

   EG_TRC2(egTmrEvnt);

   egTmr = (EgTimer *)cb;

   EG_DBG_INFO(0,0, (egCb.init.prntBuf,"egthrTmrEvnt(event(%d))\n", event));

   switch (event)
   {
      case EG_TMR_REORD_EXP:
      {
         /* Re-oredring timer expiry */
         egReordTmrExpiry((EgTeIdCb *)egTmr->egCbPtr);
         break;
      }
      case EG_TMR_TUNDEL_EXP:
      {
  /* Tunnel Delete request */
        egtnlDelTmrExpiry((EgUEvnt *)egTmr->egCbPtr);
        break;
      }
      default:
      {
         EG_DBG_ERR(0, 0,(egp, "\nTimer event for an reOrdering timer\n"));
      }
   }

   EG_RETVOID;
} /* end of egthrTmrEvnt*/
/* 
 * ***  FUNCTION  **********************************************************************
 *
 *         Name:  egSchedreOrdTmr                               
 *
 *  Description: This will schedule the reodering timer and tunnel deletion timer
 *
 *       Return: ROK
 *
 * *************************************************************************************
 */

#ifdef ANSI
PUBLIC S16 egSchedreOrdTmr
(
Ptr          egHandle,     /* Control block */
S16          tmrEvnt,      /* timer event */
Action       action,       /* action -- start/stop/restart */
U32          tmrVal        /* timer value */
)
#else
PUBLIC S16 egSchedreOrdTmr (egHandle, tmrEvnt, action, tmrVal)
Ptr          egHandle;     /* Control block */
S16          tmrEvnt;      /* timer event */
Action       action;       /* action -- start/stop/restart */
U32          tmrVal;       /* timer value */
#endif
{
   CmTmrArg  arg;          /* timer arguments */
   EgTimer   *egTmr;
   CmTimer   *tmr;
   EG_TRC2(egSchedreOrdTmr);

#ifdef ALIGN_64BIT
   EG_DBG_PARAM(0, 0,(egp, "Timer Event: %s, Action %d, Value %d",
         egTmrGetEvntStr(tmrEvnt), action, tmrVal) );
#else
   EG_DBG_PARAM(0, 0,(egp, "Timer Event: %s, Action %d, Value %ld",
         egTmrGetEvntStr(tmrEvnt), action, tmrVal) );
#endif
   if (tmrEvnt == EG_TMR_REORD_EXP)
   {
       egTmr = &(((EgTeIdCb *)egHandle)->reOrderTimer);
       arg.tqCp   = (CmTqCp *)&(((EgTeIdCb *)egHandle)->egThreadCb->thrdTqCp);
       arg.tq     = (CmTqType *) (((EgTeIdCb *)egHandle)->egThreadCb->thrdTq);
   }
   else if(tmrEvnt == EG_TMR_TUNDEL_EXP)
   {
       egTmr = &(((EgUEvnt *)egHandle)->teIdCb->tnlDelTimer);
       arg.tqCp   = (CmTqCp *)&(((EgUEvnt *)egHandle)->teIdCb->egThreadCb->thrdTqCp);
       arg.tq     = (CmTqType *) (((EgUEvnt *)egHandle)->teIdCb->egThreadCb->thrdTq);
   }
   else
   {
 EG_DBG_INFO(0, 0,(egp, "\negSchedreOrdTmr:Unknown Timer Event\n"));
       RETVALUE(RFAILED);
   }
   egTmr->egCbPtr  = (PTR)egHandle;
   tmr = &egTmr->tmr;

   if (action == TMR_STOP)
   {
      /* return ROK if timer has already been stopped.*/
      if (tmr->tmrEvnt == TMR_NONE)
         RETVALUE(ROK);

         arg.timers = tmr;
         arg.cb     = (PTR)egTmr;
         arg.evnt   = NOTUSED;
         arg.wait   = NOTUSED;
         arg.tNum   = 0;
         arg.max    = 1;

         cmRmvCbTq (&arg);
         RETVALUE(ROK);
   }
   else if (action == TMR_START)
   {
      if (tmr->tmrEvnt == TMR_NONE)
      {
         arg.timers = tmr;
         arg.cb     = (PTR)egTmr;
         arg.evnt   = tmrEvnt;
         arg.wait   = tmrVal;
         arg.tNum   = NOTUSED;
         arg.max    = 1;

         cmPlcCbTq (&arg);
         RETVALUE (ROK);
      }
      else
      {
         RETVALUE (RFAILED);
      }
   }
   RETVALUE(ROK);
}
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */
 
/*
 *
 *       Fun:   egBndTmrExpiry
 *
 *       Desc:  Process the EGTP bind timer expiry. If the number of retry is
 *              less than the maximum retry counter, the bind request is sent
 *              again, else an alarm is raised to the layer manager.
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 *
 */
#ifdef ANSI
PRIVATE Void egBndTmrExpiry
(
EgTSapCb       *tSapCb
)
#else
PRIVATE Void egBndTmrExpiry(tSapCb)
EgTSapCb       *tSapCb;
#endif
{
   EgUstaDgn dgn;

   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egBndTmrExpiry);

   if (tSapCb->state == LEG_TSAP_WAIT_BNDENA)
   {
      if (tSapCb->bndRetryCnt < tSapCb->reCfg.maxBndRetry)
      {
         tSapCb->bndRetryCnt++;
         egSchedTmr(tSapCb, EG_TMR_TPT_BND, TMR_START,
               tSapCb->reCfg.bndTmCfg.val);

         (Void)EgLiHitBndReq(&tSapCb->pst, tSapCb->suId, tSapCb->spId);
      }
      else
      {
         tSapCb->bndRetryCnt = 0;
         tSapCb->state = LEG_TSAP_UBNDDIS;
         cmMemset( (U8* )&dgn, 0, sizeof(EgUstaDgn));
         dgn.dgnVal[0].type = LEG_PAR_TSAP;
         dgn.dgnVal[0].t.sapId = tSapCb->suId;
         egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                     LCM_CAUSE_UNKNOWN, &dgn);
      }
   }
   RETVOID;
} /* end of egBndTmrExpiry */

/* eg012.201: Enhanced the piggy backing feature by adding the Timer. */
#ifdef EGTP_C
#ifdef EG_PIGGYBACK_SUPP
/*
 *
 *       Fun:   egSigRspTmrExpiry
 *
 *       Desc:  Delete the buffered response message and send error indication to application,when did nor receive the associated 
                piggibacked request message and the timer EG_TMR_PIGGY_RSP expires.
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 *
 */

#ifdef ANSI
PRIVATE Void egSigRspTmrExpiry
(
  EgTECCb *teCCb /* tunnel control block*/
)
#else
PRIVATE Void egSigRspTmrExpiry(teCCb)
EgTECCb *teCCb; /* tunnel control block */
#endif
{
   EgErrEvnt errEvnt;
   EgUSapCb *usap;

   EG_TRC2(egSigRspTmrExpiry);

   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"did not receive piggibacked initial message"));

   /*allocation of error event structure*/
   cmMemset( (U8* )&errEvnt, 0, sizeof(EgErrEvnt));

   /* filling the error event structure to send the application  */
   errEvnt.localTeid =  teCCb->tempPiggyEvnt->lclTeid.val; 
   errEvnt.seqNo = teCCb->tempPiggyEvnt->seqNumber;  
   errEvnt.errCause = EGT_ERR_RSP_PIGGY_TMR_EXP;
   errEvnt.failedIeInst =  teCCb->tempPiggyEvnt->failedIeInst;
   errEvnt.failedIeType =  teCCb->tempPiggyEvnt->failedIeType;
   errEvnt.failedMsgType = teCCb->tempPiggyEvnt->msgType;
   egCpyCmTptAddr(&(errEvnt.remAddr), &(teCCb->tempPiggyEvnt->remAddr));
   usap = teCCb->usap;
   errEvnt.intfType = usap->cfg.intfType;

   /* Send the error indication to application */
   EgUiEgtErrInd(&(usap->pst), usap->suId, &errEvnt);
   /*freeing the message buffer */
   EG_FREEMBUF (teCCb->tempPiggyEvnt->u.mBuf);
        /* Reset the timer value to zero */
        teCCb->timeToWaitPbTmr = 0;
   /* freeing the event structure */
   egCmFreeEvent(teCCb->tempPiggyEvnt); 
   teCCb->piggyBackStatus = EG_PIGGYBACK_NONE;

   EG_RETVOID;
}/* end of egSigRspTmrExpiry */
#endif
#endif


/***********************************************************************************
 *
 *       Fun:   egTptOpenSrvTmrExpiry
 *
 *       Desc:  Process the EGTP TPT Open Server timer expiry. 
 *              If the number of retry is
 *              less than the maximum retry counter, the Server Open request is sent
 *              again, else an alarm is raised to the layer manager.
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 *
 ***********************************************************************************/
#ifdef ANSI
PRIVATE Void egTptOpenSrvTmrExpiry
(
  EgTptSrvCb    *serverCb /* Server connection control block */
)
#else
PRIVATE Void egTptOpenSrvTmrExpiry(serverCb)
EgTptSrvCb    *serverCb; /* Server connection control block */
#endif
{

   EG_TRC2(egTptOpenSrvTmrExpiry);


   EG_DBG_INFO(0, 0,(egp, "\n Timer Open Server Expired \n"));

   if (NULLP == serverCb)
   {
      EG_DBG_ERR(0, 0,(egp, "\n Null Timer CB \n"));
      EG_RETVOID;
   }

   if ((serverCb->state == LEG_TPTSRV_WAIT_ENA || 
            serverCb->state == LEG_TPTSRV_DIS))
   {
      /*-- Make the state to disalbe again, as egTptOpenServer
        function will require the same --*/

      serverCb->state = LEG_TPTSRV_DIS;
      /* Retry to open the transport server */
      if (egTptOpenServer(serverCb, NULLP) != ROK)
         EG_RETVOID;
   }
   else if (serverCb->state != LEG_TPTSRV_WAIT_ENA)
   {
      EG_DBG_ERR(0, 0, (egp,"Invalid serverCb->state %d", serverCb->state));
   }

   EG_RETVOID;

}/* egTptOpenSrvTmrExpiry */


#ifdef EGTP_U
/***********************************************************************************
 *
 *       Fun:   egReordTmrExpiry 
 *
 *       Desc:  This function will be called when reodering timer expires 
 *
 *       Ret:   RETVOID
 *
 *       Notes: None
 *
 *       File:  eg_tmr.c
 *
 ***********************************************************************************/

#ifdef ANSI
PRIVATE Void egReordTmrExpiry
(
EgTeIdCb *egTeidCb
)
#else
PRIVATE Void egReordTmrExpiry(egTeidCb)
EgTeIdCb *egTeidCb;
#endif
{
   EG_TRC2(egReordTmrExpiry);

#ifdef SS_MT_CB
    EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
    egFlushReordRcvdGtpuPdus(egTeidCb);
#ifdef SS_MT_CB
    EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

   EG_RETVOID;
}

#endif /* EGTP_U */

#ifdef EGTP_U_MULTI_THREADED
/***********************************************************************************
 *
 *       Fun:   egtnlDelTmrExpiry
 *
 *       Desc:  This function will be called when EG_TMR_TUNDEL_EXP expires
 *
 *       Ret:   RETVOID
 *
 *       Notes: This will be used only the layer is running in Multi Threaded and
 *        reodering is enabled for that tunnel.
 *        When ever we get a control request to Delete a Tunnel
 *        main thread posts that to worker thread where reodering is going on
 *        Worker thread will flushes all the packets and starts this timer 
 *        with one tick so that Deletion of tunnel will be the last event 
 *        for that thread-tunnel pair which will avoid race conditions.
 *
 *       File:  eg_tmr.c
 *
 ***********************************************************************************/

#ifdef ANSI
PRIVATE S16 egtnlDelTmrExpiry
(
EgUEvnt     *eguEvntMsg
)
#else
PRIVATE S16 egtnlDelTmrExpiry(eguEvntMsg)
EgUEvnt     *eguEvntMsg;
#endif
{
   Buffer      *cfmMbuf = NULLP;
   EgUstaDgn   dgn;
S16         retVal;
Pst         pst;                /* post message  */
Pst         *tmpPst = &pst;

   EG_TRC2(egtnlDelTmrExpiry);

   /* send delete tunnel conformation to the main thread */
   retVal = SGetMsg(eguEvntMsg->region, eguEvntMsg->pool, &cfmMbuf);
   if(retVal != ROK)
   {
      /********************************************************
       * Memory allocation failed, send alarm to LM and error *
       *********************************************************/
       EG_DBG_ERR(0, 0,(egp, "\n egtnlDelTmrExpiry(): Allocation of memory for mBuf failed \n"));
       EGU_SND_ALARM_MEM_ALLOC_FAIL(eguEvntMsg->region, eguEvntMsg->pool, dgn);
       EGU_FREEEVNT(eguEvntMsg);
       EG_RETVALUE(RFAILED);
    }/* end of if */

    pst.dstProcId = egCb.init.procId;
    pst.srcProcId = egCb.init.procId;
    pst.srcEnt = egCb.init.ent;
    pst.dstEnt = egCb.init.ent;
    pst.srcInst = eguEvntMsg->teIdCb->egThreadCb->thrId;
    pst.dstInst = egCb.init.inst;
    pst.prior = PRIOR0;
    pst.route = RTESPEC;
    pst.region = eguEvntMsg->region;
    pst.pool = eguEvntMsg->pool;
    pst.selector = 0;

 if(eguEvntMsg->eventType == EGTEVTTNLDELREQ)
 {
    pst.event = EGTEVTEGUTNLCFM;
 }/* End of if () */
 else if(eguEvntMsg->eventType == EGTEVTGENCNTRLREQ)
 {
   pst.event = EGTEVTLEGCNTRLCFM;
 }/* end of else */
 else if(eguEvntMsg->eventType == EGTEVTUSAPUBNDREQ)
 {
   pst.event = EGTEVTUSAPUBNDCFM;
 }/* end of else */
    CMCHKPKLOG(cmPkPtr, (PTR)eguEvntMsg, cfmMbuf, EEG331, tmpPst);
    SPstTsk(&pst, cfmMbuf);
    EG_RETVALUE(ROK);
} /* End of egtnlDelTmrExpiry() */
#endif /* EGTP_U_MULTI_THREADED */

#ifdef DEBUGP
/***********************************************************
*
*      Fun:    egTmrGetEvntStr
*
*      Desc:   Handle the expiration of a particular timer event
*
*       Ret:   RETVOID
*
*       Notes: This function dispatches the timer event expiry
*              to the appropriate module.
*
*       File:  eg_tmr.c
*
***********************************************************/

#ifdef ANSI
PRIVATE S8* egTmrGetEvntStr
(
S16              tmrEvnt      /* timer event */
)
#else
PRIVATE S8* egTmrGetEvntStr(tmrEvnt)
S16              tmrEvnt;     /* timer event */
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC2(egTmrGetEvntStr)
      
   /* If timer is not handled on a module basis, process individually */
   switch (tmrEvnt)
   {
        /*------------- Transport Module Timers ------------*/
      case EG_TMR_TPT_BND:
         RETVALUE((S8 *)"EG_TMR_TPT_BND");

      case EG_TMR_TPT_OPEN_SRV: /*-- Open Server Timer --*/
         RETVALUE((S8 *)"EG_TMR_TPT_OPEN_SRV");

      case EG_TMR_TPT_MSG_RETX: /*-- Re-Trans Timer --*/
         RETVALUE((S8 *)"EG_TMR_TPT_MSG_RETX");

#ifdef EGTP_U
#ifndef EGTP_U_MULTI_THREADED
      case EG_TMR_REORD_EXP:
         RETVALUE((S8 *)"EG_TMR_REORD_EXP");
#endif

#ifdef EGTP_U_MULTI_THREADED
      case EG_TMR_LOAD_DISTRIBUTE:
         RETVALUE((S8 *)"EG_TMR_LOAD_DISTRIBUTE");
#endif

      case EG_TMR_ECHO_GLOBAL:
         RETVALUE((S8 *)"EG_TMR_ECHO_GLOBAL");

      case EG_TMR_N3T3_REQ:
         RETVALUE((S8 *)"EG_TMR_N3T3_REQ");
#endif /* EGTP_U */

      default:
         break;
   }

   RETVALUE("EG_TMR_INVALID");
}
#endif /*-- DEBUGP --*/



/********************************************************************30**

  End of file:     pg_tmr.c@@/main/3 - Fri Feb  4 18:18:16 2011

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
 
ver        pat        init                  description
------ --------      ---- --------------------------------------
/main/1      ---      an                1. Created for Initial Release 1.1
/main/2      ---      pmacharla     
                      akulkarni         1. Initial for eGTP 1.2 Release
/main/2     eg001.102 snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss           2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/3      ---       pmacharla         Initial for eGTP release 2.1
/main/3      eg003.201 psingh           1. Merged code from eg006.102 to Fix
                                           TRACE5 macro related issue
/main/3      eg004.201 magnihotri       1. Header files added for eGTP-C PSF
                                        2. Check added Send ECHO Request only from ACTIVE PSF node 
/main/3    eg012.201   shpandey         1. Enhanced the piggy backing feature by adding the Timer 
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/
