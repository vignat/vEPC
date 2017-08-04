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
  
     Name:     TUCL user - lower interface
  
     Type:     C source file
  
     Desc:     Code for Lower Interface of TCP UDP Convergence Layer
               user.
              
     File:     l5_ptli.c
  
     Sid:      l5_ptli.c@@/main/6 - Mon Mar  3 20:09:36 2008

     Prg:      asa
  
*********************************************************************21*/

/*

The following functions are provided in this file:
     HcLiHitBndReq      - Bind Request
     HcLiHitUbndReq     - Un Bind Request
     HcLiHitServOpenReq - Server Open Request
     HcLiHitConReq      - Connect Request
     HcLiHitConRsp      - Connect Response 
     HcLiHitDatReq      - Data Request
     HcLiHitUDatReq     - Unit Data Request
     HcLiHitDiscReq     - Disconnect Request
It should be noted that not all of these functions may be required
by a particular TUCL user

*/  
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*
*
*/
 
   
/* header include files (.h) */
#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

/* external headers */
#ifdef HI_TLS
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport defines */
#include "hit.h"           /* HI Upper Interface */
#include "lhi.h"           /* HI Mgmt Interface */
#ifdef FTHA
#include "sht.h"
#endif
#include "hi_acc.h"        /* HI acceptance */

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* common sockets */
#include "cm_lib.x"        /* common library functions */
#include "cm_tpt.x"        /* common transport typedefs */
#ifdef HI_LKSCTP
#include "sct.h"        
#include "sct.x"        
#endif
#include "hit.x"           /* HI Upper Interface */
#include "lhi.x"           /* HI Mgmt Interface */
#ifdef FTHA
#include "sht.x"
#endif
#include "hi_acc.x"        /* HI acceptance */

/* local defines */
#define MAXHCLI     2

#ifndef LCHCLIHIT
#define PTHCLIHIT
#else
#ifndef HI
#define PTHCLIHIT
#endif
#endif

#ifdef HI_LKSCTP
#ifndef LCHCLISCT
#define PTHCLISCT
#endif
#endif

#ifdef PTHCLIHIT
/* declaration of portable functions */
PRIVATE S16 PtLiHitBndReq  ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiHitUbndReq ARGS((Pst *pst, SuId suId, Reason reason));
PRIVATE S16 PtLiHitServOpenReq ARGS((Pst *pst, SpId spId, UConnId servConId, 
                                     CmTptAddr *servTAddr, CmTptParam *tPar,
                                     CmIcmpFilter *icmpFilter, U8 srvcType)); 
PRIVATE S16 PtLiHitConReq  ARGS((Pst *pst, SpId spId, UConnId suConId, 
                                 CmTptAddr *remAddr, CmTptAddr *localAddr, 
                                 CmTptParam *tPar, U8 srvcType)); 
PRIVATE S16 PtLiHitConRsp  ARGS((Pst *pst, SpId spId, UConnId suConId, 
                                 UConnId spConId));
PRIVATE S16 PtLiHitDatReq  ARGS((Pst *pst, SpId spId, UConnId spConId, 
                                 Buffer *mBuf));
PRIVATE S16 PtLiHitUDatReq ARGS((Pst *pst, SpId spId, UConnId spConId, 
                                 CmTptAddr *remAddr, CmTptAddr *srcAddr,
                                 CmIpHdrParm *hdrParm, CmTptParam *tPar,
                                 Buffer *mBuf));
PRIVATE S16 PtLiHitDiscReq ARGS((Pst *pst, SpId spId, U8 choice, UConnId conId, 
                                 Action action, CmTptParam *tPar));
#endif 

#ifdef HI_LKSCTP
#ifdef PTHCLISCT
PRIVATE S16 PtLiSctBndReq            ARGS((Pst           *pst,
                                          SuId           suId,
                                          SpId           spId));

#ifdef SCT_ENDP_MULTI_IPADDR
PRIVATE S16 PtLiSctEndpOpenReq       ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        suEndpId,
                                          SctPort        port,
                                          SctNetAddrLst  *srcAddrLst));
#else
PRIVATE S16 PtLiSctEndpOpenReq       ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        suEndpId,
                                          SctPort        port,
                                          CmNetAddr     *intfNAddr));
#endif
PRIVATE S16 PtLiSctEndpCloseReq      ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        endpId,
                                          U8             endpIdType));
#ifdef SCT3
PRIVATE S16 PtLiSctAssocReq          ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spEndpId,
                                          UConnId        suAssocId,
                                          CmNetAddr     *priDstNAddr,
                                          SctPort        dstPort,
                                          SctStrmId      outStrms,
                                          SctNetAddrLst *dstNAddrLst,
                                          SctNetAddrLst *srcNAddrLst,
                                          SctTos         tos,
                                          Buffer        *vsInfo));
#else /* SCT3 */
PRIVATE S16 PtLiSctAssocReq          ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spEndpId,
                                          UConnId        suAssocId,
                                          CmNetAddr     *priDstNAddr,
                                          SctPort        dstPort,
                                          SctStrmId      outStrms,
                                          SctNetAddrLst *dstNAddrLst,
                                          SctNetAddrLst *srcNAddrLst,
                                          Buffer        *vsInfo));
#endif /* SCT3 */
#ifdef SCT3
PRIVATE S16 PtLiSctAssocRsp          ARGS((Pst               *pst,
                                          SpId               spId,
                                          UConnId            spEndpId,
                                          SctAssocIndParams *assocIndParams,
                                          SctTos             tos,
                                          SctResult          result,
                                          Buffer            *vsInfo));
#else /* SCT3 */
PRIVATE S16 PtLiSctAssocRsp          ARGS((Pst               *pst,
                                          SpId               spId,
                                          UConnId            spEndpId,
                                          SctAssocIndParams *assocIndParams,
                                          SctResult          result,
                                          Buffer            *vsInfo));
#endif /* SCT3 */
PRIVATE S16 PtLiSctTermReq           ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        assocId,
                                          U8             assocIdType,
                                          Bool           abrtFlg));
PRIVATE S16 PtLiSctDatReq            ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spAssocId,
                                          CmNetAddr     *dstNAddr,
                                          SctStrmId      strmId,
                                          Bool           unorderFlg,
                                          Bool           nobundleFlg,
                                          U16            lifeTime,
                                          U32            protId,
                                          Buffer        *mBuf));
PRIVATE S16 PtLiSctStaReq            ARGS((Pst           *pst,
                                          SpId           spId,
                                          UConnId        spAssocId,
                                          CmNetAddr     *dstNAddr,
                                          U8             staType));

PRIVATE S16 PtLiSctSetPriReq         ARGS((Pst            *pst,
                                          SpId            spId,
                                          UConnId         spAssocId,
                                          CmNetAddr      *dstNAddr));

