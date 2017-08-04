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


/**********************************************************************
 
     Name:     Trillium LTE CNE - MME
  
     Type:     C include file
  
     Desc:     C structures for all CNE nodes MME
  
     File:     vb.h
  
     Sid:      vb.h@@/main/1 - Mon Jun 28 19:35:34 2010
  
     Prg:      sv

**********************************************************************/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*      
*
*/

#ifndef __VBH__
#define __VBH__

/* vb007.101 Added support to send DNS Ip */
#define VB_MAX_LEN_PROT_CFG_OPT 247
#define VB_HASH_LIST_SIZE 1024

#define VB_SZT_SAP_SEL    0

#define VB_SAP_NOT_BOUND  0
#define VB_SAP_BOUND      1

#define VB_MME_CONNST_IDLE        0
#define VB_MME_CONNST_CONNECTING  1
#define VB_MME_CONNST_ESTABLISHED 2

#define VB_MME_TMR_DATA 1
#define VB_MME_TQSIZE   64

/* TODO: Check them : */ 
/* Protocol Discriminator*/
#define VB_EMM_MSG       1
#define VB_ESM_MSG       2
#define VB_MME_APP_TQSIZE   64

#define VB_MAX_UETUNL_BIN   1024
#define VB_MAX_ENODEB_BIN   10
#define VB_MAX_UECB_BIN     1024
/* vb006.101: ccpu00118283, Maximum IMSI GUTI map CB in hashlist */
#define VB_MAX_IMSIGUTIMAPCB_BIN     1024


#define VB_EMM_UE_MAX_EVENTS  19

#define VB_EMM_UE_MAX_STATES  5

#define VB_MME_EPS_DFLT_BEARER_ID 5

/* Defines used in utilities */
#define VB_UTL_S1_SDU_SIZE 2048

/*-- Different memory types --*/
#define VB_MEMTYPE_EVNT  1
#define VB_MEMTYPE_MSG   2
#define VB_MEMTYPE_BUF   3

/*-- Different memory modes --*/
#define VB_MEMMODE_ALLOC 1
#define VB_MEMMODE_FREE  2
#define VB_MEMMODE_SEND  3
#define VB_MEMMODE_RCV   4

#define VB_UTIL_MEM_SIZE  1024



#define VB_MME_ENC_U32(_val, _buf)                                      \
{                                                                       \
   _buf[0] = (_val & 0xff000000) >> 24;                                 \
   _buf[1] = (_val & 0x00ff0000) >> 16;                                 \
   _buf[2] = (_val & 0x0000ff00) >> 8;                                  \
   _buf[3] = (_val & 0x000000ff);                                       \
}

#define VB_MME_EMM_RE_ATTACH_REQD      1
#define VB_MME_EMM_RE_ATTACH_NOT_REQD  2

#define VB_HOME_ENB_ID_BYTESTR_LEN     4
#define VB_HOME_ENB_ID_BITSTR_LEN      28

/* vb005.101: Added support for Network initiated Detach ccpu00117691 */
/* Detach type network to UE direction */
#define VB_MME_RE_ATTACH_REQ        0x01
#define VB_MME_RE_ATTACH_NOT_REQ      0x02
#define VB_MME_RE_IMSI_DETACH         0x03

/* PDN Context SubContext Information */
#define DFLT_QCI                      0x01

/* These hash defines are borrowed from EGTP product,
 * make sure they are in-sync */
/* vb005.101 Changes */
#ifndef EG_REL_930
#define  EG_CAUSE_IETYPE               2
#define  EG_BEARER_CNTX_IETYPE         96
#define  EG_PAA_IETYPE                 79
#define EG_FTEID_IETYPE                88
/* vb001.101: Added TID & TFT IE types */
#define  EG_PROCEDURE_TID_IETYPE       105
#define EG_BEARER_TFT_IETYPE           85     
#endif /* EG_REL_930 */

#define VB_ENB_NAME_LEN                25
#define VB_TAC_LIST_LEN                10
#define VB_PLMN_ID_LEN                 3
#define VB_PLMN_LIST_LEN               6

