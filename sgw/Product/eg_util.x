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
 
    Name:   EGTP layer
 
    Type:   C include file
 
    Desc:   Structures, variables and typedefs required by EGTP
 
    File:   eg_util.x
 
    Sid:      eg_util.x@@/main/3 - Fri Feb  4 18:18:14 2011
 
    Prg:    sn
 
*********************************************************************21*/

#ifndef __EGUTILX__
#define __EGUTILX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***************************************
*      EGT      DATA       TYPES       *
***************************************/
typedef enum _egDataTypes
{
  EG_FTEID     =1,
  EG_UNSIGNED_8 ,
  EG_UNSIGNED_16,
  EG_UNSIGNED_32,
  EG_CAUSE  ,
  EG_PVT_EXTN   ,
  EG_TRC_INFO   ,
  EG_ULI_DATA   ,
  EG_FQOS       ,
  EG_BQOS_IEDATA =10, 
  EG_PAA_IEDATA ,
  EG_GLOBAL_CNID, 
  EG_STRING_8   ,
  EG_STRING_16  ,
  EG_STRING_32  , 
  EG_STRING_132 ,    
  EG_STRING_MAX ,   
  EG_SNETWORK  ,  
  EG_INDICATION , 
  EG_BEARER_FLAG =20,
  EG_GROUPED    ,
  EG_IP_ADDR    ,
  EG_S103_PDN_FW_INFO,
  EG_S1U_DATA_FW,
  EG_PDUNUM,
  EG_UE_TIME_ZONE,
  EG_TRACE_REF,
  EG_RAB_CONTEXT,
  EG_GSM_KEY_TRIP,
  EG_UMTS_KEY_CIPH_QUIN =30,
  EG_GSM_KEY_CIPH_QUIN,
  EG_UMTS_KEY_QUIN,
  EG_EPS_SEC_QUAD_QUIN,
  EG_UMTS_KEY_QUAD_QUIN,
  EG_AUTH_QUIN,
  EG_AUTH_QUAD = 36,
  EG_CMP_REQ,
  EG_GUTI,
  EG_F_CONT,
  EG_F_CAUSE =40,
  EG_TRGTID,
  EG_SRCID,
  EG_PKTFLOWID,
  EG_SRC_RNC_PDCP_CTX,
  EG_PDN_CONN_SET_ID =45,
  EG_AMBR,
  EG_GLOBAL_CN_ID,
#if defined(EG_REL_930) || defined(EG_REL_AC0)
  EG_MBMS_IPMCAST_DIST, /* MBMS IP Multicast distribution */
  EG_UCI,               /* user CSG Information */
  EG_CSG_INFO_REP_ACT =50, /* CSG info reporting action */
#endif
  EG_EXT_HDR_TYPE_LST,
  EG_BCD,      /*fix for ccpu00135094 */
  EG_STRING_400 ,    
 /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
  EG_ARP,
  EG_EPC_TMR = 55,
  EG_MDT_CONF,
  EG_CNG_TO_RPT_FLG,
  EG_ADD_MM_CNTXT_SRVCC,
  EG_STN_SR,
  EG_THROTTLING = 60,
#endif
  EG_MAX_DATATYPE    /* This data type should be at the end of the list */
}EgDataTypes;

/*********************************************************************************
 **********************************Utility Procedures*****************************
 *********************************************************************************/
#ifdef EGTP_C

/** @defgroup UtilityProcedures
* @{
*/

/**

  @brief The eGTP message will be initialized using this API, that is, memory will be
allocated for the message.
 
   @details The eGTP message will be initialized using this API, that is, 
memory is allocated for the message and the members inside the message will be
initialized with the default values, and the memory for the list of IEs too will be
allocated and initialized.<br>

<b>Note</b>: The user has to pass the same message for appending the IEs without
changing any of the memory information inside the message.
The eGTP message has to be allocated first using this API and before
using any other API. The eGTP message has to be de-allocated using
the EgUtilDeAllocGmMsg API only.

   @param[in] egMsg The eGTP message to which the memory has to be allocated.<br>
It is an INOUT parameter for this method.

   @param[in] memInfo The region and pool information from where the memory has to be
allocated for the IEs and other internal structures.
It is an IN parameter for this method.
   
   @return  ROK  - If memory allocation and initialization is success. <br>
            EG_UTIL_ERR_MEM_ALLOC_FAILED - In case of memory allocation fails. <br>
            EG_UTIL_ERR_INV_PARAM - If the given message is invalid. <br>
   */

