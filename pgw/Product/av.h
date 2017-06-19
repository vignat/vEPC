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

     Name:    Trillium LTE PGW Application 

     Type:    C Include file

     Desc:    This file contains the PGW defines code

     File:     av.h

     Sid:      av.h@@/main/1 - Tue Aug 17 18:54:32 2010

     Prg:     sv 
*********************************************************************21*/

#ifndef __AV_PGWH__
#define __AV_PGWH__

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#define AV_PGW_PRIM_INSTANCE    0
#define AV_PGW_SECOND_INSTANCE  1

#define EVTAVRCVARPREQ            111   /* Event for post to start capturing */
#define EVTAVSENDDLDATREQ         112   /* Event for post to start DL Data load */

#define AV_PGW_ROUT_DBG_BUF_LEN             1024

#define AV_HASH_LIST_SIZE                   1024

#define AV_PGW_ROUT_IP_ADDR_LEN             4
#define AV_PGW_ROUT_MAX_ETH_PKT_LEN         1600
#define AV_PGW_ROUT_MAX_ARP_PKT_LEN         60
#define AV_PGW_ROUT_MAC_ADDR_LEN            6
#define AV_PGW_ROUT_MIN_ETH_PKT_SIZE        64
/* av003.101: 115767  To avoide the packet fragmentation. */
#define AV_PGW_ROUT_MAX_DATA_SIZE           1504

#define AV_PGW_ROUT_PCAP_BUF_SIZE           65536
#define AV_PGW_ROUT_PCAP_TIMEOUT            10    /* In milli second */
#define AV_PGW_ROUT_PCAP_MAX_DEV_LEN        16

#define AV_PGW_MAX_NW_NODES                  32
#define AV_PDN_TUN_MAX                       16
/* av001.101: Added support for TFT */
   /*TFT Related */
#define AV_PGW_MAX_TFT                       100
#define AV_PGW_IPV4_SIZE                     4    
#define AV_PGW_IPV6_SIZE                     6    
#define AV_PGW_IP_SEC_SIZE                   4
#define  AV_PGW_IPV6_FLOW_LABEL_SIZE         3
#define AV_PGW_TFT_MAX_PARAM_BUF             10   
#define AV_PGW_MAX_PKT_FILTERS               10   
#define AV_PGW_TFT_MAX_TFT_BUF               50
#define AV_PGW_MAX_PARAMS                    10   

   /* TFT Related Macro */
#define AV_PGW_PF_CMPTYPE_IP4_ADDR        0x10
#define AV_PGW_PF_CMPTYPE_IP6_ADDR        0x20
#define AV_PGW_PF_CMPTYPE_PROT_ID         0x30
#define AV_PGW_PF_CMPTYPE_LOC_PORT        0x40
#define AV_PGW_PF_CMPTYPE_LOC_PORT_RANGE  0x41
#define AV_PGW_PF_CMPTYPE_REM_PORT        0x50
#define AV_PGW_PF_CMPTYPE_REM_PORT_RANGE  0x51
#define AV_PGW_PF_CMPTYPE_SEC_PARAM       0x60
#define AV_PGW_PF_CMPTYPE_TOS             0x70
#define AV_PGW_PF_CMPTYPE_FLOW_LABEL      0x80

#define AV_PGW_EGT_T3_RESP_TMR            10
#define AV_PGW_MAX_BEARERS                11

#define AV_PGW_LOC_PORT                   2124
#define AV_PGW_REM_PORT                   2123

/* Changing the value of maximum no of UEs to support performance setup */
#define AV_PGW_MAX_UE                     200
#define AV_PGW_PRIMARY_GW_IDX             0

#define AV_PGW_CHRGNG_ID                  1

#define AV_PGW_CMD1                       "netstat -rn > gateway.txt"
#define AV_PGW_CMD2                       "rm -f gateway.txt"
#define AV_PGW_FILENAME                   "gateway.txt"
#define AV_PGW_DEFAULT                    "0.0.0.0"

#define ERRAVXXX                          0

/* av001.101: Moved up to TFT related macros */
#define AV_PGW_MAX_PF_COMP                3

/* 00 - pre Rel-7 TFT filter*/
/* 01 - downlink only */
/* 10 - uplink only */
/* 11 - bidirectional */
#define GTP_PRE_REL_7_TFT_FILTER            0x00    
#define GTP_DOWNLINK_TFT_FILTER             0x01    
#define GTP_UPLINK_TFT_FILTER               0x02    
#define GTP_BIDIRECTIONAL_TFT_FILTER        0x03    

#define GTP_MAX_PKT_FILTERS_PER_TFT         15  /* 4 bits : 15 */

/* E bit == 0 */
/* E bit == 1 */
#define GTP_PARAM_LIST_NOT_INCLUDED         0x00    
#define GTP_PARAM_LIST_INCLUDED             0x01    

/* TFTCODE */
/*  Various Component Type Presence masks */
#define GTP_IPV4_REM_ADDR_PKT_FLTR_MASK     0x0001 /* IPv4 remote address type*/
#define GTP_IPV6_REM_ADDR_PKT_FLTR_MASK     0x0002 /* IPv6 remote address type*/
#define GTP_PROTO_ID_PKT_FLTR_MASK          0x0004 /* Protocol id/Nxt header  */
#define GTP_SNGL_LOC_PORT_TY_PKT_FLTR_MASK  0x0008 /* Single local port type  */
#define GTP_LOC_PORT_RNG_TY_PKT_FLTR_MASK   0x0010 /* Local port range type   */
#define GTP_SNGL_REM_PORT_TY_PKT_FLTR_MASK  0x0020 /* Single remote port type */
#define GTP_REM_PORT_RNG_TY_PKT_FLTR_MASK   0x0040 /* Remote port range type  */
#define GTP_SECURITY_PARAM_TY_PKT_FLTR_MASK 0x0080 /* Security parameter index*/
#define GTP_SERV_N_CLASS_TY_PKT_FLTR_MASK   0x0100 /* Type of service/Traffic */
#define GTP_FLOW_LABEL_TY_PKT_FLTR_MASK     0x0200 /* Flow label type         */

