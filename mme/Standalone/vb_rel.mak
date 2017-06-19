#/*
# * Copyright 2017-present Open Networking Laboratory
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
#********************************************************************20**
#
#       Name:   LTE CNE makefile
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link S1AP software for
#               various configusztions. Further comments are
#               embedded in the file.
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Solaris 2.xwith SUNpro C(cc)
#
#               ENVIRONMENTS:
#
#               Solaris 2.x with SUNpro C(cc)
#
#               Solaris 2.x with GNU C (gcc)
#
#               Solaris 2.x with MRI C (mcc68k) cross compiled for
#               a relatively standard 68040 processor board
#
#               Microsoft Visual C++ 4.x for Windows '95 and Windows NT
#               environments(msvc)
#
#               BUILD TARGETS:
#
#               pt               portable software (without MOS) -- default
#               acc              acceptance tests (with MOS)
#
#               depend.acc       makedepend for acceptance tests
#               depend.pt        makedepend for portable software
#
#               clean            remove all object files
#               win_clean        remove all object files(Windows environment)
#
#
#
#
#       Sid:      vb.mak@@/main/1 - Mon Jun 28 19:35:27 2010
#
#       Prg:     ap
#
#********************************************************************21*/
#=======================================================================
#
# macros for product specific options

# commented for RTR

# For Product Flags
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


# -------------- Product options for acceptance test build
PRDACCOPTS=$(PRDOPTS) $(PRDLCOPTS) $(EGOPTS) -DSM -DUZ -DCM_INET2 -DEG \
		-DSZ -DSZ_MME -DSZ_DYNAMIC_PEER -DSZ_USTA -DLCSZLISCT -DLCSZUISZT \
		-DLWLCSZUISZT -DLWLCUZLISZT -DLCSULISCT -DxSS_MULTIPLE_PROCS -DSB \
		-DLSB4 -DLSB8 -DLCSBMILSB -UUE_RAD_CAP -DLCSMSBMILSB -DHI \
		-DHI_MULTI_THREADED -DxHI_CMPTBL_FLAG -DVB -DLCLVB -DLCVBMILVB \
		-DLCSMMILVB -DLCEGMILEG -DLCSMEGMILEG -DLCEGT -DLCEULIEGT -DLCLEG \
		-DCM_MME -DTRACKING_AREA_UPDATE -DLCVBLIHIT

#
# -------------- CSB OPTS for acceptance test build
CSBOPTS=-DSB -DLSB4 -DLSB8 -DHI_REL_1_4 -DCM_INET2 -DLCSCT -DHI -DLCSBUISCT -DSB_CHECKSUM -DSB_CHECKSUM_CRC
#
# -------------- HI OPTS for acceptance test build
CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DSB -DHI_MULTI_THREADED

#for KSCTP
#CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DSB -DHI_MULTI_THREADED -DLCHIUISCT -DPTHIUISCT

# Product options for portable build
PRDPTOPTS=$(PRDOPTS) $(EGOPTS) $(CSBOPTS) $(CHIOPTS) -DSM \
          -DCMFILE_REORG_1 -DPTSZUISZT -DCM_INET2 \
          -USZ -DUZ -DEG_PHASE2 -DVB_PT

#PRDPTOPTS=$(PRDOPTS) $(EGOPTS) -DCMFILE_REORG_1 -DPTSZUISZT -DCM_INET2

# Product options for library build
PRDLIBOPTS=$(PRDOPTS) $(EGOPTS) -DCM_MME -DDBG4 -DCMFILE_REORG_1 -DPTSZUISZT -DCM_INET2

#-------------------MOS Product specific option define
MSOPTS=-DMS

# System Services (e.g. MOS, MTSS etc.) Product specific option define
#
MSCOPTS=-DCMFILE_REORG_1 -D__EXTENSIONS__ -DCMFILE_REORG_2

EGOPTS=-DEGTP_C -DEU -DEG_REL_930 -DEG_PHASE2

#=======================================================================
# SECTION 2
# Options for compilation with different OS's
# Default is for Portable build
# For acceptance test build either uncomment specific OS environment
# required or specify on command line

# Gen OS Compile Options
GENOSOPTS= -DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2 \
           -DxSS_SINGLE_THREADED -DLONG_MSG -DxSS_MULTIPLE_PROCS \
           -DSLES9_PLUS

# Portable build options
PTOSOPTS="-DPORTVER"

# Options for acceptance test build with MOS OS
MSOSOPTS="-DMSVER2"

# Options for acceptance test build with PSOS OS
PSOSOPTS="-DPS"

# Options for acceptance test build with VrTx OS
VXOSOPTS="-DVX"

# Options for acceptance test build with VxWorks OS
VWOSOPTS="-DSS_VW"

# Options for acceptance test build with WINNT OS kernel build
WINOSOPTS="-DWINNT"

# Options for acceptance test build with MTSS - MOS multithread OS
MTOSOPTS=-DSS_MT -D__EXTENSIONS__ -DDEBUGNOEXIT -USS_M_PROTO_REGION -USS_MULTICORE_SUPPORT -USSI_MEM_CHNG -DSS_MT_TMR -DxCM_MT_HASH_BIN -USS_MEM_LEAK_STS -USS_MEM_LEAK_FREE_TRACE

# Options for library build with MTSS - MOS multithread OS
LIBOSOPTS="-DSS_MT -DSS  -DxSS_SINGLE_THREADED"

# Linux  specific options
#LNXOSOPTS=-D_GNU_SOURCE -DSS_LINUX -DQCACC_PRETTY_PRINT $(MTOSOPTS)
LNXOSOPTS=-DSS_LINUX $(MTOSOPTS) -D_GNU_SOURCE

