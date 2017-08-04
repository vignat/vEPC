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
#********************************************************************20**
#
#       Name:   LTE CNE makefile
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link PDNGW module.
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
#               acc              acceptance tests (with MOS)
#
#               clean            remove all object files
#
#
#
#
#       Sid:      av.mak@@/main/1 - Tue Aug 17 18:54:38 2010
#
#       Prg:     rp
#
#********************************************************************21*/
#=======================================================================
#
# macros for product specific options

# commented for RTR

#=======================================================================
# SECTION 2
# Product specific options
#
## TCP/UDP convergence layer (HI)
# HI_REL_1_3 : enable to be compatible to rel 1.3
# HI_REL_1_2 : enable to be compatible to rel 1.2
# HI_LPBK    : enable to run TUCL in loopback mode
# DEBUGP     : enable to turn ON debug support
# NO_ERRCLS : enable to compile out error class checking
#
## Common options
# CM_INET2      : enable to be compatable to the cm_inet rel 2
# CMINETDBG     : enable socket debugging supprt
# DEBUGP        : enable to turn ON debug support

## GTP layer (EG)
# LEG_FILE_LOG   : enable to turn on File log
# EG_GROUPED_CHK : enable for the validatin of grouped IE's
# EG_PRNT_DATA   : To print the ie data portion on console
# NO_ERRCLS : enable to compile out error class checking
# DEBUGP     : enable runtime debugging

## PDN-GW Application layer (AV)
# NO_ERRCLS : enable to compile out error class checking
# DEBUGP    : enable runtime debugging
# AV_PERF   : enable for performance setup without external network contact.
# SS_MEM_LEAK_STS: enable for checking memory leak with SSI memory leak tool.
# NOTE: Rest of the flags are required to be enabled/disabled for this product
# by default. Example: EG, EGTP_C, EGTP_U, AV

# for loose coupling
PRDOPTS=-DAV -DCNS_PH1 -DLCLHI -DLCHIT -DLWLCHIT \
        -DEU -DEG -DEGTP_C -DEGTP_U -DDEBUGP \
        -DLCLAV -DLCAVMILAV -DLCSMAVMILAV -UAV_PERF\
        -DHI -DHI_MULTI_THREADED -DLCSMHIMILHI -DLCSMEGMILEG \
        -DLCLEG -DLCEULIEGT -DLCEGT -DLCEGUIEGT \
        -DEG_REL_930 -DEG_PHASE2 -DDSATPA -UUSE_PURE -UPURE -UINCLUDE_EXT_HDR

# Product options for portable build
PRDPTOPTS=$(PRDOPTS) -DCMFILE_REORG_1 -DCM_INET2

# -------------- Product options for acceptance test build
PRDACCOPTS=$(PRDOPTS) $(EGOPTS) -DSM -DCM_INET2      \
      -DxSS_MULTIPLE_PROCS \
      -DAV -DLCLAV -DLCAVMILAV -DLCSMAVMILAV

EGOPTS=-DEGTP_C -DEGTP_U -DEG_REL_930 -DEG_PHASE2


#
# -------------- HI OPTS for acceptance test build
CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DHI_MULTI_THREADED


# Product options for library build
PRDLIBOPTS=$(PRDOPTS) -DDBG4 -DCMFILE_REORG_1 -DCM_INET2

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
           -DxSS_SINGLE_THREADED -DLONG_MSG -DxSS_MULTIPLE_PROCS

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
MTOSOPTS=-DSS_MT -DSS_PERF -D__EXTENSIONS__ -DDEBUGNOEXIT -USS_M_PROTO_REGION \
         -DxSS_MULTICORE_SUPPORT -USSI_MEM_CHNG -DSLES9_PLUS -USS_MEM_LEAK_STS -DUSE_PURE

# Options for library build with MTSS - MOS multithread OS
LIBOSOPTS="-DSS_MT -DSS  -DxSS_SINGLE_THREADED"

# Linux  specific options
#LNXOSOPTS=-D_GNU_SOURCE -DSS_LINUX -DQCACC_PRETTY_PRINT $(MTOSOPTS)
#shwetha
#LNXOSOPTS=-DSS_LINUX $(MTOSOPTS)

LNXOSOPTS= -DSS_LINUX $(MTOSOPTS) \
           -DCONAVL -D_REENTRANT -D_GNU_SOURCE -DCONSTDIO



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
BASENAME=av

MAKENAME=$(BASENAME).mak

# default acceptance test for Solaris cc environment
BINACC="$(BASENAME)_acc"

# default portable for Solaris cc environment
BINPT="$(BASENAME)_pt"

# default portable build for KlocWork
BINKW="$(BASENAME)_kw"

# default library for Solaris cc environment
BINLIB="lib$(BASENAME).a"

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
LIBBIN=lib$(BASENAME).a

# Internal target for makedepend
DEPEND=depend

# Build targets for testing in different environments

