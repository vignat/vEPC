#!/bin/sh

set -e

make -f vb.mak -kj6 BLDENV=lnx_acc acc || true

ld -m elf_i386 -r -o ./libvb_lnx32sz.a  ./sz_ex_ms.o  ./sz_id.o ./sz_ptmi.o ./sz_ptui.o ./sz_ui.o ./sz_li.o ./sz_mi.o ./sz_ndb.o ./sz_smlimme.o ./sz_smuimme.o ./sz_mgmt.o ./sz_utils.o ./sz_msgfn.o ./sz_sm.o ./sz_db.o ./sz_tmr.o ./sz_ptli.o ./cm_gen.o ./cm_tkns.o ./cm_lib.o ./cm_hash.o ./cm_llist.o ./cm_dns.o ./cm_inet.o ./cm_pasn1.o ./cm_pasn2.o ./cm_pasn3.o ./cm_pasn4.o ./cm_mblk.o ./sct.o ./lsb.o ./cm_bdy5.o ./cm_tpt.o ./szt.o ./szt_pk.o ./szt_unpk.o ./lsz.o ./hit.o ./lhi.o ./smhiexms.o ./smhiptmi.o ./smsbexms.o ./smsbptmi.o ./smszexms.o ./smszptmi.o ./sb_bdy1.o ./sb_bdy2.o ./sb_bdy3.o ./sb_bdy4.o ./sb_id.o ./sb_ptmi.o ./sb_ptui.o ./sb_ptli.o ./sb_ex_ms.o ./sb_port.o ./sb_mtu.o ./hi_bdy1.o ./hi_bdy2.o ./hi_bdy3.o ./hi_ex_ms.o ./hi_ptmi.o ./hi_ptui.o ./hi_id.o
ld -m elf_i386 -r -o ./libvb_lnx32eg.a  ./leg.o ./egt.o ./smegptmi.o ./eg_li.o ./eg_ptli.o ./eg_tpt.o ./eg_ui.o ./eg_ptui.o ./eg_id.o ./eg_ptmi.o ./eg_cpm.o ./eg_tunn.o ./eg_util.o ./eg_ex_ms.o ./eg_mi.o ./eg_dbutl.o ./eg_dbs3.o ./eg_dbs4.o ./eg_dbs5.o ./eg_dbs10.o ./eg_dbs11.o ./eg_dbsm.o ./eg_dbsn.o ./eg_dbsi.o ./eg_dbs16.o ./eg_edm.o ./eg_tmr.o ./egt_util.o 

make -f vb.mak BLDENV=lnx_acc libvb_lnx32vbsec.a
make -f vb.mak BLDENV=lnx_acc acc

echo
echo Build successful.
