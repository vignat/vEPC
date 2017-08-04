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

     Name:    Trillium LTE CNE - Stack Manager

     Type:    C Source file

     Desc:    

     File:    vbsm.c

     Sid:      vbsm.c@@/main/1 - Mon Jun 28 19:35:40 2010

     Prg:     
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
#include "sct.h"           /* SB (SCTP) layer */
#include "lsz.h"           /* layer management defines for S1AP */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
/* #include "hi.h" */
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */

/* #include "sz.h"  */        /* defines and macros for S1AP */
#include "szt.h"         /* defines and macros for S1AP */
#include "lvb.h"
#include "vb.h"
#include "cm_esm.h"
#include "szt_asn.h"       
#include "cm_emm.h"
#include "vb_hss.h"
#include "vbsm.h"
#include <stdlib.h> /* vb005.101 : ccpu00117528 exit warning */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_tkns.x"       /* common tokens */
/* vb007.101 s1ap include files order has changed */
#include "cm_mblk.x"       /* memory management */
#include "cm_llist.x"      /* common linked list */
#include "szt_asn.x"       
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"         /* common DNS libraru defines */
#include "cm_os.x"         /* common OS functionality */

#include "leg.x"
#include "egt.x"
#include "sct.x"           /* SCTP Interface Structures */
#include "szt.x"           /* SCTP Interface Structures */
#include "lsz.x"           /* S1AP types */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
/* #include "hi.x" */
#endif

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

/*#include "sz.x"  */          /* typedefs for S1AP */
#include "lvb.x"
#include "vbsm.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

VbSmCb vbSmCb;

/* Tokens used in configuration file */
PRIVATE S8 vbSmCfgParams[][VBSM_MAX_CFG_PARAM_LEN] =
{
   "VBSM_EG_DFLT_PORT", /* Shall be 2123 by default for EGTP-C */
   "VBSM_EG_NONDFLT_PORT", /* Shall be other than 2123 for EGTP-C */
   "VBSM_EG_DFLT_HOST_NAME",
   "VBSM_ENB_ADDR_1",
   "VBSM_ENB_PORT_1",
   "VBSM_MME_IPADDR",
   "VBSM_MME_SCTP_PORT",
   "VBSM_SGW_IPADDR",
   "VBSM_PGW_IPADDR",
   "VBSM_UE_NUM",
   "VBSM_SCTP_UDP_SERV_TYPE",
   "VBSM_MME_DBG_MASK",
   "VBSM_DBG_MASK",
   "VBSM_NW_INIATED_DETACH_TIMER",
   "VBSM_MCC_DIG1",
   "VBSM_MCC_DIG2",
   "VBSM_MCC_DIG3",
   "VBSM_MNC_DIG1",
   "VBSM_MNC_DIG2",
   "VBSM_MNC_DIG3",
   "VBSM_TARGET_MME_PLMN_ID",
   "VBSM_TARGET_MME_IP_ADDR",
   "VBSM_T3412",
   "VBSM_EG_S10_NONDFLT_PORT",
   "VBSM_ENB_ADDR_2",
   "VBSM_ENB_PORT_2",
   "VBSM_SGSN_IP_ADDR",
   "VBSM_EG_S3_NONDFLT_PORT",
   "VBSM_SGSN_PLMN_ID"

};
/* Function declarations */

PRIVATE Void vbSmInitCfgCb ARGS ((Void));

PRIVATE Void vbMmeSM ARGS((VbSmCneEvent event));

PRIVATE Void vbMmeHdlInitEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeCfgInit ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeCntrlInit ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlTuclCfgEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlSctpCfgEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlS1apCfgEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlEgtpCfgEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlMmeCfgEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlSctpBoundEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlS1apBoundEvent ARGS((VbSmCneEvent  event));

PRIVATE Void vbMmeHdlEgtpBoundEvent ARGS((VbSmCneEvent  event));



