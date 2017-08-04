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

    File: eg_dbs11.h

    Sid:      eg_dbs11.h@@/main/3 - Fri Feb  4 18:18:03 2011

    Prg : an

************************************************************************/


#ifdef EGTP_C

/* Message Codes   */
#define EG_S11_BRERESCMD_CMD_CODE      68        

#ifdef EG_REL_AC0 
#define EG_S11_BRERESCMD_NMB_IES       12
#else
#define EG_S11_BRERESCMD_NMB_IES       11
#endif

#define EG_S11_BRERESFLRIND_CMD_CODE   69        
#define EG_S11_BRERESFLRIND_NMB_IES    5         
#define EG_S11_CHGNTFREQ_CMD_CODE      38        
#ifdef EG_REL_AC0 
#define EG_S11_CHGNTFREQ_NMB_IES       10        
#elif EG_REL_930 
#define EG_S11_CHGNTFREQ_NMB_IES       8        
#else
#define EG_S11_CHGNTFREQ_NMB_IES       6        
#endif 
#define EG_S11_CHGNTFRSP_CMD_CODE      39        
#ifdef EG_REL_AC0 
#define EG_S11_CHGNTFRSP_NMB_IES       5         
#elif EG_REL_930
#define EG_S11_CHGNTFRSP_NMB_IES       4         
#else
#define EG_S11_CHGNTFRSP_NMB_IES       3         
#endif 
#define EG_S11_CRTBREREQ_CMD_CODE      95

#if defined(EG_REL_930) || defined(EG_REL_AC0) 
#define EG_S11_CRTBREREQ_NMB_IES       9         
#else 
#define EG_S11_CRTBREREQ_NMB_IES       8         
#endif

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 7         
#else 
#define EG_CRTBREREQ_BEARER_CNTX0_GRP_NMB_IES 6      
#endif 

#define EG_S11_CRTBRERSP_CMD_CODE      96        
#define EG_S11_CRTBRERSP_NMB_IES       9     

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 5         
#else 
#define EG_CRTBRERSP_BEARER_CNTX0_GRP_NMB_IES 4
#endif

#define EG_S11_CRTFWDTUNREQ_CMD_CODE   160       
#define EG_S11_CRTFWDTUNREQ_NMB_IES    2         
#define EG_S11_CRTFWDTUNRSP_CMD_CODE   161       
#define EG_S11_CRTFWDTUNRSP_NMB_IES    3         
#define EG_S11_CRTINDDATFWDTUNREQ_CMD_CODE 166       
#ifdef EG_REL_AC0 
#define EG_S11_CRTINDDATFWDTUNREQ_NMB_IES 7         
#else 
#define EG_S11_CRTINDDATFWDTUNREQ_NMB_IES 5         
#endif 
#define EG_CRTINDDATFWDTUNREQ_BEARER_CNTX0_GRP_NMB_IES 7         
#define EG_S11_CRTINDDATFWDTUNRSP_CMD_CODE 167       
#define EG_S11_CRTINDDATFWDTUNRSP_NMB_IES 5         
#define EG_CRTINDDATFWDTUNRSP_BEARER_CNTX0_GRP_NMB_IES 6         
#define EG_S11_CRTSESREQ_CMD_CODE      32        

#ifdef EG_REL_AC0 
#define EG_S11_CRTSESREQ_NMB_IES       28
#elif EG_REL_930
#define EG_S11_CRTSESREQ_NMB_IES       26
#else
#define EG_S11_CRTSESREQ_NMB_IES       25
#endif

#define EG_CRTSESREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#define EG_CRTSESREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S11_CRTSESRSP_CMD_CODE      33        

#ifdef EG_REL_AC0
#define EG_S11_CRTSESRSP_NMB_IES       18
#elif EG_REL_930
#define EG_S11_CRTSESRSP_NMB_IES       16
#else
#define EG_S11_CRTSESRSP_NMB_IES       15
#endif

#define EG_CRTSESRSP_BEARER_CNTX0_GRP_NMB_IES 7

