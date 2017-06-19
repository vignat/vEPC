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

     Name:      avsm.h 

     Type:     C include file

     Desc:     Defines and Macros used by stack manager

     File:     avsm.h

     Sid:      avsm.h@@/main/1 - Tue Aug 17 18:54:35 2010

     Prg:       sp      

*********************************************************************21*/


#ifndef __AV_SM_H__
#define __AV_SM_H__

#define AVSM_TRANSID         999999999   /* default transaction ID */
#define AVSM_MSGPRIOR        PRIOR0      /* message priorities */

/* AV ---->SM posts */
#define AVSM_AVSMSEL           0
/* SM ---->AV posts */
#define AVSM_SMAVSEL           0


/* HI ----> SM selector */
#define AVSM_HISMSEL           0
/* SM ----> HI selector */
#define AVSM_SMHISEL           0

/* EG ---->SM posts */
#define AVSM_EGSMSEL           0
/* SM ---->EG posts */
#define AVSM_SMEGSEL           0



/* AV ----> EG selector */
#define AVSM_AVEGSEL           2
/* EG ----> AV selector */
#define AVSM_EGAVSEL           2


/* EG ----> SM selector */
#define AVSM_EGSMSEL           0

/* EG ----> HI selector */
#define AVSM_EGHISEL           0

/* HI ----> EG selector */
#define AVSM_HIEGSEL           0

#define AVSM_DEFAULT_PROCID        0

/**************CONFIGURATION PARAMETERS FOR EGTP  (eg)************/
#define AVSM_SGW_EG_C_SPID              0

#define AVSM_SGW_EG_U_SPID              1

#define AVSM_SGW_ECHO_SPID              2

#define AVSM_EG_HI_SPID              1
#define AVSM_EG_HI_SUID              0

#define AVSM_S5S8_C_SPID       0
#define AVSM_S5S8_C_SUID       0

#define AVSM_S5S8_U_SPID       1
#define AVSM_S5S8_U_SUID       1

#define AVSM_ECHO_SPID       2
#define AVSM_ECHO_SUID       2

#define AVSM_TPT_SRVR_EGTPC        0
#define AVSM_TPT_SRVR_EGTPU        1
#define AVSM_TPT_SRVR_ECHO         2

#define AVSM_MAX_IP_ADDR_STR_LEN    16
#define AVSM_MAX_HOST_NAME_LEN      139 /*just to make the structure size 256*/
#define AVSM_MAX_CFG_FILE_LINE_LEN  256
#define AVSM_MAX_CFG_PARAM_LEN      256

#define AVSM_EG_DFLT_ADDR           "172.25.0.91"
#define AVSM_EG_DFLT_PORT           2152
#define AVSM_EG_DFLT_HOST_NAME      "kukadi.ccin.ccpu.com"
#define AVEGSM_LEG_DFLT_GTP_C_ON   LEG_EGTP_C
#define AVEGSM_LEG_DFLT_GTP_U_ON   LEG_EGTP_U


/**************GENERAL CONFIGURATION PARAMETERS FOR TUCL (hi)************/
#define AVSM_HI_NUMBSAPS           10
#define AVSM_HI_NMBCONS            10

#ifdef HI_MULTI_THREADED
#define AVSM_HI_FDS                1024
#else
#define AVSM_HI_FDS                64
#endif /* HI_MULTI_THREADED */
#define AVSM_HI_FDBINS             4

#define AVSM_HI_SELTIMEOUT         2
#define AVSM_HI_UDP_MSGS_TOREAD    20
#define AVSM_HI_NUMCL_TOACCEPT     5

#define AVSM_HI_PERM_TSK_FLAG           FALSE
#define AVSM_HI_SCHDTMR_VAL             1

/* timer resolution unused */
#define AVSM_HI_TIME_RES                0
#define AVSM_HI_STRT_THRESH             1
#define AVSM_HI_DRP_THRESH              1
#define AVSM_HI_STP_THRESH              1

#define AVSM_HI_CONG_STRT                15000
#define AVSM_HI_CONG_DRP                 20000
#define AVSM_HI_CONG_STP                 10000
#define AVSM_HI_NMB_HLBINS               16

#define AVSM_HI_MMESPID                      0
#define AVSM_HI_SGWSPID                      1

