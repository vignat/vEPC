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

     Name:    Trillium LTE CNE - Stack Manager

     Type:    C Source file

     Desc:    This file contains functions required by Stack Manager     

     File:    jgsm.c

     Sid:      avsm.c@@/main/1 - Tue Aug 17 18:54:35 2010

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
#endif

#include "lav.h"
#include "av.h"
#include "avsm.h"
#include <stdlib.h>     /* av004.101: ccpu00117528  exit warning */

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
#endif

#include "lav.x"
#include "avsm.x"

AvSmCb avSmCb;

/* Tokens used in configuration file */
PRIVATE S8 avSmCfgParams[][AVSM_MAX_CFG_PARAM_LEN] =
{
   "AVSM_EG_C_DFLT_PORT",
   "AVSM_EG_C_NONDFLT_PORT",
   "AVSM_EG_U_DFLT_PORT",
   "AVSM_EG_DFLT_HOST_NAME",
   "AVSM_PGW_DFLT_ADDR",
   "AVSM_PGW_DFLT_EXGW_ADDR",
   "AVSM_PGW_UE_START_ADDR",
   "AVSM_PGW_UE_NUM",
   "AVSM_PGW_IP_NUM",
   "AVSM_SGW_IPADDR",
   "AVSM_SGW_PORT",
   "AVSM_PGW_DBG_MASK",
   "AVSM_DBG_MASK",
   "AVSM_PCSCF_ADDR"
};

PRIVATE Void avSmEvtHdlr ARGS((AvSmCneEvent  event));

PRIVATE Void avSmCfgInit ARGS((AvSmCneEvent  event));

PRIVATE Void avSmCntrlInit ARGS((AvSmCneEvent  event));


#ifdef ANSI
PUBLIC Void avSmInitCb
(
Void
)
#else
PUBLIC Void avSmInitCb()
#endif /* ANSI */
{

   /* used at SM---->HI */
   avSmCb.hiPst.dstProcId = SFndProcId();
   avSmCb.hiPst.srcProcId = SFndProcId();

   avSmCb.hiPst.dstEnt    = (Ent)ENTHI;
   avSmCb.hiPst.dstInst   = (Inst)0;
   avSmCb.hiPst.srcEnt    = (Ent)ENTSM;
   avSmCb.hiPst.srcInst   = (Inst)0;
   avSmCb.hiPst.prior     = (Prior)AVSM_MSGPRIOR;
   avSmCb.hiPst.route     = (Route)RTESPEC;
   avSmCb.hiPst.event     = (Event)EVTNONE;
   avSmCb.hiPst.region    = (Region)avSmCb.init.region;
   avSmCb.hiPst.pool      = (Pool)avSmCb.init.region;
   avSmCb.hiPst.selector  = (Selector)AVSM_SMHISEL;


   /* used at SM---->AV */
   avSmCb.avPgwPst.dstProcId = SFndProcId();
   avSmCb.avPgwPst.srcProcId = SFndProcId();
   avSmCb.avPgwPst.dstEnt    = (Ent)ENTEU;
   avSmCb.avPgwPst.dstInst   = (Inst)0;
   avSmCb.avPgwPst.srcEnt    = (Ent)ENTSM;
   avSmCb.avPgwPst.srcInst   = (Inst)0;
   avSmCb.avPgwPst.prior     = (Prior)AVSM_MSGPRIOR;
   avSmCb.avPgwPst.route     = (Route)RTESPEC;
   avSmCb.avPgwPst.event     = (Event)EVTNONE;
   avSmCb.avPgwPst.region    = (Region)avSmCb.init.region;
   avSmCb.avPgwPst.pool      = (Pool)avSmCb.init.pool;
   avSmCb.avPgwPst.selector  = (Selector)AVSM_SMAVSEL;

   /* used at SM---->EG */
   avSmCb.egPst.dstProcId = SFndProcId();
   avSmCb.egPst.srcProcId = SFndProcId();
   avSmCb.egPst.dstEnt    = (Ent)ENTEG;
   avSmCb.egPst.dstInst   = (Inst)0;
   avSmCb.egPst.srcEnt    = (Ent)ENTSM;
   avSmCb.egPst.srcInst   = (Inst)0;
   avSmCb.egPst.prior     = (Prior)AVSM_MSGPRIOR;
   avSmCb.egPst.route     = (Route)RTESPEC;
   avSmCb.egPst.event     = (Event)EVTNONE;
   avSmCb.egPst.region    = (Region)avSmCb.init.region;
   avSmCb.egPst.pool      = (Pool)avSmCb.init.pool;
   avSmCb.egPst.selector  = (Selector)AVSM_SMEGSEL;  

   /* Read the configuration file and initialize the CfgCb */
   avSmInitCfgCb(); 

   RETVOID;
} /* end of avSmInitCb() */


