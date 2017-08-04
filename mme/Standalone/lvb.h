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
 
        File:     lvb.h

        Sid:      lvb.h@@/main/nodal_main/2 - Wed Dec 18 15:02:02 2013
  
        Prg:      ap
  
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

#ifndef __LVB_H__
#define __LVB_H__

/********************************************************************VB**
 Event types
*********************************************************************VB*/

/********************************************************************VB**
 Element values for VbMngmt structure
*********************************************************************VB*/
#define STVBGEN         0x01
#define STVBSZTSAP      0x02
#define STVBEGTSAP      0x04
/********************************************************************VB**
 * LTE CNE Node types             
 ********************************************************************VB */
#define LVB_NODE_MME    0

/********************************************************************VB*
 * MME General configuration  
 ********************************************************************VB*/

/* All Maximum limits */
#define LVB_MAX_ENBS         2
#define LVB_MAX_MCC_LEN      3
#define LVB_MAX_MNC_LEN      3
#define LVB_MAX_MMECODE_LEN  3
#define LVB_MME_TMR_RES      10
#define LVB_MAX_PLMN_LIST    15
#define LVB_MAX_TAI_LIST     10
#define LVB_MME_MAX_NAME     150
#define LVB_MAX_CODE_LIST    256 
#define LVB_MAX_SERVED_GUMMEI_LIST 8
/* As per the Spec this value is 65535 */
#define LVB_MAX_GRPID_LIST   15

#define LVB_MAX_PLMN_ID_LEN   3
#define LVB_MAX_ENB_ID_LEN    3

/* All Current values configurable */

#define LVB_SZT_SAPS         1
#define LVB_EGT_SAPS         4
#define LVB_ENBS             1
#define LVB_UES              5
#define LVB_GUMMEIS          1 
#define LVB_PLMNS            1
#define LVB_GROUPIDS         1 
#define LVB_MMECODES         1 
#define LVB_TAIS             1

#define LVB_PLMN_DIG0        0x00
#define LVB_PLMN_DIG1        0x01
#define LVB_PLMN_DIG2        0x02

#define LVB_LAC_DIG0         0x00
#define LVB_LAC_DIG1         0x00
#define LVB_LAC_DIG2         0x00

#define LVB_RAC_DIG0         0x10
#define LVB_RAC_DIG1         0x11
#define LVB_RAC_DIG2         0x12

#define LVB_MCC_DIG0        VBSM_MCC_DIG0 
#define LVB_MCC_DIG1        VBSM_MCC_DIG1 
#define LVB_MCC_DIG2        VBSM_MCC_DIG2 

#define LVB_MNC_DIG0        VBSM_MNC_DIG0 
#define LVB_MNC_DIG1        VBSM_MNC_DIG1 
/* lvb_h_001.main_1: Added MNC DIG2 */
#define LVB_MNC_DIG2        VBSM_MNC_DIG2 
#define LVB_MNC_DIG2_PRES   VBSM_MNC_DIG2_PRES /* lvb_h_001.main_4 : 0-not pres 1-pres */

#define LVB_LAC              0x0A0B
#define LVB_RAC              1
#define LVB_TA_CODE          1

/* Served GUMMEIs           1..<maxnoofRATs>
The LTE related pool configuration is included on the first place in the list. 

This index specifies the first index value in the SERVED GUMMEI List by MME. 
User/Developer of MME Group ID, MME Code and MME 's MCC and MNC shall always 
be accessed from this index only as given by spec 36.413 8.6 version above */
#define LVB_MME_SELF_GUMMEI_IDX       0
#define LVB_MME_SELF_BASE_GRPID_INDX       0
#define LVB_MME_SELF_BASE_CODE_INDX       0

#define LVB_MME_GROUP_ID        1
#define LVB_MME_CODE            1

#define LVB_MME_NAME          "www.ccpu.com"
#define LVB_MME_REL_CAP       1

#define LVB_MAX_SZTSAP_TMRS   10
#define LVB_MAX_EGTSAP_TMRS   2

/* SAP States */
#define LVB_SAP_CFG     0
#define LVB_SAP_ISBOUND 1
#define LVB_SAP_UBND    2


/*Layer manager error codes */
#define   ELVBBASE     0
#define   ELVBXXX      (ELVBBASE   +0)    /* reserved */
#define   ERRLVB       (ELVBBASE   +0)    /* reserved */