# Build with the Solaris CC compiler and linker
# Acceptance Test Build
BINSOLACC=sol_acc
# Portable Build
BINSOLPT=sol_pt

# Library Build
BINSOLLIB=sol_lib

# Build with the GNU compiler and linker on Solaris
# Acceptance Test Build
BINGNUACC=gnu_acc
# Portable Build
BINGNUPT=gnu_pt
# library Build
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
#Library Build
BINCPPLIB=cpp_lib

# Build with the Insite Memory Check programme - Acceptance Tests only
# Acceptance Test Build
BININSITEACC=ins_acc

# Build with the Visual C++ on Windows NT or Windows 95(user space only)
# Acceptance Test Build
BINWINACC=win_acc
# Portable Build
BINWINPT=win_pt

# Library Build
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
#CODEPATH = ../../src
CODEPATH = .
CM_CODE_DIR=$(CODEPATH)
#CM_SS_CODE_DIR=/u/prd/rtr/mt
CM_SS_CODE_DIR=$(CODEPATH)
#Windows
#CM_SS_CODE_DIR=.
SAMPLE_CODE_DIR=$(CODEPATH)
SM_CODE_DIR=$(CODEPATH)

# common files directory
CM_CM_DIR=.

# common files directory
CM_SS_DIR=$(CM_SS_CODE_DIR)
CM_HASH_DIR=$(CM_CODE_DIR)
CM_MEM_DIR=$(CM_CODE_DIR)
CM_TMR_DIR=$(CM_CODE_DIR)
CM_TPT_DIR=$(CM_CODE_DIR)
CM_DNS_DIR=$(CM_CODE_DIR)
CM_ASN_DIR=$(CM_CODE_DIR)
CM_LIB_DIR=$(CM_CODE_DIR)
CM_SS7_DIR=$(CM_CODE_DIR)
CM_SCT_DIR=$(CM_CODE_DIR)
CM_MRS_DIR=$(CM_CODE_DIR)
CM_HIT_DIR=$(CM_CODE_DIR)
CM_LLIST_DIR=$(CM_CODE_DIR)
CM_INET_DIR=$(CM_CODE_DIR)
CM_SM_DIR=$(SM_CODE_DIR)
CM_OS_DIR=$(CM_CODE_DIR)


# TUCL directory
HI_DIR=$(CODEPATH)


# common layer manager interface files directory
CM_PGW_DIR=$(AV_DIR)

CM_LAV_DIR=$(AV_DIR)

# stack manager directory
PGWSM_DIR=$(SAMPLE_CODE_DIR)

EG_DIR=$(CODEPATH)

# OS directory
#
# MS_DIR=/u/sw/tds/ms/crnt
# NS_DIR=../ns
# VW_DIR=../mt

#SS_DIR=/u/prd/rtr/mt
#SS_DIR=.
#windows
#SS_DIR=$(NS_DIR)
MT_DIR=$(CODEPATH)
SS_DIR=$(MT_DIR)
#MT_DIR=.

# OS Directory for Solaris MTSS
OS_DIR=$(CODEPATH)
#OS_DIR=.

# OS Directory for Windows
#OS_DIR=.

# OS Directory for VxWorks
#OS_DIR= $(SS_DIR) $(VW_DIR)

# acceptance directory
AV_DIR=.

# build directory
SY_DIR=.

# include path

# portable include path
#PT_INC_PATH="-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) -I$(CM_TMR_DIR) \
   -I$(CM_ASN_DIR) -I$(CM_SS7_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) -I$(AV_DIR)\
   -I$(SP_DIR) -I$(CM_MEM_DIR) -I$(CM_LLIST_DIR) -I$(CM_PGW_DIR)"

# library include path
LIB_INC_PATH="-I$(AV_DIR) -I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR)"

# Acceptance test include path
# ACC_INC_PATH=" -I/tools/wind/target/h -I$(AV_DIR) -I$(CM_DIR) -I$(AV_DIR) \
#              -I$(CM_SM_DIR) -I$(PGWSM_DIR) -I$(OS_DIR) -I$(HI_DIR)"
#
ACC_INC_PATH="-I$(AV_DIR) -I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) \
   -I$(CM_TMR_DIR) -I$(CM_ASN_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) \
   -I$(AV_DIR) -I$(CM_SM_DIR) -I$(PGWSM_DIR) -I$(OS_DIR) -I$(HI_DIR) \
   -I$(CM_SS7_DIR) -I$(SB_DIR) -I$(CM_LLIST_DIR) -I$(CM_MEM_DIR) -I$(CM_PGW_DIR)\
   -I$(CM_PYTHON_DIR)-I$(PYTHON_PATH_WIN) -I$(SS_DIR) -I$(MT_DIR)"

# OS - files include path
MSINC_PATH=-I$(CM_DIR) -I$(OS_DIR)

# library path - used for integrated products and simulation env.
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

