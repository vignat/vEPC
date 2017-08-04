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

     Name:     SGW Layer error file     

     Type:     C include file

     Desc:     Error Hash Defines required by SGW layer

     File:     qo_err.h

     Sid:      qo_err.h@@/main/1 - Thu Jul  8 21:22:18 2010

     Prg:      sp

*********************************************************************21*/
#ifndef __QOERRH__
#define __QOERRH__


#define   EQOBASE     0
#define   ERRQO       (EQOBASE   +0)    /* reserved */
/* so003.202 : Updated for SIP FTHA */
#define   EQO000      ERRQO             /* reserved */


#define   EQO001      (ERRQO +    1)    /*         qo.h: 159 */

#define   EQO002      (ERRQO +    2)    /*      qo_mi.c: 316 */
#define   EQO003      (ERRQO +    3)    /*      qo_mi.c: 357 */
#define   EQO004      (ERRQO +    4)    /*      qo_mi.c: 502 */
#define   EQO005      (ERRQO +    5)    /*      qo_mi.c: 511 */
#define   EQO006      (ERRQO +    6)    /*      qo_mi.c: 688 */
#define   EQO007      (ERRQO +    7)    /*      qo_mi.c: 696 */
#define   EQO008      (ERRQO +    8)    /*      qo_mi.c: 703 */
#define   EQO009      (ERRQO +    9)    /*      qo_mi.c: 710 */
#define   EQO010      (ERRQO +   10)    /*      qo_mi.c: 717 */
#define   EQO011      (ERRQO +   11)    /*      qo_mi.c: 870 */
#define   EQO012      (ERRQO +   12)    /*      qo_mi.c: 913 */
#define   EQO013      (ERRQO +   13)    /*      qo_mi.c: 956 */
#define   EQO014      (ERRQO +   14)    /*      qo_mi.c: 999 */
#define   EQO015      (ERRQO +   15)    /*      qo_mi.c:1091 */
#define   EQO016      (ERRQO +   16)    /*      qo_mi.c:1240 */
#define   EQO017      (ERRQO +   17)    /*      qo_mi.c:1376 */
#define   EQO018      (ERRQO +   18)    /*      qo_mi.c:1525 */
#define   EQO019      (ERRQO +   19)    /*      qo_mi.c:1778 */
#define   EQO020      (ERRQO +   20)    /*      qo_mi.c:1790 */
#define   EQO021      (ERRQO +   21)    /*      qo_mi.c:1804 */
#define   EQO022      (ERRQO +   22)    /*      qo_mi.c:1814 */
#define   EQO023      (ERRQO +   23)    /*      qo_mi.c:1868 */

#define   EQO024      (ERRQO +   24)    /* qosm_ex_ms.c: 231 */
#define   EQO025      (ERRQO +   25)    /* qosm_ex_ms.c: 237 */

#endif /* __QOERRH__ */



/********************************************************************30**

         End of file:     qo_err.h@@/main/1 - Thu Jul  8 21:22:18 2010

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
/main/1      ---        sp                  1.LTE-CNE:SGW Initial Release
*********************************************************************91*/
