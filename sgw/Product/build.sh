#!/bin/sh

set -e

make -kj6 -f qo.mak BLDENV=lnx_acc acc || true

ld -m elf_i386 -r -o libqo_eg.a  ./eg_li.o ./eg_ptli.o ./eg_tpt.o ./eg_ui.o ./eg_ptui.o ./eg_id.o ./eg_ptmi.o ./eg_cpm.o ./eg_tunn.o ./eg_util.o ./eg_ex_ms.o ./eg_mi.o ./eg_dbutl.o ./eg_dbs3.o ./eg_dbs4.o ./eg_dbs5.o ./eg_dbs10.o ./eg_dbs11.o ./eg_dbsm.o ./eg_dbsn.o ./eg_dbsi.o ./eg_dbs16.o ./eg_edm.o ./eg_tmr.o ./leg.o ./egt.o

make -f qo.mak BLDENV=lnx_acc acc

echo
echo Build successful.
