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

     Name:    eGTP 

     Type:    C source file

     Desc:    C code for controlling acceptance tests execution.

     File:    xu_ptli.c 

     Sid:      smegbdy1.c@@/main/3 - Fri Feb  4 18:18:28 2011

     Prg:     

*********************************************************************21*/
/* header include files (.h) */
#include "envopt.h"         /* environment options          */
#include "envdep.h"         /* environment dependent        */
#include "envind.h"         /* environment independent      */
#include "gen.h"            /* general layer                */
#include "ssi.h"            /* system services              */
#include "cm5.h"            /* common timer module          */
#include "cm_hash.h"        /* common hash list             */
#include "cm_tkns.h"        /* common tokens                */
#include "cm_inet.h"        /* Inet header file             */
#include "cm_tpt.h"         /* Transport  header file       */
#include "cm_mblk.h"        /* common memory allocation     */
#include "cm_llist.h"      /* common link list */

/* external headers */
#ifdef EG_ACC_INT 
#ifdef EG_TLS
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif /* EG_TLS */

#include "lhi.h"            /* TUCL */
#include "hit.h"
#include "hi.h"
#endif

#ifdef EG
#include "leg.h"
#include "egt.h"
#include "eg_edm.h"
#include "eg.h"            /* TUCL */
#endif /* EG */


#include "cm_xta.h"
#include "cm_xta_app.h"
#include "egac_acc.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   --*/
#include "ssi.x"           /* System services interface       --*/
#include "cm5.x"           /* Common timer module             --*/
#include "cm_lib.x"        /* Common linrary function         --*/
#include "cm_llist.x"      /* Common link list library        --*/
#include "cm_hash.x"       /* Common hash list library        --*/
#include "cm_tkns.x"       /* Common tokens                   --*/
#include "cm_tpt.x"        /* Common transport library        --*/
#include "cm_mblk.x"       /* Common memory allocation        --*/
#include "cm_inet.x"       /* Common socket library           --*/

#ifdef EG_ACC_INT
#include "lhi.x"            /* TUCL */
#include "hit.x"
#endif
/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif
#ifdef EG
#include "leg.x"
#include "egt.x"
#include "eg_edm.x"
#include "eg.x"            /* TUCL */
#endif /* EG */


#include "cm_xta.x"
#include "egac_acc.x"


/* public routines */
/*
*
*       Fun:   dummy Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegbdy1.c
*
*/

#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 smEgActvInit
(
ProcId      proc,           /* Proc Id  */
Ent         ent,         /* entity */
Inst        inst,        /* instance */
Region      region,      /* region */
Reason      reason,       /* reason */
void      **xxCb            /* protocol control block */
)
#else
PUBLIC S16 smEgActvInit(proc,ent, inst, region, reason, xxCb)
ProcId      proc;           /* Proc Id */
Ent         ent;         /* entity */
Inst        inst;        /* instance */
Region      region;      /* region */
Reason      reason;      /* reason */
void        **xxCb;            /* protocol control block */
#endif
#else /* SS_MULTIPLE_PROC  */
#ifdef ANSI
PUBLIC S16 smEgActvInit
(
Ent    ent,                   /* entity */
Inst   inst,                  /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smEgActvInit(ent, inst, region, reason)
Ent    ent;                   /* entity */
Inst   inst;                  /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(smEgActvInit)
#ifdef SS_MULTIPLE_PROC
   UNUSED(proc);
#endif
   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);

   RETVALUE(ROK);
} /* smEgActvInit */

/*
 *
 *       Fun:    SmMiLegCfgCfm
 *
 *       Desc:   Configuration Confirm
 *
 *       Ret:    ROK     - succeeded
 *
 *       Notes:  None
 *
 *       File:   smegbdy1.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegCfgCfm
(
Pst     *pst,
EgMngmt *cfg
)
#else
PUBLIC S16 SmMiLegCfgCfm(pst, cfg)
Pst     *pst;
EgMngmt *cfg;
#endif /* ANSI */
{
   /* local variables */
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(SmMiLegCfgCfm)

   /*-- Update Event --*/
   pst->event = EVTLEGCFGCFM;
   
   egAcSmMiLegCfgCfm(pst, cfg);

   RETVALUE(ROK);

} /* SmMiLegCfgCfm */

