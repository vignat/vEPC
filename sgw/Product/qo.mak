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
#               various configurations. Further comments are
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
#       Sid:      qo.mak@@/main/1 - Thu Jul  8 21:22:15 2010
#
#       Prg:     ap
#
#********************************************************************21*/
#=======================================================================
#
# macros for product specific options

# commented for RTR

# for loose coupling
PRDOPTS=-DQO -DEG -DHI -DLCSMEGMILEG -DLCEGMILEG -DLCEULIEGT \
	-DLCHIUIHIT -DLCHIMILHI -DLCSMHIMILHI -DLCLHI -DLCEGT -DLCLQO \
	-DLCQOMILQO -DLCSMMILQO -DQO_USTA -DLCEULIEGT -DLWLCEULIEGT \
	-DLCEGUIEGT -DLWLCEGUIEGT -DDSATPA -DUSE_PURE -DPURE

# Product options for portable build
PRDPTOPTS=$(PRDOPTS) -DCMFILE_REORG_1 -DCM_INET2

EGOPTS=-DEGTP_C -DEGTP_U -DEU -DLWLCEGT -DEG_REL_930

# -------------- Product options for acceptance test build
PRDACCOPTS=$(PRDOPTS) $(EGOPTS) -DSM -DCM_INET2 -DLCSULISCT \
	-DxSS_MULTIPLE_PROCS -DHI -DHI_MULTI_THREADED -DxHI_CMPTBL_FLAG \
	-DQO -DLCLQO -DLCQOMILQO -DLCSMMILQO -DDBG4 -DCMFILE_REORG_1 \
	-DCM_INET2 -UQO_DEBUG -DDEBUGP -DQO_SGW -DSM -DCNS_PH1 -DLCSMQOMILQO \
	-DLCSMMILQO -DLCHIUIHIT -DLCHIMILHI -DLCSMHIMILHI -DLCLHI

#
# -------------- CSB OPTS for acceptance test build
CSBOPTS=--DHI_REL_1_4 -DCM_INET2 -DLCSCT -DHI
#
# -------------- HI OPTS for acceptance test build
CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DHI_MULTI_THREADED

#for KSCTP
#CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DHI_MULTI_THREADED -DLCHIUISCT -DPTHIUISCT

# Product options for library build
PRDLIBOPTS=$(PRDOPTS) -DDBG4 -DCMFILE_REORG_1 -DCM_INET2 -UQO_DEBUG -DDEBUGP -DQO_SGW -DSM\
           $(EGOPTS)

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
MTOSOPTS=-DSS_MT -USS_PERF -D__EXTENSIONS__ -DDEBUGNOEXIT -USS_M_PROTO_REGION -USS_MULTICORE_SUPPORT -USSI_MEM_CHNG -DSLES9_PLUS

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
BASENAME=qo

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

# Build with the Insite Memory Check programe - Acceptance Tests only
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
#CM_CODE_DIR=$(CODEPATH)/cm
CM_CODE_DIR=.
#CM_SS_CODE_DIR=/u/prd/rtr/mt
#CM_SS_CODE_DIR=../mt_mlti
CM_SS_CODE_DIR=.
#Windows
#CM_SS_CODE_DIR=.
SAMPLE_CODE_DIR=$(CODEPATH)
#SM_CODE_DIR=$(CODEPATH)/sgw
SM_CODE_DIR=.

# common files directory
CM_CM_DIR=.

CM_LEG_DIR=$(SM_CODE_DIR)
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
CM_SHT_DIR=$(CM_CODE_DIR)
CM_HIT_DIR=$(CM_CODE_DIR)
CM_LLIST_DIR=$(CM_CODE_DIR)
CM_INET_DIR=$(CM_CODE_DIR)
CM_SM_DIR=$(SM_CODE_DIR)
#CM_PYTHON_DIR=/usr/ccpu/python/solaris/10/32bit/python-2.4.4/include/python2.4
#PYTHON_PATH_WIN = c:\Python24\include


# TUCL directory
#HI_DIR=$(CODEPATH)/tucl
HI_DIR=.

# common layer manager interface files directory
CM_LQO_DIR=$(QO_DIR)

# SCCP directory
SP_DIR=.

# stack manager directory
SM_DIR=$(SAMPLE_CODE_DIR)

