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

     Name:    Trillium LTE CNE - SGW Stack Module

     Type:    C Include file

     Desc:    This file contains the stack manager source code

     File:    qosm_qocfg.c

     Sid:      qosm_qocfg.c@@/main/1 - Thu Jul  8 21:22:23 2010

     Prg:       sp      
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
#include "cm_dns.h"         /* common DNS libraru defines */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "lqo.h"           /* CNE Layer management           */
#include "qo.h"           /* CNE Layer management           */
#include "qosm.h"          /* */

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
#include "cm_dns.x"         /* common DNS libraru defines */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "lqo.x"           /* CNE Layer management           */
#include "qo.x"           /* CNE Layer management           */
#include "qosm.x"          



/* QO_TODO_SECOND_PHASE - Expect a separate config request to configure
   the EGT SAP at MME. Hence should add another macro to track the
   confirmation of the same SAP similar to SZT SAP. */

#define QO_BND_SAP_EG_U    0x1
#define QO_BND_SAP_PGW_C   0x2
#define QO_BND_SAP_MME_C   0x4
#define QO_BND_SAP_ECHO_C  0x8
#define QO_BND_SAP_SGSN_C  0x10

#define QO_SGW_CONFIGURED  (STQOGEN | STQOEGUSAP | \
                            STQOPGWCSAP | STQOMMECSAP | STQOECHOCSAP | STQOSGSNCSAP)

#define QO_SM_EG_SAP_BND  (QO_BND_SAP_EG_U | QO_BND_SAP_PGW_C \
      | QO_BND_SAP_MME_C | QO_BND_SAP_ECHO_C | QO_BND_SAP_SGSN_C)

U16     qoSgwEgSapBnd = 0;
#ifdef QO_SGW
PRIVATE U8      qoSgwCfg = 0;
#endif /* QO_SGW */

PRIVATE Void qoSgwLqoGenCfg ARGS ((Void));

PRIVATE Void qoSgwLqoLSAPCfg ARGS ((SuId        suId,
                                    SpId        spId,
                                    Elmnt       elem,
                                    U16         port));


#ifdef QO_SGW
/*
 *
 *       Fun:    qoSgwLqoCfg - configure SGW
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_qocfg.c
 *
 */

#ifdef ANSI
PUBLIC Void qoSgwLqoCfg
(
Void
)
#else
PUBLIC Void qoSgwLqoCfg()
#endif /* ANSI */
{
   SM_TRC2(qoSgwLqoCfg);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sending SGW Qo configuration...\n"));
   qoSgwLqoGenCfg();
   qoSgwLqoLSAPCfg(QOSM_SGW_EG_U_SUID, QOSM_SGW_EG_U_SPID, STQOEGUSAP,
         qoSmCb.cfgCb.egDfltUPort);
   qoSgwLqoLSAPCfg(QOSM_SGW_S5S8_C_SUID, QOSM_SGW_S5S8_C_SPID, STQOPGWCSAP,
         qoSmCb.cfgCb.egS5S8CPort);
   qoSgwLqoLSAPCfg(QOSM_SGW_S11_C_SUID , QOSM_SGW_S11_C_SPID , STQOMMECSAP,
         qoSmCb.cfgCb.egS11CPort);
   qoSgwLqoLSAPCfg(QOSM_SGW_ECHO_EG_SUID , QOSM_SGW_ECHO_EG_SPID , STQOECHOCSAP,
         qoSmCb.cfgCb.egDfltCPort);
   qoSgwLqoLSAPCfg(QOSM_SGW_S4_C_SUID, QOSM_SGW_S4_C_SPID, STQOSGSNCSAP,
         qoSmCb.cfgCb.egS4CPort);

   RETVOID;
}
/*
 *
 *       Fun:    qoSgwLqoGenCfg - fill in default genCfg for QO
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_qocfg.c
 *
 */

