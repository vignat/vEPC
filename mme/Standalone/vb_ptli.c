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

     Name:    LTE CNE - MME, S-GW, PDN-GW Portable Lower Interface \

     Type:    C source file

     Desc:    

     File:    vb_ptli.c

     Sid:      vb_ptli.c@@/main/1 - Mon Jun 28 19:35:39 2010

     Prg:     ap
*********************************************************************21*/


/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library           */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */

#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "lvb.h"           /* CNE Layer management           */
#include "vb_hss.h"        /* CNE Application defines        */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "vbsm.h"          /* */
#include "vb_err.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library           */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "sct.x"           /* SCT interface defines           */

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */
/* vb007.101 : s1ap include files order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */
#include "vbsm.x"          /*    */


#define PTVBLISOT
#define PTVBLISZT
/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTVBLISOT

PRIVATE S16 PtLiSztBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiSztUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
PRIVATE S16 PtLiSztUDatReq      ARGS((Pst *pst, SpId spId, SztUDatEvnt *uDatEvnt));
PRIVATE S16 PtLiSztConReq       ARGS((Pst *pst, SpId spId, SztConReq *conReq));
PRIVATE S16 PtLiSztConRsp       ARGS((Pst *pst, SpId spId, SztConRsp *conRsp));
PRIVATE S16 PtLiSztRelReq       ARGS((Pst *pst, SpId spId, SztRelReq *relReq));
PRIVATE S16 PtLiSztRelRsp       ARGS((Pst *pst, SpId spId, SztRelRsp *relRsp));
PRIVATE S16 PtLiSztDatReq       ARGS((Pst *pst, SpId spId, SztDatEvntReq *datEvnt));
PRIVATE S16 PtLiSztEncDecReq    ARGS((Pst *pst, SpId spId, SztEncDecEvnt *sztEncDecEvnt));
PRIVATE S16 PtLiSztAudReq       ARGS((Pst *pst, SpId spId, SztAudEvnt *audEvnt));

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PRIVATE S16 PtLiSztPeerAssocRsp ARGS((Pst *pst, SpId spId, SztPeerAssocRsp *peerAssocRsp));
#endif

#endif


/* Following matrices define the mapping between the primitives called by the
 * S1AP user (as VbLiSztXXXReq) and the corresponding primitives of the 
 * S1AP layer (service provider) (as SzUiSztXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCVBLISZT    loose         cmPkSztXXXReq
 * 1                 SZ          tight         SzUiSztXXXReq
 * 2                LWLCVBLISZT  LW loose      cmPkSztXXXReq
 */


PUBLIC SztBndReq VbLiSztBndReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztBndReq,        /* 0 - loosely coupled */
#else
   PtLiSztBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztUbndReq VbLiSztUbndReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztUbndReq,        /* 0 - loosely coupled */
#else
   PtLiSztUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztUbndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztUDatReq VbLiSztUDatReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztUDatReq,        /* 0 - loosely coupled */
#else
   PtLiSztUDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztUDatReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztUDatReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztUDatReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztUDatReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztConnReq VbLiSztConReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztConReq,        /* 0 - loosely coupled */
