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
  
     Name:     e-GTP utility API definition
  
     Type:     C source file
  
     Desc:     Definitions of the utility API
  
     File:     eg_util.c
  
     Sid:      eg_util.c@@/main/3 - Fri Feb  4 18:18:13 2011
  
     Prg:      kc
  
*********************************************************************21*/

/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */

#include "cm_dns.h"        /* common transport */
#include "hit.h"           /* TUCL Layer */
#include "leg.h"           /* eGTP Layer */
#include "eg.h"            /* eGTP Layer */
#include "eg_err.h"        /* eGTP Error Codes */
/* eg004.201 eGTPC PSF header files included*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"
#endif
#include "egt.h"           /* GTP Upper Interface */
#include "eg_util.h"       /* GTP utilities header */
#include "eg_err.h"        /* Error codes */
#include "eg_edm.h"        /* Error codes */

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_dns.x"        /* common transport */
#include "hit.x"           /* TUCL Layer */
#include "egt.x"           /* GTP Upper Interface */
#include "leg.x"           /* eGTP layer */
#include "eg_edm.x"        /* EDM Module structures            */
/* eg004.201 eGTPC PSF header files included*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "eg.x"           /* eGTP layer */
#include "eg_tpt.x"            /* EGTP TPT module defines */
#include "eg_util.x"       /* GTP Utility library */

/* eg004.201 eGTPC PSF header files included*/
#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

/********************************************************************
 * L O C A L     D E F I N E S                                      *
 *******************************************************************/

/********************************************************************
 * L O C A L     E X T E R N S                                      *
 *******************************************************************/

/********************************************************************
 * L O C A L     V A R I A B L E    D E C L A R A T I O N S         *
 *******************************************************************/

#ifdef __cplusplus
EXTERN "C" {
#endif

#ifdef EGTP_C
/********************************************************************
 * L O C A L     F U N C T I O N    D E C L A R A T I O N S        *
 *******************************************************************/

/******************************************************************
 * This method will be used for deleting the IEs from the ielist. *
 ******************************************************************/
/* fix for ccpu00136098 */
PUBLIC S16 egUtilDropIeList ARGS( (CmLListCp *ieList));
/*********************************************************
 * This is used to get the IE list at a particular level *
 *********************************************************/
PRIVATE S16 egUtilGetTrgtList ARGS( (CmLListCp *ieList, EgIeLevelInfo *ieLvlInfo,
                             CmLListCp **trgtList, U16 strtLvl, U16 *trgtLvl));

#ifdef EG_PHASE2
/**************************************************************************************
 * This method is used to compare the IE value with the value given in IE information *
 **************************************************************************************/
PRIVATE S16 egUtilCompareIeValue ARGS( (EgIe *egIe, EgIeInfo *ieInfo));

/***********************************************
 * This method is used to duplicate an ie list *
 ***********************************************/
PRIVATE S16 egUtilDupList ARGS( (CmLListCp *ieList, CmLListCp *dupIeList, CmMemListCp *memCp));
#endif

/**************************************
 * This method is used to dump the IE *
 **************************************/
#ifdef LEG_FILE_LOG
PRIVATE S16 egUtilDumpIe ARGS( (EgIe *egIe, S16 lvl, FILE *fp, U16 *nmbLines));
#else
PRIVATE S16 egUtilDumpIe ARGS( (EgIe *egIe, S16 lvl));
#endif /* end of LEG_FILE_LOG */

/*******************************************
 * This method is used to dump the IE list *
 *******************************************/
#ifdef LEG_FILE_LOG
PRIVATE S16 egUtilDumpList ARGS( (CmLListCp *ieList, S16 lvl, FILE *fp, U16 *nmbLines));
#else
PRIVATE S16 egUtilDumpList ARGS( (CmLListCp *ieList, S16 lvl));
#endif /* end of LEG_FILE_LOG */

/***************************************************
 * This method is used to dump the diameter header *
 ***************************************************/
#ifdef LEG_FILE_LOG
PRIVATE S16 egUtilDumpGmHdr ARGS( (EgMsgHdr egGmHdr, FILE *fp, U16 *nmbLines));
#else
PRIVATE S16 egUtilDumpGmHdr ARGS( (EgMsgHdr egGmHdr));
#endif /* end of LEG_FILE_LOG */

/***************************************
 * This method is used to delete an IE *
 ***************************************/

/**********************************************
 * This method is used to delete a grouped IE *
 *********************************************/
PRIVATE S16 egUtilDelGrpIe ARGS ( (CmLListCp *ieList, EgIeLevelInfo *ieLvlInfo,
                                    CmLListCp **trgtList, U16 strtLvl, U16 *trgtLv));

#ifdef EG_PRNT_DATA
/***************************************************
 Utility functions for dumping ipv4 / ipv6 address *
 ***************************************************/
PRIVATE S16 egUtilDumpIpv4Addr ARGS ( (Txt *buff, U8 *ipv4Addr) );
PRIVATE S16 egUtilDumpIpv6Addr ARGS ( (Txt *buff, U8 *ipv6Addr ) );

/***********************************************
 Utility functions for dumping Serving Network *
 ***********************************************/
PRIVATE S16 egUtilDumpSNwk ARGS ( (Txt *buff, EgSNetwork sNwk) );


PRIVATE S16 egUtilDumpAuthQt ARGS((Txt *buff, EgAuthQt authQuin ));

PRIVATE S16 egUtilDumpAuthQuadruplet ARGS((Txt *buff, EgAuthQuadruplet quadruplet));

PRIVATE S16 egUtilDumpIpAddr ARGS ((Txt *buff, EgIpAddr *ipAddr));
#endif

/*******************************************************
 Utility functions for duplicating ipv4 / ipv6 address *
 *******************************************************/
PRIVATE S16 egUtilDupIpv4Addr ARGS ( (U8 *dupIp4Addr, U8 *oldIp4Addr) );
PRIVATE S16 egUtilDupIpv6Addr ARGS ( (U8 *dupIp6Addr, U8 *oldIp6Addr ) );

/***************************************************
 Utility functions for duplicating Serving Network *
 ***************************************************/
PRIVATE S16 egUtilDupSNwk ARGS ( (EgSNetwork *dupSNwk, EgSNetwork oldSNwk) );

PRIVATE S16 egUtilDupUliData ARGS ( (EgIe **dupIe,EgIe *oldIe ) );
PRIVATE S16 egUtilDupPaa ARGS ( (EgIe **dupIe,EgIe *oldIe ) );

#endif  /* End of EGTP_C */

#ifdef EGTP_U

/********************************************************************
 * L O C A L     F U N C T I O N    D E C L A R A T I O N S        *
 *******************************************************************/

#ifdef EG_FILE_LOG
/**************************************
 * This method is used to GTPU Hdr    *
 **************************************/
PRIVATE S16 egUUtilDumpGmHdr ARGS (( EgUMsgHdr egGmHdr, FILE *fp,U16 *nmbLines));

/*******************************************
 * This method is used to dump the IE List *
 *******************************************/
PRIVATE S16 egUUtilDumpList ARGS (( CmLListCp *ieList, FILE *fp, U16 *nmbLines));

/**************************************
 * This method is used to dump the IE *
 **************************************/
PRIVATE S16 egUUtilDumpIe ARGS (( EgUIe *egIe,FILE *fp, U16 *nmbLines ));

#else
/**************************************
 * This method is used to GTPU Hdr    *
 **************************************/
PRIVATE S16 egUUtilDumpGmHdr ARGS (( EgUMsgHdr egGmHdr ));

/*******************************************
 * This method is used to dump the IE List *
 *******************************************/
PRIVATE S16 egUUtilDumpList ARGS (( CmLListCp *ieList ));

/**************************************
 * This method is used to dump the IE *
 **************************************/
PRIVATE S16 egUUtilDumpIe ARGS (( EgUIe *egIe ));

#endif

/********************************************
 * This method is used to get the target IE *
 ********************************************/
PRIVATE S16 egUUtilGetTrgtIe ARGS (( CmLListCp  *ieList, U8 ieType, 
                                     U8 occrCnt, EgUIe **egIe ));

/******************************************
 * This method is used to duplicate an IE *
 ******************************************/
PRIVATE S16 egUUtilDupIe ARGS ((CmMemListCp *memCp, EgUIe *oldIe, EgUIe **dupIe ));


#ifdef EG_PRNT_DATA
/***************************************************
 Utility functions for dumping ipv4 / ipv6 address *
 ***************************************************/
PRIVATE S16 egUUtilDumpIpAddr ARGS ((Txt *buff, EgUIpAddr *ipAddr));
#endif

#endif /* End of EGTP_U */

#ifdef __cplusplus
} /* end of EXTERN "C" */
#endif /* end of __cplusplus */


#ifdef EGTP_C
/***********************************************************************************
*
*    Fun:    egUtilDupIpv4Addr
*
*    Desc:   Duplicates IPv4 address
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDupIpv4Addr
(
U8 *dupIp4Addr,    /* Destnation for duplication */
U8 *oldIp4Addr     /* Source fir duplication */
)
#else
PRIVATE S16 egUtilDupIpv4Addr(dupIp4Addr,oldIp4Addr)
U8 *dupIp4Addr;    /* Destnation for duplication */
U8 *oldIp4Addr;    /* Source fir duplication */
#endif
{
   U8 idx = 0;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupIpv4Addr)
 
   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!dupIp4Addr) || (!oldIp4Addr))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /***************************
   *  Duplicating IP address *
   ***************************/
   for (idx = 0; idx < 4; idx++)
   {
      dupIp4Addr[idx] = oldIp4Addr[idx];
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDupIpv6Addr
*
*    Desc:   Duplicates IPv6 address
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDupIpv6Addr
(
U8 *dupIp6Address,    /* Destnation for duplication */
U8 *oldIp6Address     /* Source fir duplication */
)
#else
PRIVATE S16 egUtilDupIpv6Addr(dupIp6Address,oldIp6Address)
U8 *dupIp6Address;    /* Destnation for duplication */
U8 *oldIp6Address;    /* Source fir duplication */
#endif
{
   U8 idx = 0;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupIpv6Addr)

   /*************************************************
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!dupIp6Address) || (!oldIp6Address))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /***************************
   *  Duplicating IP address *
   ***************************/
   for (idx = 0; idx < 16; idx++)
   {
      dupIp6Address[idx] = oldIp6Address[idx];
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDupSNwk
*
*    Desc:   Duplicates Serving Network
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDupSNwk
(
EgSNetwork *dupSNwk,    /* Destnation for duplication */
EgSNetwork oldSNwk      /* Source fir duplication */
)
#else
PRIVATE S16 egUtilDupSNwk(dupSNwk,oldSNwk)
EgSNetwork *dupSNwk;    /* Destnation for duplication */
EgSNetwork oldSNwk;    /* Source fir duplication */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupSNwk)

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!dupSNwk))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * Duplicating Serving Network *
   *******************************/
   dupSNwk->mccDigit1 = oldSNwk.mccDigit1;
   dupSNwk->mccDigit2 = oldSNwk.mccDigit2;
   dupSNwk->mccDigit3 = oldSNwk.mccDigit3;
   dupSNwk->mncDigit1 = oldSNwk.mncDigit1;
   dupSNwk->mncDigit2 = oldSNwk.mncDigit2;
   dupSNwk->mncDigit3 = oldSNwk.mncDigit3;

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}


/***********************************************************************************
*
*    Fun:    egUtilDupUliData
*
*    Desc:   Duplicates ULI Data
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDupUliData
(
EgIe          **dupIe,  /* Destnation for duplication */
EgIe          *oldIe    /* Source fir duplication */
)
#else
PRIVATE S16 egUtilDupUliData(dupIe,oldIe)
EgIe          **dupIe;  /* Destnation for duplication */
EgIe          *oldIe;    /* Source fir duplication */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupUliData)

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!dupIe) || (!oldIe))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   ( (EgIe *)(*dupIe))->t.uli.isCGIPres  = oldIe->t.uli.isCGIPres;
   ( (EgIe *)(*dupIe))->t.uli.isSAIPres  = oldIe->t.uli.isSAIPres;
   ( (EgIe *)(*dupIe))->t.uli.isRAIPres  = oldIe->t.uli.isRAIPres;
   ( (EgIe *)(*dupIe))->t.uli.isTAIPres  = oldIe->t.uli.isTAIPres;
   ( (EgIe *)(*dupIe))->t.uli.isECGIPres = oldIe->t.uli.isECGIPres; 
      /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   ( (EgIe *)(*dupIe))->t.uli.isLAIPres = oldIe->t.uli.isLAIPres; 
#endif

   if (oldIe->t.uli.isCGIPres)
   {
      egUtilDupSNwk(&(( (EgIe *)(*dupIe))->t.uli.cgi.sNwk), oldIe->t.uli.cgi.sNwk);

      ( (EgIe *)(*dupIe))->t.uli.cgi.lac = oldIe->t.uli.cgi.lac;   
      ( (EgIe *)(*dupIe))->t.uli.cgi.ci = oldIe->t.uli.cgi.ci;
   }

   if (oldIe->t.uli.isSAIPres)
   {
      egUtilDupSNwk(&(( (EgIe *)(*dupIe))->t.uli.sai.sNwk), oldIe->t.uli.sai.sNwk);

      ( (EgIe *)(*dupIe))->t.uli.sai.lac = oldIe->t.uli.sai.lac;   
      ( (EgIe *)(*dupIe))->t.uli.sai.sac = oldIe->t.uli.sai.sac;
   }

   if (oldIe->t.uli.isRAIPres)
   {
      egUtilDupSNwk(&(( (EgIe *)(*dupIe))->t.uli.rai.sNwk), oldIe->t.uli.rai.sNwk);

      ( (EgIe *)(*dupIe))->t.uli.rai.lac = oldIe->t.uli.rai.lac;
      ( (EgIe *)(*dupIe))->t.uli.rai.rac = oldIe->t.uli.rai.rac;
   }

   if (oldIe->t.uli.isTAIPres)
   {
      egUtilDupSNwk(&(( (EgIe *)(*dupIe))->t.uli.tai.sNwk), oldIe->t.uli.tai.sNwk);

      ( (EgIe *)(*dupIe))->t.uli.tai.tac  = oldIe->t.uli.tai.tac; 
   }

   if (oldIe->t.uli.isECGIPres)
   {
      egUtilDupSNwk(&(( (EgIe *)(*dupIe))->t.uli.ecgi.sNwk), oldIe->t.uli.ecgi.sNwk);

      ( (EgIe *)(*dupIe))->t.uli.ecgi.ecId  = oldIe->t.uli.ecgi.ecId;
   }
      /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
   if (oldIe->t.uli.isLAIPres)
   {
      egUtilDupSNwk(&(( (EgIe *)(*dupIe))->t.uli.lai.sNwk), oldIe->t.uli.lai.sNwk);

      ( (EgIe *)(*dupIe))->t.uli.lai.lac = oldIe->t.uli.lai.lac;   
   }
#endif

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDupPaa
*
*    Desc:   Duplicates PAA
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDupPaa
(
EgIe          **dupIe,  /* Destnation for duplication */
EgIe          *oldIe    /* Source fir duplication */
)
#else
PRIVATE S16 egUtilDupPaa(dupIe,oldIe)
EgIe          **dupIe;  /* Destnation for duplication */
EgIe          *oldIe;    /* Source fir duplication */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupPaa)

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!dupIe) || (!oldIe))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   ( (EgIe *)(*dupIe))->t.paa.pdnType =  oldIe->t.paa.pdnType; 
 
   switch(oldIe->t.paa.pdnType)
   {
      case 1: 
         {
            /* if the pdn type is 1, then the pdn address is of type Ipv4 */
            egUtilDupIpv4Addr(( (EgIe *)(*dupIe))->t.paa.u.pdnIpv4Addr, 
                  oldIe->t.paa.u.pdnIpv4Addr);
            break;
         }
      case 2:
         {
            /* If the pdnType is 2, then first octet will tell the pdn prifix */
            /* next 16octets will tell the ipv6 address */
            ( (EgIe *)(*dupIe))->t.paa.pdnPrefix = oldIe->t.paa.pdnPrefix; 
            egUtilDupIpv6Addr(( (EgIe *)(*dupIe))->t.paa.u.pdnIpv6Addr, 
                  oldIe->t.paa.u.pdnIpv6Addr);
            break;
         }
      case 3:
         {
            /* If the pdnType is 3, then first octet will tell the pdn prifix */
            /* next 16octets will be the ipv6 address */
            /* next 4 octets will be the ipv4 address */
            ( (EgIe *)(*dupIe))->t.paa.pdnPrefix = oldIe->t.paa.pdnPrefix; 
            egUtilDupIpv4Addr(( (EgIe *)(*dupIe))->t.paa.u.pdnBoth.pdnIpv4Addr, 
                  oldIe->t.paa.u.pdnBoth.pdnIpv4Addr);
            egUtilDupIpv6Addr(( (EgIe *)(*dupIe))->t.paa.u.pdnBoth.pdnIpv6Addr, 
                  oldIe->t.paa.u.pdnBoth.pdnIpv6Addr);
            break;
         }
      default:
      break;
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

#ifdef EG_PRNT_DATA
/***********************************************************************************
*
*    Fun:    egUtilDumpIpv4Addr
*
*    Desc:   Dump IPv4 address
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDumpIpv4Addr
(
Txt *buff,    /* To hold print buffer */
U8  *ip4Addr  /* Source for dumping */
)
#else
PRIVATE S16 egUtilDumpIpv4Addr(buff,ip4Addr)
Txt *buff;    /* To hold print buffer */
U8  *ip4Addr;    /* Source for dumping */
#endif
{
   U8 idx = 0;
   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpIpv4Addr)
 
   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff) || (!ip4Addr))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

   sprintf(tmpBuff,"ipv4Addr: ");
   EG_STRCAT(buff,tmpBuff);

  /***************************
   * Copy IP address to buff *
   ***************************/
   for (idx = 0; idx < 4; idx++)
   {
      if(idx < 3)
      {
         sprintf(tmpBuff,"%d.", ip4Addr[idx]);  
      }
      else
      {
         sprintf(tmpBuff,"%d \n", ip4Addr[idx]);  
      }
      EG_STRCAT(buff,tmpBuff);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDumpIpv6Addr
*
*    Desc:   Dump IPv6 address
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDumpIpv6Addr
(
Txt *buff,    /* To hold print buffer */
U8 *ip6Address     /* Source for dumping */
)
#else
PRIVATE S16 egUtilDumpIpv6Addr(buff,ip6Address)
Txt *buff;    /* To hold print buffer */
U8 *ip6Address;    /* Source for dumping */
#endif
{
   U8 idx = 0;
   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpIpv6Addr) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff) || (!ip6Address))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

   sprintf(tmpBuff,"ipv6Addr: ");
   EG_STRCAT(buff,tmpBuff);

  /***************************
   * Copy IP address to buff *
   ***************************/
   for (idx = 0; idx < 16; idx++)
   {
      sprintf(tmpBuff,"%d ", ip6Address[idx]);  
      EG_STRCAT(buff,tmpBuff);
   }

   EG_STRCAT(buff,"\n");
   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDumpSNwk
*
*    Desc:   Dump Serving Network
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDumpSNwk
(
Txt *buff,    /* To hold print buffer */
EgSNetwork sNwk      /* Source for dumping */
)
#else
PRIVATE S16 egUtilDumpSNwk(buff,sNwk)
Txt *buff;    /* To hold print buffer */
EgSNetwork sNwk;    /* Source for dumping */
#endif
{

   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpSNwk) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

  /****************************************
   * Copy Serving Network details to buff *
   ****************************************/
   sprintf(tmpBuff,"mccDigit1: %d\n", sNwk.mccDigit1);  
   EG_STRCAT(buff,tmpBuff);
   sprintf(tmpBuff,"mccDigit2: %d\n", sNwk.mccDigit2); 
   EG_STRCAT(buff,tmpBuff);
   sprintf(tmpBuff,"mccDigit3: %d\n", sNwk.mccDigit3);  
   EG_STRCAT(buff,tmpBuff);
   sprintf(tmpBuff,"mncDigit1: %d\n", sNwk.mncDigit1);  
   EG_STRCAT(buff,tmpBuff);
   sprintf(tmpBuff,"mncDigit2: %d\n", sNwk.mncDigit2);  
   EG_STRCAT(buff,tmpBuff);
   sprintf(tmpBuff,"mncDigit3: %d\n", sNwk.mncDigit3);  
   EG_STRCAT(buff,tmpBuff);

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}


/***********************************************************************************
*
*    Fun:    egUtilDumpAuthQt
*
*    Desc:   Dump Authentication Quintuplet
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDumpAuthQt
(
Txt *buff,              /* To hold print buffer */
EgAuthQt authQuin       /* Source for dumping */
)
#else
PRIVATE S16 egUtilDumpAuthQt(buff,authQuin)
Txt *buff;          /* To hold print buffer */
EgAuthQt authQuin;    /* Source for dumping */
#endif
{

   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */
   Txt tempBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpAuthQt) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
   cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

   cmMemcpy( (U8* )tempBuff, authQuin.rand.val, 
         authQuin.rand.length);
   sprintf(tmpBuff," rand.length: %d\n rand.val: %s", authQuin.rand.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   sprintf(tmpBuff,"xresLen: %d\n", authQuin.xresLen);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, authQuin.xres.val, 
         authQuin.xres.length);
   sprintf(tmpBuff," xres.length: %d\n xres.val: %s", authQuin.xres.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, authQuin.ck.val, 
         authQuin.ck.length);
   sprintf(tmpBuff," ck.length: %d\n ck.val: %s", authQuin.ck.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, authQuin.ik.val, 
         authQuin.ik.length);
   sprintf(tmpBuff," ik.length: %d\n ik.val: %s", authQuin.ik.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   sprintf(tmpBuff,"autnLen: %d\n", authQuin.autnLen);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, authQuin.autn.val, 
         authQuin.autn.length);
   sprintf(tmpBuff," autn.length: %d\n autn.val: %s", authQuin.autn.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDumpAuthQuadruplet
*
*    Desc:   Dump Authentication Quadruplets
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDumpAuthQuadruplet
(
Txt *buff,                      /* To hold print buffer */
EgAuthQuadruplet quadruplet       /* Source for dumping */
)
#else
PRIVATE S16 egUtilDumpAuthQuadruplet(buff,quadruplet)
Txt *buff;                      /* To hold print buffer */
EgAuthQuadruplet quadruplet;    /* Source for dumping */
#endif
{

   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */
   Txt tempBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpAuthQuadruplet) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
   cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

   cmMemcpy( (U8* )tempBuff, quadruplet.rand.val, 
         quadruplet.rand.length);
   sprintf(tmpBuff," rand.length: %d\n rand.val: %s", quadruplet.rand.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, quadruplet.xres.val, 
         quadruplet.xres.length);
   sprintf(tmpBuff," xres.length: %d\n xres.val: %s", quadruplet.xres.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, quadruplet.autn.val, 
         quadruplet.autn.length);
   sprintf(tmpBuff," autn.length: %d\n autn.val: %s", quadruplet.autn.length, 
         tempBuff);
   EG_STRCAT(buff,tmpBuff);

   cmMemcpy( (U8* )tempBuff, quadruplet.kasme.val, 
         quadruplet.kasme.length);
   sprintf(tmpBuff," kasme.length: %d\n kasme.val: %s", 
         quadruplet.kasme.length,tempBuff);
   EG_STRCAT(buff,tmpBuff);

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDumpIpv4Addr
*
*    Desc:   Dump IPv4 address
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDumpIpAddr
(
Txt *buff,    /* To hold print buffer */
EgIpAddr  *ipAddr  /* Source for dumping */
)
#else
PRIVATE S16 egUtilDumpIpAddr(buff,ipAddr)
Txt *buff;    /* To hold print buffer */
EgIpAddr  *ipAddr;    /* Source for dumping */
#endif
{
#ifdef EG_IPV6_SUPPORTED
   U8 idx = 0;
#endif
   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpIpAddr)

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff) || (!ipAddr))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

   if(ipAddr->type == EG_IP_V4_TYPE)
   {
      sprintf(tmpBuff,"  ipv4Addr:0x %lx \n",ipAddr->u.ipv4);
      EG_STRCAT(buff,tmpBuff);
   }
#ifdef EG_IPV6_SUPPORTED
   else if (ipAddr->type == EG_IP_V6_TYPE)
   {
      sprintf(tmpBuff,"  ipv6Addr: ");
      EG_STRCAT(buff,tmpBuff);

      /***************************
       * Copy IP address to buff *
       ***************************/
      for (idx = 0; idx < 16; idx++)
      {
         if(idx == 0)
         {
            sprintf(tmpBuff,"0x %x ", ipAddr->u.ipv6[idx]);  
         }
         else
         {
            sprintf(tmpBuff,"%x ", ipAddr->u.ipv6[idx]); 
         }
         EG_STRCAT(buff,tmpBuff);
      }

      EG_STRCAT(buff,"\n");
   }
#endif

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

#endif /* End of EG_PRNT_DATA*/  

/***********************************************************************************
*
*    Fun:    EgUtilAllocGmMsg
*
*    Desc:   Allocate memory for the GTP message and initialize
*
*    Ret:    ROK  - if memory allocation and initialization is success
*            EG_UTIL_ERR_MEM_ALLOC_FAILED - in case of memory allocation fails
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilAllocGmMsg
(
EgMsg **egMsg,    /* GTP Message */
Mem   *memInfo    /* memory information */
)
#else
PUBLIC S16 EgUtilAllocGmMsg(egMsg, memInfo)
EgMsg **egMsg;    /* GTP Message */
Mem   *memInfo;   /* memory information */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilAllocGmMsg) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*****************************************************
    * allocate memory for the gtp message of size EgMsg *
    *****************************************************/
   if (cmAllocEvnt(sizeof(EgMsg), EG_UTIL_MEM_SIZE, memInfo, (Ptr* )egMsg) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG022, ERRZERO,
               "EgUtilAllocGmMsg: Allocating memory for the GTP Message failed.\n");
      RETVALUE(EG_UTIL_ERR_MEM_ALLOC_FAILED);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    EgUtilDeAllocGmMsg
*
*    Desc:   De-Allocate memory for the GTP message and uninitialize
*
*    Ret:    ROK  - if memory de-allocation and initialization is success
*            RFAILED - in case of memory de-allocation fails
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*            Return value of the child methods (if any)
*
*    Notes:  The gtp message will be destoryed in this method.  The
*            memory allocated either for the message or for all the IEs
*            inside the message (of any level) will be deallocated inside
*            this method.
*
*    File:   eg_util.c
*
***********************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilDeAllocGmMsg
(
EgMsg **egMsg     /* GTP Message */
)
#else
PUBLIC S16 EgUtilDeAllocGmMsg(egMsg)
EgMsg **egMsg;    /* GTP Message */
#endif
{
   S16 retValue = ROK;                 /* to store the return values of sub methods */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilDeAllocGmMsg) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/

   /*****************************************
    * eg009.201 - Check for NULL ptr *
    *****************************************/
      if (!(*egMsg))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*************************************************
    * un allocate the memory for all IEs in message *
    *************************************************/
   if ( (retValue = egUtilDropIeList(&(*egMsg)->egIeList)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG023, ERRZERO,
               "EgUtilDeAllocGmMsg: De-allocating memory for the IE \
               list failed and continuing with dropping the message.\n");
   }

   /*****************************************
    * De-allocate memory of the gtp message *
    *****************************************/
   cmFreeMem(&(*egMsg)->memCp);

   /************************************
    * Initialize the pointer with zero *
    ************************************/
   *egMsg = NULLP;

   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    egUtilDropIeList