EXTERN S16 EgUtilAllocGmMsg ARGS((EgMsg **egMsg, Mem *memInfo));

/**
  @brief The eGTP message will be de-initialized using this API, that is, memory will be
de-allocated.

  @details The eGTP message will be de-initialized using this API, that is, memory is
de-allocated and the members inside the message will be de-initialized. The
memory for the list of IEs too will be de-allocated including the nested IEs.

  @param[in] egMsg The eGTP message, which has to be de-allocated.

  @return   ROK  - If memory de-allocation and initialization is success. <br>
            RFAILED - In case of memory de-allocation fails. <br> 
            EG_UTIL_ERR_INV_PARAM - If the given message is invalid. <br>
            Return value of the child methods (if any).

  */

EXTERN S16 EgUtilDeAllocGmMsg ARGS ((EgMsg **egMsg));


/**
  @brief This method is used for setting the header part of the eGTP message.
  
  @param[in] egMsg The actual eGTP message in to which the header has to be
filled.

  @param[in] egHdr The header of the eGTP message.

  @return   ROK  - If header part of the message is set successfully. <br>
            EG_UTIL_ERR_INV_PARAM - If the given message is invalid.

  */

EXTERN S16 EgUtilSetGtpHdr ARGS ((EgMsg *egMsg, EgMsgHdr egHdr));


/**
  @brief This method is used for getting the eGTP message header from the eGTP
message.

  @details This method is used for getting the eGTP message header from the eGTP
message. The header in the eGTP message will be extracted and will be copied
into the given parameter. The header in the eGTP message will not be changed.

  @param[in] egMsg The actual eGTP message, from which the header has to be
retrieved.
It is an IN parameter for this method.

  @param[in] egHdr The header of the eGTP inside the message will be filled into
this variable.<br>
It is an OUT parameter for this method.

  @return ROK  - If header part of the message is retreived successfully. <br>
          EG_UTIL_ERR_INV_PARAM - If the given message or header are invalid.

  */

EXTERN S16 EgUtilGetGtpHdr ARGS((EgMsg *egMsg, EgMsgHdr *egHdr));

/**
  @brief This method is used for appending an IE to the eGTP message.

  @details This method is used for appending an IE to the eGTP message. The level
information will specify where the IE has to be appended, that is, if the IE has
to be appended to a grouped IE, the level information will specify the level of the
grouped IE inside the IE list, grouped IE code, occurrence or position of the
grouped IE in that level.

  @param[in] egMsg The eGTP message, to which the IE has to be appended.<br>
It is an INOUT parameter for this method.

  @param[in] ieLvlInfo The level information of the message, where the IE has to be
appended.<br>
It is an IN parameter for this method.

  @param[in] egIe The IE which has to be appended.<br>
It is an IN parameter for this method.

  @return ROK - If the IE is appended successfully. <br>
          RFAILED - If appending IE fails. <br>
          EG_UTIL_ERR_MEM_ALLOC_FAILED - If memory allocation for the new IE fails. <br>
          EG_UTIL_ERR_INV_PARAM - If the given message or ieLvlInfo or IE are invalid
          Return value of the child method (if any).
  
  */

EXTERN S16 EgUtilGmAppendIe ARGS((EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, EgIe *egIe));

