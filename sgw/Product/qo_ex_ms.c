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

     Name:    SGW Application external interface file

     Type:    C source file

     Desc:    SGW Application external interface file

     File:    qo_ex_ms.c

     Sid:      qo_ex_ms.c@@/main/1 - Thu Jul  8 21:22:19 2010

     Prg:     mm
*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"         /* common DNS libraru defines */

#include "leg.h"           /* layer management defines for EGCP */
#include "eg_edm.h"        /* EDM Module structures            */
#include "eg.h"            /* defines and macros for EG */
#include "egt.h"            /* defines and macros for EG */
#include "qo.h"
#include "qo_err.h"


/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"         /* common DNS libraru defines */

#include "egt.x"           /* EGT Interface Structures */
#include "leg.x"           /* layer management typedefs for EGCP */

#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* typedefs for EGCP */

#include "eg_err.h"        /* EG error defines */
#include "qo.x"

/*
 *
 *      Fun:   QoActvInit
 *
 *      Desc:  Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI 
PUBLIC S16 QoActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 QoActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   TRC3(QoActvInit);
   
   cmMemset ((U8 *)&qoSgwCb, 0, sizeof(QoSgwCb));
   qoSgwCb.init.procId = SFndProcId();

   qoSgwCb.init.ent = entity;
   qoSgwCb.init.inst = inst;
   qoSgwCb.init.region = region;
   qoSgwCb.init.reason = reason;
   qoSgwCb.init.cfgDone = FALSE;
   qoSgwCb.init.pool = 0;
   qoSgwCb.init.acnt = FALSE;

#ifdef QO_USTA
   qoSgwCb.init.usta = TRUE;
#else
   qoSgwCb.init.usta = FALSE;
#endif /* QO_USTA */

#ifdef DEBUGP
#ifdef QO_DEBUG
   qoSgwCb.init.dbgMask = QO_DBG_MASK;
#else
   qoSgwCb.init.dbgMask = 0;
#endif /* QO_DEBUG */
#endif /* DEBUGP */

   RETVALUE(ROK);

} /* */


/*
 *
 *       Fun:    qoSgwInitExt - external initialization
 *
 *       Desc:   performs additional initializations as required
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  to be changed by the customer if required
 *
         File:   qo_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 qoSgwInitExt
(
void
)
#else
PUBLIC S16 qoSgwInitExt()
#endif /* ANSI */
{
   TRC2(qoSgwInitExt)
   RETVALUE(ROK);
} /* end of qoSgwInitExt()*/


/*
 *
 *       Fun:    QoActvTsk - eGTP user activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   qo_sgw_ex_ms.c
 *
 */
