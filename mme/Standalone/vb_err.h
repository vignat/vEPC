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
 
     Name:    

     Type:     C include file

     Desc:     Error Hash Defines required by VB layer

     File:     vb_err.h

     Sid:      vb_err.h@@/main/1 - Mon Jun 28 19:35:34 2010

     Prg:      sp

*********************************************************************21*/
  
#ifndef __VBERRH__
#define __VBERRH__

#define   EVBBASE     0
#define   ERRVB       EVBBASE
 
#define   EVB001      (ERRVB +    1)    /* vb_esm_gtp.c: 402 */
#define   EVB002      (ERRVB +    2)    /* vb_esm_gtp.c: 421 */
#define   EVB003      (ERRVB +    3)    /* vb_esm_gtp.c: 608 */

#define   EVB004      (ERRVB +    4)    /*      vb_mi.c: 272 */
#define   EVB005      (ERRVB +    5)    /*      vb_mi.c: 327 */
#define   EVB006      (ERRVB +    6)    /*      vb_mi.c: 361 */
#define   EVB007      (ERRVB +    7)    /*      vb_mi.c: 501 */
#define   EVB008      (ERRVB +    8)    /*      vb_mi.c: 510 */
#define   EVB009      (ERRVB +    9)    /*      vb_mi.c: 824 */
#define   EVB010      (ERRVB +   10)    /*      vb_mi.c: 832 */
#define   EVB011      (ERRVB +   11)    /*      vb_mi.c: 839 */
#define   EVB012      (ERRVB +   12)    /*      vb_mi.c: 846 */
#define   EVB013      (ERRVB +   13)    /*      vb_mi.c: 860 */
#define   EVB014      (ERRVB +   14)    /*      vb_mi.c: 867 */
#define   EVB015      (ERRVB +   15)    /*      vb_mi.c: 959 */
#define   EVB016      (ERRVB +   16)    /*      vb_mi.c:1135 */
#define   EVB017      (ERRVB +   17)    /*      vb_mi.c:1224 */
#define   EVB018      (ERRVB +   18)    /*      vb_mi.c:1438 */
#define   EVB019      (ERRVB +   19)    /*      vb_mi.c:1455 */
#define   EVB020      (ERRVB +   20)    /*      vb_mi.c:1467 */
#define   EVB021      (ERRVB +   21)    /*      vb_mi.c:1482 */

#define   EVB022      (ERRVB +   22)    /* vb_mme_ex_ms.c: 234 */
#define   EVB023      (ERRVB +   23)    /* vb_mme_ex_ms.c: 312 */
#define   EVB024      (ERRVB +   24)    /* vb_mme_ex_ms.c: 372 */

#define   EVB025      (ERRVB +   25)    /* vb_mme_lim.c: 202 */
#define   EVB026      (ERRVB +   26)    /* vb_mme_lim.c: 220 */
#define   EVB027      (ERRVB +   27)    /* vb_mme_lim.c: 412 */
#define   EVB028      (ERRVB +   28)    /* vb_mme_lim.c: 421 */
#define   EVB029      (ERRVB +   29)    /* vb_mme_lim.c: 468 */
#define   EVB030      (ERRVB +   30)    /* vb_mme_lim.c: 489 */
#define   EVB031      (ERRVB +   31)    /* vb_mme_lim.c: 499 */
#define   EVB032      (ERRVB +   32)    /* vb_mme_lim.c: 551 */
#define   EVB033      (ERRVB +   33)    /* vb_mme_lim.c: 594 */
#define   EVB034      (ERRVB +   34)    /* vb_mme_lim.c: 646 */
#define   EVB035      (ERRVB +   35)    /* vb_mme_lim.c: 697 */
#define   EVB036      (ERRVB +   36)    /* vb_mme_lim.c: 719 */
#define   EVB037      (ERRVB +   37)    /* vb_mme_lim.c: 740 */
#define   EVB038      (ERRVB +   38)    /* vb_mme_lim.c: 750 */
#define   EVB039      (ERRVB +   39)    /* vb_mme_lim.c: 945 */
#define   EVB040      (ERRVB +   40)    /* vb_mme_lim.c:1535 */
#define   EVB041      (ERRVB +   41)    /* vb_mme_lim.c:1550 */
#define   EVB042      (ERRVB +   42)    /* vb_mme_lim.c:1586 */

