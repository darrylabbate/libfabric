/* Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */

#include "efa_rdm_proto_longread.h"

#include "efa_rdm_ope.h"
#include "efa_rdm_pke_nonreq.h"

void efa_rdm_proto_longread_handle_eor_recv(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_eor_hdr *eor_hdr =
		(struct efa_rdm_eor_hdr *) pkt_entry->wiredata;
	struct efa_rdm_ope *txe =
		efa_rdm_ep_live_txe_from_id(pkt_entry->ep, eor_hdr->send_id);

	if (!txe) {
		EFA_INFO(FI_LOG_CQ,
			 "EOR names a send that is no longer live, dropping it\n");
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	efa_rdm_txe_release_read_msg_slot(txe);

	txe->bytes_acked += txe->total_len - txe->bytes_runt;
	if (txe->bytes_acked == txe->total_len) {
		efa_rdm_txe_report_completion(txe);
		txe->internal_flags |= EFA_RDM_TXE_REMOTE_ACK_RECEIVED;
		if (efa_rdm_txe_with_remote_ack_ready_for_release(txe))
			efa_rdm_txe_release(txe);
	}

	efa_rdm_pke_release_rx(pkt_entry);
}