#ifdef ANSI
PRIVATE Void qoSgwLqoGenCfg
(
Void
)
#else
PRIVATE Void qoSgwLqoGenCfg()
#endif /* ANSI */
{
   QoMngmt  qoSgwMgt;
   QoGenCfg  *cfg;

   SM_TRC2(qoSgwLqoGenCfg);

   cmMemset((U8 *)&qoSgwMgt, 0, sizeof(QoMngmt));
   qoSmDefHdr(&qoSgwMgt.hdr, ENTEU, STQOGEN, QOSM_QOSMSEL);

   cfg   = &(qoSgwMgt.u.cfg.s.sgwGenCfg);

   cfg->lmPst.srcProcId = SFndProcId();
   cfg->lmPst.dstProcId = SFndProcId();
   cfg->lmPst.srcEnt = (Ent)ENTEU;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;

   cfg->lmPst.prior = (Prior)QOSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = (Region)qoSmCb.init.region;
   cfg->lmPst.pool = (Pool)qoSmCb.init.pool;
   cfg->lmPst.selector = (Selector)QOSM_QOSMSEL;
   cfg->mem.region = (Region)qoSmCb.init.region;
   cfg->mem.pool   = (Pool)qoSmCb.init.pool;
   cfg->maxNmbEgtSaps = 6;
   cfg->maxNmbBearers  = 10;
   cfg->maxNmbTunls   = qoSmCb.cfgCb.ueNum;
   cfg->tmrRes        = LQO_SGW_TMR_RES;
   (Void)SmMiLqoCfgReq(&qoSmCb.qoSgwPst, &qoSgwMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent SGW Qo GenCfg...\n"));

   RETVOID;
} /* end of qoSgwLqoGenCfg() */

/*
 *
 *       Fun:    qoSgwLqoLSAPCfg - fill in default EGT Sap
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_qocfg.c
 *
 */

#ifdef ANSI
PRIVATE Void qoSgwLqoLSAPCfg
(
SuId        suId,
SpId        spId,
Elmnt       elem,
U16         port
)
#else
PRIVATE Void qoSgwLqoLSAPCfg(suId, spId, elem, port)
SuId        suId;
SpId        spId;
Elmnt       elem;
U16         port;
#endif /* ANSI */
{
   QoMngmt      qoSgwMgt;
   QoLSapCfg  *cfg;

   SM_TRC2(qoSgwLqoLSapCfg);

   cmMemset((U8 *)&qoSgwMgt, 0, sizeof(QoMngmt));
   qoSmDefHdr(&qoSgwMgt.hdr, ENTEU, elem, QOSM_QOSMSEL);

   cfg   = &(qoSgwMgt.u.cfg.s.sgwLSapCfg);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = QOSM_QOEGSEL;
   cfg->mem.region   = qoSmCb.init.region;
   cfg->mem.pool     = qoSmCb.init.pool;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SFndProcId();
   cfg->dstEntId     = ENTEG;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = QOSM_LQO_VAL_10;
   cfg->maxBndRetry = QOSM_LQO_VAL_3;

   cfg->localAddr.type   = CM_TPTADDR_IPV4;
   cfg->localAddr.u.ipv4TptAddr.address = qoSmCb.cfgCb.sgwIpAddr;
   cfg->localAddr.u.ipv4TptAddr.port    = port;
   
   cfg->nmbTunn = qoSmCb.cfgCb.ueNum;

   (Void)SmMiLqoCfgReq(&qoSmCb.qoSgwPst, &qoSgwMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent SGW Qo LSapCFg...\n"));
   RETVOID;
} /* end of qoSgwLqoLSapCfg() */

#endif /* QO_SGW */

#ifdef QO_SGW
/*
 *      FUN:   qoSgwEgtSapBndReq
 *
 *      Desc:  Binds SGW with EGTP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  qosm_qocfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgtSapBndReq
(
Void
)
#else
PUBLIC S16 qoSgwEgtSapBndReq()
#endif /* ANSI */
{
   QoMngmt  cntrl;
   SM_TRC2(qoSgwEgtSapBndReq);
   cmMemset((U8 *)&cntrl, 0, sizeof(QoMngmt));
   qoSmDefHdr(&cntrl.hdr, ENTEU, STQOEGUSAP, QOSM_QOSMSEL);

   cntrl.u.cntrl.action    = ABND;
   cntrl.u.cntrl.subAction = 0;
   cntrl.u.cntrl.u.sap.id  = QOSM_SGW_EG_U_SPID;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */
   qoSmCb.qoSgwPst.event = EVTLQOCNTRLREQ;
   (Void)SmMiLqoCntrlReq(&qoSmCb.qoSgwPst, &cntrl);

   qoSmDefHdr(&cntrl.hdr, ENTEU, STQOPGWCSAP, QOSM_QOSMSEL);
   cntrl.u.cntrl.u.sap.id  = QOSM_SGW_S5S8_C_SPID;
   (Void)SmMiLqoCntrlReq(&qoSmCb.qoSgwPst, &cntrl);

   qoSmDefHdr(&cntrl.hdr, ENTEU, STQOMMECSAP, QOSM_QOSMSEL);
   cntrl.u.cntrl.u.sap.id  = QOSM_SGW_S11_C_SPID;
   (Void)SmMiLqoCntrlReq(&qoSmCb.qoSgwPst, &cntrl);

   qoSmDefHdr(&cntrl.hdr, ENTEU, STQOECHOCSAP, QOSM_QOSMSEL);
   cntrl.u.cntrl.u.sap.id  = QOSM_SGW_ECHO_EG_SPID;
   (Void)SmMiLqoCntrlReq(&qoSmCb.qoSgwPst, &cntrl);
   
   qoSmDefHdr(&cntrl.hdr, ENTEU, STQOSGSNCSAP, QOSM_QOSMSEL);
   cntrl.u.cntrl.u.sap.id  = QOSM_SGW_S4_C_SPID;
   (Void)SmMiLqoCntrlReq(&qoSmCb.qoSgwPst, &cntrl);


   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent SGW EgtSapBndREq...\n"));
   RETVALUE(ROK);
}
#endif /* QO_SGW */

/*
 *      FUN:   qoSgwDbgCntrlReq
 *
 *      Desc:  Binds SGW with EGTP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  qosm_qocfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwDbgCntrlReq
(
Void
)
#else
PUBLIC S16 qoSgwDbgCntrlReq()
#endif /* ANSI */
{
   QoMngmt  cntrl;
   SM_TRC2(qoSgwDbgCntrlReq);
#ifdef DEBUGP
   cmMemset((U8 *)&cntrl, 0, sizeof(QoMngmt));
   qoSmDefHdr(&cntrl.hdr, ENTEU, STQOGEN, QOSM_QOSMSEL);

   cntrl.u.cntrl.action    = AENA;
   cntrl.u.cntrl.subAction = SADBG;
   cntrl.u.cntrl.u.dbg.dbgMask  = qoSmCb.cfgCb.sgwDbgMask;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */
   qoSmCb.qoSgwPst.event = EVTLQOCNTRLREQ;
   (Void)SmMiLqoCntrlReq(&qoSmCb.qoSgwPst, &cntrl);

   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent SGW Cntrl Req for Debug...\n"));
#endif
   RETVALUE(ROK);
}
/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by Layer to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  nysm_qocfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoCfgCfm
(
Pst     *pst,          /* post structure */
QoMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLqoCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
QoMngmt *cfm;          /* configuration */
#endif
{
   SM_TRC2(SmMiLqoCfgCfm);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Received SGW Qo CfgCfm with - status(%d)...\n",
            cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      qoSgwCfg |=  cfm->hdr.elmId.elmnt;
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received SGW Configuration Cfm...\n"));    
      /* If S-GW Configuration is complete*/
      if(qoSgwCfg == QO_SGW_CONFIGURED)
      {
         qoSgwSendMsg(EVTQOSGWCFGDONE);
      }
      else
      {
         QOSM_DBG_INFO((QOSM_PRNTBUF,"Received SGW Configuration PENDING...\n"));
         /* Debug print here that config is still pending */
      }
   }
   else
   {
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Configuration FAILED for node...\n"));
      /* Debug print here that confirmation is not OK */
   }

   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));
   RETVALUE(ROK);
} /* end of SmMiLqoCfgCfm */

