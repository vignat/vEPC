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





/****************************************************************20**

     Name:     S1AP Layer 
  
     Type:     Message Function file
  
     Desc:     Message formatting/validation to be done  before 
               encoding, and after decoding. 

     File:     sz_msgfn.c

     Sid:      sq_msgfn.c@@/main/nodal_main/3 - Fri Nov 22 17:58:21 2013

     Prg:      rcs

*****************************************************************21*/




/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* common header 1 */
#include "cm_pasn.h"       /* common per asn.1 */
#include "cm_hash.h"       /* common header 1 */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common header 3 */
#include "cm_err.h"        /* common error */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "sct.h"        /* common error */
#include "szt_asn.h"        /* asn */
#include "szt.h"           /* upper interface */
#ifdef SZ_FTHA
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.h"           /* layer manager */
#include "sz.h"            /* s1ap */
#include "sz_err.h"        /* s1ap - error */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* header/extern include files (.x) */
  
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_mblk.x"       /* common header 1 */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_tkns.x"       /* common header 1 */
#include "cm_pasn.x"       /* common per asn.1 */
#include "cm_llist.x"      /* cm link list */
#include "cm_lib.x"        /* cm library */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "sct.x"           /* common structs 3 */
#include "szt_asn.x"       /* asn structure */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* upper interface */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* layer manager */
#include "sz.x"            /* s1ap */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef __cplusplus
}
#endif /* __cplusplus */

EXTERN CmPAsnElmDef *szMsgDb[];

#ifdef SZTV1
EXTERN CmPAsnElmDef *szMsgDbIe[][2];
#endif /* SZTV1 */

#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
EXTERN CmPAsnElmDef *szMsg3gDbIe[][2];
#endif

/* Array of strings to print the error cause */
S8 szErrPrntArr[SZ_MAX_ENC_DEC_ERR_CAUSE][SZ_ERR_DBG_SIZE]={
      "Transfer Syntax Error: Unexpected value(%d) \n",
      "Transfer Syntax Error: Resources error (%d)\n",
      "Transfer Syntax Error: Out of DB defined range (%d) \n",
      "Transfer Syntax Error: Extra parameter at the end of msg (%d)\n",
      "Transfer Syntax Error: Element index is bad for choice (%d)\n",
      "Transfer Syntax Error: Undefined parameter (%d)\n",
      "Transfer Syntax Error: Mandatory element missing (%d)\n",
      "Transfer Syntax Error: Database error (%d)\n",
      "Transfer Syntax Error: Default element missing (%d)\n",
      "Abstract Syntax Error: User escape function failed (%d)\n"};
 
/* Response for error handling - Unsuccessful msg or Error Indication */
U8 rspMsg[SZ_MSG_INDEX] = 
{
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_INIT_CXT_SETUP_FAIL,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_UE_CXT_MOD_FAIL,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_SETUP_FAIL,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG
#ifndef S1AP_REL8A0
,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG,
SZT_MSG_ID_NO_MSG
#endif /* S1AP_REL8A0 */
};



/*
 *  
 *   Fun:  szEncode 
 *   
 *   Desc:  To encode the message as per ASN.1 PER. 
 *   
 *   Ret:   ROK/RFAILED
 *   
 *   Notes: Invoked thorugh non-database module. 
 *   
 *   File:  sz_msgfn.c
 *   
*/
#ifdef ANSI
PUBLIC S16 szEncode
(
 Mem *mem,
 S1apPdu **s1apPdu,
 Buffer **mBuf
)
#else /* ANSI */
PUBLIC S16 szEncode (mem, s1apPdu, mBuf)
 Mem *mem;
 S1apPdu **s1apPdu; 
 Buffer  **mBuf;
