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
  
     Name:     common asn.1 encoding/decoding
  
     Type:     C include file
  
     Desc:     defines required for encoding/decoding routines
  
     File:     cm_asn.h
  
     Sid:      cm_asn.h@@/main/nodal_main/2 - Thu Nov 21 09:40:00 2013
  
     Prg:      sg
  
*********************************************************************21*/

#ifndef __CMASNH__
#define __CMASNH__

/* If the Encoder is not to encode indefinite form of length encoding then
   the use of static string is mandatory */
 
#ifdef CM_ASN_NO_INDEF_LEN
#ifdef CM_ASN_SS
#undef CM_ASN_SS
#endif /* CM_ASN_SS */
#endif /* CM_ASN_NO_INDEF_LEN */

/* static buffer size. This parameter is used if the encoding and */
/* decoding of messages is to be done in static strings. This gives */
/* size of the static string to be used for encoding. Note that the size */
/* of the static string employed should be able to atleast fit the */
/* largest fully encoded parameter of basic asn.1 types defined by the */
/* protocol. The user of the asn.1 library may modify this parameter */
/* to suit his needs */

/* cm_asn_h_001.main_11 - modificaiton - updated static buffer size to take care
 * of larger buffer requirement for INAP CS2.
 * for other products the value should be 4096, unles the requirement is for
 * larger size.
 */
#ifdef LONG_MSG
#define CM_ASN_SBUF_SIZE    131071 /* static buffer size */
#else
#define CM_ASN_SBUF_SIZE    4096  /* static buffer size */
#endif

/* action to be taken on receiving an unrecognized element */

#define CM_ASN_DROP_ELMNTS   1        /* drop unrecognized elements */
#define CM_ASN_PASS_ELMNTS   2        /* pass unrecognized elements to user */
#define CM_ASN_GEN_ERR       3        /* generate error unrecognized elmnt */
#define CM_ASN_IGN_ELMNTS    4        /* ignore unrecognized elements */

/* defines for switch and the version flag. The defines below give the
   bit positions in the flag for the following protocol types */

#define CM_ASN_ANSI_1988    0         /* protocol is ANSI 1988 */
#define CM_ASN_ANSI_1992    4         /* protocol is ANSI 1992 */
#define CM_ASN_ANSI_1996    8         /* protocol is ANSI 1996 */
#define CM_ASN_ITU_1988     12        /* protocol is ITU 1988 */
#define CM_ASN_ITU_1992     16        /* protocol is ITU 1992 */
#define CM_ASN_ITU_1996     20        /* protocol is ITU 1996 */
#define CM_ASN_MAP_IS41C    24        /* protocol is MAP IS41C */
/* cm_asn_h_001.main_13 : introduce version flag for NTT intra-network*/
#define CM_ASN_NTT_INTR     28        /* protocol is NTT Intra-network */
 
/* values returned to the calling modules from asn.1 module */

#define CM_ASN_MAND_MIS     0x11  /* Mandatory information is missing */
#define CM_ASN_TAG_ERR      0x12  /* Error in the tag */
#define CM_ASN_LEN_ERR      0x13  /* Error in the length field */
#define CM_ASN_UNEXP_VAL    0x14  /* Unexpected value (for enum's) */
#define CM_ASN_RES_ERR      0x15  /* resources error */
#define CM_ASN_OUT_RANGE    0x16  /* out of database defined range */
#define CM_ASN_UNDEF_PARAM  0x17  /* Parameter undefined for the protocol */
#define CM_ASN_NO_FUNC      0x18  /* No user function */
#define CM_ASN_EXTRA_PARAM  0x19  /* extra parameters at the end of the msg */
#define CM_ASN_DUP_ELMNT    0x1A  /* duplicate element */
#define CM_ASN_BAD_IDX      0x1B  /* element index is bad for choice */
#define CM_ASN_INV_IA5STR   0x1C  /* Invalid IA5String */
#define CM_ASN_MEM_ALRDY_ALLOCD 0x1D /* memory already allocated for which 
                                      * lib should allocate the memory */
