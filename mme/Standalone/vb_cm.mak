#/*
# * Copyright 2017-present Open Networking Foundation
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *     http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# */

#*****************************************************************************/

PRDOPTS=-DVB -DSZ_MME -UVB_TEST_CODE -DCM_PASN_ESC_SUPPORT \
			-DCM_PASN_CLASS_IDTYPE -UCM_PASN_DBG -DVB_DEBUG \
        	-DDEBUGP -DVB_USTA -DVB_IMSI_REATTACH \
        	-UEG -DVB_MME -UVB_PERF_MEAS -DLSZV1 -DVB_SB_SCTP \
			-UVB_MME_NW_INIT_DETACH -UTRACE1 -UTRACE2 -UTRACE3 \
			-DVB_MME_AUTH -DVB_MME_NAS_SEC -DCM_NAS_SEC


# For loose coupling
PRDLCOPTS=-DLCSZT -DLCLSZ -DLCSCT -DLCSMSZMILSZ -DLCSZMILSZ -DLCUZLISZT \
			-DLWLCSZT -DLCHIUIHIT -DLCSBLIHIT -DLCHIMILHI -DLCSMHIMILHI \
        	-DLCLHI -DLCSBUISCT -DLCDMUISCT -DLCLVB -DLCVBMILVB -DLCSMMILVB \
		  	-DLCSMVBMILVB

EGOPTS=-DEGTP_C -DEU -DEG_REL_930 -DEG_PHASE2

# -------------- Product options for acceptance test build
PRDACCOPTS=$(PRDOPTS) $(PRDLCOPTS) $(EGOPTS) -DSM -DUZ -DCM_INET2 -DEG \
		-DSZ -DSZ_MME -DSZ_DYNAMIC_PEER -DSZ_USTA -DLCSZLISCT -DLCSZUISZT \
		-DLWLCSZUISZT -DLWLCUZLISZT -DLCSULISCT -DxSS_MULTIPLE_PROCS -DSB \
		-DLSB4 -DLSB8 -DLCSBMILSB -UUE_RAD_CAP -DLCSMSBMILSB -DHI \
		-DHI_MULTI_THREADED -DxHI_CMPTBL_FLAG -DVB -DLCLVB -DLCVBMILVB \
		-DLCSMMILVB -DLCEGMILEG -DLCSMEGMILEG -DLCEGT -DLCEULIEGT -DLCLEG \
		-DCM_MME -DTRACKING_AREA_UPDATE -DLCVBLIHIT

# Product options for library build
PRDLIBOPTS=$(PRDOPTS) $(PRDACCOPTS) $(EGOPTS) -DDBG4 -DCMFILE_REORG_1 \
		-UPTSZUISZT \
		-DCM_INET2 \
          -DSM -DLCSZLISCT -DLCSZUISZT -DUZ -DCM_INET2      \
          -DLWLCSZUISZT -DLWLCUZLISZT -DLCSULISCT \
	  -DxSS_MULTIPLE_PROCS -UDEBUGP -USB_DEBUGP\
          -DCM_PASN_ESC_SUPPORT -DCM_PASN_CLASS_IDTYPE -UCM_PASN_DBG \
          -DSB -DLSB4 -DLSB8 -DLCSBMILSB \
          -DLCSMSBMILSB -DHI -DHI_MULTI_THREADED -DxHI_CMPTBL_FLAG -DEG


#-------------------MOS Product specific option define
MSOPTS=-DMS

# System Services (e.g. MOS, MTSS etc.) Product specific option define
#
MSCOPTS=-DCMFILE_REORG_1 -D__EXTENSIONS__ -DCMFILE_REORG_2

# SECTION 2
# Options for compilation with different OS's
# Default is for Portable build
# For acceptance test build either uncomment specific OS environment
# required or specify on command line

# Gen OS Compile Options
GENOSOPTS= -DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2 \
           -DxSS_SINGLE_THREADED -DLONG_MSG -DxSS_MULTIPLE_PROCS \
           -DSLES9_PLUS

# Options for acceptance test build with MTSS - MOS multithread OS
MTOSOPTS=-DSS_MT -D__EXTENSIONS__ -DDEBUGNOEXIT -USS_M_PROTO_REGION -USS_MULTICORE_SUPPORT -USSI_MEM_CHNG -DSS_MT_TMR -DxCM_MT_HASH_BIN -USS_MEM_LEAK_STS -USS_MEM_LEAK_FREE_TRACE

