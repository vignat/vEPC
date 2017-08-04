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



/************************************************************************

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for global definitions

     File:     sz.h

     Sid:      sz.h@@/main/nodal_main/1 - Fri Nov 22 15:56:40 2013

     Prg:      sn

************************************************************************/
#ifndef __SZH__
#define __SZH__

/* max values of ENB-UE-S1AP-ID and MME-UE-S1AP-ID as per the spec */
#define SZ_MAX_ENBID   0xffffff
#define SZ_MAX_MMEID   0xffffffff
#define SZ_MAX_GWID    0xffffffff

#define SZ_BND_TMR 0         /* S1ap bind timer */
#define SZ_LCL_SU_ENDPID 1   /* S1ap local end point id of the user */
#define SZ_ENDP_OPEN_IN_PRG 2  /* S1ap end point open is in progress */
#define SZ_ENDP_CFG 1          /* S1ap end point configuration */
#define SZ_MAX_PLMN_SIZE 10    /* S1ap PLMN maximum size */
#define SZ_MAX_MME_GROUP 10    /* S1ap maximum group of MME */
#define SZ_MAX_NUM_OF_PEERS 10 /* S1ap maximum number of peers*/ 
#define SZ_MAX_LOG_CONN 256    /* S1ap maximum log connection */
#define SZ_USTA_MAX_DGNVAL 10  /* S1ap USAT maximum DGNVAL */
#define SZ_MAX_SCT_SAP_TMRS 10 /* S1ap maximum lower sap timers */
#define SZ_TQSIZE 64           /* S1ap TQ size */
#define SZ_SCT_MAX_SEL 4       /* S1ap maximum number of SCT selectors */
#define SZ_ASSOC_IN_PRG 12     /* S1ap Association is in progress */

#define SZ_ENDP_OPEN 3    /* S1ap End point is open */
#define SZ_ASSOC_UP 1     /* S1ap End point is up */
#define SZ_ASSOC_DOWN 0   /* S1ap End point is down */

#define SCT_PROTID_S1AP 18  /* SCT protocol id */

#define SZ_FULL_RESET    0  /* Reset full S1ap*/
#define SZ_PARTIAL_RESET 1  /* Reset partial s1ap*/

#ifdef SS_MULTIPLE_PROCS
#ifndef SZ_MAX_INSTANCES
#define SZ_MAX_INSTANCES 5  /* S1ap maximum instance */
#endif /* SZ_MAX_INSTANCES */
#define szCb    (*szCbPtr)  /* S1ap control block */
#endif /* SS_MULTIPLE_PROCS */

/* Type of info type in error indication request */
#define  SZ_INFO_SUCONNID      1 /* suConnId */
#define  SZ_INFO_SPCONNID      2 /* spConnId */
#define  SZ_INFO_TRANSID       3 /* transId */
#define  SZ_INFO_NONE          4 /* no info */

/* State machine states */
#define SZ_SMSZ_IDLE         0 /* State machine is idle state */
#define SZ_SMSZ_CONNECTING   1 /* State machine is in connecting state */
#define SZ_SMSZ_ESTABLISHED  2 /* State machine is in establishing state */
#define SZ_SMSZ_RELEASING    3 /* State machine is in releasing */

#define SZ_UI 1  /* Upper interface */
#define SZ_LI 2  /* Lower interface */

/* Operations on SZ */
#define SZ_DEL_SAP   1  /* Delete the sap */  
#define SZ_UBND_SAP  2  /* Unbind the sap */
#define SZ_SHUT_DOWN 3  /* Shut down the sap */

#define SZ_TMR_WAIT_BNDCFM  12   /* Timer wait for bind confirm */
#define SZ_HASH_LIST_SIZE   1024 /* Hash list size */
#define SZ_HASH_KEYTYPE     CM_HASH_KEYTYPE_U32MOD /* Hash key type*/
#define SZ_MAX_SCT_SAP_TMRS 10   /* Maximum number of SCT sap timer */

/* Max no. of msgs will vary depending on the s1ap specification */
#ifndef S1AP_REL8A0
#define SZ_MAX_MSG_ID 73   /* For S1AP release 9.4.0 */
#define SZ_MSG_INDEX  73   /* S1ap message index */
#else /* S1AP_REL8A0 */
#define SZ_MAX_MSG_ID 67   /* For S1AP release 8.10.0 */
#define SZ_MSG_INDEX  67   /* S1ap message index */
#endif  /* S1AP_REL8A0 */

