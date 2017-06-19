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
#       Name:   S1AP makefile
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
#       Sid:      sz_win.mak@@/main/4 - Tue May  8 16:04:38 2012
#
#       Prg:    sn
#
#********************************************************************21*/
#=======================================================================
#
# macros for product specific options

# commented for RTR
#PRDOPTS=-DSZ -DSZ_MME -DSZ_ENB -DUZ -DLCSZT -DLCSCT -DSS_MULTIPLE_PROCS

# for loose coupling
PRDOPTS=-DSZ -DSZ_MME -DSZ_ENB -DSZ_DYNAMIC_PEER \
	-DCM_PASN_ESC_SUPPORT -DCM_PASN_CLASS_IDTYPE -DCM_PASN_DBG    \
        -DLCSZT -DLCLSZ -DLCSCT -DLCSMSZMILSZ -DLCSZMILSZ -DLCUZLISZT \
	-DLWLCSZT -DLCHIUIHIT -DLCSBLIHIT -DLCHIMILHI -DLCSMHIMILHI -DLCLHI -DLCSBUISCT -DLCDMUISCT \
   -DLSZV1 -DSZTV1 -DCM_PASN_CRIT_HANDL -US1AP_REL8A0 -US1AP_REL9A6 -DSZTV2
# for ksctp
#PRDOPTS=-DSZ -DSZ_MME -DSZ_ENB \
#	-DCM_PASN_ESC_SUPPORT -DCM_PASN_CLASS_IDTYPE -DCM_PASN_DBG    \
#        -DLCSZT -DLCLSZ -DLCSCT -DLCSMSZMILSZ -DLCSZMILSZ -DLCUZLISZT \
#	-DLWLCSZT -DLCHIUIHIT -DLCSBLIHIT -DLCHIMILHI -DLCSMHIMILHI -DLCLHI -DHI_LKSCTP -DSZAC_LKSCTP -DCM_LKSCTP -DSUN_KSCTP

# For 64 Bit Compilation Use
# PRDOPTS=-DSZ -DSZ_ENB -DALIGN_64BIT

# Product options for portable build
PRDPTOPTS=$(PRDOPTS) -DCMFILE_REORG_1 -DPTSZUISZT -DCM_INET2


# -------------- Product options for acceptance test build
PRDACCOPTS=$(PRDOPTS) -DSM -DLCSZLISCT -DLCSZUISZT -DUZ -DDM -DCM_INET2      \
                      -DCMXTA_MASTER -DLWLCSZUISZT -DLWLCUZLISZT -DLCSULISCT \
		      -DSS_MULTIPLE_PROCS -DDEBUGP -DSB -DHI -DHI_MULTI_THREADED -DSZ_ACC_TST_ESC_FUNC
#
# -------------- CSB OPTS for acceptance test build
CSBOPTS=-DSB -DLSB8 -DHI_REL_1_4 -DCM_INET2 -DLCSCT -DHI -DLCSBUISCT
#
# -------------- HI OPTS for acceptance test build
CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DSB -DHI_MULTI_THREADED

#for KSCTP
#CHIOPTS=-DHI -DHI_REL_1_4 -UDM -DSB -DHI_MULTI_THREADED -DLCHIUISCT

# Product options for library build
PRDLIBOPTS=$(PRDOPTS) -DDBG4 -DCMFILE_REORG_1 -DPTSZUISZT -DCM_INET2

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
           -DxSS_SINGLE_THREADED -DLONG_MSG -DSS_MULTIPLE_PROCS

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
MTOSOPTS=-DSS_MT -D__EXTENSIONS__ -DDEBUGNOEXIT

# Options for library build with MTSS - MOS multithread OS
LIBOSOPTS="-DSS_MT -DSS  -DxSS_SINGLE_THREADED"

# Linux  specific options
#LNXOSOPTS=-D_GNU_SOURCE -DSS_LINUX -DQCACC_PRETTY_PRINT $(MTOSOPTS)
LNXOSOPTS=-DSS_LINUX $(MTOSOPTS)

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
#OSOPTS=$(GENOSOPTS) $(MTOSOPTS)

#for Windows NT MTSS
OSOPTS=$(GENOSOPTS) $(NSOSOPTS)

#=======================================================================
# SECTION 3
# macro for output file name and makefile name
#
BASENAME=sz

MAKENAME=$(BASENAME).mak

# default acceptance test for Solaris cc environment
BINACC="$(BASENAME)_acc"

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
#Windows
CM_SS_CODE_DIR=.
SZ_CODE_DIR=$(CODEPATH)
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
CM_SHT_DIR=$(CM_CODE_DIR)
CM_HIT_DIR=$(CM_CODE_DIR)
CM_LLIST_DIR=$(CM_CODE_DIR)
CM_INET_DIR=$(CM_CODE_DIR)
CM_SM_DIR=$(SM_CODE_DIR)
#64 bit python path
PYTHON_PATH_64_LNX=/usr/ccpu/python/linux/64bit/python-2.4.4/include/python2.4
PYTHON_PATH_64_SOL=/usr/ccpu/python/solaris/10/64bit/python-2.4.4/include/python2.4
#32 bit python path
PYTHON_PATH_32=/usr/ccpu/python/solaris/10/32bit/python-2.4.4/include/python2.4
#For windows Add suitable Python Path
PYTHON_PATH_WIN = c:\Python24\include

# S1AP directory
SZ_DIR=$(SZ_CODE_DIR)

# SCTP directory
SB_DIR=$(SZ_CODE_DIR)

# TUCL directory
#HI_DIR=$(SZ_CODE_DIR)/hi/
HI_DIR=.

# common layer manager interface files directory
CM_LSZ_DIR=$(SZ_DIR)

# Upper interface files directory
CM_SZT_DIR=$(SZ_DIR)

# SCCP directory
SP_DIR=.

# stack manager directory
SZSM_DIR=$(SAMPLE_CODE_DIR)

# system agent directory
SZSH_DIR=$(SAMPLE_CODE_DIR)

# acceptance directory
SZACC_DIR=/vobs/core_prd/gprs/s1ap/src/acc

# OS directory
#
# MS_DIR=/u/sw/tds/ms/crnt
# NS_DIR=/u/prd/rtr/ns
# VW_DIR=/u/prd/rtr/vw

#SS_DIR=/u/prd/rtr/mt/
#MT_DIR=/u/prd/rtr/mt/
#windows
SS_DIR=.

# OS Directory for Solaris MTSS
#OS_DIR=/u/prd/rtr/mt

# OS Directory for Windows
OS_DIR=.

# OS Directory for VxWorks
#OS_DIR= $(SS_DIR) $(VW_DIR)

# WIN-NT system services directory
NS_DIR=.

