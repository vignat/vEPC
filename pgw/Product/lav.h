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
  
        Name:     LTE CNE Layer management interface
    
        Type:     C include file
  
        Desc:     This file contains defines required by LTE CNE layer 
                  management interface
 
        File:     lav.h

        Sid:      lav.h@@/main/1 - Tue Aug 17 18:55:17 2010
  
        Prg:      ve
  
*********************************************************************21*/
 

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      
 *     
 */

#ifndef __LAV_H__
#define __LAV_H__

/********************************************************************AV**
 Event types
*********************************************************************AV*/

/********************************************************************AV**
 Element values for AvMngmt structure
*********************************************************************AV*/
#define STAVGEN         0x01
#define STAVEGTPUSAP    0x02
#define STAVEGTPCSAP    0x04
#define STAVECHOSAP     0x08
#define STAVTRGARP      0x10
#define STAVPEER        0x20

/********************************************************************AV**
 * LTE CNE Node types             
 ********************************************************************AV */
#define LAV_NODE_PGW    2

/********************************************************************AV*
 * MME General configuration  
 ********************************************************************AV*/


/* All Current values configurable */

#define LAV_UES              5
#define LAV_PGW_TMR_RES   10

#define LAV_MAX_ENODEB_BIN    10
#define LAV_MAX_UECB_BIN      10

/*********************************************************************/
#define LAV_MAX_PGWUETUNL_BIN 10
#define LAV_MAX_PGWUE_BIN     10
/*********************************************************************/


/* SAP States */
#define LAV_SAP_CFG     0
#define LAV_SAP_ISBOUND 1
#define LAV_SAP_UBND    2


#define   ELAV000            0
#define   ERRLAV        ELAV000  
#define   ELAV001      (ERRLAV +    1)    /*        lav.c: 212 */
#define   ELAV002      (ERRLAV +    2)    /*        lav.c: 221 */
#define   ELAV003      (ERRLAV +    3)    /*        lav.c: 233 */
#define   ELAV004      (ERRLAV +    4)    /*        lav.c: 246 */
#define   ELAV005      (ERRLAV +    5)    /*        lav.c: 280 */
#define   ELAV006      (ERRLAV +    6)    /*        lav.c: 299 */
#define   ELAV007      (ERRLAV +    7)    /*        lav.c: 326 */
#define   ELAV008      (ERRLAV +    8)    /*        lav.c: 335 */
#define   ELAV009      (ERRLAV +    9)    /*        lav.c: 347 */
#define   ELAV010      (ERRLAV +   10)    /*        lav.c: 360 */
#define   ELAV011      (ERRLAV +   11)    /*        lav.c: 394 */
#define   ELAV012      (ERRLAV +   12)    /*        lav.c: 413 */
#define   ELAV013      (ERRLAV +   13)    /*        lav.c: 440 */
#define   ELAV014      (ERRLAV +   14)    /*        lav.c: 449 */
#define   ELAV015      (ERRLAV +   15)    /*        lav.c: 461 */
#define   ELAV016      (ERRLAV +   16)    /*        lav.c: 474 */
#define   ELAV017      (ERRLAV +   17)    /*        lav.c: 508 */
#define   ELAV018      (ERRLAV +   18)    /*        lav.c: 527 */
#define   ELAV019      (ERRLAV +   19)    /*        lav.c: 556 */
#define   ELAV020      (ERRLAV +   20)    /*        lav.c: 565 */
#define   ELAV021      (ERRLAV +   21)    /*        lav.c: 577 */
#define   ELAV022      (ERRLAV +   22)    /*        lav.c: 588 */
#define   ELAV023      (ERRLAV +   23)    /*        lav.c: 600 */
#define   ELAV024      (ERRLAV +   24)    /*        lav.c: 634 */
#define   ELAV025      (ERRLAV +   25)    /*        lav.c: 644 */
#define   ELAV026      (ERRLAV +   26)    /*        lav.c: 663 */
#define   ELAV027      (ERRLAV +   27)    /*        lav.c: 690 */
#define   ELAV028      (ERRLAV +   28)    /*        lav.c: 699 */
#define   ELAV029      (ERRLAV +   29)    /*        lav.c: 711 */
#define   ELAV030      (ERRLAV +   30)    /*        lav.c: 724 */
#define   ELAV031      (ERRLAV +   31)    /*        lav.c: 758 */
#define   ELAV032      (ERRLAV +   32)    /*        lav.c: 777 */
#define   ELAV033      (ERRLAV +   33)    /*        lav.c: 804 */
#define   ELAV034      (ERRLAV +   34)    /*        lav.c: 813 */
#define   ELAV035      (ERRLAV +   35)    /*        lav.c: 825 */
#define   ELAV036      (ERRLAV +   36)    /*        lav.c: 838 */
#define   ELAV037      (ERRLAV +   37)    /*        lav.c: 872 */
#define   ELAV038      (ERRLAV +   38)    /*        lav.c: 891 */
#define   ELAV039      (ERRLAV +   39)    /*        lav.c: 918 */
#define   ELAV040      (ERRLAV +   40)    /*        lav.c: 927 */
#define   ELAV041      (ERRLAV +   41)    /*        lav.c: 939 */
#define   ELAV042      (ERRLAV +   42)    /*        lav.c: 952 */
#define   ELAV043      (ERRLAV +   43)    /*        lav.c: 986 */
#define   ELAV044      (ERRLAV +   44)    /*        lav.c:1005 */
#define   ELAV045      (ERRLAV +   45)    /*        lav.c:1032 */
#define   ELAV046      (ERRLAV +   46)    /*        lav.c:1041 */
#define   ELAV047      (ERRLAV +   47)    /*        lav.c:1053 */
#define   ELAV048      (ERRLAV +   48)    /*        lav.c:1066 */
#define   ELAV049      (ERRLAV +   49)    /*        lav.c:1100 */
#define   ELAV050      (ERRLAV +   50)    /*        lav.c:1119 */
#define   ELAV051      (ERRLAV +   51)    /*        lav.c:1146 */
#define   ELAV052      (ERRLAV +   52)    /*        lav.c:1155 */
#define   ELAV053      (ERRLAV +   53)    /*        lav.c:1167 */
#define   ELAV054      (ERRLAV +   54)    /*        lav.c:1180 */
#define   ELAV055      (ERRLAV +   55)    /*        lav.c:1214 */
#define   ELAV056      (ERRLAV +   56)    /*        lav.c:1233 */
#define   ELAV057      (ERRLAV +   57)    /*        lav.c:1264 */
#define   ELAV058      (ERRLAV +   58)    /*        lav.c:1273 */
#define   ELAV059      (ERRLAV +   59)    /*        lav.c:1283 */
#define   ELAV060      (ERRLAV +   60)    /*        lav.c:1298 */
#define   ELAV061      (ERRLAV +   61)    /*        lav.c:1311 */
#define   ELAV062      (ERRLAV +   62)    /*        lav.c:1324 */
#define   ELAV063      (ERRLAV +   63)    /*        lav.c:1338 */
#define   ELAV064      (ERRLAV +   64)    /*        lav.c:1373 */
#define   ELAV065      (ERRLAV +   65)    /*        lav.c:1392 */


