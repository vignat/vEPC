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

     Name:     HSS DB

     Type:     C source file

     Desc:     C source code for maitaining HSS Database.

     File:     vb_hss_uedb.c
  
     Sid:      vb_hss_uedb.c@@/main/1 - Mon Jun 28 19:35:38 2010
 
     Prg:      ad

*********************************************************************21*/

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
#include "cm_os.h"
#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */


#include "lvb.h"           /* CNE Layer management           */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"
#include "vb_hss.h"
#include "cm_emm.h"
#include "cm_esm.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */

#include <stdlib.h>        /* For strtoul() */

#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm_os.x"
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "sct.x"           /* SCT interface defines           */
#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

/* vb007.101 :S1AP include files order has changed. */
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
#include "vb_hss_common.x"
#include "vb.x"            /* CNE Application structures     */
#include "vb_hss.x"

PRIVATE S16 vbHssPrintUeDB ARGS((Void));

PUBLIC VbHssUeDB vbHssUeDB;

/** HSS UE Database keywords/tags. 
  * Reading & parsing of the DB file is based on these keywords. 
  * Also while writing these keywords have to be written before 
  * the values in DB file. */
PRIVATE Keys vbHssUeDB_kw[]=
{
   {"beginCfg",               VB_HSS_TKN_BEGIN_CFG },
   {"numUEEntries",           VB_HSS_TKN_NUM_UE_ENTRIES },
   {"UE_Entry",               VB_HSS_TKN_UE_ENTRY },
   {"IMSI_numDigits",         VB_HSS_TKN_IMSI_NUM_DGTS },
   {"IMSI",                   VB_HSS_TKN_IMSI },
   {"MSISDN",                 VB_HSS_TKN_MSISDN },
   {"IMEI",                   VB_HSS_TKN_IMEI },
   {"IMEISV",                 VB_HSS_TKN_IMEISV },
   {"MME_ADDR",               VB_HSS_TKN_MME_ADDR },
   {"MS_PS_Purged_from_EPS",  VB_HSS_TKN_PURGED },
   {"Sub_UE_AMBR",           VB_HSS_TKN_UE_AMBR },
   {"Cntxt_ID",              VB_HSS_TKN_CNTXT_ID },
   {"PDN_ADDR_NUM",          VB_HSS_TKN_PDN_ADDR_NUM },
   {"PDN_ADDR_LST",          VB_HSS_TKN_PDN_ADDR_LST },
   {"PDN_TYPE",              VB_HSS_TKN_PDN_TYPE },
   {"APN",                   VB_HSS_TKN_APN },
   {"QOS_Profile",           VB_HSS_TKN_QOS_PROFILE },
   {"Sub_APN_AMBR",          VB_HSS_TKN_SUB_APN_AMBR },
   {"PDN_GW_ADDR",           VB_HSS_TKN_PDN_GW_ADDR },
   {"PDN_GW_Allc_Type",      VB_HSS_TKN_PDN_GW_ALLC_TYPE },
   {"AUTH_KEY",              VB_HSS_TKN_AUTH_KEY },
   {"OP_KEY",                VB_HSS_TKN_OP_KEY },
   {"AUTH_TYPE",             VB_HSS_TKN_AUTH_TYPE },
   {"INTEGRITY",             VB_HSS_TKN_INTEGRITY_TYPE },
   {"CIPHERING",             VB_HSS_TKN_CIPHERING_TYPE },
   {"ROAM_STATUS",           VB_HSS_TKN_ROAM_STATUS },
   {"REDIR_STATUS",          VB_HSS_TKN_REDIR_STATUS },
   {"UE_Status",             VB_HSS_TKN_UE_STATUS },
   {"endCfg",                VB_HSS_TKN_END_CFG },
   {"",                      MATCH_FAILED }
};

PRIVATE S16 vbHssLineToTkn ARGS((S8 *line, VbHssTkn *tkn));
PRIVATE S16 vbHssParseU32 ARGS((VbHssTkn *tkn, U32 low, U32 high, U32 *ret));
PRIVATE U32 vbHssStrtoul ARGS((CONSTANT S8 *str, S8 **ptr, S32 base));
PRIVATE S16 vbHssParseArr ARGS((VbHssTkn *tkn, U8 *ptr, U32 size, U32 nmbElem));
PRIVATE S16 vbHssParseIpAddr ARGS((VbHssTkn *tkn,  CmInetIpAddr *address));
PRIVATE S16 vbHssParseStr ARGS((VbHssTkn *tkn, S8 *ptr, U8 size));
PRIVATE S16 vbHssParseAMBR ARGS((VbHssTkn *tkn, VbHssAMBR *ptr, U32 nmbElem));
PRIVATE S16 vbHssParseQosPrfle ARGS((VbHssTkn *tkn, VbHssEPSSubQOSPrf *ptr, U32 nmbElem));