/*  Component Type as per specs */
#define GTP_IPV4_REM_ADDR_PKT_FLTR     0x10 /*IPv4 remote address type */
#define GTP_IPV6_REM_ADDR_PKT_FLTR     0x20 /*IPv6 remote address type */
#define GTP_PROTO_ID_PKT_FLTR          0x30 /*Protocol id/Next header type*/
#define GTP_SNGL_LOC_PORT_TY_PKT_FLTR  0x40 /*Single local port type */
#define GTP_LOC_PORT_RNG_TY_PKT_FLTR   0x41 /*Local port range type */
#define GTP_SNGL_REM_PORT_TY_PKT_FLTR  0x50 /*Single remote port type */
#define GTP_REM_PORT_RNG_TY_PKT_FLTR   0x51 /*Remote port range type */
#define GTP_SECURITY_PARAM_TY_PKT_FLTR 0x60 /*Security parameter index type*/
#define GTP_SERV_N_CLASS_TY_PKT_FLTR   0x70 /*Type of service/Traffic class */
#define GTP_FLOW_LABEL_TY_PKT_FLTR     0x80 /*Flow label type */
/*  Component Type as per specs */


/*  0x00    Spare */
/* 0 0 1 Create new TFT */
/* 0 1 0 Delete existing TFT */
/* 0 1 1 Add packet filters to existing TFT     */
/* 1 0 0 Replace packet filters in existing TFT */
/* 1 0 1 Delete packet filters from existing TFT*/
/* 1 1 0 No TFT operation */
/* 1 1 1 Reserved  */
#define GTP_CREATE_NEW_TFT                  0x01    
#define GTP_DELETE_EXISTING_TFT             0x02    
#define GTP_ADD_PKT_FLTR_TO_EXISTNG_TFT     0x03    
#define GTP_REPLACE_PKT_FLTR_IN_EXISTNG_TFT 0x04    
#define GTP_DEL_PKT_FLTR_FRM_EXISTNG_TFT    0x05    
#define GTP_NO_TFT_OPERATION                0x06    

#define AV_PGW_TFT_IE_MIN_LEN             1
#define AV_PGW_TFT_IE_MAX_LEN             255

/* Validate TFT operation code (octet)
 * Bits  
 * 8 7 6
 * 0 0 0  Spare 0x00
 * 1 1 1  Reserved 0x07
 * */
#define GTP_TFT_OPERATION_CODE_VALID(value) \
         ((value >= GTP_CREATE_NEW_TFT) &&  \
          (value <= GTP_NO_TFT_OPERATION)) 

#define GTP_LEFT_NIBBLE_MASK                            0xF0
#define GTP_RIGHT_NIBBLE_MASK                           0x0F
#define GTP_TFT_E_BIT_MASK                              0x10
#define GTP_TFT_PKT_DIR_MASK                            0x30 
/* Protocol component identifier/header type */

#define GTP_PROTO_TYPE_SIZE                             1    
/* IPv4 remote address + mask */
#define GTP_IPV4_REM_ADDR_SIZE                          8    
/* IPv6 remote address + mask */
#define GTP_IPV6_REM_ADDR_SIZE                          32   
/* Protocol identifier/Next header  */
#define GTP_PROTO_ID_SIZE                               1 
/* Single local port  */
#define GTP_SNGL_LOC_PORT_TY_SIZE                       2    
/* Local port range  (low + high)*/
#define GTP_LOC_PORT_RNG_TY_SIZE                        4    
/* Single remote port  */
#define GTP_SNGL_REM_PORT_TY_SIZE                       2    
/* Remote port range (low + high)*/
#define GTP_REM_PORT_RNG_TY_SIZE                        4    
/* Security parameter index */
#define GTP_SECURITY_PARAM_TY_SIZE                      4    
/* Type of service/Traffic class */
#define GTP_SERV_N_CLASS_TY_SIZE                        2    
/* Flow label */
#define GTP_FLOW_LABEL_TY_SIZE                          3    

#define GTP_TFT_FLOW_LABEL_TY_LEN                       3


#define GTP_FETCH_LEFT_NIBBLE(byte)    (byte >> 4)

#define GTP_FETCH_RIGHT_NIBBLE(byte)   (byte & GTP_RIGHT_NIBBLE_MASK)

/* E bit (bit 5 of octet) */
#define GTP_GET_TFT_E_BIT(byte_info, param_present) \
        param_present = (byte_info & GTP_TFT_E_BIT_MASK); \
        param_present = (GTP_FETCH_LEFT_NIBBLE(param_present));

#define GTP_GET_TFT_NO_PKT_FILTERS(byte_info, filters_count) \
                filters_count = GTP_FETCH_RIGHT_NIBBLE(byte_info);

#define LTE_GET_U8(p_buf, value) { \
    value = p_buf[0]; \
    p_buf += 1; \
}

#define LTE_GET_U16(p_buf, value) { \
    value = p_buf[1] | (p_buf[0] << 8); \
    p_buf += 2; \
}

#define LTE_GET_U24(p_buf, value) { \
    value = p_buf[2] | (p_buf[1] << 8) | (p_buf[0] << 16); \
    p_buf += 3; \
}