#else
   PtLiSztConReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztConReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztConReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztConReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztConReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztConnRsp VbLiSztConRspMt [] =
{
#ifdef LCVBLISZT
   cmPkSztConRsp,        /* 0 - loosely coupled */
#else
   PtLiSztConRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztConRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztConRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztConRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztConRsp,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztRelsReq VbLiSztRelReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztRelReq,        /* 0 - loosely coupled */
#else
   PtLiSztRelReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztRelReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztRelReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztRelReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztRelReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztRelsRsp VbLiSztRelRspMt [] =
{
#ifdef LCVBLISZT
   cmPkSztRelRsp,        /* 0 - loosely coupled */
#else
   PtLiSztRelRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztRelRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztRelRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztRelRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztRelRsp,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztDatReq VbLiSztDatReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztDatReq,        /* 0 - loosely coupled */
#else
   PtLiSztDatReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztDatReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztDatReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztDatReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztDatReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC SztEncDecReq VbLiSztEncDecReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztEncDecReq,        /* 0 - loosely coupled */
#else
   PtLiSztEncDecReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztEncDecReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztEncDecReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztEncDecReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztEncDecReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC SztAudReq VbLiSztAudReqMt [] =
{
#ifdef LCVBLISZT
   cmPkSztAudReq,        /* 0 - loosely coupled */
#else
   PtLiSztAudReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztAudReq,        /* 1 - light weight loosely coupled */
#else
   PtLiSztAudReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztAudReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztAudReq,        /* 2 - tightly coupled, portable */
#endif
};

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
PUBLIC SztDynamPeerAssocRsp VbLiSztPeerAssocRspMt [] =
{
#ifdef LCVBLISZT
   cmPkSztPeerAssocRsp,        /* 0 - loosely coupled */
#else
   PtLiSztPeerAssocRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCVBLISZT
   cmPkSztPeerAssocRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiSztPeerAssocRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef SZ
   SzUiSztPeerAssocRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiSztPeerAssocRsp,        /* 2 - tightly coupled, portable */
#endif
};
#endif

#if (!defined(LWLCEULIEGT) || !defined(LCEULIEGT))
#define PTEULIEGT
#else
#ifndef EG
#define PTEULIEGT
#else
#endif
#endif

#if (!defined(LCEULIHIT))
#define PTEULIHIT
#else
#ifndef HI
#define PTEULIHIT
#else
#endif
#endif

/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTEULIEGT

PRIVATE S16 PtLiEgtBndReq   ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiEgtUbndReq  ARGS((Pst *pst, SpId spId, Reason reason));

#ifdef EGTP_C
PRIVATE S16 PtLiEgtSigReq   ARGS((Pst *pst, SpId spId,                
                                  CmTptAddr   *localAddr, /* Local node address */
                                  CmTptAddr   *remAddr,   /* Remote node address */
                                  TknU32      lclTeid,    /* Local Tunnel Identifier */
                                  TknU32      transId,    /* Transaction Identifier */
                                  TknU8       crtFlag,    /* Create tunnel or not */
                                  TmrCfg      T3,         /* Request timer expiry */
                                  EgMsg    *egMsg));

PRIVATE S16 PtLiEgtSigRsp   ARGS((Pst *pst, SpId spId, 
                                  TknU32    lclTeid,   /* Local TEID */
                                  EgMsg    *egMsg));

PRIVATE S16 PtLiEgtStaReq   ARGS((Pst *pst, SpId spId, 
                                  U8        eventType, 
                                  EgPathInfo   *pathInfo));

PRIVATE S16 PtLiEgtLclDelReq   ARGS((Pst *pst, SpId spId, 
                                     U32   transId,
                                     U32   teid));
#endif /* EGTP_C */

#endif


/* Following matrices define the mapping between the primitives called by the
 * eGTP user (as VbLiEgtXXXReq) and the corresponding primitives of the 
 * eGTP layer (service provider) (as EgUiEgtXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCEULIEGT    loose         cmPkEgtXXXReq
 * 1                 EG          tight         EgUiEgtXXXReq
 * 2                LWLCEULIEGT  LW loose      cmPkEgtXXXReq
 */

PUBLIC EgtBndReq VbLiEgtBndReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtBndReq,        /* 0 - loosely coupled */
#else
   PtLiEgtBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC EgtUbndReq VbLiEgtUbndReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtUbndReq,        /* 0 - loosely coupled */
#else
   PtLiEgtUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtUbndReq,        /* 2 - tightly coupled, portable */
#endif
};


#ifdef EGTP_C
PUBLIC EgtSigReq VbLiEgtSigReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtSigReq,        /* 0 - loosely coupled */
#else
   PtLiEgtSigReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtSigReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtSigReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtSigReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtSigReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC EgtSigRsp VbLiEgtSigRspMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtSigRsp,        /* 0 - loosely coupled */
#else
   PtLiEgtSigRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtSigRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtSigRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtSigRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtSigRsp,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtStaReq VbLiEgtStaReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtStaReq,        /* 0 - loosely coupled */
#else
   PtLiEgtStaReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtStaReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtStaReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtStaReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtStaReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtLclDelReq VbLiEgtLclDelReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtLclDelReq,        /* 0 - loosely coupled */
#else
   PtLiEgtLclDelReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtLclDelReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtLclDelReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtLclDelReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtLclDelReq,        /* 2 - tightly coupled, portable */
#endif
};