/**

   @brief Convert Line format to Token Format

   @param *line pointer to line
   @param *tkn  pointer to tkn of type VbHssTkn

   @return 
      - Number of arguments on success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssLineToTkn
(
S8 *line,
VbHssTkn *tkn
)
#else
PRIVATE S16 vbHssLineToTkn(line, tkn)
S8 *line;
VbHssTkn *tkn;
#endif
{
   S8 *lPtr;
   S8 word[VB_HSS_DB_LINE_LEN];
   S32 i;

   lPtr = osGetWord(line, word);

   if (osStrlen(word) >= sizeof(tkn->tok.val)) /* make sure there's room */
      RETVALUE(-1);

   osStrcpy((S8*)tkn->tok.val, word);
   tkn->tok.pres = TRUE;
   /* vb006.101: Fix for compilation warning */
   tkn->tok.len = (U8) osStrlen((S8*)tkn->tok.val);
   tkn->nmbArgs = 0;

   for (i = 0; i < VB_HSS_DB_MAX_ARGS; i++)
   {
      lPtr = osGetWord(lPtr, word);

      if (osStrlen(word) >= sizeof(tkn->tok.val)) /* make sure there's room */
         RETVALUE(-1);

      if (*word == '\0' || *word == '#') /* end of line or comment */
      {
         tkn->args[i].pres = FALSE;
         break;
      }

      osStrcpy((S8*)tkn->args[i].val, word);
      tkn->args[i].pres = TRUE;
      /* vb006.101: Fix for compilation warning */
      tkn->args[i].len = (U8)osStrlen((S8*)tkn->args[i].val);
      tkn->nmbArgs++;
   }

   RETVALUE(tkn->nmbArgs);
} /* end of vbHssLineToTkn */



/**

   @brief Parse argument into U32

   @param *tkn
   @param low
   @param high
   @param *ret

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssParseU32
(
VbHssTkn *tkn,
U32 low,
U32 high,
U32 *ret
)
#else
PRIVATE S16 vbHssParseU32(tkn, low, high, ret)
VbHssTkn *tkn;
U32 low;
U32 high;
U32 *ret;
#endif
{
   VB_TRC2(vbHssParseU32)
   if (!tkn->args[0].pres || *tkn->args[0].val != '=')
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseU32: equals sign missing"));
      RETVALUE(RFAILED);
   }

   if (!tkn->args[1].pres)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseU32: arg not present"));
      RETVALUE(RFAILED);
   }

   *ret = (U32)vbHssStrtoul((S8*)tkn->args[1].val, NULLP, NULLP);

   if (*ret < low || *ret > high)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseU32() - value out of range"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of vbHssParseU32 */


/**

   @brief Wrapper for strtoul

   @param *str
   @param **ptr
   @param base

   @return 
      - ascii string to long value

 */
#ifdef ANSI
PRIVATE U32 vbHssStrtoul
(
CONSTANT S8 *str,
S8 **ptr,
S32 base
)
#else
PRIVATE U32 vbHssStrtoul(str, ptr, base)
CONSTANT S8 *str;
S8 **ptr;
S32 base;
#endif
{
   VB_TRC2(vbHssStrtoul)
   RETVALUE(strtoul(str, ptr, base));

} /* end of vbHssStrtoul */


/**

   @brief Parse an array

   @param *tkn
   @param *ptr
   @param size
   @param nmbElem

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssParseArr
(
VbHssTkn *tkn,
U8 *ptr,
U32 size,
U32 nmbElem
)
#else
PRIVATE S16 vbHssParseArr(tkn, ptr, size, nmbElem)
VbHssTkn *tkn;
U8 *ptr;
U32 size;
U32 nmbElem;
#endif
{
   U32 count;
   U32 val;
   

   VB_TRC2(vbHssParseArr)
   if (nmbElem != tkn->nmbArgs)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "vbHssParseArr: Invalid No of Argument "));
      RETVALUE(RFAILED);
   }
      
   for (count = 0; count < nmbElem; count++)
   {
      
      if (!tkn->args[count].pres)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "vbHssParseArr: Argument No not present"));
         RETVALUE(RFAILED);
      }
      
      val = strtoul((S8*)tkn->args[count].val, NULLP, NULLP);
      /* vb005.101:    */
      switch (size)
      {
         case 1:
         {
            *(U8 *)(ptr+(count * 1)) = (U8) val;
            break;
         }
         case 2: 
         {
            *(U16 *)(ptr+(count * 2)) = (U16) val;
            break;
         }
         case 4:
         {
            *(U32 *)(ptr+(count * 4)) = (U32) val;
            break;
         }
         default:
         {
            break;
         }
      }
    /* vb004.101: switch added for compatibility based on size of val 
               received. */
      switch (size)
      {
         case 1:
         {
            *(U8 *)(ptr+(count * size)) = (U8) val;
            break;
         }
         case 2: 
         {
            *(U16 *)(ptr+(count * size)) = (U16) val;
            break;
         }
         case 4:
         {
            *(U32 *)(ptr+(count * size)) = (U32) val;
            break;
         }
         default:
         {
            break;
         }
      }

   }/* End of for loop */

   RETVALUE(ROK);
   
}/* End of vbHssParseArr */

/**

   @brief Parse argument into CmInetIpAddr (U32) format.

   @param *tkn
   @param *address

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssParseIpAddr
(
VbHssTkn *tkn,
CmInetIpAddr *address
)
#else
PRIVATE S16 vbHssParseIpAddr(tkn,  address)
VbHssTkn *tkn;
CmInetIpAddr *address;
#endif
{
   VB_TRC2(vbHssParseIpAddr)
   if (!tkn->args[0].pres || *tkn->args[0].val != '=')
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseIpAddr: equals sign missing"));
      RETVALUE(RFAILED);
   }

   if (!tkn->args[1].pres)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseIpAddr: arg not present"));
      RETVALUE(RFAILED);
   }

   if (ROK != (cmInetAddr((S8*)tkn->args[1].val, address)))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseIpAddr() - Failed to get the IP Address "));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of vbHssParseIpAddr */

