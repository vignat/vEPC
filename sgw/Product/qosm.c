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

     Name:    Trillium LTE SGW Application - Stack Manager

     Type:    C Source file

     Desc:    This file contains Stack Manager Functions  

     File:    qosm.c

     Sid:      qosm.c@@/main/1 - Thu Jul  8 21:22:21 2010

     Prg:      sp    
*********************************************************************21*/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"         /* common DNS libraru defines */
#include "cm_os.h"         /* common OS functionality */
#include "leg.h"
#include "egt.h"

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
/* qo003.101: HI .[xh] include files removed. */
#endif

#include "lqo.h"
#include "qosm.h"
#include "qo_err.h"
#include "qo.h" /* qo003.101 : ccpu00117528 config file checking */
#include <stdlib.h>
/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"         /* common DNS libraru defines */
#include "cm_os.x"         /* common OS functionality */

#include "leg.x"
#include "egt.x"

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
/* qo003.101: HI .[xh] include files removed. */
#endif

#include "lqo.x"
#include "qosm.x"

QoSmCb qoSmCb;

/* Tokens used in configuration file */
PRIVATE S8 qoSmCfgParams[][QOSM_MAX_CFG_PARAM_LEN] =
{
   "QOSM_EG_C_DFLT_PORT",
   "QOSM_EG_C_S11_PORT",
   "QOSM_EG_C_S5S8_PORT",
   "QOSM_EG_U_DFLT_PORT",
   "QOSM_EG_DFLT_HOST_NAME",
   "QOSM_SGW_IPADDR",
   "QOSM_UE_NUM"
#ifdef DEBUGP
    ,
   "QOSM_SGW_DBG_MASK",
   "QOSM_SM_DBG_MASK",
#endif
   "QOSM_EG_C_S4_PORT"
};
/* Function declarations */
PRIVATE Void qoSmInitCfgCb ARGS ((Void));

PRIVATE Void qoSgwSM ARGS((QoSmCneEvent event));

PRIVATE Void qoSgwCfgInit ARGS((QoSmCneEvent  event));

PRIVATE Void qoSgwCntrlInit ARGS((QoSmCneEvent  event));

PRIVATE Void qoSgwHdlInitEvent ARGS((QoSmCneEvent  event));

PRIVATE Void qoSgwHdlSgwCfgEvent ARGS((QoSmCneEvent  event));

