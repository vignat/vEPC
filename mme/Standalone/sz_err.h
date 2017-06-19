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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for error definitions

     File:     sz_err.h

     Sid:      sz_err.h@@/main/4 - Tue May  8 16:04:57 2012

     Prg:      rs

*********************************************************************21*/

#ifndef __SZERRH__
#define __SZERRH__

#ifndef SLOGERROR
#define SLOGERROR  SLogError
#endif  /* SLOGERROR */

#ifdef SS_MULTIPLE_PROCS

#define SZLOGERROR(errCls, errCode, errVal, errDesc) \
        SLogError(szCbPtr->init.ent, szCbPtr->init.inst, szCbPtr->init.procId, \
                   __FILE__, __LINE__, errCls, errCode, errVal, errDesc)

#if (ERRCLASS & ERRCLS_DEBUG)    
#define SZLOGERROR_DEBUG(errCode, errVal, errDesc) \
        SLogError(szCbPtr->init.ent, szCbPtr->init.inst, szCbPtr->init.procId, \
                   __FILE__, __LINE__, ERRCLS_DEBUG, errCode, errVal, errDesc)
#else  /* (ERRCLASS & ERRCLS_DEBUG) */
#define SZLOGERROR_DEBUG(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

#if (ERRCLASS & ERRCLS_INT_PAR)    
#define SZLOGERROR_INTFC(errCode, errVal, errDesc) \
        SLogError(szCbPtr->init.ent, szCbPtr->init.inst, szCbPtr->init.procId, \
                   __FILE__, __LINE__, ERRCLS_INT_PAR, errCode, errVal, errDesc)
#else   /* (ERRCLASS & ERRCLS_INT_PAR) */
#define SZLOGERROR_INTFC(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

#if (ERRCLASS & ERRCLS_ADD_RES)    
#define SZLOGERROR_ADD_RES(errCode, errVal, errDesc) \
        SLogError(szCbPtr->init.ent, szCbPtr->init.inst, szCbPtr->init.procId, \
                   __FILE__, __LINE__, ERRCLS_ADD_RES, errCode, errVal, errDesc)
#else  /* (ERRCLASS & ERRCLS_ADD_RES) */
#define SZLOGERROR_ADD_RES(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */

#else /* SS_MULTIPLE_PROCS */

#define SZLOGERROR(errCls, errCode, errVal, errDesc) \
        SLogError(szCb.init.ent, szCb.init.inst, szCb.init.procId, \
                   __FILE__, __LINE__, errCls, errCode, errVal, errDesc)

#if (ERRCLASS & ERRCLS_DEBUG)    
#define SZLOGERROR_DEBUG(errCode, errVal, errDesc) \
        SLogError(szCb.init.ent, szCb.init.inst, szCb.init.procId, \
                   __FILE__, __LINE__, ERRCLS_DEBUG, errCode, errVal, errDesc)
#else  /* (ERRCLASS & ERRCLS_DEBUG) */ 
#define SZLOGERROR_DEBUG(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

#if (ERRCLASS & ERRCLS_INT_PAR)    
#define SZLOGERROR_INTFC(errCode, errVal, errDesc) \
        SLogError(szCb.init.ent, szCb.init.inst, szCb.init.procId, \
                   __FILE__, __LINE__, ERRCLS_INT_PAR, errCode, errVal, errDesc)
#else  /* (ERRCLASS & ERRCLS_INT_PAR) */
#define SZLOGERROR_INTFC(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

#if (ERRCLASS & ERRCLS_ADD_RES)    
#define SZLOGERROR_ADD_RES(errCode, errVal, errDesc) \
        SLogError(szCb.init.ent, szCb.init.inst, szCb.init.procId, \
                   __FILE__, __LINE__, ERRCLS_ADD_RES, errCode, errVal, errDesc)
