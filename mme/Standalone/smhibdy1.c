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
  
     Name:     TCP UDP Convegence Layer Sample Stack Manager
  
     Type:     C source file
  
     Desc:     Sample code for Management Interface primitives supplied 
               by TRILLIUM
              
     File:     smhibdy1.c
  
     Sid:      smhibdy1.c@@/main/5 - Mon Mar  3 20:09:30 2008

     Prg:      asa
  
*********************************************************************21*/

/*
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions are provided in this file for the
layer management service user:
  
     SmMiLhiStaInd      Status Indication
     SmMiLhiStsCfm      Statistics Confirm
     SmMiLhiStaCfm      Status Confirm
     SmMiLhiAcntInd     Accounting Indication
     SmMiLhiTrcInd      Trace Indication

It should be noted that not all of these functions may be required
by a particular layer management service user.

It is assumed that the following functions are provided in the
layer managment service user file:
  
     SmMiLhiCfgReq      Configure Request
     SmMiLhiStaReq      Status Request
     SmMiLhiStsReq      Statistics Request
     SmMiLhiCntrlReq    Control Request
  
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
#include "cm_hash.h"       /* common hashing */
#include "cm_err.h"        /* common error */
#include "cm5.h"           /* common timer */
#include "cm_tpt.h"        /* common transport defines */
#include "lhi.h"           /* layer management, HI Layer */

#ifdef FTHA
#include "sht.h"           /* SHT interface */
#endif

#ifdef HI_ACC
#include "hi_acc.h"        /* acceptance */
#else 
#ifdef HC_ACC
#include "hc_acc.h"
#else
#ifdef HG_ACC
#include "cm_tkns.h"       /* common tokens */
#include "cm_llist.h"      /* llist mgmt*/
#include "cm_mblk.h"       /* common memory alloc */
#include "cm32x.h"         /* encoding/decoding */
#include "cm_pasn.h"
#include "cm_dns.h"
#include "lhgt.h"
#include "hgt.h"
#include "lhg.h"
#include "hg_acc.h"        /* acceptance */
#else
#ifdef SO_ACC
#include "cm_tkns.h"       /* common tokens */
#include "cm_sdp.h"        /* common SDP */
#include "cm_mblk.h"       /* common memory allocation */
#include "sot.h"           /* SOT interface */
#include "lso.h"           /* layer management, SIP  */
#include "so_acc.h"        /* defines for SIP test layer */

/* hi022.201 Add  unsolicited status indication for RTP */
#else
#ifdef HR_ACC
#include "cm5.h"                /* common timer */
#include "cm_hash.h"            /* common hash list */
#include "cm_llist.h"           /* common linked list */
#include "cm_tpt.h"             /* common transport defines */
#include "cm_rtp.h"             /* common RTP/RTCP defines */
#include "hrt.h"                /* HR upper interface defines */
#include "lhr.h"                /* HR layer management defines */
#include "hr.h"
#include "hr_acc.h"        /* acceptance */

#endif /* HR_ACC */
#endif /* SO_ACC */
#endif /* HG_ACC */
#endif /* HC_ACC */
#endif /* HI_ACC */

/*hi005.201 MGCP release 2.1 */
#ifdef MGAC_ACC
#include "cm_tkns.h"       /* common tokens */
#include "cm_sdp.h"        /* common SDP */
#include "cm_mblk.h"       /* common memory allocation */
#include "cm_dns.h"
#include "mgt.h"           /* MGT defines */
#include "lmg.h"           /* layer management defines for MGCP */
#include "cm_xta.h"
#include "mgacc.h"        /* defines for MGCP acceptance tests */
#endif /* MGAC_ACC */

#include "smhi_err.h"      /* Stack Manager - error */

/* common header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* common sockets */
#include "cm_hash.x"       /* common hashing */
#include "cm5.x"           /* common timer */
#include "cm_tpt.x"        /* common transport typedefs */
#include "lhi.x"           /* layer management, HI layer */