#endif /* ANSI */
{
   U8 dbgFlag;
   S16  ret;
   CmPAsnErr asnErr;
   U32 len=0;
   
   TRC2(szEncode)

   dbgFlag = FALSE;

#ifdef DEBUGP
   if (szCb.init.dbgMask & LSZ_DBGMASK_ASN)
      dbgFlag = TRUE;
#endif  /* DEBUGP */

   if (*s1apPdu == NULLP)
   {
      RETVALUE(SZT_CAUSE_UNEXP_MSG);
   } 
       
   if ((SGetMsg(mem->region, mem->pool, mBuf)) != ROK)
   {
      SZLOGERROR_ADD_RES(ESZ110, 0, "Message Buffer Not allocated");

      SZ_FREE_SDU(*s1apPdu);

      RETVALUE(SZT_CAUSE_MEM_ALLOC_FAIL);
   }

   /* Initialize the PASN Error code */ 
   asnErr.errCode = 0;

   ret = cmPAsnEncMsg((TknU8 *)(&(*s1apPdu)->pdu), CM_PASN_USE_MBUF, 
                      (Void *)(*mBuf), SZ_PASN_VER_ZERO, 
                      (CmPAsnElmDef **)szMsgDb, 
                      (CmPAsnErr*)&asnErr,
                      &len, dbgFlag, FALSE );
  
   if (ret == RFAILED)
   {
      /* It's an ASN.1 Error */
      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf,
            " Encoding failed \n")); 

      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf, szErrPrntArr[asnErr.errCode-1],
            asnErr.errCode));

      SZ_FREE_SDU(*s1apPdu);

      SZ_FREE_BUF(*mBuf);

      if (asnErr.errCode == CM_PASN_ESC_FUNC_FLD)
         RETVALUE(SZT_CAUSE_PC_ABS_SYNTAX_ERR);
      else
         RETVALUE(SZT_CAUSE_PC_TX_SYNTAX_ERR);
  }

   RETVALUE(ROK);
} /* end of szEncode */

#ifdef SZTV1

/*
 *  
 *   Fun:  szIeEncode 
 *   
 *   Desc:  To encode the IE message as per ASN.1 PER. 
 *   
 *   Ret:   ROK/RFAILED
 *   
 *   Notes: Invoked thorugh non-database module. 
 *   
 *   File:  sz_msgfn.c
 *   
*/
#ifdef ANSI
PUBLIC S16 szIeEncode
(
 Mem *mem,
 SztEncDecEvnt *sztEncDecEvnt
)
#else /* ANSI */
PUBLIC S16 szIeEncode (mem, sztEncDecEvnt)
 Mem *mem;
 SztEncDecEvnt *sztEncDecEvnt;
#endif /* ANSI */
{
   U8           dbgFlag;
   S16          ret;
   CmPAsnErr    asnErr;
   U32          len=0;
   TknU8        *event;
   CmPAsnElmDef **msgDb;
   Buffer       *mBuf;

   TRC2(szIeEncode)

   dbgFlag = FALSE;

#ifdef DEBUGP
   if (szCb.init.dbgMask & LSZ_DBGMASK_ASN)
      dbgFlag = TRUE;
#endif  /* DEBUGP */
        
   if (sztEncDecEvnt->u.iePdu == NULLP)
   {
     RETVALUE(SZT_CAUSE_UNEXP_MSG);
   }

   if ((SGetMsg(mem->region, mem->pool, &mBuf)) != ROK)
   {
      SZLOGERROR_ADD_RES(ESZ111, 0, "Message Buffer Not allocated");

      SZ_FREE_SDU(sztEncDecEvnt->u.iePdu);

      RETVALUE(SZT_CAUSE_MEM_ALLOC_FAIL);
   }

   /* Initialize the PASN Error code */ 
   asnErr.errCode = 0;


   switch (sztEncDecEvnt->ieType.val)
   {
      case SZT_REQ_TYPE_ENCDEC_IE_SRCTGET:
      {
         event = (TknU8 *)(&sztEncDecEvnt->u.iePdu->u.srcToTget);
         msgDb = szMsgDbIe[SZT_REQ_TYPE_ENCDEC_IE_SRCTGET];

      }
      break;

      case SZT_REQ_TYPE_ENCDEC_IE_TGETSRC:
      {
         event = (TknU8 *)(&sztEncDecEvnt->u.iePdu->u.tgetToSrc);
         msgDb = szMsgDbIe[SZT_REQ_TYPE_ENCDEC_IE_TGETSRC];
      }
      break;
#ifdef SZTV3
    /* sz008.301 Support for Source RNC to Target RNC */
      case SZT_REQ_TYPE_ENCDEC_IE_SRCRNCTGETRNC:
         {
            event = (TknU8 *)(&sztEncDecEvnt->u.iePdu->u.srcRncToTgetRnc);
            msgDb = szMsg3gDbIe[SZT_REQ_TYPE_ENCDEC_IE_SRCRNCTGETRNC];
         }
         break;

      case SZT_REQ_TYPE_ENCDEC_IE_TGETRNCSRCRNC:
         {
            event = (TknU8 *)(&sztEncDecEvnt->u.iePdu->u.TgetRncToSrcRnc);
            msgDb = szMsg3gDbIe[SZT_REQ_TYPE_ENCDEC_IE_TGETRNCSRCRNC];
         }
         break;
#endif
      default:
      {
         RETVALUE(SZT_CAUSE_INV_TYPE);
      }
      break;
   }


   ret = cmPAsnEncMsg(event, CM_PASN_USE_MBUF, 
           (Void *)(mBuf), SZ_PASN_VER_ZERO, 
            (CmPAsnElmDef **) msgDb ,(CmPAsnErr*)&asnErr, 
            &len, dbgFlag, FALSE );

   if (ret == RFAILED)
   {
      /* It's an ASN.1 Error */
      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf,
            " Encoding failed \n")); 

      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf, szErrPrntArr[asnErr.errCode-1],
            asnErr.errCode));

      SZ_FREE_SDU(sztEncDecEvnt->u.iePdu);

      SZ_FREE_BUF(mBuf);

      if (asnErr.errCode == CM_PASN_ESC_FUNC_FLD)
         RETVALUE(SZT_CAUSE_PC_ABS_SYNTAX_ERR);
      else
         RETVALUE(SZT_CAUSE_PC_TX_SYNTAX_ERR);
  }

   SZ_FREE_SDU(sztEncDecEvnt->u.iePdu);
   sztEncDecEvnt->u.mBuf = mBuf;

   RETVALUE(ROK);
} /* end of szIeEncode */
#endif   /* SZTV1 */
 