/**

   @brief Parse argument into String

   @param *tkn
   @param *address

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssParseStr
(
VbHssTkn *tkn,
S8 *str,
U8  size
)
#else
PRIVATE S16 vbHssParseStr(tkn,  str, size)
VbHssTkn *tkn;
S8     *str;
U8     size;
#endif
{
   U8 strLen;
   VB_TRC2(vbHssParseStr)

   if (!tkn->args[0].pres || *tkn->args[0].val != '=')
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseStr: equals sign missing"));
      RETVALUE(RFAILED);
   }

   if (!tkn->args[1].pres)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseStr: arg not present"));
      RETVALUE(RFAILED);
   }

   /* vb006.101: Fix for compilation warning */
   strLen = (U8)osStrlen((S8*)tkn->tok.val);

   if (strLen >= size)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"vbHssParseStr: Given string lenght is more "));
      RETVALUE(RFAILED);
   }

   osStrcpy(str, (S8*)tkn->args[1].val);

   RETVALUE(ROK);

} /* end of vbHssParseStr */


/**

   @brief Parse an AMBR

   @param *tkn
   @param *ptr
   @param size
   @param nmbElem

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssParseAMBR
(
VbHssTkn *tkn,
VbHssAMBR *ptr,
U32 nmbElem
)
#else
PRIVATE S16 vbHssParseAMBR(tkn, ptr, nmbElem)
VbHssTkn *tkn;
VbHssAMBR *ptr;
U32 nmbElem;
#endif
{
   U32 count;
   U32 val;
   
   VB_TRC2(vbHssParseAMBR)

   if (nmbElem != tkn->nmbArgs)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "Invalid No Of Argument"));
      RETVALUE(RFAILED);
   }
      
   for (count = 0; count < nmbElem; count++)
   {
      
      if (!tkn->args[count].pres)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "Argument No Not Present"));
         RETVALUE(RFAILED);
      }
      
      val = strtoul((S8*)tkn->args[count].val, NULLP, NULLP);

      switch(count)
      {
         case 0:
            ptr->dl = val;
            break;
         case 1:
            ptr->ul = val;
            break;
         case 2:
            ptr->dlExt = val;
            break;
         case 3:
            ptr->ulExt = val;
            break;
         case 4:
            ptr->dlExt2 = val;
            break;
         case 5:
            ptr->ulExt2 = val;
            break;
      }


   }/* End of for loop */

   RETVALUE(ROK);
   
}/* End of vbHssParseAMBR */


/**

   @brief Parse an QOS Profile

   @param *tkn
   @param *ptr
   @param size
   @param nmbElem

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PRIVATE S16 vbHssParseQosPrfle
(
VbHssTkn *tkn,
VbHssEPSSubQOSPrf *ptr,
U32 nmbElem
)
#else
PRIVATE S16 vbHssParseQosPrfle(tkn, ptr, nmbElem)
VbHssTkn *tkn;
VbHssEPSSubQOSPrf *ptr;
U32 nmbElem;
#endif
{
   U32 count = 0;
   U32 val = 0;
   
   VB_TRC2(vbHssParseQosPrfle)

   if (nmbElem != tkn->nmbArgs)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Invalid Number Of Arguments"));

      RETVALUE(RFAILED);
   }
      
   for (count = 0; count < nmbElem; count++)
   {
      
      if (!tkn->args[count].pres)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "Argument No Not Present"));
         RETVALUE(RFAILED);
      }
      
      val = strtoul((S8*)tkn->args[count].val, NULLP, NULLP);

      switch(count)
      {
         case 0:
            ptr->qci = val;
            break;
         case 1:
            ptr->arp.priLevel = val;
            break;
         case 2:
            ptr->arp.preCapbFlg = val;
            break;
         case 3:
            ptr->arp.preVlnbFlg = val;
            break;
         case 4:
            ptr->maxBitRateUL = val;
            break;
         case 5:
            ptr->maxBitRateDL = val;
            break;
         case 6:
            ptr->guaraBitRateUL = val;
            break;
         case 7:
            ptr->guaraBitRateDL = val;
            break;
         case 8:
            ptr->maxBitRateULExt = val;
            break;
         case 9:
            ptr->maxBitRateDLExt = val;
            break;
         case 10:
            ptr->guaraBitRateULExt = val;
            break;
         case 11:
            ptr->guaraBitRateDLExt = val;
            break;
      }


   }/* End of for loop */

   RETVALUE(ROK);
   
}/* End of vbHssParseQosPrfle */


/**

   @brief Reads file "vb_hss_ue.db" which is the database of UEs configured in the HSS. 
          Reading is based on the keywords. After reading, the data is parsed & 
          stored in *ueDB.

   @param *ueDB pointer to ueDB of type VbHssUeDB which hold the information 
           related to all the UEs configured

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PUBLIC S16 vbHssReadUeDB
(
  Void
)
#else
PUBLIC S16 vbHssReadUeDB(Void)
#endif
{
   VbHssUeDB         *ueDB;
   VbHssImsi          imsi;
   VbHssMsisdn        msisdn;
   VbHssImei          imei;
   VbHssImeisv        imeisv;
   U32               numCtxt = 0;
   VbHssTkn           tkn;
   S8               line[VB_HSS_DB_LINE_LEN + 1];
   OsFile           *fp = NULLP;
   S16              ret = ROK;
   S8               *ret1 = NULLP;
   U32              k = 0;
   S32              val;
   U32              numImsiDigit = 0;
   PRIVATE S32      lineNo = 0;
   CmInetIpAddr     address = 0;
   U16              ueIdx;
   U8               imsiIndex;
   U8               nextOrder;
   U8               indx = 0;
   U8               ipAddr[CM_INET_IPV4ADDR_SIZE];
#ifdef VB_MME_AUTH
   U8               prm[VB_HSS_SHARED_KEY_SIZE] = {'\0'};
#endif /* VB_MME_AUTH */

   VB_TRC2(vbHssReadUeDB);

   ueDB = &vbHssUeDB;
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Reading HSS UE Database"));

   if ((fp = (OsFile*)osFopen("vb_hss_ue.db", "r")) == (S32)NULLP)
   {
      /* vb005.101 : ccpu00117528 File vb_hss_ue.db file check */
      VB_MME_LOGERROR(ERRCLS_DEBUG, 0, ERRZERO,
            "FAILED : File Open Error, File vb_hss_ue.db");
      exit(0);
   }

   lineNo = 1;

   for (;;)
   {
      ret1 = osFgets(line, VB_HSS_DB_LINE_LEN, fp);
   
      if (ret1 == (S8*)-1 || ret1 == NULLD) /* eof or NULL */
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "End Of File Reached, vb_hss_ue.db"));
         break;
      }

      if (!osStripBlanks(line))
      {
         lineNo++;
         continue;   /* blank line */
      }

      if (*line == '#')
      {
         lineNo++;
         continue;   /* blank line */
      }

      /* convert line to tokens */
      if (vbHssLineToTkn(line, &tkn) == -1)
      {
#ifdef ALIGN_64BIT
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "Syntax Error Reading File. Line(%d)",lineNo));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "Syntax Error Reading File. Line(%ld)",lineNo));

