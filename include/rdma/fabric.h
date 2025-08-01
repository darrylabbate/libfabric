/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FABRIC_H
#define FABRIC_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <rdma/fi_errno.h>

#ifdef __GNUC__
#define FI_DEPRECATED_FUNC __attribute__((deprecated))
#define FI_DEPRECATED_FIELD __attribute__((deprecated))
#define FI_FORMAT_PRINTF(string, first) \
	__attribute__ ((__format__ (__printf__, (string), (first))))
#elif defined(_MSC_VER)
#define FI_DEPRECATED_FUNC __declspec(deprecated)
#define FI_DEPRECATED_FIELD
#define FI_FORMAT_PRINTF(string, first)
#else
#define FI_DEPRECATED_FUNC
#define FI_DEPRECATED_FIELD
#define FI_FORMAT_PRINTF(string, first)
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define EXTERNALLY_VISIBLE externally_visible
#else
#define EXTERNALLY_VISIBLE
#endif

#if defined(_WIN32)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define FI_MAJOR_VERSION 2
#define FI_MINOR_VERSION 2
#define FI_REVISION_VERSION 0

/* Removing these breaks the build for some apps.
 * The use of FI_NAME_MAX is undefined.
 * FI_ATOMIC_OP_LAST and FI_DATATYPE_LAST values cannot change
 * (such as inserting new enum values that they are intended to be the
 * last of) without breaking apps that recompile.  So, they are hard-coded
 * here.
 */
enum {
	FI_NAME_MAX = 64,
	FI_ATOMIC_OP_LAST = 19,
	FI_DATATYPE_LAST = 14, /* not actual last datatype */
};

#define FI_VERSION(major, minor) (((major) << 16) | (minor))
#define FI_MAJOR(version)	(version >> 16)
#define FI_MINOR(version)	(version & 0xFFFF)
#define FI_VERSION_GE(v1, v2)	(v1 >= v2)
#define FI_VERSION_LT(v1, v2)	(v1 < v2)

uint32_t fi_version(void);

struct fid;
struct fid_fabric;
struct fid_domain;
struct fid_av;
struct fid_wait;
struct fid_poll;
struct fid_eq;
struct fid_cq;
struct fid_cntr;
struct fid_ep;
struct fid_pep;
struct fid_stx;
struct fid_mr;
struct fid_nic;

typedef struct fid *fid_t;

/*
 * Flags
 * The 64-bit flag field is used as follows:
 * 1-grow up    common (usable with multiple operations)
 * 59-grow down operation specific (used for single call/class)
 * 60 - 63      provider specific
 */

#define FI_MSG			(1ULL << 1)
#define FI_RMA			(1ULL << 2)
#define FI_TAGGED		(1ULL << 3)
#define FI_ATOMIC		(1ULL << 4)
#define FI_ATOMICS		FI_ATOMIC
#define FI_MULTICAST		(1ULL << 5)
#define FI_COLLECTIVE		(1ULL << 6)

#define FI_READ			(1ULL << 8)
#define FI_WRITE		(1ULL << 9)
#define FI_RECV			(1ULL << 10)
#define FI_SEND			(1ULL << 11)
#define FI_TRANSMIT		FI_SEND
#define FI_REMOTE_READ		(1ULL << 12)
#define FI_REMOTE_WRITE		(1ULL << 13)

#define FI_MULTI_RECV		(1ULL << 16)
#define FI_REMOTE_CQ_DATA	(1ULL << 17)
#define FI_MORE			(1ULL << 18)
#define FI_PEEK			(1ULL << 19)
#define FI_TRIGGER		(1ULL << 20)
#define FI_FENCE		(1ULL << 21)
/* #define FI_PRIORITY		(1ULL << 22) */

#define FI_COMPLETION		(1ULL << 24)
#define FI_EVENT		FI_COMPLETION
#define FI_INJECT		(1ULL << 25)
#define FI_INJECT_COMPLETE	(1ULL << 26)
#define FI_TRANSMIT_COMPLETE	(1ULL << 27)
#define FI_DELIVERY_COMPLETE	(1ULL << 28)
#define FI_AFFINITY		(1ULL << 29)
#define FI_COMMIT_COMPLETE	(1ULL << 30)
#define FI_MATCH_COMPLETE	(1ULL << 31)