#ifdef ANSI
PUBLIC Void qoSmInitCb
(
Void
)
#else
PUBLIC Void qoSmInitCb()
#endif /* ANSI */
{

   cmMemset((U8 *)&qoSmCb, 0, sizeof(QoSmCb));

   /* used at SM---->HI */
   qoSmCb.hiPst.dstProcId = SFndProcId();
   qoSmCb.hiPst.srcProcId = SFndProcId();

   qoSmCb.hiPst.dstEnt    = (Ent)ENTHI;
   qoSmCb.hiPst.dstInst   = (Inst)0;
   qoSmCb.hiPst.srcEnt    = (Ent)ENTSM;
   qoSmCb.hiPst.srcInst   = (Inst)0;
   qoSmCb.hiPst.prior     = (Prior)QOSM_MSGPRIOR;
   qoSmCb.hiPst.route     = (Route)RTESPEC;
   qoSmCb.hiPst.event     = (Event)EVTNONE;
   qoSmCb.hiPst.region    = (Region)qoSmCb.init.region;
   qoSmCb.hiPst.pool      = (Pool)qoSmCb.init.region;
   qoSmCb.hiPst.selector  = (Selector)QOSM_SMHISEL;


#ifdef QO_SGW

#ifdef EG
   /* used at SM---->EG */
   qoSmCb.egPst.dstProcId = SFndProcId();
   qoSmCb.egPst.srcProcId = SFndProcId();
   qoSmCb.egPst.dstEnt    = (Ent)ENTEG;
   qoSmCb.egPst.dstInst   = (Inst)0;
   qoSmCb.egPst.srcEnt    = (Ent)ENTSM;
   qoSmCb.egPst.srcInst   = (Inst)0;
   qoSmCb.egPst.prior     = (Prior)QOSM_MSGPRIOR;
   qoSmCb.egPst.route     = (Route)RTESPEC;
   qoSmCb.egPst.event     = (Event)EVTNONE;
   qoSmCb.egPst.region    = (Region)qoSmCb.init.region;
   qoSmCb.egPst.pool      = (Pool)qoSmCb.init.pool;
   qoSmCb.egPst.selector  = (Selector)QOSM_SMEGSEL;  
#endif

   /* used at SM---->EU */
   qoSmCb.qoSgwPst.dstProcId = SFndProcId();
   qoSmCb.qoSgwPst.srcProcId = SFndProcId();
   qoSmCb.qoSgwPst.dstEnt    = (Ent)ENTEU;
   qoSmCb.qoSgwPst.dstInst   = (Inst)0;
   qoSmCb.qoSgwPst.srcEnt    = (Ent)ENTSM;
   qoSmCb.qoSgwPst.srcInst   = (Inst)0;
   qoSmCb.qoSgwPst.prior     = (Prior)QOSM_MSGPRIOR;
   qoSmCb.qoSgwPst.route     = (Route)RTESPEC;
   qoSmCb.qoSgwPst.event     = (Event)EVTNONE;
   qoSmCb.qoSgwPst.region    = (Region)qoSmCb.init.region;
   qoSmCb.qoSgwPst.pool      = (Pool)qoSmCb.init.pool;
   qoSmCb.qoSgwPst.selector  = (Selector)QOSM_SMQOSEL;

#endif
   /* Read the configuration file and initialize the CfgCb */
   qoSmInitCfgCb();

   RETVOID;
} /* end of qoSmInitCb() */


/*
*
*       Fun:   qoSmInitCfgCb
*
*       Desc:  Read configuration file and populate the CfgCb.
*
*       Ret:   ROK, RFAILED
*
*       File:  qosm.c
*
*/


