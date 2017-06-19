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
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:     
  
     File:      vbsm.h
  
     Sid:      vbsm.h@@/main/1 - Mon Jun 28 19:35:41 2010
  
     Prg:        sp 
  
*********************************************************************21*/


#ifndef _VBSM_H_
#define _VBSM_H_

#define VBSM_TRANSID         999999999   /* default transaction ID */
#define VBSM_MSGPRIOR        PRIOR0      /* message priorities */

/* VB ---->SM posts */
#define VBSM_VBSMSEL           0
/* SM ---->VB posts */
#define VBSM_SMVBSEL           0

/* SZ ---->SM posts */
#define VBSM_SZSMSEL           0
/* SM ---->SZ posts */
#define VBSM_SMSZSEL           0

/* SB ----> SM selector */
#define VBSM_SBSMSEL           0
/* SM ----> SB selector */
#define VBSM_SMSBSEL           0

/* HI ----> SM selector */
#define VBSM_HISMSEL           0
/* SM ----> HI selector */
#define VBSM_SMHISEL           0

/* EG ---->SM posts */
#define VBSM_EGSMSEL           0
/* SM ---->EG posts */
#define VBSM_SMEGSEL           0



/* VB ----> SZ selector */
#define VBSM_VBSZSEL           2
/* SZ ----> VB selector */
#define VBSM_SZVBSEL           2

/* SZ ----> SB selector */
#define VBSM_SZSBSEL           0
/* SB ----> SZ selector */
#define VBSM_SBSZSEL           0

/* SB ----> HI selector */
#define VBSM_SBHISEL           0
/* HI ----> SB selector */
#define VBSM_HISBSEL           0

/* VB ----> EG selector */
#define VBSM_VBEGSEL           0  /* LC from MME to EGTP-C */
/* EG ----> VB selector */
#define VBSM_EGVBSEL           1 /* LWLC from EGTP-C to MME */

/* EG ----> SM selector */
#define VBSM_EGSMSEL           0

/* EG ----> HI selector */
#define VBSM_EGHISEL           0

#define VBSM_DEFAULT_PROCID        0

/**************CONFIGURATION PARAMETERS FOR EGTP  (eg)************/
#define VBSM_EG_HI_SPID              1
#define VBSM_EG_HI_SUID              0

#define VBSM_EG_SPID                 0
#define VBSM_EG_SUID                 0
#define VBSM_EG_ECHO_SPID            1
#define VBSM_EG_ECHO_SUID            1
#define VBSM_EG_S10_SPID             2
#define VBSM_EG_S10_SUID             2
#define VBSM_EG_S3_SPID              3
#define VBSM_EG_S3_SUID              3

#define VBSM_EG_ECHO_TPTSRVID        1
#define VBSM_EG_TPTSRVID             2
#define VBSM_EG_S10_TPTSRVID         3
#define VBSM_EG_S3_TPTSRVID          4

#define VBSM_MAX_IP_ADDR_STR_LEN    16
#define VBSM_MAX_HOST_NAME_LEN      139 /*just to make the structure size 256*/
#define VBSM_MAX_CFG_FILE_LINE_LEN  256
#define VBSM_MAX_CFG_PARAM_LEN      256

#define VBSM_EG_MAX_TNLS           10


/**************GENERAL CONFIGURATION PARAMETERS FOR TUCL (hi)************/
#define VBSM_HI_NUMBSAPS           10
#define VBSM_HI_NMBCONS            10

#ifdef HI_MULTI_THREADED
#define VBSM_HI_FDS                1024
#else
#define VBSM_HI_FDS                64
#endif /* HI_MULTI_THREADED */
#define VBSM_HI_FDBINS             4

#define VBSM_HI_SELTIMEOUT         2
#define VBSM_HI_UDP_MSGS_TOREAD    20
#define VBSM_HI_NUMCL_TOACCEPT     5

#define VBSM_HI_PERM_TSK_FLAG           FALSE
#define VBSM_HI_SCHDTMR_VAL             1

/* timer resolution unused */
#define VBSM_HI_TIME_RES                0
#define VBSM_HI_STRT_THRESH             1
#define VBSM_HI_DRP_THRESH              1
#define VBSM_HI_STP_THRESH              1

#define VBSM_HI_CONG_STRT                15000
#define VBSM_HI_CONG_DRP                 20000
#define VBSM_HI_CONG_STP                 10000
#define VBSM_HI_NMB_HLBINS               16

#define VBSM_HI_MMESPID                  0

/**************GENERAL CONFIGURATION PARAMETERS FOR SCTP (sb)************/

#define VBSM_SBSCTSAP_CFG     0x4
#define VBSM_SB_SPID          0
#define VBSM_SB_SUID          0

#define VBSM_SB_HI_SPID       0


/**************GENERAL CONFIGURATION PARAMETERS FOR S1AP (sz)************/
/* gen cfg */
#define VBSM_SZ_SPID          0
#define VBSM_SZ_SUID          0
#define VBSM_LSZ_MAX_SZTSAP   10
#define VBSM_LSZ_MAX_SCTSAP   10

#define VBSM_LSZ_TIMERES      10

#define VBSM_LSZ_MME_PRC1     0

#define VBSM_MAX_PROFILE      50
#define VBSM_LSZ_INV_NODE     10
#define VBSM_LSZ_RESETRETRY   10


/* Peer Cfg */
#define VBSM_LSZ_NUM_PEERS       1
#define VBSM_LSZ_OUT_STRMS       3

/* Peer cntrl */
#define VBSM_LSZ_PEERID       0

