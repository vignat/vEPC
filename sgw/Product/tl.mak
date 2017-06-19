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
#
#********************************************************************20**
#
#       Name:   TCP UDP Convergence Layer - makefile
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file.
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Solaris 2.x with SUSpro C(cc).
#
#               ENVIRONMENTS:
#
#               Solaris 2.x with SUNpro C (cc)
#
#               Solaris 2.x with GNU C (gcc)
#
#               Solaris 2.x with MRI C (mcc68k) cross compiled for
#               a relatively standard 68040 processor board
#
#               Microsoft Visual C++ 4.x for Windows '95 and Windows NT
#               environments
#
#               BUILD TARGETS:
#
#               pt               portable software (without MOS)
#               acc              acceptance tests (with MOS) -- default
#
#               pure             acceptance tests with purify enabled
#               quant            acceptance tests with quantify enabled
#
#               depend.acc       makedepend for acceptance tests
#               depend.pt        makedepend for portable software
#
#               clean            remove all object files
#               win_clean        remove all object files(Windows environment)
#
#       Sid:      tl.mak@@/main/3 - Mon Mar  3 20:09:19 2008
#
#       Prg:    asa
#
#********************************************************************21*/
#
# NOTE: for compilation on different platforms, the BLDENV specification
#       is not sufficient. The OS_DIR needs to be set to the correct
#       directory on line 158 and the OBJ needs to be set to the correct
#       object file extension on line 557

#=======================================================================
# SECTION 1
# Defines to specify appropriate directories
# Set the defines below to respective product directories

# common directory
CM_DIR=.

# HI directory
HI_DIR=.

# HI user directory
HIHC_DIR=.

# stack manager directory
SM_DIR=.

# stack manager directory
HISM_DIR=.

# Common SSI directory
SS_DIR=/u/prd/rtr/mt

# MTSS directory
MT_DIR=/u/prd/rtr/mt

# NT SSI directory
NS_DIR=/u/prd/rtr/ns

# VxWorks SSI directory
VW_DIR=/u/prd/rtr/vw

# for Solaris MTSS
OS_DIR=$(MT_DIR)

# for Windows NT MTSS
#OS_DIR=$(NS_DIR)

# for VxWorks SSI
#OS_DIR=$(VW_DIR)

# acceptance directory
HIACC_DIR=.

#Provide correct SSL library include path for TLS.
SSL_DIR=.
#SSL_DIR=/usr/local/include

# Acceptance build directory
SY_DIR=.

# include paths

# VxWorks specific includes
VW_INC=/tornado/target/h
#VW_INC=.

# library path - used for integrated products and simulation env.
LIB_PATH=.

# portable include path
PT_INC_PATH="-I$(CM_DIR) -I$(HI_DIR) -I$(SSL_DIR)"

# Acceptance test include path
ACC_INC_PATH="-I$(HI_DIR) -I$(CM_DIR) -I$(HIACC_DIR) -I$(SSL_DIR) \
              -I$(SM_DIR) -I$(OS_DIR) -I$(SS_DIR) -I$(HIHC_DIR) \
              -I$(HISM_DIR) -I$(SY_DIR)"
              #-I$(VW_INC)

#=======================================================================
# SECTION 2
# Product specific options

# Interface coupling flags
# All interfaces are loose coupled by default, comment the
# corresponding flag to tight couple an interface.
# Enable this flag if compiling with multi-threaded TUCL
#HIT (TUCL - user)
#HITLCFLG=
HITLCFLG=-DLCHIUIHIT -DLCHCLIHIT -DLCSOLIHIT

# Enable this flag if compiling with multi-threaded TUCL
#LHI (TUCL - LM)
#LHILCFLG=
LHILCFLG=-DLCHIMILHI -DLCSMHIMILHI

#TUCL Debug flags
#HIDBGFLG=
HIDBGFLG=-DCMINETDBG -DDEBUGP

# TUCL common file and error options
HIOPTS=-DHI $(HITLCFLG) $(LHILCFLG) $(HIDBGFLG) -DCM_INET2 -DHI_LKSCTP -DCM_LKSCTP \
       -DHI_MULTI_THREADED \
       #-DHI_TLS \
       #-DFTHA -DTDS_CORE2 -DTDS_ROLL_UPGRADE_SUPPORT \
       #-DHI_RUG -DHIT2 -DHITV1 -DLHIV1 \
       #-DCMINETFLATBUF \
       #-D_XPG4_2 \
       #-DIPV6_SUPPORTED \
       #-DNO_ERRCLASS \


#-------------------TUCL options for portable build
HIPTOPTS=$(HIOPTS)

#-------------------TUCL options for acceptance test build
HIACCOPTS=$(HIOPTS) -DSM -DHC -DHI_ACC -DHI_ACC_ENB_PRNT -DHI_LKSCTP -DCM_LKSCTP \
          #-DHI_ACC_MCAST_ENB
          #-DHI_EXIT_ON_FAILURE \
          #-DHI_TEST_FOREVER \
          #-DHI_ACC_RAW_ENB \
          #-DHI_ACC_TLS_PRNG_FILE \
	  #-DFTHA

#======================================================================
# Options for compilation with different OS's
# SECTION 3

#-------------------System Service options

# options for a Portable Build
GENPTOSOPTS=-DCMFILE_REORG_2 -DCMFILE_REORG_1

# Enable for a Multithreaded acceptance test build
GENACCOSOPTS=-DCMFILE_REORG_1 -DCMFILE_REORG_2 #-DCONSTDIO

#-------------------System Service options
# Enable for Singlethreaded acceptance test build
#GENACCOSOPTS=-DCMFILE_REORG_1 -DCMFILE_REORG_2 -DCONSTDIO -DSS_SINGLE_THREADED

#------------------ SSL libraries
SSLLIBS=
#SSLLIBS=-L/usr/local/lib -lssl -lcrypto -ldl
#SSLLIBS=-lssl -lcrypto -ldl

# Portable build options
PTOSOPTS="-DPORTVER -DCMFILE_REORG_1 -DCMFILE_REORG_2"
#PTOSOPTS=""

# Options for acceptance test build with PSOS Os
PSOSOPTS="-DSS_PS"

# Options for acceptance test build with VrTx Os
VXOSOPTS="-DSS_VX"

# Options for acceptance test build with VxWorks Os
VWOSOPTS="-DSS_VW"

# Options for acceptance test build with WINNT Os kernel build
WINOSOPTS="-DWINNT"