# acceptance directory
SZACC_DIR=.

# build directory
SY_DIR=.

# include path

# portable include path
PT_INC_PATH="-I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) -I$(CM_TMR_DIR) \
   -I$(CM_ASN_DIR) -I$(CM_SS7_DIR) -I$(CM_RPT_DIR) -I$(CM_SHT_DIR) -I$(CM_SPT_DIR) -I$(SZ_DIR)\
   -I$(SP_DIR) -I$(CM_MEM_DIR) -I$(CM_LLIST_DIR) -I$(CM_LSZ_DIR)"

# library include path
LIB_INC_PATH="-I$(SZ_DIR) -I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR)"

# Acceptance test include path
# ACC_INC_PATH=" -I/tools/wind/target/h -I$(SZ_DIR) -I$(CM_DIR) -I$(SZACC_DIR) \
#              -I$(CM_SM_DIR) -I$(SZSM_DIR) -I$(OS_DIR) -I$(SZL5_DIR) -I$(HI_DIR)"
#
ACC_INC_PATH="-I$(SZ_DIR) -I$(CM_CM_DIR) -I$(CM_LIB_DIR) -I$(CM_HASH_DIR) \
   -I$(CM_TMR_DIR) -I$(CM_ASN_DIR) -I$(CM_RPT_DIR) -I$(CM_SPT_DIR) -I$(CM_SHT_DIR) \
   -I$(SZACC_DIR) -I$(CM_SM_DIR) -I$(SZSM_DIR) -I$(OS_DIR) -I$(HI_DIR) \
   -I$(CM_SS7_DIR) -I$(SB_DIR) -I$(CM_LLIST_DIR) -I$(CM_MEM_DIR) -I$(CM_LSZ_DIR) -I$(PYTHON_PATH_WIN)"

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
SOLLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lnsl -lpython2.4 "

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
LNXENV="-DSUNOS   -DSS_LINUX -DANSI"
#LNXENV="-DSUNOS   -DSS_LINUX "
LNXCOPTS="-g3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused"

# For non-ANSI Compilation
#LNXENV="-DSUNOS"
#LNXENV="-DSUNOS -DSS_LINUX"
#LNXCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-align -Wcast-qual"


#LNXCC="gcc -m64 -DALIGN_64BIT -DBIT_64"
LNXCC="gcc -m32 "
#LNXLD="gcc -m64"
LNXLD="gcc -m32"
LNXLIBLD="gcc -m32"
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
#INSCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow -Wcast-align -Wcast-qual"



INSCC="/usr/local/insure71/bin/insure -m32"
INSLD="/usr/local/insure71/bin/insure -m32"
INSLOPTS="-o $(OUTNAME)"
INSLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lpython2.4 -lm -lrt -lutil -ldl"
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

LNXLOPTS="-o $(OUTNAME)"
LNXLIBOPTS="-r -o $(OUTNAME) -lrt "
#LNXLIBOPTS="-r -o $(OUTNAME) "
LNXLIBS="-Xlinker -export-dynamic -lpthread  -lnsl -lpython2.4 -lm -lrt -lutil -ldl"
#LNXLIBS=""
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
#OBJ=$(SOL_OBJ_SUFFIX)
OBJ=$(WIN_OBJ_SUFFIX)

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

# Product - sample acceptance test includes
SZACC_INC=$(SZACC_DIR)/szac_acc.h $(SZACC_DIR)/szac_acc.x

# sample acceptance tests all includes
SZACCALL_INC=$(SZALL_INC) $(SZACC_INC)

