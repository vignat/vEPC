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

     Name:    Trillium LTE  SGW Application 

     Type:    C Include file

     Desc:    This file contains Macros required by Stack Manager

     File:    qosm.h

     Sid:      qosm.h@@/main/1 - Thu Jul  8 21:22:21 2010

     Prg:      sp
*********************************************************************21*/
#ifndef __QO_SM_H__
#define __QO_SM_H__


/* qo003.101 : CR 7468 Debug Mask removed */
#define QOSM_TRANSID         999999999   /* default transaction ID */
#define QOSM_MSGPRIOR        PRIOR0      /* message priorities */

/* QO ---->SM posts */
#define QOSM_QOSMSEL           0
/* SM ---->QO posts */
#define QOSM_SMQOSEL           0

/* HI ----> SM selector */
#define QOSM_HIEGSEL           0

/* HI ----> SM selector */
#define QOSM_HISMSEL           0
/* SM ----> HI selector */
#define QOSM_SMHISEL           0

/* EG ---->SM posts */
#define QOSM_EGSMSEL           0
/* SM ---->EG posts */
#define QOSM_SMEGSEL           0



/* QO ----> EG selector */
#define QOSM_QOEGSEL           2
/* EG ----> QO selector */
#define QOSM_EGQOSEL           2


/* EG ----> SM selector */
#define QOSM_EGSMSEL           0

/* EG ----> HI selector */
#define QOSM_EGHISEL           0

#define QOSM_DEFAULT_PROCID        0

/**************CONFIGURATION PARAMETERS FOR EGTP  (eg)************/
#define QOSM_EG_HI_SPID              1
#define QOSM_EG_HI_SUID              0

#define   QOSM_SGW_S11_C_SPID      1
#define   QOSM_SGW_S11_C_SUID     1

#define   QOSM_SGW_S5S8_C_SPID     2
#define   QOSM_SGW_S5S8_C_SUID      2

#define QOSM_SGW_EG_U_SPID        3
#define QOSM_SGW_EG_U_SUID        3

#define QOSM_SGW_ECHO_EG_SPID       4
#define QOSM_SGW_ECHO_EG_SUID       4

#define   QOSM_SGW_S4_C_SPID     5
#define   QOSM_SGW_S4_C_SUID     5

#define QOSM_MAX_IP_ADDR_STR_LEN    16
#define QOSM_MAX_HOST_NAME_LEN      139 /*just to make the structure size 256*/
#define QOSM_MAX_CFG_FILE_LINE_LEN  256
#define QOSM_MAX_CFG_PARAM_LEN      256

#define QOSM_EG_DFLT_ADDR           "172.25.0.91"
#define QOSM_EG_DFLT_PORT           2152
#define QOSM_EG_DFLT_HOST_NAME      "kukadi.ccin.ccpu.com"
#define QOEGSM_LEG_DFLT_GTP_C_ON   LEG_EGTP_C
#define QOEGSM_LEG_DFLT_GTP_U_ON   LEG_EGTP_U

#define QOSM_EG_S11_PORT           5000
#define QOSM_EG_S5S8_PORT          5001

#define QO_SGW_EGT_T3_RESP_TMR 10

#define QO_EG_SRV_U_DEF_ID         1
#define QO_EG_SRV_C_DEF_ID     2
#define QO_EG_SRV_C_S11_ID     3
#define QO_EG_SRV_C_S5S8_ID    4
#define QO_EG_SRV_C_S4_ID      5

#define QOSM_MAX_EG_TNLS    10

/**************GENERAL CONFIGURATION PARAMETERS FOR TUCL (hi)************/
#define QOSM_HI_NUMBSAPS           10
#define QOSM_HI_NMBCONS            10

#ifdef HI_MULTI_THREADED
#define QOSM_HI_FDS                1024
#else
#define QOSM_HI_FDS                64
#endif /* HI_MULTI_THREADED */
#define QOSM_HI_FDBINS             4