#define VB_MME_BEAR_STUP_SUCCESS       0
#define VB_MME_BEAR_STUP_ONGOING       1
#define VB_MME_BEAR_STUP_FAILURE       2
#define VB_MME_BEAR_REL_SUCCESS       0
#define VB_MME_BEAR_REL_ONGOING       1
#define VB_MME_BEAR_REL_FAILURE       2

#define VB_MME_BEAR_MOD_SUCCESS       0
#define VB_MME_BEAR_MOD_ONGOING       1
#define VB_MME_BEAR_MOD_FAILURE       2

#define VB_TMR_EMM_MOB_REACHABLE_OFFSET            0xF0   /* 4 Min */

/* ESM Failure Causes */
#define VB_ESM_INV_CAUSE                           0xFF
#define VB_ESM_OP_DET_BARRING                      0x08
#define VB_ESM_INSUF_RES                           0x1A
#define VB_ESM_UNKNOWN_OR_MISSING_APN              0x1B
#define VB_ESM_UNKNOWN_PDN_TYPE                    0x1C
#define VB_ESM_USER_AUTH_FAILED                    0x1D
#define VB_ESM_REQ_REJ_BY_SGW_OR_PGW               0x1E
#define VB_ESM_REQ_REJ_UNSPECIFIED                 0x1F
#define VB_ESM_SERV_OPT_NOT_SUPP                   0x20
#define VB_ESM_REQ_SERV_OPT_NOT_SUBS               0x21
#define VB_ESM_SERV_OPT_TEMP_OUT_OF_ORDER          0x22
#define VB_ESM_PTI_ALREADY_IN_USE                  0x23
#define VB_ESM_REGULAR_DEACTIVATION                0x24
#define VB_ESM_EPS_QOS_NOT_ACCEPTED                0x25
#define VB_ESM_NETWORK_FAILURE                     0x26
#define VB_ESM_SEM_ERR_IN_TFT_OPER                 0x29
#define VB_ESM_SYN_ERR_IN_TFT_OPER                 0x2A
#define VB_ESM_INV_EPS_BEARER_ID                   0x2B
#define VB_ESM_SEM_ERR_IN_PACKET_FILTER            0x2C
#define VB_ESM_SYN_ERR_IN_PACKET_FILTER            0x2D
#define VB_ESM_EPS_BEARER WITHOUT_TFT_ACTV         0x2E
#define VB_ESM_PTI_MISMATCH                        0x2F
#define VB_ESM_LAST_PDN_DISCON_NOT_ALWD            0x31     
#define VB_ESM_PDN_IPV4_ONLY_ALWD                  0x32
#define VB_ESM_PDN_IPV6_ONLY_ALWD                  0x33
#define VB_ESM_SIN_ADDR_BEARER_ONLY_ALWD           0x34
#define VB_ESM_ESM_INFO_NOT_RECV                   0x35
#define VB_ESM_PDN_CONN_NOT_EXIST                  0x36
#define VB_ESM_MULT_PDN_CONN_FOR_APN               0x37
#define VB_ESM_COLLISION_WITH_NW_REQ               0x38
#define VB_ESM_UNSUPPORTED_QCI_VAL                 0x3B
#define VB_ESM_INV_PTI_VAL                         0x51
#define VB_ESM_SEM_INCORRECT_MSG                   0x5F
#define VB_ESM_INV_MAND_INFO                       0x60
#define VB_ESM_MSG_TYPE_NOT_EXIST                  0x61
#define VB_ESM_MSG_TYPE_INCOMP_WITH_PROTO_STATE    0x62
#define VB_ESM_IE_NOT_EXIST                        0x63
#define VB_ESM_COND_IE_ERROR                       0x64
#define VB_ESM_MSG_INCOMP_WITH_PROTO_STATE         0x65
#define VB_ESM_VB_ESM_PROTOCOL_ERROR               0x6F
#define VB_ESM_VB_ESM_APN_RES_INCOMP               0x70