*
*    Desc:   This method will be used for deleting the IEs from the ielist.
*            This includes the nested (grouped) IEs also.
*
*    Ret:    ROK  - if IEs from the ielist are deleted successfully
*            RFAILED - in case of deleting the IEs from the IE list fails
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*            Return value of the child methods (if any)
*
*    Notes:  The memory for the individual IEs will not be freed here.
*            Here only the IEs inside the list will be deleted and the
*            memory will not be claimed back.  The memory claiming will
*            be done by the destructor of the message once every IE
*            inside the message is deleted.
*
*    File:   eg_util.c
*
***********************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilDropIeList
(
CmLListCp *ieList     /* IE list */
)
#else
PUBLIC S16 egUtilDropIeList(ieList)
CmLListCp *ieList;    /* IE list */
#endif
{
   EgIe   *ie = NULLP;                 /* temporary ie to hold the element in the list */ 
   CmLList *node = NULLP;              /* temporary node to walk through the list */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDropIeList) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !(ieList->first))
   {
      /* ieList can be zero */
      /*EGUTILLOGERROR(ERRCLS_DEBUG, EEG024, ERRZERO,
               "egUtilDropIeList: ieList is NULL or no element is list\n");*/
      RETVALUE(ROK);
   }

   /***************************************************
    * in a loop iterate through the list and if       *
    * the type is not of grouped type, then delete    *
    * the node and if it is a grouped type, then call *
    * the same method with the grouped IE list        *
    ***************************************************/
   for (ie = (EgIe* )CM_LLIST_FIRST_NODE(ieList, node);
        ie != NULLP;
        ie = (EgIe* )CM_LLIST_NEXT_NODE(ieList, node))
   {
      /*******************************************************
       * check the type of the IE.  If the type is of        *
       * grouped IE, then call the same method recursively   *
       * with the IE list inside the IE as arugment          *
       *******************************************************/
      if (ie->dataType == EG_GROUPED)
      {
        /********************************************* 
         * call the same method with grouped IE list *
         *********************************************/
         egUtilDropIeList(&ie->t.grouped.gIeList);
      }

      /*********************************
       * delete the node from the list *
       *********************************/
      if (!cmLListDelFrm(ieList, node))
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG024, ERRZERO,
               "egUtilDropIeList: Deleting node from the IE list failed.\n");
         RETVALUE(RFAILED);
      }
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

#ifdef EG_PHASE2
/***********************************************************************************
*
*    Fun:    EgUtilSetGtpHdr
*
*    Desc:   The header part of the gtp message will be set.
*
*    Ret:    ROK  - if header part of the message is set successfully
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  The given header will be over written with the given values.
*
*    File:   eg_util.c
*
***********************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilSetGtpHdr
(
EgMsg     *egMsg,    /* GTP message */
EgMsgHdr  egHdr      /* GTP message header */
)
#else
PUBLIC S16 EgUtilSetGtpHdr(egMsg, egHdr)
EgMsg     *egMsg;    /* GTP message */
EgMsgHdr  egHdr;     /* GTP message header */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilSetGtpHdr) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*******************************************************
    * set the header of the message with the given header *
    *******************************************************/
   cmMemcpy( (U8* )&egMsg->msgHdr, (U8* )&egHdr, sizeof(EgMsgHdr));

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
*
*    Fun:    EgUtilGetGtpHdr
*
*    Desc:   The header part of the gtp message will be returned.
*
*    Ret:    ROK  - if header part of the message is retreived successfully
*            EG_UTIL_ERR_INV_PARAM - if the given message or header are invalid
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
***********************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGetGtpHdr
(
EgMsg     *egMsg,    /* GTP message */
EgMsgHdr  *egHdr     /* GTP message header */
)
#else
PUBLIC S16 EgUtilGetGtpHdr(egMsg, egHdr)
EgMsg     *egMsg;    /* GTP message */
EgMsgHdr  *egHdr;    /* GTP message header */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGetGtpHdr) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !egHdr)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*****************************************************
    * set the header of the message in the given header *
    *****************************************************/
   *egHdr = egMsg->msgHdr;

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}
#endif

/****************************************************************************************
*
*    Fun:    EgUtilGmAppendIe
*
*    Desc:   Method is used to append an IE to the message at a particular
*            level
*
*    Ret:    ROK - if the IE is appended successfully
*            RFAILED - if appending IE fails
*            EG_UTIL_ERR_MEM_ALLOC_FAILED - if memory allocation for the new IE fails
*            EG_UTIL_ERR_INV_PARAM - if the given message or ieLvlInfo or ie are invalid
*            Return value of the child method (if any)
*
*    Notes:  This method will allocate memory separately for the new IE that is
*            being inserted at a particular level.
*
*    File:   eg_util.c
*
*****************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGmAppendIe
(
EgMsg         *egMsg,        /* GTP message */
EgIeLevelInfo *ieLvlInfo,    /* IE level information */
EgIe          *egIe          /* IE that has to be appended */
)
#else
PUBLIC S16 EgUtilGmAppendIe(egMsg, ieLvlInfo, egIe)
EgMsg         *egMsg;        /* GTP message */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
EgIe          *egIe;         /* IE that has to be appended */
#endif
{
   /***************************************************************
    * this will hold the IE list, where the IE has to be appended *
    ***************************************************************/
   CmLListCp *ieList = NULLP;

   S16 retValue = 0;                   /* to hold the return value of sub methods */       
   U16 pos = 0;                        /* to hold the temporary position value */
   U16 trgtLvl = 0;                    /* to hold the returned target level */
   CmLList *node = NULLP;              /* this will hold the IE node address */
   EgIe *ie = NULLP;                   /* ie that has to be appended to the list */
   EgIe *tmpIe = NULLP;                /* ie to iterate through the list */ 

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmAppendIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !ieLvlInfo || !egIe)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***************************************************
    * get the target IE list as per the given IE info *
    ***************************************************/
   switch ( (retValue = egUtilGetTrgtList(&egMsg->egIeList, ieLvlInfo, &ieList, EG_IE_LEVEL_ZERO, &trgtLvl)))
   {
      /************************************
       * in case of ok or invalid IE info *
       * we will take the ie list given   *
       * as the target list and continue  *
       ************************************/
      case ROK:
      case EG_UTIL_ERR_INV_IE_INFO:
      {
         break;
      }

      default:
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG025, ERRZERO,
               "EgUtilGmAppendIe: Getting the target list from the message failed.\n");
         RETVALUE(retValue);
      }
   }

   /***********************************************************
    * make the new copy of the given IE to append to the list *
    ***********************************************************/
   if ( (retValue = egUtilDupIe(&egMsg->memCp, egIe, &ie)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG026, ERRZERO,
               "EgUtilGmAppendIe: Making the new IE from the given IE failed.\n");
      RETVALUE(retValue);
   }

   /******************************************************
    * check if position has to be considered in the list *
    ******************************************************/
   if (ieLvlInfo->ieInfo[trgtLvl].posPres == TRUE)
   {
      /*****************************************************************
       * iterate through the list until the given position is occurred *
       *****************************************************************/
      for (tmpIe = (EgIe* )CM_LLIST_FIRST_NODE(ieList, node);
           (tmpIe != NULLP) && (pos++ < ieLvlInfo->ieInfo[trgtLvl].pos);
           tmpIe = (EgIe* )CM_LLIST_NEXT_NODE(ieList, node));

      /*****************************************************
       * insert the IE at the current position in the list *
       *****************************************************/
      cmLListInsCrnt(ieList, &ie->node);

     /******************
      * return success *
      ******************/
      RETVALUE(ROK);
   }

   /*****************************************
    * append the IE to the tail of the list *
    *****************************************/
   cmLListAdd2Tail(ieList, &ie->node);

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}


/***********************************************************************************
*
*    Fun:    egUtilGetTrgtList
*
*    Desc:   This is used to get the IE list at a particular level
*
*    Ret:    ROK - if the IE list at a particular level is retreived successfully
*            RFAILED -  if retreiving the IE list at a particular level fails
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*
*    Notes:  n/a
*
*    File:   eg_util.c
*
***********************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilGetTrgtList
(
CmLListCp      *ieList,       /* IE list from where to start the traversing */
EgIeLevelInfo *ieLvlInfo,    /* IE level information */
CmLListCp      **trgtList,     /* target IE list */
U16            strtLvl,        /* start the searhcing from this level */
U16            *trgtLvl        /* target IE list's level */
)
#else
PUBLIC S16 egUtilGetTrgtList(ieList, ieLvlInfo, trgtList, strtLvl, trgtLvl)
CmLListCp      *ieList;       /* IE list from where to start the traversing */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
CmLListCp      **trgtList;     /* target IE list */
U16            strtLvl;        /* start the searhcing from this level */
U16            *trgtLvl;       /* target IE list's level */
#endif
{

   EgIe *egTempIe = NULLP;             /* this is used to hold the temporary IE */
   U16 posOfIe = 0;                    /* used to hold the position of the IE */

   S16 retValue = 0;                   /* used to hold the return value of sub methods */
   U16            tmpStrtLvl = 0;        /* start the searhcing from this level */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilGetTrgtList) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !ieLvlInfo || !trgtList || !trgtLvl || 
       (ieLvlInfo->level > EG_MAX_IE_LEVEL) || (strtLvl > EG_MAX_IE_LEVEL))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*********************************************************   
    * initially the given ieList will be the target list,   *  
    * this will be helpful when the user gives the level    *
    * information with both the IE type and position        *
    * reference flags as false.  In such case there is      *
    * no way to progress in the list into the further       *
    * grouped lists                                         *
    *********************************************************/
   *trgtList = ieList;

   /****************************************************
    * copy the iterated IE level into the target level *
    ****************************************************/
   *trgtLvl = strtLvl;


   /****************************************************************
    * in a loop go through all the levels, and get the target list *
    ****************************************************************/
   while ((*trgtLvl < ieLvlInfo->level) && (strtLvl < EG_MAX_IE_LEVEL))
   {
      /**********************************************
       * get the target IE as per the given IE info *
       **********************************************/
      switch ( (retValue = egUtilGetTrgtIe(ieList, &ieLvlInfo->ieInfo[strtLvl], &egTempIe, &posOfIe)))
      {
         /************************************
          * in case of ok or invalid IE info *
          * we will take the ie list given   *
          * as the target list and continue  *
          ************************************/
         case ROK:
         {
            break;
         }

         /****************************************************
          * incase of EG_UTIL_ERR_INV_IE_INFO, check whether *  
          * there is one more level in the ieLvlInfo.        *
          * In such case, the given information is wrong,    *
          * so return invalid level info error.  If there    *
          * no further level, then return the same error     *
          ****************************************************/
         case EG_UTIL_ERR_INV_IE_INFO:
         {
            if (strtLvl + 1 >= ieLvlInfo->level)
            {
               RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
            }
            else
            {
               RETVALUE(EG_UTIL_ERR_INV_IE_INFO);
            }
         }

         default:
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG027, ERRZERO,
               "egUtilGetTrgtList: Getting the target node from the IE list failed.\n");
            RETVALUE(retValue);
         }
      }

      /******************************************************************  
       * if there is one more level than the current level              *
       * and the current target ie which has matched the criteria       *
       * of the given IE information is of type grouped, then it        *
       * means that we need to jump into the nested grouped IE.         *
       * So call the same method with IE list in the target IE,         *
       * start level as the strtLvl + 1.                                *
       * If there is one more level than the current level              *
       * and the current target IE is not an grouped IE, then return    *
       * EG_UTIL_ERR_INV_LEVEL_INFO as error                            *
       ******************************************************************/
      if ((strtLvl + 1 <= ieLvlInfo->level) && (egTempIe->dataType == EG_GROUPED))
      {
         /***************************************************
          * call the same method with the level higher than *
          * the current level                               *
          ***************************************************/
         tmpStrtLvl = strtLvl + 1;
         switch ( (retValue = 
                  egUtilGetTrgtList(&egTempIe->t.grouped.gIeList, ieLvlInfo, 
                                    trgtList, tmpStrtLvl, trgtLvl)))
         {
            case ROK:
            {
               break;
            }
            case EG_UTIL_ERR_INV_IE_INFO:
            {
               /* return the same error */
               RETVALUE(EG_UTIL_ERR_INV_IE_INFO);
            }

            default:
            {
               EGUTILLOGERROR(ERRCLS_DEBUG, EEG028, ERRZERO,
               "egUtilGetTrgtList: Getting the target node from the IE list failed.\n");
               RETVALUE(retValue);
            }
         }
      }
      else if (strtLvl + 1 <= ieLvlInfo->level && egTempIe->dataType != EG_GROUPED)
      {
         RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
      }

      /*****************************
       * increment the start level *
       *****************************/
      strtLvl++;
   }

   RETVALUE(ROK);
}


/********************************************************************************************
*
*    Fun:    egUtilGetTrgtIe
*
*    Desc:   This is used to get a particular IE from an IE list
*
*    Ret:    ROK - if the IE in IE is found successfully
*            RFAILED -  if finding the IE in IE list fails
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*            EG_UTIL_ERR_INV_PARAM - if the given ielist or ie info or target ie are invalid
*
*    Notes:  n/a
*
*    File:   eg_util.c
*
***********************************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilGetTrgtIe
(
CmLListCp     *ieList,       /* IE list from where to retreive the IE */
EgIeInfo      *ieInfo,       /* IE information */
EgIe          **egIe,        /* target IE */
U16           *iePos         /* position of target IE */
)
#else
PUBLIC S16 egUtilGetTrgtIe(ieList, ieInfo, egIe, iePos)
CmLListCp     *ieList;       /* IE list from where to retreive the IE */
EgIeInfo      *ieInfo;       /* IE information */
EgIe          **egIe;        /* target IE */
U16           *iePos;        /* position of target IE */
#endif
{
   U16 occrCnt = 0;                    /* used to hold the occurrence count */

   CmLList *node = NULLP;              /* used to hold the temporary node address */

#ifdef EG_PHASE2
   /***********************************************
    * this is used to hold the position of the IE *
    * this will be referred only when the target  *
    * IE has to be retreived using the position   *
    * information                                 *
    ***********************************************/
   U16 posOfIe = 0;

#endif

   /*-- eg003.201 : fixed warning due to misplaced macro --*/
   EG_TRC2(egUtilGetTrgtIe) 


   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !ieInfo || !egIe || !iePos)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /****************************************************
    * sanity check for the given ie information        *
    * Here if the IE type is present and occurrence    *
    * of IE has also to be taken in to count while     *
    * retreiving the IE, then the occurrence count     *
    * will be cross verified with the total IE list    *
    * count.  If occurrence count is greater than the  *
    * total IEs in the list, then return error.        *
    * This sort of sanity check is needed to avoid     *
    * invalid traversal in the list.                   *
    ****************************************************/
   if ( (ieInfo->ieTypePres == TRUE && ieInfo->occrPres == TRUE && ieInfo->occr > ieList->count)
        || (ieInfo->posPres == TRUE && ieInfo->pos > ieList->count))
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG029, ERRZERO,
               "egUtilGetTrgtIe: the given occr or position count is greater than the total ie count.\n");
      RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
   }

   /******************************************************
    * check whether the traversal has to be done based   *  
    * on the type.  If so check if occurrence count is   *
    * to be taken into consideration, then iterate in    *
    * list until the same IE type and occurrence count   *
    * is matched.  If the occurrence count is matching   *
    * then assign the IE to the OUT parameter and return *
    * success.  If the occurrence count is not matching  *
    * and the end of list is encountered, then return    *
    * EG_UTIL_ERR_INV_LEVEL_INFO error                   *
    ******************************************************/
   if (ieInfo->ieTypePres == TRUE)
   {
      /********************************
       * initialize the iePos to zero *
       ********************************/
      *iePos = 0;

      /********************************************************
       * if occurrence count is present, then iterate through *
       * the list until occurrence count matches              *
       ********************************************************/
      if (ieInfo->occrPres == TRUE)
      {
         /***************************************** 
          * iterate through the list and for each *
          * node increment the iePos value        *
          *****************************************/
         for (*egIe = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
              (*egIe != NULLP ) && (occrCnt < ieInfo->occr);
              *egIe = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node), (*iePos)++)
         {
            /**********************************************
             * if the IE type is matching, then increment *
             * the occrrence count                        *
             **********************************************/
            if ( ((*egIe)->ieType == ieInfo->ieType) && ( (*egIe)->ieInst == ieInfo->ieInst))
            {
               occrCnt++;
            }

            /**************************************************
             * if occurrence count is reached, break the loop *
             **************************************************/
            if (occrCnt == ieInfo->occr)
            {
               break;
            }
         }

         /*************************************************
          * if the occurrence count is matching with the  *
          * given value, then return success              *
          *************************************************/
         /* eg012.201 : Klocwork fix for validating the pointer */
         if ((occrCnt == ieInfo->occr) && (*egIe))
         {
            /***************************************************
             * check whether the value has to be referred.     *
             * If so, then compare the value of the IE         *
             * with the IE Info.  If both are same,            *
             * then return success.  Else return failure       *
             * NOTE:  This checking will be used while         *
             * deleting or finding only.  Usually while        *
             * inserting a node in a multi hierarchy level     *
             * (grouped IEs) the reference flag will be false. *
             ***************************************************/
#ifdef EG_PHASE2
            if (ieInfo->refValue == TRUE)
            {
               /********************************************
                * if the IE value is not matching with the *
                * given value, then return error           *
                ********************************************/
               if (egUtilCompareIeValue( (*egIe), ieInfo) != EG_UTIL_IE_VALUES_MATCHING)
               {
                  RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
               }
            }
#endif

            /********************************************
             * if the control comes here, it means that *
             * either the reference flag is false or    *
             * the value in IE Info is matching with    *
             * the IE value, so return success          *
             ********************************************/
            RETVALUE(ROK);
         }
         else
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG030, ERRZERO,
               "egUtilGetTrgtIe: the actual occr count is less than given occurrence count.\n");
            RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
         }
      }

      /************************************************
       * if the control comes here, it means that     *
       * the occurrence count need not be considered. *
       * so iterate through the list and if the IE    *
       * type in list matches with the given IE type, *
       * then return success.  For each node in       *
       * the list, increment the iePos counter.       *
       ************************************************/
      for (*egIe = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
           *egIe != NULLP;
           *egIe = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node), (*iePos)++)
      {
         /******************************************************
          * if the IE type is matching, then check whether     *
          * the value has to be referred.  If so compare the   *
          * values and if the values are matching, then        *
          * return success                                     *
          ******************************************************/
         if ( ((*egIe)->ieType == ieInfo->ieType) && ((*egIe)->ieInst == ieInfo->ieInst))
         {
            /***************************************************
             * check whether the value has to be referred.     *     
             * If so, then compare the value of the IE         *
             * with the IE Info.  If both are same,            *
             * then return success.  Else return failure       *
             * NOTE:  This checking will be used while         *
             * deleting or finding only.  Usually while        *
             * inserting a node in a multi hierarchy level     *
             * (grouped IEs) the reference flag will be false. *
             ***************************************************/
#ifdef EG_PHASE2
            if (ieInfo->refValue == TRUE)
            {
               /*********************************************
                * if the IE value is not matching with the  *
                * given value, then continue in loop to     *
                * find the next IE whith same type          *
                *********************************************/
               if (egUtilCompareIeValue( (*egIe), ieInfo) != EG_UTIL_IE_VALUES_MATCHING)
               {
                  continue;
               }
            }
#endif

            /***************************************
             * if the control comes here it means  *
             * whether the val reference flag is   *
             * false or either the value given in  *
             * IE Info is matching with the value  *
             * in the IE.  So return success       *
             ***************************************/
            RETVALUE(ROK);
         }
      }

      /************************************************
       * if the control comes here it means that the  *
       * IE was not in the list.  So return error     *
       ************************************************/
      RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
   }

   /************************************************************
    * check whether the traversal has to be done based         *
    * on the position.  If so iterate through the list         *
    * until the position given in IE Info is reached.          *
    * If any IE is present in the list at the given position,  *
    * then assign the IE to the OUT parameter and return       *
    * success.  If the value has to be referenced, then        *
    * compare the value at the occurred position.              *
    * If there is no IE at the given position or               *
    * if the value given as a referrence at that level and     *
    * at that position is not matching, then return            *
    * EG_UTIL_ERR_INV_LEVEL_INFO error                         *
    ************************************************************/
#ifdef EG_PHASE2
   else if (ieInfo->posPres == TRUE)
   {
      /*********************************************************
       * iterate through the list until the given position     *
       * is encountered or there are no members in the list    *
       ********************************************************/
      for (*egIe = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
           (*egIe != NULLP) && (posOfIe++ < ieInfo->pos);
           *egIe = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node));

      /******************************************************
       * if the IE is non null, then check whether          *
       * the value has to be referrenced at that position.  *
       * If so compare the values.                          *
       ******************************************************/
      if (*egIe)
      {
         /***************************************************
          * check whether the value has to be referred.     *
          * If so, then compare the value of the IE         *
          * with the IE Info.  If both are same,            *
          * then return success.  Else return failure       * 
          * NOTE:  This checking will be used while         *
          * deleting or finding only.  Usually while        *
          * inserting a node in a multi hierarchy level     *
          * (grouped IEs) the reference flag will be false. *
          ***************************************************/
         if (ieInfo->refValue == TRUE)
         {
            /*********************************************
             * if the IE value is not matching with the  *
             * given value, then return error.           *
             *********************************************/
            if (egUtilCompareIeValue( (*egIe), ieInfo) != EG_UTIL_IE_VALUES_MATCHING)
            {
               EGUTILLOGERROR(ERRCLS_DEBUG, EEG031, ERRZERO,
                        "egUtilGetTrgtIe: the IE value was not matching -- criteria is by position.\n");
               RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
            }
         }

         /***************************************
          * if the control comes here it means  *      
          * whether the val reference flag is   *
          * false or either the value given in  *
          * IE Info is matching with the value  *
          * in the IE.  So return success       *
          ***************************************/
         RETVALUE(ROK);
      }

      /************************************************
       * if the control comes here it means that the  *
       * IE was not in the list or is null.           *
       * So return error                              *
       ************************************************/
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG032, ERRZERO,
               "egUtilGetTrgtIe: the IE was not found in the list.\n");

      RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
   }
#endif
   /******************************************************************
    * if the control comes here, it means that the IE Info           *  
    * is referring to neither the IE type nor the position           *
    * in the list.  In such case return the last IE in the IE list   *
    * and error type as EG_UTIL_ERR_INV_IE_INFO                      *
    ******************************************************************/
   else
   {
      /***********************************
       * get the last node from the list *
       ***********************************/
      node = cmLListLast(ieList);

      if (node->node)
      {
         /*******************************************
          * take the node address as the IE address *
          *******************************************/
         *egIe = (EgIe* ) node->node;
         RETVALUE(ROK);
      }
      else
      {
         /************************************************
          * if the control comes here it means that the  *
          * IE was not in the list or is null.           *
          * So return error                              *
          ************************************************/
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG033, ERRZERO,
                  "egUtilGetTrgtIe: no IEs present in the list.\n");

         RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
      }
   }

}

#ifdef EG_PHASE2
/********************************************************************************************
*
*    Fun:    egUtilCompareIeValue
*
*    Desc:   Method is used to compare the IE value with the value given in IE information
*
*    Ret:    EG_UTIL_IE_VALUES_MATCHING - if the IE values are matching
*            EG_UTIL_IE_VALUES_NOT_MATCHING - if the IE values are not matching
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the IE type in IE and type in info are not same
*            EG_UTIL_ERR_INV_PARAM - if the given ie or ie info are invalid
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
**********************************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilCompareIeValue
(
EgIe          *egIe,         /* IE whose value has to be compared */
EgIeInfo      *ieInfo        /* ie information that has to be taken as reference */
)
#else
PUBLIC S16 egUtilCompareIeValue(egIe, ieInfo)
EgIe          *egIe;         /* IE whose value has to be compared */
EgIeInfo      *ieInfo;       /* ie information that has to be taken as reference */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilCompareIeValue) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egIe || !ieInfo)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***********************************************
    * if the types of the egIe and the ieInfo are *
    * not same then return error                  *
    ***********************************************/
   if (egIe->dataType != ieInfo->valDet.dataType)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG034, ERRZERO,
               "egUtilCompareIeValue: the egIe type and ieInfo type are not same.\n");
      RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
   }

   /****************************
    * check the type of the IE *
    ****************************/
   switch(egIe->dataType)
   {
      case EG_UNSIGNED_32:
      {
         if (egIe->t.valUnSgnd32 == ieInfo->valDet.t.valUnSgnd32)
         {
            RETVALUE(EG_UTIL_IE_VALUES_MATCHING);
         }

         break;
      }

      default:
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG035, ERRZERO,
               "egUtilCompareIeValu: Ie datatype is not valid.\n");
         RETVALUE(EG_UTIL_ERR_INV_IE_DATATYPE);
      }

   }

   /* return value not matching error */
   RETVALUE(EG_UTIL_IE_VALUES_NOT_MATCHING);
}

