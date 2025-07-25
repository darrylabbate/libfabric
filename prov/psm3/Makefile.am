#
# Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2017-2024 Intel Corporation, Inc. All right reserved.
# Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
# (C) Copyright 2020 Hewlett Packard Enterprise Development LP
#
# Makefile.am for libpsm3-fi
EXTRA_DIST =

AM_CPPFLAGS = \
	-I$(srcdir)/inc \
	-D_GNU_SOURCE -D__USE_XOPEN2K8 \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DRDMADIR=\"@rdmadir@\" \
	-DPROVDLDIR=\"$(pkglibdir)\" \
	-I$(srcdir)/psm3

noinst_LTLIBRARIES =
libfabric_pkglibdir = $(libdir)/libfabric
libfabric_pkglib_LTLIBRARIES =
noinst_PROGRAMS =

if EMBEDDED
noinst_LTLIBRARIES += src/libpsm3-fi.la
else
libfabric_pkglib_LTLIBRARIES += src/libpsm3-fi.la
endif

ACLOCAL_AMFLAGS = -I config
AM_CFLAGS = -Wall

if HAVE_LD_VERSION_SCRIPT
	libpsm3_fi_version_script = -Wl,--version-script=$(builddir)/libpsm3-fi.map
else !HAVE_LD_VERSION_SCRIPT
	libpsm3_fi_version_script =
endif !HAVE_LD_VERSION_SCRIPT

# rdmaincludedir = $(includedir)/rdma
# providersincludedir = $(rdmaincludedir)/providers

# rdmainclude_HEADERS =
# providersinclude_HEADERS =

# internal utility functions shared by in-tree providers:
common_srcs = \
	shared/hmem.c \
	shared/hmem_rocr.c \
	shared/hmem_cuda.c \
	shared/hmem_cuda_gdrcopy.c \
	shared/hmem_ze.c \
	shared/hmem_neuron.c \
	shared/hmem_synapseai.c \
	shared/hmem_ipc_cache.c \
	shared/xpmem.c \
	shared/xpmem_cache.c \
	shared/common.c \
	shared/enosys.c \
	shared/rbtree.c \
	shared/tree.c \
	shared/fasthash.c \
	shared/indexer.c \
	shared/mem.c \
	shared/iov.c \
	shared/ofi_str.c \
	util/src/util_atomic.c \
	util/src/util_attr.c \
	util/src/util_av.c \
	util/src/util_buf.c \
	util/src/util_cq.c \
	util/src/util_cntr.c \
	util/src/util_domain.c \
	util/src/util_ep.c \
	util/src/util_eq.c \
	util/src/util_fabric.c \
	util/src/util_main.c \
	util/src/util_mem_hooks.c \
	util/src/util_mem_monitor.c \
	util/src/util_mr_cache.c \
	util/src/util_mr_map.c \
	util/src/util_ns.c \
	util/src/util_pep.c \
	util/src/util_poll.c \
	util/src/util_profile.c \
	util/src/util_srx.c \
	util/src/util_wait.c \
	util/src/rxm_av.c \
	util/src/cuda_mem_monitor.c \
	util/src/cuda_ipc_monitor.c \
	util/src/rocr_mem_monitor.c \
	util/src/rocr_ipc_monitor.c \
	util/src/ze_mem_monitor.c \
	util/src/ze_ipc_monitor.c \
	util/src/xpmem_monitor.c \
	util/src/kdreg2_mem_monitor.c \
	util/src/uffd_mem_monitor.c \
	util/src/import_mem_monitor.c \
	shared/fabric.c \
	shared/fi_tostr.c \
	shared/perf.c \
	shared/log.c \
	shared/var.c \
	shared/abi_1_0.c


if MACOS
common_srcs += shared/osx/osd.c
common_srcs += shared/unix/osd.c
common_srcs += inc/osx/osd.h
common_srcs += inc/unix/osd.h
endif

if FREEBSD
common_srcs += shared/unix/osd.c
common_srcs += inc/freebsd/osd.h
common_srcs += inc/unix/osd.h
endif

if LINUX
common_srcs += shared/unix/osd.c
common_srcs += shared/linux/osd.c
if HAVE_LINUX_PERF_RDPMC
common_srcs += shared/linux/rdpmc.c
endif
common_srcs += inc/linux/rdpmc.h
common_srcs += inc/linux/osd.h
common_srcs += inc/unix/osd.h
endif

# ensure dl-built providers link back to libfabric
# linkback = src/libfabric.la

bin_SCRIPTS =