#ifdef ANSI
PUBLIC Void vbSmInitCb
(
Void
)
#else
PUBLIC Void vbSmInitCb()
#endif /* ANSI */
{

   cmMemset((U8 *)&vbSmCb, 0, sizeof(VbSmCb));

   /* Used at SM---->HI */
   vbSmCb.hiPst.dstProcId = SFndProcId();
   vbSmCb.hiPst.srcProcId = SFndProcId();
   vbSmCb.hiPst.dstEnt    = (Ent)ENTHI;
   vbSmCb.hiPst.dstInst   = (Inst)0;
   vbSmCb.hiPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.hiPst.srcInst   = (Inst)0;
   vbSmCb.hiPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.hiPst.route     = (Route)RTESPEC;
   vbSmCb.hiPst.event     = (Event)EVTNONE;
   vbSmCb.hiPst.region    = (Region)vbSmCb.init.region;
   vbSmCb.hiPst.pool      = (Pool)vbSmCb.init.region;
   vbSmCb.hiPst.selector  = (Selector)VBSM_SMHISEL;

   /* Used at SM---->SB */
   vbSmCb.sbPst.dstProcId = SFndProcId();
   vbSmCb.sbPst.srcProcId = SFndProcId();
   vbSmCb.sbPst.dstEnt    = (Ent)ENTSB;
   vbSmCb.sbPst.dstInst   = (Inst)0;
   vbSmCb.sbPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.sbPst.srcInst   = (Inst)0;
   vbSmCb.sbPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.sbPst.route     = (Route)RTESPEC;
   vbSmCb.sbPst.event     = (Event)EVTNONE;
   vbSmCb.sbPst.region    = (Region)vbSmCb.init.region;
   vbSmCb.sbPst.pool      = (Pool)vbSmCb.init.pool;
   vbSmCb.sbPst.selector  = (Selector)VBSM_SMSBSEL;

   /* Used at SM---->SZ */
   vbSmCb.szPst.dstProcId = SFndProcId();
   vbSmCb.szPst.srcProcId = SFndProcId();
   vbSmCb.szPst.dstEnt    = (Ent)ENTSZ;
   vbSmCb.szPst.dstInst   = (Inst)0;
   vbSmCb.szPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.szPst.srcInst   = (Inst)0;
   vbSmCb.szPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.szPst.route     = (Route)RTESPEC;
   vbSmCb.szPst.event     = (Event)EVTNONE;
   vbSmCb.szPst.region    = (Region)vbSmCb.init.region;
   vbSmCb.szPst.pool      = (Pool)vbSmCb.init.pool;
   vbSmCb.szPst.selector  = (Selector)VBSM_SMSZSEL;

   /* Used at SM---->EG */
   vbSmCb.egPst.dstProcId = SFndProcId();
   vbSmCb.egPst.srcProcId = SFndProcId();
   vbSmCb.egPst.dstEnt    = (Ent)ENTEG;
   vbSmCb.egPst.dstInst   = (Inst)0;
   vbSmCb.egPst.srcEnt    = (Ent)ENTSM;
   vbSmCb.egPst.srcInst   = (Inst)0;
   vbSmCb.egPst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.egPst.route     = (Route)RTESPEC;
   vbSmCb.egPst.event     = (Event)EVTNONE;
   vbSmCb.egPst.region    = (Region)vbSmCb.init.region;
   vbSmCb.egPst.pool      = (Pool)vbSmCb.init.pool;
   vbSmCb.egPst.selector  = (Selector)VBSM_SMEGSEL;  

   /* used at SM---->UZ */
   vbSmCb.vbMmePst.dstProcId = SFndProcId();
   vbSmCb.vbMmePst.srcProcId = SFndProcId();
   vbSmCb.vbMmePst.dstEnt    = (Ent)ENTUZ;
   vbSmCb.vbMmePst.dstInst   = (Inst)0;
   vbSmCb.vbMmePst.srcEnt    = (Ent)ENTSM;
   vbSmCb.vbMmePst.srcInst   = (Inst)0;
   vbSmCb.vbMmePst.prior     = (Prior)VBSM_MSGPRIOR;
   vbSmCb.vbMmePst.route     = (Route)RTESPEC;
   vbSmCb.vbMmePst.event     = (Event)EVTNONE;
   vbSmCb.vbMmePst.region    = (Region)vbSmCb.init.region;
   vbSmCb.vbMmePst.pool      = (Pool)vbSmCb.init.pool;
   vbSmCb.vbMmePst.selector  = (Selector)VBSM_SMVBSEL;

   /* Read the configuration file and initialize the CfgCb */
   vbSmInitCfgCb();

   RETVOID;
} /* end of vbSmInitCb() */