# system agent directory
SH_DIR=$(SAMPLE_CODE_DIR)

EG_DIR=.

# OS directory
#
# MS_DIR=/u/sw/tds/ms/crnt
# NS_DIR=../ns
# VW_DIR=../mt

#SS_DIR=/u/prd/rtr/mt
#SS_DIR=.
#windows
#SS_DIR=$(NS_DIR)
#MT_DIR=$(CODEPATH)/mt
MT_DIR=.
SS_DIR=$(MT_DIR)
#MT_DIR=.

# OS Directory for Solaris MTSS
#OS_DIR=$(CODEPATH)/mt
OS_DIR=.

# OS Directory for Windows
#OS_DIR=.

# OS Directory for VxWorks
#OS_DIR= $(SS_DIR) $(VW_DIR)

# acceptance directory
QO_DIR=.

# build directory
SY_DIR=.

# include path

# portable include path
PT_INC_PATH="-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) -I$(CM_TMR_DIR) \
   -I$(CM_ASN_DIR) -I$(CM_SS7_DIR) -I$(CM_RPT_DIR) -I$(CM_SHT_DIR) -I$(CM_SPT_DIR) \
   -I$(SP_DIR) -I$(CM_MEM_DIR) -I$(CM_LLIST_DIR)"

# library include path
LIB_INC_PATH="-I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR)"

# Acceptance test include path
# ACC_INC_PATH=" -I/tools/wind/target/h -I$(CM_DIR) -I$(QO_DIR) \
#              -I$(CM_SM_DIR) -I$(OS_DIR) -I$(HI_DIR)"
#
ACC_INC_PATH="-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) \
   -I$(CM_TMR_DIR) -I$(CM_ASN_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) -I$(CM_SHT_DIR) \
   -I$(QO_DIR) -I$(CM_SM_DIR) -I$(OS_DIR) -I$(HI_DIR) \
   -I$(CM_SS7_DIR) -I$(CM_LLIST_DIR) -I$(CM_MEM_DIR)\
   -I$(CM_PYTHON_DIR)-I$(PYTHON_PATH_WIN) -I$(SS_DIR) -I$(MT_DIR)"

# OS - files include path
MSINC_PATH=-I$(CM_DIR) -I$(OS_DIR)

# library path - used for integrated products and simulation env.
LIB_PATH=/tmp

EGLIB=$(SY_DIR)/libqo_eg.a
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
SOCKLIBS="-lnsl -L/usr/lib/nptl -lpthread -lpython2.4 "

# For 32 Bit Compilation
#SOLCC="gcc"
SOLCC="cc"
#SOLLD="gcc"
SOLLD="cc"
SOLLIBLD="/usr/ccs/bin/ld"
SOLLOPTS="-o $(OUTNAME) -L/usr/lib/nptl -lpthread -lposix4 -lnsl -lpython2.4 -lsocket -lm -ldl"

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
GNUCOPTS="-g3 -Wimplicit -Wunused -Wcomment -Wchar-subscripts \
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
LNXENV="-DSUNOS   -DSS_LINUX -DANSI -D_GNU_SOURCE"
#LNXENV="-DSUNOS   -DSS_LINUX "

ifeq ($(MACHINE), BIT64)
LNXCOPTS="-g -march=core2 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment \
	-pipe -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes \
	-Wuninitialized  -Wunused "
else
LNXCOPTS=" -g3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused "
endif


# For non-ANSI Compilation
#LNXENV="-DSUNOS"
#LNXENV="-DSUNOS -DSS_LINUX"
#LNXCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
          -Wcast-align -Wcast-qual"

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
LNXLIBS="-L. -Xlinker -export-dynamic -lpthread -lnsl -lm -lrt -lutil -ldl"
#for KSCTP
#LNXLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lm -lrt -lutil -ldl -lsctp"
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"

# For 64 bit compilation
#LNXCC="gcc -m64 -DALIGN_64BIT -DBIT_64"
#LNXLD="gcc -m64"
#LNXCOPTS="-g3 -ansi -Wall -fno-strict-aliasing -Wno-comment -pipe -Wshadow -Wcast-qual \
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
GEN_INC=$(CM_CM_DIR)/gen.h $(CM_CM_DIR)/gen.x  $(QO_DIR)/qo.x $(QO_DIR)/qo.h
#$(CM_DIR)/cm_gen.h $(CM_DIR)/cm_gen.x
#GEN_INC=

