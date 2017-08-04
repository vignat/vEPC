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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for Node Data base functions

     File:     sz_ndb.c

     Sid:      sq_ndb.c@@/main/nodal_main/3 - Fri Nov 22 17:58:23 2013

     Prg:      sn

*********************************************************************21*/
/* Header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#ifdef SZ_FTHA
#include "cm_psfft.h"
#include "cm_ftha.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#endif /* SZ_FTHA */

#include "sct.h"           /* SCTP Layer */
#include "szt.h"           /* S1AP Upper Interface */
#include "szt_asn.h"
#ifdef SZ_FTHA
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP LM Interface */
#ifdef YT
#include "lyt.h"            /* S1AP-PSF                        */
#include "yt.h"            /* S1AP-PSF                        */
#endif /* YT */
#include "sz.h"            /* S1AP Layer */
#include "sz_err.h"
#include "cm_pasn.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* Header include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCTP Layer */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_pftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */

#include "lsz.x"           /* S1AP LM Interface */
#include "cm_pasn.x"
#ifdef YT
#include "lyt.x"            /* S1AP-PSF                        */
#include "yt.x"            /* S1AP-PSF                        */
#endif /* YT */
#include "sz.x"            /* S1AP Layer */


#ifdef SZ_MME
PRIVATE S16 szFreeNdbDynamPeerCb ARGS ((SzPeerCb *peerCb));
#endif /* SZ_MME */
PRIVATE S16 szFreeNdbPeerCb ARGS ((SzPeerCb *locCb));


/*
*
*       Fun:   szNdbAllocConCb
*
*       Desc:  Called to allocate an available connection control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbAllocConCb
(
SzConCb **conCb,
SzPeerCb *peer
)
#else /* ANSI */
PUBLIC S16 szNdbAllocConCb(conCb, peer) 
SzConCb **conCb;
SzPeerCb *peer;
#endif /* ANSI */
{
   SzConCb   *locConCb = NULLP;
   S16 ret = RFAILED;

   TRC2(szNdbAllocConCb)

#ifdef DEBUGP
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf, \
                         "szNdbAllocConCb\n"));
#endif  /* DEBUGP */

   *conCb = (SzConCb *)NULLP;

   SZ_ALLOC(locConCb, sizeof(SzConCb));
   if (locConCb == NULLP)
   {
      szCb.genSts.nmbFailConn++;
      SZ_SND_LM_ALARM(ESZ112,"szNdbAllocConCb",peer->sztSapCb->pst.region,peer->sztSapCb->pst.pool);
   }

   /* Mark the control block state as idle */
   locConCb->state = SZ_SMSZ_IDLE;

   locConCb->cd.peerCb = peer;

   szUtilsAllocConId(&locConCb->lclConRefNo);

   /* allocate the connection reference number */
#ifdef SZ_FTHA
   if (YT_IS_ACTV() == TRUE)
   {
#endif /* SZ_FTHA */

      ret = cmHashListInsert (&szCb.lclConCp, (PTR)locConCb,
            (U8 *)&locConCb->lclConRefNo, sizeof (SzConRefNo));
      if (ret != ROK)
      {
         SZ_FREE(locConCb, sizeof(SzConCb));
         szCb.genSts.nmbFailConn++;
         SZ_SND_LM_ALARM(ESZ113,"szNdbAllocConCb",peer->sztSapCb->pst.region,peer->sztSapCb->pst.pool);
      }

      /* Add connection CB in peerCb */
      ret = cmHashListInsert (&peer->conRefLst, (PTR)locConCb,
            (U8 *)&locConCb->lclConRefNo, sizeof (SzConRefNo));
      if (ret != ROK)
      {
         cmHashListDelete (&szCb.lclConCp, (PTR)locConCb);
         SZ_FREE(locConCb, sizeof(SzConCb));
         szCb.genSts.nmbFailConn++;
         SZ_SND_LM_ALARM(ESZ114,"szNdbAllocConCb",peer->sztSapCb->pst.region,peer->sztSapCb->pst.pool);
      }
#ifdef SZ_FTHA
   } /* if YT_IS_ACTV() */
#endif /* SZ_FTHA */

   szCb.genSts.nmbConn++;
   szCb.genSta.nmbActvPeerConn++;
   peer->nmbConn++;

   /* Add mpping for the connection control block */
#ifdef YT
   YT_ADD_MAPPING(YT_UPD_CONN_CB, locConCb);
   YT_RTUPD_ADD(YT_UPD_CONN_CB, locConCb);
#endif /* YT */

   *conCb = locConCb;

   RETVALUE(ROK);
} /* end of szNdbAllocConCb */

/*
*
*       Fun:   szNdbGetConCb
*
*       Desc:  Called to retrieve a connection control block
*
*       Ret:   ok:      ROK
*              failed:  RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbGetConCb
(
SzConRefNo  conId,
SzConCb    **connectionCb
)
#else /* ANSI */
PUBLIC S16 szNdbGetConCb(conId, connectionCb)
SzConRefNo  conId;
SzConCb    **connectionCb;
#endif /* ANSI */
{
   S16 found = RFAILED;

   TRC2(szNdbGetConCb)

#ifdef DEBUGP
#ifdef ALIGN_64BIT
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf, \
      "szNdbGetConCb(conId = %d)\n", conId));
