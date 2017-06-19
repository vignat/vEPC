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
 
     Name:     Trillium LTE CNE - EMM/ESM EDM module
  
     Type:     C include file
  
     Desc:     C structures for EMM/ESM
  
     File:     cm_emm_esm.x 
  
     Sid:      cm_emm_esm.x@@/main/nodal_main/akaranth_feat_lpdev_branch/1 - Tue Sep 29 21:39:56 2015
  
     Prg:      rp
  
*********************************************************************21*/
#ifndef __CMEMMESMX__
#define __CMEMMESMX__

#ifdef __cplusplus
extern "C" {
#endif


/* The NAS Event Structure */
/* This structure is used across by EMM, ESM Modules, MME/UE Lower Interface
   module and MME/UE controller module */

struct cmNasEvnt
{
   CmMemListCp    memCp;   /* Memory control pointer */

   U8          protDisc;   /* Protocol Discriminator to distinguish
                              between EMM and ESM from/to the NAS PDU of
                              S1AP/RRC Message */

   /*Security Feature Eanbled*/
#ifdef CM_NAS_SEC 
   U8  secStatus;          /* For sending indication to upper layer
                              about security check status
                            */
   U8  secPD;
   U8  secHT;
   U8  seqNmb;
   U32 mac;
#endif
   /*Security Feature Eanbled*/

/*cm_emm_esm_x_001.main_1: Security header type for both emm and esm messages*/
#ifdef VB_MME_AUTH
   U8          secHdrType; /* Security header type for both emm and 
                              esm messages */
#endif /* #ifdef VB_MME_AUTH */
   union
   {
      CmEmmMsg *emmEvnt;   /* Complete EMM PDU */
                           /* ESM can still be piggy backed in EMM Message
                              here */

      CmEsmMsg *esmEvnt;   /* Complete ESM PDU */
   }m;

   Ptr            pdu;
} ;

/* EDM Init structure */
typedef struct _cmEdmInit         /* EDM initialization */
{
   Ent ent;                       /* entity */
   Inst inst;                     /* instance */
   Region region;                 /* static region */
   Pool pool;                     /* static pool */
   ProcId procId;                 /* processor id */
#ifdef DEBUGP
   U32 dbgMask;                   /* debug mask */
   Txt prntBuf[PRNTSZE];          /* print buffer */
#endif
}CmEdmInit;

EXTERN CmEdmInit cmEdmInit;

/*******************************************************/
/* Function declarations */
/*******************************************************/
EXTERN S16 cmEmmEncMsg ARGS((CmNasEvnt  *evnt, U8 **buf, U16 *len));

EXTERN S16 cmEmmDecMsg ARGS((U8 *buf, CmNasEvnt **evnt, U32 len ));

EXTERN S16 cmEsmEncMsg ARGS((CmNasEvnt  *evnt, U8 **buf, U16 *len));

EXTERN S16 cmEsmDecMsg ARGS((U8 *buf, CmNasEvnt **evnt, U32 len ));

EXTERN S16 cmEmmEsmEdmInit ARGS((CmEdmInit init));

EXTERN S16 cmEdmUtlCreateEvent ARGS((CmNasEvnt   **newEvnt,
                                     U8          evntType));

EXTERN S16 cmEdmUtlFreeEvent ARGS((CmNasEvnt  **evnt));

#ifdef CM_NAS_SEC
EXTERN S16 cmEdmUtlDecodeHdr ARGS((CmNasEvnt  *evnt,    
                              U8         *buf,
                              U16        len));

#endif
PUBLIC S16 cmEmmEncHdr ARGS(( CmEmmMsg *msg, U8 *buf));
PUBLIC S16 cmEmmDecHdr ARGS((U8* buf, CmEmmMsg* msg));

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __CMEMMESMX__ */

/**********************************************************************

         End of file:     cm_emm_esm.x@@/main/nodal_main/akaranth_feat_lpdev_branch/1 - Tue Sep 29 21:39:56 2015

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
   /main/1   -       rp               1. LTE-CNE Initial Release.
/main/2      ---      cm_emm_esm_x_001.main_1 akaranth 1.Security header type for both 
                                            emm and esm messages.
*********************************************************************91*/
