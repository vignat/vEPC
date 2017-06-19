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
  
     Name:     S1AP - Management Interface
  
     Type:     C source file
  
     Desc:     C source code for the management interface primitives of
               S1AP
 
     File:     sq_ptmi.c
  
     Sid:      sq_ptmi.c@@/main/nodal_main/3 - Fri Nov 22 17:58:25 2013
  
     Prg:      nm
  
*********************************************************************21*/
  
/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.
 
The following functions are provided in this file:

     SzMiLszStaInd      Status Indication
     SzMiLszStaCfm      Status Confirm
     SzMiLszStsCfm      Statistics Confirm
     SzMiLszTrcInd      Trace Indication
     SzMiLszCfgCfm      Configure confirm 
     SzMiLszCntrlCfm    Control confirm 
 
It is assumed that the following functions are provided in the
S1AP protocol file:
  
     SzMiLszCfgReq      Configure Request
     SzMiLszStaReq      Status Request
     SzMiLszStsReq      Statistics Request
     SzMiLszCntrlReq    Control Request
 
*/
  
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000361     S1AP
*
*/
 
/* header include files (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* SZT interface defines           */
#include "szt_asn.h"
#ifdef SZ_FTHA  
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent                    */
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP Layer management           */
#include "sz.h"            /* S1AP layer defines              */
#include "sz_err.h"        /* S1AP - error */
#include "cm_pasn.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "sct.x"           /* SCT interface defines           */
#ifdef SZ_FTHA  
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent                    */
#endif /* SZ_FTHA */
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "szt.x"           /* SZT interface structures        */
#include "lsz.x"           /* S1AP Layer management           */
#include "cm_pasn.x"
#include "sz.x"            /* S1AP layer structures           */

/* local defines */
#define MAXSZMI     2  

#ifndef LCSZMILSZ
#define PTSZMILSZ   1      /* Portable LGT Interface */
#else /* LCSZMILSZ */
#ifndef SM
#define PTSZMILSZ   1      /* Portable LGT Interface */
#endif /* SM */
#endif /* LCSZMILSZ */

/* local typedefs */
  
/* local externs */
  
/* forward references */
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
 
#ifdef PTSZMILSZ
PRIVATE S16 PtMiLszStaInd ARGS((Pst *pst, SzMngmt *sta));
PRIVATE S16 PtMiLszStaCfm ARGS((Pst *pst, SzMngmt *sta));
PRIVATE S16 PtMiLszStsCfm ARGS((Pst *pst, SzMngmt *sts));
PRIVATE S16 PtMiLszTrcInd ARGS((Pst *pst, SzMngmt *trc, Buffer *mBuf));
PRIVATE S16 PtMiLszCfgCfm ARGS((Pst *pst, SzMngmt *cfm));
PRIVATE S16 PtMiLszCntrlCfm ARGS((Pst *pst, SzMngmt *cfm));
#endif /* PTSZMILSZ */
#ifdef SZ_FTHA
PRIVATE S16 PtMiShtCntrlCfm ARGS((Pst *pst, ShtCntrlCfmEvnt *cfmInfo));
#endif /* SZ_FTHA */ 

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* functions in other modules */
  
/* public variable declarations */
  
/* private variable declarations */
 
/*

   The following matrices define the mapping between the primitives 
   called by the layer management interface of S1AP and the 
   corresnonding primitives of the S1AP service user.
 
   The parameter MAXSZMI defines the maximum number of service users on
   top of S1AP. There is an array of functions per primitive invoked by 
   S1AP. Every array is MAXSZMI long (i.e. there are as many functions 
   as the number of service users).
 
   The disnatching is performed by the configurable variable: selector.
   The selector is configured during genESZl configuration.
 
   The selectors are:
 
   0 - loosely coupled - forawrd compatibility (#define LCSZMILSZ)
   1 - Tightly Coupled  (#define SM)
 
*/
 
/* Status Indication primitive */
PRIVATE LszStaInd SzMiLszStaIndMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszStaInd,          /* 0 - loosely coupled  */
#else /* LCSZMILSZ */
   PtMiLszStaInd,          /* 0 - tightly coupled, Portable */
#endif /* LCSZMILSZ */
#ifdef SM
   SmMiLszStaInd,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLszStaInd,          /* 1 - tightly coupled, Portable */
