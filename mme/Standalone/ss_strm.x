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
 
     Name:     System Services -- STREAMS
 
     Type:     C include file
 
     Desc:     Data structure definitions for STREAMS messages.
 
     File:     ss_strm.x
 
     Sid:      ss_strm.x@@/main/3 - Mon Nov 17 15:54:23 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSSTRMX__
#define __SSSTRMX__

#ifdef __cplusplus
extern "C" {
#endif


/* message block is defined in ssi.x */
typedef struct ssmsgb    SsMblk;



/* data block */
struct ssdatab
{
   U8                   db_ref;                 /* reference count */
   U8                   db_type;                /* type of data block */
	/* ss006.301 */
   Bool                 shared;                 /* is this data block shared ? */
   U8                   *db_base;               /* base of data buffer */
   U8                   *db_lim;                /* limit of data buffer */
	/* ss008.301 - deprecating for normal OS, useful for Cavium/RMI OS*/
#ifdef SS_DBLK_FREE_RTN
   struct ssfree_rtn    *db_frtnp;             /* free routine */
#endif /* SS_DBLK_FREE_RTN */
};

typedef struct ssdatab   SsDblk;



/* to free memory used with esballoc() */
struct ssfree_rtn
{
   void                 (*free_func)(char *);   /* free function */
   char                 *free_arg;              /* argument to function */
};

typedef struct ssfree_rtn SsFrtn;



/* STREAMS functions prototypes */
PUBLIC S16      ssStrmCfg ARGS((Region, Region));
PUBLIC S32      ssAdjMsg ARGS((SsMblk *, S32));
PUBLIC SsMblk   *ssAllocB ARGS((S32, U32));
PUBLIC SsMblk   *ssCopyB ARGS((SsMblk *));
PUBLIC SsMblk   *ssCopyMsg ARGS((SsMblk *));
PUBLIC SsMblk   *ssDupB ARGS((SsMblk *));
PUBLIC SsMblk   *ssDupMsg ARGS((SsMblk *));
PUBLIC SsMblk   *ssESBAlloc ARGS((U8 *, S32, S32, SsFrtn *));
PUBLIC void     ssFreeB ARGS((SsMblk *));
PUBLIC void     ssFreeMsg ARGS((SsMblk *));
PUBLIC void     ssLinkB ARGS((SsMblk *, SsMblk *));
PUBLIC S32      ssMsgDSize ARGS((SsMblk *));
PUBLIC S32      ssPullupMsg ARGS((SsMblk *, S32));
PUBLIC SsMblk   *ssRmvB ARGS((SsMblk *, SsMblk *));
PUBLIC S32      ssTestB ARGS((S32, U32));
PUBLIC SsMblk   *ssUnlinkB ARGS((SsMblk *));


#ifdef __cplusplus
}
#endif

#endif  /*  __SSSTRMX__  */


  
/********************************************************************30**

         End of file:     ss_strm.x@@/main/3 - Mon Nov 17 15:54:23 2008

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
1.1          ---      kp   1. initial release

1.2          ---      kp   1. C++ compilable, cosmetic changes

/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/3+   ss006.301  vsrao 1. Optimized the Dblk sharing in mBuf
/main/3+   ss008.301 srinivas  1. DBlks Free Routine guarded under
                                  Flag SS_DBLK_FREE_RTN
                               2. Optimization of New buffer management API's
*********************************************************************91*/
