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
  
     Name:     s1ap - id
  
     Type:     C source file
  
     Desc:     Structure for software part number, version and
               revision.

     File:     sq_id.c
  
     Sid:      sq_id.c@@/main/5 - Tue May  8 16:04:49 2012
 
     Prg:      sn
  
*********************************************************************21*/

/* header include files (.h) */

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */

#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */

/* header/extern include files (.x) */

#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */

/* defines */

#define SZSWMV 4             /* s1ap - main version */
#define SZSWMR 1             /* s1ap - main revision */
#define SZSWBV 0             /* s1ap - branch version */
#define SZSWBR 0             /* s1ap - branch revision */
#define SZSWPN "1000361"     /* s1ap - part number */

/* public variable declarations */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

EXTERN S16 szGetSId ARGS((SystemId *s));

#ifdef __cplusplus
}
#endif /* __cplusplus */ 

/* copyright banner */

PUBLIC CONSTANT Txt szBan1[] =
   {"(c) Copyright 2017-present Open Networking Laboratory."};

PUBLIC CONSTANT Txt szBan2[] =
   {"                 All rights reserved."};

/* system id */

PRIVATE CONSTANT SystemId sId ={
   SZSWMV,              /* s1ap - main version    */
   SZSWMR,              /* s1ap - main revision   */
   SZSWBV,              /* s1ap - branch version  */
   SZSWBR,              /* s1ap - branch revision */
   SZSWPN               /* s1ap - part number     */
};

/*
*     support functions
*/

/*
*
*       Fun:   szGetSId 
*
*       Desc:  Get system id consisting of part number, main version and
*              revision and branch version and branch.
*
*       Ret:   TRUE      - ok
*
*       Notes: None
*
*       File:  sq_id.c
*
*/

#ifdef ANSI
PUBLIC S16 szGetSId
(
SystemId *syId                 /* system id */
) 
#else /* ANSI */
PUBLIC S16 szGetSId(syId)
SystemId *syId;                /* system id */
#endif /* ANSI */ 
{
   TRC2(szGetSId);

   syId->mVer = sId.mVer;
   syId->mRev = sId.mRev;
   syId->bVer = sId.bVer;
   syId->bRev = sId.bRev;
   syId->ptNmb = sId.ptNmb;

   RETVALUE(TRUE);

} /* end of szGetSid */

/*********************************************************************
  
         End of file:     sq_id.c@@/main/5 - Tue May  8 16:04:49 2012
 
**********************************************************************/

/**********************************************************************
  
        Notes:
  
**********************************************************************/

/**********************************************************************

**********************************************************************/

/********************************************************************
  
        Revision history:
  
**********************************************************************/

/********************************************************************90**
 
     ver       pat    init                  description
----------- -------- ------ ----------------------------------------------
/main/5      ---      va   1. Updated for release 4.1
*********************************************************************91*/