# Options for library build with MTSS - MOS multithread OS
LIBOSOPTS="$(GENOSOPTS) $(MTOSOPTS)"

# Linux  specific options
LNXOSOPTS=-DSS_LINUX $(MTOSOPTS) -D_GNU_SOURCE

#for Solaris MTSS
OSOPTS=$(GENOSOPTS) $(MTOSOPTS)

#=======================================================================
# SECTION 3
# macro for output file name and makefile name
#
BASENAME=vb_cm

MAKENAME=$(BASENAME).mak

# default acceptance test for Solaris cc environment
BINACC="$(BASENAME)_acc"

# Internal targets for the acceptance test build
MAKEACC=mkacc
ACCBIN=$(BASENAME)_acc

# Internal target for the library build
MAKELIB=mklib
LIBBIN=lib$(BASENAME).a
LIBBIN=$(BASENAME)_lib.lib

# Internal target for makedepend
DEPEND=depend

# Build targets for testing in different environments

# Build with the GNU compiler and linker on Solaris
# Acceptance Test Build
BINGNUACC=gnu_acc
# Portable Build
BINGNUPT=gnu_pt
# libszry Build
BINGNULIB=gnu_lib
# KlocWork Build
BINGNUKW=gnu_kw

# Build with the GNU Compiler and linker on Solaris
# Acceptance Test Build
BINGPPACC=gpp_acc
BINGPPINT=gpp_int
BINGPPPT=gpp_pt
BINGPPLIB=gpp_lib

# Build with the GNU Compiler and linker on Solaris
# Acceptance Test Build
BINCPPACC=cpp_acc
BINCPPINT=cpp_int
# Portable Build
BINCPPPT=cpp_pt
#Libszry Build
BINCPPLIB=cpp_lib

# Build with the Insite Memory Check progszme - Acceptance Tests only
# Acceptance Test Build
BININSITEACC=ins_acc

# Build with the Microtek MRI C compiler on Solaris - portable only
BINMRIPT=mri_pt

# Build with the MRI compiler for vxWorks - 68040 target
BINPPCACC=ppc_acc

# Build  with the GNU compiler and linker on Linux
# Acceptance Test Build
BINLNXACC=lnx_acc
BINLNXINT=lnx_int
# Portable Build
BINLNXPT=lnx_pt
#purcov build
BINLNXPACC=lnx_pacc

#library Build
BINLNXLIB=lnx_lib

# purify
BINPURACC=pur_acc


#=======================================================================
# SECTION 4
# macro for directories and include paths
#
# Treat each of the following as examples and modify to specific product
# requirements

#Directory path
CODEPATH = .
CM_CODE_DIR=$(CODEPATH)
#CM_SS_CODE_DIR=/u/prd/rtr/mt
CM_SS_CODE_DIR=../mt_mlti
#Windows
#CM_SS_CODE_DIR=.
SZ_CODE_DIR=$(CODEPATH)
SAMPLE_CODE_DIR=$(CODEPATH)
SM_CODE_DIR=$(CODEPATH)

# common files directory
CM_CM_DIR=.

# common files directory
CM_SS_DIR=$(CM_SS_CODE_DIR)
CM_HASH_DIR=$(CM_CODE_DIR)
CM_NAS_DIR=$(CM_CODE_DIR)
CM_MEM_DIR=$(CM_CODE_DIR)
CM_TMR_DIR=$(CM_CODE_DIR)
CM_TPT_DIR=$(CM_CODE_DIR)
CM_DNS_DIR=$(CM_CODE_DIR)
CM_ASN_DIR=$(CM_CODE_DIR)
CM_LIB_DIR=$(CM_CODE_DIR)
CM_SS7_DIR=$(CM_CODE_DIR)
CM_SCT_DIR=$(CM_CODE_DIR)
CM_MRS_DIR=$(CM_CODE_DIR)
CM_SHT_DIR=$(CM_CODE_DIR)
CM_HIT_DIR=$(CM_CODE_DIR)
CM_LLIST_DIR=$(CM_CODE_DIR)
CM_INET_DIR=$(CM_CODE_DIR)
CM_SM_DIR=$(SM_CODE_DIR)
CM_OS_DIR=$(CM_CODE_DIR)
#CM_PYTHON_DIR=/usr/ccpu/python/solaris/10/32bit/python-2.4.4/include/python2.4
#PYTHON_PATH_WIN = c:\Python24\include