PRIVATE S16 PtLiSctHBeatReq          ARGS((Pst            *pst,
                                          SpId            spId,
                                          UConnId         spAssocId,
                                          CmNetAddr      *dstNAddr,
                                          U16             intervalTime,
                                          SctStatus       status));
#endif
#endif


/*
   Primitive Mapping Functions 

   0 - loosely coupled TUCL (#ifdef LCHCLIHIT)
   1 - tightly coupled TUCL (#ifdef HI) 
*/
/* Bind Request primitive */

PRIVATE HitBndReq hcLiHitBndReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitBndReq,         /* 0 - loosely coupled */
#else
   PtLiHitBndReq,         /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitBndReq          /* 1 - tightly coupled, HI */
#else
   PtLiHitBndReq          /* 1 - tightly coupled, portable */
#endif
};

/* Un-Bind Request primitive */

PRIVATE HitUbndReq hcLiHitUbndReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitUbndReq,        /* 0 - loosely coupled */
#else
   PtLiHitUbndReq,        /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitUbndReq         /* 1 - tightly coupled, HI */
#else
   PtLiHitUbndReq         /* 1 - tightly coupled, portable */
#endif
};

/* Server Open Request primitive */

PRIVATE HitServOpenReq hcLiHitServOpenReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitServOpenReq,    /* 0 - loosely coupled */
#else
   PtLiHitServOpenReq,    /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitServOpenReq     /* 1 - tightly coupled, HI */
#else
   PtLiHitServOpenReq     /* 1 - tightly coupled, portable */
#endif
};

/* Connect Request primitive */

PRIVATE HitConReq hcLiHitConReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitConReq,         /* 0 - loosely coupled */
#else
   PtLiHitConReq,         /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitConReq          /* 1 - tightly coupled, HI */
#else
   PtLiHitConReq          /* 1 - tightly coupled, portable */
#endif
};


/* Connect Response primitive */

PRIVATE HitConRsp hcLiHitConRspMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitConRsp,        /* 0 - loosely coupled */
#else
   PtLiHitConRsp,        /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitConRsp         /* 1 - tightly coupled, HI */
#else
   PtLiHitConRsp         /* 1 - tightly coupled, portable */
#endif
};

/* Data Request primitive */

PRIVATE HitDatReq hcLiHitDatReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitDatReq,        /* 0 - loosely coupled */
#else
   PtLiHitDatReq,        /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitDatReq         /* 1 - tightly coupled, HI */
#else
   PtLiHitDatReq         /* 1 - tightly coupled, portable */
#endif
};


/* Unit Data Request primitive */

PRIVATE HitUDatReq hcLiHitUDatReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitUDatReq,       /* 0 - loosely coupled */
#else
   PtLiHitUDatReq,       /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitUDatReq        /* 1 - tightly coupled, HI */
#else
   PtLiHitUDatReq        /* 1 - tightly coupled, portable */
#endif
};

/* Disconnect Request primitive */

PRIVATE HitDiscReq hcLiHitDiscReqMt[MAXHCLI] =
{
#ifdef LCHCLIHIT
   cmPkHitDiscReq,      /* 0 - loosely coupled */
#else
   PtLiHitDiscReq,      /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiHitDiscReq       /* 1 - tightly coupled, HI */
#else
   PtLiHitDiscReq       /* 1 - tightly coupled, portable */
#endif
};


#ifdef HI_LKSCTP

/* Bind Request primitive */

PRIVATE SctBndReq hcLiSctBndReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctBndReq,         /* 0 - loosely coupled */
#else
   PtLiSctBndReq,         /* 0 - tightly coupled, portable */
#endif

#ifdef HI
   HiUiSctBndReq          /* 1 - tightly coupled, HI */
#else
   PtLiSctBndReq          /* 1 - tightly coupled, portable */
#endif
};

/* Open endpoint request primitive */

PRIVATE SctEndpOpenReq HcLiSctEndpOpenReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctEndpOpenReq,     /* 0 - loosely coupled */
#else
   PtLiSctEndpOpenReq,     /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctEndpOpenReq,     /* 1 - tightly coupled, layer management */
#else
   PtLiSctEndpOpenReq      /* 1 - tightly coupled, portable */
#endif
};

/* Close endpoint request primitive */

PRIVATE SctEndpCloseReq HcLiSctEndpCloseReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctEndpCloseReq,     /* 0 - loosely coupled */
#else
   PtLiSctEndpCloseReq,     /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctEndpCloseReq,     /* 1 - tightly coupled, layer management */
#else
   PtLiSctEndpCloseReq      /* 1 - tightly coupled, portable */
#endif
};

/* Association request primitive */

PRIVATE SctAssocReq HcLiSctAssocReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctAssocReq,        /* 0 - loosely coupled */
#else
   PtLiSctAssocReq,        /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctAssocReq,        /* 1 - tightly coupled, layer management */
#else
   PtLiSctAssocReq        /* 1 - tightly coupled, portable */
#endif
};

/* Association response primitive */

PRIVATE SctAssocRsp HcLiSctAssocRspMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctAssocRsp,        /* 0 - loosely coupled */
#else
   PtLiSctAssocRsp,        /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctAssocRsp,        /* 1 - tightly coupled, layer management */
#else
   PtLiSctAssocRsp         /* 1 - tightly coupled, portable */
#endif
};

/* Association termination request primitive */

PRIVATE SctTermReq HcLiSctTermReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctTermReq,        /* 0 - loosely coupled */
#else
   PtLiSctTermReq,        /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctTermReq,        /* 1 - tightly coupled, layer management */
#else
   PtLiSctTermReq         /* 1 - tightly coupled, portable */
#endif
};

/* Set primary address request primitive */

PRIVATE SctSetPriReq HcLiSctSetPriReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctSetPriReq,       /* 0 - loosely coupled */
#else
   PtLiSctSetPriReq,       /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctSetPriReq,       /* 1 - tightly coupled, layer management */
#else
   PtLiSctSetPriReq        /* 1 - tightly coupled, portable */
#endif
};

/* Heartbeat request primitive */

PRIVATE SctHBeatReq HcLiSctHBeatReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctHBeatReq,        /* 0 - loosely coupled */
#else
   PtLiSctHBeatReq,        /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctHBeatReq,        /* 1 - tightly coupled, layer management */
#else
   PtLiSctHBeatReq         /* 1 - tightly coupled, portable */
#endif
};

/* Data request primitive */

PRIVATE SctDatReq HcLiSctDatReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctDatReq,          /* 0 - loosely coupled */
#else
   PtLiSctDatReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctDatReq,          /* 1 - tightly coupled, layer management */