/*
*
*       Fun:   vbSmInitCfgCb
*
*       Desc:  Read configuration file and populate the CfgCb.
*
*       Ret:   ROK, RFAILED
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbSmInitCfgCb
(
)
#else
PRIVATE Void vbSmInitCfgCb (Void)
#endif
{

   S8               line[VBSM_MAX_CFG_FILE_LINE_LEN];
   OsFile           *fp = NULLP;
   S8               *ret1 = NULLP;
   U32              idx = 0;
   S32              lineNo = 0;
   U8               numParams;
   S8               *lPtr;
   S8               word[VBSM_MAX_CFG_PARAM_LEN];
   U8               lineLen;
   Bool             inToken = TRUE;
   VbSmUsrCfgCb     *cfgCb = &vbSmCb.cfgCb;

#ifdef DEBUGP
   U8               mskIdx = 0;
   U32              dbgMask = 0;
   U8              noOfDigits = 4; /* equal to the number of masks to be set */
#endif 
 

   SM_TRC2(vbSmInitCfgCb);

   if((fp = (OsFile*)osFopen((CONST S8 *)"vbsm_cfg.txt",(CONST S8 *)"r")) == \
      (S32)NULLP)
   {
      /* vb005.101 : ccpu00117528 configuration file check */
      VBLOGERROR(ERRCLS_DEBUG,0,ERRZERO,
            "FAILED to open the file vbsm_cfg.txt\n ");
      exit(0);

   } /* if */

   numParams = sizeof(vbSmCfgParams)/VBSM_MAX_CFG_PARAM_LEN;

   lineNo = 1;

   for (;;)
   {
      cmMemset((U8*)line, 0, VBSM_MAX_CFG_FILE_LINE_LEN);

      ret1 = osFgets(line, VBSM_MAX_CFG_FILE_LINE_LEN, fp);

      if (ret1 == (S8*)-1 || ret1 == NULLD) /* eof or NULL */
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "End of File reached, vbsm_cfg.txt\n"));
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
               if(!osStrcmp(word, vbSmCfgParams[idx]))
               {
                  inToken = FALSE;
                  break;
               }
            }
            if (idx == numParams)
            {
               VBSM_DBG_ERROR((VBSM_PRNTBUF, " invalid configuration "\
                     "parameter \n"));
               if(fp) osFclose(fp);
               RETVOID;
            }
         }
         else
         {
            /* Now based on the index store the configuration values */
            switch(idx)
            {
               case 0: /* VBSM_EG_DFLT_PORT */
               {
                  cfgCb->egDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 1: /* VBSM_EG_NONDFLT_PORT */
               {
                  cfgCb->egNonDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 2: /* VBSM_EG_DFLT_HOST_NAME */
               {
                  osStrcpy(cfgCb->egDfltHstNm, word);
                  break;
               }
               case 3: /* VBSM_ENB_ADDR_1 */
               {
                  osStrcpy(cfgCb->eNbAddr, word);
                  cmInetAddr(word, &cfgCb->eNbIpAddr);
                  cfgCb->eNbIpAddr = CM_INET_NTOH_U32(cfgCb->eNbIpAddr);
                  break;
               }
               case 4: /* VBSM_ENB_PORT_1 */
               {
                  cfgCb->eNbPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 5: /* VBSM_MME_IPADDR */
               {
                  cmInetAddr(word, &cfgCb->mmeIpAddr);
                  cfgCb->mmeIpAddr = CM_INET_NTOH_U32(cfgCb->mmeIpAddr);
                  break;
               }
               case 6: /* VBSM_MME_SCTP_PORT */
               {
                  cfgCb->mmeSctPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 7: /* VBSM_SGW_IPADDR */
               {
                  cmInetAddr(word, &cfgCb->sgwIpAddr);
                  cfgCb->sgwIpAddr = CM_INET_NTOH_U32(cfgCb->sgwIpAddr);
                  break;
               }
               case 8: /* VBSM_PGW_IPADDR */
               {
                  cmInetAddr(word, &cfgCb->pgwIpAddr);
                  cfgCb->pgwIpAddr = CM_INET_NTOH_U32(cfgCb->pgwIpAddr);
                  break;
               }
               case 9: /* VBSM_UE_NUM */
               {
                  cfgCb->ueNum = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               case 10: /* vb005.101 : ccpu00117528  VBSM_SCTP_UDP_SERV_TYPE */
               {
                  cfgCb->sctpUdpServiceType = (U8)osStrtol(word, NULL, 10);
                  break;
               }

#ifdef DEBUGP
               case 11: /* VBSM_MME_DBG_MASK */
               {
                  dbgMask = (U32)osStrtol(word, NULL, 10);
                  /* Initialize the debug mask */
                  cfgCb->mmeDbgMask = 0;
                  mskIdx = 0;
                  
                  /* Apply debug mask as per the digits set in  configuration */
                  /* noOfDigits = 4; equal to the number of masks to be set */
                  for(noOfDigits = 4; noOfDigits > 0; noOfDigits--)
                  {
                     /* extracting each digit from the number
                      * if number is set to 1 then apply debug mask */
                     if(dbgMask % 10)
                     {
                        cfgCb->mmeDbgMask |= (DBGMASK_LYR << mskIdx);
                     }
                     /* divide to extract the next digit */
                     dbgMask = dbgMask / 10;
                     mskIdx++;
                  }
                  break;
               }
               case 12: /* VBSM_DBG_MASK */
               {
                  dbgMask = (U32)osStrtol(word, NULL, 10);
                  /* Initialize the debug mask */
                  vbSmCb.init.dbgMask = 0;
                  mskIdx = 0;
                  
                  /* Apply debug mask as per the digits set in configuration */
                  /* noOfDigits = 4; equal to the number of masks to be set */
                  for(noOfDigits = 4; noOfDigits > 0; noOfDigits--)
                  {
                     /* extracting each digit from the number
                      * if number is set to 1 then apply debug mask */
                     if(dbgMask % 10)
                     {
                         vbSmCb.init.dbgMask |= (DBGMASK_LYR << mskIdx);
                     }
                     /* divide to extract the next digit */
                     dbgMask = dbgMask / 10;
                     mskIdx++;
                  }
                  break;
               }

#endif 
#ifdef VB_MME_NW_INIT_DETACH
               case 13: /* VBSM_NW_INIATED_DETACH_TIMER */
               {
                  cfgCb->nwInitDetachTmr = (U32)osStrtol(word, NULL, 10);
                  break;
               }
#endif /* end of VB_MME_NW_INIT_DETACH */

               case 14:
               { 
                  cfgCb->mccDig1 = (U8)osStrtol(word, NULL, 16);
                  printf("MCC DIG1:%d", cfgCb->mccDig1);
                  break;
               }
               case 15:
               { 
                  cfgCb->mccDig2 = (U8)osStrtol(word, NULL, 16);
                  printf("MCC DIG2:%d", cfgCb->mccDig2);
                  break;
               }
               case 16:
               { 
                  cfgCb->mccDig3 = (U8)osStrtol(word, NULL, 16);
                  printf("MCC DIG3:%d", cfgCb->mccDig3);
                  break;
               }
               case 17:
               { 
                  cfgCb->mncDig1 = (U8)osStrtol(word, NULL, 16);
                  printf("MNC DIG1:%d", cfgCb->mncDig1);
                  break;
               }
               case 18:
               { 
                  cfgCb->mncDig2 = (U8)osStrtol(word, NULL, 16);
                  printf("MNC DIG2:%d", cfgCb->mncDig2);
                  break;
               }
               case 19:
               { 
                  cfgCb->mncDig3 = (U8)osStrtol(word, NULL, 16);
                  printf("MNC DIG3:%d", cfgCb->mncDig3);
                  break;
               }
               case 20: /* VBSM_TARGET_MME_PLMN_ID */
               { 
                  VBSM_ENC_PLMN_ID(cfgCb->tgtMmePlmnId, word);
                  printf("Target MME PLMN ID: %s", cfgCb->tgtMmePlmnId);
                  break;
               }

               case 21: /* VBSM_TARGET_MME_IP_ADDR */
               { 
                  cmInetAddr(word, &cfgCb->tgtMmeIpAddr);
                  cfgCb->tgtMmeIpAddr = CM_INET_NTOH_U32(cfgCb->tgtMmeIpAddr);
                  break;
               }
               case 22:
               {
                  cfgCb->t3412 = (U32)osStrtol(word, NULL, 10);
                  printf("t3412:%d\n", cfgCb->t3412);
                  break;
               }
               case 23: /* VBSM_EG_S10_NONDFLT_PORT */
               {
                  cfgCb->egS10NonDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }

               case 24: /* VBSM_ENB_ADDR_2 */
               {
                  osStrcpy(cfgCb->eNbAddr_2, word);
                  cmInetAddr(word, &cfgCb->eNbIpAddr_2);
                  cfgCb->eNbIpAddr_2 = CM_INET_NTOH_U32(cfgCb->eNbIpAddr_2);
                  break;
               }

               case 25: /* VBSM_ENB_PORT_2 */
               {
                  cfgCb->eNbPort_2 = (U16)osStrtol(word, NULL, 10);
                  break;
               }
               
               case 26: /* VBSM_SGSN_IP_ADDR */
               { 
                  cmInetAddr(word, &cfgCb->sgsnIpAddr);
                  cfgCb->sgsnIpAddr = CM_INET_NTOH_U32(cfgCb->sgsnIpAddr);
                  break;
               }
               case 27: /* VBSM_EG_S3_NONDFLT_PORT */
               {
                  cfgCb->egS3NonDfltPort = (U16)osStrtol(word, NULL, 10);
                  break;
               }

               case 28: /* VBSM_SGSN_PLMN_ID */
               { 
                  VBSM_ENC_PLMN_ID(cfgCb->sgsnPlmnId, word);
                  printf("SGSN PLMN ID: %s", cfgCb->sgsnPlmnId);
                  break;
               }
               
               default:
               {
                  printf("\n vbSmInitCfgCb, Entered Default state \n");
                  break;
               }
            }

            inToken = TRUE;
         }

         cmMemset((U8*)word, 0, VBSM_MAX_CFG_PARAM_LEN);

      }
   } /* End of for loop */

   if(fp) osFclose(fp);

   RETVOID;

} /* end of vbSmInitCfgCb */

/*
*
*       Fun:   vbSmMain
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PUBLIC Void vbSmMain
(
Void
)
#else
PUBLIC Void vbSmMain()
#endif /* ANSI */
{
   VbSmCneEvent smEvent;

   SM_TRC2 (vbSmMain);

   /* Read the configuration file */
   vbSmCb.state = VB_SM_STATE_INIT;
   smEvent = EVTVBSMINIT;

   vbMmeSM(smEvent);
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlTuclCfgEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/

#ifdef ANSI
PRIVATE Void vbMmeHdlTuclCfgEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlTuclCfgEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlTuclCfgEvent);
   switch (event)
   {
      case EVTVBTUCLCFGDONE:
      {
         break;
      }
      case EVTVBSCTPCFGDONE:
      {
          VBSM_DBG_INFO((VBSM_PRNTBUF,"SCTP Configuration complete: "\
               "Triggering S1AP Configuration...\n"));
          vbSmCb.state = VB_SM_STATE_SCTP_CFG; 
          vbMmeSzCfg();
          break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, " invalid event(%d)\n",event));
         break;
      }
   }
   RETVOID;
}



