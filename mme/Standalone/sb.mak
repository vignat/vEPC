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
#       Name:   Simple Control Transmission Protocol (SCTP) (SB) - makefile
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file.
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target with gcc compiler for Solaris.
#
#               ENVIRONMENTS:
#
#               Solaris 2.x with gcc compiler for any CPU
#
#               Windows NT with gcc compiler for x86
#
#               BUILD TARGETS:
#
#               pt               portable software -- default
#               acc              acceptance tests using Solaris or NT SS
#               lib              library (with MTSS)
#
#               depend.acc       makedepend for acceptance tests
#               depend.pt        makedepend for portable software
#
#               clean            remove all object files
#
#       Sid:      sb.mak@@/main/4 - Thu Jul 26 07:53:18 2007
#
#       Prg:    wvdl
#
#********************************************************************21*/
#=======================================================================
#
# macros for product specific options
# SECTION 1
#PRDOPTS=-DSB -DCMFILE_REORG_1 -DCMFILE_REORG_2 -DLSB8 -DSB_CHECKSUM -DHI_REL_1_4 -DLSB6 -DLSB7 -DSCT3

PRDOPTS= -DHI_REL_1_4 -DSB -DCMFILE_REORG_1 -DCMFILE_REORG_2 -DSB_CHECKSUM -DSB_CHECKSUM_CRC -DHI_REL_1_4 -DLSB2 -DLSB3 -DLSB4 -DLSB5 -DLSB6 -DLSB7 -DLSB8 -DSCT2 -DSCT3 -DSCT4 -DSCT_ENDP_MULTI_IPADDR #-DSB_MD5 #-DSB_CHECKSUM_DUAL #SB_ETSI #SB_SWS

# Product options for loosely coupled interfaces
#PRDLCOPTS=
PRDLCOPTS=-DLCSBMILSB -DLCSMSBMILSB -DLCSBUISCT -DLCITLISCT -DLCSBLIHIT\
          -DLCHIUIHIT -DLCSMHIMILHI

# Product options for portable build
PRDPTOPTS=$(PRDOPTS) $(PRDLCOPTS)

# For WIN-NT/VxWorks compilation undefine flag SBACC_CMD_LINE from PRDACCOPTS.
# For VxWorks compilation,  SB_NOCFG_FILE flag must be defined in PRDACCOPTS
# For More Information about SB_NOCFG_FILE refer to  Portation Guide

# Product options for acceptance test build
PRDACCOPTS=$(PRDOPTS) $(PRDLCOPTS) -DHI -DIT -DSM -DSBACC_CMD_LINE -DSB_ACC


# Product options for inter-op test build
PRDINOPOPTS=$(PRDOPTS) $(PRDLCOPTS) -DHI -DIT -DSM  -DSB_INOP -D_GCC_LIMITS_H_ \
            -DHI_REL_1_2

# Product options for library build
PRDLIBOPTS=$(PRDPTOPTS) -DDBG4


#-------------------System Service options
GENOSOPTS=-DCMFILE_REORG_1 -DCMFILE_REORG_2
#          -DSS_SINGLE_THREADED
#          -DALIGN_64BIT -DALPHA

# Options for compilation with different OS's
# Default is for MTSS build
# For acceptance test build either uncomment specific OS environment
# required or specify on command line

# Portable build options
#for Linux
PTOSOPTS="-DPORTVER"

# Options for acceptance test build with MOS Os
MSOSOPTS="-DMSVER2"

# Options for acceptance test build with PSOS Os
PSOSOPTS="-DPS"

# Options for acceptance test build with VrTx Os
VXOSOPTS="-DVX"

# Options for acceptance test build with VxWorks Os
VWOSOPTS="-DSS_VW"

# Options for acceptance test build with WINNT Os kernel build
WINOSOPTS="-DWINNT"

# Linux specific options
LNXOSOPTS=-D_GNU_SOURCE -DSS_LINUX

# Options for acceptance test build with MTSS - WINNT
NSOSOPTS=-DNS

# Options for acceptance test build with MTSS - MOS multithread OS
MTOSOPTS=-DSS_MT -D_REENTRANT -D__EXTENSIONS__  -DDEBUGNOEXIT
         #-DSTDIO_INCLD

# For non-Linux Options for library build with MTSS - MOS multithread OS
GENLIBOSOPTS="-DSS_MT -D_REENTRANT -D__EXTENSIONS__  -DDEBUGNOEXIT"
# For Linux Options for library build with MTSS - MOS multithread OS
LNXLIBOSOPTS="-DSS_MT -D_REENTRANT -D__EXTENSIONS__  -DDEBUGNOEXIT -D_GNU_SOURCE -DSS_LINUX"

# Set OS options to include the OS build being used

#for Portable
#OSOPTS=$(GENOSOPTS) $(PTOSOPTS)

#for VxWorks
#OSOPTS=$(GENOSOPTS) $(VWOSOPTS)

#for Solaris MTSS
#OSOPTS=$(GENOSOPTS) $(MTOSOPTS)

#for Linux MTSS
#OSOPTS=$(GENOSOPTS) $(MTOSOPTS) $(LNXOSOPTS)

#for Windows NT MTSS
#OSOPTS=$(GENOSOPTS) $(NSOSOPTS)

#=======================================================================
#
# macro for output file name and makefile name
#
# SECTION 2
BASENAME=sb

MAKENAME=$(BASENAME).mak

# default acceptance test
BINACC="$(BASENAME)_acc"

# default portable
BINPT="$(BASENAME)_pt"

# default inter-op test
BININOP="$(BASENAME)_inop"

# default library for Solaris cc environment
BINLIB="$(BASENAME)_lib.lib"

# Internal targets for the acceptance test build
MAKEACC=mkacc
ACCBIN=$(BASENAME)_acc

# Internal target for the portable build
MAKEPT=mkpt
PTBIN=$(BASENAME)_pt

# Internal target for the inter-op test build
MAKEINOP=mkinop
INOPBIN=$(BASENAME)_inop

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
# Inter-op Build
BINSOLINOP=sol_inop
# Library Build
BINSOLLIB=sol_lib


# Build with the gcc compiler and linker on Solaris
# Acceptance Test Build
BINGNUACC=gnu_acc
# Portable Build
BINGNUPT=gnu_pt
# library Build
BINGNULIB=gnu_lib

# Inter-op Test Build
BINGNUINOP=gnu_inop

# Build with the GNU compiler and linker on Linux
# Acceptance Test Build
BINLNXACC=lnx_acc
# Portable Buil
BINLNXPT=lnx_pt
# Inter-op Test Build
BINLNXINOP=lnx_inop
# library Build
BINLNXLIB=lnx_lib

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
# Portable Build
BINMRIPT=mri_pt

# Build with the MRI compiler for vxWorks - 68040 target
# Acceptance Test Build
BINPPCACC=ppc_acc
# Portable Build
BINPPCPT=ppc_pt

#=======================================================================
#
# macro for directories and include paths
#
# SECTION 3

#Directory path
CODEPATH = ../../src

# common files directory
CM_DIR=$(CODEPATH)/cm

# product directory
SB_DIR=$(CODEPATH)/sctp

# product directory
HI_DIR=$(CODEPATH)/tucl

# lower layer files directory
ACCLL_DIR=.

# lower layer files directory for inter-op
INOPLL_DIR=$(CODEPATH)/mme

# common stack manager files directory
SM_DIR=$(CODEPATH)/mme

# stack manager directory
SBSM_DIR=$(CODEPATH)/mme

# common system services directory
SS_DIR=$(CODEPATH)/mt

# Multi-threaded (MTSS) Solaris system services directory
MT_DIR=$(CODEPATH)/mt

# WIN-NT system services directory
NS_DIR=./ns

# VxWorks system services directory
VW_DIR=.

# acceptance directory
SBACC_DIR=.

# inter-op test directory
SBINOP_DIR=.