#define CM_ASN_INVLD_ELMNT  0x1E  /* invalid element type */

/* defines for the flag bits */
#define ELMNT_INV         0x0         /* entry is invalid for this protocol */
#define ELMNT_OPT         0x1         /* element is optional */
#define ELMNT_MAND        0x2         /* element is mandatory */ 
#define IGN_EXTRA_OCTETS  0x4         /* ignore extra octets */ 

/* defines for token element types */
#define TET_STR4        1     /* extra small string */
#define TET_STR12       2     /* small string */
#define TET_STR32       3     /* medium string */
#define TET_STR64       4     /* regular string */
#define TET_STR256      5     /* extended string */
#define TET_U8          6     /* unsigned eigth bit value */
#define TET_ENUM        7     /* enumerated type */
#define TET_NULL        8     /* null type */
#define TET_BITSTR      9     /* bit string */
#define TET_SEQ         10    /* sequence */
#define TET_SEQ_OF      11    /* sequence of */
#define TET_SET         12    /* set */
#define TET_SET_OF      13    /* set of */
#define TET_CHOICE      14    /* choice */
#define TET_INT         15    /* integer */
#define TET_OID         16    /* Object Idebtifier */
#define TET_STRXL       17    /* extra long string */
#define TET_ESC_PRIM    18    /* escape type is primitive */
#define TET_TAG         19    /* tagged type */
#define TET_BOOL        20    /* boolean type */
#define TET_ESC_CONST   21    /* escape type is constructor */
#define TET_INT_RNG     22    /* integer - range check */
#define TET_STRXL_MEM      23    /* mem allocated by library while decoding */
#define TET_UNCONS_SEQ_OF  24    /* mem allocated by library while decoding */
#define TET_UNCONS_SET_OF  25    /* mem allocated by library while decoding */
#define TET_IA5STR4        26    /* extra small ia5 string */
#define TET_IA5STR12       27    /* small ia5 string */
#define TET_IA5STR32       28    /* medium ia5 string */
#define TET_IA5STR64       29    /* regular small ia5 string */
#define TET_IA5STR256      30    /* extended ia5 string */
#define TET_IA5STRXL       31    /* extra long ia5 string */
#define TET_IA5STRXL_MEM   32    /* mem allocated by library while decoding */

/* Some big value for set/seq terminator */
#define TET_SETSEQ_TERM 99    /* set or sequence terminator */

/* escape type by default is primitve - done for BC */
#define TET_ESC TET_ESC_PRIM  /* type for database escape routine */

/* universal tag types used in common asn.1 library */
#define CM_ASN_UBOOL       0x1   /* universal boolean type tag */ 
#define CM_ASN_UINT        0x2   /* universal integer type tag */ 
#define CM_ASN_UBITSTR     0x3   /* universal bit string type tag */ 
#define CM_ASN_UOCTSTR     0x4   /* universal octet string type tag */ 
#define CM_ASN_UNULL       0x5   /* universal null type tag */ 
#define CM_ASN_PRIM_UOID   0x6   /* universal oid type tag - primitive */ 
#define CM_ASN_CONST_UOID  0x26  /* universal oid type tag - constructor */ 
#define CM_ASN_UENUM       0xA   /* universal enumerated type tag */ 
#define CM_ASN_UIA5        0x16  /* universal IA5 string tag */ 
#define CM_ASN_USEQ        0x30  /* universal sequence type tag */ 
#define CM_ASN_USEQOF      0x30  /* universal sequence of type tag */ 
#define CM_ASN_USET        0x31  /* universal set type tag */ 
#define CM_ASN_USETOF      0x31  /* universal set of type tag */ 

/* values for boolean type */
#define CM_ASN_TRUE        1     /* boolean true */
#define CM_ASN_FALSE       0     /* boolean false */

/* minimum length */
#define MIN_LEN_NA  0x0        /* minimum length not applicable */
#define MAX_LEN_NA  0x0        /* maximum length not applicable */

