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

     Name:     smsbexms.c - stack manager - interface with SCTP layer

     Type:     C source file

     Desc:     Sample C source code for the stack manager.
               - SCTP layer interface primitives.
               - Functions required for unpacking layer management
                 service provider primitives in loosely coupled systems.

     File:     smsbexms.c

     Sid:      smsbexms.c@@/main/4 - Thu Jul 26 07:54:02 2007

     Prg:      wvdl

*********************************************************************21*/

/*

  smsbexms.c -

Following functions are provided in this file:
*/

/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000163                      SCTP layer
 */

/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_hash.h"       /* common hashing */
#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* timer defines */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"        /* Common DNS library */
#ifdef SB_FTHA
#include "sht.h"           /* SHT Interface header file */
#endif /* SB_FTHA */
#include "sct.h"           /* SCT interface */
#include "lsb.h"           /* layer management defines for SCTP layer */
#include "sb_mtu.h"
#include "sb.h"            /* defines and macros for SCTP layer */
#include "sb_err.h"        /* error defines for SCTP layer */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm_hash.x"       /* common hashing */
#include "cm_llist.x"      /* common linked list */
#include "cm_lib.x"        /* common library */
#include "cm5.x"           /* timer typedefs */
#include "cm_tpt.x"              /* common transport address */
#include "cm_dns.x"        /* Common DNS library */
#ifdef SB_FTHA
#include "sht.x"           /* SHT Interface header file */
#endif /* SB_FTHA */
#include "sct.x"           /* SCT interface */
#include "lsb.x"           /* layer management typedefs for SCTP layer */
#include "sb_mtu.x"
#include "sb.x"            /* typedefs for SCTP layer */


/* local defines */

/* local typedefs */

/* forward references */
EXTERN  S16 smSbActvTsk     ARGS((Pst *pst, Buffer *mBuf));


/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from SCTP layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   smsbexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smSbActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smSbActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;
   /*sb082.102: SIP changes*/
   TRC3(smSbActvTsk)
   ret = ROK;
  
   /*sb085.102: Removed the debug prints*/

   switch(pst->event)
   {
#ifdef LCSMSBMILSB
    
      case LSB_EVTCFGCFM:             /* Config confirm */
         ret = cmUnpkLsbCfgCfm(SmMiLsbCfgCfm, pst, mBuf);
         break;
      case LSB_EVTCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLsbCntrlCfm(SmMiLsbCntrlCfm, pst, mBuf);
         break;
      case LSB_EVTSTACFM:             /* Status Confirm */
         ret = cmUnpkLsbStaCfm(SmMiLsbStaCfm, pst, mBuf);
         break;
      case LSB_EVTSTSCFM:             /* Statistics Confirm */
         ret = cmUnpkLsbStsCfm(SmMiLsbStsCfm, pst, mBuf);
         break;
      case LSB_EVTSTAIND:             /* Status Indication */
           /*sb085.102: Removed the debug prints*/

            ret = cmUnpkLsbStaInd(SmMiLsbStaInd, pst, mBuf);
         break;
      case LSB_EVTTRCIND:             /* Trace Indication */
         ret = cmUnpkLsbTrcInd(SmMiLsbTrcInd, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         break;
   }
   SExitTsk();
   RETVALUE(ret);
} /* end of smSbActvTsk */


/********************************************************************30**

         End of file:     smsbexms.c@@/main/4 - Thu Jul 26 07:54:02 2007

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
1.1          ---      wvdl 1. Initial version
/main/3      ---      sb   1. Modified for SCTP release based on 
                              RFC-2960 'Oct 2000
          sb042.102   hl   1. Added change for SHT interface and Rolling
                              UpGrade

          sb082.102   kp   1. Added changes for SIP.
          sb085.102   kp   1. Removed the debug prints.
/main/4      ---   rsr/ag  1. Updated for Release of 1.3
*********************************************************************91*/