# S1AP directory
SZ_DIR=$(SZ_CODE_DIR)

# SCTP directory
SB_DIR=$(SZ_CODE_DIR)

# TUCL directory
#HI_DIR=$(SZ_CODE_DIR)/hi/
HI_DIR=.

MT_DIR=.

SS_DIR=.

# OS Directory for Solaris MTSS
OS_DIR=.

# acceptance directory
VB_DIR=.

# acceptance directory
EG_DIR=.

# build directory
SY_DIR=.

# common layer manager interface files directory
CM_LSZ_DIR=$(CODEPATH)

CM_LVB_DIR=$(VB_DIR)

# Upper interface files directory
CM_SZT_DIR=$(CODEPATH)

# SCCP directory
SP_DIR=.

# stack manager directory
SZSM_DIR=$(SAMPLE_CODE_DIR)

# system agent directory
SZSH_DIR=$(SAMPLE_CODE_DIR)

# stack manager directory
EGSM_DIR=$(SAMPLE_CODE_DIR)

# stack manager directory
VBSM_DIR=$(CODEPATH)

# OS directory
#
# MS_DIR=/u/sw/tds/ms/crnt
# NS_DIR=../ns
# VW_DIR=../mt

#SEC_DIR=.
SEC_DIR=./sec
LIB_DIR=../../lib/mme
# include path

# include path

# portable include path
PT_INC_PATH="-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) -I$(CM_TMR_DIR) \
   -I$(CM_ASN_DIR) -I$(CM_SS7_DIR) -I$(CM_RPT_DIR) -I$(CM_SHT_DIR) -I$(CM_SPT_DIR) -I$(SZ_DIR)\
   -I$(SP_DIR) -I$(CM_MEM_DIR) -I$(CM_LLIST_DIR) -I$(CM_LSZ_DIR)"

# library include path
LIB_INC_PATH="-I$(CM_CM_DIR) -I$(SZ_DIR) -I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) \
	-I$(MT_DIR) -I$(CM_DIR) -I$(EG_DIR)"

ACC_INC_PATH="-I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR) -I$(SZ_DIR) \
	-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) -I$(CM_NAS_DIR) \
   -I$(CM_TMR_DIR) -I$(CM_ASN_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) \
	-I$(CM_SHT_DIR) -I$(VB_DIR) -I$(CM_SM_DIR) -I$(SZSM_DIR) -I$(OS_DIR) \
	-I$(HI_DIR) -I$(CM_SS7_DIR) -I$(SB_DIR) -I$(CM_LLIST_DIR) -I$(CM_MEM_DIR) \
	-I$(CM_LSZ_DIR) -I$(CM_PYTHON_DIR) -I$(PYTHON_PATH_WIN) -I$(SEC_DIR)"

#=======================================================================
# SECTION 5
# macros for environments
#

# SUN SPARC Compiler for Solaris----------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with Sun Spro C version 4.0 running
# Solaris 2.4
#
# For ANSI Compilation
#SOLENV="-DSUNOS -DANSI"
SOLENV="-DSUNOS -DANSI -D__EXTENSIONS__"

# For 32 Bit Compilation
SOLCOPTS="-Xa -g"

# For 64 Bit Compilation
# SOLCOPTS="-g -mt -xarch=v9"

# For non-ANSI Compilation
#SOLENV="-DSUNOS"
#SOLCOPTS="-Xa -g"

#------------------ Socket library options
#SOCKLIBS="-lsocket -lnsl -lpthread -lposix4"
SOCKLIBS="-lnsl -lpthread -lpython2.4 "

# For 32 Bit Compilation
#SOLCC="gcc"
SOLCC="cc"
#SOLLD="gcc"
SOLLD="cc"
SOLLIBLD="/usr/ccs/bin/ld"
SOLLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lnsl -lpython2.4 -lsocket -lm -ldl"

#for KSCTP
#SOLLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lnsl -lpython2.4 -lsctp"