# build directory
SY_DIR=.

# OS Directory - MTSS
OS_DIR=$(MT_DIR)

# OS Directory - WinNT
#OS_DIR=$(NS_DIR)

# OS Directory - VxWorks
#OS_DIR=$(VW_DIR)

# include path

# OS specific includes - VxWorks includes
VW_INC=
#VW_INC=/tools/vxworks/ppc/target/h

# portable include path
#PT_INC_PATH="-I$(CM_DIR) -I$(SB_DIR) -I$(VW_INC)"
PT_INC_PATH="-I$(SB_DIR) -I$(CM_DIR) -I$(SBACC_DIR) -I$(SM_DIR) \
   -I$(SBSM_DIR) -I$(SS_DIR) -I$(OS_DIR)"


# Acceptance test include path for Solaris MTSS
ACC_INC_PATH="-I$(SB_DIR) -I$(CM_DIR) -I$(SBACC_DIR) -I$(SM_DIR) \
   -I$(SBSM_DIR) -I$(SS_DIR) -I$(OS_DIR) -I$(VW_INC)"

# library include path
LIB_INC_PATH="-I$(SB_DIR) -I$(SY_DIR) -I$(CM_DIR) -I$(SS_DIR) -I$(MT_DIR)"

# inter-op test include path for Solaris MTSS
INOP_INC_PATH="-I$(SB_DIR) -I$(CM_DIR) -I$(SBACC_DIR) -I$(SM_DIR) \
   -I$(SBSM_DIR) -I$(SS_DIR) -I$(OS_DIR) -I$(INOPLL_DIR)"

# MTSS - files include path
MTINC_PATH=-I$(CM_DIR) -I$(MT_DIR)

# WIN-NT - files include path
NSINC_PATH=-I$(CM_DIR) -I$(NS_DIR)

# VxWorks-NT - files include path
VWINC_PATH=-I$(CM_DIR) -I$(VW_DIR)

# library path - used for integrated products and simulation env.
LIB_PATH=/u/lib

#=======================================================================
#
# macros for environments
#
# SUN SPARC Compiler for Solaris----------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with Sun Spro C version 4.0 running
# Solaris 2.4
#
# For ANSI Compilation
SOLENV="-DSUNOS -DANSI"
SOLCOPTS="-Xa -g"
#SOLCOPTS="-mt -xarch=v9 -o $(OUTNAME)"  #Added for 64 bit compilation

# For non-ANSI Compilation
#SOLENV="-DSUNOS"
#SOLCOPTS="-Xa -g"
#SOLENV="-DSUNOS"
#SOLCOPTS="-g -o$@"


SOLCC="/opt/SUNWspro/bin/cc"
#SOLCC="/usr/local/bin/g++" # RFC 4460 -- RTR -- for g++ compilation
#SOLCC="/usr/local/bin/c++" # RFC 4460 -- RTR -- for c++ compilation
SOLLD="/opt/SUNWspro/bin/cc"
#SOLLD="/usr/local/bin/g++" # RFC 4460 -- RTR -- for g++ compilation
#SOLLD="/usr/local/bin/c++" # RFC 4460 -- RTR -- for c++ compilation

SOLLIBLD="/usr/ccs/bin/ld"

SOLLOPTS="-o $(OUTNAME) -lposix4 -lpthread -lsocket -lnsl -lmd5" #RFC 4460 -- added lmd5

#RFC 4460 -- Added for 64-bit compilation
#SOLLOPTS="-mt -xarch=v9 -o $(OUTNAME) -lposix4 -lpthread -lsocket -lnsl -lmd5" #added for 64-bit compilation

#SOLLIBS=""
SOLLINT="lint"
SOLLNTOPTS="-c -F -p -v"

# for library compilation
SOLLIBOPTS="-r -o $(OUTNAME)"

#Added for 64 bit compilation
#SOLCC="/opt/SUNWspro/bin/cc"
#SOLLD="/opt/SUNWspro/bin/cc"
#SOLLOPTS="-o $(OUTNAME) -lpthread -lsocket -lposix4 -lnsl -g -mt -xarch=v9"



# SUN SPARC Compiler With Insight-----------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with SUN Spro C version 4.0 C compiler running
# Solaris 2.4 , with the INSURE(insight) run time memory check tool.
#
# For ANSI Compilation
INSENV="-DSUNOS -DANSI -DPURE"
INSCOPTS="-Xa -g"

# For non-ANSI Compilation
#INSENV=-DSUNOS -DPURE
#INSCOPTS="-g"

INSCC="insure"
INSLD="insure"
INSLOPTS="-o $(OUTNAME) -lpthread -lposix4 -lsocket"
INSLIBS=""
INSLINT="lint"
INSLNTOPTS="-c -F -p -v"

# GNU gcc compiler for SPARC--------------------------------------------
#
# These flags define the environment required to build an image on
# a Sun/SPARC workstation with GNU C version 2.5.7 (or greater) running
# Solaris 2.4
#
# For ANSI Compilation
GNUENV="-DSUNOS -DANSI"

# For non-ANSI Compilation
#GNUENV="-DSUNOS"

GNUCC="gcc"
GNULD="gcc"
GNULIBLD="ld"
# RFC 4460 -- general --
GNUCOPTS="-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
	-Wstrict-prototypes -Wmissing-prototypes -Wuninitialized  -Wunused -O "
GNULOPTS="-o $(OUTNAME) -lnsl -lpthread -lsocket -lposix4 -lmd5"

# RFC 4460 -- for 64 bit compilation
#GNUCOPTS="-g -m64 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
          -Wstrict-prototypes -Wmissing-prototypes -Wuninitialized  -Wunused -O "
#GNULOPTS=" -m64 -DALIGN_64BIT -DBIT_64 -o $(OUTNAME) -lnsl -lpthread -lsocket -lposix4 "
#GNUCOPTS="-g3 -ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
#	-Wstrict-prototypes -Wmissing-prototypes -Wuninitialized  -Wunused -O "

#GNULIBS="-lmd5"
GNULINT="lint"
GNULNTOPTS="-c -F -p -v"

# for library compilation
GNULIBOPTS="-r -o $(OUTNAME)"

# GNU gcc compiler for Linux------------------------------------------
#
# These flags define the environment required to build an image on
# a Linux workstation with GNU C version 2.5.7 (or greater)
#
# For ANSI Compilation
#LNXENV="-DSUNOS -DANSI"
#RFC 4460 -- new flags are added
LNXENV="-DSUNOS -DANSI -DSS_LINUX"

# For non-ANSI Compilation
#LNXENV="-DSUNOS"

LNXCC="gcc"
LNXLD="gcc"
LNXLIBLD="ld"
#LNXCOPTS="-O1 -g3 -ansi -Wall -Wcomment -pipe -Wshadow -Wcast-qual \
#	-Wstrict-prototypes -Wmissing-prototypes -pedantic -Wformat \
#	-Wimplicit -Wunused -Wchar-subscripts -Wuninitialized -Winline \
#	-Wreturn-type -fPIC"

#RFC 4460 -- as per RTR
LNXCOPTS="-g3 -ansi -pipe -pedantic -Wall -Wno-comment -Wshadow \
          -Wcast-align -Wcast-qual -Wstrict-prototypes \
          -Wmissing-prototypes"

# RFC 4460 -- for 64 bit compilation
#LNXCOPTS="-g -m64 -DALIGN_64BIT -DBIT_64 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
#          -Wstrict-prototypes -Wmissing-prototypes -Wuninitialized  -Wunused -O "

