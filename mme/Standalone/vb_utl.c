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
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:       
    
     File:        vb_utl.c    
  
     Sid:      vb_utl.c@@/main/1 - Mon Jun 28 19:35:40 2010
  
     Prg:       sp   
  
*********************************************************************21*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common pasn define              */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */
#include "egt.h"           /* EGTP Upper Interface */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "lvb.h"
#include "vb_hss.h"        /* CNE Application defines        */
#include "vb.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"
#include "cm_esm.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common pasn library             */
#include "sct.x"           /* S1AP Upper Interface */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */
#include "egt.x"           /* EGTP Upper Interface */
#include "lvb.x"
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */


/********************************************************************20**

     Name:    LTE-CNE

     Type:    C Include file

     Desc:    This file contains utility functions for LTE-CNE

     File:    vb_utl.c

     Sid:      vb_utl.c@@/main/1 - Mon Jun 28 19:35:40 2010

     Prg:     
*********************************************************************21*/

/************************ Prototypes **********************************/

/**********************************************************************/


/***************************************************************/
#ifdef ANSI
PUBLIC S16 vbUtlIpv4InetToArr
(
U8           *addrArr,        /* ascii address representation */
CmInetIpAddr *address         /* 4 byte interent address */
)
#else
PUBLIC S16 vbUtlIpv4InetToArr(addrArr, address)
U8           *addrArr;        /* ascii address representation */
CmInetIpAddr *address;        /* 4 byte interent address */
#endif
{
   addrArr[3] = (U8)(*address & 0xff);
   addrArr[2] = (U8)((*address >> 8) & 0xff);
   addrArr[1] = (U8)((*address >> 16) & 0xff);
   addrArr[0] = (U8)((*address >> 24) & 0xff);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 vbUtlProtoCfgOptToArr
(
U8               *protCfgArr,          /* ascii representation */
U8               *len,
CmEsmProtCfgOpt  *protCfgOpt           /* protocol configuration option*/
)
#else
PUBLIC S16 vbUtlProtoCfgOptToArr(protCfgArr, len, protCfgOpt)
U8               *protCfgArr;           /* ascii representation */
U8               *len; 
CmEsmProtCfgOpt  *protCfgOpt;           /* protocol configuration option*/
#endif
{
   U8     indx = 0;
   S16    i;
   S16    readLen = 0;

   *len = 0;
   protCfgArr[indx]   = protCfgOpt->cfgProt; /* copy 1-3 bits */
   protCfgArr[indx++]|= protCfgOpt->ext << 7;   /* copy 8th bit */
   *len += 1;
   readLen = protCfgOpt->len - 1;

   /* protocol Id list */
   for(i=0; readLen > 0; i++)
   {
      protCfgArr[indx++] = (protCfgOpt->p[i].pid & 0xff00) >> 8;
      protCfgArr[indx++] = protCfgOpt->p[i].pid & 0x00ff;
      protCfgArr[indx++] = protCfgOpt->p[i].len;
      *len += 3;
      readLen -= 3;

      cmMemcpy((U8* )&protCfgArr[indx], (U8* )protCfgOpt->p[i].val, protCfgOpt->p[i].len);
         (indx) += protCfgOpt->p[i].len;
         *len += (protCfgOpt->p[i].len);
         readLen -= protCfgOpt->p[i].len;
   }

   RETVALUE(ROK);
}   

#ifdef ANSI
PUBLIC S16 vbUtlArrToProtoCfgOpt
(
U8               *protCfgArr,          /* ascii representation */
U8               len,
CmEsmProtCfgOpt  *protCfgOpt           /* protocol configuration option*/
)
#else
PUBLIC S16 vbUtlArrToProtoCfgOpt(protCfgArr, len, protCfgOpt)
U8               *protCfgArr;           /* ascii representation */
U8               len; 
CmEsmProtCfgOpt  *protCfgOpt;           /* protocol configuration option*/
#endif
{
   U8    indx = 0;
   U8    i    = 0;
   U16   pci;

   protCfgOpt->pres    = TRUE;
   protCfgOpt->len     = len; 
   protCfgOpt->cfgProt = (protCfgArr[indx] & 0x07); /* copy 1-3 bits */
   protCfgOpt->ext     = ((protCfgArr[(indx)++] >> 7) & 0x01); /* copy 8th bit */
   len -= 1;

   /*for(i=0; len > 0 ;i++)*/
   while(len > 0)
   {
      pci = protCfgArr[(indx)++];
      pci = (pci << 8) | (protCfgArr[(indx)++]);
      if(pci == 0x0c) /*P-CSCF IPV4 Address*/
      {
         protCfgOpt->c[i].cid    = pci;
         protCfgOpt->c[i].len    = protCfgArr[(indx)++];
         len -= 3;

         cmMemcpy((U8* )protCfgOpt->c[i].val, (U8* )&protCfgArr[indx], protCfgOpt->c[i].len);
         indx += protCfgOpt->c[i].len;
         len -= (protCfgOpt->c[i].len);
      }
      else
      {
         protCfgOpt->p[i].pid    = pci;
         protCfgOpt->p[i].len    = protCfgArr[(indx)++];
         len -= 3;

         cmMemcpy((U8* )protCfgOpt->p[i].val, (U8* )&protCfgArr[indx], protCfgOpt->p[i].len);
         indx += protCfgOpt->p[i].len;
         len -= (protCfgOpt->p[i].len);
      }
   }

   RETVALUE(ROK);
}   
/*

*
*       Fun:   vbUtlFillTknStrOSXL
*
*       Desc:  Call handler to fill Octet string
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 vbUtlFillTknStrOSXL
(
TknStrOSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data       *val,     /* byte to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 vbUtlFillTknStrOSXL(ptr, len, val, pdu)
TknStrOSXL *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data       *val;  /* byte to be repetitively filled in */
S1apPdu    **pdu;
#endif
{
   U16     i;     /* counter */

   VB_TRC2(vbUtlFillTknStrOSXL);


   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RETVALUE(RFAILED);
   }

   for(i = 0; i < len; i++)
   {
      ptr->val[i] = val[i];
   }

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;
   ptr->len =  len;

   RETVALUE(ROK);
}

