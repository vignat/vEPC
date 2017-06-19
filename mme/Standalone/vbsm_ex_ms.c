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


/**********************************************************************

  Name:     VBSM

  Type:     C source file

  Desc:     LTE CNE 

  File:     vbsm_ex_ms.c

  Sid:      vbsm_ex_ms.c@@/main/1 - Mon Jun 28 19:35:41 2010

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
#include "cm_pasn.h"       /* Common ASN.1 library           */
#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "lsb.h"           /* SCTP LM Interface */
#include "lhi.h"           /* TUCL LM Interface */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#include "vb.h"            /* CNE Layer                  */
#include "vbsm.h"
#include "vb_err.h"            /* CNE Layer                  */

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
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "sct.x"           /* SCT interface defines           */

#include "lsb.x"           
#include "lhi.x" 

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */
/* vb007.101 : s1ap include files order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "vbsm.x"            /* CNE Application structures     */


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
 *      File:  vbsm_ex_ms.c
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
   cmMemset ((U8 *)&vbSmCb, 0, sizeof(vbSmCb));
   vbSmCb.init.procId = SFndProcId();

   vbSmCb.init.ent = entity;
   vbSmCb.init.inst = inst;
   vbSmCb.init.region = region;
   vbSmCb.init.reason = reason;
   vbSmCb.init.cfgDone = FALSE;
   vbSmCb.init.pool = 0;
   vbSmCb.init.acnt = FALSE;

#ifdef VB_USTA
   vbSmCb.init.usta = TRUE;
#else
   vbSmCb.init.usta = FALSE;
#endif /* VB_USTA */

#ifdef DEBUGP
   vbSmCb.init.dbgMask = 0;
#endif /* DEBUGP */
   VBSM_DBG_INFO((VBSM_PRNTBUF,"smActvInit returning OK...\n"));
   RETVALUE(ROK);
} /* vbSmActvInit */

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
 *       File:   vbsm_ex_ms.c
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
   SM_TRC3(smActvTsk);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst == (Pst *)NULLP)
   {
      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }
      VBSM_LOGERROR(ERRCLS_DEBUG, EVB102, ERRZERO,
                    "Null pst in smActvTsk\n");
      RETVALUE(RFAILED);
   }
   if (mBuf == (Buffer *)NULLP)
   {
      VBSM_LOGERROR(ERRCLS_DEBUG, EVB102, ERRZERO,
            "Null mBuf in smActvTsk\n");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   switch (pst->srcEnt)
   {
      /* Event from VB */
      case ENTUZ:
      case ENTEU:
      case ENTVB:
      {
         switch (pst->event)
         {
#ifdef LCSMVBMILVB
            case EVTLVBCFGCFM:
            {
               (Void)cmUnpkLvbCfgCfm(SmMiLvbCfgCfm, pst, mBuf);
               break;
            }
            case EVTLVBSTSCFM:
            {
               (Void)cmUnpkLvbStsCfm(SmMiLvbStsCfm, pst, mBuf);
               break;
            }
            case EVTLVBCNTRLCFM:
            {
               (Void)cmUnpkLvbCntrlCfm(SmMiLvbCntrlCfm, pst, mBuf);
               break;
            }
            case EVTLVBSTACFM:
            {
               (Void)cmUnpkLvbStaCfm(SmMiLvbStaCfm, pst, mBuf);
               break;
            }
            case EVTLVBSTAIND:             /* Status Indication */
            {
               (Void)cmUnpkLvbStaInd(SmMiLvbStaInd, pst, mBuf);
               break;
            }
            case EVTLVBTRCIND:
            {
               (Void)cmUnpkLvbTrcInd(SmMiLvbTrcInd, pst, mBuf);
               break;
            }
#endif            
            default:
            {
               VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
               (Void)SPutMsg(mBuf);
               break;
            }
            break;
         }
         break;
      }
      /* Event from S1AP */
      case ENTSZ:
      {
         switch(pst->event)
         {
#ifdef LCSMSZMILSZ
            case EVTLSZSTAIND:             /* Status Indication */
            {
               (Void)cmUnpkLszStaInd(SmMiLszStaInd, pst, mBuf);
               break;
            }
            case EVTLSZTRCIND:
            {
               (Void)cmUnpkLszTrcInd(SmMiLszTrcInd, pst, mBuf);
               break;
            }
            case EVTLSZCFGCFM:
            {
               (Void)cmUnpkLszCfgCfm(SmMiLszCfgCfm, pst, mBuf);
               break;
            }
            case EVTLSZCNTRLCFM:
            {
               (Void)cmUnpkLszCntrlCfm(SmMiLszCntrlCfm, pst, mBuf);
               break;
            }
            case EVTLSZSTACFM:
            {
               (Void)cmUnpkLszStaCfm(SmMiLszStaCfm, pst, mBuf);
               break;
            }
            case EVTLSZSTSCFM:
            {
               (Void)cmUnpkLszStsCfm(SmMiLszStsCfm, pst, mBuf);
               break;
            }
#endif
            default:
            {
               VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
               (Void)SPutMsg(mBuf);
               break;
            }
         }
         break;
      }
         /* Event from SCTP */
      case ENTSB:
      {
         switch (pst->event)
         {
#ifdef LCSMSBMILSB
            case LSB_EVTCFGCFM:
            {
               (Void)cmUnpkLsbCfgCfm(SmMiLsbCfgCfm, pst, mBuf);
               break;
            }
            case LSB_EVTCNTRLCFM:
            {
               (Void)cmUnpkLsbCntrlCfm(SmMiLsbCntrlCfm, pst, mBuf);
               break;
            }
            case LSB_EVTSTACFM:
            {
               (Void)cmUnpkLsbStaCfm(SmMiLsbStaCfm, pst, mBuf);
               break;
            }
            case LSB_EVTSTSCFM:
            {
               (Void)cmUnpkLsbStsCfm(SmMiLsbStsCfm, pst, mBuf);
               break;
            }
            case LSB_EVTSTAIND:             /* Status Indication */
            {
               (Void)cmUnpkLsbStaInd(SmMiLsbStaInd, pst, mBuf);
               break;
            }
            case LSB_EVTTRCIND:
            {
               (Void)cmUnpkLsbTrcInd(SmMiLsbTrcInd, pst, mBuf);
               break;
            }
#endif
            default:
            {
               VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
               (Void)SPutMsg(mBuf);
               break;
            }
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
                 VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
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
               VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid evnt(%d)...\n",pst->event));
               (Void)SPutMsg(mBuf);
               break;
            }
         }
         break;
      }
#endif /* EG */

      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid srcEnt(%d)...\n",pst->srcEnt));
         (Void)SPutMsg(mBuf);
         break;
      }
   }

   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of vbSmActvTsk() */


/**********************************************************************
  
         End of file:     vbsm_ex_ms.c@@/main/1 - Mon Jun 28 19:35:41 2010
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rk              1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1   vb007.101   akaranth     1. s1ap include files order has changed
*********************************************************************91*/