LNXLOPTS="-o $(OUTNAME) -lpthread -lnsl"
LNXLIBS=""
LNXLINT="lint"
LNXLNTOPTS="-c -F -p -v"
# for library compilation
LNXLIBOPTS="-r -o $(OUTNAME)"

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
MRICOPTS="-p68040 -H -A -v -Fsm -O -Ob -Oe -Ot -nKc -nKm  -Mcp -Mdp -Ml -Gf"
MRILOPTS="-e$(OUTNAME).lnk -Wl,\"-m > $(OUTNAME).map -o $(OUTNAME).vcs -F s\""
MRILIBS="$(LIB_PATH)/mcc68kpc040.lib"
MRILINT=""
MRILNTOPTS=""

# MRI mcc68k for 68040------------------------------------------
#
# These flags define the environment required to build an image for
# a vxWorks Motorola MVME162 Board (68040) with gnu compiler cc68k
#
# For ANSI Compilation
VWENV="-DMOTDCI_68360 -DANSI -DCPU=MC68040"

# For non-ANSI Compilation
#VWENV="-DMOTDCI_68360 -DCPU=MC68040"

VWCC="cc68k"
VWLD="cc68k"
VWCOPTS="-g3 -msoft-float -mnobitfield -fvolatile -fno-builtin -Wall \
        -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
        -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes"
VWLOPTS="-r"
VWLIBS=
VWLINT=
VWLNTOPTS=

# ccppc for Solaris
# These flags define the environment required to build an image on a Sun/SPARC
# workstation with ccppc v. 2.7.2 running Solaris 2.5.1
#
# For ANSI Compilation
#PPCENV=-DPCORE -DCPU=PPC604 -DANSI -DCCPPC
PPCENV=-DCPU=PPC860 -DCCPPC -DANSI -DPQUICCADS

# For non-ANSI Compilation
#PPCENV=-DPCORE -DCPU=PPC604 -DCCPPC

PPCCC=/tools/wind/host/sun4-solaris2/bin/ccppc
PPCLD=/tools/wind/host/sun4-solaris2/bin/ldppc

PPCCOPTS=-mcpu=860 -msoft-float -fvolatile -fno-builtin -Wall \
              -Wno-comment -ansi -pedantic -pipe -nostdinc -Wshadow \
              -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

PPCLOPTS=-r -e vwMain -Map $(OUTNAME).map -o $(OUTNAME)
PPCLIBS=
PPCLINT=
PPCLNTOPTS=

# Microsoft Visual C++ for Win 95/NT------------------------------------
#
# These flags define the environment required to build an image for
# a Windows '95 or Windows NT environment
#
#
# For ANSI Compilation
WINENV="-DWIN32 -DANSI -DWIN32_LEAN_AND_MEAN"

# For non-ANSI Compilation
#WINENV="-DWIN32"

WINCC="cl"
WINLD="cl"
#WINCOPTS="-Od -W3 -Zi -Yd"
WINCOPTS="-Od -W3 -Zi" #RFC 4460 -- -Yd has been removed
WINLOPTS="-o $(OUTNAME) -F25000"
WINLIBS="ws2_32.lib  -link -debug:full -debugtype:cv"
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

# object file suffix----------------------------------------------------
#
# this defaults to 'o' for most UNIX based systems
# this can be set to 'obj' for DOS/Windows/NT platforms
#
SOL_OBJ_SUFFIX=o
MS_OBJ_SUFFIX=obj
WIN_OBJ_SUFFIX=obj # RFC 4460
#OBJ=$(WIN_OBJ_SUFFIX) # RFC 446o -- for windows
OBJ=$(SOL_OBJ_SUFFIX)

#=======================================================================
#
# macros for compile time options
#

# portable options
CPTOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(PTOSOPTS)

# acceptance test options - OSOPTS should be set to required environment
COSOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS)

# Combined environment, OS, compiler options and include path
CENVOPTS=$(MODEL) $(ENV) $(COPTS) $(INC_PATH) $(OSOPTS)

# product options for portable build
CPRDPTOPTS="$(CENVOPTS) $(PRDPTOPTS)"

# product options for acceptance test build
CPRDACCOPTS="$(COSOPTS) $(PRDACCOPTS)"

# product options for inter-op test build
CPRDINOPTS="$(COSOPTS) $(PRDINOPOPTS)"

# product options for library build
CPRDLIBOPTS="$(CENVOPTS) $(PRDLIBOPTS)"

# Common Stack Manager File Options for portable build
CSMPTOPTS="$(CENVOPTS) $(PRDOPTS)"

# Common Stack Manager File Options for acceptance test build
CSMACCOPTS="$(CENVOPTS) $(PRDOPTS)"

# Common Stack Manager File Options for library build
CSMLIBOPTS="$(CENVOPTS) $(PRDOPTS)"

# MOS options - for use with the MOS files in test environment
CMSOPTS=$(CENVOPTS) $(MSINC_PATH) $(MSCOPTS)

CMTOPTS=$(CENVOPTS)
CSSOPTS=$(CENVOPTS)
CCMOPTS=$(CENVOPTS)

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
# SECTION 4

# environment
ENV_INC=$(CM_DIR)/envdep.h $(CM_DIR)/envind.h $(CM_DIR)/envopt.h
#ENV_INC=

# system service interface
SSI_INC=$(CM_DIR)/ssi.h $(CM_DIR)/ssi.x
#SSI_INC=

# general
GEN_INC=$(CM_DIR)/gen.h $(CM_DIR)/gen.x

# all system includes
SYS_INC=$(ENV_INC) $(GEN_INC) $(SSI_INC)

# Product Layer Management - LSB includes
LSB_INC=$(CM_DIR)/lsb.h $(CM_DIR)/lsb.x

# Product Upper Interface - SCT includes
SCT_INC=$(CM_DIR)/sct.h $(CM_DIR)/sct.x

# Product Lower Interface - HIT includes
HIT_INC=$(CM_DIR)/hit.h $(CM_DIR)/hit.x

# common function library includes
CM_INC=$(CM_DIR)/cm_err.h $(CM_DIR)/cm5.h $(CM_DIR)/cm5.x $(CM_DIR)/cm_hash.h \
	$(CM_DIR)/cm_hash.x $(CM_DIR)/cm_llist.h  $(CM_DIR)/cm_llist.x

# Product includes
SB_INC=$(SB_DIR)/sb.h $(SB_DIR)/sb.x $(SB_DIR)/sb_mtu.x $(SB_DIR)/sb_mtu.h \
	$(SB_DIR)/sb_err.h

# ALL product includes
SBALL_INC=$(LSB_INC) $(SB_INC) $(HIT_INC) $(SCT_INC)

# Product - sample acceptance test includes
SBACC_INC=$(SBACC_DIR)/sb_acc.h $(SBACC_DIR)/sb_acc.x

# Product - sample inter-op test includes
SBINOP_INC=$(SBINOP_DIR)/sb_ibdy.h $(SBINOP_DIR)/sb_ibdy.x

