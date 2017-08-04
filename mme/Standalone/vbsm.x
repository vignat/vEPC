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
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:     
  
     File:     vbsm.x    
  
     Sid:      vbsm.x@@/main/1 - Mon Jun 28 19:35:41 2010
  
     Prg:        sp
  
*********************************************************************21*/

#ifndef __VBSM_X__
#define __VBSM_X__


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/********************************************************************VB**
 * LTE CNE - MME Stack Manager Events             
 ********************************************************************VB*/

typedef enum vbSmCneEvent
{
   EVTVBSMINIT  = 0,      /* State Machine Init */
   
   /* MME - Configuration and Control */

   /* S1-C Stack Coniguration */
   EVTVBTUCLCFGDONE,      /* TUCL Configuration Complete */
   EVTVBSCTPCFGDONE,      /* SCTP Configuration Complete */
   EVTVBS1APCFGDONE,      /* S1AP Configuration Complete */
  
   /* S11-C Stack Configuration */
   EVTVBEGTPCFGDONE,     /* EGTP-C Configuration Complete */

   EVTVBMMECFGDONE,       /* MME Configuration Complete */

   /* S1-C Stack Control - Bind */
   EVTVBSCTPBINDCFM,      /* SCTP Bind with TUCL Complete */
   EVTVBS1APBINDCFM,      /* S1AP Bind with SCTP Complete */
   EVTVBAPPBINDCFM,       /* MME Bind with S1AP Complete */

   /* S11-C Stack Control - Bind */
   EVTVBEGTPCSERVEROPEN,  /* EGTP-C Server Open Complete */ 
   EVTVBS11APPBINDCFM    /* Bind with EGTPC[S11 Interface] Complete */
#ifdef DEBUGP
   ,
   EVTVBMMEDBGCNTRLCFM    /* MME Debug Control Confirm  */
#endif

}VbSmCneEvent;

/********************************************************************VB**
 * LTE CNE - MME Stack Manager States            
 ********************************************************************VB*/
typedef enum vbSmMmeState
{
   VB_SM_STATE_INIT =  0,
   VB_SM_STATE_TUCL_CFG,
   VB_SM_STATE_SCTP_CFG,
   VB_SM_STATE_S1AP_CFG,
   VB_SM_STATE_EGTP_CFG,
   VB_SM_STATE_MME_CFG,  
   VB_SM_STATE_SCTP_BOUND,
   VB_SM_STATE_S1AP_BOUND,
   VB_SM_STATE_EGTP_BOUND,
   VB_SM_STATE_MME_CNTRL 
}VbSmMmeState;

/* Configuration given by the user through the file */
typedef struct vbSmUsrCfgCb
{
   U32 mmeIpAddr;
   U16 mmeSctPort;

   U32 sgwIpAddr;

   U32 pgwIpAddr;

   U16 eNbPort;
   S8  eNbAddr[VBSM_MAX_IP_ADDR_STR_LEN];
   U32 eNbIpAddr;

   U16 eNbPort_2;
   S8  eNbAddr_2[VBSM_MAX_IP_ADDR_STR_LEN];
   U32 eNbIpAddr_2;

   U16 egDfltPort;
   U16 egNonDfltPort;
   U16 egS10NonDfltPort;
   U16 egS3NonDfltPort;
   S8  egDfltHstNm[VBSM_MAX_HOST_NAME_LEN];

#ifdef DEBUGP
   U32 mmeDbgMask;
#endif
   U16 ueNum; /* No of UEs to be served by this MME */
   U8  sctpUdpServiceType; /* vb005.101 : CR 7528 service type 0 sctp 1 udp */
   U8  mccDig1;
   U8  mccDig2;
   U8  mccDig3;
   U8  mncDig1;
   U8  mncDig2;
   U8  mncDig3;
   U8  tgtMmePlmnId[VBSM_MAX_PLMN_ID_LEN];
   U32 tgtMmeIpAddr;
   U8 t3412;
   U32 sgsnIpAddr;
   U8  sgsnPlmnId[VBSM_MAX_PLMN_ID_LEN];

} VbSmUsrCfgCb;

typedef struct vbSmCb
{
   TskInit      init;          /* Task initialization structure */
   Pst          hiPst;         /* Stack manager -> HI post structure */
   Pst          sbPst;         /* Stack manager -> SCTP post structure */
   Pst          szPst;         /* Stack manager -> S1AP post structure */
   Pst          egPst;         /* Stack manager -> EGTP post structure */
   Pst          vbMmePst;      /* Stack manager -> MME App post structure */
   VbSmMmeState state;
   VbSmUsrCfgCb cfgCb;         /* All runtime configurable information */
}VbSmCb;