#define FI_RESCAN		(1ULL << 35)
#define FI_PEER_TRANSFER	(1ULL << 36)
/* #define FI_MR_DMABUF		(1ULL << 40) */
#define FI_AV_USER_ID		(1ULL << 41)
#define FI_FIREWALL_ADDR	(1ULL << 42)
#define FI_PEER			(1ULL << 43)
/* #define FI_XPU_TRIGGER		(1ULL << 44) */

#define FI_TAGGED_DIRECTED_RECV	(1ULL << 45)
#define FI_TAGGED_MULTI_RECV	(1ULL << 46)
#define FI_HMEM			(1ULL << 47)
#define FI_EXACT_DIRECTED_RECV	(1ULL << 48)
#define FI_RMA_PMEM		(1ULL << 49)
#define FI_SOURCE_ERR		(1ULL << 50)
#define FI_LOCAL_COMM		(1ULL << 51)
#define FI_REMOTE_COMM		(1ULL << 52)
#define FI_SHARED_AV		(1ULL << 53)
#define FI_PROV_ATTR_ONLY	(1ULL << 54)
#define FI_NUMERICHOST		(1ULL << 55)
#define FI_RMA_EVENT		(1ULL << 56)
#define FI_SOURCE		(1ULL << 57)
#define FI_NAMED_RX_CTX		(1ULL << 58)
#define FI_DIRECTED_RECV	(1ULL << 59)


/* Tagged messages, buffered receives, CQ flags */
#define FI_CLAIM		(1ULL << 59)
#define FI_DISCARD		(1ULL << 58)
#define FI_AUTH_KEY		(1ULL << 57)

struct fi_ioc {
	void			*addr;
	size_t			count;
};

/*
 * Format for transport addresses to insert into address vectors
 */
enum {
	FI_FORMAT_UNSPEC,	/* void * */
	FI_SOCKADDR,		/* struct sockaddr */
	FI_SOCKADDR_IN,		/* struct sockaddr_in */
	FI_SOCKADDR_IN6,	/* struct sockaddr_in6 */
	FI_SOCKADDR_IB,		/* struct sockaddr_ib */
	/*  PSMX provider is deprecated.
	 *  We will keep this value in order to save binary compatibility.
	 */
	FI_ADDR_PSMX,		/* uint64_t */
	FI_ADDR_GNI,
	FI_ADDR_BGQ,
	FI_ADDR_MLX,
	FI_ADDR_STR,		/* formatted char * */
	FI_ADDR_PSMX2,		/* uint64_t[2] */
	FI_ADDR_IB_UD,		/* uint64_t[4] */
	FI_ADDR_EFA,
	FI_ADDR_PSMX3,		/* uint64_t[4] */
	FI_ADDR_OPX,
	FI_ADDR_CXI,
	FI_ADDR_UCX,

	FI_SOCKADDR_IP,		/* FI_SOCKADDR_IN and FI_SOCKADDR_IN6 */
};

#define FI_ADDR_UNSPEC		((uint64_t) -1)
#define FI_ADDR_NOTAVAIL	((uint64_t) -1)
#define FI_KEY_NOTAVAIL		((uint64_t) -1)
#define FI_SHARED_CONTEXT	SIZE_MAX
#define FI_AV_AUTH_KEY		SIZE_MAX
typedef uint64_t		fi_addr_t;

enum fi_av_type {
	FI_AV_UNSPEC,
	FI_AV_MAP,
	FI_AV_TABLE
};

#define FI_MR_UNSPEC		_Pragma("GCC warning \"'FI_MR_UNSPEC' is deprecated\"")		(0)
#define FI_MR_BASIC		_Pragma("GCC warning \"'FI_MR_BASIC' is deprecated\"")		(1 << 0)
#define FI_MR_SCALABLE		_Pragma("GCC warning \"'FI_MR_SCALABLE' is deprecated\"")	(1 << 1)