#ifdef FTHA
#include "sht.x"           /* SHT interface */
#endif

#ifdef HI_LKSCTP
#include "sct.h"
#include "sct.x"
#endif

#ifdef HI_ACC
#include "hi_acc.x"        /* acceptance */
#else
#ifdef HC_ACC
#include "hc_acc.x"        /* H.323 acceptance test */
#else
#ifdef HG_ACC
#include "cm_tkns.x"       /* common tokens */
#include "cm_llist.x"      /* llist mngmt */
#include "cm_lib.x"        /* commmon Library */
#include "cm_mblk.x"       /* common memory alloc */
#include "cm32x.x"         /* encoding/decoding */
#include "cm_pasn.x"
#include "cm_xtree.x"
#include "cm_dns.x"
#include "lhgt.x"
#include "hgt.x"
#include "lhg.x"
#include "hg_acc.x"
#else
#ifdef SO_ACC
#include "cm_lib.x"        /* common library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_sdp.x"        /* common SDP */
#include "cm_mblk.x"       /* common memory allocation */
#include "sot.x"           /* SOT interface */
#include "lso.x"           /* layer management, SIP  */
#include "so_acc.x"        /* typedefs for SIP test layer */
/* hi022.201 Add  unsolicited status indication for RTP */
#else
#ifdef HR_ACC
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* common timer */
#include "cm_hash.x"            /* common hash list */
#include "cm_llist.x"           /* common linked list */
#include "cm_tpt.x"             /* common transport defines */
#include "cm_rtp.x"             /* common RTP/RTCP defines */
#include "hrt.x"                /* HR upper interface defines */
#include "lhr.x"                /* HR layer management defines */
#include "hr.x"
#include "hr_acc.x"        /* acceptance */

#endif /* HR_ACC */

#endif /* SO_ACC */

#endif /* HG_ACC */ 
#endif /* HC_ACC */
#endif /* HI_ACC */
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
#include "cm_xta.h"
#include "cm_xta.x"
#include "soac_acc.x"
#endif
/* hi004.201: Addition for Iu-UP protocol */
/* hi008.201: 1. IU_UP 1.2 release changes */
#ifdef LX_ACC 
#include "llx.h"
#include "lxt.h"
#include "cm_xta.h"
#include "cm_xta_app.h"
#ifdef LX_RTP
#include "hrt.h"
#include "lhr.h"
#include "cm_rtp.h"
#endif
#ifdef LX_GTP
#include "ggu.h"
#include "lgt.h"
#endif
#include "lxac_acc.h"

#include "llx.x"
#include "lxt.x"
#ifdef LX_RTP
#include "cm_rtp.x"
#include "hrt.x"
#include "lhr.x"
#endif
#ifdef LX_GTP
#include "ggu.x"
#include "lgt.x"
#endif
#include "cm_xta.x"
#include "lxac_acc.x"
#endif /* LX_ACC */

/*hi005.201 MGCP release 2.1 */
#ifdef MGAC_ACC
#include "cm_lib.x"        /* common library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_sdp.x"        /* common SDP */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_dns.x"
#include "cm_xta.x"
#include "mgac_lhi.x"
#endif /* MGAC_ACC */

/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
#include "cm_xta.h"
#include "cm_xta.x"
#include "szac_lhi.x"
#endif


/* local typedefs */
  
/* private variable declarations */ 


/*
*     interface functions to layer management service user
*/
/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 SmMiLhiCfgCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
   TRC2(SmMiLhiCfgCfm)

#ifdef HI_ACC 
   (Void)hiAccProcCfgCfm(pst, cfm);
#else
#ifdef HC_ACC
   (Void)hcAccProcLhiCfgCfm(pst, cfm);
#else
#ifdef HG_ACC
   (Void)hgAccHndlLhiCfgCfm(cfm);
#else
#ifdef SO_ACC
   (Void) soAccProcLhiCfgCfm(pst, cfm);