# Options for acceptance test build with MTSS - WINNT
NSOSOPTS=-DNS

# Set OS options to include the OS build being used
#portable build
#OSOPTS=$(PTOSOPTS)

#for MOS
#OSOPTS=$(GENOSOPTS) $(MSOSOPTS)

#for VxWorks
#OSOPTS=$(GENOSOPTS) $(VWOSOPTS)

#for Solaris MTSS
OSOPTS=$(GENOSOPTS) $(MTOSOPTS)

#for Windows NT MTSS
#OSOPTS=$(GENOSOPTS) $(NSOSOPTS)

#=======================================================================
# SECTION 3
# macro for output file name and makefile name
#
BASENAME=vb_rel

MAKENAME=$(BASENAME).mak

# default acceptance test for Solaris cc environment
BINACC="vb_acc"

# default portable for Solaris cc environment
BINPT="$(BASENAME)_pt"

# default portable build for KlocWork
BINKW="$(BASENAME)_kw"

# default library for Solaris cc environment
BINLIB="$(BASENAME)_lib.lib"

# Internal targets for the acceptance test build
MAKEACC=mkacc
ACCBIN=$(BASENAME)_acc

# Internal target for the portable build
MAKEPT=mkpt
PTBIN=$(BASENAME)_pt

# Internal target for the kw build
MAKEKW=mkkw
KWBIN=$(BASENAME)_kw

# Internal target for the library build
MAKELIB=mklib
LIBBIN=$(BASENAME)_lib.lib

# Internal target for makedepend
DEPEND=depend

# Build targets for testing in different environments

# Build with the Solaris CC compiler and linker
# Acceptance Test Build
BINSOLACC=sol_acc
# Portable Build
BINSOLPT=sol_pt

# Libszry Build
BINSOLLIB=sol_lib

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

# Build with the Visual C++ on Windows NT or Windows 95(user space only)
# Acceptance Test Build
BINWINACC=win_acc
# Portable Build
BINWINPT=win_pt

# Libszry Build
BINWINLIB=win_lib

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
CM_SS_CODE_DIR=.
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

SEC_DIR=./sec
LIB_DIR=.
# include path

# portable include path
#PT_INC_PATH="-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) -I$(CM_NAS_DIR) -I$(CM_TMR_DIR) \
   -I$(CM_ASN_DIR) -I$(CM_SS7_DIR) -I$(CM_RPT_DIR) -I$(CM_SHT_DIR) -I$(CM_SPT_DIR) -I$(SZ_DIR)\
   -I$(SP_DIR) -I$(CM_MEM_DIR) -I$(CM_LLIST_DIR) -I$(CM_LSZ_DIR) -I$(SY_DIR) -I$(CM_DIR) \
   -I$(SS_DIR) -I$(MT_DIR) -I$(VB_DIR)"

# library include path
LIB_INC_PATH="-I$(SZ_DIR) -I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR)"

# Acceptance test include path
# ACC_INC_PATH=" -I/tools/wind/target/h -I$(SZ_DIR) -I$(CM_DIR) -I$(VB_DIR) \
#              -I$(CM_SM_DIR) -I$(SZSM_DIR) -I$(OS_DIR) -I$(SZL5_DIR) -I$(HI_DIR)"
#
ACC_INC_PATH="-I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR) -I$(SZ_DIR) -I$(CM_CM_DIR) -I$(CM_LIB_DIR) \
              -I$(CM_HASH_DIR) -I$(CM_NAS_DIR) \
   -I$(CM_TMR_DIR) -I$(CM_ASN_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) -I$(CM_SHT_DIR) \
   -I$(VB_DIR) -I$(CM_SM_DIR) -I$(SZSM_DIR) -I$(OS_DIR) -I$(HI_DIR) \
   -I$(CM_SS7_DIR) -I$(SB_DIR) -I$(CM_LLIST_DIR) -I$(CM_MEM_DIR) -I$(CM_LSZ_DIR)\
   -I$(CM_PYTHON_DIR) -I$(PYTHON_PATH_WIN) -I$(SEC_DIR)"

#TODO:AD: check this
#PT_INC_PATH="-I$(ACC_INC_PATH)"

PT_INC_PATH="-I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR) -I$(SZ_DIR) -I$(CM_CM_DIR) -I$(CM_LIB_DIR) \
              -I$(CM_HASH_DIR) -I$(CM_NAS_DIR) \
   -I$(CM_TMR_DIR) -I$(CM_ASN_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) -I$(CM_SHT_DIR) \
   -I$(VB_DIR) -I$(CM_SM_DIR) -I$(SZSM_DIR) -I$(OS_DIR) -I$(HI_DIR) \
   -I$(CM_SS7_DIR) -I$(SB_DIR) -I$(CM_LLIST_DIR) -I$(CM_MEM_DIR) -I$(CM_LSZ_DIR)\
   -I$(CM_PYTHON_DIR) -I$(PYTHON_PATH_WIN)"


# OS - files include path
MSINC_PATH=-I$(CM_DIR) -I$(OS_DIR)

# library path - used for integszted products and simulation env.
LIB_PATH=/tmp

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

# GNU gcc compiler for SPARC--------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#

# ---------------- For ANSI Compilation
#GNUENV="-DSUNOS -DANSI"
GNUENV="-DSUNOS -DANSI -DSOLARIS -DSPARC -DPROC_SPARC -DUNIX -D__EXTENSIONS__"
#GNUENV="-DSUNOS -DANSI -std=c99 -D_XPG6"