# For 64 bit compilation
#SOLCC="/opt/SUNWspro/bin/cc -DALIGN_64BIT -DBIT_64"
#SOLLD="/opt/SUNWspro/bin/cc"

#SOLCC="cc -DALIGN_64BIT -DBIT_64"
#SOLLD="cc"
#SOLLIBLD="/usr/ccs/bin/ld"
#SOLLOPTS="-o $(OUTNAME) -lpthread -lposix4 -g -mt -xarch=v9 -lnsl -lsocket"

#SOLLIBS=$(SOCKLIBS)
#SOLLIBS="-lnsl -lsocket -lpthread -lposix4 -lpython2.4 -lm -ldl"
SOLLIBS=""
SOLLINT="lint"
SOLLNTOPTS="-c -F -p -v"

# for library compilation
SOLLIBOPTS="-r -o $(OUTNAME)"

# ccppc for Solaris
# These flags define the environment required to build an image on a Sun/SPARC
# workstation with ccppc v. 2.7.2 running Solaris 2.5.1
#
# For ANSI Compilation
PPCENV=-DPCORE -DCPU=PPC604 -DANSI -DCCPPC
PPCCOPTS=-mcpu=604 -msoft-float -fvolatile -fno-builtin -Wall \
              -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
              -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

PPCCC=/tools/wind/host/sun4-solaris2/bin/ccppc
PPCLD="ldppc"
#PPCLOPTS=-r -Map $(OUTNAME).map -o $(OUTNAME)
PPCLOPTS=-r -e vwMain -Map $(OUTNAME).map -o $(OUTNAME)
PPCLIBS=
PPCLINT=
PPCLNTOPTS=

# For non-ANSI Compilation
#PPCENV=-DCPU=PPC860 -DCCPPC -UANSI
#PPCCOPTS=-mcpu=860 -g3 -msoft-float -fvolatile -fno-builtin -Wall \
#         -Wno-comment -pipe -nostdinc -Wshadow -Wredundant-decls\
#         -Wcast-qual

PPCCC=/tools/wind/host/sun4-solaris2/bin/ccppc

# GNU gcc compiler for SPARC--------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#

# ---------------- For ANSI Compilation
GNUENV="-DSUNOS -DANSI -DSOLARIS -DSPARC -DPROC_SPARC -DUNIX -D__EXTENSIONS__"
GNUCOPTS=" -O1 -g3 -Wimplicit -Wunused -Wcomment -Wchar-subscripts \
-Wuninitialized -Wparentheses -Wformat -Winline -Wreturn-type -fPIC \
-Wmissing-prototypes -pedantic"


# ---------------- For non-ANSI Compilation
#GNUENV="-DSUNOS"
#GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual "


GNULD="gcc"
GNUCC="gcc"
#GNULIBLD="/usr/ccs/bin/ld"
#GNULIBLD="ld"
GNULIBLD="/usr/ccs/bin/ld"

#--------------------- GNU g++ compiler for SPARC
#GNUCC="g++"
#GNULD="g++"
#GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wcast-qual -Wstrict-prototypes \
#          -Wmissing-prototypes"


GNULOPTS="-o $(OUTNAME) -lpthread -lposix4 -lnsl -lsocket -lpython2.4 -lm -ldl -lgcc"
#GNULIBS="-lnsl -lsocket -lpthread -lposix4 -lpython2.4 -lm -ldl -lgcc"
GNULIBS=""
#GNULIBS=$(SOCKLIBS)
GNULINT="lint"
GNULNTOPTS="-c -F -p -v"

# for library compilation
GNULIBOPTS="-r -o $(OUTNAME)"

#----------------------------------------- c++ compiler
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
CPPENV="-DSUNOS -DANSI "
CPPCC=c++
CPPLD=c++
#change for LIB complaion. verify the path od ld.
CPPLIBLD=/usr/ccpu/suntools/bin/ld
CPPCOPTS="-g -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual  \
	  -Wredundant-decls"
CPPLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lsocket -lnsl -lpython2.4 -lm -ldl"
CPPLIBOPTS="-r -o $(OUTNAME)"
CPPLIBS=""
CPPLINT="lint"
CPPLNTOPTS="-c -F -p -v"

