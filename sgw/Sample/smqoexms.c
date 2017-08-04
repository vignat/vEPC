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

     Name:     Trillium LTE SGW Application 

     Type:     C source file

     Desc:      Sample C source code for the stack manager.
               - SGW layer interface primitives.
               - Functions required for unpacking layer management
                 service provider primitives in loosely coupled systems.
               - FTHA for SGW present here as well.


     File:     smqoexms.c

     Sid:      smqoexms.c@@/main/1 - Thu Jul  8 21:22:24 2010

     Prg:      ap 

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
#include "lqo.h"           /* LTE CNE Layer management           */


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
#include "lqo.x"           /* LTE CNE Layer management            */

/* public routines */

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from LTE CNE layer
*
*       Ret:    ROK  - ok
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   smqoexms.c
*
*/


#ifdef ANSI
PUBLIC S16 smQoActvTsk
(
Pst    *pst,                /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smQoActvTsk(pst, mBuf)
Pst    *pst;                /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16         ret;           /* return code */

   TRC3(smQoActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCSMQOMILQO
                   
      case EVTLQOCFGCFM:             /* Config confirm */
         ret = cmUnpkLqoCfgCfm(SmMiLqoCfgCfm, pst, mBuf);
         break;

      case EVTLQOCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLqoCntrlCfm(SmMiLqoCntrlCfm, pst, mBuf);
         break;

      case EVTLQOSTACFM:             /* Status Confirm */
         ret = cmUnpkLqoStaCfm(SmMiLqoStaCfm, pst, mBuf);
         break;

      case EVTLQOSTSCFM:             /* Statistics Confirm */
         ret = cmUnpkLqoStsCfm(SmMiLqoStsCfm, pst, mBuf);
         break;

      case EVTLQOSTAIND:             /* Status Indication */
         ret = cmUnpkLqoStaInd(SmMiLqoStaInd, pst, mBuf);
         break;

      case EVTLQOTRCIND:             /* Trace Indication */
         ret = cmUnpkLqoTrcInd(SmMiLqoTrcInd, pst, mBuf);
         break;

#endif /* LCSMQOMILQO */

      default:
         /* we should never get here, especially if tightly coupled */
         SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)0, 
                   (ErrVal)pst->event, "smQoActvTsk () Failed"); 
#endif
         ret = RFAILED;
         break;
   }
   
   SExitTsk();

   RETVALUE(ret);
} /* smQoActvTsk */


/********************************************************************30**

         End of file:     smqoexms.c@@/main/1 - Thu Jul  8 21:22:24 2010

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
/main/1      -      sp                  1.LTE-CNE:SGW Initial Release
*********************************************************************91*/
