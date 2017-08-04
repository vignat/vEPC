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
  
     Name:     common asn.1
  
     Type:     C Source file
  
     Desc:     C source code for common ASN.1 encoding/decoding 
               routines
  
     File:     cm_asn.c
  
     Sid:      cm_asn.c@@/main/nodal_main/1 - Thu Nov 21 09:39:58 2013
  
     Prg:      sg
  
*********************************************************************21*/


/************************************************************************
 
     Note: 
 
     This file has been extracted to support the following options:
 
     Option             Description
     ------    ------------------------------

************************************************************************/
 

/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_err.h"        /* common error */
#include "cm_ss7.h"        /* common ss7 */
#include "cm_asn.h"        /* common asn.1 encoding/decoding */

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_ss7.x"        /* common ss7 */
#include "cm_asn.x"        /* common asn.1 encoding/decoding */
#include "cm_lib.x"        /*cm_asn_c_001.main_23: common lib file -
			     adding cmMemset()*/


/* Macro for logging error */
/* cm_asn_c_001.main_16 - Addition - Changes for SS_MULTIPLE_PROCS */
#ifdef SS_MULTIPLE_PROCS

#define CMASNLOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(ENTNC, INSTNC, (ProcId) 0, __FILE__, __LINE__, \
              errCls, errCode, errVal, errDesc )
#else 
#define CMASNLOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(ENTNC, INSTNC, SFndProcId(), __FILE__, __LINE__, \
              errCls, errCode, errVal, errDesc )

#endif /* SS_MULTIPLE_PROCS */

/* prototypes */

PRIVATE S16 cmSkipElmnt ARGS((CmAsnMsgCp *msgCp));
PRIVATE S16 cmInitAsnMsgCp ARGS((CmAsnMsgCp *msgCp));
PRIVATE Void cmEncTag ARGS((CmElmntDef *elmntDef, U8 *pkArray, U16 *numBytes));
/* cm_asn_c_001.main_26: declaration as PUBLIC */
/* cm_asn_c_001.main_29 Removed redundant declaration. */
PRIVATE S16 cmDecChkFlag ARGS((CmAsnMsgCp *msgCp));

/* cm_asn_c_001.main_29 Removed redundant declaration. */
PRIVATE S16 cmGetLen ARGS((CmAsnMsgCp *msgCp, MsgLen offset, MsgLen *len, MsgLen *size, Bool *eocFlag));

PRIVATE S16 cmChkEnum ARGS((U8 val, CmElmntDef *elmntDef));
PRIVATE S16 cmChkEncElmnt ARGS((U8 flag, CmAsnMsgCp *msgCp, TknU8 *evntStr));

PRIVATE S16 cmChkSeqMandMis ARGS((CmAsnMsgCp *msgCp));
PRIVATE S16 cmChkSetMandMis ARGS((CmAsnMsgCp *msgCp));
PRIVATE Bool cmChkEleMandMis ARGS((CmAsnMsgCp *msgCp));
PRIVATE S16 cmInitConstType ARGS((CmElmntDef ***ptr));
PRIVATE S16 cmCpyUnrecogElmnt ARGS((CmAsnMsgCp *msgCp, MsgLen *bytes));
#ifdef CM_ASN_DBG
PUBLIC  Void cmDumpMsgDb        ARGS((CmElmntDef **msgDef));
PRIVATE S16  cmPrntElmntDb ARGS((CmElmntDef *elmntDef));
#endif /* CM_ASN_DBG */

#ifdef CM_ASN_DBG
#ifndef CM_ASN_SS
PUBLIC Void cmPrntSBuf  ARGS((CmAsnMsgCp *msgCp));
#endif /* CM_ASN_SS */
#endif /* CM_ASN_DBG */

#ifdef CM_ASN_NO_INDEF_LEN
PRIVATE U8   cmFindNumOctets      ARGS((U32          value));
PRIVATE S16  cmRmvIndefLenMsg     ARGS((CmAsnMsgCp  *msgCp));
PRIVATE S16  cmRmvIndefLenElmnt   ARGS((CmAsnMsgCp  *msgCp, Bool  tagType, U32 *length));
#endif /* CM_ASN_NO_INDEF_LEN */

#ifdef CM_ASN_DBG

/* ------------------------------------------------------------- */

/*
*
*       Fun:   cmPrntElmntDb 
*
*       Desc:  This prints a single the message database element
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmPrntElmntDb 
(
CmElmntDef *elmntDef               /* message definition */
)
#else
PUBLIC S16 cmPrntElmntDb (elmntDef)
CmElmntDef *elmntDef;              /* message definition */
#endif
{
   Txt        prntBuf[255];        /* Buffer to print */
   S16        i;                   /* counter */

   TRC2(cmPrntElmntDb)

   SPrint("\n");
#ifdef CM_ASN_DBG
   sprintf(prntBuf,"   Description: %s\n", elmntDef->str);
   SPrint(prntBuf);
#endif /* CM_ASN_DBG */

#ifdef ALIGN_64BIT
   sprintf(prntBuf, "   Tag        : 0x%x, 0x%x, 0x%x, 0x%x\n",
           elmntDef->tag.val[0], elmntDef->tag.val[1], elmntDef->tag.val[2], 
           elmntDef->tag.val[3]);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Min. Length: %ld\n",elmntDef->minLen);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Max. Length: %ld\n",elmntDef->maxLen);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Db Size    : %ld\n",elmntDef->dbSize);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Ev Size    : %ld\n",elmntDef->evSize);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Repeat Cntr: %ld\n", (Cntr) elmntDef->repCntr);
   SPrint(prntBuf);
#else
   sprintf(prntBuf, "   Tag        : 0x%x, 0x%x, 0x%x, 0x%x\n",
           elmntDef->tag.val[0], elmntDef->tag.val[1], elmntDef->tag.val[2], 
           elmntDef->tag.val[3]);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Min. Length: %ld\n",elmntDef->minLen);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Max. Length: %ld\n",elmntDef->maxLen);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Db Size    : %ld\n",elmntDef->dbSize);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Ev Size    : %ld\n",elmntDef->evSize);
   SPrint(prntBuf);
   sprintf(prntBuf, "   Repeat Cntr: %ld\n", (Cntr) elmntDef->repCntr);
   SPrint(prntBuf);
#endif /* ALIGN_64BIT */

   if (elmntDef->flagp != NULLP)
   {
      sprintf(prntBuf, "   Flag       : 0x%lx\n", (PTR) *elmntDef->flagp);
      SPrint(prntBuf);
   }
   else
   {
      SPrint("   Flag       : NULLP\n");
   }

   /* print the enumerated list */
   if (elmntDef->enumLst != NULLP)
   {
      sprintf(prntBuf, "   Enum List  :");
      SPrint(prntBuf);

      for (i=0; i < (S16) elmntDef->enumLst[0]; i++)
      {
         sprintf(prntBuf, " 0x%x",elmntDef->enumLst[i+1]);
         SPrint(prntBuf);
      } 

      SPrint("\n");
   }

   sprintf(prntBuf, "   Escape Func: 0x%lx\n", (PTR) elmntDef->func);
   SPrint(prntBuf);
   SPrint("\n");

   RETVALUE(ROK);
 
} /* cmPrntElmntDb */


/*
*
*       Fun:   cmDumpMsgDb 
*
*       Desc:  This function parses the message database and
*              calls routines to print the element defintions
*              in the message database
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC Void cmDumpMsgDb 
(
CmElmntDef **msgDef               /* message definition */
)
#else
PUBLIC Void cmDumpMsgDb (msgDef)
CmElmntDef **msgDef;              /* message definition */
#endif
{
   CmElmntDef **dbPtr;            /* database pointer */
   CmElmntDef *elmnt;             /* database element pointer */

   TRC2(cmDumpMsgDb)

   /* initialize the database pointer to the message start */
   dbPtr = msgDef;

   while (*dbPtr != NULLP)
   {
      /* get the current element from the database pointer */
      elmnt = *dbPtr;

      cmPrntElmntDb(elmnt);
      dbPtr++;

   } /* end while */

   RETVOID;

} /* cmDumpMsgDb */

#endif /* CM_ASN_DBG */


/*
*
*       Fun:   cmCpyUnrecogElmnt
*
*       Desc:  This function copies a unrecognized element to a
*              message buffer
*             
*       Ret:   ROK 
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/

#ifdef ANSI
PRIVATE S16 cmCpyUnrecogElmnt
(
CmAsnMsgCp   *msgCp,  /* message control pointer */
MsgLen       *bytes   /* number of bytes copied to the user buffer */
)
#else
PRIVATE S16 cmCpyUnrecogElmnt (msgCp, bytes)
CmAsnMsgCp   *msgCp;  /* message control pointer */
MsgLen       *bytes;  /* number of bytes copied to the user buffer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   MsgLen       lfLen;          /* Length of length field */
   MsgLen       elmntLen;       /* length of the set */
   MsgLen       msgLen;         /* length of the set */
   Bool         elmntEoc;       /* eoc flag of the set */
   MsgLen       elmntSize;      /* element size */
   Data         *pData;         /* static buffer pointer */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmCpyUnrecogElmnt)

   /* initialize the number of bytes copied */
   if (bytes != NULLP)
   {
      *bytes = 0;
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* now get the length of the set */
   if ((ret = cmFindLen(msgCp, tagSize, &elmntLen, &elmntEoc, &lfLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get the size of the element */
   elmntSize = (elmntEoc == TRUE) ?
     (tagSize + lfLen + elmntLen + EOC_TAG_LEN):(tagSize + lfLen + elmntLen);

   switch(msgCp->cfg)
   {
      case CM_ASN_GEN_ERR:
      {
         CM_ASN_ERR(msgCp, CM_ASN_EXTRA_PARAM, NULLP);
         RETVALUE(RFAILED);
      }
      case CM_ASN_DROP_ELMNTS:
      /* cm_asn_c_001.main_27: Adding missing ueAction type*/
      case CM_ASN_IGN_ELMNTS: 
      {
         /* drop the unrecongnized element */

#ifdef CM_ASN_SS

         /* allocate a static buffer */

         if (SGetSBuf(msgCp->region, msgCp->pool, &pData, elmntSize)
               != ROK)
         {
            CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
            RETVALUE(RFAILED);
         }

         /* strip of the bytes from the begining of the message buffer */

         ret = SRemPreMsgMult(pData, (MsgLen)elmntSize, msgCp->mBuf);

         /* throw away the static buffer */
         (Void) SPutSBuf(msgCp->region, msgCp->pool, pData, elmntSize);  

#else
         CM_ASN_REM_PRE_MULT(NULLP, elmntSize, msgCp, &ret);
#endif

         if (ret != ROK)
         {
            CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
            RETVALUE(RFAILED);
         }
         break;
      }

      case CM_ASN_PASS_ELMNTS:
      {
         /* pass the unrecongnized element to the user */

         /* allocate a static buffer */

         if (SGetSBuf(msgCp->region, msgCp->pool, &pData, (Size) elmntSize)
               != ROK)
         {
            CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
            RETVALUE(RFAILED);
         }

         /* strip of the bytes from the begining of the message buffer */
#ifdef CM_ASN_SS
         ret = SRemPreMsgMult(pData, (MsgLen)elmntSize, msgCp->mBuf);
#else
         CM_ASN_REM_PRE_MULT(pData, elmntSize, msgCp, &ret);
#endif

         if (ret != ROK)
         {
            (Void) SPutSBuf(msgCp->region,msgCp->pool,pData,(Size) elmntSize);  
            CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
            RETVALUE(RFAILED);
         }

         /* get a new message buffer if not already allocated */
         if (msgCp->uBuf == NULLP)
         {
            if ((ret = SGetMsg(msgCp->region, msgCp->pool, 
                                              &msgCp->uBuf)) != ROK)
            {
               (Void) SPutSBuf(msgCp->region, msgCp->pool, pData, 
                               (Size) elmntSize);  
               CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
               RETVALUE(RFAILED);
            }
         }

         /* copy the static encoded string to message buffer */
         if ((ret = SAddPstMsgMult(pData, elmntSize, msgCp->uBuf)) != ROK)
         {
            /* throw away the static buffer */
            (Void) SPutSBuf(msgCp->region, msgCp->pool, pData,(Size) elmntSize);
            (Void)SFndLenMsg (msgCp->uBuf, &msgLen);

            if (msgLen == 0)
            {
               (Void) SPutMsg(msgCp->uBuf);
               msgCp->uBuf = NULLP;
            }

            CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
            RETVALUE(RFAILED);
         }

         /* throw away the static buffer */
         (Void) SPutSBuf(msgCp->region, msgCp->pool, pData, (Size) elmntSize);  

         break;
      }

      default:
         /* unknown configuration */
         RETVALUE(RFAILED);

   } /* end switch */

   /* indicate the number of bytes copied */
   if (bytes != NULLP)
   {
      *bytes = elmntSize;
   }

   RETVALUE(ROK);

} /* cmCpyUnrecogElmnt */ 

#ifdef CM_ASN_DBG
#ifndef CM_ASN_SS

/*
*
*       Fun:   cmPrntSBuf 
*
*       Desc:  This routines prints the static buffer associated
*              message control point. 
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC Void cmPrntSBuf 
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PUBLIC Void cmPrntSBuf (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   Txt  prntBuf[255];        /* Buffer to print */
   U16  i;                   /* counter */
   U16  j;                   /* counter */
   U8   data;                /* data */
   U16  tCount;              /* temporary counter */
   Data *msg;                /* message pointer */
 
   TRC2(cmPrntSBuf)

   if (msgCp == NULLP)
   {
      RETVOID;
   }

   tCount = msgCp->sBuf.size;
   msg    = (Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx);
 
   SPrint("\n");
   sprintf(prntBuf,"   message size: %4ld\n", msgCp->sBuf.size);
   SPrint(prntBuf);
   SPrint("   ");
 
   if (msgCp->sBuf.size == 0)
   {
      SPrint("<none>");
      return;
   }
 
   j = 0;
   i = 0;
 
   while (1)
   {
      if (tCount > 0)
      {
         data = msg[i];
 
         if (j < 16)
         {
            /* print the hex values first */
            sprintf(prntBuf, "%02x ", data);
            SPrint(prntBuf);
         }
         else
         {
            j = 0;
 
            /* for the next line, print three preceeding spaces */
            SPrint("\n   ");
 
            continue;
         }
 
      }
      else
      {
         break;
      }
 
      i++;
      j++;
 
      tCount--;
 
   } /* end while */
 
   SPrint("\n");
   SPrint("\n");
 
} /* cmPrntSBuf */

#endif /* CM_ASN_SS */
#endif /* CM_ASN_DBG */


/*
*
*       Fun:   cmInitAsnMsgCp 
*
*       Desc:  This function initializes the ASN.1 message control 
*              structure 
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmInitAsnMsgCp 
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PRIVATE S16 cmInitAsnMsgCp (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   TRC2(cmInitAsnMsgCp)

   /*cm_asn_c_001.main_23:- Initialise the CmAsnMsgCp msg. Uninitialized 
   value was causing a core dump in a wrong TC-CONTINUE.fix for ccpu00073999*/

   cmMemset((U8 *)msgCp, 0, (PTR)sizeof(CmAsnMsgCp));

   msgCp->edFlag     = 0;             /* encode or decode */
   msgCp->octEnum    = FALSE;         /* not an enumerated type */
   msgCp->flagp      = NULLP;         /* protocol flag */
   msgCp->proType    = 0;             /* protocol flag */
   msgCp->mBuf       = NULLP;         /* msg buffer */
   msgCp->uBuf       = NULLP;         /* msg buffer */
   msgCp->region     = 0;             /* region */
   msgCp->pool       = 0;             /* pool */
   msgCp->cfg        = 0;             /* configuration */

   msgCp->sBuf.bufP  = NULLP;         /* pointer to static buffer */
   msgCp->sBuf.stIdx = 0;             /* start index */
   msgCp->sBuf.size  = 0;             /* size */
   msgCp->sBuf.max   = 0;             /* maximum size */

   msgCp->evntStr    = NULLP;         /* event structure */ 
   msgCp->elmntDef   = NULLP;         /* token list def in database */
   msgCp->err        = NULLP;         /* error definition */

  /*cm_asn_c_001.main_23:- Initialise the CmAsnMsgCp msg. Uninitialized 
    value was causing a core dump in a wrong TC-CONTINUE.fix for ccpu00073999*/
   
   msgCp->choiceResInSet = FALSE;     /* Used in cmDecSet() to indicate that choice */
   				      /* is present in set*/

#ifdef CM_ASN_NO_INDEF_LEN
   msgCp->sBuf.ctIdx = 0;             /* current index */
   msgCp->crntEncLen = 0;             /* Current encoded length */
   msgCp->usrActReq  = FALSE;         /* User action required flag */
#endif /* CM_ASN_NO_INDEF_LEN */

   RETVALUE(ROK);

} /* cmInitAsnMsgCp */


/*
*
*      Fun:   cmIncPtr
*
*      Desc:  increments address pointer, "addr", by "len".
*             used to access structures during encoding/decoding.
*
*      Ret:   ROK      - increment successful
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PUBLIC  S16 cmIncPtr
(
PTR *addr,     /* address pointer */
U32 len        /* length by which to increment the pointer */
)
#else
PUBLIC  S16 cmIncPtr(addr, len)
PTR *addr;     /* address pointer */
U32 len;       /* length by which to increment the pointer */
#endif
{

   TRC2(cmIncPtr)

   *addr += len;

   RETVALUE(ROK);

} /* end of cmIncPtr */


/*
*
*       Fun:   cmSkipElmnt
*
*       Desc:  This function skips the elements in event structure 
*              and increments the database pointer appropriately. 
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmSkipElmnt 
(
CmAsnMsgCp  *msgCp    /* message control pointer */
)
#else
PRIVATE S16 cmSkipElmnt (msgCp)
CmAsnMsgCp  *msgCp;   /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */

   TRC2 (cmSkipElmnt)

   /* get the element defintion for the current element */
   elmntDef = *msgCp->elmntDef;

   if (elmntDef->type == TET_TAG)
   {
      /* increment the database pointer */
      msgCp->elmntDef++;

      (Void) cmSkipElmnt(msgCp);

      /* skip over the end of constructor element */
      msgCp->elmntDef++;

   }
   else
   {

      /* increment the database pointer */
      msgCp->elmntDef = 
             (CmElmntDef **) ((PTR) msgCp->elmntDef + elmntDef->dbSize); 

      /* increment the event structure pointer */
      msgCp->evntStr = 
             (TknU8 *) ((PTR) msgCp->evntStr + elmntDef->evSize); 
   }

   RETVALUE(ROK);

} /* cmSkipElmnt */


/*
*
*      Fun:   cmEncTag
*
*      Desc:  Copies the ASN.1 tag to an array, pointer to which
*             is passed.
*
*      Ret:   Void
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PRIVATE Void cmEncTag
(
CmElmntDef *elmntDef,     /* element defintion */
U8         *pkArray,      /* array where the tag is copied */
U16        *numBytes      /* number of bytes copied */
)
#else
PRIVATE Void cmEncTag(elmntDef, pkArray, numBytes)
CmElmntDef *elmntDef;     /* element defintion */
U8         *pkArray;      /* array where the tag is copied */
U16        *numBytes;     /* number of bytes copied */
#endif
{

   U8     i;              /* counter */

   TRC2(cmEncTag)

   /* copy the tag into the array */
   for (i=0; i < elmntDef->tag.len; i++)
   {
      pkArray[i] = elmntDef->tag.val[i];
   }

   /* return the number of bytes copied */
   *numBytes = i;

} /* end of cmEncTag */


/*
*
*      Fun:   cmChkTag
*
*      Desc:  Checks the tag value against the database defintion of
*             the element
*
*      Ret:   ROK (tag matches) 
*             RFAILED (otherwise)
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PUBLIC S16 cmChkTag
(
CmAsnMsgCp *msgCp,        /* message control point */
U8         *pkArray,      /* array where the tag is present */
U8         size,          /* size of the tag */
Bool       *skipElmnt     /* TRUE is element needs to be skipped */
)
#else
PUBLIC S16 cmChkTag(msgCp, pkArray, size, skipElmnt)
CmAsnMsgCp *msgCp;        /* message control point */
U8         *pkArray;      /* array where the tag is copied */
U8         size;          /* size of the tag */
Bool       *skipElmnt;    /* TRUE is element needs to be skipped */
#endif
{
   U8          i;         /* counter */
   CmElmntDef  *elmntDef; /* database defintion for this element */
   Bool        err;       /* mismatch in tag */
   S16         ret;
   TRC2(cmChkTag)

   /* get the element defintion from the message control point */
   elmntDef = *msgCp->elmntDef;

   ret = ROK;
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      *skipElmnt = TRUE;
      RETVALUE(RFAILED);
   }
   /* make the error flag flase */
   err = FALSE;

   /* check for length mismatch */
   if (size != elmntDef->tag.len)
   {
      err = TRUE;
   }

   /* copy the tag into the array */
   for (i=0; i < elmntDef->tag.len; i++)
   {
      /* check for tag value mismatch */
      if (pkArray[i] != elmntDef->tag.val[i])
      {
         err = TRUE;
         break;
      }

   } /* end for */

   /* number of bytes copied in the array is the length of the tag in the
      element defintion for this element */ 

   if (err == FALSE)
   {
      RETVALUE(ROK);
   }
   else
   {
      /* there was an error, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         *skipElmnt = FALSE;
      }
      else
      {
         /* element is not mandatory, can be skipped */
         *skipElmnt = TRUE;
      }

      RETVALUE(RFAILED);
   }

} /* end of cmChkTag */


/*
*
*      Fun:   cmGetTag
*
*      Desc:  Copies the ASN.1 tag to an array, pointer to which
*             is passed.
*
*      Ret:   ROK (Decode was successful)
*             RFAILED (otherwise)
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PUBLIC S16 cmGetTag
(
CmAsnMsgCp *msgCp,        /* message control point */
U8         *pkArray,      /* array where the tag is copied */
U16        offset,        /* Offset in the mBuf */
U8         *size          /* size of the tag */
)
#else
PUBLIC S16 cmGetTag(msgCp, pkArray, offset, size)
CmAsnMsgCp *msgCp;        /* message control point */
U8         *pkArray;      /* array where the tag is copied */
U16        offset;        /* offset in the mBuf */
U8         *size;         /* size of the tag */
#endif
{
   U8          i;         /* counter */
   S16         ret;       /* return values */

   TRC2(cmGetTag)
   
   i = 0;    /* Intialize the array index */

#ifdef CM_ASN_SS
   ret = SExamMsg(&pkArray[i], msgCp->mBuf, (offset + i));
#else
   CM_ASN_EXAM_MSG(&pkArray[i], msgCp, (offset + i), &ret);
#endif

   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Check if more than one tag byte */
   if ((pkArray[i] & LONG_TAG_IND) == LONG_TAG_IND)
   {
      /* more than one tag byte, go through the loop */

      i++;

      while (i < MAX_TAG_LEN)
      {

#ifdef CM_ASN_SS
         ret = SExamMsg(&pkArray[i], msgCp->mBuf, (offset + i));
#else
         CM_ASN_EXAM_MSG(&pkArray[i], msgCp, (offset + i), &ret);
#endif

         if (ret != ROK)
         {
            RETVALUE(RFAILED);
         }

         if (!(pkArray[i] & TAG_MORE_BIT))
         {
            /* no more tag bytes */
            break;
         }
 
         i++;           /* increment the number of tag bytes received */

      };

   } /* end if */

   if (i == MAX_TAG_LEN)
   {
      RETVALUE(RFAILED);
   }

   /* indicate the size of the tag to the caller */
   *size = (i+1);          /* one more than the array index */ 
   
   RETVALUE(ROK);

} /* cmGetTag */