#----------------------------------------- g++ compiler
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
GPPENV="-DSUNOS -DANSI "
GPPCC=g++
GPPLD=g++
GPPLIBLD=/usr/ccpu/suntools/bin/ld
GPPCOPTS="-g -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual  \
	     -Wredundant-decls"
GPPLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lpython2.4 -lsocket -lnsl -lm -ldl"
GPPLIBOPTS="-r -o $(OUTNAME)"
GPPLIBS=""
GPPLINT="lint"
GPPLNTOPTS="-c -F -p -v"

# GNU gcc compiler for Linux------------------------------------------
#
# These  flags define the environment required to build an image  on
# a Linux workstation with GNU C version 2.5.7 (or greater)
#
# For ANSI Compilation
#LNXENV="-DSUNOS -DANSI"
LNXENV="-DSUNOS -DSS_LINUX -DANSI -D_GNU_SOURCE -DUSE_PURE -DPURE"
#LNXENV="-DSUNOS   -DSS_LINUX "

ifeq ($(MACHINE), BIT64)
LNXCOPTS=" -O3 -march=core2 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment \
	-pipe -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
	-Wuninitialized  -Wunused "
else
LNXCOPTS=" -O3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused "
endif

# For non-ANSI Compilation
ifeq ($(MACHINE), BIT64)
LNXCC="gcc -m64 "
LNXLD="gcc  "
else
LNXCC="gcc -m32 "
LNXLD="gcc -m32 "
endif

LNXLIBLD="ld"

#LNXLIBLD="gcc -m32"
PLNXCC="purecov gcc -m32"
PLNXLD="purecov gcc -m32"

#LNXCC="mips64-octeon-linux-gnu-gcc -DALIGN_64BIT -DBIT_64"
#LNXLD="mips64-octeon-linux-gnu-gcc"
#LNXLIBLD="mips64-octeon-linux-gnu-ar"

#PURIFY
PURCC="purify --cache-dir=/tmp gcc -m32"
PURLD="purify --cache-dir=/tmp gcc -m32"
PURLIB="purify --cache-dir=/tmp gcc -m32"

#INSURE
# For ANSI Compilation
INSENV="-DSUNOS -DSS_LINUX -DANSI -DUSE_PURE -DPURE"
INSCOPTS="-g3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused"

# For non-ANSI Compilation
#INSENV="-DSUNOS -DSS_LINUX -DPURE"
#INSCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
          -Wcast-align -Wcast-qual"


INSCC="/usr/local/insure71/bin/insure -m32"
INSLD="/usr/local/insure71/bin/insure -m32"
INSLOPTS="-o $(OUTNAME)"
INSLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lpython2.4 -lm -lrt -lutil -ldl"
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

LNXLOPTS="-o $(OUTNAME)"
LNXLIBOPTS="-r -o $(OUTNAME) "
LNXLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lpython2.4 -lm -lrt -lutil -ldl"
#for KSCTP
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"

# Actual Build Environment----------------------------------------------
#
#
# this defaults to 'o' for most UNIX based systems
# this can be set to 'obj' for DOS/Windows/NT platforms
#
SOL_OBJ_SUFFIX=o
WIN_OBJ_SUFFIX=obj
UNIX_OBJ_SUFFIX=o
MS_OBJ_SUFFIX=obj
OBJ=$(SOL_OBJ_SUFFIX)
#OBJ=$(WIN_OBJ_SUFFIX)

#=======================================================================
#
# macros for compile time options
#

# portable options
CPTOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS)

# acceptance test options - OSOPTS should be set to required environment
COSOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS)
CSMOSOPTS=-DSM $(COSOPTS)

# Combined environment, OS, compiler options and include path
CENVOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS)

# product options for library build
CPRDLIBOPTS="$(CENVOPTS) $(PRDLIBOPTS)"

# product options for acceptance test build
CPRDACCOPTS="$(CENVOPTS) $(PRDACCOPTS)"

# Common Stack Manager File Options for portable build
CSMPTOPTS="$(CENVOPTS) $(PRDOPTS)"

# Common Stack Manager File Options for library build
CSMLIBOPTS="$(CENVOPTS) $(PRDOPTS)"