/* Max no. of IEs present in S1AP */
#ifndef S1AP_REL8A0
#define SZ_MAX_IE_ID 151   /* S1ap maximun IE id */
#else /* S1AP_REL8A0 */
#define SZ_MAX_IE_ID 140   /* S1ap maximun IE id */
#endif /* S1AP_REL8A0 */

#define SZ_MAX_ST 4          /* Maximum number of ST */
#define SZ_MAX_SCTSAP_TMRS 2 /* Maximum number of SCT sap timers */
 
#define SZ_UI_CON_REQ 1  /* Upper Interface connection request */
#define SZ_UI_CON_RSP 2  /* Upper Interface connection response */
#define SZ_UI_REL_REQ 3  /* Upper Interface release request */
#define SZ_UI_REL_RSP 4  /* Upper Interface release response */
#define SZ_UI_DAT_REQ 5  /* Upper Interface data response */

#define SZ_MEM_SDU_SIZE     2048   /* S1ap SDU memory size */
 
#define SZ_PASN_VER_ZERO 0         /* S1ap pasn version is zero */

#define SZ_MSG_NOT_HANDLED 0xffffffff /* S1ap messages not handled */

/* Protocol specific cause */
#define SZ_MSG_NOT_COMP_WITH_REC_ST 3 /* S1ap message not compile with REC ST*/

/* Connection reference types */
#define SZ_CONN_REF_LCL 1   /* Local connection referance */
#define SZ_CONN_REF_RMT 2   /* Remote connection referance */

/* Node level debug size */
#define SZ_DBGSIZE      10  /* Debug size */

#ifndef SZ_ENC_DEC     
#ifdef DEBUGP
#define SZDBGP(_msgClass, _arg) \
            DBGP(&szCb.init,  szCb.nodeDbg, _msgClass, _arg)
#else /* DEBUGP */
#define SZDBGP(_msgClass, _arg)
#endif /* DEBUGP */
#else /* SZ_ENC_DEC */
#ifdef DEBUGP
#define SZDBGP(_msgClass, _arg) \
            DBGP(&szCb.init, "S1AP ENC/DEC: ", _msgClass, _arg)
#else  /* DEBUGP */
#define SZDBGP(_msgClass, _arg)
#endif /*DEBUGP */
#endif /* SZ_ENC_DEC */

/* Radio network errors handled by S1AP */
#ifdef LSZV1
#define SZ_UNKNOWN_MME_UE_S1AP_ID    13  /* Unknown MME UE id */
#define SZ_UNKNOWN_ENB_UE_S1AP_ID    14  /* Unknown ENB UE id */
#define SZ_UNKNOWN_PAIR_UE_S1AP_ID   15  /* Unknown PAIR UE id */

/* Protocol errors handled by S1AP */
#define SZ_TRANSFER_SYNTAX_ERROR          0 /* Transfer syntax error */
#define SZ_ABS_SYNTAX_ERR_REJ             1 /* Reject the ABS syntax error */
#define SZ_ABS_SYNTAX_ERR_IGN_AND_NTFY    2 /* Ignore & notify the syntax err*/
#define SZ_MSG_NOT_COMP_WITH_RCVR_STATE   3 /* With rec st msg is not comp */
#define SZ_SEMANTIC_ERR                   4 /* Semantic error */
#define SZ_ABS_SYNTAX_ERR_FALSLY_CONS_MSG 5 /* Constant message syntax error */

/* sz008.301: Maximum no. of protocol errors handled by S1AP */
#define SZ_MAX_PROTOCOL_ERRS_HANDLD  7
#endif /* LSZV1 */

/* Used for Initializing szErrPrntArr */
#define SZ_MAX_ENC_DEC_ERR_CAUSE     10  /* Max encode decode error cause */
#define SZ_ERR_DBG_SIZE              100 /* Error debug size*/

/* Specifies the connection type */
#define SZ_INCMG_CONN 0  /* Incoming connection */
#define SZ_OUTGNG_CONN 1 /* Outgoing connection */