/**
  @brief This method is used for getting reference of an IE which is at a particular level in
the eGTP message.

  @details This method is used for getting reference of an IE which is at a particular level in
the eGTP message. Even if the IE inside the message is in the zero level, the
user has to fill the level information with the IE code and the occurrence
information.

  @param[in] egMsg The eGTP message from which the IE has to be retrieved.<br>
  It is an IN parameter for this method.

  @param[in] ieLvlInfo The level information of the message, from where the IE has to
be retrieved. This includes the IE code and the occurrence of the
IE in that level.<br>
It is an IN parameter for this method.

  @param[in] egIe The IE where the reference of IE from message has to be filled.<br>
  It is an INOUT parameter for this method.

  @param[in] iePos This will hold the position of the IE in the list. This will have
relevance only if the IE has to be retrieved using the IE code
and the occurrence count.<br>
It is an OUT parameter for this method.

  @return ROK - If the IE is retreived successfully. <br>
          RFAILED - If retreiving IE fails. <br>
          EG_UTIL_ERR_INV_LEVEL_INFO - If the given level information is invalid. <br>
          EG_UTIL_ERR_INV_PARAM - If the given message or end-to-end id are invalid 
          Return value of the child method (if any).

  */

EXTERN S16 EgUtilGmGetIe ARGS((EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, EgIe **egIe, U16 *iePos));

/**
  @brief This method is used for deleting an IE which is at a particular level in the
GTP message.

  @details
This method is used for deleting an IE which is at a particular level in the
GTP message. Even if the IE inside message is in the zero level, the user has
to fill the level information with the IE code and the occurrence information.

  @param[in] egMsg The eGTP message from which the IE has to be deleted.<br>
  It is an INOUT parameter for this method.

  @param[in] levelInfo The level information of the message, from where the IE has to
be deleted. This includes the IE code and the occurrence of the
IE in that level.<br>
It is an IN parameter for this method.

  @param[in] allOccr This will specify whether to delete all occurrences of the IE or
not.<br>
It is an IN parameter for this method.

  @return  ROK - If the IE is deleted successfully.<br>
           RFAILED - If deleting IE fails.<br>
           EG_UTIL_ERR_INV_LEVEL_INFO - If the given level information is invalid.<br>
           EG_UTIL_ERR_INV_PARAM - If the given message or IE level information id are invalid
           Return value of the child method (if any).
  */

EXTERN S16 EgUtilGmDelIe ARGS(( EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, Bool allOccr));

/**
  @brief This method is used for duplicating a eGTP message.
  @details This method is used for duplicating a eGTP message, this includes all the IEs
at all the levels in the original eGTP message. The memory will be allocated for
the eGTP message in the region and pool given in the mem details.

  @param[in] egMsg The eGTP message, which has to be duplicated.<br>
It is an IN parameter for this method.

  @param[in] dupEgMsg The original eGTP message egMsg will be copied into this
variable.<br>
It is an OUT parameter for this method.

  @param[in] memInfo The memory information (region and pool) from where the
memory has to be allocated for the duplicated eGTP message.
It is an IN parameter for this method.

  @return ROK - If the eGTP message is duplicated successfully. <br>
          RFAILED - If the eGTP message duplicating fails. <br>
          EG_UTIL_ERR_MEM_ALLOC_FAILED - If memory allocation for the new eGTP message fails. <br>
          EG_UTIL_ERR_INV_PARAM - If the given message or memory information are invalid
           Return value of the child method (if any).
  */

EXTERN S16 EgUtilGmDup ARGS((EgMsg *egMsg, EgMsg **dupEgMsg, Mem *memInfo));

/**
  @brief This method is used for getting the IE count from the IE List.

  @param[in] egMsg The eGTP message, from which the IE count has to be
calculated.<br>
It is an IN parameter for this method.

  @param[in] ieLvlInfo The level information of the message, from where the IE count
has to be retrieved. This includes the IE code and the occurrence
of the IE in that level.<br>
It is an IN parameter for this method.

  @param[in] ieCount The IE Count in that level will be filled into this variable.<br>
It is an OUT parameter for this method.

  @return ROK - If the IE count is retreived successfully. <br>
          EG_UTIL_ERR_INV_LEVEL_INFO - If the given level information is invalid.<br>
          EG_UTIL_ERR_INV_PARAM - If the given message or IE level information<br>
                   or ieCnt are invalid Return value of the child method (if any).

  */

EXTERN S16 EgUtilGmGetIeCnt ARGS((EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, S16 *ieCount));

/**
  @brief This method is used for dumping the eGTP message on console.

  @param[in] egMsg The eGTP message, which need to dumped on console.<br>
  This parameter is an IN parameter for this method.

  @return ROK - If the IE list is duplicated successfully. <br>
  EG_UTIL_ERR_INV_PARAM - If the given message is invalid.

  */