SMOPTS=-DSM
# Common Stack Manager File Options for acceptance test build
CSMACCOPTS="$(CENVOPTS) $(PRDOPTS) $(SMOPTS)"

# MOS options - for use with the MOS/MTSS files in test environment
#=======================================================================
#
# macros for depend and lint options
#

# depend acceptance test
DEPOPTS_ACC="$(PRDACCOPTS)"

#=======================================================================
#
# macros for hardcoded dependencies
#

# environment
ENV_INC=$(CM_CM_DIR)/envdep.h $(CM_CM_DIR)/envind.h $(CM_CM_DIR)/envopt.h
#ENV_INC=

# system service interface
SSI_INC=$(CM_CM_DIR)/ssi.h $(CM_CM_DIR)/ssi.x
#SSI_INC=

# geneszl
GEN_INC=$(CM_CM_DIR)/gen.h $(CM_CM_DIR)/gen.x
#$(CM_DIR)/cm_gen.h $(CM_DIR)/cm_gen.x
#GEN_INC=

# all system includes
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)
#SYS_INC=

# Product layer management - LSZ includes
LSZ_INC=$(CM_LSZ_DIR)/lsz.h $(CM_LSZ_DIR)/lsz.x
#LSI_INC=

# Product Upper Interface - SZT includes
SZT_INC=$(CM_SZT_DIR)/szt.h $(CM_SZT_DIR)/szt.x $(CM_SZT_DIR)/szt_asn.h $(CM_SZT_DIR)/szt_asn.x

# Product lower Interface - SCT includes
SCT_INC= $(CM_SCT_DIR)/sct.h $(CM_SCT_DIR)/sct.x

# common function library includes
CM_INC = \
   $(CM_HASH_DIR)/cm_hash.h      \
   $(CM_HASH_DIR)/cm_hash.x      \
   $(CM_TMR_DIR)/cm5.h           \
   $(CM_TMR_DIR)/cm5.x           \
   $(CM_MEM_DIR)/cm_mem.h        \
   $(CM_MEM_DIR)/cm_mem.x        \
   $(CM_MEM_DIR)/cm_mblk.h       \
   $(CM_MEM_DIR)/cm_mblk.x       \
   $(CM_LIB_DIR)/cm_lib.x        \
   $(CM_LLIST_DIR)/cm_llist.h    \
   $(CM_LLIST_DIR)/cm_llist.x    \
   $(CM_CM_DIR)/cm_tkns.h        \
   $(CM_CM_DIR)/cm_tkns.x        \
   $(CM_CM_DIR)/cm_err.h

# message functions
MF_INC = \
   $(CM_ASN_DIR)/cm_pasn.h        \
   $(CM_ASN_DIR)/cm_pasn.x


# Database functions
#DB_INC=

# Product - S1AP includes
#SZ_INC=
SZ_INC=$(SZ_DIR)/sz.h $(SZ_DIR)/sz.x $(SZ_DIR)/sz_err.h

SB_INC=$(SB_DIR)/sb.h $(SB_DIR)/sb.x $(SB_DIR)/sb_mtu.h $(SB_DIR)/sb_mtu.x

HI_INC=$(HI_DIR)/hi.h $(HI_DIR)/hi.x

# ALL product includes
SZALL_INC=$(LSZ_INC) $(SZT_INC) $(SZ_INC) $(SB_INC) $(HI_INC)