#for KSCTP
#SOLLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lnsl -lpython2.4 -lsctp"

# For 64 bit compilation
#SOLCC="/opt/SUNWspro/bin/cc -DALIGN_64BIT -DBIT_64"
#SOLLD="/opt/SUNWspro/bin/cc"
#SOLLOPTS="-o $(OUTNAME) -lpthread -lposix4 -g -mt -xarch=v9"

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
#GNUENV="-DSUNOS -DANSI"
GNUENV="-DSUNOS -DANSI -DSOLARIS -DSPARC -DPROC_SPARC -DUNIX -D__EXTENSIONS__"
#GNUENV="-DSUNOS -DANSI -std=c99 -D_XPG6"

#GNUCOPTS="-g3 -Wall -pedantic -pedantic-errors -Wno-comment -pipe -Wshadow \
#          -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes "
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
LNXENV="-DSUNOS   -DSS_LINUX -DANSI -DNO_ERRCLS -D_GNU_SOURCE"

#LNXCOPTS="-O3 -ansi -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused"

# With pedantic and -ansi defined, Berkley Packet Filter's LIBPCAP
#types are not allowed, as expected from glibc. Hence taking these options out
ifeq ($(MACHINE), BIT64)
LNXCOPTS="-g -march=core2 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment \
	-pipe -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
	-Wuninitialized  -Wunused "
else
LNXCOPTS=" -g3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused"
endif



# For non-ANSI Compilation
#LNXENV="-DSUNOS"
#LNXENV="-DSUNOS -DSS_LINUX"
#LNXCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
          -Wcast-align -Wcast-qual"

# For Cavium Compilation
#LNXCC="mips64-octeon-linux-gnu-gcc  -DALIGN_64BIT -DBIT_64"
#LNXLD="mips64-octeon-linux-gnu-gcc"
#LNXLIBLD="mips64-octeon-linux-gnu-ar"
#LNXCC="gcc -m64 -DALIGN_64BIT -DBIT_64"

ifeq ($(MACHINE), BIT64)
LNXCC="gcc -m64 "
LNXLD="gcc -m64 "
LNXLIBLD="gcc -m64 "
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
INSLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lpython2.4 -lm -lrt -lutil -ldl"
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

LNXLOPTS="-o $(OUTNAME)"
#LNXLOPTS="-o $(OUTNAME) -L/usr/lib/nptl"
LNXLIBOPTS="-r -o $(OUTNAME) "
#LNXLIBOPTS="-r -o $(OUTNAME) "
LNXLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lm -lrt -lutil -ldl"
#for KSCTP
#LNXLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lm -lrt -lutil -ldl -lsctp"
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"

# For 64 bit compilation
#LNXCC="gcc -m64 -DALIGN_64BIT -DBIT_64"
#LNXLD="gcc -m64"
#LNXCOPTS="-Os -g3 -ansi -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow -Wcast-qual \
#        -Wstrict-prototypes -Wmissing-prototypes -pedantic \
#        -Wimplicit -Wunused"
#LNXLOPTS="-o $(OUTNAME) -lpthread -lrt -Xlinker -zmuldefs -export-dynamic -lpython2.4 -lm -ldl -lutil"


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
#
# This is the actual environment used, set it appropriately
# (default Solaris)
# Uncomment this for default compilation under the Solaris
# environment for portable build and the acceptance test build
#ENV=$(SOLENV)
#CC=$(SOLCC)
#LD=$(SOLLD)
#COPTS=$(SOLCOPTS)
#LOPTS=$(SOLLOPTS)
#LIBS=$(SOLLIBS)
#LINT=$(SOLLINT)
#LNTOPTS=$(SOLLNTOPTS)

#----- uncomment the following for GNU compiler
#ENV=$(GNUENV)
#CC=$(GNUCC)
#LD=$(GNULD)
#COPTS=$(GNUCOPTS)
#LOPTS=$(GNULOPTS)
#LIBS=$(GNULIBS)
#LINT=$(GNULINT)
#LNTOPTS=$(GNULNTOPTS)

#----- uncomment the following for ppc
#ENV=$(PPCENV)
#CC=$(PPCCC)
#LD=$(PPCLD)
#COPTS=$(PPCCOPTS)
#LOPTS=$(PPCLOPTS)
#LIBS=$(PPCLIBS)
#LINT=$(PPCLINT)
#LNTOPTS=$(PPCLNTOPTS)

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

# SECTION 6
# Use the following as examples and modify it to the specific
# product requirements

# environment
ENV_INC=$(CM_CM_DIR)/envdep.h $(CM_CM_DIR)/envind.h $(CM_CM_DIR)/envopt.h
#ENV_INC=

# system service interface
SSI_INC=$(CM_CM_DIR)/ssi.h $(CM_CM_DIR)/ssi.x
#SSI_INC=

