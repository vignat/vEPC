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

     Name:     eGTP Layer

     Type:     C source file

     Desc:     C Upper Interface structures

     File:     eu.x

     Sid:      smegexms.c@@/main/4 - Fri Feb  4 18:18:29 2011

     Prg:      rs

*********************************************************************21*/


/* header include files (.h) */
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
#include "cm_dns.h"        /* Common DNS library              */
#include "cm_inet.h"       /* Common socket library           */
#include "hit.h"           /* HIT interface defines           */
#ifdef EG_SCTP
#include "sct.h"           /* SCT interface */
#endif /*-- EG_SCTP --*/
#include "eg_util.h"
#include "egt.h"           /* EGU  interface defines          */
#include "leg.h"           /* Layer management, Diameter      */
#ifdef EG_ACC
#include "egac_acc.h"      /* Defines for EGTP test layer */
#endif

#include "smeg_err.h"      /* Defines for SM EG */

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
#include "cm_dns.x"        /* Common DNS library              */
#include "cm_inet.x"       /* Common socket library           */
#include "hit.x"           /* HIT interface defines           */
#include "leg.x"           /* Layer management EGTP            */
#ifdef EG_ACC
#include "egac_acc.x"        /* typedefs for EGTP test layer     */
#endif

/* public routines */

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from Diameter  layer
*
*       Ret:    ROK  - ok
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   smaqexms.c
*
*/


#ifdef ANSI
PUBLIC S16 smEgActvTsk
(
Pst    *pst,                /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smEgActvTsk(pst, mBuf)
Pst    *pst;                /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16         ret;           /* return code */

   TRC3(smEgActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCSMEGMILEG
                   
      case EVTLEGCFGCFM:             /* Config confirm */
         ret = cmUnpkLegCfgCfm(SmMiLegCfgCfm, pst, mBuf);
         break;

      case EVTLEGCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLegCntrlCfm(SmMiLegCntrlCfm, pst, mBuf);
         break;

      case EVTLEGSTACFM:             /* Status Confirm */
         ret = cmUnpkLegStaCfm(SmMiLegStaCfm, pst, mBuf);
         break;

      case EVTLEGSTSCFM:             /* Statistics Confirm */
         ret = cmUnpkLegStsCfm(SmMiLegStsCfm, pst, mBuf);
         break;

      case EVTLEGSTAIND:             /* Status Indication */
         ret = cmUnpkLegStaInd(SmMiLegStaInd, pst, mBuf);
         break;

      case EVTLEGTRCIND:             /* Trace Indication */
         ret = cmUnpkLegTrcInd(SmMiLegTrcInd, pst, mBuf);
         break;

#endif /* LCSMEGMILEG */

      default:
/*eg004.201 Active task for eGTPS PSF layer manager called*/         
#ifdef HW 
         smHwActvTsk(pst, mBuf);
         break;
#else
         /* we should never get here, especially if tightly coupled */
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)ESMEG001, 
                   (ErrVal)pst->event, "smEgActvTsk () Failed"); 
#endif
         ret = RFAILED;
         break;
#endif         
   }
   
   SExitTsk();

   RETVALUE(ret);
} /* smEgActvTsk */


/********************************************************************30**

         End of file:     smegexms.c@@/main/4 - Fri Feb  4 18:18:29 2011

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

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---     an              1. Initial Release for EGTP
/main/3      ---     sv                1. Initial for eGTP 1.2 Release
/main/4      ---      pmacharla         Initial for eGTP release 2.1
/main/4      eg004.201      magnihotri  1.Active task for eGTPS PSF layer manager called
*********************************************************************91*/
