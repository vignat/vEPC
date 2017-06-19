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

     Name:    Trillium LTE SGW Application 

     Type:    C Include file

     Desc:    This file contains typedefs and prototypes required by Stack Manager

     File:    qosm.x

     Sid:      qosm.x@@/main/1 - Thu Jul  8 21:22:22 2010

     Prg:      sp  
*********************************************************************21*/

#ifndef __QOSM_X__
#define __QOSM_X__

/********************************************************************QO**
 * LTE CNE Stack Manager Events             
 ********************************************************************QO*/

typedef enum qoSmCneEvent
{
   EVTQOSMINIT  = 0,      /* State Machine Init */
   EVTQOTUCLCFGDONE,      /* TUCL Configuration Complete */
   
   /* S-GW - Configuration and Control */
   EVTQOEGTPCFGDONE,      /* EGTP-U Configuration Complete */
   EVTQOSGWCFGDONE,        /* S-GW Configuration Complete */
   EVTQOEGTPSERVEROPEN,   /* S-GW EGTP Server Open Complete */ 
   EVTQOSGWBINDCFM,       /* S-GW Bind with EGTP Complete */
   EVTQOEGTPCSERVEROPEN   /* EGTP-C Server Open Complete */ 
#ifdef DEBUGP
   ,
   EVTQOSGWDBGCNTRLCFM    /* SGW Debug Control Confirm  */
#endif
}QoSmCneEvent;

/* Configuration given by the user through the file */
typedef struct qoSmUsrCfgCb
{
   U32 sgwIpAddr;
   U16 egDfltCPort;
   U16 egS11CPort;
   U16 egS5S8CPort;
   U16 egS4CPort;
   U16 egDfltUPort;
   S8  egDfltAddr[QOSM_MAX_IP_ADDR_STR_LEN];
   S8  egDfltHstNm[QOSM_MAX_HOST_NAME_LEN];
   U16 ueNum;
#ifdef DEBUGP
   U32 sgwDbgMask;
   U32 smDbgMask;
#endif
} QoSmUsrCfgCb;

typedef struct qoSmCb
{
   TskInit     init;          /* Task initialization structure */
   Pst         hiPst;         /* Stack manager -> HI post structure */
   Pst         egPst;         /* Stack manager -> EGTP post structure */
   Pst         qoSgwPst;      /* Stack manager -> SGW App post structure */
   U8          state;
   QoSmUsrCfgCb cfgCb;
}QoSmCb;

EXTERN QoSmCb qoSmCb;

EXTERN Void qoSmInitCb ARGS((Void));



EXTERN Void qoSmMain ARGS((Void));
PUBLIC Void qoSmDefHdr ARGS((Header *, Ent, Elmnt, Selector ));
PUBLIC Void qoSmExit ARGS((Void));

EXTERN S16 qoMmeQoShutDwn ARGS ((Void));

/* S-GW */

EXTERN S16 qoSgwSendMsg ARGS((QoSmCneEvent event));

EXTERN Void qoSgwLqoCfg ARGS((Void));

EXTERN Void qoSgwEgCfg ARGS((Void));

EXTERN S16 qoSgwEgShutDwn ARGS ((Void));

EXTERN S16 qoSgwEgLSapBndReq ARGS ((Void));

EXTERN S16 qoSgwEgtSapBndReq ARGS ((Void));

EXTERN S16 qoSgwDbgCntrlReq ARGS((Void));

/* PDN-GW */
EXTERN S16 qoPgwSendMsg ARGS((QoSmCneEvent  event));

EXTERN Void qoPgwLqoCfg ARGS((Void));

EXTERN S16 qoPgwSendArpReq ARGS ((Void));

EXTERN  S16 QoMiLqoStaInd ARGS(( Pst     *pst, QoMngmt *usta ));
EXTERN  S16 QoMiLqoCfgCfm ARGS(( Pst     *pst, QoMngmt *cfm  ));
EXTERN  S16 QoMiLqoCntrlCfm ARGS(( Pst     *pst, QoMngmt *cfm ));
EXTERN  S16 QoMiLqoStsCfm ARGS(( Pst     *pst, QoMngmt *cfm ));
EXTERN  S16 QoMiLqoStaCfm ARGS(( Pst     *pst, QoMngmt *cfm ));
EXTERN  S16 QoMiLqoTrcInd ARGS(( Pst     *pst, QoMngmt *trc, Buffer  *trcBuf));

EXTERN S16 SmMiLqoCfgCfm ARGS((Pst     *pst,       /* Post structure */
         QoMngmt *cfm));  /* management structure */

EXTERN S16 SmMiLqoCntrlCfm ARGS((Pst     *pst,       /* Post structure */
         QoMngmt *cfm));  /* management structure */

EXTERN S16 SmMiLqoStaInd ARGS((Pst     *pst,       /* Post structure */
         QoMngmt *usta));  /* management structure */

EXTERN S16 SmMiLqoTrcInd ARGS((Pst *pst,       /* Post structure */
         QoMngmt *trc,  /* management structure */
         Buffer  *mBuf));    /* trace buffer */

EXTERN S16 SmMiLqoStsCfm ARGS((Pst     *pst,       /* Post structure */
         QoMngmt *sts));  /* management structure */

EXTERN S16 SmMiLqoStaCfm ARGS((Pst     *pst,       /* Post structure */
         QoMngmt *sta));  /* management structure */

EXTERN S16 qoSmActvInit ARGS ((
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
));

EXTERN S16 qoSmActvTsk ARGS ((
Pst      *pst,
Buffer   *mBuf
));
EXTERN Void qoSgwHiCfg ARGS ((Void));
EXTERN S16 SRegInfoShow  ARGS((Region region, U32 *availmem));
EXTERN S16 qoSgwHiShutDwn ARGS ((Void));
EXTERN  S16  qosmParseRdConQ ARGS ((Data data));
#endif/* __QOSM_X__ */

/**********************************************************************

         End of file:     qosm.x@@/main/1 - Thu Jul  8 21:22:22 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -          sp                  1. LTE-CNE:SGW Initial Release
*********************************************************************91*/