#else
   PtLiSctDatReq          /* 1 - tightly coupled, portable */
#endif
};


/* Status request primitive */

PRIVATE SctStaReq HcLiSctStaReqMt[MAXHCLI] =
{
#ifdef LCHCLISCT
   cmPkSctStaReq,         /* 0 - loosely coupled */
#else
   PtLiSctStaReq,         /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiUiSctStaReq,        /* 1 - tightly coupled, layer management */
#else
   PtLiSctStaReq         /* 1 - tightly coupled, portable */
#endif
};

#endif




/* 
 * Layer3 (Service User) Lower Interface Functions
 */

/*
*
*       Fun:   lower interface - Bind Request
*
*       Desc:  This function resolves the Bind Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitBndReq
(
Pst       *pst,          /* Post structure */
SuId      suId,          /* service user sap id */
SpId      spId           /* service provider sap id */
)
#else
PUBLIC S16 HcLiHitBndReq(pst, suId, spId)
Pst      *pst;           /* Post structure */
SuId      suId;          /* service user sap id */
SpId      spId;          /* service provider sap id */
#endif
{
   TRC3(HcLiHitBndReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiHitBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);
} /* end of HcLiHitBndReq */


/*
*
*       Fun:   lower interface - Bind Request
*
*       Desc:  This function resolves the Bind Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitUbndReq
(
Pst *pst,              /* Post structure */
SpId spId,             /* Service provider Id */
Reason reason          /* cause for unbind operation */
)
#else
PUBLIC S16 HcLiHitUbndReq(pst, spId, reason)
Pst *pst;              /* Post structure */
SpId spId;             /* Service provider Id */
Reason reason;         /* cause for unbind operation */
#endif
{
   TRC3(HcLiHitUbndReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiHitUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);
} /* end of HcLiHitUbndReq */


/*
*
*       Fun:   lower interface - Server Open Request 
*
*       Desc:  This function resolves the Server Open Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitServOpenReq
(
Pst *pst,                 /* post stucture */
SpId spId,                /* service provider Id */
UConnId servConId,        /* service user's server connection Id */
CmTptAddr *servTAddr,     /* transport address of the server */ 
CmTptParam *tPar,         /* transport options */
CmIcmpFilter *icmpFilter, /* Filter parameters */
U8  srvcType              /* type of service */
)
#else
PUBLIC S16 HcLiHitServOpenReq(pst, spId, servConId, servTAddr, tPar,
                              icmpFilter, srvcType) 
Pst *pst;                  /* post stucture */
SpId spId;                 /* service provider Id */
UConnId servConId;         /* service user's server connection Id */
CmTptAddr  *servTAddr;     /* transport address of the server */ 
CmTptParam *tPar;          /* transport options */
CmIcmpFilter *icmpFilter; /* Filter parameters */
U8  srvcType;              /* service type */
#endif
{
   TRC3(HcLiHitServOpenReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */
   (Void) (*hcLiHitServOpenReqMt[pst->selector])(pst, spId, servConId,
                                                 servTAddr, tPar, 
                                                 icmpFilter, srvcType);

   RETVALUE(ROK);
} /* end of HcLiHitServOpenReq */


/*
*
*       Fun:   lower interface - Connect Request
*
*       Desc:  This function resolves the Connect Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitConReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId suConId,         /* service user's connection id */ 
CmTptAddr *remAddr,      /* address of the server */ 
CmTptAddr *localAddr,    /* local address */
CmTptParam *tPar,        /* transport parameters */
U8  srvcType             /* type of service */
)
#else
PUBLIC S16 HcLiHitConReq(pst, spId, suConId, remAddr, localAddr, tPar, srvcType) 
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId suConId;         /* service user's connection id */ 
CmTptAddr *remAddr;      /* address of the server */ 
CmTptAddr *localAddr;    /* local address */
CmTptParam *tPar;        /* transport parameters */
U8  srvcType;            /* type of service */
#endif
{
   TRC3(HcLiHitConReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiHitConReqMt[pst->selector])(pst, spId, suConId, remAddr,
                                            localAddr, tPar, srvcType);

   RETVALUE(ROK);
} /* end of HcLiHitConReq */


/*
*
*       Fun:   lower interface - Connect Response
*
*       Desc:  This function resolves the Connect Response primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitConRsp
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId suConId,          /* service user's connection id */ 
UConnId spConId           /* service provider's connection id */
)
#else
PUBLIC S16 HcLiHitConRsp(pst, spId, suConId, spConId)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId suConId;          /* service user's connection id */ 
UConnId spConId;          /* service provider's connection id */
#endif
{
   TRC3(HcLiHitConRsp)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiHitConRspMt[pst->selector])(pst,spId,suConId,spConId);

   RETVALUE(ROK);
} /* end of HcLiHitConRsp */


/*
*
*       Fun:   lower interface - Data Request
*
*       Desc:  This function resolves the Data Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitDatReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId spConId,          /* service provider's connection id */
Buffer *mBuf             /* message buffer */
)
#else
PUBLIC S16 HcLiHitDatReq(pst, spId, spConId, mBuf)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId spConId;          /* service provider's connection id */
Buffer *mBuf;            /* message buffer */
#endif
{
   TRC3(HcLiHitDatReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiHitDatReqMt[pst->selector])(pst,spId,spConId,mBuf);

   RETVALUE(ROK);
} /* end of HcLiHitDatReq */


/*
*
*       Fun:   lower interface - Unit Data Request
*
*       Desc:  This function resolves the Unit Data Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitUDatReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId spConId,         /* service provider connection id */
CmTptAddr *remAddr,      /* remote address */
CmTptAddr *srcAddr,      /* Source Address */
CmIpHdrParm *hdrParm,    /* Header Parameters */
CmTptParam *tPar,        /* transport parameters */
Buffer *mBuf             /* message buffer to be sent */
)
#else
PUBLIC S16 HcLiHitUDatReq(pst, spId, spConId, remAddr, srcAddr, 
                          hdrParm, tPar, mBuf)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId  spConId;        /* service provider connection id */
