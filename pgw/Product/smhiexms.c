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
  
     Name:     Stack Manager
  
     Type:     C source file
  
     Desc:     Activation function for TCP UDP Convergence Layer (TUCL)
               Stack Manager.
              
     File:     smhiexms.c
  
     Sid:      smhiexms.c@@/main/5 - Mon Mar  3 20:09:31 2008

     Prg:      asa
  
*********************************************************************21*/

/*
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.
*/
  
  
/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_inet.h"       /* common sockets */
#include "lhi.h"           /* stack management- TUCL */
#include "smhi_err.h"      /* stack management- TUCL */

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* common sockets */
#include "lhi.x"           /* stack management- TUCL */

  
/* local defines */

/* local typedefs */
  
/* local externs */
  
/* forward references */
  

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from TUCL
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   smhiexms.c
*
*/
  
#ifdef ANSI
PUBLIC S16 smHiActvTsk
(
Pst *spst,                  /* post structure */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smHiActvTsk(spst, mBuf)
Pst *spst;                  /* post structure */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16           ret = ROK; /* return value from the function */

   switch(spst->event)
   {
#ifdef LCSMHIMILHI
      case EVTLHICFGCFM:             /* Configuration Confirm */
         if ((ret = cmUnpkLhiCfgCfm(SmMiLhiCfgCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
      
      case EVTLHISTSCFM:             /* Statistics Confirm */
         if ((ret = cmUnpkLhiStsCfm(SmMiLhiStsCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHICNTRLCFM:           /* Conntrol Confirm */
         if ((ret = cmUnpkLhiCntrlCfm(SmMiLhiCntrlCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHISTACFM:             /* Status Confirm */
         if ((ret = cmUnpkLhiStaCfm(SmMiLhiStaCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

     case EVTLHISTAIND:             /* Status Indication */
         if ((ret = cmUnpkLhiStaInd(SmMiLhiStaInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHITRCIND:             /* Trace Indication */
         if ((ret = cmUnpkLhiTrcInd(SmMiLhiTrcInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
#endif
      default:
        (Void)SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(spst->dstEnt, spst->dstInst, spst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)ESMHI001, 
                   (ErrVal)spst->event, "smHiActvTsk () Failed"); 
#endif
         break;
   }

   SExitTsk();

   RETVALUE(ret);
}/* end of smHiActvTsk() */


/********************************************************************30**
 
         End of file:     smhiexms.c@@/main/5 - Mon Mar  3 20:09:31 2008

*********************************************************************31*/
 
/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/
/********************************************************************70**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
1.1         ---      asa  1. initial release.
/main/2     ---      cvp  1. changed the copyright header.
/main/3     ---      cvp  1. changed the copyright header.
/main/4      ---      kp   1. Updated for release 1.5.
/main/5      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
