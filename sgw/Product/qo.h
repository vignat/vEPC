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

     Name:    Trillium LTE CNE - SGW 

     Type:    C Include file

     Desc:    This file contains the SGW preprocessors code

     File:    qo.h

     Sid:      qo.h@@/main/1 - Thu Jul  8 21:22:18 2010

     Prg:     mm 
*********************************************************************21*/


#ifndef __QO_SGWH__
#define __QO_SGWH__

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#define QO_SGW_GTP_HDR_LEN             12
#define QO_SGW_APP_TQSIZE              64
#define QO_SGW_EGT_T3_RESP_TMR         10
/* qo001.101: Maximum number of tunnels */
#define QO_MAX_TUNNEL                  10
#define QO_SGW_MAX_BEARERS             11
#define QO_SGW_MIN_BEARER_ID           5
#define QO_SGW_MAX_EGU_PKT_BUF_COUNT	100

#define QO_SGW_BEARER_IND(ebi)         ((ebi) - QO_SGW_MIN_BEARER_ID)
#define QO_SGW_SET_BEARER_MAP(_bmap, _ebi)   ((_bmap) |= (1 << (_ebi)))
#define QO_SGW_CHK_BEARER_MAP(_bmap, _ebi)   ((_bmap) & (1 << (_ebi)))
#define QO_SGW_UNSET_BEARER_MAP(_bmap, _ebi) ((_bmap) &= ~(1 << (_ebi)))

#define QO_SGW_GET_BEARER(_bTable, _ebi, _bearer)        \
{                                                        \
   _bearer = NULLP;                                      \
   if (QO_SGW_VAL_EBI(_ebi))                             \
   {                                                     \
      _bearer = _bTable[QO_SGW_BEARER_IND(_ebi)];        \
   }                                                     \
}

/* As per the spec 29.274 Maximum digits in IMSI is 15 with 8 octets
   When Odd number of digits in IMSI, the MSB [Bits 5-8] of last octet will
   have filler digit "0x0F" */
#define QO_SGW_IMSI_LEN     15  
#define QO_SGW_MSISDN_LEN   10  

/* Maximum TPDU size allowed with GTP Data packet */
#define QO_SGW_DFLT_MAXTPDUSZ     3000

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define QO_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#else
#define QO_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#endif

#define QODBGP(_arg, _arg1)

/* Allocation/Dellocation Macros for static buffers in S-GW */
#define QO_SGW_ALLOC(_buf, _size)                                    \
{                                                                    \
   if (SGetSBuf(qoSgwCb.init.region, qoSgwCb.init.pool,              \
                (Data **) _buf, (Size) _size) == ROK)                \
   {                                                                 \
      QO_DBG_MEM(qoSgwCb.init, QO_SGW_LYR_NAME, _buf, QO_MEMTYPE_BUF, QO_MEMMODE_ALLOC, _size);     \
      cmMemset((U8 *)(*_buf), 0, _size);                             \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (*_buf) = NULLP;                                               \
   }                                                                 \
}

#define QO_SGW_FREE(_buf, _size)                                     \
{                                                                    \
   QO_DBG_MEM(qoSgwCb.init, QO_SGW_LYR_NAME, _buf, QO_MEMTYPE_BUF, QO_MEMMODE_FREE, _size);         \
   (Void) SPutSBuf(qoSgwCb.init.region, qoSgwCb.init.pool,           \
                   (Data *) _buf, (Size) _size);                     \
   (_buf) = NULLP;                                                   \
}

/*-- Different memory types --*/
#define QO_MEMTYPE_EVNT  1
#define QO_MEMTYPE_MSG   2
#define QO_MEMTYPE_BUF   3

/*-- Different memory modes --*/
#define QO_MEMMODE_ALLOC 1
#define QO_MEMMODE_FREE  2
#define QO_MEMMODE_SEND  3
#define QO_MEMMODE_RCV   4

#define QO_SGW_INITIAL_ATTACH             (1 << 0)
#define QO_SGW_PDN_CONN_REQUEST           (1 << 1)
#define QO_SGW_TAU_WITH_SGW_CHANGE        (1 << 2)
#define QO_SGW_S1_HO_WITH_SGW_RELOC       (1 << 3)
#define QO_SGW_FWD_MBREQ_TO_PGW           (1 << 4)
#define QO_SGW_ULI_PRESENT                (1 << 5)
#define QO_SGW_ULI_CHANGED                (1 << 6)