/***************************************************************************************
*
*    Fun:    EgUtilGmGetIe
*
*    Desc:   Method is used to get an IE from the message which is at a particular
*            level
*
*    Ret:    ROK - if the IE is retreived successfully
*            RFAILED - if retreiving IE fails
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*            EG_UTIL_ERR_INV_PARAM - if the given message or end-to-end id are invalid
*            Return value of the child method (if any)
*
*    Notes:  This method will return the reference of the IE which is
*            at a particular level.  So this will assume that the user of
*            this API will not use the IE after freeing the message
*
*    File:   eg_util.c
*
****************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGmGetIe
(
EgMsg         *egMsg,        /* GTP message */
EgIeLevelInfo *ieLvlInfo,    /* IE level information */
EgIe          **egIe,        /* pointer to hold the reference of IE */
U16           *iePos         /* position of IE */
)
#else
PUBLIC S16 EgUtilGmGetIe(egMsg, ieLvlInfo, egIe, iePos)
EgMsg         *egMsg;        /* GTP message */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
EgIe          **egIe;        /* pointer to hold the reference of IE */
U16           *iePos;        /* position of IE */
#endif
{
   CmLListCp *ieList = NULLP;          /* this will hold the IE list, 
                                          where the IE has to be appended */

   S16 retValue = 0;                   /* to hold the return value of sub methods */

   U16 trgtLvl = 0;                    /* to hold the returned target level */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmGetIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !ieLvlInfo || !egIe || ieLvlInfo->level > EG_MAX_IE_LEVEL)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }
   
   /***************************************************
    * get the target IE list as per the given IE info *
    ***************************************************/
   if ( (retValue = egUtilGetTrgtList(&egMsg->egIeList, ieLvlInfo, &ieList, EG_IE_LEVEL_ZERO, &trgtLvl)) != ROK)
   {
      RETVALUE(retValue);
   }

   /************************************************************
    * get the target IE from the list as per the given IE info *
    ************************************************************/
   if ( (retValue = egUtilGetTrgtIe(ieList, &ieLvlInfo->ieInfo[trgtLvl], egIe, iePos)) != ROK)
   {
      RETVALUE(retValue);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    EgUtilGmGetIeCnt
*
*    Desc:   Method is used to retreive the number of IEs at a particular
*            level
*
*    Ret:    ROK - if the IE count is retreived successfully
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*            EG_UTIL_ERR_INV_PARAM - if the given message or IE level information
*                                or ieCnt are invalid
*            Return value of the child method (if any)
*
*    Notes:  
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGmGetIeCnt
(
EgMsg         *egMsg,        /* GTP message */
EgIeLevelInfo *ieLvlInfo,    /* IE level information */
S16           *ieCnt         /* IE count at a level */
)
#else
PUBLIC S16 EgUtilGmGetIeCnt(egMsg, ieLvlInfo, ieCnt)
EgMsg         *egMsg;        /* GTP message */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
S16           *ieCnt;        /* IE count at a level */
#endif
{

   CmLListCp *ieList = NULLP;          /* this will hold the IE list,
                                          where the IE has to be appended */

   S16 retValue = 0;                   /* to hold the return value of sub methods */

   U16 trgtLvl = 0;                    /* to hold the returned target level */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmGetIeCnt) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !ieLvlInfo || !ieCnt || ieLvlInfo->level > EG_MAX_IE_LEVEL)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***************************************************
    * get the target IE list as per the given IE info *
    ***************************************************/
   if ( (retValue = egUtilGetTrgtList(&egMsg->egIeList, ieLvlInfo, &ieList, EG_IE_LEVEL_ZERO, &trgtLvl)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG036, ERRZERO,
               "EgUtilGmGetIeCnt: Getting the target list from the message failed.\n");
      RETVALUE(retValue);
   }

   /*********************************************
    * assign the count of IEs in the ie list to *
    * the given variable                        *
    *********************************************/
   *ieCnt = ieList->count;

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/*********************************************************************************************
*
*    Fun:    EgUtilGmDup
*
*    Desc:   Method is used to duplicate a gtp message
*
*    Ret:    ROK - if the gtp message is duplicated successfully
*            RFAILED - if the gtp message duplicating fails
*            EG_UTIL_ERR_MEM_ALLOC_FAILED - if memory allocation for the new gtp message fails
*            EG_UTIL_ERR_INV_PARAM - if the given message or memory information are invalid
*            Return value of the child method (if any)
*
*    Notes:  
*
*    File:   eg_util.c
*
**********************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGmDup
(
EgMsg      *egMsg,         /* GTP message */
EgMsg      **dupEgMsg,     /* duplicated gtp message */
Mem        *memInfo        /* memory information */
)
#else
PUBLIC S16 EgUtilGmDup(egMsg, dupEgMsg, memInfo)
EgMsg      *egMsg;         /* GTP message */
EgMsg      **dupEgMsg;     /* duplicated gtp message */
Mem        *memInfo;       /* memory information */
#endif
{
   S16 retValue = 0;                   /* to hold the return value of sub methods */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmDup) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !dupEgMsg || !memInfo)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*********************************************************
    * allocate memory for the new gtp message of size EgMsg *
    *********************************************************/
   if ( (retValue = EgUtilAllocGmMsg(dupEgMsg, memInfo)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG037, ERRZERO,
               "EgUtilGmDup: Allocating memory for the GTP Message failed.\n");
      RETVALUE(EG_UTIL_ERR_MEM_ALLOC_FAILED);
   }

   /************************************************************
    * copy the header part of given message to new gtp message *
    ************************************************************/
   cmMemcpy( (U8* )&(*dupEgMsg)->msgHdr, (U8* )&egMsg->msgHdr, sizeof(EgMsgHdr));

   /*************************************************************
    * duplicate the gtp message by duplicating the main IE list *
    *************************************************************/
   if ( (retValue = egUtilDupList(&egMsg->egIeList, &(*dupEgMsg)->egIeList, &(*dupEgMsg)->memCp)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG038, ERRZERO,
               "EgUtilGmDup: Duplicating the gtp list from the original message failed.\n");
      RETVALUE(retValue);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/*********************************************************************************************
*
*    Fun:    egUtilDupList
*
*    Desc:   Method is used to duplicate an ie list
*
*    Ret:    ROK - if the ie list is duplicated successfully
*            RFAILED - if the ie list duplicating fails
*            EG_UTIL_ERR_MEM_ALLOC_FAILED - if memory allocation for the new gtp message fails
*            EG_UTIL_ERR_INV_PARAM - if the given message or memory information are invalid
*            Return value of the child method (if any)
*
*    Notes:  
*
*    File:   eg_util.c
*
**********************************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilDupList
(
CmLListCp      *ieList,         /* original ie list */
CmLListCp      *dupIeList,      /* duplicated ie list */
CmMemListCp    *memCp            /* memory information */
)
#else
PUBLIC S16 egUtilDupList(ieList, dupIeList, memCp)
CmLListCp      *ieList;         /* original ie list */
CmLListCp      *dupIeList;      /* duplicated ie list */
CmMemListCp    *memCp;           /* memory information */
#endif
{
   /*********************************************************
    * this will hold the address of IE in the original list *
    *********************************************************/
   EgIe *egIe = NULLP;                 

   /**********************************************************
    * this will hold the address of IE that is newly created *
    **********************************************************/
   EgIe *egDupIe = NULLP;

   S16 retValue = 0;                   /* to hold the return value of sub methods */

   CmLList *node = NULLP;              /* to hold the temporary node address */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupList) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !dupIeList || !memCp)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /************************************************
    * iterate through the list and for each ie,    *
    * allocate a local ie and append it to the     *
    * duplicate list.  If the ie is a grouped      *
    * ie, call the same method with the created    *
    * ie's grouped ie list                         *
    ************************************************/
   for (egIe = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
        egIe != NULLP;
        egIe = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node))
   {
      /********************   
       * duplicate the IE *
       ********************/
      if ( (retValue = egUtilDupIe(memCp, egIe, &egDupIe)) != ROK)
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG039, ERRZERO,
               "egUtilDupList: Duplicating the IE from the original IE failed.\n");
         RETVALUE(retValue);
      }

      /****************************************************
       * append the duplicated IE to the tail of the list *
       ****************************************************/
      cmLListAdd2Tail(dupIeList, &egDupIe->node);

      /************************************************************
       * if the IE type is grouped, then call the same method     *
       * with the IE list in the original IE and the duplicate    *
       * IE, memory information as arguments                      *
       ************************************************************/
      if (egIe->dataType  == EG_GROUPED)
      {
         if ( (retValue = egUtilDupList(&egIe->t.grouped.gIeList, &egDupIe->t.grouped.gIeList, memCp)) != ROK)
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG040, ERRZERO,
               "egUtilDupList: Duplicating the grouped IE list from the original IE list failed.\n");
            RETVALUE(retValue);
         }
      }
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

#endif

/*******************************************************************************************
*
*    Fun:    egUtilDupIe
*
*    Desc:   Method is used to duplicate an ie list
*
*    Ret:    ROK - if the ie list is duplicated successfully
*            EG_UTIL_ERR_MEM_ALLOC_FAILED - if memory allocation for the new ie fails
*            EG_UTIL_ERR_INV_PARAM - if the given message or memory information are invalid
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
*******************************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilDupIe
(
CmMemListCp   *memCp,          /* memory information */
EgIe          *oldIe,          /* original ie list */
EgIe          **dupIe          /* duplicated ie list */
)
#else
PUBLIC S16 egUtilDupIe(memCp, oldIe, dupIe)
CmMemListCp   *memCp;          /* memory information */
EgIe          *oldIe;          /* original ie list */
EgIe          **dupIe;         /* duplicated ie list */
#endif
{
   PTR length = 0;                     /* to store the length of the IE */
   S16 idx; /* ccpu00137353  */
#ifdef EG_REL_AC0
   U32 count = 0;
#endif
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDupIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!oldIe || !dupIe || !memCp)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*************************************************
    * take the normal size of the IE as the length  *
    * of the new IE                                 *
    *************************************************/
   length = sizeof(EgIe);

   /**********************************************************
    * allocate memory for the ie and then the local ie will  *
    * be appended to the list                                *
    **********************************************************/
   if (cmGetMem(memCp, length, (Ptr* )dupIe) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG041, ERRZERO,
               "egUtilDupIe: Allocating the memory for the IE failed.\n");
      RETVALUE(EG_UTIL_ERR_MEM_ALLOC_FAILED);
   }

   /*************************************
    * Initialize the elements of new ie *
    *************************************/
   cmMemset( (U8* )(*dupIe), 0, length);

   ( (EgIe *)(*dupIe))->ieType = oldIe->ieType;
   ( (EgIe *)(*dupIe))->ieInst = oldIe->ieInst;
   ( (EgIe *)(*dupIe))->dataType = oldIe->dataType;

   switch (oldIe->dataType)
   {
      case EG_UNSIGNED_8:
      {
         ( (EgIe *)(*dupIe))->t.valUnSgnd8 = oldIe->t.valUnSgnd8;
         break;
      }

      case EG_UNSIGNED_16:
      {
         ( (EgIe *)(*dupIe))->t.valUnSgnd16 = oldIe->t.valUnSgnd16;
         break;
      }

      case EG_UNSIGNED_32:
      {
         ( (EgIe *)(*dupIe))->t.valUnSgnd32 = oldIe->t.valUnSgnd32;
         break;
      }
      
      case EG_STRING_8:
      {
         ( (EgIe *)(*dupIe))->t.valStr8.length = oldIe->t.valStr8.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr8.val, oldIe->t.valStr8.val, 
         oldIe->t.valStr8.length);
         break;
      }

      case EG_STRING_16:
      {
         ( (EgIe *)(*dupIe))->t.valStr16.length = oldIe->t.valStr16.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr16.val, oldIe->t.valStr16.val, 
                     oldIe->t.valStr16.length);
         break;
      }

      case EG_STRING_32:
      {
         ( (EgIe *)(*dupIe))->t.valStr32.length = oldIe->t.valStr32.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr32.val, oldIe->t.valStr32.val, 
                     oldIe->t.valStr32.length);
         break;
      }

      case EG_STRING_132:
      {
         ( (EgIe *)(*dupIe))->t.valStr132.length = oldIe->t.valStr132.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr132.val, oldIe->t.valStr132.val, 
                     oldIe->t.valStr132.length);
         break;
      }

      case EG_STRING_MAX:
      {
         ( (EgIe *)(*dupIe))->t.valStr.length = oldIe->t.valStr.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr.val, oldIe->t.valStr.val, 
                     oldIe->t.valStr.length);
         break;
      }

      case EG_SNETWORK:
      {
         egUtilDupSNwk(&(((EgIe *)(*dupIe))->t.sNwk), oldIe->t.sNwk);
         break;
      }

      case EG_INDICATION:
      {
         ( (EgIe *)(*dupIe))->t.indication.isSGWCIPres = oldIe->t.indication.isSGWCIPres;
         ( (EgIe *)(*dupIe))->t.indication.isISRAIPres = oldIe->t.indication.isISRAIPres;
         ( (EgIe *)(*dupIe))->t.indication.isISRSIPres = oldIe->t.indication.isISRSIPres;
         ( (EgIe *)(*dupIe))->t.indication.isOIPres    = oldIe->t.indication.isOIPres;
         ( (EgIe *)(*dupIe))->t.indication.isDFIPres   = oldIe->t.indication.isDFIPres;
         ( (EgIe *)(*dupIe))->t.indication.isHIPres = oldIe->t.indication.isHIPres;
         ( (EgIe *)(*dupIe))->t.indication.isDTFPres = oldIe->t.indication.isDTFPres;
         ( (EgIe *)(*dupIe))->t.indication.isDAFPres = oldIe->t.indication.isDAFPres;
         ( (EgIe *)(*dupIe))->t.indication.isMSVPres = oldIe->t.indication.isMSVPres;
         ( (EgIe *)(*dupIe))->t.indication.isSIPres =  oldIe->t.indication.isSIPres;
         ( (EgIe *)(*dupIe))->t.indication.isPTPres = oldIe->t.indication.isPTPres;
         ( (EgIe *)(*dupIe))->t.indication.isPPres = oldIe->t.indication.isPPres;
         ( (EgIe *)(*dupIe))->t.indication.isCRSIPres = oldIe->t.indication.isCRSIPres;
         ( (EgIe *)(*dupIe))->t.indication.isCFSIPres = oldIe->t.indication.isCFSIPres;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         ( (EgIe *)(*dupIe))->t.indication.isUIMSIPres= oldIe->t.indication.isUIMSIPres;
#endif
#ifdef EG_REL_AC0
         ( (EgIe *)(*dupIe))->t.indication.isSQCIPres = oldIe->t.indication.isSQCIPres;
         ( (EgIe *)(*dupIe))->t.indication.isCCRSIPres = oldIe->t.indication.isCCRSIPres;
         ( (EgIe *)(*dupIe))->t.indication.isISRAUPres = oldIe->t.indication.isISRAUPres;
         ( (EgIe *)(*dupIe))->t.indication.isMBMDTPres = oldIe->t.indication.isMBMDTPres;
         ( (EgIe *)(*dupIe))->t.indication.isS4AFPres = oldIe->t.indication.isS4AFPres;
         ( (EgIe *)(*dupIe))->t.indication.isS6AFPres = oldIe->t.indication.isS6AFPres;
#endif
         break;
      }

      case EG_PVT_EXTN:
      {
         ( (EgIe *)(*dupIe))->t.pvtExt.entId = oldIe->t.pvtExt.entId;
         ( (EgIe *)(*dupIe))->t.pvtExt.propVal.length = oldIe->t.pvtExt.propVal.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.pvtExt.propVal.val, 
                    oldIe->t.pvtExt.propVal.val, oldIe->t.pvtExt.propVal.length);
         break;
      }

      case EG_CAUSE:
      {
         ( (EgIe *)(*dupIe))->t.cause.causeVal = oldIe->t.cause.causeVal;
         ( (EgIe *)(*dupIe))->t.cause.ieCS = oldIe->t.cause.ieCS;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         ( (EgIe *)(*dupIe))->t.cause.pce = oldIe->t.cause.pce;
         ( (EgIe *)(*dupIe))->t.cause.bce = oldIe->t.cause.bce;
#endif
         ( (EgIe *)(*dupIe))->t.cause.ieType = oldIe->t.cause.ieType;
         ( (EgIe *)(*dupIe))->t.cause.ieLength= oldIe->t.cause.ieLength;
         ( (EgIe *)(*dupIe))->t.cause.ieInst = oldIe->t.cause.ieInst;
         break;
      }

      case EG_TRC_INFO:
      {
         egUtilDupSNwk(&(((EgIe *)(*dupIe))->t.trcInfo.sNwk), oldIe->t.trcInfo.sNwk);
         ( (EgIe *)(*dupIe))->t.trcInfo.trcId = oldIe->t.trcInfo.trcId;
         cmMemcpy(( (EgIe *)(*dupIe))->t.trcInfo.trigEvnts,
                    oldIe->t.trcInfo.trigEvnts,9);
         ( (EgIe *)(*dupIe))->t.trcInfo.lstNETypes = oldIe->t.trcInfo.lstNETypes;
         ( (EgIe *)(*dupIe))->t.trcInfo.sesnTrcDep = oldIe->t.trcInfo.sesnTrcDep;
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.trcInfo.trcColEntAddr),
                     (U8 *)& oldIe->t.trcInfo.trcColEntAddr, sizeof(EgIpAddr));
         cmMemcpy(( (EgIe *)(*dupIe))->t.trcInfo.lstOfIntfs,
                    oldIe->t.trcInfo.lstOfIntfs,EGT_MAX_INTF);
         break; 
      }

      case EG_BEARER_FLAG:
      {
         ( (EgIe *)(*dupIe))->t.bearFlags.isPPCPres = oldIe->t.bearFlags.isPPCPres;
         ( (EgIe *)(*dupIe))->t.bearFlags.isVBPres = oldIe->t.bearFlags.isVBPres;
#ifdef EG_REL_AC0
         ( (EgIe *)(*dupIe))->t.bearFlags.isASIPres = oldIe->t.bearFlags.isASIPres;
#endif
         break; 
      }

      case EG_FTEID:
      {
         ( (EgIe *)(*dupIe))->t.fTeid.isIPv4Pres = oldIe->t.fTeid.isIPv4Pres;
         ( (EgIe *)(*dupIe))->t.fTeid.isIPv6Pres = oldIe->t.fTeid.isIPv6Pres;
         ( (EgIe *)(*dupIe))->t.fTeid.intfType  = oldIe->t.fTeid.intfType;
         ( (EgIe *)(*dupIe))->t.fTeid.teid   = oldIe->t.fTeid.teid;     

         if(oldIe->t.fTeid.isIPv4Pres)
         {
            egUtilDupIpv4Addr(( (EgIe *)(*dupIe))->t.fTeid.ip4Addr, 
                                 oldIe->t.fTeid.ip4Addr);
         }
         /* ccpu00137940 */
         if(oldIe->t.fTeid.isIPv6Pres)
         {
            egUtilDupIpv6Addr(( (EgIe *)(*dupIe))->t.fTeid.ipv6Address, 
                                 oldIe->t.fTeid.ipv6Address);
         }
         break; 
      }

      case EG_ULI_DATA:
      {
         egUtilDupUliData(dupIe,oldIe);
         break; 
      }

      case EG_FQOS:
      {
         ( (EgIe *)(*dupIe))->t.fQos.qci = oldIe->t.fQos.qci;

         ( (EgIe *)(*dupIe))->t.fQos.upMBR.length = oldIe->t.fQos.upMBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.fQos.upMBR.val, oldIe->t.fQos.upMBR.val, 
                     oldIe->t.fQos.upMBR.length);
         ( (EgIe *)(*dupIe))->t.fQos.dnMBR.length = oldIe->t.fQos.dnMBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.fQos.dnMBR.val, oldIe->t.fQos.dnMBR.val, 
                     oldIe->t.fQos.dnMBR.length);
         ( (EgIe *)(*dupIe))->t.fQos.upGBR.length = oldIe->t.fQos.upGBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.fQos.upGBR.val, oldIe->t.fQos.upGBR.val, 
                     oldIe->t.fQos.upGBR.length);
         ( (EgIe *)(*dupIe))->t.fQos.dnGBR.length = oldIe->t.fQos.dnGBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.fQos.dnGBR.val, oldIe->t.fQos.dnGBR.val, 
                     oldIe->t.fQos.dnGBR.length);
         break; 
      }

      case EG_BQOS_IEDATA:
      {
         ( (EgIe *)(*dupIe))->t.bQos.isPVIPres = oldIe->t.bQos.isPVIPres;
         ( (EgIe *)(*dupIe))->t.bQos.isPCIPres = oldIe->t.bQos.isPCIPres;
         ( (EgIe *)(*dupIe))->t.bQos.prityLvl = oldIe->t.bQos.prityLvl;
         ( (EgIe *)(*dupIe))->t.bQos.qci = oldIe->t.bQos.qci;

         ( (EgIe *)(*dupIe))->t.bQos.upMBR.length = oldIe->t.bQos.upMBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.bQos.upMBR.val, 
                     oldIe->t.bQos.upMBR.val, oldIe->t.bQos.upMBR.length);
         ( (EgIe *)(*dupIe))->t.bQos.dnMBR.length = oldIe->t.bQos.dnMBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.bQos.dnMBR.val, 
                     oldIe->t.bQos.dnMBR.val, oldIe->t.bQos.dnMBR.length);
         ( (EgIe *)(*dupIe))->t.bQos.upGBR.length = oldIe->t.bQos.upGBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.bQos.upGBR.val, 
                     oldIe->t.bQos.upGBR.val, oldIe->t.bQos.upGBR.length);
         ( (EgIe *)(*dupIe))->t.bQos.dnGBR.length = oldIe->t.bQos.dnGBR.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.bQos.dnGBR.val, 
                     oldIe->t.bQos.dnGBR.val, oldIe->t.bQos.dnGBR.length);
         break; 
      }

      case EG_PAA_IEDATA:
      {
         egUtilDupPaa(dupIe,oldIe);
         break;
      }

      case EG_GROUPED:
      {
         break;
      }

      case EG_PDN_CONN_SET_ID:
      {
         ( (EgIe *)(*dupIe))->t.csid.nodeIdType = oldIe->t.csid.nodeIdType;
         ( (EgIe *)(*dupIe))->t.csid.noOfCSIDs = oldIe->t.csid.noOfCSIDs;
         ( (EgIe *)(*dupIe))->t.csid.nodeId.length = oldIe->t.csid.nodeId.length;
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.csid.nodeId.val), 
                     (U8 *)& oldIe->t.csid.nodeId.val, oldIe->t.csid.nodeId.length);
         /* fix for ccpu00137353 */
         for(idx=0; idx < oldIe->t.csid.noOfCSIDs; idx++)
         {   
            ( (EgIe *)(*dupIe))->t.csid.csid[idx].length = oldIe->t.csid.csid[idx].length;
            cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.csid.csid[idx].val), 
                        (U8 *)& oldIe->t.csid.csid[idx].val, oldIe->t.csid.csid[idx].length);
         }
         break;
      }

      case EG_GLOBAL_CN_ID:
      {
         egUtilDupSNwk(&(((EgIe *)(*dupIe))->t.gblCnId.sNwk), oldIe->t.gblCnId.sNwk);
         ( (EgIe *)(*dupIe))->t.gblCnId.cnId.length = oldIe->t.gblCnId.cnId.length;
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.gblCnId.cnId.val), 
                     (U8 *)& oldIe->t.gblCnId.cnId.val, oldIe->t.gblCnId.cnId.length);
         break;
      }

      case EG_S103_PDN_FW_INFO:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.s103PDF), 
                     (U8 *)& oldIe->t.s103PDF, sizeof(EgS103PDF));
         break;
      }

      case EG_S1U_DATA_FW:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.s1UDF), 
                     (U8 *)& oldIe->t.s1UDF, sizeof(EgS1UDF));
         break;
      }

      case EG_GSM_KEY_TRIP:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.gsmKeyT), 
                     (U8 *)& oldIe->t.gsmKeyT, sizeof(EgGSMKeyT));
         break;
      }

      case EG_UMTS_KEY_CIPH_QUIN:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.umtsKeyUCQt), 
                     (U8 *)& oldIe->t.umtsKeyUCQt, sizeof(EgUMTSKeyUCQt));
         break;
      }

      case EG_GSM_KEY_CIPH_QUIN:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.gsmKeyUCQt), 
                     (U8 *)& oldIe->t.gsmKeyUCQt, sizeof(EgGSMKeyUCQt));
         break;
      }

      case EG_UMTS_KEY_QUIN:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.umtsKeyQt), 
                     (U8 *)& oldIe->t.umtsKeyQt, sizeof(EgUMTSKeyQt));
         break;
      }

      case EG_EPS_SEC_QUAD_QUIN:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.eSecCxtQdQt), 
                     (U8 *)& oldIe->t.eSecCxtQdQt, sizeof(EgESecCxtQdQt));
         break;

      }

      case EG_UMTS_KEY_QUAD_QUIN:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.umtsKeyQdQt), 
                     (U8 *)& oldIe->t.umtsKeyQdQt, sizeof(EgUMTSKeyQdQt));
         break;
      }

      case EG_AUTH_QUIN:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.authQt), 
                     (U8 *)& oldIe->t.authQt, sizeof(EgAuthQt));
         break;
      }

      case EG_AUTH_QUAD:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.quadruplet), 
                     (U8 *)& oldIe->t.quadruplet, sizeof(EgAuthQuadruplet));
         break;
      }

      case EG_CMP_REQ:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.crm), 
                     (U8 *)& oldIe->t.crm, sizeof(EgCrm));
         break;
      }

      case EG_GUTI:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.guti), 
                     (U8 *)& oldIe->t.guti, sizeof(EgGuti));
         break;
      }

      case EG_PDUNUM:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.pduNum), 
                     (U8 *)& oldIe->t.pduNum, sizeof(EgPduNum));
         break;
      }

      case EG_F_CONT:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.fContainer), 
                     (U8 *)& oldIe->t.fContainer, sizeof(EgFContainer));
         break;
      }

      case EG_F_CAUSE:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.fCause), 
                     (U8 *)& oldIe->t.fCause, sizeof(EgFCause));
         break;
      }

      case EG_TRGTID:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.targetId), 
                     (U8 *)& oldIe->t.targetId, sizeof(EgTargetId));
         break;
      }

      case EG_PKTFLOWID:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.pktFlowId), 
                     (U8 *)& oldIe->t.pktFlowId, sizeof(EgPktFlowId));
         break;
      }

      case EG_SRCID:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.srcId), 
                     (U8 *)& oldIe->t.srcId, sizeof(EgSrcId));
         break;
      }

      case EG_IP_ADDR:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.ipAddr), 
                     (U8 *)& oldIe->t.ipAddr, sizeof(EgIpAddr));
         break;
      }

      case EG_AMBR:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.ambr), 
                     (U8 *)& oldIe->t.ambr, sizeof(EgAMBR));
         break;
      }

      case EG_UE_TIME_ZONE:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.timeZone), 
                     (U8 *)& oldIe->t.timeZone, sizeof(EgTimeZone));
         break;
      }

      case EG_TRACE_REF:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.trcRef), 
                     (U8 *)& oldIe->t.trcRef, sizeof(EgTraceRef));
         break;
      }

      case EG_RAB_CONTEXT:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.rabCntxt), 
                     (U8 *)& oldIe->t.rabCntxt, sizeof(EgRABCntxt));
         break;
      }
#if defined(EG_REL_930) || defined(EG_REL_AC0)
      case EG_MBMS_IPMCAST_DIST:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.mbmsIPMCastDist), 
                     (U8 *)& oldIe->t.mbmsIPMCastDist, sizeof(EgMBMSIPMCastDist));
         break;
      }

      case EG_UCI:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.uci), 
                     (U8 *)& oldIe->t.uci, sizeof(EgUCI));
         break;
      }

      case EG_CSG_INFO_REP_ACT:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.csgInfoReqAct), 
                     (U8 *)& oldIe->t.csgInfoReqAct, sizeof(EgCSGInfoRepAct));
         break;
      }
#endif /* EG_REL_930 */

      case EG_BCD:
      {

         ( (EgIe *)(*dupIe))->t.valStr32.length = oldIe->t.valStr32.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr32.val, oldIe->t.valStr32.val, 
                     oldIe->t.valStr32.length); /*fix for ccpu00135094 */
         break;
      }