#else /* ALIGN_64BIT */
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf, \
      "szNdbGetConCb(conId = %ld)\n", conId));
#endif /* ALIGN_64BIT */
#endif /* DEBUGP */

   found = cmHashListFind(&szCb.lclConCp, (U8 *)&conId, 
                    sizeof (SzConRefNo), 0, (PTR *)connectionCb);

   RETVALUE(found);
} /* end of szNdbGetConCb */

/*
*
*       Fun:   szNdbDeallocConCb
*
*       Desc:  Called to deallocate a connection control block
*
*       Ret:   ok:      ROK
*              failed:  RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbDeallocConCb
(
SzConRefNo conRef,
SzPeerCb   *peerCb,
U8         conRefType
)
#else /* ANSI */
PUBLIC S16 szNdbDeallocConCb(conRef, peerCb, conRefType)
SzConRefNo conRef;
SzPeerCb   *peerCb;
U8         conRefType;
#endif /* ANSI */
{
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzConCb *connectionCb;
   S16      found = RFAILED;

   TRC2(szNdbDeallocConCb)

#ifdef DEBUGP
#ifdef ALIGN_64BIT
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
                        "szNdbDeallocConCb(conRef=%d)\n", conRef));
#else /* ALIGN_64BIT */
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
                        "szNdbDeallocConCb(conRef=%ld)\n", conRef));
#endif /* ALIGN_64BIT */
#endif /* DEBUGP */
   uSapCb = peerCb->sztSapCb;

   /* Search in the hash list depending on the conn ref type */
   if(conRefType == SZ_CONN_REF_RMT)
   {
      found = cmHashListFind(&peerCb->conRefLstRem, (U8 *)&conRef,
                             sizeof (SzConRefNo), 0, (PTR *)&connectionCb);
   }
   else
   {
      found = cmHashListFind(&szCb.lclConCp, (U8 *)&conRef,
                             sizeof (SzConRefNo), 0, (PTR *)&connectionCb);
   }

   if( found == ROK)
   {
      cmHashListDelete (&szCb.lclConCp, (PTR)connectionCb);
      cmHashListDelete (&peerCb->conRefLst, (PTR)connectionCb);
      /* 
       * If the connection control block is added to the 
       * SAP connection List delete from the hash list
       */
      cmHashListDelete (&uSapCb->suConnLst, (PTR)connectionCb);

      /* Delete the connection control block from remote list */
      cmHashListDelete (&peerCb->conRefLstRem, (PTR)connectionCb);

      /* Add mpping for the connection control block */
#ifdef YT
      YT_RTUPD_DEL(YT_UPD_CONN_CB, connectionCb);
      YT_DEL_MAPPING(YT_UPD_CONN_CB, connectionCb);
#endif /* YT */

      SZ_FREE(connectionCb,sizeof(SzConCb));
      szCb.genSts.nmbConn--;
      szCb.genSta.nmbActvPeerConn--;
      peerCb->nmbConn--;
   }
   else
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of szNdbDeallocConCb */

/*
 *       Fun:  szNdbChkPeer    
 *
 *       Desc:  This function checks whether peer is present for the
 *              specified peer Id
 *
 *       Ret:   ROK - SUCCESS;
 *              RFAILED - FAILURE
 *
 *       Notes: 
 *
 *       File:  sz_ndb.c
 *
 */
#ifdef ANSI
PUBLIC S16 szNdbChkPeer
(
TknU32     peerId,
SzPeerCb   **peerCb
)
#else /* ANSI */
PUBLIC S16 szNdbChkPeer (peerId,peerCb)
TknU32     peerId;
SzPeerCb   **peerCb;
#endif /* ANSI */
{
   S16 found = RFAILED;

   TRC3(szNdbChkPeer)

   *peerCb = NULLP;
   
   if (peerId.pres != NOTPRSNT)
   {
      found = cmHashListFind(&szCb.peerLst, (U8 *)&peerId.val, 
                    sizeof (U32), 0, (PTR *)peerCb);

      RETVALUE(found);
   }
   RETVALUE(RFAILED);
} /* szNdbChkPeer */

/*
*
*       Fun:   szNdbGetStream
*
*       Desc:  Called to allocate an SCTP stream to be used across an
*              SCTP association by a SCCP connection or connectionless traffic
*
*       Ret:   ok:      ROK
*              failed:  RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbGetStream
(
SzPeerCb    *peerCb,
SctStrmId   *strmId
)
#else /* ANSI */
PUBLIC S16 szNdbGetStream(peerCb, strmId)
SzPeerCb    *peerCb;
SctStrmId   *strmId;
#endif /* ANSI */
{
   TRC2(szNdbGetStream)

#ifdef DEBUGP
#ifdef ALIGN_64BIT
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
                        "szCmGetStream(peerId=%d)\n",
                        peerCb->peerId));
#else /* ALIGN_64BIT */
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
                        "szCmGetStream(peerId=%ld)\n",
                        peerCb->peerId));
#endif /* ALIGN_64BIT */
#endif /* DEBUGP */
      /* sz008.301: Kwork Fix: Type cast into SctStrmId.*/
   *strmId = (SctStrmId)((peerCb->nextStreamId) % peerCb->assocCb->assocCfg->locOutStrms);

   if (*strmId == peerCb->assocCb->assocCfg->comProcStrm.val)
   {
      /* Since stream comProcStrm is reserved for common procedure messages,
           use another stream */
      /*sz008.301: Kwork Fix: Type Cast into SctStrmId.*/
	*strmId = (SctStrmId)((*strmId + 1) % peerCb->assocCb->assocCfg->locOutStrms);
   }