#endif /* EGTP_C */

/************************************************************************
                       SZT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   VbLiSztBndReq
 *
 *       Desc:  This function resolves the SztBndReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztBndReq
(
Pst   *pst,
SuId   suId,
SpId   spId
)
#else 
PUBLIC S16 VbLiSztBndReq (pst, suId, spId)
Pst   *pst;
SuId   suId; 
SpId   spId;
#endif
{
   TRC3(VbLiSztBndReq)

   (*VbLiSztBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);
} /* VbLiSztBndReq */

/*
 *
 *       Fun:   VbLiSztUbndReq
 *
 *       Desc:  This function resolves the SztUbndReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztUbndReq
(
Pst   *pst,
SpId   spId,
Reason reason
)
#else 
PUBLIC S16 VbLiSztUbndReq (pst, spId, reason)
Pst   *pst;
SpId   spId;
Reason reason;
#endif
{
   TRC3(VbLiSztUbndReq)

   (*VbLiSztUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);
} /* VbLiSztUbndReq */

/*
 *
 *       Fun:   VbLiSztUDatReq
 *
 *       Desc:  This function resolves the SztUDatReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztUDatReq
(
Pst         *pst,
SpId         spId,
SztUDatEvnt *uDatEvnt
)
#else 
PUBLIC S16 VbLiSztUDatReq (pst, spId, uDatEvnt)
Pst         *pst;
SpId         spId;
SztUDatEvnt *uDatEvnt;
#endif
{
   TRC3(VbLiSztUDatReq)

   (*VbLiSztUDatReqMt[pst->selector])(pst, spId, uDatEvnt);

   RETVALUE(ROK);
} /* VbLiSztUDatReq */

/*
 *
 *       Fun:   VbLiSztConReq
 *
 *       Desc:  This function resolves the SztConReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztConReq
(
Pst       *pst,
SpId       spId,
SztConReq *conReq
)
#else 
PUBLIC S16 VbLiSztConReq (pst, spId, conReq)
Pst       *pst;
SpId       spId;
SztConReq *conReq;
#endif
{
   TRC3(VbLiSztConReq)

   (*VbLiSztConReqMt[pst->selector])(pst, spId, conReq);

   RETVALUE(ROK);
} /* VbLiSztConReq */

/*
 *
 *       Fun:   VbLiSztConRsp
 *
 *       Desc:  This function resolves the SztConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztConRsp
(
Pst       *pst,
SpId       spId,
SztConRsp *conRsp
)
#else 
PUBLIC S16 VbLiSztConRsp (pst, spId, conRsp)
Pst       *pst;
SpId       spId;
SztConRsp *conRsp;
#endif
{
   TRC3(VbLiSztConRsp)

   (*VbLiSztConRspMt[pst->selector])(pst, spId, conRsp);

   RETVALUE(ROK);
} /* VbLiSztConRsp */

/*
 *
 *       Fun:   VbLiSztRelReq
 *
 *       Desc:  This function resolves the SztRelReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztRelReq
(
Pst       *pst,
SpId       spId,
SztRelReq *relReq
)
#else 
PUBLIC S16 VbLiSztRelReq (pst, spId, relReq)
Pst       *pst;
SpId       spId;
SztRelReq *relReq;
#endif
{
   TRC3(VbLiSztRelReq)

   (*VbLiSztRelReqMt[pst->selector])(pst, spId, relReq);

   RETVALUE(ROK);
} /* VbLiSztRelReq */

/*
 *
 *       Fun:   VbLiSztRelRsp
 *
 *       Desc:  This function resolves the SztRelRsp primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztRelRsp
(
Pst       *pst,
SpId       spId,
SztRelRsp *relRsp
)
#else 
PUBLIC S16 VbLiSztRelRsp (pst, spId, relRsp)
Pst       *pst;
SpId       spId;
SztRelRsp *relRsp;
#endif
{
   TRC3(VbLiSztRelRsp)

   (*VbLiSztRelRspMt[pst->selector])(pst, spId, relRsp);

   RETVALUE(ROK);
} /* VbLiSztRelRsp */

