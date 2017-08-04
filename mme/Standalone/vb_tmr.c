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

     Name:    LTE CNE - vb file

     Type:    C Include file

     Desc:    This file contains the vb application source code

     File:    vb_tmr.c

     Sid:      vb_tmr.c@@/main/1 - Mon Jun 28 19:35:40 2010

     Prg:     ap
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
#include "lhi.h"
#include "hit.h"           /* HI layer */
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
#include "vb_hss_auth.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "cm_emm_esm.h"
#include "vbsm.h"          /* */
#include "vb_err.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */

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
#include "lhi.x"
#include "hit.x"           /* HI layer */
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
#include "vb_hss_common.x"  /* CNE HSS Related structures     */
#include "vb.x"            /* CNE Application structures     */
#include "vbsm.x"          /*    */


PRIVATE Void vbMmeProcTmr ARGS ((Ptr cb, S16 tmrEvnt));

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
 *       File:  vb_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeActvTmr
(
void
)
#else
PUBLIC S16 vbMmeActvTmr()
#endif
{
   TRC3(vbMmeActvTmr);

   cmPrcTmr(&vbMmeCb.vbTqCp, vbMmeCb.vbTq, (PFV) vbMmeProcTmr);

   RETVALUE(ROK);

} /* end of vbMmeActvTmr */

/*
 *
 *       Fun:   vbMmeRegInitTmr
 *
 *       Desc:  Register and initialize timer queue structures in 
 *              LTE CNE during general config.
 *              
 *       Ret:   ROK - ok; 
 *              RFAILED - failed;
 *
 *       Notes: none
 *
 *       File:  vb_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeRegInitTmr
(
Void
)
#else
PUBLIC S16 vbMmeRegInitTmr()
#endif
{
   U16 idx;
   S16 ret;

   TRC2(vbMmeRegInitTmr);

   ret = SRegTmr((Ent)ENTUZ, (Inst)0, vbMmeCb.mmeCfg.tmrRes, vbMmeActvTmr);
   if (ret != ROK)
   {
      printf("\nvbMmeRegInitTmr:SRegTmr failed\n");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   vbMmeCb.vbTqCp.tmrLen      = VB_MME_APP_TQSIZE;

   for (idx = 0; idx < VB_MME_APP_TQSIZE; idx++)
   {
      vbMmeCb.vbTq[idx].first    = NULLP;
      vbMmeCb.vbTq[idx].tail     = NULLP;
   }

   RETVALUE(ROK);

} /* vbMmeRegInitTmr */


/*
*
*       Fun:    vbMmeStartTmr
*
*       Desc:   start a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   vb_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void vbMmeStartTmr
(
PTR                 cb,
S16                 tmrEvnt,
U32                 delay
)
#else
PUBLIC Void vbMmeStartTmr (cb, tmrEvnt, delay)
PTR                 cb;
S16                 tmrEvnt;
U32                 delay;
#endif /* ANSI */
{
   U16         wait;
   CmTmrArg    arg;
   CmTimer     *tmr;
   U8          maxTmrs;
   VbMmeUeCb   *ueCb;
   VbMmeEmmCb  *emmCb;
   VbMmeEsmCb  *esmCb;
   VbMmeSztSapCb  *sztSapCb;

   wait = 0;
   switch (tmrEvnt)
   {
      /* For SZT Bind Timer Timer */
      case VB_TMR_SZT_EVT:
         sztSapCb = ( VbMmeSztSapCb*)cb;
         tmr      = sztSapCb->timers;
         maxTmrs  = VB_MME_MAX_TMRS;
         break;
      case VB_TMR_EMM_3413:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_3422:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_3450:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_3460:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_3470:
         ueCb   = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_MOB_REACHABLE:
         ueCb   = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_IMPLI_DTCH_TMR:
         ueCb   = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_UE_CTX_DEL:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_INT_MGMT:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;
      case VB_TMR_ESM_3485:
         esmCb   = (VbMmeEsmCb*)cb;
         tmr     = esmCb->esmTimers;
         maxTmrs = VB_ESM_MAX_TIMERS;
         break;
      case VB_TMR_ESM_3486:
         esmCb   = (VbMmeEsmCb*)cb;
         tmr     = esmCb->esmTimers;
         maxTmrs = VB_ESM_MAX_TIMERS;
         break;
      case VB_TMR_ESM_3489:
         esmCb   = (VbMmeEsmCb*)cb;
         tmr     = esmCb->esmTimers;
         maxTmrs = VB_ESM_MAX_TIMERS;
         break;
      case VB_TMR_ESM_3495:
         esmCb   = (VbMmeEsmCb*)cb;
         tmr     = esmCb->esmTimers;
         maxTmrs = VB_ESM_MAX_TIMERS;
         break;
      case VB_TMR_EMM_IRAT_UE_CTXT_REL:
         ueCb    = (VbMmeUeCb *)cb;
         emmCb   = &ueCb->emmCb;
         tmr     = emmCb->emmTimers;
         maxTmrs = VB_EMM_MAX_TIMERS;
         break;

      default:
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB101, (ErrVal)tmrEvnt, \
            "vbStartTmr: Invalid tmrEvnt");
         RETVOID;

   } /* end of switch */

   wait = delay;

   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(vbMmeCb.vbTqCp);
      arg.tq     = vbMmeCb.vbTq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVOID;
} /* end of vbMmeStartTmr() */