CmTptAddr *remAddr;      /* remote address */
CmTptAddr *srcAddr;      /* Source Address */
CmIpHdrParm *hdrParm;    /* Header Parameters */
CmTptParam *tPar;        /* transport parameters */
Buffer *mBuf;            /* message buffer to be sent */
#endif
{
   TRC3(HcLiHitUDatReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */
   (Void)(*hcLiHitUDatReqMt[pst->selector])(pst, spId, spConId, remAddr,
                                            srcAddr, hdrParm, tPar, mBuf);

   RETVALUE(ROK);
} /* end of HcLiHitUDatReq */


/*
*
*       Fun:   lower interface - Disconnect Request
*
*       Desc:  This function resolves the Disconnect Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitDiscReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
U8 choice,               /* choice of user or provider connection id */
UConnId conId,            /* connection id */
Action action,           /* action to be performed */
CmTptParam  *tPar      /* transport parameters */ 
)
#else
PUBLIC S16 HcLiHitDiscReq(pst, spId, choice, conId, action, tPar)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
U8 choice;               /* choice of user or provider connection id */
UConnId conId;            /* connection id */
Action action;           /* action to be performed */
CmTptParam  *tPar;     /* transport parameters */ 
#endif
{
   TRC3(HcLiHitDiscReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiHitDiscReqMt[pst->selector])(pst,spId,choice,conId,action,tPar);

   RETVALUE(ROK);
} /* end of HcLiHitDiscReq */


#ifdef HI_LKSCTP

#ifdef PTHCLISCT
/* Portable lower interface functions */