#ifdef EG_REL_AC0
      case EG_STRING_400:
      {
         ( (EgIe *)(*dupIe))->t.valStr400.length = oldIe->t.valStr400.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.valStr400.val, oldIe->t.valStr400.val, 
                     oldIe->t.valStr400.length);
         break;
      }
      case EG_ARP:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.arp), 
                     (U8 *)& oldIe->t.arp, sizeof(EgARP));
         break;
      }
      case EG_EPC_TMR:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.epcTmr), 
                     (U8 *)& oldIe->t.epcTmr, sizeof(EgEPCTmr));
         break;
      }
      case EG_THROTTLING:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.egThrot), 
                     (U8 *)& oldIe->t.egThrot, sizeof(EgThrottling));
         break;
      }

      case EG_MDT_CONF:
      {
         ( (EgIe *)(*dupIe))->t.mdtConf.jobType = oldIe->t.mdtConf.jobType;
         ( (EgIe *)(*dupIe))->t.mdtConf.rptTrigger = oldIe->t.mdtConf.rptTrigger;
         ( (EgIe *)(*dupIe))->t.mdtConf.rptInterval = oldIe->t.mdtConf.rptInterval;
         ( (EgIe *)(*dupIe))->t.mdtConf.rptAmount = oldIe->t.mdtConf.rptAmount;
         ( (EgIe *)(*dupIe))->t.mdtConf.evtThrRsrp = oldIe->t.mdtConf.evtThrRsrp;
         ( (EgIe *)(*dupIe))->t.mdtConf.evtThrRsrq = oldIe->t.mdtConf.evtThrRsrq;

         /* To-Do Mavnir Remove the hard coding */
         cmMemcpy((U8*)( (EgIe *)(*dupIe))->t.mdtConf.listOfMsmt, 
                     (U8*)oldIe->t.mdtConf.listOfMsmt, 4);

         ( (EgIe *)(*dupIe))->t.mdtConf.areaScope.length = oldIe->t.mdtConf.areaScope.length;
         cmMemcpy(( (EgIe *)(*dupIe))->t.mdtConf.areaScope.val, 
                     oldIe->t.mdtConf.areaScope.val, oldIe->t.mdtConf.areaScope.length);
         break;
      }
      case EG_CNG_TO_RPT_FLG:
      {
         cmMemcpy( (U8* )&(((EgIe *)(*dupIe))->t.cngRptFlg), 
                     (U8 *)& oldIe->t.cngRptFlg, sizeof(EgCngToRptFlg));
         break;
      }
      case EG_ADD_MM_CNTXT_SRVCC:
      {
         ( (EgIe *)(*dupIe))->t.mmCntxtForSrvcc.classMk2Len = oldIe->t.mmCntxtForSrvcc.classMk2Len;
         cmMemcpy(( (EgIe *)(*dupIe))->t.mmCntxtForSrvcc.classMk2, 
                     oldIe->t.mmCntxtForSrvcc.classMk2, oldIe->t.mmCntxtForSrvcc.classMk2Len);

         ( (EgIe *)(*dupIe))->t.mmCntxtForSrvcc.classMk3Len = oldIe->t.mmCntxtForSrvcc.classMk3Len;
         cmMemcpy(( (EgIe *)(*dupIe))->t.mmCntxtForSrvcc.classMk3, 
                     oldIe->t.mmCntxtForSrvcc.classMk3, oldIe->t.mmCntxtForSrvcc.classMk3Len);

         ( (EgIe *)(*dupIe))->t.mmCntxtForSrvcc.numOfcodecList = oldIe->t.mmCntxtForSrvcc.numOfcodecList;
         for(count = 0; count <  oldIe->t.mmCntxtForSrvcc.numOfcodecList; count++)
         {
            cmMemcpy((U8*)&(( (EgIe *)(*dupIe))->t.mmCntxtForSrvcc.codec[count]), 
                  (U8*)&(oldIe->t.mmCntxtForSrvcc.codec[count]), sizeof(EgCodecList));
         }
         break;
      }
      case EG_STN_SR:
      {
         cmMemcpy((U8*)&(( (EgIe *)(*dupIe))->t.stnsr), 
               (U8*)&(oldIe->t.stnsr), sizeof(EgSTNSR));
         break;
      }
#endif /* EG_REL_AC0 */

      default:
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG042, ERRZERO,
               "egUtilDupIe: Ie datatype is not valid.\n");
         RETVALUE(EG_UTIL_ERR_INV_IE_DATATYPE);
      }
   }

   /*********************************************
    * initialize the node address of the new IE *
    *********************************************/
   ( (EgIe *)(*dupIe))->node.node = (PTR) (*dupIe);

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    EgUtilGmDump
*
*    Desc:   Method is used to dump the diameter message on
*            console
*
*    Ret:    ROK - if the ie list is duplicated successfully
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef LEG_FILE_LOG
#ifdef ANSI
PUBLIC S16 EgUtilGmDump
(
EgMsg      *egMsg,          /* GTP message */
FILE           *fp,            /* file pointer */
U16            *nmbLines       /* number of lines */
)
#else
PUBLIC S16 EgUtilGmDump(egMsg , fp, nmbLines)
EgMsg      *egMsg;         /* GTP message */
FILE           *fp;            /* file pointer */
U16            *nmbLines;      /* number of lines */
#endif
#else 
#ifdef ANSI
PUBLIC S16 EgUtilGmDump
(
EgMsg      *egMsg          /* GTP message */
)
#else
PUBLIC S16 EgUtilGmDump(egMsg)
EgMsg      *egMsg;         /* GTP message */
#endif
#endif /* LEG_FILE_LOG*/
{
   /* this is used to hold the return value of sub methods */
   S16 retValue = 0;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmDump) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg
#ifdef LEG_FILE_LOG
       || !fp || !nmbLines
#endif /* end of LEG_FILE_LOG */
      )
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*******************************
    * dump the GTP message header *
    *******************************/
   if ( (retValue = egUtilDumpGmHdr(egMsg->msgHdr
#ifdef LEG_FILE_LOG
                                                , fp, nmbLines
#endif /* end of LEG_FILE_LOG */
                                   )) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG043, ERRZERO,
               "EgUtilGmDump: Dumping the DM header failed.\n");
      RETVALUE(retValue);
   }

   /********************
    * dump the ie list *
    ********************/
   if ( (retValue = egUtilDumpList(&egMsg->egIeList, EG_IE_LEVEL_ZERO
#ifdef LEG_FILE_LOG
                                                , fp, nmbLines
#endif /* end of LEG_FILE_LOG */
                                   )) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG044, ERRZERO,
               "EgUtilGmDump: Dumping the ie list failed.\n");
      RETVALUE(retValue);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    egUtilDumpGmHdr
*
*    Desc:   Method is used to dump the diameter header
*
*    Ret:    ROK - if the diameter header is dumped successfully
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef LEG_FILE_LOG
#ifdef ANSI
PUBLIC S16 egUtilDumpGmHdr
(
EgMsgHdr       egGmHdr,          /* GTP header */
FILE           *fp,             /* file pointer */
U16            *nmbLines        /* number of lines */
)
#else
PUBLIC S16 egUtilDumpGmHdr(egGmHdr , fp, nmbLines)
EgMsgHdr       egGmHdr;         /* GTP header */
FILE           *fp;             /* file pointer */
U16            *nmbLines;       /* number of lines */
#endif
#else

#ifdef ANSI
PUBLIC S16 egUtilDumpGmHdr
(
EgMsgHdr       egGmHdr          /* GTP header */
)
#else
PUBLIC S16 egUtilDumpGmHdr(egGmHdr)
EgMsgHdr       egGmHdr;         /* GTP header */
#endif

#endif /* LEG_FILE_LOG */
{
   Txt buff[EG_UTIL_PRNT_BUFF_SIZE];
   Txt prnBuff[EG_UTIL_PRNT_BUFF_SIZE];
   Txt newLine[EG_THREE];

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpGmHdr) 

   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )prnBuff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   /* eg012.201 : replaced sprintf with snprintf */
   snprintf(newLine,EG_THREE," \n");

   snprintf(buff, EG_UTIL_PRNT_BUFF_SIZE,"   ********************* GTP Message *********************\n");
   
   snprintf(buff, EG_UTIL_PRNT_BUFF_SIZE,"  Ver    = %x\n", egGmHdr.version);
   EG_STRCAT(prnBuff, buff);

   snprintf(buff, EG_UTIL_PRNT_BUFF_SIZE,"  MsgType    = %d\n", egGmHdr.msgType);
   EG_STRCAT(prnBuff, buff);
#ifdef ALIGN_64BIT
   snprintf(buff, EG_UTIL_PRNT_BUFF_SIZE,"  teid  = %d\n", egGmHdr.teid);
   EG_STRCAT(prnBuff, buff);
#else
   snprintf(buff, EG_UTIL_PRNT_BUFF_SIZE,"  teid  = %ld\n", egGmHdr.teid);
   EG_STRCAT(prnBuff, buff);
#endif /* end of  ALIGN_64BIT */

   snprintf(buff, EG_UTIL_PRNT_BUFF_SIZE,"  Seq No    = %ld\n", egGmHdr.seqNumber);
   EG_STRCAT(prnBuff, buff);
#ifdef LEG_FILE_LOG
   fprintf(fp, "%s\n", prnBuff);
   nmbLines += 6;
#else
   SPrint(prnBuff);
   SPrint(newLine);
#endif /* end of LEG_FILE_LOG */

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    egUtilDumpList
*
*    Desc:   Method is used to dump an ie list
*
*    Ret:    ROK - if the ie list is dumped successfully
*            RFAILED - if the ie list dumping fails
*            Return value of the child method (if any)
*
*    Notes:  
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef LEG_FILE_LOG
#ifdef ANSI
PUBLIC S16 egUtilDumpList
(
CmLListCp      *ieList,         /* ie list */
S16            lvl,               /* level of the list */
FILE           *fp,              /* file pointer */
U16            *nmbLines         /* number of lines */
)
#else
PUBLIC S16 egUtilDumpList(ieList, lvl, fp, nmbLines)
CmLListCp      *ieList;          /* ie list */
S16            lvl;              /* level of the list */
FILE           *fp;              /* file pointer */
U16            *nmbLines;        /* number of lines */
#endif

#else

#ifdef ANSI
PUBLIC S16 egUtilDumpList
(
CmLListCp      *ieList,         /* ie list */
S16            lvl               /* level of the list */
)
#else
PUBLIC S16 egUtilDumpList(ieList, lvl)
CmLListCp      *ieList;          /* ie list */
S16            lvl;              /* level of the list */
#endif

#endif /* LEG_FILE_LOG */
{
   EgIe *egIe = NULLP;            /* to hold the address of IE in the original list */

   S16 retValue = 0;              /* to hold the return value of sub methods */

   S16            tmpLvl = 0;              /* level of the list */

   CmLList *node = NULLP;         /* to hold the temporary node address */

   /************************
    * to hold print buffer *
    ************************/
   Txt buff[EG_UTIL_PRNT_BUFF_SIZE];
   Txt prnBuff[EG_UTIL_PRNT_BUFF_SIZE];
   Txt newLine[EG_THREE];

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDumpList) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )prnBuff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   sprintf(newLine," \n");

#ifdef ALIGN_64BIT
   sprintf(buff, "           ******* IE list *******              \n");
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  count = %d\n", ieList->count);
   EG_STRCAT(prnBuff, buff);
#else
   sprintf(buff, "           ******* IE list *******              \n");
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  count = %ld\n", ieList->count);
   EG_STRCAT(prnBuff, buff);
#endif /* end of ALIGN_64BIT */

#ifdef LEG_FILE_LOG
   fprintf(fp, "%s\n", prnBuff);
   nmbLines += 2;
#else
   SPrint(prnBuff);
   SPrint("\n");
#endif /* end of LEG_FILE_LOG */

   /***********************************************
    * iterate through the list and for each ie,   *
    * call egUtilDumpIe to dump the IE.           *
    * If the ie is a grouped ie, call the         *
    * same method with the grouped ie's ie list   *
    ***********************************************/
   for (egIe = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
        egIe != NULLP;
        egIe = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node))
   {
      /***************
       * dump the IE *
       ***************/
      if ( (retValue = egUtilDumpIe(egIe, lvl
#ifdef LEG_FILE_LOG
                                               , fp, nmbLines
#endif /* end of LEG_FILE_LOG */
                                    )) != ROK)
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG045, ERRZERO,
               "egUtilDumpList: Dumping the IE failed.\n");
         RETVALUE(retValue);
      }

      /*********************************************************
       * if the IE type is grouped, then call the same method  *
       * with the IE list in the IE and level as current level *
       * plus one as argument.                                 *
       *********************************************************/
      if (egIe->dataType == EG_GROUPED)
      {
         tmpLvl = lvl + 1;
         if ( (retValue = egUtilDumpList(&egIe->t.grouped.gIeList, tmpLvl
#ifdef LEG_FILE_LOG
                                                                            , fp, nmbLines
#endif /* end of LEG_FILE_LOG */
                                    )) != ROK)
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG046, ERRZERO,
               "egUtilDumpList: Dumping the grouped IE list failed.\n");
            RETVALUE(retValue);
         }
      }
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    egUtilDumpIe
*
*    Desc:   Method is used to dump the IE
*
*    Ret:    ROK - if dumping IE succeeds
*            RFAILED - if dumping IE fails
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef LEG_FILE_LOG

#ifdef ANSI
PUBLIC S16 egUtilDumpIe
(
EgIe           *egIe,          /* IE that has to be dumped */
S16            lvl,             /* level of the list */
FILE           *fp,            /* file pointer */
U16            *nmbLines       /* number of lines */
)
#else
PUBLIC S16 egUtilDumpIe(egIe, lvl, fp, nmbLines)
EgIe           *egIe;          /* IE that has to be dumped */
S16            lvl;            /* level of the list */
FILE           *fp;            /* file pointer */
U16            *nmbLines;      /* number of lines */
#endif

#else
#ifdef ANSI
PUBLIC S16 egUtilDumpIe
(
EgIe           *egIe,          /* IE that has to be dumped */
S16            lvl             /* level of the list */
)
#else
PUBLIC S16 egUtilDumpIe(egIe, lvl )
EgIe           *egIe;          /* IE that has to be dumped */
S16            lvl;            /* level of the list */
#endif

#endif /* LEG_FILE_LOG */
{
  /*********************************************
   * string to hold the spaces buffer          *
   * this will hold four spaces for each       *
   * level upto maximum levels in the message  *
   *********************************************/
   S8 spaceBuff[EG_MAX_IE_LEVEL * 4];
#ifdef EG_PRNT_DATA
   U8 temp = 0;                          /* for looping */
   int count = 0, i =0,length=0;
#endif

   Txt buff[EG_UTIL_PRNT_BUFF_SIZE];      /* to hold buffer (Ie data) */
   Txt prnBuff[EG_UTIL_PRNT_BUFF_SIZE];   /* to hold print buffer (Ie header and data)*/  
   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */
   Txt tempBuff[EG_UTIL_PRNT_BUFF_SIZE-5]; /* to avoid the overflow */
   Txt newLine[EG_THREE];

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egIe)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /****************************************
    * initialize the space buffer with "-" *
    ****************************************/
   cmMemset( (U8* )spaceBuff, 45, EG_MAX_IE_LEVEL * 4);

   /***************************************
    * set the space buffer with space " " *
    ***************************************/
   /* eg012.201 : Added array out of bounds check */
   if (lvl <= EG_MAX_IE_LEVEL && lvl > EG_ZERO)
      cmMemset( (U8* )spaceBuff, 32, lvl * 4);

   /*******************************
    * initialize the print buffer *
    *******************************/
   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )prnBuff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
   cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
   spaceBuff[(EG_MAX_IE_LEVEL * 4) - 1] = '\0';
   sprintf(newLine," \n");

   /************************************************************
    * set the terminating character '\0' at last of the spaces *
    ************************************************************/
   /* eg012.201 : Added array out of bounds check */
   if (lvl != EG_ZERO)
      spaceBuff[(lvl * 4)-1] = '\0';

   /*********************
    * Dumping IE header *
    *********************/
   sprintf(buff, "  %s******* IE dump (%p) ********\n", spaceBuff, (Ptr)egIe);
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  %sIeType  = %d\n", spaceBuff, egIe->ieType);
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  %sIeInstance = %d\n", spaceBuff, egIe->ieInst);
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  %sIeDatatype = %d\n", spaceBuff, egIe->dataType);
   EG_STRCAT(prnBuff, buff);

#ifdef EG_PRNT_DATA
   /****************************
    * check the type of the IE *
    ****************************/
   switch(egIe->dataType)
   {
      case EG_UNSIGNED_8: 
         {
            sprintf(buff, "  value = %d\n",egIe->t.valUnSgnd8);
            break;
         }

      case EG_UNSIGNED_16: 
         {
            sprintf(buff, "  value = %d\n",egIe->t.valUnSgnd16);
            break;
         }

      case EG_UNSIGNED_32: 
         {
#ifdef ALIGN_64BIT
            sprintf(buff, "  value = %d\n",egIe->t.valUnSgnd32);
#else
            sprintf(buff, "  value = %ld\n",egIe->t.valUnSgnd32);
#endif
            break;
         }

      case EG_STRING_8: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr8.val, egIe->t.valStr8.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr8.length, tmpBuff);
            break;
         }

      case EG_STRING_16: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr16.val, egIe->t.valStr16.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr16.length, tmpBuff);
            break;
         }

      case EG_BCD: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr32.val, egIe->t.valStr32.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr32.length, tmpBuff);
            break;
         }
      case EG_STRING_32: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr32.val, egIe->t.valStr32.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr32.length, tmpBuff);
            break;
         }

      case EG_STRING_132: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr132.val, egIe->t.valStr132.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr132.length, tmpBuff);
            break;
         }

      case EG_STRING_400: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr400.val, egIe->t.valStr400.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr400.length, tmpBuff);
            break;
         }
      case EG_STRING_MAX: 
         {
            cmMemcpy( (U8* )tmpBuff, egIe->t.valStr.val, egIe->t.valStr.length);
            sprintf(buff, "  length = %d \n  value = %s\n", egIe->t.valStr.length, tmpBuff);
            break;
         }

      case EG_SNETWORK:
         {
            egUtilDumpSNwk(buff, egIe->t.sNwk);
            break; 
         }

      case EG_INDICATION:
         {
            sprintf(buff,"isSGWCIPres: %d\n", egIe->t.indication.isSGWCIPres); 

            sprintf(tmpBuff,"isISRAIPres: %d\n", egIe->t.indication.isISRAIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isISRSIPres: %d\n", egIe->t.indication.isISRSIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isOIPres: %d\n", egIe->t.indication.isOIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isDFIPres: %d\n", egIe->t.indication.isDFIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isHIPres: %d\n", egIe->t.indication.isHIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isDTFPres: %d\n", egIe->t.indication.isDTFPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isDAFPres: %d\n", egIe->t.indication.isDAFPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isMSVPres: %d\n", egIe->t.indication.isMSVPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isSIPres: %d\n", egIe->t.indication.isSIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isPTPres: %d\n", egIe->t.indication.isPTPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isPPres: %d\n", egIe->t.indication.isPPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isCRSIPres: %d\n", egIe->t.indication.isCRSIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isCFSIPres: %d\n", egIe->t.indication.isCFSIPres);  
            EG_STRCAT(buff,tmpBuff);
#if defined(EG_REL_930) || defined(EG_REL_AC0)
            sprintf(tmpBuff,"isUIMSIPres: %d\n", egIe->t.indication.isUIMSIPres);  
            EG_STRCAT(buff,tmpBuff);
#endif
#ifdef EG_REL_AC0
            sprintf(tmpBuff,"isSQCIPres: %d\n", egIe->t.indication.isSQCIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isCCRSIPres: %d\n", egIe->t.indication.isCCRSIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isISRAUPres: %d\n", egIe->t.indication.isISRAUPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isMBMDTPres: %d\n", egIe->t.indication.isMBMDTPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isS4AFPres: %d\n", egIe->t.indication.isS4AFPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isS6AFPres: %d\n", egIe->t.indication.isS6AFPres);  
            EG_STRCAT(buff,tmpBuff);
#endif
            break; 
         }
#ifdef EG_REL_AC0
      case EG_STN_SR:
         {
            sprintf(buff,"extension: %d\n", egIe->t.stnsr.ext);  

            sprintf(tmpBuff,"nature of address: %d\n", egIe->t.stnsr.NatOfAdd);  
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"numbering plan indicator: %d\n", egIe->t.stnsr.NPI);  
            EG_STRCAT(buff,tmpBuff);
            cmMemcpy( (U8* )tempBuff, egIe->t.stnsr.val, 15);
            sprintf(tmpBuff,"stn-sr val: %s\n", egIe->t.stnsr.val);  
            EG_STRCAT(buff,tmpBuff);
            break; 
         }
#endif
      case EG_PVT_EXTN:
         {
            sprintf(buff,"entId: %d\n", egIe->t.pvtExt.entId);  

            sprintf(tmpBuff,"propVal.length: %d\n", egIe->t.pvtExt.propVal.length);  
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.pvtExt.propVal.val, egIe->t.pvtExt.propVal.length);
            sprintf(tmpBuff,"propVal.val: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);
            break; 
         }

      case EG_CAUSE:
         {
            sprintf(buff,"causeVal: %d\n", egIe->t.cause.causeVal);  

            sprintf(tmpBuff,"ieCs: %d\n", egIe->t.cause.ieCS);  
            EG_STRCAT(buff,tmpBuff);
#if defined(EG_REL_930) || defined(EG_REL_AC0)
            sprintf(tmpBuff,"pce: %d\n", egIe->t.cause.pce);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"bce: %d\n", egIe->t.cause.bce);  
            EG_STRCAT(buff,tmpBuff);
