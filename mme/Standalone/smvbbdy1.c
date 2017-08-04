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

     Name:    LTE CNE 

     Type:    C source file

     Desc:    C code for controlling acceptance tests execution.

     File:    smvbbdy1.c 

     Sid:      smvbbdy1.c@@/main/1 - Mon Jun 28 19:35:33 2010

     Prg:     ap

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
#include "cm_xtree.x"      /* Common radix tree library       --*/
#include "cm_mblk.x"       /* Common memory allocation        --*/
#include "cm_inet.x"       /* Common socket library           --*/

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

#ifdef ANSI
PUBLIC S16 smVbActvInit
(
Ent    ent,                   /* entity */
Inst   inst,                  /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smVbActvInit(ent, inst, region, reason)
Ent    ent;                   /* entity */
Inst   inst;                  /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smVbActvInit)
   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   (Void)vbInitExt();

   RETVALUE(ROK);
} /* smVbActvInit */


/*
 *
 *       Fun:    vbInitExt - external initialization
 *
 *       Desc:   performs additional initializations as required
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  to be changed by the customer if required
 *
         File:   vb_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 vbInitExt
(
void
)
#else
PUBLIC S16 vbInitExt()
#endif /* ANSI */
{
   TRC2(vbInitExt)
   vbSmInitCb();
   RETVALUE(ROK);
} /* end of vbInitExt()*/

#ifdef ANSI
PUBLIC Void vbSmInitCb
(
void
)
#else
PUBLIC Void vbSmInitCb()
#endif /* ANSI */
{

   cmMemset((U8 *)&vbSmCb, 0, sizeof(VbSmCb));

   /* used at SM---->HI */
   vbSmCb.hiPst.dstProcId = SFndProcId();
   vbSmCb.hiPst.srcProcId = SFndProcId();
   
   vbSmCb.hiPst.dstEnt    = (Ent)ENTHI;
   vbSmCb.hiPst.dstInst   = (Inst)0;
   vbSmCb.hiPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.hiPst.srcInst   = (Inst)0;
   vbSmCb.hiPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.hiPst.route     = (Route)RTESPEC;
   vbSmCb.hiPst.event     = (Event)EVTNONE;
   vbSmCb.hiPst.region    = (Region)vbSmCb.mem.region;
   vbSmCb.hiPst.pool      = (Pool)vbSmCb.mem.pool;
   vbSmCb.hiPst.selector  = (Selector)VBSM_SMHISEL;  

   /* used at SM---->SB */
   vbSmCb.sbPst.dstProcId = SFndProcId();
   vbSmCb.sbPst.srcProcId = SFndProcId();
   vbSmCb.sbPst.dstEnt    = (Ent)ENTSB;
   vbSmCb.sbPst.dstInst   = (Inst)0;
   vbSmCb.sbPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.sbPst.srcInst   = (Inst)0;
   vbSmCb.sbPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.sbPst.route     = (Route)RTESPEC;
   vbSmCb.sbPst.event     = (Event)EVTNONE;
   vbSmCb.sbPst.region    = (Region)vbSmCb.mem.region;
   vbSmCb.sbPst.pool      = (Pool)vbSmCb.mem.pool;
   vbSmCb.sbPst.selector  = (Selector)VBSM_SMSBSEL;  

   /* used at SM---->SZ */
   vbSmCb.szPst.dstProcId = SFndProcId();
   vbSmCb.szPst.srcProcId = SFndProcId();
   vbSmCb.szPst.dstEnt    = (Ent)ENTSZ;
   vbSmCb.szPst.dstInst   = (Inst)0;
   vbSmCb.szPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.szPst.srcInst   = (Inst)0;
   vbSmCb.szPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.szPst.route     = (Route)RTESPEC;
   vbSmCb.szPst.event     = (Event)EVTNONE;
   vbSmCb.szPst.region    = (Region)vbSmCb.mem.region;
   vbSmCb.szPst.pool      = (Pool)vbSmCb.mem.pool;
   vbSmCb.szPst.selector  = (Selector)VBSM_SMSZSEL;  

   /* used at SM---->VB */
   vbSmCb.vbMmePst.dstProcId = SFndProcId();
   vbSmCb.vbMmePst.srcProcId = SFndProcId();
   vbSmCb.vbMmePst.dstEnt    = (Ent)ENTVB;
   vbSmCb.vbMmePst.dstInst   = (Inst)0;
   vbSmCb.vbMmePst.srcEnt    = (Ent)ENTSM;
   vbSmCb.vbMmePst.srcInst   = (Inst)0;
   vbSmCb.vbMmePst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.vbMmePst.route     = (Route)RTESPEC;
   vbSmCb.vbMmePst.event     = (Event)EVTNONE;
   vbSmCb.vbMmePst.region    = (Region)vbSmCb.mem.region;
   vbSmCb.vbMmePst.pool      = (Pool)vbSmCb.mem.pool;
   vbSmCb.vbMmePst.selector  = (Selector)VBSM_SMVBSEL;  
   
   /* used at SM---->EG */
   vbSmCb.vbEgPst.dstProcId = SFndProcId();
   vbSmCb.vbEgPst.srcProcId = SFndProcId();
   vbSmCb.vbEgPst.dstEnt    = (Ent)ENTEG;
   vbSmCb.vbEgPst.dstInst   = (Inst)0;
   vbSmCb.vbEgPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.vbEgPst.srcInst   = (Inst)0;
   vbSmCb.vbEgPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.vbEgPst.route     = (Route)RTESPEC;
   vbSmCb.vbEgPst.event     = (Event)EVTNONE;
   vbSmCb.vbEgPst.region    = (Region)vbSmCb.mem.region;
   vbSmCb.vbEgPst.pool      = (Pool)vbSmCb.mem.pool;
   vbSmCb.vbEgPst.selector  = (Selector)VBSM_SMVBSEL;  

   RETVOID;
} /* end of vbSmInitCb() */
/*
 *
 *       Fun:    SmMiLvbCfgCfm
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
PUBLIC S16 SmMiLvbCfgCfm
(
Pst     *pst,
VbMngmt *cfg
)
#else
PUBLIC S16 SmMiLvbCfgCfm(pst, cfg)
Pst     *pst;
VbMngmt *cfg;
#endif /* ANSI */
{
   /* local variables */
   TRC3(SmMiLvbCfgCfm)

   vbAcSmMiLvbCfgCfm(pst, cfg);

   RETVALUE(ROK);

} /* SmMiLvbCfgCfm */