#ifdef DEBUGP
   SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
                    "szCmGetStream(*strmId = %d)\n", *strmId));
#endif /* DEBUGP */
      /*sz008.301: Kwork Fix: Type cast into SctStrmId. */
   peerCb->nextStreamId = (SctStrmId)((*strmId + 1) % peerCb->assocCb->assocCfg->locOutStrms);

   RETVALUE(ROK);
} /* end of szNdbGetStream */


/*
*
*       Fun:   szNdbInitPeerLst
*
*       Desc:  Called to Initialize peer List
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbInitPeerLst
(
Void
)
#else /* ANSI */
PUBLIC S16 szNdbInitPeerLst()
#endif /* ANSI */
{
   S16 ret = RFAILED;
 
   TRC2(szNdbInitPeerLst)

   ret = cmHashListInit(&szCb.peerLst,          /* hash list Cp */
            (U16)szCb.genCfg.nmbPeer,                /* HL bins */
            0,                             /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            SZ_HASH_KEYTYPE,                    /* HL key type */
            szCb.init.region,                   /* Mem region for HL */
            szCb.init.pool);                    /* Mem pool for HL */

   if(ret != ROK)
   {
      RETVALUE(ret);
   }

   ret = cmHashListInit(&szCb.peerAddrPortLst,        /* hash list Cp */
            (U16)(szCb.genCfg.nmbPeer * SCT_MAX_NET_ADDRS),  /* HL bins */
            0,                                        /* Offset of HL Entry */
            FALSE,                                    /* Allow dup. keys ? */
            CM_HASH_KEYTYPE_ANY,                      /* HL key type */
            szCb.init.region,                         /* Mem region for HL */
            szCb.init.pool);                          /* Mem pool for HL */

   RETVALUE(ret);
}  /* end of szNdbInitPeerLst */

/*
*
*       Fun:   szNdbAllocPeerCb
*
*       Desc:  Called to allocate an available peer control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbAllocPeerCb
(
U32 peerId,
SzPeerCb **peer
)
#else /* ANSI */
PUBLIC S16 szNdbAllocPeerCb(peerId,peer)
U32 peerId;
SzPeerCb **peer;
#endif /* ANSI */
{
   SzPeerCb *locCb = NULLP;
   S16 ret = RFAILED;
   U16 offset = 0;
   SzConCb  conCb;
   U32 nmbCon;

   TRC3(szNdbAllocPeerCb)

   *peer = NULLP;

   /* Allocate peer control block */
   SZ_ALLOC(locCb,sizeof(SzPeerCb));
   if(locCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ115, (ErrVal) 0,
           "szNdbAllocPeerCb: Couldn't allocate memory for peerCB");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      RETVALUE(RFAILED);
   }

   /* Allocate Assoc control block in peer cb */
   SZ_ALLOC(locCb->assocCb,sizeof(SzAssocCb));
   if(locCb->assocCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ116, (ErrVal) 0,
           "szNdbAllocPeerCb: Couldn't allocate memory for assocCB");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      SZ_FREE(locCb,sizeof(SzPeerCb));
      RETVALUE(RFAILED);
   }

   /* Allocate assoc config in assoc control block */
   SZ_ALLOC(locCb->assocCb->assocCfg,sizeof(SzAssocCfg));
   if(locCb->assocCb->assocCfg == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ117, (ErrVal) 0,
           "szNdbAllocPeerCb: Couldn't allocate memory for assocCfg");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      SZ_FREE(locCb->assocCb,sizeof(SzAssocCb));
      SZ_FREE(locCb,sizeof(SzPeerCb));
      RETVALUE(RFAILED);
   }

   locCb->setupReq = NULLP;
   if(szCb.genCfg.nmbPeer > 1)
   {
      nmbCon = szCb.genCfg.nmbConn / szCb.genCfg.nmbPeer;
      nmbCon++;
   }
   else
   {
      nmbCon = szCb.genCfg.nmbConn;
   }
   offset = (U16)((PTR)&conCb.peerConHlEnt - (PTR)&conCb);
   ret = cmHashListInit(&locCb->conRefLst,
            (U16)nmbCon,
            offset,
            FALSE,
            SZ_HASH_KEYTYPE,
            szCb.init.region,
            szCb.init.pool);

   if (ret != ROK)
   {
      szFreeNdbPeerCb(locCb);
      RETVALUE(RFAILED);
   }
   /* Initialize the hash list of remote connection control block */
   offset = (U16)((PTR)&conCb.peerConHlEntRem - (PTR)&conCb);
   ret = cmHashListInit(&locCb->conRefLstRem,
            (U16)nmbCon,
            offset,
            FALSE,
            SZ_HASH_KEYTYPE,
            szCb.init.region,
            szCb.init.pool);

   if (ret != ROK)
   {
      szFreeNdbPeerCb(locCb);
      /* Undo the hash list init before returning */
      cmHashListDeinit(&locCb->conRefLst);
      RETVALUE(RFAILED);
   }

   locCb->peerId = peerId;
   locCb->assocCb->peer = locCb;

   ret = cmHashListInsert (&szCb.peerLst, (PTR)locCb,
            (U8 *)&locCb->peerId, sizeof (U32));
   if (ret != ROK)
   {
      szFreeNdbPeerCb(locCb);
      /* Undo the hash list init before returning */
      cmHashListDeinit(&locCb->conRefLst);
      cmHashListDeinit(&locCb->conRefLstRem);
      RETVALUE(RFAILED);
   }

   *peer = locCb;
   szCb.nodeCb.nmbMap ++;
   szCb.genSta.nmbPeer++;

   RETVALUE(ROK);   
} /* end of szNdbAllocPeerCb */