/*
*
*       Fun:    vbMmeStopTmr
*
*       Desc:   stop a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   vb_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void vbMmeStopTmr
(
PTR     cb,
S16     event
)
#else
PUBLIC Void vbMmeStopTmr (cb, event)
PTR     cb;
S16     event;
#endif /* ANSI */
{

   CmTmrArg     arg;
   U8           idx;
   Bool         tmrRunning;
   CmTimer      *timers = NULLP;
   U8           max = 0;
   VbMmeUeCb   *ueCb;
   VbMmeEmmCb  *emmCb;
   VbMmeSztSapCb  *sztSapCb;
   VbMmeEsmCb  *esmCb;

   TRC3(vbMmeStopTmr)

   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
      case VB_TMR_SZT_EVT:
      {
         sztSapCb = (VbMmeSztSapCb *)cb;
         max     =  VB_MME_MAX_TMRS;
         for(idx = 0; idx < VB_MME_MAX_TMRS; idx++)
         {
            if(sztSapCb->timers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_3413:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_3422:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_3450:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_3460:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_3470:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_MOB_REACHABLE:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_IMPLI_DTCH_TMR:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_UE_CTX_DEL:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_INT_MGMT:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_ESM_3485:
      {
         esmCb  = (VbMmeEsmCb *)cb;
         timers = esmCb->esmTimers;
         max    = VB_ESM_MAX_TIMERS;
         for(idx = 0; idx < VB_ESM_MAX_TIMERS; idx++)
         {
            if(timers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_ESM_3486:
      {
         esmCb  = (VbMmeEsmCb *)cb;
         timers = esmCb->esmTimers;
         max    = VB_ESM_MAX_TIMERS;
         for(idx = 0; idx < VB_ESM_MAX_TIMERS; idx++)
         {
            if(timers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_ESM_3489:
      {
         esmCb  = (VbMmeEsmCb *)cb;
         timers = esmCb->esmTimers;
         max    = VB_ESM_MAX_TIMERS;
         for(idx = 0; idx < VB_ESM_MAX_TIMERS; idx++)
         {
            if(timers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_ESM_3495:
      {
         esmCb  = (VbMmeEsmCb *)cb;
         timers = esmCb->esmTimers;
         max    = VB_ESM_MAX_TIMERS;
         for(idx = 0; idx < VB_ESM_MAX_TIMERS; idx++)
         {
            if(timers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      case VB_TMR_EMM_IRAT_UE_CTXT_REL:
      {
         ueCb  = (VbMmeUeCb *)cb;
         emmCb = &ueCb->emmCb;
         timers     = emmCb->emmTimers;
         max        = VB_EMM_MAX_TIMERS;
         for(idx = 0; idx < VB_EMM_MAX_TIMERS; idx++)
         {
            if(emmCb->emmTimers[idx].tmrEvnt == event)
            {
               tmrRunning = TRUE;
               break;
            }
         }
         break;
      }
      
      default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   }

   /* initialize argument for common timer function */
   arg.tqCp    = &vbMmeCb.vbTqCp;
   arg.tq      = vbMmeCb.vbTq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum  = idx;
   cmRmvCbTq(&arg);

   RETVOID;
} /* end of vbMmeStopTmr() */

/*
*
*       Fun:    vbMmeProcTmr
*
*       Desc:   Handle the expiration of timers
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   vb_tmr.c
*
*/

#ifdef ANSI
PRIVATE Void vbMmeProcTmr
(
Ptr     cb,
S16     event
)
#else
PRIVATE Void vbMmeProcTmr (cb, event)
Ptr     cb;
S16     event;
#endif /* ANSI */
{
   TRC3(vbMmeProcTmr)

   switch(event)
   {
      case VB_TMR_SZT_EVT:
      {
         break;
      }
      case VB_TMR_EMM_3413:
      case VB_TMR_EMM_3422:
      case VB_TMR_EMM_3450:
      case VB_TMR_EMM_3460:
      case VB_TMR_EMM_3470:
      case VB_TMR_EMM_MOB_REACHABLE:
      case VB_TMR_EMM_IMPLI_DTCH_TMR:
      case VB_TMR_UE_CTX_DEL:
      case VB_TMR_INT_MGMT:
      case VB_TMR_EMM_IRAT_UE_CTXT_REL:
      {
         vbMmeEmmHdlTmrEvnt((VbMmeUeCb *)cb, event);
         break;
      }
      case VB_TMR_ESM_3485:
      case VB_TMR_ESM_3486:
      case VB_TMR_ESM_3489:
      case VB_TMR_ESM_3495:
      {
         /* call timer expiry handler function for esm timer */
         vbMmeEsmHdlTmrEvnt((VbMmeEsmCb *)cb, event);
         break;
      }

      default:
         /* Invalid Timer */
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB102, ERRZERO,
               "vbMmeProcTmr: Invalid timer\n");
         break;
   }

   RETVOID;
} /* end of vbMmeProcTmr() */


/********************************************************************30**

         End of file:     vb_tmr.c@@/main/1 - Mon Jun 28 19:35:40 2010

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
/main/1      -         -               1. LTE-CNE Initial Release.
*********************************************************************91*/