#ifdef ANSI
PUBLIC S16 QoActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
#else
PUBLIC S16 QoActvTsk(pst, mBuf)
Pst      *pst;
Buffer   *mBuf;
#endif /* ANSI */
{
   TRC3(QoActvTsk)
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst == (Pst *)NULLP)
   {
      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }
      RETVALUE(RFAILED);
   }
   if (mBuf == (Buffer *)NULLP)
   {
      QO_SGW_LOGERR(ERRCLS_INT_PAR, 
         ("Null mBuf in QoActvTsk\n"));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   switch (pst->srcEnt)
   {
      case ENTEG:
         {
            switch (pst->event)
            {
#if ((defined(LCEULIEGT)) || ((defined(LWLCEULIEGT))))
               case EVTEGTBNDCFM:
                  {
                     (Void)cmUnpkEgtBndCfm(EuLiEgtBndCfm, pst, mBuf);
                     break;
                  }

#ifdef EGTP_C
               case EVTEGTSIGCFM:
                  {
                     (Void)cmUnpkEgtSigCfm(EuLiEgtSigCfm, pst, mBuf); 

                     break;
                  }

               case EVTEGTSIGIND:
                  {
                     (Void)cmUnpkEgtSigInd(EuLiEgtSigInd, pst, mBuf); 

                     break;
                  }

               case EVTEGTERRIND:
                  {
                     (Void)cmUnpkEgtErrInd(EuLiEgtErrInd, pst, mBuf); 
                     break;
                  }
#ifdef EG_PHASE2
               case EVTEGTSTAIND:
                  {
                     (Void)cmUnpkEgtStaInd(EuLiEgtStaInd, pst, mBuf);
                     break;
                  }
#endif /* EG_PHASE2 */ 
               case EVTEGTSTACFM:
                  {
                     (Void)cmUnpkEgtStaCfm(EuLiEgtStaCfm, pst, mBuf);
                     break;
                  }

               case EVTEGTLCLDELCFM:
                  {
                     (Void)cmUnpkEgtLclDelCfm(EuLiEgtLclDelCfm, pst, mBuf);
                     break;
                  }
#endif /* EGTP_C */
#ifdef EGTP_U
               case EVTEGTUDATIND:
                  {
                     (Void)cmUnpkEgtEguDatInd(EuLiEgtEguDatInd, pst, mBuf);
                     break;
                  }

               case EVTEGTULCLTNLMGMTCFM:
                  {
                     (Void)cmUnpkEgtEguLclTnlMgmtCfm(EuLiEgtEguLclTnlMgmtCfm, pst, mBuf);
                     break;
                  }

               case EVTEGTUSTACFM:
                  {
                     (Void)cmUnpkEgtEguStaCfm(EuLiEgtEguStaCfm, pst, mBuf);
                     break;
                  }

               case EVTEGTUUSTAIND:
                  {
                     (Void)cmUnpkEgtEguUStaInd(EuLiEgtEguUStaInd, pst, mBuf);
                     break;
                  }

               case EVTEGTUERRIND:
                  {
                     (Void)cmUnpkEgtEguErrInd(EuLiEgtEguErrInd, pst, mBuf);
                     break;
                  }

#endif /* EGTP_U */

#endif /* LCEULIEGT */
               default:
                  {
#if (ERRCLASS & ERRCLS_INT_PAR)
                     QO_SGW_LOGERR(ERRCLS_INT_PAR,
                           ("invalid pst->event in LQoActvTsk\n"));
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
                     (Void)SPutMsg(mBuf);
                     break;
                  }
            }
            break;
         }
      case ENTSM:
         {
            switch(pst->event)
            {
#if ((defined(LCQOMILQO)) || (defined(LWCQOMILQO)))
               case EVTLQOCFGREQ:
                  {
                     /* Process a config request */
                     cmUnpkLqoCfgReq(QoMiLqoCfgReq, pst, mBuf);
                     break;
                  }
               case EVTLQOCNTRLREQ:
                  {  
                     /* Process a control request */
                     cmUnpkLqoCntrlReq(QoMiLqoCntrlReq, pst, mBuf);
                     break;
                  }
               case EVTLQOSTAREQ:
                  {
                     /* Process a status request  */
                     cmUnpkLqoStaReq(QoMiLqoStaReq, pst, mBuf);
                     break;
                  } 
               case EVTLQOSTSREQ:
                  {
                     /* Process a statistics request */
                     cmUnpkLqoStsReq(QoMiLqoStsReq, pst, mBuf);
                     break;
                  }
#endif /* ((defined(LCQOMILQO)) || ((defined(LWCQOMILQO)))) */

               default:
                  /* we should never get here, especially if tightly coupled */
                  SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_INT_PAR)
                  QO_SGW_LOGERR(ERRCLS_INT_PAR, 
                        ("invalid pst->evnt in LQoActvTsk\n"));
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
                  break;
            } 
            break;
         }
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         QO_SGW_LOGERR(ERRCLS_INT_PAR, 
               ("invalid pst->srcEnt in LQoActvTsk\n"));
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
         break;
   }
   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of QoActvTsk() */

/********************************************************************30**

         End of file:     qo_ex_ms.c@@/main/1 - Thu Jul  8 21:22:19 2010

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
/main/1      -      sp                  1.LTE-CNE:SGW Initial Release.
*********************************************************************91*/