/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by  Layer to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  nysm_qocfg.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLqoCntrlCfm
(
Pst     *pst,          /* post structure */
QoMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLqoCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
QoMngmt *cfm;          /* control */
#endif
{
   SM_TRC2(SmMiLqoCntrlCfm);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received SGW Control Cfm...\n"));
      /* qoSgwSendMsg(EVTQOSGWBINDCFM); */
   }
   else
   {
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Control Cfm. NOT OK for node..\n"));
      /* Debug print here that confirmation is not OK */
   }
   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(QoMngmt));

   RETVALUE(ROK);
} /* end of SmMiLqoCntrlCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by Layer to present  unsolicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  nysm_qocfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoStaInd
(
Pst     *pst,           /* post structure */
QoMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLqoStaInd(pst, usta)
Pst     *pst;           /* post structure */
QoMngmt *usta;          /* unsolicited status */
#endif
{
   SM_TRC2(SmMiLqoStaInd);

   /* Debug print here - print all the information from usta's Alarm structure
      QoUstaDgn */
   if ((usta->u.usta.alarm.event == LQO_EVENT_SGW_EGTP_BNDCFM) &&
         (usta->u.usta.alarm.cause == LCM_CAUSE_UNKNOWN))
   {
      switch(usta->u.usta.sapId)
      {
         case QOSM_SGW_EG_U_SUID:
            qoSgwEgSapBnd |= QO_BND_SAP_EG_U;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw Alarm\
                     Sap S1-U interface bound\n"));
            break;
         case QOSM_SGW_S11_C_SUID:
            qoSgwEgSapBnd |= QO_BND_SAP_MME_C;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw Alarm\
                     Sap S11-C interface bound\n"));
            break;
         case QOSM_SGW_S5S8_C_SUID:
            qoSgwEgSapBnd |= QO_BND_SAP_PGW_C;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw Alarm\
                     Sap S5S8-C interface bound\n"));
            break;
         case QOSM_SGW_ECHO_EG_SUID:
            qoSgwEgSapBnd |= QO_BND_SAP_ECHO_C;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw Alarm\
                     Sap Echo-C interface bound\n"));
            break;
         case QOSM_SGW_S4_C_SUID:
            qoSgwEgSapBnd |= QO_BND_SAP_SGSN_C;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw Alarm\
                     Sap S4-C interface bound\n"));
            break;
         default:
            QOSM_DBG_ERROR((QOSM_PRNTBUF,"Invalid Sap\n"));
            break;
      }
      if(qoSgwEgSapBnd  == QO_SM_EG_SAP_BND)
         qoSgwSendMsg(EVTQOSGWBINDCFM);
   }

   SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(QoMngmt));
   RETVALUE(ROK);
} /* end of SmMiLqoStaInd */


/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used by  Layer to present trace
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  nysm_qocfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoTrcInd
(
Pst *pst,               /* post structure */
QoMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLqoTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
QoMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   SM_TRC2(SmMiLqoTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLqoTrcInd */

/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used by Layer to present solicited statistics 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  nysm_qocfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoStsCfm
(
Pst       *pst,         /* post structure */
QoMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLqoStsCfm(pst, sts)
Pst       *pst;         /* post structure */
QoMngmt   *sts;         /* confirmed statistics */
#endif
{
   SM_TRC2(SmMiLqoStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLqoStsCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by Layer to present solicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  nysm_qocfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoStaCfm
(
Pst     *pst,           /* post structure */
QoMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLqoStaCfm(pst, sta)
Pst     *pst;           /* post structure */
QoMngmt *sta;            /* confirmed status */
#endif
{
   SM_TRC2(SmMiLqoStaCfm);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLqoStaCfm */

/**********************************************************************

         End of file:     qosm_qocfg.c@@/main/1 - Thu Jul  8 21:22:23 2010

**********************************************************************/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp                 1.LTE-CNE:SGW Initial Release
*********************************************************************91*/