nodist_src_libpsm3_fi_la_SOURCES =
src_libpsm3_fi_la_SOURCES = \
	inc/ofi_hmem.h \
	inc/ofi_cma.h \
	inc/ofi_xpmem.h \
	inc/ofi.h \
	inc/ofi_abi.h \
	inc/ofi_atom.h \
	inc/ofi_enosys.h \
	inc/ofi_file.h \
	inc/ofi_hook.h \
	inc/ofi_indexer.h \
	inc/ofi_iov.h \
	inc/ofi_list.h \
	inc/ofi_bitmask.h \
	inc/ofi_str.h \
	inc/ofi_lock.h \
	inc/ofi_mem.h \
	inc/ofi_osd.h \
	inc/ofi_proto.h \
	inc/ofi_recvwin.h \
	inc/ofi_rbuf.h \
	inc/ofi_shm_p2p.h \
	inc/ofi_signal.h \
	inc/ofi_epoll.h \
	inc/ofi_tree.h \
	inc/ofi_util.h \
	inc/ofi_atomic.h \
	inc/ofi_atomic_queue.h \
	inc/ofi_mr.h \
	inc/ofi_net.h \
	inc/ofi_perf.h \
	inc/ofi_coll.h \
	inc/ofi_mb.h \
	inc/fasthash.h \
	inc/rbtree.h \
	inc/uthash.h \
	inc/ofi_prov.h \
	inc/ofi_profile.h \
	inc/rdma/providers/fi_log.h \
	inc/rdma/providers/fi_prov.h \
	inc/rdma/providers/fi_peer.h \
	inc/rdma/fabric.h \
	inc/rdma/fi_atomic.h \
	inc/rdma/fi_cm.h \
	inc/rdma/fi_collective.h \
	inc/rdma/fi_domain.h \
	inc/rdma/fi_eq.h \
	inc/rdma/fi_ext.h \
	inc/rdma/fi_rma.h \
	inc/rdma/fi_endpoint.h \
	inc/rdma/fi_errno.h \
	inc/rdma/fi_tagged.h \
	inc/rdma/fi_trigger.h \
	inc/rdma/fi_profile.h \
	src/psmx3.h \
	src/psmx3_am.c \
	src/psmx3_atomic.c \
	src/psmx3_attr.c \
	src/psmx3_av.c \
	src/psmx3_cm.c \
	src/psmx3_cntr.c \
	src/psmx3_cq.c \
	src/psmx3_domain.c \
	src/psmx3_ep.c \
	src/psmx3_fabric.c \
	src/psmx3_init.c \
	src/psmx3_mr.c \
	src/psmx3_msg.c \
	src/psmx3_rma.c \
	src/psmx3_tagged.c \
	src/psmx3_trigger.h \
	src/psmx3_trx_ctxt.c \
	src/psmx3_util.c \
	src/psmx3_wait.c \
	src/version.h \
	$(common_srcs)

src_libpsm3_fi_la_CPPFLAGS = $(AM_CPPFLAGS)
src_libpsm3_fi_la_DEPENDENCIES = libpsm3-fi.map
src_libpsm3_fi_la_LDFLAGS =
src_libpsm3_fi_la_LIBADD =

src_libpsm3_fi_la_LDFLAGS += \
	-Wl,--exclude-libs,ALL \
	$(libpsm3_fi_version_script)

chksum_srcs = $(src_libpsm3_fi_la_SOURCES)
if HAVE_PSM3_SRC
nodist_src_libpsm3_fi_la_SOURCES += \
	src/psm3_revision.c \
	src/psm3_src_chksum.h

include psm3/Makefile.include
src_libpsm3_fi_la_LIBADD += psm3/libpsm3i.la
src_libpsm3_fi_la_DEPENDENCIES += \
	src/psm3_src_chksum.h \
	psm3/libpsm3i.la

else !HAVE_PSM3_SRC
src_libpsm3_fi_la_LDFLAGS += -lpsm2
endif !HAVE_PSM3_SRC

if !EMBEDDED
src_libpsm3_fi_la_LDFLAGS += -version-info 28:0:27
endif

prov_install_man_pages = man/man7/fi_psm3.7

prov_dist_man_pages = man/man7/fi_psm3.7

man_MANS = $(prov_install_man_pages)

EXTRA_DIST += \
	autogen.sh \
	libpsm3-fi.spec.in \
	config/distscript.pl \
	$(prov_dist_man_pages) \
	VERSION

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpsm3-fi.pc

chksum_srcs += $(EXTRA_DIST) $(pkgconfig_DATA)

BUILT_SOURCES = src/psm3_src_chksum.h
CLEANFILES = src/psm3_src_chksum.h
DATE_FMT = +%Y-%m-%dT%H:%M:%S
src/psm3_src_chksum.h: Makefile $(chksum_srcs)
	$(AM_V_GEN) chksum=`for file in $(chksum_srcs); do cat $(top_srcdir)/$$file; done | sha1sum | cut -d' ' -f 1`; \
	if ! grep -q $$chksum src/psm3_src_chksum.h 2>/dev/null; then \
		echo "#define PSMX3_SRC_CHECKSUM \"$$chksum\"" > src/psm3_src_chksum.h; \
		echo "#define PSMX3_BUILD_TIMESTAMP \"`if test -z "$(SOURCE_DATE_EPOCH)" ; then date "$(DATE_FMT)" ; else date -u -d "@$(SOURCE_DATE_EPOCH)" "$(DATE_FMT)" 2>/dev/null || date -u -r "$(SOURCE_DATE_EPOCH)" "$(DATE_FMT)" 2>/dev/null || date -u "$(DATE_FMT)" ; fi`\"" >> src/psm3_src_chksum.h; \
	fi

nroff:
	@for file in $(prov_install_man_pages); do \
		source=`echo $$file | sed -e 's@/man[0-9]@@'`; \
		perl $(top_srcdir)/config/md2nroff.pl --source=$(top_srcdir)/$$source.md; \
	done

dist-hook: libpsm3-fi.spec
	cp libpsm3-fi.spec $(distdir)
	perl $(top_srcdir)/config/distscript.pl "$(distdir)" "$(PACKAGE_VERSION)"
	cp -r $(top_srcdir)/debian.template $(distdir)/debian
	$(top_srcdir)/config/finalize-deb "$(distdir)/debian" "$(PACKAGE_NAME)" "$(PACKAGE_VERSION)" "$(RPM_RELEASE)"