#else  /* (ERRCLASS & ERRCLS_ADD_RES) */
#define SZLOGERROR_ADD_RES(errCode, errVal, errDesc) 
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */

#endif /* SS_MULTIPLE_PROCS */

#define   ERRSZ       0
#define   ESZXXX      ERRSZ

#define   ESZ001      (ERRSZ +    1)    /*   sq_ex_ms.c: 255 */
#define   ESZ002      (ERRSZ +    2)    /*   sq_ex_ms.c: 368 */
#define   ESZ003      (ERRSZ +    3)    /*   sq_ex_ms.c: 373 */
#define   ESZ004      (ERRSZ +    4)    /*   sq_ex_ms.c: 433 */
#define   ESZ005      (ERRSZ +    5)    /*   sq_ex_ms.c: 465 */

#define   ESZ006      (ERRSZ +    6)    /*      sq_li.c: 386 */
#define   ESZ007      (ERRSZ +    7)    /*      sq_li.c: 403 */
#define   ESZ008      (ERRSZ +    8)    /*      sq_li.c: 529 */
#define   ESZ009      (ERRSZ +    9)    /*      sq_li.c: 575 */
#define   ESZ010      (ERRSZ +   10)    /*      sq_li.c: 686 */
#define   ESZ011      (ERRSZ +   11)    /*      sq_li.c: 704 */
#define   ESZ012      (ERRSZ +   12)    /*      sq_li.c: 849 */
#define   ESZ013      (ERRSZ +   13)    /*      sq_li.c: 903 */
#define   ESZ014      (ERRSZ +   14)    /*      sq_li.c: 921 */
#define   ESZ015      (ERRSZ +   15)    /*      sq_li.c: 932 */
#define   ESZ016      (ERRSZ +   16)    /*      sq_li.c: 943 */
#define   ESZ017      (ERRSZ +   17)    /*      sq_li.c: 956 */
#define   ESZ018      (ERRSZ +   18)    /*      sq_li.c:1066 */
#define   ESZ019      (ERRSZ +   19)    /*      sq_li.c:1084 */
#define   ESZ020      (ERRSZ +   20)    /*      sq_li.c:1097 */
#define   ESZ021      (ERRSZ +   21)    /*      sq_li.c:1113 */
#define   ESZ022      (ERRSZ +   22)    /*      sq_li.c:1220 */
#define   ESZ023      (ERRSZ +   23)    /*      sq_li.c:1242 */
#define   ESZ024      (ERRSZ +   24)    /*      sq_li.c:1254 */
#define   ESZ025      (ERRSZ +   25)    /*      sq_li.c:1349 */
#define   ESZ026      (ERRSZ +   26)    /*      sq_li.c:1367 */
#define   ESZ027      (ERRSZ +   27)    /*      sq_li.c:1378 */
#define   ESZ028      (ERRSZ +   28)    /*      sq_li.c:1390 */
#define   ESZ029      (ERRSZ +   29)    /*      sq_li.c:1426 */
#define   ESZ030      (ERRSZ +   30)    /*      sq_li.c:1446 */
#define   ESZ031      (ERRSZ +   31)    /*      sq_li.c:1469 */
#define   ESZ032      (ERRSZ +   32)    /*      sq_li.c:1620 */
#define   ESZ033      (ERRSZ +   33)    /*      sq_li.c:1638 */
#define   ESZ034      (ERRSZ +   34)    /*      sq_li.c:1650 */
#define   ESZ035      (ERRSZ +   35)    /*      sq_li.c:1672 */
#define   ESZ036      (ERRSZ +   36)    /*      sq_li.c:1748 */
#define   ESZ037      (ERRSZ +   37)    /*      sq_li.c:1766 */
#define   ESZ038      (ERRSZ +   38)    /*      sq_li.c:1778 */
#define   ESZ039      (ERRSZ +   39)    /*      sq_li.c:1831 */
#define   ESZ040      (ERRSZ +   40)    /*      sq_li.c:1941 */
#define   ESZ041      (ERRSZ +   41)    /*      sq_li.c:1963 */
#define   ESZ042      (ERRSZ +   42)    /*      sq_li.c:1975 */
#define   ESZ043      (ERRSZ +   43)    /*      sq_li.c:1997 */
#define   ESZ044      (ERRSZ +   44)    /*      sq_li.c:2084 */
#define   ESZ045      (ERRSZ +   45)    /*      sq_li.c:2102 */
#define   ESZ046      (ERRSZ +   46)    /*      sq_li.c:2114 */
#define   ESZ047      (ERRSZ +   47)    /*      sq_li.c:2127 */
#define   ESZ048      (ERRSZ +   48)    /*      sq_li.c:2143 */
#define   ESZ049      (ERRSZ +   49)    /*      sq_li.c:2211 */
#define   ESZ050      (ERRSZ +   50)    /*      sq_li.c:2229 */
#define   ESZ051      (ERRSZ +   51)    /*      sq_li.c:2241 */
#define   ESZ052      (ERRSZ +   52)    /*      sq_li.c:2256 */
#define   ESZ053      (ERRSZ +   53)    /*      sq_li.c:2291 */
#define   ESZ054      (ERRSZ +   54)    /*      sq_li.c:2514 */
#define   ESZ055      (ERRSZ +   55)    /*      sq_li.c:2536 */
#define   ESZ056      (ERRSZ +   56)    /*      sq_li.c:2548 */
#define   ESZ057      (ERRSZ +   57)    /*      sq_li.c:2565 */
#define   ESZ058      (ERRSZ +   58)    /*      sq_li.c:2635 */
#define   ESZ059      (ERRSZ +   59)    /*      sq_li.c:2995 */
#define   ESZ060      (ERRSZ +   60)    /*      sq_li.c:3005 */
#define   ESZ061      (ERRSZ +   61)    /*      sq_li.c:3047 */
#define   ESZ062      (ERRSZ +   62)    /*      sq_li.c:3145 */
#define   ESZ063      (ERRSZ +   63)    /*      sq_li.c:3179 */