# Options for acceptance test build with MTSS - Solaris/HP Unix
MTOSOPTS=-DSS_MT -D_REENTRANT -D__EXTENSIONS__

# Options for acceptance test build with MTSS - WINNT
NSOSOPTS="-DNS -DNS_ENB_MGMT -DSM"

# Options for acceptance test build with Linux
# SOACC_PRETTY_PRINT is an addition for colour debug prints during acceptance
# testing
LNXOSOPTS=-D_GNU_SOURCE -DSS_LINUX $(MTOSOPTS)

#=======================================================================
#
# Macro for output file name and makefile name
# SECTION 4

BASENAME=tl

MAKENAME=$(BASENAME).mak

# default acceptance test for Server Solaris cc environment
BINACC="$(BASENAME)_acc"


# default portable for Solaris cc environment
BINPT="$(BASENAME)_pt"

# Internal targets for Acceptance test build
MAKEACC=mkacc
ACCBIN=$(BASENAME)_acc

# Internal target for the portable build
MAKEPT=mkpt
PTBIN=$(BASENAME)_pt

# Internal target for makedepend
DEPEND=depend

#
# Build targets for testing in different environments

# Build with the Solaris CC compiler and linker
BINSOLACC=sol_acc
# Portable Build
BINSOLPT=sol_pt

# Build with the GNU compiler and linker on Solaris
BINGNUACC=gnu_acc
# Portable Build
BINGNUPT=gnu_pt

# Build with the Insite Memory Check programe - Acceptance Tests only
# Acceptance Test Build for server
BININSITEACC=ins_acc

# Build with the Visual C++ on Windows NT or Windows 95(user space only)
# Acceptance Test Build for server
BINWINACC=win_acc
# Portable Build
BINWINPT=win_pt

# Build with the microtech cross compiler
BINMRIACC=mri_acc
# Build with the Microtek MRI C compiler on Solaris
BINMRIPT=mri_pt

# Build with ccppc for VxWorks - 860 target
# Acceptance Test Build
BINWPPCACC=wppc_acc
# Portable Build
BINWPPCPT=wppc_pt

# Build with the MRI compiler for vxWorks - 68040 target
BINPPCACC=ppc_acc

# Build with the HP Unix cc compiler and linker
# Acceptance Test Build
BINHPACC=hpcc_acc
# Portable Build
BINHPPT=hpcc_pt

#Build with the cc386 compiler and liner on
BINVWACC=vw_acc

# Build with the GNU compiler and linker on Linux
# Acceptance Test Build
BINLNXACC=lnx_acc
# Portable Build
BINLNXPT=lnx_pt


#=======================================================================
#
# Macros for environments
# SECTION 5

# SUN SPARC Compiler for Solaris----------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with Sun Spro C version 4.0 running
# Solaris 2.4+
#For ANSI Compilation
SOLENV="-DSUNOS -DANSI"
SOLCOPTS="-Xa -g"

#added for 64 bit compilation
#SOLCOPTS="-g -mt -xarch=v9"

#For non ANSI compilation
#SOLENV="-DSUNOS"
#SOLCOPTS="-g "

SOLCC="cc"
SOLLD="cc"
SOLLOPTS="-o $(OUTNAME)"
SOLLIBS=-lrt -lnsl -lsocket -lpthread -lposix4
SOLLINT="lint"
SOLLNTOPTS="-c -F -p -v"

# SUN SPARC Compiler With Insight---------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with SUN Spro C version 4.0 C compiler running
# Solaris 2.4 , with the INSURE(insight) run time memory check tool.
#
# For ANSI Compilation
INSENV="-DSUNOS -DANSI -DPURE -DUSE_PURE"
INSCOPTS="-Xa -g -DNOERRCLASS"

# For non-ANSI Compilation
#INSENV=-DSUNOS -DUSE_PURE
#INSCOPTS="-g -DNOERRCLASS"

INSCC="insure"
INSLD="insure"
INSLOPTS="-o $(OUTNAME)"
INSLIBS=-lnsl -lsocket -lpthread -lposix4
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

# ccppc for Solaris
# These flags define the environment required to build an image on a Sun/SPARC
# workstation with ccppc v. 2.7.2 running Solaris 2.5.1
#
# For ANSI Compilation
PPCENV=-DCPU=PPC860 -DANSI -DPQUICCADS
PPCCOPTS=-g -mcpu=860 -msoft-float -fvolatile -fno-builtin -Wall \
	      -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
	      -Wcast-qual -fsigned-char

# For non-ANSI Compilation
#PPCENV=-DCPU=PPC860 -DCCPPC -UANSI
#PPCCOPTS=-mcpu=860 -g3 -msoft-float -fvolatile -fno-builtin -Wall \
#         -Wno-comment -pipe -nostdinc -Wshadow -Wredundant-decls \
#         -Wcast-qual -fsigned-char

PPCCC=ccppc
PPCLD="ldppc"
#PPCLOPTS=-r -Map $(OUTNAME).map -o $(OUTNAME)
PPCLOPTS=-r -e vwMain -Map $(OUTNAME).map -o $(OUTNAME)
PPCLIBS=
PPCLINT=
PPCLNTOPTS=

# ccppc on Windows NT
# These flags define the environment required to build an image on
# a Pentium machine with ccppc for an 860 target
#
# For ANSI Compilation
WPPCENV="-DCPU=PPC860 -DANSI -DPQUICCADS"
WPPCCOPTS="-g -mcpu=860 -msoft-float -fvolatile -fno-builtin -Wall \
	   -Wno-comment -ansi -pedantic -Wshadow \
	   -Wcast-qual -fsigned-char"
# For non-ANSI Compilation
#WPPCENV="-DCPU=PPC860 -DCCPPC -UANSI"
#WPPCCOPTS="-mcpu=860 -g3 -msoft-float -fvolatile -fno-builtin -Wall \
#           -Wno-comment -Wshadow -Wredundant-decls \
#           -Wcast-qual -fsigned-char"

WPPCCC="ccppc"
WPPCLD="ldppc"
#WPPCLOPTS=-r -Map $(OUTNAME).map -o $(OUTNAME)
WPPCLOPTS="-r -e vwMain -Map $(OUTNAME).map -o $(OUTNAME)"
WPPCLIBS=
WPPCLINT=
WPPCLNTOPTS=


# cc386 for VxWorks - Pentium Target
# These flags define the environment required to build an image on Vxworks
# with cc386 on Pentium Target
#
# For ANSI Compilation
VWINTENV="-DANSI -DCPU=PENTIUM -DCPU_VARIANT=PENTIUMPRO -DPQUICCADS"
VWINTCOPTS="-g -mpentium -msoft-float -fvolatile -fno-builtin -Wall \
	      -Wno-comment -ansi -pedantic -nostdinc -Wshadow \
         -Wcast-qual -fsigned-char"

