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


/***********************************************************************

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: smegsi.h

    Sid:      eg_dbsi.h@@/main/1 - Fri Feb  4 18:18:07 2011

    Prg: an

************************************************************************/


/* Message Codes   */

#define EG_SI_CFGTFRTNL_CMD_CODE       141       
#define EG_SI_CFGTFRTNL_NMB_IES        2         
#define EG_SI_CRTINDDATFWDTUNREQ_CMD_CODE 166       
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_SI_CRTINDDATFWDTUNREQ_NMB_IES 7         
#else 
#define EG_SI_CRTINDDATFWDTUNREQ_NMB_IES 5         
#endif 
#define EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES 7         
#define EG_SI_CRTSESREQ_CMD_CODE       32        

/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_SI_CRTSESREQ_NMB_IES        34
#elif EG_REL_930
#define EG_SI_CRTSESREQ_NMB_IES        27
#else
#define EG_SI_CRTSESREQ_NMB_IES        26
#endif

/*eg009.201 Modified for FTEID new instances added*/
#if defined(EG_S2A_SUPP) || defined(EG_REL_AC0)
#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 10         
#else
#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 8         
#endif

#define EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_SI_CTXREQ_CMD_CODE          130       

#ifdef EG_REL_AC0 
#define EG_SI_CTXREQ_NMB_IES           16    
#else
#define EG_SI_CTXREQ_NMB_IES           13
#endif

#define EG_SI_DELPDNCONSETREQ_CMD_CODE 101       

#ifdef EG_REL_AC0 
#define EG_SI_DELPDNCONSETREQ_NMB_IES   5
#else
#define EG_SI_DELPDNCONSETREQ_NMB_IES  4
#endif


#define EG_SI_DELPDNCONSETRSP_CMD_CODE 102       
#define EG_SI_DELPDNCONSETRSP_NMB_IES  2         
#define EG_SI_DWLNKDATNOT_CMD_CODE     176       

#ifdef EG_REL_AC0 
#define EG_SI_DWLNKDATNOT_NMB_IES      5        
#else
#define EG_SI_DWLNKDATNOT_NMB_IES      2
#endif

#define EG_SI_DWLNKDATNOTACK_CMD_CODE  177       

#ifdef EG_REL_AC0 
#define EG_SI_DWLNKDATNOTACK_NMB_IES   6
#else
#define EG_SI_DWLNKDATNOTACK_NMB_IES   4
#endif

#define EG_SI_DWLNKDATNOTFLRIND_CMD_CODE 70        
#ifdef EG_REL_AC0 
#define EG_SI_DWLNKDATNOTFLRIND_NMB_IES 4         
#else 
#define EG_SI_DWLNKDATNOTFLRIND_NMB_IES 2         
#endif 
#define EG_SI_FWDRELREQ_CMD_CODE       133       

#ifdef EG_REL_AC0
#define EG_SI_FWDRELREQ_NMB_IES        41
#elif EG_REL_930
#define EG_SI_FWDRELREQ_NMB_IES        30
#else
#define EG_SI_FWDRELREQ_NMB_IES        28
#endif 

#ifdef EG_REL_AC0
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 16
#elif EG_REL_930
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 13
#else
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 12
#endif

#define EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES 8

/* #define EG_FWDRELREQ_PDN_CONN00_GRP_NMB_IES 8         */ 
#define EG_SI_IDENTREQ_CMD_CODE        128       
#define EG_SI_IDENTREQ_NMB_IES         10        
#define EG_SI_IDENTRSP_CMD_CODE        129       
#define EG_SI_IDENTRSP_NMB_IES         10        
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_SI_MBMSESNSTRTREQ_CMD_CODE  231      

#ifdef EG_REL_AC0
#define EG_SI_MBMSESNSTRTREQ_NMB_IES   11        
#else
#define EG_SI_MBMSESNSTRTREQ_NMB_IES   10        
#endif

#endif

#define EG_SI_PGWRSTNOT_CMD_CODE       179       
#define EG_SI_PGWRSTNOT_NMB_IES        3         
#define EG_SI_PGWRSTNOTACK_CMD_CODE    180       
#define EG_SI_PGWRSTNOTACK_NMB_IES     2         
#define EG_SI_RANINFORLY_CMD_CODE      152       
#define EG_SI_RANINFORLY_NMB_IES       3         
#define EG_SI_RELCANCELREQ_CMD_CODE    139       
#ifdef EG_REL_AC0 
#define EG_SI_RELCANCELREQ_NMB_IES     5         
#else 
#define EG_SI_RELCANCELREQ_NMB_IES     3         
#endif 
#define EG_SI_SUSPNDACK_CMD_CODE       163       
#define EG_SI_SUSPNDACK_NMB_IES        2         
#define EG_SI_SUSPNDNOTFN_CMD_CODE     162       
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_SI_SUSPNDNOTFN_NMB_IES      5         
#else
#define EG_SI_SUSPNDNOTFN_NMB_IES      4
#endif


/********************************************************************30**

         End of file:     eg_dbsi.h@@/main/1 - Fri Feb  4 18:18:07 2011

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

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      an                1. Initial Release
/main/2      ---      kchitanya         1. initial for egtp release 1.2
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/1   eg009.201   asaurabh          1. Modified for FTEID new instances 
                                           added
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