#define FI_MR_LOCAL		(1 << 2)
#define FI_MR_RAW		(1 << 3)
#define FI_MR_VIRT_ADDR		(1 << 4)
#define FI_MR_ALLOCATED		(1 << 5)
#define FI_MR_PROV_KEY		(1 << 6)
#define FI_MR_MMU_NOTIFY	(1 << 7)
#define FI_MR_RMA_EVENT		(1 << 8)
#define FI_MR_ENDPOINT		(1 << 9)
#define FI_MR_HMEM		(1 << 10)
#define FI_MR_COLLECTIVE	(1 << 11)

enum fi_progress {
	FI_PROGRESS_UNSPEC,
	FI_PROGRESS_AUTO,
	FI_PROGRESS_MANUAL,
	FI_PROGRESS_CONTROL_UNIFIED,
};

enum fi_threading {
	FI_THREAD_UNSPEC,
	FI_THREAD_SAFE,
	FI_THREAD_FID,
	FI_THREAD_DOMAIN,
	FI_THREAD_COMPLETION,
	FI_THREAD_ENDPOINT,
};

enum fi_resource_mgmt {
	FI_RM_UNSPEC,
	FI_RM_DISABLED,
	FI_RM_ENABLED
};

#define FI_ORDER_NONE		_Pragma("GCC warning \"'FI_ORDER_NONE' is deprecated\"")	0ULL
#define FI_ORDER_RAR		(1ULL << 0)
#define FI_ORDER_RAW		(1ULL << 1)
#define FI_ORDER_RAS		(1ULL << 2)
#define FI_ORDER_WAR		(1ULL << 3)
#define FI_ORDER_WAW		(1ULL << 4)
#define FI_ORDER_WAS		(1ULL << 5)
#define FI_ORDER_SAR		(1ULL << 6)
#define FI_ORDER_SAW		(1ULL << 7)
#define FI_ORDER_SAS		(1ULL << 8)
#define FI_ORDER_STRICT		_Pragma("GCC warning \"'FI_ORDER_STRICT' is deprecated\"")	0x1FF

#define FI_ORDER_RMA_RAR	(1ULL << 32)
#define FI_ORDER_RMA_RAW	(1ULL << 33)
#define FI_ORDER_RMA_WAR	(1ULL << 34)
#define FI_ORDER_RMA_WAW	(1ULL << 35)
#define FI_ORDER_ATOMIC_RAR	(1ULL << 36)
#define FI_ORDER_ATOMIC_RAW	(1ULL << 37)
#define FI_ORDER_ATOMIC_WAR	(1ULL << 38)
#define FI_ORDER_ATOMIC_WAW	(1ULL << 39)

#define FI_ORDER_DATA		_Pragma("GCC warning \"'FI_ORDER_DATA' is deprecated\"")	(1ULL << 16)

enum fi_ep_type {
	FI_EP_UNSPEC,
	FI_EP_MSG,
	FI_EP_DGRAM,
	FI_EP_RDM,
	/* FI_EP_SOCK_STREAM, */
	/* FI_EP_SOCK_DGRAM, */
};

/* Endpoint protocol
 * If two providers support the same protocol, then they shall interoperate
 * when the protocol capabilities match.
 */
enum {
	FI_PROTO_UNSPEC,
	FI_PROTO_RDMA_CM_IB_RC,
	FI_PROTO_IWARP,
	FI_PROTO_IB_UD,
	/*  PSMX provider is deprecated.
	 *  We will keep this value in order to save binary compatibility.
	 */
	FI_PROTO_PSMX,
	FI_PROTO_UDP,
	FI_PROTO_SOCK_TCP,
	/*  MXM provider is deprecated.
	 *  We will keep this value in order to save binary compatibility.
	 */
	FI_PROTO_MXM,
	FI_PROTO_IWARP_RDM,
	FI_PROTO_IB_RDM,
	FI_PROTO_GNI,
	FI_PROTO_RXM,
	FI_PROTO_RXD,
	FI_PROTO_MLX,
	FI_PROTO_NETWORKDIRECT,
	FI_PROTO_PSMX2,
	FI_PROTO_SHM,
	FI_PROTO_MRAIL,
	FI_PROTO_RSTREAM,
	FI_PROTO_RDMA_CM_IB_XRC,
	FI_PROTO_EFA,
	FI_PROTO_PSMX3,
	FI_PROTO_RXM_TCP,
	FI_PROTO_OPX,
	FI_PROTO_CXI,
	FI_PROTO_XNET,
	FI_PROTO_COLL,
	FI_PROTO_UCX,
	FI_PROTO_SM2,
	FI_PROTO_CXI_RNR,
	FI_PROTO_LPP,
	FI_PROTO_LNX,
};