#define   ESZ064      (ERRSZ +   64)    /*    sq_mgmt.c: 629 */
#define   ESZ065      (ERRSZ +   65)    /*    sq_mgmt.c: 753 */
#define   ESZ066      (ERRSZ +   66)    /*    sq_mgmt.c: 774 */
#define   ESZ067      (ERRSZ +   67)    /*    sq_mgmt.c: 822 */
#define   ESZ068      (ERRSZ +   68)    /*    sq_mgmt.c: 844 */
#define   ESZ069      (ERRSZ +   69)    /*    sq_mgmt.c:1322 */
#define   ESZ070      (ERRSZ +   70)    /*    sq_mgmt.c:1334 */
#define   ESZ071      (ERRSZ +   71)    /*    sq_mgmt.c:1390 */
#define   ESZ072      (ERRSZ +   72)    /*    sq_mgmt.c:1491 */
#define   ESZ073      (ERRSZ +   73)    /*    sq_mgmt.c:1511 */

#define   ESZ074      (ERRSZ +   74)    /*      sq_mi.c: 373 */
#define   ESZ075      (ERRSZ +   75)    /*      sq_mi.c: 393 */
#define   ESZ076      (ERRSZ +   76)    /*      sq_mi.c: 445 */
#define   ESZ077      (ERRSZ +   77)    /*      sq_mi.c: 506 */
#define   ESZ078      (ERRSZ +   78)    /*      sq_mi.c: 528 */
#define   ESZ079      (ERRSZ +   79)    /*      sq_mi.c: 570 */
#define   ESZ080      (ERRSZ +   80)    /*      sq_mi.c: 638 */
#define   ESZ081      (ERRSZ +   81)    /*      sq_mi.c: 662 */
#define   ESZ082      (ERRSZ +   82)    /*      sq_mi.c: 692 */
#define   ESZ083      (ERRSZ +   83)    /*      sq_mi.c: 707 */
#define   ESZ084      (ERRSZ +   84)    /*      sq_mi.c: 716 */
#define   ESZ085      (ERRSZ +   85)    /*      sq_mi.c: 777 */
#define   ESZ086      (ERRSZ +   86)    /*      sq_mi.c: 799 */
#define   ESZ087      (ERRSZ +   87)    /*      sq_mi.c: 849 */
#define   ESZ088      (ERRSZ +   88)    /*      sq_mi.c: 910 */
#define   ESZ089      (ERRSZ +   89)    /*      sq_mi.c: 983 */
#define   ESZ090      (ERRSZ +   90)    /*      sq_mi.c:1011 */
#define   ESZ091      (ERRSZ +   91)    /*      sq_mi.c:1023 */
#define   ESZ092      (ERRSZ +   92)    /*      sq_mi.c:1960 */
#define   ESZ093      (ERRSZ +   93)    /*      sq_mi.c:1984 */
#define   ESZ094      (ERRSZ +   94)    /*      sq_mi.c:2001 */
#define   ESZ095      (ERRSZ +   95)    /*      sq_mi.c:2426 */
#define   ESZ096      (ERRSZ +   96)    /*      sq_mi.c:2458 */
#define   ESZ097      (ERRSZ +   97)    /*      sq_mi.c:2672 */
#define   ESZ098      (ERRSZ +   98)    /*      sq_mi.c:2684 */
#define   ESZ099      (ERRSZ +   99)    /*      sq_mi.c:2696 */
#define   ESZ100      (ERRSZ +  100)    /*      sq_mi.c:3096 */
#define   ESZ101      (ERRSZ +  101)    /*      sq_mi.c:3105 */
#define   ESZ102      (ERRSZ +  102)    /*      sq_mi.c:3165 */
#define   ESZ103      (ERRSZ +  103)    /*      sq_mi.c:3322 */
#define   ESZ104      (ERRSZ +  104)    /*      sq_mi.c:4089 */
#define   ESZ105      (ERRSZ +  105)    /*      sq_mi.c:4117 */
#define   ESZ106      (ERRSZ +  106)    /*      sq_mi.c:4130 */
#define   ESZ107      (ERRSZ +  107)    /*      sq_mi.c:4165 */
#define   ESZ108      (ERRSZ +  108)    /*      sq_mi.c:4521 */
#define   ESZ109      (ERRSZ +  109)    /*      sq_mi.c:4545 */