#endif
			 if(fp) osFclose(fp);
         RETVALUE(RFAILED);
      }

      val = osMatchKeyword(vbHssUeDB_kw, (S8*)tkn.tok.val);

      switch (val)
      {
         case VB_HSS_TKN_BEGIN_CFG: /* beginCfg */
            break;

         case VB_HSS_TKN_NUM_UE_ENTRIES: /* numUEEntries */
         {
            ret = vbHssParseU32(&tkn, 0, VB_HSS_MAX_UES, &ueDB->numUEEntries);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}
            break;
         }
         case VB_HSS_TKN_UE_ENTRY: /* UE_Entry */
         {
            ret = vbHssParseU32(&tkn, 0, VB_HSS_MAX_UES,\
                  &ueDB->ueInfo[k].ueEntry);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

				/* RIVADA DROP3 FIX1 */
				if (ueDB->ueInfo[k].ueEntry > ueDB->numUEEntries)
				{
					ueDB->ueInfo[k].ueEntry = 0;
					if(fp) osFclose(fp);
					RETVALUE(ROK);
				}


            /* Reset for new entry */
            numCtxt = 0;

            break;
         }
         case VB_HSS_TKN_IMSI_NUM_DGTS: /* IMSI_numDigits */
         {
            ret = vbHssParseU32(&tkn, 0, VB_HSS_IMSI_LEN, &numImsiDigit);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            ueDB->ueInfo[k].imsi.len = (U8)numImsiDigit;
            break;
         }
         case VB_HSS_TKN_IMSI: /* IMSI */
         {
            ret = vbHssParseArr(&tkn, (U8*)imsi.val, sizeof(U8), numImsiDigit);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }
            cmMemcpy(ueDB->ueInfo[k].imsi.val, imsi.val, numImsiDigit);
            ueDB->ueInfo[k].imsi.len = (U8)numImsiDigit;
            break;
         }

         case VB_HSS_TKN_MSISDN: /* MSISDN */
         {
            ret = vbHssParseArr(&tkn, (U8*)msisdn, sizeof(U8),\
                  VB_HSS_MSISDN_LEN);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

            cmMemcpy(ueDB->ueInfo[k].msisdn, msisdn, VB_HSS_MSISDN_LEN);
            VB_MME_SET_MASK(ueDB->ueInfo[k].pres,\
                  VB_MME_HSS_CTX_MSISDN_PRESENT);
            break;
         }
         case VB_HSS_TKN_IMEI: /* IMEI */
         {
            ret = vbHssParseArr(&tkn, (U8*)imei, sizeof(U8), VB_HSS_IMEI_LEN);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

            cmMemcpy(ueDB->ueInfo[k].imei, imei, VB_HSS_IMEI_LEN);
            
            break;
         }
         case VB_HSS_TKN_IMEISV: /* IMEISV */
         {
            ret = vbHssParseArr(&tkn, (U8*)imeisv, sizeof(U8),\
                  VB_HSS_IMEISV_LEN);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

#ifdef VB_MME_AUTH
           ueDB->ueInfo[k].imeisv[VB_HSS_IMEISV_LEN - 2] = \
               imeisv[VB_HSS_IMEISV_LEN - 2]; 
           ueDB->ueInfo[k].imeisv[VB_HSS_IMEISV_LEN - 1] = \
               imeisv[VB_HSS_IMEISV_LEN - 1];
           
#else
            cmMemcpy(ueDB->ueInfo[k].imeisv, imeisv, VB_HSS_IMEISV_LEN);
#endif
            break;
         }

         case VB_HSS_TKN_MME_ADDR: /* MME_ADDR */
         {
            ret = vbHssParseIpAddr(&tkn, &address);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

            /* For Phase 1, hard coding for IPv4, we will enhance it for
                * IPv6 later on*/
            ueDB->ueInfo[k].mmeAddr.type = CM_INET_IPV4ADDR_TYPE;
            cmMemcpy((U8*)&ueDB->ueInfo[k].mmeAddr.u.ipv4NetAddr,\
                  (U8*) &address, sizeof(CmInetIpAddr));
            break;
         }