# For non-ANSI Compilation
#VWINTENV="-DCPU=PENTIUM -DCPU_VARIANT=PENTIUMPRO -DPQUICCADS"
#VWINTCOPTS="-g -mpentium -msoft-float -fvolatile -fno-builtin -Wall \
#	       -Wno-comment -pipe -nostdinc -Wshadow -Wredundant-decls \
#         -Wcast-qual -fsigned-char"

VWINTCC="cc386 -mcpu=pentium"
VWINTLD="ld386"
VWINTLOPTS="-o $(OUTNAME) -r"
VWINTLIBS=


# GNU gcc for SPARC-----------------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
# For ANSI Compilation
GNUENV="-DSUNOS -DANSI"
GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wcast-qual \
          -Wstrict-prototypes -Wmissing-prototypes \
          -fno-strict-aliasing"

# For NON-ANSI Compilation
#GNUENV="-DSUNOS"
#GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
#          -fno-strict-aliasing"

GNUCC="gcc"
GNULD="gcc"

GNULOPTS="-o $(OUTNAME)"
GNULIBS=-lnsl -lsocket -lpthread -lposix4
GNULINT="lint"
GNULNTOPTS="-c -F -p -v"

# MRI mcc68k compiler for 68040-----------------------------------------
#
# These flags define the environment required to build an image for
# a Motorola MVME162 Board (68040) with Microtec Research C version 4.4
# for a portable environment.
#
# For ANSI Compilation
MRIENV="-DELVIS -DPROC_68XXX -DPROC_68040 -DENB_RELAY -DRY_ENBRAW \
	-DSOLMRI43 -DMAXBRDS=4 -DANSI -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"

# For non-ANSI Compilation
#MRIENV="-DELVIS -DPROC_68XXX -DPROC_68040 -DENB_RELAY -DRY_ENBRAW \
#         -DSOLMRI43 -DMAXBRDS=4 -DNOERRCHK -DNO_ERRCLS -DNOCONAVL"

MRICC="mcc68k"
MRILD="mcc68k"
MRICOPTS="-p68040 -H -A -v -Fsm -O -Ob -Oe -Ot -nKc -nKm  -Mcp -Mdp -Ml -Gf"
MRILOPTS="-e$(OUTNAME).lnk -Wl,\"-m > $(OUTNAME).map -o $(OUTNAME).vcs -F s\""
MRILIBS=$(LIB_PATH)/lib040.lib
MRILINT=""
MRILNTOPTS=""

# HP cc Compiler for HP Unix     ---------------------------------------
#
# These flags define the environment required to build an image on
# a HP PA workstation
#
# For ANSI Compilation
HPENV="-DHPOS -DANSI"
HPCOPTS="-g"

# For non-ANSI Compilation
#HPENV="-DSUNOS"
#HPCOPTS="-g -o"

HPCC="cc"
HPLD="cc"
HPLOPTS="-o $(OUTNAME)"
HPLIBS=-lnsl -lrt -lpthread
HPLINT="lint"
HPLNTOPTS="-c -F -p -v"

# GNU gcc compiler for Linux------------------------------------------
#
# These flags define the environment required to build an image on
# a Linux workstation with GNU C version 2.5.7 (or greater)
#
# For ANSI Compilation
LNXENV="-DSUNOS -DSS_LINUX -DANSI"
LNXCOPTS="-g3 -ansi -pipe -pedantic -Wall -Wno-comment -Wshadow \
	  -Wcast-align -Wcast-qual -Wstrict-prototypes \
          -Wmissing-prototypes -fno-strict-aliasing"

# For non-ANSI Compilation
#LNXENV="-DSUNOS -DSS_LINUX"
#LNXCOPTS="-g3 -pipe -pedantic -Wall -Wno-comment -Wshadow \
#	  -Wcast-align -Wcast-qual -fno-strict-aliasing"

LNXCC="gcc"
LNXLD="gcc"
LNXLOPTS="-o $(OUTNAME) -lpthread"
LNXLIBS=-lrt -lpthread
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"

# Microsoft Visual C++ for Win 95/NT------------------------------------
#
# These flags define the environment required to build an image for
# a Windows '95 or Windows NT environment
#
#
# For ANSI Compilation
WINENV="-DWIN32 -DWIN2K -DANSI -DSS_NO_DOT_NET"

# For non-ANSI Compilation
#WINENV="-DWIN32"

WINCC="cl"
WINLD="cl"
WINCOPTS="-Od -W3 -Zi -Yd -MD"
WINLOPTS="-o$(OUTNAME) -F250000 -Zd"
WINDBGOPTS="-link -debug:full -debugtype:cv"
# without OpenSSL
WINLIBS="ws2_32.lib -link -debug:full -debugtype:cv"
# with OpenSSL DLLs in /openssl/lib
#WINLIBS="ws2_32.lib ssleay32.lib libeay32.lib -link -debug:full -debugtype:cv -libpath:/openssl/lib"
WINLINT=""
WINLNTOPTS=""

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

# object file suffix----------------------------------------------------
#
# this defaults to 'o' for most UNIX based systems
# this can be set to 'obj' for DOS/Windows/NT platforms
#
UNIX_OBJ_SUFFIX=o
WIN_OBJ_SUFFIX=obj
OBJ=$(UNIX_OBJ_SUFFIX)

#=======================================================================
#
# macros for compile time options
#

# Combined environment, OS, compiler options and include path
CENVOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH)

# portable options
CPTOPTS=$(CENVOPTS) $(GENPTOSOPTS) $(OSOPTS)

 # acceptance test options
CACCOPTS=$(CENVOPTS) $(GENACCOSOPTS) $(OSOPTS)

# acceptance test options - OSOPTS should be set to required environment

# product options for portable build
CHIPTOPTS="$(CPTOPTS) $(HIPTOPTS)"

# product options for acceptance test build
CHIACCOPTS="$(CACCOPTS) $(HIACCOPTS)"

# product stack manager options for portable build
CHISMPTOPTS="$(CPTOPTS) $(HIPTOPTS)"

# product stack manager options for acceptance test build
CHISMACCOPTS="$(CACCOPTS) $(HIACCOPTS)"

# common stack manager options for portable build
CSMPTOPTS="$(CPTOPTS) $(HIOPTS)"