#define   ESZ110      (ERRSZ +  110)    /*   sq_msgfn.c: 307 */
#define   ESZ111      (ERRSZ +  111)    /*   sq_msgfn.c: 396 */

#define   ESZ112      (ERRSZ +  112)    /*     sq_ndb.c: 206 */
#define   ESZ113      (ERRSZ +  113)    /*     sq_ndb.c: 228 */
#define   ESZ114      (ERRSZ +  114)    /*     sq_ndb.c: 239 */
#define   ESZ115      (ERRSZ +  115)    /*     sq_ndb.c: 595 */
#define   ESZ116      (ERRSZ +  116)    /*     sq_ndb.c: 606 */
#define   ESZ117      (ERRSZ +  117)    /*     sq_ndb.c: 618 */
#define   ESZ118      (ERRSZ +  118)    /*     sq_ndb.c: 767 */
#define   ESZ119      (ERRSZ +  119)    /*     sq_ndb.c: 778 */
#define   ESZ120      (ERRSZ +  120)    /*     sq_ndb.c: 790 */
#define   ESZ121      (ERRSZ +  121)    /*     sq_ndb.c: 803 */
#define   ESZ122      (ERRSZ +  122)    /*     sq_ndb.c:1366 */
#define   ESZ123      (ERRSZ +  123)    /*     sq_ndb.c:1396 */