#define EG_CRTSESRSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S11_DELBRECMD_CMD_CODE      66        
#define EG_S11_DELBRECMD_NMB_IES       2         
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 2
#else
#define EG_DELBRECMD_BEARER_CNTX0_GRP_NMB_IES 1
#endif
#define EG_S11_DELBREFLRIND_CMD_CODE   67        
#define EG_S11_DELBREFLRIND_NMB_IES    4         
#define EG_DELBREFLRIND_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S11_DELBREREQ_CMD_CODE      99        
#define EG_S11_DELBREREQ_NMB_IES       9         
#define EG_DELBREREQ_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_S11_DELBRERSP_CMD_CODE      100       
#define EG_S11_DELBRERSP_NMB_IES       9    

#if defined(EG_REL_930)|| defined(EG_REL_AC0)
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#else 
#define EG_DELBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#endif

#define EG_S11_DELINDDATFWDTUNREQ_CMD_CODE 168       
#define EG_S11_DELINDDATFWDTUNREQ_NMB_IES 1         
#define EG_S11_DELINDDATFWDTUNRSP_CMD_CODE 169       
#define EG_S11_DELINDDATFWDTUNRSP_NMB_IES 3         
#define EG_S11_DELPDNCONSETREQ_CMD_CODE 101       
#define EG_S11_DELPDNCONSETREQ_NMB_IES 2
#define EG_S11_DELPDNCONSETRSP_CMD_CODE 102       
#define EG_S11_DELPDNCONSETRSP_NMB_IES 2         
#define EG_S11_DELSESREQ_CMD_CODE      36

#ifdef EG_REL_AC0 
#define EG_S11_DELSESREQ_NMB_IES       9         
#else 
#define EG_S11_DELSESREQ_NMB_IES       7         
#endif

#define EG_S11_DELSESRSP_CMD_CODE      37        
#define EG_S11_DELSESRSP_NMB_IES       4         
#define EG_S11_DWLNKDATNOT_CMD_CODE    176       

#ifdef EG_REL_AC0
#define EG_S11_DWLNKDATNOT_NMB_IES     5         
#else
#define EG_S11_DWLNKDATNOT_NMB_IES     2
#endif

#define EG_S11_DWLNKDATNOTACK_CMD_CODE 177      

#ifdef EG_REL_AC0
#define EG_S11_DWLNKDATNOTACK_NMB_IES  6
#else
#define EG_S11_DWLNKDATNOTACK_NMB_IES  4
#endif

#define EG_S11_DWLNKDATNOTFLRIND_CMD_CODE 70        
#ifdef EG_REL_AC0 
#define EG_S11_DWLNKDATNOTFLRIND_NMB_IES 4         
#else 
#define EG_S11_DWLNKDATNOTFLRIND_NMB_IES 2         
#endif 
#define EG_S11_MODACCBREREQ_CMD_CODE   211       

#ifdef EG_REL_AC0
#define EG_S11_MODACCBREREQ_NMB_IES    7
#define EG_MODACCBREREQ_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_MODACCBREREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S11_MODACCBRERSP_CMD_CODE   212       
#define EG_S11_MODACCBRERSP_NMB_IES    5
#define EG_MODACCBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_MODACCBRERSP_BEARER_CNTX1_GRP_NMB_IES 2         
#endif

#define EG_S11_MODBRECMD_CMD_CODE      64        
#define EG_S11_MODBRECMD_NMB_IES       3         
#define EG_MODBRECMD_BEARER_CNTX0_GRP_NMB_IES 2         
#define EG_S11_MODBREFLRIND_CMD_CODE   65        
#define EG_S11_MODBREFLRIND_NMB_IES    3         
#define EG_S11_MODBREREQ_CMD_CODE      34        

#ifdef EG_REL_AC0
#define EG_S11_MODBREREQ_NMB_IES       15
#elif EG_REL_930
#define EG_S11_MODBREREQ_NMB_IES       14
#else
#define EG_S11_MODBREREQ_NMB_IES       13
#endif