/*
*
*       Fun:   avSmInitCfgCb
*
*       Desc:  Read configuration file and populate the CfgCb.
*
*       Ret:   ROK, RFAILED
*
*       File:  avsm.c
*
*/


#ifdef ANSI
PUBLIC Void avSmInitCfgCb
(
)
#else
PUBLIC Void avSmInitCfgCb (Void)
#endif
{

   S8               line[AVSM_MAX_CFG_FILE_LINE_LEN];
   OsFile           *fp = NULLP;
   S8               *ret1 = NULLP;
   U32              idx = 0;
   S32              lineNo = 0;
   U8               numParams;
   S8               *lPtr;
   S8               word[AVSM_MAX_CFG_PARAM_LEN];
   U8               lineLen;
   Bool             inToken = TRUE;
   AvSmUsrCfgCb     *cfgCb = &avSmCb.cfgCb;

#ifdef DEBUGP
   U8              mskIdx = 0;
   U32             dbgMask = 0;
   U8              noOfDigits = 4; /* equal to the number of masks to be set */
#endif 

   SM_TRC2(avSmInitCfgCb);

   if((fp = (OsFile*)osFopen("avsm_cfg.txt","r")) == (S32)NULLP)
   {
      /* av004.101: ccpu00117528 config file checking */
      AVSM_LOGERROR(ERRCLS_DEBUG, 0, ERRZERO,
            "FAILED to open the file avsm_cfg.txt\n");

      exit(0); 

   } /* if */

   numParams = sizeof(avSmCfgParams)/AVSM_MAX_CFG_PARAM_LEN;

   lineNo = 1;

   for (;;)
   {
      cmMemset((U8*)line, 0, AVSM_MAX_CFG_FILE_LINE_LEN);

      ret1 = osFgets(line, AVSM_MAX_CFG_FILE_LINE_LEN, fp);

      if (ret1 == (S8*)-1 || ret1 == NULLD) /* eof or NULL */
      {
         AVSM_DBG_INFO((AVSM_PRNTBUF, "End of File reached, avsm_cfg.txt\n"));
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
               if(!osStrcmp(word, avSmCfgParams[idx]))
               {
                  inToken = FALSE;
                  break;
               }
            }
            if (idx == numParams)
            {
               AVSM_DBG_ERROR((AVSM_PRNTBUF, " invalid configuration parameter \n"));
               if(fp) osFclose(fp);
               RETVOID;
            }
         }
         else
         {
            /* Now based on the index store the configuration values */
            switch(idx)
            {
               case 0: /* AVSM_EG_C_DFLT_PORT */
               {
                  cfgCb->egCDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 1: /* AVSM_EG_C_NONDFLT_PORT */
               {
                  cfgCb->egCNonDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 2: /* AVSM_EG_U_DFLT_PORT */
               {
                  cfgCb->egUDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 3: /* AVSM_EG_DFLT_HOST_NAME */
               {
                  osStrcpy(cfgCb->egDfltHstNm, word);
                  break;
               }
               case 4: /* AVSM_PGW_DFLT_ADDR */
               {
                  osStrcpy(cfgCb->egDfltAddr, word);
                  osStrcpy(cfgCb->pgwDfltIpAddr, word);
                  break;
               }
               case 5: /* AVSM_PGW_DFLT_EXGW_ADDR */
               {
                  osStrcpy(cfgCb->pgwDfltExgwIpAddr, word);
                  break;
               }
               case 6: /* AVSM_PGW_UE_START_ADDR */
               {
                  cmInetAddr(word, &cfgCb->pgwUeStartAddr);
                  cfgCb->pgwUeStartAddr = CM_INET_NTOH_U32(cfgCb->pgwUeStartAddr);
                  break;
               }
               case 7: /* AVSM_PGW_UE_NUM */
               {
                  cfgCb->pgwUeNum = (U8)osStrtol(word, NULL, 10);
                  break;
               }
                 /* av004.101 Reading number of IPs configured at uesim */
               case 8: /* AVSM_PGW_IP_NUM */
               {
                  cfgCb->pgwIpNum = (U8)osStrtol(word, NULL, 10);
                  break;
               }
               case 9: /* AVSM_SGW_IPADDR */
               {
                  cmInetAddr(word, &cfgCb->sgwIpAddr);
                  cfgCb->sgwIpAddr = CM_INET_NTOH_U32(cfgCb->sgwIpAddr);
                  break;
               }
               case 10: /* AVSM_SGW_PORT */
               {
                  cfgCb->sgwPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
#ifdef DEBUGP
               case 11: /* AVSM_PGW_DBG_MASK */
               {
                  dbgMask = (U32)osStrtol(word, NULL, 10);
                  /* Initialize the debug mask */
                  cfgCb->dbgMask = 0;
                  mskIdx = 0;
                  
                  /* Apply debug mask as per the digits set in the configuration */
                  /* noOfDigits = 4; equal to the number of masks to be set */
                  for(noOfDigits = 4; noOfDigits > 0; noOfDigits--)
                  {
                     /* extracting each digit from the number
                      * if number is set to 1 then apply debug mask */
                     if(dbgMask % 10)
                     {
                        cfgCb->dbgMask |= (DBGMASK_LYR << mskIdx);
                     }
                     /* divide to extract the next digit */
                     dbgMask = dbgMask / 10;
                     mskIdx++;
                  }
                  break;
               }
               case 12: /* AVSM_DBG_MASK */
               {
                  dbgMask = (U32)osStrtol(word, NULL, 10);
                  /* Initialize the debug mask */
                  avSmCb.init.dbgMask = 0;
                  mskIdx = 0;
                  
                  /* Apply debug mask as per the digits set in the configuration */
                  /* noOfDigits = 4; equal to the number of masks to be set */
                  for(noOfDigits = 4; noOfDigits > 0; noOfDigits--)
                  {
                     /* extracting each digit from the number
                      * if number is set to 1 then apply debug mask */
                     if(dbgMask % 10)
                     {
                         avSmCb.init.dbgMask |= (DBGMASK_LYR << mskIdx);
                     }
                     /* divide to extract the next digit */
                     dbgMask = dbgMask / 10;
                     mskIdx++;
                  }
                  break;
               }
#endif 
               case 14:  /*AVSM_PCSCF_ADDR*/
                  cmInetAddr(word, &cfgCb->pcscfAddr);
                  cfgCb->pcscfAddr = CM_INET_NTOH_U32(cfgCb->pcscfAddr);
                 break; 
               default:
               {
                  break;
               }
            }
            inToken = TRUE;
         }

         cmMemset((U8*)word, 0, AVSM_MAX_CFG_PARAM_LEN);

      }


   } /* End of for loop */

   if(fp) osFclose(fp);

   RETVOID;

} /* end of avSmInitCfgCb */

/*
*
*       Fun:   avSmMain
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  avsm.c
*
*/


#ifdef ANSI
PUBLIC Void avSmMain
(
Void
)
#else
PUBLIC Void avSmMain()
#endif /* ANSI */
{
   AvSmCneEvent smEvent;

   SM_TRC2 (avSmMain);

   /* Read the configuration file */
   avSmCb.state = AV_SM_STATE_INIT;
   smEvent = EVTAVSMINIT;

   avSmEvtHdlr(smEvent);
   RETVOID;
}


/*
*
*       Fun:   avSmCfgInit
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  avsm.c
*
*/


#ifdef ANSI
PRIVATE Void avSmCfgInit
(
AvSmCneEvent  event
)
#else
PRIVATE Void avSmCfgInit(event)
AvSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (avSmCfgInit);

   switch (event)
   {
      case EVTAVSMINIT:
      {
         avSmCb.state = AV_SM_STATE_TUCL_CFG;
         avSmInitCb();
         avPgwHiCfg();
         break;
      }
      case EVTAVEGTPCCFGDONE:
      {
         avSmCb.state = AV_SM_STATE_EGTPC_CFG;
         avPgwEgECfg();
         break;
      }
      case EVTAVTUCLCFGDONE:
      {
         avSmCb.state = AV_SM_STATE_EGTPC_CFG;
         avPgwEgCCfg();
         break;
      }
      case EVTAVEGTPECHOCFGDONE:
      {
         avSmCb.state = AV_SM_STATE_EGTPU_CFG;
         avPgwEgUCfg();
         break;
      }
      case EVTAVEGTPUCFGDONE:
      {
         avSmCb.state = AV_SM_STATE_PGW_CFG;
         avPgwLavCfg();
         break;
      }
      default:
      {
         AVSM_DBG_ERROR((AVSM_PRNTBUF, "Invalid Event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   avSmCntrlInit
*
*       Desc:  This function sends the control request for all the
*              PGW application
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  avsm.c
*
*/


#ifdef ANSI
PRIVATE Void avSmCntrlInit
(
AvSmCneEvent  event
)
#else
PRIVATE Void avSmCntrlInit(event)
AvSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2(avSmCntrlInit);
   switch (event)
   {
      case EVTAVPGWCFGDONE:
      {
         avPgwEgLSapBndReq();
         break;
      }
      case EVTAVTPTSRVROPEN:
      {
         avPgwDbgMaskCntrlReq();
         avPgwEgtSapBndReq(STAVEGTPCSAP);
         break;
      }
      case EVTAVGTPCBINDCFM:
      {
         avPgwEgtSapBndReq(STAVEGTPUSAP);
         break;
      }
      case EVTAVGTPUBINDCFM:
      {
         avPgwEgtSapBndReq(STAVECHOSAP);
         break;
      }
      case EVTAVECHOBINDCFM:
      {
         avPgwSendArpReq();
         break;
      }
      case EVTAVPGWROUTECFM:
      {
         /* End of P-GW Control Confirmation */
         AVSM_DBG_INFO((AVSM_PRNTBUF, "PDN-GW configured successfully"));
         avSmCb.state = AV_SM_STATE_INIT;
         break;
      }
      default:
      {
         AVSM_DBG_ERROR((AVSM_PRNTBUF, "Invalid Event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}


/*
*
*       Fun:   avSmEvtHdlr
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  avsm.c
*
*/


#ifdef ANSI
PRIVATE Void avSmEvtHdlr
(
AvSmCneEvent  event
)
#else
PRIVATE Void avSmEvtHdlr( event)
AvSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (avSmEvtHdlr);

   switch (avSmCb.state)
   {
      case AV_SM_STATE_INIT:
      case AV_SM_STATE_TUCL_CFG:
      case AV_SM_STATE_EGTPC_CFG:
      case AV_SM_STATE_EGTPU_CFG:
      {
         avSmCfgInit(event);
         break;
      }
      case AV_SM_STATE_PGW_CFG:
      case AV_SM_STATE_EGTPC_CNTRL:
      case AV_SM_STATE_EGTPU_CNTRL:
      case AV_SM_STATE_PGW_CNTRL:
      {
         avSmCntrlInit(event);
         break;
      }
      default:
      {
         AVSM_DBG_ERROR((AVSM_PRNTBUF,"Invalid State(%d)\n",avSmCb.state));
         break;
      }

   }
   RETVOID;
}


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
*       File:  avsm.c
*
*/
#ifdef ANSI
PUBLIC S16 avPgwSendMsg
(
AvSmCneEvent  event
)
#else
PUBLIC S16 avPgwSendMsg(event)
AvSmCneEvent  event;
#endif
{
   SM_TRC2(avPgwSendMsg);
   avSmEvtHdlr (event);

   RETVALUE(ROK);
} /* end of avPgwSendMsg */


/**********************************************************************
  
         End of file:     avsm.c@@/main/1 - Tue Aug 17 18:54:35 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp                 1. LTE-CNE:PGW Initial Release.
/main/1     av004.101 ms                 1. config file check.
                      pmacharla          1. Multi UE support added
*********************************************************************91*/