#endif /* SM */
};

/* Status confirm primitive */
 
PRIVATE LszStaCfm SzMiLszStaCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszStaCfm,          /* 0 - loosely coupled  */
#else /* LCSZMILSZ */
   PtMiLszStaCfm,          /* 0 - tightly coupled, Portable */
#endif /* LCSZMILSZ */
#ifdef SM
   SmMiLszStaCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLszStaCfm,          /* 1 - tightly coupled, Portable */
#endif /* SM */
};

/* Statistic Confirm primitive */
 
PRIVATE LszStsCfm SzMiLszStsCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszStsCfm,          /* 0 - loosely coupled  */
#else /* LCSZMILSZ */
   PtMiLszStsCfm,          /* 0 - tightly coupled, Portable */
#endif /* LCSZMILSZ */
#ifdef SM
   SmMiLszStsCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLszStsCfm,          /* 1 - tightly coupled, Portable */
#endif /* SM */
};

/* Trace Indication primitive */
 
PRIVATE LszTrcInd SzMiLszTrcIndMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszTrcInd,          /* 0 - loosely coupled  */
#else /* LCSZMILSZ */
   PtMiLszTrcInd,          /* 0 - tightly coupled, Portable */
#endif /* LCSZMILSZ */
#ifdef SM
   SmMiLszTrcInd,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLszTrcInd,          /* 1 - tightly coupled, Portable */
#endif /* SM */
};

/* configuration confirm primitive */
PRIVATE LszCfgCfm SzMiLszCfgCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszCfgCfm,          /* 0 - loosely coupled  */
#else /* LCSZMILSZ */
   PtMiLszCfgCfm,          /* 0 - loosely coupled, Portable */
#endif /* LCSZMILSZ */
#ifdef SM
   SmMiLszCfgCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLszCfgCfm,          /* 1 - tightly coupled, Portable */
#endif /* SM */
};

/* control confirm primitive */
PRIVATE LszCntrlCfm SzMiLszCntrlCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkLszCntrlCfm,          /* 0 - loosely coupled  */
#else /* LCSZMILSZ */
   PtMiLszCntrlCfm,          /* 0 - loosely coupled, Portable */
#endif /* LCSZMILSZ */
#ifdef SM
   SmMiLszCntrlCfm,          /* 1 - tightly coupled, layer management */
#else /* SM */
   PtMiLszCntrlCfm,          /* 1 - tightly coupled, Portable */
#endif /* SM */
};

#ifdef SZ_FTHA
PUBLIC ShtCntrlCfm SzMiShtCntrlCfmMt[MAXSZMI] =
{
#ifdef LCSZMILSZ
   cmPkMiShtCntrlCfm,     /* 0 - loosely coupled (default mechanism) */
#else /* LCSZMILSZ */
   PtMiShtCntrlCfm,       /* 0 - loosely coupled, portable */
#endif /* LCSZMILSZ */
#ifdef SH
   ShMiShtCntrlCfm,       /* 1 - tightly coupled, stack manager */
#else /* SH */
   PtMiShtCntrlCfm,       /* 1 - tightly coupled, portable */
#endif /* SH */
};
#endif /* SZ_FTHA */

/*
*     support functions
*/
                 
/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:  SzMiLszStaInd
*
*       Desc:  This function is used to indicate the status of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszStaInd
(
Pst *pst,                 /* post structure */
SzMngmt *sta              /* unsolicited status */
)
#else /* ANSI */
PUBLIC S16 SzMiLszStaInd(pst, sta)
Pst *pst;                 /* post structure */   
SzMngmt *sta;             /* unsolicited status */
#endif /* ANSI */
{
   TRC3(SzMiLszStaInd);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszStaIndMt[pst->selector])(pst, sta); 

   RETVALUE(ROK);
} /* end of SzMiLszStaInd */

 
/*
*
*       Fun:  SzMiLszStaCfm
*
*       Desc:  This function is used to return the status of S1AP
*              to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszStaCfm
(
Pst *pst,                 /* post structure */     
SzMngmt *sta              /* solicited status */
)
#else /* ANSI */
PUBLIC S16 SzMiLszStaCfm(pst, sta)
Pst *pst;                 /* post structure */     
SzMngmt *sta;             /* solicited status */
#endif /* ANSI */
{
   TRC3(SzMiLszStaCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszStaCfmMt[pst->selector])(pst, sta); 
   
   RETVALUE(ROK);
} /* end of SzMiLszStaCfm */
 