#define LTE_GET_U32(p_buf, value) { \
    value = p_buf[3] | (p_buf[2] << 8) | (p_buf[1] <<16) | (p_buf[0] << 24); \
    p_buf += 4; \
}

#define LTE_GET_NBYTES(p_buf, p_array, length) { \
    cmMemcpy(p_array, p_buf, length); \
    p_buf += length; \
}

#define AV_PGW_PKT_FILTER_ID(_ebi, _id)      ((_ebi << 4) | (_id))
#define AV_PGW_GET_EBI_FROM_PFID(_pfid)      (_pfid >> 4)

 /* Extracts the packet filter  component type identifier. */
#define GTP_GET_TFT_PKT_COMPONENT_ID(p_buf, presence_mask) \
        LTE_GET_U8(p_buf, presence_mask)   

/* Reads 1 octet field  */
#define GTP_GET_TFT_INFO(p_buf, value) \
        LTE_GET_U8(p_buf, value) 

/* Extracts the packet filter id. */
#define GTP_GET_TFT_PKT_FILTER_ID(byte_info, filterid) \
         filterid = GTP_FETCH_RIGHT_NIBBLE(byte_info); 

#define GTP_GET_TFT_PKT_FILTER_DIRECTION(byte_info, filter_dir) \
        filter_dir = (byte_info & GTP_TFT_PKT_DIR_MASK); \
        filter_dir = (GTP_FETCH_LEFT_NIBBLE(filter_dir));

/* Extracts the packet filter evaluation precedence. */
#define GTP_GET_TFT_PKT_FILTER_EVALUATION(p_buf, precedence) \
        LTE_GET_U8(p_buf, precedence)   

/* length of the packet filter contents field  */
#define GTP_GET_TFT_PKT_FLTR_CONTENT_LEN(p_buf, value) \
        LTE_GET_U8(p_buf, value) 

/* Check presence of filed.*/
/*  IPv4 remote address type */
#define GTP_TFT_IPV4_REM_ADDR_PRESENT(flags) \
    (((flags & GTP_IPV4_REM_ADDR_PKT_FLTR_MASK) ? TRUE : FALSE))

/*  IPv6 remote address type */
#define GTP_TFT_IPV6_REM_ADDR_PRESENT(flags) \
    (((flags & GTP_IPV6_REM_ADDR_PKT_FLTR_MASK) ? TRUE : FALSE))

 /*  Protocol identifier/Next header type */
#define GTP_TFT_PROTO_ID_PRESENT(flags) \
            (((flags & GTP_PROTO_ID_PKT_FLTR_MASK) ? TRUE : FALSE))

/*  Single local port type */
#define GTP_TFT_SNGL_LOC_PORT_TY_PRESENT(flags) \
            (((flags & GTP_SNGL_LOC_PORT_TY_PKT_FLTR_MASK) ?\
              TRUE : FALSE))

 /*  Local port range type */
#define GTP_TFT_LOC_PORT_RNG_TY_PRESENT(flags) \
            (((flags & GTP_LOC_PORT_RNG_TY_PKT_FLTR_MASK) ? \
              TRUE : FALSE))

 /*  Single remote port type */
#define GTP_TFT_SNGL_REM_PORT_TY_PRESENT(flags) \
            (((flags & GTP_SNGL_REM_PORT_TY_PKT_FLTR_MASK) ? \
              TRUE : FALSE))

 /*  Remote port range type */
#define GTP_TFT_REM_PORT_RNG_TY_PRESENT(flags) \
            (((flags & GTP_REM_PORT_RNG_TY_PKT_FLTR_MASK) ? \
              TRUE : FALSE))

 /*  Security parameter index type */
#define GTP_TFT_SECURITY_PARAM_TY_PRESENT(flags) \
            (((flags & GTP_SECURITY_PARAM_TY_PKT_FLTR_MASK) ?\
              TRUE : FALSE))

 /*  Type of service/Traffic class type */
#define GTP_TFT_SERV_N_CLASS_TY_PRESENT(flags) \
            (((flags & GTP_SERV_N_CLASS_TY_PKT_FLTR_MASK) ?\
              TRUE : FALSE))

 /*  Flow label type */
#define GTP_TFT_FLOW_LABEL_TY_PRESENT(flags) \
            (((flags & GTP_FLOW_LABEL_TY_PKT_FLTR_MASK) ? TRUE : FALSE))

#define GTP_GET_TFT_IPV4_REM_ADDR(p_field, component) \
{\
   LTE_GET_U32(p_field,\
      component.uRemIpAddr.remIpv4Comp.ipv4_addr.address); \
}

#define GTP_GET_TFT_IPV4_REM_ADDR_MASK(p_field, component) \
{\
   LTE_GET_U32(p_field, \
            component.uRemIpAddr.remIpv4Comp.ipv4_addr_mask.address);\
}

#define GTP_GET_TFT_PROTO_ID_PKT_FLTR(p_field, proto_id) \
        LTE_GET_U8(p_field, proto_id)   

#define GTP_GET_TFT_SNGL_LOC_PORT(p_field, component) \
        LTE_GET_U16(p_field, component.locPort)   

#define GTP_GET_TFT_LOC_PORT_RNG(p_field, component) \
        LTE_GET_U16(p_field, component.locPort);  \
        LTE_GET_U16(p_field, component.locPortRange);   

#define GTP_GET_TFT_SNGL_REM_PORT(p_field, component) \
        LTE_GET_U16(p_field, component.remPort)   

#define GTP_GET_TFT_REM_PORT_RNG(p_field, component) \
        LTE_GET_U16(p_field, component.remPort);  \
        LTE_GET_U16(p_field, component.remPortRange);   