EXTERN S16 EgUtilGmDump ARGS((EgMsg *egMsg));

/**
  @brief This method is used to get the occurrence count of an IE at a particular level in the
  eGTP message.

  @param[in] egMsg The eGTP message, which need to dumped on console.<br>
  This parameter is an IN parameter for this method.

  @param[in] ieLvlInfo The level information of the message, where the IE counts must
be calculated. This includes the IE code and the maximum
occurrences of the IE in that level.<br>
This parameter is an IN parameter for this method.

  @param[out] ieCount This will hold the number of IEs that are present with the given
IE code and at a particular level.<br>
It is an OUT parameter for this method.

  @return ROK - If the IE list is duplicated successfully.<br>
          EG_UTIL_ERR_INV_LEVEL_INFO - If the given level information is invalid.<br>
          EG_UTIL_ERR_INV_PARAM - If the given message or level information are invalid.
  */
/*eg012.201: signedness warning fixed */
EXTERN S16 EgUtilGmGetIeOccrCnt ARGS((EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo, U16 *ieOccrCnt));


#ifndef EGTP_U
/** @} */ /* End of Utility procedures */
#endif

/*****************************************
 * Utility function to delete grouped IE *
 *****************************************/
PUBLIC S16 EgUtilGmDelGrpIe ARGS ( (EgMsg *egMsg, EgIeLevelInfo *ieLvlInfo));

/*******************************************************
 * This is used to get a particular IE from an IE list *
 *******************************************************/
PUBLIC S16 egUtilGetTrgtIe ARGS( (CmLListCp *ieList, EgIeInfo *ieInfo, EgIe **egIe, U16 *iePos));

PUBLIC S16 egUtilDelIe ARGS( (CmLListCp *ieList, EgIe *egIe, Bool allOccr));

#endif /* End of EGTP_C */

#ifdef EGTP_U

#ifndef EGTP_C
/** @defgroup UtilityProcedures
* @{
*/
#endif

/**

  @brief The eGTP-U message will be initialized using this API, that is, memory will be
allocated for the message.
 
   @details The eGTP-U message will be initialized using this API, that is, 
memory is allocated for the message and the members inside the message will be
initialized with the default values, and the memory for the list of IEs too will be
allocated and initialized.<br>

<b>Note</b>: The user has to pass the same message for appending the IEs without
changing any of the memory information inside the message.
The eGTP-U message has to be allocated first using this API and before
using any other API. The eGTP-U message has to be de-allocated using
the EgUUtilDeAllocGmMsg API only.

   @param[in] egMsg The eGTP-U message to which the memory has to be allocated.<br>
It is an INOUT parameter for this method.

   @param[in] memInfo The region and pool information from where the memory has to be
allocated for the IEs and other internal structures.
It is an IN parameter for this method.
   
   @return  ROK  - If memory allocation and initialization is success. <br>
            EG_UTIL_ERR_MEM_ALLOC_FAILED - In case of memory allocation fails. <br>
            EG_UTIL_ERR_INV_PARAM - If the given message is invalid. <br>
   */

PUBLIC S16 EgUUtilAllocGmMsg ARGS ((EgUMsg **egMsg, Mem *memInfo ));

/**
  @brief The eGTP-U message will be de-initialized using this API, that is, memory will be
de-allocated.

  @details The eGTP-U message will be de-initialized using this API, that is, memory is
de-allocated and the members inside the message will be de-initialized. The
memory for the list of IEs / mBuf incase of G-PDUs too will be de-allocated.

  @param[in] egMsg The eGTP-U message, which has to be de-allocated.

  @return   ROK  - If memory de-allocation and initialization is success. <br>
            RFAILED - In case of memory de-allocation fails. <br> 
            EG_UTIL_ERR_INV_PARAM - If the given message is invalid. <br>
            Return value of the child methods (if any).
  */

PUBLIC S16 EgUUtilDeAllocGmMsg ARGS ((EgUMsg **egMsg));