/*
*
*       Fun:   SzMiLszStsCfm
*
*       Desc:  This function is used to return the statistics of S1AP  
*              to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszStsCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *sts              /* statistics */
)
#else /* ANSI */
PUBLIC S16 SzMiLszStsCfm(pst, sts)
Pst *pst;                 /* post structure */    
SzMngmt *sts;             /* statistics */
#endif /* ANSI */
{
   TRC3(SzMiLszStsCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszStsCfmMt[pst->selector])(pst, sts); 

   RETVALUE(ROK);
} /* end of SzMiLszStsCfm */
 
/*
*
*       Fun:   SzMiLszTrcInd
*
*       Desc:  This function is used to indicate the trace of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszTrcInd
(
Pst *pst,                 /* post structure */
SzMngmt *trc,             /* trace */
Buffer *mBuf              /* buffer */
)
#else /* ANSI */
PUBLIC S16 SzMiLszTrcInd(pst, trc, mBuf)
Pst *pst;                 /* post structure */   
SzMngmt *trc;             /* trace */
Buffer *mBuf;             /* buffer */
#endif /* ANSI */
{
   TRC3(SzMiLszTrcInd);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszTrcIndMt[pst->selector])(pst, trc, mBuf); 

   RETVALUE(ROK);
} /* end of SzMiLszTrcInd */

/*
*
*       Fun:   SzMiLszCfgCfm
*
*       Desc:  This function is used to confirm the receipt of configuration
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszCfgCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else /* ANSI */
PUBLIC S16 SzMiLszCfgCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif /* ANSI */
{
   TRC3(SzMiLszCfgCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszCfgCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of SzMiLszCfgCfm */

/*
*
*       Fun:   SzMiLszCntrlCfm
*
*       Desc:  This function is used to confirm the receipt of control
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SzMiLszCntrlCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else /* ANSI */
PUBLIC S16 SzMiLszCntrlCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif /* ANSI */
{
   TRC3(SzMiLszCntrlCfm);

   /* jump to specific primitive depending on configured selector */
   (*SzMiLszCntrlCfmMt[pst->selector])(pst, cfm); 

   RETVALUE(ROK);
} /* end of SzMiLszCntrlCfm */

#ifdef SZ_FTHA

/**********************************************************
*
*       Fun:   SzMiShtCntrlCfm
*
*       Desc:  This function sends System Agent Control Confirm
*
*       Ret:   ROK - ok
*
*       Notes: none
*
*       File:  sz_ptmi.c
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 SzMiShtCntrlCfm
(
Pst             *pst,            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo         /* system agent control confirm event */
)
#else /* ANSI */
PUBLIC S16 SzMiShtCntrlCfm (pst, cfmInfo)
Pst             *pst;            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo;        /* system agent control confirm event */
#endif /* ANSI */
{
   TRC3(SzMiShtCntrlCfm);

   (*SzMiShtCntrlCfmMt[pst->selector])(pst, cfmInfo);

   RETVALUE(ROK);
}/* SzMiShtCntrlCfm */
#endif /* SZ_FTHA */

/*
*     layer management interface Portable functions
*/
#ifdef PTSZMILSZ
/*
*
*       Fun:   PtMiLszStaInd
*
*       Desc:  This function is used to indicate the status of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
 
#ifdef ANSI
PRIVATE S16 PtMiLszStaInd
(
Pst *pst,                 /* post structure */
SzMngmt *sta              /* unsolicited status */
)
#else /* ANSI */
PRIVATE S16 PtMiLszStaInd(pst, sta)
Pst *pst;                 /* post structure */
SzMngmt *sta;             /* unsolicited status */
#endif /* ANSI */
{
   TRC3(PtMiLszStaInd);
#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ125, (ErrVal) ERRZERO, "PtMiLszStaInd");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of PtMiLszStaInd */
 
/*
*
*       Fun:   PtMiLszStaCfm
*
*       Desc:  This function is used to return the status of S1AP
*              to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLszStaCfm
(
Pst *pst,                 /* post structure */     
SzMngmt *sta              /* solicited status */
)
#else /* ANSI */
PRIVATE S16 PtMiLszStaCfm(pst, sta)
Pst *pst;                 /* post structure */     
SzMngmt *sta;             /* solicited status */
#endif /* ANSI */
{
   TRC3(PtMiLszStaCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ126, (ErrVal) ERRZERO, "PtMiLszStaCfm");
#endif /* ERRCLASS & ERRCLS_DEBUG */


   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of PtMiLszStaCfm */
 
/*
*
*       Fun:   PtMiLszStsCfm
*
*       Desc:  This function is used to return the statistics of Network 
*              Services to layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLszStsCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *sts              /* statistics */
)
#else /* ANSI */
PRIVATE S16 PtMiLszStsCfm(pst, sts)
Pst *pst;                 /* post structure */    
SzMngmt *sts;             /* statistics */
#endif /* ANSI */
{
   TRC3(PtMiLszStsCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ127, (ErrVal) ERRZERO, "PtMiLszStsCfm");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of PtMiLszStsCfm */
 
/*
*
*       Fun:   PtMiLszTrcInd
*
*       Desc:  This function is used to indicate the trace of S1AP
*              to the layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
 
#ifdef ANSI
PRIVATE S16 PtMiLszTrcInd
(
Pst *pst,                 /* post structure */
SzMngmt *trc,             /* trace */
Buffer *mBuf              /* buffer */
)
#else /* ANSI */
PRIVATE S16 PtMiLszTrcInd(pst, trc, mBuf)
Pst *pst;                 /* post structure */
SzMngmt *trc;             /* trace */
Buffer *mBuf;             /* buffer */
#endif /* ANSI */
{
   TRC3(PtMiLszTrcInd);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ128, (ErrVal) ERRZERO, "PtMiLszTrcInd");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of PtMiLszTrcInd */
 

