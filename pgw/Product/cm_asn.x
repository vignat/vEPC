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
  
     Desc:     strutures and typedef's for encoding/decoding routines
  
     File:     cm_asn.x
  
     Sid:      cm_asn.x@@/main/17 - Thu Apr  2 14:32:16 2009
  
     Prg:      sg
  
*********************************************************************21*/

#ifndef __CMASNX__
#define __CMASNX__

#ifdef __cplusplus
EXTERN "C" {
#endif

/* forward decleration for message control point */
typedef struct cmAsnMsgCp CmAsnMsgCp;

/* ASN.1 tag type */
typedef struct cmAsnTag        /* ASN.1 tag type */
{
   U8        len;              /* number of bytes in the tag */
   U8        val[MAX_TAG_LEN]; /* tag value */
} CmAsnTag;

/* enumerated token */
/* cm_asn_x_001.main_15: Deleted typedef for TknEnum */

/* Element Defintion. Each message element consisting of basic ASN.1 type
   is reperesented in this form in the database */

typedef struct cmElmntDef     /* Message element definition */
{
#ifdef CM_ASN_DBG
   Data      str[64];         /* string describing the element type */
#endif
   U8        type;            /* Message element type */
   CmAsnTag  tag;             /* tag value */
   U32       minLen;          /* minimum length of the element */
   U32       maxLen;          /* maximum length of the element */
   /* cm_asn_x_001.main_12 - modification - data type changed from U16 to MsgLen */
   MsgLen    dbSize;          /* database size */
   MsgLen    evSize;          /* event structure size */
   S32       repCntr;         /* counter for repeatble elements */
   U32       *flagp;          /* protocol flags */
   U8        *enumLst;        /* list of enumerated values */
   S16 (*func) ARGS((CmAsnMsgCp *msgCp)); /* user function */

} CmElmntDef;

/* Message definition of each message in the database. This is also
   an entry in the messaage table of protocol messages */

typedef struct cmMsgTblElmnt  /* message definition */
{
   U32        *flagp;         /* flags for protocol version */
   U8         proType;        /* protocol type for protocol specific checks */
   CmElmntDef **msgDef;       /* token element list */
} CmMsgTblElmnt;


/* ASN.1 error structure. This is returned to the caller to indicate
   the error which occurred during the encoding/decoding of the message */

typedef struct cmAsnErr       /* Encode/decode error */
{
   CmAsnTag  tag;             /* tag of the parameter in error */ 
   S16       errCode;         /* Error type */
} CmAsnErr;

/* static buffer information - the encoder decoder will either use
   the static buffer or the message buffer to encode/decode */
 
/* cm_asn_x_001.main_14 - modifiction */
typedef struct sBufInfo       /* static buffer */
{
   Data      *bufP;           /* pointer to the static buffer */
   /* cm_asn_x_001.main_12 - modification - data type changed from U16 to MsgLen */
   MsgLen    stIdx;           /* start index in the static buffer */
   MsgLen    ctIdx;           /* Current index in the static buffer */
   MsgLen    size;            /* size of the static buffer */
   MsgLen    max;             /* maximum size of the static buffer */
} SBufInfo;

/* ASN.1 message control point. This structure is used to pass
   information to the encoding/decoding routines.  */

struct cmAsnMsgCp             /* message encode/decode control point */
{
   U8          edFlag;        /* whether to encode/decode */
   Bool        octEnum;       /* whether this is an enumerated or octet */
   U32         *flagp;        /* flag for the message */
   U8          proType;       /* protocol type */
   Buffer      *mBuf;         /* message buffer for encoding/decoding */
   Buffer      *uBuf;         /* buffer containing unrecognized elements */
   Region      region;        /* region to allocate the memory from */
   Pool        pool;          /* pool to allocate the memory from */
   U8          cfg;           /* configuration for unrecognized elements */
   SBufInfo    sBuf;          /* static buffer information */
   TknU8       *evntStr;      /* event structure for encoding/decoding */
   CmElmntDef  **elmntDef;    /* element definition in the database */
   CmAsnErr    *err;          /* ASN.1 encode/decode error */

#ifdef CM_ASN_NO_INDEF_LEN
   U32         crntEncLen;    /* Current length required to encode constructor type
                                 length */
   Bool        usrActReq;     /* User action required */
   TknU8       *strtEvPtr;    /* Save the start pointer for the event structure */
   CmElmntDef  **strtElmntPtr;/* Save the start pointer for the element definition */
#endif /* CM_ASN_NO_INDEF_LEN */
   U8          choiceRes;     /* Choice result */
   U8          choiceResInSet;     /* Choice result in a set */
};

/* External declerations for routines exported by the ASN.1 module */

EXTERN S16 cmEncMsg  ARGS((TknU8 *evntStr, Buffer *mBuf, U8 proType, CmElmntDef **msgDef, Region region, Pool pool, CmAsnErr *err, Buffer *uBuf));
EXTERN S16 cmDecMsg  ARGS((TknU8 *evntStr, Buffer *mBuf, U8 proType, CmElmntDef **msgDef, Region region, Pool pool, U8 cfg, CmAsnErr *err, Buffer **uBuf));
EXTERN S16 cmIncPtr  ARGS((PTR *addr, U32 len));
EXTERN S16 cmEncLen  ARGS((CmElmntDef *elmntDef, U32 len, Data *pkArray, U16 *numBytes));
EXTERN S16 cmFindLen ARGS((CmAsnMsgCp *msgCp, MsgLen offset, MsgLen *len, Bool *eocFlg, MsgLen *lfLen)); 
EXTERN S16 cmInitMsgDb ARGS((CmElmntDef **msgDef));
/* cm_asn_x_001.main_12 - modification - data type for strLen changed from U16 to
 * MsgLen
 */
EXTERN S16 cmRemPrim ARGS((Buffer *mBuf, CmAsnMsgCp *msgCp, U8 *destStr, MsgLen *strLen));

/* encoding/decoding routines */
EXTERN S16 cmEncOctetEnum ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncNull ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncOctetStr ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncIa5Str ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncBitStr ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncSetSeq ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncSetSeqOf ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncUnconsSetSeqOf ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncChoice ARGS((CmAsnMsgCp *msgCp));

EXTERN S16 cmEncInteger ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncOid ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncBool ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncTagType ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncIntRng ARGS((CmAsnMsgCp *msgCp));

EXTERN S16 cmDecOctetEnum ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecNull ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecOctetStr ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecIa5Str ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecBitStr ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecSeq ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecSetSeqOf ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecUnconsSetSeqOf ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecSet ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecChoice ARGS((CmAsnMsgCp *msgCp));

EXTERN S16 cmDecInteger ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecOid ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecBool ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecTagType ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecAnyDefinedBy ARGS((CmAsnMsgCp *msgCp, U8 idx));
EXTERN S16 cmGetTag ARGS((CmAsnMsgCp *msgCp, U8 *pkArray, U16 offset, U8 *size));
EXTERN S16 cmDecIntRng ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmEncElmnt ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmDecElmnt ARGS((CmAsnMsgCp *msgCp));
EXTERN S16 cmChkMsgMandMis ARGS((CmAsnMsgCp *msgCp));
/* cm_asn_x_001.main_16 : Added prototype */
EXTERN S16 cmChkTag ARGS((CmAsnMsgCp *msgCp, U8 *pkArray, U8 size, Bool *skipElmnt));


#ifdef __cplusplus
}
#endif