# general
GEN_INC=$(CM_CM_DIR)/gen.h $(CM_CM_DIR)/gen.x
#$(CM_DIR)/cm_gen.h $(CM_DIR)/cm_gen.x
#GEN_INC=

# all system includes
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)
#SYS_INC=

# LTE CNE layer manager - LAV includes
LAV_INC=$(CM_LAV_DIR)/lav.h $(CM_LAV_DIR)/lav.x

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
   $(CM_OS_DIR)/cm_os.h        \
   $(CM_OS_DIR)/cm_os.x        \
   $(CM_CM_DIR)/cm_err.h

# message functions
MF_INC = \
   $(CM_ASN_DIR)/cm_pasn.h        \
   $(CM_ASN_DIR)/cm_pasn.x


# Database functions
#DB_INC=

# Product - sample acceptance test includes
AV_INC=$(AV_DIR)/avsm.h $(AV_DIR)/avsm.x $(AV_DIR)/av.x

# PGW
PGW_INC=$(AV_DIR)/av.x \
        $(AV_DIR)/av.h

EGT_INC=$(CODEPATH)/egt.x

EG_INC=$(EG_DIR)/eg_util.x

# sample acceptance tests all includes
#AVALL_INC= $(AV_INC) $(EGT_INC) $(EG_INC)

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

AVALL_INC=$(PGW_INC) $(AV_INC) $(EGT_INC) $(EG_INC) $(SS_INC) $(MAKENAME)

#=======================================================================
# SECTION 7
# macros for source and object files

# acceptance test source------------------------------------------------
AV_SRCS= \
	$(AV_DIR)/avsm.c  \
	$(AV_DIR)/avsm_tst.c  \
	$(AV_DIR)/avsm_utl.c  \
	$(AV_DIR)/av_app.c  \
	$(AV_DIR)/av_tmr.c  \
	$(AV_DIR)/av_apputl.c  \
	$(AV_DIR)/av_ex_ms.c  \
	$(AV_DIR)/av_ptli.c



# acceptance test object------------------------------------------------
AV_OBJS= \
	$(SY_DIR)/av_utl.$(OBJ)  \
	$(SY_DIR)/av_tmr.$(OBJ)  \

# PGW
PGW_SRCS=\
	$(AV_DIR)/av.c \
	$(AV_DIR)/av_lim.c \
	$(AV_DIR)/av_rout.c \
	$(AV_DIR)/av_id.c \
	$(AV_DIR)/av_egt_utl.c \
	$(AV_DIR)/av_ex_ms.c

PGW_OBJS=\
	$(AV_DIR)/av.$(OBJ) \
	$(AV_DIR)/av_mi.$(OBJ) \
	$(AV_DIR)/av_rout.$(OBJ) \
	$(AV_DIR)/av_utl.$(OBJ) \
	$(AV_DIR)/av_egt_utl.$(OBJ) \
	$(AV_DIR)/av_lim.$(OBJ) \
	$(AV_DIR)/av_ptmi.$(OBJ) \
	$(AV_DIR)/av_ex_ms.$(OBJ)

EG_SRCS=\
	$(AV_DIR)/av_eu_ptli.c

EG_OBJS=\
	$(AV_DIR)/av_eu_ptli.$(OBJ) \
	$(EGLIB)

# QO stack manager objects-----------------------------------------
AVSM_OBJS= \
	$(SY_DIR)/avsm.$(OBJ)  \
	$(SY_DIR)/avsm_hicfg.$(OBJ)  \
	$(SY_DIR)/avsm_avcfg.$(OBJ)  \
	$(SY_DIR)/avsm_egcfg.$(OBJ)  \
	$(SY_DIR)/avsm_ex_ms.$(OBJ)  \
	$(SY_DIR)/avsm_tst.$(OBJ)  \
	$(SY_DIR)/avsm_utl.$(OBJ)  \
        $(SY_DIR)/smavptmi.$(OBJ)


#----------------------------------------- HI source
HI_SRCS=

#----------------------------------------- HI objects
HI_OBJS=


# common source(C header files)-----------------------------------------
PGWCMH_SRCS= \
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
	lav.h     \
	lav.x     \
	lhi.h     \
	lhi.x     \
	hit.x     \
	hit.h     \
	ssi.h     \
	ssi.x

# common source(C source files)-----------------------------------------
PGWCM_SRCS= \
	$(CM_PGW_DIR)/lav.c        \
	$(CM_CM_DIR)/cm_os.c

# common objects--------------------------------------------------------
PGWCM_OBJS= \
	$(SY_DIR)/cm_os.$(OBJ)   \
	$(SY_DIR)/lav.$(OBJ)


# common objects--------------------------------------------------------
PGWHICM_SRCS=

PGWHICM_OBJS=

# common stack manager source-------------------------------------------
SM_SRCS=

# common stack manager objects------------------------------------------
SM_OBJS=