/* Bearer operation bitmask */
#define QO_SGW_DEL_BEARER                 (1 << 0)
#define QO_SGW_MOD_BEARER                 (1 << 1)
#define QO_SGW_DIS_BEARER                 (1 << 2)
#define QO_SGW_CRT_BEARER                 (1 << 3)

#define QO_SGW_VAL_EBI(_ebi)              (((_ebi) >= 5) && ((_ebi) <= 15))
#define QO_SGW_CHK_MASK(val, mask)        ((val) & (mask))

#define QO_SGW_SET_MASK(val, mask)                                   \
{                                                                    \
   val = ((val) | (mask));                                           \
}

#define QO_SGW_UNSET_MASK(val, mask)                                 \
{                                                                    \
   val = ((val) & ~(mask));                                          \
}


#define QO_HASH_LIST_SIZE 1024
#ifndef NO_ERRCLS
/* For SGW */
#define QO_SGW_LOGERR(errCls, errDesc) \
do \
{ \
   QO_ERRCLS_CHK(errCls) \
   { \
   SLogError(qoSgwCb.init.ent, qoSgwCb.init.inst, qoSgwCb.init.procId,     \
         (Txt *)__FILE__, __LINE__, errCls, EQO001, (ErrVal)ERRZERO, \
         (Txt *)errDesc); \
   } \
}while(0)

#define QOLOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   QO_ERRCLS_CHK(errCls) \
   { \
   SLogError(0, 0, 0,     \
         (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
         (Txt *)errDesc); \
   } \
}while(0)

#else  /* ERRCLS */
#define QO_SGW_LOGERR(errCls, errDesc) 
#define QOLOGERROR(errCls, errCode, errVal, errDesc)
#endif  /* ERRCLS */

#define QO_DBG_MASK     0xFFFFFFFF
#ifdef DEBUGP

#define QO_SGW_PRNTBUF   qoSgwCb.init.prntBuf
#define QO_SGW_LYR_NAME "LTE-SGW"

#define QOPRNTMSG(_init, _prntBuf)   \
{                          \
   Txt *_str; \
   _str = qoSprintf _prntBuf;      \
   SPrint(_str);            \
}

/* macro to print buffer
*/
#define QOPRNT(_init, _prntBuf)   \
{                          \
   sprintf _prntBuf;      \
   SPrint(_init.prntBuf);            \
}

#define QO_ERRCLS_CHK(errCls) \
   if((errCls & ERRCLASS))


/* macro to get and print timestamp value */
#ifdef QO_PRNT_TIMESTAMP
#define QO_PRNTTIMESTAMP(_init)                                                \
{                                                                       \
   S8 _buf[60];                                                    \
   (Void) SGetTimeStamp(_buf);                                          \
   QOPRNT(_init, (_init.prntBuf, "[%s]:", _buf));                                        \
}
#else
#define QO_PRNTTIMESTAMP(_init)
#endif

/* macro to print layer Name */
#define QO_PRNT_LAYER(_init, _lyr)                                             \
{                                                                       \
   QOPRNT(_init, (_init.prntBuf, " [%s 0x%x:%x]", _lyr,                           \
         _init.ent, _init.inst));                                \
}

/* macro to print filename, line number and function name */
#define QO_PRNT_FILE_LINE(_init)                                        \
{                                                                       \
   QOPRNT(_init, (_init.prntBuf, " [%s:%d:%s]",__FILE__,__LINE__, __tapa_fun)); \
}