#define   ESZ124      (ERRSZ +  124)    /*    sq_ptli.c:1885 */

#define   ESZ125      (ERRSZ +  125)    /*    sq_ptmi.c: 645 */
#define   ESZ126      (ERRSZ +  126)    /*    sq_ptmi.c: 683 */
#define   ESZ127      (ERRSZ +  127)    /*    sq_ptmi.c: 723 */
#define   ESZ128      (ERRSZ +  128)    /*    sq_ptmi.c: 764 */
#define   ESZ129      (ERRSZ +  129)    /*    sq_ptmi.c: 804 */
#define   ESZ130      (ERRSZ +  130)    /*    sq_ptmi.c: 843 */
#define   ESZ131      (ERRSZ +  131)    /*    sq_ptmi.c: 883 */

#define   ESZ132      (ERRSZ +  132)    /*    sq_ptui.c:1021 */
#define   ESZ133      (ERRSZ +  133)    /*    sq_ptui.c:1058 */
#define   ESZ134      (ERRSZ +  134)    /*    sq_ptui.c:1095 */
#define   ESZ135      (ERRSZ +  135)    /*    sq_ptui.c:1132 */
#define   ESZ136      (ERRSZ +  136)    /*    sq_ptui.c:1169 */
#define   ESZ137      (ERRSZ +  137)    /*    sq_ptui.c:1206 */
#define   ESZ138      (ERRSZ +  138)    /*    sq_ptui.c:1243 */
#define   ESZ139      (ERRSZ +  139)    /*    sq_ptui.c:1281 */
#define   ESZ140      (ERRSZ +  140)    /*    sq_ptui.c:1319 */
#define   ESZ141      (ERRSZ +  141)    /*    sq_ptui.c:1356 */
#define   ESZ142      (ERRSZ +  142)    /*    sq_ptui.c:1393 */
#define   ESZ143      (ERRSZ +  143)    /*    sq_ptui.c:1432 */
#define   ESZ144      (ERRSZ +  144)    /*    sq_ptui.c:1469 */
#define   ESZ145      (ERRSZ +  145)    /*    sq_ptui.c:1507 */

#define   ESZ146      (ERRSZ +  146)    /*      sq_sm.c: 248 */
#define   ESZ147      (ERRSZ +  147)    /*      sq_sm.c: 355 */
#define   ESZ148      (ERRSZ +  148)    /*      sq_sm.c: 408 */
#define   ESZ149      (ERRSZ +  149)    /*      sq_sm.c: 603 */
#define   ESZ150      (ERRSZ +  150)    /*      sq_sm.c: 664 */
#define   ESZ151      (ERRSZ +  151)    /*      sq_sm.c: 677 */
#define   ESZ152      (ERRSZ +  152)    /*      sq_sm.c: 737 */
#define   ESZ153      (ERRSZ +  153)    /*      sq_sm.c: 750 */
#define   ESZ154      (ERRSZ +  154)    /*      sq_sm.c: 889 */
#define   ESZ155      (ERRSZ +  155)    /*      sq_sm.c: 904 */
#define   ESZ156      (ERRSZ +  156)    /*      sq_sm.c:1017 */
#define   ESZ157      (ERRSZ +  157)    /*      sq_sm.c:1192 */
#define   ESZ158      (ERRSZ +  158)    /*      sq_sm.c:1286 */
#define   ESZ159      (ERRSZ +  159)    /*      sq_sm.c:1334 */
#define   ESZ160      (ERRSZ +  160)    /*      sq_sm.c:1407 */
#define   ESZ161      (ERRSZ +  161)    /*      sq_sm.c:1477 */
#define   ESZ162      (ERRSZ +  162)    /*      sq_sm.c:1530 */
#define   ESZ163      (ERRSZ +  163)    /*      sq_sm.c:1673 */
#define   ESZ164      (ERRSZ +  164)    /*      sq_sm.c:1736 */