enum {
	FI_TAG_BITS,
	FI_TAG_MPI,
	FI_TAG_CCL,
	FI_TAG_MAX_FORMAT = (1ULL << 16),
};

enum {
	FI_TC_UNSPEC = 0,
	FI_TC_DSCP = 0x100,
	FI_TC_LABEL = 0x200,
	FI_TC_BEST_EFFORT = FI_TC_LABEL,
	FI_TC_LOW_LATENCY,
	FI_TC_DEDICATED_ACCESS,
	FI_TC_BULK_DATA,
	FI_TC_SCAVENGER,
	FI_TC_NETWORK_CTRL,
};

static inline uint32_t fi_tc_dscp_set(uint8_t dscp)
{
	return ((uint32_t) dscp) | FI_TC_DSCP;
}

static inline uint8_t fi_tc_dscp_get(uint32_t tclass)
{
	return tclass & FI_TC_DSCP ? (uint8_t) tclass : 0;
}

/* Mode bits */
#define FI_CONTEXT		(1ULL << 59)
#define FI_MSG_PREFIX		(1ULL << 58)
#define FI_ASYNC_IOV		(1ULL << 57)
#define FI_RX_CQ_DATA		(1ULL << 56)
#define FI_LOCAL_MR		_Pragma("GCC warning \"'FI_LOCAL_MR' is deprecated\"")	(1ULL << 55)
/* #define FI_NOTIFY_FLAGS_ONLY	(1ULL << 54) */
/* #define FI_RESTRICTED_COMP	(1ULL << 53) */
#define FI_CONTEXT2		(1ULL << 52)
/* #define FI_BUFFERED_RECV	(1ULL << 51) */
/* #define FI_PEER_TRANSFER	(1ULL << 36) */

struct fi_tx_attr {
	uint64_t		caps;
	uint64_t		mode;
	uint64_t		op_flags;
	uint64_t		msg_order;
	uint64_t		comp_order;
	size_t			inject_size;
	size_t			size;
	size_t			iov_limit;
	size_t			rma_iov_limit;
	uint32_t		tclass;
};

struct fi_rx_attr {
	uint64_t		caps;
	uint64_t		mode;
	uint64_t		op_flags;
	uint64_t		msg_order;
	uint64_t		comp_order;
	size_t			total_buffered_recv;
	size_t			size;
	size_t			iov_limit;
};

struct fi_ep_attr {
	enum fi_ep_type		type;
	uint32_t		protocol;
	uint32_t		protocol_version;
	size_t			max_msg_size;
	size_t			msg_prefix_size;
	size_t			max_order_raw_size;
	size_t			max_order_war_size;
	size_t			max_order_waw_size;
	uint64_t		mem_tag_format;
	size_t			tx_ctx_cnt;
	size_t			rx_ctx_cnt;
	size_t			auth_key_size;
	uint8_t			*auth_key;
};

struct fi_domain_attr {
	struct fid_domain	*domain;
	char			*name;
	enum fi_threading	threading;
	enum fi_progress	control_progress;
	union {
		enum fi_progress data_progress;
		enum fi_progress progress;
	};
	enum fi_resource_mgmt	resource_mgmt;
	enum fi_av_type		av_type;
	int			mr_mode;
	size_t			mr_key_size;
	size_t			cq_data_size;
	size_t			cq_cnt;
	size_t			ep_cnt;
	size_t			tx_ctx_cnt;
	size_t			rx_ctx_cnt;
	size_t			max_ep_tx_ctx;
	size_t			max_ep_rx_ctx;
	size_t			max_ep_stx_ctx;
	size_t			max_ep_srx_ctx;
	size_t			cntr_cnt;
	size_t			mr_iov_limit;
	uint64_t		caps;
	uint64_t		mode;
	uint8_t			*auth_key;
	size_t 			auth_key_size;
	size_t			max_err_data;
	size_t			mr_cnt;
	uint32_t		tclass;
	size_t			max_ep_auth_key;
	uint32_t		max_group_id;
};