/*
*
*       Fun:  szFreeNdbPeerCb  
*
*       Desc:  Called to free the peer control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PRIVATE S16 szFreeNdbPeerCb
(
SzPeerCb *locCb
)
#else /* ANSI */
PRIVATE S16 szFreeNdbPeerCb(locCb)
SzPeerCb *locCb;
#endif /* ANSI */
{
   TRC3(szFreeNdbPeerCb);

      SZ_FREE(locCb->assocCb->assocCfg,sizeof(SzAssocCfg));
      SZ_FREE(locCb->assocCb,sizeof(SzAssocCb));
      SZ_FREE(locCb,sizeof(SzPeerCb));

      RETVALUE(ROK);
} /* end of szFreeNdbPeerCb */

#ifdef SZ_MME
/*
*
*       Fun:   szNdbAllocDynamPeerCb
*
*       Desc:  Allocates a transient peer and assoc control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/

#ifdef ANSI
PUBLIC S16 szNdbAllocDynamPeerCb
(
SuId      suId,
SzPeerCb **peer
)
#else /* ANSI */
PUBLIC S16 szNdbAllocDynamPeerCb(suId, peer)
SuId      suId;
SzPeerCb **peer;
#endif /* ANSI */
{
   SzPeerCb   *peerCb = NULLP;
   U16         offset = 0;
   SzConCb    conCb;
   U32 nmbCon;
   S16         ret;

   TRC3(szNdbAllocDynamPeerCb)

   *peer = NULLP;


   /* Allocate peer control block */
   SZ_ALLOC(peerCb, sizeof(SzPeerCb));
   if(peerCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ118, (ErrVal) 0,
           "szNdbAllocDynamPeerCb: Couldn't allocate memory for peerCB");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      RETVALUE(RFAILED);
   }
    
   /* Allocate Assoc control block in peer cb */
   SZ_ALLOC(peerCb->assocCb, sizeof(SzAssocCb));
   if(peerCb->assocCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ119, (ErrVal) 0,
           "szNdbAllocDynamPeerCb: Couldn't allocate memory for assocCB");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      SZ_FREE(peerCb, sizeof(SzPeerCb));
      RETVALUE(RFAILED);
   }

   /* Allocate assoc config in assoc control block */
   SZ_ALLOC(peerCb->assocCb->assocCfg, sizeof(SzAssocCfg));
   if(peerCb->assocCb->assocCfg == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ120, (ErrVal) 0,
           "szNdbAllocDynamPeerCb: Couldn't allocate memory for assocCfg");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      SZ_FREE(peerCb->assocCb, sizeof(SzAssocCb));
      SZ_FREE(peerCb, sizeof(SzPeerCb));
      RETVALUE(RFAILED);
   }

   /* Allocate assoc config in assoc control block */
   SZ_ALLOC(peerCb->assocParams, sizeof(SctAssocIndParams));
   if(peerCb->assocParams == NULLP)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ121, (ErrVal) 0,
           "szNdbAllocDynamPeerCb: Couldn't allocate memory for assocParams");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
      SZ_FREE(peerCb->assocCb->assocCfg, sizeof(SzAssocCfg));
      SZ_FREE(peerCb->assocCb, sizeof(SzAssocCb));
      SZ_FREE(peerCb, sizeof(SzPeerCb));
      RETVALUE(RFAILED);
   }


   peerCb->assocCb->peer = peerCb;
   peerCb->assocCb->assocCfg->comProcStrm.pres = PRSNT_NODEF;
   peerCb->assocCb->assocCfg->comProcStrm.val  = 0;

   peerCb->isDynamCfgd = TRUE;
   peerCb->trcMask     = 0;
   peerCb->trcLen      = 0;
   peerCb->setupReq    = NULLP;
   peerCb->comStreamId = 0;
   peerCb->sctSapCb = szCb.sctSapLst[suId];
   peerCb->state = SZT_ND_ST_IDLE;

   /* 
    * Update the Szt SAP as the default SAP 
    * This shall be finally updated when the
    * response to the dynamic Peer
    */
   peerCb->sztSapCb = szCb.sztSapLst[szCb.genCfg.defDPeerUSapId];

   if(szCb.genCfg.nmbPeer > 1)
   {
      nmbCon = szCb.genCfg.nmbConn / szCb.genCfg.nmbPeer;
      nmbCon++;
   }
   else
   {
      nmbCon = szCb.genCfg.nmbConn;
   }

   offset = (U16)((PTR)&conCb.peerConHlEnt - (PTR)&conCb);
   ret = cmHashListInit(&peerCb->conRefLst,
            (U16)nmbCon,
            offset,
            FALSE,
            SZ_HASH_KEYTYPE,
            szCb.init.region,
            szCb.init.pool);

   if (ret != ROK)
   {
      szFreeNdbDynamPeerCb(peerCb);

      RETVALUE(RFAILED);
   }

   /* Initialize the hash list of remote connection control block */
   offset = (U16)((PTR)&conCb.peerConHlEntRem - (PTR)&conCb);
   ret = cmHashListInit(&peerCb->conRefLstRem,
            (U16)nmbCon,
            offset,
            FALSE,
            SZ_HASH_KEYTYPE,
            szCb.init.region,
            szCb.init.pool);

   if (ret != ROK)
   {
      szFreeNdbDynamPeerCb(peerCb);

      /* Undo the hash list init before returning */
      cmHashListDeinit(&peerCb->conRefLst);

      RETVALUE(RFAILED);
   }

   szCb.genSta.nmbPeer++;
   *peer = peerCb;

   RETVALUE(ROK);   
} /* end of szNdbAllocDynamPeerCb */