#define   ESZ165      (ERRSZ +  165)    /* sq_smlimme.c: 723 */
#define   ESZ166      (ERRSZ +  166)    /* sq_smlimme.c: 734 */

#define   ESZ167      (ERRSZ +  167)    /* sq_smuienb.c: 726 */
#define   ESZ168      (ERRSZ +  168)    /* sq_smuienb.c: 735 */

#define   ESZ169      (ERRSZ +  169)    /* sq_smuimme.c: 727 */

#define   ESZ170      (ERRSZ +  170)    /*     sq_tmr.c: 218 */
#define   ESZ171      (ERRSZ +  171)    /*     sq_tmr.c: 279 */
#define   ESZ172      (ERRSZ +  172)    /*     sq_tmr.c: 464 */

#define   ESZ173      (ERRSZ +  173)    /*      sq_ui.c: 237 */
#define   ESZ174      (ERRSZ +  174)    /*      sq_ui.c: 272 */
#define   ESZ175      (ERRSZ +  175)    /*      sq_ui.c: 334 */
#define   ESZ176      (ERRSZ +  176)    /*      sq_ui.c: 422 */
#define   ESZ177      (ERRSZ +  177)    /*      sq_ui.c: 507 */
#define   ESZ178      (ERRSZ +  178)    /*      sq_ui.c: 740 */
#define   ESZ179      (ERRSZ +  179)    /*      sq_ui.c: 998 */
#define   ESZ180      (ERRSZ +  180)    /*      sq_ui.c:1156 */
#define   ESZ181      (ERRSZ +  181)    /*      sq_ui.c:1211 */
#define   ESZ182      (ERRSZ +  182)    /*      sq_ui.c:1223 */
#define   ESZ183      (ERRSZ +  183)    /*      sq_ui.c:1382 */
#define   ESZ184      (ERRSZ +  184)    /*      sq_ui.c:1517 */
#define   ESZ185      (ERRSZ +  185)    /*      sq_ui.c:1743 */
#define   ESZ186      (ERRSZ +  186)    /*      sq_ui.c:2087 */
#define   ESZ187      (ERRSZ +  187)    /*      sq_ui.c:2265 */
#define   ESZ188      (ERRSZ +  188)    /*      sq_ui.c:2314 */
#define   ESZ189      (ERRSZ +  189)    /*      sq_ui.c:2339 */
#define   ESZ190      (ERRSZ +  190)    /*      sq_ui.c:2522 */
#define   ESZ191      (ERRSZ +  191)    /*      sq_ui.c:2556 */
#define   ESZ192      (ERRSZ +  192)    /*      sq_ui.c:2578 */
#define   ESZ193      (ERRSZ +  193)    /*      sq_ui.c:2633 */
#define   ESZ194      (ERRSZ +  194)    /*      sq_ui.c:2688 */
#define   ESZ195      (ERRSZ +  195)    /*      sq_ui.c:2744 */
#define   ESZ196      (ERRSZ +  196)    /*      sq_ui.c:2795 */
#define   ESZ197      (ERRSZ +  197)    /*      sq_ui.c:2851 */
#define   ESZ198      (ERRSZ +  198)    /*      sq_ui.c:2906 */
#define   ESZ199      (ERRSZ +  199)    /*      sq_ui.c:2961 */
#define   ESZ200      (ERRSZ +  200)    /*      sq_ui.c:3194 */
#define   ESZ201      (ERRSZ +  201)    /*      sq_ui.c:3216 */


#endif /* __SZERRH__ */

/**********************************************************************

         End of file:     sz_err.h@@/main/4 - Tue May  8 16:04:57 2012

**********************************************************************/

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
------------ -------- ---- ----------------------------------------------
/main/4      ---     va  1. Updated for S1AP release 4.1
*********************************************************************91*/