#endif
            sprintf(tmpBuff,"ieType: %d\n", egIe->t.cause.ieType);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"ieLength: %d\n", egIe->t.cause.ieLength);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"ieInst: %d\n", egIe->t.cause.ieInst);  
            EG_STRCAT(buff,tmpBuff);

            break; 
         }

      case EG_TRC_INFO:
         {
            egUtilDumpSNwk(buff, egIe->t.sNwk);
#ifdef ALIGN_64BIT
            sprintf(buff,"trcId: %d\n", egIe->t.trcInfo.trcId);  
#else
            sprintf(buff,"trcId: %ld\n", egIe->t.trcInfo.trcId);  
#endif

            cmMemcpy( (U8* )tempBuff, egIe->t.trcInfo.trigEvnts, 9);
            sprintf(tmpBuff,"trigEvnts: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"lstNETypes: %d\n", egIe->t.trcInfo.lstNETypes);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"sesnTrcDep: %d\n", egIe->t.trcInfo.sesnTrcDep);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"lstOfIntfs: %s\n", egIe->t.trcInfo.lstOfIntfs);  
            EG_STRCAT(buff,tmpBuff);

            egUtilDumpIpAddr(buff,&egIe->t.trcInfo.trcColEntAddr);
            break; 
         }

      case EG_BEARER_FLAG:
         {
            sprintf(tmpBuff,"isPPCPres: %x\n", egIe->t.bearFlags.isPPCPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isVBPres: %xn", egIe->t.bearFlags.isVBPres);  
            EG_STRCAT(buff,tmpBuff);
#ifdef EG_REL_AC0
            sprintf(tmpBuff,"isASIPres: %x\n", egIe->t.bearFlags.isASIPres);  
            EG_STRCAT(buff,tmpBuff);
#endif
            break; 
         }

      case EG_FTEID:
         {
            sprintf(buff,"isIPv4Pres: %d\n", egIe->t.fTeid.isIPv4Pres);  

            sprintf(tmpBuff,"isIPv6Pres: %d\n", egIe->t.fTeid.isIPv6Pres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"intfType: %d\n", egIe->t.fTeid.intfType);  
            EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT 
            sprintf(tmpBuff,"teid: %d\n", egIe->t.fTeid.teid);  
#else
            sprintf(tmpBuff,"teid: %ld\n", egIe->t.fTeid.teid);  
#endif
            EG_STRCAT(buff,tmpBuff);

            if(egIe->t.fTeid.isIPv4Pres)
            {
               egUtilDumpIpv4Addr(buff,egIe->t.fTeid.ip4Addr);
            }
            /* ccpu00137940 */
            if(egIe->t.fTeid.isIPv6Pres)
            {
               egUtilDumpIpv6Addr(buff,egIe->t.fTeid.ipv6Address);
            }
            break; 
         }

      case EG_ULI_DATA:
         {
            sprintf(buff,"isCGIPres: %d\n", egIe->t.uli.isCGIPres);  

            sprintf(tmpBuff,"isSAIPres: %d\n", egIe->t.uli.isSAIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isRAIPres: %d\n", egIe->t.uli.isRAIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isTAIPres: %d\n", egIe->t.uli.isTAIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isECGIPress: %d\n", egIe->t.uli.isECGIPres);  
            EG_STRCAT(buff,tmpBuff);
#ifdef EG_REL_AC0
            sprintf(tmpBuff,"isLAIPress: %d\n", egIe->t.uli.isLAIPres);  
            EG_STRCAT(buff,tmpBuff);
#endif
            if (egIe->t.uli.isCGIPres)
            {
               sprintf(tmpBuff,"CGI details :\n");  
               EG_STRCAT(buff,tmpBuff);
               egUtilDumpSNwk(buff, egIe->t.uli.cgi.sNwk);

               sprintf(tmpBuff,"lac: %d\n", egIe->t.uli.cgi.lac);  
               EG_STRCAT(buff,tmpBuff);
               sprintf(tmpBuff,"lac: %d\n", egIe->t.uli.cgi.lac);  
               EG_STRCAT(buff,tmpBuff);
            }

            if (egIe->t.uli.isSAIPres)
            {
               sprintf(tmpBuff,"SAI details :\n");  
               EG_STRCAT(buff,tmpBuff);
               egUtilDumpSNwk(buff, egIe->t.uli.sai.sNwk);

               sprintf(tmpBuff,"lac: %d\n", egIe->t.uli.sai.lac);  
               EG_STRCAT(buff,tmpBuff);
               sprintf(tmpBuff,"sac: %d\n", egIe->t.uli.sai.sac);  
               EG_STRCAT(buff,tmpBuff);
            }

            if (egIe->t.uli.isRAIPres)
            {
               sprintf(tmpBuff,"RAI details :\n");  
               EG_STRCAT(buff,tmpBuff);
               egUtilDumpSNwk(buff, egIe->t.uli.rai.sNwk);

               sprintf(tmpBuff,"lac: %d\n", egIe->t.uli.rai.lac);  
               EG_STRCAT(buff,tmpBuff);
               sprintf(tmpBuff,"rac: %d\n", egIe->t.uli.rai.rac);  
               EG_STRCAT(buff,tmpBuff);
            }

            if (egIe->t.uli.isTAIPres)
            {
               sprintf(tmpBuff,"TAI details :\n");  
               EG_STRCAT(buff,tmpBuff);
               egUtilDumpSNwk(buff, egIe->t.uli.tai.sNwk);

               sprintf(tmpBuff,"tac: %d\n", egIe->t.uli.tai.tac);  
               EG_STRCAT(buff,tmpBuff);
            }
#ifdef EG_REL_AC0
            if (egIe->t.uli.isLAIPres)
            {
               sprintf(tmpBuff,"LAI details :\n");  
               EG_STRCAT(buff,tmpBuff);
               egUtilDumpSNwk(buff, egIe->t.uli.lai.sNwk);

               sprintf(tmpBuff,"lac: %d\n", egIe->t.uli.lai.lac);  
               EG_STRCAT(buff,tmpBuff);
            }
#endif

            if (egIe->t.uli.isECGIPres)
            {
               sprintf(tmpBuff,"ECGI details :\n");  
               EG_STRCAT(buff,tmpBuff);

               egUtilDumpSNwk(buff, egIe->t.uli.ecgi.sNwk);
#ifdef ALIGN_64BIT
               sprintf(tmpBuff,"ecId: %d\n", egIe->t.uli.ecgi.ecId);  
#else
               sprintf(tmpBuff,"ecId: %ld\n", egIe->t.uli.ecgi.ecId); 
#endif 
               EG_STRCAT(buff,tmpBuff);
            }
            break; 
         }

      case EG_FQOS:
         {
            sprintf(buff,"qci: %d\n", egIe->t.fQos.qci);  

            cmMemcpy( (U8* )tempBuff, egIe->t.fQos.upMBR.val, egIe->t.fQos.upMBR.length);
            sprintf(tmpBuff,"upMBR: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.fQos.dnMBR.val, egIe->t.fQos.dnMBR.length);
            sprintf(tmpBuff,"dnMBR: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.fQos.upGBR.val, egIe->t.fQos.upGBR.length);
            sprintf(tmpBuff,"upGBR: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.fQos.dnGBR.val, egIe->t.fQos.dnGBR.length);
            sprintf(tmpBuff,"dnGBR: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);
            break; 
         }

      case EG_BQOS_IEDATA:
         {
            sprintf(buff,"isPVIPres: %d\n", egIe->t.bQos.isPVIPres);  

            sprintf(tmpBuff,"isPCIPres: %d\n", egIe->t.bQos.isPCIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"prityLvl: %d\n", egIe->t.bQos.prityLvl);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"qci: %d\n", egIe->t.bQos.qci);  
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.bQos.upMBR.val, egIe->t.bQos.upMBR.length);
            sprintf(tmpBuff,"upMBR: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.bQos.dnMBR.val, egIe->t.bQos.dnMBR.length);
            sprintf(tmpBuff,"dnMBR: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.bQos.upGBR.val, egIe->t.bQos.upGBR.length);
            sprintf(tmpBuff,"upGBR: %s\n", tempBuff);  
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.bQos.dnGBR.val, egIe->t.bQos.dnGBR.length);
            sprintf(tmpBuff,"dnGBR: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);
            break; 
         }

      case EG_PAA_IEDATA:
         {
            sprintf(buff,"pdnType: %d\n", egIe->t.paa.pdnType);  
            switch(egIe->t.paa.pdnType)
            {
               case 1: 
                  {
                     /* if the pdn type is 1, then the pdn address is of type Ipv4 */
                     egUtilDumpIpv4Addr(buff,egIe->t.paa.u.pdnIpv4Addr);
                     break;
                  }
               case 2:
                  {
                     /* If the pdnType is 2, then first octet will tell the pdn prifix */
                     /* next 16octets will tell the ipv6 address */
                     sprintf(tmpBuff,"pdnPrefix: %d\n", egIe->t.paa.pdnPrefix);  
                     EG_STRCAT(buff,tmpBuff);

                     egUtilDumpIpv6Addr(buff,egIe->t.paa.u.pdnIpv6Addr);
                     break;
                  }
               case 3:
                  {
                     /* If the pdnType is 3, then first octet will tell the pdn prifix */
                     /* next 16octets will be the ipv6 address */
                     /* next 4 octets will be the ipv4 address */
                     sprintf(tmpBuff,"pdnPrefix: %d\n", egIe->t.paa.pdnPrefix);  
                     EG_STRCAT(buff,tmpBuff);

                     egUtilDumpIpv4Addr(buff,egIe->t.paa.u.pdnBoth.pdnIpv4Addr);
                     egUtilDumpIpv6Addr(buff,egIe->t.paa.u.pdnBoth.pdnIpv6Addr);
                     break;
                  }
          default:
        break;
            }
         }

      case EG_GROUPED:
         {
            break;
         }

      case EG_PDN_CONN_SET_ID:
      {
         sprintf(buff,"nodeIdType: %d\n", egIe->t.csid.nodeIdType);  
         sprintf(tmpBuff,"noOfCSIDs: %d\n", egIe->t.csid.noOfCSIDs);  
         EG_STRCAT(buff,tmpBuff);
         cmMemcpy( (U8* )tempBuff, egIe->t.csid.nodeId.val, 
               egIe->t.csid.nodeId.length);
         sprintf(buff," nodeId.length: %d\n nodeId.val: %s\n", 
               egIe->t.csid.nodeId.length, tempBuff);
         cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
       for(i=0;i<egIe->t.csid.noOfCSIDs;i++)
        {
          cmMemcpy( (U8* )tempBuff, egIe->t.csid.csid[i].val, 
               egIe->t.csid.csid[i].length);
         sprintf(tmpBuff," csid.length: %d\n csid.val: %s\n", 
               egIe->t.csid.csid[i].length, tempBuff);
         }
         cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_GLOBAL_CN_ID:
      {
         egUtilDumpSNwk(buff, egIe->t.gblCnId.sNwk);

         cmMemcpy( (U8* )tempBuff, egIe->t.gblCnId.cnId.val, egIe->t.gblCnId.cnId.length);
         sprintf(tmpBuff," cnId.length: %d\n cnId.val: %s\n",egIe->t.gblCnId.cnId.length,tempBuff);
         cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_S103_PDN_FW_INFO:
      {
         sprintf(buff," hsgwAddrLen: %d\n", egIe->t.s103PDF.hsgwAddrLen);

         egUtilDumpIpAddr(buff,&egIe->t.s103PDF.hsgwAddr);
         /*
         if(egIe->t.s103PDF.psdnAddr.type == CM_TPTADDR_IPV4)
         {
            egUtilDumpIpv4Addr(buff,egIe->t.s103PDF.psdnAddr.u.ipv4);
         }
#ifdef EG_IPV6_SUPPORTED
         else if (egIe->t.s103PDF.psdnAddr.type == CM_TPTADDR_IPV6)
         {
            egUtilDumpIpv6Addr(buff,egIe->t.s103PDF.psdnAddr.u.ipv6);
         }
#endif
*/
#ifdef ALIGN_64BIT
         sprintf( tmpBuff," greKey: %d\n", egIe->t.s103PDF.greKey);
#else
         sprintf( tmpBuff," greKey: %ld\n", egIe->t.s103PDF.greKey);
#endif
         EG_STRCAT(buff,tmpBuff);

         sprintf( tmpBuff," ebiNumber: %d\n", egIe->t.s103PDF.ebiNumber);
         EG_STRCAT(buff,tmpBuff);

         for(temp = 0; temp < egIe->t.s103PDF.ebiNumber; temp++)
         {
            printf( tmpBuff," ebi%d: %d\n", (temp+1), egIe->t.s103PDF.ebi[temp]);
         }

         break;
      }

      case EG_S1U_DATA_FW:
      {
         sprintf(buff," ebi: %d\n", egIe->t.s1UDF.ebi);


         egUtilDumpIpAddr(buff,&egIe->t.s1UDF.sgwAddrLen);
         egUtilDumpIpAddr(buff,&egIe->t.s1UDF.sgwAddr);
         /*
         if(egIe->t.s1UDF.sgwAddr.type == CM_TPTADDR_IPV4)
         {
            egUtilDumpIpv4Addr(buff,egIe->t.s1UDF.sgwAddr.u.ipv4);
         }
#ifdef EG_IPV6_SUPPORTED
         else if (egIe->t.s1UDF.sgwAddr.type == CM_TPTADDR_IPV6)
         {
            egUtilDumpIpv6Addr(buff,egIe->t.s1UDF.sgwAddr.u.ipv6);
         }
#endif
*/
#ifdef ALIGN_64BIT
         sprintf( tmpBuff," sgwS1UTEID: %d\n", egIe->t.s1UDF.sgwS1UTEID);
#else
         sprintf( tmpBuff," sgwS1UTEID: %ld\n", egIe->t.s1UDF.sgwS1UTEID);
#endif
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_GSM_KEY_TRIP:
      {
         sprintf(buff," secMode :%d\n",egIe->t.gsmKeyT.secMode);

         sprintf(tmpBuff," isDRXIPres:%d\n",egIe->t.gsmKeyT.isDRXIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," cksn:%d\n",egIe->t.gsmKeyT.cksn);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbTrip:%d\n",egIe->t.gsmKeyT.nmbTrip);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUAMBRIPres:%d\n",egIe->t.gsmKeyT.isUAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isSAMBRIPres:%d\n",egIe->t.gsmKeyT.isSAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," usedCipher:%d\n",egIe->t.gsmKeyT.usedCipher);
         EG_STRCAT(buff,tmpBuff);

         for(temp = 0; temp < 8; temp++)
         {
            sprintf(tmpBuff," kc%d: %d\n", (temp+1), egIe->t.gsmKeyT.kc[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

         for(temp = 0; temp < egIe->t.gsmKeyT.nmbTrip; temp++)
         {
            cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyT.authTrip[temp].rand, 16);
            sprintf(tmpBuff," authTrip.rand: %s\n", tempBuff);
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyT.authTrip[temp].sres, 4);
            sprintf(tmpBuff," authTrip.rand: %s\n", tempBuff);
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyT.authTrip[temp].kc, 8);
            sprintf(tmpBuff," authTrip.rand: %s\n", tempBuff);
            cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
            EG_STRCAT(buff,tmpBuff);
         }
         
         for(temp = 0; temp < 2; temp++)
         {
            sprintf(tmpBuff," drxParam%d :%d\n",(temp+1),egIe->t.gsmKeyT.drxParam[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upSubsUEAMBR: %d\n", egIe->t.gsmKeyT.upSubsUEAMBR);
#else
         sprintf( tmpBuff," upSubsUEAMBR: %x\n", egIe->t.gsmKeyT.upSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnSubsUEAMBR: %d\n", egIe->t.gsmKeyT.dnSubsUEAMBR);
#else
         sprintf( tmpBuff," dnSubsUEAMBR: %ld\n", egIe->t.gsmKeyT.dnSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upUsedUEAMBR: %d\n", egIe->t.gsmKeyT.upUsedUEAMBR);
#else
         sprintf( tmpBuff," upUsedUEAMBR: %ld\n", egIe->t.gsmKeyT.upUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnUsedUEAMBR: %d\n", egIe->t.gsmKeyT.dnUsedUEAMBR);
#else
         sprintf( tmpBuff," dnUsedUEAMBR: %ld\n", egIe->t.gsmKeyT.dnUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyT.ueNtwrkCapb.val, 
               egIe->t.gsmKeyT.ueNtwrkCapb.length);
         sprintf(tmpBuff," ueNtwrkCapb.length: %d\n ueNtwrkCapb.val: %s\n", 
               egIe->t.gsmKeyT.ueNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyT.msNtwrkCapb.val, 
               egIe->t.gsmKeyT.msNtwrkCapb.length);
         sprintf(tmpBuff," msNtwrkCapb.length: %d\n msNtwrkCapb.val: %s\n", 
               egIe->t.gsmKeyT.msNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyT.meId.val, 
               egIe->t.gsmKeyT.meId.length);
         sprintf(tmpBuff," meId.length: %d\n meId.val: %s\n", 
               egIe->t.gsmKeyT.meId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         sprintf( tmpBuff," isUNA: %d\n", egIe->t.gsmKeyT.isUNA);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," isGENA: %d\n", egIe->t.gsmKeyT.isGENA);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," isGANA: %d\n", egIe->t.gsmKeyT.isGANA);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," isINA: %d\n", egIe->t.gsmKeyT.isINA);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," isENA: %d\n", egIe->t.gsmKeyT.isENA);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," isHNNA: %d\n", egIe->t.gsmKeyT.isHNNA);
         EG_STRCAT(buff,tmpBuff);
#if defined(EG_REL_930) || defined(EG_REL_AC0)

         sprintf( tmpBuff," ueUsageSett: %d\n", egIe->t.gsmKeyT.ueUsageSett);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," vDomPref: %d\n", egIe->t.gsmKeyT.vDomPref);
         EG_STRCAT(buff,tmpBuff);
#endif
         break;
      }

      case EG_UMTS_KEY_CIPH_QUIN:
      {
         sprintf(buff," secMode :%d\n",egIe->t.umtsKeyUCQt.secMode);

         sprintf(tmpBuff," isDRXIPres:%d\n",egIe->t.umtsKeyUCQt.isDRXIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," cksn:%d\n",egIe->t.umtsKeyUCQt.cksn);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuin:%d\n",egIe->t.umtsKeyUCQt.nmbQuin);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUAMBRIPres:%d\n",egIe->t.umtsKeyUCQt.isUAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isSAMBRIPres:%d\n",egIe->t.umtsKeyUCQt.isSAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," usedCipher:%d\n",egIe->t.umtsKeyUCQt.usedCipher);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyUCQt.ck.val, 
               egIe->t.umtsKeyUCQt.ck.length);
         sprintf(tmpBuff," ck.length: %d\n ck.val: %s\n", 
               egIe->t.umtsKeyUCQt.ck.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyUCQt.ik.val, 
               egIe->t.umtsKeyUCQt.ik.length);
         sprintf(tmpBuff," ik.length: %d\n ik.val: %s\n", 
               egIe->t.umtsKeyUCQt.ik.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
       
         for(temp = 0; temp < egIe->t.umtsKeyUCQt.nmbQuin; temp++)
         {
            egUtilDumpAuthQt(buff, egIe->t.umtsKeyUCQt.authQuin[temp]);
         }

         for(temp = 0; temp < 2; temp++)
         {
            sprintf(tmpBuff," drxParam%d :%d\n",(temp+1),egIe->t.umtsKeyUCQt.drxParam[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upSubsUEAMBR: %d\n", egIe->t.umtsKeyUCQt.upSubsUEAMBR);
#else
         sprintf( tmpBuff," upSubsUEAMBR: %x\n", egIe->t.umtsKeyUCQt.upSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnSubsUEAMBR: %d\n", egIe->t.umtsKeyUCQt.dnSubsUEAMBR);
#else
         sprintf( tmpBuff," dnSubsUEAMBR: %ld\n", egIe->t.umtsKeyUCQt.dnSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upUsedUEAMBR: %d\n", egIe->t.umtsKeyUCQt.upUsedUEAMBR);
#else
         sprintf( tmpBuff," upUsedUEAMBR: %ld\n", egIe->t.umtsKeyUCQt.upUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnUsedUEAMBR: %d\n", egIe->t.umtsKeyUCQt.dnUsedUEAMBR);
#else
         sprintf( tmpBuff," dnUsedUEAMBR: %ld\n", egIe->t.umtsKeyUCQt.dnUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyUCQt.ueNtwrkCapb.val, 
               egIe->t.umtsKeyUCQt.ueNtwrkCapb.length);
         sprintf(tmpBuff," ueNtwrkCapb.length: %d\n ueNtwrkCapb.val: %s\n", 
               egIe->t.umtsKeyUCQt.ueNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyUCQt.msNtwrkCapb.val, 
               egIe->t.umtsKeyUCQt.msNtwrkCapb.length);
         sprintf(tmpBuff," msNtwrkCapb.length: %d\n msNtwrkCapb.val: %s\n", 
               egIe->t.umtsKeyUCQt.msNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyUCQt.meId.val, 
               egIe->t.umtsKeyUCQt.meId.length);
         sprintf(tmpBuff," meId.length: %d\n meId.val: %s\n", 
               egIe->t.umtsKeyUCQt.meId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         sprintf(tmpBuff," isUNA:%d\n",egIe->t.umtsKeyUCQt.isUNA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGENA:%d\n",egIe->t.umtsKeyUCQt.isGENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGANA:%d\n",egIe->t.umtsKeyUCQt.isGANA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isINA:%d\n",egIe->t.umtsKeyUCQt.isINA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isENA:%d\n",egIe->t.umtsKeyUCQt.isENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isHNNA:%d\n",egIe->t.umtsKeyUCQt.isHNNA);
         EG_STRCAT(buff,tmpBuff);

#if defined(EG_REL_930) || defined(EG_REL_AC0)

         sprintf( tmpBuff," ueUsageSett: %d\n", egIe->t.umtsKeyUCQt.ueUsageSett);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," vDomPref: %d\n", egIe->t.umtsKeyUCQt.vDomPref);
         EG_STRCAT(buff,tmpBuff);
#endif

#ifdef  EG_REL_AC0

         sprintf( tmpBuff," HighBitRateThan16Mbps: %d\n", egIe->t.umtsKeyUCQt.HighBitRateThan16Mbps);
         EG_STRCAT(buff,tmpBuff);
#endif
         break;
      }

      case EG_GSM_KEY_CIPH_QUIN:
      {
         sprintf(buff," secMode :%d\n",egIe->t.gsmKeyUCQt.secMode);

         sprintf(tmpBuff," isDRXIPres:%d\n",egIe->t.gsmKeyUCQt.isDRXIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," cksn:%d\n",egIe->t.gsmKeyUCQt.cksn);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuin:%d\n",egIe->t.gsmKeyUCQt.nmbQuin);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUAMBRIPres:%d\n",egIe->t.gsmKeyUCQt.isUAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isSAMBRIPres:%d\n",egIe->t.gsmKeyUCQt.isSAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," usedCipher:%d\n",egIe->t.gsmKeyUCQt.usedCipher);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyUCQt.kc, 
               EG_LEN_EIGHT);
         sprintf(tmpBuff," kc.length: %d\n kc.val: %s\n", 
               EG_LEN_EIGHT, tempBuff);
         EG_STRCAT(buff,tmpBuff);
        
         for(temp = 0; temp < egIe->t.gsmKeyUCQt.nmbQuin; temp++)
         {
            egUtilDumpAuthQt(buff, egIe->t.gsmKeyUCQt.authQuin[temp]);
         }
         for(temp = 0; temp < 2; temp++)
         {
            sprintf(tmpBuff," drxParam%d :%d\n",(temp+1),egIe->t.gsmKeyUCQt.drxParam[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upSubsUEAMBR: %d\n", egIe->t.gsmKeyUCQt.upSubsUEAMBR);
#else
         sprintf( tmpBuff," upSubsUEAMBR: %x\n", egIe->t.gsmKeyUCQt.upSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnSubsUEAMBR: %d\n", egIe->t.gsmKeyUCQt.dnSubsUEAMBR);
#else
         sprintf( tmpBuff," dnSubsUEAMBR: %ld\n", egIe->t.gsmKeyUCQt.dnSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upUsedUEAMBR: %d\n", egIe->t.gsmKeyUCQt.upUsedUEAMBR);
#else
         sprintf( tmpBuff," upUsedUEAMBR: %ld\n", egIe->t.gsmKeyUCQt.upUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnUsedUEAMBR: %d\n", egIe->t.gsmKeyUCQt.dnUsedUEAMBR);
#else
         sprintf( tmpBuff," dnUsedUEAMBR: %ld\n", egIe->t.gsmKeyUCQt.dnUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyUCQt.ueNtwrkCapb.val, 
               egIe->t.gsmKeyUCQt.ueNtwrkCapb.length);
         sprintf(tmpBuff," ueNtwrkCapb.length: %d\n ueNtwrkCapb.val: %s\n", 
               egIe->t.gsmKeyUCQt.ueNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyUCQt.msNtwrkCapb.val, 
               egIe->t.gsmKeyUCQt.msNtwrkCapb.length);
         sprintf(tmpBuff," msNtwrkCapb.length: %d\n msNtwrkCapb.val: %s\n", 
               egIe->t.gsmKeyUCQt.msNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.gsmKeyUCQt.meId.val, 
               egIe->t.gsmKeyUCQt.meId.length);
         sprintf(tmpBuff," meId.length: %d\n meId.val: %s\n", 
               egIe->t.gsmKeyUCQt.meId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         sprintf(tmpBuff," isUNA:%d\n",egIe->t.gsmKeyUCQt.isUNA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGENA:%d\n",egIe->t.gsmKeyUCQt.isGENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGANA:%d\n",egIe->t.gsmKeyUCQt.isGANA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isINA:%d\n",egIe->t.gsmKeyUCQt.isINA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isENA:%d\n",egIe->t.gsmKeyUCQt.isENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isHNNA:%d\n",egIe->t.gsmKeyUCQt.isHNNA);
         EG_STRCAT(buff,tmpBuff);

#if defined(EG_REL_930) || defined(EG_REL_AC0)

         sprintf( tmpBuff," ueUsageSett: %d\n", egIe->t.gsmKeyUCQt.ueUsageSett);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," vDomPref: %d\n", egIe->t.gsmKeyUCQt.vDomPref);
         EG_STRCAT(buff,tmpBuff);
#endif
#ifdef EG_REL_AC0

         sprintf( tmpBuff," HighBitRateThan16Mbps: %d\n", egIe->t.gsmKeyUCQt.HighBitRateThan16Mbps);
         EG_STRCAT(buff,tmpBuff);
#endif
         break;
      }

      case EG_UMTS_KEY_QUIN:
      {
         sprintf(buff," secMode :%d\n",egIe->t.umtsKeyQt.secMode);

         sprintf(tmpBuff," isDRXIPres:%d\n",egIe->t.umtsKeyQt.isDRXIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," ksi:%d\n",egIe->t.umtsKeyQt.ksi);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuin:%d\n",egIe->t.umtsKeyQt.nmbQuin);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUAMBRIPres:%d\n",egIe->t.umtsKeyQt.isUAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isSAMBRIPres:%d\n",egIe->t.umtsKeyQt.isSAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQt.ck.val, 
               egIe->t.umtsKeyQt.ck.length);
         sprintf(tmpBuff," ck.length: %d\n ck.val: %s\n", 
               egIe->t.umtsKeyQt.ck.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
        
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQt.ik.val, 
               egIe->t.umtsKeyQt.ik.length);
         sprintf(tmpBuff," ik.length: %d\n ik.val: %s\n", 
               egIe->t.umtsKeyQt.ik.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
        
         for(temp = 0; temp < egIe->t.umtsKeyQt.nmbQuin; temp++)
         {
            egUtilDumpAuthQt(buff, egIe->t.umtsKeyQt.authQuin[temp]);
         }

         for(temp = 0; temp < 2; temp++)
         {
            sprintf(tmpBuff," drxParam%d :%d\n",(temp+1),egIe->t.umtsKeyQt.drxParam[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upSubsUEAMBR: %d\n", egIe->t.umtsKeyQt.upSubsUEAMBR);
#else
         sprintf( tmpBuff," upSubsUEAMBR: %x\n", egIe->t.umtsKeyQt.upSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnSubsUEAMBR: %d\n", egIe->t.umtsKeyQt.dnSubsUEAMBR);
#else
         sprintf( tmpBuff," dnSubsUEAMBR: %ld\n", egIe->t.umtsKeyQt.dnSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upUsedUEAMBR: %d\n", egIe->t.umtsKeyQt.upUsedUEAMBR);
#else
         sprintf( tmpBuff," upUsedUEAMBR: %ld\n", egIe->t.umtsKeyQt.upUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnUsedUEAMBR: %d\n", egIe->t.umtsKeyQt.dnUsedUEAMBR);
#else
         sprintf( tmpBuff," dnUsedUEAMBR: %ld\n", egIe->t.umtsKeyQt.dnUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQt.ueNtwrkCapb.val, 
               egIe->t.umtsKeyQt.ueNtwrkCapb.length);
         sprintf(tmpBuff," ueNtwrkCapb.length: %d\n ueNtwrkCapb.val: %s\n", 
               egIe->t.umtsKeyQt.ueNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQt.msNtwrkCapb.val, 
               egIe->t.umtsKeyQt.msNtwrkCapb.length);
         sprintf(tmpBuff," msNtwrkCapb.length: %d\n msNtwrkCapb.val: %s\n", 
               egIe->t.umtsKeyQt.msNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQt.meId.val, 
               egIe->t.umtsKeyQt.meId.length);
         sprintf(tmpBuff," meId.length: %d\n meId.val: %s\n", 
               egIe->t.umtsKeyQt.meId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUNA:%d\n",egIe->t.umtsKeyQt.isUNA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGENA:%d\n",egIe->t.umtsKeyQt.isGENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGANA:%d\n",egIe->t.umtsKeyQt.isGANA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isINA:%d\n",egIe->t.umtsKeyQt.isINA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isENA:%d\n",egIe->t.umtsKeyQt.isENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isHNNA:%d\n",egIe->t.umtsKeyQt.isHNNA);
         EG_STRCAT(buff,tmpBuff);
 
#if defined(EG_REL_930) || defined(EG_REL_AC0)

         sprintf( tmpBuff," ueUsageSett: %d\n", egIe->t.umtsKeyQt.ueUsageSett);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," vDomPref: %d\n", egIe->t.umtsKeyQt.vDomPref);
         EG_STRCAT(buff,tmpBuff);
#endif
#ifdef EG_REL_AC0

         sprintf( tmpBuff," HighBitRateThan16Mbps: %d\n", egIe->t.umtsKeyQt.HighBitRateThan16Mbps);
         EG_STRCAT(buff,tmpBuff);
#endif
         break;
      }

      case EG_EPS_SEC_QUAD_QUIN:
      {
         sprintf(buff," secMode :%d\n",egIe->t.eSecCxtQdQt.secMode);

         sprintf(tmpBuff," isNHIPres:%d\n",egIe->t.eSecCxtQdQt.isNHIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isDRXIPres:%d\n",egIe->t.eSecCxtQdQt.isDRXIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," ksi:%d\n",egIe->t.eSecCxtQdQt.ksi);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuin:%d\n",egIe->t.eSecCxtQdQt.nmbQuin);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuadru:%d\n",egIe->t.eSecCxtQdQt.nmbQuadru);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUAMBRIPres:%d\n",egIe->t.eSecCxtQdQt.isUAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isOCSIPres:%d\n",egIe->t.eSecCxtQdQt.isOCSIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isSAMBRIPres:%d\n",egIe->t.eSecCxtQdQt.isSAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nasProtAlgo:%d\n",egIe->t.eSecCxtQdQt.nasProtAlgo);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," usedNasCipher:%d\n",egIe->t.eSecCxtQdQt.usedNasCipher);
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf(tmpBuff," nasDlCount:%d\n",egIe->t.eSecCxtQdQt.nasDlCount);
#else
         sprintf(tmpBuff," nasDlCount:%ld\n",egIe->t.eSecCxtQdQt.nasDlCount);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf(tmpBuff," nasUlCount:%d\n",egIe->t.eSecCxtQdQt.nasUlCount);
#else
         sprintf(tmpBuff," nasUlCount:%ld\n",egIe->t.eSecCxtQdQt.nasUlCount);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.eSecCxtQdQt.kasme.val, 
               egIe->t.eSecCxtQdQt.kasme.length);
         sprintf(tmpBuff," kasme.length: %d\n kasme.val: %s\n", 
               egIe->t.eSecCxtQdQt.kasme.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         for(temp = 0; temp < egIe->t.eSecCxtQdQt.nmbQuadru; temp++)
         {
           egUtilDumpAuthQuadruplet(buff, egIe->t.eSecCxtQdQt.authQuadru[temp]);
         }
         for(temp = 0; temp < egIe->t.eSecCxtQdQt.nmbQuin; temp++)
         {
           egUtilDumpAuthQt(buff, egIe->t.eSecCxtQdQt.authQuin[temp]);
         }
         for(temp = 0; temp < 2; temp++)
         {
            sprintf(tmpBuff," drxParam%d :%d\n",(temp+1),egIe->t.eSecCxtQdQt.drxParam[temp]);
            EG_STRCAT(buff,tmpBuff);
         }
         for(temp = 0; temp < 32; temp++)
         {
            sprintf(tmpBuff," nxtHop%d :%d\n",(temp+1),egIe->t.eSecCxtQdQt.nxtHop[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

         sprintf(tmpBuff," ncc:%d\n",egIe->t.eSecCxtQdQt.ncc);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upSubsUEAMBR: %d\n", egIe->t.eSecCxtQdQt.upSubsUEAMBR);
#else
         sprintf( tmpBuff," upSubsUEAMBR: %x\n", egIe->t.eSecCxtQdQt.upSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnSubsUEAMBR: %d\n", egIe->t.eSecCxtQdQt.dnSubsUEAMBR);
#else
         sprintf( tmpBuff," dnSubsUEAMBR: %ld\n", egIe->t.eSecCxtQdQt.dnSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upUsedUEAMBR: %d\n", egIe->t.eSecCxtQdQt.upUsedUEAMBR);
#else
         sprintf( tmpBuff," upUsedUEAMBR: %ld\n", egIe->t.eSecCxtQdQt.upUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnUsedUEAMBR: %d\n", egIe->t.eSecCxtQdQt.dnUsedUEAMBR);
#else
         sprintf( tmpBuff," dnUsedUEAMBR: %ld\n", egIe->t.eSecCxtQdQt.dnUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.eSecCxtQdQt.ueNtwrkCapb.val, 
               egIe->t.eSecCxtQdQt.ueNtwrkCapb.length);
         sprintf(tmpBuff," ueNtwrkCapb.length: %d\n ueNtwrkCapb.val: %s\n", 
               egIe->t.eSecCxtQdQt.ueNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.eSecCxtQdQt.msNtwrkCapb.val, 
               egIe->t.eSecCxtQdQt.msNtwrkCapb.length);
         sprintf(tmpBuff," msNtwrkCapb.length: %d\n msNtwrkCapb.val: %s\n", 
               egIe->t.eSecCxtQdQt.msNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.eSecCxtQdQt.meId.val, 
               egIe->t.eSecCxtQdQt.meId.length);
         sprintf(tmpBuff," meId.length: %d\n meId.val: %s\n", 
               egIe->t.eSecCxtQdQt.meId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUNA:%d\n",egIe->t.eSecCxtQdQt.isUNA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGENA:%d\n",egIe->t.eSecCxtQdQt.isGENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGANA:%d\n",egIe->t.eSecCxtQdQt.isGANA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isINA:%d\n",egIe->t.eSecCxtQdQt.isINA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isENA:%d\n",egIe->t.eSecCxtQdQt.isENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isHNNA:%d\n",egIe->t.eSecCxtQdQt.isHNNA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nccOld:%d\n",egIe->t.eSecCxtQdQt.nccOld);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," ksiOld:%d\n",egIe->t.eSecCxtQdQt.ksiOld);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isNHIOldPres:%d\n",egIe->t.eSecCxtQdQt.isNHIOldPres);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.eSecCxtQdQt.kasmeOld.val,
               egIe->t.eSecCxtQdQt.kasmeOld.length);
         sprintf(tmpBuff," kasmeOld.length: %d\n kasmeOld.val: %s\n",
               egIe->t.eSecCxtQdQt.kasmeOld.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);

         for(temp = 0; temp < 32; temp++)
         {
            sprintf(tmpBuff," nxtHopOld%d :%d\n",(temp+1),egIe->t.eSecCxtQdQt.nxtHopOld[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

#if defined(EG_REL_930) || defined(EG_REL_AC0)

         sprintf( tmpBuff," ueUsageSett: %d\n", egIe->t.eSecCxtQdQt.ueUsageSett);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," vDomPref: %d\n", egIe->t.eSecCxtQdQt.vDomPref);
         EG_STRCAT(buff,tmpBuff);
#endif
         break;
      }

      case EG_UMTS_KEY_QUAD_QUIN:
      {
         sprintf(buff," secMode :%d\n",egIe->t.umtsKeyQdQt.secMode);

         sprintf(tmpBuff," isDRXIPres:%d\n",egIe->t.umtsKeyQdQt.isDRXIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," ksi:%d\n",egIe->t.umtsKeyQdQt.ksi);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuin:%d\n",egIe->t.umtsKeyQdQt.nmbQuin);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," nmbQuadru:%d\n",egIe->t.umtsKeyQdQt.nmbQuadru);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUAMBRIPres:%d\n",egIe->t.umtsKeyQdQt.isUAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isSAMBRIPres:%d\n",egIe->t.umtsKeyQdQt.isSAMBRIPres);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQdQt.ck.val, 
               egIe->t.umtsKeyQdQt.ck.length);
         sprintf(tmpBuff," ck.length: %d\n ck.val: %s\n", 
               egIe->t.umtsKeyQdQt.ck.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQdQt.ik.val, 
               egIe->t.umtsKeyQdQt.ik.length);
         sprintf(tmpBuff," ik.length: %d\n ik.val: %s\n", 
               egIe->t.umtsKeyQdQt.ik.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
       
         for(temp = 0; temp < egIe->t.umtsKeyQdQt.nmbQuadru; temp++)
         {
            egUtilDumpAuthQuadruplet(buff, egIe->t.umtsKeyQdQt.authQuadru[temp]);
         }
         for(temp = 0; temp < egIe->t.umtsKeyQdQt.nmbQuin; temp++)
         {
            egUtilDumpAuthQt(buff, egIe->t.umtsKeyQdQt.authQuin[temp]);
         }
         for(temp = 0; temp < 2; temp++)
         {
            sprintf(tmpBuff," drxParam%d :%d\n",(temp+1),egIe->t.umtsKeyQdQt.drxParam[temp]);
            EG_STRCAT(buff,tmpBuff);
         }

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upSubsUEAMBR: %d\n", egIe->t.umtsKeyQdQt.upSubsUEAMBR);
#else
         sprintf( tmpBuff," upSubsUEAMBR: %x\n", egIe->t.umtsKeyQdQt.upSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnSubsUEAMBR: %d\n", egIe->t.umtsKeyQdQt.dnSubsUEAMBR);
#else
         sprintf( tmpBuff," dnSubsUEAMBR: %ld\n", egIe->t.umtsKeyQdQt.dnSubsUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," upUsedUEAMBR: %d\n", egIe->t.umtsKeyQdQt.upUsedUEAMBR);
#else
         sprintf( tmpBuff," upUsedUEAMBR: %ld\n", egIe->t.umtsKeyQdQt.upUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnUsedUEAMBR: %d\n", egIe->t.umtsKeyQdQt.dnUsedUEAMBR);
#else
         sprintf( tmpBuff," dnUsedUEAMBR: %ld\n", egIe->t.umtsKeyQdQt.dnUsedUEAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQdQt.ueNtwrkCapb.val, 
               egIe->t.umtsKeyQdQt.ueNtwrkCapb.length);
         sprintf(tmpBuff," ueNtwrkCapb.length: %d\n ueNtwrkCapb.val: %s\n", 
               egIe->t.umtsKeyQdQt.ueNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQdQt.msNtwrkCapb.val, 
               egIe->t.umtsKeyQdQt.msNtwrkCapb.length);
         sprintf(tmpBuff," msNtwrkCapb.length: %d\n msNtwrkCapb.val: %s\n", 
               egIe->t.umtsKeyQdQt.msNtwrkCapb.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         
         cmMemcpy( (U8* )tempBuff, egIe->t.umtsKeyQdQt.meId.val, 
               egIe->t.umtsKeyQdQt.meId.length);
         sprintf(tmpBuff," meId.length: %d\n meId.val: %s\n", 
               egIe->t.umtsKeyQdQt.meId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isUNA:%d\n",egIe->t.umtsKeyQdQt.isUNA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGENA:%d\n",egIe->t.umtsKeyQdQt.isGENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isGANA:%d\n",egIe->t.umtsKeyQdQt.isGANA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isINA:%d\n",egIe->t.umtsKeyQdQt.isINA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isENA:%d\n",egIe->t.umtsKeyQdQt.isENA);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," isHNNA:%d\n",egIe->t.umtsKeyQdQt.isHNNA);
         EG_STRCAT(buff,tmpBuff);

#if defined(EG_REL_930) || defined(EG_REL_AC0)

         sprintf( tmpBuff," ueUsageSett: %d\n", egIe->t.umtsKeyQdQt.ueUsageSett);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," vDomPref: %d\n", egIe->t.umtsKeyQdQt.vDomPref);
         EG_STRCAT(buff,tmpBuff);
#endif
         break;
      }

      case EG_AUTH_QUIN:
      {
         egUtilDumpAuthQt(buff, egIe->t.authQt);
         break;
      }

      case EG_AUTH_QUAD:
      {
         egUtilDumpAuthQuadruplet(buff, egIe->t.quadruplet);
         break;
      }

      case EG_CMP_REQ:
      {
         sprintf(buff," crmType: %d\n" , egIe->t.crm.crmType);
        
         cmMemcpy( (U8* )tempBuff,egIe->t.crm.crm.val, egIe->t.crm.crm.length);
         sprintf(tmpBuff," crm.length: %d\n crm.val: %s\n", egIe->t.crm.crm.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_GUTI:
      {
         egUtilDumpSNwk(buff, egIe->t.guti.sNwk);

         sprintf(tmpBuff," mmeGrpId: %d\n", egIe->t.guti.mmeGrpId);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," mmeCode: %d\n", egIe->t.guti.mmeCode);
         EG_STRCAT(buff,tmpBuff);

#ifdef ALIGN_64BIT
         sprintf(tmpBuff," mTmsi: %d\n", egIe->t.guti.mTmsi);
#else
         sprintf(tmpBuff," mTmsi: %ld\n", egIe->t.guti.mTmsi);
#endif
         EG_STRCAT(buff,tmpBuff);

         break;
      }

      case EG_PDUNUM:
      {
         sprintf(buff," nsapi: %d\n", egIe->t.pduNum.nsapi);

         sprintf(tmpBuff," dnLinkSeqNo: %d\n", egIe->t.pduNum.dnLinkSeqNo );
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," upLinkSeqNo: %d\n", egIe->t.pduNum.upLinkSeqNo);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," SNPduNo: %d\n", egIe->t.pduNum.SNPduNo);
         EG_STRCAT(buff,tmpBuff);

         sprintf(tmpBuff," RNPduNo: %d\n", egIe->t.pduNum.RNPduNo);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_F_CONT:
      {
         sprintf(buff," containerType: %d\n", egIe->t.fContainer.containerType);

         cmMemcpy( (U8* )tempBuff,egIe->t.fContainer.fContainer.val, 
               egIe->t.fContainer.fContainer.length);
         sprintf(tmpBuff," fContainer.length: %d\n fContainer.val: %s\n", 
               egIe->t.fContainer.fContainer.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_F_CAUSE:
      {
         sprintf(buff," causeType :%d\n",egIe->t.fCause.causeType);
/*
         cmMemcpy( (U8* )tempBuff, egIe->t.fCause.cause, 
               egIe->t.fCause.cause.length); */
         sprintf(tmpBuff," cause.val: %d\n", 
               egIe->t.fCause.cause);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_TRGTID:
      {
         sprintf(buff," targetType: %d\n", egIe->t.targetId.targetType);

         cmMemcpy( (U8* )tempBuff,egIe->t.targetId.targetId.val, 
               egIe->t.targetId.targetId.length);
         sprintf(tmpBuff," targetId.length: %d\n targetId.val: %s\n", 
               egIe->t.targetId.targetId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_PKTFLOWID:
      {
         sprintf(buff," ebi: %d\n", egIe->t.pktFlowId.ebi);

         sprintf(tmpBuff," pktFlowId: %d\n", egIe->t.pktFlowId.pktFlowId);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_SRCID:
      {
         cmMemcpy( (U8* )tempBuff, egIe->t.srcId.targetCellId.val, 
               egIe->t.srcId.targetCellId.length);
         sprintf(buff," targetCellId.length: %d\n targetCellId.val: %s\n", 
               egIe->t.srcId.targetCellId.length, tempBuff);

         sprintf(tmpBuff," sourceType: %d\n", egIe->t.srcId.sourceType);
         EG_STRCAT(buff,tmpBuff);

         cmMemcpy( (U8* )tempBuff, egIe->t.srcId.sourceId.val, 
               egIe->t.srcId.sourceId.length);
         sprintf(tmpBuff," sourceId.length: %d\n sourceId.val: %s\n", 
               egIe->t.srcId.sourceId.length, tempBuff);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_IP_ADDR:
      {
         sprintf(buff," type: %d\n", egIe->t.ipAddr.type);

         egUtilDumpIpAddr(buff,&egIe->t.ipAddr);
/*
         if(egIe->t.ipAddr.type == CM_TPTADDR_IPV4)
         {
            egUtilDumpIpv4Addr(buff,egIe->t.ipAddr.u.ipv4);
         }
#ifdef EG_IPV6_SUPPORTED
         else if (egIe->t.ipAddr.type == CM_TPTADDR_IPV6)
         {
            egUtilDumpIpv6Addr(buff,egIe->t.ipAddr.u.ipv6);
         }
#endif
*/
         break;
      }

      case EG_AMBR:
      {
#ifdef ALIGN_64BIT
         sprintf( buff," upSubsUEAMBR: %d\n", egIe->t.ambr.uplAPNAMBR);
#else
         sprintf( buff," upSubsUEAMBR: %xn", egIe->t.ambr.uplAPNAMBR);
#endif

#ifdef ALIGN_64BIT
         sprintf( tmpBuff," dnlAPNAMBR: %d\n", egIe->t.ambr.dnlAPNAMBR);
#else
         sprintf( tmpBuff," dnlAPNAMBR: %x\n", egIe->t.ambr.dnlAPNAMBR);
#endif
         EG_STRCAT(buff,tmpBuff);
         break;
      }      

      case EG_UE_TIME_ZONE:
      {
         sprintf( buff," timezone: %d\n", egIe->t.timeZone.timezone);
         sprintf( tmpBuff," dayLiteSavTime: %d\n", egIe->t.timeZone.dayLiteSavTime);
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_TRACE_REF:
      {
         egUtilDumpSNwk(buff, egIe->t.trcRef.sNwk);
#ifdef ALIGN_64BIT
         sprintf( tmpBuff," trcId: %d\n", egIe->t.trcRef.trcId);
#else
         sprintf( tmpBuff," trcId: %ld\n", egIe->t.trcRef.trcId);
#endif
         EG_STRCAT(buff,tmpBuff);
         break;
      }

      case EG_RAB_CONTEXT:
      {
         sprintf( buff," nsapi: %d\n", egIe->t.rabCntxt.nsapi);
         sprintf( tmpBuff," dnlEgUSeqNo: %d\n", egIe->t.rabCntxt.dnlEgUSeqNo);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," uplEgUSeqNo: %d\n", egIe->t.rabCntxt.uplEgUSeqNo);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," dnlPdcpSeqNo: %d\n", egIe->t.rabCntxt.dnlPdcpSeqNo);
         EG_STRCAT(buff,tmpBuff);
         sprintf( tmpBuff," uplPdcpSeqNo: %d\n", egIe->t.rabCntxt.uplPdcpSeqNo);
         EG_STRCAT(buff,tmpBuff);
         break;
      }
#if defined(EG_REL_930) || defined(EG_REL_AC0)
      case EG_UCI:
         {
            egUtilDumpSNwk(buff, egIe->t.sNwk);
            sprintf(tmpBuff,"csgid: %d\n", egIe->t.uci.csgId);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"accMode: %d\n", egIe->t.uci.accMode);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isLCSGPres: %d\n", egIe->t.uci.isLCSGPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isCMIPres: %d\n", egIe->t.uci.isCMIPres);  
            EG_STRCAT(buff,tmpBuff);
            break;
         }

      case EG_CSG_INFO_REP_ACT:
         {
            sprintf(tmpBuff,"isUCICSGPres: %d\n", egIe->t.csgInfoReqAct.isUCICSGPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isUCISHCPres: %d\n", egIe->t.csgInfoReqAct.isUCISHCPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isUCIUHCPres: %d\n", egIe->t.csgInfoReqAct.isUCIUHCPres);  
            EG_STRCAT(buff,tmpBuff);
            break;
         }
#endif
#ifdef EG_REL_AC0
      case EG_ARP:
         {
            sprintf(tmpBuff,"PL: %d\n", egIe->t.arp.PL);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isPCIPres: %d\n", egIe->t.arp.isPCIPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isPCIPres: %d\n", egIe->t.arp.isPVIPres);  
            EG_STRCAT(buff,tmpBuff);
            break;
         }
      case EG_EPC_TMR:
         {
            sprintf(tmpBuff,"tmrUnit: %d\n", egIe->t.epcTmr.tmrUnit);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"tmrVal: %d\n", egIe->t.epcTmr.tmrVal);  
            EG_STRCAT(buff,tmpBuff);
            break;
         }

      case EG_THROTTLING:
         {
            sprintf(tmpBuff,"thrDelayValue: %d\n", egIe->t.egThrot.thrDelayValue);
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"thrDelayUnit: %d\n", egIe->t.egThrot.thrDelayUnit);
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"thrFactor: %d\n", egIe->t.egThrot.thrFactor);
            EG_STRCAT(buff,tmpBuff);
            break;
          }
      case EG_ADD_MM_CNTXT_SRVCC:
         {

            EG_STRCAT(buff,tmpBuff);
            cmMemcpy( (U8* )tempBuff,egIe->t.mmCntxtForSrvcc.classMk2, egIe->t.mmCntxtForSrvcc.classMk2Len);
            sprintf(tmpBuff,"classMk2 val: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);
            cmMemcpy( (U8* )tempBuff,egIe->t.mmCntxtForSrvcc.classMk3, egIe->t.mmCntxtForSrvcc.classMk3Len);
            sprintf(tmpBuff,"classMk3 val: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);
           for(count = 0;  count < egIe->t.mmCntxtForSrvcc.numOfcodecList;count++)
             {
            cmMemset(tempBuff, 0, (EG_UTIL_PRNT_BUFF_SIZE-5));
            sprintf(tmpBuff,"egIe->t.mmCntxtForSrvcc.codec[%d].sysId :%d\n", count,egIe->t.mmCntxtForSrvcc.codec[count].sysId);  
            EG_STRCAT(buff,tmpBuff);

            cmMemset(tempBuff, 0, (EG_UTIL_PRNT_BUFF_SIZE-5));
            cmMemcpy( (U8* )tempBuff,egIe->t.mmCntxtForSrvcc.codec[count].bitMap, egIe->t.mmCntxtForSrvcc.codec[count].bitMapLen);
            sprintf(tmpBuff,"bitmap val: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);


             }
            break; 
         }


      case EG_MDT_CONF:
         {

            sprintf(tmpBuff,"Job type: %d\n", egIe->t.mdtConf.jobType);  
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"listOfMsmt: %s\n", egIe->t.mdtConf.listOfMsmt);  
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"rptTrigger: %d\n", egIe->t.mdtConf.rptTrigger);  
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"rptInterval: %d\n",egIe->t.mdtConf.rptInterval);  
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"rptAmount: %d\n", egIe->t.mdtConf.rptAmount);  
            EG_STRCAT(buff,tmpBuff);


            sprintf(tmpBuff,"evtThrRsrp: %d\n",egIe->t.mdtConf.evtThrRsrp);  
            EG_STRCAT(buff,tmpBuff);

            sprintf(tmpBuff,"evtThrRsrq: %d\n", egIe->t.mdtConf.evtThrRsrq);  
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.mdtConf.areaScope.val, egIe->t.mdtConf.areaScope.length);
            sprintf(tmpBuff,"area scope val: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);


            break; 
         }

      case EG_CNG_TO_RPT_FLG:
         {
            sprintf(tmpBuff,"isSNCRPres: %d\n",egIe->t.cngRptFlg.isSNCRPres);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"isTZCRPres: %d\n", egIe->t.cngRptFlg.isTZCRPres);  
            EG_STRCAT(buff,tmpBuff);
            break;
         }
#endif
#if defined(EG_REL_930) || defined(EG_REL_AC0)
      case EG_MBMS_IPMCAST_DIST:
         {
            sprintf(tmpBuff,"cteidDat: %d\n",egIe->t.mbmsIPMCastDist.cteidDat);  
            EG_STRCAT(buff,tmpBuff);
            sprintf(tmpBuff,"addressType: %d\n",egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type);  
            EG_STRCAT(buff,tmpBuff);
            switch (egIe->t.mbmsIPMCastDist.ipMCastDistAddr.type)
            {
               case CM_TPTADDR_IPV4:
                  {
                     sprintf(tmpBuff,"ipv4 address: %x\n",egIe->t.mbmsIPMCastDist.ipMCastDistAddr.u.ipv4);  
                     EG_STRCAT(buff,tmpBuff);
                     break;
                  } /* end of case */
#ifdef EG_IPV6_SUPPORTED
               case CM_TPTADDR_IPV6:
                  {
                     sprintf(tmpBuff,"ipv6 address: %x\n",egIe->t.mbmsIPMCastDist.ipMCastDistAddr.u.ipv6);  
                     EG_STRCAT(buff,tmpBuff);
                     break;
                  } /* end of case */
#endif /* EG_IPV6_SUPPORTED */

                  sprintf(tmpBuff,"addressType: %x\n",egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type);  
                  EG_STRCAT(buff,tmpBuff);
            }

            switch (egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.type)
            {
               case CM_TPTADDR_IPV4:
                  {
                     sprintf(tmpBuff,"ipv4 address: %x\n",egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.u.ipv4);  
                     EG_STRCAT(buff,tmpBuff);
                     break;
                  } /* end of case */
#ifdef EG_IPV6_SUPPORTED
               case CM_TPTADDR_IPV6:
                  {
                     sprintf(tmpBuff,"ipv6 address: %x\n",egIe->t.mbmsIPMCastDist.ipMCastSrcAddr.u.ipv6);  
                     EG_STRCAT(buff,tmpBuff);
                     break;
                  } /* end of case */
#endif /* EG_IPV6_SUPPORTED */
            }

            sprintf(tmpBuff,"mbmsHCInd: %x\n",egIe->t.mbmsIPMCastDist.mbmsHCInd);  
            EG_STRCAT(buff,tmpBuff);
            break;
         }
#endif
      default:
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG047, ERRZERO,
                  "egUtilDumpIe: Ie datatype is not valid.\n");
            RETVALUE(EG_UTIL_ERR_INV_IE_DATATYPE);
         }

   }
   EG_STRCAT(prnBuff, buff);
#endif /* EG_PRNT_DATA */


#ifdef LEG_FILE_LOG
   fprintf(fp, "%s\n", prnBuff);
   nmbLines += 5;
#else
   SPrint(prnBuff);
   SPrint("\n");
#endif /* end of LEG_FILE_LOG */

   /* return value not matching error */
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    egUtilDelIe
*
*    Desc:   Method is used to delete an IE from the message
*
*    Ret:    ROK - if the diameter header is dumped successfully
*            IE_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef ANSI
PUBLIC S16 egUtilDelIe
(
CmLListCp      *ieList,         /* IE list from which the IE has to be deleted */
EgIe           *egIe,           /* IE to be deleted */
Bool           allOccr          /* flag specifying whether to delete all occr or not */
)
#else
PUBLIC S16 egUtilDelIe(ieList, egIe, allOccr)
CmLListCp      *ieList;         /* IE list from which the IE has to be deleted */
EgIe           *egIe;           /* IE to be deleted */
Bool           allOccr;         /* flag specifying whether to delete all occr or not */
#endif
{
   S16 retValue = 0;                   /* to hold the return value of sub methods */
 

#ifdef EG_PHASE2
   CmLList *node = NULLP;              /* to hold the temporary node address */
   /*********************************************************
    * this will hold the address of IE in the original list *
    *********************************************************/
   EgIe *ie = NULLP;
#endif

   /*-- eg002.201 : fixed warning due to misplaced macro --*/
   EG_TRC2(egUtilDelIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !egIe)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

#ifdef EG_PHASE2
   /***************************************************************
    * check whether all occurrences of the IE are to be deleted   *
    * If so, iterate through the list delete all the occurrences  *
    * of IE                                                       *
    ***************************************************************/
   if (allOccr)
   {
      /*************************************************
       * iterate through the list and for each ie,     *
       * if the ie type matches, then delete the IE    *
       * from the list                                 *
       *************************************************/
      for (ie = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
           ie != NULLP;
           ie = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node))
      {
         /****************************************************
          * if the ie type doesn't matches, then continue in *
          * loop to get the next ie                          *
          ****************************************************/
         if ((ie->ieType != egIe->ieType) && (ie->ieInst != egIe->ieInst))
         {
            continue;
         }

         /************************************************
          * check whether the IE is grouped IE           *
          * If so drop the IE list of the grouped IE     *
          ************************************************/   
         if (ie->dataType == EG_GROUPED)
         {
             if ( (retValue = egUtilDropIeList(&ie->t.grouped.gIeList)) != ROK)
             {
               EGUTILLOGERROR(ERRCLS_DEBUG, EEG048, ERRZERO,
               "egUtilDelIe: Deleting grouped IE from the IE list failed.\n");
               RETVALUE(retValue);
            }
         }

         /*********************************
          * delete the node from the list *
          *********************************/
         if (!cmLListDelFrm(ieList, &ie->node))
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG049, ERRZERO,
               "egUtilDelIe: Deleting node from the IE list failed.\n");
            RETVALUE(RFAILED);
         }
      }
   }
   else
   {
#endif

      /*********************************************
       * check whether the IE is grouped IE        *
       * If so drop the IE list of the grouped IE  *
       *********************************************/
      if (egIe->dataType == EG_GROUPED)
      {
          if ( (retValue = egUtilDropIeList(&egIe->t.grouped.gIeList)) != ROK)
          {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG050, ERRZERO,
            "egUtilDelIe: Deleting grouped IE from the IE list failed.\n");
            RETVALUE(retValue);
         }
      }
      /*********************************
       * delete the node from the list *
       *********************************/
      if (!cmLListDelFrm(ieList, &egIe->node))
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG051, ERRZERO,
            "egUtilDelIe: Deleting node from the IE list failed.\n");
         RETVALUE(RFAILED);
      }

#ifdef EG_PHASE2
   }
#endif

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
*
*    Fun:    egUtilDelGrpIe
*
*    Desc:   This is used to get the IE list at a particular level
*
*    Ret:    ROK - if the IE list at a particular level is retreived successfully
*            RFAILED -  if retreiving the IE list at a particular level fails
*            IE_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*
*    Notes:  n/a
*
*    File:   eg_util.c
*
***************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUtilDelGrpIe
(
CmLListCp      *ieList,       /* IE list from where to start the traversing */
EgIeLevelInfo  *ieLvlInfo,    /* IE level information */
CmLListCp      **trgtList,    /* target IE list */
U16            strtLvl,       /* start the searhcing from this level */
U16            *trgtLvl       /* target IE list's level */
)
#else
PRIVATE S16 egUtilDelGrpIe(ieList, ieLvlInfo, trgtList, strtLvl, trgtLvl)
CmLListCp      *ieList;        /* IE list from where to start the traversing */
EgIeLevelInfo  *ieLvlInfo;     /* IE level information */
CmLListCp      **trgtList;     /* target IE list */
U16            strtLvl;        /* start the searhcing from this level */
U16            *trgtLvl;       /* target IE list's level */
#endif
{
   S16            retValue = 0;        /* to hold the return value of sub methods */
   EgIe          *egTempIe = NULLP;    /* to hold the temporary IE */

#ifdef EG_PHASE2
   U16            tmpStrtLvl=0;        /* start the searhcing from this level */

   /******************************************************************
    * temporary variable to hold the list before calling recursively *
    ******************************************************************/
   CmLListCp      *tempIeList = NULLP; 
#endif /* EG_PHASE2 */

   U16            posOfIe = 0;         /* to hold the position of the IE */
   /******************************************************
    * this will hold the current level in the grouped IE *
    ******************************************************/
   U16            curLvl = strtLvl;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUtilDelGrpIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !ieLvlInfo || !trgtList || !trgtLvl || 
       (ieLvlInfo->level > EG_MAX_IE_LEVEL) || (strtLvl > EG_MAX_IE_LEVEL))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*********************************************************
    * initially the given ieList will be the target list,   *  
    * this will be helpful when the user gives the level    *
    * information with both the IE type and position        *
    * reference flags as false.  In such case there is      *
    * no way to progress in the list into the further       *
    * grouped lists                                         *
    *********************************************************/
   *trgtList = ieList;

   /****************************************************
    * copy the iterated IE level into the target level *
    ****************************************************/
   *trgtLvl = strtLvl;

   /****************************************************************
    * in a loop go through all the levels, and get the target list *
    ****************************************************************/
   while ((*trgtLvl < ieLvlInfo->level) && (strtLvl < EG_MAX_IE_LEVEL))
   {
#ifdef EG_PHASE2
      /**********************************************
       * get the target IE as per the given IE info *
       **********************************************/
      switch ( (retValue = egUtilGetTrgtIe(ieList, &ieLvlInfo->ieInfo[strtLvl], &egTempIe, &posOfIe)))
      {
         /************************************
          * in case of ok or invalid IE info *  
          * we will take the ie list given   *
          * as the target list and continue  *
          ************************************/
         case ROK:
         {
            break;
         }

         /******************************************************
          * incase of IE_UTIL_ERR_INV_IE_INFO, check whether   *  
          * there is one more level in the ieLvlInfo.          *
          * In such case, the given information is wrong,      *
          * so return invalid level info error.  If there      *
          * no further level, then return the same error       * 
          ******************************************************/
         case EG_UTIL_ERR_INV_IE_INFO:
         {
            if (strtLvl + 1 >= ieLvlInfo->level)
            {
               RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
            }
            else
            {
               RETVALUE(EG_UTIL_ERR_INV_IE_INFO);
            }

         }

         default:
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG052, ERRZERO,
               "egUtilDelGrpIe: Getting the target node from the IE list failed.\n");
            RETVALUE(retValue);
         }
      }

      /******************************************************************
       * if there is one more level than the current level              *
       * and the current target ie which has matched the criteria       *
       * of the given IE information is of type grouped, then it        *
       * means that we need to jump into the nested grouped IE.         *
       * So call the same method with IE list in the target IE,         *
       * start level as the strtLvl + 1.                                *
       * If there is one more level than the current level              *
       * and the current target IE is not an grouped IE, then return    *
       * IE_UTIL_ERR_INV_LEVEL_INFO as error                            *
       ******************************************************************/      
      if (strtLvl + 1 <= ieLvlInfo->level && egTempIe->dataType == EG_GROUPED)
      {
         /**********************************
          * take a copy of the target list *
          **********************************/
         tempIeList = *trgtList;

         /***************************************************
          * call the same method with the level higher than *
          * the current level                               *
          ***************************************************/
         tmpStrtLvl = strtLvl + 1;
         switch ( (retValue = egUtilDelGrpIe(&egTempIe->t.grouped.gIeList, 
                                       ieLvlInfo, trgtList,tmpStrtLvl , trgtLvl)))
         {
            case ROK:
            {
               /*********************************
                * delete the node from the list *
                *********************************/
               if ( (retValue = egUtilDelIe(tempIeList, egTempIe, FALSE)) != ROK)
               {
                  EGUTILLOGERROR(ERRCLS_DEBUG, EEG053, ERRZERO,
                                 "egUtilDelGrpIe: Deleting node from the IE list failed.\n");
                  RETVALUE(RFAILED);
               }

               break;
            }
            case EG_UTIL_ERR_INV_IE_INFO:
            {
               /*************************
                * return the same error *
                *************************/
               RETVALUE(EG_UTIL_ERR_INV_IE_INFO);
            }

            default:
            {
               EGUTILLOGERROR(ERRCLS_DEBUG, EEG054, ERRZERO,
               "egUtilDelGrpIe: Getting the target node from the IE list failed.\n");
               RETVALUE(retValue);
            }
         }
      }
      else if (strtLvl + 1 <= ieLvlInfo->level && egTempIe->dataType != EG_GROUPED)
      {
         RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
      }

      /*****************************
       * increment the start level *
       *****************************/
      strtLvl++;

#endif /* EG_PHASE2 */
   }

   /******************************************************************
    * if we are at the end level, search for the given node in the   *
    * list and delete the node from the list                         *
    ******************************************************************/
   if ((curLvl == ieLvlInfo->level) && (curLvl < EG_MAX_IE_LEVEL))
   {
      /************************************************************
       * get the target IE from the list as per the given IE info *
       ************************************************************/
      if ( (retValue = egUtilGetTrgtIe(*trgtList, &ieLvlInfo->ieInfo[curLvl], &egTempIe, &posOfIe)) != ROK)
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG055, ERRZERO,
                        "egUtilDelGrpIe: Getting the target IE from the message failed.\n");
         RETVALUE(retValue);
      }

      /*********************************
       * delete the node from the list *
       *********************************/
      if ( (retValue = egUtilDelIe(*trgtList, egTempIe, FALSE)) != ROK)
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG056, ERRZERO,
                        "egUtilDelGrpIe: Deleting node from the IE list failed.\n");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}


/***********************************************************************************************
*
*    Fun:    EgUtilGmDelGrpIe
*
*    Desc:   Method is used to delete an IE from the message which is at a particular
*            level
*
*    Ret:    ROK - if the IE is deleted successfully
*            RFAILED - if deleting IE fails
*            IE_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*            IE_UTIL_ERR_INV_PARAM - if the given message or IE level information id are invalid
*            Return value of the child method (if any)
*
*    Notes:  
*
*    File:   eg_util.c
*
************************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGmDelGrpIe
(
EgMsg         *egMsg,        /* Diameter message */
EgIeLevelInfo *ieLvlInfo     /* IE level information */
)
#else
PUBLIC S16 EgUtilGmDelGrpIe(egMsg, ieLvlInfo)
EgMsg         *egMsg;        /* Diameter message */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
#endif
{
   /***************************************************************
    * this will hold the IE list, where the IE has to be appended *
    ***************************************************************/
   CmLListCp *ieList = NULLP;          

   S16 retValue = 0;                   /* to hold the return value of sub methods */ 

   /**************************************************
    * this is used to hold the returned target level *
    **************************************************/
   U16 trgtLvl = 0;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmDelGrpIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !ieLvlInfo || ieLvlInfo->level > EG_MAX_IE_LEVEL)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***************************************************
    * get the target IE list as per the given IE info *
    ***************************************************/
   if ( (retValue = egUtilDelGrpIe(&egMsg->egIeList, ieLvlInfo, &ieList, EG_IE_LEVEL_ZERO, &trgtLvl)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG057, ERRZERO,
               "EgUtilGmDelGrpIe: Getting the target list from the message failed.\n");
      RETVALUE(retValue);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/************************************************************************************************
*
*    Fun:    EgUtilGmDelIe
*
*    Desc:   Method is used to delete an IE from the message which is at a particular
*            level
*
*    Ret:    ROK - if the IE is deleted successfully
*            RFAILED - if deleting IE fails
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*            EG_UTIL_ERR_INV_PARAM - if the given message or IE level information id are invalid
*            Return value of the child method (if any)
*
*    Notes:  
*
*    File:   eg_util.c
*
************************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUtilGmDelIe
(
EgMsg      *egMsg,         /* eGtp message */
EgIeLevelInfo *ieLvlInfo,    /* IE level information */
Bool           allOccr         /* flag specifying whether to delete all occr or not */
)
#else
PUBLIC S16 EgUtilGmDelIe(egMsg, ieLvlInfo, allOccr)
EgMsg      *egMsg;         /* eGtp message */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
Bool           allOccr;        /* flag specifying whether to delete all occr or not */
#endif
{
   /***************************************************************
    * this will hold the IE list, where the IE has to be appended *
    ***************************************************************/
   CmLListCp *ieList = NULLP;

   S16 retValue = 0;                   /* to hold the return value of sub methods */

   U16 trgtLvl = 0;                    /* to hold the returned target level */

   EgIe *egIe = NULLP;                 /* this will hold the IE node address */

   U16 posOfIe = 0;                    /* to hold the position of the IE */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmDelIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !ieLvlInfo || ieLvlInfo->level > EG_MAX_IE_LEVEL)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***************************************************
    * get the target IE list as per the given IE info *
    ***************************************************/
   if ( (retValue = egUtilGetTrgtList(&egMsg->egIeList, ieLvlInfo, &ieList, EG_IE_LEVEL_ZERO, &trgtLvl)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG058, ERRZERO,
               "EgUtilGmDelIe: Getting the target list from the message failed.\n");
      RETVALUE(retValue);
   }

   /************************************************************
    * get the target IE from the list as per the given IE info *
    ************************************************************/
   if ( (retValue = egUtilGetTrgtIe(ieList, &ieLvlInfo->ieInfo[trgtLvl], &egIe, &posOfIe)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG059, ERRZERO,
               "EgUtilGmDelIe: Getting the target IE from the message failed.\n");
      RETVALUE(retValue);
   }

   /*********************************
    * delete the node from the list *
    *********************************/
   if ( (retValue = egUtilDelIe(ieList, egIe, allOccr)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG060, ERRZERO,
            "EgUtilGmDelIe: Deleting node from the IE list failed.\n");
      RETVALUE(RFAILED);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/*
*
*    Fun:    EgUtilGmGetIeOccrCnt
*
*    Desc:   Method is used to get the number of occurrences
*            of an IE at a particular level
*
*    Ret:    ROK - if the ie list is duplicated successfully
*            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
*            EG_UTIL_ERR_INV_PARAM - if the given message or level information are invalid
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUtilGmGetIeOccrCnt
(
EgMsg      *egMsg,         /* Diameter message */
EgIeLevelInfo *ieLvlInfo,    /* IE level information */
/*eg012.201:signedness warning fixed */
U16            *ieOccrCnt     /* IE count at a level */
)
#else
PUBLIC S16 EgUtilGmGetIeOccrCnt(egMsg, ieLvlInfo, ieOccrCnt)
EgMsg      *egMsg;         /* Diameter message */
EgIeLevelInfo *ieLvlInfo;    /* IE level information */
/*eg012.201:signedness warning fixed */
U16            *ieOccrCnt;    /* IE count at a level */
#endif
{
   /* this is used to hold the return value of sub methods */
   S16 retValue = 0;

   /* this is used to hold the returned target level */
   U16 trgtLvl = 0;

   /* this will hold the address of IE in the original list */
   EgIe *egIe = NULLP;

   /* this is used to hold the temporary node address */
   CmLList *node = NULLP;

   /* this will hold the target IE list */
   CmLListCp *ieList = NULLP;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUtilGmGetIeOccrCnt) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /* sanity check */
   if (!egMsg || !ieLvlInfo || !ieOccrCnt || ieLvlInfo->level > EG_MAX_IE_LEVEL)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   } /* end of if egMsg or ieLvlInfo or ieOccrCnt are not valid pointers */

   /* get the target IE list as per the given IE info */
   if ( (retValue = egUtilGetTrgtList(&egMsg->egIeList, ieLvlInfo, &ieList, EG_IE_LEVEL_ZERO, &trgtLvl)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG061, ERRZERO,
               "EgUtilGmGetIeOccrCnt: Getting the target list from the message failed.\n");
      RETVALUE(retValue);
   } /* if getting the target list from the message as per the given IE info fails */

   /* initialize the occrrence count as zero initially */
   *ieOccrCnt = 0;

   /* list and if the ie code is present in the level
    * information, then iterate through the list of the
    * IEs in the target list and if the IE code is matching
    * with the IE given, increment the count given.
    */
   if (ieLvlInfo->ieInfo[trgtLvl].ieTypePres == TRUE)
   {
      /* iterate through the list */
      for (egIe = (EgIe* ) CM_LLIST_FIRST_NODE(ieList, node);
           egIe != NULLP;
           egIe = (EgIe* ) CM_LLIST_NEXT_NODE(ieList, node))
      {
         /* if the IE code is matching, then increment
          * the occrrence count
          */
         /*eg012.201:added instance value check,fix for  ccpu00130411  */
         if ((egIe->ieType == ieLvlInfo->ieInfo[trgtLvl].ieType) && ( egIe->ieInst == ieLvlInfo->ieInfo[trgtLvl].ieInst ) )
         {
            (*ieOccrCnt)++;
         } /* end of if code is matching */
      } /* end of loop until the occurrence count is matching or end of list is reached */
   } /* end of if ieType is present in that level */

   /* return success */
   RETVALUE(ROK);
} /* end of EgUtilGmGetIeOccrCnt */










#ifdef NEXT

/************************************************************************
 *   Fun:  egGetDataFrmIeType
 *
 *   Desc:  This Functions returns the Ie Data Type based 
 *          on the IE Type.
 *
 *   Ret:   ROK  - for Success
 *          RFAILED - for Failure
 *
 *   Notes: None
 *
 *   File:  eg_util.c
 ***********************************************************************/
#ifdef ANSI
PUBLIC U8 egGetDataFrmIeType
(
U8   ieType           /* refers to the type of the IE to get the Data Type */
)
#else
PUBLIC U8 egGetDataFrmIeType(ieType)
U8   ieType;          /* refers to the type of the IE to get the Data Type */
#endif
{
   U8  posIdx=0; 
   
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egGetDataFrmIeType) 

   /****************************************
    * Get the actual Position Index of the *
    * Ie Type from the EDM Control Block   *
    ****************************************/
   posIdx = edmCb.egIeCb.posIdx[ieType]; 
   if( posIdx != 0)
   {
      RETVALUE( edmCb.egIeCb.ieEntry[posIdx-1].dataType); 
   }
   else
   {
      RETVALUE(RFAILED);
   }
} 
#endif /* NEXT */
#endif /* End of EGTP_C */

#ifdef EG_MULTI_IP
/**********************************************************************
*       Fun:  egTptGetTsapFrmAddr
*
*       Desc: This function is used to obtain TSapCb from a given IP addr
*
*       Ret:   ROK   on success
*              EGT_ERR_MSG_INVOVR_LEN_PBMSG on error
*
*       Notes: --
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptGetTsapFrmAddr
(
 CmTptAddr   tptSrvAddr,                  /* Server Address --*/
 EgTSapCb**   tSapCb
)
#else
PUBLIC S16 egTptGetTsapFrmAddr(tptSrvAddr, tSapCb)
 CmTptAddr   tptSrvAddr;                  /* Server Address  --*/
 EgTSapCb**   tSapCb;
#endif
{
   CmTptAddr   *localAddrPtr = NULLP;     /* Ptr to Local Address */
   Bool         addrMatch    = FALSE;
   U16          idx;

   EG_TRC2(egTptGetTsapFrmAddr);

   for (idx = 0;idx < egCb.genCfg.maxNmbTSaps;idx++)
   {
      *tSapCb = egCb.tSapLst[idx]; 
      if ((*tSapCb) == NULL)
         continue;

      localAddrPtr= &(*tSapCb)->tSapIpAddr;

      egCmpTptAddr(localAddrPtr, &tptSrvAddr, &addrMatch);
      if (addrMatch )
      {
         /*-- Retrun TSapCb if the match is found --*/
         RETVALUE (ROK);
      }
   }

   EG_DBG_INFO(0,0, (egp, "\n TSapCb not found"));
   RETVALUE (RFAILED);
} /*egTptGetTsapFrmAddr*/
#endif /* EG_MULTI_IP */




#ifdef EGTP_U
/* 
 * ***  FUNCTION  **************************************************************
 *
 *         Name: egUUCmpTptAddr 
 *
 *  Description:  
 *
 *       Return:
 *
 * *****************************************************************************
 */
#ifdef ANSI
PUBLIC S16 egUUtilCmpTptAddr
(
CmTptAddr *srcAddr,
CmTptAddr *destAddr,
U16 *port
)
#else
PUBLIC S16 egUUtilCmpTptAddr(srcAddr, destAddr, port)
CmTptAddr *srcAddr;
CmTptAddr *destAddr;
U16 *port;
#endif
{
#ifdef IPV6_SUPPORTED
   S16 retVal = 0;
#endif

   EG_TRC2(egUUtilCmpTptAddr);

   if (srcAddr->type != destAddr->type)
   {
      RETVALUE (RFAILED);
   }

   if ((srcAddr->type != CM_TPTADDR_IPV4) && (srcAddr->type != CM_TPTADDR_IPV6))
   {
      RETVALUE (RFAILED);
   }

   if (srcAddr->type == CM_TPTADDR_IPV4)
   {
      if (srcAddr->u.ipv4TptAddr.address ^ destAddr->u.ipv4TptAddr.address)
         RETVALUE(RFAILED);
      else
         *port = destAddr->u.ipv4TptAddr.port;
   }
#ifdef IPV6_SUPPORTED
   else if (srcAddr->type == CM_TPTADDR_IPV6)
   {
      if ((retVal = cmMemcmp (srcAddr->u.ipv6TptAddr.ipv6NetAddr,
                              destAddr->u.ipv6TptAddr.ipv6NetAddr,
                              sizeof (CmIpv6NetAddr))))
         RETVALUE(retVal);
      else
         *port = destAddr->u.ipv6TptAddr.port;
   }
#endif

   RETVALUE(ROK);
}

/***********************************************************************************
 *
 *    Fun:    EgUUtilAllocGmMsg
 *
 *    Desc:   Allocate memory for the GTP-U message and initialize
 *
 *    Ret:    ROK  - if memory allocation and initialization is success
 *            EG_UTIL_ERR_MEM_ALLOC_FAILED - in case of memory allocation fails
 *            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
 *
 *    Notes:  None
 *
 *    File:   eg_util.c
 *
 *************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUUtilAllocGmMsg
(
   EgUMsg **egMsg,    /* GTP-U Message */
   Mem    *memInfo    /* memory information */
 )
#else
PUBLIC S16 EgUUtilAllocGmMsg(egMsg, memInfo)
   EgUMsg **egMsg;    /* GTP-U Message */
   Mem    *memInfo;   /* memory information */
#endif
{
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUUtilAllocGmMsg) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /******************************************************
    * allocate memory for the GTP-U message of size EgUMsg *
    ******************************************************/
   if (cmAllocEvnt(sizeof(EgUMsg), EG_UTIL_MEM_SIZE, memInfo, (Ptr* )egMsg) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG062, ERRZERO,
            "EgUUtilAllocGmMsg: Allocating memory for the GTP-U Message failed.\n");
      RETVALUE(EG_UTIL_ERR_MEM_ALLOC_FAILED);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***********************************************************************************
 *
 *    Fun:    EgUUtilDeAllocGmMsg
 *
 *    Desc:   De-Allocate memory for the GTP-U message and uninitialize
 *
 *    Ret:    ROK  - if memory de-allocation and initialization is success
 *            RFAILED - in case of memory de-allocation fails
 *            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
 *            Return value of the child methods (if any)
 *
 *    Notes:  The GTP-U message will be destoryed in this method.  The
 *            memory allocated either for the message or for all the IEs
 *            inside the message (of any level) will be deallocated inside
 *            this method.
 *
 *    File:   eg_util.c
 *
 ***********************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUUtilDeAllocGmMsg
(
   EgUMsg **egMsg     /* GTP-U Message */
 )
#else
PUBLIC S16 EgUUtilDeAllocGmMsg(egMsg)
   EgUMsg **egMsg;    /* GTP-U Message */
#endif
{
   EgUIe   *ie = NULLP;    /* temporary ie to hold the element in the list */
   CmLList *node = NULLP;  /* temporary node to walk through the list */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUUtilDeAllocGmMsg) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   if (!egMsg || !(*egMsg))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   if((*egMsg)->msgHdr.msgType != EGT_GTPU_MSG_GPDU)
   {
      /***************************************************************
       * In a loop iterate through the list and then delete the node * 
       ***************************************************************/
      for (ie = (EgUIe* )CM_LLIST_FIRST_NODE(&(*egMsg)->u.egIeList, node); ie != NULLP;
            ie = (EgUIe* )CM_LLIST_NEXT_NODE(&(*egMsg)->u.egIeList, node))
      {
         /*********************************
          * delete the node from the list *
          *********************************/
         if (!cmLListDelFrm(&(*egMsg)->u.egIeList, node))
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG063, ERRZERO,
                  "EgUUtilDeAllocGmMsg: Deleting IE has failed....\
                  continuing with dropping the message.\n");
            break;
         }
      }
   }
   else
   {
      if((*egMsg)->u.mBuf)
      {
         SPutMsg((*egMsg)->u.mBuf);
      }
   }

   /*****************************************
    * De-allocate memory of the gtp message *
    *****************************************/
   cmFreeMem(&(*egMsg)->memCp);

   /************************************
    * Initialize the pointer with zero *
    ************************************/
   *egMsg = NULLP;

   RETVALUE(ROK);
}

/****************************************************************************************
 *
 *    Fun:    EgUUtilGmAppendIe
 *
 *    Desc:   Method is used to append an IE to the GTP-Umessage
 *
 *    Ret:    ROK - if the IE is appended successfully
 *            RFAILED - if appending IE fails
 *            EG_UTIL_ERR_MEM_ALLOC_FAILED - if memory allocation for the new IE fails
 *            EG_UTIL_ERR_INV_PARAM - if the given message or ie are invalid
 *            Return value of the child method (if any)
 *
 *    Notes:  This method will allocate memory separately for the new IE that is
 *            being inserted
 *
 *    File:   eg_util.c
 *
 *****************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUUtilGmAppendIe
(
   EgUMsg   *egMsg,        /* GTP-U message */
   EgUIe    *egIe          /* IE that has to be appended */
 )
#else
PUBLIC S16 EgUUtilGmAppendIe(egMsg, egIe)
   EgUMsg   *egMsg;        /* GTP-U message */
   EgUIe    *egIe;         /* IE that has to be appended */
#endif
{
   S16   retValue = ROK;   /* to hold the return value of sub methods */       
   EgUIe *ie = NULLP;      /* ie that has to be appended to the list */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUUtilGmAppendIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !egIe)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***********************************************************
    * make the new copy of the given IE to append to the list *
    ***********************************************************/
   if ( (retValue = egUUtilDupIe(&egMsg->memCp, egIe, &ie)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG064, ERRZERO,
            "EgUUtilGmAppendIe: Making the new IE from the given IE failed.\n");
      RETVALUE(retValue);
   }

   /*****************************************
    * append the IE to the tail of the list *
    *****************************************/
   cmLListAdd2Tail(&egMsg->u.egIeList, &ie->node);

   RETVALUE(retValue);
}

/********************************************************************************************
 *
 *    Fun:    egUUtilGetTrgtIe
 *
 *    Desc:   This is used to get a particular IE from an IE list
 *
 *    Ret:    ROK - if the IE in IE is found successfully
 *            RFAILED -  if finding the IE in IE list fails
 *            EG_UTIL_ERR_INV_PARAM - if the given ielist or ie info or target ie are invalid
 *
 *    Notes:  n/a
 *
 *    File:   eg_util.c
 *
 ***********************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUUtilGetTrgtIe
(
   CmLListCp  *ieList,       /* IE list from where to retreive the IE */
   U8       ieType,        /* Type of the IT tobe retreived */
   U8       occrCnt,       /* Occurance of the IE */
   EgUIe      **egIe        /* target IE */
)
#else
PRIVATE S16 egUUtilGetTrgtIe(ieList, ieType, occrCnt, egIe)
   CmLListCp   *ieList;      /* IE list from where to retreive the IE */
   U8        ieType;       /* Type of the IT tobe retreived */
   U8        occrCnt;      /* Occurance of the IE */
   EgUIe       **egIe;       /* target IE */
#endif
{
   U16      cnt = 0;          /* used to hold the occurrence count */
   CmLList  *node = NULLP;    /* used to hold the temporary node address */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUUtilGetTrgtIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList || !occrCnt || (occrCnt > ieList->count))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /***************************************** 
    * iterate through the list and for each *
    * node increment the iePos value        *
    *****************************************/
   for (*egIe = (EgUIe* ) CM_LLIST_FIRST_NODE(ieList, node); (*egIe != NULLP);
         *egIe = (EgUIe* ) CM_LLIST_NEXT_NODE(ieList, node))
   {
      /**************************************************
       * if the IE type is matching, then increment cnt *
       **************************************************/
      if ((*egIe)->ieType == ieType)
      {
         cnt++;
         /********************************
          * occurrence count is matching *
          ********************************/
         if (cnt == occrCnt)
         {
            RETVALUE(ROK);
         }
      }

   }

   EGUTILLOGERROR(ERRCLS_DEBUG, EEG065, ERRZERO,
         "egUUtilGetTrgtIe: Targer IE is not found.\n");
   RETVALUE(EG_UTIL_ERR_INV_LEVEL_INFO);
}

/*******************************************************************************************
 *
 *    Fun:    egUUtilDupIe
 *
 *    Desc:   Method is used to duplicate a GTP-U ie
 *
 *    Ret:    ROK - if the ie list is duplicated successfully
 *            EG_UTIL_ERR_MEM_ALLOC_FAILED - if memory allocation for the new ie fails
 *            EG_UTIL_ERR_INV_PARAM - if the given message or memory information are invalid
 *
 *    Notes:  N/A
 *
 *    File:   eg_util.c
 *
 *******************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUUtilDupIe
(
   CmMemListCp    *memCp,          /* memory information */
   EgUIe          *oldIe,          /* original ie list */
   EgUIe          **dupIe          /* duplicated ie list */
 )
#else
PRIVATE S16 egUUtilDupIe(memCp, oldIe, dupIe)
   CmMemListCp    *memCp;          /* memory information */
   EgUIe          *oldIe;          /* original ie list */
   EgUIe          **dupIe;         /* duplicated ie list */
#endif
{
   PTR length = 0;                 /* to store the length of the IE */
   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUUtilDupIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!oldIe || !dupIe || !memCp)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /**************************************
    * Take the size of the IE to length  *
    **************************************/
   length = sizeof(EgUIe);

   /**********************************************************
    * allocate memory for the ie and then the local ie will  *
    * be appended to the list                                *
    **********************************************************/
   if (cmGetMem(memCp, length, (Ptr* )dupIe) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG066, ERRZERO,
            "egUUtilDupIe: Allocating the memory for the IE failed.\n");
      RETVALUE(EG_UTIL_ERR_MEM_ALLOC_FAILED);
   }

   /*************************************
    * Initialize the elements of new ie *
    *************************************/
   cmMemset( (U8* )(*dupIe), 0, length);

   /***********************
    * Duplicate IE header *
    ***********************/
   ( (EgUIe *)(*dupIe))->ieType = oldIe->ieType;
   ( (EgUIe *)(*dupIe))->dataType = oldIe->dataType;

   /*****************************************
    * Duplicate IE based on the IE datatype *
    *****************************************/
   switch (oldIe->dataType)
   {
      case EG_UNSIGNED_8:
         {
            ( (EgUIe *)(*dupIe))->t.valUnSgnd8 = oldIe->t.valUnSgnd8;
            break;
         }

      case EG_UNSIGNED_32:
         {
            ( (EgUIe *)(*dupIe))->t.valUnSgnd32 = oldIe->t.valUnSgnd32;
            break;
         }

      case EG_PVT_EXTN:
         {
            ( (EgUIe *)(*dupIe))->t.pvtExt.entId = oldIe->t.pvtExt.entId;
            ( (EgUIe *)(*dupIe))->t.pvtExt.propVal.length = oldIe->t.pvtExt.propVal.length;
            cmMemcpy(( (EgUIe *)(*dupIe))->t.pvtExt.propVal.val, 
                  oldIe->t.pvtExt.propVal.val, oldIe->t.pvtExt.propVal.length);
            break;
         }

      case EG_IP_ADDR:
         {
            ((EgUIe *)(*dupIe))->t.ipAddr.addrType = oldIe->t.ipAddr.addrType;

            if(oldIe->t.ipAddr.addrType == EG_IP_V4_TYPE)
            {
               cmMemcpy((U8*)&(((*dupIe))->t.ipAddr.u.ipv4), (U8*)&(oldIe->t.ipAddr.u.ipv4),
                        sizeof(CmIpAddr));
            }
#ifdef EG_IPV6_SUPPORTED
            else if(oldIe->t.ipAddr.addrType == EG_IP_V6_TYPE)
            {
               cmMemcpy((U8*)&(((*dupIe))->t.ipAddr.u.ipv6), (U8*)&(oldIe->t.ipAddr.u.ipv6), 
                        sizeof(CmIpAddr6));
            }
#endif /* EG_IPV6_SUPPORTED */

            break; 
         }

      case EG_EXT_HDR_TYPE_LST:
         {
            U16 cnt = 0;
            ((EgUIe *)(*dupIe))->t.extHdrTypeLst.noOfExtHdrs = oldIe->t.extHdrTypeLst.noOfExtHdrs;
            if(oldIe->t.extHdrTypeLst.noOfExtHdrs <= EGT_U_MAX_EXT_TYPES)
            {
              for (cnt = 0; cnt < oldIe->t.extHdrTypeLst.noOfExtHdrs; cnt++)
              {
                 ((EgUIe *)(*dupIe))->t.extHdrTypeLst.extType[cnt] = 
                    oldIe->t.extHdrTypeLst.extType[cnt];
              }
            }
            break;
         }

      default:
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG067, ERRZERO,
                  "egUUtilDupIe: Ie datatype is not valid.\n");
            RETVALUE(EG_UTIL_ERR_INV_IE_DATATYPE);
         }
   }

   /*********************************************
    * initialize the node address of the new IE *
    *********************************************/
   ( (EgUIe *)(*dupIe))->node.node = (PTR) (*dupIe);

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
 *
 *    Fun:    EgUUtilGmDump
 *
 *    Desc:   Method is used to dump the GTP-U message on
 *            console / Write to file
 *
 *    Ret:    ROK - if the ie list is duplicated successfully
 *            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
 *
 *    Notes:  N/A
 *
 *    File:   eg_util.c
 *
 ***************************************************************************************/
#ifdef EG_FILE_LOG
#ifdef ANSI
PUBLIC S16 EgUUtilGmDump
(
   EgUMsg      *egMsg,        /* GTP-U message */
   FILE        *fp,           /* file pointer */
   U16         *nmbLines      /* number of lines */
 )
#else
PUBLIC S16 EgUUtilGmDump(egMsg , fp, nmbLines)
   EgUMsg      *egMsg;         /* GTP-U message */
   FILE        *fp;            /* file pointer */
   U16         *nmbLines;      /* number of lines */
#endif
#else 
#ifdef ANSI
PUBLIC S16 EgUUtilGmDump
(
    EgUMsg      *egMsg          /* GTP-U message */
)
#else
PUBLIC S16 EgUUtilGmDump(egMsg)
   EgUMsg      *egMsg;         /* GTP-U message */
#endif
#endif /* EG_FILE_LOG*/
{
   /* this is used to hold the return value of sub methods */
   S16 retValue = 0;

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUUtilGmDump) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg
#ifdef EG_FILE_LOG
         || !fp || !nmbLines
#endif /* end of EG_FILE_LOG */
      )
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*******************************
    * dump the GTP-U message header *
    *******************************/
   if ( (retValue = egUUtilDumpGmHdr(egMsg->msgHdr
#ifdef EG_FILE_LOG
               , fp, nmbLines
#endif /* end of EG_FILE_LOG */
               )) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG068, ERRZERO,
            "EgUUtilGmDump: Dumping the GTP-U header failed.\n");
      RETVALUE(retValue);
   }

   if(egMsg->msgHdr.msgType != EGT_GTPU_MSG_GPDU)
   {
   /********************
    * dump the ie list *
    ********************/
   if ( (retValue = egUUtilDumpList(&egMsg->u.egIeList
#ifdef EG_FILE_LOG
               , fp, nmbLines
#endif /* end of EG_FILE_LOG */
               )) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG069, ERRZERO,
            "EgUUtilGmDump: Dumping the ie list failed.\n");
      RETVALUE(retValue);
   }
   }
   else
   {
      SPrntMsg(egMsg->u.mBuf,0,0);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
 *
 *    Fun:    egUUtilDumpGmHdr
 *
 *    Desc:   Method is used to dump the GTP-U header
 *
 *    Ret:    ROK - if the GTP-U header is dumped successfully
 *            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
 *
 *    Notes:  
 *
 *    File:   eg_util.c
 *
 ***************************************************************************************/
#ifdef EG_FILE_LOG
#ifdef ANSI
PRIVATE S16 egUUtilDumpGmHdr
(
   EgUMsgHdr      egGmHdr,   /* GTP header */
   FILE           *fp,       /* file pointer */
   U16            *nmbLines  /* number of lines */
 )
#else
PRIVATE S16 egUUtilDumpGmHdr(egGmHdr , fp, nmbLines)
   EgUMsgHdr      egGmHdr;         /* GTP header */
   FILE           *fp;             /* file pointer */
   U16            *nmbLines;       /* number of lines */
#endif
#else

#ifdef ANSI
PRIVATE S16 egUUtilDumpGmHdr
(
   EgUMsgHdr      egGmHdr          /* GTP header */
)
#else
PRIVATE S16 egUUtilDumpGmHdr(egGmHdr)
   EgUMsgHdr       egGmHdr;         /* GTP header */
#endif

#endif /* EG_FILE_LOG */
{
   Txt buff[EG_UTIL_PRNT_BUFF_SIZE];     /* Temp Buffer for Printing */
   Txt prnBuff[EG_UTIL_PRNT_BUFF_SIZE];  /* Actual Print Buffer */
   Txt newLine[EG_THREE];

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUUtilDumpGmHdr) 

   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )prnBuff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   sprintf(newLine," \n");

   sprintf(buff, "   ********************* GTP-U Message *********************\n");

   /*******************************************
    * Dumping GTP-U Message Header Elements   *
    *******************************************/

   /************************
    * Dumping Message Type *
    ************************/
   sprintf(buff, "  MsgType    = %d\n", egGmHdr.msgType);
   EG_STRCAT(prnBuff, buff);

   /************************
    * Dumping TEID         *
    ************************/
#ifdef ALIGN_64BIT
   sprintf(buff, "  Teid  = %d\n", egGmHdr.teId);
   EG_STRCAT(prnBuff, buff);
#else
   sprintf(buff, "  Teid  = %ld\n", egGmHdr.teId);
   EG_STRCAT(prnBuff, buff);
#endif /* end of  ALIGN_64BIT */

   /****************************************
    * Dumping Sequence Number, if Present  *
    ****************************************/
   if(egGmHdr.seqNumber.pres)
   {
      /*-- eg005.201: Format specifier aligned to 32-bit val of Seq Num --*/
      sprintf(buff, "  Seq No    = %ld\n", egGmHdr.seqNumber.val);
      EG_STRCAT(prnBuff, buff);
   }

   /****************************************
    * Dumping N-PDU Number, if Present     *
    ****************************************/
   if(egGmHdr.nPduNmb.pres)
   {
      sprintf(buff, "  N-PDU Number    = %d\n", egGmHdr.nPduNmb.val);
      EG_STRCAT(prnBuff, buff);
   }

   /***************************************************
    * Dumping Extension Header - UDP Port, if Present *
    ***************************************************/
   if(egGmHdr.extHdr.udpPort.pres)
   {
      sprintf(buff, " Extension Header : UDP Port    = %d\n", egGmHdr.extHdr.udpPort.val);
      EG_STRCAT(prnBuff, buff);
   }

   /***************************************************
    * Dumping Extension Header - PDCP Num, if Present *
    ***************************************************/
   if(egGmHdr.extHdr.pdcpNmb.pres)
   {
      sprintf(buff, " Extension Header : PDCP Number  = %d\n", egGmHdr.extHdr.pdcpNmb.val);
      EG_STRCAT(prnBuff, buff);
   }

#ifdef EG_FILE_LOG
   fprintf(fp, "%s\n", prnBuff);
   nmbLines += 6;
#else
   SPrint(prnBuff);
   SPrint(newLine);
#endif /* end of EG_FILE_LOG */

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
 *
 *    Fun:    egUUtilDumpList
 *
 *    Desc:   Method is used to dump an ie list
 *
 *    Ret:    ROK - if the ie list is dumped successfully
 *            RFAILED - if the ie list dumping fails
 *            Return value of the child method (if any)
 *
 *    Notes:  
 *
 *    File:   eg_util.c
 *
 ***************************************************************************************/
#ifdef EG_FILE_LOG
#ifdef ANSI
PRIVATE S16 egUUtilDumpList
(
   CmLListCp      *ieList,    /* ie list */
   FILE           *fp,        /* file pointer */
   U16            *nmbLines   /* number of lines */
 )
#else
PRIVATE S16 egUUtilDumpList(ieList, fp, nmbLines)
   CmLListCp      *ieList;          /* ie list */
   FILE           *fp;              /* file pointer */
   U16            *nmbLines;        /* number of lines */
#endif

#else

#ifdef ANSI
PRIVATE S16 egUUtilDumpList
(
   CmLListCp      *ieList          /* ie list */
)
#else
PRIVATE S16 egUUtilDumpList(ieList)
   CmLListCp      *ieList;          /* ie list */
#endif

#endif /* EG_FILE_LOG */
{
   EgUIe *egIe = NULLP;          /* to hold the address of IE in the original list */
   S16 retValue = 0;              /* to hold the return value of sub methods */
   CmLList *node = NULLP;         /* to hold the temporary node address */

   /************************
    * to hold print buffer *
    ************************/
   Txt buff[EG_UTIL_PRNT_BUFF_SIZE];
   Txt prnBuff[EG_UTIL_PRNT_BUFF_SIZE];
   Txt newLine[EG_THREE];

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUUtilDumpList) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!ieList)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )prnBuff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   sprintf(newLine," \n");

   sprintf(buff, "           ******* IE list *******              \n");
   EG_STRCAT(prnBuff, buff);
#ifdef ALIGN_64BIT
   sprintf(buff, "  count = %d\n", ieList->count);
#else
   sprintf(buff, "  count = %ld\n", ieList->count);
#endif /* end of ALIGN_64BIT */
   EG_STRCAT(prnBuff, buff);

#ifdef EG_FILE_LOG
   fprintf(fp, "%s\n", prnBuff);
   nmbLines += 2;
#else
   SPrint(prnBuff);
   SPrint(newLine);
#endif /* end of EG_FILE_LOG */

   /***********************************************
    * iterate through the list and for each ie,   *
    * call egUUtilDumpIe to dump the IE.           *
    ***********************************************/
   for (egIe = (EgUIe* ) CM_LLIST_FIRST_NODE(ieList, node); egIe != NULLP;
         egIe = (EgUIe* ) CM_LLIST_NEXT_NODE(ieList, node))
   {
      /***************
       * dump the IE *
       ***************/
      if ( (retValue = egUUtilDumpIe(egIe
#ifdef EG_FILE_LOG
                  , fp, nmbLines
#endif /* end of EG_FILE_LOG */
                  )) != ROK)
      {
         EGUTILLOGERROR(ERRCLS_DEBUG, EEG070, ERRZERO,
               "egUUtilDumpList: Dumping the IE failed.\n");
         RETVALUE(retValue);
      }
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

/***************************************************************************************
 *
 *    Fun:    egUUtilDumpIe
 *
 *    Desc:   Method is used to dump the IE
 *
 *    Ret:    ROK - if dumping IE succeeds
 *            RFAILED - if dumping IE fails
 *
 *    Notes:  N/A
 *
 *    File:   eg_util.c
 *
 ***************************************************************************************/
#ifdef EG_FILE_LOG

#ifdef ANSI
PRIVATE S16 egUUtilDumpIe
(
   EgUIe          *egIe,          /* IE that has to be dumped */
   FILE           *fp,            /* file pointer */
   U16            *nmbLines       /* number of lines */
 )
#else
PRIVATE S16 egUUtilDumpIe(egIe, fp, nmbLines)
   EgUIe          *egIe;          /* IE that has to be dumped */
   FILE           *fp;            /* file pointer */
   U16            *nmbLines;      /* number of lines */
#endif

#else
#ifdef ANSI
PRIVATE S16 egUUtilDumpIe
(
   EgUIe           *egIe           /* IE that has to be dumped */
)
#else
PRIVATE S16 egUUtilDumpIe(egIe)
   EgUIe           *egIe;          /* IE that has to be dumped */
#endif

#endif /* EG_FILE_LOG */
{
   Txt buff[EG_UTIL_PRNT_BUFF_SIZE];      /* to hold buffer (Ie data) */
   Txt prnBuff[EG_UTIL_PRNT_BUFF_SIZE];   /* to hold print buffer (Ie header and data)*/  
   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */
   Txt tempBuff[EG_UTIL_PRNT_BUFF_SIZE-5]; /* to avoid the overflow */
   Txt newLine[EG_THREE];

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUUtilDumpIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egIe)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /*******************************
    * initialize the print buffer *
    *******************************/
   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )prnBuff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
   cmMemset( (U8* )tempBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);
   sprintf(newLine," \n");

   /*********************
    * Dumping IE header *
    *********************/
   sprintf(buff, "  ******* IE dump (%p) ********\n", (Ptr)egIe);
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  IeType  = %d\n", egIe->ieType);
   EG_STRCAT(prnBuff, buff);
   sprintf(buff, "  IeDatatype = %d\n", egIe->dataType);
   EG_STRCAT(prnBuff, buff);

   EG_STRCAT(prnBuff, "\n");

#ifdef EG_PRNT_DATA
   cmMemset( (U8* )buff, 0, EG_UTIL_PRNT_BUFF_SIZE);
   /****************************
    * check the type of the IE *
    ****************************/
   switch(egIe->dataType)
   {
      case EG_UNSIGNED_8: 
         {
            sprintf(buff, "  value = %d\n",egIe->t.valUnSgnd8);
            break;
         }

      case EG_UNSIGNED_32: 
         {
#ifdef ALIGN_64BIT
            sprintf(buff, "  value = %d\n",egIe->t.valUnSgnd32);
#else
            sprintf(buff, "  value = %ld\n",egIe->t.valUnSgnd32);
#endif
            break;
         }

      case EG_PVT_EXTN:
         {
            sprintf(buff,"  entId: %d\n", egIe->t.pvtExt.entId);  

            sprintf(tmpBuff,"  propVal.length: %d\n", egIe->t.pvtExt.propVal.length);  
            EG_STRCAT(buff,tmpBuff);

            cmMemcpy( (U8* )tempBuff, egIe->t.pvtExt.propVal.val, egIe->t.pvtExt.propVal.length+1);
            sprintf(tmpBuff,"  propVal.val: %s\n", tempBuff);  
            EG_STRCAT(buff,tmpBuff);
            break; 
         }

      case EG_IP_ADDR:
         {
            sprintf(buff,"  addrType: %d\n", egIe->t.ipAddr.addrType);  
            egUUtilDumpIpAddr(buff,&egIe->t.ipAddr);
            break; 
         }

      case EG_EXT_HDR_TYPE_LST:
         {
            U16 loopCount = 0;
            sprintf(buff,"  no. of Ext Headers: %d\n", egIe->t.extHdrTypeLst.noOfExtHdrs);  

            for(loopCount=0; loopCount <  egIe->t.extHdrTypeLst.noOfExtHdrs; loopCount++)
            {
               sprintf(tmpBuff,"  Ext Header Type%d : %d\n", (loopCount+1), 
                     egIe->t.extHdrTypeLst.extType[loopCount]);  
               EG_STRCAT(buff,tmpBuff);
            }

            break;
         } 

      default:
         {
            EGUTILLOGERROR(ERRCLS_DEBUG, EEG071, ERRZERO,
                  "egUUtilDumpIe: Ie datatype is not valid.\n");
            RETVALUE(EG_UTIL_ERR_INV_IE_DATATYPE);
         }

   }
   EG_STRCAT(prnBuff, buff);
#endif /* EG_PRNT_DATA */

#ifdef EG_FILE_LOG
   fprintf(fp, "%s\n", prnBuff);
   nmbLines += 5;
#else
   SPrint(prnBuff);
   SPrint(newLine);
#endif /* end of EG_FILE_LOG */

   RETVALUE(ROK);
}

/************************************************************************************************
 *
 *    Fun:    EgUUtilGmDelIe
 *
 *    Desc:   Method is used to delete an IE from the GTP-U message
 *
 *    Ret:    ROK - if the IE is deleted successfully
 *            RFAILED - if deleting IE fails
 *            EG_UTIL_ERR_INV_LEVEL_INFO - if the given level information is invalid
 *            EG_UTIL_ERR_INV_PARAM - if the given message or Ie Type or Occurance Cnt is invalid
 *            Return value of the child method (if any)
 *
 *    Notes:  
 *
 *    File:   eg_util.c
 *
 ************************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUUtilGmDelIe
(
   EgUMsg   *egMsg,         /* eGtp message */
   U8       ieType,
   U8       occrCnt
 )
