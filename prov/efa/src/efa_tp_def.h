/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER EFA_TP_PROV

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "efa_tp_def.h"

#if !defined(_EFA_TP_DEF_H) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _EFA_TP_DEF_H

#include <lttng/tracepoint.h>
#include <rdma/fi_domain.h>
#include "efa_mr.h"

#define EFA_TP_PROV efa

/* Pre-defined tracepoints */

#define X_WR_ID_ARGS \
	size_t, wr_id

#define X_WR_ID_FIELDS \
	lttng_ust_field_integer_hex(size_t, wr_id, wr_id)

#define X_WR_ID_OPCODE_ARGS \
	size_t, wr_id, \
	int, opcode

#define X_WR_ID_OPCODE_FIELDS \
	lttng_ust_field_integer_hex(size_t, wr_id, wr_id) \
	lttng_ust_field_integer_hex(int, opcode, opcode)

#define X_PKT_ARGS \
	size_t, wr_id, \
	size_t, context

#define X_PKT_FIELDS \
	lttng_ust_field_integer_hex(size_t, wr_id, wr_id) \
	lttng_ust_field_integer_hex(size_t, context, context)

#define MSG_ARGS \
	size_t, msg_ctx, \
	size_t, addr

#define MSG_FIELDS \
	lttng_ust_field_integer_hex(size_t, msg_ctx, msg_ctx) \
	lttng_ust_field_integer_hex(size_t, addr, addr)


LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, msg_context,
	LTTNG_UST_TP_ARGS(MSG_ARGS),
	LTTNG_UST_TP_FIELDS(MSG_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, msg_context, EFA_TP_PROV,
	send_begin_msg_context,
	LTTNG_UST_TP_ARGS(MSG_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, send_begin_msg_context, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, msg_context, EFA_TP_PROV,
	recv_begin_msg_context,
	LTTNG_UST_TP_ARGS(MSG_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, recv_begin_msg_context, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, msg_context, EFA_TP_PROV,
	read_begin_msg_context,
	LTTNG_UST_TP_ARGS(MSG_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, read_begin_msg_context, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, msg_context, EFA_TP_PROV,
	write_begin_msg_context,
	LTTNG_UST_TP_ARGS(MSG_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, write_begin_msg_context, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)


LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, post_wr_id,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS),
	LTTNG_UST_TP_FIELDS(X_PKT_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, post_wr_id, EFA_TP_PROV,
	post_send,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, post_send, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, post_wr_id, EFA_TP_PROV,
	post_recv,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, post_recv, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, post_wr_id, EFA_TP_PROV,
	post_read,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, post_read, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, post_wr_id, EFA_TP_PROV,
	post_write,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, post_write, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, handle_completion,
	LTTNG_UST_TP_ARGS(X_WR_ID_ARGS),
	LTTNG_UST_TP_FIELDS(X_WR_ID_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, handle_completion, EFA_TP_PROV,
	handle_rx_completion,
	LTTNG_UST_TP_ARGS(X_WR_ID_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, handle_rx_completion, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, handle_completion, EFA_TP_PROV,
	handle_tx_completion,
	LTTNG_UST_TP_ARGS(X_WR_ID_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, handle_tx_completion, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, completion_with_opcode,
	LTTNG_UST_TP_ARGS(X_WR_ID_OPCODE_ARGS),
	LTTNG_UST_TP_FIELDS(X_WR_ID_OPCODE_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, completion_with_opcode, EFA_TP_PROV,
	handle_completion,
	LTTNG_UST_TP_ARGS(X_WR_ID_OPCODE_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, handle_completion, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_ENUM(efa, hmem_iface,
    LTTNG_UST_TP_ENUM_VALUES(
        lttng_ust_field_enum_value("FI_HMEM_SYSTEM", FI_HMEM_SYSTEM)
        lttng_ust_field_enum_value("FI_HMEM_CUDA", FI_HMEM_CUDA)
        lttng_ust_field_enum_value("FI_HMEM_ROCR", FI_HMEM_ROCR)
        lttng_ust_field_enum_value("FI_HMEM_NEURON", FI_HMEM_NEURON)
        lttng_ust_field_enum_value("FI_HMEM_SYNAPSEAI", FI_HMEM_SYNAPSEAI)
    )
)

#define HMEM_COPY_ARGS \
	struct efa_mr_peer *, peer, \
	void *, dest, \
	const void *, src, \
	size_t, size

#define HMEM_COMMON_FIELDS \
	lttng_ust_field_enum(efa, hmem_iface, int, iface, peer->iface) \
	lttng_ust_field_integer_hex(void *, dest, dest) \
	lttng_ust_field_integer_hex(void *, src, src) \
	lttng_ust_field_integer(size_t, size, size)

#define HMEM_COPY_FIELDS \
	HMEM_COMMON_FIELDS \
	lttng_ust_field_integer(uint64_t, device, peer->device)

#define HMEM_DEV_REG_COPY_FIELDS \
	HMEM_COMMON_FIELDS \
	lttng_ust_field_integer_hex(void *, handle, peer->hmem_data)


LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, hmem_copy,
	LTTNG_UST_TP_ARGS(HMEM_COPY_ARGS),
	LTTNG_UST_TP_FIELDS(HMEM_COPY_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, hmem_dev_reg_copy,
	LTTNG_UST_TP_ARGS(HMEM_COPY_ARGS),
	LTTNG_UST_TP_FIELDS(HMEM_DEV_REG_COPY_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, hmem_copy, EFA_TP_PROV,
	copy_to_hmem,
	LTTNG_UST_TP_ARGS(HMEM_COPY_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, copy_to_hmem, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, hmem_copy, EFA_TP_PROV,
	copy_from_hmem,
	LTTNG_UST_TP_ARGS(HMEM_COPY_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, copy_from_hmem, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, hmem_dev_reg_copy, EFA_TP_PROV,
	dev_reg_copy_to_hmem,
	LTTNG_UST_TP_ARGS(HMEM_COPY_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, dev_reg_copy_to_hmem, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, hmem_dev_reg_copy, EFA_TP_PROV,
	dev_reg_copy_from_hmem,
	LTTNG_UST_TP_ARGS(HMEM_COPY_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, dev_reg_copy_from_hmem, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

#endif /* _EFA_TP_DEF_H */

#include <lttng/tracepoint-event.h>
