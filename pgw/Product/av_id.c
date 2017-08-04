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
  
     Name:     Trillium LTE PGW Application
  
     Type:     C Source file
  
     Desc:     product id file

     File:     av_id.c

     Sid:      av_id.c@@/main/1 - Tue Aug 17 18:54:33 2010
  
     Prg:      ve
  
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
#define AVSWMV 1                                /* main version         */
#define AVSWMR 1                                /* main revision        */
#define AVSWBV 0                                /* branch version       */
/* av006.101: patch release */
#define AVSWBR 6                                /* branch revision      */
#define AVSWPN "1000419"                        /* part number          */

PRIVATE CONSTANT SystemId sId =
{
   AVSWMV,                                      /* main version         */
   AVSWMR,                                      /* main revision        */
   AVSWBV,                                      /* branch version       */ 
   AVSWBR,                                      /* branch revision      */
   AVSWPN                                       /* part number          */
};

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

PUBLIC S16 avGetSysId   ARGS((SystemId *sid));


/*
 *
 *      Fun  : avGetSysId
 *
 *      Desc : 
 *            
 *
 *      Ret  : ROK     - successful.
 *             RFAILED - failed.
 *
 *      Notes: None.
 *
 *      File : av_id.c
 *
 */
#ifdef ANSI
PUBLIC S16 avGetSysId
(
SystemId        *sid                            /* write sys id here    */
)
#else
PUBLIC S16 avGetSysId(sid)
SystemId        *sid;                           /* write sys id here    */
#endif
{

   TRC2(avGetSysId)

   /* write system identification information to specified buf  */
   sid->mVer  = sId.mVer;
   sid->mRev  = sId.mRev;
   sid->bVer  = sId.bVer;
   sid->bRev  = sId.bRev;
   sid->ptNmb = sId.ptNmb;
   RETVALUE(ROK);
} /* end of avGetSysId */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */

/********************************************************************30**
  
         End of file:     av_id.c@@/main/1 - Tue Aug 17 18:54:33 2010

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
/main/1      -          ve                 1.LTE-CNE:PGW Initial Release. 
/main/1      av001.101  rp                 1.Patch av001.101 release
/main/1      av002.101  rk                 1.Patch av002.101 release
/main/1      av003.101  ve                 1.Patch av003.101 release
/main/1      av004.101  pmacharla          1.Patch av004.101 release
/main/1      av005.101  pbyadgi            1.Patch av005.101 release
/main/1      av006.101  akaranth           1.Patch av006.101 release
*********************************************************************91*/