#define   ELVB001      (ERRLVB +    1)    /*        lvb.c: 228 */
#define   ELVB002      (ERRLVB +    2)    /*        lvb.c: 237 */
#define   ELVB003      (ERRLVB +    3)    /*        lvb.c: 249 */
#define   ELVB004      (ERRLVB +    4)    /*        lvb.c: 262 */
#define   ELVB005      (ERRLVB +    5)    /*        lvb.c: 296 */
#define   ELVB006      (ERRLVB +    6)    /*        lvb.c: 315 */
#define   ELVB007      (ERRLVB +    7)    /*        lvb.c: 342 */
#define   ELVB008      (ERRLVB +    8)    /*        lvb.c: 351 */
#define   ELVB009      (ERRLVB +    9)    /*        lvb.c: 363 */
#define   ELVB010      (ERRLVB +   10)    /*        lvb.c: 376 */
#define   ELVB011      (ERRLVB +   11)    /*        lvb.c: 410 */
#define   ELVB012      (ERRLVB +   12)    /*        lvb.c: 429 */
#define   ELVB013      (ERRLVB +   13)    /*        lvb.c: 456 */
#define   ELVB014      (ERRLVB +   14)    /*        lvb.c: 465 */
#define   ELVB015      (ERRLVB +   15)    /*        lvb.c: 477 */
#define   ELVB016      (ERRLVB +   16)    /*        lvb.c: 490 */
#define   ELVB017      (ERRLVB +   17)    /*        lvb.c: 524 */
#define   ELVB018      (ERRLVB +   18)    /*        lvb.c: 543 */
#define   ELVB019      (ERRLVB +   19)    /*        lvb.c: 572 */
#define   ELVB020      (ERRLVB +   20)    /*        lvb.c: 581 */
#define   ELVB021      (ERRLVB +   21)    /*        lvb.c: 593 */
#define   ELVB022      (ERRLVB +   22)    /*        lvb.c: 604 */
#define   ELVB023      (ERRLVB +   23)    /*        lvb.c: 616 */
#define   ELVB024      (ERRLVB +   24)    /*        lvb.c: 650 */
#define   ELVB025      (ERRLVB +   25)    /*        lvb.c: 660 */
#define   ELVB026      (ERRLVB +   26)    /*        lvb.c: 679 */
#define   ELVB027      (ERRLVB +   27)    /*        lvb.c: 706 */
#define   ELVB028      (ERRLVB +   28)    /*        lvb.c: 715 */
#define   ELVB029      (ERRLVB +   29)    /*        lvb.c: 727 */
#define   ELVB030      (ERRLVB +   30)    /*        lvb.c: 740 */
#define   ELVB031      (ERRLVB +   31)    /*        lvb.c: 774 */
#define   ELVB032      (ERRLVB +   32)    /*        lvb.c: 793 */
#define   ELVB033      (ERRLVB +   33)    /*        lvb.c: 820 */
#define   ELVB034      (ERRLVB +   34)    /*        lvb.c: 829 */
#define   ELVB035      (ERRLVB +   35)    /*        lvb.c: 841 */
#define   ELVB036      (ERRLVB +   36)    /*        lvb.c: 854 */
#define   ELVB037      (ERRLVB +   37)    /*        lvb.c: 888 */
#define   ELVB038      (ERRLVB +   38)    /*        lvb.c: 907 */
#define   ELVB039      (ERRLVB +   39)    /*        lvb.c: 934 */
#define   ELVB040      (ERRLVB +   40)    /*        lvb.c: 943 */
#define   ELVB041      (ERRLVB +   41)    /*        lvb.c: 955 */
#define   ELVB042      (ERRLVB +   42)    /*        lvb.c: 968 */
#define   ELVB043      (ERRLVB +   43)    /*        lvb.c:1002 */
#define   ELVB044      (ERRLVB +   44)    /*        lvb.c:1021 */
#define   ELVB045      (ERRLVB +   45)    /*        lvb.c:1048 */
#define   ELVB046      (ERRLVB +   46)    /*        lvb.c:1057 */
#define   ELVB047      (ERRLVB +   47)    /*        lvb.c:1069 */
#define   ELVB048      (ERRLVB +   48)    /*        lvb.c:1082 */
#define   ELVB049      (ERRLVB +   49)    /*        lvb.c:1116 */
#define   ELVB050      (ERRLVB +   50)    /*        lvb.c:1135 */
#define   ELVB051      (ERRLVB +   51)    /*        lvb.c:1162 */
#define   ELVB052      (ERRLVB +   52)    /*        lvb.c:1171 */
#define   ELVB053      (ERRLVB +   53)    /*        lvb.c:1183 */
#define   ELVB054      (ERRLVB +   54)    /*        lvb.c:1196 */
#define   ELVB055      (ERRLVB +   55)    /*        lvb.c:1230 */
#define   ELVB056      (ERRLVB +   56)    /*        lvb.c:1249 */
#define   ELVB057      (ERRLVB +   57)    /*        lvb.c:1280 */
#define   ELVB058      (ERRLVB +   58)    /*        lvb.c:1289 */
#define   ELVB059      (ERRLVB +   59)    /*        lvb.c:1299 */
#define   ELVB060      (ERRLVB +   60)    /*        lvb.c:1314 */
#define   ELVB061      (ERRLVB +   61)    /*        lvb.c:1327 */
#define   ELVB062      (ERRLVB +   62)    /*        lvb.c:1340 */
#define   ELVB063      (ERRLVB +   63)    /*        lvb.c:1354 */
#define   ELVB064      (ERRLVB +   64)    /*        lvb.c:1389 */
#define   ELVB065      (ERRLVB +   65)    /*        lvb.c:1408 */