#else
PUBLIC S16 EgUUtilGmDelIe(egMsg, ieType, occrCnt)
   EgUMsg   *egMsg;         /* eGtp message */
   U8       ieType;
   U8       occrCnt;
#endif
{
   S16      retValue = 0;                 /* to hold the return value of sub methods */
   EgUIe    *egIe = NULLP;                /* this will hold the IE node address */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUUtilGmDelIe) 

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if (!egMsg || !occrCnt)
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

   /************************************************************
    * get the target IE from the list as per the given IE info *
    ************************************************************/
   if ( (retValue = egUUtilGetTrgtIe(&egMsg->u.egIeList, ieType, occrCnt, &egIe)) != ROK)
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG072, ERRZERO,
            "EgUUtilGmDelIe: Getting the target IE from the message failed.\n");
      RETVALUE(retValue);
   }

   /*********************************
    * delete the node from the list *
    *********************************/
   if (!cmLListDelFrm(&egMsg->u.egIeList, &egIe->node))
   {
      EGUTILLOGERROR(ERRCLS_DEBUG, EEG073, ERRZERO,
            "EgUUtilGmDelIe: Deleting node from the IE list failed.\n");
      RETVALUE(RFAILED);
   }

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}


/*
*
*    Fun:    EgUUtilGmGetIeOccrCnt
*
*    Desc:   Method is used to get the number of occurrences of an IE
*
*    Ret:    ROK - if the ie list is duplicated successfully
*            EG_UTIL_ERR_INV_PARAM - if the given message or level information are invalid
*
*    Notes:  N/A
*
*    File:   eg_util.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUUtilGmGetIeOccrCnt
(
EgUMsg         *egMsg,       /* GTP-U message */
U8             *ieType,    /* IE Type */
U16            *ieOccrCnt     /* IE count */
)
#else
PUBLIC S16 EgUUtilGmGetIeOccrCnt(egMsg, ieType, ieOccrCnt)
EgUMsg      *egMsg;         /* GTP-U message */
U8             *ieType;    /* IE Type */
U16            *ieOccrCnt;    /* IE count */
#endif
{
   /* eg012.201 : klocwork fix - removed unnecessary variable initializations */
   EgUIe       *egIe; /* this is used to hold the temporary node address */
   CmLList     *node; /* initialize the occrrence count as zero initially */
   S16         retVal  = RFAILED; /* return value */
   
   *ieOccrCnt = EG_ZERO; /* initialize the occrrence count as zero initially */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(EgUUtilGmGetIeOccrCnt)

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /* eg012.201 : klocwork fix - sanity check removed, egMsg is always verified in pack/unpack */

   /*****************************************************************
    * Iterate through the list of the IEs and if the IE code        *
    * is matching with the IE given, increment the count given.     *
    *****************************************************************/
   /* iterate through the list */
   for (egIe = (EgUIe* ) CM_LLIST_FIRST_NODE(&egMsg->u.egIeList, node);
        egIe != NULLP; egIe = (EgUIe* ) CM_LLIST_NEXT_NODE(&egMsg->u.egIeList, node))
   {
      /* if the IE code is matching, then increment
      * the occrrence count
      */
      if (egIe->ieType == *ieType)
      {
         (*ieOccrCnt)++;
         retVal = ROK;
      }
   } 
   RETVALUE(retVal);
} /* end of EgUtilGmGetIeOccrCnt */