/*
*
*       Fun:  szFreeNdbDynamPeerCb  
*
*       Desc:  Called to free the peer control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PRIVATE S16 szFreeNdbDynamPeerCb
(
SzPeerCb *peerCb
)
#else  /* ANSI */
PRIVATE S16 szFreeNdbDynamPeerCb(peerCb)
SzPeerCb *peerCb;
#endif /* ANSI */
{
   TRC3(szFreeNdbDynamPeerCb);

   SZ_FREE(peerCb->assocParams, sizeof(SctAssocIndParams));
   SZ_FREE(peerCb->assocCb->assocCfg, sizeof(SzAssocCfg));
   SZ_FREE(peerCb->assocCb, sizeof(SzAssocCb));
   SZ_FREE(peerCb, sizeof(SzPeerCb));

   RETVALUE(ROK);
} /* end of szFreeNdbDynamPeerCb */

#endif /* SZ_MME */

/*
*
*       Fun:   szNdbRelConns 
*
*       Desc:  Called to release all connections from peer control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbRelConns
(
SzPeerCb *peer
)
#else  /* ANSI */
PUBLIC S16 szNdbRelConns(peer)
SzPeerCb *peer;
#endif  /* ANSI */
{
   SzConCb* pCb;
   SzConCb* nCb;
   S16 ret = RFAILED;

   TRC2(szNdbRelConns)

   /* Remove the Connection Control Blocks */
   pCb = NULLP;
   nCb = NULLP;
   ret = cmHashListGetNext(&peer->conRefLst, (PTR)pCb, (PTR *)&nCb);
   for (;;)
   {
      if (ret != ROK)
      {
         break;
      }
      else
      {
         pCb = nCb;
         ret = cmHashListGetNext(&peer->conRefLst, (PTR)pCb, (PTR *)&nCb);
         cmHashListDelete (&szCb.lclConCp, (PTR)pCb);
         cmHashListDelete (&peer->conRefLst, (PTR)pCb);
         cmHashListDelete (&(peer->sztSapCb->suConnLst), (PTR)pCb);

         /* Delete the connection from remote connection list */
         cmHashListDelete (&peer->conRefLstRem, (PTR)pCb);

         /* Delete mpping for the connection control block */
#ifdef YT
         YT_DEL_MAPPING(YT_UPD_CONN_CB, pCb);
#endif /* YT */
         SZ_FREE(pCb, sizeof(SzConCb));
         szCb.genSts.nmbConn--;
         szCb.genSta.nmbActvPeerConn--;
         peer->nmbConn--;
      }
   }

   /* Send the runtime update message for PeerCb and AssocCb */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_PEER_CON_CB, peer);
#endif /* YT */

   RETVALUE(ROK);
} /* szNdbRelConns */

/*
*
*       Fun:   szNdbDeallocPeerCb
*
*       Desc:  Called to deallocate an available peer control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbDeallocPeerCb
(
SzPeerCb *peer
)
#else /* ANSI */
PUBLIC S16 szNdbDeallocPeerCb(peer)
SzPeerCb *peer;
#endif   /* ANSI */
{
   PTR pCb, nCb;
   S16 ret = RFAILED;

   TRC2(szNdbDeallocPeerCb)

   /* Remove the Connection Control Blocks */
   pCb = (PTR)NULLP;
   nCb = (PTR)NULLP;
   ret = cmHashListGetNext(&peer->conRefLst, pCb, &nCb);
   for (;;)
   {
      if (ret != ROK)
      {
         break;
      }
      else
      {
         pCb = nCb;
         ret = cmHashListGetNext(&peer->conRefLst, pCb, &nCb);
         cmHashListDelete (&szCb.lclConCp, (PTR)pCb);
         cmHashListDelete (&peer->conRefLst, (PTR)pCb);
         /* Delete the connection from the upper Sap List also */
         if(peer->sztSapCb != NULLP)
         {
            cmHashListDelete (&peer->sztSapCb->suConnLst, (PTR)pCb);
         }

         /* Delete the connection Cb from remote list */
         cmHashListDelete (&peer->conRefLstRem, (PTR)pCb);

         /* Delete mpping for the connection control block */
#ifdef YT
         YT_DEL_MAPPING(YT_UPD_CONN_CB, (SzConCb *)pCb);
#endif /* YT */

         SZ_FREE(pCb, sizeof(SzConCb));
         szCb.genSts.nmbConn--;
         szCb.genSta.nmbActvPeerConn--;
      }
   }

   /* Send the runtime update to delete the connections */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_PEER_CON_CB, peer);
   YT_RTUPD_DEL(YT_UPD_PEER_CB, peer);
   YT_DEL_MAPPING(YT_UPD_ASSOC_CB, peer->assocCb);
   YT_DEL_MAPPING(YT_UPD_PEER_CB, peer);