#GNUCOPTS="-g3 -Wall -pedantic -pedantic-errors -Wno-comment -pipe -Wshadow \
#          -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes "
GNUCOPTS=" -g3 -Wimplicit -Wunused -Wcomment -Wchar-subscripts \
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
CPPCOPTS=" -g3 -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual  \
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
GPPCOPTS=" -g3 -Wall -fno-strict-aliasing -Wno-comment -pipe -Wcast-qual  \
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
LNXENV="-DSUNOS -DSS_LINUX -DANSI -D_GNU_SOURCE -DUSE_PURE -DPURE"

ifeq ($(MACHINE), BIT64)
LNXCOPTS=" -g3 -march=core2 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment \
	-pipe -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
	-Wuninitialized  -Wunused "
else
LNXCOPTS=" -g3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused"
endif

ifeq ($(MACHINE), BIT64)
LNXCC="gcc -m64 "
LNXLD="gcc "
LNXLIBLD="gcc "
else
LNXCC="gcc -m32 "
LNXLD="gcc -m32"
LNXLIBLD="gcc -m32"
endif

PLNXCC="purecov gcc -m32"
PLNXLD="purecov gcc -m32"

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
INSLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lpython2.4 -lm -lrt -lutil -ldl -lvb_sgw -lvb_pgw"
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

LNXLOPTS="-o $(OUTNAME)"
LNXLIBOPTS="-r -o $(OUTNAME) "
LNXLIBS="-Xlinker -export-dynamic -L/usr/lib/nptl -lpthread  -lnsl -lm -lrt -lutil -ldl -L. "
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"

# MRI mcc68k compiler for 68040-----------------------------------------
#
# These flags define the environment required to build an image for
# a Motorola MVME162 Board (68040) with Microtec Research C version 4.4
# for a portable environment.
#
# For ANSI Compilation
MRIENV="-DELVIS_68040 -DANSI -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"

# For non-ANSI Compilation
#MRIENV="-DELVIS_68040 -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"

MRICC="mcc68k"
MRILD="mcc68k"
MRICOPTS="-p68040 -H -A -v -Fsm -O -Ob -Oe -Ot -nKc -nKm  -Mcp -Mdp -Ml -Gf "
MRILOPTS="-e$(OUTNAME).lnk -Wl,\"-m > $(OUTNAME).map -o $(OUTNAME).vcs -F s\""
MRILIBS="$(LIB_PATH)/lib040.lib"
MRILINT=""
MRILNTOPTS=""


# Microsoft Visual C++ for Win 95/NT------------------------------------
#
# These flags define the environment required to build an image for
# a Windows '95 or Windows NT environment
#
#
# For ANSI Compilation
WINENV="-DWIN32 -DWIN2K -DANSI -UDSS_NO_DOT_NET -DWIN32_LEAN_AND_MEAN -DSS_DRVR_SUPPORT"

# For non-ANSI Compilation
#WINENV="-DWIN32"

WINCC="cl"
WINLD="cl"
WINCOPTS="-Od -W3 -Zi -Yd -MD"
WINLOPTS="-o $(OUTNAME) -F950000 -Zd"
WINDBGOPTS="-link -debug:full -debugtype:cv"
WINLIBS="ws2_32.lib -link -debug:full -debugtype:cv"
WINLINT=""
WINLNTOPTS=""

# for library compilation
WINLIBOPTS="-r -o $(OUTNAME)"

# Microsoft Visual C++ for DOS------------------------------------------
#
# These flags define the environment required to build an image for
# a DOS environment
#
# This environment is TBD
#
# For ANSI Compilation
DOSENV="-DMSC60 -DDOS -DANSI"

# For non-ANSI Compilation
#DOSENV="-DMSC60 -DDOS"

DOSCC="cl"
DOSLD="cl"
DOSCOPTS="-Od -W3 -Zi -Yd -Gx- -Gd -Gy"
DOSLOPTS="@$(OUTNAME).lnk /ST:25000 /CO /NOI /NOE"
DOSLIBS=""
DOSLINT=""
DOSLNTOPTS=""
# legacy flag for x86 machines
#
# define if required (only for DOS!).
#
# -AH -- huge model
# -AL -- large model
# -AS -- small model
#
MODEL=


# Actual Build Environment----------------------------------------------

# object file suffix----------------------------------------------------
#
# this defaults to 'o' for most UNIX based systems
# this can be set to 'obj' for DOS/Windows/NT platforms
#
SOL_OBJ_SUFFIX=o
WIN_OBJ_SUFFIX=obj
UNIX_OBJ_SUFFIX=o
MS_OBJ_SUFFIX=obj
OBJ=$(SOL_OBJ_SUFFIX)

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

# product options for portable build
CPRDPTOPTS="$(CENVOPTS) $(PRDPTOPTS)"

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
#CMSOPTS=$(CENVOPTS) $(MSINC_PATH) $(MSCOPTS)

#=======================================================================
#
# macros for depend and lint options
#

# depend acceptance test
DEPOPTS_ACC="$(PRDACCOPTS)"

# depend portable test
DEPOPTS_PT="$(PRDPTOPTS)"

#=======================================================================
#
# macros for hardcoded dependencies
#

#
# The following are enabled by default since we assume makedepend
# is not available. If makedepend is available in your environemnt
# uncomment out this section.
#
# DIAB cross compiler under Sun Solaris for PSOS----------------
#
# These flags define the environment required to build an image for
# a DIAB cross compiler on Sun Solaris
#
#
# For ANSI Compilation
# DIABENV=<compiler-specific options>