/*
 *
 *       Fun:   VbLiSztDatReq
 *
 *       Desc:  This function resolves the SztDatReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztDatReq
(
Pst           *pst,
SpId           spId,
SztDatEvntReq *datEvnt
)
#else 
PUBLIC S16 VbLiSztDatReq (pst, spId, datEvnt)
Pst           *pst;
SpId           spId;
SztDatEvntReq *datEvnt;
#endif
{
   TRC3(VbLiSztDatReq)

   (*VbLiSztDatReqMt[pst->selector])(pst, spId, datEvnt);

   RETVALUE(ROK);
} /* VbLiSztDatReq */

/*
 *
 *       Fun:   VbLiSztEncDecReq
 *
 *       Desc:  This function resolves the SztEncDecReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztEncDecReq
(
Pst           *pst,
SpId           spId,
SztEncDecEvnt *sztEncDecEvnt
)
#else 
PUBLIC S16 VbLiSztEncDecReq (pst, spId, sztEncDecEvnt)
Pst           *pst;
SpId           spId;
SztEncDecEvnt *sztEncDecEvnt;
#endif
{
   TRC3(VbLiSztEncDecReq)

   (*VbLiSztEncDecReqMt[pst->selector])(pst, spId, sztEncDecEvnt);

   RETVALUE(ROK);
} /* VbLiSztEncDecReq */

/*
 *
 *       Fun:   VbLiSztAudReq
 *
 *       Desc:  This function resolves the SztAudReq primitive
 *
 *       Ret:  ROK - ok
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiSztAudReq
(
Pst        *pst,
SpId        spId,
SztAudEvnt *audEvnt
)
#else 
PUBLIC S16 VbLiSztAudReq (pst, spId, audEvnt)
Pst        *pst;
SpId        spId;
SztAudEvnt *audEvnt;
#endif
{
   TRC3(VbLiSztAudReq)

   (*VbLiSztAudReqMt[pst->selector]) (pst, spId, audEvnt);

   RETVALUE(ROK);
} /* VbLiSztAudReq */



/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTVBLISZT
/*
 *
 *       Fun:   PtLiSztBndReq
 *
 *       Desc:  Portable version of SztBndReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PRIVATE S16 PtLiSztBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(PtLiSztBndReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB088, (ErrVal) 0, 
              "PtLiSztBndReq() called");
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiSztBndReq */

/*
 *
 *       Fun:   PtLiSztUbndReq
 *
 *       Desc:  Portable version of SztUbndReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztUbndReq
(
Pst   *pst,
SpId   spId,
Reason reason
)
#else 
PRIVATE S16 PtLiSztUbndReq (pst, spId, reason)
Pst   *pst;
SpId   spId;
Reason reason;
#endif
{
   TRC3(PtLiSztUbndReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB089, (ErrVal) 0, 
              "PtLiSztUbndReq() called");
   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiSztUbndReq */

/*
 *
 *       Fun:   PtLiSztUDatReq
 *
 *       Desc:  Portable version of SztUDatReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztUDatReq
(
Pst         *pst,
SpId         spId,
SztUDatEvnt *uDatEvnt
)
#else 
PRIVATE S16 PtLiSztUDatReq (pst, spId, uDatEvnt)
Pst         *pst;
SpId         spId;
SztUDatEvnt *uDatEvnt;
#endif
{
   TRC3(PtLiSztUDatReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB090, (ErrVal) 0, 
              "PtLiSztUDatReq() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(uDatEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztUDatReq */

/*
 *
 *       Fun:   PtLiSztConReq
 *
 *       Desc:  Portable version of SztConReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztConReq
(
Pst       *pst,
SpId       spId,
SztConReq *conReq
)
#else 
PRIVATE S16 PtLiSztConReq (pst, spId, conReq)
Pst       *pst;
SpId       spId;
SztConReq *conReq;
#endif
{
   TRC3(PtLiSztConReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB091, (ErrVal) 0, 
              "PtLiSztConReq() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conReq);

   RETVALUE(RFAILED);
} /* PtLiSztConReq */