#endif /* YT */

   cmHashListDeinit(&peer->conRefLst);

   /* De-Initialize the remote connection hash list also */
   cmHashListDeinit(&peer->conRefLstRem);

   cmHashListDelete (&szCb.peerLst,(PTR)peer);
   ret = szNdbRemPeerAddrPortCb(peer);
   SZ_FREE_SDU(peer->resetPdu);
   SZ_FREE_SDU(peer->setupReq);
   SZ_FREE(peer->assocCb->assocCfg,sizeof(SzAssocCfg));
   SZ_FREE(peer->assocCb,sizeof(SzAssocCb));
   SZ_FREE(peer,sizeof(SzPeerCb));

   szCb.nodeCb.nmbMap--;
   szCb.genSta.nmbPeer--;

   RETVALUE(ROK);
} /* end of szNdbDeallocPeerCb */

#ifdef SZ_MME
/*
*
*       Fun:   szNdbDeallocDynamPeerCb
*
*       Desc:  deallocates the transient peer and assoc control block
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbDeallocDynamPeerCb
(
SzPeerCb *peer
)
#else /* ANSI */
PUBLIC S16 szNdbDeallocDynamPeerCb(peer)
SzPeerCb *peer;
#endif /* ANSI */
{
   TRC2(szNdbDeallocDynamPeerCb)

   cmHashListDeinit(&peer->conRefLst);

   /* De-Initialize the remote connection list also */
   cmHashListDeinit(&peer->conRefLstRem);

   SZ_FREE(peer->assocParams, sizeof(SctAssocIndParams));
   SZ_FREE(peer->assocCb->assocCfg, sizeof(SzAssocCfg));
   SZ_FREE(peer->assocCb ,sizeof(SzAssocCb));
   SZ_FREE(peer, sizeof(SzPeerCb));
   szCb.genSta.nmbPeer--;

   RETVALUE(ROK);
} /* end of szNdbDeallocDynamPeerCb */
#endif /* SZ_MME */

/*
*
*       Fun:   szNdbDeallocAllPeerCb
*
*       Desc:  Called to deallocate all the peers in szCb
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbDeallocAllPeerCb 
(
SpId   sapId,
U8     sapDir,
U8     opn
)
#else /* ANSI */
PUBLIC S16 szNdbDeallocAllPeerCb(sapId, sapDir, opn)
SpId   sapId;
U8     sapDir;
U8     opn;
#endif /* ANSI */
{
   SztStaInd   s1apSta;
   SzPeerCb   *crnt = NULLP;
   SzPeerCb   *next = NULLP; 
   Bool        assocAbort = FALSE;
   Bool        skip = FALSE;
   S16         ret = ROK;

   TRC2(szNdbDeallocAllPeerCb);

   while (cmHashListGetNext(&szCb.peerLst,
            (PTR)crnt,
            (PTR *)&next) == ROK)
   {
      skip = FALSE;
      if (sapDir == SZ_UI)
      {
         assocAbort = FALSE;
         if ( (next->sztSapCb != NULL) && 
              (next->sztSapCb->spId == sapId) )
         {
            skip = FALSE;
         }
         else 
         {
            if (opn == SZ_SHUT_DOWN)
            {
               skip = FALSE; 
            }
            else 
            {
               skip = TRUE;
            }
         }
      }
      else
      {
         assocAbort = TRUE;
         if ( (next->sctSapCb != NULL) &&
              (next->sctSapCb->suId == sapId) )
         {
           skip = FALSE;  
         }
         else
         {
            if (opn == SZ_SHUT_DOWN)
            {
               skip = FALSE; 
            } 
            else
            {
               skip = TRUE;
            }
         }
      }
      if (skip == FALSE)
      {
         next->state = SZT_ND_ST_RESET; 
         if (opn == SZ_UBND_SAP)
         {
             next->delPcb = FALSE;
         }
         else
         {
              next->delPcb = TRUE; 
         }
      /* next->delPcb = (opn == SZ_UBND_SAP) ? FALSE :
                        TRUE;*/

         /* Fill s1apSta for the user */
         s1apSta.type = SZT_STA_TYPE_ASSOC;
         s1apSta.status = SZT_STATUS_ASSOC_DOWN;
         s1apSta.reason = SZT_REASON_UBND_SAP;
         s1apSta.peerId.pres = TRUE;
         s1apSta.peerId.val = next->peerId;

         /* Send StaInd to the user */
         if ( (next->sztSapCb != NULL) && 
              (next->sztSapCb->state == LSZ_SAP_ISBOUND) )
         {
            SzUiSztStaInd (&next->sztSapCb->pst,
                            next->sztSapCb->suId, &s1apSta);
         }

         /* Terminate the association */
         ret = szLiTermAssoc(&next,assocAbort);
         /* sz008.301 : Handling ROKPENDING also */
         if((ret != ROK) && (ret != ROKPENDING))
         {
            RETVALUE(RFAILED);
         }
      }
      crnt = next;
      next = NULLP;
   }
   RETVALUE(ROK);
} /* szNdbDeallocAllPeerCb */