/* Layer manager event fields */

#define LVB_EVENT_MME_S1AP_BNDCFM     (LCM_EVENT_LYR_SPECIFIC + 1) 
#define LVB_EVENT_MME_EGTP_BNDCFM     (LCM_EVENT_LYR_SPECIFIC + 2) 

#define LVB_EGT_DFLT_ECHO_SAP 0
#define LVB_EGT_SAP           1
#define LVB_EGT_S10_SAP       2
#define LVB_EGT_S3_SAP        3

/* Layer manager cause fields */
#define LVB_CAUSE_BND_SUCC    1
#define LVB_CAUSE_ARP_SUCC    2

#define LVB_ENB_PEER_ID_0     0
#define LVB_ENB_PEER_ID_1     1

/* All Timer values required */
#define LVB_T3412      60 
#define LVB_T3413      30 
#define LVB_T3402      7200 /* Default 12 minutes */
/* lvb_h_001.main_2: Added 3422 timer */
#define LVB_T3422      60   /* Default 6 seconds */
#define LVB_T3450      60   /* Default 6 seconds */
#define LVB_T3460      60   /* Default 6 seconds */
#define LVB_T3470      60   /* Default 6 seconds */
/* lvb_h_001.main_1: Added 3485 timer */
#define LVB_T3485      80   /* Default 8 seconds */
#define LVB_T3486      8   /* Default 8 seconds */
#ifdef VB_MME_AUTH
/* Timer added for ESM inforamtion request */
#define LVB_T3489      40   /* Default 4 seconds */
#endif
 /* lvb_h_001.main_3: Added 3495 timer */
#define LVB_T3495      70   /* Default 7 seconds */
/***********************************************************************
  Defines for Debug Masks
***********************************************************************/
#define LVB_DBGMASK_MEM                       (DBGMASK_LYR << 0)
#define LVB_DBGMASK_TRC                       (DBGMASK_LYR << 1)
#define LVB_DBGMASK_ERROR                     (DBGMASK_LYR << 2)
#define LVB_DBGMASK_INFO                      (DBGMASK_LYR << 3)

/***********************************************************************
  Defines related to events across the management interface
***********************************************************************/

#define  EVTLVBCFGREQ                   1
#define  EVTLVBSTSREQ                   2
#define  EVTLVBCNTRLREQ                 3
#define  EVTLVBSTAREQ                   4
#define  EVTLVBCFGCFM                   5
#define  EVTLVBSTSCFM                   6
#define  EVTLVBCNTRLCFM                 7
#define  EVTLVBSTACFM                   8
#define  EVTLVBSTAIND                   9
#define  EVTLVBTRCIND                   10


/********************************************************************CZ**
 Selector Values
*********************************************************************CZ*/

#define LVB_SEL_LC            0     /**< Loose coupling */
#define LVB_SEL_LWLC          1     /**< Light-weight loose coupling -
                                     * applicable only on upper interface */

#endif /* __LVB_H__ */

  
/********************************************************************30**
  
         End of file:     lvb.h@@/main/nodal_main/2 - Wed Dec 18 15:02:02 2013
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
/main/1      ---      ap   1. Initial release.
/main/2      lvb_h_001.main_1 ap 1. Added definitions for tarces
/main/3      --- lvb_h_002.main_1 kmanoharan 1. Jumbo patch for sid change
    /main/1  ---      rk   1. Updated for release
/main/2      ---      lvb_h_001.main_1  rk  1. Added 3485 timer
/main/3     lvb_h_001.main_2  rk  1. Added 3422 timer
/main/4     lvb_h_001.main_3  ragrawal 1.Added 3495 timer
/main/5      ---      lvb_h_001.main_4  akaranth 1.Made MCC and MNC value to configurable parameter.
                                       2.MNC 2nd Digit present flag added.
*********************************************************************91*/