struct fi_fabric_attr {
	struct fid_fabric	*fabric;
	char			*name;
	char			*prov_name;
	uint32_t		prov_version;
	uint32_t		api_version;
};

struct fi_info {
	struct fi_info		*next;
	uint64_t		caps;
	uint64_t		mode;
	uint32_t		addr_format;
	size_t			src_addrlen;
	size_t			dest_addrlen;
	void			*src_addr;
	void			*dest_addr;
	fid_t			handle;
	struct fi_tx_attr	*tx_attr;
	struct fi_rx_attr	*rx_attr;
	struct fi_ep_attr	*ep_attr;
	struct fi_domain_attr	*domain_attr;
	struct fi_fabric_attr	*fabric_attr;
	struct fid_nic		*nic;
};

struct fi_device_attr {
	char			*name;
	char			*device_id;
	char			*device_version;
	char			*vendor_id;
	char			*driver;
	char			*firmware;
};

enum fi_bus_type {
	FI_BUS_UNSPEC,
	FI_BUS_UNKNOWN = FI_BUS_UNSPEC,
	FI_BUS_PCI,
};

struct fi_pci_attr {
	uint16_t		domain_id;
	uint8_t			bus_id;
	uint8_t			device_id;
	uint8_t			function_id;
};

struct fi_bus_attr {
	enum fi_bus_type	bus_type;
	union {
		struct fi_pci_attr	pci;
	} attr;
};

enum fi_link_state {
	FI_LINK_UNKNOWN,
	FI_LINK_DOWN,
	FI_LINK_UP,
};

struct fi_link_attr {
	char			*address;
	size_t			mtu;
	size_t			speed;
	enum fi_link_state	state;
	char			*network_type;
};

enum {
	FI_CLASS_UNSPEC,
	FI_CLASS_FABRIC,
	FI_CLASS_DOMAIN,
	FI_CLASS_EP,
	FI_CLASS_SEP,
	FI_CLASS_RX_CTX,
	FI_CLASS_SRX_CTX,
	FI_CLASS_TX_CTX,
	FI_CLASS_STX_CTX,
	FI_CLASS_PEP,
	FI_CLASS_INTERFACE,
	FI_CLASS_AV,
	FI_CLASS_MR,
	FI_CLASS_EQ,
	FI_CLASS_CQ,
	FI_CLASS_CNTR,
	FI_CLASS_WAIT,
	FI_CLASS_POLL,
	FI_CLASS_CONNREQ,
	FI_CLASS_MC,
	FI_CLASS_NIC,
	FI_CLASS_AV_SET,
	FI_CLASS_MR_CACHE,
	FI_CLASS_MEM_MONITOR,
	FI_CLASS_PEER_CQ,
	FI_CLASS_PEER_SRX,
	FI_CLASS_LOG,
	FI_CLASS_PEER_AV,
	FI_CLASS_PEER_AV_SET,
	FI_CLASS_PEER_CNTR,
	FI_CLASS_PROFILE,
};

struct fi_eq_attr;
struct fi_wait_attr;

/* fi_bind()-specific flags */
#define FI_SELECTIVE_COMPLETION	(1ULL << 59)

struct fi_ops {
	size_t	size;
	int	(*close)(struct fid *fid);
	int	(*bind)(struct fid *fid, struct fid *bfid, uint64_t flags);
	int	(*control)(struct fid *fid, int command, void *arg);
	int	(*ops_open)(struct fid *fid, const char *name,
			    uint64_t flags, void **ops, void *context);
	int	(*tostr)(const struct fid *fid, char *buf, size_t len);
	int	(*ops_set)(struct fid *fid, const char *name, uint64_t flags,
			   void *ops, void *context);
};

/* All fabric interface descriptors must start with this structure */
struct fid {
	size_t			fclass;
	void			*context;
	struct fi_ops		*ops;
};