/* cm_asn_h_001.main_12-addition-Max length defines for TET_STR tokens */
#define TET_STR12_SIZE 12
#define TET_STR32_SIZE 32
#define TET_STR64_SIZE 64
#define TET_STR132_SIZE 132


/* repeat counter not applicable */
#define REP_CNTR_NA   0x0      /* repeat counter not applicable */

/* maximum's for the ASN.1 module */
#define MAX_TAG_LEN   4         /* maximum tag length */
#define MAX_LEN_BYTES 4         /* maximum number of bytes in length field */ 

/* defines for the encode decode flag */
#define ENCODE_ELMNT  1         /* encode the element */
#define DECODE_ELMNT  2         /* decode the element */

#define PROCESS_LENGTH  3       /* Process the indefinite length encoding */

/* defines for the sizes of the ASN.1 types */
#define SIZEOF_OCTET    1       /* length of the octet in bytes */
#define SIZEOF_NULL     0       /* length of ASN.1 null type */

/* size of the buffer for encoding an octet */
#define OCTET_BUFSIZE   (MAX_TAG_LEN + SIZEOF_OCTET + 1)
#define NULL_BUFSIZE    (MAX_TAG_LEN + 1)
#define STR_BUFSIZE     (MAX_TAG_LEN + MAX_LEN_BYTES)
#define BITSTR_BUFSIZE  (MAX_TAG_LEN + MAX_LEN_BYTES + MF_SIZE_TKNSTRS)
#define SEQ_BUFSIZE     (MAX_TAG_LEN + MAX_LEN_BYTES)
#define OIDSTR_BUFSIZE  (MAX_TAG_LEN + MAX_LEN_BYTES + 64)

/* Return values internal to the ASN.1 module - change later */

/* defines related to encoding/decoding of length */
#define INITIAL_SHIFTBITS  7     /* number of shift bits for initial shift */
#define NUMBITS_BYTE       8     /* number of bits in a byte */
#define EOC_FLAG           0x80  /* Indeterminate length flag */
#define MAX_SBYTE_VAL      127   /* maximum value in a signed byte value */        
#define EOC_LEN            EOC_FLAG   /* Length for EOC type encoding */
#define EOC_LEN_SIZE       1          /* Size of length field - EOC encoding */
#define EOC_TAG            0x00       /* End of constructor type */

#define EOC_TAG_LEN        2     /* EOC Tag length is two bytes */

#define LONG_TAG_IND       0x1f  /* Indicates more than one byte in the tag */
#define TAG_MORE_BIT       0x80  /* Indicates more than one byte in the tag */
#define IA5_CHARMASK       0x80  /* Character mask for IA5String */

/* macro for updating the error code and tag in the error structure */
#define CM_ASN_ERR(msgCp, error, elmntTag)                 \
   {                                                       \
      U8         k;                     /* counter */      \
      CmAsnTag   *tagP;                 /* tag pointer */  \
                                                           \
      msgCp->err->errCode = error;                          \
      tagP = elmntTag;                                     \
      if (tagP != NULLP)                                   \
      {                                                    \
         for (k=0;k < tagP->len; k++)                      \
         {                                                 \
            msgCp->err->tag.val[k] = tagP->val[k];          \
         }                                                 \
      }                                                    \
   }

/* get the error code from the message structure */
#define CM_ASN_GETERR(msgCp)    (msgCp->err->errCode)

/* get the tag from the message structure */
#define CM_ASN_TAGVAL(msgCp)     (msgCp->err->tag)

/* get the tag from the message structure */
#define CM_ASN_EVNTSTR(msgCp)     (msgCp->evntStr)

/* get the tag from the message structure */
#define CM_ASN_MSGBUF(msgCp)     (msgCp->mBuf)

/* this macro generates a flag depending on the element information */
#define TYPE_TO_FLAG(prot, type)   (type << (prot%32))

/* this macro gets the element information (optional/mandatory) */
#define FLAG_TO_TYPE(flag, prot)   ((*(flag+(prot/32))>>(prot%32))&0x3) 