#ifdef EG_PRNT_DATA
/***********************************************************************************
*
*    Fun:    egUtilDumpIpv4Addr
*
*    Desc:   Dump IPv4 address
*
*    Ret:    ROK  - if duplication is success
*            EG_UTIL_ERR_INV_PARAM - if the given message is invalid
*
*    Notes:  None
*
*    File:   eg_util.c
*
*************************************************************************************/
#ifdef ANSI
PRIVATE S16 egUUtilDumpIpAddr
(
Txt *buff,    /* To hold print buffer */
EgUIpAddr  *ipAddr  /* Source for dumping */
)
#else
PRIVATE S16 egUUtilDumpIpAddr(buff,ipAddr)
Txt *buff;    /* To hold print buffer */
EgUIpAddr  *ipAddr;    /* Source for dumping */
#endif
{
#ifdef EG_IPV6_SUPPORTED
   U8 idx = 0;
#endif
   Txt tmpBuff[EG_UTIL_PRNT_BUFF_SIZE-5];  /* to avoid the overflow */

   /*-- eg002.201 : eg006.102 changes --*/
   EG_TRC2(egUUtilDumpIpAddr)

   /************************************************* 
    * eg009.201: Removed ERRCHK, as its supposed to *
    * suppress Error Logs, not Pointers checks.     *
    *************************************************/

   /****************
    * sanity check *
    ****************/
   if ((!buff) || (!ipAddr))
   {
      RETVALUE(EG_UTIL_ERR_INV_PARAM);
   }

  /*******************************
   * initialize the print buffer *
   *******************************/
   cmMemset( (U8* )tmpBuff, 0, EG_UTIL_PRNT_BUFF_SIZE-5);

   if(ipAddr->addrType == EG_IP_V4_TYPE)
   {
      sprintf(tmpBuff,"  ipv4Addr:0x %lx \n",ipAddr->u.ipv4);
      EG_STRCAT(buff,tmpBuff);
   }
#ifdef EG_IPV6_SUPPORTED
   else if (ipAddr->addrType == EG_IP_V6_TYPE)
   {
      sprintf(tmpBuff,"  ipv6Addr: ");
      EG_STRCAT(buff,tmpBuff);

      /***************************
       * Copy IP address to buff *
       ***************************/
      for (idx = 0; idx < 16; idx++)
      {
         if(idx == 0)
         {
            sprintf(tmpBuff,"0x %x ", ipAddr->u.ipv6[idx]);  
         }
         else
         {
            sprintf(tmpBuff,"%x ", ipAddr->u.ipv6[idx]); 
         }
         EG_STRCAT(buff,tmpBuff);
      }

      EG_STRCAT(buff,"\n");
   }
#endif

   /******************
    * return success *
    ******************/
   RETVALUE(ROK);
}

