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
#
#********************************************************************20**
#
#       Name:   Evolved GPRS Tunneling Protocol (eGTP) (EG) - makefile
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file.
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Solaris 2.x with SUNpro C(cc)
#
#               ENVIRONMENTS:
#
#               Solaris 2.x with SUNpro C(cc)
#
#               Solaris 2.x with GNU C(gcc)
#
#               Solaris 2.x with MRI C (mcc68k) cross compiled for
#               a relatively standard 68040 procesegr board
#
#               Microegft Visual C++ 4.x for Windows '95 and Windows NT
#               environments(meuc)
#
#               BUILD TARGETS:
#
#               pt               portable software (without MOS) -- default
#               acc              acceptance tests - DUMMY TUCL
#               acc1             acceptance tests with DUMMY TUCL & TUCL
#               acc2             Stub Testing of EDM Module
#               acc3             Building the slave.
#
#               depend.acc       makedepend for acceptance tests
#               depend.pt        makedepend for portable egftware
#
#               clean            remove all object files
#               win_clean        remove all object files(Windows environment)
#
#       Sid:      vb_eg.mak@@/main/1 - Mon Jun 28 19:35:29 2010
#
#       Prg:    kc
#
#********************************************************************21*/
# NOTE: Search for TODO - To get the options what to change for other
#       platforms/rules .
#       For DUMMY TUCL related rules, search for DUMMY
#       For STUB releated rules, search for STUB
#       For Slave Releater rules, search for SLAVE


# NOTE: for compilation on different platforms, the BLDENV specification
#       is not sufficient. The OS_DIR needs to be set to the correct
#       directory  and the OBJ needs to be set to the correct
#       object file extension
#

#=======================================================================
# SECTION 1
# Defines to specify appropriate directories
# Set the defines below to respective product directories

#Directory path
CODEPATH = .
# EG files directory
EG_DIR=$(CODEPATH)

# HI files directory
HI_DIR=$(CODEPATH)

# Common files directory
CM_DIR=$(CODEPATH)

# common stack manager files directory
SM_DIR=$(CODEPATH)

# EG stack manager files directory
EGSM_DIR=$(CODEPATH)

# HI stack manager files directory
HISM_DIR=$(CODEPATH)

# acceptance directory
EGACC_DIR=.


# TODO  - Default is for linux 32bit. User has to change the PY_DIR
#         TO corresponding paths for both 32 and 64-bit compilations.
#         For Vx-Works there is no need for the PY_DIR path.

# acceptance directory(added for CC compilation on solaris)

PY_DIR=/usr/local/include/python2.4
#PY_DIR=/usr/ccpu/python/solaris/10/32bit/python-2.4.4/include/python2.4/
#PY_DIR=/usr/ccpu/python/linux/64bit/python-2.4.4
#PY_DIR=C:\Python24\include

# acceptance directory
EGNEWACC_DIR=.

# acceptance directory
EGTCACC_DIR=.

# TODO  - Default is for linux 32bit. User has to change the SS_DIR
#         to the SS files path.

# system services directories
# common SS
SS_DIR=.
#SS_DIR=.

#  TODO  - Default is for linux/solaris - MT files
#        - For Windows comment the MT_DIR and uncomment NS_DIR and update path
#        - For Vx-Works comment the MT_DIR and uncomment VW_DIR and update path

# for MTSS
MT_DIR=.
#MT_DIR=.

# for NT
NS_DIR=.

#for VxWorks
VW_DIR=.

#  TODO  - Default is for linux/solaris - OS_DIR=MT_DIR
#        - For Windows OS_DIR=$(NS_DIR)
#        - For Vx-Works OS_DIR=$(VW_DIR)

OS_DIR=$(MT_DIR)
#OS_DIR=$(NS_DIR)
#OS_DIR=$(VW_DIR)

#Provide correct SSL library include path for TLS.
#SSL_DIR=
SSL_DIR=/usr/local/ssl
SSL_INC_PATH=$(SSL_DIR)/include
SSL_LIB_PATH=$(SSL_DIR)/lib
# build directory
SY_DIR=.
LIB_DIR=.

# include path
# portable include path
# new acceptance test include path
PT_INC_PATH="-I$(CM_DIR) -I$(EG_DIR) -I$(HI_DIR)"

# library include path
# new acceptance test include path
LIB_INC_PATH="-I$(CM_DIR) -I$(SS_DIR) -I$(OS_DIR) -I$(EG_DIR) -I$(HI_DIR)"

# For the WorkBench windRiver
VW_INC=C:/WindRiver/vxworks-6.3/target/h
VW_INC1=C:/WindRiver/vxworks-6.3/target/h/wrn
VW_INC2=C:/WindRiver/vxworks-6.3/target/h/wrn/coreip
VW_INC3=C:/WindRiver/vxworks-6.3/target/h/wrn/coreip/netinet
VW_INC4=C:/WindRiver/vxworks-6.3/sys-include


# TODO  ---- For acceptance Test Build
ACC_INC_PATH="-I$(CM_DIR) -I$(EG_DIR) -I$(HI_DIR) -I$(EGNEWACC_DIR) \
	      -I$(SM_DIR) -I$(EGSM_DIR) -I$(HISM_DIR) -I$(SS_DIR) -I$(OS_DIR) \
	      -I$(PY_DIR)"

# For the VX-WORKS SLAVE Testing
#ACC_INC_PATH="-I$(CM_DIR) -I$(EG_DIR) -I$(HI_DIR) -I$(EGNEWACC_DIR) \
	      -I$(SM_DIR) -I$(EGSM_DIR) -I$(HISM_DIR) -I$(SS_DIR) -I$(OS_DIR) \
	      -I$(VW_INC) -I$(VW_INC1) -I$(VW_INC2) -I$(VW_INC3) -I$(VW_INC4)"


# SS - include path
SSINC_PATH=-I$(SS_DIR) -I$(OS_DIR) -I$(CM_DIR)

# library path - used for integrated products and simulation env.
LIB_PATH=./

#=======================================================================
# SECTION 2
# Product specific options
#
## TCP/UDP convergence layer (HI)
# HI_REL_1_3 : enable to be compatable to rel 1.3
# HI_REL_1_2 : enable to be compatable to rel 1.2
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

## GTP extensions flags

# Interface wrapper flag

# Interface coupling flags
# All interfaces are loose coupled by default, comment the
# corresponding flag to tight couple an interface.
# Enable this flag if compiling with multi-threaded TUCL
#HIT (TUCL - user)
HITLCFLG=-DLCHIUIHIT -DLCEGLIHIT -DLCSBLIHIT -DLCHIT -DLCLHI

# Enable this flag if compiling with multi-threaded TUCL
#LHI (TUCL - LM)
LHILCFLG=-DLCHIMILHI -DLCSMHIMILHI -DLCEGT

#EGT (eGTP - user (EU))
EGTLCFLG=-DLCEGUIEGT -DLCEULIEGT

# light weight loose coupling at the EGT interface
EGTLWLCFLG=-DLWLCEGUIEGT -DLWLCEULIEGT

#LEG (eGTP - LM)
LEGLCFLG=-DLCEGMILEG -DLCSMEGMILEG -DLWLCEGMILEG

# TCP/UDP convergence layer options

#TUCL Release 1.2 flags
#HIREL12FLG=-DHI_REL_1_2

#TUCL Release 1.3 flags
HIREL13FLG=-DHI_REL_1_3 -DCM_INET2

#TUCL Release 1.4 flags
HIREL14FLG=-DHI_REL_1_4 -DHI_REL_1_3 -DCM_INET2

#TUCL flags added to make eGTP compilable with 1.4 and 1.5
HIREL15FLG=-DHI_REL_1_4 -DHI_REL_1_3 -DCM_INET2 -DHI_REL_1_2 -DHI006_12

#TUCL Fast Find flags. Enable this to improve performance
HIFASTFINDFLG=-DHI_FAST_FIND -DCM_INET_FDSET_PEEK

#TUCL Debug flags
HIDBGFLG=-DCMINETDBG

HIOPTS=-DHI $(HITLCFLG) $(LHILCFLG) $(HIREL15FLG) $(HIFASTFINDFLG) \
       $(HIDBGFLG) -DHI_CMPTBL_FLAG -DHI_MULTI_THREADED -DDHI
DHIOPTS=$(HITLCFLG) $(LHILCFLG) $(HIREL15FLG) $(HIFASTFINDFLG)
HIOPTS1=-DHI $(HITLCFLG) $(LHILCFLG) $(HIREL14FLG) $(HIREL15FLG) $(HIFASTFINDFLG) \
       $(HIDBGFLG) -DHI_CMPTBL_FLAG -DxHI_MULTI_THREADED -DHI_TLS

#Enable this for TUCL loopback compiling
#HIOPTS=-DHI $(HITLCFLG) $(LHILCFLG) $(HIREL13FLG) -DHI_LPBK \

#Enable this for TUCL multi-threading. Aleg enable HITLCFLG
#HIOPTS=-DHI $(HITLCFLG) $(LHILCFLG) $(HIREL13FLG) $(HIFASTFINDFLG) \
#       $(HIDBGFLG) -DHI_MULTI_TREADED

# GTP layer options
EGOPTS = -DEG \
         -DDEBUGP \
         -UEG_GROUPED_CHK \
         -UEG_PRNT_DATA \
         -U_EG_PRINT_BUF_ \
         -DxEGAC_LOOP_TEST \
         -UIPV6_SUPPORTED \
         -USS_DRVR_SUPPORT -DEGTP_C -DLCLEG -DLCEGT -DLWLCEGUIEGT \
         -DLONG_MSG -USS_MEM_LEAK_STS


# Placeholder for SFT. Dont modify this line.

# Without both TLS and EGTP
EGPTOPTS=$(EGOPTS) -DCM_INET2 -DDONT_USE_SYS_LIB -DSS_MT_TMR

EGLIBOPTS =$(EGOPTS) -DCM_INET2 -DDONT_USE_SYS_LIB -DSS_MT_TMR -DEG_REL_930 -DEG_PHASE2 $(LEGLCFLG)  -DEU -DLCEGLIHIT -DLCEGUIEGT

# Product options for Acceptance Test Build - DUMMY TUCL ONLY
EGACCOPTS=$(EGOPTS) -DEG_ACC -UDEBUGP -DEU -DCMXTA_MASTER -DCMXTA_RET_ERR_NONE -DSM $(DHIOPTS) $(EGTLCFLG) $(EGTLWLCFLG) $(LEGLCFLG) -DSS_MULTIPLE_PROCS -DDHI

# Product options for Acceptance Test Build - TUCL + DUMMY-TUCL
EGACCOPTS1=$(EGOPTS) -DEG_ACC -UDEBUGP -DEU -DCMXTA_MASTER -DCMXTA_RET_ERR_NONE -DSM $(HIOPTS) $(EGTLCFLG) $(EGTLWLCFLG) $(LEGLCFLG) -DSS_MULTIPLE_PROCS -DDHI -DEG_ACC_INT

# Product options for Acceptance Test Build - STUB for EDM Testing
EGACCOPTS2=$(EGOPTS) -UDEBUGP -UEU -DCMXTA_MASTER -DCMXTA_RET_ERR_NONE  -DSM $(HIOPTS) $(EGTLCFLG) $(EGTLWLCFLG) $(LEGLCFLG)

# Product options for SLAVE Compilation.
EGACCOPTS3=$(EGOPTS) -DxDEBUGP -UEU  -DxSM $(DHIOPTS) $(EGTLCFLG) $(EGTLWLCFLG) $(LEGLCFLG)

#======================================================================
# Options for compilation with different OS's
# SECTION 3

# System Services product specific options
# options for a Portable Build
GENPTOSOPTS=-DCMFILE_REORG_2 -DCMFILE_REORG_1 -DSS_MULTIPLE_PROCS

# options for a Library Build
GENLIBOSOPTS=-DCMFILE_REORG_2 -DCMFILE_REORG_1 -DCM_INET2

LNXLIBOSOPTS="-DSS_MT -D_REENTRANT -D__EXTENSIONS__  -DDEBUGNOEXIT -D_GNU_SOURCE -DSS_LINUX"
SOLLIBOSOPTS="-DSS_MT -D_REENTRANT -D__EXTENSIONS__  -DDEBUGNOEXIT"

# Enable for a Multithreaded Acceptance Test Build
#GENACCOSOPTS=-DSS -DCMFILE_REORG_2 -DCMFILE_REORG_1 -DCONSTDIO #-DUSE_PURE

# TODO - Enable the corresponding acc opts based on the Rules:

# This Rule is for the TUCL/DUMMY-TUCL testing for SINGLE_THREAD & MULTIPLE_PROC
GENACCOSOPTS=-DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2 -USS_SINGLE_THREADED -DSS_TICKS_SEC=SS_1MS  -DSS_DRVR_SUPPORT -DSM -DSS_MULTIPLE_PROCS -UIPV6_SUPPORTED #-DUSE_PURE

# Enable for a STUB Testing Mode
#GENACCOSOPTS=-DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2 -USS_SINGLE_THREADED -DSS_TICKS_SEC=SS_1MS  -DSS_DRVR_SUPPORT -DxSM

# Enable for a SLAVE Testing Mode - ONLY MULTIPLE_PROC Mode
#GENACCOSOPTS=-DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2 -USS_SINGLE_THREADED -DSS_TICKS_SEC=SS_100MS  -DSS_DRVR_SUPPORT -DCMINETFLATBUF -DSS_VW -DVW_COMPILE -DSS_VW6X -DCMINET_SINLEN -DDEBUGNOEXIT -DSS_MULTIPLE_PROCS -DCM_INET2

# Portable build options
PTOSOPTS=-DPORTVER

# Options for acceptance test build with PEGS Os
PEGEGPTS=-DSS_PS

# Options for acceptance test build with VrTx Os
VXOSOPTS=-DSS_VX

# Options for acceptance test build with VxWorks Os
VWOSOPTS=-DSS_VW -DVW_COMPILE -DSS_VW6X -DENB_RELAY -DSS_DRVR_SUPPORT

# Options for acceptance test build with WINNT Os kernel build
WINOSOPTS=-DWINNT

# Options for acceptance test build with MTSS-Solaris
MTOSOPTS=-DSS_MT -D_REENTRANT -D__EXTENSIONS__ -DDEBUGNOEXIT

# Options for acceptance test build with Linux
# EGACC_PRETTY_PRINT is an addition for colour debug prints during acceptance
# testing
LNXOSOPTS=-D_xGNU_SOURCE -DSS_LINUX -DEGACC_PRETTY_PRINT $(MTOSOPTS)

# Options for MTSS-Windows NT
NEGEGPTS="-DNS -DSS_OLD_THREAD -DNS_ENB_MGMT"

#=======================================================================
# Macros for output file name and makefile name
# SECTION 4

BASENAME=vb_eg

MAKENAME=$(BASENAME).mak