/* check to see in the flag if ignore flag is set */
#define IS_IGNORE_EXT(flag, prot)  ((*(flag+(prot/32))>>((prot%32)+2))&0x1)


/* update the memory paramameters (region and pool) in the message */
/* control structure */

#define CM_ASN_UPD_MEMP(msgCp, region, pool)                 \
do                                                           \
{                                                            \
   msgCp->region = region;                                   \
   msgCp->pool   = pool;                                     \
} while(0);

/* macro for incrementing the event structure pointer over the start */
/* token of the constructor types */

#define CM_ASN_SKIP_TKNPRES(msgCp)                           \
do                                                           \
{                                                            \
   msgCp->evntStr =                                          \
       (TknU8 *) ((PTR) msgCp->evntStr + sizeof(TknU8));     \
                                                             \
}while(0);


/* static buffer manipulation macros */

#ifndef CM_ASN_SS       /* do not use system services */ 

/* --------------------------------------------------------- */ 

/* Find the message length */

#define CM_ASN_FND_LEN(msgCp, lenP)       \
do {                                      \
   *lenP = msgCp->sBuf.size;              \
} while (0);

/* --------------------------------------------------------- */ 
/* examine the message */

#define CM_ASN_EXAM_MSG(c, msgCp, offset, ret)                \
do {                                                          \
   if (msgCp->sBuf.size <= (U16)offset)                       \
   {                                                          \
      *ret = ROKDNA;                                          \
   }                                                          \
   else                                                       \
   {                                                          \
                                                              \
      *c = *(msgCp->sBuf.bufP + msgCp->sBuf.stIdx + offset);  \
      *ret = ROK;                                             \
   }                                                          \
} while(0);

/* --------------------------------------------------------- */ 

/* remove multiple bytes from the end of the message */

#define CM_ASN_REM_PST_MULT(data, count, msgCp, ret)                    \
{                                                                       \
   if (msgCp->sBuf.size < (U16) count)                                  \
   {                                                                    \
      *ret = ROKDNA;                                                    \
   }                                                                    \
   else                                                                 \
   {                                                                    \
                                                                        \
      if (data != NULLP)                                                \
      {                                                                 \
         (Void) cmCopy((U8 *)(msgCp->sBuf.bufP + stIdx + size - count), \
                       (U8 *)data, (U32) count);                        \
      }                                                                 \
                                                                        \
      msgCp->sBuf.size  = (msgCp->sBuf.size - count);                   \
      *ret = ROK;                                                       \
   }                                                                    \
}

/* --------------------------------------------------------- */ 

/* remove multiple bytes from the start of the message */

#define CM_ASN_REM_PRE_MULT(data, count, msgCp, ret)                  \
{                                                                     \
   if (msgCp->sBuf.size < (U16) count)                                \
   {                                                                  \
      *ret = ROKDNA;                                                  \
   }                                                                  \
   else                                                               \
   {                                                                  \
      if (data != NULLP)                                              \
      {                                                               \
         (Void) cmCopy((U8 *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx),  \
                       (U8 *)data, (U32) count);                      \
      }                                                               \
                                                                      \
      msgCp->sBuf.stIdx = msgCp->sBuf.stIdx + count;                  \
      msgCp->sBuf.size  = (msgCp->sBuf.size - count);                 \
      *ret = ROK;                                                     \
   }                                                                  \
}

/* --------------------------------------------------------- */ 

/* remove single byte from the start of the message */

#define CM_ASN_REM_PRE(c, msgCp, ret)                             \
{                                                                 \
   if (msgCp->sBuf.size == 0)                                     \
   {                                                              \
      *ret = ROKDNA;                                              \
   }                                                              \
   else                                                           \
   {                                                              \
      if (c != NULLP)                                             \
      {                                                           \
         *c = *((U8 *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx));    \
      }                                                           \
                                                                  \
      msgCp->sBuf.stIdx++;                                        \
      msgCp->sBuf.size--;                                         \
      *ret = ROK;                                                 \
   }                                                              \
}