/*
 *
 *       Fun:   PtLiSztConRsp
 *
 *       Desc:  Portable version of SztConRsp primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztConRsp
(
Pst       *pst,
SpId       spId,
SztConRsp *conRsp
)
#else 
PRIVATE S16 PtLiSztConRsp (pst, spId, conRsp)
Pst       *pst;
SpId       spId;
SztConRsp *conRsp;
#endif
{
   TRC3(PtLiSztConRsp)

   VBLOGERROR(ERRCLS_DEBUG, EVB092, (ErrVal) 0, 
              "PtLiSztConRsp() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(conRsp);

   RETVALUE(RFAILED);
} /* PtLiSztConRsp */

/*
 *
 *       Fun:   PtLiSztRelReq
 *
 *       Desc:  Portable version of SztRelReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztRelReq
(
Pst       *pst,
SpId       spId,
SztRelReq *relReq
)
#else 
PRIVATE S16 PtLiSztRelReq (pst, spId, relReq)
Pst       *pst;
SpId       spId;
SztRelReq *relReq;
#endif
{
   TRC3(PtLiSztRelReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB093, (ErrVal) 0, 
              "PtLiSztRelReq() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relReq);

   RETVALUE(RFAILED);
} /* PtLiSztRelReq */

/*
 *
 *       Fun:   PtLiSztRelRsp
 *
 *       Desc:  Portable version of SztRelRsp primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztRelRsp
(
Pst       *pst,
SpId       spId,
SztRelRsp *relRsp
)
#else 
PRIVATE S16 PtLiSztRelRsp (pst, spId, relRsp)
Pst       *pst;
SpId       spId;
SztRelRsp *relRsp;
#endif
{
   TRC3(PtLiSztRelRsp)

   VBLOGERROR(ERRCLS_DEBUG, EVB094, (ErrVal) 0, 
              "PtLiSztRelRsp() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(relRsp);

   RETVALUE(RFAILED);
} /* PtLiSztRelRsp */

/*
 *
 *       Fun:   PtLiSztDatReq
 *
 *       Desc:  Portable version of SztDatReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztDatReq
(
Pst *pst,
SpId spId,
SztDatEvntReq *datEvnt
)
#else 
PRIVATE S16 PtLiSztDatReq (pst, spId, datEvnt)
Pst *pst;
SpId spId;
SztDatEvntReq *datEvnt;
#endif
{
   TRC3(PtLiSztDatReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB095, (ErrVal) 0, 
              "PtLiSztDatReq() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(datEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztDatReq */

/*
 *
 *       Fun:   PtLiSztEncDecReq
 *
 *       Desc:  Portable version of SztEncDecReq primitive
 *
 *       Ret:   RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztEncDecReq
(
Pst *pst,
SpId spId,
SztEncDecEvnt *sztEncDecEvnt
)
#else 
PRIVATE S16 PtLiSztEncDecReq (pst, spId, sztEncDecEvnt)
Pst *pst;
SpId spId;
SztEncDecEvnt *sztEncDecEvnt;
#endif
{
   TRC3(PtLiSztEncDecReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB096, (ErrVal) 0, 
              "PtLiSztEncDecReq() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(sztEncDecEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztEncDecReq */

/*
 *
 *       Fun:   PtLiSztAudReq
 *
 *       Desc:  Portable version of SztAudReq primitive
 *
 *       Ret:  RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztAudReq
(
Pst *pst,
SpId spId,
SztAudEvnt *audEvnt
)
#else 
PRIVATE S16 PtLiSztAudReq (pst, spId, audEvnt)
Pst *pst;
SpId spId;
SztAudEvnt *audEvnt;
#endif
{
   TRC3(PtLiSztAudReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB097, (ErrVal) 0, 
              "PtLiSztAudReq() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(audEvnt);

   RETVALUE(RFAILED);
} /* PtLiSztAudReq */

#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
/*
 *
 *       Fun:   PtLiSztPeerAssocRsp
 *
 *       Desc:  Portable version of SztPeerAssocRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiSztPeerAssocRsp
(
Pst *pst,
SpId spId,
SztPeerAssocRsp *peerAssocRsp
)
#else 
PRIVATE S16 PtLiSztPeerAssocRsp (pst, spId, peerAssocRsp)
Pst *pst;
SpId spId;
SztPeerAssocRsp *peerAssocRsp;
#endif
{
   TRC3(PtLiSztPeerAssocRsp)

   VBLOGERROR(ERRCLS_DEBUG, EVB098, (ErrVal) 0, 
              "PtLiSztPeerAssocRsp() called");
   UNUSED(pst);
   UNUSED(spId);
   UNUSED(peerAssocRsp);

   RETVALUE(RFAILED);
} /* PtLiSztPeerAssocRsp */
#endif