#----------------------------------------- mos source
MS_SRCS= \
   $(OS_DIR)/ms_bdy1.c       \
   $(OS_DIR)/ms_bdy2.c       \
   $(OS_DIR)/ms_id.c         \
   $(CM_DIR)/cm_ss.c         \
   $(OS_DIR)/ms_ptmi.c       \
   $(OS_DIR)/ms_ex_ms.c

#----------------------------------------- mos objects
MS_OBJS= \
   $(SY_DIR)/ms_bdy1.$(OBJ)  \
   $(SY_DIR)/ms_bdy2.$(OBJ)  \
   $(SY_DIR)/ms_id.$(OBJ)    \
	$(SY_DIR)/cm_ss.$(OBJ)    \
   $(SY_DIR)/ms_ptmi.$(OBJ)  \
   $(SY_DIR)/ms_ex_ms.$(OBJ)

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

#------------------------------------------ MT sources
VW_SRCS= \
   $(OS_DIR)/vw_ss.c               \
   $(OS_DIR)/vw_id.c               \
   $(CM_MEM_DIR)/cm_mem.c

#------------------------------------------- MT objects
VW_OBJS= \
   $(SY_DIR)/vw_ss.$(OBJ)          \
   $(SY_DIR)/vw_id.$(OBJ)          \
   $(SY_DIR)/cm_mem.$(OBJ)

#------------------------------------------ NS sources
NS_SRCS= \
   $(NS_DIR)/ns_gen.c              \
   $(NS_DIR)/smnsexms.c            \
   $(NS_DIR)/smnsbdy1.c            \
   $(NS_DIR)/lns.c                 \
   $(NS_DIR)/ns_ex_ms.c            \
   $(NS_DIR)/ns_id.c               \
   $(NS_DIR)/ns_ptmi.c             \
   $(NS_DIR)/ns_space.c            \
   $(NS_DIR)/ns_task.c             \
   $(NS_DIR)/ns_timer.c            \
   $(CM_MEM_DIR)/cm_mem.c

#------------------------------------------- NS objects
NS_OBJS= \
   $(SY_DIR)/ns_gen.$(OBJ)         \
   $(SY_DIR)/smnsexms.$(OBJ)       \
   $(SY_DIR)/smnsbdy1.$(OBJ)       \
   $(SY_DIR)/lns.$(OBJ)            \
   $(SY_DIR)/ns_ex_ms.$(OBJ)       \
   $(SY_DIR)/ns_id.$(OBJ)          \
   $(SY_DIR)/ns_ptmi.$(OBJ)        \
   $(SY_DIR)/ns_space.$(OBJ)       \
   $(SY_DIR)/ns_task.$(OBJ)        \
   $(SY_DIR)/ns_timer.$(OBJ)       \
   $(SY_DIR)/cm_mem.$(OBJ)


# Portable Build Sources------------------------------------------------
PT_SRCS= \
	$(CM_DIR)/ss_ptsp.c

# Portable Build Objects------------------------------------------------
PT_OBJS= \
	$(SY_DIR)/ss_ptsp.$(OBJ)

# select which OS build-------------------------------------------------
#OS_SRCS=$(MS_SRCS)
#OS_OBJS=$(MS_OBJS)

#for Solaris MTSS
OS_SRCS=$(SS_SRCS) $(MT_SRCS)
OS_OBJS=$(SS_OBJS) $(MT_OBJS)

#for Windows NT MTSS
#OS_SRCS=$(SS_SRCS) $(NS_SRCS)
#OS_OBJS=$(SS_OBJS) $(NS_OBJS)

#for VxWorks
#OS_SRCS=$(SS_SRCS) $(VW_SRCS)
#OS_OBJS=$(SS_OBJS) $(VW_OBJS)

# all acceptance test source--------------------------------------------
PRDACC_SRCS= \
	$(PGW_SRCS)
#	$(AV_SRCS) \
	$(PGWCM_SRCS) \
#	$(PGWHICM_SRCS) \
#	$(HI_SRCS)    \
	$(OS_SRCS)    \

#for linux 32 bit machine
#EGLIB=libeg.a
ifeq ($(MACHINE), BIT64)
EGLIB=$(SY_DIR)/libav_lnx64eg.a
else
EGLIB=$(SY_DIR)/libav_lnx32eg.a
endif
#for linux 64 bit machine
#for solaris 32 bit machine
#EGLIB=libav_sol32eg.a
#for solaris 64 bit machine
#EGLIB=libav_sol64eg.a

# all acceptance test objects-------------------------------------------
PRDACC_OBJS= $(PGW_OBJS) \
	$(PGWHICM_OBJS)  \
	$(PGWCM_OBJS)  \
	$(AVSM_OBJS)  \
	$(SM_OBJS)     \
	$(OS_OBJS)    \
   $(EG_OBJS)

# all portable sources--------------------------------------------------
PRDPT_SRCS=  \
	$(PT_SRCS) \
	$(PGWCM_SRCS)