/**
  @brief This method is used for appending an IE to the eGTP-U message.

  @details This method is used for appending an IE to the eGTP-U message. The IE will be
           appended at the end of the existing IEs.

  @param[in] egMsg The eGTP-U message, to which the IE has to be appended.<br>
It is an INOUT parameter for this method.

  @param[in] egIe The IE which has to be appended.<br>
It is an IN parameter for this method.

  @return ROK - If the IE is appended successfully. <br>
          RFAILED - If appending IE fails. <br>
          EG_UTIL_ERR_MEM_ALLOC_FAILED - If memory allocation for the new IE fails. <br>
          EG_UTIL_ERR_INV_PARAM - If the given message or ieLvlInfo or IE are invalid
          Return value of the child method (if any).
  */

PUBLIC S16 EgUUtilGmAppendIe ARGS ((EgUMsg *egMsg, EgUIe *egIe ));

/**
  @brief This method is used for deleting an IE from a eGTP-U message

  @details This method is used for deleting an IE inside the eGTP-U message. 
           Deletion is done based on the IE type and the occurrence information.

  @param[in] egMsg The eGTP-U message from which the IE has to be deleted.<br>
  It is an INOUT parameter for this method.

  @param[in] ieType IE type to be deleted.<br>
  It is an IN parameter for this method.

  @param[in] occrCnt Occurrence count of the IE to be deleted.<br>
It is an IN parameter for this method.

  @return  ROK - If the IE is deleted successfully.<br>
           RFAILED - If deleting IE fails.<br>
           EG_UTIL_ERR_INV_PARAM - If the given message or IE level information id are invalid
           Return value of the child method (if any).
  */

PUBLIC S16 EgUUtilGmDelIe ARGS (( EgUMsg *egMsg,U8 ieType,U8 occrCnt ));

/**
  @brief This method is used to get the occurrence count of an IE in the eGTP-U message.

  @param[in] egMsg The eGTP-U message, which needs to be searched.<br>
  This parameter is an IN parameter for this method.

  @param[in] ieType IE type to be searched.<br>
  It is an IN parameter for this method.

  @param[out] ieOccrCnt This will hold the number of occurrances of the IE.<br>
It is an OUT parameter for this method.

  @return ROK - If the IE list is successfully searched.<br>
          EG_UTIL_ERR_INV_PARAM - If the given message or level information are invalid.
  */

PUBLIC S16 EgUUtilGmGetIeOccrCnt ARGS (( EgUMsg *egMsg, U8 *ieType, 
                                         U16 *ieOccrCnt ));

#ifdef EG_FILE_LOG
/**
  @brief This method is used for dumping the eGTP-U message to a file.

  @param[in] egMsg The eGTP-U message, which need to dumped to a file.<br>
  This parameter is an IN parameter for this method.

  @param[in] fp File Pointer to the file which needs to be filled.<br>
  This parameter is an INOUT parameter for this method.

  @param[in] nmbLines Number of lines<br>
  This parameter is an IN parameter for this method.

  @return ROK - If the IE list is duplicated successfully. <br>
  EG_UTIL_ERR_INV_PARAM - If the given message is invalid.

  */
PUBLIC S16 EgUUtilGmDump ARGS (( EgUMsg *egMsg, FILE  *fp, U16 *nmbLines ));
#else
/**
  @brief This method is used for dumping the eGTP-U message on console.

  @param[in] egMsg The eGTP-U message, which need to dumped on console.<br>
  This parameter is an IN parameter for this method.

  @return ROK - If the IE list is duplicated successfully. <br>
  EG_UTIL_ERR_INV_PARAM - If the given message is invalid.

  */
PUBLIC S16 EgUUtilGmDump ARGS (( EgUMsg *egMsg ));
#endif

/** @} */ /* End of Utility procedures */

PUBLIC S16 egUUtilCmpTptAddr ARGS((CmTptAddr *srcAddr, CmTptAddr *destAddr, U16 *port));
#endif /* End of EGTP_U */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EGUTILX__ */


/**********************************************************************
 
         End of file:     eg_util.x@@/main/3 - Fri Feb  4 18:18:14 2011
 
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
/main/1      ---      sn                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya          1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3   eg012.201   shpandey          1. klockwork warning fixes added  
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/