#define QOSM_HI_SELTIMEOUT         2
#define QOSM_HI_UDP_MSGS_TOREAD    20
#define QOSM_HI_NUMCL_TOACCEPT     5

#define QOSM_HI_PERM_TSK_FLAG           FALSE
#define QOSM_HI_SCHDTMR_VAL             1

/* timer resolution unused */
#define QOSM_HI_TIME_RES                0
#define QOSM_HI_STRT_THRESH             1
#define QOSM_HI_DRP_THRESH              1
#define QOSM_HI_STP_THRESH              1

#define QOSM_HI_CONG_STRT                15000
#define QOSM_HI_CONG_DRP                 20000
#define QOSM_HI_CONG_STP                 10000
#define QOSM_HI_NMB_HLBINS               16

#define QOSM_HI_SGWSPID                      0
#define QOSM_HI_SGWSUID                      1

/**************GENERAL CONFIGURATION PARAMETERS FOR S1AP (sz)************/

#define QOSM_SGW_IPADDR       0xAC19005B

/* SGetSBuf & SPutSBuf Macros */
#define QOSM_ALLOC(_buf, _size)\
{\
   if (SGetSBuf(qoSmCb.init.region, qoSmCb.init.pool, (Data **) _buf,    \
                _size) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(*_buf), 0, _size);                             \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (*_buf) = NULLP;                                               \
   }                                                                 \
}
#define QOSM_FREE(_buf, _size)\
{\
   (Void) SPutSBuf(qoSmCb.init.region, qoSmCb.init.pool, (Data *) _buf, \
   (Size) _size);\
   (_buf) = NULLP;\
}

/* Timer values */
#define QOSM_LQO_VAL_10    10
#define QOSM_LQO_VAL_3     3

/* Stack Manager States and Events */
#define  QO_SM_STATE_INIT        0
#define  QO_SM_STATE_TUCL_CFG    1
#define  QO_SM_STATE_EGTP_CFG    2
#define  QO_SM_STATE_SGW_CFG     3
#define  QO_SM_STATE_SGW_CNTRL   4
/*#define  QO_SM_STATE_QOAPP_BOUND 6*/


#ifdef DEBUGP

/*-----------------DEBUG PRINTS-------------------------*/

#define QOSM_LYR_NAME "LTE-CNE-SM"
#define QOSM_PRNTBUF  qoSmCb.init.prntBuf

/*------------------------- Trace Prints------------------------*/




/* macro to print buffer 
 * */
#define SMPRNT(_init, _prntBuf)   \
{                          \
      sprintf _prntBuf;      \
      SPrint(_init.prntBuf);            \
}

/* macro to get and print timestamp value */
#ifdef SM_PRNT_TIMESTAMP
#define SM_PRNTTIMESTAMP(_init)                                        \
{                                                                       \
      S8 _buf[60];                                                    \
      (Void) SGetTimeStamp(_buf);                                          \
      SMPRNT(_init, (_init.prntBuf, "[%s]:", _buf));                       \
}
#else
#define SM_PRNTTIMESTAMP(_init)
#endif

/* macro to print layer Name */
#define SM_PRNT_LAYER(_init, _lyr)                                     \
{                                                                       \
      SMPRNT(_init, (_init.prntBuf, " [%s 0x%x:%x]", _lyr,                \
                _init.ent, _init.inst));                                \
}

/* macro to print filename, line number and function name */
#define SM_PRNT_FILE_LINE(_init)                                       \
{                                                                       \
      SMPRNT(_init, (_init.prntBuf, " [%s:%d:%s]",__FILE__,__LINE__,      \
        __tapa_fun));                                                        \
}