# all system includes
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)
#SYS_INC=

# Product layer management - LEG includes
LEG_INC=$(EG_DIR)/leg.h $(EG_DIR)/leg.x

# LTE CNE layer manager - LQO includes
LQO_INC=$(SM_CODE_DIR)/lqo.h $(SM_CODE_DIR)/lqo.x

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

HI_INC=$(HI_DIR)/hi.h $(HI_DIR)/hi.x

# ALL product includes

# Product - sample acceptance test includes
QO_INC=$(QO_DIR)/qosm.h $(QO_DIR)/qosm.x

# SGW
#SGW_INC=$(QO_DIR)/qo.x $(CODEPATH)/cm/lqo.x
SGW_INC=$(QO_DIR)/qo.x $(CODEPATH)/lqo.x

#EGT_INC=$(CODEPATH)/cm/egt.x $(EG_DIR)/egt_util.x
EGT_INC=./egt.x $(EG_DIR)/egt_util.x

EG_INC=$(EG_DIR)/eg_util.x $(QO_DIR)/qo_eu.x

# sample acceptance tests all includes

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

QOALL_INC=$(QO_INC) $(EGT_INC) $(EG_INC) $(SS_INC) $(SCT_INC) $(MAKENAME)

#=======================================================================
# SECTION 7
# macros for source and object files


#EG_SRCS=\
	$(EG_DIR)/qo_eu_ex_ms.c \
	$(EG_DIR)/qo_eu_ptli.c \
	$(QO_DIR)/egt_util.c
EG_SRCS=\
	$(QO_DIR)/qo_eu_ex_ms.c \
	$(QO_DIR)/qo_eu_ptli.c \
	$(EG_DIR)/egt_util.c

#	$(EG_DIR)/eg_util.$(OBJ) \
	$(EG_DIR)/egt.$(OBJ) \

# EG stack manager objects-----------------------------------------
EGSM_OBJS= \
   $(SY_DIR)/smegexms.$(OBJ) \
   $(SY_DIR)/smegptmi.$(OBJ)

# QO stack manager objects-----------------------------------------
QOSM_OBJS= \
   $(SY_DIR)/smqoexms.$(OBJ) \
   $(SY_DIR)/smqoptmi.$(OBJ)

EG_OBJS=\
	$(EG_DIR)/qo_eu_ptli.$(OBJ) \
	$(EG_DIR)/qo_eu_ex_ms.$(OBJ) \
	$(SY_DIR)/egt_util.$(OBJ) \
	$(EGLIB)

# SGW
SGW_SRCS=\
	$(QO_DIR)/qo_app.c \
	$(QO_DIR)/qo_li.c \
	$(QO_DIR)/qo_utl.c \
	$(QO_DIR)/qo_ex_ms.c \
	$(QO_DIR)/qo_mi.c \
	$(QO_DIR)/qo_egt_utl.c \
	$(QO_DIR)/qo_ptmi.c \
	$(QO_DIR)/qosm.c  \
	$(QO_DIR)/qosm_egcfg.c  \
	$(QO_DIR)/qosm_hicfg.c  \
	$(QO_DIR)/qosm_qocfg.c  \
	$(QO_DIR)/qosm_utl.c  \
	$(QO_DIR)/qosm_tst.c  \
	$(QO_DIR)/qosm_ex_ms.c


SGW_OBJS=\
	$(SY_DIR)/qo_app.$(OBJ) \
	$(SY_DIR)/qo_li.$(OBJ) \
	$(SY_DIR)/qo_utl.$(OBJ) \
	$(SY_DIR)/qo_ex_ms.$(OBJ) \
	$(SY_DIR)/qo_mi.$(OBJ) \
	$(SY_DIR)/qo_egt_utl.$(OBJ) \
	$(SY_DIR)/qo_ptmi.$(OBJ) \
	$(SY_DIR)/qosm.$(OBJ)  \
	$(SY_DIR)/qosm_egcfg.$(OBJ)  \
	$(SY_DIR)/qosm_hicfg.$(OBJ)  \
	$(SY_DIR)/qosm_qocfg.$(OBJ)  \
	$(SY_DIR)/qosm_utl.$(OBJ)  \
	$(SY_DIR)/qosm_tst.$(OBJ)  \
	$(SY_DIR)/qosm_ex_ms.$(OBJ)