#ifdef ANSI
PUBLIC Void qoSmInitCfgCb
(
)
#else
PUBLIC Void qoSmInitCfgCb (Void)
#endif
{

   S8               line[QOSM_MAX_CFG_FILE_LINE_LEN];
   OsFile           *fp = NULLP;
   S8               *ret1 = NULLP;
   U32              idx = 0;
   S32              lineNo = 0;
   U8               numParams;
   S8               *lPtr;
   S8               word[QOSM_MAX_CFG_PARAM_LEN];
   U8               lineLen;
   Bool             inToken = TRUE;
   QoSmUsrCfgCb     *cfgCb = &qoSmCb.cfgCb;
#ifdef DEBUGP
   U8               mskIdx = 0;
   U32              dbgMask = 0;
   U8              noOfDigits = 4; /* equal to the number of masks to be set */
#endif


   SM_TRC2(qoSmInitCfgCb);

   if((fp = (OsFile*)osFopen("qosm_cfg.txt","r")) == (S32)NULLP)
   {
     /* qo003.101 : ccpu00117528 config file checking */
      QOLOGERROR(ERRCLS_DEBUG,0,0, 
            "FAILED to open the file qosm_cfg.txt\n ");
      exit(0);

   } /* if */

   numParams = sizeof(qoSmCfgParams)/QOSM_MAX_CFG_PARAM_LEN;

   lineNo = 1;

   for (;;)
   {
      cmMemset((U8*)line, 0, QOSM_MAX_CFG_FILE_LINE_LEN);

      ret1 = osFgets(line, QOSM_MAX_CFG_FILE_LINE_LEN, fp);

      if (ret1 == (S8*)-1 || ret1 == NULLD) /* eof or NULL */
      {
         QOSM_DBG_ERROR((QOSM_PRNTBUF, "End of File reached, qosm_cfg.txt\n"));
         break;
      }

      if (!(lineLen = osStripBlanks(line)))
      {
         lineNo++;
         continue;   /* blank line */
      }

      if (*line == '#')
      {
         lineNo++;
         continue;   /* blank line */
      }

      lPtr = line;

      /* Get each of the words from the line */
      while ((lPtr = osGetWord(lPtr, word)) != NULLP)
      {
         if (word[0] == '#')
         {
            break;
         }

         if (inToken)
         {
            /* Compare the word with the list of tokens */
            for (idx = 0; idx < numParams; idx++)
            {
               if(!osStrcmp(word, qoSmCfgParams[idx]))
               {
                  inToken = FALSE;
                  break;
               }
            }
            if (idx == numParams)
            {
               QOSM_DBG_ERROR((QOSM_PRNTBUF, " invalid configuration parameter \n"));
               if(fp) osFclose(fp);
               RETVOID;
            }
         }
         else
         {
            /* Now based on the index store the configuration values */
            switch(idx)
            {
               case 0: /* QOSM_EG_C_DFLT_PORT */
               {
                  cfgCb->egDfltCPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 1: /* QOSM_EG_C_S11_PORT */
               {
                  cfgCb->egS11CPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 2: /* QOSM_EG_C_S5S8_PORT */
               {
                  cfgCb->egS5S8CPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 3: /* QOSM_EG_U_DFLT_PORT */
               {
                  cfgCb->egDfltUPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 4: /* QOSM_EG_DFLT_HOST_NAME */
               {
                  osStrcpy(cfgCb->egDfltHstNm, word);
                  break;
               }
               case 5: /* QOSM_SGW_IPADDR */
               {
                  cmInetAddr(word, &cfgCb->sgwIpAddr);
                  cfgCb->sgwIpAddr = CM_INET_NTOH_U32(cfgCb->sgwIpAddr);
                  break;
               }
               case 6: /* QOSM_UE_NUM */
               {
                  cfgCb->ueNum = (U16)osStrtol(word, NULL, 10);
                  break;
               }

#ifdef DEBUGP
               case 7: /* QOSM_SGW_DBG_MASK */
               {
                  dbgMask = (U32)osStrtol(word, NULL, 10);
                  /* Initialize the debug mask */
                  cfgCb->sgwDbgMask = 0;
                  mskIdx = 0;

                  /* Apply debug mask as per the digits set in the configuration */
                  /* noOfDigits = 4; equal to the number of masks to be set */
                  for(noOfDigits = 4; noOfDigits > 0; noOfDigits--)
                  {
                     /* extracting each digit from the number
                      *                       * if number is set to 1
                      *                       then apply debug mask */
                     if(dbgMask % 10)
                     {
                        cfgCb->sgwDbgMask |= (DBGMASK_LYR << mskIdx);
                     }
                     /* divide to extract the next
                      * digit */
                     dbgMask = dbgMask / 10;
                     mskIdx++;
                  }
                  break;
               }
               case 8: /* QOSM_SM_DBG_MASK */
               {
                  dbgMask = (U32)osStrtol(word, NULL, 10);
                  /* Initialize the debug mask */
                  cfgCb->smDbgMask = 0;
                  mskIdx = 0;

                  /* Apply debug mask as per the digits set in the configuration */
                  /* noOfDigits = 4; equal to the number of masks to be set */
                  for(noOfDigits = 4; noOfDigits > 0; noOfDigits--)
                  {
                     /* extracting each digit from the number
                      *                       * if number is set to 1
                      *                       then apply debug mask */
                     if(dbgMask % 10)
                     {
                        cfgCb->smDbgMask |= (DBGMASK_LYR << mskIdx);
                     }
                     /* divide to extract the next
                      * digit */
                     dbgMask = dbgMask / 10;
                     mskIdx++;
                  }
                  qoSmCb.init.dbgMask = cfgCb->smDbgMask;/* qo003.101 : ccpu00117468 debug mask correction */
                  break;
               }

#endif 
               case 9: /* QOSM_EG_C_S4_PORT */
               {
                  cfgCb->egS4CPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               default:
               {
                  break;
               }
            }
            inToken = TRUE;
         }

         cmMemset((U8*)word, 0, QOSM_MAX_CFG_PARAM_LEN);

      }


   } /* End of for loop */

   if(fp) osFclose(fp);

   RETVOID;

} /* end of qoSmInitCfgCb */

/*
*
*       Fun:   qoSmMain
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  qosm.c
*
*/


#ifdef ANSI
PUBLIC Void qoSmMain
(
Void
)
#else
PUBLIC Void qoSmMain()
#endif /* ANSI */
{
   QoSmCneEvent smEvent;

   SM_TRC2 (qoSmMain);

   /* Read the configuration file */
   qoSmCb.state = QO_SM_STATE_INIT;
   smEvent = EVTQOSMINIT;

   qoSgwSM(smEvent);
   RETVOID;
}

#ifdef QO_SGW
/*
*
*       Fun:   
*
*       Desc:  This function is used by  Layer to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  qosm_hicfg.c
*
*/
#ifdef ANSI
PUBLIC S16 qoSgwSendMsg
(
QoSmCneEvent  event
)
#else
PUBLIC S16 qoSgwSendMsg(event)
QoSmCneEvent  event;
#endif
{
   SM_TRC2(qoSgwSendMsg);
   qoSgwSM (event);

   RETVALUE(ROK);
} /* end of qoSgwSendMsg */
/*
*
*       Fun:   qoSgwCfgInit
*
*       Desc:  This function sends the configuration for all the
*              SGW  layers
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  qosm.c
*
*/

#ifdef ANSI
PRIVATE Void qoSgwCfgInit
(
QoSmCneEvent  event
)
#else
PRIVATE Void qoSgwCfgInit(event)
QoSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2(qoSgwCfgInit);
   switch (qoSmCb.state)
   {
      case QO_SM_STATE_INIT:
      {
         qoSgwHdlInitEvent(event);
         break;
      }
      case QO_SM_STATE_TUCL_CFG:
      {
         qoSmCb.state = QO_SM_STATE_SGW_CFG;
         qoSgwHdlSgwCfgEvent(event);
         /*qoMmeHdlTuclCfgEvent(event);*/
         break;
      }
      case QO_SM_STATE_EGTP_CFG:
      {
         /*qoMmeHdlTuclCfgEvent(event);*/
         break;
      }
      case QO_SM_STATE_SGW_CFG:
      {
         break;
      }
      default:
      {
         QOSM_DBG_ERROR((QOSM_PRNTBUF, "Invalid State(%d)...\n",qoSmCb.state));
         break;
      }
   }
   RETVOID;
}

/*
*
*       Fun:   qoSgwCntrlInit
*
*       Desc:  This function sends the control request for all the
*              SGW application
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  qosm.c
*
*/


#ifdef ANSI
PRIVATE Void qoSgwCntrlInit
(
QoSmCneEvent  event
)
#else
PRIVATE Void qoSgwCntrlInit(event)
QoSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2(qoSgwCntrlInit);
   switch (event)
   {
      case EVTQOSMINIT:
      {
         break;
      }
      case EVTQOTUCLCFGDONE:
      {
         break;
      }
      case EVTQOEGTPCFGDONE:
      {
         break;
      }
      case EVTQOSGWCFGDONE:
      {
         qoSgwEgLSapBndReq();
         break;
      }
      case EVTQOEGTPSERVEROPEN:
      {
         qoSgwEgtSapBndReq();
         break;
      }
      case EVTQOSGWBINDCFM:
      {
         /* End of SGW Config and Control. Trigger state machine for
         PDN-GW Configuration */
         QOSM_DBG_INFO((QOSM_PRNTBUF, "SGW is bound with EGTP\n"));
         QOSM_DBG_INFO ((QOSM_PRNTBUF, "SGW Configured successfully \n"));
         qoSgwDbgCntrlReq();
         qoSmCb.state = QO_SM_STATE_INIT;
         break;
      }
      default:
         QOSM_DBG_ERROR((QOSM_PRNTBUF, "Invalid event(%d)...\n",event));
         break;
   }
   RETVOID;
}

/*
*
*       Fun:   qoSgwSM
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  qosm.c
*
*/


#ifdef ANSI
PRIVATE Void qoSgwSM
(
QoSmCneEvent  event
)
#else
PRIVATE Void qoSgwSM(event)
QoSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (qoSgwSM);

   switch (qoSmCb.state)
   {
      case QO_SM_STATE_INIT:
      case QO_SM_STATE_TUCL_CFG:
      case QO_SM_STATE_EGTP_CFG:
      case QO_SM_STATE_SGW_CFG:
      {
         qoSgwCfgInit(event);
         break;
      }
      case QO_SM_STATE_SGW_CNTRL:
      {
         qoSgwCntrlInit(event);
         break;
      }
      default:
      {
         QOSM_DBG_ERROR((QOSM_PRNTBUF, "Invalid state(%d)\n",qoSmCb.state));
         break;
      }

   }
   RETVOID;
}
/*
*
*       Fun:   qoSgwHdlInitEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  qosm.c
*
*/


#ifdef ANSI
PRIVATE Void qoSgwHdlInitEvent
(
   QoSmCneEvent  event
)
#else
PRIVATE Void qoSgwHdlInitEvent(event)
QoSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (qoSgwHdlInitEvent);

   switch (event)
   {
      case EVTQOSMINIT:
      {
         qoSmCb.state = QO_SM_STATE_SGW_CFG;
         qoSmInitCb();
         qoSgwHiCfg();
         break;
      }
      case EVTQOTUCLCFGDONE:
      {
         QOSM_DBG_INFO((QOSM_PRNTBUF, "TUCL Configuration complete: \
                  Triggering SCTP Configuration...\n"));
         qoSmCb.state = QO_SM_STATE_TUCL_CFG;
         qoSgwEgCfg();
         break;
      }
      case EVTQOEGTPCFGDONE:
      {
         break;
      }
      case EVTQOSGWBINDCFM:
      {
         break;
      }
      case EVTQOEGTPSERVEROPEN:
      {
         break;
      }
      default:
      {
         QOSM_DBG_ERROR((QOSM_PRNTBUF, "Invalid Event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   qoSgwHdlSgwCfgEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  qosm.c
*
*/


#ifdef ANSI
PRIVATE Void qoSgwHdlSgwCfgEvent
(
QoSmCneEvent  event
)
#else
PRIVATE Void qoSgwHdlSgwCfgEvent(event)
QoSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (qoSgwHdlSgwCfgEvent);

   switch (event)
   {
      case EVTQOSMINIT:
      {
         break;
      }
      case EVTQOTUCLCFGDONE:
      {
         break;
      }
      case EVTQOEGTPCFGDONE:
      {
         qoSmCb.state = QO_SM_STATE_SGW_CNTRL;
         qoSgwLqoCfg();
         break;
      }
      default:
      {
         QOSM_DBG_ERROR((QOSM_PRNTBUF, "Invalid Event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}
#endif/* End of QO_SGW */


/**********************************************************************
  
         End of file:     qosm.c@@/main/1 - Thu Jul  8 21:22:21 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       sp                1. LTE-CNE:SGW Initial Release.
/main/1  qo003.101     ve                1. HI .[xh] include files removed.
                       ms                1. config file checking and
                                         2. debug mask correction.
*********************************************************************91*/

