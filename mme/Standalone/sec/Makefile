#CC=arm-none-linux-gnueabi-g++
CC=gcc

CPRDOPTS=-DSS -DCMFILE_REORG_1 -DCMFILE_REORG_2 -DxSS_SINGLE_THREADED \
			-DLONG_MSG -DxSS_MULTIPLE_PROCS -DSLES9_PLUS -DSS_MT \
			-D__EXTENSIONS__ -DDEBUGNOEXIT \
         -USS_M_PROTO_REGION -USS_MULTICORE_SUPPORT -USSI_MEM_CHNG -DSS_MT_TMR \
         -DxCM_MT_HASH_BIN -USS_MEM_LEAK_STS -USS_MEM_LEAK_FREE_TRACE

ifeq ($(MACHINE), BIT64)
LNXCCOPTS=  -g3 -DALIGN_64BIT -DBIT_64 -DSUNOS \
		-DSS_LINUX -DANSI -D_GNU_SOURCE \
		-ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
      -Wno-strict-aliasing -pedantic -Wimplicit -Wunused
else
LNXCCOPTS= -g3 -DSUNOS -DSS_LINUX -DANSI -D_GNU_SOURCE \
	-ansi -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
   -Wstrict-prototypes -Wmissing-prototypes -pedantic \
   -Wimplicit -Wunused
endif

INC_DIR=\
	-I ./ \
	-I ../ 

LIBNAME=libepcsec
OBJ=.
LIB_DIR=../

ifeq ($(MACHINE), BIT64)
# default portable for 64 bit linux cc environment
BINLIB=$(LIB_DIR)/libvb_lnx64vbsec.a
else
# default portable for 32 bit linux cc environment
BINLIB=$(LIB_DIR)/libvb_lnx32vbsec.a
endif

SOURCE=vb_hss_auth.o \
       vb_aes_mode.o \
		 vb_alg_intf.o \
       auth_alg.o \
       endianness.o \
       hmac.o \
       hmacsha256.o \
       mp.o \
       sha256.o \
       snow_3g.o \
       snow3g_f8f9.o \
       aes.o

OBJSRC=$(SOURCE:%=$(OBJ)/%)

$(BINLIB) : $(OBJSRC)
	 ar rv $@ $?
#	 arm-none-linux-gnueabi-ar rv $@ $?

$(OBJ)/%.o : %.c $(INC_FILES)
	$(CC) -c $(LNXCCOPTS) $(CPRDOPTS) $(INC_DIR) -o $@ $?

clean:
	rm -f $(OBJ)/*.o $(LIB_DIR)/*.a 