#endif /* PTVBLISZT */


/************************************************************************
                       EGT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   VbLiEgtBndReq
 *
 *       Desc:  This function resolves the EgtBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiEgtBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PUBLIC S16 VbLiEgtBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(VbLiEgtBndReq)

   (*VbLiEgtBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* VbLiEgtBndReq */

/*
 *
 *       Fun:   VbLiEgtUbndReq
 *
 *       Desc:  This function resolves the EgtUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiEgtUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PUBLIC S16 VbLiEgtUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(VbLiEgtUbndReq)

   (*VbLiEgtUbndReqMt[pst->selector])
      (pst, spId, reason);

   RETVALUE(ROK);
} /* VbLiEgtUbndReq */

#ifdef EGTP_C
/*
 *
 *       Fun:   VbLiEgtSigReq
 *
 *       Desc:  This function resolves the EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiEgtSigReq
(
Pst         *pst,
SpId        spId,
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
TknU32      transId,    /* Transaction Identifier */
TknU8       crtFlag,    /* Create tunnel or not */
TmrCfg      T3,         /* Request timer expiry */
EgMsg       *egMsg
)
#else 
PUBLIC S16 VbLiEgtSigReq (pst, spId, localAddr, remAddr, lclTeid, transId, 
                          crtFlag, T3, egMsg)
Pst         *pst;
SpId        spId;
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
TknU32      transId;    /* Transaction Identifier */
TknU8       crtFlag;    /* Create tunnel or not */
TmrCfg      T3;         /* Request timer expiry */
EgMsg       *egMsg;
#endif
{
   TRC3(VbLiEgtSigReq)

   (*VbLiEgtSigReqMt[pst->selector])
      (pst, spId, localAddr,remAddr, lclTeid, transId, crtFlag, T3, egMsg);

   RETVALUE(ROK);
} /* VbLiEgtSigReq */

/*
 *
 *       Fun:   VbLiEgtSigRsp
 *
 *       Desc:  This function resolves the EgtConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiEgtSigRsp
(
Pst *pst,
SpId spId,
TknU32      lclTeid,   /* Local TEID */
EgMsg    *egMsg
)
#else 
PUBLIC S16 VbLiEgtSigRsp (pst, spId, lclTeid, egMsg)
Pst *pst;
SpId spId;
TknU32      lclTeid;   /* Local TEID */
EgMsg    *egMsg;
#endif
{
   TRC3(VbLiEgtSigRsp)

   (*VbLiEgtSigRspMt[pst->selector])
      (pst, spId, lclTeid, egMsg);

   RETVALUE(ROK);
} /* VbLiEgtSigRsp */


/*
 *
 *       Fun:   VbLiEgtStaReq
 *
 *       Desc:  This function resolves the EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiEgtStaReq
(
Pst         *pst,
SpId         spId,
U8           eventType,         /* Request timer expiry */
EgPathInfo  *pathInfo
)
#else 
PUBLIC S16 VbLiEgtStaReq (pst, spId, eventType, pathInfo)
Pst         *pst;
SpId         spId;
U8           eventType;         /* Request timer expiry */
EgPathInfo  *pathInfo;
#endif
{
   TRC3(VbLiEgtStaReq)

   (*VbLiEgtStaReqMt[pst->selector])
      (pst, spId, eventType, pathInfo);

   RETVALUE(ROK);
} /* VbLiEgtStaReq */


/*
 *
 *       Fun:   VbLiEgtLclDelReq
 *
 *       Desc:  This function resolves the EgtLclDelReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbLiEgtLclDelReq
(
Pst *pst,
SpId spId,
U32 transId, 
U32 teid
)
#else 
PUBLIC S16 VbLiEgtLclDelReq (pst, spId, transId, teid)
Pst *pst;
SpId spId;
U32 transId;
U32 teid;
#endif
{
   TRC3(VbLiEgtLclDelReq)

   (*VbLiEgtLclDelReqMt[pst->selector])
      (pst, spId, transId, teid);

   RETVALUE(ROK);
} /* VbLiEgtLclDelReq */