#else
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
   soAcSmMiLhiCfgCfm(pst, cfm);
#else
/* hi013.201: Additions for eGTP */
#ifdef EG_ACC
   egAcSmMiLhiCfgCfm(pst, cfm);
#else
/* hi005.201 : Making sync with TUCL 1.5 files */
#ifdef MGAC_ACC
   mgAcSmMiLhiCfgCfm(pst, cfm);
#else
/* hi004.201 : Updated for IuUP 1.1  release */
#ifdef LX_ACC
   lxAcSmMiLhiCfgCfm(pst, cfm);
#else
/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
   szAcSmMiLhiCfgCfm(pst, cfm);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
   RETVALUE(ROK);
} /* end of SmMiLhiCfgCfm */


/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
 
#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLhiCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* control */
#endif
{
 
   TRC2(SmMiLhiCntrlCfm)
 
#ifdef HI_ACC 
   (Void)hiAccProcCntrlCfm(pst, cfm);
#else
#ifdef HC_ACC
   (Void)hcAccProcLhiCntrlCfm(pst, cfm);
#else
#ifdef HG_ACC
   (Void)hgAccHndlCntrlCfm((HgMngmt *) cfm);
#else
#ifdef SO_ACC
   (Void)soAccProcLhiCntrlCfm(pst, cfm);
#else
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
   soAcSmMiLhiCntrlCfm(pst, cfm);
#else
/* hi013.201: Additions for eGTP */
#ifdef EG_ACC
   egAcSmMiLhiCntrlCfm(pst, cfm);
#else
/* hi005.201 : Updated for GCP 2.1 release */
#ifdef MGAC_ACC
   mgAcSmMiLhiCntrlCfm(pst, cfm);
#else 
/* hi004.201 : Updated for IuUP 1.1  release */
#ifdef LX_ACC
   lxAcSmMiLhiCntrlCfm(pst, cfm);
#else
/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
   szAcSmMiLhiCntrlCfm(pst, cfm);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

   RETVALUE(ROK);
} /* end of SmMiLhiCntrlCfm */

    
/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present  unsolicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStaInd
(
Pst     *pst,           /* post structure */
HiMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLhiStaInd(pst, usta)
Pst     *pst;           /* post structure */
HiMngmt *usta;          /* unsolicited status */
#endif
{

   TRC2(SmMiLhiStaInd)

#ifdef HI_ACC 
   (Void)hiAccProcStaInd(pst, usta);
#else
#ifdef HC_ACC
   (Void)hcAccProcLhiStaInd(pst, usta);
#else
#ifdef HG_ACC
   (Void)hgAccHndlLhiStaInd((HiMngmt *) usta);
#else
#ifdef SO_ACC
   (Void)soAccProcLhiStaInd(pst, usta);
#else
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
   soAcSmMiLhiStaInd(pst, usta);
#else
/* hi013.201: Additions for eGTP */
#ifdef EG_ACC 
   egAcSmMiLhiStaInd(pst, usta);
#else
/* hi005.201 : Updated for GCP 2.1 release */
#ifdef MGAC_ACC
   mgAcSmMiLhiStaInd(pst, usta);
#else 
/* hi004.201 : Updated for IuUP 1.1  release */
#ifdef LX_ACC
   lxAcSmMiLhiStaInd(pst, usta);
#else
/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
   szAcSmMiLhiStaInd(pst, usta);
#else
/* hi022.201 Add  unsolicited status indication for RTP */
#ifdef HR_ACC
   (Void)hrAccProcStaInd(pst, (HrMngmt*)usta);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

   RETVALUE(ROK);
} /* end of SmMiLhiStaInd */


  
/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used to present trace
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiTrcInd
(
Pst *pst,               /* post structure */
HiMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLhiTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
HiMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   TRC2(SmMiLhiTrcInd)

#ifdef HI_ACC 
   (Void)hiAccProcTrcInd(pst, trc, mBuf);
#else
#ifdef HC_ACC
   (Void)hcAccProcLhiTrcInd(pst, trc, mBuf);
#else
#ifdef HG_ACC
   (Void)hgAccHndlLhiTrcInd((HiMngmt *) trc, mBuf);
#else
#ifdef SO_ACC
   (Void)soAccProcLhiTrcInd(pst, (HiMngmt *) trc, mBuf);
#else
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
   soAcSmMiLhiTrcInd(pst, trc, mBuf);
#else
/* hi005.201 : Updated for GCP 2.1 release */
#ifdef MGAC_ACC
   mgAcSmMiLhiTrcInd(pst, trc, mBuf);
#else
/* hi004.201 : Updated for IuUP 1.1  release */  
#ifdef LX_ACC
   lxAcSmMiLhiTrcInd(pst, trc, mBuf);
#else
/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
   szAcSmMiLhiTrcInd(pst, trc, mBuf);
#else
	/* hi025.201 free buffer in default case */
	SPrntMsg(mBuf, 0, 0);
	(Void)SPutMsg(mBuf);
#endif
#endif
#endif
#endif
#endif 
#endif
#endif
#endif
   RETVALUE(ROK);
} /* end of SmMiLhiTrcInd */

    
/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used to present solicited statistics 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStsCfm
(
Pst       *pst,         /* post structure */
HiMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLhiStsCfm(pst, sts)
Pst       *pst;         /* post structure */
HiMngmt   *sts;         /* confirmed statistics */
#endif
{
   TRC2(SmMiLhiStsCfm)

#ifdef HI_ACC 
   (Void)hiAccProcStsCfm(pst, sts);
#else
#ifdef HC_ACC
   (Void)hcAccProcLhiStsCfm(pst, sts);
#else
#ifdef HG_ACC
   (Void)hgAccHndlLhiStsCfm((HiMngmt *) sts);
#else
#ifdef SO_ACC
   (Void)soAccProcLhiStsCfm(pst, (HiMngmt *) sts);
#else
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
   soAcSmMiLhiStsCfm(pst, sts);
#else
/* hi013.201: Additions for eGTP */
#ifdef EG_ACC 
   egAcSmMiLhiStsCfm(pst, sts); 
#else
/* hi005.201 : Updated for GCP 2.1 release */
#ifdef MGAC_ACC
   mgAcSmMiLhiStsCfm(pst, sts);
#else
/* hi004.201 : Updated for IuUP 1.1  release */
#ifdef LX_ACC
   lxAcSmMiLhiStsCfm(pst, sts);
#else
/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
   szAcSmMiLhiStsCfm(pst, sts);
#else
	/* hi025.201 free buffer in default case */
#ifdef LHI_THR_STS
      if(sts->hdr.elmId.elmnt == STHITHR)
      {
         HiThrSts      *thrSts;         /* Thread conneciton statistics */
         U32 iCount, jCount;
			Txt        prntBuf[PRNTSZE];
         SPrint("----- Thread connection statistics -----\n");
         thrSts = &sts->t.sts.s.thrSts;
         sprintf(prntBuf, "Num of Receive threads =  %ld\n", thrSts->numRcvThrds);
         SPrint(prntBuf);

         for(iCount = 0; iCount < thrSts->numRcvThrds; iCount++)
         {
            sprintf(prntBuf, "Num of Connections in thread (%ld) = %ld \n",
                          (iCount+1), thrSts->thrConSts[iCount].numCons);
            SPrint(prntBuf);
            
            for(jCount = 0; jCount < thrSts->thrConSts[iCount].numCons; jCount++)
            {
              sprintf(prntBuf, "Msg received on connection (%ld) in bytes = %ld\n",(jCount+1),
                             thrSts->thrConSts[iCount].conSts[jCount].numRxBytes ); 
              SPrint(prntBuf);
              sprintf(prntBuf, "Msg transmitted on connection (%ld) in bytes = %ld\n",(jCount+1),
                             thrSts->thrConSts[iCount].conSts[jCount].numTxBytes ); 
              SPrint(prntBuf);

            } 
      
            SPutSBuf(pst->region, pst->pool, (Data* )thrSts->thrConSts[iCount].conSts,
                    (Size)(sizeof(HiConSts) * thrSts->thrConSts[iCount].numCons));
            thrSts->thrConSts[iCount].conSts = NULLP;
 
         }
      
         SPutSBuf(pst->region, pst->pool, (Data* )thrSts->thrConSts,
                       (Size)(sizeof(HiThrConSts) * thrSts->numRcvThrds));
         thrSts->thrConSts = NULLP;

      }
#endif /*LHI_THR_STS*/
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
 
   RETVALUE(ROK);
} /* end of SmMiLhiStsCfm */

    
/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used to present solicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStaCfm
(
Pst     *pst,           /* post structure */
HiMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLhiStaCfm(pst, sta)
Pst     *pst;           /* post structure */
HiMngmt *sta;            /* confirmed status */
#endif
{
   TRC2(SmMiLhiStaCfm)

#ifdef HI_ACC 
   (Void)hiAccProcStaCfm(pst, sta);
#else
#ifdef HC_ACC
   (Void)hcAccProcLhiStaCfm(pst, sta);
#else
#ifdef HG_ACC
   (Void)hgAccHndlLhiStaCfm((HiMngmt *) sta);
#else
#ifdef SO_ACC
   (Void)soAccProcLhiStaCfm(pst, (HiMngmt *) sta);
#else
/* hi012.105 : Updated for SIP 2.2  release */
#ifdef SO_ACC22
   soAcSmMiLhiStaCfm(pst, sta);
#else
/* hi013.201: Additions for eGTP */
#ifdef EG_ACC 
   egAcSmMiLhiStaCfm(pst, sta); 
#else
/* hi005.201 : Updated for GCP 2.1 release */
#ifdef MGAC_ACC
   mgAcSmMiLhiStaCfm(pst, sta);
#else
/* hi004.201 : Updated for IuUP 1.1  release */
#ifdef LX_ACC
   lxAcSmMiLhiStaCfm(pst, sta);
#else
/* hi007.201 Updated for S1AP release 1.1 */
#ifdef SZ
   szAcSmMiLhiStaCfm(pst, sta);
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif

   RETVALUE(ROK);
} /* end of SmMiLhiStaCfm */

  
/*
*
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhibdy1.c
*
*/
  
#ifdef ANSI
PUBLIC S16 smHiActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smHiActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smHiActvInit);

   UNUSED(region);
   UNUSED(reason);
   UNUSED(ent);
   UNUSED(inst);

   RETVALUE(ROK);
} /* end of smHiActvInit */


/********************************************************************30**
 
         End of file:     smhibdy1.c@@/main/5 - Mon Mar  3 20:09:30 2008

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
/main/2     ---       cvp  1. changed HC to HC_ACC.
                           2. changed the copyright header. 
/main/2+    hi002.13  cvp  1. Annex G related changes.
/main/2+    hi006.13  asa  1. SIP related additions.
/main/3     ---       cvp  1. Release with multi-threaded TUCL.
/main/3+    hi002.104 an   1. Removed H.323 core header file 
/main/4      ---       kp   1. Updated for release 1.5.
/main/5     hi012.105  svp  1. Updated for SIP 2.2  release
/main/5      ---       hs   1. Updated for release of 2.1
/main/5     hi004.201  skv  1. Updated for Iu-UP release of 1.1
/main/5     hi005.201  skv  1. MGCP release 2.1  
/main/5     hi007.201  hsingh  1. Updated for S1AP release 1.1
/main/5     hi008.201  rss  1. IU_UP 1.2 release changes
$SID$       hi013.201  pchebolu  1. EGTPP 1.1 release changes
            hi022.201   mm  1. Add  unsolicited status indication for unbind
                                request.
/main/5     hi025.201  rss  1. Free buffer in default case 
*********************************************************************91*/