# OS specific includes - SS includes
#SS_INC=
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h \
	$(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x    \
	$(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x $(SS_DIR)/ss_msg.h     \
	$(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x \
	$(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x $(SS_DIR)/ss_task.h  \
	$(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x \

#MT_INC=
MT_INC=$(MT_DIR)/mt_ss.h $(MT_DIR)/mt_ss.x $(MT_DIR)/mt_err.h


#=======================================================================
# SECTION 7
# macros for source and object files
VBCM_SRCS= \
	$(CM_NAS_DIR)/cm_esm_edm.c   \
	$(CM_NAS_DIR)/cm_emm_esm.c   \
	$(CM_NAS_DIR)/cm_emm_edm.c   \
	$(CM_OS_DIR)/cm_os.c

# common objects--------------------------------------------------------
VBCM_OBJS= \
	$(SY_DIR)/cm_esm_edm.$(OBJ)   \
	$(SY_DIR)/cm_emm_edm.$(OBJ)   \
	$(SY_DIR)/cm_emm_esm.$(OBJ)   \
   $(SY_DIR)/cm_os.$(OBJ)

#----------------------------------------- SS Sources
SS_SRCS= \
   $(SS_DIR)/ss_gen.c              \
   $(SS_DIR)/ss_task.c             \
   $(SS_DIR)/ss_drvr.c             \
   $(SS_DIR)/ss_timer.c            \
   $(SS_DIR)/ss_mem.c              \
   $(SS_DIR)/ss_strm.c             \
   $(SS_DIR)/ss_msg.c              \
   $(SS_DIR)/ss_queue.c            \
   $(SS_DIR)/ss_pack.c             \
   $(SS_DIR)/ss_rtr.c

#----------------------------------------- SS Objects
SS_OBJS= \
   $(SY_DIR)/ss_gen.$(OBJ)         \
   $(SY_DIR)/ss_task.$(OBJ)        \
   $(SY_DIR)/ss_drvr.$(OBJ)        \
   $(SY_DIR)/ss_timer.$(OBJ)       \
   $(SY_DIR)/ss_mem.$(OBJ)         \
   $(SY_DIR)/ss_strm.$(OBJ)        \
   $(SY_DIR)/ss_msg.$(OBJ)         \
   $(SY_DIR)/ss_queue.$(OBJ)       \
   $(SY_DIR)/ss_pack.$(OBJ)        \
   $(SY_DIR)/ss_rtr.$(OBJ)

#------------------------------------------ MT sources
MT_SRCS= \
   $(OS_DIR)/mt_ss.c               \
   $(OS_DIR)/mt_id.c               \
   $(CM_MEM_DIR)/cm_mem.c

#------------------------------------------- MT objects
MT_OBJS= \
   $(SY_DIR)/mt_ss.$(OBJ)          \
   $(SY_DIR)/mt_id.$(OBJ)          \
   $(SY_DIR)/cm_mem.$(OBJ)

# common source(C header files)-----------------------------------------
SZCMH_SRCS= \
	cm_hash.h \
	cm_hash.x \
	cm_lib.x  \
	cm5.h     \
	cm5.x     \
	cm_err.h  \
	envdep.h  \
	envind.h  \
	gen.h     \
	gen.x     \
	lsz.h     \
	lsz.x     \
	lsb.h     \
	lsb.x     \
	lhi.h     \
	lhi.x     \
	sct.x     \
	sct.h     \
	hit.x     \
	hit.h     \
	szt.x     \
	szt_pk.x     \
	szt_unpk.x     \
	szt_asn.x     \
	szt.h     \
	szt_asn.h     \
	ssi.h     \
	ssi.x

#for Solaris MTSS
OS_SRCS=$(SS_SRCS) $(MT_SRCS)
OS_OBJS=$(SS_OBJS) $(MT_OBJS)

# all acceptance test source--------------------------------------------
PRDACC_SRCS= \
	$(VBCM_SRCS) \
	$(OS_OBJS)

# all acceptance test objects-------------------------------------------
PRDACC_OBJS= \
	$(VBCM_OBJS) \
	$(OS_OBJS)

PRDLIB_OBJS= \
	$(VBCM_OBJS) \
	$(OS_OBJS)

ifeq ($(MACHINE), BIT64)
# default library for 64 linux cc environment
BINLIB=$(LIB_DIR)/libvb_lnx64vbcm.a
else
# default library for 32 bit linux cc environment
BINLIB=$(LIB_DIR)/libvb_lnx32vbcm.a
endif

#=======================================================================
#
# special progszms required for Internal Development by makefile
#
# adjust as required
#
# System Binary Dir - replace with appropriate directory and progszme
# for system
BIN=/bin
MAKEDEPEND=makedepend
REMOVE=$(BIN)/rm -f
DELETE=del

#=======================================================================
#
# build targets
#

# clean-----------------------------------------------------------------
#
# cleans up everything...
#
clean:
	$(REMOVE) *.$(OBJ) $(BINACC) *.ln *.s $(SY_DIR)/*.$(OBJ) \
      $(SY_DIR)/$(BINACC) $(SY_DIR)/*.ln $(SY_DIR)/*.s

# External target for acceptance test build
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC)

# External target for libs build
lib:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINLIB)  \
   OSOPTS=$(LIBOSOPTS) INC_PATH=$(ACC_INC_PATH)

#
# Linux  GNU acceptance test build-------------------------------------
$(BINLNXACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

# Solaris GNU acceptance test build-------------------------------------
#
$(BINGNUACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

$(PTBIN): $(PRDPT_OBJS)
	$(LD) $(LOPTS) $(PRDPT_OBJS) $(LIBS)

$(LIBBIN): $(PRDLIB_OBJS)
	$(LD) $(LOPTS) $(PRDLIB_OBJS)


#
# Linux GNU library build-------------------------------------
#
$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(LNXCC) \
    LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBOPTS) LIBS=$(LNXLIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)

# acceptance test-------------------------------------------------------
# SECTION 9
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEACC):
	@$(MAKE) -f $(MAKENAME) $(ACCBIN) CPRDOPTS=$(CPRDACCOPTS) \
	CSMOPTS=$(CSMACCOPTS)

$(MAKEINT):
	@$(MAKE) -f $(MAKENAME) $(INTBIN) CPRDOPTS=$(CPRDINTOPTS) \
	CSMOPTS=$(CSMACCOPTS)

$(ACCBIN): $(PRDACC_OBJS)
	$(LD)  $(PRDACC_OBJS) $(LIBS) $(LOPTS)

$(INTBIN): $(PRDINT_OBJS)
	$(LD)  $(PRDINT_OBJS) $(LIBS) $(LOPTS)

$(MAKELIB):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN) CPRDOPTS=$(CPRDACCOPTS) \
    CSMOPTS=$(CSMLIBOPTS)


$(SY_DIR)/cm_emm_edm.$(OBJ): $(CM_NAS_DIR)/cm_emm_edm.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_NAS_DIR)/cm_emm_edm.c -o $(SY_DIR)/cm_emm_edm.$(OBJ)

$(SY_DIR)/cm_esm_edm.$(OBJ): $(CM_NAS_DIR)/cm_esm_edm.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_NAS_DIR)/cm_esm_edm.c -o $(SY_DIR)/cm_esm_edm.$(OBJ)

$(SY_DIR)/cm_emm_esm.$(OBJ): $(CM_NAS_DIR)/cm_emm_esm.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_NAS_DIR)/cm_emm_esm.c -o $(SY_DIR)/cm_emm_esm.$(OBJ)

$(SY_DIR)/cm_os.$(OBJ): $(CM_OS_DIR)/cm_os.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_OS_DIR)/cm_os.c -o $(SY_DIR)/cm_os.$(OBJ)


# System services--------------------------------------------------

$(SY_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_gen.c -o $(SY_DIR)/ss_gen.$(OBJ)

$(SY_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_task.c -o $(SY_DIR)/ss_task.$(OBJ)

$(SY_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_drvr.c -o $(SY_DIR)/ss_drvr.$(OBJ)

$(SY_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_timer.c -o $(SY_DIR)/ss_timer.$(OBJ)

$(SY_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_mem.c -o $(SY_DIR)/ss_mem.$(OBJ)

$(SY_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_strm.c -o $(SY_DIR)/ss_strm.$(OBJ)

$(SY_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_msg.c -o $(SY_DIR)/ss_msg.$(OBJ)

$(SY_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_queue.c -o $(SY_DIR)/ss_queue.$(OBJ)

$(SY_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_pack.c -o $(SY_DIR)/ss_pack.$(OBJ)

$(SY_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_rtr.c -o $(SY_DIR)/ss_rtr.$(OBJ)


# mtss Solaris -------------------------------------------------------

$(SY_DIR)/cm_mem.$(OBJ): $(CM_MEM_DIR)/cm_mem.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(CM_MEM_DIR)/cm_mem.c -o $(SY_DIR)/cm_mem.$(OBJ)

$(SY_DIR)/mt_ss.$(OBJ): $(OS_DIR)/mt_ss.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/mt_ss.c -o $(SY_DIR)/mt_ss.$(OBJ)

$(SY_DIR)/mt_id.$(OBJ): $(OS_DIR)/mt_id.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/mt_id.c -o $(SY_DIR)/mt_id.$(OBJ)