/*
*
*       Fun:   szNdbFillPeerAssocParams
*
*       Desc:  Adds peer addr Port control block to the hash list
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbFillPeerAssocParams
(
SzPeerCb           *peerCb,
SzAssocCfg         *assocCfg
)
#else /* ANSI */
PUBLIC S16 szNdbFillPeerAssocParams(peerCb,assocCfg)
SzPeerCb   *peerCb;
SzAssocCfg         *assocCfg;
#endif /* ANSI */
{
   U16                    idx;
   SctNetAddrLst          addrLst;
   S16                    ret;

   TRC2(szNdbFillPeerAssocParams)

   cmMemcpy ((U8 *)peerCb->assocCb->assocCfg, 
             (U8 *)assocCfg, sizeof (SzAssocCfg));

   addrLst.nmb = 0;
   cmMemcpy ((U8 *) &addrLst.nAddr[addrLst.nmb],(U8 *) &assocCfg->priDstAddr,
             sizeof (CmNetAddr));
   addrLst.nmb++;

   
   for (idx = 0; idx < assocCfg->dstAddrLst.nmb;idx++)
   {
      if(addrLst.nmb < SCT_MAX_NET_ADDRS)    /* Bound checking for array */
      {
         if (assocCfg->dstAddrLst.nAddr[idx].type == CM_NETADDR_IPV4)
         {
            if ( assocCfg->dstAddrLst.nAddr[idx].u.ipv4NetAddr !=
                  assocCfg->priDstAddr.u.ipv4NetAddr)
            {
               addrLst.nAddr[addrLst.nmb].u.ipv4NetAddr = 
                  assocCfg->dstAddrLst.nAddr[idx].u.ipv4NetAddr;
               addrLst.nmb++;
            }
         }
         else
         {
            if (cmMemcmp((U8 *)&(assocCfg->dstAddrLst.nAddr[idx].u.ipv6NetAddr),
                     (U8 *)&(assocCfg->priDstAddr.u.ipv6NetAddr),
                     (PTR) CM_IPV6ADDR_SIZE))
            {
               (Void) cmMemcpy( addrLst.nAddr[addrLst.nmb].u.ipv6NetAddr,
                                (U8 *)&(assocCfg->dstAddrLst.nAddr[idx].u.ipv6NetAddr),
                                (PTR) CM_IPV6ADDR_SIZE);
               addrLst.nmb++;
            }
         }
      }
   }
   ret = szNdbAddPeerAddrPortCb(&addrLst, assocCfg->dstPort, 
                                peerCb);

   RETVALUE(ret);
} /* end of  szNdbFillPeerAssocParams */

