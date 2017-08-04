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


/**********************************************************************

  Name:     Trillium LTE PGW Application

  Type:     C source file

  Desc:      Functions required for scheduling and initialization.

  File:     jgsm_ex_ms.c

  Sid:      avsm_ex_ms.c@@/main/1 - Tue Aug 17 18:54:36 2010

  Prg:      rk

 **********************************************************************/

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
#include "cm_dns.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "lhi.h"           /* TUCL LM Interface */

#include "lav.h"           /* CNE Layer management           */
#include "avsm.h"
#include "av_err.h"
/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_dns.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */

#include "lhi.x" 

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "lav.x"           /* CNE Layer management           */
#include "avsm.x"            /* CNE Application structures     */


/*
 *
 *      Fun:   smActvInit
 *
 *      Desc:  Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  avsm_ex_ms.c
 *
 */
#ifdef ANSI 
PUBLIC S16 smActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 smActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   SM_TRC3(smActvInit);
   cmMemset ((U8 *)&avSmCb, 0, sizeof(avSmCb));
   avSmCb.init.procId = SFndProcId();


   avSmCb.init.ent = entity;
   avSmCb.init.inst = inst;
   avSmCb.init.region = region;
   avSmCb.init.reason = reason;
   avSmCb.init.cfgDone = FALSE;
   avSmCb.init.pool = 0;
   avSmCb.init.acnt = FALSE;

#ifdef AV_USTA
   avSmCb.init.usta = TRUE;
#else
   avSmCb.init.usta = FALSE;
#endif /* AV_USTA */

#ifdef DEBUGP
   avSmCb.init.dbgMask = 0xffffffff;
#endif 

   AVSM_DBG_INFO((AVSM_PRNTBUF,"smActvInit returning OK...\n"));
   RETVALUE(ROK);
} /* avSmActvInit */

/*
 *
 *       Fun:    smActvTsk - SM Activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   avsm_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 smActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
#else
PUBLIC S16 smActvTsk(pst, mBuf)
Pst      *pst;
Buffer   *mBuf;
#endif /* ANSI */
{
   Mem sMem;

   SM_TRC3(smActvTsk);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst == (Pst *)NULLP)
   {
      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }
      AVSM_LOGERROR(ERRCLS_DEBUG, EAV050, ERRZERO,
                   "Null pst in smActvTsk\n");
      RETVALUE(RFAILED);
   }
   if (mBuf == (Buffer *)NULLP)
   {
      AVSM_LOGERROR(ERRCLS_DEBUG, EAV051, ERRZERO,
            "Null mBuf in smActvTsk\n");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   sMem.region = avSmCb.init.region;
   sMem.pool = avSmCb.init.pool;
   switch (pst->srcEnt)
   {
      /* Event from AV */
      case ENTEU:
      {
         switch (pst->event)
         {
            case EVTLAVCFGCFM:
            {
               (Void)cmUnpkLavCfgCfm(SmMiLavCfgCfm, pst, mBuf);
               break;
            }
            case EVTLAVSTSCFM:
            {
               (Void)cmUnpkLavStsCfm(SmMiLavStsCfm, pst, mBuf);
               break;
            }
            case EVTLAVCNTRLCFM:
            {
               (Void)cmUnpkLavCntrlCfm(SmMiLavCntrlCfm, pst, mBuf);
               break;
            }
            case EVTLAVSTACFM:
            {
               (Void)cmUnpkLavStaCfm(SmMiLavStaCfm, pst, mBuf);
               break;
            }
            case EVTLAVSTAIND:             /* Status Indication */
            {
               (Void)cmUnpkLavStaInd(SmMiLavStaInd, pst, mBuf);
               break;
            }
            case EVTLAVTRCIND:
            {
               (Void)cmUnpkLavTrcInd(SmMiLavTrcInd, pst, mBuf);
               break;
            }
            default:
            {
               AVSM_DBG_ERROR((AVSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
               (Void)SPutMsg(mBuf);
               break;
            }
            break;
         }
         break;
      }
      /* Event from TUCL */
      case ENTHI:
      {
         switch (pst->event)
         {
#ifdef LCSMHIMILHI
            case EVTLHISTAIND:             /* Status Indication */
            {
               (Void)cmUnpkLhiStaInd(SmMiLhiStaInd, pst, mBuf);
               break;
            }
            case EVTLHITRCIND:
            {
               (Void)cmUnpkLhiTrcInd(SmMiLhiTrcInd, pst, mBuf);
               break;
            }
            case EVTLHICFGCFM:
            {
               (Void)cmUnpkLhiCfgCfm(SmMiLhiCfgCfm, pst, mBuf);
               break;
            }
            case EVTLHICNTRLCFM:
            {
               (Void)cmUnpkLhiCntrlCfm(SmMiLhiCntrlCfm, pst, mBuf);
               break;
            }
            case EVTLHISTACFM:
            {
               (Void)cmUnpkLhiStaCfm(SmMiLhiStaCfm, pst, mBuf);
               break;
            }
            case EVTLHISTSCFM:
            {
               (Void)cmUnpkLhiStsCfm(SmMiLhiStsCfm, pst, mBuf);
               break;
            }
#endif
            default:
            {

             AVSM_DBG_ERROR((AVSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
              (Void)SPutMsg(mBuf);
               break;
            }
         }
         break;
      }

#ifdef EG
      /* Event from S-GW EG */
      case ENTEG :
      {
         switch (pst->event)
         {
#ifdef LCSMEGMILEG
            case EVTLEGSTAIND:             /* Status Indication */
            {
               (Void)cmUnpkLegStaInd(SmMiLegStaInd, pst, mBuf);
               break;
            }
            case EVTLEGTRCIND:
            {
               (Void)cmUnpkLegTrcInd(SmMiLegTrcInd, pst, mBuf);
               break;
            }
            case EVTLEGCFGCFM:
            {
               (Void)cmUnpkLegCfgCfm(SmMiLegCfgCfm, pst, mBuf);
               break;
            }
            case EVTLEGCNTRLCFM:
            {
               (Void)cmUnpkLegCntrlCfm(SmMiLegCntrlCfm, pst, mBuf);
               break;
            }
            case EVTLEGSTACFM:
            {
               (Void)cmUnpkLegStaCfm(SmMiLegStaCfm, pst, mBuf);
               break;
            }
            case EVTLEGSTSCFM:
            {
               (Void)cmUnpkLegStsCfm(SmMiLegStsCfm, pst, mBuf);
               break;
            }
#endif /* LCSMEGMILEG */
            default:
            {
              AVSM_DBG_ERROR((AVSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
              (Void)SPutMsg(mBuf);
               break;
            }
         }
         break;
      }
#endif /* EG */

      default:
      {
           AVSM_DBG_ERROR((AVSM_PRNTBUF,"Invalid srcEnt(%d)...\n",pst->srcEnt));
         (Void)SPutMsg(mBuf);
         break;
      }
   }

   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of avSmActvTsk() */


/**********************************************************************
  
         End of file:     avsm_ex_ms.c@@/main/1 - Tue Aug 17 18:54:36 2010
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp              1. LTE-CNE:PGW Initial Release.
*********************************************************************91*/