/* --------------------------------------------------------- */ 

/* add multiple bytes to the end of the message */
/* only during encoding, the size of the static string can be */
/* less than the number of bytes being added to the static string */

#ifndef CM_ASN_NO_INDEF_LEN
#define CM_ASN_ADD_PST_MULT(data, count, msgCp, ret)                      \
{                                                                         \
   if (((U16)(msgCp->sBuf.stIdx + msgCp->sBuf.size + (U16)count)) >       \
                                              msgCp->sBuf.max)            \
   {                                                                      \
      /* see if the static string can be emptied */                       \
      /* Assuming we will reach here only while encoding */               \
      if ((*ret = SAddPstMsgMult(                                         \
                  (Data *) (msgCp->sBuf.bufP + msgCp->sBuf.stIdx),        \
                  (MsgLen) msgCp->sBuf.size,                              \
                           msgCp->mBuf                                    \
                                )) == ROK)                                \
      {                                                                   \
         msgCp->sBuf.size  = 0;         /* make the size 0 */             \
         msgCp->sBuf.stIdx = 0;         /* Init the start index */        \
                                                                          \
         /* If data is large enough to be fitted in the static string     \
            then copy it directly to message buffer */                    \
         if (((U16)(msgCp->sBuf.stIdx + (U16)count)) > msgCp->sBuf.max)   \
         {                                                                \
            *ret = SAddPstMsgMult(data, count, msgCp->mBuf);              \
         }                                                                \
         else                                                             \
         {                                                                \
            (Void) cmCopy((U8 *)data,                                     \
                          (U8 *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx),   \
                          (U32) count);                                   \
                                                                          \
            msgCp->sBuf.size = count;                                     \
            *ret = ROK;                                                   \
         }                                                                \
      }                                                                   \
   }                                                                      \
   else                                                                   \
   {                                                                      \
      (Void) cmCopy((U8 *)data,                                           \
                    (U8 *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +         \
                           msgCp->sBuf.size),                             \
                           (U32) count);                                  \
                                                                          \
      msgCp->sBuf.size  = (msgCp->sBuf.size + count);                     \
      *ret = ROK;                                                         \
   }                                                                      \
}
#else
#define CM_ASN_ADD_PST_MULT(data, count, msgCp, ret)                      \
{                                                                         \
   if (((U16)(msgCp->sBuf.stIdx + msgCp->sBuf.size + (U16)count)) >       \
                                              msgCp->sBuf.max)            \
   {                                                                      \
      *ret = RFAILED;                                                     \
   }                                                                      \
   else                                                                   \
   {                                                                      \
      (Void) cmCopy((U8 *)data,                                           \
                    (U8 *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx +         \
                           msgCp->sBuf.size),                             \
                           (U32) count);                                  \
                                                                          \
      msgCp->sBuf.size  = (msgCp->sBuf.size + count);                     \
      *ret = ROK;                                                         \
   }                                                                      \
}
#endif /* CM_ASN_NO_INDEF_LEN */

/* --------------------------------------------------------- */ 

/* add multiple bytes to the end of the message */

#define CM_ASN_CPY_MSG_FIX(msgCp, srcIdx, bToCpy, dst, bActCpy, ret)        \
{                                                                           \
   if (((U16)(srcIdx + bToCpy)) > msgCp->sBuf.size)                         \
   {                                                                        \
      *ret = ROKDNA;                                                        \
   }                                                                        \
   else                                                                     \
   {                                                                        \
      (Void) cmCopy((U8 *) (msgCp->sBuf.bufP + msgCp->sBuf.stIdx + srcIdx), \
                    (U8 *) dst,                                             \
                    (U32)  bToCpy);                                         \
                                                                            \
      *bActCpy = bToCpy;                                                    \
      *ret = ROK;                                                           \
   }                                                                        \
}

/* --------------------------------------------------------- */ 