#----------------------------------------- HI source
HI_SRCS= \
	$(HI_DIR)/hi_bdy1.c       \
	$(HI_DIR)/hi_bdy2.c       \
	$(HI_DIR)/hi_bdy3.c       \
	$(HI_DIR)/hi_ex_ms.c      \
	$(HI_DIR)/hi_ptmi.c       \
	$(HI_DIR)/hi_ptui.c       \
	$(HI_DIR)/hi_id.c

#----------------------------------------- HI objects
HI_OBJS= \
	$(SY_DIR)/hi_bdy1.$(OBJ)  \
	$(SY_DIR)/hi_bdy2.$(OBJ)  \
	$(SY_DIR)/hi_bdy3.$(OBJ)  \
	$(SY_DIR)/hi_ex_ms.$(OBJ)  \
	$(SY_DIR)/hi_ptmi.$(OBJ)  \
	$(SY_DIR)/hi_ptui.$(OBJ)  \
	$(SY_DIR)/hi_id.$(OBJ)

# common source(C header files)-----------------------------------------
CMH_SRCS= \
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
	lqo.h     \
	lqo.x     \
	lhi.h     \
	lhi.x     \
	hit.x     \
	hit.h     \
	ssi.h     \
	ssi.x

# common source(C source files)-----------------------------------------
CM_SRCS= \
	$(CM_CM_DIR)/cm_gen.c      \
	$(CM_LIB_DIR)/cm_lib.c     \
	$(CM_HASH_DIR)/cm_hash.c   \
	$(CM_LLIST_DIR)/cm_llist.c \
	$(CM_CM_DIR)/cm_tkns.c     \
	$(CM_DNS_DIR)/cm_dns.c     \
	$(CM_INET_DIR)/cm_inet.c   \
	$(CM_SCT_DIR)/sct.c        \
	$(CM_TMR_DIR)/cm_bdy5.c    \
	$(CM_TPT_DIR)/cm_tpt.c     \
	$(CM_LQO_DIR)/lqo.c        \

#	$(CM_SS7_DIR)/cm_ss7.c     \

# common objects--------------------------------------------------------
CM_OBJS= \
	$(SY_DIR)/cm_gen.$(OBJ)    \
	$(SY_DIR)/cm_tkns.$(OBJ)   \
	$(SY_DIR)/cm_lib.$(OBJ)    \
	$(SY_DIR)/cm_hash.$(OBJ)   \
	$(SY_DIR)/cm_llist.$(OBJ)  \
	$(SY_DIR)/cm_dns.$(OBJ)  	\
	$(SY_DIR)/cm_os.$(OBJ)  	\
	$(SY_DIR)/cm_inet.$(OBJ)  	\
	$(SY_DIR)/cm_mblk.$(OBJ)   \
	$(SY_DIR)/cm_bdy5.$(OBJ)   \
	$(SY_DIR)/cm_tpt.$(OBJ)    \
	$(SY_DIR)/lqo.$(OBJ)

# common objects--------------------------------------------------------
HICM_SRCS= \
	$(CM_HIT_DIR)/hit.c        \
	$(HI_DIR)/lhi.c

HICM_OBJS= \
	$(SY_DIR)/hit.$(OBJ)      \
	$(SY_DIR)/lhi.$(OBJ)


# common stack manager source-------------------------------------------
SM_SRCS= \
	$(CM_SM_DIR)/sm_bdy1.c 	\
	$(CM_SM_DIR)/sm_ex_ms.c \
	$(CM_SM_DIR)/smhiexms.c \
	$(CM_SM_DIR)/smhiptmi.c \

# common stack manager objects------------------------------------------
SM_OBJS= \
	$(SY_DIR)/smhiexms.$(OBJ) \
	$(SY_DIR)/smhiptmi.$(OBJ) \
#	$(SY_DIR)/sm_ex_ms.$(OBJ) \
	$(SY_DIR)/sm_bdy1.$(OBJ) \
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
	$(SGW_SRCS) \
	$(EG_SRCS)  \
	$(HI_SRCS)    \
	$(OS_SRCS)    \
	$(SM_SRCS)
