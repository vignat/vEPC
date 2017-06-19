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


/***********************************************************************

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: eg_dbs5.h

    Sid:      eg_dbs5.h@@/main/1 - Fri Feb  4 18:18:07 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_S5_BRERESCMD_CMD_CODE       68

/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_S5_BRERESCMD_NMB_IES        12        
#else 
#define EG_S5_BRERESCMD_NMB_IES        11        
#endif

#define EG_S5_BRERESFLRIND_CMD_CODE    69        
#define EG_S5_BRERESFLRIND_NMB_IES     5         
#define EG_S5_CHGNTFREQ_CMD_CODE       38        
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_S5_CHGNTFREQ_NMB_IES        8         
#elif EG_REL_930 
#define EG_S5_CHGNTFREQ_NMB_IES        6         
#else
#define EG_S5_CHGNTFREQ_NMB_IES        4         
#endif 
#define EG_S5_CHGNTFRSP_CMD_CODE       39        
#ifdef EG_REL_AC0 
#define EG_S5_CHGNTFRSP_NMB_IES        6         
#elif EG_REL_930 
#define EG_S5_CHGNTFRSP_NMB_IES        5         
#else
#define EG_S5_CHGNTFRSP_NMB_IES        4         
#endif 
#define EG_S5_CRTBREREQ_CMD_CODE       95        

#ifdef EG_REL_AC0 
#define EG_S5_CRTBREREQ_NMB_IES        8
#elif EG_REL_930 
#define EG_S5_CRTBREREQ_NMB_IES        8
#else
#define EG_S5_CRTBREREQ_NMB_IES        7
#endif 

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 7         
#else 
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 6
#endif 

#define EG_S5_CRTBRERSP_CMD_CODE       96        
#define EG_S5_CRTBRERSP_NMB_IES        9     

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 5         
#else 
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 4
#endif

#define EG_S5_CRTSESREQ_CMD_CODE       32        

#ifdef EG_REL_AC0 
#define EG_S5_CRTSESREQ_NMB_IES        26        
#elif EG_REL_930 
#define EG_S5_CRTSESREQ_NMB_IES        24
#else
#define EG_S5_CRTSESREQ_NMB_IES        23        
#endif

#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_S5_CRTSESRSP_CMD_CODE       33        

#ifdef EG_REL_AC0 
#define EG_S5_CRTSESRSP_NMB_IES        17        
#elif EG_REL_930
#define EG_S5_CRTSESRSP_NMB_IES        15        
#else
#define EG_S5_CRTSESRSP_NMB_IES        14
#endif 

#ifdef EG_REL_AC0 
#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 6         
#else 
#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 7
#endif 
#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S5_DELBRECMD_CMD_CODE       66        
#define EG_S5_DELBRECMD_NMB_IES        2         
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#else
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 1         
#endif
#define EG_S5_DELBREFLRIND_CMD_CODE    67        
#define EG_S5_DELBREFLRIND_NMB_IES     4         
#define EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_DELBREREQ_CMD_CODE       99        
#define EG_S5_DELBREREQ_NMB_IES        8         
#define EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_DELBRERSP_CMD_CODE       100       
#define EG_S5_DELBRERSP_NMB_IES        10        

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#else 
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#endif 

#define EG_S5_DELPDNCONSETREQ_CMD_CODE 101       
#define EG_S5_DELPDNCONSETREQ_NMB_IES  3         
#define EG_S5_DELPDNCONSETRSP_CMD_CODE 102       
#define EG_S5_DELPDNCONSETRSP_NMB_IES  2         
#define EG_S5_DELSESREQ_CMD_CODE       36        

#ifdef EG_REL_AC0 
#define EG_S5_DELSESREQ_NMB_IES        5         
#else 
#define EG_S5_DELSESREQ_NMB_IES        4
#endif 

#define EG_S5_DELSESRSP_CMD_CODE       37        
#define EG_S5_DELSESRSP_NMB_IES        4         
#define EG_S5_MODBRECMD_CMD_CODE       64        
#define EG_S5_MODBRECMD_NMB_IES        3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S5_MODBREFLRIND_CMD_CODE    65        
#define EG_S5_MODBREFLRIND_NMB_IES     3         
#define EG_S5_MODBREREQ_CMD_CODE       34        

#ifdef EG_REL_AC0 
#define EG_S5_MODBREREQ_NMB_IES        14        
#elif EG_REL_930 
#define EG_S5_MODBREREQ_NMB_IES        13
#else
#define EG_S5_MODBREREQ_NMB_IES        12
#endif 

#define EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S5_MODBRERSP_CMD_CODE       35        

#ifdef EG_REL_AC0 
#define EG_S5_MODBRERSP_NMB_IES        16        
#elif EG_REL_930 
#define EG_S5_MODBRERSP_NMB_IES        14        
#else
#define EG_S5_MODBRERSP_NMB_IES        13
#endif

#define EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S5_RESUMEACK_CMD_CODE       165       
#define EG_S5_RESUMEACK_NMB_IES        2         
#define EG_S5_RESUMENOT_CMD_CODE       164       
#define EG_S5_RESUMENOT_NMB_IES        2         
#define EG_S5_SUSPENDACK_CMD_CODE      163       
#define EG_S5_SUSPENDACK_NMB_IES       2         
#define EG_S5_SUSPENDNOT_CMD_CODE      162       
#define EG_S5_SUSPENDNOT_NMB_IES       1         
#define EG_S5_TRACESESSACTV_CMD_CODE   71        
#define EG_S5_TRACESESSACTV_NMB_IES    3         
#define EG_S5_TRACESESSDEACTV_CMD_CODE 72        
#define EG_S5_TRACESESSDEACTV_NMB_IES  1         
#define EG_S5_UPDBREREQ_CMD_CODE       97         
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_S5_UPDBREREQ_NMB_IES        8         
#else 
#define EG_S5_UPDBREREQ_NMB_IES        7         
#endif 
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#else 
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 4         
#endif 
#define EG_S5_UPDBRERSP_CMD_CODE       98        
#define EG_S5_UPDBRERSP_NMB_IES        9         
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#else 
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#endif 
#define EG_S5_UPDPDNCONSETREQ_CMD_CODE 200       
#define EG_S5_UPDPDNCONSETREQ_NMB_IES  3         
#define EG_S5_UPDPDNCONSETRSP_CMD_CODE 201       
#define EG_S5_UPDPDNCONSETRSP_NMB_IES  3         



#endif
/********************************************************************30**

         End of file:     eg_dbs5.h@@/main/1 - Fri Feb  4 18:18:07 2011

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
/main/1      ---      svenkat           1. Initial for eGTP 2.1 Release
/main/1      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