/* vb001.101: Added new function */
#ifdef ANSI
PUBLIC S16 vbUtlFillTknStrOSXL1
(
TknStrOSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val,     /* value to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 vbUtlFillTknStrOSXL1(ptr, len, val, pdu)
TknStrOSXL *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
U32         val;  /* value to be filled in */
S1apPdu    **pdu;
#endif
{
   U16   i;     /* counter */
   U8    tmpVal;

   VB_TRC2(vbUtlFillTknStrOSXL1);


   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RETVALUE(RFAILED);
   }

   for(i = 0; i < len; i++)
   {
      tmpVal = (U8)(val >> (8 * i));
      ptr->val[len - (i + 1)] = tmpVal;
   }

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;
   ptr->len =  len;

   RETVALUE(ROK);
}
/*
*
*       Fun:   vbUtlFillTknStrBSXL
*
*       Desc:  Call handler to fill Octet string
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 vbUtlFillTknStrBSXL
(
TknStrBSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data       *val,     /* byte to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 vbUtlFillTknStrBSXL(ptr, len, val, pdu)
TknStrBSXL *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data       *val;  /* byte to be repetitively filled in */
S1apPdu    **pdu;
#endif
{
   U16     i;     /* counter */

   VB_TRC2(vbUtlFillTknStrBSXL);


   if (cmGetMem(pdu[0], len * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RETVALUE(RFAILED);
   }

   for(i = 0; i < len; i++)
   {
      ptr->val[i] = val[i];
   }

   /* make the element as present */
   /*Fill the Length in bits */
   ptr->pres = PRSNT_NODEF;
   ptr->len =  len * 8;

   RETVALUE(ROK);
}

/*
*
*       Fun:   vbUtlFillTknStr4
*
*       Desc:  Call handler to fill Octet string of length less than 4
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 vbUtlFillTknStr4
(
TknStr4 *ptr,     /* Pointer to the structure */
U8      len,      /* length to be filled */
U8      val       /* byte to be repetitively filled in */
)
#else
PUBLIC S16 vbUtlFillTknStr4(ptr, len, val)
TknStr4 *ptr;     /* Pointer to the structure */
U8      len;      /* length to be filled */
U8      val;      /* byte to be repetitively filled in */
#endif
{
   U8      i;     /* counter */

   VB_TRC2(vbUtlFillTknStr4)


   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(i=0; i < len; i++)
   {
      ptr->val[i] = val;
   }

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;

   RETVALUE(ROK);

} /* end of vbUtlFillTknStr4 */