#define GTP_GET_TFT_SECURITY_PARAM(p_field, component) \
        LTE_GET_U32(p_field, component.ipsecParamInd)

#define GTP_GET_TFT_SERV_N_CLASS(p_field, component) \
        LTE_GET_U8(p_field, component.srvClass.tos_tc);\
        LTE_GET_U8(p_field, component.srvClass.tos_tc_mask);

#define GTP_GET_TFT_FLOW_LABEL(p_field, component) \
{ U8 buf[4], *pbuf;\
    buf[0] = 0;                                      \
    pbuf = &buf[0];  \
    LTE_GET_NBYTES(p_field, &buf[1], GTP_TFT_FLOW_LABEL_TY_LEN);\
    buf[1] &= GTP_RIGHT_NIBBLE_MASK;                            \
    LTE_GET_U32(pbuf, component.flowLabelType);\
}

/* length of the packet filter contents field  */
#define GTP_SET_TFT_PKT_FLTR_CONTENT_LEN(p_buf, value) \
        LTE_SET_U8(p_buf, value) 

    /* Set flag for presence of filed.*/
    /*  IPv4 remote address type */
#define GTP_SET_TFT_IPV4_REM_ADDR_PRESENT(flags) \
            (flags = (flags | GTP_IPV4_REM_ADDR_PKT_FLTR_MASK))

 /*  IPv6 remote address type */
#define GTP_SET_TFT_IPV6_REM_ADDR_PRESENT(flags) \
            (flags = (flags | GTP_IPV6_REM_ADDR_PKT_FLTR_MASK))

 /*  Protocol identifier/Next header type */
#define GTP_SET_TFT_PROTO_ID_PRESENT(flags) \
            (flags = (flags | GTP_PROTO_ID_PKT_FLTR_MASK))

 /*  Single local port type */
#define GTP_SET_TFT_SNGL_LOC_PORT_TY_PRESENT(flags) \
            (flags = (flags | GTP_SNGL_LOC_PORT_TY_PKT_FLTR_MASK))

 /*  Local port range type */
#define GTP_SET_TFT_LOC_PORT_RNG_TY_PRESENT(flags) \
            (flags = (flags | GTP_LOC_PORT_RNG_TY_PKT_FLTR_MASK))

 /*  Single remote port type */
#define GTP_SET_TFT_SNGL_REM_PORT_TY_PRESENT(flags) \
            (flags = (flags | GTP_SNGL_REM_PORT_TY_PKT_FLTR_MASK))

 /*  Remote port range type */
#define GTP_SET_TFT_REM_PORT_RNG_TY_PRESENT(flags) \
            (flags = (flags | GTP_REM_PORT_RNG_TY_PKT_FLTR_MASK))

 /*  Security parameter index type */
#define GTP_SET_TFT_SECURITY_PARAM_TY_PRESENT(flags) \
            (flags = (flags | GTP_SECURITY_PARAM_TY_PKT_FLTR_MASK))

 /*  Type of service/Traffic class type */
#define GTP_SET_TFT_SERV_N_CLASS_TY_PRESENT(flags) \
            (flags = (flags | GTP_SERV_N_CLASS_TY_PKT_FLTR_MASK))

 /*  Flow label type */
#define GTP_SET_TFT_FLOW_LABEL_TY_PRESENT(flags) \
            (flags = (flags | GTP_FLOW_LABEL_TY_PKT_FLTR_MASK))

#define GTP_FREE_TFT_FILTER(p_module_ctx, p_gtp_tft) {\
    U32  cnt=0; \
    for(cnt = 0; cnt < p_gtp_tft->pkt_filters_count; cnt++){\
        \
        LTE_MEMFREE(p_module_ctx, p_gtp_tft->pkt_filters[cnt]); \
    }\
    p_gtp_tft->pkt_filters_count = GTP_ZERO_VAL;\
}



/*
Packet filter component type identifier
Bits
8 7 6 5 4 3 2 1         
0 0 0 1 0 0 0 0 IPv4 remote address type
0 0 1 0 0 0 0 0 IPv6 remote address type
0 0 1 1 0 0 0 0 Protocol identifier/Next header type
0 1 0 0 0 0 0 0 Single local port type
0 1 0 0 0 0 0 1 Local port range type
0 1 0 1 0 0 0 0 Single remote port type 
0 1 0 1 0 0 0 1 Remote port range type
0 1 1 0 0 0 0 0 Security parameter index type
0 1 1 1 0 0 0 0 Type of service/Traffic class type
1 0 0 0 0 0 0 0 Flow label type

All other values are reserved.


For "IPv4 remote address type", the packet filter component value field shall be
encoded as a sequence of a four octet IPv4 address field and a four octet IPv4
address mask field. The IPv4 address field shall be transmitted first. */

#define AV_PGW_PF_COMP_PROT_TYPE         0x30

/* The packet filter direction is used to indicate, in bits 5 and 6, for what 
   traffic direction the filter applies:

00 - pre Rel-7 TFT filter
01 - downlink only
10 - uplink only
11 - bidirectional
Bits 8 through 7 are spare bits. */

#define AV_PGW_DL_PKT_FILTER             0x01

/* As per the spec 29.274 Maximum digits in IMSI is 15 with 8 octets
   When Odd number of digits in IMSI, the MSB [Bits 5-8] of last octet will
   have filler digit "0x0F" */
#define AV_PGW_IMSI_LEN     15  

/* All EGTP Related defines */

#define AV_PGW_DFLT_REORDER              FALSE

#define AV_PGW_DFLT_EXPSEQNMB            1000