# common stack manager options for acceptance test build
CSMACCOPTS="$(CACCOPTS) $(HIOPTS)"

# OS options - for use with the MOS files in test environment
COSOPTS=$(CENVOPTS) $(GENACCOSOPTS) $(OSOPTS)

#=======================================================================
#
# Macros for depend and lint options
#
#
#
# depend acceptance test
DEPOPTS_ACC="$(HIACCOPTS)"

# depend portable test
DEPOPTS_PT="$(HIPTOPTS)"

#=======================================================================
#
# macros for hardcoded dependencies
# SECTION 6

#
# The following are enabled by default since we assume makedepend
# is not available. If makedepend is available in your environemnt
# uncomment out this section.
#

# environment (no makedepend)
ENV_INC=$(CM_DIR)/envdep.h $(CM_DIR)/envind.h $(CM_DIR)/envopt.h
#ENV_INV=

# system service (no makedepend)
SSI_INC=$(CM_DIR)/ssi.h $(CM_DIR)/ssi.x
#SSI_INC=

# general (no makedepend)
GEN_INC=$(CM_DIR)/gen.h $(CM_DIR)/gen.x $(CM_DIR)/cm_gen.h $(CM_DIR)/cm_gen.h
#GEN_INC=

# all system includes (no makedepend)
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)
#SYS_INC=

## Layer manager Interface management (no makedepend)
LHI_INC=$(CM_DIR)/lhi.h     \
	$(CM_DIR)/lhi.x

##TUCL upper interface (no makedepend)
HIT_INC= $(CM_DIR)/cm_tpt.h  \
        $(CM_DIR)/cm_tpt.x   \
	$(CM_DIR)/hit.h      \
        $(CM_DIR)/hit.x

# common (no makedepend)
CM_INC=$(CM_DIR)/cm_hash.h   \
	$(CM_DIR)/cm_hash.x  \
	$(CM_DIR)/cm_llist.h \
	$(CM_DIR)/cm_llist.x \
        $(CM_DIR)/cm5.h      \
        $(CM_DIR)/cm5.x      \
        $(CM_DIR)/cm_inet.h  \
        $(CM_DIR)/cm_inet.x  \
	$(CM_DIR)/cm_err.h   \
	$(CM_DIR)/cm_lib.x

HIMF_INC=

HIDB_INC=

# H.323 Transport Layer Interface (no makedepend)
HI_INC=$(HI_DIR)/hi.h        \
	$(HI_DIR)/hi.x       \
	$(HI_DIR)/hi_err.h
#HI_INC=

# ALL product includes
HIALL_INC=$(LHI_INC) $(HIT_INC) $(HI_INC) $(HIMF_INC) $(HIDB_INC)

HIACC_INC=$(HIACC_DIR)/hi_acc.h  \
           $(HIACC_DIR)/hi_acc.x \
           $(HIACC_DIR)hi_accen.h

#HI - sample acceptance test all includes
HIACCALL_INC=$(HIALL_INC) $(HIACC_INC)

# mos (no makedepend)
MS_INC=$(OS_DIR)/ms.h    \
	$(OS_DIR)/ms.x   \
	$(CM_DIR)/lms.h  \
        $(CM_DIR)/hw.x   \
        $(CM_DIR)/hw.h   \
	$(OS_DIR)/ms.x   \
	$(OS_DIR)/ms_err.h