/*
*
*       Fun:   vbUtlFillU16TknStr4
*
*       Desc:  Call handler to fill Octet string of length less than 4
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 vbUtlFill16TknStr4
(
TknStr4 *ptr,     /* Pointer to the structure */
U8      len,      /* length to be filled */
U16     val       /* byte to be repetitively filled in */
)
#else
PUBLIC S16 vbUtlFill16TknStr4(ptr, len, val)
TknStr4 *ptr;     /* Pointer to the structure */
U8      len;      /* length to be filled */
U16      val;      /* byte to be repetitively filled in */
#endif
{
   U8      i;     /* counter */

   VB_TRC2(vbUtlFill16TknStr4)


   if (len > 4)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      ptr->len = (U8) len;
   }

   for(i=0; i < len; i++)
   {
      if(i == 0)
      {
         ptr->val[i] = val & 0xff00;
      }
      else
      {
         ptr->val[i] = val & 0xff;
      }
   }

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;

   RETVALUE(ROK);

} /* end of vbUtlFillTknStr4 */

/*                                     
*
*       Fun:   vbUtlFillTknU8
*
*       Desc:  Fills  the TknU8 structure with the specified value
*
*       Ret:   ROK       - ok
*
*       Notes: None
*
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16  vbUtlFillTknU8
(
TknU8  *encParam,
U8     val
)
#else
PUBLIC S16 vbUtlFillTknU8(encParam, val)
TknU8  *encParam;
U8     val;
#endif
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
} /* end of vbUtlFillTknU8 */

/*                                     
*
*       Fun:   vbUtlFillTknU16
*
*       Desc:  Fill a TknU16 structure
*
*       Ret:   ROK       - ok
*
*       Notes: None
*
*       File:  vb_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbUtlFillTknU16
(
TknU16  *encParam,
U16     val
)
#else
PUBLIC S16 vbUtlFillTknU16(encParam, val)
TknU16  *encParam;
U16     val;
#endif
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
} /* end of vbUtlFillTknU16 */

/*                                     
*
*       Fun:   vbUtlFillTknU32
*
*       Desc:  Fills the TknU32 structure
*
*       Ret:   RA_ROK       - ok
*
*       Notes: None
*
*       File:  vb_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbUtlFillTknU32
(
TknU32  *encParam,
U32     val
)
#else
PUBLIC S16 vbUtlFillTknU32(encParam, val)
TknU32  *encParam;
U32     val;
#endif
{
   encParam->pres = PRSNT_NODEF;
   encParam->val  = val;
   RETVALUE(ROK);
} /* end of vbUtlFillTknU32 */


/*
*
*       Fun:   vbUtlFillTknBStr32
*
*       Desc:  Call handler to fill Bit String of length less than 32 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 vbUtlFillTknBStr32
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data         *val      /* value to be filled in */
)
#else
PUBLIC S16 vbUtlFillTknBStr32(ptr, len, val)
TknBStr32  *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
Data        *val;  /* value to be filled in */
#endif
{
   U16      i;
   VB_TRC2(vbUtlFillTknBStr32)

      /*KWORKS Fix*/
#ifdef ALIGN_64BIT
   if(len > 8)
#else
   if(len > 4)
#endif
   {
      RETVALUE(RFAILED);
   }

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;
   ptr->len = (U8) (len * 8);
   for(i = 0; i < len; i++)
   {
      ptr->val[i] = val[i];
   }

   RETVALUE(ROK);
}

