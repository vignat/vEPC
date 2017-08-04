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

     Name:     PGW Layer error file.

     Type:     C include file

     Desc:     Error Hash Defines required by PGW layer

     File:     av_err.h

     Sid:      av_err.h@@/main/1 - Tue Aug 17 18:54:32 2010

     Prg:      sp

*********************************************************************21*/

#ifndef __AVERRH__
#define __AVERRH__


#define   EAVBASE      0
#define   ERRAV       (EAVBASE   +0)    /* reserved */
#define   EAV000      ERRAV             /* reserved */
#define   EAV001      (ERRAV +    1)    /*           :w: 179 */

#define   EAV002      (ERRAV +    2)    /*   jg_ex_ms.c: 232 */
#define   EAV003      (ERRAV +    3)    /*   jg_ex_ms.c: 288 */
#define   EAV004      (ERRAV +    4)    /*   jg_ex_ms.c: 303 */
#define   EAV005      (ERRAV +    5)    /*   jg_ex_ms.c: 365 */
#define   EAV006      (ERRAV +    6)    /*   jg_ex_ms.c: 375 */
#define   EAV007      (ERRAV +    7)    /*   jg_ex_ms.c: 389 */

#define   EAV008      (ERRAV +    8)    /*     jg_lim.c: 183 */
#define   EAV009      (ERRAV +    9)    /*     jg_lim.c: 189 */
#define   EAV010      (ERRAV +   10)    /*     jg_lim.c: 289 */
#define   EAV011      (ERRAV +   11)    /*     jg_lim.c: 548 */
#define   EAV012      (ERRAV +   12)    /*     jg_lim.c: 650 */
#define   EAV013      (ERRAV +   13)    /*     jg_lim.c: 670 */
#define   EAV014      (ERRAV +   14)    /*     jg_lim.c: 819 */
#define   EAV015      (ERRAV +   15)    /*     jg_lim.c: 827 */
#define   EAV016      (ERRAV +   16)    /*     jg_lim.c: 851 */
#define   EAV017      (ERRAV +   17)    /*     jg_lim.c: 864 */
#define   EAV018      (ERRAV +   18)    /*     jg_lim.c:1007 */
#define   EAV019      (ERRAV +   19)    /*     jg_lim.c:1026 */
#define   EAV020      (ERRAV +   20)    /*     jg_lim.c:1046 */
#define   EAV021      (ERRAV +   21)    /*     jg_lim.c:1066 */

#define   EAV022      (ERRAV +   22)    /*      jg_mi.c: 348 */
#define   EAV023      (ERRAV +   23)    /*      jg_mi.c: 382 */
#define   EAV024      (ERRAV +   24)    /*      jg_mi.c: 522 */
#define   EAV025      (ERRAV +   25)    /*      jg_mi.c: 529 */
#define   EAV026      (ERRAV +   26)    /*      jg_mi.c: 744 */
#define   EAV027      (ERRAV +   27)    /*      jg_mi.c: 751 */
#define   EAV028      (ERRAV +   28)    /*      jg_mi.c: 758 */
#define   EAV029      (ERRAV +   29)    /*      jg_mi.c: 765 */
#define   EAV030      (ERRAV +   30)    /*      jg_mi.c: 812 */
#define   EAV031      (ERRAV +   31)    /*      jg_mi.c: 855 */
#define   EAV032      (ERRAV +   32)    /*      jg_mi.c: 904 */
#define   EAV033      (ERRAV +   33)    /*      jg_mi.c: 950 */
#define   EAV034      (ERRAV +   34)    /*      jg_mi.c:1002 */
#define   EAV035      (ERRAV +   35)    /*      jg_mi.c:1046 */
#define   EAV036      (ERRAV +   36)    /*      jg_mi.c:1142 */
#define   EAV037      (ERRAV +   37)    /*      jg_mi.c:1345 */
#define   EAV038      (ERRAV +   38)    /*      jg_mi.c:1504 */
#define   EAV039      (ERRAV +   39)    /*      jg_mi.c:1516 */
#define   EAV040      (ERRAV +   40)    /*      jg_mi.c:1530 */
#define   EAV041      (ERRAV +   41)    /*      jg_mi.c:1540 */
#define   EAV042      (ERRAV +   42)    /*      jg_mi.c:1585 */
#define   EAV043      (ERRAV +   43)    /*      jg_mi.c:1595 */
#define   EAV044      (ERRAV +   44)    /*      jg_mi.c:1605 */
#define   EAV045      (ERRAV +   45)    /*      jg_mi.c:1624 */
#define   EAV046      (ERRAV +   46)    /*      jg_mi.c:1688 */
#define   EAV047      (ERRAV +   47)    /*      jg_mi.c:1716 */
#define   EAV048      (ERRAV +   48)    /*      jg_mi.c:1861 */

#define   EAV049      (ERRAV +   49)    /*     jg_tmr.c: 179 */

#define   EAV050      (ERRAV +   50)    /* jgsm_ex_ms.c: 236 */
#define   EAV051      (ERRAV +   51)    /* jgsm_ex_ms.c: 242 */

#endif /* __AVERRH__ */



/********************************************************************30**

         End of file:     av_err.h@@/main/1 - Tue Aug 17 18:54:32 2010

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
/main/1      -      sp                 LTE-CNE:PGW Initial Release
*********************************************************************91*/