/* Layer manager event fields */

#define LAV_EVENT_SGW_EGTPC_BNDCFM    (LCM_EVENT_LYR_SPECIFIC + 1) 
#define LAV_EVENT_SGW_EGTPU_BNDCFM    (LCM_EVENT_LYR_SPECIFIC + 2) 
#define LAV_EVENT_SGW_ECHO_BNDCFM     (LCM_EVENT_LYR_SPECIFIC + 3) 
#define LAV_EVENT_PGW_ROUTECFM        (LCM_EVENT_LYR_SPECIFIC + 4) 

/* Layer manager cause fields */
#define LAV_CAUSE_BND_SUCC    1
#define LAV_CAUSE_ARP_SUCC    2

/* All Timer values required */
#define LAV_T3402      7200 /* Default 12 minutes */
#define LAV_T3450      60   /* Default 6 seconds */
#define LAV_T3460      60   /* Default 6 seconds */
#define LAV_T3470      60   /* Default 6 seconds */

/***********************************************************************
  Defines for Debug Masks
***********************************************************************/
#define LAV_DBGMASK_MEM                       (DBGMASK_LYR << 0)
#define LAV_DBGMASK_TRC                       (DBGMASK_LYR << 1)
#define LAV_DBGMASK_ERROR                     (DBGMASK_LYR << 2)
#define LAV_DBGMASK_INFO                      (DBGMASK_LYR << 3)

/***********************************************************************
  Defines related to events across the management interface
***********************************************************************/

#define  EVTLAVCFGREQ                   1
#define  EVTLAVSTSREQ                   2
#define  EVTLAVCNTRLREQ                 3
#define  EVTLAVSTAREQ                   4
#define  EVTLAVCFGCFM                   5
#define  EVTLAVSTSCFM                   6
#define  EVTLAVCNTRLCFM                 7
#define  EVTLAVSTACFM                   8
#define  EVTLAVSTAIND                   9
#define  EVTLAVTRCIND                   10


/********************************************************************CZ**
 Selector Values
*********************************************************************CZ*/

#define LAV_SEL_LC            0     /**< Loose coupling */
#define LAV_SEL_LWLC          1     /**< Light-weight loose coupling -
                                     * applicable only on upper interface */

#endif /* __LAV_H__ */

  
/********************************************************************30**
  
         End of file:     lav.h@@/main/1 - Tue Aug 17 18:55:17 2010
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
/main/1      ---      ve               1. LTE-CNE Initial Release.
*********************************************************************91*/