# default acceptance test for Solaris cc environment
BINACC="$(BASENAME)_acc"

ifeq ($(MACHINE), BIT64)
# default portable for 64 bit linux cc environment
BINLIB=$(LIB_DIR)/libvb_lnx64eg.a
else
# default portable for 32 bit linux cc environment
BINLIB=$(LIB_DIR)/libvb_lnx32eg.a
endif

# Internal targets for the acceptance test build
MAKEACC=mkacc

ACCBIN=$(BASENAME)_acc

# Internal target for the portable build
MAKEPT=mkpt

PTBIN=$(BASENAME)_pt

# Internal target for the library build
MAKELIB=mklib

LIBBIN=$(BASENAME)_lib

# Internal target for makedepend
DEPEND=depend

# Build targets for testing in different environments

# Build with the GNU compiler and linker on Solaris
# Acceptance Test Build
BINGNUACC=gnu_acc
BINGNUACC1=gnu_acc1
BINGNUACC2=gnu_acc2
BINGNUACC3=gnu_acc3
BINGNUACC4=gnu_acc4
# Portable Build
BINGNUPT=gnu_pt
BINGNUPT1=gnu_pt1
BINGNUPT2=gnu_pt2
BINGNUPT3=gnu_pt3
BINGNUPT4=gnu_pt4

# Build with the GNU compiler and linker on Linux
# Acceptance Test Build
BINLNXACC=lnx_acc

# Purify Acceptance Test Build
BINPURACC=pur_acc

# Purcove Acceptance Test Build
BINCOVACC=purccov_acc

# Quantify  Test Build
BINQUANACC=purquantify_acc

# Portable Build
BINLNXPT=lnx_pt

# Build with the Insite Memory Check programe - Acceptance Tests only
# Acceptance Test Build
BININSITEACC=ins_acc

# Build with the Microtek MRI C compiler on Solaris - portable only
# Portable Build
BINMRIPT=mri_pt
BINMRIPT1=mri_pt1
BINMRIPT2=mri_pt2
BINMRIPT3=mri_pt3
BINMRIPT4=mri_pt4

# Build with the MRI compiler for vxWorks - 68040 target
# Acceptance Tests only
BINPPCACC=ppc_acc
BINPPCACC1=ppc_acc1
BINPPCACC2=ppc_acc2
BINPPCACC3=ppc_acc3
BINPPCACC4=ppc_acc4

# Portable build
BINPPCPT=ppc_pt
BINPPCPT1=ppc_pt1
BINPPCPT2=ppc_pt2
BINPPCPT3=ppc_pt3
BINPPCPT4=ppc_pt4

# g++ Acceptance Test Build
BINGPPACC=gpp_acc
BINGPPACC1=gpp_acc1
BINGPPACC2=gpp_acc2
BINGPPACC3=gpp_acc3
BINGPPACC4=gpp_acc4

# c++ Acceptance Test Build
BINCPPACC=cpp_acc
BINCPPACC1=cpp_acc1
BINCPPACC2=cpp_acc2
BINCPPACC3=cpp_acc3
BINCPPACC4=cpp_acc4

# g++ portable Build
BINGPPPT=gpp_pt
BINGPPPT1=gpp_pt1
BINGPPPT2=gpp_pt2
BINGPPPT3=gpp_pt3
BINGPPPT4=gpp_pt4

# c++ portable Build
BINCPPPT=cpp_pt
BINCPPPT1=cpp_pt1
BINCPPPT2=cpp_pt2
BINCPPPT3=cpp_pt3
BINCPPPT4=cpp_pt4

# Library Build
BINSOLLIB=sol_lib
BINSOLLIB1=sol_lib1
BINSOLLIB2=sol_lib2
BINSOLLIB3=sol_lib3
BINSOLLIB4=sol_lib4

BINGNULIB=gnu_lib
BINGNULIB1=gnu_lib1
BINGNULIB2=gnu_lib2
BINGNULIB3=gnu_lib3
BINGNULIB4=gnu_lib4

BINLNXLIB=lnx_lib
BINLNXLIB1=lnx_lib1
BINLNXLIB2=lnx_lib2
BINLNXLIB3=lnx_lib3
BINLNXLIB4=lnx_lib4

BINGPPLIB=gpp_lib
BINGPPLIB1=gpp_lib1
BINGPPLIB2=gpp_lib2
BINGPPLIB3=gpp_lib3
BINGPPLIB4=gpp_lib4

BINCPPLIB=cpp_lib
BINCPPLIB1=cpp_lib1
BINCPPLIB2=cpp_lib2
BINCPPLIB3=cpp_lib3
BINCPPLIB4=cpp_lib4

#=======================================================================
# macros for environments
# SECTION 5
#
ifeq ($(MACHINE), BIT64)
ARCH=64
else
ARCH=32
endif

ifeq ($(COMPILER), GPP)
GCC=g++
else
GCC=gcc
endif

# SUN SPARC Compiler for Solaris----------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with Sun Spro C version 4.0 running
# Solaris 2.4
#
# For ANSI Compilation
SOLENV="-DSUNOS -DANSI"
SOLCOPTS="-Xa -g -mt"

# For non-ANSI Compilation
#SOLENV="-DSUNOS"
#SOLCOPTS="-g -o$@"

# For 32 bit normal compilation
#SOLCC="/opt/SUNWspro/bin/cc"
#SOLLD="/opt/SUNWspro/bin/cc"

SOLCC="cc"
SOLLD="cc"

#TODO - For 64-bit enable the below CC/LD.
#SOLCC="/opt/SUNWspro/bin/cc -xarch=v9"
#SOLLD="/opt/SUNWspro/bin/cc -xarch=v9"
#SOLCC="cc -xarch=v9"
#SOLLD="cc -xarch=v9"
SOLLOPTS="-o $(OUTNAME)"
SOLLOPTS1="-o $(OUTNAME) -L$(SSL_LIB_PATH) -lrt -lssl -lcrypto -ldl"
SOLLIBS="-lnsl -lsocket -lpthread -lposix4 -lpython2.4 -lm -ldl "
SOLLINT="lint"
SOLLIBLD="/usr/ccs/bin/ld"
SOLLNTOPTS="-c -F -p -v"
SOLLIBLOPTS="-r -o $(OUTNAME)"
SOLLLIBS=""

# cc386 for VxWorks - Pentium Target
# These flags define the environment required to build an image on Vxworks
# with cc386 on Pentium Target
#
# For ANSI Compilation
VWINTENV="-DANSI -DCPU=PENTIUM -DCPU_VARIANT=PENTIUMPRO -DPQUICCADS"
VWINTCOPTS="-g -mpentium -megft-float -fvolatile -fno-builtin -Wall \
	      -Wno-comment -ansi -pedantic -nostdinc -Wshadow \
         -Wcast-qual -fsigned-char"

# For non-ANSI Compilation
#VWINTENV="-DCPU=PENTIUM -DCPU_VARIANT=PENTIUMPRO -DPQUICCADS"
#VWINTCOPTS="-g -mpentium -megft-float -fvolatile -fno-builtin -Wall \
#	       -Wno-comment -pipe -nostdinc -Wshadow -Wredundant-decls \
#         -Wcast-qual -fsigned-char"

VWINTCC="cc386 -mcpu=pentium"
VWINTLD="ld386"
VWINTLOPTS="-o $(OUTNAME) -r"
VWINTLIBS=""

# SUN SPARC Compiler With Insight-----------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with SUN Spro C version 4.0 C compiler running
# Solaris 2.4 , with the INSURE(Insight) run time memory check tool.
#
# For ANSI Compilation
INSENV="-DSUNOS -DSS_LINUX -DANSI -DPURE -DUSE_PURE"
INSCOPTS="-Xa -g -DEG_INSURE -DNO_ERRCLS"

# For non-ANSI Compilation
#INSENV=-DSUNOS -DPURE
#INSCOPTS="-g -DEG_INSURE -DNO_ERRCLS"

INSCC="insure"
INSLD="insure"
INSLOPTS="-o $(OUTNAME)"
INSLOPTS1="-o $(OUTNAME) -lrt -lssl -lcrypto -ldl"
INSLIBS="-Xlinker -export-dynamic -lnsl -lpthread -lpython2.4 -lm -ldl -lutil"
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

PPCENV=-DCPU=SIMNT -DANSI -DPQUICCADS
#PPCENV=-DANSI -DPCORE -DCPU=PPC604
PPCCC=ccpentium
PPCLD=ldpentium


PPCCOPTS=-g -mtune=i486 -fvolatile -fno-builtin -Wall \
	      -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
	      -Wcast-qual -fsigned-char

PPCLOPTS=-r -e vwMain -Map $(OUTNAME).map -o $(OUTNAME)
PPCLIBS=""
PPCLINT=
PPCLNTOPTS=

# GNU gcc compiler for SPARC--------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
# For ANSI Compilation
GNUENV="-DSUNOS -DANSI"
GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
	  -Wstrict-prototypes -Wmissing-prototypes"

# For non-ANSI Compilation
#GNUENV="-DSUNOS"
#GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual"

GNUCC="$(GCC)"
GNULD="$(GCC)"
GNULOPTS="-o $(OUTNAME)"
GNULOPTS1="-o $(OUTNAME) -L$(SSL_LIB_PATH) -lrt -lssl -lcrypto -ldl"
GNULIBS="-lpython2.4 -lm -ldl -lnsl -lsocket -lpthread -lposix4"
GNULINT="lint"
GNULNTOPTS="-c -F -p -v"

GNULIBLD="ld"
GNULIBLOPTS="-r -o $(OUTNAME)"
GNULLIBS=""

# GNU gcc compiler for Linux------------------------------------------
#
# These flags define the environment required to build an image on
# a Linux workstation with GNU C version 2.5.7 (or greater)
#
# For ANSI Compilation
#LNXENV="-DSUNOS -DSS_LINUX -DANSI -DUSE_PURE -DPURE"
LNXENV="-DSUNOS -DSS_LINUX -DANSI "

# For non-ANSI Compilation
#LNXENV="-DSUNOS -DSS_LINUX"
#LNXCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
#	  -Wcast-align -Wcast-qual"

# For Cavium Compilation
#LNXCC="mips64-octeon-linux-gnu-gcc -DALIGN_64BIT -DBIT_64"
#LNXLD="mips64-octeon-linux-gnu-gcc"
#LNXLIBLD="mips64-octeon-linux-gnu-ar"

ifeq ($(MACHINE), BIT64)
LNXCOPTS=" -O3 -march=core2 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment \
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

LNXCC="$(GCC) -m$(ARCH) "
LNXLD="$(GCC) -m$(ARCH)"
LNXLIBLD="$(GCC) -m$(ARCH)"

LNXLOPTS="-o $(OUTNAME) -Xlinker -export-dynamic -lpthread"
#LNXCC="gcc -m64 -DALIGN_64BIT -DBIT_64"
#LNXLD="gcc -m64 -DALIGN_64BIT -DBIT_64"
#LNXLOPTS=" -m64 -DALIGN_64BIT -DBIT_64 -o $(OUTNAME) -Xlinker -export-dynamic -lpthread"
LNXLOPTS1="-o $(OUTNAME) -Xlinker -export-dynamic -lpthread  -lnsl -lrt -lssl -lcrypto -ldl"
LNXLIBS="-lpython2.4 -lm -ldl -lutil"
LNXLIBS4="-lm -ldl -lutil"
LNXLINT="lint"
LNXLIBLD="ld"
LNXLNTOPTS="-c -F -p -v"
LNXLIBLOPTS="-r -o $(OUTNAME)"
LNXLLIBS=""
LNXCCCC="cc"

# Purify gcc compiler for Linux------------------------------------------
#
# These flags define the environment required to build an image on
# a Linux workstation with GNU C version 2.5.7 (or greater)
#
# For ANSI Compilation
PURENV="-DSUNOS -DSS_LINUX -DANSI -DPURE -DUSE_PURE"
PURCOPTS="-g3 -ansi -pipe -pedantic -Wall -Wno-comment -Wshadow \
	  -Wcast-align -Wcast-qual \
	  -Wstrict-prototypes -Wmissing-prototypes"

# For non-ANSI Compilation
#PURENV="-DSUNOS -DSS_LINUX -DPURE -DUSE_PURE"
#PURCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
#	  -Wcast-align -Wcast-qual"

PURCC="purify -cache-dir=\tmp -windows=yes $(GCC) -m$(ARCH)"
PURLD="purify -cache-dir=\tmp -windows=yes $(GCC) -m$(ARCH)"
PURLOPTS="-o $(OUTNAME) -Xlinker -export-dynamic -lpthread"
PURLOPTS1="-o $(OUTNAME) -Xlinker -export-dynamic -lpthread  -lrt -lssl -lcrypto -ldl"
PURLIBS="-lpython2.4 -lm -ldl -lutil"
PURLINT="lint"
PURLNTOPTS="-c -F -p -v"

# Quantify gcc compiler for Linux--------------------------------------
QUANTIFYCC="quantify $(GCC)"
QUANTIFYLD="quantify $(GCC)"

# Purecov gcc compiler for Linux--------------------------------------
PURCOVCC="purecov -cache-dir=\tmp -windows=yes $(GCC) -m$(ARCH)"
PURCOVLD="purecov -cache-dir=\tmp -windows=yes $(GCC) -m$(ARCH)"

#  g++ compiler for Linux--------------------------------------
GPPCC="g++"
GPPLD="g++"
GPPLIBLD="ld"
GPPCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual"

#  c++ compiler for Linux--------------------------------------
CPPCC="c++"
CPPLD="c++"
CPPCOPTS="-g"
CPPLIBLD="ld"



#  g++ compiler for Solaris--------------------------------------
#GPPCC="g++"
#GPPLD="g++"
#GPPLIBLD="ld"
#GPPCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual"

#  c++ compiler for Solaris--------------------------------------
#CPPCC="c++"
#CPPLD="c++"
#CPPCOPTS="-g"
#CPPLIBLD="ld"


# MRI mcc68k compiler for 68040-----------------------------------------
#
# These flags define the environment required to build an image for
# a Motorola MVME162 Board (68040) with Microtec Research C version 4.4
# for a portable environment.
#
# For ANSI Compilation
MRIENV="-DELVIS -DPROC_68XXX -DPROC_68040 -DRY_ENBRAW \
	-DSOLMRI43 -DMAXBRDS=4 -DANSI -DNOERRCHK -DNO_ERRCLS -DNOCONAVL "

# For non-ANSI Compilation
#MRIENV="-DELVIS -DPROC_68XXX -DPROC_68040 -DENB_RELAY -DRY_ENBRAW \
#         -DSOLMRI43 -DMAXBRDS=4 -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"

