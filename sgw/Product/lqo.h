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
  
        Name:     LTE CNE Layer management interface
    
        Type:     C include file
  
        Desc:     This file contains defines required by LTE CNE layer 
                  management interface
 
        File:     lqo.h

        Sid:      lqo.h@@/main/1 - Thu Jul  8 21:25:40 2010
  
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

#ifndef __LQO_H__
#define __LQO_H__

/********************************************************************QO**
 Event types
*********************************************************************QO*/

/********************************************************************QO**
 Element values for LqoMngmt structure
*********************************************************************QO*/
#define STQOGEN         0x01
#define STQOEGUSAP      0x02
#define STQOPGWCSAP     0x04
#define STQOMMECSAP     0x08
#define STQOECHOCSAP    0x10
#define STQOSGSNCSAP    0x20

/********************************************************************QO**
 * LTE CNE Node types             
 ********************************************************************QO */
#define LQO_NODE_SGW    1

/********************************************************************QO*
 * SGW General configuration  
 ********************************************************************QO*/

/* All Maximum limits */
#define LQO_SGW_TMR_RES      10

/* All Current values configurable */

#define LQO_UES              5

#define LQO_MAX_ENODEB_BIN    10
#define LQO_MAX_UECB_BIN      10

/*********************************************************************/

/* SAP States */
#define LQO_SAP_CFG     0
#define LQO_SAP_ISBOUND 1
#define LQO_SAP_UBND    2


#define EQOXXX 0

/* Layer manager event fields */

#define LQO_EVENT_SGW_EGTP_BNDCFM    (LCM_EVENT_LYR_SPECIFIC + 2) 

/* Layer manager cause fields */
#define LQO_CAUSE_BND_SUCC    1

/***********************************************************************
  Defines for Debug Masks
***********************************************************************/
#define LQO_DBGMASK_MEM                       (DBGMASK_LYR << 0)
#define LQO_DBGMASK_TRC                       (DBGMASK_LYR << 1)
#define LQO_DBGMASK_ERROR                     (DBGMASK_LYR << 2)
#define LQO_DBGMASK_INFO                      (DBGMASK_LYR << 3)

/***********************************************************************
  Defines related to events across the management interface
***********************************************************************/

#define  EVTLQOCFGREQ                   1
#define  EVTLQOSTSREQ                   2
#define  EVTLQOCNTRLREQ                 3
#define  EVTLQOSTAREQ                   4
#define  EVTLQOCFGCFM                   5
#define  EVTLQOSTSCFM                   6
#define  EVTLQOCNTRLCFM                 7
#define  EVTLQOSTACFM                   8
#define  EVTLQOSTAIND                   9
#define  EVTLQOTRCIND                   10


/********************************************************************CZ**
 Selector Values
*********************************************************************CZ*/

#define LQO_SEL_LC            0     /**< Loose coupling */
#define LQO_SEL_LWLC          1     /**< Light-weight loose coupling -
                                     * applicable only on upper interface */

#endif /* __LQO_H__ */

  
/********************************************************************30**
  
         End of file:     lqo.h@@/main/1 - Thu Jul  8 21:25:40 2010
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
/main/1      ---      mm   1. Initial release.
*********************************************************************91*/