#	$(QO_SRCS) \
	$(CM_SRCS) \
	$(HICM_SRCS) \
	$(CM_SRCS)  \
	$(SH_SRCS)

# all acceptance test objects-------------------------------------------
PRDACC_OBJS= \
	$(SGW_OBJS) \
	$(EG_OBJS)  \
	$(EGSM_OBJS)  \
	$(QOSM_OBJS)  \
	$(HI_OBJS)     \
	$(OS_OBJS)    \
	$(CM_OBJS)  \
	$(HICM_OBJS)  \
	$(SM_OBJS)
#	$(QO_OBJS)  \
	$(CM_OBJS)  \
	$(SM_OBJS)   \

# all portable sources--------------------------------------------------
PRDPT_SRCS=  \
	$(PT_SRCS) \
	$(CM_SRCS)

# all portable objects--------------------------------------------------
PRDPT_OBJS= \
	$(PT_OBJS) \
	$(CM_OBJS)

#----------------------------------------- Klocwork analysis objects
PRDKW_OBJS= \
	$(QO_OBJS)  \
	$(CM_XTA_OBJS) \
	$(CM_OBJS)  \
	$(HI_OBJS)     \
	$(SM_OBJS)     \
	$(SM_OBJS)   \
	$(OS_OBJS)    \

# all library sources--------------------------------------------------
PRDLIB_SRCS=\
	$(SGW_SRCS)

# all library objects--------------------------------------------------
PRDLIB_OBJS=\
	$(SGW_OBJS)