int fi_getinfo(uint32_t version, const char *node, const char *service,
	       uint64_t flags, const struct fi_info *hints,
	       struct fi_info **info);
void fi_freeinfo(struct fi_info *info);
struct fi_info *fi_dupinfo(const struct fi_info *info);

static inline struct fi_info *fi_allocinfo(void)
{
	return fi_dupinfo(NULL);
}

struct fi_ops_fabric {
	size_t	size;
	int	(*domain)(struct fid_fabric *fabric, struct fi_info *info,
			struct fid_domain **dom, void *context);
	int	(*passive_ep)(struct fid_fabric *fabric, struct fi_info *info,
			struct fid_pep **pep, void *context);
	int	(*eq_open)(struct fid_fabric *fabric, struct fi_eq_attr *attr,
			struct fid_eq **eq, void *context);
	int	(*wait_open)(struct fid_fabric *fabric, struct fi_wait_attr *attr,
			struct fid_wait **waitset);
	int	(*trywait)(struct fid_fabric *fabric, struct fid **fids,
			int count);
	int	(*domain2)(struct fid_fabric *fabric, struct fi_info *info,
			struct fid_domain **dom, uint64_t flags, void *context);
};

struct fid_fabric {
	struct fid		fid;
	struct fi_ops_fabric	*ops;
	uint32_t		api_version;
};

int fi_fabric2(struct fi_info *info, struct fid_fabric **fabric,
	       uint64_t flags, void *context);
int fi_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
	      void *context);
int fi_open(uint32_t version, const char *name, void *attr, size_t attr_len,
	    uint64_t flags, struct fid **fid, void *context);

struct fid_nic {
	struct fid		fid;
	struct fi_device_attr	*device_attr;
	struct fi_bus_attr	*bus_attr;
	struct fi_link_attr	*link_attr;
	void			*prov_attr;
};

#define FI_CHECK_OP(ops, opstype, op) \
	(ops && (ops->size > offsetof(opstype, op)) && ops->op)

static inline int fi_close(struct fid *fid)
{
	return fid->ops->close(fid);
}

struct fi_alias {
	struct fid 		**fid;
	uint64_t		flags;
};

struct fi_fid_var {
	int		name;
	void		*val;
};

struct fi_mr_raw_attr {
	uint64_t	flags;
	uint64_t	*base_addr;
	uint8_t		*raw_key;
	size_t		*key_size;
};

struct fi_mr_map_raw {
	uint64_t	flags;
	uint64_t	base_addr;
	uint8_t		*raw_key;
	size_t		key_size;
	uint64_t	*key;
};

/* control commands */
enum {
	FI_GETFIDFLAG,		/* uint64_t flags */
	FI_SETFIDFLAG,		/* uint64_t flags */
	FI_GETOPSFLAG,		/* uint64_t flags */
	FI_SETOPSFLAG,		/* uint64_t flags */
	FI_ALIAS,		/* struct fi_alias * */
	FI_GETWAIT,		/* void * wait object */
	FI_ENABLE,		/* NULL */
	FI_BACKLOG,		/* integer * */
	FI_GET_RAW_MR,		/* fi_mr_raw_attr */
	FI_MAP_RAW_MR,		/* fi_mr_map_raw */
	FI_UNMAP_KEY,		/* uint64_t key */
	FI_QUEUE_WORK,		/* struct fi_deferred_work */
	FI_CANCEL_WORK,		/* struct fi_deferred_work */
	FI_FLUSH_WORK,		/* NULL */
	FI_REFRESH,		/* mr: fi_mr_modify */
	FI_DUP,			/* struct fid ** */
	FI_GETWAITOBJ,		/*enum fi_wait_obj * */
	FI_GET_VAL,		/* struct fi_fid_var */
	FI_SET_VAL,		/* struct fi_fid_var */
	FI_EXPORT_FID,		/* struct fi_fid_export */
	FI_GET_FD, 		/* int */
};

static inline int fi_control(struct fid *fid, int command, void *arg)
{
	return fid->ops->control(fid, command, arg);
}

