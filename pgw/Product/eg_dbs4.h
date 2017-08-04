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

    File: eg_dbs4.h

    Sid:      eg_dbs4.h@@/main/2 - Fri Feb  4 18:18:06 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_S4_BRERESCMD_CMD_CODE       68        
#ifdef EG_REL_AC0 
#define EG_S4_BRERESCMD_NMB_IES        14        
#else
#define EG_S4_BRERESCMD_NMB_IES        13        
#endif 
#define EG_S4_BRERESFLRIND_CMD_CODE    69        
#define EG_S4_BRERESFLRIND_NMB_IES     5         
#define EG_S4_CHGNTFREQ_CMD_CODE       38        
#ifdef EG_REL_AC0 
#define EG_S4_CHGNTFREQ_NMB_IES        9         
#elif EG_REL_930
#define EG_S4_CHGNTFREQ_NMB_IES        7        
#else
#define EG_S4_CHGNTFREQ_NMB_IES        5         
#endif 
#define EG_S4_CHGNTFRSP_CMD_CODE       39        
#ifdef EG_REL_AC0 
#define EG_S4_CHGNTFRSP_NMB_IES        6         
#elif EG_REL_930 
#define EG_S4_CHGNTFRSP_NMB_IES        5         
#else
#define EG_S4_CHGNTFRSP_NMB_IES        4
#endif 
#define EG_S4_CRTBREREQ_CMD_CODE       95    

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_S4_CRTBREREQ_NMB_IES        7         
#else 
#define EG_S4_CRTBREREQ_NMB_IES        6
#endif 

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 9
#else
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 8
#endif

#define EG_S4_CRTBRERSP_CMD_CODE       96        
#define EG_S4_CRTBRERSP_NMB_IES        7      

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 7         
#else
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 6
#endif

#define EG_S4_CRTINDDATFWDTUNREQ_CMD_CODE 166       
#ifdef EG_REL_AC0 
#define EG_S4_CRTINDDATFWDTUNREQ_NMB_IES 7         
#else 
#define EG_S4_CRTINDDATFWDTUNREQ_NMB_IES 5         
#endif 
#define EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_S4_CRTINDDATFWDTUNRSP_CMD_CODE 167       
#define EG_S4_CRTINDDATFWDTUNRSP_NMB_IES 5         
#define EG_CRTINDDATFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_S4_CRTSESREQ_CMD_CODE       32

#ifdef EG_REL_AC0 
#define EG_S4_CRTSESREQ_NMB_IES        27        
#elif EG_REL_930
#define EG_S4_CRTSESREQ_NMB_IES        25        
#else
#define EG_S4_CRTSESREQ_NMB_IES        24
#endif

#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S4_CRTSESRSP_CMD_CODE       33        

#ifdef EG_REL_AC0 
#define EG_S4_CRTSESRSP_NMB_IES        16        
#elif EG_REL_930
#define EG_S4_CRTSESRSP_NMB_IES        14
#else
#define EG_S4_CRTSESRSP_NMB_IES        13
#endif 

#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 8         
#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S4_DELBRECMD_CMD_CODE       66        
#define EG_S4_DELBRECMD_NMB_IES        2         
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#else
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 1
#endif
#define EG_S4_DELBREFLRIND_CMD_CODE    67        
#define EG_S4_DELBREFLRIND_NMB_IES     4         
#define EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S4_DELBREREQ_CMD_CODE       99        
#define EG_S4_DELBREREQ_NMB_IES        5         
#define EG_S4_DELBRERSP_CMD_CODE       100       

#if defined(EG_REL_930) || defined(EG_REL_AC0) 
#define EG_S4_DELBRERSP_NMB_IES        8         
#else 
#define EG_S4_DELBRERSP_NMB_IES        8         
#endif 

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#else 
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#endif 