# MTSS Options
SS_INC=$(SS_DIR)/cm_mem.h $(SS_DIR)/cm_mem.x $(SS_DIR)/ss_dep.h \
	$(SS_DIR)/ss_err.h $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_mem.h \
	$(SS_DIR)/ss_msg.h $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_strm.h \
	$(SS_DIR)/ss_task.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_drvr.x \
	$(SS_DIR)/ss_gen.x $(SS_DIR)/ss_mem.x \
	$(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.x $(SS_DIR)/ss_strm.x \
	$(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x

# OS specific includes - MTSS includes
MT_INC=$(MT_DIR)/mt_ss.h $(MT_DIR)/mt_ss.x $(MT_DIR)/mt_err.h

# sample acceptance tests all includes
SBACCALL_INC=$(SBALL_INC) $(SBACC_INC) $(CM_INC)

# sample inter-op test all includes
SBINOPALL_INC= $(SBACCALL_INC) $(SBINOP_INC)

# OS specific includes - MOS includes
MS_INC=$(OS_DIR)/ms.h $(OS_DIR)/ms.x $(CM_DIR)/lms.x  \
	$(CM_DIR)/lms.h $(CM_DIR)/hw.x $(CM_DIR)/hw.h $(OS_DIR)/ms_err.h
#MS_INC=

#=======================================================================
#
# macros for source and object files
# SECTION 5

# acceptance test source------------------------------------------------
SBAC_SRCS= \
	$(SBACC_DIR)/sb_acc1.c \
	$(SBACC_DIR)/sb_acc2.c \
	$(SBACC_DIR)/sb_act1.c \
	$(SBACC_DIR)/sb_act2.c \
	$(SBACC_DIR)/sb_act3.c\
	$(SBACC_DIR)/sb_act4.c\
	$(SBACC_DIR)/sb_act5.c\
	$(SBACC_DIR)/sb_act6.c

# acceptance test object------------------------------------------------
SBAC_OBJS= \
	$(SY_DIR)/sb_acc1.$(OBJ) \
	$(SY_DIR)/sb_acc2.$(OBJ) \
	$(SY_DIR)/sb_act1.$(OBJ) \
	$(SY_DIR)/sb_act2.$(OBJ) \
	$(SY_DIR)/sb_act3.$(OBJ) \
	$(SY_DIR)/sb_act4.$(OBJ)\
	$(SY_DIR)/sb_act5.$(OBJ)\
	$(SY_DIR)/sb_act6.$(OBJ)

# inter-op test source------------------------------------------------
SBINOP_SRCS= \
	$(SBINOP_DIR)/sb_ibdy.c \
	$(SBINOP_DIR)/sb_icfg.c \
	$(SBINOP_DIR)/sb_iprim.c \
	$(SBINOP_DIR)/sb_isupp.c \
	$(CM_DIR)/cm_os.c	# for file operations in inter-op build

# inter-op test object------------------------------------------------
SBINOP_OBJS= \
	$(SY_DIR)/sb_ibdy.$(OBJ) \
	$(SY_DIR)/sb_icfg.$(OBJ) \
	$(SY_DIR)/sb_iprim.$(OBJ) \
	$(SY_DIR)/sb_isupp.$(OBJ) \
	$(SY_DIR)/cm_os.$(OBJ)	# for file operations in inter-op build

# sample upper layer source-------------------------------------------------
SBUL_SRCS= \
	$(SBACC_DIR)/it_bdy1.c \
	$(SBACC_DIR)/it_ex_ms.c \
	$(SBACC_DIR)/it_ptli.c

# sample upper layer objects------------------------------------------------
SBUL_OBJS= \
	$(SY_DIR)/it_bdy1.$(OBJ) \
	$(SY_DIR)/it_ex_ms.$(OBJ) \
	$(SY_DIR)/it_ptli.$(OBJ)

# sample lower layer source-------------------------------------------------
SBACCLL_SRCS= \
	$(LLDIR_USED)/hi_bdy1.c \
	$(LLDIR_USED)/hi_ex_ms.c \
	$(LLDIR_USED)/hi_ptui.c

# sample lower layer objects------------------------------------------------
SBACCLL_OBJS= \
	$(SY_DIR)/hi_bdy1.$(OBJ) \
	$(SY_DIR)/hi_ex_ms.$(OBJ) \
	$(SY_DIR)/hi_ptui.$(OBJ)

# sample inter-op lower layer source-------------------------------------
SBINOPLL_SRCS= \
        $(LLDIR_USED)/hi_bdy1.c       \
        $(LLDIR_USED)/hi_bdy2.c       \
        $(LLDIR_USED)/hi_bdy3.c       \
        $(LLDIR_USED)/hi_ptmi.c       \
        $(LLDIR_USED)/hi_ptui.c       \
        $(LLDIR_USED)/hi_id.c

# sample lower layer objects------------------------------------------------
SBINOPLL_OBJS= \
        $(SY_DIR)/hi_bdy1.$(OBJ)  \
        $(SY_DIR)/hi_bdy2.$(OBJ)  \
        $(SY_DIR)/hi_bdy3.$(OBJ)  \
        $(SY_DIR)/hi_ptmi.$(OBJ)  \
        $(SY_DIR)/hi_ptui.$(OBJ)  \
        $(SY_DIR)/hi_id.$(OBJ)


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


# common source(C source files)-----------------------------------------
SBCM_SRCS= \
	$(CM_DIR)/cm_gen.c \
	$(CM_DIR)/cm_lib.c \
	$(CM_DIR)/cm_hash.c \
	$(CM_DIR)/cm_llist.c \
	$(CM_DIR)/cm_bdy5.c \
	$(CM_DIR)/cm_tpt.c \
	$(CM_DIR)/cm_dns.c \
	$(CM_DIR)/cm_inet.c \
	$(CM_DIR)/hit.c \
	$(CM_DIR)/sct.c \
	$(SB_DIR)/lsb.c

# common objects--------------------------------------------------------
SBCM_OBJS= \
	$(SY_DIR)/cm_gen.$(OBJ) \
	$(SY_DIR)/cm_lib.$(OBJ) \
	$(SY_DIR)/cm_hash.$(OBJ) \
	$(SY_DIR)/cm_llist.$(OBJ) \
	$(SY_DIR)/cm_bdy5.$(OBJ) \
	$(SY_DIR)/cm_tpt.$(OBJ) \
	$(SY_DIR)/cm_dns.$(OBJ) \
	$(SY_DIR)/cm_inet.$(OBJ) \
	$(SY_DIR)/hit.$(OBJ) \
	$(SY_DIR)/sct.$(OBJ) \
	$(SB_DIR)/lsb.$(OBJ)

# common stack manager source-------------------------------------------
SM_SRCS= \
	$(SM_DIR)/sm_bdy1.c \
	$(SM_DIR)/sm_ex_ms.c

# common stack manager objects------------------------------------------
SM_OBJS= \
	$(SY_DIR)/sm_bdy1.$(OBJ) \
	$(SY_DIR)/sm_ex_ms.$(OBJ)

# Product stack manager source------------------------------------------
SBSM_SRCS= \
	$(SBSM_DIR)/smsbbdy1.c \
	$(SBSM_DIR)/smsbexms.c \
	$(SBSM_DIR)/smsbptmi.c

# Product stack manager objects-----------------------------------------
SBSM_OBJS= \
	$(SY_DIR)/smsbbdy1.$(OBJ) \
	$(SY_DIR)/smsbexms.$(OBJ) \
	$(SY_DIR)/smsbptmi.$(OBJ)

# Lower Layer stack manager source-------------------------------------
LLSM_SRCS= \
 	$(HI_DIR)/hi_ex_ms.c     \
	$(INOPLL_DIR)/smhibdy1.c     \
	$(INOPLL_DIR)/smhiexms.c     \
	$(INOPLL_DIR)/smhiptmi.c

# Lower Layer stack manager objects-----------------------------------
LLSM_OBJS= \
 	$(SY_DIR)/hi_ex_ms.$(OBJ) \
	$(SY_DIR)/smhibdy1.$(OBJ) \
	$(SY_DIR)/smhiexms.$(OBJ) \
	$(SY_DIR)/smhiptmi.$(OBJ)

# OS - MOS source-------------------------------------------------------
MS_SRCS= \
	$(OS_DIR)/ms_bdy1.c  \
	$(OS_DIR)/ms_bdy2.c  \
	$(OS_DIR)/ms_bdy5.c  \
	$(OS_DIR)/ms_id.c    \
	$(CM_DIR)/cm_ss.c    \
	$(OS_DIR)/ms_ex_ms.c \
	$(OS_DIR)/ms_ptmi.c

# OS - MOS objects------------------------------------------------------
MS_OBJS= \
	$(SY_DIR)/ms_bdy1.$(OBJ)  \
	$(SY_DIR)/ms_bdy2.$(OBJ)  \
	$(SY_DIR)/ms_bdy5.$(OBJ)  \
	$(SY_DIR)/ms_id.$(OBJ)    \
	$(SY_DIR)/cm_ss.$(OBJ)    \
	$(SY_DIR)/ms_ex_ms.$(OBJ) \
	$(SY_DIR)/ms_ptmi.$(OBJ)

# OS - common sources---------------------------------------------------
SS_SRCS= \
   $(SS_DIR)/ss_gen.c \
   $(SS_DIR)/ss_task.c \
   $(SS_DIR)/ss_drvr.c \
   $(SS_DIR)/ss_timer.c \
   $(SS_DIR)/ss_mem.c \
   $(SS_DIR)/ss_strm.c \
   $(SS_DIR)/ss_msg.c \
   $(SS_DIR)/ss_queue.c \
   $(SS_DIR)/ss_pack.c \
   $(SS_DIR)/ss_rtr.c \
   $(SS_DIR)/ss_id.c \
   $(SS_DIR)/cm_mem.c

# OS - common objects--------------------------------------------------
SS_OBJS= \
   $(SY_DIR)/ss_mem.$(OBJ) \
   $(SY_DIR)/ss_queue.$(OBJ) \
   $(SY_DIR)/ss_timer.$(OBJ) \
   $(SY_DIR)/ss_drvr.$(OBJ) \
   $(SY_DIR)/ss_msg.$(OBJ) \
   $(SY_DIR)/ss_rtr.$(OBJ) \
   $(SY_DIR)/ss_gen.$(OBJ) \
   $(SY_DIR)/ss_pack.$(OBJ) \
   $(SY_DIR)/ss_strm.$(OBJ) \
   $(SY_DIR)/ss_task.$(OBJ) \
   $(SY_DIR)/ss_id.$(OBJ) \
   $(SY_DIR)/cm_mem.$(OBJ)

# OS - MTSS sources--------------------------------------------------
MT_SRCS= \
	$(MT_DIR)/mt_ss.c \
	$(MT_DIR)/mt_id.c

# OS - MTSS objects--------------------------------------------------
MT_OBJS= \
	$(SY_DIR)/mt_ss.$(OBJ) \
	$(SY_DIR)/mt_id.$(OBJ)

# VxWorks sources---------------------------------------------------
VW_SRCS= \
	$(VW_DIR)/vw_ss.c               \
	$(VW_DIR)/vw_id.c

# VxWorks objects---------------------------------------------------
VW_OBJS= \
	$(SY_DIR)/vw_ss.$(OBJ)          \
	$(SY_DIR)/vw_id.$(OBJ)

# NT SS sources---------------------------------------------------
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

# NT SS objects---------------------------------------------------
NS_OBJS= \
	$(SY_DIR)/ns_gen.$(OBJ)        \
	$(SY_DIR)/smnsexms.$(OBJ)       \
	$(SY_DIR)/smnsbdy1.$(OBJ)       \
	$(SY_DIR)/lns.$(OBJ)            \
	$(SY_DIR)/ns_ex_ms.$(OBJ)       \
	$(SY_DIR)/ns_id.$(OBJ)          \
	$(SY_DIR)/ns_ptmi.$(OBJ)        \
	$(SY_DIR)/ns_space.$(OBJ)       \
	$(SY_DIR)/ns_task.$(OBJ)        \
	$(SY_DIR)/ns_timer.$(OBJ)

# OS -  MTSS sources---------------------------------------------
OS_SRCS=$(MT_SRCS) $(SS_SRCS)

# OS -  MTSS objects---------------------------------------------
OS_OBJS=$(MT_OBJS) $(SS_OBJS)

# OS -  WinNT sources---------------------------------------------
#OS_SRCS=$(NS_SRCS) $(SS_SRCS)

# OS -  WinNT objects---------------------------------------------
#OS_OBJS=$(NS_OBJS) $(SS_OBJS)

# OS - VxWorks sources---------------------------------------------
#OS_SRCS=$(VW_SRCS) $(SS_SRCS)

# OS - VxWorks objects---------------------------------------------
#OS_OBJS=$(VW_OBJS) $(SS_OBJS)

# Portable Build Sources------------------------------------------------
PT_SRCS= \
	$(SB_DIR)/sb_tst.c \
	$(SB_DIR)/ss_ptsp.c

# Portable Build Objects------------------------------------------------
PT_OBJS= \
	$(SY_DIR)/sb_tst.$(OBJ) \
	$(SY_DIR)/ss_ptsp.$(OBJ)

# all acceptance test source--------------------------------------------
PRDACC_SRCS= \
	$(SB_SRCS) \
	$(SBAC_SRCS) \
	$(SBUL_SRCS) \
	$(SBACCLL_SRCS) \
	$(SBCM_SRCS) \
	$(SM_SRCS) \
	$(SBSM_SRCS) \
	$(OS_SRCS)

# all acceptance test objects-------------------------------------------
PRDACC_OBJS= \
	$(SB_OBJS) \
	$(SBAC_OBJS) \
	$(SBUL_OBJS) \
	$(SBACCLL_OBJS) \
	$(SBCM_OBJS) \
	$(SM_OBJS) \
	$(SBSM_OBJS) \
	$(OS_OBJS)

# all inter-op test source--------------------------------------------
PRDINOP_SRCS= \
	$(SB_SRCS) \
	$(SBINOP_SRCS) \
	$(SBUL_SRCS) \
	$(SBINOPLL_SRCS) \
	$(SBCM_SRCS) \
	$(SM_SRCS) \
	$(SBSM_SRCS) \
	$(LLSM_SRCS) \
	$(OS_SRCS) \

# all inter-op test objects-------------------------------------------
PRDINOP_OBJS= \
	$(SB_OBJS) \
	$(SBINOP_OBJS) \
	$(SBUL_OBJS) \
	$(SBINOPLL_OBJS) \
	$(SBCM_OBJS) \
	$(SM_OBJS) \
	$(SBSM_OBJS) \
	$(LLSM_OBJS) \
	$(OS_OBJS) \

# all portable sources--------------------------------------------------
PRDPT_SRCS= \
	$(PT_SRCS) \
	$(SB_SRCS) \
	$(SBCM_SRCS)

# all portable objects--------------------------------------------------
PRDPT_OBJS= \
	$(PT_OBJS) \
	$(SB_OBJS) \
	$(SBCM_OBJS)

# all library sources--------------------------------------------------
#PRDLIB_SRCS=  $(SB_DIR)/sb_ex_ms.c $(SB_SRCS)
PRDLIB_SRCS= $(SB_SRCS)

# all library objects--------------------------------------------------
#PRDLIB_OBJS= $(SY_DIR)/sb_ex_ms.$(OBJ)  $(SB_OBJS)
PRDLIB_OBJS= $(SB_OBJS)

#=======================================================================
#
# special programs required for Internal Development by makefile
#
# adjust as required
#
# System Binary Dir - replace with appropriate directory and program
# for system
BIN=/bin
MAKEDEPEND=makedepend
REMOVE=$(BIN)/rm -f

#=======================================================================
#
# build targets
#

# clean-----------------------------------------------------------------
#
# cleans up everything...
#

all: acc

clean:
	$(REMOVE) $(SY_DIR)/*.$(OBJ) $(SY_DIR)/$(BINACC) $(SY_DIR)/$(BININOP) $(SY_DIR)/*.map
	$(REMOVE) $(SY_DIR)/$(BINPT) $(SY_DIR)/*.ln $(SY_DIR)/*.s

# External target for acceptance test build
acc:
	$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(ACC_INC_PATH) \
	OUTNAME=$(BINACC) LLDIR_USED=$(ACCLL_DIR)

# External target for inter-op test build
inop:
	$(MAKE) -f $(MAKENAME) $(BLDENV) INC_PATH=$(INOP_INC_PATH) \
	OUTNAME=$(BININOP) LLDIR_USED=$(INOPLL_DIR)

# External target for portable build
pt:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINPT) \
	INC_PATH=$(PT_INC_PATH)

# External target for portable build
lib:
	@$(MAKE) -f $(MAKENAME) $(BLDENV) OUTNAME=$(BINLIB)  \
	INC_PATH=$(LIB_INC_PATH)

# Portable Target builds for Solaris CC, Solaris GNU, MRI mcc68k on
# Solaris and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC portable build---------------------------------------------
#
$(BINSOLPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(SOLENV) CC=$(SOLCC)  \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(PTOSOPTS)
#
# Solaris GNU portable build--------------------------------------------
#
$(BINGNUPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(PTOSOPTS)
#
# Linux GNU portable build--------------------------------------------
#
$(BINLNXPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(PTOSOPTS) $(LNXOSOPTS)"
#
# Microsoft Visual C++ for WINNT/WIN95 portable build-------------------
$(BINWINPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(MS_OBJ_SUFFIX) OSOPTS=$(PTOSOPTS)
#
# MicroTec Research Compiler in Solaris Environment---------------------
$(BINMRIPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) -e ENV=$(MRIENV) CC=$(MRICC) \
	LD=$(MRILD) COPTS=$(MRICOPTS) LOPTS=$(MRILOPTS) LIBS=$(MRILIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(PTOSOPTS)

#
# PPC portable build-------------
$(BINPPCPT):
	@$(MAKE) -f $(MAKENAME) $(MAKEPT) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(PTOSOPTS)

#
# Acceptance test target builds for Solaris CC, Solaris GNU,
# Inisite Memory Check tool and Microsoft Visual C++ for WINNT/WIN95
#
# Solaris CC acceptance test build--------------------------------------
#
$(BINSOLACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS)"
#
# Solaris GNU acceptance test build-------------------------------------
#
$(BINGNUACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS)"
#
# Linux GNU acceptance test build-------------------------------------
#
$(BINLNXACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS) $(LNXOSOPTS)"
#
# Microsoft Visual C++ for WINNT/WIN95 acceptance test build------------
$(BINWINACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLD) COPTS=$(WINCOPTS) LOPTS=$(WINLOPTS) LIBS=$(WINLIBS) \
	OBJ=$(MS_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(NSOSOPTS)"
#
# Insite Memory Check Tool in Solaris acceptance test build-------------
$(BININSITEACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) -e ENV=$(INSENV) CC=$(INSCC) \
	LD=$(INSLD) COPTS=$(INSCOPTS) LOPTS=$(INSLOPTS) LIBS=$(INSLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS)"

#
# PPC acceptance test build-------------
$(BINPPCACC):
	@$(MAKE) -f $(MAKENAME) $(MAKEACC) "ENV=$(PPCENV)" CC=$(PPCCC) \
	LD=$(PPCLD) "COPTS=$(PPCCOPTS)" "LOPTS=$(PPCLOPTS)" LIBS=$(PPCLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX)

#
# Inter-op test target builds for Solaris GNU,
#
# Solaris GNU Inter-op test build-------------------------------------
#
$(BINGNUINOP):
	@$(MAKE) -f $(MAKENAME) $(MAKEINOP) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULD) COPTS=$(GNUCOPTS) LOPTS=$(GNULOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS)"

#
# Inter-op test target builds for Linux GNU,
#
# Linux GNU Inter-op test build-------------------------------------
#
$(BINLNXINOP):
	$(MAKE) -f $(MAKENAME) $(MAKEINOP) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS) $(LNXOSOPTS)"

#
# Interoperatibility test for solaris CC compiler
#
# Solaris CC Interoperatibility  test build--------------------------------------
#
$(BINSOLINOP):
	@$(MAKE) -f $(MAKENAME) $(MAKEINOP) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS="$(GENOSOPTS) $(MTOSOPTS)"

# portable--------------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEPT):
	@$(MAKE) -f $(MAKENAME) $(PTBIN) CPRDOPTS=$(CPRDPTOPTS) \
	CSMOPTS=$(CSMPTOPTS)

$(PTBIN): $(PRDPT_OBJS)
	$(LD) $(LOPTS) $(PRDPT_OBJS) $(LIBS)

$(LIBBIN): $(PRDLIB_OBJS)
	$(LD) $(LOPTS) $(PRDLIB_OBJS) $(LIBS)

#
# Library target builds for Solaris CC &  Solaris GNU
#
# Solaris CC library build--------------------------------------
#
$(BINSOLLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(SOLENV) CC=$(SOLCC) \
	LD=$(SOLLIBLD) COPTS=$(SOLCOPTS) LOPTS=$(SOLLIBOPTS) LIBS=$(SOLLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(GENLIBOSOPTS)

#
# Solaris GNU library build-------------------------------------
#
$(BINGNULIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(GNUENV) CC=$(GNUCC) \
	LD=$(GNULIBLD) COPTS=$(GNUCOPTS) LOPTS=$(GNULIBOPTS) LIBS=$(GNULIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(GENLIBOSOPTS)

#
# Window NT library build-------------------------------------
#
$(BINWINLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(WINENV) CC=$(WINCC) \
	LD=$(WINLIBLD) COPTS=$(WINCOPTS) LOPTS=$(WINLIBOPTS) LIBS=$(WINLIBS) \
	OBJ=$(MS_OBJ_SUFFIX) OSOPTS=$(GENLIBOSOPTS)


#
# Linux library build-------------------------------------
#
$(BINLNXLIB):
	@$(MAKE) -f $(MAKENAME) $(MAKELIB) -e ENV=$(LNXENV) CC=$(LNXCC) \
	LD=$(LNXLIBLD) COPTS=$(LNXCOPTS) LOPTS=$(LNXLIBOPTS) LIBS=$(LNXLIBS) \
	OBJ=$(SOL_OBJ_SUFFIX) OSOPTS=$(LNXLIBOSOPTS)

# acceptance test-------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEACC):
	@$(MAKE) -f $(MAKENAME) $(ACCBIN) CPRDOPTS=$(CPRDACCOPTS) \
	CSMOPTS=$(CSMACCOPTS)

$(ACCBIN): $(PRDACC_OBJS)
	$(LD) $(LOPTS) $(PRDACC_OBJS) $(LIBS)


# inter-op test-------------------------------------------------------
#
# The following assumes that the compile and linker options are
# set in the make-file before the definition of this target
#
$(MAKEINOP):
	$(MAKE) -f $(MAKENAME) $(INOPBIN) CPRDOPTS=$(CPRDINOPTS) \
	CSMOPTS=$(CSMACCOPTS)

$(INOPBIN): $(PRDINOP_OBJS)
	$(LD) $(LOPTS) $(PRDINOP_OBJS) $(LIBS)

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
	@$(MAKE) -f $(MAKENAME) $(DEPEND) -e OSOPTS=$(MTOPTS) \
	DEPOPTS=$(DEPOPTS_ACC) INC_PATH=$(ACC_INC_PATH) DEP_SRCS="$(PRDACC_SRCS)"

# Build the dependency
$(DEPEND):
	$(MAKEDEPEND) -f $(MAKENAME) $(OSOPTS) $(DEPOPTS) $(INC_PATH) $(DEP_SRCS)
#=======================================================================
#
# Source code build directives
# SECTION 6

# Portable build ------------------------------------------------
$(SY_DIR)/ss_ptsp.$(OBJ): $(CM_DIR)/ss_ptsp.c
	$(CC) -c $(CPTOPTS) $(CM_DIR)/ss_ptsp.c -o $(SY_DIR)/ss_ptsp.$(OBJ)

$(SY_DIR)/sb_tst.$(OBJ): $(SB_DIR)/sb_tst.c
	$(CC) -c $(CPTOPTS) $(SB_DIR)/sb_tst.c -o $(SY_DIR)/sb_tst.$(OBJ)
#acceptance test build------------------------------------------------

$(SY_DIR)/sb_acc1.$(OBJ): $(SBACC_DIR)/sb_acc1.c $(SYS_INC) $(SBACCALL_INC) $(VW_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_acc1.c -o $(SY_DIR)/sb_acc1.$(OBJ)

$(SY_DIR)/sb_acc2.$(OBJ): $(SBACC_DIR)/sb_acc2.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_acc2.c -o $(SY_DIR)/sb_acc2.$(OBJ)

$(SY_DIR)/sb_act1.$(OBJ): $(SBACC_DIR)/sb_act1.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_act1.c -o $(SY_DIR)/sb_act1.$(OBJ)

$(SY_DIR)/sb_act2.$(OBJ): $(SBACC_DIR)/sb_act2.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_act2.c -o $(SY_DIR)/sb_act2.$(OBJ)

$(SY_DIR)/sb_act3.$(OBJ): $(SBACC_DIR)/sb_act3.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_act3.c -o $(SY_DIR)/sb_act3.$(OBJ)

$(SY_DIR)/sb_act4.$(OBJ): $(SBACC_DIR)/sb_act4.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_act4.c -o $(SY_DIR)/sb_act4.$(OBJ)

$(SY_DIR)/sb_act5.$(OBJ): $(SBACC_DIR)/sb_act5.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_act5.c -o $(SY_DIR)/sb_act5.$(OBJ)

$(SY_DIR)/sb_act6.$(OBJ): $(SBACC_DIR)/sb_act6.c $(SYS_INC) $(SBACCALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_act6.c -o $(SY_DIR)/sb_act6.$(OBJ)

#inter-op test build--------------------------------------------------------
$(SY_DIR)/sb_ibdy.$(OBJ): $(SBACC_DIR)/sb_ibdy.c $(SYS_INC) $(SBINOPALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_ibdy.c -o $(SY_DIR)/sb_ibdy.$(OBJ)

$(SY_DIR)/sb_icfg.$(OBJ): $(SBACC_DIR)/sb_icfg.c $(SYS_INC) $(SBINOPALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_icfg.c -o $(SY_DIR)/sb_icfg.$(OBJ)

$(SY_DIR)/sb_iprim.$(OBJ): $(SBACC_DIR)/sb_iprim.c $(SYS_INC) $(SBINOPALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_iprim.c -o $(SY_DIR)/sb_iprim.$(OBJ)

$(SY_DIR)/sb_isupp.$(OBJ): $(SBACC_DIR)/sb_isupp.c $(SYS_INC) $(SBINOPALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/sb_isupp.c -o $(SY_DIR)/sb_isupp.$(OBJ)


# sample upper layer build-------------------------------------------------

$(SY_DIR)/it_bdy1.$(OBJ): $(SBACC_DIR)/it_bdy1.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/it_bdy1.c -o $(SY_DIR)/it_bdy1.$(OBJ)

$(SY_DIR)/it_ex_ms.$(OBJ): $(SBACC_DIR)/it_ex_ms.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/it_ex_ms.c -o $(SY_DIR)/it_ex_ms.$(OBJ)

$(SY_DIR)/it_ptli.$(OBJ): $(SBACC_DIR)/it_ptli.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SBACC_DIR)/it_ptli.c -o $(SY_DIR)/it_ptli.$(OBJ)

# sample lower layer build-------------------------------------------------

$(SY_DIR)/hi_bdy1.$(OBJ): $(LLDIR_USED)/hi_bdy1.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_bdy1.c -o $(SY_DIR)/hi_bdy1.$(OBJ)

$(SY_DIR)/hi_bdy2.$(OBJ): $(LLDIR_USED)/hi_bdy2.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_bdy2.c -o $(SY_DIR)/hi_bdy2.$(OBJ)

$(SY_DIR)/hi_bdy3.$(OBJ): $(LLDIR_USED)/hi_bdy3.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_bdy3.c -o $(SY_DIR)/hi_bdy3.$(OBJ)

$(SY_DIR)/hi_id.$(OBJ): $(LLDIR_USED)/hi_id.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_id.c -o $(SY_DIR)/hi_id.$(OBJ)

$(SY_DIR)/hi_ex_ms.$(OBJ): $(LLDIR_USED)/hi_ex_ms.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_ex_ms.c -o $(SY_DIR)/hi_ex_ms.$(OBJ)

$(SY_DIR)/hi_ptui.$(OBJ): $(LLDIR_USED)/hi_ptui.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_ptui.c -o $(SY_DIR)/hi_ptui.$(OBJ)

$(SY_DIR)/hi_ptmi.$(OBJ): $(LLDIR_USED)/hi_ptmi.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_ptmi.c -o $(SY_DIR)/hi_ptmi.$(OBJ)

$(SY_DIR)/hi_tst.$(OBJ): $(LLDIR_USED)/hi_tst.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_tst.c -o $(SY_DIR)/hi_tst.$(OBJ)

$(SY_DIR)/hi_ex_pt.$(OBJ): $(LLDIR_USED)/hi_ex_pt.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(LLDIR_USED)/hi_ex_pt.c -o $(SY_DIR)/hi_ex_pt.$(OBJ)

# product Id  file---------------------------------------------------

$(SY_DIR)/sb_id.$(OBJ): $(SB_DIR)/sb_id.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_id.c -o $(SY_DIR)/sb_id.$(OBJ)

# product build---------------------------------------------------

$(SY_DIR)/sb_bdy1.$(OBJ): $(SB_DIR)/sb_bdy1.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_bdy1.c -o $(SY_DIR)/sb_bdy1.$(OBJ)

$(SY_DIR)/sb_bdy2.$(OBJ): $(SB_DIR)/sb_bdy2.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_bdy2.c -o $(SY_DIR)/sb_bdy2.$(OBJ)

$(SY_DIR)/sb_bdy3.$(OBJ): $(SB_DIR)/sb_bdy3.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_bdy3.c -o $(SY_DIR)/sb_bdy3.$(OBJ)

$(SY_DIR)/sb_bdy4.$(OBJ): $(SB_DIR)/sb_bdy4.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_bdy4.c -o $(SY_DIR)/sb_bdy4.$(OBJ)

$(SY_DIR)/sb_ptmi.$(OBJ): $(SB_DIR)/sb_ptmi.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_ptmi.c -o $(SY_DIR)/sb_ptmi.$(OBJ)

$(SY_DIR)/sb_ptui.$(OBJ): $(SB_DIR)/sb_ptui.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_ptui.c -o $(SY_DIR)/sb_ptui.$(OBJ)

$(SY_DIR)/sb_ptli.$(OBJ): $(SB_DIR)/sb_ptli.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_ptli.c -o $(SY_DIR)/sb_ptli.$(OBJ)

$(SY_DIR)/sb_ex_ms.$(OBJ): $(SB_DIR)/sb_ex_ms.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_ex_ms.c -o $(SY_DIR)/sb_ex_ms.$(OBJ)

$(SY_DIR)/sb_port.$(OBJ): $(SB_DIR)/sb_port.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_port.c -o $(SY_DIR)/sb_port.$(OBJ)

$(SY_DIR)/sb_mtu.$(OBJ): $(SB_DIR)/sb_mtu.c $(SYS_INC) $(CM_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/sb_mtu.c -o $(SY_DIR)/sb_mtu.$(OBJ)

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

$(SY_DIR)/cm_tpt.$(OBJ): $(CM_DIR)/cm_tpt.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_tpt.c -o $(SY_DIR)/cm_tpt.$(OBJ)

$(SY_DIR)/cm_dns.$(OBJ): $(CM_DIR)/cm_dns.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_dns.c -o $(SY_DIR)/cm_dns.$(OBJ)

$(SY_DIR)/cm_inet.$(OBJ): $(CM_DIR)/cm_inet.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/cm_inet.c -o $(SY_DIR)/cm_inet.$(OBJ)

$(SY_DIR)/cm_os.$(OBJ): $(CM_DIR)/cm_os.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CPRDOPTS) -D_SVID_SOURCE $(CM_DIR)/cm_os.c -o $(SY_DIR)/cm_os.$(OBJ)

$(SY_DIR)/hit.$(OBJ): $(CM_DIR)/hit.c $(SYS_INC) $(HIT_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/hit.c -o $(SY_DIR)/hit.$(OBJ)

$(SY_DIR)/sct.$(OBJ): $(CM_DIR)/sct.c $(SYS_INC) $(SCT_INC)
	$(CC) -c $(CPRDOPTS) $(CM_DIR)/sct.c -o $(SY_DIR)/sct.$(OBJ)

$(SY_DIR)/lsb.$(OBJ): $(SB_DIR)/lsb.c $(SYS_INC) $(SBALL_INC)
	$(CC) -c $(CPRDOPTS) $(SB_DIR)/lsb.c -o $(SY_DIR)/lsb.$(OBJ)

# stack manager build-------------------------------------------

$(SY_DIR)/sm_bdy1.$(OBJ): $(SM_DIR)/sm_bdy1.c $(SYS_INC) $(LSB_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/sm_bdy1.c -o $(SY_DIR)/sm_bdy1.$(OBJ)

$(SY_DIR)/sm_ex_ms.$(OBJ): $(SM_DIR)/sm_ex_ms.c $(SYS_INC) $(LSB_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/sm_ex_ms.c -o $(SY_DIR)/sm_ex_ms.$(OBJ)

$(SY_DIR)/smsbbdy1.$(OBJ): $(SBSM_DIR)/smsbbdy1.c $(SYS_INC) $(LSB_INC)
	$(CC) -c $(CPRDOPTS) $(SBSM_DIR)/smsbbdy1.c -o $(SY_DIR)/smsbbdy1.$(OBJ)

$(SY_DIR)/smsbexms.$(OBJ): $(SBSM_DIR)/smsbexms.c $(SYS_INC) $(LSB_INC)
	$(CC) -c $(CPRDOPTS) $(SBSM_DIR)/smsbexms.c -o $(SY_DIR)/smsbexms.$(OBJ)

$(SY_DIR)/smsbptmi.$(OBJ): $(SBSM_DIR)/smsbptmi.c $(SYS_INC) $(LSB_INC)
	$(CC) -c $(CPRDOPTS) $(SBSM_DIR)/smsbptmi.c -o $(SY_DIR)/smsbptmi.$(OBJ)

#stack manager objects for HI----------------------------------------------
$(SY_DIR)/smhibdy1.$(OBJ): $(SM_DIR)/smhibdy1.c $(SYS_INC) $(LMS_INC) $(SM_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/smhibdy1.c

$(SY_DIR)/smhiexms.$(OBJ): $(SM_DIR)/smhiexms.c $(SYS_INC) $(LMS_INC) $(SM_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/smhiexms.c

$(SY_DIR)/smhiptmi.$(OBJ): $(SM_DIR)/smhiptmi.c $(SYS_INC) $(LMS_INC) $(SM_INC)
	$(CC) -c $(CSMOPTS) $(SM_DIR)/smhiptmi.c

# OS - common build-------------------------------------------------------
$(SY_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_mem.c -o $(SY_DIR)/ss_mem.$(OBJ)

$(SY_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_queue.c -o $(SY_DIR)/ss_queue.$(OBJ)

$(SY_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_timer.c -o $(SY_DIR)/ss_timer.$(OBJ)

$(SY_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_drvr.c -o $(SY_DIR)/ss_drvr.$(OBJ)

$(SY_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_msg.c -o $(SY_DIR)/ss_msg.$(OBJ)

$(SY_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_rtr.c -o $(SY_DIR)/ss_rtr.$(OBJ)

$(SY_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_gen.c -o $(SY_DIR)/ss_gen.$(OBJ)

$(SY_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_pack.c -o $(SY_DIR)/ss_pack.$(OBJ)

$(SY_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_strm.c -o $(SY_DIR)/ss_strm.$(OBJ)

$(SY_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_task.c -o $(SY_DIR)/ss_task.$(OBJ)

$(SY_DIR)/ss_id.$(OBJ): $(SS_DIR)/ss_id.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/ss_id.c -o $(SY_DIR)/ss_id.$(OBJ)

$(SY_DIR)/cm_mem.$(OBJ): $(SS_DIR)/cm_mem.c $(SYS_INC)
	$(CC) -c $(CSSOPTS) $(SS_DIR)/cm_mem.c -o $(SY_DIR)/cm_mem.$(OBJ)

# MTSS-Solaris build-------------------------------------------------------
$(SY_DIR)/mt_ss.$(OBJ): $(OS_DIR)/mt_ss.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/mt_ss.c -o $(SY_DIR)/mt_ss.$(OBJ)

$(SY_DIR)/mt_id.$(OBJ): $(OS_DIR)/mt_id.c $(SYS_INC) $(SS_INC) $(MT_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/mt_id.c -o $(SY_DIR)/mt_id.$(OBJ)

# MTSS-Windows NT build-------------------------------------------------------
$(SY_DIR)/ns_gen.$(OBJ): $(OS_DIR)/ns_gen.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/ns_gen.c

$(SY_DIR)/smnsbdy1.$(OBJ): $(OS_DIR)/smnsbdy1.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(CSMOPTS) $(COSOPTS) $(OS_DIR)/smnsbdy1.c

$(SY_DIR)/smnsexms.$(OBJ): $(OS_DIR)/smnsexms.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(CSMOPTS) $(COSOPTS) $(OS_DIR)/smnsexms.c

$(SY_DIR)/lns.$(OBJ): $(CM_DIR)/lns.c $(SYS_INC) $(SS_INC) $(NS_INC)
	$(CC) -c $(COSOPTS) $(CM_DIR)/lns.c

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

# VxWorks build-------------------------------------------------------
$(SY_DIR)/vw_ss.$(OBJ): $(OS_DIR)/vw_ss.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_ss.c -o $(SY_DIR)/vw_ss.$(OBJ)

$(SY_DIR)/vw_id.$(OBJ): $(OS_DIR)/vw_id.c $(SYS_INC) $(SS_INC) $(VW_INC)
	$(CC) -c $(COSOPTS) $(OS_DIR)/vw_id.c -o $(SY_DIR)/vw_id.$(OBJ)

# mos files build-------------------------------------------------------

$(SY_DIR)/ms_bdy1.$(OBJ): $(OS_DIR)/ms_bdy1.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_bdy1.c

$(SY_DIR)/ms_bdy2.$(OBJ): $(OS_DIR)/ms_bdy2.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_bdy2.c

$(SY_DIR)/ms_bdy5.$(OBJ): $(OS_DIR)/ms_bdy5.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_bdy5.c

$(SY_DIR)/ms_ptmi.$(OBJ): $(OS_DIR)/ms_ptmi.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_ptmi.c

$(SY_DIR)/ms_id.$(OBJ): $(OS_DIR)/ms_id.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_id.c

$(SY_DIR)/ms_ex_ms.$(OBJ): $(OS_DIR)/ms_ex_ms.c $(SYS_INC) $(MS_INC)
	$(CC) -c $(CMSOPTS) $(OS_DIR)/ms_ex_ms.c

$(SY_DIR)/cm_ss.$(OBJ): $(CM_DIR)/cm_ss.c $(SYS_INC) $(CM_INC)
	$(CC) -c $(CMSOPTS) $(CM_DIR)/cm_ss.c


#********************************************************************30**
#
#        End of file:     sb.mak@@/main/4 - Thu Jul 26 07:53:18 2007
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
#   /main/2   ---     wvdl  1. initial release.
#   /main/2   ---     sb    1. Added the interop files for SCTP
#   /main/4   ---   rsr/ag  1. Updated for Release of 1.3
#********************************************************************91*/

# DO NOT DELETE THIS LINE -- make depend depends on it.
