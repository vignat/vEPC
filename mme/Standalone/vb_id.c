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
  
     Name:     Core Network Emulator - MSC
  
     Type:     C Source Code.
  
     Desc:     This file contains femto access point identifiaction 
               information including version and revision information.

     File:     vb_id.c

     Sid:      vb_id.c@@/main/1 - Mon Jun 28 19:35:38 2010
  
     Prg:      ns
  
*********************************************************************21*/

/************************************************************************
 
     Note: 
 
************************************************************************/
/* Header include files (.h)            */
  
#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
 
/* header/extern include files (.x)     */

#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */

/* NOTE: 
 *
 *
 * 1. Related to Table 1, refer to respective <Sl No> below for 
 *    the dependent protocol used on LTE CNE <version and Patch numer>
 *
 *    Sl No 1
 *------------------------------------------------------------------ 
 *   Product   Prefix    Partno  version  gen   Patchlevel(GPR) 
 *------------------------------------------------------------------   
 *   S1AP         sz     1000361   2.1    01       02 
 *   TUCL         hi     1000158   2.1    00       23
 *   EGTP         eg     1000368   1.2    01       00
 *   SCTP         sb     1000191   1.3    00       28
 *   MTSS         mt     -------   3.1    --       00
 *------------------------------------------------------------------   
 */



/* local defines and global extern declerations */
#define VBSWMV 1                                /* main version         */
#define VBSWMR 1                                /* main revision        */
#define VBSWBV 0                                /* branch version       */
#define VBSWBR 7                                /* branch revision: vb007.101 
                                                   Patch 5 for MME 1.1 */
#define VBSWPN "1000417"                        /* part number          */

PRIVATE CONSTANT SystemId sId =
{
   VBSWMV,                                      /* main version         */
   VBSWMR,                                      /* main revision        */
   VBSWBV,                                      /* branch version       */ 
   VBSWBR,                                      /* branch revision      */
   /* vb007.101: MOD: Fixed warning for g++ compilation on Linux */
   VBSWPN                                /* part number          */
   /* vb007.101: MOD_END: Fixed warning for g++ compilation on Linux */
};

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

PUBLIC S16 vbGetSysId   ARGS((SystemId *sid));


/*
 *
 *      Fun  : vbGetSysId
 *
 *      Desc : 
 *            
 *
 *      Ret  : ROK     - successful.
 *             RFAILED - failed.
 *
 *      Notes: None.
 *
 *      File : vb_id.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbGetSysId
(
SystemId        *sid                            /* write sys id here    */
)
#else
PUBLIC S16 vbGetSysId(sid)
SystemId        *sid;                           /* write sys id here    */
#endif
{

   TRC2(vbGetSysId)

   /* write system identification information to specified buf  */
   sid->mVer  = sId.mVer;
   sid->mRev  = sId.mRev;
   sid->bVer  = sId.bVer;
   sid->bRev  = sId.bRev;
   sid->ptNmb = sId.ptNmb;
   RETVALUE(ROK);
} /* end of vbGetSysId */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */

/********************************************************************30**
  
         End of file:     vb_id.c@@/main/1 - Mon Jun 28 19:35:38 2010

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
/main/1      -          sp            1. LTE-CNE Initial Release.
/main/1  vb001.101      rk            1. Patch 1 for MME Release 1.1
/main/1  vb002.101      rk            1. Patch 2 for MME Release 1.1
/main/1  vb003.101      cshekar       1. Patch 3 ueRad Capability support.
/main/1  vb004.101      ve            1. Patch 4 for MME Release 1.1.
/main/1  vb005.101      rk            1. Patch 5 for MME Release 1.1.
/main/1  vb006.101     ragrawal       1. Patch 6 for MME Release 1.1.
/main/1  vb007.101     akaranth       1. Patch 7 for MME Release 1.1.
*********************************************************************91*/