/*
*
*       Fun:   vbMmeHdlSctpCfgEvent
*
*       Desc:  
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/

#ifdef ANSI
PRIVATE Void vbMmeHdlSctpCfgEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlSctpCfgEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlSctpCfgEvent);

   switch (event)
   {
      case EVTVBSCTPCFGDONE:
      {
         break;
      }
      case EVTVBS1APCFGDONE:
      {
         VBSM_DBG_INFO((VBSM_PRNTBUF,"S1AP Configuration complete: "\
               "Triggering EGTP Configuration...\n"));
         vbSmCb.state = VB_SM_STATE_S1AP_CFG;
         vbMmeEgCfg();
         break;
      }
      case EVTVBMMECFGDONE:
      {
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, " invalid event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlS1apCfgEvent
*
*       Desc:  
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeHdlS1apCfgEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlS1apCfgEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlS1apCfgEvent);

   switch (event)
   {
      case EVTVBEGTPCFGDONE:
      {
         VBSM_DBG_INFO((VBSM_PRNTBUF,"EGTP Configuration complete: "\
               "Triggering MME Configuration..\n"));
         vbSmCb.state = VB_SM_STATE_EGTP_CFG; 
         vbMmeLvbCfg();
         break;
      }
      case EVTVBS1APCFGDONE:
      {
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "invalid event(%d)\n",event));
         break;
      }
   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlEgtpCfgEvent
*
*       Desc:  
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeHdlEgtpCfgEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlEgtpCfgEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlEgtpCfgEvent);

   switch (event)
   {
      case EVTVBMMECFGDONE:
      {
         /* End of MME Configuration; Trigger Control request */
         VBSM_DBG_INFO((VBSM_PRNTBUF,"MME Configuration complete: "\
               "Triggering control operations..\n"));
         vbSmCb.state = VB_SM_STATE_MME_CFG; 
         /* vb006.101: Introduced new flag VB_SB_SCTP for Trillium 
          * SCTP and added support for Kernel SCTP */