#define VB_EMM_TAU_WITH_SGW_CHANGE                 (1 << 0)
#define VB_EMM_TAU_WITHOUT_SGW_CHANGE              (1 << 1)
#define VB_EMM_TAU_WITH_MME_CHANGE                 (1 << 2)
#define VB_EMM_TAU_WITHOUT_MME_CHANGE              (1 << 3)
#define VB_EMM_HO_WITH_SGW_CHANGE                  (1 << 4)
#define VB_EMM_HO_WITHOUT_SGW_CHANGE               (1 << 5)
#define VB_EMM_HO_WITH_MME_CHANGE                  (1 << 6)
#define VB_EMM_HO_WITHOUT_MME_CHANGE               (1 << 7)
#define VB_EMM_DETACH_TYPE_SWITCHOFF               (1 << 8)
#define VB_EMM_INIT_UE_MSG                         (1 << 9)
#define VB_EMM_COMBINED_ATTACH                     (1 << 10)
#define VB_EMM_TAU_FOR_HO_REQD                     (1 << 11)

#define VB_EMM_HO_INTRA_LTE               0
#define VB_EMM_HO_LTE_TO_UTRAN            1
#define VB_EMM_HO_LTE_TO_GERAN            2
#define VB_EMM_HO_UTRAN_TO_LTE            3
#define VB_EMM_HO_GERAN_TO_LTE            4
#define VB_EMM_HO_RESERVED                5




#define VB_MME_SPND_NULL_EVNT     0
#define VB_MME_SPND_ESM_EVNT      1
#define VB_MME_SPND_EMM_EVNT      2
#define VB_MME_SPND_S11_EVNT      3
#define VB_MME_SPND_S1AP_EVNT     4

#define VB_MME_MAX_SPND_PROC      12
#define VB_MME_MAX_PDN_CONN       8   

#define VB_MME_CHK_MASK(val, mask)                 ((val) & (mask))

#define VB_MME_SET_MASK(val, mask)                                   \
{                                                                    \
   val = ((val) | (mask));                                           \
}

#define VB_MME_UNSET_MASK(val, mask)                                 \
{                                                                    \
   val = ((val) & ~(mask));                                          \
}

#define VB_MME_SET_MSB(_val)                                         \
{                                                                    \
   _val |= (1 << 23);                                                \
}

/* Allocation/Dellocation Macros for static buffers in MME */
#define VB_MME_ALLOC(_buf, _size)                                    \
{                                                                    \
   if (SGetSBuf(vbMmeCb.init.region, vbMmeCb.init.pool,              \
                (Data **) _buf, (Size) _size) == ROK)                \
   {                                                                 \
      VB_DBG_MEM(vbMmeCb.init, VB_MME_LYR_NAME, _buf, VB_MEMTYPE_BUF, VB_MEMMODE_ALLOC, _size);     \
      cmMemset((U8 *)(*_buf), 0, _size);                             \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (*_buf) = NULLP;                                               \
   }                                                                 \
}