/*
 *
 *       Fun:    SmMiLegCntrlCfm
 *
 *       Desc:   Control Confirm
 *
 *       Ret:    ROK     - succeeded
 *
 *       Notes:  None
 *
 *       File:   smegbdy1.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegCntrlCfm
(
Pst     *pst,
EgMngmt *cntrl
)
#else
PUBLIC S16 SmMiLegCntrlCfm(pst, cntrl)
Pst     *pst;
EgMngmt *cntrl;
#endif /* ANSI */
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(SmMiLegCntrlCfm)

   /*-- Update Event --*/
   pst->event = EVTLEGCNTRLCFM;
   egAcSmMiLegCntrlCfm(pst, cntrl);

   RETVALUE(ROK);
} /* SmMiLegCntrlCfm */

/*
 *
 *       Fun:    SmMiLegStaInd
 *
 *       Desc:   Unsolicited Status Indication
 *
 *       Ret:    ROK     - succeeded
 *
 *       Notes:  None
 *
 *       File:   smegbdy1.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStaInd
(
Pst     *pst,
EgMngmt *usta
)
#else
PUBLIC S16 SmMiLegStaInd(pst, usta)
Pst     *pst;
EgMngmt *usta;
#endif /* ANSI */
{

   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(SmMiLegStaInd)

   /*-- Update Event --*/
   pst->event = EVTLEGSTAIND;

    egAcSmMiLegStaInd(pst, usta);  

   RETVALUE(ROK);
} /* SmMiLegStaInd */

/*
 *
 *       Fun:    SmMiLegTrcInd
 *
 *       Desc:   Trace Indication
 *
 *       Ret:    ROK     - succeeded
 *
 *       Notes:  None
 *
 *       File:   smegbdy1.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegTrcInd
(
Pst     *pst,
EgMngmt *trc,
Buffer  *mBuf
)
#else
PUBLIC S16 SmMiLegTrcInd(pst, trc, mBuf)
Pst     *pst;
EgMngmt *trc;
Buffer  *mBuf;
#endif /* ANSI */
{

   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(SmMiLegTrcInd)

    /*-- Update Event --*/
    pst->event = EVTLEGTRCIND;
    egAcSmMiLegTrcInd(pst, trc, mBuf);  

    RETVALUE(ROK);

} /* SmMiLegTrcInd */

/*
 *
 *       Fun:    SmMiLegStaCfm
 *
 *       Desc:   Status Confirm
 *
 *       Ret:    ROK     - succeeded
 *
 *       Notes:  None
 *
 *       File:   smegbdy1.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStaCfm
(
Pst     *pst,
EgMngmt *sta
)
#else
PUBLIC S16 SmMiLegStaCfm(pst, sta)
Pst     *pst;
EgMngmt *sta;
#endif /* ANSI */
{

   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(SmMiLegStaCfm)

      /*-- Update Event --*/
   pst->event = EVTLEGSTACFM;
   egAcSmMiLegStaCfm(pst, sta);

   RETVALUE(ROK);
} /* SmMiLegStaCfm */

/*
 *
 *       Fun:    SmMiLegStsCfm
 *
 *       Desc:   Dummy Statistics Confirm
 *
 *       Ret:    ROK     - succeeded
 *
 *       Notes:  None
 *
 *       File:   smegbdy1.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLegStsCfm
(
Pst     *pst,
EgMngmt *sts
)
#else
PUBLIC S16 SmMiLegStsCfm(pst, sts)
Pst     *pst;
EgMngmt *sts;
#endif /* ANSI */
{
   
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(SmMiLegStsCfm)

   /*-- Update Event --*/
   pst->event = EVTLEGSTSCFM;

   egAcSmMiLegStsCfm(pst, sts);

   RETVALUE(ROK);
} /* SmMiLegStsCfm */

/********************************************************************30**

         End of file:     smegbdy1.c@@/main/3 - Fri Feb  4 18:18:28 2011

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
/main/1      -----    sv    1. Initial release of eGTP.
/main/2      ---      sv                1. Initial for eGTP 1.2 Release
/main/2      eg003.102    rss      1.  Removed cm_xtree from the include list.
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3      eg003.201    psingh      1. Merged code from eg006.102 to Fix
                                         TRACE5 macro related issue
/main/3      eg004.201    magnihotri  1. Header files added for eGTPC PSF Upgrade
*********************************************************************91*/