#ifdef VB_SB_SCTP
         vbMmeLsbSbTSapCntrlReq(VBSM_SB_SUID);
#else
         vbMmeSzLSapBndReq();
#endif
         break;
      }
      case EVTVBS1APCFGDONE:
      {
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "invalid event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlSctpBoundEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeHdlSctpBoundEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlSctpBoundEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlSctpBoundEvent);

   switch (event)
   {
      case EVTVBSCTPBINDCFM:
      {
         break;
      }
      case EVTVBS1APBINDCFM:
      {
         vbSmCb.state = VB_SM_STATE_S1AP_BOUND;
         vbMmeSztSapBndReq();
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "invalid event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlS1apBoundEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeHdlS1apBoundEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlS1apBoundEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlS1apBoundEvent);

   switch (event)
   {
       case EVTVBSCTPBINDCFM:
      {
         break;
      }
      case EVTVBS1APBINDCFM:
      {
         vbMmeSztSapBndReq();
         break;
      }
      /* End of S1-C stack control. Trigger S11-C control operation */
      case EVTVBAPPBINDCFM: 
      {
         vbSmCb.state = VB_SM_STATE_EGTP_BOUND;
         vbMmeEgLSapBndReq();
         break; 
      }
      case EVTVBEGTPCSERVEROPEN:
      {
         vbMmeEgtSapBndReq();
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "invalid event(%d)\n",event));
         break;
      }
   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlEgtpBoundEvent
*
*       Desc: 
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeHdlEgtpBoundEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlEgtpBoundEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlEgtpBoundEvent);

   switch (event)
   {
      case EVTVBEGTPCSERVEROPEN:
      {
         vbMmeEgtSapBndReq();
         break;
      }
      case EVTVBS11APPBINDCFM:
      {
#ifdef DEBUGP
         /* Trigger cntrl req */
         vbMmeDbgMaskCntrlReq();
#else
         /* End of MME Config and Control */
         VBSM_DBG_INFO((VBSM_PRNTBUF, "MME configured successfully"));
         vbSmCb.state = VB_SM_STATE_INIT;
#endif 
         break;
      }
#ifdef DEBUGP
      case EVTVBMMEDBGCNTRLCFM:
      {
         /* End of MME Config and Control */
         VBSM_DBG_INFO((VBSM_PRNTBUF, "MME configured successfully"));
         vbSmCb.state = VB_SM_STATE_INIT;
         break;
      }
#endif
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "invalid event(%d)\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlInitEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/

#ifdef ANSI
PRIVATE Void vbMmeHdlInitEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlInitEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlInitEvent);

   switch (event)
   {
      case EVTVBSMINIT:
      {
         VBSM_DBG_INFO((VBSM_PRNTBUF, "Starting Configuration of TUCL...\n"));
         vbSmInitCb();
         vbMmeHiCfg();
         break;
      }
      case EVTVBTUCLCFGDONE:
      {
         /* vb006.101: Introduced new flag VB_SB_SCTP for Trillium 
          * SCTP and added support for Kernel SCTP */
#ifdef VB_SB_SCTP
         VBSM_DBG_INFO((VBSM_PRNTBUF, "TUCL Configuration complete: "\
               "Triggering SCTP Configuration...\n"));
         vbSmCb.state = VB_SM_STATE_TUCL_CFG;
         vbMmeSbCfg();
#else
         /* skippeing sctp configuration 
            so passing sctp conf done */
         vbSmCb.state = VB_SM_STATE_SCTP_CFG;
         vbMmeSzCfg();
#endif
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "Invalid Event(%d)...\n",event));
         break;
      }
   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlMmeCfgEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeHdlMmeCfgEvent
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeHdlMmeCfgEvent(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeHdlMmeCfgEvent);

   switch (event)
   {
      case EVTVBMMECFGDONE:
      {
         break;
      }
         /* vb006.101: Introduced new flag VB_SB_SCTP for Trillium 
          * SCTP and added support for Kernel SCTP */
#ifdef VB_SB_SCTP
      case EVTVBSCTPBINDCFM:
      {
         vbSmCb.state = VB_SM_STATE_SCTP_BOUND;
         vbMmeSzLSapBndReq();
         break;
      }
#endif
      case EVTVBS1APBINDCFM:
      {
#ifndef VB_SB_SCTP
         vbSmCb.state = VB_SM_STATE_S1AP_BOUND;
         vbMmeSztSapBndReq();
#endif
         break;
      }
      case EVTVBAPPBINDCFM:
      {
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "Invalid Event(%d)...\n",event));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeCfgInit
*
*       Desc:  This function sends the configuration for all the
*              MME layers
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeCfgInit
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeCfgInit(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeCfgInit);
   switch (vbSmCb.state)
   {
      case VB_SM_STATE_INIT:
      {
         vbMmeHdlInitEvent(event);
         break;
      }
      case VB_SM_STATE_TUCL_CFG:
      {
         vbMmeHdlTuclCfgEvent(event);
         break;
      }
      case VB_SM_STATE_SCTP_CFG:
      {
         vbMmeHdlSctpCfgEvent(event);
         break;
      }
      case VB_SM_STATE_S1AP_CFG:
      {
         vbMmeHdlS1apCfgEvent(event); 
         break;
      }
      case VB_SM_STATE_EGTP_CFG:
      {
         vbMmeHdlEgtpCfgEvent(event); 
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "Invalid State(%d)...\n",vbSmCb.state));
         break;
      }
   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeCntrlInit
*
*       Desc:  This function sends the control request for all the
*              MME layers
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/


#ifdef ANSI
PRIVATE Void vbMmeCntrlInit
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeCntrlInit(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeCntrlInit);
   switch (vbSmCb.state)
   {
       case VB_SM_STATE_MME_CFG:
       {
          vbMmeHdlMmeCfgEvent(event);
          break;
       }
         /* vb006.101: Introduced new flag VB_SB_SCTP for Trillium SCTP */
#ifdef VB_SB_SCTP       
      case VB_SM_STATE_SCTP_BOUND:
      {
         vbMmeHdlSctpBoundEvent(event);
         break;
      }
#endif       
      case VB_SM_STATE_S1AP_BOUND:
      {
         vbMmeHdlS1apBoundEvent(event);
         break;
      }
      case VB_SM_STATE_EGTP_BOUND:
      {
         vbMmeHdlEgtpBoundEvent(event);
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "Invalid State(%d)...\n",vbSmCb.state));
         break;
      }

   }
   RETVOID;
}

/*
*
*       Fun:   vbMmeSM
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm.c
*
*/

#ifdef ANSI
PRIVATE Void vbMmeSM
(
VbSmCneEvent  event
)
#else
PRIVATE Void vbMmeSM(event)
VbSmCneEvent  event;
#endif /* ANSI */
{
   SM_TRC2 (vbMmeSM);

   switch (vbSmCb.state)
   {
      case VB_SM_STATE_INIT:
      case VB_SM_STATE_TUCL_CFG:
      case VB_SM_STATE_SCTP_CFG:
      case VB_SM_STATE_S1AP_CFG: /* End of configuration of S1 stack for MME */
      case VB_SM_STATE_EGTP_CFG: /* End of configuration of S11 stack for MME */ 
      {
         vbMmeCfgInit(event);
         break;
      }
      case VB_SM_STATE_MME_CFG:
      case VB_SM_STATE_SCTP_BOUND:
      case VB_SM_STATE_S1AP_BOUND:/* End of control operation of S1 Stack for MME */
      case VB_SM_STATE_EGTP_BOUND: /* End of control operation of 
                                      S11 stack configuration from here */
      {
         vbMmeCntrlInit(event);
         break;
      }
      default:
      {
         VBSM_DBG_ERROR((VBSM_PRNTBUF, "Invalid State(%d)...\n",vbSmCb.state));
         break;
      }

   }
   RETVOID;
}