/* Increasing the size to accumalate huge buffers during data plane */
#define AV_PGW_DFLT_MAXTPDUSZ            3000

#define AV_PGW_EGT_REORDER_TYPE_1 1
#define AV_PGW_EGT_REORDER_TYPE_2 2

/* All GTP-U related defines - Check these params */
#define AV_PGW_EGU_DFLT_PDU_NMB         10
#define AV_PGW_EGU_DFLT_SEQ_NMB         10

#define AV_PGW_ROUT_REMOTE_PORT         2152
#define AV_PGW_MAX_WELL_KNOWN_IP_PORT   1234

#define AV_PGW_EGU_DFLT_UDP_PORT_NMB  6001
#define AV_PGW_EGU_DFLT_PDCP_PORT_NMB 6000


/*-- Different memory types --*/
#define AV_PGW_MEMTYPE_EVNT  1
#define AV_PGW_MEMTYPE_MSG   2
#define AV_PGW_MEMTYPE_BUF   3

/*-- Different memory modes --*/
#define AV_PGW_MEMMODE_ALLOC 1
#define AV_PGW_MEMMODE_FREE  2
#define AV_PGW_MEMMODE_SEND  3
#define AV_PGW_MEMMODE_RCV   4

/* Bearer operation */
#define AV_PGW_DEL_BEARER     (1 << 0)
#define AV_PGW_MOD_BEARER     (1 << 1)

#define AV_PGW_BEARER_IND(_ebi)              ((_ebi) - 5)
#define AV_PGW_VALIDATE_EBI(_ebi)            ((_ebi) >= 5 && (_ebi) <= 15)
#define AV_PGW_SET_BEARER_MAP(_bmap, _ebi)   ((_bmap) |= (1 << (_ebi)))
#define AV_PGW_CHK_BEARER_MAP(_bmap, _ebi)   ((_bmap) & (1 << (_ebi)))

#define AV_PGW_SET_MASK(val, mask)                                   \
{                                                                    \
   val = ((val) | (mask));                                           \
}

#define AV_PGW_UNSET_MASK                                            \
{                                                                    \
   val = ((val) & ~(mask));                                          \
}

#define AV_PGW_GET_BEARER(_bTable, _ebi, _bearer)        \
{                                                        \
   _bearer = NULLP;                                      \
   if (AV_PGW_VALIDATE_EBI(_ebi))                             \
   {                                                     \
      _bearer = _bTable[AV_PGW_BEARER_IND(_ebi)];        \
   }                                                     \
}

#define AV_PGW_IPCB_INDX(_ipaddr, _index)                            \
{                                                                    \
   _index = ((_ipaddr) - (avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.      \
            startAddr.u.ipv4NetAddr));                               \
}

#define AV_PGW_GET_IPCB(_ipaddr, _pIpCb)                             \
{                                                                    \
   U32 _index = ((_ipaddr) - (avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.  \
            startAddr.u.ipv4NetAddr));                               \
   _pIpCb = NULLP;                                                   \
   if (_index < avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb)            \
   {                                                                 \
      _pIpCb = &(avPgwCb.ipCbsList[_index]);                         \
   }                                                                 \
}
   

