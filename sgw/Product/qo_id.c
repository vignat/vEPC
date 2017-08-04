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
  
     Type:     C Source file
  
     Desc:     product id file

     File:     qo_id.c

     Sid:      qo_id.c@@/main/1 - Thu Jul  8 21:22:20 2010
  
     Prg:      ns
  
*********************************************************************21*/

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
#define QOSWMV 1                                /* main version         */
#define QOSWMR 1                                /* main revision        */
#define QOSWBV 0                                /* branch version       */
/* qo005.101: patch release */
#define QOSWBR 5                                /* branch revision      */
#define QOSWPN "1000420"                        /* part number          */

PRIVATE CONSTANT SystemId sId =
{
   QOSWMV,                                      /* main version         */
   QOSWMR,                                      /* main revision        */
   QOSWBV,                                      /* branch version       */ 
   QOSWBR,                                      /* branch revision      */
   QOSWPN                                       /* part number          */
};

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

PUBLIC S16 qoGetSysId   ARGS((SystemId *sid));


/*
 *
 *      Fun  : qoGetSysId
 *
 *      Desc : 
 *            
 *
 *      Ret  : ROK     - successful.
 *             RFAILED - failed.
 *
 *      Notes: None.
 *
 *      File : qo_id.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoGetSysId
(
SystemId        *sid                            /* write sys id here    */
)
#else
PUBLIC S16 qoGetSysId(sid)
SystemId        *sid;                           /* write sys id here    */
#endif
{

   TRC2(qoGetSysId)

   /* write system identification information to specified buf  */
   sid->mVer  = sId.mVer;
   sid->mRev  = sId.mRev;
   sid->bVer  = sId.bVer;
   sid->bRev  = sId.bRev;
   sid->ptNmb = sId.ptNmb;
   RETVALUE(ROK);
} /* end of qoGetSysId */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */

/********************************************************************30**
  
         End of file:     qo_id.c@@/main/1 - Thu Jul  8 21:22:20 2010

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
/main/1      -          sp                 1.LTE-CNE:SGW Initial Release. 
/main/1      qo001.101  rp                 1.Patch qo001.101 Release. 
/main/1      qo002.101  rk                 1.Patch qo002.101 Release. 
/main/1      qo003.101  pmacharla          1.Patch qo003.101 Release. 
/main/1      qo004.101  pbyadgi            1.Patch qo004.101 Release. 
/main/1      qo005.101  akaranth           1.Patch qo005.101 Release. 
*********************************************************************91*/