#define EG_S4_DELINDDATFWDTUNREQ_CMD_CODE 168       
#define EG_S4_DELINDDATFWDTUNREQ_NMB_IES 1         
#define EG_S4_DELINDDATFWDTUNRSP_CMD_CODE 169       
#define EG_S4_DELINDDATFWDTUNRSP_NMB_IES 3         
#define EG_S4_DELSESREQ_CMD_CODE       36        
#ifdef EG_REL_AC0 
#define EG_S4_DELSESREQ_NMB_IES        9         
#else 
#define EG_S4_DELSESREQ_NMB_IES        7         
#endif 
#define EG_S4_DELSESRSP_CMD_CODE       37        
#define EG_S4_DELSESRSP_NMB_IES        4         
#define EG_S4_DWLNKDATNOT_CMD_CODE     176       
#ifdef EG_REL_AC0 
#define EG_S4_DWLNKDATNOT_NMB_IES      5         
#else
#define EG_S4_DWLNKDATNOT_NMB_IES      2         
#endif 
#define EG_S4_DWLNKDATNOTACK_CMD_CODE  177       
#ifdef EG_REL_AC0 
#define EG_S4_DWLNKDATNOTACK_NMB_IES   6         
#else
#define EG_S4_DWLNKDATNOTACK_NMB_IES   4
#endif 
#define EG_S4_DWLNKDATNOTFLRIND_CMD_CODE 70        
#ifdef EG_REL_AC0 
#define EG_S4_DWLNKDATNOTFLRIND_NMB_IES 4         
#else 
#define EG_S4_DWLNKDATNOTFLRIND_NMB_IES 2         
#endif 
#define EG_S4_MODBRECMD_CMD_CODE       64        
#define EG_S4_MODBRECMD_NMB_IES        3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S4_MODBREFLRIND_CMD_CODE    65        
#define EG_S4_MODBREFLRIND_NMB_IES     3         
#define EG_S4_MODBREREQ_CMD_CODE       34        

#ifdef EG_REL_AC0 
#define EG_S4_MODBREREQ_NMB_IES        15        
#elif EG_REL_930
#define EG_S4_MODBREREQ_NMB_IES        14
#else
#define EG_S4_MODBREREQ_NMB_IES        13
#endif

#define EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S4_MODBRERSP_CMD_CODE       35        

#ifdef EG_REL_AC0 
#define EG_S4_MODBRERSP_NMB_IES        14        
#elif EG_REL_930
#define EG_S4_MODBRERSP_NMB_IES        13
#else
#define EG_S4_MODBRERSP_NMB_IES        12
#endif

#define EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S4_PGWRSTNOT_CMD_CODE       179       
#define EG_S4_PGWRSTNOT_NMB_IES        3         
#define EG_S4_PGWRSTNOTACK_CMD_CODE    180       
#define EG_S4_PGWRSTNOTACK_NMB_IES     2         
#define EG_S4_RELACCBREREQ_CMD_CODE    170       
#ifdef EG_REL_AC0 
#define EG_S4_RELACCBREREQ_NMB_IES     3         
#else 
#define EG_S4_RELACCBREREQ_NMB_IES     2         
#endif 
#define EG_S4_RELACCBRERSP_CMD_CODE    171       
#define EG_S4_RELACCBRERSP_NMB_IES     3         
#define EG_S4_RESUMEACK_CMD_CODE       165       
#define EG_S4_RESUMEACK_NMB_IES        2         
#define EG_S4_RESUMENOT_CMD_CODE       164       
#ifdef EG_REL_AC0
#define EG_S4_RESUMENOT_NMB_IES        3         
#else
#define EG_S4_RESUMENOT_NMB_IES        2         
#endif
#define EG_S4_STOPPAGINGIND_CMD_CODE   73        
#define EG_S4_STOPPAGINGIND_NMB_IES    1         
#define EG_S4_SUSPENDACK_CMD_CODE      163       
#define EG_S4_SUSPENDACK_NMB_IES       2         
#define EG_S4_SUSPENDNOT_CMD_CODE      162       
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_S4_SUSPENDNOT_NMB_IES       2         
#else
#define EG_S4_SUSPENDNOT_NMB_IES       1         
#endif
#define EG_S4_TRACESESSACTV_CMD_CODE   71        
#define EG_S4_TRACESESSACTV_NMB_IES    3         
#define EG_S4_TRACESESSDEACTV_CMD_CODE 72        
#define EG_S4_TRACESESSDEACTV_NMB_IES  1         
#define EG_S4_UPDBREREQ_CMD_CODE       97        
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_S4_UPDBREREQ_NMB_IES        7         
#else 
#define EG_S4_UPDBREREQ_NMB_IES        6         
#endif 
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#else 
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 4         
#endif 
#define EG_S4_UPDBRERSP_CMD_CODE       98        
#define EG_S4_UPDBRERSP_NMB_IES        8         
 
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 5         
#else 
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 4         
#endif 


#endif

/********************************************************************30**

         End of file:     eg_dbs4.h@@/main/2 - Fri Feb  4 18:18:06 2011

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
/main/1      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/2      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/2     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