#endif /* End of EG_PRNT_DATA */

#endif /* End of EGTP_U */

/********************************************************************30**

  End of file:     eg_util.c@@/main/3 - Fri Feb  4 18:18:13 2011

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
 
*********************************************************************81*/
/********************************************************************90**
 
 ver           pat    init                  description
---------  --------   ---- ---------------------------------------
/main/1      ---        kc                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya          1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg002.201 psingh              1. Fixed warnings due to improper 
                                             placement of macro 
/main/3     eg003.201 psingh              1. Merged code from eg006.102 to Fix 
                                             TRACE5 macro related issue
                                          2. Merged code from eg008.102 to Fix 
                                             warning due to improper EG_TRC2 
                                             declaration                                             
/main/3     eg004.201 magnihotri          1.eGTPC PSF header files included
/main/3     eg005.201 psingh              1. Format specifier aligned to 32-bit val of Seq Num
/main/3     eg007.201 psingh              1. egTptGetTsapFrmAddr(): introduced to obtain TSapCb 
                                             from IP address
                                          2. EgUUtilDeAllocGmMsg: Moved Null ptr chk out
                                             of ERRCHK flag 
/main/3     eg009.201 asaurabh            1. Removed ERRCHK, as its supposed to suppress
                                             Error Logs, not Pointers checks.
                                          2. EgUtilDeAllocGmMsg(): check for NULL ptr.   
/main/3     eg012.201 shpandey            1. Added klocwork fixes
                                          2. In function egUtilDumpGmHdr:replaced sprintf with snprintf
                                          3. Added instance value check,fix for  ccpu00130411  
/main/3     eg014.201 rss                 1. Egtpc Release 10 Upgrade
*********************************************************************91*/