/* vb001.101: Added new function */
/*
*
*       Fun:   vbUtlFillTknBStr32WithVal
*
*       Desc:  Call handler to fill Bit String of length less than 32 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vb_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 vbUtlFillTknBStr32WithVal
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val      /* value to be filled in */
)
#else
PUBLIC S16 vbUtlFillTknBStr32WithVal(ptr, len, val)
TknBStr32  *ptr;  /* Pointer to the structure */
U16         len;  /* length to be filled */
U32         val;  /* value to be filled in */
#endif
{
   U16      i;
   U8       byteLen = 0;
   U8      crntByte;
   VB_TRC2(vbUtlFillTknBStr32WithVal)

   if(len > 32)
      RETVALUE(RFAILED);

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;
   ptr->len = (U8) (len );

   if((len / 8) == 0)
   {
      byteLen = len/8;
   }
   else
   {
      byteLen = (len/8) + 1;
   }
  
   for(i=0; i< byteLen; i++)
   {
      crntByte= (U8) ((0x000000ff) & val);
      ptr->val[i] = crntByte;
      val = val >> 8;
   }
   RETVALUE(ROK);
}


/*********************************************************************/
PUBLIC S16 vbUtlItoa(U8 *str,U32 num)
{
   U8 temp[32];
   U16 len=0;
   U16 i,j=0;
   cmMemset(temp,0,32); 
   if (!num)
   {
      temp[len]='0';         
      len++;
   }
   else
   {
      while(num)
      {
         temp[len] = num%10 + '0';
         num /= 10;
         len++;
      }
   }
   for(i=len;i>0;i--)
   {
      str[j++]=temp[i-1];
   }
   str[len]='\0';
   return len;
}
/***************************************************************/
PUBLIC S16 vbUtlIpv4ArrToInet(
   U8           *addrArr,        /* Byte Array address representation */
   CmInetIpAddr *address          /* 4 byte interent address */
   )
{
   *address = ((U32)addrArr[0] << 24)|  ((U32)addrArr[1] << 16)   | 
      ((U32)addrArr[2] << 8)  |  (U32)(addrArr[3]);
   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 vbUtlFillTknStrOSXL2
(
TknStrOSXL *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
Data       *val,     /* byte to be repetitively filled in */
S1apPdu    **pdu
)
#else
PUBLIC S16 vbUtlFillTknStrOSXL2(ptr, len, val, pdu)
TknStrOSXL *ptr;  /* Pointer to the structure */
U16         len;  /* lenrah to be filled */
Data       *val;  /* byte to be repetitively filled in */
S1apPdu    **pdu;
#endif
{
   U16     i;     /* counter */
   U8      byteLen = 0;

   VB_TRC2(vbUtlFillTknStrOSXL2);


   if (len %2 == 0)
   {
       byteLen = len / 2;
   }
   else
   {
       byteLen = (len / 2)+ 1;
   }
   if (cmGetMem(pdu[0], byteLen * sizeof(U8), (Ptr *)&ptr->val) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Check for filler digit for odd number of imsi digits*/
   if (len % 2)
   {
       ptr->val[byteLen-1] |= (0x0F << 4);
   }

   for(i = 0; i < byteLen; i++)
   {
      ptr->val[i] |= (val[2*i] & 0x0F);
      /* check for last byte, fill if even number of imsi digits */
      if (!((i + 1 == byteLen) && (len % 2)))
      ptr->val[i] |= ((val[(2*i)+1] & 0x0F)<<4);
   }

   /* make the element as present */
   ptr->pres = PRSNT_NODEF;
   ptr->len =  byteLen;

   RETVALUE(ROK);
}

/*
* 
* This function is used to convert MCC+MNC digit array 
* of 6 bytes into 3 byte PLMN Identity 
* 
*
*/

PUBLIC S16 vbUtlMccMncToPlmn(U8 *plmn,CmEmmTai *ptai)
{
   cmMemset(plmn, 0, 3);
   /* fill 3 digits of mcc in plmn */
   plmn[0] = ptai->mcc[0];
   plmn[0] |= ((ptai->mcc[1]) << 4);
   plmn[1] = ptai->mcc[2];
   /* fill 2 or 3 digits of mnc in plmn */
   plmn[1] |= ((ptai->mnc[2]) << 4);
   plmn[2] = ptai->mnc[0];
   plmn[2] |= ((ptai->mnc[1]) << 4);
   RETVALUE(ROK);

}

/********************************************************************30**

         End of file:     vb_utl.c@@/main/1 - Mon Jun 28 19:35:40 2010

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
/main/1      -          sp             1. LTE-CNE Initial Release.
/main/1  vb001.101      rk             1. Added new functions vbUtlFillTknStrOSXL1,
                                          vbUtlFillTknBStr32WithVal
*********************************************************************91*/
