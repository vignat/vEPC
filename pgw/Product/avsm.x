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

     Name:     avsm.x

     Type:     C include file

     Desc:     this file contains structures,typedefs and function prototypes
               used by stack manager

     File:     avsm.x

     Sid:      avsm.x@@/main/1 - Tue Aug 17 18:54:35 2010

     Prg:        sp      

*********************************************************************21*/

#ifndef __AVSM_X__
#define __AVSM_X__

/********************************************************************AV**
 * LTE CNE Stack Manager Events             
 ********************************************************************AV*/

typedef enum avSmCneEvent
{
   /* P-GW - Configuration and Control */
   EVTAVSMINIT  = 0,      /* State Machine Init */
   EVTAVTUCLCFGDONE,      /* TUCL Configuration Complete */
   EVTAVEGTPCCFGDONE,      /* EGTP-C Configuration Complete */
   EVTAVEGTPECHOCFGDONE,      /* EGTP-C Configuration Complete */
   EVTAVEGTPUCFGDONE,      /* EGTP-U Configuration Complete */
   EVTAVPGWCFGDONE,        /* P-GW Configuration Complete */
   EVTAVTPTSRVROPEN,   /* S-GW EGTPU Server Open Complete */
   EVTAVGTPCBINDCFM,        /* S-GW Bind with EGTPC Complete */
   EVTAVECHOBINDCFM,        /* S-GW Bind with EGTPC Complete */
   EVTAVGTPUBINDCFM,        /* S-GW Bind with EGTPU Complete */
   EVTAVPGWROUTECFM        /* P-GW Route Control Complete */
}AvSmCneEvent;

/* Configuration given by the user through the file */
typedef struct avSmUsrCfgCb
{
   S8  pgwDfltIpAddr[AVSM_MAX_IP_ADDR_STR_LEN];
   S8  pgwDfltExgwIpAddr[AVSM_MAX_IP_ADDR_STR_LEN];
   U32 pgwUeStartAddr;

   U32 sgwIpAddr;
   U16 sgwPort;

   U16 egCDfltPort;
   U16 egCNonDfltPort;
   U16 egUDfltPort;
   S8  egDfltAddr[AVSM_MAX_IP_ADDR_STR_LEN];
   S8  egDfltHstNm[AVSM_MAX_HOST_NAME_LEN];
   /* av004.101 Configuring no.of IPs and UEs */
   U8  pgwUeNum;  /* Maximum number of UEs supporting */
   U8  pgwIpNum; /* Maximum number of Ips available */
#ifdef DEBUGP
   U32 dbgMask;
#endif
   U32 pcscfAddr; /* SIP signalling  IP address */
   U16   pcscfport; /* Port number for the SIP signalling */
} AvSmUsrCfgCb;

typedef struct avSmCb
{
   TskInit     init;          /* Task initialization structure */
   Pst         hiPst;         /* Stack manager -> HI post structure */
   Pst         egPst;         /* Stack manager -> EGTP post structure */
   Pst         avPgwPst;      /* Stack manager -> PGW App post structure */
   Pst         avSgwPst;      /* Stack manager -> SGW App post structure */
   U8          state;
   AvSmUsrCfgCb cfgCb;
}AvSmCb;

EXTERN AvSmCb avSmCb;

EXTERN Void avSmInitCb ARGS((Void));



EXTERN Void avSmMain ARGS((Void));
PUBLIC Void avSmDefHdr ARGS((Header *, Ent, Elmnt, Selector ));
PUBLIC Void avSmExit ARGS((Void));

EXTERN Void avSmInitCfgCb ARGS ((Void));

EXTERN S16 avMmeAvShutDwn ARGS ((Void));

/* S-GW */

EXTERN S16 avSgwSendMsg ARGS((AvSmCneEvent event));

EXTERN Void avSgwLavCfg ARGS((Void));

EXTERN Void avSgwEgCfg ARGS((Void));

EXTERN S16 avSgwEgShutDwn ARGS ((Void));

EXTERN S16 avSgwEgLSapBndReq ARGS ((Void));

EXTERN S16 avSgwEgtSapBndReq ARGS ((Void));


/* PDN-GW */
EXTERN S16 avPgwSendMsg ARGS((AvSmCneEvent  event));

EXTERN Void avPgwLavCfg ARGS((Void));

EXTERN S16 avPgwSendArpReq ARGS ((Void));

EXTERN S16 avPgwDbgMaskCntrlReq ARGS((Void));


EXTERN  S16 AvMiLavStaInd ARGS(( Pst     *pst, AvMngmt *usta ));
EXTERN  S16 AvMiLavCfgCfm ARGS(( Pst     *pst, AvMngmt *cfm  ));
EXTERN  S16 AvMiLavCntrlCfm ARGS(( Pst     *pst, AvMngmt *cfm ));
EXTERN  S16 AvMiLavStsCfm ARGS(( Pst     *pst, AvMngmt *cfm ));
EXTERN  S16 AvMiLavStaCfm ARGS(( Pst     *pst, AvMngmt *cfm ));
EXTERN  S16 AvMiLavTrcInd ARGS(( Pst     *pst, AvMngmt *trc, Buffer  *trcBuf));

EXTERN S16 SmMiLavCfgCfm ARGS((Pst     *pst,       /* Post structure */
         AvMngmt *cfm));  /* management structure */

EXTERN S16 SmMiLavCntrlCfm ARGS((Pst     *pst,       /* Post structure */
         AvMngmt *cfm));  /* management structure */

EXTERN S16 SmMiLavStaInd ARGS((Pst     *pst,       /* Post structure */
         AvMngmt *usta));  /* management structure */

EXTERN S16 SmMiLavTrcInd ARGS((Pst *pst,       /* Post structure */
         AvMngmt *trc,  /* management structure */
         Buffer  *mBuf));    /* trace buffer */

EXTERN S16 SmMiLavStsCfm ARGS((Pst     *pst,       /* Post structure */
         AvMngmt *sts));  /* management structure */

EXTERN S16 SmMiLavStaCfm ARGS((Pst     *pst,       /* Post structure */
         AvMngmt *sta));  /* management structure */

EXTERN S16 avSmActvInit ARGS ((
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
));

EXTERN S16 avSmActvTsk ARGS ((
Pst      *pst,
Buffer   *mBuf
));

EXTERN Void avPgwHiCfg ARGS ((Void));
EXTERN Void avPgwEgCCfg ARGS ((Void));
EXTERN Void avPgwEgECfg ARGS ((Void));
EXTERN Void avPgwEgUCfg ARGS ((Void));
EXTERN S16 avPgwEgtSapBndReq ARGS (( S16 type ));
EXTERN S16 avPgwEgLSapBndReq ARGS (( Void ));
EXTERN S16 avPgwRouteInit    ARGS((Void));
EXTERN S16 SRegInfoShow  ARGS((Region region, U32 *availmem));
EXTERN S16 avPgwHiShutDwn     ARGS ((Void));
EXTERN S16 avPgwEgShutDwn     ARGS((Void));
EXTERN S16  avsmParseRdConQ ARGS((Data data));
#endif/* __AVSM_X__ */

/******************************************************************30**

         End of file:     avsm.x@@/main/1 - Tue Aug 17 18:54:35 2010

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
/main/1      -        sp                1. LTE-CNE:PGW Initial Release.
/main/1    av004.101  pmacharla         1. Multi UE support added
*********************************************************************91*/