/* Allocation/Dellocation Macros for static buffers in sending task PDN-GW */
#define AV_PDNGW_SENDTSK_ALLOC(_buf, _size)                          \
{                                                                    \
   if (SGetSBuf(avPgwCb.init.region, avPgwCb.init.pool,              \
                (Data **)&(_buf), (Size) _size) == ROK)              \
   {                                                                 \
      AV_PGW_DBG_MEM(_buf, AV_PGW_MEMTYPE_BUF, AV_PGW_MEMMODE_ALLOC, _size);\
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define AV_PDNGW_SENDTSK_FREE(_buf, _size)                           \
{                                                                    \
   AV_PGW_DBG_MEM(_buf, AV_PGW_MEMTYPE_BUF, AV_PGW_MEMMODE_FREE, _size);\
   (Void) SPutSBuf(avPgwCb.init.region, avPgwCb.init.pool,           \
                   (Data *) _buf, (Size) _size);                     \
   (_buf) = NULLP;                                                   \
}


/* Allocation/Dellocation Macros for static buffers receiving task in PDN-GW */
#define AV_PDNGW_RECVTSK_ALLOC(_buf, _size)                          \
{                                                                    \
   if (SGetSBuf(avPgwCb.routeCb.init.region,                        \
                avPgwCb.routeCb.init.pool, (Data **)&(_buf),        \
                (Size) _size) == ROK)                                \
   {                                                                 \
      AV_PGW_DBG_MEM(_buf, AV_PGW_MEMTYPE_BUF, AV_PGW_MEMMODE_ALLOC, _size);\
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define AV_PDNGW_RECVTSK_FREE(_buf, _size)                           \
{                                                                    \
   AV_PGW_DBG_MEM(_buf, AV_PGW_MEMTYPE_BUF, AV_PGW_MEMMODE_FREE, _size);\
   (Void) SPutSBuf(avPgwCb.routeCb.init.region,                     \
         avPgwCb.routeCb.init.pool, (Data *) _buf,                  \
         (Size) _size);                                              \
   (_buf) = NULLP;                                                   \
}

/* TODO: Define this macro */
#define AV_PGW_DBG_MEM(a, b, c, d)

#define AVPGWSM_DFLT_PEER_PORT  5656

#ifndef NO_ERRCLS

#define AV_ERRCLS_CHK(errCls) \
   if((errCls & ERRCLASS))

#define AV_PGW_SND_LOGERROR(errCls, errCode, errVal, errDesc) \
do \
{  \
   AV_ERRCLS_CHK(errCls) \
   { \
   SLogError(avPgwCb.init.ent, avPgwCb.init.inst, avPgwCb.init.procId, \
         (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal,   \
         (Txt *)errDesc); \
   } \
}while(0)

#define AV_PGW_RCV_LOGERROR(errCls, errCode, errVal, errDesc) \
do \
{ \
   AV_ERRCLS_CHK(errCls) \
   { \
   SLogError(avPgwCb.routeCb.init.ent, avPgwCb.routeCb.init.inst,    \
         avPgwCb.routeCb.init.procId,                                \
         (Txt *)__FILE__, __LINE__, errCls, errCode, (ErrVal)errVal, \
         (Txt *)errDesc); \
   } \
}while(0)

#else  /* ERRCLS */

#define AV_PGW_SND_LOGERROR(errCls, errCode, errVal, errDesc) 
#define AV_PGW_RCV_LOGERROR(errCls, errCode, errVal, errDesc) 
#endif  /* ERRCLS */

#ifdef DEBUGP
#define AV_PGW_SND_PRNTBUF   avPgwCb.init.prntBuf
#define AV_PGW_RCV_PRNTBUF   avPgwCb.routeCb.init.prntBuf
#define AV_PGW_LYR_NAME "LTE-CNE-PGW"

/* Debug prints for PGW */
#define AV_PGW_RCV_DBG_INFO(_arg) \
   AV_DBG_INFO(avPgwCb.routeCb.init, AV_PGW_LYR_NAME, LAV_DBGMASK_INFO, _arg)

#define AV_PGW_RCV_DBG_ERROR(_arg) \
   AV_DBG_ERROR(avPgwCb.routeCb.init, AV_PGW_LYR_NAME, LAV_DBGMASK_ERROR, _arg)

#define AV_PGW_SND_DBG_INFO(_arg) \
   AV_DBG_INFO(avPgwCb.init, AV_PGW_LYR_NAME, LAV_DBGMASK_INFO, _arg)

#define AV_PGW_SND_DBG_ERROR(_arg) \
   AV_DBG_ERROR(avPgwCb.init, AV_PGW_LYR_NAME, LAV_DBGMASK_ERROR, _arg)

#ifdef DEBUGP

#define AV_PGW_PRNTBUF   avPgwCb.init.prntBuf

/* macro to print buffer 
*/
#define AVPRNT(_init, _prntBuf)   \
{                          \
   sprintf _prntBuf;      \
   SPrint(_init.prntBuf);            \
}

/* macro to get and print timestamp value */
#ifdef AV_PRNT_TIMESTAMP
#define AV_PRNTTIMESTAMP(_init)                                                \
{                                                                       \
   S8 _buf[60];                                                    \
   (Void) SGetTimeStamp(_buf);                                          \
   AVPRNT(_init, (_init.prntBuf, "[%s]:", _buf));                                        \
}
#else
#define AV_PRNTTIMESTAMP(_init)
#endif

/* macro to print layer Name */
#define AV_PRNT_LAYER(_init, _lyr)                                             \
{                                                                       \
   AVPRNT(_init, (_init.prntBuf, " [%s 0x%x:%x]", _lyr,                           \
         _init.ent, _init.inst));                                \
}

/* macro to print filename, line number and function name */
#define AV_PRNT_FILE_LINE(_init)                                        \
{                                                                       \
   AVPRNT(_init, (_init.prntBuf, " [%s:%d:%s]",__FILE__,__LINE__, __tapa_fun)); \
}

/* macro to collect print all the information */
#define AV_DBG(_init, _lyr, _mask, _arg, _lvl) \
do \
{ \
   if(_init.dbgMask & (_mask)) \
   { \
      AV_PRNTTIMESTAMP(_init)                                     \
      AV_PRNT_LAYER(_init, _lyr)                                    \
      AV_PRNT_FILE_LINE(_init)                                      \
      AVPRNT(_init, (_init.prntBuf, " [" #_lvl "]" ));              \
      AVPRNT(_init, (_init.prntBuf, " : "));                        \
      AVPRNT(_init, _arg);                         \
      AVPRNT(_init, (_init.prntBuf, "\n"));    \
   } \
} while (0)

/* Debug macros for different debug levels */
#define AV_DBG_INFO(_init, _layerName, _mask, _arg)\
   AV_DBG(_init, _layerName, _mask, _arg, "INFO")

#define AV_DBG_ERROR(_init, _layerName, _mask, _arg)\
   AV_DBG(_init, _layerName, _mask, _arg, "ERROR")

/*--------------- Debug prints used under DEBUGP -------------------*/

/* Debug prints for MME */
#define AV_PGW_DBG_INFO(_arg) \
   AV_DBG_INFO(avPgwCb.init, AV_PGW_LYR_NAME, LAV_DBGMASK_INFO, _arg)

#define AV_PGW_DBG_ERROR(_arg) \
   AV_DBG_ERROR(avPgwCb.init, AV_PGW_LYR_NAME, LAV_DBGMASK_ERROR, _arg)

/* Debug prints for General / Utilities */
/* Note: For now the common places, VbMmeCb is used presently.
 * Take care in the process of segregation of code.*/

#define AV_UTL_DBG_INFO(_arg) \
   AV_DBG_INFO(avPgwCb.init, AV_LYR_NAME, LAV_DBGMASK_INFO, _arg)

#define AV_UTL_DBG_ERROR(_arg) \
   AV_DBG_ERROR(avPgwCb.init, AV_LYR_NAME, LAV_DBGMASK_ERROR, _arg)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define AV_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)             \
{                                                                       \
   if (_init.dbgMask & (LAV_DBGMASK_MEM))                            \
   {                                                                    \
      AV_PRNTTIMESTAMP(_init)                                                  \
      AV_PRNT_LAYER(_init, _lyr)                                                   \
      AV_PRNT_FILE_LINE(_init)                                          \
      AVPRNT(_init, (_init.prntBuf, " [ MEM  : %d %d %p %d]\n",                 \
              _memtype, _memmode,(Void*)_ptr, (U32)_size ));             \
   }                                                                    \
}
#else
#define AV_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)                     \
{                                                                       \
   if (_init.dbgMask & (LAV_DBGMASK_MEM))                            \
   {                                                                    \
      AV_PRNTTIMESTAMP(_init)                                           \
      AV_PRNT_LAYER(_init, _lyr)                                               \
      AV_PRNT_FILE_LINE(_init)                                          \
      AVPRNT(_init, (_init.prntBuf, " [ MEM  : %d %d %p %ld]\n",     \
              _memtype, _memmode,(Void*)_ptr, (U32)_size ));             \
   }                                                                    \
}
#endif /*ALIGN_64BIT */

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, VbMmeCb is used presently.
 * Take care in the process of segregation of code.*/
#define AV_TRC2(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init, AV_LYR_NAME)                                 \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                  \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf," Entering\n"));                     \
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define AV_TRC3(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                           \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init, AV_LYR_NAME)                                 \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                                 \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf," Entering\n"));                     \
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define AV_RETVOID                                                      \
do                                                                      \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                           \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init,  AV_LYR_NAME)                                 \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                                 \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define AV_RETVALUE(_arg)                                               \
{                                                                       \
   if (VbMmeCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init,  AV_LYR_NAME)                                 \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                                 \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}

/* Trace for MME Code */

#define AV_PGW_TRC2(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                           \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init,  AV_PGW_LYR_NAME)                                 \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                                 \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf," Entering\n"));                     \
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define AV_PGW_TRC3(_arg)                                                   \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                           \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init,  AV_PGW_LYR_NAME)                             \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                   \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf," Entering\n"));                     \
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define AV_PGW_RETVOID                                                      \
do                                                                      \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                           \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init,  AV_PGW_LYR_NAME)                             \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                                 \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define AV_PGW_RETVALUE(_arg)                                               \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init,  AV_PGW_LYR_NAME)                             \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                                 \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf, " Exiting\n"));                     \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}