# OS specific includes - SS includes
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h \
       $(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x \
       $(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x \
       $(SS_DIR)/ss_msg.h $(SS_DIR)/ss_msg.x \
       $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x \
       $(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x \
       $(SS_DIR)/ss_task.h $(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x
#SS_INC=


# OS specific includes - MTSS includes
MT_INC = $(MT_DIR)/mt_ss.h \
	 $(MT_DIR)/mt_ss.x \
	 $(MT_DIR)/mt_err.h

##=======================================================================
##
# macros for source and object files
# SECTION 7
#----------------------------------------- acceptance test source
ACC_SRCS=\
	$(HIACC_DIR)/hi_acc.c     \
	$(HIACC_DIR)/tl_acc1.c    \
	$(HIACC_DIR)/tl_acc2.c    \
	$(HIACC_DIR)/hi_accsh.c

#----------------------------------------- acceptance test server objects
ACC_OBJS= \
	$(SY_DIR)/hi_acc.$(OBJ)   \
	$(SY_DIR)/tl_acc1.$(OBJ)  \
	$(SY_DIR)/tl_acc2.$(OBJ)  \
	$(SY_DIR)/hi_accsh.$(OBJ)

#----------------------------------------- service user source
HIUL_SRCS= \
	$(HIHC_DIR)/layer5.c      \
	$(HIHC_DIR)/l5_ex_ms.c    \
	$(HIHC_DIR)/l5_ptli.c

#----------------------------------------- service user objects
HIUL_OBJS= \
	$(SY_DIR)/layer5.$(OBJ)   \
	$(SY_DIR)/l5_ex_ms.$(OBJ) \
	$(SY_DIR)/l5_ptli.$(OBJ)

#----------------------------------------- HI source
HI_SRCS= \
	$(HI_DIR)/tl_bdy1.c       \
 	$(HI_DIR)/tl_bdy2.c       \
 	$(HI_DIR)/tl_bdy3.c       \
 	$(HI_DIR)/hi_ptmi.c       \
 	$(HI_DIR)/hi_ptui.c       \
 	$(HI_DIR)/hi_id.c

#----------------------------------------- HI objects
HI_OBJS= \
	$(SY_DIR)/tl_bdy1.$(OBJ)  \
 	$(SY_DIR)/tl_bdy2.$(OBJ)  \
 	$(SY_DIR)/tl_bdy3.$(OBJ)  \
 	$(SY_DIR)/hi_ptmi.$(OBJ)  \
 	$(SY_DIR)/hi_ptui.$(OBJ)  \
 	$(SY_DIR)/hi_id.$(OBJ)

#----------------------------------------- common source
CMP_SRCS= \
	$(CM_DIR)/hit.c           \
	$(CM_DIR)/lhi.c

#----------------------------------------- common object
CMP_OBJS= \
	$(SY_DIR)/hit.$(OBJ)      \
	$(SY_DIR)/lhi.$(OBJ)


#----------------------------------------- common source
CM_SRCS= \
	$(CM_DIR)/cm_bdy5.c       \
	$(CM_DIR)/cm_hash.c       \
	$(CM_DIR)/cm_llist.c      \
	$(CM_DIR)/cm_inet.c       \
	$(CM_DIR)/cm_tpt.c        \
	$(CM_DIR)/cm_gen.c        \
	$(CM_DIR)/cm_lib.c        \
   $(CM_DIR)/sht.c

#----------------------------------------- common object
CM_OBJS= \
	$(SY_DIR)/cm_bdy5.$(OBJ)  \
	$(SY_DIR)/cm_hash.$(OBJ)  \
	$(SY_DIR)/cm_llist.$(OBJ) \
	$(SY_DIR)/cm_inet.$(OBJ)  \
	$(SY_DIR)/cm_tpt.$(OBJ)   \
	$(SY_DIR)/cm_gen.$(OBJ)   \
	$(SY_DIR)/cm_lib.$(OBJ)   \
	$(SY_DIR)/sht.$(OBJ)

#----------------------------------------- stack manager source
SM_SRCS= \
	$(SM_DIR)/sm_bdy1.c      \
	$(SM_DIR)/sm_ex_ms.c

#----------------------------------------- stack manager objects
SM_OBJS= \
	$(SY_DIR)/sm_bdy1.$(OBJ)  \
	$(SY_DIR)/sm_ex_ms.$(OBJ)


#----------------------------------------- HI stack manager source
HISM_SRCS= \
 	$(HI_DIR)/hi_ex_ms.c     \
	$(SM_DIR)/smhibdy1.c     \
	$(SM_DIR)/smhiexms.c     \
	$(SM_DIR)/smhiptmi.c

#----------------------------------------- HI stack manager objects
HISM_OBJS= \
 	$(SY_DIR)/hi_ex_ms.$(OBJ) \
	$(SY_DIR)/smhibdy1.$(OBJ) \
	$(SY_DIR)/smhiexms.$(OBJ) \
	$(SY_DIR)/smhiptmi.$(OBJ)

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
        $(SS_DIR)/ss_rtr.c              \
        $(OS_DIR)/cm_mem.c

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
        $(SY_DIR)/ss_rtr.$(OBJ)         \
        $(SY_DIR)/cm_mem.$(OBJ)

#------------------------------------------ MT sources
MT_SRCS= \
        $(OS_DIR)/mt_ss.c               \
        $(OS_DIR)/mt_id.c

#------------------------------------------- MT objects
MT_OBJS= \
        $(SY_DIR)/mt_ss.$(OBJ)          \
        $(SY_DIR)/mt_id.$(OBJ)

#------------------------------------------ MT sources
VW_SRCS= \
        $(OS_DIR)/vw_ss.c               \
        $(OS_DIR)/vw_id.c

#------------------------------------------- MT objects
VW_OBJS= \
        $(SY_DIR)/vw_ss.$(OBJ)          \
        $(SY_DIR)/vw_id.$(OBJ)

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
        $(NS_DIR)/ns_timer.c

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
        $(SY_DIR)/ns_timer.$(OBJ)

#----------------------------------------- portable sources
PT_SRCS= \
	$(HI_DIR)/hi_tst.c        \
	$(HI_DIR)/hi_ex_pt.c      \
	$(CM_DIR)/ss_ptsp.c

#----------------------------------------- portable sources
PT_OBJS= \
	$(SY_DIR)/hi_tst.$(OBJ)   \
	$(SY_DIR)/hi_ex_pt.$(OBJ) \
	$(SY_DIR)/ss_ptsp.$(OBJ)

#----------------------------------------- portable source
HIPT_SRCS= \
	$(HI_SRCS)                \
        $(CMP_SRCS)               \
        $(CM_SRCS)                \
        $(PT_SRCS)

#----------------------------------------- all acceptance test source
HIACC_SRCS=                       \
	$(ACC_SRCS)               \
 	$(HI_SRCS)                \
	$(HIUL_SRCS)              \
	$(CM_SRCS)                \
        $(CMP_SRCS)               \
	$(SM_SRCS)                \
        $(HISM_SRCS)              \
	$(OS_SRCS)

#----------------------------------------- all acceptance test Server objects
HIACC_OBJS=                       \
	    $(ACC_OBJS)           \
	    $(HI_OBJS)            \
	    $(HIUL_OBJS)            \
	    $(CM_OBJS)            \
	    $(CMP_OBJS)           \
	    $(SM_OBJS)            \
            $(HISM_OBJS)          \
	    $(OS_OBJS)

#
#----------------------------------------- portable objects
HIPT_OBJS= \
	$(HI_OBJS)                \
        $(CMP_OBJS)               \
        $(CM_OBJS)                \
        $(PT_OBJS)

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
CAT=cat
#CAT=type
OPENSSL=openssl


#----------------------------------------- child files (extract targets)
CHILD_LIST=           # HI does not have any Child Files.

#=======================================================================
#
# build targets
#

# clean-----------------------------------------------------------------
#
# cleans up everything...
#
clean:
	$(REMOVE) *.$(OBJ) $(BINACC) $(BINPT) *.ln *.s *.o

win_clean:
	$(DELETE) *.$(OBJ)
	$(DELETE) $(BINACC).exe
	$(DELETE) $(BINPT).exe
	$(DELETE) *.ln
	$(DELETE) *.s

# TLS certificates
tls_certs: root.cnf serverCA.cnf server.cnf client.cnf
	$(OPENSSL) req -newkey rsa:1024 -sha1 -keyout rootkey.pem \
	-out rootreq.pem -config root.cnf
	$(OPENSSL) x509 -req -in rootreq.pem -sha1 -extfile root.cnf \
	-extensions certificate_extensions -signkey rootkey.pem \
	-out rootcert.pem
	$(CAT) rootcert.pem rootkey.pem > root.pem
	$(OPENSSL) req -newkey rsa:1024 -sha1 -keyout serverCAkey.pem \
	-out serverCAreq.pem -config serverCA.cnf
	$(OPENSSL) x509 -req -in serverCAreq.pem -sha1 \
	-extfile serverCA.cnf -extensions certificate_extensions \
	-CA root.pem -CAkey root.pem -CAcreateserial -out serverCAcert.pem
	$(CAT) serverCAcert.pem serverCAkey.pem rootcert.pem > serverCA.pem
	$(OPENSSL) req -newkey rsa:1024 -sha1 -keyout serverkey.pem \
	-out serverreq.pem -config server.cnf -reqexts req_extensions
	$(OPENSSL) x509 -req -in serverreq.pem -sha1 -extfile server.cnf \
	-extensions certificate_extensions -CA serverCA.pem \
	-CAkey serverCA.pem -CAcreateserial -out servercert.pem
	$(CAT) servercert.pem serverkey.pem serverCAcert.pem \
	rootcert.pem > server.pem
	$(OPENSSL) req -newkey rsa:1024 -sha1 -keyout clientkey.pem \
	-out clientreq.pem -config client.cnf -reqexts req_extensions
	$(OPENSSL) x509 -req -in clientreq.pem -sha1 -extfile client.cnf \
	-extensions certificate_extensions -CA root.pem -CAkey root.pem \
	-CAcreateserial -out clientcert.pem
	$(CAT) clientcert.pem clientkey.pem rootcert.pem > client.pem

#External target for acceptance test build
acc:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC) F_OBJS="$(HIACC_OBJS)"

#External target for a portable build
pt:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OSOPTS=$(PTOSOPTS) \
	INC_PATH=$(PT_INC_PATH) OUTNAME=$(BINPT)

#=======================================================================
# SECTION 8 and 9
# Portable Target builds for Solaris CC, Solaris GNU, MRI mcc68k on
# Solaris and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC portable build---------------------------------------------
#
$(BINSOLPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) \
	LIBS="$(SOLLIBS) $(SSLLIBS)" OBJ=$(UNIX_OBJ_SUFFIX) \
	INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

# Solaris GNU portable build--------------------------------------------
#
$(BINGNUPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) \
	LIBS="$(GNULIBS) $(SSLLIBS)" OBJ=$(UNIX_OBJ_SUFFIX) \
	INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

# Linux GNU portable build--------------------------------------------
#
$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) \
	LIBS="$(LNXLIBS) $(SSLLIBS)" OBJ=$(UNIX_OBJ_SUFFIX) \
	INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

# HP Unix GNU portable build---------------------------------------------
#
$(BINHPPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(HPENV) CC=$(HPCC)  \
	LD=$(HPLD) COPTS=$(HPCOPTS) LOPTS=$(HPLOPTS) \
	LIBS="$(HPLIBS) $(SSLLIBS)" OBJ=$(UNIX_OBJ_SUFFIX)

# Microsoft Visual C++ for WINNT/WIN95 portable build-------------------
$(BINWINPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) \
	LIBS=$(WINLIBS) OBJ=$(WIN_OBJ_SUFFIX) \
	INC_PATH=$(PT_INC_PATH) OSOPTS=$(PTOSOPTS)

# MicroTec Research Compiler in Solaris Environment---------------------
$(BINMRIPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(MRIENV) CC=$(MRICC) \
	LD=$(MRILD) COPTS=$(MRICOPTS) LOPTS=$(MRILOPTS) \
	LIBS="$(MRILIBS) $(SSLLIBS)" OBJ=$(UNIX_OBJ_SUFFIX)

# Vxworks ccppc on Pentium - 860 target---------------------------------
#
$(BINWPPCPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(WPPCENV) CC=$(WPPCCC) \
	LD=$(WPPCLD) COPTS=$(WPPCCOPTS) LOPTS=$(WPPCLOPTS) \
	LIBS="$(WPPCLIBS) $(SSLLIBS)" INC_PATH=$(PT_INC_PATH) \
	OSOPTS="$(PTOSOPTS)" OBJ=$(UNIX_OBJ_SUFFIX)

#
# Acceptance test target builds for Solaris CC, Solaris GNU,
# Inisite Memory Check tool and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC acceptance test build-------------------------------
#
$(BINSOLACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) \
	LIBS="$(SOLLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS) $(SS_OBJS)" OS_DIR=$(MT_DIR)

# Solaris GNU acceptance test build-------------------------------------
#
$(BINGNUACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) \
	LIBS="$(GNULIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS) $(SS_OBJS)" OS_DIR=$(MT_DIR)

# Linux GNU acceptance test build-------------------------------------
#
$(BINLNXACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) \
	LIBS="$(LNXLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(LNXOSOPTS)" OS_SRCS="$(MT_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS) $(SS_OBJS)" OS_DIR=$(MT_DIR)

# Microtech cross compiler acceptance test build-------------------------
#
$(BINMRIACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(MRIENV) CC=$(MRICC) \
	LD=$(MRILD) COPTS=$(MRICOPTS) LOPTS=$(MRILOPTS) \
	LIBS="$(MRILIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS) $(SS_OBJS)" OS_DIR=$(MT_DIR)

# Microsoft Visual C++ for WINNT/WIN95 acceptance test Server build-----
$(BINWINACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) \
	LIBS=$(WINLIBS) INC_PATH=$(ACC_INC_PATH) \
	OSOPTS=$(NSOSOPTS) OS_SRCS="$(NS_SRCS) $(SS_SRCS)" \
	OBJ=$(WIN_OBJ_SUFFIX) OS_OBJS="$(NS_OBJS) $(SS_OBJS)" OS_DIR=$(NS_DIR)

# Vxworks cc386 acceptance test build on Pentium Target----------------
#
$(BINVWACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(VWINTENV) CC=$(VWINTCC) \
	LD=$(VWINTLD) COPTS=$(VWINTCOPTS) LOPTS=$(VWINTLOPTS) \
	LIBS="$(VWINTLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(VWOSOPTS)" OS_SRCS="$(VW_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS) $(SS_OBJS)" OS_DIR=$(VW_DIR)

# Vxworks ccppc acceptance test build on Pentium - 860 target-----------
#
$(BINWPPCACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(WPPCENV) CC=$(WPPCCC) \
	LD=$(WPPCLD) COPTS=$(WPPCCOPTS) LOPTS=$(WPPCLOPTS) \
	LIBS="$(WPPCLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(VWOSOPTS)" OS_SRCS="$(VW_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS) $(SS_OBJS)" OS_DIR=$(VW_DIR)

# Vxworks ccppc acceptance test build on Solaris------------------------
#
$(BINPPCACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" \
	LIBS="$(PPCLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS=$(VWOSOPTS) OS_SRCS="$(VW_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(VW_OBJS) $(SS_OBJS)" OS_DIR=$(VW_DIR)

# Inisite Memory Check Toolin Solaris acceptance test Server build-------
$(BININSITEACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) \
	LIBS="$(INSLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS) $(SS_OBJS)" OS_DIR=$(MT_DIR)

# HP Unix cc acceptance test build
$(BINHPACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(HPENV) CC=$(HPCC) \
        LD=$(HPLD) COPTS=$(HPCOPTS) LOPTS=$(HPLOPTS) \
	LIBS="$(HPLIBS) $(SSLLIBS)" INC_PATH=$(ACC_INC_PATH) \
	OSOPTS="$(MTOSOPTS)" OS_SRCS="$(MT_SRCS) $(SS_SRCS)" \
	OBJ=$(UNIX_OBJ_SUFFIX) OS_OBJS="$(MT_OBJS) $(SS_OBJS)" OS_DIR=$(MT_DIR)

# portable--------------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEPT):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CHIOPTS=$(CHIPTOPTS) \
	CHISMOPTS=$(CHISMPTOPTS) CSMOPTS=$(CSMPTOPTS)

$(PTBIN): $(HIPT_OBJS)
	$(LD) $(LOPTS) $(HIPT_OBJS) $(LIBS)


# acceptance test-------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEACC):
	@$(MAKE) -f $(MAKENAME) $(ACCBIN) CHIOPTS=$(CHIACCOPTS) \
	CHISMOPTS=$(CHISMACCOPTS) CSMOPTS=$(CSMACCOPTS)

$(ACCBIN): $(HIACC_OBJS)
	$(LD) $(LOPTS) $(HIACC_OBJS) $(LIBS)

#
# Make Depend
# depend portable test--------------------------------------------------
# dependency for portable source
depend_pt: $(HIPT_SRCS)
	$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(PTOSOPTS) \
	DEPOPTS=$(DEPOPTS_PT) INC_PATH=$(PT_INC_PATH) DEP_SRCS="$(HIPT_SRCS)"

# depend acceptance test------------------------------------------------
# dependency for acceptance test
depend_acc: $(HIACC_SRCS)
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(MSOSOPTS) \
	DEPOPTS=$(DEPOPTS_ACC) INC_PATH=$(ACC_INC_PATH) DEP_SRCS="$(HIACC_SRCS)"

# Build the dependency
$(DEPEND):
	$(MAKEDEPEND) -f $(MAKENAME) $(COSOPTS) $(DEPOPTS) $(INC_PATH) $(DEP_SRCS)
#=======================================================================
# Source code build directives
# SECTION 10
# Modify this section to list out the build directives for the specific
# product

#----------------------------------------- acceptance test objects
$(SY_DIR)/hi_acc.$(OBJ): $(HIACC_DIR)/hi_acc.c
	$(CC) -c $(CHIOPTS) $(HIACC_DIR)/hi_acc.c

$(SY_DIR)/tl_acc1.$(OBJ): $(HIACC_DIR)/tl_acc1.c
	$(CC) -c $(CHIOPTS) $(HIACC_DIR)/tl_acc1.c

$(SY_DIR)/tl_acc2.$(OBJ): $(HIACC_DIR)/tl_acc2.c
	$(CC) -c $(INC_PATH) $(CHIOPTS) $(HIACC_DIR)/tl_acc2.c

$(SY_DIR)/hi_accsh.$(OBJ): $(HIACC_DIR)/hi_accsh.c
	$(CC) -c $(CHIOPTS) $(HIACC_DIR)/hi_accsh.c
#----------------------------------------- HI Layer user objects

$(SY_DIR)/layer5.$(OBJ): $(HIHC_DIR)/layer5.c
	$(CC) -c $(CHIOPTS) $(HIACC_DIR)/layer5.c

$(SY_DIR)/l5_ex_ms.$(OBJ): $(HIHC_DIR)/l5_ex_ms.c
	$(CC) -c $(CHIOPTS) $(HIACC_DIR)/l5_ex_ms.c

$(SY_DIR)/l5_ptli.$(OBJ): $(HIHC_DIR)/l5_ptli.c
	$(CC) -c $(CHIOPTS) $(HIACC_DIR)/l5_ptli.c

#----------------------------------------- HI Layer objects

$(SY_DIR)/tl_bdy1.$(OBJ): $(HI_DIR)/tl_bdy1.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS)  $(HI_DIR)/tl_bdy1.c

$(SY_DIR)/tl_bdy2.$(OBJ): $(HI_DIR)/tl_bdy2.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/tl_bdy2.c

$(SY_DIR)/tl_bdy3.$(OBJ): $(HI_DIR)/tl_bdy3.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/tl_bdy3.c

$(SY_DIR)/hi_id.$(OBJ): $(HI_DIR)/hi_id.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/hi_id.c

$(SY_DIR)/hi_ex_ms.$(OBJ): $(HI_DIR)/hi_ex_ms.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/hi_ex_ms.c

$(SY_DIR)/hi_ptui.$(OBJ): $(HI_DIR)/hi_ptui.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/hi_ptui.c

$(SY_DIR)/hi_ptmi.$(OBJ): $(HI_DIR)/hi_ptmi.c $(SYS_INC) $(LHI_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/hi_ptmi.c

$(SY_DIR)/hi_tst.$(OBJ): $(HI_DIR)/hi_tst.c $(SYS_INC) $(LHI_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/hi_tst.c

$(SY_DIR)/hi_ex_pt.$(OBJ): $(HI_DIR)/hi_ex_pt.c $(SYS_INC) $(LHI_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(HI_DIR)/hi_ex_pt.c

#----------------------------------------- common objects

$(SY_DIR)/cm_hash.$(OBJ): $(CM_DIR)/cm_hash.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_hash.c

#$(SY_DIR)/cm_tpt.$(OBJ): $(CM_DIR)/cm_tpt.c $(SYS_INC) $(CM_INC)
#	$(CC) -c $(CHIOPTS) $(GENOSOPTS) $(CM_DIR)/cm_tpt.c

$(SY_DIR)/cm_tpt.$(OBJ): $(CM_DIR)/cm_tpt.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_tpt.c

$(SY_DIR)/cm_llist.$(OBJ): $(CM_DIR)/cm_llist.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_llist.c

$(SY_DIR)/cm_inet.$(OBJ): $(CM_DIR)/cm_inet.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_inet.c

#$(SY_DIR)/cm_bdy5.$(OBJ): $(CM_DIR)/cm_bdy5.c $(SYS_INC) $(CM_INC)
#	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_bdy5.c

$(SY_DIR)/cm_bdy5.$(OBJ): $(CM_DIR)/cm_bdy5.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_bdy5.c

$(SY_DIR)/hit.$(OBJ): $(CM_DIR)/hit.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/hit.c

$(SY_DIR)/lhi.$(OBJ): $(CM_DIR)/lhi.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/lhi.c

#$(SY_DIR)/cm_gen.$(OBJ): $(CM_DIR)/cm_gen.c $(SYS_INC) $(CM_INC)
#	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_gen.c

$(SY_DIR)/cm_gen.$(OBJ): $(CM_DIR)/cm_gen.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_gen.c

#$(SY_DIR)/cm_lib.$(OBJ): $(CM_DIR)/cm_lib.c $(SYS_INC) $(CM_INC)
#	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_lib.c

$(SY_DIR)/cm_lib.$(OBJ): $(CM_DIR)/cm_lib.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/cm_lib.c

#$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_DIR)/ss_ptsp.c $(SYS_INC) $(CM_INC)
#	$(CC) -c $(CHIOPTS) $(CM_DIR)/ss_ptsp.c

$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_DIR)/ss_ptsp.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/ss_ptsp.c

$(SY_DIR)/sht.$(OBJ): $(CM_DIR)/sht.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CHIOPTS) $(CM_DIR)/sht.c
#----------------------------------------- mos objects

$(SY_DIR)/ms_bdy1.$(OBJ): $(OS_DIR)/ms_bdy1.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(MSOPTS) $(COSOPTS) $(OS_DIR)/ms_bdy1.c

$(SY_DIR)/ms_bdy2.$(OBJ): $(OS_DIR)/ms_bdy2.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(MSOPTS) $(COSOPTS) $(OS_DIR)/ms_bdy2.c

$(SY_DIR)/ms_id.$(OBJ): $(OS_DIR)/ms_id.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(MSOPTS) $(COSOPTS) $(OS_DIR)/ms_id.c

$(SY_DIR)/ms_ptmi.$(OBJ): $(OS_DIR)/ms_ptmi.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(MSOPTS) $(COSOPTS) $(OS_DIR)/ms_ptmi.c

$(SY_DIR)/ms_ex_ms.$(OBJ): $(OS_DIR)/ms_ex_ms.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(MSOPTS) $(COSOPTS) $(OS_DIR)/ms_ex_ms.c

$(SY_DIR)/cm_ss.$(OBJ): $(CM_DIR)/cm_ss.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(MSOPTS) $(COSOPTS) $(CM_DIR)/cm_ss.c

#----------------------------------------- product stack manager objects
$(SY_DIR)/smhibdy1.$(OBJ): $(HISM_DIR)/smhibdy1.c $(SYS_INC) $(LMS_INC) $(SM_INC)
	$(CC) -c $(CHISMOPTS) $(HISM_DIR)/smhibdy1.c

$(SY_DIR)/smhiexms.$(OBJ): $(HISM_DIR)/smhiexms.c $(SYS_INC) $(LMS_INC) $(SM_INC)
	$(CC) -c $(CHISMOPTS) $(HISM_DIR)/smhiexms.c

$(SY_DIR)/smhiptmi.$(OBJ): $(HISM_DIR)/smhiptmi.c $(SYS_INC) $(LMS_INC) $(SM_INC)
	$(CC) -c $(CHISMOPTS) $(HISM_DIR)/smhiptmi.c

#----------------------------------------- common stack manager objects
$(SY_DIR)/sm_bdy1.$(OBJ): $(SM_DIR)/sm_bdy1.c $(SYS_INC) $(LHI_INC) $(SM_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/sm_bdy1.c

$(SY_DIR)/sm_ex_ms.$(OBJ): $(SM_DIR)/sm_ex_ms.c $(SYS_INC) $(LHI_INC) $(SM_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/sm_ex_ms.c

# System Services-------------------------------------------------------
$(SY_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_gen.c

$(SY_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_task.c

$(SY_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_drvr.c

$(SY_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_timer.c

$(SY_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_mem.c

$(SY_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_strm.c

$(SY_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_msg.c

$(SY_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_queue.c

$(SY_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_pack.c

$(SY_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c $(SYS_INC) $(SS_INC) $(OS_INC)
	$(CC) -c $(COSOPTS) $(SS_DIR)/ss_rtr.c

# MTSS-Solaris----------------------------------------------------------
$(SY_DIR)/mt_ss.$(OBJ): $(OS_DIR)/mt_ss.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/mt_ss.c

$(SY_DIR)/mt_id.$(OBJ): $(OS_DIR)/mt_id.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/mt_id.c

$(SY_DIR)/cm_mem.$(OBJ): $(OS_DIR)/cm_mem.c $(SYS_INC) $(SS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/cm_mem.c

# MTSS-Windows NT-------------------------------------------------------
$(SY_DIR)/ns_gen.$(OBJ): $(OS_DIR)/ns_gen.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_gen.c

$(SY_DIR)/smnsbdy1.$(OBJ): $(OS_DIR)/smnsbdy1.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(CSMOPTS) $(COSOPTS) $(OS_DIR)/smnsbdy1.c

$(SY_DIR)/smnsexms.$(OBJ): $(OS_DIR)/smnsexms.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/smnsexms.c

$(SY_DIR)/lns.$(OBJ): $(OS_DIR)/lns.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/lns.c

$(SY_DIR)/ns_ex_ms.$(OBJ): $(OS_DIR)/ns_ex_ms.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_ex_ms.c

$(SY_DIR)/ns_id.$(OBJ): $(OS_DIR)/ns_id.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_id.c

$(SY_DIR)/ns_ptmi.$(OBJ): $(OS_DIR)/ns_ptmi.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_ptmi.c

$(SY_DIR)/ns_space.$(OBJ): $(OS_DIR)/ns_space.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_space.c

$(SY_DIR)/ns_task.$(OBJ): $(OS_DIR)/ns_task.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_task.c

$(SY_DIR)/ns_timer.$(OBJ): $(OS_DIR)/ns_timer.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_timer.c

# vxWorks----------------------------------------------------------
$(SY_DIR)/vw_ss.$(OBJ): $(OS_DIR)/vw_ss.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_ss.c

$(SY_DIR)/vw_id.$(OBJ): $(OS_DIR)/vw_id.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_id.c


#
#********************************************************************30**
#
#        End of file:     tl.mak@@/main/3 - Mon Mar  3 20:09:19 2008
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
#---------- -------- ---- -----------------------------------------------
# 1.1       ---      asa  1. initial release.
# 1.2       ---      asa  1. removed smms* files
# 1.3       ---      asa  1. removed loose coupling flags
# 1.4       ---      asa  1. uncommented PORTVER flag
# 1.5       ---      asa  1. synchronizing versions
# /main/9   ---      cvp  1. added sht.c file
# /main/9   ---      cvp  1. Added HP Unix Section
# /main/1     ---      kp   1. Updated for release 1.5.
# /main/3     ---      hs   1. Updated for release of 2.1
#********************************************************************91*/

# DO NOT DELETE THIS LINE -- make depend depends on it.