/*************CONFIGURATION PARAMETERS FOR PGW  */
#define AVSM_PGW_DFLT_PORT                 2123
#define AVSM_PGW_DFLT_PEER_PORT            2123
#define AVSM_PGW_DFLT_ADDR               "172.25.0.91"
#define AVSM_PGW_DFLT_EXGW_ADDR          "172.25.0.254"
#define AVSM_PGW_DFLT_SGW_ADDR           "172.25.0.91"
#define AVSM_PGW_UE_START_ADDR            0xC0A90003
#define AVSM_PGW_UE_NUM                  1

/**************GENERAL CONFIGURATION PARAMETERS FOR S1AP (sz)************/

#define AVSM_SGW_IPADDR       0xAC19005B
/*Port No */
#define AVSM_SGW_PORT         9090

/* SGetSBuf & SPutSBuf Macros */
#define AVSM_ALLOC(_buf, _size)\
{\
   if (SGetSBuf(avSmCb.init.region, avSmCb.init.pool, (Data **) _buf,    \
                _size) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(*_buf), 0, _size);                             \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (*_buf) = NULLP;                                               \
   }                                                                 \
}
#define AVSM_FREE(_buf, _size)\
{\
   (Void) SPutSBuf(avSmCb.init.region, avSmCb.init.pool, (Data *) _buf, \
   (Size) _size);\
   (_buf) = NULLP;\
}

/* Timer values */
#define AVSM_LAV_VAL_10    10
#define AVSM_LAV_VAL_3     3

/* Stack Manager States and Events */
#define  AV_SM_STATE_INIT        0
#define  AV_SM_STATE_TUCL_CFG    1
#define  AV_SM_STATE_EGTPC_CFG   2
#define  AV_SM_STATE_EGTPU_CFG   3
#define  AV_SM_STATE_PGW_CFG     4
#define  AV_SM_STATE_EGTPC_CNTRL   5
#define  AV_SM_STATE_EGTPU_CNTRL   6
#define  AV_SM_STATE_PGW_CNTRL   7
#endif

#ifdef DEBUGP

/*-----------------DEBUG PRINTS-------------------------*/

#define AVSM_LYR_NAME "LTE-CNE-SM"
#define AVSM_PRNTBUF  avSmCb.init.prntBuf

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

#define AVSM_DBG_INFO(_arg) \
      SMDBG_INFO(avSmCb.init, AVSM_LYR_NAME, DBGMASK_LYR, _arg)

#define AVSM_DBG_ERROR(_arg) \
      SMDBG_ERROR(avSmCb.init, AVSM_LYR_NAME, DBGMASK_LYR, _arg)


#define AVSM_LOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(avSmCb.init.ent, avSmCb.init.inst, avSmCb.init.procId,     \
         (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
         (Txt *)errDesc);

/* Trace for Utilities / General Code */
#define SM_TRC2(_arg)                                                   \
   Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
      if (avSmCb.init.dbgMask & (DBGMASK_LYR))                        \
      {                                                                    \
             SM_PRNTTIMESTAMP(avSmCb.init)                                    \
             SM_PRNT_LAYER(avSmCb.init, AVSM_LYR_NAME)                             \
             SM_PRNT_FILE_LINE(avSmCb.init)                                  \
             SMPRNT(avSmCb.init, (avSmCb.init.prntBuf," Entering\n"));               \
             TRC2(_arg);                                                       \
        }                                                                    \
}

#define SM_TRC3(_arg)                                                   \
   Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
      if (avSmCb.init.dbgMask & (DBGMASK_LYR))                           \
      {                                                                    \
               SM_PRNTTIMESTAMP(avSmCb.init)                                    \
               SM_PRNT_LAYER(avSmCb.init, AVSM_LYR_NAME)                             \
               SM_PRNT_FILE_LINE(avSmCb.init)                                       \
               SMPRNT(avSmCb.init, (avSmCb.init.prntBuf," Entering\n"));                \
               TRC3(_arg);                                                       \
      }                                                                    \
}

#else

/* Layer Name appearing in the Debug prints */
#define AVSM_LYR_NAME 

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

#define AVSM_DBG_INFO(_arg)
#define AVSM_DBG_ERROR(_arg)

#define AVSM_LOGERROR(errCls, errCode, errVal, errDesc)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define SM_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#else
#define SM_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#endif

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, AvMmeCb is used presently.
 *  * Take care in the process of segregation of code.*/
#define SM_TRC2(_arg)
#define SM_TRC3(_arg)

#endif

/******************************************************************30**

         End of file:     avsm.h@@/main/1 - Tue Aug 17 18:54:35 2010

*********************************************************************31*/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

/********************************************************************60**

        Revision history:

*********************************************************************61*/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -      sp                 1. LTE-CNE:PGW Initial Release.
*********************************************************************91*/