#define   EVB043      (ERRVB +   43)    /* vb_mme_utl.c: 601 */
#define   EVB044      (ERRVB +   44)    /* vb_mme_utl.c: 804 */
#define   EVB045      (ERRVB +   45)    /* vb_mme_utl.c: 841 */
#define   EVB046      (ERRVB +   46)    /* vb_mme_utl.c:1025 */
#define   EVB047      (ERRVB +   47)    /* vb_mme_utl.c:1325 */
#define   EVB048      (ERRVB +   48)    /* vb_mme_utl.c:1483 */
#define   EVB049      (ERRVB +   49)    /* vb_mme_utl.c:1550 */
#define   EVB050      (ERRVB +   50)    /* vb_mme_utl.c:1663 */
#define   EVB051      (ERRVB +   51)    /* vb_mme_utl.c:1689 */
#define   EVB052      (ERRVB +   52)    /* vb_mme_utl.c:1709 */
#define   EVB053      (ERRVB +   53)    /* vb_mme_utl.c:1729 */
#define   EVB054      (ERRVB +   54)    /* vb_mme_utl.c:1746 */
#define   EVB055      (ERRVB +   55)    /* vb_mme_utl.c:1767 */
#define   EVB056      (ERRVB +   56)    /* vb_mme_utl.c:1794 */
#define   EVB057      (ERRVB +   57)    /* vb_mme_utl.c:1875 */
#define   EVB058      (ERRVB +   58)    /* vb_mme_utl.c:1900 */
#define   EVB059      (ERRVB +   59)    /* vb_mme_utl.c:1979 */
#define   EVB060      (ERRVB +   60)    /* vb_mme_utl.c:2004 */
#define   EVB061      (ERRVB +   61)    /* vb_mme_utl.c:2049 */
#define   EVB062      (ERRVB +   62)    /* vb_mme_utl.c:2059 */
#define   EVB063      (ERRVB +   63)    /* vb_mme_utl.c:2081 */
#define   EVB064      (ERRVB +   64)    /* vb_mme_utl.c:2147 */
#define   EVB065      (ERRVB +   65)    /* vb_mme_utl.c:2166 */
#define   EVB066      (ERRVB +   66)    /* vb_mme_utl.c:2185 */
#define   EVB067      (ERRVB +   67)    /* vb_mme_utl.c:2236 */
#define   EVB068      (ERRVB +   68)    /* vb_mme_utl.c:2295 */
#define   EVB069      (ERRVB +   69)    /* vb_mme_utl.c:2319 */
#define   EVB070      (ERRVB +   70)    /* vb_mme_utl.c:2354 */
#define   EVB071      (ERRVB +   71)    /* vb_mme_utl.c:2414 */
#define   EVB072      (ERRVB +   72)    /* vb_mme_utl.c:2441 */
#define   EVB073      (ERRVB +   73)    /* vb_mme_utl.c:2567 */
#define   EVB074      (ERRVB +   74)    /* vb_mme_utl.c:2596 */
#define   EVB075      (ERRVB +   75)    /* vb_mme_utl.c:2658 */
#define   EVB076      (ERRVB +   76)    /* vb_mme_utl.c:2665 */
#define   EVB077      (ERRVB +   77)    /* vb_mme_utl.c:2690 */
#define   EVB078      (ERRVB +   78)    /* vb_mme_utl.c:2744 */
#define   EVB079      (ERRVB +   79)    /* vb_mme_utl.c:2751 */
#define   EVB080      (ERRVB +   80)    /* vb_mme_utl.c:2837 */
#define   EVB081      (ERRVB +   81)    /* vb_mme_utl.c:2843 */
#define   EVB082      (ERRVB +   82)    /* vb_mme_utl.c:2947 */
#define   EVB083      (ERRVB +   83)    /* vb_mme_utl.c:2954 */
#define   EVB084      (ERRVB +   84)    /* vb_mme_utl.c:3092 */
#define   EVB085      (ERRVB +   85)    /* vb_mme_utl.c:3099 */

#define   EVB086      (ERRVB +   86)    /*    vb_ptli.c: 982 */
#define   EVB087      (ERRVB +   87)    /*    vb_ptli.c:1020 */
#define   EVB088      (ERRVB +   88)    /*    vb_ptli.c:1057 */
#define   EVB089      (ERRVB +   89)    /*    vb_ptli.c:1095 */
#define   EVB090      (ERRVB +   90)    /*    vb_ptli.c:1133 */
#define   EVB091      (ERRVB +   91)    /*    vb_ptli.c:1171 */
#define   EVB092      (ERRVB +   92)    /*    vb_ptli.c:1209 */
#define   EVB093      (ERRVB +   93)    /*    vb_ptli.c:1247 */
#define   EVB094      (ERRVB +   94)    /*    vb_ptli.c:1285 */
#define   EVB095      (ERRVB +   95)    /*    vb_ptli.c:1323 */
#define   EVB096      (ERRVB +   96)    /*    vb_ptli.c:1362 */
#define   EVB097      (ERRVB +   97)    /*    vb_ptli.c:1647 */
#define   EVB098      (ERRVB +   98)    /*    vb_ptli.c:1685 */

#define   EVB099      (ERRVB +   99)    /*     vb_tmr.c: 358 */
#define   EVB100      (ERRVB +  100)    /*     vb_tmr.c: 700 */
#define   EVB101      (ERRVB +  101)    /*     vb_tmr.c: 359 */
#define   EVB102      (ERRVB +  102)    /*     vb_tmr.c: 702 */
#endif /* __VBERRH__ */
 


/********************************************************************30**
 
         End of file:     vb_err.h@@/main/1 - Mon Jun 28 19:35:34 2010

*********************************************************************31*/
 

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 

/********************************************************************60**
 
        Revision vbstory:
 
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
/main/1      ---      sp         1. initial release.
*********************************************************************91*/