/*
*
*       Fun:   PtMiLszCfgCfm
*
*       Desc:  This function is used to confirm the receipt of configuration
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 PtMiLszCfgCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else /* ANSI */
PUBLIC S16 PtMiLszCfgCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif /* ANSI */
{
   TRC3(PtMiLszCfgCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ129, (ErrVal)ERRZERO, "PtMiLszCfgCfm");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of PtMiLszCfgCfm */


/*
*
*       Fun:   PtMiLszCntrlCfm
*
*       Desc:  This function is used to confirm the receipt of control
*              request from layer management.
*
*       Ret:   ROK - ok
*
*       Notes: None
*
*       File:  sq_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 PtMiLszCntrlCfm
(
Pst *pst,                 /* post structure */    
SzMngmt *cfm              /* confirm */
)
#else /* ANSI */
PUBLIC S16 PtMiLszCntrlCfm(pst, cfm)
Pst *pst;                 /* post structure */    
SzMngmt *cfm;             /* confirm */
#endif /* ANSI */
{
   TRC3(PtMiLszCntrlCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ130, (ErrVal)ERRZERO, "PtMiLszCntrlCfm");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of PtMiLszCntrlCfm */
#endif /* PTSZMILSZ */

#ifdef SZ_FTHA

/**********************************************************
*
*       Fun:   PtMiShtCntrlCfm
*
*       Desc:  Portable version of ShtCntrlCfm primitive
*
*       Ret:   RFAILED
*
*       Notes: none
*
*       File:  sz_ptmi.c
*
**********************************************************/
#ifdef ANSI
PRIVATE S16 PtMiShtCntrlCfm
(
Pst             *pst,            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo         /* system agent control confirm event */
)
#else /* ANSI */
PRIVATE S16 PtMiShtCntrlCfm (pst, cfmInfo)
Pst             *pst;            /* post structure */      
ShtCntrlCfmEvnt *cfmInfo;        /* system agent control confirm event */
#endif /* ANSI */
{
   TRC3(PtMiShtCntrlCfm);

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ131, (ErrVal)ERRZERO,
         "PtMiShtCntrlCfm(): Invalid Selector");
#endif /* ERRCLASS & ERRCLS_DEBUG */
   
   UNUSED(pst);
   UNUSED(cfmInfo);

   RETVALUE(RFAILED);
} /* PtMiShtCntrlCfm */
#endif /* SZ_FTHA */

/**********************************************************************

         End of file:     sq_ptmi.c@@/main/nodal_main/3 - Fri Nov 22 17:58:25 2013

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/4      ---      va    1. Updated for S1AP release 4.1
*********************************************************************91*/