/*
*
*       Fun:   szNdbAddPeerAddrPortCb
*
*       Desc:  Adds peer addr Port control block to the hash list
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbAddPeerAddrPortCb
(
SctNetAddrLst      *dstAddrLst,
SctPort             dstPort,
SzPeerCb           *peerCb
)
#else /* ANSI */
PUBLIC S16 szNdbAddPeerAddrPortCb(dstAddrLst, dstPort, peerCb)
SctNetAddrLst      *dstAddrLst;
SctPort             dstPort;
SzPeerCb   *peerCb;
#endif /* ANSI */
{
   SzPeerAddrPortCb    *peerAssocCb;
   U16                    idx;
   S16                    ret;

   TRC2(szNdbAddPeerAddrPortCb)

   peerAssocCb = NULLP;

   for (idx = 0; idx < dstAddrLst->nmb; idx++)
   {
      SZ_ALLOC(peerAssocCb, sizeof(SzPeerAddrPortCb));
      if (NULLP == peerAssocCb)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR(ERRCLS_INT_PAR, ESZ122, (ErrVal) 0,
               "SzLiSctAssocInd: Failed to allocate the dynamPeerAssocCb\n");
#endif /* if ERRCLASS */

         szNdbRemPeerAddrPortCb(peerCb);

         RETVALUE(RFAILED);
      }

      peerAssocCb->peerCbPtr   = peerCb;
      peerAssocCb->peerAddrPort.peerAddr.type = dstAddrLst->nAddr[idx].type;

      if (peerAssocCb->peerAddrPort.peerAddr.type == CM_NETADDR_IPV4)
      {
         peerAssocCb->peerAddrPort.peerAddr.u.ipv4NetAddr = dstAddrLst->nAddr[idx].u.ipv4NetAddr;
      }
      else
      {
         (Void) cmMemcpy((U8 *)&(peerAssocCb->peerAddrPort.peerAddr.u.ipv6NetAddr),
                         (U8 *)&(dstAddrLst->nAddr[idx].u.ipv6NetAddr),
                         (PTR) CM_IPV6ADDR_SIZE);
      }
      peerAssocCb->peerAddrPort.port = dstPort;

      ret = cmHashListInsert(&(szCb.peerAddrPortLst), (PTR)(peerAssocCb),
             (U8 *)&(peerAssocCb->peerAddrPort), sizeof(SzPeerAddrPortEntry));

      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR(ERRCLS_INT_PAR, ESZ123, (ErrVal) 0,
               "SzLiSctAssocInd: Failed to insert in the peerAddr hash list\n");
#endif /* if ERRCLASS */

         szNdbRemPeerAddrPortCb(peerCb);
         szNdbDeallocPeerCb(peerCb);

         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* end of szNdbAddPeerAddrPortCb */

/*
*
*       Fun:   szNdbFindPeerAddrPortCb
*
*       Desc:  finds peer addr Port control block from the hash list
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbFindPeerAddrPortCb
(
SctNetAddrLst      *dstAddrLst,
SctPort             dstPort,
SzPeerCb **peerAssoc
)
#else /* ANSI */
PUBLIC S16 szNdbFindPeerAddrPortCb(dstAddrLst, dstPort, peerAssoc)
SctNetAddrLst      *dstAddrLst;
SctPort             dstPort;
SzPeerCb   **peerAssoc;
#endif /* ANSI */
{
   SzPeerAddrPortEntry  peerAssocEntry;
   SzPeerAddrPortCb    *peerAssocCb;
   U16                    idx;
   S16                    ret;

   TRC2(szNdbFindPeerAddrPortCb)

   peerAssocCb = NULLP;

   for (idx = 0; idx < dstAddrLst->nmb; idx++)
   {
      SZ_ZERO(&peerAssocEntry, sizeof(SzPeerAddrPortEntry));

      peerAssocEntry.peerAddr.type = dstAddrLst->nAddr[idx].type;

      if (peerAssocEntry.peerAddr.type == CM_NETADDR_IPV4)
      {
         peerAssocEntry.peerAddr.u.ipv4NetAddr = dstAddrLst->nAddr[idx].u.ipv4NetAddr;
      }
      else
      {
         (Void) cmMemcpy((U8 *)&(peerAssocEntry.peerAddr.u.ipv6NetAddr),
                         (U8 *)&(dstAddrLst->nAddr[idx].u.ipv6NetAddr),
                         (PTR) CM_IPV6ADDR_SIZE);
      }

      peerAssocEntry.port = dstPort;

      ret = cmHashListFind(&(szCb.peerAddrPortLst), (U8 *) &(peerAssocEntry),
            sizeof(SzPeerAddrPortEntry), 0, (PTR *) &peerAssocCb);
      if (ret == ROK)
      {
         break;
      }
   }

   if ( peerAssocCb == (SzPeerAddrPortCb*)NULLP )
   {
      RETVALUE(RFAILED);
   }
   else
   {
      *peerAssoc = peerAssocCb->peerCbPtr;
      ret = ROK;
   }

   RETVALUE(ret);
}  /* end of szNdbFindPeerAddrPortCb */

/*
*
*       Fun:   szNdbRemPeerAddrPortCb
*
*       Desc:  removes peer and assoc control block from the hash list
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_ndb.c
*
*/
#ifdef ANSI
PUBLIC S16 szNdbRemPeerAddrPortCb
(
SzPeerCb *peer
)
#else /* ANSI */
PUBLIC S16 szNdbRemPeerAddrPortCb(peer)
SzPeerCb *peer;
#endif /* ANSI */
{
   SctNetAddrLst         *dstAddrLst;
   SzPeerAddrPortEntry  peerAssocEntry;
   SzPeerAddrPortCb    *peerAssocCb;
   Bool                   allFound;
   U16                    idx;
   S16                    ret;

   TRC2(szNdbRemPeerAddrPortCb)

   allFound = TRUE;
   dstAddrLst = &peer->assocCb->assocCfg->dstAddrLst;

   for (idx = 0; idx < dstAddrLst->nmb; idx++)
   {
      SZ_ZERO(&peerAssocEntry, sizeof(SzPeerAddrPortEntry));

      peerAssocEntry.peerAddr.type = dstAddrLst->nAddr[idx].type;

      if (peerAssocEntry.peerAddr.type == CM_NETADDR_IPV4)
         peerAssocEntry.peerAddr.u.ipv4NetAddr = dstAddrLst->nAddr[idx].u.ipv4NetAddr;
      else
         (Void) cmMemcpy((U8 *)&(peerAssocEntry.peerAddr.u.ipv6NetAddr),
                         (U8 *)&(dstAddrLst->nAddr[idx].u.ipv6NetAddr),
                         (PTR) CM_IPV6ADDR_SIZE);

      peerAssocEntry.port = peer->assocCb->assocCfg->dstPort;

      ret = cmHashListFind(&(szCb.peerAddrPortLst), (U8 *) &(peerAssocEntry),
            sizeof(SzPeerAddrPortEntry), 0, (PTR *) &peerAssocCb);
      if (ret != ROK)
      {
         allFound = FALSE;
         continue;
      }
      
      cmHashListDelete(&(szCb.peerAddrPortLst), (PTR)(peerAssocCb));
      SZ_FREE(peerAssocCb, sizeof(SzPeerAddrPortCb));
   }

   if (allFound == FALSE)
      RETVALUE(RFAILED);
   
   RETVALUE(ROK);
} /* end of szNdbRemPeerAddrPortCb */


/**********************************************************************
 
         End of file:     sq_ndb.c@@/main/nodal_main/3 - Fri Nov 22 17:58:23 2013
 
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
/main/5      ---      va          1. Updated for S1AP release 4.1
*********************************************************************91*/
