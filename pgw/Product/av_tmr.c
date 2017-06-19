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

     Name:    Trillium LTE SGW Application

     Type:    C Include file

     Desc:    This file contains the PGW application source code

     File:    jg_tmr.c

     Sid:      av_tmr.c@@/main/1 - Tue Aug 17 18:54:34 2010

     Prg:     rp
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

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "lav.h"           /* PGW Layer management           */
#include "av.h"            /* PGW Application defines        */
#include "avsm.h"          /* */

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

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "pcap.h"
#include "lav.x"           /* PGW Layer management           */
#include "av.x"            /* PGW Application structures     */
#include "avsm.x"          /*    */




PRIVATE Void avProcTmr ARGS ((Ptr cb, S16 tmrEvnt));

/*
*
*       Fun:    avProcTmr
*
*       Desc:   Handle the expiration of timers
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   av_tmr.c
*
*/

#ifdef ANSI
PRIVATE Void avProcTmr
(
Ptr     cb,
S16     event
)
#else
PRIVATE Void avProcTmr (cb, event)
Ptr     cb;
S16     event;
#endif /* ANSI */
{

   TRC3(avProcTmr)

   switch(event)
   {
      default:
         /* Invalid Timer */
         AV_LOGERROR(ERRCLS_DEBUG, EAV049, ERRZERO,
               "avProcTmr: Invalid timer\n");
         break;
   }

   RETVOID;
} /* end of avProcTmr() */


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
 *       File:  av_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 avActvTmr
(
void
)
#else
PUBLIC S16 avActvTmr()
#endif
{
   TRC3(avActvTmr);

   cmPrcTmr(&avCb.avTqCp, avCb.avTq, (PFV) avProcTmr);

   RETVALUE(ROK);
} /* end of avActvTmr */

/*
 *
 *       Fun:   avRegInitTmr
 *
 *       Desc:  Register and initialize timer queue structures in 
 *              LTE CNE during general config.
 *              
 *       Ret:   ROK - ok; 
 *              RFAILED - failed;
 *
 *       Notes: none
 *
 *       File:  av_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 avRegInitTmr
(
Void
)
#else
PUBLIC S16 avRegInitTmr()
#endif
{
   U16 idx;
   S16 ret;

   TRC2(avRegInitTmr);

   ret = SRegTmr((Ent)ENTEU, (Inst)0, 10, avActvTmr);
   if (ret != ROK)
   {
      AV_LOGERROR(ERRCLS_DEBUG, EAV049, ERRZERO,"\navRegInitTmr:SRegTmr failed\n");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   avCb.avTqCp.tmrLen      = AV_APP_TQSIZE;

   for (idx = 0; idx < AV_APP_TQSIZE; idx++)
   {
      avCb.avTq[idx].first    = NULLP;
      avCb.avTq[idx].tail     = NULLP;
   }

   RETVALUE(ROK);
} /* avRegInitTmr */

/*
*
*       Fun:    avStartTmr
*
*       Desc:   start a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   av_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void avStartTmr
(
S16                 event,              /* Event */
U32                 wait                /* Timeout Waiting Period */
)
#else
PUBLIC Void avStartTmr (event, wait)
S16                 event;              /* Event */
U32                 wait;               /* Timeout Waiting Period */
#endif /* ANSI */
{
   CmTmrArg         arg;                /* Timer Arguments to be passed */

   TRC3(avStartTmr)

   if (wait > 0)
   {
      if (avCb.timer.tmrEvnt != TMR_NONE)
      {
  /* Timer is currently in use - reset */
         (Void) avStopTmr(&avCb.timer);
      }
      arg.tq      = avCb.avTq;
      arg.tqCp    = &avCb.avTqCp;
      arg.timers  = &avCb.timer;
      arg.cb      = NULLP;
      arg.evnt    = (U8)event;
      arg.wait    = wait;
      arg.tNum    = NOTUSED;
      arg.max     = 1;
      cmPlcCbTq(&arg);
   }

   RETVOID;
} /* end of avStartTmr() */


/*
*
*       Fun:    avStopTmr
*
*       Desc:   stop a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   av_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void avStopTmr
(
CmTimer            *tmr                 /* Common Timer Structure */
)
#else
PUBLIC Void avStopTmr (tmr)
CmTimer            *tmr;                /* Common Timer Structure */
#endif /* ANSI */
{
   CmTmrArg         arg;                /* Timer Arguments to be passed */

   TRC3(avStopTmr)

   if (tmr->tmrEvnt != TMR_NONE)
   {
      arg.tq      = avCb.avTq;
      arg.tqCp    = &avCb.avTqCp;
      arg.timers  = tmr;
      arg.cb      = (PTR)NULLP;
      arg.evnt    = NOTUSED;
      arg.wait    = NOTUSED;
      arg.tNum    = 0;
      arg.max     = 1;
      cmRmvCbTq(&arg);
   }

   RETVOID;
} /* end of avStopTmr() */



/********************************************************************30**

         End of file:     av_tmr.c@@/main/1 - Tue Aug 17 18:54:34 2010

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
/main/1      -      sp              1. LTE-CNE:PGW Initial Release
*********************************************************************91*/