#define VB_MME_FREE(_buf, _size)                                     \
{                                                                    \
   VB_DBG_MEM(vbMmeCb.init, VB_MME_LYR_NAME, _buf, VB_MEMTYPE_BUF, VB_MEMMODE_FREE, _size);         \
   if(NULLP != _buf)                                                 \
   {                                                                 \
      (Void) SPutSBuf(vbMmeCb.init.region, vbMmeCb.init.pool,        \
                     (Data *) _buf, (Size) _size);                   \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define VB_MME_FREE_S1AP_PDU(_pdu) \
do                                \
{\
   if (_pdu != NULLP) \
   {\
      cmFreeMem ((Ptr)(_pdu));\
      _pdu = NULLP;\
   }\
}while(0)

#define VB_FREE_BUF(_buf) \
do                         \
{ \
   if (_buf != NULLP) \
   {\
      SPutMsg (_buf); \
      _buf = NULLP; \
   }\
}while(0)

/* vb005.101: Added support for Network initiated Detach */
#ifdef VB_PERF_MEAS
#define VB_MME_INIT_PERF_MEAS(_ueCb) \
   gettimeofday(&_ueCb->eNodebCb->uePrevTime, NULL);
#endif

/* suConnId for every UE required for S1AP is made of
   peerId [eNodeB Id] | ueCbId [ueId/MME UE S1AP Id] */

/* Retrieve eNodeB Id from suConnId of S1AP */
#define VB_MME_GET_PEERID(ueConnId,peerId)   \
   peerId = GetHiWord(ueConnId);


/* Retrieve ueId from suConnId of S1AP */
#define VB_MME_GET_UEID(ueConnId, ueId)      \
   (ueId) = (ueConnId);

/* Retrieve both peerId and ueId from suConnId of S1AP */
#define VB_MME_GET_PEER_UEID(ueConnId, peerId, ueId)   \
   peerId = GetHiWord(ueConnId);                       \
   ueId = GetLoWord(ueConnId);                         \

/* Generates a new KSI different from UE's current ksi
 *  * 7 is KSI bit set for no-key available
 *   */
#define VB_MME_GET_NEW_KSI(_currUeKsi, _newKsi) \
do {\
         _newKsi = (_currUeKsi + 1) % 7;\
} while (0)




/* Timers */
/* emm timers attempt at network side */
#define VB_TMR_EMM_MAX_ATCH_ACC_ATMPS 5
#define VB_TMR_EMM_MAX_DTCH_REQ_ATMPS 5
#define VB_TMR_EMM_MAX_AUTH_REQ_ATMPS 2
#define VB_TMR_EMM_MAX_ID_REQ_ATMPS 5

#define VB_TMR_EMM_IRAT_CTXT_REL_CFG  10  /* 1 second*/
#define VBSM_EG_DFLT_PORT  2123  /* GTP-C Default port */

/* RANAP Causes recieved on Fwd Reloc Request */
#define VB_RANAP_CAUSE_RELOC_DESR_RADIO_RSN 43

/* esm timers attempt at network side */
#define VB_TMR_ESM_MAX_ACT_DEF_BEAR_CTXT_REQ 5
#define VB_TMR_ESM_MAX_ACT_DED_BEAR_CTXT_REQ 5
#define VB_TMR_ESM_MAX_DEACT_BEAR_CTXT_REQ   5
#define VB_TMR_ESM_MAX_MOD_BEAR_CTXT_REQ     5
#define VB_TMR_ESM_MAX_ESM_INFO_REQ          5
#define VB_TMR_ESM_MAX_DEACTBER_REQ_ATMPS 5
/* S1AP related macros/definitions */
/* szt timers */

/* vb006.101: Test Code for OverLoading Feature*/
#ifdef VB_TEST_CODE
#define VB_TMR_SND_RESET  14
#endif /* VB_TEST_CODE */

/* Timer for UE Context release */
#define VB_TMR_UE_REL_CMD 15

/* Max s1ap procedures */
#define VB_S1AP_MAX_PROC_ID 39

#define VB_S1AP_INIT_MSG 0
#define VB_S1AP_SUCCESFUL_OUTCOME 1
#define VB_S1AP_UNSUCCESFUL_OUTCOME 2

#define VB_S1AP_ALLOC_RET_PRIO 1


/*--------------- Debug prints used under ERRCLS -------------------*/
#ifndef NO_ERRCLS

#define VB_ERRCLS_CHK(errCls) \
   if((errCls & ERRCLASS))

/* For MME */
/* vb006.101: Fixed g++ compilation warning*/
#define VB_MME_LOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   VB_ERRCLS_CHK(errCls) \
   { \
    SLogError(vbMmeCb.init.ent, vbMmeCb.init.inst, vbMmeCb.init.procId,     \
          (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
          (Txt *)errDesc); \
   } \
}while(0)


/* For Utilities / General */
/* vb006.101: Fixed g++ compilation warning*/
#define VBLOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   VB_ERRCLS_CHK(errCls) \
   { \
   SLogError(0, 0, 0,     \
         (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
         (Txt *)errDesc); \
   } \
}while(0)

#else  /* ERRCLS */

#define VB_MME_LOGERROR(errCls, errCode, errVal, errDesc) 
#define VBLOGERROR(errCls, errCode, errVal, errDesc) 
#endif  /* ERRCLS */

/**********************************************************************/
#ifdef DEBUGP

#define VB_MME_PRNTBUF   vbMmeCb.init.prntBuf

/* Layer Name appearing in the Debug prints */
#define VB_MME_LYR_NAME "LTE-CNE-MME"
#define VB_LYR_NAME     "LTE-CNE-UTIL"

/* macro to print buffer 
*/
#define VBPRNT(_init, _prntBuf)   \
{                          \
   sprintf _prntBuf;      \
   SPrint(_init.prntBuf);            \
}

/* macro to get and print timestamp value */
#ifdef VB_PRNT_TIMESTAMP
#define VB_PRNTTIMESTAMP(_init)                                                \
{                                                                       \
   S8 _buf[60];                                                    \
   (Void) SGetTimeStamp(_buf);                                          \
   VBPRNT(_init, (_init.prntBuf, "[%s]:", _buf));                                        \
}
#else
#define VB_PRNTTIMESTAMP(_init)
#endif

/* macro to print layer Name */
#define VB_PRNT_LAYER(_init, _lyr)                                             \
{                                                                       \
   VBPRNT(_init, (_init.prntBuf, " [%s 0x%x:%x]", _lyr,                           \
         _init.ent, _init.inst));                                \
}

/* macro to print filename, line number and function name */
#define VB_PRNT_FILE_LINE(_init)                                        \
{                                                                       \
   VBPRNT(_init, (_init.prntBuf, " [%s:%d:%s]",__FILE__,__LINE__, __tapa_fun)); \
}

/* macro to collect print all the information */
#define VB_DBG(_init, _lyr, _mask, _arg, _lvl) \
do \
{ \
   if(_init.dbgMask & (_mask)) \
   { \
      VB_PRNTTIMESTAMP(_init)                                     \
      VB_PRNT_LAYER(_init, _lyr)                                    \
      VB_PRNT_FILE_LINE(_init)                                      \
      VBPRNT(_init, (_init.prntBuf, " [" #_lvl "]" ));              \
      VBPRNT(_init, (_init.prntBuf, " : "));                        \
      VBPRNT(_init, _arg);                         \
      VBPRNT(_init, (_init.prntBuf, "\n"));    \
   } \
} while (0)

/* Debug macros for different debug levels */
#define VB_DBG_INFO(_init, _layerName, _mask, _arg)\
   VB_DBG(_init, _layerName, _mask, _arg, "INFO")

#define VB_DBG_ERROR(_init, _layerName, _mask, _arg)\
   VB_DBG(_init, _layerName, _mask, _arg, "ERROR")

/*--------------- Debug prints used under DEBUGP -------------------*/

/* Debug prints for MME */
#define VB_MME_DBG_INFO(_arg) \
   VB_DBG_INFO(vbMmeCb.init, VB_MME_LYR_NAME, LVB_DBGMASK_INFO, _arg)

#define VB_MME_DBG_ERROR(_arg) \
   VB_DBG_ERROR(vbMmeCb.init, VB_MME_LYR_NAME, LVB_DBGMASK_ERROR, _arg)

/* Debug prints for General / Utilities */
/* Note: For now the common places, VbMmeCb is used presently.
 * Take care in the process of segregation of code.*/

#define VB_UTL_DBG_INFO(_arg) \
   VB_DBG_INFO(vbMmeCb.init, VB_LYR_NAME, LVB_DBGMASK_INFO, _arg)

#define VB_UTL_DBG_ERROR(_arg) \
   VB_DBG_ERROR(vbMmeCb.init, VB_LYR_NAME, LVB_DBGMASK_ERROR, _arg)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define VB_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)             \
{                                                                       \
   if (_init.dbgMask & (LVB_DBGMASK_MEM))                            \
   {                                                                    \
      VB_PRNTTIMESTAMP(_init)                                                  \
      VB_PRNT_LAYER(_init, _lyr)                                                   \
      VB_PRNT_FILE_LINE(_init)                                          \
      VBPRNT(_init, (_init.prntBuf, " [ MEM  : %d %d %p %d]\n",                 \
              _memtype, _memmode,(Void*)_ptr, (U32)_size ));             \
   }                                                                    \
}
#else
#define VB_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)                     \
{                                                                       \
   if (_init.dbgMask & (LVB_DBGMASK_MEM))                            \
   {                                                                    \
      VB_PRNTTIMESTAMP(_init)                                           \
      VB_PRNT_LAYER(_init, _lyr)                                               \
      VB_PRNT_FILE_LINE(_init)                                          \
      VBPRNT(_init, (_init.prntBuf, " [ MEM  : %d %d %p %ld]\n",     \
              _memtype, _memmode,(Void*)_ptr, (U32)_size ));             \
   }                                                                    \
}
#endif /*ALIGN_64BIT */

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, VbMmeCb is used presently.
 * Take care in the process of segregation of code.*/
#define VB_TRC2(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (vbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                        \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init, VB_LYR_NAME)                                 \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                  \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf," Entering\n"));                     \
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define VB_TRC3(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (vbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                           \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init, VB_LYR_NAME)                                 \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                                 \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf," Entering\n"));                     \
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define VB_RETVOID                                                      \
do                                                                      \
{                                                                       \
   if (vbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                           \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init,  VB_LYR_NAME)                                 \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                                 \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define VB_RETVALUE(_arg)                                               \
{                                                                       \
   if (VbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                        \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init,  VB_LYR_NAME)                                 \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                                 \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}

/* Trace for MME Code */

#define VB_MME_TRC2(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (vbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                           \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init,  VB_MME_LYR_NAME)                                 \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                                 \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf," Entering\n"));                     \
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define VB_MME_TRC3(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (vbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                           \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init,  VB_MME_LYR_NAME)                             \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                   \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf," Entering\n"));                     \
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define VB_MME_RETVOID                                                      \
do                                                                      \
{                                                                       \
   if (vbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                           \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init,  VB_MME_LYR_NAME)                             \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                                 \
      VBPRNT(vbMmeCb.init, (vbMmeCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define VB_MME_RETVALUE(_arg)                                               \
{                                                                       \
   if (VbMmeCb.init.dbgMask & (LVB_DBGMASK_TRC))                        \
   {                                                                    \
      VB_PRNTTIMESTAMP(vbMmeCb.init)                                    \
      VB_PRNT_LAYER(vbMmeCb.init,  VB_MME_LYR_NAME)                             \
      VB_PRNT_FILE_LINE(vbMmeCb.init)                                                 \
      VBPRNT(vbMmeCb.init, (VbMmeCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}


#else /* DEBUGP */

/* Layer Name appearing in the Debug prints */
#define VB_MME_LYR_NAME 
#define VB_LYR_NAME  

/* macro to print buffer */
#define VBPRNT(_prntbuf) 

/* macro to get and print timestamp value */
#define VB_PRNTTIMESTAMP(_init)

/* macro to print layer Name */
#define VB_PRNT_LAYER(_init, _lyr)

/* macro to print filename, line number and function name */
#define VB_PRNT_FILE_LINE(_init) 

/* macro to collect print all the information */
#define VB_DBG(_init, _lyr, _mask, _arg, _lvl)

/* Debug macros for different debug levels */
#define VB_DBG_INFO(_init, _layerName, _mask, _arg)
#define VB_DBG_ERROR(_init, _layerName, _lyr, _arg)

/*--------------- Debug prints used under DEBUGP -------------------*/

/* Debug prints for MME */
#define VB_MME_DBG_INFO(_arg)
#define VB_MME_DBG_ERROR(_arg)
#define VB_MME_HEX_DUMP(_arg)

/* Debug prints for General / Utilities */
#define VB_UTL_DBG_INFO(_arg)
#define VB_UTL_DBG_ERROR(_arg)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define VB_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#else
#define VB_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#endif

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, VbMmeCb is used presently.
 * Take care in the process of segregation of code.*/
#define VB_TRC2(_arg)
#define VB_TRC3(_arg)

#define VB_RETVOID
#define VB_RETVALUE(_arg)

/* Trace for MME Code */

#define VB_MME_TRC2(_arg)
#define VB_MME_TRC3(_arg)

#define VB_MME_RETVOID
#define VB_MME_RETVALUE(_arg)

#endif /* DEBUGP */

/* TODO Remove these */
#define VB_MME_DBGP(_arg)
#define VBLOGERROR_DEBUG
#define VB_DBG_FATAL(_arg)
#define VBDBGP(_arg, _arg1)
#define VB_MME_DBG_PRNT(_arg)

#define VB_MME_EGT_T3_RESP_TMR 150

/* Message to common procedure FSM index map macro */
#define VB_EMM_GET_COMMON_PROC_EVNT(_msgType, _evnt) \
   do \
   { \
      if (_msgType < CM_EMM_MSG_GUTI_RLC_CMD || \
          _msgType > CM_EMM_MSG_SEC_MODE_REJ) \
      { \
         if (_msgType == CM_EMM_MSG_ATTACH_CMP) \
         { \
            _evnt = VB_EMM_EVNT_CMN_PRC_ATTACH_CMP; \
            break; \
         } \
         if (_msgType == CM_EMM_MSG_ATTACH_ACC) \
         { \
            _evnt = VB_EMM_EVNT_CMN_PRC_ATTACH_ACC; \
            break; \
         } \
         if (_msgType == CM_EMM_MSG_TAU_CMP) \
         { \
            _evnt = VB_EMM_EVNT_CMN_PRC_TAU_CMP; \
            break; \
         } \
         _evnt = VB_EMM_EVNT_CMN_PRC_INVALID; \
         break; \
      } \
      _evnt = VbEmmCmnProcMsgToEvntMap[_msgType & 0x0f]; \
   } while(0)


/* Message to FSM index map macro */
#define VB_EMM_GET_EVNT(_msgType, _dir, _evnt) \
   do \
   { \
      if ((_msgType & 0x3f) > CM_EMM_MAX_MSG_VALUE) \
      { \
         _evnt = VB_EMM_EVNT_INVALID; \
         break; \
      } \
      _evnt = VbEmmMsgToEvntMap[_msgType & 0x3f]; \
      if (_msgType == CM_EMM_MSG_DETACH_REQ && \
          _dir == CM_EMM_DIR_NW_TO_UE) \
      { \
         _evnt = VB_EMM_EVNT_NW_DETACH_REQ; \
      } \
      else if (_msgType == CM_EMM_MSG_DETACH_ACC && \
          _dir == CM_EMM_DIR_UE_TO_NW) \
      { \
         _evnt = VB_EMM_EVNT_UE_DETACH_ACC; \
      } \
      /* vb001.101: Added service req message */ \
      if(_msgType == CM_EMM_MSG_SERVICE_REQ) \
      { \
        _evnt =  VB_EMM_EVNT_SRVCE_PROC; \
      } \
      if(_msgType == CM_EMM_MSG_SERVICE_REJ) \
      { \
        _evnt =  VB_EMM_EVNT_SRVCE_PROC; \
      } \
   } while(0)


/* vb001.101: Get STMSI value */
#if 0 /* Harsh: STMSI filling problem becuase of Cavium-Linux endianness */
#define VB_MME_GET_STMSI_VAL(_key, _val)\
{\
  S8  cnt1;\
  _key=0;\
  if((_val != NULLP) && \
     (_val->pres.pres== PRSNT_NODEF))\
  {\
     /* COPY the osxl strying */ \
     for ( cnt1 = _val->m_TMSI.len-1; cnt1 >= 0; cnt1--) \
     {\
      _key = _key << 8;\
      _key |= (U8) _val->m_TMSI.val[(U8)cnt1];\
     }\
  }\
}
#else
#define VB_MME_GET_STMSI_VAL(_key, _val)\
{\
  U8  cnt1;\
  if((_val != NULLP) && \
     (_val->pres.pres== PRSNT_NODEF))\
  {\
     /* COPY the osxl strying */ \
     _key[0] = _val->mMEC.val[0]; \
     for ( cnt1 = 0; (cnt1 < _val->m_TMSI.len) & (cnt1 < 4); cnt1++) \
     {\
      _key[cnt1+1] = (U8) _val->m_TMSI.val[cnt1];\
     }\
  }\
}

#define VB_MME_DEC_U32(tar, src)\
{\
   (tar) |= ((U8)((src)[0] << 24)); \
   (tar) |= ((U8)((src)[1] << 16)); \
   (tar) |= ((U8)((src)[2] << 8)); \
   (tar) |= ((U8)((src)[3])); \
}

#define VB_MME_GET_U16(tar, src)\
{\
   (tar) |= ((U8)((src)[0] << 8)); \
   (tar) |= ((U8)((src)[1])); \
}

#define VB_MME_DEC_S1AP_IPADDR(_tgtAddr, _srcAddr) \
{\
   _tgtAddr.type = CM_INET_IPV4ADDR_TYPE;\
   _tgtAddr.u.ipv4NetAddr = 0;\
   _tgtAddr.u.ipv4NetAddr |= ((_srcAddr.val[0] << 24) & 0xff000000);\
   _tgtAddr.u.ipv4NetAddr |= ((_srcAddr.val[1] << 16) & 0x00ff0000);\
   _tgtAddr.u.ipv4NetAddr |= ((_srcAddr.val[2] << 8) & 0x0000ff00);\
   _tgtAddr.u.ipv4NetAddr |= ((_srcAddr.val[3]) & 0x000000ff);\
}

#define VB_MME_DEC_S1AP_TEID(teid, szTeid) \
{\
   U16   _teidIdx = 0;\
   U8    _shiftBits = szTeid.len - 1;\
   teid = 0;\
   for (_teidIdx = 0; _teidIdx < _shiftBits; _teidIdx++) \
   {\
      teid |= (U32)(szTeid.val[_teidIdx]);\
      teid <<= 8;\
   }\
   teid |= ((U32)(szTeid.val[_teidIdx]));\
}

#define VB_MME_END_EMM_PROC(arg)  \
{                                  \
    (arg)->proc = VB_MME_NULL_PROC; \
    vbMmeUtlResumeProc(arg);       \
}                                  \

#define VB_MME_END_ESM_PROC(esmCb)         \
{                                          \
    esmCb->ueCb->proc = VB_MME_NULL_PROC;  \
    esmCb->proc = VB_MME_NULL_PROC;        \
    vbMmeUtlResumeProc(esmCb->ueCb);       \
}                                          \

#define VB_MME_GET_BEARER_INDEX(ebi)   ((ebi) - 5);   /* Bearer Index */

#define VB_MME_SET_IE_LVL_INFO(arg, lvl, type, inst)                    \
{                                                                       \
   arg.level                  = lvl;                                    \
   arg.ieInfo[lvl].ieTypePres = TRUE;                                   \
   arg.ieInfo[lvl].ieType     = type;                                   \
   arg.ieInfo[lvl].ieInst     = inst;                                   \
}

/* Bitmask for checking HO Received or Not */
#define VB_MME_HO_FAILURE_RCVD               (1 << 0)
#define VB_MME_RELOC_CANCEL_RCVD             (1 << 1)

/* Checking Bit mask */
#define VB_MME_CHK_MASK(val, mask)        ((val) & (mask))

/* Setting bitmask */
#define VB_MME_SET_MASK(val, mask)                                   \
{                                                                    \
   val = ((val) | (mask));                                           \
}

/* Unsetting bitmask */
#define VB_MME_UNSET_MASK(val, mask)                                 \
{                                                                    \
   val = ((val) & ~(mask));                                          \
}

#endif /* __VBH__ */
#endif /* __VBH__ */


/**********************************************************************

         End of file:     vb.h@@/main/1 - Mon Jun 28 19:35:34 2010
-
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -       ad               1. LTE-CNE Initial Release.
/main/1  vb001.101   rk               1. Added TID & TFT IE types.
                                      2. Added service req message in VB_EMM_GET_EVNT
                                      3. Added macro VB_MME_GET_STMSI_VAL
/main/1  vb005.101   rk               1. Added support for Network initiated Detach 
                    pmacharla        1. eGTP 2.1 Release 9 upgrade under EG_REL_930 flag
/main/1  vb005.101  psb              1. TIC_ID:ccpu00117534 Fixed warning for gcc compilation on CentOS
/main/1  vb006.101 ragrawal          1. CRID:ccpu00117391 ERAB RELEASE support.
                                     2. ccpu00118283:Added macro for maximum IMSI GUTI map cb in hashlist. 
                                     3. Test Code for OverLoading Feature.
                                     4. Fixed g++ compilation warning.
                                     5. TIC_ID:ccpu00117534 Fixed warning for gcc compilation 
                                        on CentOS.
/main/1  vb007.101 akaranth          1. Added support to send DNS IP.
*********************************************************************91*/