# For non-ANSI Compilation
# DIABENV=<compiler-specific options>
# DIABCC=<compiler tool>
# DIABLD=<linker tool>
# DIABCOPTS=<compiler options>
# DIABLOPTS=<linker options>
# DIABLIBS=<library files if required>
# DIABLINT=<lint tool>
# DIABLNTOPTS=<lint tool options>

#=======================================================================
# SECTION 6
# Use the following as examples and modify it to the specific
# product requirements

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
LSZ_INC=$(CODEPATH)/s1ap_mme/lsz.h $(CODEPATH)/s1ap_mme/lsz.x
#LSI_INC=

# Product layer management - LEG includes
LEG_INC=$(CODEPATH)/egtp/leg.h $(CODEPATH)/egtp/leg.x

# Product Upper Interface - SZT includes
SZT_INC=$(CM_SZT_DIR)/szt.h $(CM_SZT_DIR)/szt.x $(CM_SZT_DIR)/szt_asn.h $(CM_SZT_DIR)/szt_asn.x

# Product lower Interface - SCT includes
SCT_INC= $(CM_SCT_DIR)/sct.h $(CM_SCT_DIR)/sct.x

# LTE CNE layer manager - LVB includes
LVB_INC=$(CM_LVB_DIR)/lvb.h $(CM_LVB_DIR)/lvb.x

# common function library includes
CM_INC = \
   $(CM_HASH_DIR)/cm_hash.h      \
   $(CM_HASH_DIR)/cm_hash.x      \
   $(CM_NAS_DIR)/cm_emm.x        \
   $(CM_NAS_DIR)/cm_emm.h        \
   $(CM_NAS_DIR)/cm_esm.x        \
   $(CM_NAS_DIR)/cm_esm.h        \
   $(CM_NAS_DIR)/cm_emm_esm.x    \
   $(CM_NAS_DIR)/cm_emm_esm.h    \
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
   $(CM_OS_DIR)/cm_os.h        \
   $(CM_OS_DIR)/cm_os.x        \
   $(CM_CM_DIR)/cm_err.h

# message functions
MF_INC = \
   $(CM_ASN_DIR)/cm_pasn.h        \
   $(CM_ASN_DIR)/cm_pasn.x


# Database functions
#DB_INC=

# Product - S1AP includes
#SZ_INC=
#SZ_INC=$(SZ_DIR)/sz.h $(SZ_DIR)/sz.x $(SZ_DIR)/sz_err.h

SB_INC=$(SB_DIR)/sb.h $(SB_DIR)/sb.x $(SB_DIR)/sb_mtu.h $(SB_DIR)/sb_mtu.x

HI_INC=$(HI_DIR)/hi.h $(HI_DIR)/hi.x

# ALL product includes
SZALL_INC=$(LSZ_INC) $(SZT_INC) $(SZ_INC) $(LVB_INC)

# Product - sample acceptance test includes
VB_INC=$(VB_DIR)/vbsm.h $(VB_DIR)/vbsm.x $(VB_DIR)/vb.x

EGT_INC=$(CODEPATH)/cm/egt.x

#EG_INC=$(EG_DIR)/eg_util.x

# MME Includes
MME_INC=\
	$(VB_DIR)/vb.h \
	$(VB_DIR)/vb.x \
	$(VB_DIR)/vb_hss.h \
	$(VB_DIR)/vb_hss.x \
	$(VB_DIR)/vb_hss_common.x \
	$(VB_DIR)/vb_hss_autn.x \