#else /* DEBUGP */

/* Layer Name appearing in the Debug prints */
#define AV_PGW_LYR_NAME 
#define AV_LYR_NAME  

/* macro to print buffer */
#define AVPRNT(_prntbuf) 

/* macro to get and print timestamp value */
#define AV_PRNTTIMESTAMP(_init)

/* macro to print layer Name */
#define AV_PRNT_LAYER(_init, _lyr)

/* macro to print filename, line number and function name */
#define AV_PRNT_FILE_LINE(_init) 

/* macro to collect print all the information */
#define AV_DBG(_init, _lyr, _mask, _arg, _lvl)

/* Debug macros for different debug levels */
#define AV_DBG_INFO(_init, _layerName, _mask, _arg)
#define AV_DBG_ERROR(_init, _layerName, _lyr, _arg)

/*--------------- Debug prints used under DEBUGP -------------------*/

/* Debug prints for MME */
#define AV_PGW_DBG_INFO(_arg)
#define AV_PGW_DBG_ERROR(_arg)

/* Debug prints for General / Utilities */
#define AV_UTL_DBG_INFO(_arg)
#define AV_UTL_DBG_ERROR(_arg)

/*-------------------------Memory Debug Prints------------------------*/
#ifdef ALIGN_64BIT
#define AV_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#else
#define AV_DBG_MEM(_init, _lyr, _ptr, _memtype, _memmode, _size)
#endif

/*------------------------- Trace Prints------------------------*/

/* Trace for Utilities / General Code */
/* Note: For now the common places, VbMmeCb is used presently.
 * Take care in the process of segregation of code.*/
#define AV_TRC2(_arg)
#define AV_TRC3(_arg)

#define AV_RETVOID
#define AV_RETVALUE(_arg)

/* Trace for MME Code */

#define AV_PGW_TRC2(_arg)
#define AV_PGW_TRC3(_arg)

#define AV_PGW_RETVOID
#define AV_PGW_RETVALUE(_arg)

#endif /* DEBUGP */
/* Trace for PGW Code */
#define AV_PGW_SND_TRC2(_arg)                                           \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init, AV_PGW_LYR_NAME)                      \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                   \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf," Entering\n"));       \
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define AV_PGW_SND_TRC3(_arg)                                           \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init, AV_PGW_LYR_NAME)                      \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                   \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf," Entering\n"));       \
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define AV_PGW_SND_RETVOID                                              \
do                                                                      \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init, AV_PGW_LYR_NAME)                      \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                   \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf, " Exiting\n"));       \
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define AV_PGW_SND_RETVALUE(_arg)                                       \
{                                                                       \
   if (avPgwCb.init.dbgMask & (LAV_DBGMASK_TRC))                        \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.init)                                    \
      AV_PRNT_LAYER(avPgwCb.init, AV_PGW_LYR_NAME)                      \
      AV_PRNT_FILE_LINE(avPgwCb.init)                                   \
      AVPRNT(avPgwCb.init, (avPgwCb.init.prntBuf, " Exiting\n"));       \
   }                                                                    \
   RETVALUE(_arg);                                                      \
}
#define AV_PGW_RCV_TRC2(_arg)                                           \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.routeCb.init.dbgMask & (LAV_DBGMASK_TRC))                \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.routeCb.init)                            \
      AV_PRNT_LAYER(avPgwCb.routeCb.init, AV_PGW_LYR_NAME)              \
      AV_PRNT_FILE_LINE(avPgwCb.routeCb.init)                           \
      AVPRNT(avPgwCb.routeCb.init, (avPgwCb.routeCb.init.prntBuf," Entering\n"));\
      TRC2(_arg);                                                       \
   }                                                                    \
}