#define VBSM_LSZ_VAL_3        3
#define VBSM_LSZ_VAL_5        5
#define VBSM_LSZ_VAL_10       10 
#define VBSM_LSZ_VAL_50       50

#define VBSM_LTYPE_CONST      1
#define VBSM_LTYPE_VAR        2

#define VBSM_MAX_PLMN_ID_LEN  3

/* SGetSBuf & SPutSBuf Macros */
#define VBSM_ALLOC(_buf, _size)\
{\
   if (SGetSBuf(vbSmCb.init.region, vbSmCb.init.pool, (Data **) _buf,    \
                _size) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(*_buf), 0, _size);                             \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (*_buf) = NULLP;                                               \
   }                                                                 \
}
#define VBSM_FREE(_buf, _size)\
{\
   (Void) SPutSBuf(vbSmCb.init.region, vbSmCb.init.pool, (Data *) _buf, \
   (Size) _size);\
   (_buf) = NULLP;\
}

/* Timer values */
#define VBSM_LVB_VAL_10    10
#define VBSM_LVB_VAL_3     3

#ifdef DEBUGP

/*-----------------DEBUG PRINTS-------------------------*/

#define VBSM_LYR_NAME "LTE-CNE-SM"
#define VBSM_PRNTBUF  vbSmCb.init.prntBuf

/***********************************************************************
  Defines for Debug Masks
***********************************************************************/
#define VBSM_DBGMASK_MEM                       (DBGMASK_LYR << 0)
#define VBSM_DBGMASK_TRC                       (DBGMASK_LYR << 1)
#define VBSM_DBGMASK_ERROR                     (DBGMASK_LYR << 2)
#define VBSM_DBGMASK_INFO                      (DBGMASK_LYR << 3)


/***********************************************************************/

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
{                                                                      \
      SMPRNT(_init, (_init.prntBuf, " [%s:%d:%s]",__FILE__,__LINE__,   \
           __tapa_fun));                                               \
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

#define VBSM_DBG_INFO(_arg) \
      SMDBG_INFO(vbSmCb.init, VBSM_LYR_NAME, VBSM_DBGMASK_INFO, _arg)

#define VBSM_DBG_ERROR(_arg) \
      SMDBG_ERROR(vbSmCb.init, VBSM_LYR_NAME, VBSM_DBGMASK_ERROR, _arg)

/* vb006.101: Fixed g++ compilation warning. */
#define VBSM_LOGERROR(errCls, errCode, errVal, errDesc) \
   SLogError(vbSmCb.init.ent, vbSmCb.init.inst, vbSmCb.init.procId,     \
         __FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
         errDesc);

/* Trace for Utilities / General Code */
#define SM_TRC2(_arg)                                                   \
   Txt __tapa_fun[PRNTSZE];                                             \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
      if (vbSmCb.init.dbgMask & (VBSM_DBGMASK_TRC))                     \
      {                                                                 \
        SM_PRNTTIMESTAMP(vbSmCb.init)                                   \
        SM_PRNT_LAYER(vbSmCb.init, VBSM_LYR_NAME)                       \
        SM_PRNT_FILE_LINE(vbSmCb.init)                                  \
        SMPRNT(vbSmCb.init, (vbSmCb.init.prntBuf," Entering\n"));       \
        TRC2(_arg);                                                     \
     }                                                                    \
}

#define SM_TRC3(_arg)                                                   \
   Txt __tapa_fun[PRNTSZE];                                             \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
      if (vbSmCb.init.dbgMask & (VBSM_DBGMASK_TRC))                     \
      {                                                                 \
        SM_PRNTTIMESTAMP(vbSmCb.init)                                   \
        SM_PRNT_LAYER(vbSmCb.init, VBSM_LYR_NAME)                       \
        SM_PRNT_FILE_LINE(vbSmCb.init)                                  \
        SMPRNT(vbSmCb.init, (vbSmCb.init.prntBuf," Entering\n"));       \
        TRC3(_arg);                                                     \
      }                                                                 \
}

#else

/* Layer Name appearing in the Debug prints */
#define VBSM_LYR_NAME 

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

#define VBSM_DBG_INFO(_arg)
#define VBSM_DBG_ERROR(_arg)

#define VBSM_LOGERROR(errCls, errCode, errVal, errDesc)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define SM_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#else
#define SM_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#endif

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, VbMmeCb is used presently.
 *  * Take care in the process of segregation of code.*/
#define SM_TRC2(_arg)
#define SM_TRC3(_arg)

#endif

#define VBSM_ENC_PLMN_ID(_plmnid, _chrbuf)                        \
{                                                                 \
   _plmnid[0] = ((_chrbuf[1] - '0') << 4) | (_chrbuf[0] - '0');   \
   if (_chrbuf[5] == 'f')                                         \
   {                                                              \
      _plmnid[1] = ((0xf0) | (_chrbuf[2] - '0'));                 \
   }                                                              \
   else                                                           \
   {                                                              \
      _plmnid[1] = ((_chrbuf[5] - '0') << 4) | (_chrbuf[2] - '0');\
   }                                                              \
   _plmnid[2] = ((_chrbuf[4] - '0') << 4) | (_chrbuf[3] - '0');   \
}

#endif

/**********************************************************************

         End of file:     vbsm.h@@/main/1 - Mon Jun 28 19:35:41 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -          sp               1. LTE-CNE Initial Release.
/main/1   vb006.101   ragrawal           1. Fixed g++ compilation warning. 
*********************************************************************91*/