/* Get the peer from peer id */
#define SZ_GET_PEER_FRM_PEERID(_id,peer)              \
   if( _id >= szCb.genCfg.nmbPeer)                    \
   {                                                  \
      peer = NULLP;                                   \
   }                                                  \
   else                                               \
   {                                                  \
     peer = (szCb.peerLst[(_id )]->peer);             \
   }

/* S1ap memory allocation */
#define SZ_ALLOC(_datPtr, _size)                        \
{                                                       \
    SGetSBuf(szCb.init.region,                          \
                  szCb.init.pool,                       \
                  (Data**)&_datPtr, _size);             \
   if (_datPtr != NULLP)                                \
   {                                                    \
      cmMemset((U8*)_datPtr, 0, _size);                 \
      /* add to general status */                       \
      szCb.genSta.memAlloc += SBUFSIZE(_size);          \
   }                                                    \
   else                                                 \
   {                                                    \
      SzUstaDgn _dgn; \
      _datPtr = NULLP;                                  \
      /* Inform the Layer Manager of resource crunch */ \
      _dgn.type = LSZ_USTA_DGNVAL_MEM;                  \
      _dgn.u.mem.region = szCb.init.region;             \
      _dgn.u.mem.pool = szCb.init.pool;                 \
      szSendLmAlarm (LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,     \
                     LCM_CAUSE_MEM_ALLOC_FAIL, &_dgn);                     \
      szCb.genSts.nmbFailConn++;                       \
   }                                                   \
} /* SZ_ALLOC */

/* S1AP memory free */
#define SZ_FREE(_datPtr, _size)                       \
{                                                     \
   if (_datPtr != NULLP)                              \
   {                                                  \
      (Void)SPutSBuf(szCb.init.region,                \
                     szCb.init.pool,                  \
                     (Data*)_datPtr, _size);          \
      /* subtract from general status */              \
      szCb.genSta.memAlloc -= SBUFSIZE(_size);        \
      _datPtr = NULLP;                                \
   }                                                  \
}

/* Macro to free the static buffer allocated from different
 * region and pool */
#define SZ_FREE_STA(_datPtr, _size, _region, _pool)   \
{                                                     \
   if(_datPtr != NULLP)                               \
   {                                                  \
      (Void)SPutSBuf(_region, _pool, (Data*)_datPtr,  \
            _size);                                   \
   }                                                  \
}

/* S1AP free the SDU */
#define SZ_FREE_SDU(_sdu) \
            {\
               if (_sdu != NULLP) \
               {\
                  cmFreeMem ((Ptr)(_sdu));\
                  _sdu = NULLP;\
               }\
            }

/* S1AP free the buffer */
#define SZ_FREE_BUF(_buf) \
            { \
               if (_buf != NULLP) \
               {\
                  SPutMsg (_buf); \
                  _buf = NULLP; \
               }\
            }

/* S1AP send local error indication */
#define SZ_SEND_LCL_ERR_IND(_iType, _iVal, _sap, _cType, _cVal)      \
            {                                                        \
               SztLclErrInd _errInd;                                 \
               cmMemset ((U8 *)&_errInd, 0, sizeof (SztLclErrInd));  \
                                                                     \
               _errInd.causeType  = (U8) _cType;                     \
               _errInd.causeValue = (U8)_cVal;                       \
                                                                     \
               _errInd.transId.pres = _iType;                        \
               _errInd.transId.val = _iVal;                          \
               SzUiSztErrInd (&_sap->pst, _sap->suId, &_errInd);     \
            }                                                        

/* Initialize a timer */
#define SZ_INITTIMER(_tmr)       cmInitTimers((_tmr), 1)
            
/* S1AP send Layer manager alarm */            
#define SZ_SND_LM_ALARM(_errCode,_fn,_region,_pool)                  \
            {                                                        \
                  SzUstaDgn _dgn; \
                  SZLOGERROR(ERRCLS_INT_PAR, _errCode, (ErrVal)0,  \
                        _fn": Unable to allocate resources"); \
                  _dgn.type = LSZ_USTA_DGNVAL_MEM; \
                  _dgn.u.mem.region = _region;     \
                  _dgn.u.mem.pool = _pool;         \
                  szSendLmAlarm (LCM_CATEGORY_RESOURCE, \
                        LCM_EVENT_DMEM_ALLOC_FAIL, LCM_CAUSE_MEM_ALLOC_FAIL, \
                        &_dgn); \
                  RETVALUE (RFAILED); \
            }