MRICC="mcc68k"
MRILD="mcc68k"
MRICOPTS="-p68040 -H -A -v -Fsm -O -Ob -Oe -Ot -nKc -nKm  -Mcp -Mdp -Ml -Gf"
MRILOPTS="-e$(OUTNAME).lnk -Wl,\"-m > $(OUTNAME).map -o $(OUTNAME).vcs -F s\""
MRILIBS="$(LIB_PATH)/lib040.lib"
MRILINT=""
MRILNTOPTS=""

# Microegft Visual C++ for Win 95/NT------------------------------------
#
# These flags define the environment required to build an image for
# a Windows '95 or Windows NT environment
#
#
# For ANSI Compilation
WINENV="-DWIN32 -DANSI -D_CRT_SECURE_NO_DEPRECATE"

# For non-ANSI Compilation
#WINENV="-DWIN32"

WINCC="cl"
WINLD="cl"
WINCOPTS="-Od -W3 -Zi -Yd"
WINLOPTS="-o $(OUTNAME) -F250000 -Zd"
WINLIBS="ws2_32.lib ssleay32MT.lib libeay32MT.lib -link -debug:full -debugtype:cv"
WINLINT=""
WINLNTOPTS=""


# Microegft Visual C++ for DOS------------------------------------------
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


#Actual Build Environment---------------------------------
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


# object file suffix----------------------------------------------------
# TODO - For windows Change the suffix to obj & for other platforms .o

# this defaults to 'o' for most UNIX based systems
# this can be set to 'obj' for DOS/Windows/NT platforms
#
UNIX_OBJ_SUFFIX=o
WIN_OBJ_SUFFIX=obj
OBJ=$(UNIX_OBJ_SUFFIX)
#OBJ=$(WIN_OBJ_SUFFIX)

#=======================================================================
#
# macros for compile time options
#
# combined environment, compiler options and include path
CENVOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH)

# portable options
CPTOPTS=$(CENVOPTS) $(GENPTOSOPTS) $(OSOPTS)

# library options
CLIBOPTS=$(CENVOPTS) $(GENLIBOSOPTS) $(OSOPTS)

# acceptance test options
CACCOPTS=$(CENVOPTS) $(GENACCOSOPTS) $(OSOPTS)

# product options for portable build
CPRDPTOPTS="$(CPTOPTS) $(EGPTOPTS)"
CPRDPTOPTS1="$(CPTOPTS) $(EGPTOPTS1)"
CPRDPTOPTS2="$(CPTOPTS) $(EGPTOPTS2)"
CPRDPTOPTS3="$(CPTOPTS) $(EGPTOPTS3)"
CPRDPTOPTS4="$(CPTOPTS) $(EGPTOPTS4)"

# product options for acceptance test build
CPRDACCOPTS="$(CACCOPTS) $(EGACCOPTS)"
CPRDACCOPTS1="$(CACCOPTS) $(EGACCOPTS1)"
CPRDACCOPTS2="$(CACCOPTS) $(EGACCOPTS2)"
CPRDACCOPTS3="$(CACCOPTS) $(EGACCOPTS3)"
CPRDACCOPTS4="$(CACCOPTS) $(EGACCOPTS4)"

# product options for library build
CPRDLIBOPTS="$(CLIBOPTS) $(EGLIBOPTS)"
CPRDLIBOPTS1="$(CLIBOPTS) $(EGLIBOPTS1)"
CPRDLIBOPTS2="$(CLIBOPTS) $(EGLIBOPTS2)"
CPRDLIBOPTS3="$(CLIBOPTS) $(EGLIBOPTS3)"
CPRDLIBOPTS4="$(CLIBOPTS) $(EGLIBOPTS4)"

# common stack manager file options for portable build
CSMPTOPTS="$(CPTOPTS) $(EGPTOPTS)"
CSMPTOPTS1="$(CPTOPTS) $(EGPTOPTS1)"
CSMPTOPTS2="$(CPTOPTS) $(EGPTOPTS2)"
CSMPTOPTS3="$(CPTOPTS) $(EGPTOPTS3)"
CSMPTOPTS4="$(CPTOPTS) $(EGPTOPTS4)"

# common stack manager file options for acceptance test build
CSMACCOPTS="$(CACCOPTS) $(EGACCOPTS)"
CSMACCOPTS1="$(CACCOPTS) $(EGACCOPTS1)"
CSMACCOPTS2="$(CACCOPTS) $(EGACCOPTS2)"
CSMACCOPTS3="$(CACCOPTS) $(EGACCOPTS3)"
CSMACCOPTS4="$(CACCOPTS) $(EGACCOPTS4)"

# common stack manager file options for library build
CSMLIBOPTS="$(CLIBOPTS) $(EGLIBOPTS)"
CSMLIBOPTS1="$(CLIBOPTS) $(EGLIBOPTS1)"
CSMLIBOPTS2="$(CLIBOPTS) $(EGLIBOPTS2)"
CSMLIBOPTS3="$(CLIBOPTS) $(EGLIBOPTS3)"
CSMLIBOPTS4="$(CLIBOPTS) $(EGLIBOPTS4)"


# OS options
# Check if Flag CONVAL should be enbale from here
COSOPTS=$(CENVOPTS) $(GENACCOSOPTS) $(OSOPTS) -DCONAVL

#=======================================================================
#
# macros for depend and lint options
#

# depend acceptance test
DEPOPTS_ACC="$(EGACCOPTS)"
DEPOPTS_ACC1="$(EGACCOPTS1)"
DEPOPTS_ACC2="$(EGACCOPTS2)"
DEPOPTS_ACC3="$(EGACCOPTS3)"
DEPOPTS_ACC4="$(EGACCOPTS4)"

# depend portable test
DEPOPTS_PT="$(EGPTOPTS)"

#=======================================================================
# Macros for hardcoded dependencies
# SECTION 6
#
# The following are enabled by default since we assume makedepend
# is not available. If makedepend is available in your environemnt
# uncomment out this section.
#
#
# environment
ENV_INC=$(CM_DIR)/envdep.h $(CM_DIR)/envopt.h

# system service interface
SSI_INC=$(CM_DIR)/ssi.h $(CM_DIR)/ssi.x

# general
GEN_INC=$(CM_DIR)/gen.h $(CM_DIR)/gen.x

# all system includes
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)

# product layer management - LEG includes
LEG_INC=$(EG_DIR)/leg.h $(EG_DIR)/leg.x

# TUCL Layer Management - LHI includes
LHI_INC=$(HI_DIR)/lhi.h $(HI_DIR)/lhi.x

# eGTP Upper Interface - EGT includes
EGT_INC = $(CM_DIR)/egt.h $(CM_DIR)/egt.x

# product lower interface - HIT includes
HIT_INC=$(CM_DIR)/hit.h $(CM_DIR)/hit.x

# common function library includes
CM_INC= $(CM_DIR)/cm_err.h   $(CM_DIR)/cm_lib.x   \
	$(CM_DIR)/cm_hash.h  $(CM_DIR)/cm_hash.x  \
	$(CM_DIR)/cm_llist.h $(CM_DIR)/cm_llist.x \
	$(CM_DIR)/cm_gen.h   $(CM_DIR)/cm_gen.x   \
	$(CM_DIR)/cm_tpt.h   $(CM_DIR)/cm_tpt.x   \
	$(CM_DIR)/cm_inet.h  $(CM_DIR)/cm_inet.x  \
	$(CM_DIR)/cm_tkns.h  $(CM_DIR)/cm_tkns.x  \
	$(CM_DIR)/cm_mblk.h  $(CM_DIR)/cm_mblk.x  \
	$(CM_DIR)/cm_xtree.x $(CM_DIR)/cm_dns.x   \
	$(CM_DIR)/cm_mem.h   $(CM_DIR)/cm_mem.x

# TUCL includes
HI_INC=$(HI_DIR)/hi.h $(HI_DIR)/hi.x $(HI_DIR)/hi_err.h

# GTP includes
EG_INC = $(EG_DIR)/eg.h $(EG_DIR)/eg.x            \
	 $(EG_DIR)/eg_edm.x                        \

# all product includes
EGALL_INC=$(LEG_INC) $(EG_INC) $(HIT_INC) $(EGT_INC)

#TODO - FOR Real TUCL include this.
#EGALL_INC=$(LEG_INC) $(LHI_INC) $(EG_INC) $(HI_INC) $(HIT_INC) $(EGT_INC)

# sample acceptance tests all includes
EGACCALL_INC=$(EGALL_INC) $(EGACC_INC)