#define QO_SGW_DBG(_init, _lyr, _mask, _arg, _lvl) \
do \
{ \
   if(_init.dbgMask & (_mask)) \
   { \
      QO_PRNTTIMESTAMP(_init)                                     \
      QO_PRNT_LAYER(_init, _lyr)                                    \
      QO_PRNT_FILE_LINE(_init)                                      \
      QOPRNT(_init, (_init.prntBuf, " [" #_lvl "]" ));              \
      QOPRNT(_init, (_init.prntBuf, " : "));                        \
      QOPRNTMSG(_init, _arg);                         \
      QOPRNT(_init, (_init.prntBuf, "\n"));    \
   } \
} while (0)

/* Debug prints for SGW */
#define QO_SGW_DBG_INFO(_arg) \
   QO_SGW_DBG(qoSgwCb.init, QO_SGW_LYR_NAME, LQO_DBGMASK_INFO, _arg, "INFO")

#define QO_SGW_DBG_ERR(_arg) \
   QO_SGW_DBG(qoSgwCb.init, QO_SGW_LYR_NAME, LQO_DBGMASK_ERROR, _arg, "ERROR")

/* Trace for SGW Code */
#define QO_SGW_TRC2(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (qoSgwCb.init.dbgMask & (LQO_DBGMASK_TRC))                           \
   {                                                                    \
      QO_PRNTTIMESTAMP(qoSgwCb.init)                                    \
      QO_PRNT_LAYER(qoSgwCb.init,  QO_SGW_LYR_NAME)                             \
      QO_PRNT_FILE_LINE(qoSgwCb.init)                                                 \
      QOPRNT(qoSgwCb.init, (qoSgwCb.init.prntBuf," Entering\n"));                     \
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define QO_SGW_TRC3(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (qoSgwCb.init.dbgMask & (LQO_DBGMASK_TRC))                           \
   {                                                                    \
      QO_PRNTTIMESTAMP(qoSgwCb.init)                                    \
      QO_PRNT_LAYER(qoSgwCb.init, QO_SGW_LYR_NAME)                             \
      QO_PRNT_FILE_LINE(qoSgwCb.init)                                                 \
      QOPRNT(qoSgwCb.init, (qoSgwCb.init.prntBuf," Entering\n"));                     \
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define QO_SGW_RETVOID                                                      \
do                                                                      \
{                                                                       \
   if (qoSgwCb.init.dbgMask & (LQO_DBGMASK_TRC))                           \
   {                                                                    \
      QO_PRNTTIMESTAMP(qoSgwCb.init)                                    \
      QO_PRNT_LAYER(qoSgwCb.init, QO_SGW_LYR_NAME)                              \
      QO_PRNT_FILE_LINE(qoSgwCb.init)                                                 \
      QOPRNT(qoSgwCb.init, (qoSgwCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define QO_SGW_RETVALUE(_arg)                                               \
{                                                                       \
   if (qoSgwCb.init.dbgMask & (LQO_DBGMASK_TRC))                        \
   {                                                                    \
      QO_PRNTTIMESTAMP(qoSgwCb.init)                                    \
      QO_PRNT_LAYER(qoSgwCb.init, QO_SGW_LYR_NAME)                             \
      QO_PRNT_FILE_LINE(qoSgwCb.init)                                                 \
      QOPRNT(qoSgwCb.init, (qoSgwCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}
#else /* DEBUGP */

#define QO_SGW_LYR_NAME

#define QO_ERRCLS_CHK(errCls) 

/* Debug prints for SGW */
#define QO_SGW_DBG_INFO(_arg)
#define QO_SGW_DBG_ERR(_arg)

/* Trace for SGW Code */
#define QO_SGW_TRC2(_arg)
#define QO_SGW_TRC3(_arg)

#define QO_SGW_RETVOID
#define QO_SGW_RETVALUE(_arg)
#endif /* DEBUGP */

#define qoUtlEgtBuildEgIeHeader egUtlEgtBuildEgIeHeader
#define qoUtlBuildEgHeader egUtlBuildEgHeader
#define qoUtlEgtAddCause egUtlEgtAddCause
/* qo001.101: Utility to add PrivateExt */
#define qoUtlEgtAddPrivateExt egUtlEgtAddPrivateExt
#define qoUtlEgtAddEpsBId egUtlEgtAddEpsBId

#define QO_SGW_SET_IE_LVL_INFO(arg, lvl, type, inst)                    \
{                                                                       \
   arg.level = lvl;                                                     \
   arg.ieInfo[lvl].ieTypePres = TRUE;                                     \
   arg.ieInfo[lvl].ieType = type;                                         \
   arg.ieInfo[lvl].ieInst = inst;                                         \
}

#define QO_SGW_GET_PDNCB(_cTun, _pdnCb)                                 \
{                                                                       \
   if (_cTun->intf != QO_IF_S11_C)                                      \
   {                                                                    \
      _pdnCb = _cTun->pdnCb;                                            \
   }                                                                    \
   else                                                                 \
   {                                                                    \
      QO_SGW_DBG_ERR(("S11 Tunnel not associated with PDN"));           \
   }                                                                    \
}

#ifdef __cplusplus
}
#endif
#endif /* __QO_SGWH__ */



/**********************************************************************

         End of file:     qo.h@@/main/1 - Thu Jul  8 21:22:18 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -       mm               1. LTE-CNE:SGW Initial Release.
/main/1      qo001.101 rp             1. Added macro for MAX tunnels.
                                      2. Added utility qoUtlEgtAddPrivateExt
*********************************************************************91*/