#define EG_MODBREREQ_BEARER_CNTX0_GRP_NMB_IES 4         
#define EG_MODBREREQ_BEARER_CNTX1_GRP_NMB_IES 1         
#define EG_S11_MODBRERSP_CMD_CODE      35        

#ifdef EG_REL_AC0
#define EG_S11_MODBRERSP_NMB_IES       16
#elif EG_REL_930
#define EG_S11_MODBRERSP_NMB_IES       15
#else
#define EG_S11_MODBRERSP_NMB_IES       14
#endif

#define EG_MODBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#define EG_MODBRERSP_BEARER_CNTX1_GRP_NMB_IES 2         
#define EG_S11_PGWRSTNOT_CMD_CODE      179       
#define EG_S11_PGWRSTNOT_NMB_IES       3         
#define EG_S11_PGWRSTNOTACK_CMD_CODE   180       
#define EG_S11_PGWRSTNOTACK_NMB_IES    2         
#define EG_S11_RELACCBREREQ_CMD_CODE   170       
#ifdef EG_REL_AC0 
#define EG_S11_RELACCBREREQ_NMB_IES    2         
#else 
#define EG_S11_RELACCBREREQ_NMB_IES    1         
#endif 
#define EG_S11_RELACCBRERSP_CMD_CODE   171       
#define EG_S11_RELACCBRERSP_NMB_IES    3         
#define EG_S11_RESUMEACK_CMD_CODE      165       
#define EG_S11_RESUMEACK_NMB_IES       2         
#define EG_S11_RESUMENOT_CMD_CODE      164       
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_S11_RESUMENOT_NMB_IES       3         
#else
#define EG_S11_RESUMENOT_NMB_IES       2         
#endif
#define EG_S11_STOPPAGINGIND_CMD_CODE  73        
#define EG_S11_STOPPAGINGIND_NMB_IES   2         
#define EG_S11_SUSPENDACK_CMD_CODE     163       
#define EG_S11_SUSPENDACK_NMB_IES      2         
#define EG_S11_SUSPENDNOT_CMD_CODE     162       
 /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_S11_SUSPENDNOT_NMB_IES      3         
#else
#define EG_S11_SUSPENDNOT_NMB_IES      2         
#endif
#define EG_S11_TRACESESSACTV_CMD_CODE  71        
#define EG_S11_TRACESESSACTV_NMB_IES   3         
#define EG_S11_TRACESESSDEACTV_CMD_CODE 72        
#define EG_S11_TRACESESSDEACTV_NMB_IES 1         
#define EG_S11_UPDBREREQ_CMD_CODE      97        
 /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined( EG_REL_AC0)
#define EG_S11_UPDBREREQ_NMB_IES       9         
#else 
#define EG_S11_UPDBREREQ_NMB_IES       8
#endif 
#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 5         
#else 
#define EG_UPDBREREQ_BEARER_CNTX0_GRP_NMB_IES 4
#endif 
#define EG_S11_UPDBRERSP_CMD_CODE      98        
#define EG_S11_UPDBRERSP_NMB_IES       10        

#if defined(EG_REL_930) || defined(EG_REL_AC0)
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 3         
#else 
#define EG_UPDBRERSP_BEARER_CNTX0_GRP_NMB_IES 2         
#endif 

#endif /* End of EGTP_C */


#ifdef EGTP_U

/* GTP-U Message Types */
#define EGU_ECHO_REQ          1
#define EGU_ECHO_RSP          2
#define EGU_ERROR_IND         26
#define EGU_SUPP_EXT_HDR_NTF  31
#define EGU_END_MARKER        254

/* GTP-U IEs */
#define EGU_RECOVERY_IETYPE   14 
#define EGU_TEID_IETYPE       16
#define EGU_GSN_ADDR_IETYPE   133
#define EGU_EXT_HDR_LST_IETYPE 141
#define EGU_PVT_EXTN_IETYPE   255


#endif /* End of EGTP_U */



/********************************************************************30**

         End of file:     eg_dbs11.h@@/main/3 - Fri Feb  4 18:18:03 2011

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
/main/1      ---      an                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