/* Send an alarm for sapId events */
#define SZ_SEND_SAPID_ALARM(_sapId, _evnt, _cause) \
{ \
   SzUstaDgn dgn; \
   cmMemset((U8*)&dgn, 0, sizeof(SzUstaDgn)); \
   dgn.type = LSZ_USTA_DGNVAL_SAPID; \
   dgn.u.sapId = _sapId; \
   szSendLmAlarm(LCM_CATEGORY_INTERFACE, _evnt, _cause, &dgn); \
}

/* Send an alarm for peer events */
#define SZ_SEND_PEER_ALARM(_peerId, _evnt, _cause) \
{ \
   SzUstaDgn dgn; \
   cmMemset((U8*)&dgn, 0, sizeof(SzUstaDgn)); \
   dgn.type = LSZ_USTA_DGNVAL_PEER; \
   dgn.u.peerId.pres = TRUE; \
   dgn.u.peerId.val = _peerId; \
   szSendLmAlarm(LCM_CATEGORY_PROTOCOL, _evnt, _cause, &dgn); \
}

/* Macro for statistics */
#define SZ_STS_INC_DATA_TX_SCT(_peer, _evntPdu, _conCb)    \
{                                                          \
   U8        _msgIdx;                                      \
   SzPduSts *_pdu;                                         \
   Cntr     *_ptr;                                         \
                                                           \
   szUtilsGetMsgIdx(&_msgIdx, _evntPdu);                   \
                                                           \
   if(_msgIdx < SZ_MAX_MSG_ID)                             \
   {                                                       \
      _pdu = &_peer->peerSts.cfm.pdu;                      \
      _ptr = (Cntr *)(((SzCntr *)_pdu) + _msgIdx);         \
      *_ptr = (*_ptr) + 1;                                 \
                                                           \
      _pdu = &_peer->sctSapCb->sts.cfm.pdu;                \
      _ptr = (Cntr *)(((SzCntr *)_pdu) + _msgIdx);         \
      *_ptr = (*_ptr) + 1;                                 \
   }                                                       \
                                                           \
   if (_conCb != NULLP)                                    \
   {                                                       \
      _peer->peerSts.cfm.szDedProcSts.tx++;                \
      _peer->sctSapCb->sts.cfm.szDedProcSts.tx++;          \
   }                                                       \
   else                                                    \
   {                                                       \
      _peer->peerSts.cfm.szComProcSts.tx++;                \
      _peer->sctSapCb->sts.cfm.szComProcSts.tx++;          \
   }                                                       \
} /* SZ_STS_INC_DATA_TX_SCT */                                                          

#define SZ_STS_INC_DATA_RX_SZT(_peer, _evntPdu, _conCb)    \
{                                                          \
                                                           \
   if (_conCb != NULLP)                                    \
   {                                                       \
      _peer->peerSts.cfm.szDedProcSts.rx++;                \
      _peer->sctSapCb->sts.cfm.szDedProcSts.rx++;          \
   }                                                       \
   else                                                    \
   {                                                       \
      _peer->peerSts.cfm.szComProcSts.rx++;                \
      _peer->sctSapCb->sts.cfm.szComProcSts.rx++;          \
   }                                                       \
}

#define SZ_STS_INC_DATA_RX_PDU(_peer, _msgIdx)             \
{                                                          \
   SzPduSts *_pdu;                                         \
   Cntr     *_ptr;                                         \
                                                           \
   if(_msgIdx < SZ_MAX_MSG_ID)                             \
   {                                                       \
      _pdu = &_peer->peerSts.cfm.pdu;                      \
      _ptr = (((Cntr *)(((SzCntr *)_pdu) + _msgIdx)) + 1); \
      *_ptr = (*_ptr) + 1;                                 \
                                                           \
      _pdu = &_peer->sctSapCb->sts.cfm.pdu;                \
      _ptr = (((Cntr *)(((SzCntr *)_pdu) + _msgIdx)) + 1); \
      *_ptr = (*_ptr) + 1;                                 \
   }                                                       \
}

/* make memory as zero */
#define SZ_ZERO(_pVal, _count)         \
{                                      \
  cmMemset((U8*)_pVal, 0, _count);     \
}