/*
*
*       Fun: vbMmeSendMsg  
*
*       Desc:  
*             
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm.c
*
*/
#ifdef ANSI
PUBLIC S16 vbMmeSendMsg
(
VbSmCneEvent  event
)
#else
PUBLIC S16 vbMmeSendMsg(event)
VbSmCneEvent  event;
#endif
{
   SM_TRC2(vbMmeSendMsg);

   vbMmeSM (event);

   RETVALUE(ROK);
} /* end of vbMmeSendMsg */
#ifdef __cplusplus
}
#endif /* __cplusplus */

/**********************************************************************
  
         End of file:     vbsm.c@@/main/1 - Mon Jun 28 19:35:40 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---                      1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1    vb005.101  rk        1. Added support for Network initiated Detach 
                      ms        2. SCTP service type made run time configurable
/main/1    vb006.101 ragrawal   1. Resolved debug print issue.
                                2. Added Duplicate case value.
                                3. Added new variable vcIpAddr.
                                4. Added  MULTIPLE_ENB Support.
                                5. Introduced new flag VB_SB_SCTP for Trillium
                                   SCTP and added support for Kernel SCTP.
/main/1    vb007.101 akaranth   1. Made MCC and MNC configurable
                                2. s1ap include files order has changed.
*********************************************************************91*/