#=======================================================================
#
# special programs required for Internal Development by makefile
#
# adjust as required
#
# System Binary Dir - replace with appropriate directory and programe
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
      $(SY_DIR)/$(BINACC) $(SY_DIR)/*.ln $(SY_DIR)/*.s libqo_eg.a

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
	$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(LNXCC) \
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

$(ACCBIN): $(PRDACC_OBJS)
	$(LD)  $(PRDACC_OBJS) $(LIBS) $(LOPTS)

$(INTBIN): $(PRDINT_OBJS)
	$(LD)  $(PRDINT_OBJS) $(LIBS) $(LOPTS)

$(MAKELIB):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN) CPRDOPTS=$(CPRDLIBOPTS) \
    CSMOPTS=$(CSMLIBOPTS)

$(EGLIB):
	@$(MAKE) -f qo_eg.mak BLDENV=lnx_lib lib

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

$(SY_DIR)/qosm.$(OBJ): $(QO_DIR)/qosm.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm.c -o $(SY_DIR)/qosm.$(OBJ)

$(SY_DIR)/qosm_hicfg.$(OBJ): $(QO_DIR)/qosm_hicfg.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm_hicfg.c -o $(SY_DIR)/qosm_hicfg.$(OBJ)

$(SY_DIR)/qosm_qocfg.$(OBJ): $(QO_DIR)/qosm_qocfg.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm_qocfg.c -o $(SY_DIR)/qosm_qocfg.$(OBJ)

$(SY_DIR)/qosm_tst.$(OBJ): $(QO_DIR)/qosm_tst.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm_tst.c -o $(SY_DIR)/qosm_tst.$(OBJ)

$(SY_DIR)/qosm_ex_ms.$(OBJ): $(QO_DIR)/qosm_ex_ms.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm_ex_ms.c -o $(SY_DIR)/qosm_ex_ms.$(OBJ)

$(SY_DIR)/qosm_utl.$(OBJ): $(QO_DIR)/qosm_utl.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm_utl.c -o $(SY_DIR)/qosm_utl.$(OBJ)

$(SY_DIR)/qo_apputl.$(OBJ): $(QO_DIR)/qo_apputl.c $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_apputl.c -o $(SY_DIR)/qo_apputl.$(OBJ)

$(SY_DIR)/lqo.$(OBJ): $(CM_LQO_DIR)/lqo.c $(SYS_INC) $(CM_INC) $(LQO_INC)
	$(CC) -c $(CPRDOPTS) $(CM_LQO_DIR)/lqo.c -o $(SY_DIR)/lqo.$(OBJ)

# SGW build files
$(SY_DIR)/qo_app.$(OBJ): $(QO_DIR)/qo_app.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_app.c -o $(SY_DIR)/qo_app.$(OBJ)

$(SY_DIR)/qo_li.$(OBJ): $(QO_DIR)/qo_li.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_li.c -o $(SY_DIR)/qo_li.$(OBJ)

$(SY_DIR)/qo_utl.$(OBJ): $(QO_DIR)/qo_utl.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_utl.c -o $(SY_DIR)/qo_utl.$(OBJ)

$(SY_DIR)/qo_mi.$(OBJ): $(QO_DIR)/qo_mi.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_mi.c -o $(SY_DIR)/qo_mi.$(OBJ)

$(SY_DIR)/qo_egt_utl.$(OBJ): $(QO_DIR)/qo_egt_utl.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_egt_utl.c -o $(SY_DIR)/qo_egt_utl.$(OBJ)

$(SY_DIR)/qo_ptmi.$(OBJ): $(QO_DIR)/qo_ptmi.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_ptmi.c -o $(SY_DIR)/qo_ptmi.$(OBJ)

$(SY_DIR)/qo_ex_ms.$(OBJ): $(QO_DIR)/qo_ex_ms.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_ex_ms.c -o $(SY_DIR)/qo_ex_ms.$(OBJ)

$(SY_DIR)/qo_esm_gtp.$(OBJ): $(QO_DIR)/qo_esm_gtp.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qo_esm_gtp.c -o $(SY_DIR)/qo_esm_gtp.$(OBJ)

$(SY_DIR)/qosm_egcfg.$(OBJ): $(QO_DIR)/qosm_egcfg.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/qosm_egcfg.c -o $(SY_DIR)/qosm_egcfg.$(OBJ)

# eg build files
$(SY_DIR)/eg_util.$(OBJ): $(EG_DIR)/eg_util.c $(QOALL_INC) $(QOALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_util.c -o $(SY_DIR)/eg_util.$(OBJ)

$(SY_DIR)/egt.$(OBJ): $(EG_DIR)/egt.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/egt.c -o $(SY_DIR)/egt.$(OBJ)

$(SY_DIR)/qo_eu_ptli.$(OBJ): $(EG_DIR)/qo_eu_ptli.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/qo_eu_ptli.c -o $(SY_DIR)/qo_eu_ptli.$(OBJ)

$(SY_DIR)/egt_util.$(OBJ): $(EG_DIR)/egt_util.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/egt_util.c -o $(SY_DIR)/egt_util.$(OBJ)

$(SY_DIR)/qo_eu_ex_ms.$(OBJ): $(EG_DIR)/qo_eu_ex_ms.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/qo_eu_ex_ms.c -o $(SY_DIR)/qo_eu_ex_ms.$(OBJ)

#----------------------------------------- HI Layer objects

$(SY_DIR)/hi_bdy1.$(OBJ): $(HI_DIR)/hi_bdy1.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_bdy1.c -o $(SY_DIR)/hi_bdy1.$(OBJ)

$(SY_DIR)/hi_bdy2.$(OBJ): $(HI_DIR)/hi_bdy2.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_bdy2.c -o $(SY_DIR)/hi_bdy2.$(OBJ)

$(SY_DIR)/hi_bdy3.$(OBJ): $(HI_DIR)/hi_bdy3.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_bdy3.c -o $(SY_DIR)/hi_bdy3.$(OBJ)

$(SY_DIR)/hi_ex_ms.$(OBJ): $(HI_DIR)/hi_ex_ms.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_ex_ms.c -o $(SY_DIR)/hi_ex_ms.$(OBJ)

$(SY_DIR)/hi_id.$(OBJ): $(HI_DIR)/hi_id.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_id.c -o $(SY_DIR)/hi_id.$(OBJ)

$(SY_DIR)/hi_ptui.$(OBJ): $(HI_DIR)/hi_ptui.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_ptui.c -o $(SY_DIR)/hi_ptui.$(OBJ)

$(SY_DIR)/hi_ptmi.$(OBJ): $(HI_DIR)/hi_ptmi.c $(SYS_INC) $(LHI_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_ptmi.c -o $(SY_DIR)/hi_ptmi.$(OBJ)

$(SY_DIR)/hi_tst.$(OBJ): $(HI_DIR)/hi_tst.c $(SYS_INC) $(LHI_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_tst.c -o $(SY_DIR)/hi_tst.$(OBJ)

$(SY_DIR)/hi_ex_pt.$(OBJ): $(HI_DIR)/hi_ex_pt.c $(SYS_INC) $(LHI_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(HI_DIR)/hi_ex_pt.c -o $(SY_DIR)/hi_ex_pt.$(OBJ)

# stack manager files build---------------------------------------------

$(SY_DIR)/sm_bdy1.$(OBJ): $(CM_SM_DIR)/sm_bdy1.c $(SYS_INC) $(LQO_INC)
	$(CC) -c $(CSMOPTS) $(CM_SM_DIR)/sm_bdy1.c -o $(SY_DIR)/sm_bdy1.$(OBJ)

$(SY_DIR)/sm_ex_ms.$(OBJ): $(CM_SM_DIR)/sm_ex_ms.c $(SYS_INC) $(LQO_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(CM_SM_DIR)/sm_ex_ms.c -o $(SY_DIR)/sm_ex_ms.$(OBJ)

$(SY_DIR)/smegptmi.$(OBJ): $(EG_DIR)/smegptmi.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(EG_DIR)/smegptmi.c -o $(SY_DIR)/smegptmi.$(OBJ)

#QO Stack Manager Files
$(SY_DIR)/smqoexms.$(OBJ): $(QO_DIR)/smqoexms.c $(SYS_INC) $(LQO_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(QO_DIR)/smqoexms.c -o $(SY_DIR)/smqoexms.$(OBJ)

$(SY_DIR)/smqoptmi.$(OBJ): $(QO_DIR)/smqoptmi.c $(SYS_INC) $(LQO_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(QO_DIR)/smqoptmi.c -o $(SY_DIR)/smqoptmi.$(OBJ)

#EGTP Stack Manager Files
$(SY_DIR)/smegexms.$(OBJ): $(EG_DIR)/smegexms.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(EG_DIR)/smegexms.c -o $(SY_DIR)/smegexms.$(OBJ)

$(SY_DIR)/smhiexms.$(OBJ): $(CM_SM_DIR)/smhiexms.c $(SYS_INC) $(LQO_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(CSMOPTS) $(CM_SM_DIR)/smhiexms.c -o $(SY_DIR)/smhiexms.$(OBJ)

$(SY_DIR)/smhiptmi.$(OBJ): $(CM_SM_DIR)/smhiptmi.c $(SYS_INC) $(LQO_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(CSMOPTS) $(CM_SM_DIR)/smhiptmi.c -o $(SY_DIR)/smhiptmi.$(OBJ)

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

$(SY_DIR)/cm_dns.$(OBJ): $(CM_DNS_DIR)/cm_dns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DNS_DIR)/cm_dns.c -o $(SY_DIR)/cm_dns.$(OBJ)

$(SY_DIR)/cm_os.$(OBJ): $(QO_DIR)/cm_os.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(QO_DIR)/cm_os.c -o $(SY_DIR)/cm_os.$(OBJ)

$(SY_DIR)/cm_inet.$(OBJ): $(CM_INET_DIR)/cm_inet.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_INET_DIR)/cm_inet.c -o $(SY_DIR)/cm_inet.$(OBJ)

$(SY_DIR)/cm_pasn1.$(OBJ): $(CM_ASN_DIR)/cm_pasn1.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_ASN_DIR)/cm_pasn1.c -o $(SY_DIR)/cm_pasn1.$(OBJ)

$(SY_DIR)/cm_pasn2.$(OBJ): $(CM_ASN_DIR)/cm_pasn2.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_ASN_DIR)/cm_pasn2.c -o $(SY_DIR)/cm_pasn2.$(OBJ)

$(SY_DIR)/cm_pasn3.$(OBJ): $(CM_ASN_DIR)/cm_pasn3.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_ASN_DIR)/cm_pasn3.c -o $(SY_DIR)/cm_pasn3.$(OBJ)

$(SY_DIR)/cm_pasn4.$(OBJ): $(CM_ASN_DIR)/cm_pasn4.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_ASN_DIR)/cm_pasn4.c -o $(SY_DIR)/cm_pasn4.$(OBJ)

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
#        End of file:     qo.mak@@/main/1 - Thu Jul  8 21:22:15 2010
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
#/main/1      ---     mm   1. SGW Initial Release.
#********************************************************************91*/

#DO NOT DELETE THIS LINE -- make depend depends on it.