#define AV_PGW_RCV_TRC3(_arg)                                           \
Txt __tapa_fun[PRNTSZE];                                                \
sprintf(__tapa_fun,#_arg);                                              \
{                                                                       \
   if (avPgwCb.routeCb.init.dbgMask & (LAV_DBGMASK_TRC))                \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.routeCb.init)                            \
      AV_PRNT_LAYER(avPgwCb.routeCb.init, AV_PGW_LYR_NAME)              \
      AV_PRNT_FILE_LINE(avPgwCb.routeCb.init)                           \
      AVPRNT(avPgwCb.routeCb.init, (avPgwCb.routeCb.init.prntBuf," Entering\n"));\
      TRC3(_arg);                                                       \
   }                                                                    \
}

#define AV_PGW_RCV_RETVOID                                              \
do                                                                      \
{                                                                       \
   if (avPgwCb.routeCb.init.dbgMask & (LAV_DBGMASK_TRC))                \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.routeCb.init)                            \
      AV_PRNT_LAYER(avPgwCb.routeCb.init, AV_PGW_LYR_NAME)              \
      AV_PRNT_FILE_LINE(avPgwCb.routeCb.init)                           \
      AVPRNT(avPgwCb.routeCb.init, (avPgwCb.routeCb.init.prntBuf, " Exiting\n"));\
   }                                                                    \
   RETVOID;                                                             \
}while(0)

#define AV_PGW_RCV_RETVALUE(_arg)                                       \
{                                                                       \
   if (avPgwCb.routeCb.init.dbgMask & (LAV_DBGMASK_TRC))                \
   {                                                                    \
      AV_PRNTTIMESTAMP(avPgwCb.routeCb.init)                            \
      AV_PRNT_LAYER(avPgwCb.routeCb.init, AV_PGW_LYR_NAME)              \
      AV_PRNT_FILE_LINE(avPgwCb.routeCb.init)                           \
      AVPRNT(avPgwCb.routeCb.init, (avPgwCb.routeCb.init.prntBuf, " Exiting\n"));\
   }                                                                    \
   RETVALUE(_arg);                                                      \
}

#define AV_PGW_FOR_EACH_LST_NODE(_type, _lst, _node, _lnk)              \
   for ((_node) = (_type *)CM_LLIST_FIRST_NODE(_lst, _lnk);             \
      (_node) != NULLP;                                                 \
      (_node) = (_type *)CM_LLIST_NEXT_NODE(_lst, _lnk))                \

#define AVDBGP(_arg1, _arg2)

#else /* DEBUGP */
#define AV_PGW_LYR_NAME

#define AVDBGP(_arg1, _arg2)

/* Debug prints for PGW */
#define AV_PGW_SND_DBG_INFO(_arg)
#define AV_PGW_RCV_DBG_INFO(_arg)

#define AV_PGW_SND_DBG_ERROR(_arg)
#define AV_PGW_RCV_DBG_ERROR(_arg)

/* Trace for PGW Code */
#define AV_PGW_SND_TRC2(_arg)
#define AV_PGW_RCV_TRC2(_arg)

#define AV_PGW_SND_TRC3(_arg)
#define AV_PGW_RCV_TRC3(_arg)

#define AV_PGW_SND_RETVOID RETVOID
#define AV_PGW_RCV_RETVOID RETVOID

#define AV_PGW_SND_RETVALUE(_arg) RETVALUE(_arg)
#define AV_PGW_RCV_RETVALUE(_arg) RETVALUE(_arg)

#endif /* DEBUGP */

#define AV_PGW_SET_IE_LVL_INFO(arg, lvl, type, inst)                    \
{                                                                       \
   arg.level = lvl;                                                     \
   arg.ieInfo[lvl].ieTypePres = TRUE;                                     \
   arg.ieInfo[lvl].ieType = type;                                         \
   arg.ieInfo[lvl].ieInst = inst;                                         \
}

/* TFT operation code (octet)
 * Bits 
 * 8 7 6
 * */
#define AV_PGW_GET_TFT_OPERATION_CODE(byte_info, operation_code)\
        operation_code =  (byte_info >> 5);


/* Validate TFT operation code (octet)
 * Bits  
 * 8 7 6
 * 0 0 0  Spare 0x00
 * 1 1 1  Reserved 0x07
 * */
#define GTP_TFT_OPERATION_CODE_VALID(value) \
         ((value >= GTP_CREATE_NEW_TFT) &&  \
          (value <= GTP_NO_TFT_OPERATION)) 

    /* E bit (bit 5 of octet) */
#define GTP_GET_TFT_E_BIT(byte_info, param_present) \
        param_present = (byte_info & GTP_TFT_E_BIT_MASK); \
        param_present = (GTP_FETCH_LEFT_NIBBLE(param_present));

#ifdef __cplusplus
}
#endif

#endif /* __AV_PGWH__ */

/**********************************************************************

         End of file:     av.h@@/main/1 - Tue Aug 17 18:54:32 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -     sp               1. LTE-CNE:PGW Initial Release.
/main/1      av001.101 rp           1. Added support for TFT
/main/1      av003.101 ve           1. To avoide the packet fragmentation 
                                      AV_PGW_ROUT_MAX_DATA_SIZE value is 
                                      changed to 1504.
*********************************************************************91*/