#ifdef VB_MME_AUTH
         case VB_HSS_TKN_AUTH_KEY: /* authentication key */
         {
            ret = vbHssParseArr(&tkn, (U8*)prm, sizeof(U8),\
                  VB_HSS_SHARED_KEY_SIZE);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

            cmMemcpy(ueDB->ueInfo[k].ueSecInfo.authKey, prm,\
                  VB_HSS_SHARED_KEY_SIZE);
            break;
         }

         case VB_HSS_TKN_OP_KEY: /* authentication key */
         {
	        cmMemset(prm, 0, VB_HSS_SHARED_KEY_SIZE );
            ret = vbHssParseArr(&tkn, (U8*)prm, sizeof(U8), VB_HSS_OP_KEY_SIZE);
            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }
            cmMemcpy(ueDB->ueInfo[k].ueSecInfo.opKey, prm, VB_HSS_OP_KEY_SIZE);

            break;
         }
         case VB_HSS_TKN_AUTH_TYPE:
         {
            ret = vbHssParseStr(&tkn, (S8 *)prm, 16); 
            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}
            
            if (cmStrcmp(prm, (U8 *)"MILANAGE") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.authType = VB_MME_AUTH_MILANAGE;
            }
            else if (cmStrcmp(prm, (U8 *)"XOR") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.authType = VB_MME_AUTH_XOR;
            }
            else
            {
               RETVALUE(RFAILED);
            }

            break;
         }
         case VB_HSS_TKN_CIPHERING_TYPE:
         {
            ret = vbHssParseStr(&tkn, (S8 *)prm, 16); 
            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}
            
            if (cmStrcmp(prm, (U8 *)"EEA0") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.ciphType = VB_MME_EEA0;
            }
            else if (cmStrcmp(prm, (U8 *)"EEA1") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.ciphType = VB_MME_EEA1_128;
            }
            else if (cmStrcmp(prm, (U8 *)"EEA2") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.ciphType = VB_MME_EEA2_128;
            }
            else
            {
               RETVALUE(RFAILED);
            }

            break;
         }
         case VB_HSS_TKN_INTEGRITY_TYPE:
         {
            ret = vbHssParseStr(&tkn, (S8 *)prm, 16); 
            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}
            
            if (cmStrcmp(prm, (U8 *)"EIA0") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.intType = VB_MME_EIA0;
            }
            else if (cmStrcmp(prm, (U8 *)"EIA1") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.intType = VB_MME_EIA1_128;
            }
            else if (cmStrcmp(prm, (U8 *)"EIA2") == 0)
            {
               ueDB->ueInfo[k].ueSecInfo.intType = VB_MME_EIA2_128;
            }
            else
            {
               RETVALUE(RFAILED);
            }

            break;
         }