#endif /* EGTP_C */

/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTEULIEGT
/*
 *
 *       Fun:   PtLiEgtBndReq
 *
 *       Desc:  Portable version of EgtBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PRIVATE S16 PtLiEgtBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   TRC3(PtLiEgtBndReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB099, (ErrVal) 0, 
              "PtLiEgtBndReq() called");
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiEgtBndReq */

/*
 *
 *       Fun:   PtLiEgtUbndReq
 *
 *       Desc:  Portable version of EgtUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PRIVATE S16 PtLiEgtUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   TRC3(PtLiEgtUbndReq)

   VBLOGERROR(ERRCLS_DEBUG, EVB100, (ErrVal) 0, 
              "PtLiEgtUbndReq() called");
   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiEgtUbndReq */


#ifdef EGTP_C
/*
 *
 *       Fun:   PtLiEgtSigReq
 *
 *       Desc:  Portable version of EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtSigReq
(
Pst *pst,
SpId spId,
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
TknU32      transId,    /* Transaction Identifier */
TknU8       crtFlag,    /* Create tunnel or not */
TmrCfg      T3,         /* Request timer expiry */
EgMsg    *egMsg
)
#else 
PRIVATE S16 PtLiEgtSigReq (pst, spId, localAddr, remAddr, lclTeid, transId,
                           crtFlag, T3, egMsg)
Pst *pst;
SpId spId;
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
TknU32      transId;    /* Transaction Identifier */
TknU8       crtFlag;    /* Create tunnel or not */
TmrCfg      T3;         /* Request timer expiry */
EgMsg       *egMsg;
#endif
{
   TRC3(PtLiEgtSigReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(egMsg);

   RETVALUE(RFAILED);
} /* PtLiEgtSigReq */

/*
 *
 *       Fun:   PtLiEgtSigRsp
 *
 *       Desc:  Portable version of EgtConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtSigRsp
(
Pst *pst,
SpId spId,
TknU32      lclTeid,   /* Local TEID */
EgMsg    *egMsg
)
#else 
PRIVATE S16 PtLiEgtSigRsp (pst, spId, lclTeid, egMsg)
Pst      *pst;
SpId     spId;
TknU32   lclTeid;   /* Local TEID */
EgMsg    *egMsg;
#endif
{
   TRC3(PtLiEgtSigRsp)


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(lclTeid);
   UNUSED(egMsg);

   RETVALUE(RFAILED);
} /* PtLiEgtSigRsp */

/*
 *
 *       Fun:   PtLiEgtStaReq
 *
 *       Desc:  Portable version of EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtStaReq
(
Pst *pst,
SpId spId,
U8   eventType,    /* Status event type */
EgPathInfo   *pathInfo
)
#else 
PRIVATE S16 PtLiEgtStaReq (pst, spId, eventType, pathInfo)
Pst *pst;
SpId spId;
U8   eventType;    /* Status event type */
EgPathInfo   *pathInfo;
#endif
{
   TRC3(PtLiEgtStaReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(eventType);
   UNUSED(pathInfo);

   RETVALUE(RFAILED);
} /* PtLiEgtStaReq */

/*
 *
 *       Fun:   PtLiEgtLclDelReq
 *
 *       Desc:  Portable version of EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  vb_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtLclDelReq
(
Pst *pst,
SpId spId,
U32   transId,    /* Status event type */
U32          teid
)
#else 
PRIVATE S16 PtLiEgtLclDelReq (pst, spId, transId, teid)
Pst *pst;
SpId spId;
U32 transId;    /* Status event type */
U32          teid;
#endif
{
   TRC3(PtLiEgtLclDelReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(teid);

   RETVALUE(RFAILED);
} /* PtLiEgtLclDelReq */

#endif /* EGTP_C */
#endif /* PTEULIEGT */

/********************************************************************30**

         End of file:     vb_ptli.c@@/main/1 - Mon Jun 28 19:35:39 2010

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
/main/1      -          sp           1. LTE-CNE Initial Release.
/main/1   vb007.101 akaranth         1. s1ap include files order has changed.
*********************************************************************91*/