#endif  /* __CMASNX__ */

  
/********************************************************************30**
  
         End of file:     cm_asn.x@@/main/17 - Thu Apr  2 14:32:16 2009
   
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
------------ -------- ---- -----------------------------------------------
1.1            ---    sg   1. initial release.
 
*********************************************************************81*/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.2          ---      nj   1. Added support for Extra Long String
                              Added support for Object Identifiers
                              Added support for Integer
                              Added asn.1 support for TCAP

1.3          ---      sg   1. Added static buffer information to message
                              control point.
1.4          ---      sg   1. Added macros for static buffer manipulations
 
1.5          ---      sg   1. Added protection for C++ compiler
 
1.6          ---      nj   1. Added few parameters to cmAsnMsgCp structure to support all
                              definite form of length encoding
/main/8      ---      pk   1. Sizes of minLen and maxLen increased to U32 to
                              store the maximum INTEGER value.
                           2. Declaration of two new routines.
/main/9      ----      pk   1. Field 'choiceRes' added for proper decoding 
                              of Choice.

/main/10     ---     ck    1. New token types TET_STRXL_MEM, TET_IA5STRXXX, 
                              TET_UNCONS_SET_OF and TET_UNCONS_SEQ_OF were 
                              added. 
                           2. Added choiceRes field to CmAsnMsgCp for decoding
                              of nested choice.
/main/11     ---     as    1. Decoding of CHOICE in a SET is corrected by
                               adding a field in CmAsnMsgCp
/main/12     ---     hr    1. Make some functions PUBLIC for use by protocol
cm_asn_x_001.main_12 mc    1. U16 data type for members of CmElmntDef and 
                               SBufInfo changed to MsgLen to take care
                               of length greater than U16.
                           2. data type of strLen Argument of cmRemPrim 
                               changed from U16 to MsgLen.
/main/14     ---     mc    1. Update for Release INAP 1.3
/main/14+  cm_asn_x_001.main_14  mc - correcting the sid problem.
/main/15     ---     mc    1. Correcting the Sid problem.
/main/16     ---      cm_asn_x_001.main_15 rkumar Deleted TknEnum
/main/17  cm_asn_x_001.main_16 sv 1. Added prototype for cmChkTag().
*********************************************************************91*/