/* Concatenate a message buffer to the msgCp.mBuf in the specified order.
   Before that put the encoded element in the static string at the end of
   msgCp.mBuf and leave the static string (msgCp.sBuf) empty */

#define CM_ASN_CAT_MSG(msgCp, buf, order, ret)                            \
{                                                                         \
   /* copy the non-empty static encoded string to message buffer */       \
   if (msgCp->sBuf.size > 0)                                              \
   {                                                                      \
      if ((*ret = SAddPstMsgMult(                                         \
                           (Data *)(msgCp->sBuf.bufP + msgCp->sBuf.stIdx),\
                           msgCp->sBuf.size,                              \
                           msgCp->mBuf                                    \
                                      )) == ROK)                          \
      {                                                                   \
         msgCp->sBuf.size  = 0; /* Empty the static string */             \
         msgCp->sBuf.stIdx = 0; /* Init the start index */                \
                                                                          \
         *ret = SCatMsg(msgCp->mBuf, buf, order);                         \
      }                                                                   \
   }                                                                      \
   else                                                                   \
   {                                                                      \
      *ret = SCatMsg(msgCp->mBuf, buf, order);                            \
   }                                                                      \
}

#endif /* CM_ASN_SS */

/* defines for error codes */
#define   ERRCMASN       0

#define   ECMASN001      (ERRCMASN +    1)    /*     cm_asn.c:1267 */
#define   ECMASN002      (ERRCMASN +    2)    /*     cm_asn.c:1607 */
#define   ECMASN003      (ERRCMASN +    3)    /*     cm_asn.c:1732 */
#define   ECMASN004      (ERRCMASN +    4)    /*     cm_asn.c:1777 */
#define   ECMASN005      (ERRCMASN +    5)    /*     cm_asn.c:2681 */
#define   ECMASN006      (ERRCMASN +    6)    /*     cm_asn.c:3432 */
#define   ECMASN007      (ERRCMASN +    7)    /*     cm_asn.c:3444 */
#define   ECMASN008      (ERRCMASN +    8)    /*     cm_asn.c:3533 */
#define   ECMASN009      (ERRCMASN +    9)    /*     cm_asn.c:3544 */
#define   ECMASN010      (ERRCMASN +    10)   /*     cm_asn.c:6360 */
#define   ECMASN011      (ERRCMASN +    11)   /*     cm_asn.c:6561 */

#endif  /* __CMASNH__ */

  
/********************************************************************30**
  
         End of file:     cm_asn.h@@/main/nodal_main/2 - Thu Nov 21 09:40:00 2013
   
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
1.3          ---      nj   1. Added support for Object Identifiers
1.4          ---      nj   1. Added support for Integer
                           2. Added asn.1 support for TCAP
1.5          ---      sg   1. Added define for ignoring elements in a
                              asn.1 encoded message
1.6          ---      nj   1. Modified CM_ASN_ADD_PST_MULT macro to support all
                              definite form of length encoding
                           2. Regenerated the error codes.

/main/8      ---      pk   1. Hash define for new token type TET_INT_RNG.
                           2. New error codes added for range checking.
/main/9      ---      pk   1. New error cause added.

/main/10     ---     ck    1. New token types TET_STRXL_MEM, TET_IA5STRXXX, 
                              TET_UNCONS_SET_OF and TET_UNCONS_SEQ_OF were 
                              added. 

/main/11     ---     yk    1. Modified FALG_TO_TYPE, TYPE_TO_FLAG and IS_IGNORE_EXT 
                              macros to support more than eight protocol flags for
                              MAP-IS41. 
/main/11+  cm_asn_h_001.main_11 1. updated static buffer size to take care
                                   of larger buffer requirement for INAP CS2.
                                   for other products the value should be 4096,
                                   unles the requirement is for larger size.
/main/12     ---     mc    1. Updated for INAP 1.3 release.                                   
cm_asn_h_001.main_12   ak  1. added defines for size of TET_STR12_SIZE,TET_STR32_SIZE
                              TET_STR64_SIZE,TET_STR132_SIZE.
*********************************************************************91*/