# OS specific includes - SS includes
#SS_INC=
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h \
	$(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x    \
	$(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x $(SS_DIR)/ss_msg.h     \
	$(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x \
	$(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x $(SS_DIR)/ss_task.h  \
	$(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x \

#MT_INC=
#MT_INC=$(MT_DIR)/mt_ss.h $(MT_DIR)/mt_ss.x $(MT_DIR)/mt_err.h


#=======================================================================
# SECTION 7
# macros for source and object files

# acceptance test source------------------------------------------------
SZ_ACC_SRCS= \
	$(SZACC_DIR)/szac_szthdl.c \
	$(SZACC_DIR)/szac_sztreg.c \
	$(SZACC_DIR)/szac_sztutl.c \
	$(SZACC_DIR)/szac_sztcb.c \
	$(SZACC_DIR)/szac_lhicb.c  \
	$(SZACC_DIR)/szac_lhihdl.c \
	$(SZACC_DIR)/szac_lhiutl.c \
	$(SZACC_DIR)/szac_lhireg.c \
	$(SZACC_DIR)/szac_lsbcb.c  \
	$(SZACC_DIR)/szac_lsbhdl.c \
	$(SZACC_DIR)/szac_lsbutl.c \
	$(SZACC_DIR)/szac_lsbreg.c \
	$(SZACC_DIR)/szac_lszcb.c  \
	$(SZACC_DIR)/szac_lszhdl.c \
	$(SZACC_DIR)/szac_lszreg.c \
	$(SZACC_DIR)/szac_scthdl.c \
	$(SZACC_DIR)/szac_sctreg.c \
	$(SZACC_DIR)/szac_sctutl.c \
	$(SZACC_DIR)/szac_sctcb.c  \
	$(SZACC_DIR)/szac_sb_ex_ms.c  \
	$(SZACC_DIR)/uz_ex_ms.c  \
	$(SZACC_DIR)/uz_ptli.c  \
	$(SZACC_DIR)/szac_tst.c  \
	$(SZACC_DIR)/szac_prof.c  \
	$(SZACC_DIR)/szac_flag.c \
	$(SZACC_DIR)/szac_lszutl.c \
	$(SZACC_DIR)/szac_szld.c \
	$(SZACC_DIR)/szac_sctptui.c


# acceptance test object------------------------------------------------
SZ_ACC_OBJS= \
	$(SY_DIR)/szac_szthdl.$(OBJ)    \
	$(SY_DIR)/szac_sztreg.$(OBJ)    \
	$(SY_DIR)/szac_sztutl.$(OBJ)    \
	$(SY_DIR)/szac_sztcb.$(OBJ)     \
	$(SY_DIR)/szac_lsbreg.$(OBJ)    \
	$(SY_DIR)/szac_lsbutl.$(OBJ)    \
	$(SY_DIR)/szac_lsbhdl.$(OBJ)    \
	$(SY_DIR)/szac_lsbcb.$(OBJ)     \
	$(SY_DIR)/szac_lhireg.$(OBJ)    \
	$(SY_DIR)/szac_lhiutl.$(OBJ)    \
	$(SY_DIR)/szac_lhihdl.$(OBJ)    \
	$(SY_DIR)/szac_lhicb.$(OBJ)     \
	$(SY_DIR)/szac_lszcb.$(OBJ)     \
	$(SY_DIR)/szac_lszhdl.$(OBJ)    \
	$(SY_DIR)/szac_lszreg.$(OBJ)    \
	$(SY_DIR)/szac_scthdl.$(OBJ)    \
	$(SY_DIR)/szac_sctreg.$(OBJ)    \
	$(SY_DIR)/szac_sctutl.$(OBJ)    \
	$(SY_DIR)/szac_sctcb.$(OBJ)     \
	$(SY_DIR)/szac_sb_ex_ms.$(OBJ)  \
	$(SY_DIR)/uz_ex_ms.$(OBJ)  \
	$(SY_DIR)/uz_ptli.$(OBJ)  \
	$(SY_DIR)/szac_tst.$(OBJ)    	  \
	$(SY_DIR)/szac_prof.$(OBJ)   	  \
	$(SY_DIR)/szac_flag.$(OBJ)   	  \
	$(SY_DIR)/szac_lszutl.$(OBJ)    \
	$(SY_DIR)/szac_szld.$(OBJ) \
	$(SY_DIR)/szac_sctptui.$(OBJ)


CM_XTA_SRCS= \
	$(SZACC_DIR)/cm_xta_txn.c \
	$(SZACC_DIR)/cm_xta_intutl.c \
	$(SZACC_DIR)/cm_xta_tmr.c \
   $(SZACC_DIR)/cm_xta_inthdl.c \
	$(SZACC_DIR)/cm_xta_intreg.c \
	$(SZACC_DIR)/cm_xta_res.c \
   $(SZACC_DIR)/cm_xta_queue.c \
   $(SZACC_DIR)/cm_xta_log.c \
	$(SZACC_DIR)/cm_xta_slave.c \
	$(SZACC_DIR)/cm_xta_master.c \
	$(SZACC_DIR)/cm_xta_reg.c \
	$(SZACC_DIR)/cm_xta_tst.c \
	$(SZACC_DIR)/cm_xta_flag.c \
	$(SZACC_DIR)/cm_xta_py.c \
	$(SZACC_DIR)/cm_xta_tcfsm.c \
	$(SZACC_DIR)/cm_xta_ldfsm.c \
	$(SZACC_DIR)/cm_xta_xml.c \
	$(SZACC_DIR)/cm_xta_ex_ms.c \
	$(SZACC_DIR)/cm_xta_tgfsm.c \
	$(SZACC_DIR)/cm_xta_tafsm.c \

CM_XTA_OBJS= \
	$(SY_DIR)/cm_xta_tmr.$(OBJ) \
	$(SY_DIR)/cm_xta_txn.$(OBJ) \
	$(SY_DIR)/cm_xta_res.$(OBJ) \
	$(SY_DIR)/cm_xta_queue.$(OBJ) \
	$(SY_DIR)/cm_xta_slave.$(OBJ) \
	$(SY_DIR)/cm_xta_master.$(OBJ) \
	$(SY_DIR)/cm_xta_reg.$(OBJ) \
	$(SY_DIR)/cm_xta_tst.$(OBJ) \
	$(SY_DIR)/cm_xta_log.$(OBJ) \
	$(SY_DIR)/cm_xta_flag.$(OBJ) \
	$(SY_DIR)/cm_xta_intreg.$(OBJ) \
	$(SY_DIR)/cm_xta_inthdl.$(OBJ) \
	$(SY_DIR)/cm_xta_intutl.$(OBJ) \
	$(SY_DIR)/cm_xta_py.$(OBJ) \
	$(SY_DIR)/cm_xta_tcfsm.$(OBJ) \
	$(SY_DIR)/cm_xta_ldfsm.$(OBJ) \
	$(SY_DIR)/cm_xta_xml.$(OBJ) \
	$(SY_DIR)/cm_xta_ex_ms.$(OBJ) \
	$(SY_DIR)/cm_xta_tgfsm.$(OBJ) \
	$(SY_DIR)/cm_xta_tafsm.$(OBJ)

# S1AP source------------------------------------------------
SZ_SRCS= \
	$(SZ_DIR)/sz_id.c     \
	$(SZ_DIR)/sz_ptui.c   \
	$(SZ_DIR)/sz_ptmi.c   \
	$(SZ_DIR)/sz_li.c   \
	$(SZ_DIR)/sz_ui.c   \
	$(SZ_DIR)/sz_mi.c   \
	$(SY_DIR)/sz_ndb.c   \
	$(SY_DIR)/sz_smlimme.c   \
	$(SY_DIR)/sz_smlienb.c   \
	$(SY_DIR)/sz_smuimme.c   \
	$(SY_DIR)/sz_smuienb.c   \
        $(SY_DIR)/sz_mgmt.c      \
        $(SY_DIR)/sz_utils.c      \
        $(SY_DIR)/sz_msgfn.c      \
        $(SY_DIR)/sz_db.c      \
	$(SY_DIR)/sz_tmr.c   \
	$(SY_DIR)/sz_sm.c   \
	$(SZ_DIR)/sz_ptli.c


# S1AP objects-------------------------------------------------------
SZ_OBJS= \
	$(SY_DIR)/sz_id.$(OBJ)     \
	$(SY_DIR)/sz_ptmi.$(OBJ)   \
	$(SY_DIR)/sz_ptui.$(OBJ)   \
	$(SY_DIR)/sz_ui.$(OBJ)   \
	$(SY_DIR)/sz_li.$(OBJ)   \
	$(SY_DIR)/sz_mi.$(OBJ)   \
	$(SY_DIR)/sz_ndb.$(OBJ)   \
	$(SY_DIR)/sz_smlimme.$(OBJ)   \
	$(SY_DIR)/sz_smlienb.$(OBJ)   \
	$(SY_DIR)/sz_smuimme.$(OBJ)   \
	$(SY_DIR)/sz_smuienb.$(OBJ)   \
   $(SY_DIR)/sz_mgmt.$(OBJ)   \
   $(SY_DIR)/sz_utils.$(OBJ)   \
   $(SY_DIR)/sz_msgfn.$(OBJ)   \
   $(SY_DIR)/sz_sm.$(OBJ)   \
   $(SY_DIR)/sz_db.$(OBJ)   \
	$(SY_DIR)/sz_tmr.$(OBJ)   \
	$(SY_DIR)/sz_ptli.$(OBJ)

# Product source------------------------------------------------
SB_SRCS= \
	$(SB_DIR)/sb_bdy1.c \
	$(SB_DIR)/sb_bdy2.c \
	$(SB_DIR)/sb_bdy3.c \
	$(SB_DIR)/sb_bdy4.c \
	$(SB_DIR)/sb_id.c \
	$(SB_DIR)/sb_ptmi.c \
	$(SB_DIR)/sb_ptui.c \
	$(SB_DIR)/sb_ptli.c \
	$(SB_DIR)/sb_ex_ms.c \
	$(SB_DIR)/sb_port.c \
	$(SB_DIR)/sb_mtu.c

# Product objects-------------------------------------------------------
SB_OBJS= \
	$(SY_DIR)/sb_bdy1.$(OBJ) \
	$(SY_DIR)/sb_bdy2.$(OBJ) \
	$(SY_DIR)/sb_bdy3.$(OBJ) \
	$(SY_DIR)/sb_bdy4.$(OBJ) \
	$(SY_DIR)/sb_id.$(OBJ) \
	$(SY_DIR)/sb_ptmi.$(OBJ) \
	$(SY_DIR)/sb_ptui.$(OBJ) \
	$(SY_DIR)/sb_ptli.$(OBJ) \
	$(SY_DIR)/sb_ex_ms.$(OBJ) \
	$(SY_DIR)/sb_port.$(OBJ) \
	$(SY_DIR)/sb_mtu.$(OBJ)

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

# common source(C source files)-----------------------------------------
SZCM_SRCS= \
	$(CM_CM_DIR)/cm_gen.c      \
	$(CM_LIB_DIR)/cm_lib.c     \
	$(CM_HASH_DIR)/cm_hash.c   \
	$(CM_LLIST_DIR)/cm_llist.c \
	$(CM_CM_DIR)/cm_tkns.c     \
	$(CM_DNS_DIR)/cm_dns.c     \
	$(CM_INET_DIR)/cm_inet.c   \
	$(CM_ASN_DIR)/cm_pasn1.c   \
	$(CM_ASN_DIR)/cm_pasn2.c   \
	$(CM_ASN_DIR)/cm_pasn3.c   \
	$(CM_ASN_DIR)/cm_pasn4.c   \
	$(CM_SCT_DIR)/sct.c        \
	$(CM_SCT_DIR)/lsb.c        \
	$(CM_TMR_DIR)/cm_bdy5.c    \
	$(CM_TPT_DIR)/cm_tpt.c     \
	$(CM_LSZ_DIR)/lsz.c        \
	$(CM_SZT_DIR)/szt.c        \
	$(CM_SZT_DIR)/szt_pk.c        \
	$(CM_SZT_DIR)/szt_unpk.c        \
	$(CM_HIT_DIR)/hit.c        \
	$(CM_HIT_DIR)/lhi.c

#	$(CM_SS7_DIR)/cm_ss7.c     \

# common objects--------------------------------------------------------
SZCM_OBJS= \
	$(SY_DIR)/cm_gen.$(OBJ)    \
	$(SY_DIR)/cm_tkns.$(OBJ)   \
	$(SY_DIR)/cm_lib.$(OBJ)    \
	$(SY_DIR)/cm_hash.$(OBJ)   \
	$(SY_DIR)/cm_llist.$(OBJ)  \
	$(SY_DIR)/cm_dns.$(OBJ)  	\
	$(SY_DIR)/cm_inet.$(OBJ)  	\
	$(SY_DIR)/cm_pasn1.$(OBJ)  \
	$(SY_DIR)/cm_pasn2.$(OBJ)  \
	$(SY_DIR)/cm_pasn3.$(OBJ)  \
	$(SY_DIR)/cm_pasn4.$(OBJ)  \
	$(SY_DIR)/cm_mblk.$(OBJ)   \
	$(SY_DIR)/sct.$(OBJ)       \
	$(SY_DIR)/lsb.$(OBJ)       \
	$(SY_DIR)/cm_bdy5.$(OBJ)   \
	$(SY_DIR)/cm_tpt.$(OBJ)    \
	$(SY_DIR)/szt.$(OBJ)       \
	$(SY_DIR)/szt_pk.$(OBJ)       \
	$(SY_DIR)/szt_unpk.$(OBJ)       \
	$(SY_DIR)/lsz.$(OBJ)       \
	$(SY_DIR)/hit.$(OBJ)      \
	$(SY_DIR)/lhi.$(OBJ)
#$(SY_DIR)/cm_ss7.$(OBJ)    \

# common stack manager source-------------------------------------------
SM_SRCS= \
	$(CM_SM_DIR)/sm_bdy1.c 	\
	$(CM_SM_DIR)/sm_ex_ms.c \
	$(CM_SM_DIR)/smhiexms.c \
	$(CM_SM_DIR)/smhiptmi.c \
	$(CM_SM_DIR)/smsbexms.c \
	$(CM_SM_DIR)/smsbptmi.c \

# common stack manager objects------------------------------------------
SM_OBJS= \
	$(SY_DIR)/sm_bdy1.$(OBJ) \
	$(SY_DIR)/sm_ex_ms.$(OBJ) \
	$(SY_DIR)/smhiexms.$(OBJ) \
	$(SY_DIR)/smhiptmi.$(OBJ) \
	$(SY_DIR)/smsbexms.$(OBJ) \
	$(SY_DIR)/smsbptmi.$(OBJ)

# sznap-lmi stack manager source------------------------------------------
SZSM_SRCS= \
	$(SZSM_DIR)/smszexms.c \
	$(SZSM_DIR)/smszptmi.c

#  $(SZSM_DIR)/smszbdy1.c \

# s1ap stack manager objects-----------------------------------------
SZSM_OBJS= \
	$(SY_DIR)/smszexms.$(OBJ) \
	$(SY_DIR)/smszptmi.$(OBJ)

#  $(SY_DIR)/smszbdy1.$(OBJ) \

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
	$(SZ_DIR)/sz_tst.c \
	$(SZ_DIR)/sz_ex_pt.c \
	$(CM_DIR)/ss_ptsp.c

# Portable Build Objects------------------------------------------------
PT_OBJS= \
	$(SY_DIR)/sz_tst.$(OBJ) \
	$(SY_DIR)/sz_ex_pt.$(OBJ) \
	$(SY_DIR)/ss_ptsp.$(OBJ)

# select which OS build-------------------------------------------------
#OS_SRCS=$(MS_SRCS)
#OS_OBJS=$(MS_OBJS)

#for Solaris MTSS
#OS_SRCS=$(SS_SRCS) $(MT_SRCS)
#OS_OBJS=$(SS_OBJS) $(MT_OBJS)

#for Windows NT MTSS
OS_SRCS=$(SS_SRCS) $(NS_SRCS)
OS_OBJS=$(SS_OBJS) $(NS_OBJS)

#for VxWorks
#OS_SRCS=$(SS_SRCS) $(VW_SRCS)
#OS_OBJS=$(SS_OBJS) $(VW_OBJS)

# all acceptance test source--------------------------------------------
PRDACC_SRCS= \
	$(SZ_ACC_SRCS) \
	$(CM_XTA_SRCS)\
	$(SZCM_SRCS) \
	$(SB_SRCS)    \
	$(HI_SRCS)    \
	$(SM_SRCS)    \
	$(SZCM_SRCS)  \
	$(OS_SRCS)    \
	$(SZ_SRCS)    \
	$(SZSH_SRCS)  \
	$(SZ_DIR)/sz_ex_ms.c \

# all acceptance test objects-------------------------------------------
PRDACC_OBJS= \
	$(SZ_ACC_OBJS)  \
	$(CM_XTA_OBJS) \
	$(SZCM_OBJS)  \
	$(SB_OBJS)     \
	$(HI_OBJS)     \
	$(SM_OBJS)     \
	$(SZSM_OBJS)   \
	$(OS_OBJS)    \
	$(SZ_OBJS)     \
	$(SZSH_OBJS)   \
	$(SY_DIR)/sz_ex_ms.$(OBJ)  \

# all portable sources--------------------------------------------------
PRDPT_SRCS=  \
	$(SZ_SRCS) \
	$(PT_SRCS) \
	$(SZCM_SRCS)

# all portable objects--------------------------------------------------
PRDPT_OBJS= \
	$(SZ_OBJS) \
	$(PT_OBJS) \
	$(SZCM_OBJS)

#----------------------------------------- Klocwork analysis objects
PRDKW_OBJS= \
	$(SZ_ACC_OBJS)  \
	$(CM_XTA_OBJS) \
	$(SZCM_OBJS)  \
	$(SB_OBJS)     \
	$(HI_OBJS)     \
	$(SM_OBJS)     \
	$(SZSM_OBJS)   \
	$(OS_OBJS)    \
	$(SZ_OBJS)     \
	$(SZSH_OBJS)   \
	$(SY_DIR)/sz_ex_ms.$(OBJ)  \

# all library sources--------------------------------------------------
PRDLIB_SRCS=  $(SZ_DIR)/sz_ex_ms.c $(SZ_SRCS)

# all library objects--------------------------------------------------
PRDLIB_OBJS= $(SY_DIR)/sz_ex_ms.$(OBJ)  $(SZ_OBJS)

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
	$(REMOVE) *.$(OBJ) $(BINACC) $(BINPT) *.ln *.s $(SY_DIR)/*.$(OBJ) \
      $(SY_DIR)/$(BINACC) $(SY_DIR)/*.ln $(SY_DIR)/*.s

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
    INC_PATH=$(LIB_INC_PATH)

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
    LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBOPTS) LIBS=$(LNXLIBS) \
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

$(ACCBIN): $(PRDACC_OBJS)
	$(LD) $(LOPTS) $(PRDACC_OBJS) $(LIBS)

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

$(SY_DIR)/szac_szthdl.$(OBJ): $(SZACC_DIR)/szac_szthdl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_szthdl.c -o $(SY_DIR)/szac_szthdl.$(OBJ)

$(SY_DIR)/szac_sztreg.$(OBJ): $(SZACC_DIR)/szac_sztreg.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sztreg.c -o $(SY_DIR)/szac_sztreg.$(OBJ)

$(SY_DIR)/szac_sztcb.$(OBJ): $(SZACC_DIR)/szac_sztcb.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sztcb.c -o $(SY_DIR)/szac_sztcb.$(OBJ)

$(SY_DIR)/szac_sztutl.$(OBJ): $(SZACC_DIR)/szac_sztutl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sztutl.c -o $(SY_DIR)/szac_sztutl.$(OBJ)

$(SY_DIR)/szac_scthdl.$(OBJ): $(SZACC_DIR)/szac_scthdl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_scthdl.c -o $(SY_DIR)/szac_scthdl.$(OBJ)

$(SY_DIR)/szac_sctreg.$(OBJ): $(SZACC_DIR)/szac_sctreg.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sctreg.c -o $(SY_DIR)/szac_sctreg.$(OBJ)

$(SY_DIR)/szac_sctcb.$(OBJ): $(SZACC_DIR)/szac_sctcb.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sctcb.c -o $(SY_DIR)/szac_sctcb.$(OBJ)

$(SY_DIR)/szac_sb_ex_ms.$(OBJ): $(SZACC_DIR)/szac_sb_ex_ms.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SZACC_DIR)/szac_sb_ex_ms.c -o $(SY_DIR)/szac_sb_ex_ms.$(OBJ)

$(SY_DIR)/uz_ex_ms.$(OBJ): $(SZACC_DIR)/uz_ex_ms.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/uz_ex_ms.c -o $(SY_DIR)/uz_ex_ms.$(OBJ)

$(SY_DIR)/uz_ptli.$(OBJ): $(SZACC_DIR)/uz_ptli.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/uz_ptli.c -o $(SY_DIR)/uz_ptli.$(OBJ)

$(SY_DIR)/szac_sctutl.$(OBJ): $(SZACC_DIR)/szac_sctutl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sctutl.c -o $(SY_DIR)/szac_sctutl.$(OBJ)

$(SY_DIR)/szac_lsbhdl.$(OBJ): $(SZACC_DIR)/szac_lsbhdl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SZACC_DIR)/szac_lsbhdl.c -o $(SY_DIR)/szac_lsbhdl.$(OBJ)

$(SY_DIR)/szac_lsbreg.$(OBJ): $(SZACC_DIR)/szac_lsbreg.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SZACC_DIR)/szac_lsbreg.c -o $(SY_DIR)/szac_lsbreg.$(OBJ)

$(SY_DIR)/szac_lsbcb.$(OBJ): $(SZACC_DIR)/szac_lsbcb.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SZACC_DIR)/szac_lsbcb.c -o $(SY_DIR)/szac_lsbcb.$(OBJ)

$(SY_DIR)/szac_lsbutl.$(OBJ): $(SZACC_DIR)/szac_lsbutl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SZACC_DIR)/szac_lsbutl.c -o $(SY_DIR)/szac_lsbutl.$(OBJ)

$(SY_DIR)/szac_lhihdl.$(OBJ): $(SZACC_DIR)/szac_lhihdl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(SZACC_DIR)/szac_lhihdl.c -o $(SY_DIR)/szac_lhihdl.$(OBJ)

$(SY_DIR)/szac_lhireg.$(OBJ): $(SZACC_DIR)/szac_lhireg.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(SZACC_DIR)/szac_lhireg.c -o $(SY_DIR)/szac_lhireg.$(OBJ)

$(SY_DIR)/szac_lhicb.$(OBJ): $(SZACC_DIR)/szac_lhicb.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(SZACC_DIR)/szac_lhicb.c -o $(SY_DIR)/szac_lhicb.$(OBJ)

$(SY_DIR)/szac_lhiutl.$(OBJ): $(SZACC_DIR)/szac_lhiutl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(SZACC_DIR)/szac_lhiutl.c -o $(SY_DIR)/szac_lhiutl.$(OBJ)

$(SY_DIR)/szac_lszhdl.$(OBJ): $(SZACC_DIR)/szac_lszhdl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_lszhdl.c -o $(SY_DIR)/szac_lszhdl.$(OBJ)

$(SY_DIR)/szac_lszreg.$(OBJ): $(SZACC_DIR)/szac_lszreg.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_lszreg.c -o $(SY_DIR)/szac_lszreg.$(OBJ)

$(SY_DIR)/szac_lszcb.$(OBJ): $(SZACC_DIR)/szac_lszcb.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_lszcb.c -o $(SY_DIR)/szac_lszcb.$(OBJ)

$(SY_DIR)/szac_lszutl.$(OBJ): $(SZACC_DIR)/szac_lszutl.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_lszutl.c -o $(SY_DIR)/szac_lszutl.$(OBJ)

$(SY_DIR)/szac_tst.$(OBJ): $(SZACC_DIR)/szac_tst.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_tst.c -o $(SY_DIR)/szac_tst.$(OBJ)

$(SY_DIR)/szac_prof.$(OBJ): $(SZACC_DIR)/szac_prof.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SZACC_DIR)/szac_prof.c -o $(SY_DIR)/szac_prof.$(OBJ)

$(SY_DIR)/szac_flag.$(OBJ): $(SZACC_DIR)/szac_flag.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_flag.c -o $(SY_DIR)/szac_flag.$(OBJ)

$(SY_DIR)/szac_szld.$(OBJ): $(SZACC_DIR)/szac_szld.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_szld.c -o $(SY_DIR)/szac_szld.$(OBJ)

$(SY_DIR)/szac_sctptui.$(OBJ): $(SZACC_DIR)/szac_sctptui.c $(SZACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/szac_sctptui.c -o $(SY_DIR)/szac_sctptui.$(OBJ)

# ---------------------------  CM_XTA files

$(SY_DIR)/cm_xta_ex_ms.$(OBJ): $(SZACC_DIR)/cm_xta_ex_ms.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_ex_ms.c -o $(SY_DIR)/cm_xta_ex_ms.$(OBJ)

$(SY_DIR)/cm_xta_xml.$(OBJ): $(SZACC_DIR)/cm_xta_xml.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_xml.c -o $(SY_DIR)/cm_xta_xml.$(OBJ)

$(SY_DIR)/cm_xta_ldfsm.$(OBJ): $(SZACC_DIR)/cm_xta_ldfsm.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_ldfsm.c -o $(SY_DIR)/cm_xta_ldfsm.$(OBJ)

$(SY_DIR)/cm_xta_tcfsm.$(OBJ): $(SZACC_DIR)/cm_xta_tcfsm.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_tcfsm.c -o $(SY_DIR)/cm_xta_tcfsm.$(OBJ)

$(SY_DIR)/cm_xta_tgfsm.$(OBJ): $(SZACC_DIR)/cm_xta_tgfsm.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_tgfsm.c -o $(SY_DIR)/cm_xta_tgfsm.$(OBJ)

$(SY_DIR)/cm_xta_tafsm.$(OBJ): $(SZACC_DIR)/cm_xta_tafsm.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_tafsm.c -o $(SY_DIR)/cm_xta_tafsm.$(OBJ)

$(SY_DIR)/cm_xta_py.$(OBJ): $(SZACC_DIR)/cm_xta_py.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_py.c -o $(SY_DIR)/cm_xta_py.$(OBJ)

$(SY_DIR)/cm_xta_inthdl.$(OBJ): $(SZACC_DIR)/cm_xta_inthdl.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_inthdl.c -o $(SY_DIR)/cm_xta_inthdl.$(OBJ)

$(SY_DIR)/cm_xta_intreg.$(OBJ): $(SZACC_DIR)/cm_xta_intreg.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_intreg.c -o $(SY_DIR)/cm_xta_intreg.$(OBJ)

$(SY_DIR)/cm_xta_res.$(OBJ): $(SZACC_DIR)/cm_xta_res.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_res.c -o $(SY_DIR)/cm_xta_res.$(OBJ)

$(SY_DIR)/cm_xta_queue.$(OBJ): $(SZACC_DIR)/cm_xta_queue.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_queue.c -o $(SY_DIR)/cm_xta_queue.$(OBJ)

$(SY_DIR)/cm_xta_flag.$(OBJ): $(SZACC_DIR)/cm_xta_flag.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_flag.c -o $(SY_DIR)/cm_xta_flag.$(OBJ)

$(SY_DIR)/cm_xta_log.$(OBJ): $(SZACC_DIR)/cm_xta_log.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_log.c -o $(SY_DIR)/cm_xta_log.$(OBJ)

$(SY_DIR)/cm_xta_slave.$(OBJ): $(SZACC_DIR)/cm_xta_slave.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_slave.c -o $(SY_DIR)/cm_xta_slave.$(OBJ)

$(SY_DIR)/cm_xta_master.$(OBJ): $(SZACC_DIR)/cm_xta_master.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_master.c -o $(SY_DIR)/cm_xta_master.$(OBJ)

$(SY_DIR)/cm_xta_reg.$(OBJ): $(SZACC_DIR)/cm_xta_reg.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_reg.c -o $(SY_DIR)/cm_xta_reg.$(OBJ)

$(SY_DIR)/cm_xta_tst.$(OBJ): $(SZACC_DIR)/cm_xta_tst.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_tst.c -o $(SY_DIR)/cm_xta_tst.$(OBJ)

$(SY_DIR)/cm_xta_ipc.$(OBJ): $(SZACC_DIR)/cm_xta_ipc.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_ipc.c -o $(SY_DIR)/cm_xta_ipc.$(OBJ)

$(SY_DIR)/cm_xta_txn.$(OBJ): $(SZACC_DIR)/cm_xta_txn.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_txn.c -o $(SY_DIR)/cm_xta_txn.$(OBJ)

$(SY_DIR)/cm_xta_intutl.$(OBJ): $(SZACC_DIR)/cm_xta_intutl.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_intutl.c -o $(SY_DIR)/cm_xta_intutl.$(OBJ)

$(SY_DIR)/cm_xta_tmr.$(OBJ): $(SZACC_DIR)/cm_xta_tmr.c
	$(CC) -c $(CPRDOPTS) $(SZACC_DIR)/cm_xta_tmr.c -o $(SY_DIR)/cm_xta_tmr.$(OBJ)

# ---------------------------  S1AP user files build

$(SY_DIR)/szuiptli.$(OBJ): $(SZL5_DIR)/szuiptli.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZL5_DIR)/szuiptli.c -o $(SY_DIR)/szuiptli.$(OBJ)

$(SY_DIR)/szulayer.$(OBJ): $(SZL5_DIR)/szulayer.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZL5_DIR)/szulayer.c -o $(SY_DIR)/szulayer.$(OBJ)

$(SY_DIR)/szuiexms.$(OBJ): $(SZL5_DIR)/szuiexms.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZL5_DIR)/szuiexms.c -o $(SY_DIR)/szuiexms.$(OBJ)

# ----------------------------- S1AP lower interface files
$(SY_DIR)/szliexms.$(OBJ): $(SZL3_DIR)/szliexms.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZL3_DIR)/szliexms.c -o $(SY_DIR)/szliexms.$(OBJ)

$(SY_DIR)/szliptui.$(OBJ): $(SZL3_DIR)/szliptui.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZL3_DIR)/szliptui.c -o $(SY_DIR)/szliptui.$(OBJ)

$(SY_DIR)/szllayer.$(OBJ): $(SZL3_DIR)/szllayer.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZL3_DIR)/szllayer.c -o $(SY_DIR)/szllayer.$(OBJ)

# S1AP files build---------------------------------------------------

# S1AP product files
$(SY_DIR)/sz_ui.$(OBJ): $(SZ_DIR)/sz_ui.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_ui.c -o $(SY_DIR)/sz_ui.$(OBJ)

$(SY_DIR)/sz_mi.$(OBJ): $(SZ_DIR)/sz_mi.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_mi.c -o $(SY_DIR)/sz_mi.$(OBJ)

$(SY_DIR)/sz_li.$(OBJ): $(SZ_DIR)/sz_li.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_li.c -o $(SY_DIR)/sz_li.$(OBJ)

$(SY_DIR)/sz_tmr.$(OBJ): $(SZ_DIR)/sz_tmr.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_tmr.c -o $(SY_DIR)/sz_tmr.$(OBJ)

$(SY_DIR)/sz_ndb.$(OBJ): $(SZ_DIR)/sz_ndb.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_ndb.c -o $(SY_DIR)/sz_ndb.$(OBJ)

$(SY_DIR)/sz_smlimme.$(OBJ): $(SZ_DIR)/sz_smlimme.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_smlimme.c -o $(SY_DIR)/sz_smlimme.$(OBJ)

$(SY_DIR)/sz_smlienb.$(OBJ): $(SZ_DIR)/sz_smlienb.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_smlienb.c -o $(SY_DIR)/sz_smlienb.$(OBJ)

$(SY_DIR)/sz_smuimme.$(OBJ): $(SZ_DIR)/sz_smuimme.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_smuimme.c -o $(SY_DIR)/sz_smuimme.$(OBJ)

$(SY_DIR)/sz_smuienb.$(OBJ): $(SZ_DIR)/sz_smuienb.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_smuienb.c -o $(SY_DIR)/sz_smuienb.$(OBJ)

$(SY_DIR)/sz_mgmt.$(OBJ): $(SZ_DIR)/sz_mgmt.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_mgmt.c -o $(SY_DIR)/sz_mgmt.$(OBJ)

$(SY_DIR)/sz_msgfn.$(OBJ): $(SZ_DIR)/sz_msgfn.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_msgfn.c -o $(SY_DIR)/sz_msgfn.$(OBJ)

$(SY_DIR)/sz_utils.$(OBJ): $(SZ_DIR)/sz_utils.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_utils.c -o $(SY_DIR)/sz_utils.$(OBJ)

$(SY_DIR)/sz_sm.$(OBJ): $(SZ_DIR)/sz_sm.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_sm.c -o $(SY_DIR)/sz_sm.$(OBJ)

# product Id  file
$(SY_DIR)/sz_id.$(OBJ): $(SZ_DIR)/sz_id.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_id.c -o $(SY_DIR)/sz_id.$(OBJ)

# All layers to SZ interface file
$(SY_DIR)/sz_ex_ms.$(OBJ): $(SZ_DIR)/sz_ex_ms.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_ex_ms.c -o $(SY_DIR)/sz_ex_ms.$(OBJ)

# S1AP to S1AP user file
$(SY_DIR)/sz_ptui.$(OBJ): $(SZ_DIR)/sz_ptui.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_ptui.c -o $(SY_DIR)/sz_ptui.$(OBJ)

# S1AP to layer3 file
$(SY_DIR)/sz_ptli.$(OBJ): $(SZ_DIR)/sz_ptli.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_ptli.c -o $(SY_DIR)/sz_ptli.$(OBJ)

# S1AP to layer management file
$(SY_DIR)/sz_ptmi.$(OBJ): $(SZ_DIR)/sz_ptmi.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_ptmi.c -o $(SY_DIR)/sz_ptmi.$(OBJ)

# product build---------------------------------------------------

$(SY_DIR)/sb_bdy1.$(OBJ): $(SB_DIR)/sb_bdy1.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_bdy1.c -o $(SY_DIR)/sb_bdy1.$(OBJ)

$(SY_DIR)/sb_bdy2.$(OBJ): $(SB_DIR)/sb_bdy2.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_bdy2.c -o $(SY_DIR)/sb_bdy2.$(OBJ)

$(SY_DIR)/sb_bdy3.$(OBJ): $(SB_DIR)/sb_bdy3.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_bdy3.c -o $(SY_DIR)/sb_bdy3.$(OBJ)

$(SY_DIR)/sb_bdy4.$(OBJ): $(SB_DIR)/sb_bdy4.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_bdy4.c -o $(SY_DIR)/sb_bdy4.$(OBJ)

$(SY_DIR)/sb_id.$(OBJ): $(SB_DIR)/sb_id.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_id.c -o $(SY_DIR)/sb_id.$(OBJ)

$(SY_DIR)/sb_ptmi.$(OBJ): $(SB_DIR)/sb_ptmi.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(CSMOPTS) $(SB_DIR)/sb_ptmi.c -o $(SY_DIR)/sb_ptmi.$(OBJ)

$(SY_DIR)/sb_ptui.$(OBJ): $(SB_DIR)/sb_ptui.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) -UDM $(SB_DIR)/sb_ptui.c -o $(SY_DIR)/sb_ptui.$(OBJ)

$(SY_DIR)/sb_ptli.$(OBJ): $(SB_DIR)/sb_ptli.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_ptli.c -o $(SY_DIR)/sb_ptli.$(OBJ)

$(SY_DIR)/sb_ex_ms.$(OBJ): $(SB_DIR)/sb_ex_ms.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_ex_ms.c -o $(SY_DIR)/sb_ex_ms.$(OBJ)

$(SY_DIR)/sb_port.$(OBJ): $(SB_DIR)/sb_port.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_port.c -o $(SY_DIR)/sb_port.$(OBJ)

$(SY_DIR)/sb_mtu.$(OBJ): $(SB_DIR)/sb_mtu.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(SB_DIR)/sb_mtu.c -o $(SY_DIR)/sb_mtu.$(OBJ)

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

$(SY_DIR)/sm_bdy1.$(OBJ): $(CM_SM_DIR)/sm_bdy1.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CSMOPTS) $(CM_SM_DIR)/sm_bdy1.c -o $(SY_DIR)/sm_bdy1.$(OBJ)

$(SY_DIR)/sm_ex_ms.$(OBJ): $(CM_SM_DIR)/sm_ex_ms.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(CM_SM_DIR)/sm_ex_ms.c -o $(SY_DIR)/sm_ex_ms.$(OBJ)

$(SY_DIR)/smhiexms.$(OBJ): $(CM_SM_DIR)/smhiexms.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(CSMOPTS) $(CM_SM_DIR)/smhiexms.c -o $(SY_DIR)/smhiexms.$(OBJ)

$(SY_DIR)/smhiptmi.$(OBJ): $(CM_SM_DIR)/smhiptmi.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CHIOPTS) $(CSMOPTS) $(CM_SM_DIR)/smhiptmi.c -o $(SY_DIR)/smhiptmi.$(OBJ)

$(SY_DIR)/smsbexms.$(OBJ): $(CM_SM_DIR)/smsbexms.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(CSMOPTS) $(CM_SM_DIR)/smsbexms.c -o $(SY_DIR)/smsbexms.$(OBJ)

$(SY_DIR)/smsbptmi.$(OBJ): $(CM_SM_DIR)/smsbptmi.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(CSMOPTS) $(CM_SM_DIR)/smsbptmi.c -o $(SY_DIR)/smsbptmi.$(OBJ)

$(SY_DIR)/smszbdy1.$(OBJ): $(SZSM_DIR)/smszbdy1.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(SZSM_DIR)/smszbdy1.c -o $(SY_DIR)/smszbdy1.$(OBJ)

$(SY_DIR)/smszexms.$(OBJ): $(SZSM_DIR)/smszexms.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(SZSM_DIR)/smszexms.c -o $(SY_DIR)/smszexms.$(OBJ)

$(SY_DIR)/smszptmi.$(OBJ): $(SZSM_DIR)/smszptmi.c $(SYS_INC) $(LSZ_INC)
	$(CC) -c $(CPRDOPTS) $(CSMOPTS) $(SZSM_DIR)/smszptmi.c -o $(SY_DIR)/smszptmi.$(OBJ)

# S1AP database files
$(SY_DIR)/sz_db.$(OBJ): $(SZ_DIR)/sz_db.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(SZ_DIR)/sz_db.c -o $(SY_DIR)/sz_db.$(OBJ)

# S1AP common files build--------------------------------------------

# S1AP interface specific files
$(SY_DIR)/lsz.$(OBJ): $(CM_LSZ_DIR)/lsz.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(CM_LSZ_DIR)/lsz.c -o $(SY_DIR)/lsz.$(OBJ)

$(SY_DIR)/sct.$(OBJ): $(CM_SCT_DIR)/sct.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(CM_SCT_DIR)/sct.c -o $(SY_DIR)/sct.$(OBJ)

$(SY_DIR)/szt.$(OBJ): $(CM_SHT_DIR)/szt.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(CM_SHT_DIR)/szt.c -o $(SY_DIR)/szt.$(OBJ)

$(SY_DIR)/szt_pk.$(OBJ): $(CM_SHT_DIR)/szt_pk.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(CM_SHT_DIR)/szt_pk.c -o $(SY_DIR)/szt_pk.$(OBJ)

$(SY_DIR)/szt_unpk.$(OBJ): $(CM_SHT_DIR)/szt_unpk.c $(SYS_INC) $(CM_INC) $(SZALL_INC)
	$(CC) -c $(CPRDOPTS) $(CM_SHT_DIR)/szt_unpk.c -o $(SY_DIR)/szt_unpk.$(OBJ)

# S1AP common library files
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

$(SY_DIR)/lhi.$(OBJ): $(CM_TMR_DIR)/lhi.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_TMR_DIR)/lhi.c -o $(SY_DIR)/lhi.$(OBJ)

$(SY_DIR)/lsb.$(OBJ): $(CM_TMR_DIR)/lsb.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CSBOPTS) $(CM_TMR_DIR)/lsb.c -o $(SY_DIR)/lsb.$(OBJ)

$(SY_DIR)/cm_gen.$(OBJ): $(CM_CM_DIR)/cm_gen.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_CM_DIR)/cm_gen.c -o $(SY_DIR)/cm_gen.$(OBJ)

$(SY_DIR)/cm_ss7.$(OBJ): $(CM_SS7_DIR)/cm_ss7.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_SS7_DIR)/cm_ss7.c -o $(SY_DIR)/cm_ss7.$(OBJ)

$(SY_DIR)/cm_tkns.$(OBJ): $(CM_CM_DIR)/cm_tkns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_CM_DIR)/cm_tkns.c -o $(SY_DIR)/cm_tkns.$(OBJ)

$(SY_DIR)/cm_dns.$(OBJ): $(CM_DNS_DIR)/cm_dns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DNS_DIR)/cm_dns.c -o $(SY_DIR)/cm_dns.$(OBJ)

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

$(SY_DIR)/sz_tst.$(OBJ): $(SZ_DIR)/sz_tst.c
	$(CC) -c $(CPTOPTS) $(SZ_DIR)/sz_tst.c -o $(SY_DIR)/sz_tst.$(OBJ)

$(SY_DIR)/sz_ex_pt.$(OBJ): $(SZ_DIR)/sz_ex_pt.c
	$(CC) -c $(CPTOPTS) $(SZ_DIR)/sz_ex_pt.c -o $(SY_DIR)/sz_ex_pt.$(OBJ)

$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_CM_DIR)/ss_ptsp.c
	$(CC) -c $(CPTOPTS) $(CM_CM_DIR)/ss_ptsp.c -o $(SY_DIR)/ss_ptsp.$(OBJ)

#
#********************************************************************30**
#
#        End of file:     sz_win.mak@@/main/4 - Tue May  8 16:04:38 2012
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
#/main/4      ----     va   1. Updated for the Release of 4.1
#********************************************************************91*/

#DO NOT DELETE THIS LINE -- make depend depends on it.