/*
 *  
 *   Fun:  szDecode
 *   
 *   Desc:  To decode the message as per ASN.1 PER. 
 *   
 *   Ret:   ROK/RFAILED
 *   
 *   Notes: Invoked thorugh non-database module. 
 *   
 *   File:  sz_msgfn.c
 *   
*/
#ifdef ANSI
PUBLIC S16 szDecode
(
 Mem *mem,
 CmPAsnErr *err,
 S1apPdu **s1apPdu, 
 Buffer **mBuf
)
#else /* ANSI */
PUBLIC S16 szDecode (mem, err, s1apPdu, mBuf)
 Mem *mem;
 CmPAsnErr *err;
 S1apPdu **s1apPdu; 
 Buffer **mBuf;
#endif /* ANSI */
{
   U32          numDecOct; /* number of octets decoded */
   S16          ret;       /* return value from different procedures */
   U8           dbgFlag;
   U32          msgLen;
      

   TRC2(szDecode)
                   
   dbgFlag = FALSE;

#ifdef DEBUGP
   if (szCb.init.dbgMask & LSZ_DBGMASK_ASN)
      dbgFlag = TRUE;
#endif  /* DEBUGP */
        
   /* Initialise memCp */
   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, mem, (Ptr *)s1apPdu); 
   if (ret != ROK)
   {
      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf,
            "Memory allocation failed in Decode \n")); 

      SZ_FREE_BUF(*mBuf);

      RETVALUE(SZT_CAUSE_MEM_ALLOC_FAIL);
   }
   /* Initialize the PASN Error code */ 

   err->errCode = 0;
   msgLen = 0;

   ret = (cmPAsnDecMsg((Void *)&((*s1apPdu)->pdu), CM_PASN_USE_MBUF, 
                  (Void **)mBuf, &numDecOct, SZ_PASN_VER_ZERO, 
                  (CmPAsnElmDef **)szMsgDb, CM_PASN_DROP_ELMNTS, 
                  (CmPAsnErr*)err, msgLen,
                  (CmMemListCp *)(&(*s1apPdu)->memCp), dbgFlag));
  
   /* Added the check for error code as in some cases the 
    * return value will be ROK, but error code set by CM PASN. When decode
    * is successful, the error code is set to zero */
   if ((ret == RFAILED) || (err->errCode != 0))
   {
      /* It's an ASN.1 Error */
      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf,
            " Decoding failed \n")); 

      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf, szErrPrntArr[err->errCode-1],
            err->errCode));
      
      SZ_FREE_BUF(*mBuf);

      if ((err->errCode == CM_PASN_ESC_FUNC_FLD) ||
          (err->errCode == CM_PASN_OUT_RANGE ))
      {
         RETVALUE(SZT_CAUSE_PC_ABS_SYNTAX_ERR);
      }
      else
      {
         SZ_FREE_SDU(*s1apPdu);
         RETVALUE(SZT_CAUSE_PC_TX_SYNTAX_ERR);
      }
   }

   RETVALUE(ROK);
} /* end of szDecode */

#ifdef SZTV1 

/*
 *  
 *   Fun:  szIeDecode
 *   
 *   Desc:  To decode the IE message as per ASN.1 PER. 
 *   
 *   Ret:   ROK/RFAILED
 *   
 *   Notes: Invoked thorugh non-database module. 
 *   
 *   File:  sz_msgfn.c
 *   
*/
#ifdef ANSI
PUBLIC S16 szIeDecode
(
 Mem *mem,
 CmPAsnErr *err,
 SztEncDecEvnt *sztEncDecEvnt 
)
#else /* ANSI */
PUBLIC S16 szIeDecode (mem, err, sztEncDecEvnt)
 Mem *mem;
 CmPAsnErr *err;
 SztEncDecEvnt *sztEncDecEvnt;