/* macro to collect print all the information */
#define SM_DBG(_init, _lyr, _mask, _arg, _lvl) \
do \
{ \
   if(_init.dbgMask & (_mask)) \
   { \
      SM_PRNTTIMESTAMP(_init)                                     \
      SM_PRNT_LAYER(_init, _lyr)                                    \
      SM_PRNT_FILE_LINE(_init)                                      \
      SMPRNT(_init, (_init.prntBuf, " [" #_lvl "]" ));              \
      SMPRNT(_init, (_init.prntBuf, " : "));                        \
      SMPRNT(_init, _arg);                         \
      SMPRNT(_init, (_init.prntBuf, "\n"));    \
   } \
} while (0)

/* Debug macros for different debug levels */
#define SMDBG_INFO(_init, _layerName, _mask, _arg)\
      SM_DBG(_init, _layerName, _mask, _arg, "INFO")

#define SMDBG_ERROR(_init, _layerName, _mask, _arg)\
      SM_DBG(_init, _layerName, _mask, _arg, "ERROR")

/*--------------- Debug prints used under DEBUGP -------------------*/

#define QOSM_DBG_INFO(_arg) \
      SMDBG_INFO(qoSmCb.init, QOSM_LYR_NAME, DBGMASK_LYR, _arg)

#define QOSM_DBG_ERROR(_arg) \
      SMDBG_ERROR(qoSmCb.init, QOSM_LYR_NAME, DBGMASK_LYR, _arg)


#define QOSM_LOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(qoSmCb.init.ent, qoSmCb.init.inst, qoSmCb.init.procId,     \
         (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
         (Txt *)errDesc);

/* Trace for Utilities / General Code */
#define SM_TRC2(_arg)                                                   \
   Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
      if (qoSmCb.init.dbgMask & (DBGMASK_LYR))                        \
      {                                                                    \
               SM_PRNTTIMESTAMP(qoSmCb.init)                                    \
               SM_PRNT_LAYER(qoSmCb.init, QOSM_LYR_NAME)                             \
               SM_PRNT_FILE_LINE(qoSmCb.init)                                  \
               SMPRNT(qoSmCb.init, (qoSmCb.init.prntBuf," Entering\n"));               \
               TRC2(_arg);                                                       \
       }                                                                    \
}                                            
                                        
#define SM_TRC3(_arg)                                                   \
   Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
      if (qoSmCb.init.dbgMask & (DBGMASK_LYR))                           \
      {                                                                    \
               SM_PRNTTIMESTAMP(qoSmCb.init)                                    \
               SM_PRNT_LAYER(qoSmCb.init, QOSM_LYR_NAME)                             \
               SM_PRNT_FILE_LINE(qoSmCb.init)                                       \
               SMPRNT(qoSmCb.init, (qoSmCb.init.prntBuf," Entering\n"));                \
               TRC3(_arg);                                                       \
      }                                                                    \
}

#else

/* Layer Name appearing in the Debug prints */
#define QOSM_LYR_NAME 

/* macro to print buffer */
#define SMPRNT(_prntbuf) 

/* macro to get and print timestamp value */
#define SM_PRNTTIMESTAMP(_init)

/* macro to print layer Name */
#define SM_PRNT_LAYER(_init, _lyr)

/* macro to print filename, line number and function name */
#define SM_PRNT_FILE_LINE(_init) 

/* macro to collect print all the information */
#define SM_DBG(_init, _lyr, _mask, _arg, _lvl)


/*--------------- Debug prints used under DEBUGP -------------------*/

#define QOSM_DBG_INFO(_arg)
#define QOSM_DBG_ERROR(_arg)

#define QOSM_LOGERROR(errCls, errCode, errVal, errDesc)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define SM_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#else
#define SM_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#endif

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, QoMmeCb is used presently.
 *  * Take care in the process of segregation of code.*/
#define SM_TRC2(_arg)
#define SM_TRC3(_arg)

#endif /* DEBUGP */
#endif /* __QO_SM_H__ */


/**********************************************************************

         End of file:     qosm.h@@/main/1 - Thu Jul  8 21:22:21 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -          sp                  1.LTE-CNE:SGW Initial Release
/main/1   qo003.101     ms                  1.debug mask correction
*********************************************************************91*/