static inline int fi_alias(struct fid *fid, struct fid **alias_fid, uint64_t flags)
{
	struct fi_alias alias;
	alias.fid = alias_fid;
	alias.flags = flags;
	return fi_control(fid, FI_ALIAS, &alias);
}

/* Provider specific names should set the uppermost bit. */

static inline int fi_get_val(struct fid *fid, int name, void *val)
{
	struct fi_fid_var var;
	var.name = name;
	var.val = val;
	return fi_control(fid, FI_GET_VAL, &var);
}

static inline int fi_set_val(struct fid *fid, int name, void *val)
{
	struct fi_fid_var var;
	var.name = name;
	var.val = val;
	return fi_control(fid, FI_SET_VAL, &var);
}

static inline int
fi_open_ops(struct fid *fid, const char *name, uint64_t flags,
	    void **ops, void *context)
{
	return fid->ops->ops_open(fid, name, flags, ops, context);
}

static inline int
fi_set_ops(struct fid *fid, const char *name, uint64_t flags,
	   void *ops, void *context)
{
	return FI_CHECK_OP(fid->ops, struct fi_ops, ops_set) ?
		fid->ops->ops_set(fid, name, flags, ops, context) : -FI_ENOSYS;
}

enum fi_type {
	FI_TYPE_INFO,
	FI_TYPE_EP_TYPE,
	FI_TYPE_CAPS,
	FI_TYPE_OP_FLAGS,
	FI_TYPE_ADDR_FORMAT,
	FI_TYPE_TX_ATTR,
	FI_TYPE_RX_ATTR,
	FI_TYPE_EP_ATTR,
	FI_TYPE_DOMAIN_ATTR,
	FI_TYPE_FABRIC_ATTR,
	FI_TYPE_THREADING,
	FI_TYPE_PROGRESS,
	FI_TYPE_PROTOCOL,
	FI_TYPE_MSG_ORDER,
	FI_TYPE_MODE,
	FI_TYPE_AV_TYPE,
	FI_TYPE_ATOMIC_TYPE,
	FI_TYPE_ATOMIC_OP,
	FI_TYPE_VERSION,
	FI_TYPE_EQ_EVENT,
	FI_TYPE_CQ_EVENT_FLAGS,
	FI_TYPE_MR_MODE,
	FI_TYPE_OP_TYPE,
	FI_TYPE_FID,
	FI_TYPE_COLLECTIVE_OP,
	FI_TYPE_HMEM_IFACE,
	FI_TYPE_CQ_FORMAT,
	FI_TYPE_LOG_LEVEL,
	FI_TYPE_LOG_SUBSYS,
	FI_TYPE_AV_ATTR,
	FI_TYPE_CQ_ATTR,
	FI_TYPE_MR_ATTR,
	FI_TYPE_CNTR_ATTR,
	FI_TYPE_CQ_ERR_ENTRY,
};

char *fi_tostr(const void *data, enum fi_type datatype);
char *fi_tostr_r(char *buf, size_t len, const void *data,
		 enum fi_type datatype);

enum fi_param_type {
	FI_PARAM_STRING,
	FI_PARAM_INT,
	FI_PARAM_BOOL,
	FI_PARAM_SIZE_T,
};

struct fi_param {
	const char *name;
	enum fi_param_type type;
	const char *help_string;
	const char *value;
};

int fi_getparams(struct fi_param **params, int *count);
void fi_freeparams(struct fi_param *params);

/* Dummy definitions for removed flags/caps/types. For compiling old fabtests */
#define FI_VARIABLE_MSG		0ULL
#define FI_NOTIFY_FLAGS_ONLY	0ULL
#define FI_RESTRICTED_COMP	0ULL
#define FI_EP_SOCK_STREAM	FI_EP_UNSPEC

#ifdef FABRIC_DIRECT
#include <rdma/fi_direct.h>
#endif	/* FABRIC_DIRECT */

#ifndef FABRIC_DIRECT_
struct fi_context {
	void			*internal[4];
};

struct fi_context2 {
	void			*internal[8];
};
#endif

struct fi_recv_context {
	struct fid_ep		*ep;
	void			*context;
};

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_H */