#ifdef SS_DIAG
/* sz008.301 : Change for Diagnostics */
/* Note: Any changes to these enums should reflect to */
/** @details Enums for special argument
 *
*/
typedef enum
{
   SZ_DIAG_PEERID,
   SZ_DIAG_SUASSOCID,
   SZ_DIAG_SPCONID,
   SZ_DIAG_CONNID,
   SZ_DIAG_NA
} SzDiagSplArg;

#define SZ_DIAG_LVL0(_tknId, _splArgEnum,_splArg, _string, _arg1, _arg2, _arg3, _arg4) \
{                                                                                      \
   if(szCb.init.logMask & SS_DIAG_LVL0)                                                \
   {                                                                                   \
      ssDiagFix(_tknId, _splArgEnum, ENTSZ, szCb.init.inst, SS_DIAG_LVL0, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                   \
}

/** @details Macro definition for S1AP error logs
 *  
*/
#define SZ_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)             \
{                                                                                      \
   if(szCb.init.logMask & SS_DIAG_LVL1)                                                \
   {                                                                                   \
      ssDiagFix(_tknId, _splArgEnum, ENTSZ, szCb.init.inst, SS_DIAG_LVL1, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                   \
}

/** @details Macro definition for S1AP critical logs
 *  
*/
#define SZ_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
   if(szCb.init.logMask & SS_DIAG_LVL2)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSZ, szCb.init.inst, SS_DIAG_LVL2, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for S1AP logs 
 *  
*/
#define SZ_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(szCb.init.logMask & SS_DIAG_LVL3)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSZ, szCb.init.inst, SS_DIAG_LVL3, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for S1AP logs
 *  
*/
#define SZ_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(szCb.init.logMask & SS_DIAG_LVL4)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSZ, szCb.init.inst, SS_DIAG_LVL4, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** 
 * @details Macro definition for S1AP Hex-Dump
 */
#define SZ_DIAG_MSG(_mBuf, _str)                                                                  \
{                                                                                                \
   if(szCb.init.logMask & SS_DIAG_MSG)                                                      \
   {                                                                                             \
      Buffer  *_newBuf;                                                                          \
      SAddMsgRef(_mBuf, szCb.init.region, szCb.init.pool, &_newBuf);                             \
      ssDiagFix(0, SZ_DIAG_NA, ENTSZ, szCb.init.inst, SS_DIAG_MSG, SS_DIAG_MSG_TYPE_HEXDUMP, (PTR)_newBuf, 0, 0, 0,0, _str);\
   }                                                                                             \
}

/** 
 * @details Macro definition for S1AP Varible length logs
 */
#define SZ_DIAG_VAR(_tknId, _splArgEnum, _splArg, _string, _stringPtr)                                                         \
{                                                                                                                              \
   if(szCb.init.logMask & SS_DIAG_VAR)                                                                                         \
   {                                                                                                                           \
      ssDiagFix(_tknId, _splArgEnum, ENTSZ, szCb.init.inst, SS_DIAG_VAR, SS_DIAG_MSG_TYPE_FIXED, _splArg, 0, 0, 0, 0, _string);\
      ssDiagVarible(ENTSZ, szCb.init.inst, SS_DIAG_VAR, SS_DIAG_MSG_TYPE_VAR, _tknId, _splArg, _stringPtr);                    \
   }                                                                                                                           \
}

#else

#define SZ_DIAG_LVL0(_tknId, _splArgEnum,_splArg, _string, _arg1, _arg2, _arg3, _arg4) 

/** @details Macro definition for S1AP error logs
 *  
*/
#define SZ_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)             

/** @details Macro definition for S1AP critical logs
 *  
*/
#define SZ_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       

/** @details Macro definition for S1AP logs 
 *  
*/
#define SZ_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        

/** @details Macro definition for S1AP logs
 *  
*/
#define SZ_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        

/** 
 * @details Macro definition for S1AP Hex-Dump
 */
#define SZ_DIAG_MSG(_mBuf, _str)                                                                  

/** 
 * @details Macro definition for S1AP Varible length logs
 */
#define SZ_DIAG_VAR(_tknId, _splArgEnum, _splArg, _string, _stringPtr)                                                         

#endif
#endif /* __SZH__ */

/**********************************************************************
 
         End of file:     sz.h@@/main/nodal_main/1 - Fri Nov 22 15:56:40 2013
 
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
/main/5      ---      va    1. Updated for S1AP release 4.1
*********************************************************************91*/
