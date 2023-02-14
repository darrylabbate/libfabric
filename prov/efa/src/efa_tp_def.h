#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER EFA_TP_PROV

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "efa_tp_def.h"

#if !defined(_EFA_TP_DEF_H) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _EFA_TP_DEF_H

#include <lttng/tracepoint.h>

#define EFA_TP_PROV efa

/* Pre-defined tracepoints */

#define X_PKT_ARGS \
	size_t, wr_id, \
	size_t, efa_rdm_ope, \
	size_t, context

#define X_PKT_FIELDS \
	lttng_ust_field_integer_hex(size_t, wr_id, wr_id) \
	lttng_ust_field_integer_hex(size_t, efa_rdm_ope, efa_rdm_ope) \
	lttng_ust_field_integer_hex(size_t, context, context)

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

#include <include/ofi_lock.h>

#define MUTEX_ARGS \
	ofi_mutex_t *, mutex_arg

#if ENABLE_DEBUG

#define MUTEX_FIELDS \
	lttng_ust_field_integer_hex (void *, mutex,  &mutex_arg->impl) \
	lttng_ust_field_integer     (int,    in_use,  mutex_arg->in_use)

#else

#define MUTEX_FIELDS \
	lttng_ust_field_integer_hex(ofi_mutex_t *, ofi_mutex, mutex_arg)

#endif

LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, ofi_mutex,
	LTTNG_UST_TP_ARGS(MUTEX_ARGS),
	LTTNG_UST_TP_FIELDS(MUTEX_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, ofi_mutex, EFA_TP_PROV,
	mutex_lock_begin,
	LTTNG_UST_TP_ARGS(MUTEX_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, mutex_lock_begin, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, ofi_mutex, EFA_TP_PROV,
	mutex_lock_end,
	LTTNG_UST_TP_ARGS(MUTEX_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, mutex_lock_end, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, ofi_mutex, EFA_TP_PROV,
	mutex_unlock_begin,
	LTTNG_UST_TP_ARGS(MUTEX_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, mutex_unlock_begin, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, ofi_mutex, EFA_TP_PROV,
	mutex_unlock_end,
	LTTNG_UST_TP_ARGS(MUTEX_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, mutex_unlock_end, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

#endif /* _EFA_TP_DEF_H */

#include <lttng/tracepoint-event.h>