# OS specific includes - SS includes
#SS_INC=
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h \
	$(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x    \
	$(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x $(SS_DIR)/ss_msg.h     \
	$(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x \
	$(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x $(SS_DIR)/ss_task.h  \
	$(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x

#MT_INC=
MT_INC=$(MT_DIR)/mt_ss.h $(MT_DIR)/mt_ss.x $(MT_DIR)/mt_err.h


# sample acceptance tests all includes
VBALL_INC=$(SS_INC) $(SYS_INC) $(SZALL_INC) $(VB_INC) $(EGT_INC) $(EG_INC) $(MME_INC) $(MAKENAME)

#=======================================================================
# SECTION 7
# macros for source and object files

# acceptance test source------------------------------------------------
VB_SRCS= \
	$(VB_DIR)/vbsm.c  \
	$(VB_DIR)/vbsm_egcfg.c  \
	$(VB_DIR)/vbsm_hicfg.c  \
	$(VB_DIR)/vbsm_sbcfg.c  \
	$(VB_DIR)/vbsm_szcfg.c  \
	$(VB_DIR)/vbsm_vbcfg.c  \
	$(VB_DIR)/vbsm_utl.c  \
	$(VB_DIR)/vbsm_tst.c  \
	$(VB_DIR)/vbsm_ex_ms.c \
	$(VB_DIR)/vb_tmr.c  \
	$(VB_DIR)/vb_ptli.c \
	$(VB_DIR)/lvb.c


# acceptance test object------------------------------------------------
VB_OBJS= \
	$(SY_DIR)/vbsm.$(OBJ)  \
	$(SY_DIR)/vbsm_egcfg.$(OBJ)  \
	$(SY_DIR)/vbsm_hicfg.$(OBJ)  \
	$(SY_DIR)/vbsm_sbcfg.$(OBJ)  \
	$(SY_DIR)/vbsm_szcfg.$(OBJ)  \
	$(SY_DIR)/vbsm_vbcfg.$(OBJ)  \
	$(SY_DIR)/vbsm_utl.$(OBJ)  \
	$(SY_DIR)/vbsm_tst.$(OBJ)  \
	$(SY_DIR)/vbsm_ex_ms.$(OBJ)  \
	$(SY_DIR)/vb_tmr.$(OBJ)  \
	$(SY_DIR)/vb_ptli.$(OBJ) \
	$(SY_DIR)/lvb.$(OBJ)

#	$(SY_DIR)/vbsm_vbcfg.$(OBJ)  \
# MME Source Files
MME_SRCS=\
	$(VB_DIR)/vb_mi.c \
	$(VB_DIR)/vb_id.c \
	$(VB_DIR)/vb_emm.c \
	$(VB_DIR)/vb_esm.c \
	$(VB_DIR)/vb_mme.c \
   $(VB_DIR)/vb_nas_hndlr.c \
   $(VB_DIR)/vb_nas_utl.c \
   $(VB_DIR)/vb_s1ap_hndlr.c \
   $(VB_DIR)/vb_s1ap_utl.c \
   $(VB_DIR)/vb_s10_hndlr.c \
   $(VB_DIR)/vb_s10_utl.c \
	$(VB_DIR)/vb_mme_ex_ms.c \
	$(VB_DIR)/vb_mme_utl.c \
	$(VB_DIR)/vb_gtp_utl.c \
	$(VB_DIR)/vb_s11_hndlr.c \
	$(VB_DIR)/vb_mme_lim.c \
	$(VB_DIR)/vb_mme_sec.c \
	$(VB_DIR)/vb_alg_intf.c \
	$(VB_DIR)/vb_sec_intf.c \
	$(VB_DIR)/vb_hss_proc.c \
	$(VB_DIR)/vb_hss_autn.c \
	$(VB_DIR)/vb_hss_uedb.c

# MME Object Files
MME_OBJS=\
   $(VB_DIR)/vb_utl.$(OBJ) \
   $(VB_DIR)/vb_mme_utl.$(OBJ) \
   $(VB_DIR)/vb_gtp_utl.$(OBJ) \
   $(VB_DIR)/vb_s11_hndlr.$(OBJ) \
	$(VB_DIR)/vb_mi.$(OBJ) \
	$(VB_DIR)/vb_ptmi.$(OBJ) \
	$(VB_DIR)/vb_id.$(OBJ) \
	$(VB_DIR)/vb_emm.$(OBJ) \
	$(VB_DIR)/vb_esm.$(OBJ) \
	$(VB_DIR)/vb_esm_gtp.$(OBJ) \
	$(VB_DIR)/vb_mme.$(OBJ) \
	$(VB_DIR)/vb_nas_hndlr.$(OBJ) \
	$(VB_DIR)/vb_nas_utl.$(OBJ) \
	$(VB_DIR)/vb_s1ap_hndlr.$(OBJ) \
	$(VB_DIR)/vb_s1ap_utl.$(OBJ) \
	$(VB_DIR)/vb_s10_hndlr.$(OBJ) \
	$(VB_DIR)/vb_s10_utl.$(OBJ) \
	$(VB_DIR)/vb_mme_ex_ms.$(OBJ) \
	$(VB_DIR)/vb_mme_lim.$(OBJ) \
	$(VB_DIR)/vb_mme_sec.$(OBJ) \
	$(VB_DIR)/vb_alg_intf.$(OBJ) \
	$(VB_DIR)/vb_sec_intf.$(OBJ) \
	$(VB_DIR)/vb_hss_proc.$(OBJ) \
	$(VB_DIR)/vb_hss_autn.$(OBJ) \
	$(VB_DIR)/vb_hss_uedb.$(OBJ)


# Target to build MME as Library
MMELIB=mmelib

MME_LIB1=mmelib1

#MME Library Name
MME_LIB= $(VB_DIR)/libmme_td.a

# S1AP source------------------------------------------------
SZ_SRCS=

# Product source------------------------------------------------
SB_SRCS=

#----------------------------------------- HI source
HI_SRCS=

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
	lvb.h     \
	lvb.x     \
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

# VB stack manager objects-----------------------------------------
VBSM_OBJS= \
	$(SY_DIR)/smvbexms.$(OBJ) \
	$(SY_DIR)/smvbptmi.$(OBJ)

# select which OS build-------------------------------------------------

# all acceptance test source--------------------------------------------
PRDACC_SRCS= \
	$(VB_SRCS) \
	$(SZSBCM_SRCS) \
	$(SZHICM_SRCS) \
	$(SB_SRCS)    \
	$(HI_SRCS)    \
	$(SM_SRCS)    \
	$(SZ_SRCS)

# all acceptance test objects-------------------------------------------
PRDACC_OBJS= \
   $(VB_OBJS) \
	$(MME_OBJS) \
   $(VBSM_OBJS) \
   $(SZLIB) \
   $(EGLIB) \
   $(SECLIB) \
   $(VBCMLIB) \

#for linux 32 bit machine
ifeq ($(MACHINE), BIT64)
SZLIB=$(LIB_DIR)/libvb_lnx64sz.a
EGLIB=$(LIB_DIR)/libvb_lnx64eg.a
SECLIB=$(LIB_DIR)/libvb_lnx64vbsec.a
VBCMLIB=$(LIB_DIR)/libvb_lnx64vbcm.a
else
SZLIB=$(LIB_DIR)/libvb_lnx32sz.a
EGLIB=$(LIB_DIR)/libvb_lnx32eg.a
SECLIB=$(LIB_DIR)/libvb_lnx32vbsec.a
VBCMLIB=$(LIB_DIR)/libvb_lnx32vbcm.a
endif

# all library sources--------------------------------------------------
PRDLIB_SRCS= $(MME_SRCS)

# all library objects--------------------------------------------------
PRDLIB_OBJS= $(MME_OBJS)

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
#clean:
#  $(REMOVE) *.$(OBJ) $(BINACC) $(BINPT) *.ln *.s $(SY_DIR)/*.$(OBJ) \
   $(SY_DIR)/$(BINACC) $(SY_DIR)/*.ln $(SY_DIR)/*.s $(SZLIB) $(EGLIB) \
	$(SECLIB) $(VBCMLIB) $(SEC_DIR)/*.o

vbclean:
	$(REMOVE) $(VB_OBJS) $(MME_OBJS) $(BINACC) $(SY_DIR)/*.$(OBJ) \
	$(SEC_DIR)/*.o

# External target for acceptance test build
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC)

# External target for portable build
pt:
	@$(MAKE) -f $(MAKENAME) $(BLDENV)  \
	INC_PATH=$(PT_INC_PATH) OUTNAME=$(BINPT)

# External target for libs build
lib:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINLIB)  \
    OSOPTS=$(LIBOSOPTS) INC_PATH=$(LIB_INC_PATH)

# External target for klocworks build
kw:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINKW)

#
# Linux  GNU acceptance test build-------------------------------------
$(BINLNXACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Linux  purecov acceptance test build-------------------------------------
$(BINLNXPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(PLNXCC) \
	LD=$(PLNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Linux purify acceptance test build-------------------------------------
$(BINPURACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Linux GNU portable build---------------------------------------------
$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#=======================================================================
# SECTION 8
# Portable Target builds for Solaris CC, Solaris GNU, MRI mcc68k on
# Solaris and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC portable build---------------------------------------------
#
$(BINSOLPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GNU portable build--------------------------------------------
#
$(BINGNUPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#
# Microsoft Visual C++ for WINNT/WIN95 portable build-------------------
$(BINWINPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(MS_OBJ_SUFFIX)
#
# MicroTec Research Compiler in Solaris Environment---------------------
$(BINMRIPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(MRIENV) CC=$(MRICC) \
	LD=$(MRILD) COPTS=$(MRICOPTS) LOPTS=$(MRILOPTS) LIBS=$(MRILIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Acceptance test target builds for Solaris CC, Solaris GNU,
# Inisite Memory Check tool and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC acceptance test build--------------------------------------
#
$(BINSOLACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GNU acceptance test build-------------------------------------
#
$(BINGNUACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GPP(g++) acceptance test build-------------------------------------
#
$(BINGPPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGPPINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGPPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINGPPLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(GPPENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(GPPCOPTS) LOPTS=$(GPPLIBOPTS) LIBS=$(GPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

# Solaris CPP (c++) acceptance test build-------------------------------------
#
$(BINCPPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINCPPINT):
	@$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINCPPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)
$(BINCPPLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(CPPENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(CPPLIBOPTS) LIBS=$(CPPLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#
# Microsoft Visual C++ for WINNT/WIN95 acceptance test build------------
$(BINWINACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(MS_OBJ_SUFFIX)
#
# Inisite Memory Check Toolin Solaris acceptance test build-------------
$(BININSITEACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

$(BINPPCACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

# portable--------------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEPT):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS) \
	CSMOPTS=$(CSMPTOPTS)

$(MAKEKW):
	@$(MAKE) -f $(MAKENAME) $(KWBIN) CPRDOPTS=$(CPRDACCOPTS) \
	CSMOPTS=$(CSMACCOPTS)

$(LIBBIN): $(PRDLIB_OBJS)
	$(LD) $(LOPTS) $(PRDLIB_OBJS) $(LIBS)

$(KWBIN): $(PRDKW_OBJS)
	$(LD) $(PRDKW_OBJS) $(LIBS) $(LOPTS)

#
# library target builds for Solaris CC &  Solaris GNU
#
# Solaris CC library build--------------------------------------
#
$(BINSOLLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(SOLENV) CC=$(SOLCC) \
    LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBOPTS) LIBS=$(SOLLIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)
#
# Solaris GNU library build-------------------------------------
#
$(BINGNULIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(GNUENV) CC=$(GNUCC) \
    LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBOPTS) LIBS=$(GNULIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)

#
# Linux GNU library build-------------------------------------
#
$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(LNXCC) \
    LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBOPTS) \
    OBJ=$(SOL_OBJ_SUFFIX)

#
# Window NT library build-------------------------------------
#
$(BINWINLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(WINENV) CC=$(WINCC) \
    LD=$(WINLIBLD) COPTS=$(WINCOPTS) LOPTS=$(WINLIBOPTS) LIBS=$(WINLIBS) \
    OBJ=$(WIN_OBJ_SUFFIX)

#
# Solaris kw build-------------------------------------
#
$(BINGNUKW):
	@$(MAKE) -f $(MAKENAME) $(MAKEKW) -e ENV=$(GNUENV) CC=$(GNUCC) \
    LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
    OBJ=$(SOL_OBJ_SUFFIX)



#=======================================================================
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

$(VBCMLIB):
	@$(MAKE) -f vb_cm.mak BLDENV=lnx_lib lib

$(SZLIB):
	@$(MAKE) -f vb_sz.mak BLDENV=lnx_lib lib

$(EGLIB):
	@$(MAKE) -f vb_eg.mak BLDENV=lnx_lib lib

$(SECLIB):
	cd ./sec; \
	make; \
	cd ..;

$(ACCBIN): $(PRDACC_OBJS)
	$(LD)  $(PRDACC_OBJS) $(LIBS) $(LOPTS)

$(INTBIN): $(PRDINT_OBJS)
	$(LD)  $(PRDINT_OBJS) $(LIBS) $(LOPTS)

$(MAKELIB):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN) CPRDOPTS=$(CPRDLIBOPTS) \
    CSMOPTS=$(CSMLIBOPTS)

#
# Make Depend
# depend portable test--------------------------------------------------
# dependency for portable source
depend_pt: $(PRDPT_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(PTOSOPTS) \
	DEPOPTS=$(DEPOPTS_PT) INC_PATH=$(PT_INC_PATH) DEP_SRCS="$(PRDPT_SRCS)"

# depend acceptance test------------------------------------------------
# dependency for acceptance test
depend_acc: $(PRDACC_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e \
	DEPOPTS=$(DEPOPTS_ACC) INC_PATH=$(ACC_INC_PATH) DEP_SRCS="$(PRDACC_SRCS)"

# Build the dependency
$(DEPEND):
	$(MAKEDEPEND) -f $(MAKENAME) $(OSOPTS) $(DEPOPTS) $(INC_PATH) $(DEP_SRCS)

#=======================================================================
# SECTION 10
# Source code build directives
# Modify this section to list out the build directives for the specific
# product

# Acceptance test files build -------------------------

$(SY_DIR)/vbsm.$(OBJ): $(VB_DIR)/vbsm.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm.c -o $(SY_DIR)/vbsm.$(OBJ)

$(SY_DIR)/vbsm_egcfg.$(OBJ): $(VB_DIR)/vbsm_egcfg.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_egcfg.c -o $(SY_DIR)/vbsm_egcfg.$(OBJ)

$(SY_DIR)/vbsm_hicfg.$(OBJ): $(VB_DIR)/vbsm_hicfg.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_hicfg.c -o $(SY_DIR)/vbsm_hicfg.$(OBJ)

$(SY_DIR)/vbsm_sbcfg.$(OBJ): $(VB_DIR)/vbsm_sbcfg.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_sbcfg.c -o $(SY_DIR)/vbsm_sbcfg.$(OBJ)

$(SY_DIR)/vbsm_szcfg.$(OBJ): $(VB_DIR)/vbsm_szcfg.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_szcfg.c -o $(SY_DIR)/vbsm_szcfg.$(OBJ)

$(SY_DIR)/vbsm_vbcfg.$(OBJ): $(VB_DIR)/vbsm_vbcfg.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_vbcfg.c -o $(SY_DIR)/vbsm_vbcfg.$(OBJ)

$(SY_DIR)/vbsm_tst.$(OBJ): $(VB_DIR)/vbsm_tst.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_tst.c -o $(SY_DIR)/vbsm_tst.$(OBJ)

$(SY_DIR)/vbsm_utl.$(OBJ): $(VB_DIR)/vbsm_utl.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_utl.c -o $(SY_DIR)/vbsm_utl.$(OBJ)

$(SY_DIR)/vb_tmr.$(OBJ): $(VB_DIR)/vb_tmr.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_tmr.c -o $(SY_DIR)/vb_tmr.$(OBJ)

$(SY_DIR)/vb_ptli.$(OBJ): $(VB_DIR)/vb_ptli.c $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_ptli.c -o $(SY_DIR)/vb_ptli.$(OBJ)

$(SY_DIR)/lvb.$(OBJ): $(CM_LVB_DIR)/lvb.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(CM_LVB_DIR)/lvb.c -o $(SY_DIR)/lvb.$(OBJ)

# MME build files
$(SY_DIR)/vb_emm.$(OBJ): $(VB_DIR)/vb_emm.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_emm.c -o $(SY_DIR)/vb_emm.$(OBJ)

$(SY_DIR)/vb_esm.$(OBJ): $(VB_DIR)/vb_esm.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_esm.c -o $(SY_DIR)/vb_esm.$(OBJ)

$(SY_DIR)/vb_esm_gtp.$(OBJ): $(VB_DIR)/vb_esm_gtp.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_esm_gtp.c -o $(SY_DIR)/vb_esm_gtp.$(OBJ)

$(SY_DIR)/vb_mme.$(OBJ): $(VB_DIR)/vb_mme.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_mme.c -o $(SY_DIR)/vb_mme.$(OBJ)

$(SY_DIR)/vb_nas_hndlr.$(OBJ): $(VB_DIR)/vb_nas_hndlr.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_nas_hndlr.c -o $(SY_DIR)/vb_nas_hndlr.$(OBJ)

$(SY_DIR)/vb_nas_utl.$(OBJ): $(VB_DIR)/vb_nas_utl.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_nas_utl.c -o $(SY_DIR)/vb_nas_utl.$(OBJ)

$(SY_DIR)/vb_s1ap_hndlr.$(OBJ): $(VB_DIR)/vb_s1ap_hndlr.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_s1ap_hndlr.c -o $(SY_DIR)/vb_s1ap_hndlr.$(OBJ)

$(SY_DIR)/vb_s10_hndlr.$(OBJ): $(VB_DIR)/vb_s10_hndlr.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_s10_hndlr.c -o $(SY_DIR)/vb_s10_hndlr.$(OBJ)

$(SY_DIR)/vb_s10_utl.$(OBJ): $(VB_DIR)/vb_s10_utl.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_s10_utl.c -o $(SY_DIR)/vb_s10_utl.$(OBJ)

$(SY_DIR)/vb_s1ap_utl.$(OBJ): $(VB_DIR)/vb_s1ap_utl.c $(MT_INC) $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_s1ap_utl.c -o $(SY_DIR)/vb_s1ap_utl.$(OBJ)

$(SY_DIR)/vb_mme_ex_ms.$(OBJ): $(VB_DIR)/vb_mme_ex_ms.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_mme_ex_ms.c -o $(SY_DIR)/vb_mme_ex_ms.$(OBJ)

$(SY_DIR)/vb_mme_utl.$(OBJ): $(VB_DIR)/vb_mme_utl.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_mme_utl.c -o $(SY_DIR)/vb_mme_utl.$(OBJ)

$(SY_DIR)/vb_gtp_utl.$(OBJ): $(VB_DIR)/vb_gtp_utl.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_gtp_utl.c -o $(SY_DIR)/vb_gtp_utl.$(OBJ)

$(SY_DIR)/vb_s11_hndlr.$(OBJ): $(VB_DIR)/vb_s11_hndlr.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_s11_hndlr.c -o $(SY_DIR)/vb_s11_hndlr.$(OBJ)

$(SY_DIR)/vbsm_ex_ms.$(OBJ): $(VB_DIR)/vbsm_ex_ms.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vbsm_ex_ms.c -o $(SY_DIR)/vbsm_ex_ms.$(OBJ)

$(SY_DIR)/vb_mme_lim.$(OBJ): $(VB_DIR)/vb_mme_lim.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_mme_lim.c -o $(SY_DIR)/vb_mme_lim.$(OBJ)

$(SY_DIR)/vb_mme_sec.$(OBJ): $(VB_DIR)/vb_mme_sec.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_mme_sec.c -o $(SY_DIR)/vb_mme_sec.$(OBJ)

$(SY_DIR)/vb_alg_intf.$(OBJ): $(VB_DIR)/vb_alg_intf.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_alg_intf.c -o $(SY_DIR)/vb_alg_intf.$(OBJ)

$(SY_DIR)/vb_sec_intf.$(OBJ): $(VB_DIR)/vb_sec_intf.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_sec_intf.c -o $(SY_DIR)/vb_sec_intf.$(OBJ)

$(SY_DIR)/vb_mi.$(OBJ): $(VB_DIR)/vb_mi.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_mi.c -o $(SY_DIR)/vb_mi.$(OBJ)

$(SY_DIR)/vb_ptmi.$(OBJ): $(VB_DIR)/vb_ptmi.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_ptmi.c -o $(SY_DIR)/vb_ptmi.$(OBJ)

$(SY_DIR)/vb_id.$(OBJ): $(VB_DIR)/vb_id.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_id.c -o $(SY_DIR)/vb_id.$(OBJ)

$(SY_DIR)/vb_utl.$(OBJ): $(VB_DIR)/vb_utl.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_utl.c -o $(SY_DIR)/vb_utl.$(OBJ)

$(SY_DIR)/vb_hss_autn.$(OBJ): $(VB_DIR)/vb_hss_autn.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_hss_autn.c -o $(SY_DIR)/vb_hss_autn.$(OBJ)

$(SY_DIR)/vb_hss_proc.$(OBJ): $(VB_DIR)/vb_hss_proc.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_hss_proc.c -o $(SY_DIR)/vb_hss_proc.$(OBJ)

$(SY_DIR)/vb_hss_uedb.$(OBJ): $(VB_DIR)/vb_hss_uedb.c $(CM_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_hss_uedb.c -o $(SY_DIR)/vb_hss_uedb.$(OBJ)

# SGW build files
$(SY_DIR)/vb_sgw.$(OBJ): $(VB_DIR)/vb_sgw.c $(VBALL_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_sgw.c -o $(SY_DIR)/vb_sgw.$(OBJ)

$(SY_DIR)/vb_sgw_ex_ms.$(OBJ): $(VB_DIR)/vb_sgw_ex_ms.c $(VBALL_INC) $(VBALL_INC)
	$(CC) -c $(CPRDOPTS) $(VB_DIR)/vb_sgw_ex_ms.c -o $(SY_DIR)/vb_sgw_ex_ms.$(OBJ)

#----------------------------------------- HI Layer objects


# stack manager files build---------------------------------------------

#EGTP Stack Manager Files

#VB Stack Manager Files
$(SY_DIR)/smvbexms.$(OBJ): $(VBSM_DIR)/smvbexms.c $(SYS_INC) $(LVB_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(VBSM_DIR)/smvbexms.c -o $(SY_DIR)/smvbexms.$(OBJ)

$(SY_DIR)/smvbptmi.$(OBJ): $(VBSM_DIR)/smvbptmi.c $(SYS_INC) $(LVB_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(VBSM_DIR)/smvbptmi.c -o $(SY_DIR)/smvbptmi.$(OBJ)


#-------------------------------------------------------------#
# Create Library for MME
#-------------------------------------------------------------#
$(MMELIB):
	@$(MAKE) -f $(MAKENAME) $(MME_LIB1) INC_PATH=$(LIB_INC_PATH) \
	ENV=$(LNXENV)

$(MME_LIB1):
	@$(MAKE) -f $(MAKENAME) $(MME_LIB) \
	CPRDOPTS=$(CPRDACCOPTS)

$(MME_LIB): $(MME_OBJS)
	ar -r $(MME_LIB) $(MME_OBJS)

#
#********************************************************************30**
#
#        End of file:     vb.mak@@/main/1 - Mon Jun 28 19:35:27 2010
#
#********************************************************************31*/
#
#********************************************************************40**
#
#        Notes:
#
#********************************************************************41*/
#
#********************************************************************50**
#
#********************************************************************51*/
#
#********************************************************************60**
#
#       Revision history:
#
#********************************************************************61*/
#
#********************************************************************90**
#
#    ver       pat    init                  description
#----------- -------- ---- -----------------------------------------------
#/main/1      ---     ad        1. Created for Lte-CNE Phase 1
#********************************************************************91*/

#DO NOT DELETE THIS LINE -- make depend depends on it.