/*
*
*      Fun:   cmGetLen
*
*      Desc:  Copies the ASN.1 length to an length var, pointer to which
*             is passed and returns the size of the length field too.
*             For EOC, returns 1.
*
*      Ret:   ROK (Decode was successful)
*             RFAILED (otherwise)
*
*      Notes: Finds length of types types not encoded as EOC format.
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PRIVATE S16 cmGetLen
(
CmAsnMsgCp *msgCp,    /* message buffer */
MsgLen     offset,    /* offset into the message buffer */
MsgLen     *len,      /* array where the tag is copied */
MsgLen     *size,     /* size of the length field */
Bool       *eocFlag   /* eoc flag */
)
#else
PRIVATE S16 cmGetLen(msgCp, offset, len, size, eocFlag)
CmAsnMsgCp *msgCp;    /* message buffer */
MsgLen     offset;    /* offset into the message buffer */
MsgLen     *len;      /* array where the tag is copied */
MsgLen     *size;     /* size of the length field */
Bool       *eocFlag;  /* eoc flag */
#endif
{
   MsgLen      i;         /* counter */
   MsgLen      totLen;    /* total length of the message */
   Data        tmpData;  /* temporary data */
   MsgLen      tmpLen;    /* temporary length of length field */
   U32         primLen;   /* primitive length */
   S16         ret;       /* return value */
#ifdef REJ_ZERO_LEN
   CmElmntDef  *elmntDef; /* Element definition */
#endif

   /* array for unpacking length */
   U8        unPkArray[MAX_LEN_BYTES];


   TRC2(cmGetLen)
   
   i        = 0;       /* Intialize the array index */
   primLen  = 0;       /* Initialize the length of the element */
   *eocFlag = FALSE;   /* not a eoc form of length encoding */

#if CM_ASN_SS
   /* get the total length of the message */
   (Void)SFndLenMsg (msgCp->mBuf, &totLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &totLen);
#endif

   /* Examine the first byte of the length field */

#ifdef CM_ASN_SS
   ret = SExamMsg(&tmpData, msgCp->mBuf, offset);
#else
   CM_ASN_EXAM_MSG(&tmpData, msgCp, offset, &ret);
#endif

   if (ret != ROK)
   {
       RETVALUE(RFAILED);
   }

   /* check for the length encoding format */

   /* Check for single byte length encoding */

   if (tmpData < EOC_FLAG)
   {
      /* length is contained in a single byte */

      /* if element length is greater than the message length, error */

      if ( (totLen - (offset + 1)) < (MsgLen)tmpData)
      {
        RETVALUE(RFAILED);
      }

/* cm_asn_c_001.main_25: Reject parameters with 0 length for type
                         TET_U8 while decoding */
#ifdef REJ_ZERO_LEN
      elmntDef = *msgCp->elmntDef;        /* get the element defintion */

      if((tmpData == 0) && (elmntDef->type == TET_U8) && (msgCp->edFlag == DECODE_ELMNT))
      {
        RETVALUE(RFAILED);
      }
#endif /* REJ_ZERO_LEN */

      /* indicate the length and the length field size to caller */
      *len = (MsgLen)tmpData;
      *size = 1;
      
      RETVALUE(ROK);

   } /* end if */

   /* Check for Long form of length encoding */

   if (tmpData > EOC_FLAG)
   {
      /* long form of length encoding */

      /* get the number of bytes in the length field */
      tmpLen = (U8)(tmpData & ~EOC_FLAG);

      /* length field is sum of first byte & no. of bytes in length field */
      *size = (tmpLen +1);
      if (*size > (MAX_LEN_BYTES + 1))
      {
         RETVALUE(RFAILED);
      }

      /* Copy the length field into an array, MSB at index 0 */
#ifdef CM_ASN_SS
      ret = SCpyMsgFix(msgCp->mBuf, (offset + 1), tmpLen, unPkArray, &i); 
#else
      CM_ASN_CPY_MSG_FIX(msgCp, (offset + 1), tmpLen, unPkArray, &i, &ret);
#endif

      /* check for errors */
      if ((ret == RFAILED) || (i != tmpLen))
      {
         /* error in length decoding */
         RETVALUE(RFAILED);
      }

      /* copy the length from the array to a variable. The type of
         primLen (U32) imposes a restriction on the size of length */

      for(i=0;i < tmpLen;i++)
      {
         primLen = ((primLen << 8) | unPkArray[i]);
      }
      
      /* if element length is greater than the message length, error */

      if ( (totLen - (offset + tmpLen + 1)) < (MsgLen)primLen)
      {
        RETVALUE(RFAILED);
      }

      *len = primLen;

      RETVALUE(ROK);

   } /* end if */

   /* Indefinate form of length encoding */

   *size    = 1;       /* indicate the length of length field as 1 */ 
   *len     = 0;       /* Indicate the element length as 0 */
   *eocFlag = TRUE;    /* indicate a indefinate form of length encoding */

   RETVALUE(ROK);

} /* cmGetLen */


/*
*
*      Fun:   cmEncLen
*
*      Desc:  encodes the ASN.1 length in an array, pointer to which
*             is passed.
*
*      Ret:   Void
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PUBLIC  S16 cmEncLen
(
CmElmntDef *elmntDef,     /* element defintion */
U32        len,           /* Length of the element */
Data       *pkArray,      /* array where the tag is copied */
U16        *numBytes      /* number of bytes copied */
)
#else
PUBLIC  S16 cmEncLen(elmntDef, len, pkArray, numBytes)
CmElmntDef *elmntDef;     /* element defintion */
U32        len;           /* Length of the element */
Data       *pkArray;      /* array where the tag is copied */
U16        *numBytes;     /* number of bytes copied */
#endif
{
   U32     tmpLen;      /* temporary length variable */
   U8      i;           /* counter */

   TRC2(cmEncLen)

   /* Length should lie in the minimum amd maximum length defined for
      this element in the element database definition */

   /* avoid checking of length for TET_INT_RNG token type, as they use the 
    * fields minLen/maxLen differently.
    */
   if (elmntDef->type != TET_INT_RNG)
   {
      if ((MsgLen)elmntDef->minLen != MIN_LEN_NA)
      {
         if (len < (U32) elmntDef->minLen)
         {
            RETVALUE(RFAILED);
         }
      }
 
      if ((MsgLen)elmntDef->maxLen != MAX_LEN_NA)
      {
         if (len > (U32) elmntDef->maxLen)
         {
            RETVALUE(RFAILED);
         }
      }
   }

   /* encode the length */

   /* check the maximum value that can fit in a signed byte value */
   if (len <= MAX_SBYTE_VAL)
   {
      /* length will fit in one byte */

      pkArray[0] = (Data)len;
      *numBytes = 1;

   }
   else 
   {
      /* length will not fit in one byte */

      tmpLen = len;                /* store the length */ 
      i = 0;                       /* initialize the loop counter */

      /* find the number bytes to encode in length */
      while (tmpLen != 0)
      {
         tmpLen = (tmpLen >> NUMBITS_BYTE);
         i++; 
      }

      *numBytes = (i+1);  /* indicate number of bytes, include 1st byte also */
      tmpLen = len;       /* store the length */ 

      /* encode the length now. The first byte contains the EOC_FLAG
         and the number of bytes in the length field */

      /* Check we should include the first byte here or not */

      pkArray[0] = (EOC_FLAG | i);

      for (; i > 0; i--)
      {
         pkArray[i] = (tmpLen & 0xff);        /* copy the LSB */
         tmpLen = (tmpLen >> NUMBITS_BYTE);   /* shift out the LSB */
      }

   } /* end if */

   RETVALUE(ROK);

} /* end of cmEncLen */


/*
*
*      Fun:   cmChkEnum
*
*      Desc:  tries to locate specified value in specified enumerated list.
*
*      Ret:   ROK          - value found in enumerated list
*             RFAILED      - value not found in enumerated list
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PRIVATE S16 cmChkEnum
(
U8 val,                    /* value */
CmElmntDef *elmntDef       /* token definition */
)
#else
PRIVATE S16 cmChkEnum(val, elmntDef)
U8 val;                    /* value */
CmElmntDef *elmntDef;        /* token definition */
#endif
{
   U8 count;          /* number of elements in enumerated list */
   U8 i;              /* counter */

   TRC2(cmChkEnum)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (!(elmntDef->enumLst))
   {
      CMASNLOGERROR(ERRCLS_DEBUG, ECMASN001, (ErrVal)0, 
                         "cmChkEnum() : Null list pointer");
   }
#endif

   /* The first element of the array "enumLst" specifies the number
      of entries in enumerated list */

   count = elmntDef->enumLst[0];

   /* try to find a match for value in enumerated list */
   for (i = 1; i <= count; i++)
   {
      if (val == elmntDef->enumLst[i])
      {
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);

} /* end of cmChkEnum */


/*
*
*      Fun:   cmDecChkFlag
*
*      Desc:  This routine checks if an element is defined for the
*             protocol being defined.
*
*      Ret:   ROK       - element defined by the protocol
*             RFAILED   - element is not defined for the protocol. 
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PRIVATE S16 cmDecChkFlag
(
CmAsnMsgCp  *msgCp    /* message control pointer */
)
#else
PRIVATE S16 cmDecChkFlag(msgCp)
CmAsnMsgCp  *msgCp;   /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   U8           flag;           /* protocol flag */

   TRC2(cmDecChkFlag)

   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 

   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check if the element is defined for this protocol */ 
   if (flag == ELMNT_INV)
   {
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of cmDecChkFlag */


/*
*
*      Fun:   cmChkEncElmnt
*
*      Desc:  This routine checks whether the element defintion in the
*             database supports the indicated protocol. It also checks
*             for a mandatory element which might be missing.
*
*      Ret:   ROK       - element present and defined by the protocol
*             RFAILED   - element invalid and present or mandatory and missing
*             RSKIP     - element optional or invalid and not present
*
*      Notes: None
*
*      File:  cm_asn.c
*
*/
 
#ifdef ANSI
PRIVATE S16 cmChkEncElmnt
(
U8          flag,     /* flag for the element */
CmAsnMsgCp  *msgCp,   /* message control pointer */
TknU8       *evntStr  /* event structure pointer */
)
#else
PRIVATE S16 cmChkEncElmnt(flag, msgCp, evntStr)
U8          flag;     /* flag for the element */
CmAsnMsgCp  *msgCp;   /* message control pointer */
TknU8       *evntStr; /* event structure pointer */
#endif
{
   Bool         pres;        /* Element present flag */
   CmElmntDef **elmntDef;    /* element definition */

   TRC2(cmChkEncElmnt)

   elmntDef = msgCp->elmntDef;

   /* if the type is a tag type, then the event structure is */
   /* associated only with the enclosed type, so for finding that */
   /* out, we will need to increment the database pointer. Note that */
   /* the message control point database pointer is unaffected by this */
   /* increment */

   if ((*elmntDef)->type == TET_TAG)
   {
      elmntDef++;
   }

   /* TknStrXL type requires special treatment because of the difference
      in type definition */
 
   if ((*elmntDef)->type == TET_STRXL || 
       (*elmntDef)->type == TET_STRXL_MEM ||
       (*elmntDef)->type == TET_IA5STRXL || 
       (*elmntDef)->type == TET_IA5STRXL_MEM)
   {
      pres = ((TknStrXL *)evntStr)->pres;
   }
   else
   {
      pres = evntStr->pres;
   }

   /* check if the element is defined for this protocol */ 
   if (flag == ELMNT_INV)
   {
      /* element not defined for this protocol */

      /* check if the element is present in the event structure */
      if (pres == TRUE)
      {
         /* parameter is not defined for the protocol */
         CM_ASN_ERR(msgCp, CM_ASN_UNDEF_PARAM, NULLP);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element not present. Skip the element in event structure and 
            increments the data base pointer appropriately */

         RETVALUE(RSKIP);
      }

   } /* end if */  


   /* if element not present and is mandatory, return error */

   if (pres == FALSE)
   {
      /* check if element is mandatory */
      if (flag == ELMNT_MAND)
      {
         /* mandatory element is missing */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, NULLP);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element not mandatory, skip it */
         RETVALUE(RSKIP);
      }

   } /* end if */

   RETVALUE(ROK);

} /* end of cmChkEncElmnt */


/*
*
*       Fun:   cmEncMsg 
*
*       Desc:  This function encodes the message by encoding all the token
*              elements it contains.  
*
*       Ret:   ROK  (encoding successful)
*              RFAILED (failed, general)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncMsg 
(
TknU8       *evntStr,     /* pointer to the event structure */
Buffer      *mBuf,        /* ASN.1 encoded message buffer */
U8          proType,      /* protocol type */ 
CmElmntDef  **elmntDef,   /* message defintion */
Region      region,       /* region to allocate the memory from */
Pool        pool,         /* pool to allocate the memory from */
CmAsnErr    *err,         /* error to be returned back to the caller */ 
Buffer      *uBuf         /* user buffer containing any unrecognized elements */
)
#else
PUBLIC S16 cmEncMsg (evntStr, mBuf, proType, elmntDef, region, pool, err, uBuf)
TknU8       *evntStr;     /* pointer to the event structure */
Buffer      *mBuf;        /* ASN.1 encoded message buffer */
U8          proType;      /* protocol type */ 
CmElmntDef  **elmntDef;   /* message defintion */
Region      region;       /* region to allocate the memory from */
Pool        pool;         /* pool to allocate the memory from */
CmAsnErr    *err;         /* error to be returned back to the caller */ 
Buffer      *uBuf;        /* user buffer containing any unrecognized elements */
#endif
{
   CmAsnMsgCp   msgCp;         /* message control point */
   CmAsnMsgCp   *cpPtr;        /* message control point */
   S16          ret;           /* return value */

   TRC2(cmEncMsg)

   /* update the msgCp pointer */
   cpPtr = &msgCp;

   /* Initialize the message control structure */
   (Void) cmInitAsnMsgCp(&msgCp);

   /* update the message control structure with passed values */

   msgCp.evntStr  = evntStr;      /* event structure */
   msgCp.mBuf     = mBuf;         /* pointer to the allocated message buffer */
   msgCp.proType  = proType;      /* the protocol type */
   msgCp.elmntDef = elmntDef;     /* message defintion */
   msgCp.region   = region;       /* region for the memory */
   msgCp.pool     = pool;         /* pool for the memory */
   msgCp.err      = err;          /* error structure pointer */
   msgCp.uBuf     = uBuf;         /* pointer to the allocated message buffer */

   msgCp.edFlag   = ENCODE_ELMNT; /* encode the message */

#ifdef CM_ASN_NO_INDEF_LEN
   /* Save the event structure and element definition pointers for further use */
   msgCp.strtEvPtr    = evntStr;
   msgCp.strtElmntPtr = elmntDef;
#endif /* CM_ASN_NO_INDEF_LEN */

#ifndef CM_ASN_SS

   if (SGetSBuf(msgCp.region, msgCp.pool, &msgCp.sBuf.bufP, 
                CM_ASN_SBUF_SIZE) != ROK)
   {
      CM_ASN_ERR(cpPtr, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(NULLP);
   }

   msgCp.sBuf.max   = CM_ASN_SBUF_SIZE;   /* max size of the sBuf */
   msgCp.sBuf.stIdx = 0;                  /* start index */
   msgCp.sBuf.size  = 0;                  /* size */

#endif /* CM_ASN_SS */

   /* encode all the elements of the messages. End of message is indicated
      by a null pointer */

   while(*msgCp.elmntDef != (CmElmntDef *)NULLP)
   {
      /* encode the element depending on the element type */
      if ((ret = cmEncElmnt(&msgCp)) != ROK)
      {

#ifndef CM_ASN_SS
         /* throw away the static buffer */
         (Void) SPutSBuf(msgCp.region, 
                         msgCp.pool, 
                         msgCp.sBuf.bufP, 
                         CM_ASN_SBUF_SIZE);
#endif

         RETVALUE(RFAILED);
      }

   } /* end while */

#ifdef CM_ASN_NO_INDEF_LEN
   msgCp.edFlag   = PROCESS_LENGTH; /* Process the indefinite length encoding */
 
   /* Use only short/long form of length encoding */
   if ((ret = cmRmvIndefLenMsg(&msgCp)) != ROK)
   {
      /* throw away the static buffer */
      (Void) SPutSBuf(msgCp.region, 
                      msgCp.pool, 
                      msgCp.sBuf.bufP, 
                      CM_ASN_SBUF_SIZE);
 
      RETVALUE(RFAILED);
   }
#endif /* CM_ASN_NO_INDEF_LEN */

#ifndef CM_ASN_SS
#ifndef CM_ASN_NO_INDEF_LEN
   /* copy the static encoded string to message buffer */
   ret = SAddPstMsgMult(
                  (Data *)(msgCp.sBuf.bufP + msgCp.sBuf.stIdx),
                  msgCp.sBuf.size,           /* size to be copied */
                  msgCp.mBuf                 /* destination */
                            );
#endif /* CM_ASN_NO_INDEF_LEN */

   /* throw away the static buffer anyway */
   (Void) SPutSBuf(msgCp.region, 
                   msgCp.pool, 
                   msgCp.sBuf.bufP, 
                   CM_ASN_SBUF_SIZE);

   if (ret != ROK)
   {
      RETVALUE(ret);
   }

#endif

   /* now add the passed unknown elements to the encoded */
   /* message buffer                                     */

   if (uBuf != NULLP)
   {
      ret = SCatMsg(mBuf,uBuf,M1M2);

      /* throw away the user buffer anyway */
      (Void) SPutMsg(uBuf);

#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         CMASNLOGERROR(ERRCLS_DEBUG, ECMASN002, (ErrVal)ERRZERO, 
               "cmEncMsg () Failed. Could not concatenate user buffer");
         CM_ASN_ERR(cpPtr, CM_ASN_RES_ERR, NULLP);  
         RETVALUE(RFAILED);
      }
#endif

   } /* end if */

   RETVALUE(ROK);

} /* end of cmEncMsg */


/*
*
*       Fun:   cmEncElmnt 
*
*       Desc:  This function invokes an appropriate encoding routine
*              depending on the element defintion. 
*
*       Ret:   ROK
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncElmnt
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PUBLIC S16 cmEncElmnt (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */
   S16          ret;           /* return value */

#ifdef CM_ASN_NO_INDEF_LEN
   MsgLen       strtEncLen;    /* Start of encoded length */
   MsgLen       endEncLen;     /* End of encoded length */
   U32          tmpLen;
#endif /* CM_ASN_NO_INDEF_LEN */

   TRC2 (cmEncElmnt)

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#ifdef CM_ASN_DBG
   cmPrntElmntDb(elmntDef);
#endif

   /* Start encoding the tokens */
   switch(elmntDef->type)
   {
      case TET_U8:
         msgCp->octEnum = FALSE;
         ret = cmEncOctetEnum(msgCp);    /* encode an octet */
         break;

      case TET_ENUM:
         msgCp->octEnum = TRUE;
         ret = cmEncOctetEnum(msgCp);    /* encode enumerated type */
         break;

      case TET_NULL:
         ret = cmEncNull(msgCp);     /* encode NULL type */
         break;

      case TET_STR4:
      case TET_STR12:
      case TET_STR32:
      case TET_STR64:
      case TET_STR256:
      case TET_STRXL:
      case TET_STRXL_MEM:
         ret = cmEncOctetStr(msgCp);  /* encode octet string type */
         break;

      case TET_IA5STR4:
      case TET_IA5STR12:
      case TET_IA5STR32:
      case TET_IA5STR64:
      case TET_IA5STR256:
      case TET_IA5STRXL:
      case TET_IA5STRXL_MEM:
         ret = cmEncIa5Str(msgCp);  /* encode IA5 string type */
         break;

      case TET_INT:
         ret = cmEncInteger(msgCp);   /* encode Integer type */
         break;
 
      case TET_INT_RNG:
         ret = cmEncIntRng(msgCp);   /* encode Integer type with range check*/
         break;
 
      case TET_OID:
         ret = cmEncOid(msgCp);       /* encode Object Identifier type */
         break;

      case TET_TAG:
         ret = cmEncTagType(msgCp);   /* encode tag type */
         break;

      case TET_BOOL:
         ret = cmEncBool(msgCp);      /* encode boolean type */
         break;

      case TET_BITSTR:
         ret = cmEncBitStr(msgCp);    /* encode bit string type */
         break;

      case TET_SEQ:
      case TET_SET:
         ret = cmEncSetSeq(msgCp);    /* encode set or a sequence type */
         break;

      case TET_SEQ_OF:
      case TET_SET_OF:

          /* encode repeatable set or a sequence */
         ret = cmEncSetSeqOf(msgCp);
         break;

      case TET_UNCONS_SEQ_OF:         
      case TET_UNCONS_SET_OF:
         /* encode repeatable set of or a seq of */
         ret = cmEncUnconsSetSeqOf(msgCp);
         break;

      case TET_CHOICE:

         /* encode a choice of the elements */
         ret = cmEncChoice(msgCp);
         break;

      case TET_ESC_PRIM:
      case TET_ESC_CONST:
 
         /* call user function to encode this type */
#if (ERRCLASS & ERRCLS_DEBUG)
         if (!elmntDef->func)
         {
            CMASNLOGERROR(ERRCLS_DEBUG, ECMASN003, (ErrVal)ERRZERO, 
                        "cmEncElmnt () Failed. Null User function pointer");
            CM_ASN_ERR(msgCp, CM_ASN_NO_FUNC, &elmntDef->tag);  
            RETVALUE(RFAILED);
         }
#endif

#ifdef CM_ASN_NO_INDEF_LEN
         strtEncLen = 0;
         endEncLen = 0;

         /* Store the current length to determine the number of octets the
            user function is going to encode */
#ifndef CM_ASN_SS
         CM_ASN_FND_LEN(msgCp, &strtEncLen);
#endif
         tmpLen = msgCp->crntEncLen;    /* Store the current encoded length */

         /* execute the user function */
         ret = (*elmntDef->func)(msgCp);

         msgCp->crntEncLen = tmpLen;    /* Restore the current encoded length */

#ifndef CM_ASN_SS
         CM_ASN_FND_LEN(msgCp, &endEncLen);
#endif
         if (msgCp->usrActReq)
         {
            msgCp->crntEncLen += elmntDef->maxLen;
         }
         else
         {
            msgCp->crntEncLen += endEncLen - strtEncLen;
         }
#else /* CM_ASN_NO_INDEF_LEN */

         /* execute the user function */
         ret = (*elmntDef->func)(msgCp);

#endif /* CM_ASN_NO_INDEF_LEN */
         break;

      default:

#if (ERRCLASS & ERRCLS_DEBUG)
           CMASNLOGERROR(ERRCLS_DEBUG, ECMASN004, (ErrVal)elmntDef->type, 
                           "cmEncElmnt () failed: Invalid element type");
#endif
           /* 'ret' initialised properly */
           ret = RFAILED;

           /* error cause filled */
           CM_ASN_ERR(msgCp, CM_ASN_INVLD_ELMNT, NULLP);

           break;

   } /* end switch */

   RETVALUE(ret);

} /* cmEncElmnt */


/*
*
*       Fun:   cmEncOctetEnum   
*
*       Desc:  This function encodes an octet or a enumerated type
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncOctetEnum 
(
CmAsnMsgCp  *msgCp    /* message control pointer */
)
#else
PUBLIC S16 cmEncOctetEnum (msgCp)
CmAsnMsgCp  *msgCp;   /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   U16           numBytes;       /* number of tag bytes copied */
   MsgLen       i;              /* message index */

   /* temporary array for encoding */
   Data         pkArray[OCTET_BUFSIZE];

   TRC2(cmEncOctetEnum)
 
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr = (TknU8 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* copy the length of the octet and increment the array index to point
      to the next byte */

   pkArray[i++] = (Data) SIZEOF_OCTET;

   /* if enumerated, validate it's value */
   if (msgCp->octEnum == TRUE)
   {
      if ((ret = cmChkEnum(evntStr->val, elmntDef)) != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_UNEXP_VAL, &elmntDef->tag);  
         RETVALUE(ret);
      }
   }

   /* copy the octet value and increment the index to indicate the 
      message length (one more than the array index) */

   pkArray[i++] = evntStr->val;

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to encode
      this primitive type */
   msgCp->crntEncLen += i;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the octet  in the message buffer */ 
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, (U16) i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncOctetEnum */


/*
*
*       Fun:   cmEncOctetStr   
*
*       Desc:  This function encodes an octet string 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncOctetStr 
(
CmAsnMsgCp   *msgCp         /* message control pointer */
)
#else
PUBLIC S16 cmEncOctetStr (msgCp)
CmAsnMsgCp   *msgCp;        /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   U8            flag;           /* protocol flag */
   U16           numBytes;       /* number of bytes copied */
   U16           len;            /* length of the string */
   U8           *val;            /* pointer to the string */
   U16           i;              /* counter */
   S16           ret;            /* return value */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE];

   TRC2(cmEncOctetStr)

   /* check for the version */
    
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, msgCp->evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   if (elmntDef->type == TET_STRXL || elmntDef->type == TET_STRXL_MEM)
   {
      len = ((TknStrXL *)msgCp->evntStr)->len;    /* length of the string */
      val = ((TknStrXL *)msgCp->evntStr)->val;    /* Pointer to the string */
   }
   else
   {
      len = (U16) ((TknStr *)msgCp->evntStr)->len;  /* length of the string */
      val = &((TknStr *)msgCp->evntStr)->val[0];    /* Pointer to the string */
   }

   /* Encode the length of the string */
   if ((ret = cmEncLen(elmntDef, (U32) len, &pkArray[i], &numBytes)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   i += numBytes;

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to 
    * encode this primitive type */
   msgCp->crntEncLen += i + len;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the tag and length */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* encode the value. Copy from the token string to the message buffer  */ 

#ifdef CM_ASN_SS
   ret = SAddPstMsgMult((Data *)val, (MsgLen)len, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT((Data *)val, len, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncOctetStr */


/*
*
*       Fun:   cmEncIa5Str   
*
*       Desc:  This function encodes an IA5 string 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncIa5Str 
(
CmAsnMsgCp   *msgCp         /* message control pointer */
)
#else
PUBLIC S16 cmEncIa5Str (msgCp)
CmAsnMsgCp   *msgCp;        /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   U8            flag;           /* protocol flag */
   U16           numBytes;       /* number of bytes copied */
   U16           len;            /* length of the string */
   U8           *val;            /* pointer to the string */
   U16           i;              /* counter */
   S16           ret;            /* return value */
   U16           idx;            /* loop counter */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE];

   TRC2(cmEncIa5Str)

   /* check for the version */
    
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, msgCp->evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
      RETVALUE(RFAILED);
   }

   /* get len and val part from token structure */
   if (elmntDef->type == TET_IA5STRXL || elmntDef->type == TET_IA5STRXL_MEM)
   {
      len = ((TknStrXL *)msgCp->evntStr)->len;    /* length of the string */
      val = ((TknStrXL *)msgCp->evntStr)->val;    /* Pointer to the string */
   }
   else
   {
      len = (U16) ((TknStr *)msgCp->evntStr)->len;  /* length of the string */
      val = &((TknStr *)msgCp->evntStr)->val[0];    /* Pointer to the string */
   }

   /* verify the data */
   for (idx = 0; idx < len; idx++)
   {
      /* MSB bit should not be set for IA5 character, IA5String consists of 
       * 128 (or 7 bits)  characters. */
      if (val[idx] & IA5_CHARMASK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_INV_IA5STR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 

   /* Encode the length of the string */
   if ((ret = cmEncLen(elmntDef, (U32) len, &pkArray[i], &numBytes)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   i += numBytes;

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to 
    * encode this primitive type */
   msgCp->crntEncLen += i + len;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the tag and length */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* encode the value. Copy from the token string to the message buffer  */ 

#ifdef CM_ASN_SS
   ret = SAddPstMsgMult((Data *)val, (MsgLen)len, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT((Data *)val, len, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncIa5Str */


/*
*
*       Fun:   cmEncNull   
*
*       Desc:  This function encodes a Null tag 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncNull 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncNull (msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   U16           numBytes;       /* number of tag bytes copied */
   MsgLen       i;              /* number of tag bytes copied */

   /* temporary array for encoding */
   Data         pkArray[OCTET_BUFSIZE];

   TRC2(cmEncNull)
 
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr = (TknU8 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* copy the length of the NULL and increment the index to indicate the 
      message length (one more than the array index) */

   pkArray[i++] = (Data) SIZEOF_NULL;

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to 
    * encode this primitive type */
   msgCp->crntEncLen += i;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the NULL type in the message buffer */ 
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncNull */


/*
*
*       Fun:   cmEncBitStr   
*
*       Desc:  This function encodes Bit string. 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: This function is used to encode 8 bit enumerated 
*              bit string. This routine does not check if the user 
*              provides a string like "234A7F000". The maximum number 
*              of trailing zero's should be 7 for the logic to work properly.
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncBitStr 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncBitStr (msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   U16          i;              /* counter */
   U16          j;              /* counter */
   S16          ret;            /* return value */
   U8           len;            /* length of the string */
   U8           flag;           /* protocol flag */
   U8           mask;           /* mask */
   TknStr       *str;           /* token string stricture - size regular */
   TknU8        *evntStr;       /* pointer to event structure */
   U16           numBytes;       /* number of bytes copied */
   U16          numBits;        /* number of bits in bit string */
   U16           ubitIndex;      /* index for unused bit octets */
   CmElmntDef   *elmntDef;      /* element definition */

   /* temporary array for encoding */
   Data         pkArray[BITSTR_BUFSIZE];

   TRC2(cmEncBitStr)

   /* check for the version */
    
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr = (TknU8 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   str = (TknStr *)msgCp->evntStr;      /* pointer to the event structure */

   len = str->len;                      /* length of the string */

   /* encode the length. One more than the string length. This is
      to take care of the octet conatining the number of unsued bits 
      Also assuming the length field is not more than one octet */

   pkArray[i++] = (len + 1);

   ubitIndex = i;    /* store the unused bit index */

   i++;              /* increment the index to go to the next byte */
   j=0;              /* Initialize the loop counter */

   /* encode the value. Copy from the token string to the array */

   while (len-- > 0)
   {
      pkArray[i++] = str->val[j++]; 
   }

   /* check the LSB which is set in the last byte. mark all bit's
      after that as unused */ 

   mask = 0x1;            /* set the mask */
   j = 0;                 /* initialize the loop counter */

   while (((mask & pkArray[i-1]) == 0) && (mask != 0x80))
   {
      mask = mask << 1;
      j++;
   }

   /* assuming "j" is a valid value between 0 - 7 */

   /* Number of bits in bit string. Ignoring trailing zero's */
   numBits = (((str->len - 1) << 3) + (NUMBITS_BYTE - j));

   /* check the length of the bit string */
   if ((numBits > elmntDef->maxLen) || (numBits < elmntDef->minLen))
   {
      CM_ASN_ERR(msgCp, CM_ASN_OUT_RANGE, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* Update the number of unused bits in the array */
   pkArray[ubitIndex] = (Data) j;

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to 
    * encode this primitive type */
   msgCp->crntEncLen += i;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the tag, length and value */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncBitStr */


/*
*
*       Fun:   cmEncSetSeq
*
*       Desc:  This function encodes the sequence or a set of tokens 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncSetSeq 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncSetSeq(msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   U16          numBytes;       /* number of bytes copied */
   U16          i;              /* counter */

#ifdef CM_ASN_NO_INDEF_LEN
   U32          subEncLen;      /* Adjusted encoded length */
#endif /* CM_ASN_NO_INDEF_LEN */

   /* temporary array for encoding */
   Data         pkArray[SEQ_BUFSIZE];

   TRC2(cmEncSetSeq)

   /* check for the version */
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr  = (TknU8 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* encode the length. EOC format */
   pkArray[i++] = EOC_LEN;
    
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* Increment the database pointer over seq start/set start defintion */
   msgCp->elmntDef++;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   /* get the pointer to the next element definition */
   elmntDef = *msgCp->elmntDef;

#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the curent length to adjust the constructor length later */
   subEncLen = msgCp->crntEncLen;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode all elements till end of sequence element is encountered */
   while (elmntDef->type != TET_SETSEQ_TERM)
   {
      /* encode the element depending on the element type */
      if ((ret = cmEncElmnt(msgCp)) != ROK)
      {
         /* the error code would already be set, do not do it again */
         RETVALUE(ret);
      }

      /* get the updated element defintion */
      elmntDef = *msgCp->elmntDef;

   } /* end while */

#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the length of the constructor type in the spare1 field of the
      set/sequence present token */
   evntStr->spare1 = msgCp->crntEncLen - subEncLen;
 
   /* Increment the current encoded length by the number of octets required
      to encode tag and length field */
   msgCp->crntEncLen += numBytes;
   msgCp->crntEncLen += cmFindNumOctets(evntStr->spare1);

#endif /* CM_ASN_NO_INDEF_LEN */

   /* Increment the database pointer over the seq/set end element */
   msgCp->elmntDef++;

   /* Encode the EOC tag to indicate the end of sequence */
   for (i = 0; i < EOC_TAG_LEN; i++)
   {
      pkArray[i] = EOC_TAG;
   }

   /* Add the EOC tag at the end of the mBuf */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   RETVALUE(ROK);

} /* cmEncSetSeq */


/*
*
*       Fun:   cmEncSetSeqOf  
*
*       Desc:  This function encodes the repeatable set or a sequence 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncSetSeqOf 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncSetSeqOf (msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **startDef;     /* element definition for seq/set start */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *startStr;      /* pointer to start of event structure */
   U16          numBytes;       /* number of bytes copied */
   U16          i;              /* temporary array counter */
   S32          count;          /* element repeat counter */
   U16          nmbEnc;         /* number of elements actually encoded */
   U16          minEnc;         /* minimum elements that need to be encoded */
   
#ifdef CM_ASN_NO_INDEF_LEN
   U32          subEncLen;      /* Adjusted encoded length */
#endif /* CM_ASN_NO_INDEF_LEN */


   /* temporary array for encoding */
   Data         pkArray[SEQ_BUFSIZE];

   TRC2(cmEncSetSeqOf)

   /* check for the version */
   nmbEnc   = 0;                       /* elements encoded is 0 */ 
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   startDef = msgCp->elmntDef;         /* pointer to seq/set start */
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   startStr = (TknU8 *)msgCp->evntStr; /* get the start of event structure */
     
   /* get the repeat count for this element */
   count = elmntDef->repCntr;

   /* get the minimum elements that need to be encoded */
   minEnc = elmntDef->minLen;

   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* encode the length. EOC format */
   pkArray[i++] = EOC_LEN;
    
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the curent length to adjust the constructor length later */
   subEncLen = msgCp->crntEncLen;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   while(count)
   {
      /* point the database pointer to seq/set start */
      msgCp->elmntDef = startDef;

      /* Increment the database pointer to the next element defintion */
      msgCp->elmntDef++;

      /* get the pointer to the next element definition */
      elmntDef = *msgCp->elmntDef;

      /* check to avoid encoding if event structure is not present.
       */
      if (msgCp->evntStr->pres != TRUE) 
         break;
 
      /* encode the element depending on the element type */
      if ((ret = cmEncElmnt(msgCp)) != ROK)
      {
         /* the error code would already be set, do not do it again */
         RETVALUE(ret);
      }
      else
      {
         nmbEnc++;
      }

      /* decrement count */
      count--;

   } /* end while */

   if (nmbEnc < minEnc)
   {
      /* a minimum number of elements were not encoded */
      CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, NULLP);
      RETVALUE(RFAILED);
   }
   
#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the length of the constructor type in the spare1 field of the
      setof/sequenceof present token */
   evntStr->spare1 = msgCp->crntEncLen - subEncLen;
 
   /* Increment the current encoded length by the number of octets required
      to encode tag and length field */
   msgCp->crntEncLen += numBytes;
   msgCp->crntEncLen += cmFindNumOctets(evntStr->spare1);

#endif /* CM_ASN_NO_INDEF_LEN */

   /* Encode the EOC tag to indicate end of sequence */
   for (i = 0; i < EOC_TAG_LEN; i++)
   {
      pkArray[i] = EOC_TAG;
   }

#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* If some more elements are present in the event structure,
      skip those elements */

   msgCp->elmntDef = startDef;
   msgCp->evntStr  = startStr;

   /* skip the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* end of cmEncSetSeqOf */


/*
*
*       Fun:   cmEncUnconsSetSeqOf  
*
*       Desc:  This function encodes the repeatable set or a sequence 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: This functions is to encode token types :
*                         TET_UNCONS_SET_OF and  
*                         TET_UNCONS_SEQ_OF.
*
*              This is similar to encoding of TET_SET_OF and TET_SEQ_OF.
*
*              Only difference is the event structure for UNCONS token types
*              contains only array of pointers instead of actual array of 
*              structure in it. This is done to support SET OF and SEQUENCE OF 
*              with number of elements unknown. 
*
*              But, there is a maximum limit on the number of pointers in the
*              event structure, similar to the number of structures in 
*              SET OF/SEQUENCE OF event structures. This limit is specified in
*              the element definition to library.
*
*              Everything else is similar to TET_SEQ_OF and TET_SET_OF.
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncUnconsSetSeqOf 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncUnconsSetSeqOf (msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **startDef;     /* element definition for seq/set start */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *startStr;      /* pointer to start of event structure */
   U16          numBytes;       /* number of bytes copied */
   U16          i;              /* temporary array counter */
   S32          count;          /* element repeat counter */
   U16          nmbEnc;         /* number of elements actually encoded */
   U16          minEnc;         /* minimum elements that need to be encoded */
   TknU8        **evntPtrs;     /* pointer to list of event structure 
                                 * pointers in SET/SEQ OF */
   
#ifdef CM_ASN_NO_INDEF_LEN
   U32          subEncLen;      /* Adjusted encoded length */
#endif /* CM_ASN_NO_INDEF_LEN */


   /* temporary array for encoding */
   Data         pkArray[SEQ_BUFSIZE];

   TRC2(cmEncUnconsSetSeqOf)

   /* check for the version */
   nmbEnc   = 0;                       /* elements encoded is 0 */ 
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   startDef = msgCp->elmntDef;         /* pointer to seq/set start */
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   startStr = (TknU8 *)msgCp->evntStr; /* get the start of event structure */

   /* get the repeat count for this element */
   count = elmntDef->repCntr;

   /* get the minimum elements that need to be encoded */
   minEnc = elmntDef->minLen;

   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* encode the length. EOC format */
   pkArray[i++] = EOC_LEN;
    
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the curent length to adjust the constructor length later */
   subEncLen = msgCp->crntEncLen;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   /* set the pointer to array of pointers in SET OF/SEQ OF event structure */
   evntPtrs = (TknU8 **)((PTR)startStr + sizeof(TknU8));
     
   while(count)
   {
      /* point the database pointer to seq/set start */
      msgCp->elmntDef = startDef;

      /* Increment the database pointer to the next element defintion */
      msgCp->elmntDef++;

      /* get the pointer to the next element definition */
      elmntDef = *msgCp->elmntDef;

      /* set correct event structure pointer to msgCp->evntStr */
      msgCp->evntStr = evntPtrs[nmbEnc];

      /* check to avoid encoding if event structure is not present.
       */
      if ((msgCp->evntStr == NULLP) || (msgCp->evntStr->pres != TRUE)) 
         break;
 
      /* encode the element depending on the element type */
      if ((ret = cmEncElmnt(msgCp)) != ROK)
      {
         /* the error code would already be set, do not do it again */
         RETVALUE(ret);
      }
      else
      {
         nmbEnc++;
      }

      /* decrement count */
      count--;

   } /* end while */

   if (nmbEnc < minEnc)
   {
      /* a minimum number of elements were not encoded */
      CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, NULLP);
      RETVALUE(RFAILED);
   }
   
#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the length of the constructor type in the spare1 field of the
      setof/sequenceof present token */
   evntStr->spare1 = msgCp->crntEncLen - subEncLen;
 
   /* Increment the current encoded length by the number of octets required
      to encode tag and length field */
   msgCp->crntEncLen += numBytes;
   msgCp->crntEncLen += cmFindNumOctets(evntStr->spare1);

#endif /* CM_ASN_NO_INDEF_LEN */

   /* Encode the EOC tag to indicate end of sequence */
   for (i = 0; i < EOC_TAG_LEN; i++)
   {
      pkArray[i] = EOC_TAG;
   }

#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* If some more elements are present in the event structure,
      skip those elements */

   msgCp->elmntDef = startDef;
   msgCp->evntStr  = startStr;

   /* skip the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* end of cmEncUnconsSetSeqOf */



/* ************************************************************ */
/* Start of decoding functions */

/*
*
*       Fun:    cmFindLen
*
*       Desc:   Return the length of the element and the length of length
*               field as well. In case of indefinate length encoding the
*               the length of length field is returned as 1 and eocFlg is
*               set to true.
*
*       Ret:    ROK     - ok
*
*       Notes:  The offset points to the tag.  It is necessary to know
*               if the tag is a constructor or a primitive. The primitive
*               length indicated to the caller does not include the
*               EOC tag at the end. It does include the nested EOC tags
*               of any elements within, encoded in the EOC format.
*
*       File:   cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC  S16 cmFindLen
(
CmAsnMsgCp    *msgCp,   /* message buffer */
MsgLen        offset,   /* message offset into the message buffer */
MsgLen        *len,     /* pointer to length */
Bool          *eocFlg,  /* EOC flag - true for indefinate length encoding */
MsgLen        *lfLen    /* length field's lenght */
)
#else
PUBLIC  S16 cmFindLen(msgCp, offset, len, eocFlg, lfLen)
CmAsnMsgCp    *msgCp;   /* message buffer */
MsgLen        offset;   /* message offset into the message buffer */
MsgLen        *len;     /* pointer to length */
Bool          *eocFlg;  /* EOC flag - true for indefinate length encoding */
MsgLen        *lfLen;   /* length field's lenght */
#endif
{
   S16       ret;        /* return */
   MsgLen    curLen;     /* current length */
   S16       i;          /* loop counter & copied bytes counter */
   MsgLen    totLen;     /* Total length of the message */ 
   MsgLen    msgIdx;     /* message index */
   Buffer    *mBuf;      /* message buffer */
   U8        eocCntr;    /* keeps the count of EOC nesting */
   U8        tagSize;    /* size of the tag */

   /* array for unpacking length */
   U8        unPkArray[MAX_LEN_BYTES];

   TRC2(cmFindLen)

   /* get the message buffer */
   mBuf = msgCp->mBuf;

   /* make EOC type of encoding as false */
   *eocFlg = FALSE;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (mBuf == (Buffer *)NULLP)
   {
      CMASNLOGERROR(ERRCLS_DEBUG, ECMASN005, (ErrVal)0, "cmFindLen() Failed");
      RETVALUE(RFAILED);
   }
#endif

#if CM_ASN_SS
   /* get the total length of the message */
   (Void)SFndLenMsg (mBuf, &totLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &totLen);
#endif

   if ((ret = cmGetLen(msgCp, offset, len, lfLen, eocFlg)) == RFAILED)
   {
      RETVALUE(RFAILED);
   }

   if (*eocFlg == FALSE) 
   {
      /* short or long format, return */
      RETVALUE(ROK);
   }

   /* Indefinate form of length encoding */

   curLen  = 0;      /* current length is assigned null */
   eocCntr = 0;      /* eoc nesting level is 0 */

   *lfLen = 1;       /* indicate the length of length field as 1 */ 

   /* make message index point to the next byte after the length field */
   msgIdx = (offset+1);

   /* loop until find the matching EOC tag */

   while (1) 
   {

      /* if length already read is equal or greater than message length
         return error */

      if (totLen <= curLen)
      {
         RETVALUE(RFAILED);
      }

      if (msgIdx >= totLen)
      {
         RETVALUE(RFAILED);
      }
     
      /* get the next two bytes */
#ifdef CM_ASN_SS
      ret = SCpyMsgFix(mBuf, msgIdx, EOC_TAG_LEN, unPkArray, &i); 
#else
      CM_ASN_CPY_MSG_FIX(msgCp, msgIdx, EOC_TAG_LEN, unPkArray, &i, &ret);
#endif

      /* check for errors */
      if ((ret == RFAILED) || (i != EOC_TAG_LEN))
      {
         /* error in length decoding */
         RETVALUE(RFAILED);
      }

      /* check if this was the EOC tag */
      if( (unPkArray[0] == EOC_TAG) && (unPkArray[1] == EOC_TAG))
      {

         /* if already encountered another EOC encoding within this
            type eg. seqeunce within sequence, decrement the eocCntr flag */

         if (eocCntr)
         {
            /* increment the length, the message index and eoc nesting 
               level */ 
            curLen += EOC_TAG_LEN;
            msgIdx += EOC_TAG_LEN;
            eocCntr--;
            continue;
         }
         else
         {
            /* no more nesting, got the EOC tag we want for this element */

            *lfLen = 1;       /* length of the length field */
            *len = curLen;    /* length of the element */
            *eocFlg = TRUE;   /* eoc flag is set to true */
            RETVALUE(ROK);
         }

      } /* endif */

      /* If not the EOC tag, then it must be the tag of some other
         element, get it */

      if ((ret = cmGetTag(msgCp, unPkArray, msgIdx, &tagSize)) == RFAILED)
      {
         RETVALUE(RFAILED);
      }

      /* Increment the message index & current length 
         by the size of the tag */

      msgIdx = msgIdx + tagSize;
      curLen = curLen + tagSize;

      /* Increment the msgIdx by the length of the element, in case
         of EOC format, increment EOC nesting counter */

      if ((ret = cmGetLen(msgCp, msgIdx, len, lfLen, eocFlg)) == RFAILED)
      {
         RETVALUE(RFAILED);
      }

      /* check if long form of length encoding */
      if (*eocFlg == FALSE) 
      {
         /* short or long form of length encoding */
         curLen = curLen + *len + *lfLen; 
         msgIdx = msgIdx + *len + *lfLen; 
      }
      else
      {
         /* long form of length encoding, increment the length and
            the message index by the size of EOC length field (1 byte) */

         curLen = curLen + *lfLen; 
         msgIdx = msgIdx + *lfLen; 

         /* Increment the eoc nesting counter */
         eocCntr++;

      }

   } /* end while */

} /* end of cmFindLen */


/*
*
*       Fun:    cmRemPrim
*
*       Desc:   read the primitive into a string
*
*       Ret:    ROK     - ok
*
*       Notes:  Does not handle contructor types. Only basic types.
*
*       File:   cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmRemPrim
(
Buffer       *mBuf,          /* message buffer */
CmAsnMsgCp   *msgCp,         /* Pointer to message control */
U8           *destStr,       /* destination string */
/* cm_asn_c_001.main_19 - modification - data type of destLen changed from U16 
 * to MsgLen.
 */
MsgLen       *destLen        /* number of bytes in the string */
)
#else
PUBLIC S16 cmRemPrim(mBuf, msgCp, destStr, destLen)
Buffer       *mBuf;          /* message buffer */
CmAsnMsgCp   *msgCp;         /* Pointer to message control */
U8           *destStr;       /* destination string */
/* cm_asn_c_001.main_19 - modification - data type of destLen changed from U16 
 * to MsgLen.
 */
MsgLen       *destLen;       /* number of bytes in the string */
#endif
{
   S16        ret;           /* return value */
   MsgLen     msgLen;        /* message length */
   MsgLen     primLen;       /* total primitive length */
   MsgLen     lfLen;         /* length of the length field */
   CmElmntDef *elmntDef;     /* element defintion */
   Bool       eocFlg;        /* end of contents flag */
   U8         ignFlag;       /* Ignore flag */
   MsgLen     extByte;       /* extra bytes at the end of the message */
   Data       tmp;           /* temporary data */
   MsgLen     i;             /* counter */

   /* array for unpacking the prmitive */
   Data       unPkArray[MAX_TAG_LEN + MAX_LEN_BYTES]; 

   TRC2(cmRemPrim)

#ifndef CM_ASN_SS
   UNUSED(mBuf);
#endif

   *destLen = 0;
   ignFlag  = 0;
   extByte  = 0;

   /* Get the element defintion */
   elmntDef = *msgCp->elmntDef; 

#if CM_ASN_SS
   /* Get the length of the message */
   (Void)SFndLenMsg(mBuf, &msgLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &msgLen);
#endif

   if ((ret = cmFindLen(msgCp, elmntDef->tag.len, 
                        &primLen, &eocFlg, &lfLen)) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Message length should be more than primitive length. Contains the
      tag and the length as well */
 
   if (msgLen <= primLen)
   {
      RETVALUE(RFAILED);
   } 

   /* primitive length should lie within the max and min values */

   /* avoid checking of length for TET_INT_RNG token type, as they use the 
    * fields minLen/maxLen differently.
    */
   if (elmntDef->type != TET_INT_RNG)
   {
      if ((MsgLen)elmntDef->minLen != MIN_LEN_NA)
      {
         if (primLen < (MsgLen)elmntDef->minLen)
         {
            RETVALUE(RFAILED);
         }
      }

      if ((MsgLen)elmntDef->maxLen != MAX_LEN_NA)
      {
         if (primLen > (MsgLen)elmntDef->maxLen)
         {
            /* check the flag to see if need to ignore the extra octets */
            ignFlag  = IS_IGNORE_EXT(elmntDef->flagp, CM_ASN_MAP_IS41C); 

            if (ignFlag)
            {
               /* extra bytes at the end */
               extByte = primLen - elmntDef->maxLen;  
               primLen = elmntDef->maxLen;
            }
/* cm_asn_c_001.main_25: Return RFAILED only incase of ignflag not set, otherwise
                         send only defined octets ignoring the rest extra bytes */
            else
            {
              RETVALUE(RFAILED);
            }
         }
      }
   }

   /* strip the tag and length from the top of the message */

#ifdef CM_ASN_SS
   ret = SRemPreMsgMult(unPkArray, (MsgLen)(elmntDef->tag.len + lfLen), mBuf);
#else
   CM_ASN_REM_PRE_MULT(unPkArray, (elmntDef->tag.len + lfLen), msgCp, &ret);
#endif

   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   if (primLen != 0)
   {
      /* For TET_STRXL_MEM and TET_IA5STRXL_MEM types, destStr was passed as 
       * null. This is because caller don't know the size of the val part. 
       * Allocate the memory for the TknStrXL.val part */
      if (elmntDef->type == TET_STRXL_MEM || elmntDef->type == TET_IA5STRXL_MEM)
      {
         if (SGetSBuf(msgCp->region, msgCp->pool, 
                      (Data **)&destStr, primLen) != ROK)
         {
            CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
            RETVALUE(RFAILED);
         }

         /* zero the memory */
         cmZero((U8 *)destStr, primLen);
         
         /* set the val part in TknStrXL */
         ((TknStrXL *)(msgCp->evntStr))->val = destStr;
      }

      /* for NULL types, primLen will be zero */

      /* copy the primitive to the destination string */

#ifdef CM_ASN_SS
      ret = SRemPreMsgMult(destStr, (MsgLen)primLen, mBuf);
#else
      CM_ASN_REM_PRE_MULT(destStr, (MsgLen)primLen, msgCp, &ret);
#endif

      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }

      /* check to see if any extra bytes need to be stripped off */
      if (ignFlag)
      {
         for (i=0; i < extByte; i++)
         {
            /* ignore extra octets in the end */
#ifdef CM_ASN_SS
            ret = SRemPreMsg(&tmp, mBuf);
#else
            CM_ASN_REM_PRE(&tmp, msgCp, &ret);
#endif
            if (ret != ROK)
            {
               CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
               RETVALUE(ret);
            }
         }
      }
   }

   /* return the length of the primitive */
   *destLen = primLen;

   /* if EOC encoding, remove the EOC tag from the end of the message */
   if (eocFlg == TRUE)
   {
#ifdef CM_ASN_SS
      ret = SRemPreMsgMult(unPkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
      CM_ASN_REM_PRE_MULT(unPkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
      if (ret != ROK)
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);

} /* end of cmRemPrim */


/*
*
*       Fun:   cmChkMsgMandMis 
*
*       Desc:  This function checks whether any mandatory element is
*              missing in a sequence from the message buffer. This routine
*              checks all the elements of the sequence.
*
*       Ret:   ROK     - No mandatory information missing
*              RFAILED - Mandatory information missing
*
*       Notes: <None>
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmChkMsgMandMis 
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PUBLIC S16 cmChkMsgMandMis (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */
   U8           flag;          /* flag */

   TRC2 (cmChkMsgMandMis)

   /* get the element defintion for this element */
   elmntDef = *msgCp->elmntDef;

   while (elmntDef != NULLP)
   {
      /* get the protocol flag for this element */
      flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

      /* if element is mandatory, error */
      if (flag == ELMNT_MAND)
      {
         RETVALUE(RFAILED);
      }

      /* element was optional, skip it */
      cmSkipElmnt(msgCp);

      /* get the updated element defintion pointer */
      elmntDef = *msgCp->elmntDef;    

   } /* end while */

   RETVALUE(ROK);

} /* cmChkMsgMandMis */


/*
*
*       Fun:   cmChkSeqMandMis 
*
*       Desc:  This function checks whether any mandatory element is
*              missing in a sequence from the message buffer. This routine
*              checks all the elements of the sequence.
*
*       Ret:   ROK/RFAILED 
*
*       Notes: db pointer - Always in the middle of the sequence over
*              the seq start token.
*              evnt pointer - Corresponding event structure.
*
*              db pointer   - element defintion after sequence end token. 
*              evnt pointer - Corresponding event structure.
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmChkSeqMandMis 
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PRIVATE S16 cmChkSeqMandMis (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */
   U8           flag;          /* flag */

   TRC2 (cmChkSeqMandMis)

   /* get the element defintion for this element */
   elmntDef = *msgCp->elmntDef;

   while (elmntDef->type != TET_SETSEQ_TERM)
   {
      /* get the protocol flag for this element */
      flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

      /* if element is mandatory, error */
      if (flag == ELMNT_MAND)
      {
         RETVALUE(RFAILED);
      }

      /* element was optional, skip it */
      cmSkipElmnt(msgCp);

      /* get the updated element defintion pointer */
      elmntDef = *msgCp->elmntDef;    

   } /* end while */

   /* increment the pointer over seq/set end element */
   msgCp->elmntDef++;

  RETVALUE(ROK);

} /* cmChkSeqMandMis */


/*
*
*       Fun:   cmChkSetMandMis 
*
*       Desc:  This function checks whether any mandatory element is
*          missing in a set from the message buffer. This routine
*              checks all the elements of the set.
*
*       Ret:   ROK/RFAILED 
*
*       Notes: db pointer - Always in the middle of the sequence over
*              the seq start token.
*              evnt pointer - Corresponding event structure.
*
*              db pointer   - element defintion after sequence end token. 
*              evnt pointer - Corresponding event structure.
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmChkSetMandMis 
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PRIVATE S16 cmChkSetMandMis (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */
   U8           flag;          /* flag */
   CmElmntDef   **defPtr;      /* Database pointer */
   TknU8        *evntStr;      /* event structure pointer */
            
   TRC2 (cmChkSetMandMis)

   /* increment over the start of set element */
   msgCp->elmntDef++;

   /* get the element defintion for the first element */
   elmntDef = *msgCp->elmntDef;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   while (elmntDef->type != TET_SETSEQ_TERM)
   {
      switch (elmntDef->type)
      {
         case TET_SEQ:
         case TET_SET:

            /* save the pointers */
            defPtr  = msgCp->elmntDef;
            evntStr = msgCp->evntStr;
               
            /* get the protocol flag for this element */
            flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

            /* sequence is mandatory and missing */
            if (flag == ELMNT_MAND)
            {
               if (cmChkSetMandMis(msgCp) == RFAILED)
               {
                  /* no element present in the seq/set of, error  */
                  RETVALUE(RFAILED);
               }
            }

            /* restore the pointers */
            msgCp->elmntDef = defPtr;
            msgCp->evntStr = evntStr;

            /* skip the element */
            cmSkipElmnt(msgCp);

            /* get the updated element defintion pointer */
            elmntDef = *msgCp->elmntDef;    

            break;

         case TET_SEQ_OF:
         case TET_SET_OF:

            /* save the pointers */
            defPtr  = msgCp->elmntDef;
            evntStr = msgCp->evntStr;
               
            /* get the protocol flag for this element */
            flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

            /* sequence is mandatory and missing */
            if (flag == ELMNT_MAND)
            {
               if (cmChkSetMandMis(msgCp) == RFAILED)
               {
                  /* no element present in the seq/set of, error  */
                  RETVALUE(RFAILED);
               }
            }

            /* retore the pointers */
            msgCp->elmntDef = defPtr;
            msgCp->evntStr = evntStr;

            /* skip over the sequence */
            cmSkipElmnt(msgCp);

            /* get the updated element defintion pointer */
            elmntDef = *msgCp->elmntDef;    

            break;

         case TET_UNCONS_SEQ_OF:
         case TET_UNCONS_SET_OF:
         {
            S32        cntr;        /* counter for elements in set/seq of */
            TknU8      **evntPtrs;  /* pointer to list of event structure 
                                     * pointers in SET/SEQ OF */
            U16        ind;       /* index to the above array of pointers */

            ind = 0;   
            cntr  = elmntDef->minLen;  /* check for minimum number of elements 
                                        * in the set/seq of */

            /* save the pointers */
            defPtr  = msgCp->elmntDef;
            evntStr = msgCp->evntStr;
               
            /* get the protocol flag for this element */
            flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

            /* set the pointer to array of pointers in SET OF/SEQ OF 
             * event structure */
            evntPtrs = (TknU8 **)((PTR)evntStr + sizeof(TknU8));

            /* set the correct evntStr */
            msgCp->evntStr = evntPtrs[ind];

            /* sequence is mandatory and missing */
            if (flag == ELMNT_MAND)
            {
               /* check all elements in set/seq of */
               while (cntr && msgCp->evntStr)
               {
                  CmElmntDef *tmpDef;
                  /* set/seq of element definition */
                  msgCp->elmntDef++;

                  tmpDef = *msgCp->elmntDef;

                  /* if element is SET */
                  if (tmpDef->type == TET_SET)
                  {
                     if (cmChkSetMandMis(msgCp) == RFAILED)
                        /* no element present in the seq/set of, error  */
                        RETVALUE(RFAILED);
                  }
                  /* any other element */
                  else if (cmChkEleMandMis(msgCp) == TRUE)
                  {
                     switch (tmpDef->type)
                     {
                        case TET_STRXL:
                        case TET_STRXL_MEM:
                        case TET_IA5STRXL:
                        case TET_IA5STRXL_MEM:
                           if (((TknStrXL *)(msgCp->evntStr))->pres == FALSE)
                              RETVALUE(RFAILED);
                           break;

                        default:
                           if ((msgCp->evntStr)->pres == FALSE)
                           {
                              /* mandatory element is missing */
                              RETVALUE(RFAILED);
                           }
                           break;
                     }
                  }

                  msgCp->evntStr = evntPtrs[ind++];
                  msgCp->elmntDef = defPtr;
                  cntr--;
               }

               /* check if minimum number of elements were present */
               if (cntr != 0)
                  RETVALUE(RFAILED);
            }

            /* retore the pointers */
            msgCp->elmntDef = defPtr;
            msgCp->evntStr = evntStr;

            /* skip over the sequence */
            cmSkipElmnt(msgCp);

            /* get the updated element defintion pointer */
            elmntDef = *msgCp->elmntDef;    

            break;
         }

         default:
         {
            evntStr = msgCp->evntStr;

            /* get the protocol flag for this element */
            flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

            /* if element is mandatory, error */
            if (flag == ELMNT_MAND)
            {   
               switch (elmntDef->type)
               {
                  case TET_STRXL:
                  case TET_STRXL_MEM:
                  case TET_IA5STRXL:
                  case TET_IA5STRXL_MEM:
                     if (((TknStrXL *)evntStr)->pres == FALSE)
                        RETVALUE(RFAILED);
                     break;

                  default:
                     if (evntStr->pres == FALSE)
                     {
                        /* mandatory element is missing */
                        RETVALUE(RFAILED);
                     }
                     break;
               }
            }

            /* skip over the element, event structure and the database
               pointers are both incremented */

            cmSkipElmnt(msgCp);

            /* get the updated element defintion pointer */
            elmntDef = *msgCp->elmntDef;    

            break;
         }

      } /* end switch */

   } /* end while */

   /* increment the pointer over seq/set end element */
   msgCp->elmntDef++;

  RETVALUE(ROK);

} /* cmChkSetMandMis */


/*
*
*       Fun:   cmChkEleMandMis 
*
*       Desc:  This function checks whether an element is mandatory. 
*
*       Ret:   TRUE (if element is mandatory)
*              FALSE (if element invalid or optional)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE Bool cmChkEleMandMis
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PRIVATE Bool cmChkEleMandMis (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */
   U8           flag;          /* flag */

   TRC2 (cmChkEleMandMis)

   /* get the element defintion for this element */
   elmntDef = *msgCp->elmntDef;

   /* get the protocol flag for this element */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* if element is mandatory, return true */
   if (flag == ELMNT_MAND)
   {
      RETVALUE(TRUE);
   }

   /* either element is optional or not defined for this protocol */
   RETVALUE(FALSE);

} /* cmChkEleMandMis */


/*
*
*       Fun:   cmDecElmnt 
*
*       Desc:  This function invokes an appropriate decoding routine
*              depending on the element defintion. 
*
*       Ret:   ROK
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecElmnt
(
CmAsnMsgCp  *msgCp  /* message control pointer */
)
#else
PUBLIC S16 cmDecElmnt (msgCp)
CmAsnMsgCp  *msgCp; /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;     /* pointer to element defintion */
   S16          ret;           /* return value */

   TRC2 (cmDecElmnt)

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#ifdef CM_ASN_DBG
   cmPrntElmntDb(elmntDef);
#endif

   switch(elmntDef->type)
   {
      case TET_U8:
         msgCp->octEnum = FALSE;         /* octet, not a enumerated type */
         ret = cmDecOctetEnum(msgCp);
         break;

      case TET_ENUM:
         msgCp->octEnum = TRUE;          /* enumerated type */
         ret = cmDecOctetEnum(msgCp);
         break;

      case TET_NULL:
         ret = cmDecNull(msgCp);       /* null type */
         break;

      case TET_STR4:
      case TET_STR12:
      case TET_STR32:
      case TET_STR64:
      case TET_STR256:
      case TET_STRXL:
      case TET_STRXL_MEM:
         ret = cmDecOctetStr(msgCp);  /* Octet string */
         break;

      case TET_IA5STR4:
      case TET_IA5STR12:
      case TET_IA5STR32:
      case TET_IA5STR64:
      case TET_IA5STR256:
      case TET_IA5STRXL:
      case TET_IA5STRXL_MEM:
         ret = cmDecIa5Str(msgCp);    /* Decode IA5 string type */
         break;

      case TET_INT:
         ret = cmDecInteger(msgCp);   /* Decode Integer Type */
         break;
 
      case TET_INT_RNG:
         ret = cmDecIntRng(msgCp);   /* Decode Integer Type with range check*/
         break;

      case TET_OID:
         ret = cmDecOid(msgCp);     /* Decode Object Identifier */
         break;
 
      case TET_BITSTR:
         ret = cmDecBitStr(msgCp);     /* bit string */
         break;

      case TET_TAG:
         ret = cmDecTagType(msgCp);    /* tag type */
         break;

      case TET_BOOL:
         ret = cmDecBool(msgCp);       /* boolean type */
         break;

      case TET_SEQ:
         ret = cmDecSeq(msgCp);       /* sequence */
         break;

      case TET_SEQ_OF:
         ret = cmDecSetSeqOf(msgCp);     /* sequence of */
         break;

      case TET_SET:
         ret = cmDecSet(msgCp);       /* set */
         break;

      case TET_SET_OF:
         ret = cmDecSetSeqOf(msgCp);     /* set of */
         break;

      case TET_UNCONS_SEQ_OF:            /* unconstrained set of or seq of */
      case TET_UNCONS_SET_OF:
         ret = cmDecUnconsSetSeqOf(msgCp);
         break;

      case TET_CHOICE:                /* choice */
         ret = cmDecChoice(msgCp);
         break;

      case TET_ESC_PRIM:
      case TET_ESC_CONST:

         /* call user function to encode this type */
#if (ERRCLASS & ERRCLS_DEBUG)
            if (!elmntDef->func)
            {
               CMASNLOGERROR(ERRCLS_DEBUG, ECMASN006, (ErrVal)ERRZERO, 
                          "cmEncElmnt () Failed. Null User function pointer");
               CM_ASN_ERR(msgCp, CM_ASN_NO_FUNC, &elmntDef->tag);  
               RETVALUE(RFAILED);
            }
#endif
            /* execute the user function */
            ret = (*elmntDef->func)(msgCp);
            break;

      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         CMASNLOGERROR(ERRCLS_DEBUG, ECMASN007, (ErrVal)elmntDef->type, 
                               "cmInvDecElmnt () Failed");
#endif
         /* 'ret' initialised properly */
         ret = RFAILED;

         /* error cause filled */
         CM_ASN_ERR(msgCp, CM_ASN_INVLD_ELMNT, NULLP);

         break;

   } /* end switch */

   RETVALUE(ret);

} /* cmDecElmnt */


/*
*
*       Fun:   cmDecMsg 
*
*       Desc:  This function encodes the message by encoding all the token
*              elements it contains.  
*
*       Ret:   ROK  (encoding successful)
*              RFAILED (failed, general)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecMsg 
(
TknU8       *evntStr,     /* pointer to the event structure */
Buffer      *mBuf,        /* ASN.1 decoded message buffer */
U8          proType,      /* protocol type */ 
CmElmntDef  **msgDef,     /* message defintion */
Region      region,       /* region to allocate the memory from */
Pool        pool,         /* pool to allocate the memory from */
U8          cfg,          /* switch for unrecognized elements */
CmAsnErr    *err,         /* error to be returned back to the caller */ 
Buffer      **uBuf        /* user buffer containing any unrecognized elements */
)
#else
PUBLIC S16 cmDecMsg (evntStr, mBuf, proType, msgDef, region,pool,cfg,err, uBuf)
TknU8       *evntStr;     /* pointer to the event structure */
Buffer      *mBuf;        /* ASN.1 decoded message buffer */
U8          proType;      /* protocol type */ 
CmElmntDef  **msgDef;     /* message defintion */
Region      region;       /* region to allocate the memory from */
Pool        pool;         /* pool to allocate the memory from */
U8          cfg;          /* switch for unrecognized elements */
CmAsnErr    *err;         /* error to be returned back to the caller */ 
Buffer      **uBuf;       /* user buffer containing any unrecognized elements */
#endif
{
   CmAsnMsgCp   msgCp;         /* message control point */
   CmAsnMsgCp   *cp = &msgCp;  /* message control point */
   CmAsnMsgCp   *cpPtr;        /* message control point */
   CmElmntDef   *elmntDef;     /* element defintion */
   MsgLen       elmntSize;     /* element size */
   S16          ret;           /* return value */
   MsgLen       msgLen;        /* message length */
   Bool         failure;

   TRC2(cmDecMsg)

   /* update the msgCp pointer */
   cpPtr = &msgCp;

   /* Initialize the message control structure */
   (Void) cmInitAsnMsgCp(&msgCp);

   /* update the message control structure with passed values */

   msgCp.evntStr  = evntStr;     /* event structure */
   msgCp.mBuf     = mBuf;        /* pointer to the allocated message buffer */
   msgCp.proType  = proType;     /* the protocol type */
   msgCp.elmntDef = msgDef;      /* message defintion */
   msgCp.region   = region;      /* region for the memory */
   msgCp.pool     = pool;        /* pool for the memory */
   msgCp.cfg      = cfg;         /* switch for unrecoggnized elements */
   msgCp.err      = err;         /* switch for unrecoggnized elements */

   msgCp.edFlag   = DECODE_ELMNT;/* encode the message */

   elmntDef = *msgCp.elmntDef;   /* get the element defintion */

   /* 
    * cm_asn_c_001.main_16: modification: 
    * if mBuf is NULLP or length is zero, check if there are any mandatory 
    * elements. If yes, return error else leave event structure NULL.
    */
 
   if (msgCp.mBuf == NULLP)
   {
      if (cmChkMsgMandMis(&msgCp) != ROK)
      {
         CM_ASN_ERR(cpPtr, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(CM_ASN_MAND_MIS);
      }
      RETVALUE(ROK);
   }
  
   /* Get the length of the ASN.1 message buffer */
   (Void) SFndLenMsg(msgCp.mBuf, &msgLen);
  
   if (msgLen == 0)
   {
      if (cmChkMsgMandMis(&msgCp) != ROK)
      {
         CM_ASN_ERR(cpPtr, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(CM_ASN_MAND_MIS);
      }
      RETVALUE(ROK);
   }
 
#ifndef CM_ASN_SS

   if (SGetSBuf(msgCp.region, msgCp.pool, &msgCp.sBuf.bufP, 
                (Size) msgLen) != ROK)
   {
      CM_ASN_ERR(cpPtr, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(NULLP);
   }

   msgCp.sBuf.max   = msgLen;             /* max size of the sBuf */
   msgCp.sBuf.stIdx = 0;                  /* start index */
   msgCp.sBuf.size  = 0;                  /* size */

   { 
      MsgLen count;         /* number of bytes copied */

      /* copy the static encoded string to message buffer */
      if ((ret = SCpyMsgFix(
                     msgCp.mBuf,                /* source */
                     0,                          /* source index */
                     msgLen,                     /* size to be copied */
                     msgCp.sBuf.bufP,           /* destination buffer */
                     &count)) != ROK)            /* size actually copied */
      {

         /* throw away the static buffer */
         (Void) SPutSBuf(msgCp.region, 
                         msgCp.pool, 
                         msgCp.sBuf.bufP, 
                         msgCp.sBuf.max);
         RETVALUE(ret);
      }

      /* initialize the static buffer structure */
      msgCp.sBuf.size  = count;
      msgCp.sBuf.stIdx = 0;
   }
#endif

   /* go through all the elements in the message database for this
      message */

   while(elmntDef != (CmElmntDef *)NULLP) 
   {

      /* Check the message length. Bytes will be removed from the mBuf
         in each call of the routines below depending on the type decoded */ 

#if CM_ASN_SS
      (Void) SFndLenMsg(msgCp.mBuf, &msgLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(cpPtr, &msgLen);
#endif

      if (msgLen == 0)           /* no information left to be decoded */
      {
         /* check if any mandatory element misssing in the message */
         if (cmChkMsgMandMis(&msgCp) != ROK)
         {

#ifndef CM_ASN_SS
            /* throw away the static buffer */
            (Void) SPutSBuf(msgCp.region, 
                            msgCp.pool, 
                            msgCp.sBuf.bufP, 
                            msgCp.sBuf.max);
#endif

            CM_ASN_ERR(cpPtr, CM_ASN_MAND_MIS, &elmntDef->tag);  
            RETVALUE(CM_ASN_MAND_MIS);
         }
         else
         {
            /* no mandatory information missing in the message */
            break;
         }
      }

      /* decode the element depending on the element type */
      if ((ret = cmDecElmnt(&msgCp)) != ROK)
      {

#ifndef CM_ASN_SS
      /* throw away the static buffer */
      (Void) SPutSBuf(msgCp.region, 
                      msgCp.pool, 
                      msgCp.sBuf.bufP, 
                      msgCp.sBuf.max);
#endif
      /*cm_asn_c_001.main_28:-remove the memory leak problem*/
      if (msgCp.uBuf != NULLP)
      {
         (Void) SPutMsg(msgCp.uBuf);
         msgCp.uBuf = NULLP;
      }
         RETVALUE(RFAILED);
      }

      /* get the incremented element defintion */
      elmntDef = *msgCp.elmntDef;

   } /* end while */

#if CM_ASN_SS
   /* get the total length of the message */
   (Void)SFndLenMsg (msgCp.mBuf, &elmntSize);
#else 
   /* get the total length of the message */
   CM_ASN_FND_LEN(cp, &elmntSize);
#endif

   failure = FALSE;

   if (elmntSize)
   {
      Data         *pData;         /* static buffer pointer */

      switch(cp->cfg)
      {
         case CM_ASN_GEN_ERR:
         {
             CM_ASN_ERR(cp, CM_ASN_EXTRA_PARAM, NULLP);
             failure = TRUE;
             break;
         }

         case CM_ASN_DROP_ELMNTS:
         case CM_ASN_IGN_ELMNTS:
         {
            /* drop the unrecongnized element */
            break;
         }

         case CM_ASN_PASS_ELMNTS:
         {
            /* pass the unrecongnized element to the user */
    
            /* allocate a static buffer */
    
            if (SGetSBuf(cp->region, cp->pool, &pData, (Size) elmntSize)
                  != ROK)
            {
               CM_ASN_ERR(cp, CM_ASN_RES_ERR, NULLP);
               failure = TRUE;
               break;
            }
    
            /* strip of the bytes from the begining of the message buffer */
#ifdef CM_ASN_SS
            ret = SRemPreMsgMult(pData, (MsgLen)elmntSize, cp->mBuf);
#else
            CM_ASN_REM_PRE_MULT(pData, elmntSize, cp, &ret);
#endif
 
            if (ret != ROK)
            {
               (Void) SPutSBuf(cp->region,cp->pool,pData,(Size) elmntSize);  
               CM_ASN_ERR(cp, CM_ASN_RES_ERR, NULLP);
               failure = TRUE;
               break;
            }
 
            /* get a new message buffer if not already allocated */
            if (cp->uBuf == NULLP)
            {
               if ((ret = SGetMsg(cp->region, cp->pool, 
                                                 &cp->uBuf)) != ROK)
               {
                  (Void) SPutSBuf(cp->region, cp->pool, pData, 
                                  (Size) elmntSize);  
                  CM_ASN_ERR(cp, CM_ASN_RES_ERR, NULLP);
                  failure = TRUE;
                  break;
               }
            }
    
            /* copy the static encoded string to message buffer */
            if ((ret = SAddPstMsgMult(pData, elmntSize, cp->uBuf)) != ROK)
            {
               /* throw away the static buffer */
               (Void) SPutSBuf(cp->region, cp->pool, pData,(Size) elmntSize);
               (Void)SFndLenMsg (cp->uBuf, &msgLen);
 
               if (msgLen == 0)
               {
                  (Void) SPutMsg(cp->uBuf);
                  cp->uBuf = NULLP;
               }
    
               CM_ASN_ERR(cp, CM_ASN_RES_ERR, NULLP);
               failure = TRUE;
               break;
            }
    
            /* throw away the static buffer */
            (Void) SPutSBuf(cp->region, cp->pool, pData, (Size) elmntSize);  
    
            break;
         }
 
         default:
            /* unknown configuration */
            failure = TRUE;
            break;
    
      } /* end switch */

   } /* end of if */

#ifndef CM_ASN_SS
      /* throw away the static buffer */
   (Void) SPutSBuf(msgCp.region, 
                   msgCp.pool, 
                   msgCp.sBuf.bufP, 
                   msgCp.sBuf.max);
#endif

   /* update the user buffer information */
   if (uBuf != NULLP)
   {
      *uBuf = msgCp.uBuf;
   }
   else
   {
      /* user buffer pointer is null, throw away the user buffer */
      /* if allocated in this routine */

      if (msgCp.uBuf != NULLP)
      {
         (Void) SPutMsg(msgCp.uBuf);
      }
   }

   if (failure)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of cmDecMsg */


/*
*
*       Fun:    cmDecOctetEnum 
*
*       Desc:   This function decodes an octet and enumerated types.
*
*       Ret:    ROK 
*
*       Notes:  None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecOctetEnum 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecOctetEnum (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{

   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen       tmpLen;         /* message length */
   Buffer       *mBuf;          /* message buffer */

   /* temporary array for encoding */
   Data         pkArray[OCTET_BUFSIZE];

   TRC2(cmDecOctetEnum)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */

   if ((ret = cmRemPrim(mBuf, msgCp, (U8 *)(&(evntStr->val)), &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   if (msgCp->octEnum == TRUE)
   {
      /* enumerated type, check it's value */
      if ((ret = cmChkEnum(evntStr->val, elmntDef)) != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_UNEXP_VAL, &elmntDef->tag);  
         RETVALUE(ret);
      }

   } /* endif */

   /* no errors, make the element present in the event structure */
   evntStr->pres = TRUE;

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecOctetEnum */


/*
*
*       Fun:   cmDecNull 
*
*       Desc:  This function decodes a null ASN.1 type. 
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecNull 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecNull (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen       tmpLen;         /* message length */
   Buffer       *mBuf;          /* message buffer */

   /* temporary array for encoding */
   Data         pkArray[NULL_BUFSIZE];

   TRC2(cmDecNull)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }



   /* read the element into the event structure, remove the element
      from the message buffer (including tag and length) */

   if ((ret = cmRemPrim(mBuf, msgCp, (U8 *)(&(evntStr->val)), &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   /* no errors, make the element present in the event structure */
   evntStr->pres = TRUE;

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecNull */


/*
*
*       Fun:    cmDecOctetStr 
*
*       Desc:  This function decodes an octet string value  
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecOctetStr 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecOctetStr (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   Bool          skipElmnt;      /* need to skip the element */
   U8            tagSize;        /* size of the tag */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen        tmpLen;         /* message length */
   Bool          pres;           /* element present flag */
   U8           *str;            /* String */
   S16           ret;            /* return value */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE];

   TRC2(cmDecOctetStr)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   elmntDef = *msgCp->elmntDef;        /* get the element definition */

   if (elmntDef->type == TET_STRXL || elmntDef->type == TET_STRXL_MEM)
   {
      pres = ((TknStrXL *)(msgCp->evntStr))->pres;
   }
   else
   {
      pres = msgCp->evntStr->pres;
   }

   /* check if this is a duplicate element */
   if (pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   if (elmntDef->type == TET_STRXL)
   {
      str = ((TknStrXL *)(msgCp->evntStr))->val;

      /* Check if storage for the sting hasn't been allocated by the user */
      if (str == NULLP)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
   }
   else if (elmntDef->type == TET_STRXL_MEM)
   {
      str = ((TknStrXL *)(msgCp->evntStr))->val;

      /* Check if storage for the string already allocated by user */
      if (str != NULLP)
      {
         CM_ASN_ERR(msgCp, CM_ASN_MEM_ALRDY_ALLOCD, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }

      /* Allocate memory for val part: This cannot be done here, because
       * we don't know the length to allocate memory for TknStrXL. So,
       * allocate the memory in cmRemPrim when token type is TET_STRXL_MEM. */
   }
   else
   {
      str = &((TknStr *)(msgCp->evntStr))->val[0];
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */
   if ((ret = cmRemPrim(msgCp->mBuf, msgCp, (U8 *)str, &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   if (elmntDef->type == TET_STRXL || elmntDef->type == TET_STRXL_MEM)
   {
      /* make the element present in the event structure */
      ((TknStrXL *)(msgCp->evntStr))->pres = TRUE;

      /* update the length of the primitive */
      ((TknStrXL *)(msgCp->evntStr))->len  = tmpLen;
   }
   else
   {
      /* make the element present in the event structure */
      ((TknStr *)(msgCp->evntStr))->pres = TRUE;

      /* update the length of the primitive */
      ((TknStr *)(msgCp->evntStr))->len  = (U8)tmpLen;
   }

   /* now skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecOctetStr */


/*
*
*       Fun:    cmDecIa5Str 
*
*       Desc:  This function decodes an IA5 string value  
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecIa5Str 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecIa5Str (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   Bool          skipElmnt;      /* need to skip the element */
   U8            tagSize;        /* size of the tag */
   /* cm_asn_c_001.main_22 - modification - data type of tmpLen changed to
    *  MsgLen from U16.
    */
   MsgLen        tmpLen;         /* message length */
   Bool          pres;           /* element present flag */
   U8           *str;            /* String */
   S16           ret;            /* return value */
   U16           i;              /* loop counter */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE];

   TRC2(cmDecIa5Str)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   elmntDef = *msgCp->elmntDef;        /* get the element definition */

   if (elmntDef->type == TET_IA5STRXL || elmntDef->type == TET_IA5STRXL_MEM)
   {
      pres = ((TknStrXL *)(msgCp->evntStr))->pres;
   }
   else
   {
      pres = msgCp->evntStr->pres;
   }

   /* check if this is a duplicate element */
   if (pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   if (elmntDef->type == TET_IA5STRXL)
   {
      str = ((TknStrXL *)(msgCp->evntStr))->val;

      /* Check if storage for the sting hasn't been allocated by the user */
      if (str == NULLP)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
   }
   else if (elmntDef->type == TET_IA5STRXL_MEM)
   {
      str = ((TknStrXL *)(msgCp->evntStr))->val;

      /* Check if storage for the sting has't been allocated by the user */
      if (str != NULLP)
      {
         CM_ASN_ERR(msgCp, CM_ASN_MEM_ALRDY_ALLOCD, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }

      /* Allocate memory for val part: This cannot be done here, because
       * we don't know the length to allocate memory for TknStrXL. So,
       * allocate the memory in cmRemPrim when token type is 
       * TET_IA5STRXL_MEM. */
   }
   else
   {
      str = &((TknStr *)(msgCp->evntStr))->val[0];
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */
   if ((ret = cmRemPrim(msgCp->mBuf, msgCp, (U8 *)str, &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   if (elmntDef->type == TET_IA5STRXL || elmntDef->type == TET_IA5STRXL_MEM)
   {
      /* make the element present in the event structure */
      ((TknStrXL *)(msgCp->evntStr))->pres = TRUE;

      /* update the length of the primitive */
      ((TknStrXL *)(msgCp->evntStr))->len  = tmpLen;

      /* get the str again, because val part was allocated in case STRXL_MEM */
      str = ((TknStrXL *)(msgCp->evntStr))->val;
   }
   else
   {
      /* make the element present in the event structure */
      ((TknStr *)(msgCp->evntStr))->pres = TRUE;

      /* update the length of the primitive */
      ((TknStr *)(msgCp->evntStr))->len  = tmpLen;
   }

   /* verify the data */
   for (i = 0; i < tmpLen; i++)
   {
      /* mask the MSB. This is required because, sender may be sending 0/1
       * as MSB */ 
      str[i] = str[i] & ~IA5_CHARMASK;
   }

   /* now skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecIa5Str */


/*
*
*       Fun:   cmDecBitStr
*
*       Desc:  This function decodes a bit string. 
*
*       Ret:   ROK 
*
*       Notes: This function decodes only 8 bit enumerated bit
*              string. 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecBitStr 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecBitStr (msgCp) 
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknStr12     *evntStr;       /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       tmpLen;         /* message length */
   Buffer       *mBuf;          /* message buffer */
   MsgLen       i;              /* counter */

   /* temporary array for decoding */
   Data         pkArray[STR_BUFSIZE];

   TRC2(cmDecBitStr)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   evntStr  = (TknStr12 *)(msgCp->evntStr); /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */

   /* cm_asn_c_001.main_19 - modification - typecasting tmpLen to MsgLen instead of
    * U16.
    */
   if ((ret = cmRemPrim(mBuf, msgCp, (U8 *)(evntStr->val), 
                                         (MsgLen *)&tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   /* the first byte of the event structure contains the number of
      unused bits in the last byte, strip this byte and advance all
      bytes forward by 1 byte */

   for (i=0; i< (tmpLen -1); i++)
   {
      evntStr->val[i] = evntStr->val[i+1];
   }

   /* zero out the last octet as it is already advanced */
   evntStr->val[tmpLen -1] = 0;

   /* make the element present in the event structure */
   evntStr->pres = TRUE; 

   /* update the length of the primitive, one less to account for the
      byte containing the unused bits */

   evntStr->len = (tmpLen - 1);

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecBitStr */


/*
*
*       Fun:   cmDecSeq 
*
*       Desc:  This function decodes the sequence element 
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecSeq 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecSeq (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       lfLen;          /* Length of length field */
   MsgLen       seqLen;         /* length of the sequence */
   MsgLen       prevLen;        /* msg length before decoding element */
   MsgLen       nextLen;        /* msg length after decoding element */
   Bool         seqEoc;         /* eoc flag of the sequence */
   Buffer       *mBuf;          /* message buffer */
   U8           ignFlag;        /* ignore extra octets or not */
   CmElmntDef   **defPtr;       /* stored element defintion pointer */
   TknU8        *defStr;        /* stored structure pointer */
   MsgLen       i;              /* counter */
   

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmDecSeq)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &prevLen);
#endif

   if (prevLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */
   seqEoc   = FALSE;                   /* not a EOC encoding */
   defStr   = msgCp->evntStr;          /* event structure pointer */
   defPtr   = msgCp->elmntDef;         /* element defintion */

   /* get the ignore flag for this protocol */
   ignFlag  = IS_IGNORE_EXT(elmntDef->flagp, CM_ASN_MAP_IS41C); 

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* sequence of cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* sequence is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */

   /* now get the length of the sequence */
   if ((ret = cmFindLen(msgCp, tagSize, &seqLen, &seqEoc, &lfLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   if (seqLen > prevLen)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* strip of the tag and length bytes */
#ifdef CM_ASN_SS
   ret = SRemPreMsgMult(pkArray, (MsgLen)(tagSize + lfLen), mBuf);
#else
   CM_ASN_REM_PRE_MULT(pkArray, (tagSize + lfLen), msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* increment the data base pointer over the start of sequence element */
   msgCp->elmntDef++;
   elmntDef = *msgCp->elmntDef;

   /* make the sequence present as true */
   evntStr->pres = TRUE;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   /* go through the loop and decode all the elements of the sequence */

   while (elmntDef->type != TET_SETSEQ_TERM)
   {
      if (seqLen == 0)
      {
         /* reached end of sequence. If EOC,still EOC tag present in mBuf
            as seqLen does not include EOC tag */

         /* if sequence length was in EOC format, strip of the EOC tag */
         if (seqEoc == TRUE)
         {
             /* length in EOC format. Need to remove the EOC tag */
#ifdef CM_ASN_SS
             ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
             CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
             if (ret != ROK)
             {
                CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
                RETVALUE(ret);
             }
         }

         /* check for any mandatory element missing and return */
         if (cmChkSeqMandMis(msgCp) != ROK)
         {
            /* Mandatory element missing */
            CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, NULLP);  
            RETVALUE(RFAILED);
         }
         RETVALUE(ROK);
      } /* end if */

#if CM_ASN_SS
      /* get the message length before decoding the element */
      (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(msgCp, &prevLen);
#endif

      if (prevLen == 0)
      {
         CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }

      /* decode the element depending on the element type */
      if ((ret = cmDecElmnt(msgCp)) != ROK)
      {
         /* do not fill error as already filled in */ 
         RETVALUE(RFAILED);
      }

#if CM_ASN_SS
      /* get the message length after decoding the element */
      (Void) SFndLenMsg(msgCp->mBuf, &nextLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(msgCp, &nextLen);
#endif

      if ((seqEoc == TRUE) && (nextLen == 0))
      {
         CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
         RETVALUE(RFAILED);
      }

      /* decrement the sequence length depending on the element size */
      seqLen = (seqLen - (prevLen - nextLen)); 

      /* Update the element defintion pointer */
      elmntDef = *msgCp->elmntDef;

   } /* end while */

   if (seqLen != 0)
   {
      /* unknown bytes at the end of the sequence */

      if (ignFlag)
      {
         for (i=0; i < seqLen; i++)
         {
            /* ignore extra octets in the end */
#ifdef CM_ASN_SS
            ret = SRemPreMsg(pkArray, mBuf);
#else
            CM_ASN_REM_PRE(pkArray, msgCp, &ret);
#endif
            if (ret != ROK)
            {
               CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
               RETVALUE(ret);
            }
         }
      }
      else
      {
         /* unrecognized element at the end of sequence, call the handler */
         prevLen = 0;
         while (seqLen > 0)
         {
            if ((ret = cmCpyUnrecogElmnt(msgCp, &prevLen)) != ROK)
            {
               RETVALUE(RFAILED);
            }

            if (seqLen >= prevLen)
            {
               seqLen = (seqLen - prevLen);
            }
            else
            {
               RETVALUE(RFAILED);
            }

            prevLen = 0;
         }
      }

   } /* end if */

   if (seqEoc == TRUE)
   {
      /* sequence length does not include the EOC tag bytes */

      /* remove the two EOC bytes */
#ifdef CM_ASN_SS
      ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
      CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
      if (ret != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
         RETVALUE(ret);
      }
   }

   /* Assuming the event structure pointer will be automatically
      incremented when the individual elements are decoded */

   /* Increment the database pointer over end of sequence element */
   msgCp->elmntDef++;

   RETVALUE(ROK);

} /* cmDecSeq */


/*
*
*       Fun:   cmDecSetSeqOf  
*
*       Desc:  This function decodes the repeatable sequence or set
*             
*       Ret:   ROK 
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/

#ifdef ANSI
PUBLIC S16 cmDecSetSeqOf 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecSetSeqOf (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **defPtr;       /* stored element definition */
   S16          ret;            /* return value */
   U8           tagSize,tagSize2;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *defStr;        /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       lfLen;          /* Length of length field */
   MsgLen       msgLen;         /* Length of message */
   MsgLen       seqLen;         /* length of the sequence */
   MsgLen       prevLen;        /* msg length before decoding element */
   MsgLen       nextLen;        /* msg length after decoding element */
   Bool         seqEoc;         /* eoc flag of the sequence */
   S32          cntr;           /* counter for elements in set/seq of */
   S32          minDec;         /* minimum number of elements to be decoded */
   S32          repCntr;        /* counter for elements in set/seq of */
   Buffer       *mBuf;          /* message buffer */
#ifdef CM_ASN_SS
   U16          i;      
#endif

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];
   Data         pkArray2[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmDecSetSeqOf)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &msgLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &msgLen);
#endif

   if (msgLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);

         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */
   defPtr    = msgCp->elmntDef;        /* store element definition pointer */
   defStr    = (TknU8 *)msgCp->evntStr;/* store the event structure pointer */

   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */

   mBuf     = msgCp->mBuf;             /* get the message buffer */
   cntr     = elmntDef->repCntr;       /* get the repeat counter */
   minDec   = elmntDef->minLen;        /* get the minimum no. to be decoded */
   repCntr  = cntr;                    /* save the repeat counter */ 

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* sequence cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* skip the element */
         cmSkipElmnt(msgCp);

         RETVALUE(ROK);
      }
   }

   /* now get the length of the sequence */
   if ((ret = cmFindLen(msgCp, tagSize, &seqLen, &seqEoc, &lfLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* check if message length is less than sequence of length */
   if (msgLen < seqLen)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }
  
   /* strip of the tag and length bytes */
#ifdef CM_ASN_SS
   ret = SRemPreMsgMult(pkArray, (MsgLen)(tagSize + lfLen), mBuf);
#else
   CM_ASN_REM_PRE_MULT(pkArray, (tagSize + lfLen), msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   } 

   /* increment the data base pointer over the start of sequence element */
   msgCp->elmntDef++;
   elmntDef = *msgCp->elmntDef;
   if (elmntDef->type != TET_CHOICE)
      {
        /* get and check the tag, do not strip the tag bytes */
        if ((ret = cmGetTag(msgCp, pkArray2, 0, &tagSize2)) == RFAILED)
           {
              CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);
              RETVALUE(RFAILED);
           }
           /* check the tag against the database defintion of the element */
           if ((ret = cmChkTag(msgCp, pkArray2, tagSize2, &skipElmnt)) == RFAILED)
           {
              /* tag in error, check if the element can be skipped (optional) */
              if (skipElmnt == FALSE)
              {
                 /* sequence cannot be skipped (mandatory), return error */
                 CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);
                 RETVALUE(RFAILED);
              }
              else
              {
                 /* skip the element */
                 msgCp->elmntDef--;
                 cmSkipElmnt(msgCp);
                 /* put back the tag and length*/
#ifdef CM_ASN_SS
                 i = tagSize + lfLen;
                 while(i)
                 {
                    ret = SAddPreMsg(pkArray[i--], mBuf);
                    if (ret != ROK)
                    {
                       CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
                       RETVALUE(ret);
                    }
                 }
#else
               /* cm_asn_c_001.main_26: fix for ccpu00099066 -
                  skip the SetSeqOf if tag mismatches and return with RFAILED */
                 msgCp->sBuf.stIdx = msgCp->sBuf.stIdx + seqLen;
                 msgCp->sBuf.size  = (msgCp->sBuf.size - seqLen);
#endif
                 CM_ASN_ERR(msgCp, CM_ASN_EXTRA_PARAM, NULLP);
                 RETVALUE(RFAILED);
             }
          }
       }


   /* make the set/sequence of present as true */
   evntStr->pres = TRUE;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   /* go through the loop and decode all the elements of the sequence */

   while (cntr)
   {
      if (seqLen == 0)
      {
         /* reached end of sequence. If EOC,still EOC tag present in mBuf
            as seqLen does not include EOC tag */

         /* if sequence length was in EOC format, strip of the EOC tag */
         if (seqEoc == TRUE)
         {
             /* length in EOC format. Need to remove the EOC tag */
#ifdef CM_ASN_SS
            ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
            CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
            if (ret != ROK)
            {
               CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);  
               RETVALUE(ret);
            }
         }

         if (minDec > (repCntr - cntr))
         {
            /* minimum number of elements not decoded */

            CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);
            RETVALUE(RFAILED);
         }
         else
         {
            /* minimum number of elements decoded, return ROK */

            msgCp->elmntDef = defPtr;  /* restore the db pointer to start */ 
            msgCp->evntStr  = defStr;  /* restore the ev str ptr to start */ 
            cmSkipElmnt(msgCp);        /* skip the element */

            RETVALUE(ROK);

         } /* end if */

      } /* end if */

      /* restore the element defintion to start of sequence/set of */
      msgCp->elmntDef = defPtr;

      /* increment over the start of set/seq */
      msgCp->elmntDef++;

      /* get the element defintion */
      elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
      /* get the message length before decoding the element */
      (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(msgCp, &prevLen);
#endif

      if (prevLen == 0)
      {
         CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }

      /* decode the element depending on the element type */
      if ((ret = cmDecElmnt(msgCp)) != ROK)
      {
         /* do not fill in the error code as already filled in */
         RETVALUE(RFAILED);
      }

#if CM_ASN_SS
      /* get the message length after decoding the element */
      (Void) SFndLenMsg(msgCp->mBuf, &nextLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(msgCp, &nextLen);
#endif

      if ((seqEoc == TRUE) && (nextLen == 0))
      {
         CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
         RETVALUE(RFAILED);
      }

      /* decrement the sequence length depending on the element size */
      seqLen = (seqLen - (prevLen - nextLen)); 

      /* Update the element defintion pointer */
      elmntDef = *msgCp->elmntDef;

      /* decrement the counter */
      cntr--;

   } /* end while */

   /* check for the seq/set of length */
   if (seqLen != 0)
   {
      /* unknown bytes at the end of the sequence of */
      CM_ASN_ERR(msgCp, CM_ASN_EXTRA_PARAM, NULLP);  
      RETVALUE(RFAILED);
   }

   if (cntr != 0)
   {
      msgCp->elmntDef = defPtr;  /* restore the db pointer to seq of start */ 
      msgCp->evntStr  = defStr;  /* restore the ev str ptr to seq of start */ 
      cmSkipElmnt(msgCp);        /* skip the element */
   }
   else
   {
      /* just increment the db pointer over end of sequence element */
      msgCp->elmntDef++;
   }

   if (seqEoc == TRUE)
   {
      /* remove the two EOC bytes */
#ifdef CM_ASN_SS
      ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
      CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif

      if (ret != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
         RETVALUE(ret);
      }
   }

   RETVALUE(ROK);

} /* end of cmDecSetSeqOf */


/*
*
*       Fun:   cmDecUnconsSetSeqOf  
*
*       Desc:  This function decodes the repeatable sequence or set
*             
*       Ret:   ROK 
*              RFAILED
*
*       Notes: This functions is to decode token types :
*                         TET_UNCONS_SET_OF and  
*                         TET_UNCONS_SEQ_OF.
*
*              This is similar to decoding of TET_SET_OF and TET_SEQ_OF.
*
*              Only difference is the event structure for UNCONS token types
*              contains only array of pointers instead of actual array of 
*              structure in it. This is done to support SET OF and SEQUENCE OF 
*              with number of elements unknown. 
*
*              But, there is a maximum limit on the number of pointers in the
*              event structure, similar to the number of structures in 
*              SET OF/SEQUENCE OF event structures. This limit is specified in
*              the element definition to library.
*
*              Here, we dynamically allocate the memory for SET/SEQUENCE OF 
*              elements before calling the cmDecElmnt, if we have some thing 
*              to decode.
*
*              Everything else is similar to TET_SEQ_OF and TET_SET_OF.
*
*       File:  cm_asn.c
*
*/

#ifdef ANSI
PUBLIC S16 cmDecUnconsSetSeqOf 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecUnconsSetSeqOf (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **defPtr;       /* stored element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *defStr;        /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       lfLen;          /* Length of length field */
   MsgLen       msgLen;         /* Length of message */
   MsgLen       seqLen;         /* length of the sequence */
   MsgLen       prevLen;        /* msg length before decoding element */
   MsgLen       nextLen;        /* msg length after decoding element */
   Bool         seqEoc;         /* eoc flag of the sequence */
   S32          cntr;           /* counter for elements in set/seq of */
   S32          minDec;         /* minimum number of elements to be decoded */
   S32          repCntr;        /* counter for elements in set/seq of */
   Buffer       *mBuf;          /* message buffer */
   TknU8        **evntPtrs;     /* pointer to list of event structure 
                                 * pointers in SET/SEQ OF */
   U16          ind;          /* index to the above array of pointers */

   /* temporary array for decoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmDecUnconsSetSeqOf)

   /* initialize index */
   ind = 0;

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &msgLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &msgLen);
#endif

   if (msgLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);

         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */
   defPtr    = msgCp->elmntDef;        /* store element definition pointer */
   defStr    = (TknU8 *)msgCp->evntStr;/* store the event structure pointer */

   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */

   mBuf     = msgCp->mBuf;             /* get the message buffer */
   cntr     = elmntDef->repCntr;       /* get the repeat counter */
   minDec   = elmntDef->minLen;        /* get the minimum no. to be decoded */
   repCntr  = cntr;                    /* save the repeat counter */ 

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* sequence cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* skip the element */
         cmSkipElmnt(msgCp);

         RETVALUE(ROK);
      }
   }

   /* now get the length of the sequence */
   if ((ret = cmFindLen(msgCp, tagSize, &seqLen, &seqEoc, &lfLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* check if message length is less than sequence of length */
   if (msgLen < seqLen)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }
  
   /* strip of the tag and length bytes */
#ifdef CM_ASN_SS
   ret = SRemPreMsgMult(pkArray, (MsgLen)(tagSize + lfLen), mBuf);
#else
   CM_ASN_REM_PRE_MULT(pkArray, (tagSize + lfLen), msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   } 

   /* increment the data base pointer over the start of sequence element */
   msgCp->elmntDef++;
   elmntDef = *msgCp->elmntDef;

   /* make the set/sequence of present as true */
   evntStr->pres = TRUE;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   /* set the pointer to array of pointers in SET OF/SEQ OF event structure */
   evntPtrs = (TknU8 **)((PTR)defStr + sizeof(TknU8));

   /* go through the loop and decode all the elements of the sequence */

   while (cntr)
   {
      if (seqLen == 0)
      {
         /* reached end of sequence. If EOC,still EOC tag present in mBuf
            as seqLen does not include EOC tag */

         /* if sequence length was in EOC format, strip of the EOC tag */
         if (seqEoc == TRUE)
         {
             /* length in EOC format. Need to remove the EOC tag */
#ifdef CM_ASN_SS
            ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
            CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
            if (ret != ROK)
            {
               CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);  
               RETVALUE(ret);
            }
         }

         if (minDec > (repCntr - cntr))
         {
            /* minimum number of elements not decoded */

            CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);
            RETVALUE(RFAILED);
         }
         else
         {
            /* minimum number of elements decoded, return ROK */

            msgCp->elmntDef = defPtr;  /* restore the db pointer to start */ 
            msgCp->evntStr  = defStr;  /* restore the ev str ptr to start */ 
            cmSkipElmnt(msgCp);        /* skip the element */

            RETVALUE(ROK);

         } /* end if */

      } /* end if */

      /* restore the element defintion to start of sequence/set of */
      msgCp->elmntDef = defPtr;

      /* increment over the start of set/seq */
      msgCp->elmntDef++;

      /* get the element defintion */
      elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
      /* get the message length before decoding the element */
      (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(msgCp, &prevLen);
#endif

      if (prevLen == 0)
      {
         CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }

      /* make sure that memory for the element in the SET OF/ SEQ OF is not
       * allocated */
      if (evntPtrs[ind] != NULLP)
      {
         CM_ASN_ERR(msgCp, CM_ASN_MEM_ALRDY_ALLOCD, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }

      /* now allocate the memory for this element */
      if (SGetSBuf(msgCp->region, 
                   msgCp->pool, 
                   (Data **)&evntPtrs[ind], 
                   (*msgCp->elmntDef)->evSize) != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);
         RETVALUE(RFAILED);
      }

      /* zero the memory */
      cmZero((U8 *)evntPtrs[ind], (*msgCp->elmntDef)->evSize);

      /* set correct event structure pointer to msgCp->evntStr */
      msgCp->evntStr = evntPtrs[ind];

      /* decode the element depending on the element type */
      if ((ret = cmDecElmnt(msgCp)) != ROK)
      {
         /* do not fill in the error code as already filled in */
         RETVALUE(RFAILED);
      }
      else
      {
         ind++;
      }

#if CM_ASN_SS
      /* get the message length after decoding the element */
      (Void) SFndLenMsg(msgCp->mBuf, &nextLen);
#else
      /* get the total length of the message */
      CM_ASN_FND_LEN(msgCp, &nextLen);
#endif

      if ((seqEoc == TRUE) && (nextLen == 0))
      {
         CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
         RETVALUE(RFAILED);
      }

      /* decrement the sequence length depending on the element size */
      seqLen = (seqLen - (prevLen - nextLen)); 

      /* Update the element defintion pointer */
      elmntDef = *msgCp->elmntDef;

      /* decrement the counter */
      cntr--;

   } /* end while */

   /* check for the seq/set of length */
   if (seqLen != 0)
   {
      /* unknown bytes at the end of the sequence of */
      CM_ASN_ERR(msgCp, CM_ASN_EXTRA_PARAM, NULLP);  
      RETVALUE(RFAILED);
   }

   msgCp->elmntDef = defPtr;  /* restore the db pointer to seq of start */ 
   msgCp->evntStr  = defStr;  /* restore the ev str ptr to seq of start */ 
   cmSkipElmnt(msgCp);        /* skip the element */


   if (seqEoc == TRUE)
   {
      /* remove the two EOC bytes */
#ifdef CM_ASN_SS
      ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
      CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif

      if (ret != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
         RETVALUE(ret);
      }
   }

   RETVALUE(ROK);

} /* end of cmDecUnconsSetSeqOf */


/*
*
*       Fun:   cmDecSet
*
*       Desc:  This function decodes the repeatable sequence or set
*             
*       Ret:   ROK 
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/

#ifdef ANSI
PUBLIC S16 cmDecSet
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecSet (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **defPtr;       /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *strPtr;        /* pointer to event structure */
   TknU8        *tmpPtr;        /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       lfLen;          /* Length of length field */
   MsgLen       prevLen;        /* Length of message */
   MsgLen       nextLen;        /* Length of message */
   MsgLen       msgLen;         /* Length of message */
   MsgLen       setLen;         /* length of the set */
   Bool         setEoc;         /* eoc flag of the set */
   Buffer       *mBuf;          /* message buffer */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmDecSet)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &msgLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &msgLen);
#endif

   if (msgLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */
   defPtr    = msgCp->elmntDef;        /* store element definition pointer */
   strPtr    = msgCp->evntStr;         /* store event structure pointer */
   tmpPtr    = msgCp->evntStr;         /* temporary event structure pointer */

   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* set cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* set is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */

   /* now get the length of the set */
   if ((ret = cmFindLen(msgCp, tagSize, &setLen, &setEoc, &lfLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* check if message length is less than set of length */
   if (msgLen < setLen)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }
  
   /* strip of the tag and length bytes */
#ifdef CM_ASN_SS
   ret = SRemPreMsgMult(pkArray, (MsgLen)(tagSize + lfLen), mBuf);
#else
   CM_ASN_REM_PRE_MULT(pkArray, (tagSize + lfLen), msgCp, &ret);
#endif

   if (ret != ROK)
   {
      /* cm_asn_c_001.main_24 : set the error code */
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, &elmntDef->tag);
      RETVALUE(RFAILED);
   } 

   /* make the set/sequence of present as true */
   evntStr->pres = TRUE;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   /* update the temporary pointer */
   tmpPtr = msgCp->evntStr;

   /* go through the loop and decode all the elements of the set */
   while (setLen != 0)
   {
      /* restore the element defintion to start of set/set of */
      msgCp->elmntDef = defPtr;

      /* increment over the start of set/seq */
      msgCp->elmntDef++;

      /* restore the element defintion to first element */
      elmntDef = *msgCp->elmntDef;

      /* restore the event structure pointer to first event structure */
      msgCp->evntStr = tmpPtr;

      /* get and check the tag, do not strip the tag bytes */
      if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
      {
         CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
         RETVALUE(RFAILED);
      }

      while (elmntDef->type != TET_SETSEQ_TERM)
      {

         /* in case of a choice, need to go inside the choice to decode  */
         /* because the choice type itself does not have a tag. So need  */
         /* to check if this tag matches any of the choice elements.     */
         /* Also there is a assumption here that the tag of one of the   */
         /* choice elements will not match the tag of any other elements */
         /* of the set. If this happens, this will make the decode       */
         /* impossible.                                                  */

         if (elmntDef->type == TET_CHOICE)
         {
 
#if CM_ASN_SS
            /* get the message length before decoding the element */
            (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
            /* get the total length of the message */
            CM_ASN_FND_LEN(msgCp, &prevLen);
#endif
            /* verify the length of the message */
            if (prevLen == 0)
            {
               CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
               RETVALUE(RFAILED);
            }

            /* set the parameter to tell cmDecChoice that the CHOICE is a member
             * of SET. In this case, cmDecChoice will not return RFAILED if it
             * finds a mandatory parameter missing, because in SET, we may find
             * this mandatory parameter at a different position in the encoded
             * message. Also, cmDecChoice will not set the pres field of the
             * event structure as FALSE.
            */
            msgCp->choiceResInSet = TRUE;

            /* cmDecChoice will set this parameter to ROK if some CHOICE
             * parameter has been decoded. We need to know whether cmDecChoice
             * has actually decoded the parameter to break out of this while
             * loop. Otherwise, we have to skip the element and continue with
             * this while loop.
            */
            msgCp->choiceRes = RFAILED;

            /* decode the CHOICE type parameter */
            if ((ret = cmDecChoice(msgCp)) != ROK)
            {
               /* error code is already filled in */
               RETVALUE(RFAILED);
            }

            /* reset the choiceResInSet parameter */
            msgCp->choiceResInSet = FALSE;

            /* if cmDecChoice has actually decoded some parameter, then
             * decrement the setLen accordingly and break from the while loop. 
             * Otherwise, skip the element and continue with the while loop.
            */
            if (msgCp->choiceRes == ROK)
            {

#if CM_ASN_SS
               /* get the message length after decoding the element */
               (Void) SFndLenMsg(msgCp->mBuf, &nextLen);
#else
               /* get the total length of the message */
               CM_ASN_FND_LEN(msgCp, &nextLen);
#endif

               if ((setEoc == TRUE) && (nextLen == 0))
               {
                  CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
                  RETVALUE(RFAILED);
               }

               /* decrement the set length depending on the element size */
               setLen = (setLen - (prevLen - nextLen)); 

               /* get the updated element definition */
               break;
            }
            else
            {
               if (msgCp->choiceRes != RSKIP)
               {
                  cmSkipElmnt(msgCp);
               }
               elmntDef = *msgCp->elmntDef;
               continue;
            }
         }
         else
         {
            /* check the tag against the database defintion of the element */

            /* Check for element flag is added. Without                    */
            /* this checking, if one element is not defined for this       */
            /* protocol, it will be skip at cmDecElmnt() and will not be   */
            /* removed from mBuf. When go back to while (setLen != 0) loop,*/
            /* TAG match for this element will be found again and it will  */
            /* go into infinite loop. Function to handle the unrecognized  */
            /* element will never be called.                               */
            if (((ret=cmDecChkFlag(msgCp)) == RFAILED) ||
                ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) 
                                                               == RFAILED))
            {
               /* tag defintions don't match, continue till we find one */
               cmSkipElmnt(msgCp);
               elmntDef = *msgCp->elmntDef;
               continue;
            } 
         }
 
#if CM_ASN_SS
         /* get the message length before decoding the element */
         (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
         /* get the total length of the message */
         CM_ASN_FND_LEN(msgCp, &prevLen);
#endif

         if (prevLen == 0)
         {
            CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
            RETVALUE(RFAILED);
         }

         /* decode the element depending on the element type */
         if ((ret = cmDecElmnt(msgCp)) != ROK)
         {
            /* error code already filled, don't do it again */
            RETVALUE(RFAILED);
         }

#if CM_ASN_SS
         /* get the message length after decoding the element */
         (Void) SFndLenMsg(msgCp->mBuf, &nextLen);
#else
         /* get the total length of the message */
         CM_ASN_FND_LEN(msgCp, &nextLen);
#endif

         if ((setEoc == TRUE) && (nextLen == 0))
         {
            CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
            RETVALUE(RFAILED);
         }

         /* decrement the set length depending on the element size */
         setLen = (setLen - (prevLen - nextLen)); 

         break;

      } /* end while */ 

      if (elmntDef->type == TET_SETSEQ_TERM)
      {
         /* did not find a match, unrecognized element */

         MsgLen    uLen;     /* length of the unrecongnized element */

         /* handle the unrecognized element */
         if ((ret = cmCpyUnrecogElmnt(msgCp, &uLen)) != ROK)
         {
            RETVALUE(RFAILED);
         }

         if (setLen >= uLen)
         {
            setLen = (setLen - uLen);
         }
         else
         {
            RETVALUE(RFAILED);
         }
      }

   } /* end while */

   if (setLen == 0)
   {
      /* reached end of set. If EOC,still EOC tag present in mBuf
         as setLen does not include EOC tag */

      /* if set length was in EOC format, strip of the EOC tag */
      if (setEoc == TRUE)
      {
          /* length in EOC format. Need to remove the EOC tag */
#ifdef CM_ASN_SS
          ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
          CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
          if (ret != ROK)
          {
             CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
             RETVALUE(ret);
          }
      }

      /* restore the element defintion to start set */
      msgCp->elmntDef = defPtr;

      /* restore the event structure */
      msgCp->evntStr = strPtr;

      /* Need to check here if any mandatory element was missing in
         the set */

      if ((ret = cmChkSetMandMis(msgCp)) == RFAILED)
      {
         /* Mandatory element missing */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, NULLP);  
         RETVALUE(RFAILED);
      }

   }
   else
   {
      /* unrecognized elements left to be decoded */

      MsgLen     uLen;       /* length of the unrecognized element */

      while (setLen > 0)
      {
         if ((ret = cmCpyUnrecogElmnt(msgCp, &uLen)) != ROK)
         {
            RETVALUE(RFAILED);
         }

         if (setLen >= uLen)
         {
            setLen = (setLen - uLen);
         }
         else
         {
            RETVALUE(RFAILED);
         }

         uLen = 0;
      }
   }

   /* restore the element defintion to set start */
   msgCp->elmntDef = defPtr;

   /* restore the event structure */
   msgCp->evntStr = strPtr;

   /* now skip over the set */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* end of cmDecSet */


/* **************************************************************** */


/*
*
*       Fun:   cmEncChoice
*
*       Desc:  This function encodes the choice ASN.1 type
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncChoice 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncChoice(msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **tmpDef;       /* temporary element defintion */
   TknU8        *evntStr;       /* pointer to event structure */
   U8           elmntIdx;       /* element index */
   TknU8        *tmpStr;        /* temporary pointer */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   U16          i;              /* counter */

   TRC2(cmEncChoice)

   /* Note : for choice type itself, there is no tag and length */

   /* check for the version */
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   tmpDef   = msgCp->elmntDef;         /* store the element defn. pointer */
   tmpStr   = (TknU8 *)msgCp->evntStr; /* get the event structure */
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */

   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* get the element index */
   elmntIdx = evntStr->val;

   /* increment the event structure over present token */
   CM_ASN_SKIP_TKNPRES(msgCp);

   evntStr = msgCp->evntStr;

   /* increment element definition over start of choice */
   msgCp->elmntDef++;

   /* initialize counter */
   i = 1;

   /* skip n elements in the database */
   while ((i < elmntIdx) && (elmntDef->type != TET_SETSEQ_TERM))
   {
      /* skip the element */
      cmSkipElmnt(msgCp);

      /* restore the event structure pointer */
      msgCp->evntStr = evntStr;

      /* update the element defintion */
      elmntDef = *msgCp->elmntDef;

      i++;
   }

   if (elmntDef->type == TET_SETSEQ_TERM)
   {
      /* reached the end of choice, error */
      CM_ASN_ERR(msgCp, CM_ASN_BAD_IDX, &(*tmpDef)->tag);
      RETVALUE(RFAILED);
   }

   /* now encode the element */
   if (cmEncElmnt(msgCp) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* restore the element defintion pointer to start of choice */
   msgCp->elmntDef = tmpDef;

   /* restore the event structure pointer to start of choice */
   msgCp->evntStr  = tmpStr;

   /* now skip the entire choice */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncChoice */


/*
*
*       Fun:   cmDecChoice
*
*       Desc:  This function decodes the repeatable sequence or set
*             
*       Ret:   ROK 
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/

#ifdef ANSI
PUBLIC S16 cmDecChoice
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecChoice (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **defPtr;       /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *strPtr;        /* pointer to event structure */
   TknU8        *tmpPtr;        /* temporary pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       msgLen;         /* Length of message */
   Buffer       *mBuf;          /* message buffer */
   U16          j;              /* element counter */
   Bool         multChoice;     /* multiple choice flag */
   CmElmntDef   **tmpDef;       /* temp ptr to element definition */

   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmDecChoice)


   multChoice = FALSE;

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      /* changes for decoding of nested choice with no tags for second level of choice */
      msgCp->choiceRes = RSKIP;
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &msgLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &msgLen);
#endif

   if (msgLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         msgCp->choiceRes = RFAILED;
         /* choiceResInSet parameter tell cmDecChoice that this CHOICE is member
          * of a SET. In this case, cmDecChoice will not return RFAILED if it
          * finds a mandatory parameter missing, because in SET, we may find
          * this mandatory parameter at a different position in the encoded
          * message. 
         */
         if (msgCp->choiceResInSet == TRUE)
            RETVALUE(ROK);
         else
            RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         msgCp->choiceRes = RFAILED;
         RETVALUE(ROK);
      }

   } /* end if */

   skipElmnt = FALSE;                  /* do not skip the element */
   defPtr    = msgCp->elmntDef;        /* store element definition pointer */
   strPtr    = msgCp->evntStr;         /* store event structure pointer */

   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* increment over the start of choice */
   msgCp->elmntDef++;

   /* get the element defintion of first element */
   elmntDef = *msgCp->elmntDef;

   /* skip over the present token */
   cmIncPtr((PTR *)&msgCp->evntStr, sizeof(TknU8)); 

   /* initialize the element count */
   j = 1;

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      msgCp->choiceRes = RFAILED;
      RETVALUE(RFAILED);
   }

   while (elmntDef->type != TET_SETSEQ_TERM)
   {
      /* decoding of nested choice with no tags for second level of choice.
       */
      if (elmntDef->type == TET_CHOICE)
      {
         /* save eventStr and elmntDef ptrs */
         tmpPtr = msgCp->evntStr;
         tmpDef = msgCp->elmntDef;

         multChoice = TRUE;
         msgCp->choiceRes = RFAILED;

         /* decode the nested choice */
         cmDecChoice(msgCp);

         /* Break the loop, if the element was decoded successfully */
         if (msgCp->choiceRes == ROK)
         {
            break;
         }

         multChoice = FALSE;

         /* restore evntStr and elmntDef ptrs in msgCp then call skipElmnt */
         msgCp->evntStr = tmpPtr;
         msgCp->elmntDef = tmpDef;
         cmSkipElmnt(msgCp);

         elmntDef = *msgCp->elmntDef;
         msgCp->evntStr = tmpPtr;

         j++;
         continue;
      }

      /* check the tag against the database defintion of the element */
      if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
      {

         /* store the event structutre pointer. this is required
            because for decoding each element of the choice we need
            to make sure that we start from the start of the event
            structure pointer */

         tmpPtr = msgCp->evntStr;

         /* tag defintions don't match, continue till we find one */
         cmSkipElmnt(msgCp);
         elmntDef = *msgCp->elmntDef;

         /* restore the pointer */
         msgCp->evntStr = tmpPtr;

         j++;
         continue;
      } 
      else
      {
         /* found the element, break out of the while loop */
         break;
      }

   } /* end while */

   if (elmntDef->type == TET_SETSEQ_TERM)
   {
      /* did not find a match, unknown element */

      /* indicate that the choice is not present */
      /* choiceResInSet parameter tells cmDecChoice that this CHOICE is a member
       * of SET. In this case, cmDecChoice will not set the pres field of the
       * event structure as FALSE.
      */
      if (msgCp->choiceResInSet != TRUE)
      {
          strPtr->pres = FALSE;                   /* choice present */
          strPtr->val  = 0;                       /* element index */
      }

      /* Restore the pointers to the start of choice */
      msgCp->elmntDef = defPtr;
      msgCp->evntStr  = strPtr;

      /* Check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         msgCp->choiceRes = RFAILED;
         /* choiceResInSet parameter tell cmDecChoice that this CHOICE is member
          * of a SET. In this case, cmDecChoice will not return RFAILED if it
          * finds a mandatory parameter missing, because in SET, we may find
          * this mandatory parameter at a different position in the encoded
          * message. 
         */
         if (msgCp->choiceResInSet == TRUE)
            RETVALUE(ROK);
         else
            RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         /* changes for decoding of nested choice with no tags for second level of choice */
         msgCp->choiceRes = RSKIP;
         RETVALUE(ROK);
      }
   }
   else
   {
      /* make the element as present in the event structure */
      strPtr->pres = TRUE;                          /* choice present */
      strPtr->val  = (U8) j;                        /* element index */
   }

   /* decode the element depending on the element type */
   if (multChoice == FALSE && ((ret = cmDecElmnt(msgCp)) != ROK))
   {
      msgCp->choiceRes = RFAILED;
      /* error code already filled, don't do it again */
      RETVALUE(RFAILED);
   }
   
   /* set the choice result to ROK */
   msgCp->choiceRes = ROK;

   /* restore the element defintion to choice start */
   msgCp->elmntDef = defPtr;

   /* restore the event structure */
   msgCp->evntStr = strPtr;

   /* now skip over the set */
   cmSkipElmnt(msgCp);

   /* set the choice result to ROK */
   msgCp->choiceRes = ROK;

   RETVALUE(ROK);

} /* end of cmDecChoice */

/*
*
*       Fun:    cmEncOid
*
*       Desc:   This function encodes an object identifier type
*
*       Ret:   ROK (encoding successful)
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes:  
*
*       File:   cm_asn.c
*
*/
#ifdef ANSI
PUBLIC S16 cmEncOid
(
CmAsnMsgCp    *msgCp             /* message control pointer */
)
#else
PUBLIC S16 cmEncOid (msgCp)
CmAsnMsgCp    *msgCp;            /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   TknOid       *evntStr;        /* pointer to event structure */
   U8            flag;           /* protocol flag */
   U16           numBytes;       /* number of bytes copied */
   U32           val;            /* value of an object Identifier component */
   Data          pkArray[OIDSTR_BUFSIZE]; /* temporary array for encoding */
   U16           idx;            /* Index thru pkArray */
   U16           i;              /* look counters */
   S16           ret;            /* return value */

   TRC2(cmEncOid)

   /* check for the version */
    
   elmntDef = *msgCp->elmntDef;         /* get the element defintion */ 
   evntStr  = (TknOid *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, msgCp->evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   idx = numBytes;
 
   idx++;     /* encode length later */

   /* derive value of first subidentifier from the first two Object Identifier
      components in the Object identifier string being encoded */

   val = (U32)(40 * evntStr->val[0] + evntStr->val[1]);

   for (i = 2; i <= evntStr->len; i++)
   {
       U8 tmpStr[6];   /* temporary string to hold subidentifier value */
       U8 k=0;         /* index into the temporary string */

       if (val == 0)
          pkArray[idx++] = 0;
       else
       {
          while (val)
          {
             tmpStr[k++] = (0x80 | (val & 0x7F));
             val >>= 7;
          }
          tmpStr[0] = (tmpStr[0] & 0x7F); /* bit 8 of LSB should be reset */

          /* copy the encoded value to the oid string in reverse order */
          while (k > 0)
          {
             pkArray[idx++] = tmpStr[k-1];
              k--;
          }
       }
       val = (U32)evntStr->val[i];
   }

   pkArray[numBytes] = idx - numBytes -1;  /* Encode length */

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to encode
      this primitive type */
   msgCp->crntEncLen += idx;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the tag, length and value */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)idx, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, idx, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncOid */

/*
*
*       Fun:    cmDecOid 
*
*       Desc:  This function decodes an Object Identifier value  
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecOid 
(
CmAsnMsgCp   *msgCp              /* message control pointer */
)
#else
PUBLIC S16 cmDecOid (msgCp)
CmAsnMsgCp   *msgCp;             /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   TknOid       *evntStr;        /* pointer to event structure */
   Buffer       *mBuf;           /* message buffer */
   U8            tagSize;        /* size of the tag */
   Bool          skipElmnt;      /* need to skip the element */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen        tmpLen;         /* message length */
   Data          unPkArray[OIDSTR_BUFSIZE]; /* temporary array for decoding */
   U16           idx;            /* index */
   U8            i;
   S16           ret;            /* return value */

   TRC2(cmDecOid)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                     /* do not skip the element */

   evntStr  = (TknOid *)(msgCp->evntStr); /* get the event structure */
   elmntDef = *msgCp->elmntDef;           /* get the element definition */
   mBuf     = msgCp->mBuf;                /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, unPkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, unPkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */

   if ((ret = cmRemPrim(mBuf, msgCp, unPkArray, &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   idx = 0;
   i   = 0;
   while (tmpLen > 0)
   {
      U32 val = 0;
      U32 oid1;
      U32 oid2;
      U8  data;

      do
      {
         data = unPkArray[i];
         val = ((val << 7) | ( data & 0x7F));
         i++;
         tmpLen--;
      } while (data & 0x80);
   
      if (idx == 0)
      {
         oid1 = (val / 40);
         oid2 = (val % 40);
         if (oid1 > 2)
         {
            oid2 += ((oid1 - 2) * 40);
            oid1 = 2;
         }
         evntStr->val[idx++] = oid1;
         evntStr->val[idx++] = oid2;
      }
      else
         evntStr->val[idx++] = val;
   }
      
   /* make the element present in the event structure */
   evntStr->pres = TRUE;

   /* update the length of the primitive */
   evntStr->len = (U8) idx;

   /* skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecOid */

/*
*
*       Fun:   cmEncInteger
*
*       Desc:  This function encodes an Integer type 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncInteger 
(
CmAsnMsgCp   *msgCp              /* message control pointer */
)
#else
PUBLIC S16 cmEncInteger (msgCp)
CmAsnMsgCp   *msgCp;             /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   TknS32       *evntStr;        /* pointer to event structure */
   U8            flag;           /* protocol flag */
   S32           val;            /* pointer to the string */
   U8            neg;            /* flag to indicate if val is negative */
   U16           numBytes;       /* number of bytes copied */
   Data          pkArray[STR_BUFSIZE]; /* temporary array for encoding */
   U16           i;              /* counter */
   S16           ret;            /* return value */

   TRC2(cmEncInteger)

   /* check for the version */
    
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr = (TknS32 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, msgCp->evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   val = evntStr->val;           /* value in the event structure */
   neg = 0;
   if (val < 0)
   {
      neg = 1;
      val = -val;
   }
 
   if (val >= 0)
   {
      if ((U32)val < (U32)0x80)
      {
         numBytes = (U8)1;
      }
      else if ((U32)val < (U32)0x8000)
      {
         /* take care of the boundary condition, -128 should be coded in 
            1 octet and +128 should be in 2 octets */
         if ((val == (U32)0x80) && (neg))
            numBytes = (U8)1;
         else
            numBytes = (U8)2;
      }
      else if ((U32)val < (U32)0x800000)
      {
         /* take care of the boundary condition */
         if ((val == (U32)0x8000) && (neg))
            numBytes = (U8)2;
         else
            numBytes = (U8)3;
      }
      else if ((U32)val < (U32)0x80000000)
      {
         /* take care of the boundary condition */
         if ((val == (U32)0x800000) && (neg))
            numBytes = (U8)3;
         else
            numBytes = (U8)4;
      }
   }
   else  /* this is to account for -2^31 */
      numBytes = (U8)4;

   pkArray[i++] = (Data) numBytes;
  
   switch(numBytes)
   {
      case 4:
         pkArray[i++] = GetHiByte(GetHiWord(evntStr->val));;
      case 3:
         pkArray[i++] = GetLoByte(GetHiWord(evntStr->val));;
      case 2:
         pkArray[i++] = GetHiByte(GetLoWord(evntStr->val));;
      case 1:
         pkArray[i++] = GetLoByte(GetLoWord(evntStr->val));;
   }
#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to encode
      this primitive type */
   msgCp->crntEncLen += i;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the tag, length and value */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncInteger */

/*
*
*       Fun:    cmDecInteger 
*
*       Desc:  This function decodes an octet string value  
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecInteger 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecInteger (msgCp)
CmAsnMsgCp   *msgCp;             /* message control pointer */
#endif
{
   CmElmntDef    *elmntDef;      /* element definition */
   TknS32        *evntStr;       /* pointer to event structure */
   Buffer        *mBuf;          /* message buffer */
   U8             tagSize;       /* size of the tag */
   Bool           skipElmnt;     /* need to skip the element */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen         tmpLen;        /* message length */
   S32            val;           /* decoded integer value */
   Data           unPkArray[STR_BUFSIZE]; /* temporary array for decoding */
   U8             i;             /* index into unPkArray */
   S16            ret;           /* return value */

   TRC2(cmDecInteger)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   evntStr  = (TknS32 *)(msgCp->evntStr); /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, unPkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, unPkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */

   if ((ret = cmRemPrim(mBuf, msgCp, unPkArray, &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   /* make the element present in the event structure */
   evntStr->pres = TRUE;

   val = (S32)(unPkArray[0] & 0x7F);  /* reset the 8th bit of MSB */
   for (i = 1; i < tmpLen; i++)
       val = (S32)((val << 8) | unPkArray[i]);

   if (unPkArray[0] & 0X80)           /* if negative value, make it negative */
      val = -((1 << (tmpLen * 8 - 1)) - val);

   evntStr->val = val;

   /* skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecInteger */


/*
*
*       Fun:   cmEncIntRng
*
*       Desc:  This function encodes an Integer type with range check.
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncIntRng 
(
CmAsnMsgCp   *msgCp              /* message control pointer */
)
#else
PUBLIC S16 cmEncIntRng (msgCp)
CmAsnMsgCp   *msgCp;             /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;       /* element definition */
   TknU32       *evntStr;        /* pointer to event structure */
   U8            flag;           /* protocol flag */
   U32           val;            /* pointer to the string */
   U16           numBytes;       /* number of bytes copied */
   Data          pkArray[STR_BUFSIZE]; /* temporary array for encoding */
   U16           i;              /* counter */
   S16           ret;            /* return value */
   U8            zero_oct;       /* flag TRUE if leading octet to be zero */

   TRC2(cmEncIntRng)

   /* check for the version */
    
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr = (TknU32 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, msgCp->evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;

   val = evntStr->val;           /* value in the event structure */

   /* check for value within specified range */
   if ((val < elmntDef->minLen) || (val > elmntDef->maxLen))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CMASNLOGERROR(ERRCLS_DEBUG, ECMASN010, (ErrVal)val, 
                   "cmEncIntRng () failed: value out of range");
#endif /* ERRCLASS & ERRCLS_DEBUG */
      /* cm_asn_c_001.main_24 : set the error code */
      CM_ASN_ERR(msgCp, CM_ASN_OUT_RANGE, &elmntDef->tag);
      RETVALUE(RFAILED);
   }

   zero_oct = FALSE;
   /* set zero_oct flag to true if MSB bit is set to 1 to fill a leading zero
    * octet.
    */

   if ((U32)val > (U32)0xFFFFFF)
   {
      if (val < 0x80000000)
        numBytes = (U8)4;
      else
      {
        numBytes = (U8)5;
        zero_oct = TRUE; 
      }
   }
   else if ((U32)val > (U32)0xFFFF)
   {
      if (val < 0x800000)
        numBytes = (U8)3;
      else
      {
        numBytes = (U8)4;
        zero_oct = TRUE;
      }
   }
   else if ((U32)val > (U32)0xFF)
   {
      if (val < 0x8000)
        numBytes = (U8)2;
      else
      {
        numBytes = (U8)3;
        zero_oct = TRUE;
      }
   }
   else 
   {
      if (val < 0x80)
        numBytes = (U8)1;
      else
      {
        numBytes = (U8)2;
        zero_oct = TRUE;
      }
   }

   pkArray[i++] = (Data) numBytes;
   
   /* fill leading zero octet if flag is TRUE */
   if (zero_oct)
   {
      pkArray[i++] = 0x00;
      numBytes--;
   }

   switch(numBytes)
   {
      case 4:
         pkArray[i++] = GetHiByte(GetHiWord(evntStr->val));;
      case 3:
         pkArray[i++] = GetLoByte(GetHiWord(evntStr->val));;
      case 2:
         pkArray[i++] = GetHiByte(GetLoWord(evntStr->val));;
      case 1:
         pkArray[i++] = GetLoByte(GetLoWord(evntStr->val));;
   }
#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to encode
      this primitive type */
   msgCp->crntEncLen += i;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the tag, length and value */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   /* skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncIntRng */

/*
*
*       Fun:    cmDecIntRng 
*
*       Desc:  This function decodes an INTEGER value with range check. 
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecIntRng 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecIntRng (msgCp)
CmAsnMsgCp   *msgCp;             /* message control pointer */
#endif
{
   CmElmntDef    *elmntDef;      /* element definition */
   TknU32       *evntStr;        /* pointer to event structure */
   Buffer        *mBuf;          /* message buffer */
   U8             tagSize;       /* size of the tag */
   Bool           skipElmnt;     /* need to skip the element */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen         tmpLen;        /* message length */
   U32            val;           /* decoded integer value */
   Data           unPkArray[STR_BUFSIZE]; /* temporary array for decoding */
   U8             i;             /* index into unPkArray */
   S16            ret;           /* return value */

   TRC2(cmDecIntRng)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   evntStr = (TknU32 *)msgCp->evntStr;  /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, unPkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, unPkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */

   if ((ret = cmRemPrim(mBuf, msgCp, unPkArray, &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   /* make the element present in the event structure */
   evntStr->pres = TRUE;

   val = (U32)unPkArray[0];

   /* check for the leading zero octet and skip if present */
   if ((val == 0) && (tmpLen > 1))
   {
      val = (U32)unPkArray[1];
      for (i = 2; i < tmpLen; i++)
      {
        val = (U32)((val << 8) | unPkArray[i]);
      }
   }
   else
   {
      for (i = 1; i < tmpLen; i++)
        val = (U32)((val << 8) | unPkArray[i]);
   }

   evntStr->val = val;

   /* check for the value in specified range */
   if ((val < elmntDef->minLen) || (val > elmntDef->maxLen))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      CMASNLOGERROR(ERRCLS_DEBUG, ECMASN011, (ErrVal)val, 
                   "cmDecIntRng () failed: value out of range");
#endif /* ERRCLASS & ERRCLS_DEBUG */
      /* cm_asn_c_001.main_24 : set the error code */
      CM_ASN_ERR(msgCp, CM_ASN_OUT_RANGE, &elmntDef->tag); 
      RETVALUE(RFAILED);
   }

   /* skip over the element */
   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecIntRng */

/* ------------------------------------------------------------- */

/*
*
*       Fun:   cmEncBool   
*
*       Desc:  This function encodes a boolean type
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncBool 
(
CmAsnMsgCp  *msgCp    /* message control pointer */
)
#else
PUBLIC S16 cmEncBool (msgCp)
CmAsnMsgCp  *msgCp;   /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   U16           numBytes;       /* number of tag bytes copied */
   MsgLen       i;              /* message index */

   /* temporary array for encoding */
   Data         pkArray[OCTET_BUFSIZE];

   TRC2(cmEncBool)
 
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr = (TknU8 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* copy the length of the octet and increment the array index to point
      to the next byte */

   pkArray[i++] = (Data) SIZEOF_OCTET;

   /* copy from the event structure the boolean value */
   if (evntStr->val == TRUE)
   {
      pkArray[i++] = CM_ASN_TRUE;
   }
   else
   {
      pkArray[i++] = CM_ASN_FALSE;
   }

#ifdef CM_ASN_NO_INDEF_LEN

   /* Increment the current encoded length by the total octets required to encode
      this primitive type */
   msgCp->crntEncLen += i;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* encode the octet  in the message buffer */ 
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, (U16) i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmEncBool */


/*
*
*       Fun:    cmDecBool 
*
*       Desc:   This function decodes an octet and enumerated types.
*
*       Ret:    ROK 
*
*       Notes:  None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecBool 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecBool (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{

   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   /* cm_asn_c_001.main_19 - modification - data type of tmpLen changed from U16 
    * to MsgLen.
    */
   MsgLen       tmpLen;         /* message length */
   Buffer       *mBuf;          /* message buffer */

   /* temporary array for encoding */
   Data         pkArray[OCTET_BUFSIZE];

   TRC2(cmDecBool)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, (S16 *)&tmpLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &tmpLen);
#endif

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

   if (tmpLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   mBuf     = msgCp->mBuf;             /* get the message buffer */

   /* check if this is a duplicate element */
   if (evntStr->pres == TRUE)
   {
      /* duplicate element */
      CM_ASN_ERR(msgCp, CM_ASN_DUP_ELMNT, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* element cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }
   }

   /* read the element value into the event structure, remove the element
      from the message buffer (including tag and length) */

   if ((ret = cmRemPrim(mBuf, msgCp, (U8 *)(&(evntStr->val)), &tmpLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(ret);
   }

   if (evntStr->val == CM_ASN_FALSE)
   {
      evntStr->val = FALSE;
   }
   else
   {
      evntStr->val = TRUE;
   }

   /* no errors, make the element present in the event structure */
   evntStr->pres = TRUE;

   cmSkipElmnt(msgCp);

   RETVALUE(ROK);

} /* cmDecBool */

/* ------------------------------------------------------------- */


/*
*
*       Fun:   cmEncTagType
*
*       Desc:  This function encodes the sequence or a set of tokens 
*
*       Ret:   ROK (encoding successful) 
*              ROUTRES (out of resources)
*              RFAILED (general failure)
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmEncTagType 
(
CmAsnMsgCp   *msgCp     /* message control pointer */
)
#else
PUBLIC S16 cmEncTagType(msgCp)
CmAsnMsgCp   *msgCp;    /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   U8           flag;           /* protocol flag */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   U16          numBytes;       /* number of bytes copied */
   U16          i;              /* counter */

#ifdef CM_ASN_NO_INDEF_LEN
   U32          tmpLen;
   U32          subEncLen;      /* Adjusted encoded length */
#endif /* CM_ASN_NO_INDEF_LEN */

   /* temporary array for encoding */
   Data         pkArray[SEQ_BUFSIZE];

   TRC2(cmEncTagType)

   /* check for the version */
   elmntDef = *msgCp->elmntDef;        /* get the element defintion */ 
   evntStr  = (TknU8 *)msgCp->evntStr;  /* get the event structure */
     
   /* get the protocol flags from the database element defintion */
   flag = FLAG_TO_TYPE(elmntDef->flagp, msgCp->proType);

   /* check the element for protocol type and mandatory/optional flags */
   if ((ret = cmChkEncElmnt(flag, msgCp, evntStr)) != ROK)
   {
      if (ret == RSKIP)
      {
         /* element can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

      RETVALUE(RFAILED);
   }

   /* encode the tag into the array */
   (Void) cmEncTag(elmntDef, pkArray, &numBytes);

   /* point the array index to the byte after the tag */
   i = numBytes;
 
   /* encode the length. EOC format */
   pkArray[i++] = EOC_LEN;
    
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)i, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, i, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the curent length to adjust the constructor length later */
   subEncLen = msgCp->crntEncLen;

#endif /* CM_ASN_NO_INDEF_LEN */

   /* Increment the database pointer over tag type start defintion */
   msgCp->elmntDef++;

   /* get the pointer to the next element definition */
   elmntDef = *msgCp->elmntDef;

   /* encode the element depending on the element type */
   if ((ret = cmEncElmnt(msgCp)) != ROK)
   {
      /* the error code would already be set, do not do it again */
        RETVALUE(ret);
   }

#ifdef CM_ASN_NO_INDEF_LEN

   /* Store the length of the constructor type */
   tmpLen = msgCp->crntEncLen - subEncLen;
 
   /* Increment the current encoded length by the number of octets required
      to encode tag and length field */
   msgCp->crntEncLen += numBytes;
   msgCp->crntEncLen += cmFindNumOctets(tmpLen);

#endif /* CM_ASN_NO_INDEF_LEN */

   /* Increment the database pointer over the tag type end element */
   msgCp->elmntDef++;

   /* Encode the EOC tag to indicate the end of tag type */
   for (i = 0; i < EOC_TAG_LEN; i++)
   {
      pkArray[i] = EOC_TAG;
   }

   /* Add the EOC tag at the end of the mBuf */
#ifdef CM_ASN_SS
   ret = SAddPstMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, msgCp->mBuf);
#else
   CM_ASN_ADD_PST_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(ret);
   }

   RETVALUE(ROK);

} /* cmEncTagType */


/*
*
*       Fun:   cmDecTagType 
*
*       Desc:  This function decodes a tag type element 
*
*       Ret:   ROK 
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecTagType 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PUBLIC S16 cmDecTagType (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   S16          ret;            /* return value */
   U8           tagSize;        /* size of the tag */
   TknU8        *evntStr;       /* pointer to event structure */
   Bool         skipElmnt;      /* need to skip the element */
   MsgLen       lfLen;          /* Length of length field */
   MsgLen       seqLen;         /* length of the sequence */
   MsgLen       prevLen;        /* msg length before decoding element */
   Bool         seqEoc;         /* eoc flag of the sequence */
   Buffer       *mBuf;          /* message buffer */
   
   /* temporary array for encoding */
   Data         pkArray[STR_BUFSIZE + EOC_TAG_LEN];

   TRC2(cmDecTagType)

   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }

   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &prevLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &prevLen);
#endif

   if (prevLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */
   
   skipElmnt = FALSE;                  /* do not skip the element */

   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */
   seqEoc   = FALSE;                   /* not a EOC encoding */

   /* get and check the tag, do not strip the tag bytes */
   if ((ret = cmGetTag(msgCp, pkArray, 0, &tagSize)) == RFAILED)
   {
      CM_ASN_ERR(msgCp, CM_ASN_TAG_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* check the tag against the database defintion of the element */
   if ((ret = cmChkTag(msgCp, pkArray, tagSize, &skipElmnt)) == RFAILED)
   {
      /* tag in error, check if the element can be skipped (optional) */
      if (skipElmnt == FALSE)
      {
         /* sequence of cannot be skipped (mandatory), return error */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* sequence is optional, skip it */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */

   /* now get the length of the tag type */
   if ((ret = cmFindLen(msgCp, tagSize, &seqLen, &seqEoc, &lfLen)) != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, &elmntDef->tag);  
      RETVALUE(RFAILED);
   }

   if (seqLen > prevLen)
   {
      CM_ASN_ERR(msgCp, CM_ASN_LEN_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* strip of the tag and length bytes */
#ifdef CM_ASN_SS
   ret = SRemPreMsgMult(pkArray, (MsgLen)(tagSize + lfLen), mBuf);
#else
   CM_ASN_REM_PRE_MULT(pkArray, (tagSize + lfLen), msgCp, &ret);
#endif

   if (ret != ROK)
   {
      CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
      RETVALUE(RFAILED);
   }

   /* increment the data base pointer over the start of tag type element */
   msgCp->elmntDef++;

   /* decode the element depending on the element type */
   if ((ret = cmDecElmnt(msgCp)) != ROK)
   {
      /* do not fill error as already filled in */ 
      RETVALUE(RFAILED);
   }

   if (seqEoc == TRUE)
   {
      /* remove the two EOC bytes */
#ifdef CM_ASN_SS
      ret = SRemPreMsgMult(pkArray, (MsgLen)EOC_TAG_LEN, mBuf);
#else
      CM_ASN_REM_PRE_MULT(pkArray, EOC_TAG_LEN, msgCp, &ret);
#endif
      if (ret != ROK)
      {
         CM_ASN_ERR(msgCp, CM_ASN_RES_ERR, NULLP);  
         RETVALUE(ret);
      }
   }

   /* Increment the database pointer over end of tag type element */
   msgCp->elmntDef++;

   RETVALUE(ROK);

} /* cmDecTagType */

/* ------------------------------------------------------------- */
/*
*
*       Fun:   cmDecAnyDefinedBy
*
*       Desc:  This function decodes a ANY DEFINED BY type.
*              This routine requires the caller to identify
*              the element to be decoded in form of a
*              element index. The encoding of the ANY DEFINED
*              BY type is the same as a choice type and is
*              handled in the routine cmEncChoice.
*
*       Ret:   ROK 
*              RFAILED
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmDecAnyDefinedBy
(
CmAsnMsgCp   *msgCp,  /* message control pointer */
U8           idx      /* element index */
)
#else
PUBLIC S16 cmDecAnyDefinedBy (msgCp, idx)
CmAsnMsgCp   *msgCp;  /* message control pointer */
U8           idx;     /* element index */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   CmElmntDef   **defPtr;       /* element definition */
   S16          ret;            /* return value */
   TknU8        *evntStr;       /* pointer to event structure */
   TknU8        *strPtr;        /* pointer to event structure */
   TknU8        *tmpPtr;        /* temporary pointer to event structure */
   MsgLen       msgLen;         /* Length of message */
   Buffer       *mBuf;          /* message buffer */
   U16          j;              /* element counter */

   TRC2(cmDecAnyDefinedBy)
 
   /* check if this element is defined for this protocol */
   if ((ret = cmDecChkFlag(msgCp)) == RFAILED)
   {
      /* this element is not defined for this protocol, skip it */
      cmSkipElmnt(msgCp);
      RETVALUE(ROK);
   }
 
   /* get the element defintion */
   elmntDef = *msgCp->elmntDef;

#if CM_ASN_SS
   /* check the message length */
   (Void) SFndLenMsg(msgCp->mBuf, &msgLen);
#else
   /* get the total length of the message */
   CM_ASN_FND_LEN(msgCp, &msgLen);
#endif

   if (msgLen == 0)
   {
      /* No bytes in the message buffer, check if element is mandatory */
      if (cmChkEleMandMis(msgCp) == TRUE)
      {
         /* element is mandatory, cannot be skipped */
         CM_ASN_ERR(msgCp, CM_ASN_MAND_MIS, &elmntDef->tag);  
         RETVALUE(RFAILED);
      }
      else
      {
         /* element is not mandatory, can be skipped */
         cmSkipElmnt(msgCp);
         RETVALUE(ROK);
      }

   } /* end if */

   /* store the choice element present */
   j = (idx - 1);
 
   defPtr    = msgCp->elmntDef;        /* store element definition pointer */
   strPtr    = msgCp->evntStr;         /* store event structure pointer */
 
   evntStr  = (TknU8 *)msgCp->evntStr; /* get the event structure */
   elmntDef = *msgCp->elmntDef;        /* get the element definition */
   mBuf     = msgCp->mBuf;             /* get the message buffer */
 
   /* increment over the start of choice */
   msgCp->elmntDef++;
 
   /* skip over the present token */
   cmIncPtr((PTR *)&msgCp->evntStr, sizeof(TknU8));
 
   /* skip over the database defintions of the first "(idx - 1)" elements */
   /* which are not present */

   for(; j > 0; j--)
   {
        /* store the event structure pointer */
        tmpPtr = msgCp->evntStr;
 
        cmSkipElmnt(msgCp);
 
        /* restore the event structure pointer */
        msgCp->evntStr = tmpPtr;
   }
 
   /* make the element as present in the event structure */
   strPtr->pres = TRUE;                          /* choice present */
   strPtr->val  = idx;                           /* element index */
 
   /* decode the element depending on the element type */
   if ((ret = cmDecElmnt(msgCp)) != ROK)
   {
      /* error code already filled, don't do it again */
      RETVALUE(RFAILED);
   }

   /* restore the element defintion to choice start */
   msgCp->elmntDef = defPtr;
 
   /* restore the event structure */
   msgCp->evntStr = strPtr;
 
   /* now skip over the choice */
   cmSkipElmnt(msgCp);
 
   RETVALUE(ROK);

} /* cmDecAnyDefinedBy */


/* ------------------------------------------------------------- */

/*
*
*       Fun:   cmInitMsgDb 
*
*       Desc:  This function initializes the message database 
*              elements with information about the sizes of
*              the database elements for constructor types.
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PUBLIC S16 cmInitMsgDb 
(
CmElmntDef **msgDef               /* message definition */
)
#else
PUBLIC S16 cmInitMsgDb (msgDef)
CmElmntDef **msgDef;              /* message definition */
#endif
{
   CmElmntDef **dbPtr;            /* database pointer */
   CmElmntDef *elmnt;             /* database element pointer */
   S16        ret;                /* return value */

   TRC2(cmInitMsgDb)

   /* initialize the database pointer to the message start */
   dbPtr = msgDef;

   while (*dbPtr != NULLP)
   {
      /* get the current element from the database pointer */
      elmnt = *dbPtr;

      switch (elmnt->type)
      {
         case TET_U8:
         case TET_ENUM:
         case TET_NULL:
         case TET_INT:
         case TET_INT_RNG:
         case TET_STR4:
         case TET_STR12:
         case TET_STR32:
         case TET_STR64:
         case TET_STR256:
         case TET_BITSTR:
         case TET_STRXL:
         case TET_STRXL_MEM:
         case TET_OID:
         case TET_BOOL:
         case TET_ESC_PRIM:
         case TET_IA5STR4:
         case TET_IA5STR12:
         case TET_IA5STR32:
         case TET_IA5STR64:
         case TET_IA5STR256:
         case TET_IA5STRXL:
         case TET_IA5STRXL_MEM:
            dbPtr++;
            break;

         case TET_SEQ:
         case TET_SET:
         case TET_SEQ_OF:
         case TET_SET_OF:
         case TET_CHOICE:
         case TET_TAG:
         case TET_ESC_CONST:
         case TET_UNCONS_SEQ_OF:
         case TET_UNCONS_SET_OF:

            if ((ret = cmInitConstType(&dbPtr)) != ROK)
            {
               RETVALUE(RFAILED);
            }
            break;

         default:
            RETVALUE(RFAILED);

      } /* end switch */

   } /* end while */

   RETVALUE(ROK);

} /* cmInitMsgDb */



/*
*
*       Fun:   cmInitConstType 
*
*       Desc:  This function parses the constructor types and
*              updates there database defintions
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmInitConstType 
(
CmElmntDef ***ptr               /* message definition */
)
#else
PRIVATE S16 cmInitConstType (ptr)
CmElmntDef ***ptr;              /* message definition */
#endif
{
   CmElmntDef **dbPtr;          /* database pointer */
   CmElmntDef **savPtr;         /* saved database pointer */
   CmElmntDef *elmnt;           /* element definition */
   S16        ret;              /* return value */
   
   TRC2(cmInitConstType)

   /* initialize the local database pointer */
   dbPtr  = *ptr;
   savPtr = *ptr;

   /* increment the database pointer over the start of the constructor type */
   dbPtr++;
   *ptr = dbPtr;

   /* get the current element defintion */
   elmnt = *dbPtr;

   while (elmnt->type != TET_SETSEQ_TERM) 
   {
      switch (elmnt->type)
      {
         case TET_U8:
         case TET_ENUM:
         case TET_NULL:
         case TET_INT:
         case TET_INT_RNG:
         case TET_STR4:
         case TET_STR12:
         case TET_STR32:
         case TET_STR64:
         case TET_STR256:
         case TET_BITSTR:
         case TET_STRXL:
         case TET_STRXL_MEM:
         case TET_OID:
         case TET_BOOL:
         case TET_ESC_PRIM:
         case TET_IA5STR4:
         case TET_IA5STR12:
         case TET_IA5STR32:
         case TET_IA5STR64:
         case TET_IA5STR256:
         case TET_IA5STRXL:
         case TET_IA5STRXL_MEM:

            /* increment the local database pointer */
            dbPtr++;

            /* update the passed pointer */
            *ptr = dbPtr;

            break;

         case TET_SEQ:
         case TET_SET:
         case TET_SEQ_OF:
         case TET_SET_OF:
         case TET_CHOICE:
         case TET_TAG:
         case TET_ESC_CONST:
         case TET_UNCONS_SEQ_OF:
         case TET_UNCONS_SET_OF:

            if ((ret = cmInitConstType(ptr)) != ROK)
            {
               RETVALUE(RFAILED);
            }

            /* get the updated database pointer */
            dbPtr = *ptr;

            break;

         default:
            RETVALUE(RFAILED);

      } /* end switch */

      /* get the next element defintion */
      elmnt = *dbPtr;
   
   } /* end while */

   /* skip over the end of constructor type */
   dbPtr++;

   /* update the caller database pointer */
   *ptr = dbPtr;

   /* get the saved database pointer */
   elmnt = *savPtr;

   /* update the database defintion for this constructor type */
   /* cm_asn_c_001.main_19 - modification - typecasting changed from U16 to MsgLen
    */
   elmnt->dbSize = (MsgLen) ((PTR)dbPtr - (PTR)savPtr);

   RETVALUE(ROK);

} /* cmInitConstType */


#ifdef CM_ASN_NO_INDEF_LEN
/*
*
*       Fun:   cmFindNumOctets 
*
*       Desc:  This function finds the number of octets required to encode a
*              length.
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE U8 cmFindNumOctets 
(
U32   value     /* Integer value */
)
#else
PRIVATE U8 cmFindNumOctets (value)
U32   value;    /* Integer value */
#endif
{
   U8   numBytes;
   U32  tmpVal;
 
   TRC2(cmFindNumOctets)
 
   if (value <= MAX_SBYTE_VAL)
   {
      numBytes = 1;
   }
   else
   {
      tmpVal   = value;
      numBytes = 1;      /* First field is number of octets in the length field */
 
      while(tmpVal != 0)
      {
         tmpVal = (tmpVal >> NUMBITS_BYTE);
         numBytes++;
      }
   }
 
   RETVALUE(numBytes);
} /* End of cmFindNumOctets */
 

/*
*
*       Fun:   cmRmvIndefLenMsg 
*
*       Desc:  This function removes the indefinite length encoding from the
*              encoded message and replace that with short/long form of length
*              encoding.
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmRmvIndefLenMsg 
(
CmAsnMsgCp   *msgCp   /* message control pointer */
)
#else
PRIVATE S16 cmRmvIndefLenMsg (msgCp)
CmAsnMsgCp   *msgCp;  /* message control pointer */
#endif
{
   U32  length;

   /* Use proper 'ret' value in case of failure. */
   S16  ret = ROK;

   TRC2(cmRmvIndefLenMsg)
 
   /* Restore the start pointers for the  event structure and the element
      definition */
   msgCp->evntStr  = msgCp->strtEvPtr;
   msgCp->elmntDef = msgCp->strtElmntPtr;
 
   /* Go thru each element definition in the database */
   while (*msgCp->elmntDef != (CmElmntDef *)NULLP)
   {
      if ((ret = cmRmvIndefLenElmnt(msgCp, FALSE, &length)) != ROK)
      {
         RETVALUE(ret);
      }

   } /* end while */
 
   RETVALUE(ROK);
} /* End of cmRmvIndefLenMsg */
 

/*
*
*       Fun:   cmRmvIndefLenElmnt 
*
*       Desc:  This function removes the indefinite length encoding from the
*              encoded element and replace that with short/long form of length
*              encoding.
*
*       Ret:   ROK 
*
*       Notes: None 
*
*       File:  cm_asn.c
*
*/
  
#ifdef ANSI
PRIVATE S16 cmRmvIndefLenElmnt 
(
CmAsnMsgCp   *msgCp,    /* message control pointer */
Bool          tagType,  /* Flag for tagType */
U32          *length    /* Length */
)
#else
PRIVATE S16 cmRmvIndefLenElmnt (msgCp, tagType, length)
CmAsnMsgCp   *msgCp;    /* message control pointer */
Bool          tagType;  /* Flag for tagType */
U32          *length;   /* Length */
#endif
{
   CmElmntDef   *elmntDef;      /* element definition */
   TknU8        *evntStr;       /* pointer to event structure */
   CmElmntDef  **tmpElmntPtr;   /* Temporary element definition pointer */
   TknU8        *tmpEvPtr;      /* Temporary event structure pointer */
   U16           numBytes;      /* number of tag bytes copied */
   MsgLen        len;           /* Length */
   MsgLen        lfLen;         /* Length field length */
   Bool          eocFlg;        /* Indefinite form of length indicator */
   Data          pkArray[STR_BUFSIZE]; /* Temporary pack array */
   U8            elmntIdx;
   U8            tagSize;
   U16           i;
   U8            tmpData1;
   U8            tmpData2;
   U16           cntr;
   S16           ret;
   U8            pres;
 
   TRC2(cmRmvIndefLenElmnt)

   ret = ROK;
   elmntDef = *msgCp->elmntDef;         /* get the element defintion */ 
   evntStr  = (TknU8 *)msgCp->evntStr;  /* get the event structure */
   pres = FALSE;
     
   /* Start encoding the tokens */
   switch(elmntDef->type)
   {
      case TET_U8:
      case TET_ENUM:
      case TET_NULL:
      case TET_STR4:
      case TET_STR12:
      case TET_STR32:
      case TET_STR64:
      case TET_STR256:
      case TET_INT:
      case TET_INT_RNG:
      case TET_OID:
      case TET_BOOL:
      case TET_BITSTR:
      case TET_IA5STR4:
      case TET_IA5STR12:
      case TET_IA5STR32:
      case TET_IA5STR64:
      case TET_IA5STR256:
      case TET_STRXL:
      case TET_STRXL_MEM:
      case TET_IA5STRXL:
      case TET_IA5STRXL_MEM:
         /* Token structure used is TknStrXL */
         if (elmntDef->type == TET_STRXL || 
             elmntDef->type == TET_STRXL_MEM ||
             elmntDef->type == TET_IA5STRXL || 
             elmntDef->type == TET_IA5STRXL_MEM)
            pres = ((TknStrXL *)evntStr)->pres;
         else
            pres = evntStr->pres;

         /* Element was encoded only if it was present */
         if (pres)
         {
            /* Get the length of the primitive type */
            cmGetLen(msgCp, (MsgLen)(msgCp->sBuf.ctIdx + elmntDef->tag.len),
                     &len, &lfLen, &eocFlg);
 
            /* If this primitive is part of TAG type then add the length for
               the tag type in the message buffer */
            if (tagType)
            {
               *length = elmntDef->tag.len + lfLen + len;
               break;
            }
 
            /* Copy the enitre encoded primitive type into the message buffer */
            SAddPstMsgMult((Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +
                                    msgCp->sBuf.ctIdx),
                           (MsgLen)(elmntDef->tag.len + lfLen + len),
                           msgCp->mBuf);
 
            /* Increment in the static string */
            msgCp->sBuf.ctIdx  += elmntDef->tag.len + lfLen + len;
         }
         cmSkipElmnt(msgCp);
         break;
 
      case TET_TAG:
 
         /* Element was encoded only if it was present */
         if (evntStr->pres)
         {
            if (tagType)
            {
               /* Skip over the tag and EOC length indicator */
               msgCp->sBuf.ctIdx  += elmntDef->tag.len + 1;
 
               /* Increment the database pointer over tag type defintion */
               msgCp->elmntDef++;
 
               /* Process the element and put the length of the tag type also */
               if ((ret = cmRmvIndefLenElmnt(msgCp, TRUE, length)) != ROK)
               {
                  RETVALUE(ret);
               }

               /* Skip over the tag and EOC length indicator */
               msgCp->sBuf.ctIdx  -= elmntDef->tag.len + 1;
 
               /* Go Back to the previous database definition of tag type */
               msgCp->elmntDef--;
 
               /* Add the tag length and length field length */
               *length += elmntDef->tag.len +              /* Tag length */
                         cmFindNumOctets(evntStr->spare1); /* Length field length */
            }
            else
            {
               /* First copy just the tag of the tag type in the message buffer */
               SAddPstMsgMult((Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +
                                       msgCp->sBuf.ctIdx),
                              elmntDef->tag.len,
                              msgCp->mBuf);
 
               /* Skip over the tag and EOC length indicator */
               msgCp->sBuf.ctIdx  += elmntDef->tag.len + 1;
 
               /* Increment the database pointer over tag type defintion */
               msgCp->elmntDef++;
 
               /* Process the element and put the length of the tag type also */
               if ((ret = cmRmvIndefLenElmnt(msgCp, TRUE, length)) != ROK)
               {
                  RETVALUE(ret);
               }
 
 
               /* Go Back to the previous database definition of tag type */
               msgCp->elmntDef--;
 
               /* Encode the length of the entire constructor type */
               cmEncLen(*msgCp->elmntDef, *length, pkArray, &numBytes);
 
               /* Copy the encoded length in the message buffer */
               SAddPstMsgMult((Data *)pkArray, numBytes, msgCp->mBuf);

               /* Increment the database pointer over tag type defintion */
               msgCp->elmntDef++;
 
               /* Process the element and put the length of the tag type also */
               if ((ret = cmRmvIndefLenElmnt(msgCp, FALSE, length)) != ROK)
               {
                  RETVALUE(ret);
               }
 
               /* Skip over the two EOC bytes from the end of a encoded constructor */
               msgCp->sBuf.ctIdx  += 2;
 
               /* Increment the database pointer over seq/set term defintion */
               msgCp->elmntDef++;
            }
         }
         else
         {
            /* Skip over the entire constructor type */
            cmSkipElmnt(msgCp);
         }
         break;
 
      case TET_SEQ:
      case TET_SET:
 
         /* Element was encoded only if it was present */
         if (evntStr->pres)
         {
            /* If this constructor is part of TAG type then add the length for
               the tag type in the message buffer */
            if (tagType)
            {
               *length = elmntDef->tag.len +               /* Tag length */
                         cmFindNumOctets(evntStr->spare1) +/* Length field length */
                         evntStr->spare1;                  /* Length of the constructor type */
               break;
            }
 
            /* First copy just the tag in the message buffer */
            SAddPstMsgMult((Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +
                                    msgCp->sBuf.ctIdx),
                           elmntDef->tag.len,
                           msgCp->mBuf);
 
            /* Skip over the tag and EOC length indicator */
            msgCp->sBuf.ctIdx  += elmntDef->tag.len + 1;
 
            /* Encode the length of the constructor type */
            cmEncLen(elmntDef, (U32)evntStr->spare1, pkArray, &numBytes);
 
            /* Copy the encoded length in the message buffer */
            SAddPstMsgMult((Data *)pkArray, numBytes, msgCp->mBuf);
 
            /* Increment the database pointer over seq/set start defintion */
            msgCp->elmntDef++;

            /* increment the event structure over present token */
            CM_ASN_SKIP_TKNPRES(msgCp);
 
            /* process all elements till end of sequence element is encountered */
            while (elmntDef->type != TET_SETSEQ_TERM)
            {
               if ((ret = cmRmvIndefLenElmnt(msgCp, FALSE, length)) != ROK)
               {
                  RETVALUE(ret);
               }
 
               /* get the updated element defintion */
               elmntDef = *msgCp->elmntDef;

            } /* end while */
 
            /* Skip over the two EOC bytes from the end of a encoded constructor */
            msgCp->sBuf.ctIdx  += 2;
 
            /* Increment the database pointer over seq/set term defintion */
            msgCp->elmntDef++;
         }
         else
         {
            /* Skip over the entire constructor type */
            cmSkipElmnt(msgCp);
         }
         break;
 
      case TET_SEQ_OF:
      case TET_SET_OF:
 
         /* Element was encoded only if it was present */
         if (evntStr->pres)
         {
            /* If this constructor is part of TAG type then add the length for
               the tag type in the message buffer */
            if (tagType)
            {
               *length = elmntDef->tag.len +               /* Tag length */
                         cmFindNumOctets(evntStr->spare1) +/* Length field length */
                         evntStr->spare1;                  /* Length of the constructor type */
               break;
            }
 
            /* First copy just the tag in the message buffer */
            SAddPstMsgMult((Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +
                                    msgCp->sBuf.ctIdx),
                           elmntDef->tag.len,
                           msgCp->mBuf);
 
            /* Skip over the tag and EOC length indicator */
            msgCp->sBuf.ctIdx  += elmntDef->tag.len + 1;
 
            /* Encode the length of the constructor type */
            cmEncLen(elmntDef, (U32)evntStr->spare1, pkArray, &numBytes);
 
            /* Copy the encoded length in the message buffer */
            SAddPstMsgMult((Data *)pkArray, numBytes, msgCp->mBuf);
 
            cntr = elmntDef->repCntr;
 
            /* Increment the database pointer over seq/set start defintion */
            msgCp->elmntDef++;

            /* increment the event structure over present token */
            CM_ASN_SKIP_TKNPRES(msgCp);
 
            tmpElmntPtr = msgCp->elmntDef;

            /* process all elements till end of sequence element is encountered */
            while (cntr > 0)
            {
               if ((ret = cmRmvIndefLenElmnt(msgCp, FALSE, length)) != ROK)
               {
                  RETVALUE(ret);
               }

               cntr--;
               
               CM_ASN_EXAM_MSG(&tmpData1, msgCp, msgCp->sBuf.ctIdx, &ret);
               CM_ASN_EXAM_MSG(&tmpData2, msgCp, msgCp->sBuf.ctIdx + 1, &ret);

               if ((tmpData1 == 0x0) && (tmpData2 == 0x0))
               {
                  while (cntr > 0)
                  {
                      msgCp->elmntDef = tmpElmntPtr;
                      cmSkipElmnt(msgCp);
                      cntr--;
                  }
                  break;
               }

               msgCp->elmntDef = tmpElmntPtr;

            } /* end while */
 
            /* Skip over the two EOC bytes from the end of a encoded constructor */
            msgCp->sBuf.ctIdx  += 2;
 
            /* Increment the database pointer over seq/set term defintion */
            msgCp->elmntDef++;
         }
         else
         {
            /* Skip over the entire constructor type */
            cmSkipElmnt(msgCp);
         }
         break;

      case TET_UNCONS_SEQ_OF:
      case TET_UNCONS_SET_OF:
 
         /* Element was encoded only if it was present */
         if (evntStr->pres)
         {
            TknU8        *startStr;      /* ptr to start of event structure */
            TknU8        **evntPtrs;     /* pointer to list of event structure 
                                          * pointers in SET/SEQ OF */
            U16          ind;          /* index into above array of ptrs */
            TknU8        *tempStr;       /* temp str points to correct 
                                          * position in the event structure */


            /* get the event structure */
            startStr = (TknU8 *)msgCp->evntStr;  
            tempStr = (TknU8 *)msgCp->evntStr;

            /* set the pointer to array of pointers in SET OF/SEQ OF 
             * event structure */
            evntPtrs = (TknU8 **)((PTR)startStr + sizeof(TknU8));

            ind = 0;

            /* If this constructor is part of TAG type then add the length for
               the tag type in the message buffer */
            if (tagType)
            {
               *length = elmntDef->tag.len +               /* Tag length */
                         cmFindNumOctets(evntStr->spare1) +/* Len field len */
                         evntStr->spare1;        /* Len of constructor type */
               break;
            }
 
            /* First copy just the tag in the message buffer */
            SAddPstMsgMult((Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +
                                    msgCp->sBuf.ctIdx),
                           elmntDef->tag.len,
                           msgCp->mBuf);
 
            /* Skip over the tag and EOC length indicator */
            msgCp->sBuf.ctIdx  += elmntDef->tag.len + 1;
 
            /* Encode the length of the constructor type */
            cmEncLen(elmntDef, (U32)evntStr->spare1, pkArray, &numBytes);
 
            /* Copy the encoded length in the message buffer */
            SAddPstMsgMult((Data *)pkArray, numBytes, msgCp->mBuf);
 
            cntr = elmntDef->repCntr;
 
            /* Increment the database pointer over seq/set start defintion */
            msgCp->elmntDef++;

            /* increment the event structure over present token */
            CM_ASN_SKIP_TKNPRES(msgCp);
            tempStr = (TknU8 *)msgCp->evntStr;
 
            tmpElmntPtr = msgCp->elmntDef;

            /* process all elements till end of sequence element is encountered */
            while (cntr > 0)
            {
               /* set correct event structure pointer to msgCp->evntStr */
               msgCp->evntStr = evntPtrs[ind++];

               /* Update to point correct location in the seq of event
                * structure */
               tempStr++;

               if ((ret = cmRmvIndefLenElmnt(msgCp, FALSE, length)) != ROK)
               {
                  RETVALUE(ret);
               }

               cntr--;
               
               CM_ASN_EXAM_MSG(&tmpData1, msgCp, msgCp->sBuf.ctIdx, &ret);
               CM_ASN_EXAM_MSG(&tmpData2, msgCp, msgCp->sBuf.ctIdx + 1, &ret);

               if ((tmpData1 == 0x0) && (tmpData2 == 0x0))
               {
                  while (cntr > 0)
                  {

                     msgCp->elmntDef = tmpElmntPtr;

                     cmSkipElmnt(msgCp);

                     /* update ptr to point correct location in the seq of
                      * event structure */
                     tempStr++;

                     /* SkipElmnt won't update the evntStr properly */
                     msgCp->evntStr = tempStr;
                     cntr--;
                  }
                  break;
               }

               msgCp->elmntDef = tmpElmntPtr;

            } /* end while */

            /* Set the correct event structure pointer */
            msgCp->evntStr = tempStr;
 
            /* Skip over the two EOC bytes from the end of a encoded 
             * constructor */
            msgCp->sBuf.ctIdx  += 2;
 
            /* Increment the database pointer over seq/set term defintion */
            msgCp->elmntDef++;
         }
         else
         {
            /* Skip over the entire constructor type */
            cmSkipElmnt(msgCp);
         }
         break;
 
      case TET_CHOICE:
 
         /* Save the pointers for further use */
         tmpEvPtr    = msgCp->evntStr;
         tmpElmntPtr = msgCp->elmntDef;
 
         /* Index of the element in the choice */
         elmntIdx = evntStr->val;
 
         /* Increment the database pointer over choice start defintion */
         msgCp->elmntDef++;
 
         /* increment the event structure over present token */
         CM_ASN_SKIP_TKNPRES(msgCp);
 
         evntStr = msgCp->evntStr;
 
         /* Skip the elements in the choice which are not encoded */
         for (i = 1; i < elmntIdx; i++)
         {
            cmSkipElmnt(msgCp);
            msgCp->evntStr = evntStr;
         }
 
         /* Process the element */
         if ((ret = cmRmvIndefLenElmnt(msgCp, tagType, length)) != ROK)
         {
            RETVALUE(ret);
         }
 
         /* Restore the pointers and then skip over the entire choice */
         msgCp->elmntDef = tmpElmntPtr;
         msgCp->evntStr  = tmpEvPtr;
 
         if (!tagType)
         {
            cmSkipElmnt(msgCp);
         }
         break;
 
      case TET_ESC_PRIM:
      case TET_ESC_CONST:

         if (evntStr->pres)
         {
            if (msgCp->usrActReq)
            {
               /* execute the user function */
               ret = (*elmntDef->func)(msgCp);
 
               /* Reset the user action required flag */
               msgCp->usrActReq = FALSE;
               if (ret != ROK)
               {
                   RETVALUE(ret);
               }

            }
            else
            {
               /* Get the length of the tag first, for ESC type elements we might
                  not have the length of the tag in the element definition */
               cmGetTag(msgCp, pkArray, msgCp->sBuf.ctIdx, &tagSize);
 
               /* Get the length of the primitive type */
               cmGetLen(msgCp, (MsgLen)(msgCp->sBuf.ctIdx + tagSize),
                        &len, &lfLen, &eocFlg);
 
               if (tagType)
               {
                  *length = tagSize + lfLen + len + (eocFlg * 2);
                  break;
               }

               /* Copy the enitre encoded primitive type into the message buffer */
               SAddPstMsgMult((Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +
                                       msgCp->sBuf.ctIdx),
                              (MsgLen)(tagSize + lfLen + len), msgCp->mBuf);
 
               /* Increment in the static string */
               msgCp->sBuf.ctIdx  += tagSize + lfLen + len;
            }
         }
         cmSkipElmnt(msgCp);
         break;
   } /* end switch */
 
   RETVALUE(ret);
} /* End of cmRmvIndefLenElmnt */
#endif /* CM_ASN_NO_INDEF_LEN */



/********************************************************************30**

         End of file:     cm_asn.c@@/main/nodal_main/1 - Thu Nov 21 09:39:58 2013

*********************************************************************31*/


/********************************************************************40**

       Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

          Revision history:

*********************************************************************61*/

/********************************************************************80**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.1            ---    sg   1. initial release.
 
*********************************************************************81*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.2          ---      nj   1. Added support for Extra Long String
                              Added support for Object Identifiers
                              Added support for Integer
                              Added asn.1 support for TCAP
 
1.3          ---      sg   1. Added additonal routines for database init
                              cmInitMsgDb and cmInitConstType
                              Added support for present tokens for
                              constructor types
1.4          ---      sg   1. Added some debugging support and functions
                              cmPrntElmntDb, cmDumpMsgDb, cmPrntSBuf.

                           2. cmEncBool, cmDecBool, cmEncTagType, cmDecTagType,
                              and dupport for AnyDefinedBy added.

                           3. INAP first Release.

1.5          ---      sg   1. Fixed a warning for sun compiler 
                           2. Fixed a non-critical error in bit string
                              decoding
                           3. Allow passing null pointer in the user buffer
                              parameter for decoding messages
                           4. Compiled with g++ compiler. Removed warnings.

             ---      nj   5. Changed to return a valid error code if a mandatory
                              element is missing in a sequence. (cmChkSeqMandMis)

1.6          ---      nj   1. Added support to encode a message using all
                              definite form (short/long) of length encoding.

/main/8      ---     jz    1. Added element flag checking in SET decoding 
                              function (cmDecSet()).

             ---     pk    2. Only the present elements in Set/Sequence Of
                              are encoded.
/main/9      ---     pk    1. Some checks on the basis of minLen/maxLen are 
                              skipped for TET_INT_RNG token.
                           2. Cases added for new token TET_INT_RNG.
                           3. New encoding/decoding routines added for token
                              TET_INT_RNG.
/main/10     ---     pk    1. Routine cmEncOctetStr() corrected for proper 
                              encoding of length of TknStrXL types.
                           2. Routine cmRmvIndefLenElmnt() corrected for proper 
                              decoding of TknStrXL.
                           3. Routine cmChkSetMandMis() corrected for proper
                              decoding of TknStrXL.
                           4. Routine cmEncIntRng() corrected for proper encoding
                              of integers with MSB as 1.
                           5. Routine cmDecIntRng() corrected for proper decoding
                              of integers with MSB as 1.
                           6. Routine cmDecChoice() corrected to decode nested
                              choice with no tags for second level of choice.
                           7. Avoiding use of uninitialised 'ret' in functions 
                              cmEncElmnt() and cmDecElmnt() in default case.
                           8. Deleted redundant cmGetTag() declaration.
/main/11     ---      pk   1. Error cause filled for failure case.
                           2. Element definition updated.

/main/12     ---     ck    1. New token types TET_STRXL_MEM, TET_IA5STRXXX, 
                              TET_UNCONS_SET_OF and TET_UNCONS_SEQ_OF were 
                              added. 
                           2. Corrected cmDecChoice to work for nested choice.
                           3. Corrected cmRmvIndefLenElmnt to work for TET..XL
                              token types.
                           4. Corrected cmChkSetMandMis to work correcly for 
                              default cases.
                           5. Corrected encoding/decoding of TET_INT_RNG type.
/main/13     ---      as    1. Decoding of CHOICE in a SET is corrected.
/main/13     ---      as    1. Decoding of CHOICE in a SET is corrected.
/main/14     ---      sg    1. Make some functions PUBLIC for use by 
                              protocol layers
                              2. cmChkTag corrected to validate tag 
                              for protocol variant
/main/14     ---      hr      Correct skipping of choice in a SET
             ---      hr      To fix wncoding errors for range
                              bound integers
             ---      hr    1) Fix in cmDecSetSeqOf();
             ---      hr    1) Fix in cmDecOid();
/main/15     ---      kk   1. Phase 4 related changes in 2.3 release
/main/16     ---      kk   1. debug printing added
/main/17     ---      mkm  1. Changes for SS_MULTIPLE_PROCS .
                           2. Changes when mBuf is NULLP or of zero length
/main/18     ---      mkm  1. Added check in cmGetlen() if length of element is
                              greater than total message length.
/main/19     ---      yk   1. compilation error removed.
/main/19+ cm_asn_c_001.main_19  mc   1. U16 data type for members of CmElmntDef and 
                              SBufInfo changed to MsgLen to take care
                              of length greater than U16.
                           2. Data type of strLen Argument of cmRemPrim 
                              changed from U16 to MsgLen.
                           3. Data type of local variables tmpLen changed
                              from U16 to MsgLen.
/main/22     ---      mc   1. Update for  INAP Release 1.3
/main/22+   cm_asn_c_001.main_22  mc  1. data type of tmpLen changed to
                                         MsgLen from U16 in cmDecIa5Str function
                                         to take care of length greater than U16.
/main/23     ---     mc    1. Updated for INAP 1.3 release.                                    
/main/23+ cm_asn_c_001.main_23  si  1. Initialise the CmAsnMsgCp msg. Uninitialized value 
					was causing a core dump in a wrong TC-CONTINUE.
					fix for ccpu00073999
/main/24  cm_asn_c_001.main_24  sv  1.Setting the error code.
/main/25  cm_asn_c_001.main_25  sv  1. Reject paramters with zero lengthfor type TET_U8.
                                    2. Send defined octets and ignore extra octets
                                       if ignore flag is set for parameters with length
                                       greater than maxLen.
/main/26  cm_asn_c_001.main_26  sv  1. Declared cmChkTag function as PUBLIC
                                    2. Corrected the compilation error for main_25 fix.
                                    3. skip the SetSeqOf if tag mismatches and return
                                       with RFAILED.
/main/28     ---      cm_asn_c_001.main_27  sv  1. Adding missing ueAction type.
/main/29     ---      cm_asn_c_001.main_28  djain  1. Remove the memory leak problem.
/main/30     ---      cm_asn_c_001.main_29  magnihotri  1.Removed redundant declaration.
*********************************************************************91*/
