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

     Name:    eGTP

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    eu_ptli.c

     Sid:      eu_ex_ms.c@@/main/3 - Fri Feb  4 18:18:20 2011

     Prg:     pc
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
#include "egac_acc.h"        /* defines for EG acceptance tests */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#include "cm_xta.h" 


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

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif

/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif

#include "cm_xta.x" 
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* typedefs for EGCP */
#include "egac_acc.x"        /* typedefs for EGCP acceptance tests */
#include "egac_leg.x"
#include "eu.x"

#include "eg_err.h"        /* EG error defines */

/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "lhw.x"
#include "hw.x"
#endif
/*
 *
 *       Fun:    euActvInit - eGTP user initialization function
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   eu_ex_ms.c
 *
 */
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 euActvInit
(
ProcId   procId,
Ent      ent,                 /* entity */
Inst     inst,                /* instance */
Region   region,              /* region */
Reason   reason,               /* reason */
Void   **xxCb               /* xxCb pointer */
)
#else
PUBLIC S16 euActvInit(procId, ent, inst, region, reason, xxCb)
ProcId   procId;
Ent      ent;                 /* entity */
Inst     inst;                /* instance */
Region   region;              /* region */
Reason   reason;              /* reason */
Void   **xxCb;
#endif /*ANSI*/
#else /*SS_MULTIPLE_PROCS*/
#ifdef ANSI
PUBLIC S16 euActvInit
(
Ent      ent,                 /* entity */
Inst     inst,                /* instance */
Region   region,              /* region */
Reason   reason               /* reason */
)
#else
PUBLIC S16 euActvInit(ent, inst, region, reason)
Ent      ent;                 /* entity */
Inst     inst;                /* instance */
Region   region;              /* region */
Reason   reason;              /* reason */
#endif /* ANSI */
#endif /*SS_MULTIPLE_PROCS */
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   TRC2(euActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   (Void)euInitExt();
   RETVALUE(ROK);
} /* end of euActvInit() */

/*
 *
 *       Fun:    euInitExt - external initialization
 *
 *       Desc:   performs additional initializations as required
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  to be changed by the customer if required
 *
         File:   eu_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 euInitExt
(
void
)
#else
PUBLIC S16 euInitExt()
#endif /* ANSI */
{
   TRC2(euInitExt)
   RETVALUE(ROK);
} /* end of euInitExt()*/


/*
 *
 *       Fun:    euActvTsk - eGTP user activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eu_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 euActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
#else
PUBLIC S16 euActvTsk(pst, mBuf)
Pst      *pst;
Buffer   *mBuf;
#endif /* ANSI */
{
   Mem sMem;

   /*-- eg003.201 : TRC changes from eg006.102--*/
   TRC2(euActvTsk)
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
      EGLOGERROR(ERRCLS_INT_PAR, EEG074, ERRZERO, 
         "Null mBuf in euActvTsk\n");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   sMem.region = DFLT_REGION;
   sMem.pool = DFLT_POOL;

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
      /* ccpu00137922 */
      /* eg014.201 - ccpu00139510 Egtpc release 10 upgrade */
      /********************************************
      *               SIG  REQUEST                *
      ********************************************/
      case EVTEGTTRANSSIGREQ:
      {
         cmUnpkEgtTransSigReq(EgUiEgtTransSigReq, pst, mBuf, &sMem);
         break;
      }

      /* eg014.201 - ccpu00139510 Egtpc release 10 upgrade */
      /********************************************
      *               SIG  RESPONSE               *
      ********************************************/
      case EVTEGTTRANSSIGRSP:
      {
         cmUnpkEgtTransSigRsp(EgUiEgtTransSigRsp, pst, mBuf, &sMem);
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
         EGLOGERROR(ERRCLS_INT_PAR, EEG075, ERRZERO, 
            "invalid pst->event in euActvTsk\n");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
         (Void)SPutMsg(mBuf);
         break;
      }
   }
   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of euActvTsk() */

/********************************************************************30**

         End of file:     eu_ex_ms.c@@/main/3 - Fri Feb  4 18:18:20 2011

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
/main/1      ---      pc       1. Updated for Initial Release 
/main/2      ---      rkumar   1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3     eg003.201 psingh   1. Merged code from eg006.102 to Fix TRACE5 
                                  macro related issue
/main/3     eg004.201 magnihotri 1. Header files added for eGTPC PSF Upgrade
/main/3      eg014.201    rss     1. Egtpc release 10 upgrade
*********************************************************************91*/