#endif /* VB_MME_AUTH */
         case VB_HSS_TKN_PURGED: /* MS_PS_Purged_from_EPS */
         {
            ret = vbHssParseU32(&tkn, 0, 1,\
                  (U32 *)&ueDB->ueInfo[k].isMsPsPurgd);
            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_UE_AMBR: /* Sub_UE_AMBR */
         {
            ret = vbHssParseAMBR(&tkn, (VbHssAMBR*)&ueDB->ueInfo[k].subUeAMBR, 
                              VB_HSS_NUM_AMBR_COMP);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_CNTXT_ID: /* Cntxt_ID */
         {
            ret = vbHssParseU32(&tkn, 0, VB_HSS_MAX_PDN_SUB_CNTXT_NUM, 
                     (U32*)&ueDB->ueInfo[k].subCntxt[numCtxt].cntxtId);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            numCtxt++;

            break;
         }
         case VB_HSS_TKN_PDN_ADDR_NUM: /* PDN_ADDR_NUM */
         {
            ret = vbHssParseU32(&tkn, 0, VB_HSS_MAX_PDN_ADDR_NUM, 
                  (U32*)&ueDB->ueInfo[k].subCntxt[numCtxt - 1].pdnAddrLst.nmb);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_PDN_ADDR_LST: /* PDN_ADDR_LST */
         {
            ret = vbHssParseIpAddr(&tkn, &address);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

            /* For Phase 1, hard coding for IPv4, we will enhance it for
                * IPv6 later on*/
               /* Also, currently we are considering only one IP in the list*/
            ueDB->ueInfo[k].subCntxt[numCtxt - 1].pdnAddrLst.nAddr[0].type = 
                  CM_INET_IPV4ADDR_TYPE;
            ueDB->ueInfo[k].subCntxt[numCtxt - 1].pdnAddrLst.nAddr[0].u.\
                  ipv4NetAddr = CM_INET_NTOH_U32(address);

            break;
         }            
         case VB_HSS_TKN_PDN_TYPE: /* PDN_TYPE */
         {
            ret = vbHssParseU32(&tkn, 0, 5, 
                     &ueDB->ueInfo[k].subCntxt[numCtxt - 1].pdnAddrType);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_APN: /* APN */
         {
            ret = vbHssParseStr(&tkn, 
                  (S8*)&ueDB->ueInfo[k].subCntxt[numCtxt - 1].apn, 
                  VB_HSS_APN_LEN);
            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_QOS_PROFILE: /* QOS_Profile */
         {
            ret = vbHssParseQosPrfle(&tkn,\
                  &ueDB->ueInfo[k].subCntxt[numCtxt - 1].qos, 
                  VB_HSS_NUM_QOS_COMP);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;            
         }
         case VB_HSS_TKN_SUB_APN_AMBR: /* Sub_APN_AMBR */
         {
            ret = vbHssParseAMBR(&tkn,\
                  (VbHssAMBR*)&ueDB->ueInfo[k].subCntxt[numCtxt - 1].ambr, 
                  VB_HSS_NUM_AMBR_COMP);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_PDN_GW_ADDR: /* PDN_GW_ADDR */
         {
            ret = vbHssParseIpAddr(&tkn, &address);

            if (ret != ROK)
            {
					if(fp) osFclose(fp);
               RETVALUE(ret);
            }

            /* For Phase 1, hard coding for IPv4, we will enhance it for
                * IPv6 later on*/
            ueDB->ueInfo[k].subCntxt[numCtxt - 1].pdnGwAddr.type = \
                  CM_INET_IPV4ADDR_TYPE;
            ueDB->ueInfo[k].subCntxt[numCtxt - 1].pdnGwAddr.u.ipv4NetAddr = \
                  CM_INET_NTOH_U32(address);

            break;
         }
         case VB_HSS_TKN_PDN_GW_ALLC_TYPE: /* PDN_GW_Allc_Type */
         {
            ret = vbHssParseU32(&tkn, 0, 1, 
                     &ueDB->ueInfo[k].subCntxt[numCtxt - 1].allocType);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

            break;
         }
         case VB_HSS_TKN_UE_STATUS: /* Last Entry : UE_Status */
         {
            ret = vbHssParseU32(&tkn, 0, 9, &ueDB->ueInfo[k].ueStatus);

            if (ret != ROK)
				{
					if(fp) osFclose(fp);
               RETVALUE(ret);
				}

#ifndef VB_MME_AUTH
            /* Authentication feature added Initialization */
            ueDB->ueInfo[k].ueSecInfo.pres = NOTPRSNT;
            ueDB->ueInfo[k].ueSecInfo.resync = FALSE;
#endif

            k++; /* Increment after reading last entry for an UE */
            break;
         }
         case VB_HSS_TKN_END_CFG: /* endCfg */
         {
            break;
         }
         default:
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid keyword in "\
                  "vb_hss_ue.db"));
            ret = RFAILED;
            break;
         }
      } /* End of switch(val) */

   } /* End of for loop */

   if(fp) osFclose(fp);

   /* Changes the identity parameter for multiple UEs:
    1. IMSI 
    2. IMEI
    3. IMEISV
    4. MSISDN
    5  AUTH KEY
   */

   for (ueIdx = k; ueIdx < ueDB->numUEEntries; ueIdx++)
   {
      cmMemcpy((U8 *)&ueDB->ueInfo[ueIdx], (U8 *)&ueDB->ueInfo[ueIdx-1], 
               sizeof(VbHssUeInfo));
      ueDB->ueInfo[ueIdx].ueEntry = ueIdx + 1;

      /*IMSI*/
      nextOrder = ueDB->ueInfo[ueIdx-1].imsi.len-1;
      for (imsiIndex = ueDB->ueInfo[ueIdx].imsi.len-1; imsiIndex > 0; 
           --imsiIndex)
      {
         if((ueDB->ueInfo[ueIdx-1].imsi.val[nextOrder] ) == 9)
         {
            ueDB->ueInfo[ueIdx].imsi.val[imsiIndex] = 0; 
            nextOrder = imsiIndex -1;
            continue;
         }
         else
         {  
            ueDB->ueInfo[ueIdx].imsi.val[imsiIndex] = 
            ueDB->ueInfo[ueIdx-1].imsi.val[imsiIndex] + 1;
            break;
         }
      }
 
      /*IMEI */
      nextOrder = VB_HSS_IMEI_LEN - 1;
      for (indx = VB_HSS_IMEI_LEN - 1; indx > 0; 
           --indx)
      {
         if((ueDB->ueInfo[ueIdx-1].imei[nextOrder] ) == 9)
         {
            ueDB->ueInfo[ueIdx].imei[indx] = 0; 
            nextOrder = indx - 1;
            continue;
         }
         else
         {  
            ueDB->ueInfo[ueIdx].imei[indx] = 
            ueDB->ueInfo[ueIdx-1].imei[indx] + 1;
            break;
         }
      }
      cmMemcpy(ueDB->ueInfo[ueIdx].imeisv, ueDB->ueInfo[ueIdx].imei, VB_HSS_IMEI_LEN);

      nextOrder = VB_HSS_MSISDN_LEN - 1;
      for (indx = VB_HSS_MSISDN_LEN - 1; indx > 0; 
           --indx)
      {
         if((ueDB->ueInfo[ueIdx-1].msisdn[nextOrder] ) == 9)
         {
            ueDB->ueInfo[ueIdx].msisdn[indx] = 0; 
            nextOrder = indx - 1;
            continue;
         }
         else
         {  
            ueDB->ueInfo[ueIdx].msisdn[indx] = 
            ueDB->ueInfo[ueIdx-1].msisdn[indx] + 1;
            break;
         }
      }

      /*If Static IP Address Allocation */
      if(ueDB->ueInfo[ueIdx - 1].subCntxt[0].pdnAddrLst.
                                 nAddr[0].u.ipv4NetAddr)
      {
         ipAddr[3] = ueDB->ueInfo[ueIdx - 1].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr >> 24 & 0xff;
         ipAddr[2] = ueDB->ueInfo[ueIdx - 1].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr >> 16 & 0xff;
         ipAddr[1] = ueDB->ueInfo[ueIdx - 1].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr >> 8 & 0xff;
         ipAddr[0] = ueDB->ueInfo[ueIdx - 1].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr & 0xff;

         nextOrder = CM_INET_IPV4ADDR_SIZE - 1; /* IPV4 Addr Len -1 */
         for (indx = CM_INET_IPV4ADDR_SIZE - 1; indx > 0; --indx)
         {
            if((ipAddr[nextOrder] ) == 254)
            {
               ipAddr[indx] = 0;
               nextOrder = indx - 1;
               continue;
            }
            else
            {
               ipAddr[indx]= ipAddr[indx]+1;
               break;
            }
         }     
         ueDB->ueInfo[ueIdx].subCntxt[0].pdnAddrLst.nAddr[0].u.ipv4NetAddr =  
            (ipAddr[3] << 24) |(ipAddr[2] << 16) |(ipAddr[1] << 8) |(ipAddr[0]);
      }
   }

   vbHssPrintUeDB();

   RETVALUE(ret);

} /* End of vbHssReadUeDB()  */


/**

   @brief Writes the UE Information into a DB file at HSS. 
          The data from *ueDB is written into DB file with appropriate keywords.

   @param *ueDB pointer to ueDB of type VbHssUeDB which holds the 
                information related to all the UEs configured

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PUBLIC S16 vbHssWriteUeDB
(
Void
)
#else
PUBLIC S16 vbHssWriteUeDB(Void)
#endif
{
   /*For the second phase*/


#ifdef VB_TODO_SECOND_PHASE
   U8   i, k;
   FILE *fp = NULL;
   VbHssUeDB  *ueDB;
   ueDB = &vbHssUeDB;

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Writing UE Info Into HSS UE Database"));


   system("cp vb_hss_ue.db vb_hss_ue.db.restore");

   if (NULL == (fp = fopen("vb_hss_ue.db", "w"))) 
   {
      fprintf(stderr, "Could Not Open vb_hss_ue.db For Writing\n");
      RETVALUE(RFAILED);
   }

   fprintf(fp, "%s", "#########################################\n");
   fprintf(fp, "%s", "# HSS UE Database\n");
   fprintf(fp, "%s", "#########################################\n\n");

   fprintf(fp, "%s", "beginCfg ueDB\n\n");

   fprintf(fp, "%s", "numUEEntries = ");
#ifdef ALIGN_64BIT	
   fprintf(fp, "%d", ueDB->numUEEntries);
#else
   fprintf(fp, "%ld", ueDB->numUEEntries);
#endif
   fprintf(fp, "%s", "\n\n");

   for (k = 0; k < ueDB->numUEEntries; k++)
   {
      fprintf(fp, "# UE %d\n", k);
      fprintf(fp, "%s", "########################\n");

      fprintf(fp, "%s", "UE_Entry = ");
#ifdef ALIGN_64BIT	
      fprintf(fp, "%d\n", ueDB->ueInfo[k].ueEntry);
#else
      fprintf(fp, "%ld\n", ueDB->ueInfo[k].ueEntry);
#endif

      fprintf(fp, "%s", "IMSI_numDigits = ");
#ifdef ALIGN_64BIT	
      fprintf(fp, "%d\n", ueDB->ueInfo[k].imsi.len );
#else
      fprintf(fp, "%ld\n", ueDB->ueInfo[k].imsi.len );
#endif

      fprintf(fp, "%s", "IMSI ");

      for (i = 0; i < ueDB->ueInfo[k].imsi.len; i++)
      {
         fprintf(fp, "%d ", ueDB->ueInfo[k].imsi[i]);
      }

      fprintf(fp, "\n");

      fprintf(fp, "%s", "TMSI = ");
#ifdef ALIGN_64BIT		
      fprintf(fp, "%d\n", ueDB->ueInfo[k].tmsi);
#else
      fprintf(fp, "%ld\n", ueDB->ueInfo[k].tmsi);
#endif

      fprintf(fp, "%s", "MSISDN  ");
      for (i = 0; i < VB_HSS_MSISDN_LEN; i++)
      {
         fprintf(fp, "%d ", ueDB->ueInfo[k].msisdn[i]);
      }
      fprintf(fp, "\n");

      fprintf(fp, "%s", "LAC = ");
      fprintf(fp, "%d\n", ueDB->ueInfo[k].lac);

      fprintf(fp, "%s", "Authentication_Key  ");
      for (i = 0; i < CM_EMM_MAX_RAND; i++)
      {
         fprintf(fp, "%d ", (ueDB->ueInfo[k].authKey[i] >> 4));
         fprintf(fp, "%d ", (ueDB->ueInfo[k].authKey[i] & 0x0F));
      }

      fprintf(fp, "\n%s", "UE_Status = ");

#ifdef ALIGN_64BIT		
      fprintf(fp, "%d\n\n", ueDB->ueInfo[k].ueStatus);
#else
      fprintf(fp, "%ld\n\n", ueDB->ueInfo[k].ueStatus);
#endif

   }

   fprintf(fp, "%s", "\nendCfg");

   fflush(fp);
   fclose(fp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"HSS UE Database Writing Success"));

#endif /* VB_TODO_SECOND_PHASE */

   RETVALUE(ROK);

}