EXTERN VbSmCb vbSmCb;

EXTERN Void vbSmInitCb ARGS((Void));

/* MME */


EXTERN Void vbMmeHiCfg ARGS ((Void));

EXTERN Void vbMmeSbCfg ARGS ((Void));

EXTERN Void vbMmeSzCfg ARGS ((Void));

EXTERN Void vbMmeLvbCfg ARGS ((Void));

EXTERN S16 vbMmeSendMsg ARGS((VbSmCneEvent event));

EXTERN Void vbSmMain ARGS((Void));

EXTERN Void vbSmDefHdr ARGS((Header *hdr, Ent ent, Elmnt elem, Selector sel));

EXTERN Void vbSmExit ARGS((Void));

EXTERN S16 vbMmeHiShutDwn ARGS ((Void));
EXTERN S16 vbMmeSbShutDwn ARGS((Void));

EXTERN Void vbMmeLsbSbTSapCntrlReq ARGS ((SpId spId));

EXTERN S16 vbMmeSzLSapBndReq ARGS ((Void));

EXTERN S16 vbMmeSzShutDwn ARGS ((Void));

EXTERN S16 vbMmeSztSapBndReq ARGS ((Void));

EXTERN S16 vbMmeVbShutDwn ARGS ((Void));

EXTERN Void vbMmeEgCfg ARGS((Void));

EXTERN S16 vbMmeEgShutDwn ARGS ((Void));

EXTERN S16 vbMmeEgLSapBndReq ARGS ((Void));

EXTERN S16 vbMmeEgtSapBndReq ARGS ((Void));
#ifdef DEBUGP
EXTERN S16 vbMmeDbgMaskCntrlReq ARGS((Void));
#endif


EXTERN  S16 VbMiLvbStaInd ARGS(( Pst  *pst, VbMngmt *usta));
EXTERN  S16 VbMiLvbCfgCfm ARGS(( Pst  *pst, VbMngmt *cfm));
EXTERN  S16 VbMiLvbCntrlCfm ARGS((Pst *pst, VbMngmt *cfm));
EXTERN  S16 VbMiLvbStsCfm ARGS(( Pst  *pst, VbMngmt *cfm));
EXTERN  S16 VbMiLvbStaCfm ARGS(( Pst  *pst, VbMngmt *cfm));
EXTERN  S16 VbMiLvbTrcInd ARGS(( Pst  *pst, VbMngmt *trc, Buffer  *trcBuf));

EXTERN S16 SmMiLvbCfgCfm ARGS((Pst     *pst,    /* Post structure */
                               VbMngmt *cfm));  /* Management structure */

EXTERN S16 SmMiLvbCntrlCfm ARGS((Pst     *pst,   /* Post structure */
                                 VbMngmt *cfm)); /* Management structure */

EXTERN S16 SmMiLvbStaInd ARGS((Pst     *pst,     /* Post structure */
                               VbMngmt *usta));  /* management structure */

EXTERN S16 SmMiLvbTrcInd ARGS((Pst *pst,        /* Post structure */
                               VbMngmt *trc,    /* management structure */
                               Buffer  *mBuf)); /* trace buffer */

EXTERN S16 SmMiLvbStsCfm ARGS((Pst     *pst,    /* Post structure */
                               VbMngmt *sts));  /* management structure */

EXTERN S16 SmMiLvbStaCfm ARGS((Pst     *pst,    /* Post structure */
                               VbMngmt *sta));  /* management structure */

EXTERN S16 vbSmActvInit ARGS ((Ent entity,       /* entity */
                               Inst inst,        /* instance */
                               Region region,    /* region */
                               Reason reason));  /* reason */

EXTERN S16 vbSmActvTsk ARGS ((Pst    *pst, Buffer   *mBuf));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif/* __VBSM_X__ */

/**********************************************************************

         End of file:     vbsm.x@@/main/1 - Mon Jun 28 19:35:41 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -          sp              1. LTE-CNE Initial Release.
/main/1    vb005.101    rk              1. Added support for Network initiated Detach
                                        2. SCTP service made run time configuration parameter
/main/1    vb006.101  ragrawal          1. Updated vbSmUsrCfgCb structure.                                       
                                        2. Added MULTIPLE_ENB Support.
/main/1    vb007.101  akaranth          1. added new configurable parameter.                                       
*********************************************************************91*/