/*
 *
 *       Fun:    SmMiLvbCntrlCfm
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
PUBLIC S16 SmMiLvbCntrlCfm
(
Pst     *pst,
VbMngmt *cntrl
)
#else
PUBLIC S16 SmMiLvbCntrlCfm(pst, cntrl)
Pst     *pst;
VbMngmt *cntrl;
#endif /* ANSI */
{
   TRC3(SmMiLvbCntrlCfm)

   vbAcSmMiLvbCntrlCfm(pst, cntrl);

   RETVALUE(ROK);
} /* SmMiLvbCntrlCfm */

/*
 *
 *       Fun:    SmMiLvbStaInd
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
PUBLIC S16 SmMiLvbStaInd
(
Pst     *pst,
VbMngmt *usta
)
#else
PUBLIC S16 SmMiLvbStaInd(pst, usta)
Pst     *pst;
VbMngmt *usta;
#endif /* ANSI */
{

   TRC3(SmMiLvbStaInd)
   if ((usta->u.usta.alarm.event == LVB_EVENT_LI_BND_CFM) &&
       ( usta->u.usta.alarm.cause  == LSZ_CAUSE_SZT_BND_SUCC))
   {
       vbMmeSendMsg(EVTVBAPPBINDCFM);
   }

   RETVALUE(ROK);
} /* SmMiLvbStaInd */

/*
 *
 *       Fun:    SmMiLvbTrcInd
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
PUBLIC S16 SmMiLvbTrcInd
(
Pst     *pst,
VbMngmt *trc,
Buffer  *mBuf
)
#else
PUBLIC S16 SmMiLvbTrcInd(pst, trc, mBuf)
Pst     *pst;
VbMngmt *trc;
Buffer  *mBuf;
#endif /* ANSI */
{

    TRC3(SmMiLvbTrcInd)

    vbAcSmMiLvbTrcInd(pst, trc, mBuf);  

    RETVALUE(ROK);

} /* SmMiLvbTrcInd */

/*
 *
 *       Fun:    SmMiLvbStaCfm
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
PUBLIC S16 SmMiLvbStaCfm
(
Pst     *pst,
VbMngmt *sta
)
#else
PUBLIC S16 SmMiLvbStaCfm(pst, sta)
Pst     *pst;
VbMngmt *sta;
#endif /* ANSI */
{

   TRC3(SmMiLvbStaCfm)


   RETVALUE(ROK);
} /* SmMiLvbStaCfm */

/*
 *
 *       Fun:    SmMiLvbStsCfm
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
PUBLIC S16 SmMiLvbStsCfm
(
Pst     *pst,
VbMngmt *sts
)
#else
PUBLIC S16 SmMiLvbStsCfm(pst, sts)
Pst     *pst;
VbMngmt *sts;
#endif /* ANSI */
{
   
   TRC3(SmMiLvbStsCfm)

   vbAcSmMiLvbStsCfm(pst, sts);

   RETVALUE(ROK);
} /* SmMiLvbStsCfm */

/********************************************************************30**

         End of file:     smvbbdy1.c@@/main/1 - Mon Jun 28 19:35:33 2010

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
/main/1      -       sp                 1.LTE-MME: Initial Release
*********************************************************************91*/