/* Authentication feature added 
 * double pointer passed for ueInfo */
/**

   @brief Searches for the UE Information for a given IMSI.

   @param *ueDB pointer to ueDB of type VbHssUeDB which holds the 
                information related to all the UEs configured
          *ueInfo the Information related the UE having the given IMSI.
          *imsi pointer to IMSI value.

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PUBLIC S16 vbHssFindImsi
(
U8         *imsi,
VbHssUeInfo **ueInfo
)
#else
PUBLIC S16 vbHssFindImsi(imsi, ueInfo)
U8         *imsi;
VbHssUeInfo **ueInfo;
#endif
{

   U8          ueIdx;
   U8          dgtIdx;
   VbHssUeDB  *ueDb;

   VB_TRC2(vbHssFindImsi);

   ueDb = &vbHssUeDB;

   if (!ueDb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid HSS UE Database "));
      RETVALUE(RFAILED);
   }
   for (ueIdx = 0; ueIdx < ueDb->numUEEntries; ueIdx++)
   {
      /* compate each digit of IMSI with the stored IMSI */
      for (dgtIdx = 0; dgtIdx < ueDb->ueInfo[ueIdx].imsi.len; dgtIdx++)
      {
         if (imsi[dgtIdx] != ueDb->ueInfo[ueIdx].imsi.val[dgtIdx])
         {
            dgtIdx = 0;
            /* One mismatch, no need to check further digits */
            break;
         }
      }
      /* Found the IMSI, return the ueInfo pointer */
      if (dgtIdx == ueDb->ueInfo[ueIdx].imsi.len)
      {
         *ueInfo = &ueDb->ueInfo[ueIdx];
         RETVALUE(ROK);
      }
   }
   if (ueIdx == ueDb->numUEEntries)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"IMSI Not Found In HSS UE Database"));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);

} /* vbHssFindImsi */