# OS specific includes - SS includes
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h \
       $(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x \
       $(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x \
       $(SS_DIR)/ss_msg.h $(SS_DIR)/ss_msg.x \
       $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x \
       $(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x \
       $(SS_DIR)/ss_task.h $(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x

# OS specific includes - MTSS includes
MT_INC = $(MT_DIR)/mt_ss.h \
	 $(MT_DIR)/mt_ss.x \
	 $(MT_DIR)/mt_err.h

#=======================================================================
# Macros for source and object files
# SECTION 7

# acceptance test controller source ----------------------------------

# acceptance test controller object ----------------------------------
TC_OBJS= $(SY_DIR)/cm_xta_ipc.$(OBJ) cm_xta_slave.$(OBJ) cm_xta_reg.$(OBJ) egac_slave.$(OBJ) egac_prof.$(OBJ)


# common source(C source files) for TC--------------------------------
TC_CM_SRCS = $(CM_DIR)/cm_gen.c  \
	  $(CM_DIR)/cm_lib.c  \
	  $(CM_DIR)/cm_hash.c \
	  $(CM_DIR)/cm_bdy5.c \
	  $(CM_DIR)/cm_inet.c \
	  $(CM_DIR)/cm_tpt.c

# common objects for TC ----------------------------------------------
TC_CM_OBJS = $(SY_DIR)/cm_gen.$(OBJ)  \
	  $(SY_DIR)/cm_lib.$(OBJ)  \
	  $(SY_DIR)/cm_hash.$(OBJ) \
	  $(SY_DIR)/cm_bdy5.$(OBJ) \
	  $(SY_DIR)/cm_inet.$(OBJ) \
	  $(SY_DIR)/cm_tpt.$(OBJ)

EGNEWAC_SRCS = $(EGNEWACC_DIR)/egac_egtutl.c \
	$(EGNEWACC_DIR)/egac_cmutl.c \
	$(EGNEWACC_DIR)/cm_xta_txn.c \
	$(EGNEWACC_DIR)/cm_xta_intutl.c \
	$(EGNEWACC_DIR)/cm_xta_tmr.c \
	$(EGNEWACC_DIR)/cm_xta_dum.c \
	$(EGNEWACC_DIR)/egac_leghdl.c \
	$(EGNEWACC_DIR)/egac_egthdl.c \
	$(EGNEWACC_DIR)/egac_hithdl.c \
	$(EGNEWACC_DIR)/egac_hitutl.c \
	$(EGNEWACC_DIR)/egac_lhihdl.c \
	$(EGNEWACC_DIR)/egac_lhiutl.c \
	$(EGNEWACC_DIR)/cm_xta_inthdl.c \
	$(EGNEWACC_DIR)/cm_xta_intreg.c \
	$(EGNEWACC_DIR)/egac_legreg.c \
	$(EGNEWACC_DIR)/egac_prof.c \
	$(EGNEWACC_DIR)/egac_lhicb.c \
	$(EGNEWACC_DIR)/egac_legcb.c \
	$(EGNEWACC_DIR)/egac_hitcb.c \
	$(EGNEWACC_DIR)/egac_egthdl.c \
	$(EGNEWACC_DIR)/egac_egtcb.c \
	$(EGNEWACC_DIR)/cm_xta_res.c \
	$(EGNEWACC_DIR)/cm_xta_queue.c \
	$(EGNEWACC_DIR)/egac_legutl.c \
	$(EGNEWACC_DIR)/cm_xta_log.c \
	$(EGNEWACC_DIR)/cm_xta_slave.c \
	$(EGNEWACC_DIR)/cm_xta_master.c \
	$(EGNEWACC_DIR)/cm_xta_reg.c \
	$(EGNEWACC_DIR)/cm_xta_tst.c \
	$(EGNEWACC_DIR)/cm_xta_ipc.c \
	$(EGNEWACC_DIR)/egac_tst.c \
	$(EGNEWACC_DIR)/cm_xta_flag.c \
	$(EGNEWACC_DIR)/egac_flag.c \
	$(EGNEWACC_DIR)/cm_xta_py.c \
	$(EGNEWACC_DIR)/eu_ex_ms.c \
	$(EGNEWACC_DIR)/eu_ptli.c \
	$(EGNEWACC_DIR)/cm_xta_tcfsm.c \
	$(EGNEWACC_DIR)/cm_xta_ldfsm.c \
	$(EGNEWACC_DIR)/cm_xta_xml.c \
	$(EGNEWACC_DIR)/egac_lhireg.c \
	$(EGNEWACC_DIR)/cm_xta_ex_ms.c \
	$(EGNEWACC_DIR)/egac_hi_ex_ms.c \
	$(EGNEWACC_DIR)/egac_egld.c \
	$(EGNEWACC_DIR)/cm_xta_tgfsm.c \
	$(EGNEWACC_DIR)/cm_xta_tafsm.c


# new acceptance test object-------------------------------------------
EGNEWAC_OBJS = $(SY_DIR)/egac_egtutl.$(OBJ) \
	$(SY_DIR)/egac_cmutl.$(OBJ) \
	$(SY_DIR)/cm_xta_txn.$(OBJ) \
	$(SY_DIR)/cm_xta_tmr.$(OBJ) \
	$(SY_DIR)/eu_ex_ms.$(OBJ) \
	$(SY_DIR)/eu_ptli.$(OBJ) \
	$(SY_DIR)/egac_hithdl.$(OBJ) \
	$(SY_DIR)/egac_hitutl.$(OBJ) \
	$(SY_DIR)/egac_egthdl.$(OBJ) \
	$(SY_DIR)/egac_leghdl.$(OBJ) \
	$(SY_DIR)/egac_legreg.$(OBJ) \
	$(SY_DIR)/egac_prof.$(OBJ) \
	$(SY_DIR)/egac_legcb.$(OBJ) \
	$(SY_DIR)/egac_hitcb.$(OBJ) \
	$(SY_DIR)/egac_egtcb.$(OBJ) \
	$(SY_DIR)/cm_xta_res.$(OBJ) \
	$(SY_DIR)/cm_xta_queue.$(OBJ) \
	$(SY_DIR)/egac_legutl.$(OBJ) \
	$(SY_DIR)/cm_xta_slave.$(OBJ) \
	$(SY_DIR)/cm_xta_master.$(OBJ) \
	$(SY_DIR)/cm_xta_reg.$(OBJ) \
	$(SY_DIR)/cm_xta_tst.$(OBJ) \
	$(SY_DIR)/cm_xta_log.$(OBJ) \
	$(SY_DIR)/cm_xta_ipc.$(OBJ) \
	$(SY_DIR)/egac_tst.$(OBJ) \
	$(SY_DIR)/cm_xta_flag.$(OBJ) \
	$(SY_DIR)/egac_flag.$(OBJ) \
	$(SY_DIR)/cm_xta_intreg.$(OBJ) \
	$(SY_DIR)/cm_xta_inthdl.$(OBJ) \
	$(SY_DIR)/cm_xta_intutl.$(OBJ) \
	$(SY_DIR)/cm_xta_py.$(OBJ) \
	$(SY_DIR)/cm_xta_tcfsm.$(OBJ) \
	$(SY_DIR)/cm_xta_ldfsm.$(OBJ) \
	$(SY_DIR)/cm_xta_xml.$(OBJ) \
	$(SY_DIR)/cm_xta_ex_ms.$(OBJ) \
	$(SY_DIR)/egac_hi_ex_ms.$(OBJ) \
	$(SY_DIR)/egac_egld.$(OBJ) \
	$(SY_DIR)/cm_xta_tgfsm.$(OBJ) \
	$(SY_DIR)/cm_xta_tafsm.$(OBJ) \
	$(SY_DIR)/egac_hitreg.$(OBJ) \
	$(SY_DIR)/egac_egtreg.$(OBJ) \


EGNEWAC_SRCS1 = $(EGNEWACC_DIR)/egac_edmhdl.c \
	$(EGNEWACC_DIR)/egac_cmutl.c \
	$(EGNEWACC_DIR)/egac_edmutl.c \
	$(EGNEWACC_DIR)/egac_edmreg.c \
	$(EGNEWACC_DIR)/egac_edmcb.c \
	$(EGNEWACC_DIR)/cm_xta_txn.c \
	$(EGNEWACC_DIR)/cm_xta_intutl.c \
	$(EGNEWACC_DIR)/cm_xta_tmr.c \
	$(EGNEWACC_DIR)/cm_xta_dum.c \
	$(EGNEWACC_DIR)/cm_xta_inthdl.c \
	$(EGNEWACC_DIR)/cm_xta_intreg.c \
	$(EGNEWACC_DIR)/cm_xta_res.c \
	$(EGNEWACC_DIR)/cm_xta_queue.c \
	$(EGNEWACC_DIR)/cm_xta_log.c \
	$(EGNEWACC_DIR)/cm_xta_slave.c \
	$(EGNEWACC_DIR)/cm_xta_master.c \
	$(EGNEWACC_DIR)/cm_xta_reg.c \
	$(EGNEWACC_DIR)/cm_xta_tst.c \
	$(EGNEWACC_DIR)/cm_xta_ipc.c \
	$(EGNEWACC_DIR)/egac_edmtst.c \
	$(EGNEWACC_DIR)/cm_xta_flag.c \
	$(EGNEWACC_DIR)/egac_flag.c \
	$(EGNEWACC_DIR)/egac_egld.c \
	$(EGNEWACC_DIR)/cm_xta_py.c \
	$(EGNEWACC_DIR)/cm_xta_tcfsm.c \
	$(EGNEWACC_DIR)/cm_xta_ldfsm.c \
	$(EGNEWACC_DIR)/cm_xta_xml.c \
	$(EGNEWACC_DIR)/cm_xta_ex_ms.c \
	$(EGNEWACC_DIR)/cm_xta_tgfsm.c \
	$(EGNEWACC_DIR)/cm_xta_tafsm.c

# new acceptance test object-------------------------------------------
EGNEWAC_OBJS1 = $(SY_DIR)/egac_edmhdl.$(OBJ) \
	$(SY_DIR)/cm_xta_txn.$(OBJ) \
	$(SY_DIR)/cm_xta_tmr.$(OBJ) \
	$(SY_DIR)/egac_edmutl.$(OBJ) \
	$(SY_DIR)/egac_cmutl.$(OBJ) \
	$(SY_DIR)/egac_edmreg.$(OBJ) \
	$(SY_DIR)/egac_edmcb.$(OBJ) \
	$(SY_DIR)/cm_xta_res.$(OBJ) \
	$(SY_DIR)/cm_xta_queue.$(OBJ) \
	$(SY_DIR)/cm_xta_slave.$(OBJ) \
	$(SY_DIR)/cm_xta_master.$(OBJ) \
	$(SY_DIR)/cm_xta_reg.$(OBJ) \
	$(SY_DIR)/cm_xta_tst.$(OBJ) \
	$(SY_DIR)/cm_xta_log.$(OBJ) \
	$(SY_DIR)/cm_xta_ipc.$(OBJ) \
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
	$(SY_DIR)/egac_edmtst.$(OBJ) \
	$(SY_DIR)/egac_egld.$(OBJ) \
	$(SY_DIR)/egac_flag.$(OBJ) \
	$(SY_DIR)/cm_xta_tafsm.$(OBJ)


# new acceptance test object--------------------------------------------
EGNEWTC_OBJS = $(SY_DIR)/cm_xta_slave.$(OBJ)

# sample upper layer source---------------------------------------------
EU_SRCS = $(EGACC_DIR)/eu_bdy1.c  \
	    $(EGACC_DIR)/eu_ex_ms.c \
	    $(EGACC_DIR)/eu_ptli.c

# sample upper layer objects--------------------------------------------
EU_OBJS = $(SY_DIR)/eu_bdy1.$(OBJ)  \
	    $(SY_DIR)/eu_ex_ms.$(OBJ) \
	    $(SY_DIR)/eu_ptli.$(OBJ)

# sample upper layer source---------------------------------------------
EU_SRCS1 = $(EGACC_DIR)/egac_ptedm.c  \

# sample upper layer objects--------------------------------------------
EU_OBJS1 = $(SY_DIR)/egac_ptedm.$(OBJ)  \


# sample upper layer source---------------------------------------------
EU_SRCS3 = $(EGACC_DIR)/eu_bdy1.c  \
	    $(EGACC_DIR)/eu_ptli.c

# sample upper layer objects--------------------------------------------
EU_OBJS3 = $(SY_DIR)/eu_ptli.$(OBJ)  \


# Product source------------------------------------------------
EG_SRCS = \
      $(EG_DIR)/eg_li.c \
      $(EG_DIR)/eg_ptli.c\
      $(EG_DIR)/eg_tpt.c\
      $(EG_DIR)/eg_ui.c\
      $(EG_DIR)/eg_ptui.c\
      $(EG_DIR)/eg_edm.c\
	   $(EG_DIR)/eg_dbs3.c \
	   $(EG_DIR)/eg_dbs4.c \
	   $(EG_DIR)/eg_dbs5.c \
	   $(EG_DIR)/eg_dbs10.c \
	   $(EG_DIR)/eg_dbs11.c \
	   $(EG_DIR)/eg_dbsm.c \
	   $(EG_DIR)/eg_dbsn.c \
	   $(EG_DIR)/eg_dbsi.c \
      $(EG_DIR)/eg_dbs16.c \
	   $(EG_DIR)/eg_dbutl.c \
      $(EG_DIR)/eg_id.c\
      $(EG_DIR)/eg_ptmi.c\
      $(EG_DIR)/eg_tunn.c\
      $(EG_DIR)/eg_util.c\
      $(EG_DIR)/eg_ex_ms.c\
      $(EG_DIR)/eg_mi.c\
      $(EG_DIR)/eg_cpm.c\
      $(EG_DIR)/eg_tmr.c\


# Product objects-------------------------------------------------------
EG_OBJS = \
      $(SY_DIR)/eg_li.$(OBJ) \
      $(SY_DIR)/eg_ptli.$(OBJ)  \
      $(SY_DIR)/eg_tpt.$(OBJ)  \
      $(SY_DIR)/eg_ui.$(OBJ) \
      $(SY_DIR)/eg_ptui.$(OBJ)  \
      $(SY_DIR)/eg_id.$(OBJ)  \
      $(SY_DIR)/eg_ptmi.$(OBJ)  \
      $(SY_DIR)/eg_cpm.$(OBJ)  \
      $(SY_DIR)/eg_tunn.$(OBJ) \
      $(SY_DIR)/eg_util.$(OBJ) \
      $(SY_DIR)/eg_ex_ms.$(OBJ) \
      $(SY_DIR)/eg_mi.$(OBJ) \
	   $(SY_DIR)/eg_dbutl.$(OBJ) \
	   $(SY_DIR)/eg_dbs3.$(OBJ) \
	   $(SY_DIR)/eg_dbs4.$(OBJ) \
	   $(SY_DIR)/eg_dbs5.$(OBJ) \
	   $(SY_DIR)/eg_dbs10.$(OBJ) \
	   $(SY_DIR)/eg_dbs11.$(OBJ) \
	   $(SY_DIR)/eg_dbsm.$(OBJ) \
	   $(SY_DIR)/eg_dbsn.$(OBJ) \
	   $(SY_DIR)/eg_dbsi.$(OBJ) \
	   $(SY_DIR)/eg_dbs16.$(OBJ) \
      $(SY_DIR)/eg_edm.$(OBJ)  \
      $(SY_DIR)/eg_tmr.$(OBJ) \
		$(SY_DIR)/egt_util.$(OBJ)


# Product source------------------------------------------------
EG_SRCS1 = $(EG_DIR)/eg_edm.c \
	        $(EG_DIR)/eg_dbs11.c \
	        $(EG_DIR)/eg_dbsi.c \
	        $(EG_DIR)/eg_dbsm.c \
	        $(EG_DIR)/eg_dbsn.c \
	        $(EG_DIR)/eg_dbs3.c \
	        $(EG_DIR)/eg_dbs5.c \
	        $(EG_DIR)/eg_dbs10.c \
	        $(EG_DIR)/eg_dbsm.c \
	        $(EG_DIR)/eg_dbsn.c \
	        $(EG_DIR)/eg_dbsi.c \
	        $(EG_DIR)/eg_dbs16.c \
	        $(EG_DIR)/eg_dbutl.c \
           $(EG_DIR)/eg_util.c \
           $(EG_DIR)/egt_util.c \

# Product objects-------------------------------------------------------
EG_OBJS1 = \
      $(SY_DIR)/eg_edm.$(OBJ) \
	   $(SY_DIR)/eg_dbs3.$(OBJ) \
	   $(SY_DIR)/eg_dbs4.$(OBJ) \
	   $(SY_DIR)/eg_dbs5.$(OBJ) \
	   $(SY_DIR)/eg_dbs10.$(OBJ) \
	   $(SY_DIR)/eg_dbs11.$(OBJ) \
	   $(SY_DIR)/eg_dbsm.$(OBJ) \
	   $(SY_DIR)/eg_dbsn.$(OBJ) \
	   $(SY_DIR)/eg_dbsi.$(OBJ) \
	   $(SY_DIR)/eg_dbs16.$(OBJ) \
	   $(SY_DIR)/eg_dbutl.$(OBJ) \
      $(SY_DIR)/eg_util.$(OBJ)


# TUCL source-------------------------------------------------
HI_SRCS =

# TUCL objects------------------------------------------------
HI_OBJS =

# common source(C source files)-----------------------------------------
EGCM_SRCS = \
    $(CM_DIR)/egt.c \
    $(EG_DIR)/leg.c
#    $(CM_DIR)/cm_gen.c \
#    $(CM_DIR)/cm_lib.c \
#    $(CM_DIR)/cm_hash.c \
#    $(CM_DIR)/cm_llist.c \
#    $(CM_DIR)/cm_bdy5.c \
#    $(CM_DIR)/cm_inet.c \
#    $(CM_DIR)/cm_tpt.c \
#    $(CM_DIR)/cm_tkns.c \
#    $(CM_DIR)/cm_mblk.c \
#    $(CM_DIR)/cm_dns.c \

# common objects--------------------------------------------------------
EGCM_OBJS = \
	$(SY_DIR)/leg.$(OBJ) \
	$(SY_DIR)/egt.$(OBJ)
  # $(SY_DIR)/cm_gen.$(OBJ) \
#	$(SY_DIR)/cm_lib.$(OBJ) \
#	$(SY_DIR)/cm_hash.$(OBJ) \
#	$(SY_DIR)/cm_llist.$(OBJ) \
#	$(SY_DIR)/cm_bdy5.$(OBJ) \
##	$(SY_DIR)/cm_inet.$(OBJ) \
#	$(SY_DIR)/cm_tpt.$(OBJ) \
##	$(SY_DIR)/cm_tkns.$(OBJ) \
#	$(SY_DIR)/cm_mblk.$(OBJ) \
#	$(SY_DIR)/cm_dns.$(OBJ) \
#	$(SY_DIR)/hit.$(OBJ) \

# common stack manager source-------------------------------------------
SM_SRCS =

# common stack manager objects------------------------------------------
SM_OBJS =

# Product stack manager source------------------------------------------
EGSM_SRCS = \
	    $(EGSM_DIR)/smegptmi.c
#       $(EGSM_DIR)/smegbdy1.c \
#	    $(EGSM_DIR)/smegexms.c \
#	    $(EGSM_DIR)/smeg_ieutl.c \
#	    $(EGSM_DIR)/smegs5.c \
#	    $(EGSM_DIR)/smeg_s3utl.c \
#	    $(EGSM_DIR)/smeg_s4utl.c \
#	    $(EGSM_DIR)/smeg_s5utl.c \
#	    $(EGSM_DIR)/smeg_s10utl.c \
#	    $(EGSM_DIR)/smeg_s11utl.c \
#	    $(EGSM_DIR)/smeg_s16utl.c

# Product stack manager objects-----------------------------------------
EGSM_OBJS = \
	    $(SY_DIR)/smegptmi.$(OBJ)
#       $(SY_DIR)/smegbdy1.$(OBJ) \
	    $(SY_DIR)/smegexms.$(OBJ) \
	    $(SY_DIR)/smeg_ieutl.$(OBJ) \
	    $(SY_DIR)/smeg_s3utl.$(OBJ) \
	    $(SY_DIR)/smeg_s4utl.$(OBJ) \
	    $(SY_DIR)/smeg_s5utl.$(OBJ) \
	    $(SY_DIR)/smeg_s10utl.$(OBJ) \
	    $(SY_DIR)/smeg_s11utl.$(OBJ) \
	    $(SY_DIR)/smeg_s16utl.$(OBJ) \

# Product stack manager source------------------------------------------
EGSM_SRCS1 = $(EGSM_DIR)/smegbdy1.c \
	    $(EGSM_DIR)/smegexms.c \
	    $(EGSM_DIR)/smeg_ieutl.c \
	    $(EGSM_DIR)/smegs5.c \
	    $(EGSM_DIR)/smeg_s3utl.c \
	    $(EGSM_DIR)/smeg_s4utl.c \
	    $(EGSM_DIR)/smeg_s5utl.c \
	    $(EGSM_DIR)/smeg_s10utl.c \
	    $(EGSM_DIR)/smeg_s11utl.c \
	    $(EGSM_DIR)/smeg_s16utl.c

# TUCL stack manager source------------------------------------------
HISM_SRCS = $(HISM_DIR)/smhibdy1.c \
	    $(HISM_DIR)/smhiexms.c \
	    $(HISM_DIR)/smhiptmi.c

# TUCL stack manager objects-----------------------------------------
HISM_OBJS = $(SY_DIR)/smhibdy1.$(OBJ) \
	    $(SY_DIR)/smhiexms.$(OBJ) \
	    $(SY_DIR)/smhiptmi.$(OBJ)

# OS - common sources---------------------------------------------------
SS_SRCS = $(SS_DIR)/ss_gen.c              \
	  $(SS_DIR)/ss_task.c             \
	  $(SS_DIR)/ss_drvr.c             \
	  $(SS_DIR)/ss_timer.c            \
	  $(SS_DIR)/ss_mem.c              \
	  $(SS_DIR)/ss_strm.c             \
	  $(SS_DIR)/ss_msg.c              \
	  $(SS_DIR)/ss_queue.c            \
	  $(SS_DIR)/ss_pack.c             \
	  $(SS_DIR)/ss_rtr.c              \
	  $(CM_DIR)/cm_mem.c

# OS - common objects---------------------------------------------------
SS_OBJS = $(SY_DIR)/ss_gen.$(OBJ)         \
	  $(SY_DIR)/ss_task.$(OBJ)        \
	  $(SY_DIR)/ss_drvr.$(OBJ)        \
	  $(SY_DIR)/ss_timer.$(OBJ)       \
	  $(SY_DIR)/ss_mem.$(OBJ)         \
	  $(SY_DIR)/ss_strm.$(OBJ)        \
	  $(SY_DIR)/ss_msg.$(OBJ)         \
	  $(SY_DIR)/ss_queue.$(OBJ)       \
	  $(SY_DIR)/ss_pack.$(OBJ)        \
	  $(SY_DIR)/ss_rtr.$(OBJ)         \
	  $(SY_DIR)/cm_mem.$(OBJ)

# OS - MT sources-------------------------------------------------------
MT_SRCS= \
	$(OS_DIR)/mt_ss.c               \
	$(OS_DIR)/mt_id.c

# OS - MT objects-------------------------------------------------------
MT_OBJS= \
	$(SY_DIR)/mt_ss.$(OBJ)          \
	$(SY_DIR)/mt_id.$(OBJ)

# OS - NT sources-------------------------------------------------------
NS_SRCS= \
	$(OS_DIR)/ns_gen.c              \
   $(OS_DIR)/smnsexms.c            \
   $(OS_DIR)/smnsbdy1.c            \
	$(OS_DIR)/smnsptmi.c            \
	$(OS_DIR)/lns.c                 \
	$(OS_DIR)/ns_ex_ms.c            \
	$(OS_DIR)/ns_id.c               \
	$(OS_DIR)/ns_ptmi.c             \
	$(OS_DIR)/ns_space.c            \
	$(OS_DIR)/ns_task.c             \
	$(OS_DIR)/ns_timer.c

# OS - NT objects-------------------------------------------------------
NS_OBJS= \
	$(SY_DIR)/ns_gen.$(OBJ)         \
   $(SY_DIR)/smnsexms.$(OBJ)       \
   $(SY_DIR)/smnsbdy1.$(OBJ)       \
	$(SY_DIR)/smnsptmi.$(OBJ)       \
	$(SY_DIR)/lns.$(OBJ)            \
	$(SY_DIR)/ns_ex_ms.$(OBJ)       \
	$(SY_DIR)/ns_id.$(OBJ)          \
	$(SY_DIR)/ns_ptmi.$(OBJ)        \
	$(SY_DIR)/ns_space.$(OBJ)       \
	$(SY_DIR)/ns_task.$(OBJ)        \
	$(SY_DIR)/ns_timer.$(OBJ)

# VxWorks sources---------------------------------------------------
VW_SRCS = $(OS_DIR)/vw_ss.c               \
	  $(OS_DIR)/vw_id.c

# VxWorks objects---------------------------------------------------
VW_OBJS = $(SY_DIR)/vw_ss.$(OBJ)          \
	  $(SY_DIR)/vw_id.$(OBJ)

# portable build sources------------------------------------------------
PT_SRCS= $(EG_DIR)/eg_tst.c              \
	 $(CM_DIR)/ss_ptsp.c

# portable build objects------------------------------------------------
PT_OBJS= $(SY_DIR)/eg_tst.$(OBJ)         \
	 $(SY_DIR)/ss_ptsp.$(OBJ)

# all acceptance test source--------------------------------------------
PRDACC_SRCS= \
    $(EG_SRCS) \
    $(EGAC_SRCS)   \
    $(EGSM_SRCS) \
    $(EGCM_SRCS) \
    $(SS_SRCS)   \
    $(OS_SRCS)   \
    $(SM_SRCS)   \
    $(HI_SRCS)   \
    $(EGT_SRCS)   \
    $(HISM_SRCS)

#TODO - This is for the DUMMY-TUCL obj
# New acceptance test objects
PRDACC_OBJS= \
    $(EG_OBJS)       \
    $(EGNEWAC_OBJS)  \
    $(EGSM_OBJS)     \
    $(EGCM_OBJS)     \
    $(SS_OBJS)       \
    $(SM_OBJS)       \

PRDACC_SRCS1= \
    $(EG_SRCS) \
    $(EGAC_SRCS)   \
    $(EGSM_SRCS) \
    $(EGCM_SRCS) \
    $(SS_SRCS)   \
    $(OS_SRCS)   \
    $(SM_SRCS)   \
    $(HI_SRCS)   \
    $(EGT_SRCS)   \
    $(HISM_SRCS)


#TODO - This is for the TUCL + DUMMY-TUCL obj
PRDACC_OBJS1= \
    $(EG_OBJS)       \
    $(EGNEWAC_OBJS)  \
	 $(SY_DIR)/egac_lhicb.$(OBJ) \
	 $(SY_DIR)/egac_lhiutl.$(OBJ) \
	 $(SY_DIR)/egac_lhihdl.$(OBJ) \
	 $(SY_DIR)/egac_lhireg.$(OBJ) \
    $(EGSM_OBJS)     \
    $(EGCM_OBJS)     \
	 $(SY_DIR)/lhi.$(OBJ) \
    $(SS_OBJS)       \
    $(SM_OBJS)       \
    $(HI_OBJS)       \
    $(HISM_OBJS)


# all acceptance test source--------------------------------------------
PRDACC_SRCS2= \
    $(EG_SRCS1) \
    $(EGAC_SRCS1)   \
    $(EGSM_SRCS1) \
    $(EU_SRCS1)  \
    $(EGCM_SRCS) \
    $(SS_SRCS)   \
    $(OS_SRCS)   \
    $(SM_SRCS)   \
    $(HI_SRCS)   \
    $(HISM_SRCS)

# New acceptance test objects
#TODO - This is for the STUB OBJS
PRDACC_OBJS2= \
    $(SY_DIR)/smeg_s3utl.$(OBJ) \
    $(SY_DIR)/smeg_s4utl.$(OBJ) \
    $(SY_DIR)/smeg_s10utl.$(OBJ) \
    $(SY_DIR)/smeg_s11utl.$(OBJ) \
    $(SY_DIR)/smeg_s16utl.$(OBJ) \
    $(SY_DIR)/smeg_ieutl.$(OBJ) \
    $(EGNEWAC_OBJS1)  \
    $(EG_OBJS1)       \
    $(EU_OBJS1)       \
    $(EGCM_OBJS)     \
    $(SS_OBJS)       \


# all acceptance test source--------------------------------------------
PRDACC_SRCS3= \
    $(EG_SRCS3) \
    $(EGAC_SRCS3)   \
    $(EGSM_SRCS3) \
    $(EU_SRCS3)  \
    $(EGCM_SRCS) \
    $(SS_SRCS)   \
    $(OS_SRCS)   \
    $(SM_SRCS)   \
    $(HI_SRCS)

# TODO - New acceptance test objects  for SLAVE
# Please uncomment the below commented lines in case of slave with real tucl
PRDACC_OBJS3= \
    $(TC_OBJS)  \
    $(EG_OBJS)       \
    $(EU_OBJS3)       \
    $(EGCM_OBJS)     \
    $(SS_OBJS)       \
#   $(EGSM_OBJS)     \
#	 $(SY_DIR)/lhi.$(OBJ) \
#  $(SM_OBJS)       \
#  $(HI_OBJS)


# all portable sources--------------------------------------------------
PRDPT_SRCS=  \
	$(PT_SRCS)                      \
	$(EG_SRCS)                      \
	$(EGCM_SRCS)

# all portable objects--------------------------------------------------
PRDPT_OBJS= \
	$(PT_OBJS)                      \
	$(EG_OBJS)

#	$(EGCM_OBJS)

# all library objects--------------------------------------------------
PRDLIB_OBJS= \
	$(LIB_OBJS)\
   $(EGCM_OBJS) \
   $(EGSM_OBJS)   \
 	$(EG_OBJS)

#	$(EGCM_OBJS)

# special programs required for Internal Development by makefile
# adjust as required
#
# System Binary Dir - replace with appropriate directory and programe
# for system

BIN=/bin
MAKEDEPEND=makedepend
REMOVE=$(BIN)/rm -f
CAT=cat
DELETE=del
OPENSSL=openssl
#
# build targets
#

# clean-----------------------------------------------------------------
#
# cleans up everything...
#
clean:
	$(REMOVE) $(SY_DIR)/*.$(OBJ) $(SY_DIR)/$(BINACC) $(SY_DIR)/$(BINTC) \
	$(SY_DIR)/$(BINPT) $(SY_DIR)/*.ln $(SY_DIR)/*.s *.$(OBJ) $(SY_DIR)/*.lib \
	$(BINACC) $(BINPT) *.ln *.s


# TLS certificates
tls_certs: root.cnf serverCA.cnf server.cnf client.cnf
	$(OPENSSL) req -passin pass:yabbadabbadoo -pasegut pass:yabbadabbadoo -newkey rsa:1024 -sha1 -keyout rootkey.pem \
	-out rootreq.pem -config root.cnf
	$(OPENSSL) x509 -passin pass:yabbadabbadoo -req -in rootreq.pem -sha1 -extfile root.cnf \
	-extensions certificate_extensions -signkey rootkey.pem \
	-out rootcert.pem
	$(CAT) rootcert.pem rootkey.pem > root.pem
	$(OPENSSL) req -passin pass:yabbadabbadoo -pasegut pass:yabbadabbadoo -newkey rsa:1024 -sha1 -keyout serverCAkey.pem \
	-out serverCAreq.pem -config serverCA.cnf
	$(OPENSSL) x509 -passin pass:yabbadabbadoo -req -in serverCAreq.pem -sha1 \
	-extfile serverCA.cnf -extensions certificate_extensions \
	-CA root.pem -CAkey root.pem -CAcreateserial -out serverCAcert.pem
	$(CAT) serverCAcert.pem serverCAkey.pem rootcert.pem > serverCA.pem
	$(OPENSSL) req -passin pass:yabbadabbadoo -pasegut pass:yabbadabbadoo -newkey rsa:1024 -sha1 -keyout serverkey.pem \
	-out serverreq.pem -config server.cnf -reqexts req_extensions
	$(OPENSSL) x509 -passin pass:yabbadabbadoo -req -in serverreq.pem -sha1 -extfile server.cnf \
	-extensions certificate_extensions -CA serverCA.pem \
	-CAkey serverCA.pem -CAcreateserial -out servercert.pem
	$(CAT) servercert.pem serverkey.pem serverCAcert.pem \
	rootcert.pem > server.pem
	$(OPENSSL) req -passin pass:yabbadabbadoo -pasegut pass:yabbadabbadoo -newkey rsa:1024 -sha1 -keyout clientkey.pem \
	-out clientreq.pem -config client.cnf -reqexts req_extensions
	$(OPENSSL) x509 -passin pass:yabbadabbadoo -req -in clientreq.pem -sha1 -extfile client.cnf \
	-extensions certificate_extensions -CA root.pem -CAkey root.pem \
	-CAcreateserial -out clientcert.pem
	$(CAT) clientcert.pem clientkey.pem rootcert.pem > client.pem

# new acceptance test include path without EGTP and without TLS
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC) F_OBJS="$(PRDACC_OBJS)"

# External target for new acceptance test build with TLS
acc1:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC) F_OBJS="$(PRDACC_OBJS1)"

# External target for new acceptance test build without TLS
acc2:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH2) \
	OUTNAME=$(BINACC) F_OBJS="$(PRDACC_OBJS2)"

# External target for new acceptance test build with ALL
acc3:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC) F_OBJS="$(PRDACC_OBJS3)"

# External target for new acceptance test build for slave
acc4:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH4) \
	OUTNAME=$(BINTC) F_OBJS="$(PRDTC_OBJS4)"


# External target for portable build
pt:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINPT)
	INC_PATH=$(PT_INC_PATH)


# External target for Library build

lib:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINLIB)
	INC_PATH=$(LIB_INC_PATH)

#=======================================================================
# SECTION 8 and 9
# Portable Target builds for Solaris CC, Solaris GNU, MRI mcc68k on
# Solaris, Linux GNU and Microegft Visual C++ for WINNT/WIN95
#
# Solaris CC portable build---------------------------------------------
#
$(BINSOLPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#
# Solaris GNU portable build--------------------------------------------
#
$(BINGNUPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#
# Linux GNU portable build--------------------------------------------
#
$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#
# Microegft Visual C++ for WINNT/WIN95 portable build-------------------
#
$(BINWINPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(WIN_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#
# MicroTec Research Compiler in Solaris Environment---------------------
$(BINMRIPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(MRIENV) CC=$(MRICC) \
	LD=$(MRILD) COPTS=$(MRICOPTS) LOPTS=$(MRILOPTS) LIBS=$(MRILIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#
# Linux g++ portable build--------------------------------------------
#
$(BINGPPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

# Linux c++ portable build--------------------------------------------
#
$(BINCPPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

#
# VxWorks portable build--------------------------------------------
#
$(BINPPCPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(PPCENV) CC=$(PPCCC) \
	LD=$(PPCLD) COPTS=$(PPCCOPTS) LOPTS=$(PPCLOPTS) LIBS=$(PPCLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)


#
# Acceptance test target builds for Solaris CC, Solaris GNU, Linux GNU,
# Insite Memory Check tool and Microegft Visual C++ for WINNT/WIN95
#
# Linux CC acceptance test build--------------------------------------
#
$(BINSOLACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINSOLACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS1) LIBS=$(SOLLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINSOLACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINSOLACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS1) LIBS=$(SOLLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINSOLACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
# Solaris GNU acceptance test build-------------------------------------
#
$(BINGNUACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GPPCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGNUACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GPPCOPTS) LOPTS=$(GNULOPTS1) LIBS=$(GNULIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGNUACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GPPCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGNUACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GPPCOPTS) LOPTS=$(GNULOPTS1) LIBS=$(GNULIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGNUACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GPPCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
# Linux GNU Purify acceptance test build--------------------------------
#
$(BINPURACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(PURENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINPURACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(PURENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS1) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINPURACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(PURENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINPURACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(PURENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS1) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINPURACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(PURENV) CC=$(PURCC) \
	LD=$(PURLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
# Linux GNU Quantify acceptance test build------------------------------
#
$(BINQUANACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(PURENV) CC=$(QUANTIFYCC) \
	LD=$(QUANTIFYLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS1) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINQUANACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(PURENV) CC=$(QUANTIFYCC) \
	LD=$(QUANTIFYLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINQUANACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(PURENV) CC=$(QUANTIFYCC) \
	LD=$(QUANTIFYLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
# Linux GNU Purify coverage acceptance test build-----------------------
#
$(BINCOVACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(PURENV) CC=$(PURCOVCC) \
	LD=$(PURCOVLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCOVACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(PURENV) CC=$(PURCOVCC) \
	LD=$(PURCOVLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS1) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCOVACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(PURENV) CC=$(PURCOVCC) \
	LD=$(PURCOVLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCOVACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(PURENV) CC=$(PURCOVCC) \
	LD=$(PURCOVLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS1) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCOVACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(PURENV) CC=$(PURCOVCC) \
	LD=$(PURCOVLD) COPTS=$(PURCOPTS) LOPTS=$(PURLOPTS) LIBS=$(PURLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)


#
#
# Linux GNU acceptance test build-------------------------------------
#
$(BINLNXACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINLNXACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS1) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINLNXACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINLNXACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS1) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINLNXACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS4) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
#
# Solaris g++ acceptance test build-------------------------------------
#
$(BINGPPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGPPACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(LNXLOPTS1) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGPPACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGPPACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(LNXLOPTS1) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINGPPACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLD) COPTS=$(GPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
#
# Solaris c++ acceptance test build-------------------------------------
#
$(BINCPPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCPPACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLOPTS1) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCPPACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCPPACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLOPTS1) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

$(BINCPPACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS)" OS_DIR=$(MT_DIR)

#
# Microegft Visual C++ for WINNT/WIN95 acceptance test build------------
#
$(BINWINACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(NEGEGPTS) OS_SRCS=$(NS_SRCS) \
	OBJ=$(WIN_OBJ_SUFFIX) OS_OBJS="$(NS_OBJS)" OS_DIR=$(NS_DIR)

$(BINWINACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(NEGEGPTS) OS_SRCS=$(NS_SRCS) \
	OBJ=$(WIN_OBJ_SUFFIX) OS_OBJS="$(NS_OBJS)" OS_DIR=$(NS_DIR)

$(BINWINACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(NEGEGPTS) OS_SRCS=$(NS_SRCS) \
	OBJ=$(WIN_OBJ_SUFFIX) OS_OBJS="$(NS_OBJS)" OS_DIR=$(NS_DIR)

$(BINWINACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	INC_PATH=$(ACC_INC_PATH3) OSOPTS=$(NEGEGPTS) OS_SRCS=$(NS_SRCS) \
	OBJ=$(WIN_OBJ_SUFFIX) OS_OBJS="$(NS_OBJS)" OS_DIR=$(NS_DIR)

$(BINWINACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(NEGEGPTS) OS_SRCS=$(NS_SRCS) \
	OBJ=$(WIN_OBJ_SUFFIX) OS_OBJS="$(NS_OBJS)" OS_DIR=$(NS_DIR)

#
# Vxworks cc386 acceptance test build on Pentium Target----------------
#
$(BINVWACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(VWINTENV) CC=$(VWINTCC) \
	LD=$(VWINTLD) COPTS=$(VWINTCOPTS) LOPTS=$(VWINTLOPTS) LIBS=$(VWINTLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(VWOSOPTS)" OS_SRCS=$(VW_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(VW_OBJS) OS_DIR=$(VW_DIR)

$(BINVWACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(VWINTENV) CC=$(VWINTCC) \
	LD=$(VWINTLD) COPTS=$(VWINTCOPTS) LOPTS=$(VWINTLOPTS) LIBS=$(VWINTLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(VWOSOPTS)" OS_SRCS=$(VW_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(VW_OBJS) OS_DIR=$(VW_DIR)

$(BINVWACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(VWINTENV) CC=$(VWINTCC) \
	LD=$(VWINTLD) COPTS=$(VWINTCOPTS) LOPTS=$(VWINTLOPTS) LIBS=$(VWINTLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(VWOSOPTS)" OS_SRCS=$(VW_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(VW_OBJS) OS_DIR=$(VW_DIR)

$(BINVWACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(VWINTENV) CC=$(VWINTCC) \
	LD=$(VWINTLD) COPTS=$(VWINTCOPTS) LOPTS=$(VWINTLOPTS) LIBS=$(VWINTLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(VWOSOPTS)" OS_SRCS="$(VW_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS)" OS_DIR=$(VW_DIR)

#
# MicroTec Research Compiler in Solaris Environment acceptance test build-
#
$(BINPPCACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(VWOSOPTS) OS_SRCS="$(VW_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS)" OS_DIR=$(VW_DIR)

$(BINPPCACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(VWOSOPTS) OS_SRCS="$(VW_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS)" OS_DIR=$(VW_DIR)

$(BINPPCACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS=$(VWOSOPTS) OS_SRCS="$(VW_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS)" OS_DIR=$(VW_DIR)

$(BINPPCACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX)

$(BINPPCACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	INC_PATH=$(ACC_INC_PATH4) OSOPTS=$(VWOSOPTS) OS_SRCS="$(VW_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS)" OS_DIR=$(VW_DIR)

#
# Inisite Memory Check Tool in Solaris acceptance test build-------------
#
$(BININSITEACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS=$(MT_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(MT_OBJS) OS_DIR=$(MT_DIR)

$(BININSITEACC1):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC1) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS1) LIBS=$(INSLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS=$(MT_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(MT_OBJS) OS_DIR=$(MT_DIR)

$(BININSITEACC2):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC2) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS=$(MT_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(MT_OBJS) OS_DIR=$(MT_DIR)

$(BININSITEACC3):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC3) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS1) LIBS=$(INSLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS=$(MT_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(MT_OBJS) OS_DIR=$(MT_DIR)

$(BININSITEACC4):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC4) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	INC_PATH=$(ACC_INC_PATH) OSOPTS="$(MTOSOPTS)" OS_SRCS=$(MT_SRCS) \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS=$(MT_OBJS) OS_DIR=$(MT_DIR)


# Library--------------------------------------------------------------
#
# cc Library build for Solaris----------------------------------------
$(BINSOLLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBLOPTS) LIBS=$(SOLLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH) OSOPTS=$(SOLLIBOSOPTS)

$(BINSOLLIB1):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB1) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBLOPTS) LIBS=$(SOLLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH1) OSOPTS=$(SOLLIBOSOPTS)

$(BINSOLLIB2):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB2) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBLOPTS) LIBS=$(SOLLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH2) OSOPTS=$(SOLLIBOSOPTS)

$(BINSOLLIB3):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB3) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBLOPTS) LIBS=$(SOLLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH3) OSOPTS=$(SOLLIBOSOPTS)

$(BINSOLLIB4):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB4) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBLOPTS) LIBS=$(SOLLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH4) OSOPTS=$(SOLLIBOSOPTS)

# gcc Library build for Solaris----------------------------------------
$(BINGNULIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBLOPTS) LIBS=$(GNULLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH) OSOPTS=$(SOLLIBOSOPTS)

$(BINGNULIB1):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB1) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBLOPTS) LIBS=$(GNULLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH1) OSOPTS=$(SOLLIBOSOPTS)

$(BINGNULIB2):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB2) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBLOPTS) LIBS=$(GNULLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH2) OSOPTS=$(SOLLIBOSOPTS)

$(BINGNULIB3):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB3) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBLOPTS) LIBS=$(GNULLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH3) OSOPTS=$(SOLLIBOSOPTS)

$(BINGNULIB4):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB4) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBLOPTS) LIBS=$(GNULLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH4) OSOPTS=$(SOLLIBOSOPTS)


# gcc Library build for Linux----------------------------------------
$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH) OSOPTS=$(LNXLIBOSOPTS)

$(BINLNXLIB1):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB1) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH1) OSOPTS=$(LNXLIBOSOPTS)

$(BINLNXLIB2):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB2) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH2) OSOPTS=$(LNXLIBOSOPTS)

$(BINLNXLIB3):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB3) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH3) OSOPTS=$(LNXLIBOSOPTS)

$(BINLNXLIB4):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB4) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH4) OSOPTS=$(LNXLIBOSOPTS)

# g++ Library build for Linux----------------------------------------
$(BINGPPLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH) OSOPTS=$(LNXLIBOSOPTS)

$(BINGPPLIB1):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB1) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH1) OSOPTS=$(LNXLIBOSOPTS)

$(BINGPPLIB2):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB2) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH2) OSOPTS=$(LNXLIBOSOPTS)

$(BINGPPLIB3):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB3) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH3) OSOPTS=$(LNXLIBOSOPTS)

$(BINGPPLIB4):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB4) -e ENV=$(LNXENV) CC=$(GPPCC) \
	LD=$(GPPLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBLOPTS) LIBS=$(LNXLLIBS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH4) OSOPTS=$(LNXLIBOSOPTS)

# c++ Library build for Linux----------------------------------------
$(BINCPPLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLIBLOPTS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH) OSOPTS=$(LNXLIBOSOPTS)

$(BINCPPLIB1):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB1) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLIBLOPTS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH1) OSOPTS=$(LNXLIBOSOPTS)

$(BINCPPLIB2):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB2) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLIBLOPTS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH2) OSOPTS=$(LNXLIBOSOPTS)

$(BINCPPLIB3):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB3) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLIBLOPTS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH3) OSOPTS=$(LNXLIBOSOPTS)

$(BINCPPLIB4):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB4) -e ENV=$(LNXENV) CC=$(CPPCC) \
	LD=$(CPPLIBLD) COPTS=$(CPPCOPTS) LOPTS=$(LNXLIBLOPTS) \
	OBJ=$(UNIX_OBJ_SUFFIX) INC_PATH=$(LIB_INC_PATH4) OSOPTS=$(LNXLIBOSOPTS)


# Library compilation--------------------------------------------------
$(MAKELIB):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN) CPRDOPTS=$(CPRDLIBOPTS) \
	CSMOPTS=$(CSMLIBOPTS)

$(MAKELIB1):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN1) CPRDOPTS=$(CPRDLIBOPTS1) \
	CSMOPTS=$(CSMLIBOPTS1)

$(MAKELIB2):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN2) CPRDOPTS=$(CPRDLIBOPTS2) \
	CSMOPTS=$(CSMLIBOPTS2)

$(MAKELIB3):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN3) CPRDOPTS=$(CPRDLIBOPTS3) \
	CSMOPTS=$(CSMLIBOPTS3)

$(MAKELIB4):
	@$(MAKE) -f $(MAKENAME) $(LIBBIN4) CPRDOPTS=$(CPRDLIBOPTS4) \
	CSMOPTS=$(CSMLIBOPTS4)

$(LIBBIN): $(PRDLIB_OBJS)
	$(LD) $(LOPTS) $(PRDLIB_OBJS) $(LIBS)

$(LIBBIN1): $(PRDLIB_OBJS1)
	$(LD) $(LOPTS) $(PRDLIB_OBJS1) $(LIBS)

$(LIBBIN2): $(PRDLIB_OBJS2)
	$(LD) $(LOPTS) $(PRDLIB_OBJS2) $(LIBS)

$(LIBBIN3): $(PRDLIB_OBJS3)
	$(LD) $(LOPTS) $(PRDLIB_OBJS3) $(LIBS)

$(LIBBIN4): $(PRDLIB_OBJS4)
	$(LD) $(LOPTS) $(PRDLIB_OBJS4) $(LIBS)
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#

$(MAKEPT):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS) \
	CSMOPTS=$(CSMPTOPTS)

$(MAKEPT1):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS1) \
	CSMOPTS=$(CSMPTOPTS1)

$(MAKEPT2):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS2) \
	CSMOPTS=$(CSMPTOPTS2)

$(MAKEPT3):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS3) \
	CSMOPTS=$(CSMPTOPTS3)

$(MAKEPT4):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS4) \
	CSMOPTS=$(CSMPTOPTS4)

$(PTBIN): $(PRDPT_OBJS)
	$(LD) $(LOPTS) $(PRDPT_OBJS) $(LIBS)


# acceptance test-------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEACC):
	@$(MAKE) -f $(MAKENAME) $(OUTNAME) CPRDOPTS=$(CPRDACCOPTS) \
	CSMOPTS=$(CSMACCOPTS)

$(MAKEACC1):
	@$(MAKE) -f $(MAKENAME) $(OUTNAME) CPRDOPTS=$(CPRDACCOPTS1) \
	CSMOPTS=$(CSMACCOPTS1)

$(MAKEACC2):
	@$(MAKE) -f $(MAKENAME) $(OUTNAME) CPRDOPTS=$(CPRDACCOPTS2) \
	CSMOPTS=$(CSMACCOPTS2)

$(MAKEACC3):
	@$(MAKE) -f $(MAKENAME) $(OUTNAME) CPRDOPTS=$(CPRDACCOPTS3) \
	CSMOPTS=$(CSMACCOPTS3)

$(MAKEACC4):
	@$(MAKE) -f $(MAKENAME) $(OUTNAME) CPRDOPTS=$(CPRDACCOPTS4) \
	CSMOPTS=$(CSMACCOPTS4)

$(ACCBIN): $(F_OBJS) $(OS_OBJS)
	$(LD) $(LOPTS) $(F_OBJS) $(OS_OBJS) $(LIBS)

$(TCBIN): $(EGTC_OBJS)
	$(LD) $(LOPTS) $(EGTC_OBJS) $(LIBS)

#
# Make Depend
# depend portable test--------------------------------------------------
# dependency for portable source
depend_pt: $(PRDPT_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e \
	DEPOPTS=$(DEPOPTS_PT) INC_PATH=$(PT_INC_PATH) \
	OSOPTS=$(PTOSOPTS) DEP_SRCS="$(PRDPT_SRCS)"

# depend acceptance test------------------------------------------------
# dependency for acceptance test
depend_acc: $(PRDACC_SRCS)
	$(MAKE) -f $(MAKENAME) $(DEPEND) -e \
	DEPOPTS=$(DEPOPTS_ACC) INC_PATH=$(ACC_INC_PATH) \
	OSOPTS=$(MTOSOPTS) OS_SRCS=$(SSMT_SRCS) OS_OBJS=$(SSMT_OBJS) \
	OS_DIR=$(MT_DIR) DEP_SRCS="$(PRDACC_SRCS)"

# Build the dependency
$(DEPEND):
	$(MAKEDEPEND) -f $(MAKENAME) $(COSOPTS) $(DEPOPTS) $(INC_PATH) $(DEP_SRCS)

#=======================================================================
#
# Source code build directives
# SECTION 10
# Modify this section to list out the build directives for the specific
# product

# Portable build ------------------------------------------------
$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_DIR)/ss_ptsp.c
	$(CC) -c $(CPTOPTS) $(CM_DIR)/ss_ptsp.c -o $(SY_DIR)/ss_ptsp.$(OBJ)

$(SY_DIR)/eg_tst.$(OBJ): $(EG_DIR)/eg_tst.c
	$(CC) -c $(CPTOPTS) $(EG_DIR)/eg_tst.c -o $(SY_DIR)/eg_tst.$(OBJ)


# acceptance test-------------------------------------------------------
$(SY_DIR)/egac_hi_ex_ms.$(OBJ): $(EGACC_DIR)/egac_hi_ex_ms.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_hi_ex_ms.c -o $(SY_DIR)/egac_hi_ex_ms.$(OBJ)

$(SY_DIR)/egac_egld.$(OBJ): $(EGACC_DIR)/egac_egld.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_egld.c -o $(SY_DIR)/egac_egld.$(OBJ)

$(SY_DIR)/cm_xta_ex_ms.$(OBJ): $(EGACC_DIR)/cm_xta_ex_ms.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_ex_ms.c -o $(SY_DIR)/cm_xta_ex_ms.$(OBJ)

$(SY_DIR)/egac_lhireg.$(OBJ): $(EGACC_DIR)/egac_lhireg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_lhireg.c -o $(SY_DIR)/egac_lhireg.$(OBJ)

$(SY_DIR)/cm_xta_xml.$(OBJ): $(EGACC_DIR)/cm_xta_xml.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_xml.c -o $(SY_DIR)/cm_xta_xml.$(OBJ)

$(SY_DIR)/cm_xta_ldfsm.$(OBJ): $(EGACC_DIR)/cm_xta_ldfsm.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_ldfsm.c -o $(SY_DIR)/cm_xta_ldfsm.$(OBJ)

$(SY_DIR)/egac_gtpld.$(OBJ): $(EGACC_DIR)/egac_gtpld.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_gtpld.c -o $(SY_DIR)/egac_gtpld.$(OBJ)

$(SY_DIR)/cm_xta_tcfsm.$(OBJ): $(EGACC_DIR)/cm_xta_tcfsm.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_tcfsm.c -o $(SY_DIR)/cm_xta_tcfsm.$(OBJ)

$(SY_DIR)/cm_xta_tgfsm.$(OBJ): $(EGACC_DIR)/cm_xta_tgfsm.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_tgfsm.c -o $(SY_DIR)/cm_xta_tgfsm.$(OBJ)

$(SY_DIR)/cm_xta_tafsm.$(OBJ): $(EGACC_DIR)/cm_xta_tafsm.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_tafsm.c -o $(SY_DIR)/cm_xta_tafsm.$(OBJ)

$(SY_DIR)/cm_xta_py.$(OBJ): $(EGACC_DIR)/cm_xta_py.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_py.c -o $(SY_DIR)/cm_xta_py.$(OBJ)

$(SY_DIR)/egac_py.$(OBJ): $(EGACC_DIR)/egac_py.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_py.c -o $(SY_DIR)/egac_py.$(OBJ)

$(SY_DIR)/cm_xta_inthdl.$(OBJ): $(EGACC_DIR)/cm_xta_inthdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_inthdl.c -o $(SY_DIR)/cm_xta_inthdl.$(OBJ)

$(SY_DIR)/cm_xta_intreg.$(OBJ): $(EGACC_DIR)/cm_xta_intreg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_intreg.c -o $(SY_DIR)/cm_xta_intreg.$(OBJ)

$(SY_DIR)/egac_legreg.$(OBJ): $(EGACC_DIR)/egac_legreg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_legreg.c -o $(SY_DIR)/egac_legreg.$(OBJ)

$(SY_DIR)/egac_hitreg.$(OBJ): $(EGACC_DIR)/egac_hitreg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_hitreg.c -o $(SY_DIR)/egac_hitreg.$(OBJ)

$(SY_DIR)/egac_egtreg.$(OBJ): $(EGACC_DIR)/egac_egtreg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_egtreg.c -o $(SY_DIR)/egac_egtreg.$(OBJ)

$(SY_DIR)/egac_cstreg.$(OBJ): $(EGACC_DIR)/egac_cstreg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_cstreg.c -o $(SY_DIR)/egac_cstreg.$(OBJ)

$(SY_DIR)/egac_prof.$(OBJ): $(EGACC_DIR)/egac_prof.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_prof.c -o $(SY_DIR)/egac_prof.$(OBJ)

$(SY_DIR)/egac_lhicb.$(OBJ): $(EGACC_DIR)/egac_lhicb.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_lhicb.c -o $(SY_DIR)/egac_lhicb.$(OBJ)

$(SY_DIR)/egac_legcb.$(OBJ): $(EGACC_DIR)/egac_legcb.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_legcb.c -o $(SY_DIR)/egac_legcb.$(OBJ)

$(SY_DIR)/egac_hitcb.$(OBJ): $(EGACC_DIR)/egac_hitcb.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_hitcb.c -o $(SY_DIR)/egac_hitcb.$(OBJ)

$(SY_DIR)/egac_egtcb.$(OBJ): $(EGACC_DIR)/egac_egtcb.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_egtcb.c -o $(SY_DIR)/egac_egtcb.$(OBJ)

$(SY_DIR)/egac_cstcb.$(OBJ): $(EGACC_DIR)/egac_cstcb.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_cstcb.c -o $(SY_DIR)/egac_cstcb.$(OBJ)

$(SY_DIR)/cm_xta_res.$(OBJ): $(EGACC_DIR)/cm_xta_res.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_res.c -o $(SY_DIR)/cm_xta_res.$(OBJ)

$(SY_DIR)/cm_xta_queue.$(OBJ): $(EGACC_DIR)/cm_xta_queue.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_queue.c -o $(SY_DIR)/cm_xta_queue.$(OBJ)

$(SY_DIR)/egac_lhiutl.$(OBJ): $(EGACC_DIR)/egac_lhiutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_lhiutl.c -o $(SY_DIR)/egac_lhiutl.$(OBJ)

$(SY_DIR)/egac_legutl.$(OBJ): $(EGACC_DIR)/egac_legutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_legutl.c -o $(SY_DIR)/egac_legutl.$(OBJ)

$(SY_DIR)/cm_xta_flag.$(OBJ): $(EGACC_DIR)/cm_xta_flag.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_flag.c -o $(SY_DIR)/cm_xta_flag.$(OBJ)

$(SY_DIR)/egac_flag.$(OBJ): $(EGACC_DIR)/egac_flag.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_flag.c -o $(SY_DIR)/egac_flag.$(OBJ)

$(SY_DIR)/cm_xta_log.$(OBJ): $(EGACC_DIR)/cm_xta_log.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_log.c -o $(SY_DIR)/cm_xta_log.$(OBJ)

$(SY_DIR)/cm_xta_slave.$(OBJ): $(EGACC_DIR)/cm_xta_slave.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_slave.c -o $(SY_DIR)/cm_xta_slave.$(OBJ)

$(SY_DIR)/cm_xta_master.$(OBJ): $(EGACC_DIR)/cm_xta_master.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_master.c -o $(SY_DIR)/cm_xta_master.$(OBJ)

$(SY_DIR)/cm_xta_reg.$(OBJ): $(EGACC_DIR)/cm_xta_reg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_reg.c -o $(SY_DIR)/cm_xta_reg.$(OBJ)

$(SY_DIR)/cm_xta_tst.$(OBJ): $(EGACC_DIR)/cm_xta_tst.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_tst.c -o $(SY_DIR)/cm_xta_tst.$(OBJ)

$(SY_DIR)/cm_xta_ipc.$(OBJ): $(EGACC_DIR)/cm_xta_ipc.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_ipc.c -o $(SY_DIR)/cm_xta_ipc.$(OBJ)

$(SY_DIR)/egac_tst.$(OBJ): $(EGACC_DIR)/egac_tst.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_tst.c -o $(SY_DIR)/egac_tst.$(OBJ)

$(SY_DIR)/egac_slave.$(OBJ): $(EGACC_DIR)/egac_slave.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_slave.c -o $(SY_DIR)/egac_slave.$(OBJ)

$(SY_DIR)/egac_edmutl.$(OBJ): $(EGACC_DIR)/egac_edmutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_edmutl.c -o $(SY_DIR)/egac_edmutl.$(OBJ)

$(SY_DIR)/egac_edmtst.$(OBJ): $(EGACC_DIR)/egac_edmtst.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_edmtst.c -o $(SY_DIR)/egac_edmtst.$(OBJ)

$(SY_DIR)/egac_edmreg.$(OBJ): $(EGACC_DIR)/egac_edmreg.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_edmreg.c -o $(SY_DIR)/egac_edmreg.$(OBJ)

$(SY_DIR)/egac_edmhdl.$(OBJ): $(EGACC_DIR)/egac_edmhdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_edmhdl.c -o $(SY_DIR)/egac_edmhdl.$(OBJ)

$(SY_DIR)/egac_edmcb.$(OBJ): $(EGACC_DIR)/egac_edmcb.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_edmcb.c -o $(SY_DIR)/egac_edmcb.$(OBJ)

$(SY_DIR)/egac_egtutl.$(OBJ): $(EGACC_DIR)/egac_egtutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_egtutl.c -o $(SY_DIR)/egac_egtutl.$(OBJ)

$(SY_DIR)/egac_cmutl.$(OBJ): $(EGACC_DIR)/egac_cmutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_cmutl.c -o $(SY_DIR)/egac_cmutl.$(OBJ)

$(SY_DIR)/cm_xta_txn.$(OBJ): $(EGACC_DIR)/cm_xta_txn.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_txn.c -o $(SY_DIR)/cm_xta_txn.$(OBJ)

$(SY_DIR)/egac_leghdl.$(OBJ): $(EGACC_DIR)/egac_leghdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_leghdl.c -o $(SY_DIR)/egac_leghdl.$(OBJ)

$(SY_DIR)/egac_hithdl.$(OBJ): $(EGACC_DIR)/egac_hithdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_hithdl.c -o $(SY_DIR)/egac_hithdl.$(OBJ)

$(SY_DIR)/egac_hitutl.$(OBJ): $(EGACC_DIR)/egac_hitutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_hitutl.c -o $(SY_DIR)/egac_hitutl.$(OBJ)

$(SY_DIR)/egac_gtphdl.$(OBJ): $(EGACC_DIR)/egac_gtphdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_gtphdl.c -o $(SY_DIR)/egac_gtphdl.$(OBJ)

$(SY_DIR)/egac_egthdl.$(OBJ): $(EGACC_DIR)/egac_egthdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_egthdl.c -o $(SY_DIR)/egac_egthdl.$(OBJ)

$(SY_DIR)/egac_lhihdl.$(OBJ): $(EGACC_DIR)/egac_lhihdl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_lhihdl.c -o $(SY_DIR)/egac_lhihdl.$(OBJ)

$(SY_DIR)/egac_egtutl2.$(OBJ): $(EGACC_DIR)/egac_egtutl2.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_egtutl2.c -o $(SY_DIR)/egac_egtutl2.$(OBJ)

$(SY_DIR)/cm_xta_intutl.$(OBJ): $(EGACC_DIR)/cm_xta_intutl.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_intutl.c -o $(SY_DIR)/cm_xta_intutl.$(OBJ)

$(SY_DIR)/cm_xta_tmr.$(OBJ): $(EGACC_DIR)/cm_xta_tmr.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_tmr.c -o $(SY_DIR)/cm_xta_tmr.$(OBJ)

$(SY_DIR)/cm_xta_dum.$(OBJ): $(EGACC_DIR)/cm_xta_dum.c $(SYS_INC) $(EGACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/cm_xta_dum.c -o $(SY_DIR)/cm_xta_dum.$(OBJ)

# sample upper layer build----------------------------------------------

$(SY_DIR)/eu_bdy1.$(OBJ): $(EGACC_DIR)/eu_bdy1.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/eu_bdy1.c -o $(SY_DIR)/eu_bdy1.$(OBJ)

$(SY_DIR)/eu_ex_ms.$(OBJ): $(EGACC_DIR)/eu_ex_ms.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/eu_ex_ms.c -o $(SY_DIR)/eu_ex_ms.$(OBJ)

$(SY_DIR)/eu_ptli.$(OBJ): $(EGACC_DIR)/eu_ptli.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/eu_ptli.c -o $(SY_DIR)/eu_ptli.$(OBJ)

$(SY_DIR)/egac_ptedm.$(OBJ): $(EGACC_DIR)/egac_ptedm.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/egac_ptedm.c -o $(SY_DIR)/egac_ptedm.$(OBJ)

# sample lower layer build----------------------------------------------

$(SY_DIR)/dhi_bdy1.$(OBJ): $(EGACC_DIR)/dhi_bdy1.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/dhi_bdy1.c -o $(SY_DIR)/dhi_bdy1.$(OBJ)

$(SY_DIR)/dhi_ex_ms.$(OBJ): $(EGACC_DIR)/dhi_ex_ms.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/dhi_ex_ms.c -o $(SY_DIR)/dhi_ex_ms.$(OBJ)

$(SY_DIR)/dhi_ptui.$(OBJ): $(EGACC_DIR)/dhi_ptui.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EGACC_DIR)/dhi_ptui.c -o $(SY_DIR)/dhi_ptui.$(OBJ)

# product Id  file---------------------------------------------------

$(SY_DIR)/eg_id.$(OBJ): $(EG_DIR)/eg_id.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_id.c -o $(SY_DIR)/eg_id.$(OBJ)

# product build---------------------------------------------------

$(SY_DIR)/eg_edm.$(OBJ): $(EG_DIR)/eg_edm.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_edm.c -o $(SY_DIR)/eg_edm.$(OBJ)

$(SY_DIR)/eg_pcm.$(OBJ): $(EG_DIR)/eg_pcm.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_pcm.c -o $(SY_DIR)/eg_pcm.$(OBJ)

$(SY_DIR)/eg_ptmi.$(OBJ): $(EG_DIR)/eg_ptmi.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_ptmi.c -o $(SY_DIR)/eg_ptmi.$(OBJ)

$(SY_DIR)/eg_ptui.$(OBJ): $(EG_DIR)/eg_ptui.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_ptui.c -o $(SY_DIR)/eg_ptui.$(OBJ)

$(SY_DIR)/eg_tpt.$(OBJ): $(EG_DIR)/eg_tpt.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_tpt.c -o $(SY_DIR)/eg_tpt.$(OBJ)

$(SY_DIR)/eg_li.$(OBJ): $(EG_DIR)/eg_li.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_li.c -o $(SY_DIR)/eg_li.$(OBJ)

$(SY_DIR)/eg_ptli.$(OBJ): $(EG_DIR)/eg_ptli.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_ptli.c -o $(SY_DIR)/eg_ptli.$(OBJ)

$(SY_DIR)/eg_ex_ms.$(OBJ): $(EG_DIR)/eg_ex_ms.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_ex_ms.c -o $(SY_DIR)/eg_ex_ms.$(OBJ)

$(SY_DIR)/eg_cpm.$(OBJ): $(EG_DIR)/eg_cpm.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_cpm.c -o $(SY_DIR)/eg_cpm.$(OBJ)

$(SY_DIR)/eg_tmr.$(OBJ): $(EG_DIR)/eg_tmr.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_tmr.c -o $(SY_DIR)/eg_tmr.$(OBJ)

$(SY_DIR)/eg_mi.$(OBJ): $(EG_DIR)/eg_mi.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_mi.c -o $(SY_DIR)/eg_mi.$(OBJ)

$(SY_DIR)/eg_tunn.$(OBJ): $(EG_DIR)/eg_tunn.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_tunn.c -o $(SY_DIR)/eg_tunn.$(OBJ)

$(SY_DIR)/eg_util.$(OBJ): $(EG_DIR)/eg_util.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_util.c -o $(SY_DIR)/eg_util.$(OBJ)

$(SY_DIR)/eg_ui.$(OBJ): $(EG_DIR)/eg_ui.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_ui.c -o $(SY_DIR)/eg_ui.$(OBJ)

$(SY_DIR)/egt_util.$(OBJ): $(EG_DIR)/egt_util.c $(SYS_INC) $(CM_INC) $(EGALL_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/egt_util.c -o $(SY_DIR)/egt_util.$(OBJ)

# TUCL layer build-------------------------------------------------


# common build---------------------------------------------------------

$(SY_DIR)/cm_gen.$(OBJ): $(CM_DIR)/cm_gen.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_gen.c -o $(SY_DIR)/cm_gen.$(OBJ)

$(SY_DIR)/cm_hash.$(OBJ): $(CM_DIR)/cm_hash.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_hash.c -o $(SY_DIR)/cm_hash.$(OBJ)

$(SY_DIR)/cm_llist.$(OBJ): $(CM_DIR)/cm_llist.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_llist.c -o $(SY_DIR)/cm_llist.$(OBJ)

$(SY_DIR)/cm_lib.$(OBJ): $(CM_DIR)/cm_lib.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_lib.c -o $(SY_DIR)/cm_lib.$(OBJ)

$(SY_DIR)/cm_bdy5.$(OBJ): $(CM_DIR)/cm_bdy5.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_bdy5.c -o $(SY_DIR)/cm_bdy5.$(OBJ)

$(SY_DIR)/cm_inet.$(OBJ): $(CM_DIR)/cm_inet.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_inet.c -o $(SY_DIR)/cm_inet.$(OBJ)

$(SY_DIR)/cm_tpt.$(OBJ): $(CM_DIR)/cm_tpt.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_tpt.c -o $(SY_DIR)/cm_tpt.$(OBJ)

$(SY_DIR)/cm_tkns.$(OBJ): $(CM_DIR)/cm_tkns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_tkns.c -o $(SY_DIR)/cm_tkns.$(OBJ)

$(SY_DIR)/cm_mblk.$(OBJ): $(CM_DIR)/cm_mblk.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_mblk.c -o $(SY_DIR)/cm_mblk.$(OBJ)

$(SY_DIR)/cm_dns.$(OBJ): $(CM_DIR)/cm_dns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_dns.c -o $(SY_DIR)/cm_dns.$(OBJ)

$(SY_DIR)/cm_xtree.$(OBJ): $(CM_DIR)/cm_xtree.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_xtree.c -o $(SY_DIR)/cm_xtree.$(OBJ)

$(SY_DIR)/cm_sdp.$(OBJ): $(CM_DIR)/cm_sdp.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_sdp.c -o $(SY_DIR)/cm_sdp.$(OBJ)

$(SY_DIR)/cm_abnf.$(OBJ): $(CM_DIR)/cm_abnf.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_abnf.c -o $(SY_DIR)/cm_abnf.$(OBJ)

$(SY_DIR)/cmabnfrx.$(OBJ): $(CM_DIR)/cmabnfrx.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cmabnfrx.c -o $(SY_DIR)/cmabnfrx.$(OBJ)

$(SY_DIR)/cst.$(OBJ): $(CM_DIR)/cst.c $(SYS_INC) $(CST_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cst.c -o $(SY_DIR)/cst.$(OBJ)

$(SY_DIR)/egt.$(OBJ): $(CM_DIR)/egt.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/egt.c -o $(SY_DIR)/egt.$(OBJ)

$(SY_DIR)/leg.$(OBJ): $(EG_DIR)/leg.c $(SYS_INC) $(EGT_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/leg.c -o $(SY_DIR)/leg.$(OBJ)


# stack manager build-------------------------------------------

$(SY_DIR)/eg_dbutl.$(OBJ): $(EG_DIR)/eg_dbutl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbutl.c -o $(SY_DIR)/eg_dbutl.$(OBJ)

$(SY_DIR)/smeg_ieutl.$(OBJ): $(EGSM_DIR)/smeg_ieutl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_ieutl.c -o $(SY_DIR)/smeg_ieutl.$(OBJ)

$(SY_DIR)/smeg_s3utl.$(OBJ): $(EGSM_DIR)/smeg_s3utl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_s3utl.c -o $(SY_DIR)/smeg_s3utl.$(OBJ)

$(SY_DIR)/smeg_s4utl.$(OBJ): $(EGSM_DIR)/smeg_s4utl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_s4utl.c -o $(SY_DIR)/smeg_s4utl.$(OBJ)

$(SY_DIR)/smeg_s5utl.$(OBJ): $(EGSM_DIR)/smeg_s5utl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_s5utl.c -o $(SY_DIR)/smeg_s5utl.$(OBJ)

$(SY_DIR)/smeg_s10utl.$(OBJ): $(EGSM_DIR)/smeg_s10utl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_s10utl.c -o $(SY_DIR)/smeg_s10utl.$(OBJ)

$(SY_DIR)/smeg_s11utl.$(OBJ): $(EGSM_DIR)/smeg_s11utl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_s11utl.c -o $(SY_DIR)/smeg_s11utl.$(OBJ)

$(SY_DIR)/smeg_s16utl.$(OBJ): $(EGSM_DIR)/smeg_s16utl.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smeg_s16utl.c -o $(SY_DIR)/smeg_s16utl.$(OBJ)

$(SY_DIR)/eg_dbs3.$(OBJ): $(EG_DIR)/eg_dbs3.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbs3.c -o $(SY_DIR)/eg_dbs3.$(OBJ)

$(SY_DIR)/eg_dbs4.$(OBJ): $(EG_DIR)/eg_dbs4.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbs4.c -o $(SY_DIR)/eg_dbs4.$(OBJ)

$(SY_DIR)/eg_dbs5.$(OBJ): $(EG_DIR)/eg_dbs5.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbs5.c -o $(SY_DIR)/eg_dbs5.$(OBJ)

$(SY_DIR)/eg_dbs10.$(OBJ): $(EG_DIR)/eg_dbs10.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbs10.c -o $(SY_DIR)/eg_dbs10.$(OBJ)

$(SY_DIR)/eg_dbs11.$(OBJ): $(EG_DIR)/eg_dbs11.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbs11.c -o $(SY_DIR)/eg_dbs11.$(OBJ)

$(SY_DIR)/eg_dbsm.$(OBJ): $(EG_DIR)/eg_dbsm.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbsm.c -o $(SY_DIR)/eg_dbsm.$(OBJ)

$(SY_DIR)/eg_dbsn.$(OBJ): $(EG_DIR)/eg_dbsn.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbsn.c -o $(SY_DIR)/eg_dbsn.$(OBJ)

$(SY_DIR)/eg_dbsi.$(OBJ): $(EG_DIR)/eg_dbsi.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbsi.c -o $(SY_DIR)/eg_dbsi.$(OBJ)

$(SY_DIR)/eg_dbs16.$(OBJ): $(EG_DIR)/eg_dbs16.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EG_DIR)/eg_dbs16.c -o $(SY_DIR)/eg_dbs16.$(OBJ)

$(SY_DIR)/smegs5.$(OBJ): $(EGSM_DIR)/smegs5.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smegs5.c -o $(SY_DIR)/smegs5.$(OBJ)

$(SY_DIR)/smegbdy1.$(OBJ): $(EGSM_DIR)/smegbdy1.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smegbdy1.c -o $(SY_DIR)/smegbdy1.$(OBJ)

$(SY_DIR)/smegexms.$(OBJ): $(EGSM_DIR)/smegexms.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smegexms.c -o $(SY_DIR)/smegexms.$(OBJ)

$(SY_DIR)/smegptmi.$(OBJ): $(EGSM_DIR)/smegptmi.c $(SYS_INC) $(LEG_INC)
	$(CC) -c $(CPRDOPTS) $(EGSM_DIR)/smegptmi.c -o $(SY_DIR)/smegptmi.$(OBJ)

$(SY_DIR)/smhibdy1.$(OBJ): $(HISM_DIR)/smhibdy1.c $(SYS_INC) $(LHI_INC)
	$(CC) -c $(CPRDOPTS) $(HISM_DIR)/smhibdy1.c -o $(SY_DIR)/smhibdy1.$(OBJ)

$(SY_DIR)/smhiexms.$(OBJ): $(HISM_DIR)/smhiexms.c $(SYS_INC) $(LHI_INC)
	$(CC) -c $(CPRDOPTS) $(HISM_DIR)/smhiexms.c -o $(SY_DIR)/smhiexms.$(OBJ)

$(SY_DIR)/smhiptmi.$(OBJ): $(HISM_DIR)/smhiptmi.c $(SYS_INC) $(LHI_INC)
	$(CC) -c $(CPRDOPTS) $(HISM_DIR)/smhiptmi.c -o $(SY_DIR)/smhiptmi.$(OBJ)

# System Services-------------------------------------------------------

$(SY_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_gen.c -o $(SY_DIR)/ss_gen.$(OBJ)

$(SY_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_mem.c -o $(SY_DIR)/ss_mem.$(OBJ)

$(SY_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_queue.c -o $(SY_DIR)/ss_queue.$(OBJ)

$(SY_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_timer.c -o $(SY_DIR)/ss_timer.$(OBJ)

$(SY_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_msg.c -o $(SY_DIR)/ss_msg.$(OBJ)

$(SY_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_rtr.c -o $(SY_DIR)/ss_rtr.$(OBJ)

$(SY_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_drvr.c -o $(SY_DIR)/ss_drvr.$(OBJ)

$(SY_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_pack.c -o $(SY_DIR)/ss_pack.$(OBJ)

$(SY_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_strm.c -o $(SY_DIR)/ss_strm.$(OBJ)

$(SY_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_task.c -o $(SY_DIR)/ss_task.$(OBJ)

$(SY_DIR)/ss_id.$(OBJ): $(SS_DIR)/ss_id.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(SS_DIR)/ss_id.c -o $(SY_DIR)/ss_id.$(OBJ)

$(SY_DIR)/cm_mem.$(OBJ): $(CM_DIR)/cm_mem.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(CM_DIR)/cm_mem.c -o $(SY_DIR)/cm_mem.$(OBJ)

# OS - MTSS build-------------------------------------------------------
$(SY_DIR)/mt_ss.$(OBJ): $(MT_DIR)/mt_ss.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(MT_DIR)/mt_ss.c -o $(SY_DIR)/mt_ss.$(OBJ)

$(SY_DIR)/mt_id.$(OBJ): $(MT_DIR)/mt_id.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(MT_DIR)/mt_id.c -o $(SY_DIR)/mt_id.$(OBJ)

$(SY_DIR)/mt_tst.$(OBJ): $(MT_DIR)/mt_sts.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(PURE) $(CC) -c $(COSOPTS) $(MT_DIR)/mt_sts.c -o $(SY_DIR)/mt_sts.$(OBJ)

# MTSS-Windows NT-------------------------------------------------------
$(SY_DIR)/ns_gen.$(OBJ): $(OS_DIR)/ns_gen.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_gen.c

$(SY_DIR)/smnsexms.$(OBJ): $(OS_DIR)/smnsexms.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(CSMOPTS) $(COSOPTS) $(OS_DIR)/smnsexms.c

$(SY_DIR)/smnsbdy1.$(OBJ): $(OS_DIR)/smnsbdy1.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(CSMOPTS) $(COSOPTS) $(OS_DIR)/smnsbdy1.c

$(SY_DIR)/lns.$(OBJ): $(OS_DIR)/lns.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/lns.c

$(SY_DIR)/ns_ex_ms.$(OBJ): $(OS_DIR)/ns_ex_ms.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_ex_ms.c

$(SY_DIR)/ns_id.$(OBJ): $(OS_DIR)/ns_id.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_id.c

$(SY_DIR)/ns_ptmi.$(OBJ): $(OS_DIR)/ns_ptmi.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_ptmi.c

$(SY_DIR)/ns_space.$(OBJ): $(OS_DIR)/ns_space.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_space.c

$(SY_DIR)/ns_task.$(OBJ): $(OS_DIR)/ns_task.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_task.c

$(SY_DIR)/ns_timer.$(OBJ): $(OS_DIR)/ns_timer.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_timer.c

$(SY_DIR)/np_gen.$(OBJ): $(OS_DIR)/np_gen.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/np_gen.c

$(SY_DIR)/np_task.$(OBJ): $(OS_DIR)/np_task.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/np_task.c

$(SY_DIR)/np_timer.$(OBJ): $(OS_DIR)/np_timer.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/np_timer.c

$(SY_DIR)/smnsptmi.$(OBJ): $(OS_DIR)/smnsptmi.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/smnsptmi.c

# VxWorks----------------------------------------------------------
$(SY_DIR)/vw_ss.$(OBJ): $(OS_DIR)/vw_ss.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_ss.c

$(SY_DIR)/vw_id.$(OBJ): $(OS_DIR)/vw_id.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_id.c

$(SY_DIR)/vw_acc.$(OBJ): $(OS_DIR)/vw_acc.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_acc.c



#********************************************************************30**
#
#        End of file:     vb_eg.mak@@/main/1 - Mon Jun 28 19:35:29 2010
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
#   ver       pat    init                  description
#----------- -------- ---- -----------------------------------------------
#/main/1      ---      ad                1. Created for Initial Release 1.1
#/main/4      ---      kchitanya         1. Initial for egtp release 1.2
#/main/1      ---      ad                1. Created for Lte-CNE Phase 1
#********************************************************************91*/

# DO NOT DELETE THIS LINE -- make depend depends on it.