#endif /* ANSI */
{
   U32          numDecOct; /* number of octets decoded */
   S16          ret;       /* return value from different procedures */
   U8           dbgFlag;
   U32          msgLen;
   Void         *event;
   CmPAsnElmDef **msgDb;
   Buffer       *mBuf;
   
   TRC2(szIeDecode)
                   
   dbgFlag = FALSE;

#ifdef DEBUGP
   if (szCb.init.dbgMask & LSZ_DBGMASK_ASN)
      dbgFlag = TRUE;
#endif /* DEBUGP */

   /* Copy the mBuf Pointer in to the local variable */
   mBuf = sztEncDecEvnt->u.mBuf;
        
   /* Initialise memCp */
   ret = cmAllocEvnt(sizeof(SzIePdu), SZ_MEM_SDU_SIZE, mem,
         (Ptr *) &(sztEncDecEvnt->u.iePdu)); 
   if (ret != ROK)
   {
      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf,
            "Memory allocation failed in Decode \n")); 

      RETVALUE(SZT_CAUSE_MEM_ALLOC_FAIL);
   }
   /* Initialize the PASN Error code */ 

   err->errCode = 0;
   msgLen = 0;

   switch (sztEncDecEvnt->ieType.val)
   {
      case SZT_REQ_TYPE_ENCDEC_IE_SRCTGET:
      {
         event = (Void *)(&sztEncDecEvnt->u.iePdu->u.srcToTget);
         msgDb = szMsgDbIe[SZT_REQ_TYPE_ENCDEC_IE_SRCTGET];
      }
      break;

      case SZT_REQ_TYPE_ENCDEC_IE_TGETSRC:
      {
         event = (Void *)(&sztEncDecEvnt->u.iePdu->u.tgetToSrc);
         msgDb = szMsgDbIe[SZT_REQ_TYPE_ENCDEC_IE_TGETSRC];
      }
      break;
#ifdef SZTV3
    /* sz008.301 Support for Source RNC to Target RNC */
      case SZT_REQ_TYPE_ENCDEC_IE_SRCRNCTGETRNC:
         {
            event = (TknU8 *)(&sztEncDecEvnt->u.iePdu->u.srcRncToTgetRnc);
            msgDb = szMsg3gDbIe[SZT_REQ_TYPE_ENCDEC_IE_SRCRNCTGETRNC];
         }
         break;

      case SZT_REQ_TYPE_ENCDEC_IE_TGETRNCSRCRNC:
         {
            event = (TknU8 *)(&sztEncDecEvnt->u.iePdu->u.TgetRncToSrcRnc);
            msgDb = szMsg3gDbIe[SZT_REQ_TYPE_ENCDEC_IE_TGETRNCSRCRNC];
         }
         break;
#endif
      default:
      {
         RETVALUE(SZT_CAUSE_INV_TYPE);
      }
      break;
   }

   ret = (cmPAsnDecMsg(event, CM_PASN_USE_MBUF, 
          (Void **)&(mBuf) , &numDecOct, SZ_PASN_VER_ZERO, 
            msgDb, CM_PASN_DROP_ELMNTS, 
            (CmPAsnErr*)err, msgLen,
            (CmMemListCp *)(&sztEncDecEvnt->u.iePdu->memCp), dbgFlag));

   if (ret == RFAILED)
   {
      /* It's an ASN.1 Error */
      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf,
            " Decoding failed \n")); 

      SZDBGP(LSZ_DBGMASK_MF, (szCb.init.prntBuf, szErrPrntArr[err->errCode-1],
            err->errCode));
      
      if (err->errCode == CM_PASN_ESC_FUNC_FLD)
      {
         RETVALUE(SZT_CAUSE_PC_ABS_SYNTAX_ERR);
      }
      else
      {
         SZ_FREE_SDU(sztEncDecEvnt->u.iePdu);
         RETVALUE(SZT_CAUSE_PC_TX_SYNTAX_ERR);
      }
   }

   SZ_FREE_BUF(mBuf);

   RETVALUE(ROK);
} /* end of szIeDecode */
#endif   /* SZTV1 */

/**********************************************************************
 
         End of file:     sq_msgfn.c@@/main/nodal_main/3 - Fri Nov 22 17:58:21 2013
 
**********************************************************************/

/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/

/********************************************************************50**
 
*********************************************************************51*/

/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/

/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/5      ---      va           1. Updated for S1AP release 4.1
*********************************************************************91*/
