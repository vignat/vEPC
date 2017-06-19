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

     Name:    EGTP - 

     Type:    C source file

     Desc:    

     File:    eg_edm.x

     Sid:      eg_edm.x@@/main/3 - Fri Feb  4 18:18:10 2011

     Prg:    ad  

*********************************************************************21*/

#ifndef __EGEDMX__
#define __EGEDMX__

#ifdef __cplusplus
extern "C" {
#endif   /* __cplusplus  */ 

/** @file eg_edm.x
    @brief Encoder-Decoder Module(eg_edm.x)
 */

   /**@struct _egIeEntry
      @brief GTP IE Entry Control Block */
typedef struct _egIeEntry
{
  U8                   ieCode;                    /**< IE Code */
  U8                   dataType;                  /**<  Data type of IE */
  U16                   minLen;                    /**< Min Length */
  U16                   maxLen;                    /**< Max Length */
#ifdef EGTP_U
  Bool                 tvType;                    /**< Is IE type TV? */
#endif
}EgIeEntry;

   
#ifdef EGTP_C
typedef struct _egIeProperties EgIeProperties;
typedef struct _egGroupedIe  EgGroupedIe;

struct _egGroupedIe
{
   U8                   nmbGrpIe;                        /* Number of grouped IEs  */
   U8                   posIndex[EG_MAX_INST][EG_MAX_IE];  /**< Store the position index */
   EgIeProperties       *ieProperties;                   /**< LEG_MAX_IES_PER_GRPIEMaximum IEs per grouped IE */
   U8                   visitedIe[LEG_MAX_IES_PER_GRPIE];       /**< just to check repeated IEs */
};


struct _egIeProperties
{
   U8                  ieType;                     /* IE Code    */
   U8                  ieClass;                    /* IE Class MANDATORY/OPTIONAL/CONDITIONAL */ 
   U8                  fixedPos;                   /* Fixed Position of the Information Element */
   Bool                ieList;                     /* IE can be list */
   U8                  ieInst;                   /* Instance  */
   EgGroupedIe        *groupedIe;                  /* List of IEs in Grouped IE */
};

typedef struct _egMsgEntry
{
   U8                  msgType;                                   /* code of the Message  */
   U8                  posIndex[EG_MAX_INST][EG_MAX_IE];  /* Store the position index */
   U8                  nmbIeProp;                                 /* Number of IE Properties */
   EgIeProperties      ieProperties[EG_MAX_IES_PER_MSG];         /* Maximum IEs per message  */
   EscFun              validateIe;                              /* Escape function */
}EgMsgEntry;

/* GTP MSG Control Block */
typedef struct _egGmCb
{
   CmHashListEnt          gmHlEnt;               /* Entity for Hash List manipulation */
   EgMsgEntry            gmEntry;                /* DM Entry */
   U8                    key;                    /* Key to insert in the Hlist */
}EgGmCb;

typedef struct _egIeCb
{
   U8        nmbIe;       /* Number of IE's present in all the msgs */
   U16        posIdx[EG_MAX_IE];    /* Contains the pos of the IE in the entry */
   EgIeEntry ieEntry[EG_MAX_IES_ALL_MSGS];          /* List of the IE Entries */
}EgIeCb;


typedef struct _egIntfCb 
{
   
   CmHashListCp  gmDictHlCp;               /* Interface specific message Dictionary */
                                           /*   Hash list control pointer */

}EgIntfCb;
typedef struct _edmCb
{
   EgIeCb       egIeCb;                       /* List of IE CB for all intf */
   EgIntfCb      **intfCbLst;                 /* list of msg CB for all intf */
   TknU8         egIdxToIntfMap[LEG_MAX_INTFTYPE]; /* Interface to Index mapping */
}EdmCb;

#ifndef SS_MULTIPLE_PROCS
EXTERN EdmCb edmCb;
#endif /* SS_MULTIPLE_PROCS */

#endif /* End of EGTP_C */

#ifdef EGTP_U
typedef struct _egUIeProperties EgUIeProperties;

/**@struct _egUIeProperties
   @brief Properties of IE */
struct _egUIeProperties
{
   U8                  ieType;                     /**< IE Code    */
   U8                  ieClass;                    /**< IE Class MANDATORY/OPTIONAL */ 
};

/**@struct _egUMsgEntry
 * @brief Message Entry in the DB */
typedef struct _egUMsgEntry
{
   U8                  msgType;                                   /**< Message Code  */
   U8                  nmbIe;                                   /**< Number of IE Properties */
   U8                  nmbMandIe;                           /**< Number of Mandatory IEs */
   EgUIeProperties     ieProperties[EGU_MAX_IES_PER_MSG];         /**< IE Properties List  */
}EgUMsgEntry;

/**@struct _egUGmCb
   @brief MSG Control Block */
typedef struct _egUGmCb
{
   U16                    posIdx[EGU_MAX_MSG];  /**< Position Index */
   EgUMsgEntry           gmEntry[EGU_SUP_MSGS]; /**< Message Entry */
}EgUGmCb;

/**@struct _egUIeCb
   @brief IE Contrl Block */
typedef struct _egUIeCb
{
   U16        posIdx[EGU_MAX_IE];    /**< Position Index */
   EgIeEntry ieEntry[EGU_SUP_IES];          /**< List of the IE Entries */
}EgUIeCb;

/**@struct _egUEdmCb
   @brief GTP-U EDM Control Block */
typedef struct _egUEdmCb
{
   EgUIeCb       egIeCb;                       /**< IE Control Block */
   EgUGmCb       egMsgCb;      /**< Msg Control Block */
}EgUEdmCb;

#endif /* End of EGTP_U */

#ifdef EGTP_C
PUBLIC EgMsgEntry* egDbGetMsgFrmGmDict ARGS((U8 msgType, U8 intfType));
PUBLIC EgIeEntry* egGetIeEntryFrmDb ARGS (( U8 ieType));

PUBLIC Void egDbCfgIeDict ARGS
((
U8           nmbIe,              /* Number of Ie's */
U16          *posIdx,           /* pos for the IE */
LegIeEntry   *ieEntry            /* Ie Entry to be inserted */
));

PUBLIC S16   egDbDropGmDict ARGS
((
U8     intfType         /* Interface type to the CP */
));


PUBLIC S16 egDbCfgGmDict ARGS
((
U8          intfType,            /* Interface Type */
U16          nmbMsg,            /* Number of Messages to be inserted */
LegMsgEntry   *gmEntry            /* GM Entry to be inserted */
));

#endif /* End of EGTP_C */


#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* __LEGX__ */

/********************************************************************30**

         End of file:     eg_edm.x@@/main/3 - Fri Feb  4 18:18:10 2011

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
----------- -------- ---- -----------------------------------------------
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
*********************************************************************91*/