/*
*
*       Fun:   Portable bind Request
*
*       Desc:  This function is used to request a bind
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctBndReq
(
Pst *pst,                 /* post structure */
SuId suId,                /* Service user SAP Id */
SpId spId                /* Service provider SAP Id */
)
#else
PRIVATE S16 PtLiSctBndReq(pst, suId, spId)
Pst *pst;                 /* post structure */
SuId suId;                /* Service user SAP Id */
SpId spId;                /* Service provider SAP Id */
#endif
{
   TRC3(PtLiSctBndReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctBndReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctBndReq */


/*
*
*       Fun:   Portable open endpoint request
*
*       Desc:  This function is used to request a new endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef SCT_ENDP_MULTI_IPADDR
#ifdef ANSI
PRIVATE S16 PtLiSctEndpOpenReq
(
Pst *pst,                    /* post structure */
SpId spId,                   /* Service provider SAP Id */
UConnId suEndpId,          /* Service user Endpoint Id */
U16 port,                    /* SCTP Port number */
SctNetAddrLst *srcAddrLst     /* Interface IP address list  */
)
#else
PRIVATE S16 PtLiSctEndpOpenReq(pst, spId, suEndpId, port, srcAddrLst)
Pst *pst;                    /* post structure */
SpId spId;                   /* Service provider SAP Id */
UConnId suEndpId;          /* Service user Endpoint Id */
U16 port;                    /* SCTP Port number */
SctNetAddrLst *srcAddrLst;    /* Interface IP address list  */
#endif
#else
#ifdef ANSI
PRIVATE S16 PtLiSctEndpOpenReq
(
Pst *pst,                    /* post structure */
SpId spId,                   /* Service provider SAP Id */
UConnId suEndpId,          /* Service user Endpoint Id */
U16 port,                    /* SCTP Port number */
CmNetAddr *intfNAddr         /* Interface IP address   */
)
#else
PRIVATE S16 PtLiSctEndpOpenReq(pst, spId, suEndpId, port, intfNAddr)
Pst *pst;                    /* post structure */
SpId spId;                   /* Service provider SAP Id */
UConnId suEndpId;          /* Service user Endpoint Id */
U16 port;                    /* SCTP Port number */
CmNetAddr *intfNAddr;        /* Interface IP address   */
#endif
#endif
{
   TRC3(PtLiSctEndpOpenReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctEndpOpenReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctEndpOpenReq */


/*
*
*       Fun:   Portable close endpoint request
*  
*       Desc:  This function is used to close an endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctEndpCloseReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAPID */
UConnId endpId,           /* endpoint ID */
U8 endpIdType             /* endpoint ID type */
)
#else
PRIVATE S16 PtLiSctEndpCloseReq(pst, spId, endpId, endpIdType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAPID */
UConnId endpId;           /* endpoint ID */
U8 endpIdType;            /* endpoint ID type */
#endif
{
   TRC3(PtLiSctEndpCloseReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctEndpCloseReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctEndpCloseReq */


/*
*
*       Fun:   Portable Association request
*
*       Desc:  This function is used to request a new association
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

/* it009.106 - Added type of service parameter */
#ifdef SCT3
#ifdef ANSI
PRIVATE S16 PtLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,        /* service provider endpoint ID */
UConnId suAssocId,      /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
U16 dstPort,               /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
SctTos tos,                 /* type of service */
Buffer *vsInfo          /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, 
                            dstPort, outStrms, dstNAddrLst, srcNAddrLst, tos, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;        /* service provider endpoint ID */
UConnId suAssocId;      /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
U16 dstPort;               /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
SctTos tos;                 /* type of service */
Buffer *vsInfo;          /* vendor specific info */
#endif
#else /* SCT3 */
#ifdef ANSI
PRIVATE S16 PtLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,        /* service provider endpoint ID */
UConnId suAssocId,      /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
U16 dstPort,               /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
Buffer *vsInfo          /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, 
                            dstPort, outStrms, dstNAddrLst, srcNAddrLst, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;        /* service provider endpoint ID */
UConnId suAssocId;      /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
U16 dstPort;               /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
Buffer *vsInfo;          /* vendor specific info */
#endif
#endif /* SCT3 */
{
   TRC3(PtLiSctAssocReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctAssocReq() called");
#endif /* ERRCLS_DEBUG */


   RETVALUE(ROK);
} /* end of PtLiSctAssocReq */


/*
*
*       Fun:   Portable association response
*
*       Desc:  This function is used to respond to an association indication
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

/* it009.106 - Added type of service parameter */
#ifdef SCT3
#ifdef ANSI
PRIVATE S16 PtLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spAssocId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctTos tos,                /* type of service */
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocRsp(pst, spId, spAssocId, assocIndParams, tos, result, 
                            vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spAssocId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctTos tos;                /* type of service */
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#else /* SCT3 */
#ifdef ANSI
PRIVATE S16 PtLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spAssocId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PRIVATE S16 PtLiSctAssocRsp(pst, spId, spAssocId, assocIndParams, result, 
                            vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spAssocId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#endif /* SCT3 */
{
   TRC3(PtLiSctAssocRsp);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctAssocRsp() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctAssocRsp */


/*
*
*       Fun:   Portable termination request
*
*       Desc:  This function is used to terminate an association
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctTermReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId assocId,          /* association ID */
U8 assocIdType,           /* association ID type */
Bool abrtFlg              /* abort flag */
)
#else
PRIVATE S16 PtLiSctTermReq(pst, spId, assocId, assocIdType, abrtFlg)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId assocId;          /* association ID */
U8 assocIdType;           /* association ID type */
Bool abrtFlg;             /* abort flag */
#endif
{
   TRC3(PtLiSctTermReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctTermReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctTermReq */


/*
*
*       Fun:   Portable data Request
*
*       Desc:  This function is used to send data to a peer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctDatReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr,      /* dest. network address */
SctStrmId strmId,         /* stream ID */
Bool unorderFlg,          /* unordered delivery flag */
Bool nobundleFlg,         /* nobundleFlg */
U16 lifetime,             /* datagram lifetime */
U32 protId,               /* protocol ID */
Buffer *mBuf              /* message buffer */
)
#else
PRIVATE S16 PtLiSctDatReq(pst, spId, spAssocId, dstNAddr, strmId, unorderFlg,
   nobundleFlg, lifetime, protId, mBuf)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
SctStrmId strmId;         /* stream ID */
Bool unorderFlg;          /* unordered delivery flag */
Bool nobundleFlg;         /* nobundleFlg */
U16 lifetime;             /* datagram lifetime */
U32 protId;               /* protocol ID */
Buffer *mBuf;             /* message buffer */
#endif
{
   TRC3(PtLiSctDatReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctDatReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctDatReq */


/*
*
*       Fun:   Portable status request
*
*       Desc:  This function is used to request status
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PRIVATE S16 PtLiSctStaReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAP ID */
UConnId spAssocId,        /* service provider association ID */
CmNetAddr *dstNAddr,      /* dest. network address */
U8 staType                /* status type */
)
#else
PRIVATE S16 PtLiSctStaReq(pst, spId, spAssocId, dstNAddr, staType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAP ID */
UConnId spAssocId;        /* service provider association ID */
CmNetAddr *dstNAddr;      /* dest. network address */
U8 staType;               /* status type */
#endif
{
   TRC3(PtLiSctStaReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctStaReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctStaReq */

#ifdef ANSI
PRIVATE S16 PtLiSctSetPriReq
(
Pst        *pst,
SpId        spId,
UConnId     spAssocId,
CmNetAddr  *dstNAddr
)
#else
PRIVATE S16 PtLiSctSetPriReq(pst, spId, spAssocId, dstNAddr)
Pst        *pst;
SpId        spId;
UConnId     spAssocId;
CmNetAddr  *dstNAddr;
#endif
{
   TRC3(PtLiSctSetPriReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctSetPriReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctSetPriReq */

#ifdef ANSI
PRIVATE S16 PtLiSctHBeatReq
(
Pst        *pst,
SpId        spId,
UConnId     spAssocId,
CmNetAddr  *dstNAddr,
U16         intervalTime,
SctStatus   status
)
#else
PRIVATE S16 PtLiSctHBeatReq(pst, spId, spAssocId, dstNAddr, intervalTime, status)
Pst        *pst;
SpId        spId;
UConnId     spAssocId;
CmNetAddr  *dstNAddr;
U16         intervalTime;
SctStatus   status;
#endif
{
   TRC3(PtLiSctHBeatReq);

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiSctHBeatReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiSctHBeatReq */

#endif

/*
*
*       Fun:   lower interface - Bind Request
*
*       Desc:  This function resolves the Bind Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiSctBndReq
(
Pst       *pst,          /* Post structure */
SuId      suId,          /* service user sap id */
SpId      spId           /* service provider sap id */
)
#else
PUBLIC S16 HcLiSctBndReq(pst, suId, spId)
Pst      *pst;           /* Post structure */
SuId      suId;          /* service user sap id */
SpId      spId;          /* service provider sap id */
#endif
{
   TRC3(HcLiSctBndReq)

   /* jump to specific primitive in service user, depending on selector 
      configured */

   (Void) (*hcLiSctBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);
}

/*
*
*       Fun:   Opening of endpoint request
*
*       Desc:  This function is used to request the opening of
*              an endpoint at the SCTP layer.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/
#ifdef SCT_ENDP_MULTI_IPADDR
#ifdef ANSI
PUBLIC S16 HcLiSctEndpOpenReq
(
Pst *pst,                        /* post structure */
SpId spId,                       /* Service provider SAP Id */
UConnId suEndpId,                /* Service user Endpoint Id */
SctPort port,                    /* SCTP Port number */
SctNetAddrLst *intfNAddrLst      /* Interface IP address list */
)
#else
PUBLIC S16 HcLiSctEndpOpenReq(pst, spId, suEndpId, port, intfNAddrLst)
Pst *pst;                        /* post structure */
SpId spId;                       /* Service provider SAP Id */
UConnId suEndpId;                /* Service user Endpoint Id */
SctPort port;                    /* SCTP Port number */
SctNetAddrLst *intfNAddrLst;     /* Interface IP address list */
#endif
#else /* SCT_ENDP_MULTI_IPADDR */
#ifdef ANSI
PUBLIC S16 HcLiSctEndpOpenReq
(
Pst *pst,                        /* post structure */
SpId spId,                       /* Service provider SAP Id */
UConnId suEndpId,                /* Service user Endpoint Id */
SctPort port,                    /* SCTP Port number */
CmNetAddr *intfNAddr             /* Interface IP address   */
)
#else
PUBLIC S16 HcLiSctEndpOpenReq(pst, spId, suEndpId, port, intfNAddr)
Pst *pst;                        /* post structure */
SpId spId;                       /* Service provider SAP Id */
UConnId suEndpId;                /* Service user Endpoint Id */
SctPort port;                    /* SCTP Port number */
CmNetAddr *intfNAddr;            /* Interface IP address   */
#endif
#endif /* SCT_ENDP_MULTI_IPADDR */
{
   TRC3(HcLiSctEndpOpenReq)
   /* jump to specific primitive depending on configured selector */
#ifdef SCT_ENDP_MULTI_IPADDR
   (*HcLiSctEndpOpenReqMt[pst->selector])(pst, spId, suEndpId, port, intfNAddrLst);
#else /* SCT_ENDP_MULTI_IPADDR */
   (*HcLiSctEndpOpenReqMt[pst->selector])(pst, spId, suEndpId, port, intfNAddr);
#endif /* SCT_ENDP_MULTI_IPADDR */
   RETVALUE(ROK);
} /* end of HcLiSctEndpOpenReq */

/*
*
*       Fun:   Endpoint closing request
*
*       Desc:  This function is used to send a request for the closing of the SCTP endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctEndpCloseReq
(
Pst *pst,                 /* post structure */
SpId spId,                /* service provider SAPID */
UConnId endpId,        /* service provider endpoint ID */
U8     endpIdType
)
#else
PUBLIC S16 HcLiSctEndpCloseReq(pst, spId, endpId, endpIdType)
Pst *pst;                 /* post structure */
SpId spId;                /* service provider SAPID */
UConnId endpId;       /* service provider endpoint ID */
U8     endpIdType;
#endif
{
   TRC3(HcLiSctEndpCloseReq)
   /* jump to specific primitive depending on configured selector */
   (*HcLiSctEndpCloseReqMt[pst->selector])(pst, spId, endpId, endpIdType);
   RETVALUE(ROK);
} /* end of HcLiSctEndpCloseReq */

/*
*
*       Fun:   Association establishment request
*
*       Desc:  This function is used to send a request for the closing of the SCTP endpoint
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef SCT3
#ifdef ANSI
PUBLIC S16 HcLiSctAssocReq
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,          /* service provider endpoint ID */
UConnId suAssocId,         /* service userassociation ID */
CmNetAddr *priDstNAddr,    /* primary destination network address */
SctPort dstPort,           /* destination port number */
SctStrmId outStrms,        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst, /* dest. network address list */
SctNetAddrLst *srcNAddrLst, /* src. network address list */
SctTos tos,                 /* type of service */
Buffer *vsInfo              /* vendor specific info */
)
#else
PUBLIC S16 HcLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, dstPort,
outStrms, dstNAddrLst, srcNAddrLst, tos, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;          /* service provider endpoint ID */
UConnId suAssocId;         /* service userassociation ID */
CmNetAddr *priDstNAddr;    /* primary destination network address */
SctPort dstPort;           /* destination port number */
SctStrmId outStrms;        /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst; /* dest. network address list */
SctNetAddrLst *srcNAddrLst; /* src. network address list */
SctTos tos;                 /* type of service */
Buffer *vsInfo;             /* vendor specific info */
#endif
#else /* SCT3 */
#ifdef ANSI
PUBLIC S16 HcLiSctAssocReq
(
Pst           *pst,                  /* post structure */
SpId          spId,                  /* service provider SAP ID */
UConnId       spEndpId,              /* service provider endpoint ID */
UConnId       suAssocId,             /* service userassociation ID */
CmNetAddr     *priDstNAddr,          /* primary destination network address */
SctPort       dstPort,               /* destination port number */
SctStrmId     outStrms,              /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst,          /* dest. network address list */
SctNetAddrLst *srcNAddrLst,         /* src. network address list */
Buffer        *vsInfo                /* VsInfo - not used any more */
)
#else
PUBLIC S16 HcLiSctAssocReq(pst, spId, spEndpId, suAssocId, priDstNAddr, dstPort,
                           outStrms, dstNAddrLst, srcNAddrLst, vsInfo)
Pst           *pst;                  /* post structure */
SpId          spId;                  /* service provider SAP ID */
UConnId       spEndpId;              /* service provider endpoint ID */
UConnId       suAssocId;             /* service userassociation ID */
CmNetAddr     *priDstNAddr;          /* primary destination network address */
SctPort       dstPort;               /* destination port number */
SctStrmId     outStrms;              /* no. of outgoing streams */
SctNetAddrLst *dstNAddrLst;          /* dest. network address list */
SctNetAddrLst *srcNAddrLst;          /* src. network address list */
Buffer        *vsInfo;               /* VsInfo - not used any more */
#endif
#endif /* SCT3 */
{
   TRC3(HcLiSctAssocReq)
   /* jump to specific primitive depending on configured selector */
#ifdef SCT3
    (*HcLiSctAssocReqMt[pst->selector])(pst, spId, spEndpId, suAssocId,
                                        priDstNAddr, dstPort, outStrms,
                                        dstNAddrLst, srcNAddrLst,
                                        tos, vsInfo);
#else /* SCT3 */
    (*HcLiSctAssocReqMt[pst->selector])(pst, spId, spEndpId, suAssocId,
                                        priDstNAddr, dstPort, outStrms,
                                        dstNAddrLst, srcNAddrLst,
                                        vsInfo);
#endif /* SCT3 */
   RETVALUE(ROK);
} /* end of HcLiSctAssocReq */

/*
*
*       Fun:   Association Establishment response
*
*       Desc:  This function is used by the service user to respond to an association
*       initialization indication by accepting the association.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef SCT3
#ifdef ANSI
PUBLIC S16 HcLiSctAssocRsp
(
Pst *pst,                  /* post structure */
SpId spId,                 /* service provider SAP ID */
UConnId spEndpId,         /* service provider association ID */
SctAssocIndParams *assocIndParams, /* association parameters */
SctTos tos,                /* type of service */
SctResult result,          /* result */
Buffer *vsInfo             /* vendor specific info */
)
#else
PUBLIC S16 HcLiSctAssocRsp(pst, spId, spEndpId, assocIndParams, tos, result, vsInfo)
Pst *pst;                  /* post structure */
SpId spId;                 /* service provider SAP ID */
UConnId spEndpId;         /* service provider association ID */
SctAssocIndParams *assocIndParams; /* association parameters */
SctTos tos;                /* type of service */
SctResult result;          /* result */
Buffer *vsInfo;            /* vendor specific info */
#endif
#else /* SCT3 */
#ifdef ANSI
PUBLIC S16 HcLiSctAssocRsp
(
Pst                 *pst,                   /* post structure */
SpId                spId,                   /* service provider SAP ID */
UConnId             spEndpId,               /* service provider endpoint ID */
SctAssocIndParams   *assocIndParams,        /*association indication parameters */
SctResult           result,                 /* results */
Buffer              *vsInfo                /* VsInfo - not used any more */
)
#else
PUBLIC S16 HcLiSctAssocRsp(pst, spId, spEndpId, assocIndParams, result, vsInfo)
Pst                 *pst;                   /* post structure */
SpId                spId;                   /* service provider SAP ID */
UConnId             spEndpId;               /* service provider endpoint ID */
SctAssocIndParams   *assocIndParams;        /*association indication parameters */
SctResult           result;                 /* results */
Buffer              *vsInfo;               /* VsInfo - not used any more */
#endif
#endif /* SCT3 */
{
   TRC3(HcLiSctAssocRsp)
   /* jump to specific primitive depending on configured selector */
#ifdef SCT3
   (*HcLiSctAssocRspMt[pst->selector])(pst, spId, spEndpId,
                                       assocIndParams, tos,
                                       result, vsInfo);
#else /* SCT3 */
   (*HcLiSctAssocRspMt[pst->selector])(pst, spId, spEndpId,
                                       assocIndParams, result,
                                       vsInfo);
#endif /* SCT3 */
   RETVALUE(ROK);
} /* end of HcLiSctAssocRsp */

/*
*
*       Fun:   Association Termination Request
*
*       Desc:  This function is used to request the termination of an association
*             which is either established or being established.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctTermReq
(
Pst      *pst,                  /* post structure */
SpId     spId,                  /* service provider SAP ID */
UConnId  assocId,               /* association ID */
U8       assocIdType,           /* association ID type */
Bool     abrtFlg                /* abort flag */
)
#else
PUBLIC S16 HcLiSctTermReq(pst, spId, assocId, assocIdType, abrtFlg)
Pst      *pst;                  /* post structure */
SpId     spId;                  /* service provider SAP ID */
UConnId  assocId;               /* association ID */
U8       assocIdType;           /* association ID type */
Bool     abrtFlg;               /* abort flag */
#endif
{
   TRC3(HcLiSctTermReq)

   /* jump to specific primitive depending on configured selector */
   (*HcLiSctTermReqMt[pst->selector])(pst, spId, assocId, assocIdType, abrtFlg);
   RETVALUE(ROK);
} /* end of HcLiSctTermReq */

/*
*
*       Fun:   Set primary Destination Network Address Request
*
*       Desc:  This function is used by the service user to set a destination network
*             address as primary if multiple destination addresses are available.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctSetPriReq
(
Pst       *pst,                 /* post structure */
SpId      spId,                 /* service provider ID */
UConnId   spAssocId,            /* service provider association ID */
CmNetAddr *dstNAddr             /* dest. network address */
)
#else
PUBLIC S16 HcLiSctSetPriReq(pst, spId, spAssocId, dstNAddr)
Pst       *pst;                 /* post structure */
SpId      spId;                 /* service provider ID */
UConnId   spAssocId;            /* service provider association ID */
CmNetAddr *dstNAddr;            /* dest. network address */
#endif
{
   TRC3(HcLiSctSetPriReq)
   /* jump to specific primitive depending on configured selector */
   (*HcLiSctSetPriReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr);
   RETVALUE(ROK);
} /* end of HcLiSctSetPriReq */

/*
*
*       Fun:   Enable/Disable Heartbeat request
*
*       Desc:  This function is used by the service user to enable/disable
*              heartbeat either on an association or on a destination transport
*              address in an association.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctHBeatReq
(
Pst        *pst,             /* post structure */
SpId       spId,             /* service provider SAP ID */
UConnId    spAssocId,        /* service provider association ID */
CmNetAddr  *dstNAddr,        /* dest. network address */
U16        intervalTime,     /* heartbeat periodicity value */
SctStatus  status            /* status */
)
#else
PUBLIC S16 HcLiSctHBeatReq(pst, spId, spAssocId, dstNAddr, intervalTime, status)
Pst        *pst;            /* post structure */
SpId       spId;            /* service provider SAP ID */
UConnId    spAssocId;       /* service provider association ID */
CmNetAddr  *dstNAddr;       /* dest. network address */
U16        intervalTime;    /* heartbeat periodicity value */
SctStatus  status;          /* status */
#endif
{
   TRC3(HcLiSctHBeatReq)
   /* jump to specific primitive depending on configured selector */
   (*HcLiSctHBeatReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr, intervalTime, status);
   RETVALUE(ROK);
} /* end of HcLiSctHBeatReq */


/*
*
*       Fun:   Data request
*
*       Desc:  This function is used to request the service provider to send a
*              user datagram to the destination.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctDatReq
(
Pst        *pst,                /* post structure */
SpId       spId,                /* service provider SAP ID */
UConnId    spAssocId,           /* service provider association ID */
CmNetAddr  *dstNAddr,           /* dest. network address */
SctStrmId  strmId,              /* stream ID */
Bool       unorderFlg,          /* unordered delivery flag */
Bool       nobundleFlg,         /* nobundleFlg */
U16        lifetime,            /* datagram lifetime */
U32        protId,              /*protocol ID */
Buffer     *mBuf                /* message buffer */
)
#else
PUBLIC S16 HcLiSctDatReq(pst, spId, spAssocId, dstNAddr, strmId, unorderFlg,
                         nobundleFlg, lifetime, protId, mBuf)
Pst        *pst;                 /* post structure */
SpId       spId;                 /* service provider SAP ID */
UConnId    spAssocId;            /* service provider association ID */
CmNetAddr  *dstNAddr;            /* dest. network address */
SctStrmId  strmId;               /* stream ID */
Bool       unorderFlg;           /* unordered delivery flag */
Bool       nobundleFlg;          /* nobundleFlg */
U16        lifetime;             /* datagram lifetime */
U32        protId;               /*protocol ID */
Buffer     *mBuf;                /* message buffer */
#endif
{
   TRC3(HcLiSctDatReq)
   /* jump to specific primitive depending on configured selector */
   (*HcLiSctDatReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr, strmId, unorderFlg,
    nobundleFlg, lifetime, protId, mBuf);
   RETVALUE(ROK);
} /* end of HcLiSctDatReq */


/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to retrieve unsent/unacknowledged/undelivered
*             datagrams from the service provider and to get statistical information
*             from the service provider.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  it_ptli.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctStaReq
(
Pst         *pst,                 /* post structure */
SpId        spId,                 /* service provider SAP ID */
UConnId     spAssocId,            /* service provider association ID */
CmNetAddr   *dstNAddr,            /* dest. network address */
U8          staType               /* status type */
)
#else
PUBLIC S16 HcLiSctStaReq(pst, spId, spAssocId, dstNAddr, staType)
Pst         *pst;                 /* post structure */
SpId        spId;                 /* service provider SAP ID */
UConnId     spAssocId;            /* service provider association ID */
CmNetAddr   *dstNAddr;            /* dest. network address */
U8          staType;              /* status type */
#endif
{
   TRC3(HcLiSctStaReq)
   /* jump to specific primitive depending on configured selector */
   (*HcLiSctStaReqMt[pst->selector])(pst, spId, spAssocId, dstNAddr, staType);

   RETVALUE(ROK);
} /* end of HcLiSctStaReq */

#endif







/*
 *      Portable Functions 
 */
#ifdef PTHCLIHIT
/*
*
*       Fun:   Portable - Bind Request
*
*       Desc:  This function resolves the Bind Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitBndReq
(
Pst       *pst,          /* Post structure */
SuId      suId,          /* service user sap id */
SpId      spId           /* service provider sap id */
)
#else
PRIVATE S16 PtLiHitBndReq(pst, suId, spId)
Pst      *pst;           /* Post structure */
SuId      suId;          /* service user sap id */
SpId      spId;          /* service provider sap id */
#endif
{
   TRC3(PtLiHitBndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC026, (ErrVal)0, "PtLiHitBndReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitBndReq */


/*
*
*       Fun:   Portable - Bind Request
*
*       Desc:  This function resolves the Bind Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitUbndReq
(
Pst *pst,              /* Post structure */
SpId spId,             /* Service provider Id */
Reason reason          /* cause for unbind operation */
)
#else
PRIVATE S16 PtLiHitUbndReq(pst, spId, reason)
Pst *pst;              /* Post structure */
SpId spId;             /* Service provider Id */
Reason reason;         /* cause for unbind operation */
#endif
{
   TRC3(PtLiHitUbndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC027, (ErrVal)0, "PtLiHitUbndReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitUbndReq */


/*
*
*       Fun:   Portable - Server Open Request 
*
*       Desc:  This function resolves the Server Open Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitServOpenReq
(
Pst *pst,                 /* post stucture */
SpId spId,                /* service provider Id */
UConnId servConId,        /* service user's server connection Id */
CmTptAddr *servTAddr,     /* transport address of the server */ 
CmTptParam *tPar,         /* transport options */
CmIcmpFilter *icmpFilter,  /* Filter parameters */
U8 srvcType               /* type of service */
)
#else
PRIVATE S16 PtLiHitServOpenReq(pst, spId, servConId, servTAddr, tPar,
                              icmpFilter, srvcType) 
Pst *pst;                  /* post stucture */
SpId spId;                 /* service provider Id */
UConnId servConId;         /* service user's server connection Id */
CmTptAddr  *servTAddr;     /* transport address of the server */ 
CmTptParam *tPar;          /* transport options */
CmIcmpFilter *icmpFilter;  /* Filter parameters */
U8  srvcType;              /* service type */
#endif /* ANSI */
{
   TRC3(PtLiHitServOpenReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC028, (ErrVal)0, "PtLiHitServOpenReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitServOpenReq */


/*
*
*       Fun:   Portable - Connect Request
*
*       Desc:  This function resolves the Connect Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitConReq
(
Pst *pst,                  /* post structure */ 
SpId spId,                 /* service provider id */ 
UConnId suConId,           /* service user's connection id */ 
CmTptAddr *remAddr,        /* address of the server */ 
CmTptAddr *localAddr,      /* local address */
CmTptParam *tPar,          /* transport parameters */
U8 srvcType                /* type of service */
)
#else
PRIVATE S16 PtLiHitConReq(pst, spId, suConId, remAddr, localAddr, tPar, srvcType) 
Pst *pst;                  /* post structure */ 
SpId spId;                 /* service provider id */ 
UConnId suConId;           /* service user's connection id */ 
CmTptAddr *remAddr;        /* address of the server */ 
CmTptAddr *localAddr;      /* local address */
CmTptParam *tPar;          /* transport parameters */
U8 srvcType;               /* type of service */
#endif
{
   TRC3(PtLiHitConReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC029, (ErrVal)0, "PtLiHitConReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitConReq */


/*
*
*       Fun:   Portable - Connect Response
*
*       Desc:  This function resolves the Connect Response primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitConRsp
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId suConId,          /* service user's connection id */ 
UConnId spConId           /* service provider's connection id */
)
#else
PRIVATE S16 PtLiHitConRsp(pst, spId, suConId, spConId)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId suConId;          /* service user's connection id */ 
UConnId spConId;          /* service provider's connection id */
#endif
{
   TRC3(PtLiHitConRsp)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC030, (ErrVal)0, "PtLiHitConRsp() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitConRsp */


/*
*
*       Fun:   Portable - Data Request
*
*       Desc:  This function resolves the Data Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitDatReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId spConId,          /* service provider's connection id */
Buffer *mBuf             /* message buffer */
)
#else
PRIVATE S16 PtLiHitDatReq(pst, spId, spConId, mBuf)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId spConId;          /* service provider's connection id */
Buffer *mBuf;            /* message buffer */
#endif
{
   TRC3(PtLiHitDatReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC031, (ErrVal)0, "PtLiHitDatReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitDatReq */


/*
*
*       Fun:   Portable - Unit Data Request
*
*       Desc:  This function resolves the Unit Data Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitUDatReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
UConnId spConId,         /* service provider connection id */
CmTptAddr *remAddr,      /* remote address */
CmTptAddr *srcAddr,      /* Source Address */
CmIpHdrParm *hdrParm,    /* Header Parameters */
CmTptParam *tPar,        /* transport parameters */
Buffer *mBuf             /* message buffer to be sent */
)
#else
PRIVATE S16 PtLiHitUDatReq(pst, spId, spConId, remAddr, srcAddr, 
                          hdrParm, tPar, mBuf)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
UConnId  spConId;        /* service provider connection id */
CmTptAddr *remAddr;      /* remote address */
CmTptAddr *srcAddr;      /* Source Address */
CmIpHdrParm *hdrParm;    /* Header Parameters */
CmTptParam *tPar;        /* transport parameters */
Buffer *mBuf;            /* message buffer to be sent */
#endif /* ANSI */
{
   TRC3(PtLiHitUDatReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC032, (ErrVal)0, "PtLiHitUDatReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitUDatReq */


/*
*
*       Fun:   Portable - Disconnect Request
*
*       Desc:  This function resolves the Disconnect Request primitive
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  l5_ptli.c
*
*/
#ifdef ANSI
PRIVATE S16 PtLiHitDiscReq
(
Pst *pst,                /* post structure */ 
SpId spId,               /* service provider id */ 
U8 choice,               /* choice of user or provider connection id */
UConnId conId,            /* connection id */
Action action,           /* action to be performed */
CmTptParam  *tPar      /* transport parameters */ 
)
#else
PRIVATE S16 PtLiHitDiscReq(pst, spId, choice, conId, action, tPar)
Pst *pst;                /* post structure */ 
SpId spId;               /* service provider id */ 
U8 choice;               /* choice of user or provider connection id */
UConnId conId;            /* connection id */
Action action;           /* action to be performed */
CmTptParam  *tPar;     /* transport parameters */ 
#endif
{
   TRC3(PtLiHitDiscReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   HCLOGERROR(ERRCLS_DEBUG, EHC033, (ErrVal)0, "PtLiHitDiscReq() called");
#endif /* ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* end of PtLiHitDiscReq */
#endif /* PTHCLIHIT */
 

/********************************************************************30**
 
         End of file:     l5_ptli.c@@/main/6 - Mon Mar  3 20:09:36 2008

*********************************************************************31*/
 

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 

/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/
/********************************************************************70**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat     init                  description
----------- --------- ---- ----------------------------------------------
1.1         ---       asa  1. initial release.
1.1+        hi002.11  asa  1. added spConId to HitUDatReq.
/main/2     ---       cvp  1. changed the copyright header.
            /main/4   sb   1. changes in primitives HiUiHitServOpenReq & 
                              HiUiHitUDatReq for Raw socket interface.
/main/4     ---       cvp  1. Changes for HI_REL_1_4.
                           2. changed the copyright header.
/main/4+    hi009.104 mmh  1. included header files sht.h and sht.x for RUG
/main/5      ---       kp   1. Updated for release 1.5.
/main/6      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