#ifdef ANSI
PRIVATE S16 vbHssPrintUeDB
(
Void
)
#else
PRIVATE S16 vbHssPrintUeDB(Void)
#endif
{
   /*For the second phase*/
   U16  i, k;
   VbHssUeDB  *ueDB;
   FILE *fp;

   VB_TRC2(vbHssPrintUeDB);

   ueDB = &vbHssUeDB;

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Writing UE info into HSS UE Database"));


   if (NULL == (fp = fopen("vb_hss_ue_wr.db", "w"))) 
   {
      fprintf(stderr, "Could not open vb_hss_ue.db for writing\n");
      RETVALUE(RFAILED);
   }

   fprintf(fp, "%s", "#########################################\n");
   fprintf(fp, "%s", "# HSS UE Database\n");
   fprintf(fp, "%s", "#########################################\n\n");

   fprintf(fp, "%s", "numUEEntries = ");
#ifdef ALIGN_64BIT	
   fprintf(fp, "%d", ueDB->numUEEntries);
#else
   fprintf(fp, "%ld", ueDB->numUEEntries);
#endif
   fprintf(fp, "%s", "\n\n");

   for (k = 0; k < ueDB->numUEEntries; k++)
   {
      fprintf(fp, "# UE %d\n", k);
      fprintf(fp, "%s", "########################\n");

      fprintf(fp, "%s", "UE_Entry = ");
#ifdef ALIGN_64BIT	
      fprintf(fp, "%d\n", ueDB->ueInfo[k].ueEntry);
#else
      fprintf(fp, "%ld\n", ueDB->ueInfo[k].ueEntry);
#endif

      fprintf(fp, "%s", "IMSI_numDigits = ");
      fprintf(fp, "%d\n", ueDB->ueInfo[k].imsi.len );

      fprintf(fp, "%s", "IMSI: ");

      for (i = 0; i < ueDB->ueInfo[k].imsi.len; i++)
      {
         fprintf(fp, "%d ", ueDB->ueInfo[k].imsi.val[i]);
      }

      fprintf(fp, "\n");

      fprintf(fp, "%s", "IMEI: ");
      for (i = 0; i < VB_HSS_IMEI_LEN; i++)
      {
         fprintf(fp, "%d ", ueDB->ueInfo[k].imei[i]);
      }
      fprintf(fp, "\n");
      fprintf(fp, "%s", "MSISDN: ");
      for (i = 0; i < VB_HSS_MSISDN_LEN; i++)
      {
         fprintf(fp, "%d ", ueDB->ueInfo[k].msisdn[i]);
      }
      fprintf(fp, "\n");

      if(ueDB->ueInfo[k].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr)
      {
         fprintf(fp, "%s", "STATIC IP ADDR: ");
         fprintf(fp, "%ld %ld %ld %ld",
             ueDB->ueInfo[k].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr & 0xFF,
             ueDB->ueInfo[k].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr>> 8 & 0xFF,
             ueDB->ueInfo[k].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr >> 16 & 0xFF,
             ueDB->ueInfo[k].subCntxt[0].pdnAddrLst.
                           nAddr[0].u.ipv4NetAddr >> 24 & 0xFF);
         fprintf(fp, "\n");
      }
   }
   fclose(fp);

   RETVALUE(ROK);

}


/**************************************************************************//**
 *
 * @brief 
 *    Resets UE Security context details
 *             
 * @param[in]  pImsi
 *    IMSI
 * @param[in]  imsiLen
 *    Length of IMSI
 * 
 * @return
 *
 *****************************************************************************/
PUBLIC Void vbHssResetSecCtxt
(
U8       *pImsi
)
{
   S16         ret      = ROK;
   VbHssUeInfo *pUeInfo = NULLP;

   VB_TRC2(vbHssResetSecCtxt);

   ret = vbHssFindImsi(pImsi, &pUeInfo);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Resetting Security Context, SUCCESS"));
#ifdef VB_MME_NAS_SEC 
      cmMemset(pUeInfo->ueSecInfo.sqnHE, 0, VB_HSS_SEQ_NMB_SIZE);
#endif
   }
   
   RETVOID;
}

/********************************************************************30**
 
         End of file:     vb_hss_uedb.c@@/main/1 - Mon Jun 28 19:35:38 2010
 
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
/main/1      ---     ad          1. Initial version for LTE-CNE Phase 1.
/main/1   vb004.101  ve          1. switch added for compatibility based 
                                    on size of val received.
/main/1   vb005.101  ms          1. File vb_hss_ue.db check.                                    
/main/1   vb006.101 ragrawal     1. ccpu00118282 support for multiple UE entries in HSS DB.
                                 2. Fix for compilation warning.
                                 3. Removed unused variables in vbHssReadUeDB().
/main/1   vb007.101 akaranth     1. s1ap include files order has changed.
 *********************************************************************91*/