# all portable objects--------------------------------------------------
PRDPT_OBJS= \
	$(PGW_OBJS) \
	$(PT_OBJS) \
	$(PGWCM_OBJS)

#----------------------------------------- Klocwork analysis objects
PRDKW_OBJS= \
	$(AV_OBJS)  \
	$(PGWCM_OBJS)  \
	$(HI_OBJS)     \
	$(SM_OBJS)     \
	$(PGWSM_OBJS)   \
	$(OS_OBJS)    \
	$(PGW_OBJS)

# all library sources--------------------------------------------------
PRDLIB_SRCS=  $(AV_DIR)/jg_ex_ms.c $(PGW_SRCS)

# all library objects--------------------------------------------------
PRDLIB_OBJS= $(PGW_OBJS)

#=======================================================================
#
# special programs required for Internal Development by makefile
#
# adjust as required
#
# System Binary Dir - replace with appropriate directory and programme
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
	$(REMOVE) *.$(OBJ) $(BINACC) $(BINPT) *.ln *.s $(SY_DIR)/*.$(OBJ) \
   $(SY_DIR)/$(BINACC) $(SY_DIR)/*.ln $(SY_DIR)/*.s $(EGLIB)

win_clean:
	$(DELETE) *.$(OBJ)
	$(DELETE) $(BINACC).exe
	$(DELETE) $(BINPT).exe
	$(DELETE) *.ln
	$(DELETE) *.s

# External target for acceptance test build
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC)

# External target for portable build
pt:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OSOPTS=$(PTOSOPTS) \
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

#
#$(BINLNXINT):
#   @$(MAKE) -f $(MAKENAME) $(MAKEINT) -e ENV=$(LNXENV) CC=$(LNXCC)   \
   LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS)   \
   INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
   OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

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

$(PTBIN): $(PRDPT_OBJS)
	$(LD) $(LOPTS) $(PRDPT_OBJS) $(LIBS)

$(LIBBIN): $(PRDLIB_OBJS)
	$(LD) $(LOPTS) $(PRDLIB_OBJS)

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

$(EGLIB):
	@$(MAKE) -f av_eg.mak BLDENV=lnx_lib lib

#	@echo "Starting PGW"
ifeq ($(MACHINE), BIT64)
$(ACCBIN): $(PRDACC_OBJS)
	$(LD)  $(PRDACC_OBJS) libpcap-64bit.a $(LIBS) $(LOPTS)
else
$(ACCBIN): $(PRDACC_OBJS)
	$(LD)  $(PRDACC_OBJS) libpcap.a $(LIBS) $(LOPTS)
endif


#for 64 bit machine use this libpcap
#	@echo "Starting PGW"
#$(ACCBIN): $(PRDACC_OBJS)
#	$(LD)  $(PRDACC_OBJS) libpcap-64bit.a $(LIBS) $(LOPTS)

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

# --------------------------  acceptance test files build

$(SY_DIR)/avsm.$(OBJ): $(AV_DIR)/avsm.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm.c -o $(SY_DIR)/avsm.$(OBJ)

$(SY_DIR)/avsm_hicfg.$(OBJ): $(AV_DIR)/avsm_hicfg.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm_hicfg.c -o $(SY_DIR)/avsm_hicfg.$(OBJ)

$(SY_DIR)/avsm_avcfg.$(OBJ): $(AV_DIR)/avsm_avcfg.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm_avcfg.c -o $(SY_DIR)/avsm_avcfg.$(OBJ)

$(SY_DIR)/avsm_egcfg.$(OBJ): $(AV_DIR)/avsm_egcfg.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm_egcfg.c -o $(SY_DIR)/avsm_egcfg.$(OBJ)

$(SY_DIR)/avsm_ex_ms.$(OBJ): $(AV_DIR)/avsm_ex_ms.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm_ex_ms.c -o $(SY_DIR)/avsm_ex_ms.$(OBJ)

$(SY_DIR)/avsm_tst.$(OBJ): $(AV_DIR)/avsm_tst.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm_tst.c -o $(SY_DIR)/avsm_tst.$(OBJ)

$(SY_DIR)/avsm_utl.$(OBJ): $(AV_DIR)/avsm_utl.c $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/avsm_utl.c -o $(SY_DIR)/avsm_utl.$(OBJ)

$(SY_DIR)/lav.$(OBJ): $(CM_LAV_DIR)/lav.c $(SYS_INC) $(CM_INC) $(LAV_INC)
	$(CC) -c $(CPRDOPTS) $(CM_LAV_DIR)/lav.c -o $(SY_DIR)/lav.$(OBJ)

# product Id  file---------------------------------------------------

$(SY_DIR)/av_id.$(OBJ): $(AV_DIR)/av_id.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_id.c -o $(SY_DIR)/av_id.$(OBJ)


# PGW build files
$(SY_DIR)/av.$(OBJ): $(AV_DIR)/av.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av.c -o $(SY_DIR)/av.$(OBJ)

$(SY_DIR)/av_lim.$(OBJ): $(AV_DIR)/av_lim.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_lim.c -o $(SY_DIR)/av_lim.$(OBJ)

$(SY_DIR)/av_ptmi.$(OBJ): $(AV_DIR)/av_ptmi.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_ptmi.c -o $(SY_DIR)/av_ptmi.$(OBJ)

$(SY_DIR)/av_rout.$(OBJ): $(AV_DIR)/av_rout.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_rout.c -o $(SY_DIR)/av_rout.$(OBJ)

$(SY_DIR)/av_mi.$(OBJ): $(AV_DIR)/av_mi.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_mi.c -o $(SY_DIR)/av_mi.$(OBJ)

$(SY_DIR)/av_egt_utl.$(OBJ): $(AV_DIR)/av_egt_utl.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_egt_utl.c -o $(SY_DIR)/av_egt_utl.$(OBJ)

$(SY_DIR)/av_utl.$(OBJ): $(AV_DIR)/av_utl.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_utl.c -o $(SY_DIR)/av_utl.$(OBJ)

$(SY_DIR)/av_ex_ms.$(OBJ): $(AV_DIR)/av_ex_ms.c $(AVALL_INC) $(AVALL_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_ex_ms.c -o $(SY_DIR)/av_ex_ms.$(OBJ)

# EGTP build files
$(SY_DIR)/av_eu_ptli.$(OBJ): $(AV_DIR)/av_eu_ptli.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(AV_DIR)/av_eu_ptli.c -o $(SY_DIR)/av_eu_ptli.$(OBJ)

#----------------------------------------- HI Layer objects


# stack manager files build---------------------------------------------

$(SY_DIR)/sm_ex_ms.$(OBJ): $(CM_SM_DIR)/sm_ex_ms.c $(SYS_INC) $(LAV_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(CM_SM_DIR)/sm_ex_ms.c -o $(SY_DIR)/sm_ex_ms.$(OBJ)


# common library files

$(SY_DIR)/smavptmi.$(OBJ): $(CM_SM_DIR)/smavptmi.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(CSMOPTS) $(CM_SM_DIR)/smavptmi.c -o $(SY_DIR)/smavptmi.$(OBJ)

$(SY_DIR)/smegptmi.$(OBJ): $(CM_SM_DIR)/smegptmi.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(CSMOPTS) $(CM_SM_DIR)/smegptmi.c -o $(SY_DIR)/smegptmi.$(OBJ)

$(SY_DIR)/cm_hash.$(OBJ): $(CM_HASH_DIR)/cm_hash.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_HASH_DIR)/cm_hash.c -o $(SY_DIR)/cm_hash.$(OBJ)

$(SY_DIR)/cm_llist.$(OBJ): $(CM_LLIST_DIR)/cm_llist.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_LLIST_DIR)/cm_llist.c -o $(SY_DIR)/cm_llist.$(OBJ)

$(SY_DIR)/cm_lib.$(OBJ): $(CM_LIB_DIR)/cm_lib.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_LIB_DIR)/cm_lib.c -o $(SY_DIR)/cm_lib.$(OBJ)

$(SY_DIR)/cm_bdy5.$(OBJ): $(CM_TMR_DIR)/cm_bdy5.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_TMR_DIR)/cm_bdy5.c -o $(SY_DIR)/cm_bdy5.$(OBJ)

$(SY_DIR)/cm_tpt.$(OBJ): $(CM_TMR_DIR)/cm_tpt.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_TMR_DIR)/cm_tpt.c -o $(SY_DIR)/cm_tpt.$(OBJ)

$(SY_DIR)/hit.$(OBJ): $(CM_TMR_DIR)/hit.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_TMR_DIR)/hit.c -o $(SY_DIR)/hit.$(OBJ)

$(SY_DIR)/lhi.$(OBJ): $(HI_DIR)/lhi.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(HI_DIR)/lhi.c -o $(SY_DIR)/lhi.$(OBJ)


$(SY_DIR)/cm_gen.$(OBJ): $(CM_CM_DIR)/cm_gen.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_CM_DIR)/cm_gen.c -o $(SY_DIR)/cm_gen.$(OBJ)

$(SY_DIR)/cm_ss7.$(OBJ): $(CM_SS7_DIR)/cm_ss7.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_SS7_DIR)/cm_ss7.c -o $(SY_DIR)/cm_ss7.$(OBJ)

$(SY_DIR)/cm_tkns.$(OBJ): $(CM_CM_DIR)/cm_tkns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_CM_DIR)/cm_tkns.c -o $(SY_DIR)/cm_tkns.$(OBJ)

$(SY_DIR)/cm_os.$(OBJ): $(CM_CM_DIR)/cm_os.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_CM_DIR)/cm_os.c -o $(SY_DIR)/cm_os.$(OBJ)

$(SY_DIR)/cm_dns.$(OBJ): $(CM_DNS_DIR)/cm_dns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DNS_DIR)/cm_dns.c -o $(SY_DIR)/cm_dns.$(OBJ)

$(SY_DIR)/cm_inet.$(OBJ): $(CM_INET_DIR)/cm_inet.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_INET_DIR)/cm_inet.c -o $(SY_DIR)/cm_inet.$(OBJ)

$(SY_DIR)/cm_mblk.$(OBJ): $(CM_MEM_DIR)/cm_mblk.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_MEM_DIR)/cm_mblk.c -o $(SY_DIR)/cm_mblk.$(OBJ)

# mos files build-------------------------------------------------------

$(SY_DIR)/ms_bdy1.$(OBJ): $(OS_DIR)/ms_bdy1.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_bdy1.c -o $(SY_DIR)/ms_bdy1.$(OBJ)

$(SY_DIR)/ms_bdy2.$(OBJ): $(SY_DIR)/ms_bdy2.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(SY_DIR)/ms_bdy2.c -o $(SY_DIR)/ms_bdy2.$(OBJ)

$(SY_DIR)/ms_ptmi.$(OBJ): $(OS_DIR)/ms_ptmi.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_ptmi.c -o $(SY_DIR)/ms_ptmi.$(OBJ)

$(SY_DIR)/ms_id.$(OBJ): $(OS_DIR)/ms_id.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_id.c -o $(SY_DIR)/ms_id.$(OBJ)

$(SY_DIR)/ms_ex_ms.$(OBJ): $(OS_DIR)/ms_ex_ms.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_ex_ms.c -o $(SY_DIR)/ms_ex_ms.$(OBJ)

$(SY_DIR)/cm_ss.$(OBJ): $(CM_DIR)/cm_ss.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CMSOPTS) $(CM_DIR)/cm_ss.c -o $(SY_DIR)/cm_ss.$(OBJ)

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

# MTSS-Windows NT-------------------------------------------------------
$(SY_DIR)/ns_gen.$(OBJ): $(OS_DIR)/ns_gen.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_gen.c -o $(SY_DIR)/ns_gen.$(OBJ)

$(SY_DIR)/smnsbdy1.$(OBJ): $(OS_DIR)/smnsbdy1.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(CSMOSOPTS) $(OS_DIR)/smnsbdy1.c -o $(SY_DIR)/smnsbdy1.$(OBJ)

$(SY_DIR)/smnsexms.$(OBJ): $(OS_DIR)/smnsexms.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(CSMOSOPTS) $(OS_DIR)/smnsexms.c -o $(SY_DIR)/smnsexms.$(OBJ)

$(SY_DIR)/lns.$(OBJ): $(OS_DIR)/lns.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/lns.c -o $(SY_DIR)/lns.$(OBJ)

$(SY_DIR)/ns_ex_ms.$(OBJ): $(OS_DIR)/ns_ex_ms.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_ex_ms.c -o $(SY_DIR)/ns_ex_ms.$(OBJ)

$(SY_DIR)/ns_id.$(OBJ): $(OS_DIR)/ns_id.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_id.c -o $(SY_DIR)/ns_id.$(OBJ)

$(SY_DIR)/ns_ptmi.$(OBJ): $(OS_DIR)/ns_ptmi.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_ptmi.c -o $(SY_DIR)/ns_ptmi.$(OBJ)

$(SY_DIR)/ns_space.$(OBJ): $(OS_DIR)/ns_space.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_space.c -o $(SY_DIR)/ns_space.$(OBJ)

$(SY_DIR)/ns_task.$(OBJ): $(OS_DIR)/ns_task.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_task.c -o $(SY_DIR)/ns_task.$(OBJ)

$(SY_DIR)/ns_timer.$(OBJ): $(OS_DIR)/ns_timer.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_timer.c -o $(SY_DIR)/ns_timer.$(OBJ)

# vxWorks----------------------------------------------------------
$(SY_DIR)/vw_ss.$(OBJ): $(OS_DIR)/vw_ss.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_ss.c -o $(SY_DIR)/vw_ss.$(OBJ)

$(SY_DIR)/vw_id.$(OBJ): $(OS_DIR)/vw_id.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_id.c -o $(SY_DIR)/vw_id.$(OBJ)

# portable files build--------------------------------------------------

$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_CM_DIR)/ss_ptsp.c
	$(CC) -c $(CPTOPTS) $(CM_CM_DIR)/ss_ptsp.c -o $(SY_DIR)/ss_ptsp.$(OBJ)

#
#********************************************************************30**
#
#        End of file:     av.mak@@/main/1 - Tue Aug 17 18:54:38 2010
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
#/main/1      ---     rp   1. LTE-CNE Initial Release.
#/main/1      av001.101 rp 1. For releasing patch
#********************************************************************91*/

#DO NOT DELETE THIS LINE -- make depend depends on it.
