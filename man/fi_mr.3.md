---
layout: page
title: fi_mr(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_mr \- Memory region operations

fi_mr_reg / fi_mr_regv / fi_mr_regattr
: Register local memory buffers for direct fabric access

fi_close
: Deregister registered memory buffers.

fi_mr_desc
: Return a local descriptor associated with a registered memory region

fi_mr_key
: Return the remote key needed to access a registered memory region

fi_mr_raw_attr
: Return raw memory region attributes.

fi_mr_map_raw
: Converts a raw memory region key into a key that is usable for data
  transfer operations.

fi_mr_unmap_key
: Releases a previously mapped raw memory region key.

fi_mr_bind
: Associate a registered memory region with a completion counter or an endpoint.

fi_mr_refresh
: Updates the memory pages associated with a memory region.

fi_mr_enable
: Enables a memory region for use.

fi_hmem_ze_device
: Returns an hmem device identifier for a level zero driver and device.

# SYNOPSIS

```c
#include <rdma/fi_domain.h>

int fi_mr_reg(struct fid_domain *domain, const void *buf, size_t len,
    uint64_t access, uint64_t offset, uint64_t requested_key,
    uint64_t flags, struct fid_mr **mr, void *context);

int fi_mr_regv(struct fid_domain *domain, const struct iovec * iov,
    size_t count, uint64_t access, uint64_t offset, uint64_t requested_key,
    uint64_t flags, struct fid_mr **mr, void *context);

int fi_mr_regattr(struct fid_domain *domain, const struct fi_mr_attr *attr,
    uint64_t flags, struct fid_mr **mr);

int fi_close(struct fid *mr);

void * fi_mr_desc(struct fid_mr *mr);

uint64_t fi_mr_key(struct fid_mr *mr);

int fi_mr_raw_attr(struct fid_mr *mr, uint64_t *base_addr,
    uint8_t *raw_key, size_t *key_size, uint64_t flags);

int fi_mr_map_raw(struct fid_domain *domain, uint64_t base_addr,
    uint8_t *raw_key, size_t key_size, uint64_t *key, uint64_t flags);

int fi_mr_unmap_key(struct fid_domain *domain, uint64_t key);

int fi_mr_bind(struct fid_mr *mr, struct fid *bfid, uint64_t flags);

int fi_mr_refresh(struct fid_mr *mr, const struct iovec *iov,
    size_t count, uint64_t flags);

int fi_mr_enable(struct fid_mr *mr);

int fi_hmem_ze_device(int driver_index, int device_index);
```

# ARGUMENTS

*domain*
: Resource domain

*mr*
: Memory region

*bfid*
: Fabric identifier of an associated resource.

*context*
: User specified context associated with the memory region.

*buf*
: Memory buffer to register with the fabric hardware.

*len*
: Length of memory buffer to register.  Must be > 0.

*iov*
: Vectored memory buffer.

*count*
: Count of vectored buffer entries.

*access*
: Memory access permissions associated with registration

*offset*
: Optional specified offset for accessing specified registered buffers.
  This parameter is reserved for future use and must be 0.

*requested_key*
: Requested remote key associated with registered buffers.  Parameter
  is ignored if FI_MR_PROV_KEY flag is set in the domain mr_mode bits.
  Parameter may be ignored if remote access permissions are not asked
  for.

*attr*
: Memory region attributes

*flags*
: Additional flags to apply to the operation.

# DESCRIPTION

Registered memory regions associate memory buffers with permissions
granted for access by fabric resources.  A memory buffer must be
registered with a resource domain before it can be used as the target
of a remote RMA or atomic data transfer.  Additionally, a fabric
provider may require that data buffers be registered before being used
in local transfers.  Memory registration restrictions are controlled
using a separate set of mode bits, specified through the domain
attributes (mr_mode field).  Each mr_mode bit requires that an
application take specific steps in order to use memory buffers with
libfabric interfaces.

As a special case, a new memory region can be created from an existing
memory region.  Such a new memory region is called a sub-MR, and the existing
memory region is called the base MR.  Sub-MRs may be used to shared hardware
resources, such as virtual to physical address translations and page pinning.
This can improve performance when creating and destroying sub-regions that
need different access rights.  The base MR itself can also be a sub-MR,
allowing for a hierarchy of memory regions.

The following apply to memory registration.

*Default Memory Registration*
: If no mr_mode bits are set, the default behaviors describe below are
  followed.  Historically, these defaults were collectively referred to as
  scalable memory registration.  The default requirements are outlined below,
  followed by definitions of how each mr_mode bit alters the definition.

  Compatibility: For library versions 1.4 and earlier, this was indicated by
  setting mr_mode to FI_MR_SCALABLE and the fi_info mode bit FI_LOCAL_MR to 0.
  FI_MR_SCALABLE and FI_LOCAL_MR were deprecated in libfabric version 1.5,
  though they are supported for backwards compatibility purposes.

  For security, memory registration is required for data buffers that are
  accessed directly by a peer process.  For example, registration is
  required for RMA target buffers (read or written to), and those accessed
  by atomic or collective operations.

  By default, registration occurs on virtual address ranges.
  Because registration refers to address ranges, rather than allocated
  data buffers, the address ranges do not need to map to
  data buffers allocated by the application at the time the registration
  call is made.  That is, an application can register any
  range of addresses in their virtual address space, whether or not those
  addresses are backed by physical pages or have been allocated.

  Note that physical pages must back addresses prior to the addresses being
  accessed as part of a data transfer operation, or the data transfers will
  fail.  Additionally, depending on the operation, this could result in the
  local process receiving a segmentation fault for accessing invalid memory.

  Once registered, the resulting memory regions are accessible by peers starting
  at a base address of 0.  That is, the target address that is specified is a
  byte offset into the registered region.

  The application also selects the access key associated with the MR.  The
  key size is restricted to a maximum of 8 bytes.

  With scalable registration, locally accessed data buffers are not registered.
  This includes source buffers for all transmit operations -- sends,
  tagged sends, RMA, and atomics -- as well as buffers posted for receive
  and tagged receive operations.

  Although the default memory registration behavior is convenient for
  application developers, it is difficult to implement in hardware.
  Attempts to hide the hardware requirements from the application often
  results in significant and unacceptable impacts to performance.  The
  following mr_mode bits are provided as input into fi_getinfo.  If a
  provider requires the behavior defined for an mr_mode bit, it will leave
  the bit set on output to fi_getinfo.  Otherwise, the provider can clear
  the bit to indicate that the behavior is not needed.

  By setting an mr_mode bit, the application has agreed to adjust its
  behavior as indicated.  Importantly, applications that choose to support
  an mr_mode must be prepared to handle the case where the mr_mode is
  not required.  A provider will clear an mr_mode bit if it is not needed.

*FI_MR_LOCAL*
: When the FI_MR_LOCAL mode bit is set, applications must register all
  data buffers that will be accessed by the local hardware and provide
  a valid desc parameter into applicable data transfer operations.

  When FI_MR_LOCAL is unset, applications are not required to register
  data buffers before using them for local operations (e.g. send and
  receive data buffers). Prior to libfabric 1.22, the desc parameter was
  ignored. In libfabric 1.22 and later, the desc parameter must be either
  valid or NULL. This behavior allows applications to optionally pass in a
  valid desc parameter. If the desc parameter is NULL, any required local
  memory registration will be handled by the provider.

  A provider may hide local registration requirements from applications
  by making use of an internal registration cache or similar mechanisms.
  Such mechanisms, however, may negatively impact performance for some
  applications, notably those which manage their own network buffers.
  In order to support as broad range of applications as possible,
  without unduly affecting their performance, applications that wish to
  manage their own local memory registrations may do so by using the
  memory registration calls and passing in a valid desc parameter.

  Note: the FI_MR_LOCAL mr_mode bit replaces the FI_LOCAL_MR fi_info mode bit.
  When FI_MR_LOCAL is set, FI_LOCAL_MR is ignored.

*FI_MR_RAW*
: Raw memory regions are used to support providers with keys larger than
  64-bits or require setup at the peer.  When the FI_MR_RAW bit is set,
  applications must use fi_mr_raw_attr() locally and fi_mr_map_raw()
  at the peer before targeting a memory region as part of any data transfer
  request.

*FI_MR_VIRT_ADDR*
: The FI_MR_VIRT_ADDR bit indicates that the provider references memory
  regions by virtual address, rather than a 0-based offset.  Peers that
  target memory regions registered with FI_MR_VIRT_ADDR specify the
  destination memory buffer using the target's virtual address, with any
  offset into the region specified as virtual address + offset.  Support
  of this bit typically implies that peers must exchange addressing data
  prior to initiating any RMA or atomic operation.

  For memory regions that are registered using FI_MR_DMABUF, the starting
  'virtual address' of the DMA-buf region is obtained by adding the offset
  field to the base_addr field of struct fi_mr_dmabuf that was specified
  through the registration call.

*FI_MR_ALLOCATED*
: When set, all registered memory regions must be backed by physical
  memory pages at the time the registration call is made. In addition,
  applications must not perform operations which may result in the underlying
  virtual address to physical page mapping to change (e.g. calling free()
  against an allocated MR). Failing to adhere to this may result in the
  virtual address pointing to one set of physical pages while the MR points
  to another set of physical pages.

  When unset, registered memory regions need not be backed by physical
  memory pages at the time the registration call is made. In addition, the
  underlying virtual address to physical page mapping is allowed to change,
  and the provider will ensure the corresponding MR is updated accordingly.
  This behavior enables application use-cases where memory may be frequently
  freed and reallocated or system memory migrating to/from device memory.

  When unset, the application is responsible for ensuring that a registered
  memory region references valid physical pages while a data transfer is
  active against it, or the data transfer may fail. Application changes to
  the virtual address range must be coordinated with network traffic to or
  from that range.

  If unset and FI_HMEM is supported, the ability for the virtual address to
  physical address mapping to change extends to HMEM interfaces as well. If a
  provider cannot support a virtual address to physical address changing for
  a given HMEM interface, the provider should support a reasonable fallback
  or the operation should fail.

*FI_MR_PROV_KEY*
: This memory region mode indicates that the provider does not support
  application requested MR keys.  MR keys are returned by the provider.
  Applications that support FI_MR_PROV_KEY can obtain the provider key
  using fi_mr_key(), unless FI_MR_RAW is also set. The returned key should
  then be exchanged with peers prior to initiating an RMA or atomic
  operation.

*FI_MR_MMU_NOTIFY*
: FI_MR_MMU_NOTIFY is typically set by providers that support memory
  registration against memory regions that are not necessarily backed by
  allocated physical pages at the time the memory registration occurs.
  (That is, FI_MR_ALLOCATED is typically 0).  However, such providers
  require that applications notify the provider prior to the MR being
  accessed as part of a data transfer operation.  This notification
  informs the provider that all necessary physical pages now back the
  region.  The notification is necessary for providers that cannot
  hook directly into the operating system page tables or memory management
  unit.  See fi_mr_refresh() for notification details.

*FI_MR_RMA_EVENT*
: This mode bit indicates that the provider must configure memory
  regions that are associated with RMA events prior to their use.  This
  includes all memory regions that are associated with completion counters.
  When set, applications must indicate if a memory region will be
  associated with a completion counter as part of the region's creation.
  This is done by passing in the FI_RMA_EVENT flag to the memory
  registration call.

  Such memory regions will be created in a disabled state and must be
  associated with all completion counters prior to being enabled.  To
  enable a memory region, the application must call fi_mr_enable().
  After calling fi_mr_enable(), no further resource bindings may be
  made to the memory region.

*FI_MR_ENDPOINT*
: This mode bit indicates that the provider associates memory regions
  with endpoints rather than domains.  Memory regions that are
  registered with the provider are created in a disabled state and
  must be bound to an endpoint prior to being enabled.  To bind the
  MR with an endpoint, the application must use fi_mr_bind().  To
  enable the memory region, the application must call fi_mr_enable().

*FI_MR_HMEM*
: This mode bit is associated with the FI_HMEM capability.

  If FI_MR_HMEM is set, the application must register buffers that
  were allocated using a device call and provide a valid desc
  parameter into applicable data transfer operations even if they are
  only used for local operations (e.g. send and receive data buffers).
  Device memory must be registered using the fi_mr_regattr call, with
  the iface and device fields filled out.

  If FI_MR_HMEM is unset, the application need not register device buffers
  for local operations. In addition, fi_mr_regattr is not required to be used
  for device memory registration. It is the responsibility of the provider to
  discover the appropriate device memory registration attributes, if
  applicable.

  Similar to if FI_MR_LOCAL is unset, if FI_MR_HMEM is unset, applications may
  optionally pass in a valid desc parameter. If the desc parameter is NULL, any
  required local memory registration will be handled by the provider.

  If FI_MR_HMEM is set, but FI_MR_LOCAL is unset, only device buffers
  must be registered when used locally.  In this case, the desc parameter
  passed into data transfer operations must either be valid or NULL.
  Similarly, if FI_MR_LOCAL is set, but FI_MR_HMEM is not, the desc
  parameter must either be valid or NULL.

*FI_MR_COLLECTIVE*
: This bit is associated with the FI_COLLECTIVE capability.

  If FI_MR_COLLECTIVE is set, the provider requires that memory regions used in
  collection operations must explicitly be registered for use with collective
  calls. This requires registering regions passed to collective calls using the
  FI_COLLECTIVE flag.

  If FI_MR_COLLECTIVE is unset, memory registration for collection operations is
  optional. Applications may optionally pass in a valid desc parameter. If the
  desc parameter is NULL, any required local memory registration will be handled
  by the provider.

*Basic Memory Registration* (deprecated)
: Basic memory registration was deprecated in libfabric version 1.5, but
  is supported for backwards compatibility.  Basic memory registration
  is indicated by setting mr_mode equal to FI_MR_BASIC.
  FI_MR_BASIC must be set alone and not paired with mr_mode bits.
  Unlike other mr_mode bits, if FI_MR_BASIC is set on input to fi_getinfo(),
  it will not be cleared by the provider.  That is, setting mr_mode equal to
  FI_MR_BASIC forces basic registration if the provider supports it.

  The behavior of basic registration is equivalent
  to requiring the following mr_mode bits: FI_MR_VIRT_ADDR,
  FI_MR_ALLOCATED, and FI_MR_PROV_KEY.  Additionally, providers that
  support basic registration usually require the (deprecated) fi_info mode
  bit FI_LOCAL_MR, which was incorporated into the FI_MR_LOCAL mr_mode
  bit.

The registrations functions -- fi_mr_reg, fi_mr_regv, and
fi_mr_regattr -- are used to register one or more memory regions with
fabric resources.  The main difference between registration functions
are the number and type of parameters that they accept as input.
Otherwise, they perform the same general function.

**Deprecated** :
By default, memory registration completes synchronously.  I.e. the
registration call will not return until the registration has
completed.  Memory registration can complete asynchronous by binding
the resource domain to an event queue using the FI_REG_MR flag.  See
fi_domain_bind.  When memory registration is asynchronous, in order to
avoid a race condition between the registration call returning and the
corresponding reading of the event from the EQ, the mr output
parameter will be written before any event associated with the
operation may be read by the application.  An asynchronous event will
not be generated unless the registration call returns success (0).

## fi_mr_reg

The fi_mr_reg call registers the user-specified memory buffer with
the resource domain.  The buffer is enabled for access by the fabric
hardware based on the provided access permissions.  See the access
field description for memory region attributes below.

Registered memory is associated with a local memory descriptor and,
optionally, a remote memory key.  A memory descriptor is a provider
specific identifier associated with registered memory.  Memory
descriptors often map to hardware specific indices or keys associated
with the memory region.  Remote memory keys provide limited protection
against unwanted access by a remote node.  Remote accesses to a memory
region must provide the key associated with the registration.

Because MR keys must be provided by a remote process, an application
can use the requested_key parameter to indicate that a specific key
value be returned.  Support for user requested keys is provider
specific and is determined by the FI_MR_PROV_KEY flag value in the
mr_mode domain attribute.

Remote RMA and atomic operations indicate the location within a
registered memory region by specifying an address.  The location
is referenced by adding the offset to either the base virtual address
of the buffer or to 0, depending on the mr_mode.

The offset parameter is reserved for future use and must be 0.

For asynchronous memory registration requests, the result will be
reported to the user through an event queue associated with the
resource domain.  If successful, the allocated memory region structure
will be returned to the user through the mr parameter.  The mr address
must remain valid until the registration operation completes.  The
context specified with the registration request is returned with the
completion event.

For domains opened with FI_AV_AUTH_KEY, fi_mr_reg is not supported
and fi_mr_regattr must be used.

## fi_mr_regv

The fi_mr_regv call adds support for a scatter-gather list to
fi_mr_reg.  Multiple memory buffers are registered as a single memory
region.  Otherwise, the operation is the same.

For domains opened with FI_AV_AUTH_KEY, fi_mr_regv is not supported
and fi_mr_regattr must be used.

## fi_mr_regattr

The fi_mr_regattr call is a more generic, extensible registration call
that allows the user to specify the registration request using a
struct fi_mr_attr (defined below).

## fi_close

Fi_close is used to release all resources associated with a
registering a memory region.  Once deregistered, further access to the
registered memory is not guaranteed.  Active or queued operations that
reference a memory region being closed may attempt to access an invalid
memory region and fail. After an MR is closed, any new operations
targeting the closed MR will also fail. Applications are responsible
for ensuring that a MR is no longer needed prior to closing it.  Note
that accesses to a closed MR from a remote peer will result in an error
at the peer.  The state of the local endpoint will be unaffected.

For MRs that are associated with an endpoint (FI_MR_ENDPOINT flag
is set), the MR must be closed before the endpoint. If resources are
still associated with the MR when attempting to close, the call will
return -FI_EBUSY.

If a memory registration cache is used, the behavior of fi_close may be
affected. More information on the memory registration cache is in the
MEMORY REGISTRATION CACHE section.

## fi_mr_desc

Obtains the local memory descriptor associated with a MR.
The memory registration must have completed successfully before invoking
this call.

## fi_mr_key

Returns the remote protection key associated with a MR.  The memory
registration must have completed successfully before invoking this.  The
returned key may be used in data transfer operations at a peer.  If the
FI_MR_RAW mode bit has been set for the domain, then the memory key must
be obtained using the fi_mr_raw_key function instead.  A return value of
FI_KEY_NOTAVAIL will be returned if the registration has not completed
or a raw memory key is required.

## fi_mr_raw_attr

Returns the raw, remote protection key and base address associated with a MR.
The memory registration must have completed successfully before invoking
this routine.  Use of this call is required if the FI_MR_RAW mode bit has
been set by the provider; however, it is safe to use this call with any
memory region.

On input, the key_size parameter should indicate the size of the raw_key
buffer.  If the actual key is larger than what can fit into the buffer, it
will return -FI_ETOOSMALL.  On output, key_size is set to the size of the
buffer needed to store the key, which may be larger than the input value.
The needed key_size can also be obtained through the mr_key_size domain
attribute (fi_domain_attr) field.

A raw key must be mapped by a peer before it can be used in data transfer
operations.  See fi_mr_map_raw below.

## fi_mr_map_raw

Raw protection keys must be mapped to a usable key value before they
can be used for data transfer operations.  The mapping is done by the
peer that initiates the RMA or atomic operation.  The mapping function
takes as input the raw key and its size, and returns the mapped key.
Use of the fi_mr_map_raw function is required if the peer has the
FI_MR_RAW mode bit set, but this routine may be called on any valid
key.  All mapped keys must be freed by calling fi_mr_unmap_key when
access to the peer memory region is no longer necessary.

## fi_mr_unmap_key

This call releases any resources that may have been allocated as part of
mapping a raw memory key.  All mapped keys must be freed before the
corresponding domain is closed.

## fi_mr_bind

The fi_mr_bind function associates a memory region with a
counter or endpoint.  Counter bindings are needed by providers
that support the generation of completions based on fabric
operations.  Endpoint bindings are needed if the provider
associates memory regions with endpoints (see FI_MR_ENDPOINT).

When binding with a counter, the type of events tracked against the
memory region is based on the bitwise OR of the following flags.

*FI_REMOTE_WRITE*
: Generates an event whenever a remote RMA write or atomic operation
  modifies the memory region.  Use of this flag requires that the endpoint
  through which the MR is accessed be created with the FI_RMA_EVENT
  capability.

When binding the memory region to an endpoint, flags should be 0.

## fi_mr_refresh

The use of this call is to notify the provider of any change to the physical
pages backing a registered memory region. This call must be supported by
providers requiring FI_MR_MMU_NOTIFY and may optionally be supported by
providers not requiring FI_MR_ALLOCATED.

This call informs the provider that the page table entries associated with
the region may have been modified, and the provider should verify and update
the registered region accordingly. The iov parameter is optional and may be
used to specify which portions of the registered region requires updating.

Providers are only guaranteed to update the specified address ranges. Failing
to update a range will result in an error being returned.

When FI_MR_MMU_NOTIFY is set, the refresh operation has the effect of
disabling and re-enabling access to the registered region. Any operations
from peers that attempt to access the region will fail while the refresh is
occurring. Additionally, attempts to access the region by the local process
through libfabric APIs may result in a page fault or other fatal operation.

When FI_MR_ALLOCATED is unset, -FI_ENOSYS will be returned if a provider
does not support fi_mr_refresh. If supported, the provider will atomically
update physical pages of the MR associated with the user specified address
ranges. The MR will remain enabled during this time.

Note: FI_MR_MMU_NOTIFY set behavior takes precedence over FI_MR_ALLOCATED unset
behavior.

The fi_mr_refresh call is only needed if the physical pages might have
been updated after the memory region was created.

## fi_mr_enable

The enable call is used with memory registration associated with the
FI_MR_RMA_EVENT mode bit.  Memory regions created in the disabled state
must be explicitly enabled after being fully configured by the
application.  Any resource bindings to the MR must be done prior
to enabling the MR.

# MEMORY REGION ATTRIBUTES

Memory regions are created using the following attributes.  The struct
fi_mr_attr is passed into fi_mr_regattr, but individual fields also
apply to other memory registration calls, with the fields passed directly
into calls as function parameters.

```c
struct fi_mr_attr {
    union {
        const struct iovec *mr_iov;
        const struct fi_mr_dmabuf *dmabuf;
    };
    size_t             iov_count;
    uint64_t           access;
    uint64_t           offset;
    uint64_t           requested_key;
    void               *context;
    size_t             auth_key_size;
    uint8_t            *auth_key;
    enum fi_hmem_iface iface;
    union {
        uint64_t       reserved;
        int            cuda;
        int            ze
        int            neuron;
        int            synapseai;
    } device;
    void               *hmem_data;
    size_t             page_size;
    const struct fid_mr *base_mr;
    size_t             sub_mr_cnt;
};

struct fi_mr_auth_key {
    struct fid_av *av;
    fi_addr_t     src_addr;
};
```
## mr_iov

This is an IO vector of virtual addresses and their length that represent
a single memory region.  The number of entries in the iovec is specified
by iov_count.

## dmabuf

DMA-buf registrations are used to share device memory between a given
device and the fabric NIC and does not require that the device memory
be mmap'ed into the virtual address space of the calling process.

This structure references a DMA-buf backed device memory region.  This field
is only usable if the application has successfully requested support for
FI_HMEM and the FI_MR_DMABUF flag is passed into the memory registration
call. DMA-buf regions are file-based references to device memory.  Such
regions are identified through the struct fi_mr_dmabuf.

```c
struct fi_mr_dmabuf {
	int      fd;
	uint64_t offset;
	size_t   len;
	void     *base_addr;
};
```
The fd is the file descriptor associated with the DMA-buf region.  The
offset is the offset into the region where the memory registration should
begin.  And len is the size of the region to register, starting at the
offset. The base_addr is the page-aligned starting virtual address of
the memory region allocated by the DMA-buf. If a base virtual address
is not available (because, for example, the calling process has not
mapped the memory region into its address space), base_addr can be
set to NULL.

The selection of dmabuf over the mr_iov field is controlled by
specifying the FI_MR_DMABUF flag.

## iov_count

The number of entries in the mr_iov array.  The maximum number of memory
buffers that may be associated with a single memory region is specified
as the mr_iov_limit domain attribute.  See `fi_domain(3)`.

## access

Indicates the type of _operations_ that the local or a peer endpoint may
perform on registered memory region.  Supported access permissions are the
bitwise OR of the following flags:

*FI_SEND*
: The memory buffer may be used in outgoing message data transfers.  This
  includes fi_msg and fi_tagged send operations, as well as fi_collective
  operations.

*FI_RECV*
: The memory buffer may be used to receive inbound message transfers.
  This includes fi_msg and fi_tagged receive operations, as well as
  fi_collective operations.

*FI_READ*
: The memory buffer may be used as the result buffer for RMA read
  and atomic operations on the initiator side.  Note that from the
  viewpoint of the application, the memory buffer is being written
  into by the network.

*FI_WRITE*
: The memory buffer may be used as the source buffer for RMA write
  and atomic operations on the initiator side.  Note that from the
  viewpoint of the application, the endpoint is reading from the memory
  buffer and copying the data onto the network.

*FI_REMOTE_READ*
: The memory buffer may be used as the source buffer of an RMA read
  operation on the target side.  The contents of the memory buffer
  are not modified by such operations.

*FI_REMOTE_WRITE*
: The memory buffer may be used as the target buffer of an RMA write
  or atomic operation.  The contents of the memory buffer may be
  modified as a result of such operations.

*FI_COLLECTIVE*
: This flag provides an explicit indication that the memory buffer may
  be used with collective operations.  Use of this flag is required if
  the FI_MR_COLLECTIVE mr_mode bit has been set on the domain.  This flag
  should be paired with FI_SEND and/or FI_RECV

Note that some providers may not enforce fine grained access permissions.
For example, a memory region registered for FI_WRITE access may also
behave as if FI_SEND were specified as well.  Relaxed enforcement of
such access is permitted, though not guaranteed, provided security is
maintained.

## offset

The offset field is reserved for future use and must be 0.

## requested_key

An application specified access key associated with the memory region.
The MR key must be provided by a remote process when performing RMA
or atomic operations to a memory region.  Applications
can use the requested_key field to indicate that a specific key be
used by the provider.  This allows applications to use well known key
values, which can avoid applications needing to exchange and store keys.
Support for user requested keys is provider specific and is determined
by the the FI_MR_PROV_KEY flag in the mr_mode domain attribute field.
Depending on the provider, the user requested key may be ignored if the
memory region is for local access only.  A provider may be unable to
do so if the hardware supports user requested keys and the same key is
used for both local and remote access.

## context

Application context associated with asynchronous memory registration
operations.  This value is returned as part of any asynchronous event
associated with the registration.  This field is ignored for synchronous
registration calls.

## auth_key_size

The size of key referenced by the auth_key field in bytes, or 0 if no authorization
key is given.  This field is ignored unless the fabric is opened with API
version 1.5 or greater.

If the domain is opened with FI_AV_AUTH_KEY, auth_key_size must equal
`sizeof(struct fi_mr_auth_key)`.

## auth_key

Indicates the key to associate with this memory registration.  Authorization
keys are used to limit communication between endpoints.  Only peer endpoints
that are programmed to use the same authorization key may access the memory
region.  The domain authorization key will be used if the auth_key_size
provided is 0.  This field is ignored unless the fabric is opened with API
version 1.5 or greater.

If the domain is opened with FI_AV_AUTH_KEY, auth_key must point to a
user-defined `struct fi_mr_auth_key`.

## iface
Indicates the software interfaces used by the application to allocate and
manage the memory region. This field is ignored unless the application has
requested the FI_HMEM capability.

*FI_HMEM_SYSTEM*
: Uses standard operating system calls and libraries, such as malloc,
  calloc, realloc, mmap, and free.  When iface is set to FI_HMEM_SYSTEM,
  the device field (described below) is ignored.

*FI_HMEM_CUDA*
: Uses Nvidia CUDA interfaces such as cuMemAlloc, cuMemAllocHost,
  cuMemAllocManaged, cuMemFree, cudaMalloc, cudaFree.

*FI_HMEM_ROCR*
: Uses AMD ROCR interfaces such as hsa_memory_allocate and hsa_memory_free.

*FI_HMEM_ZE*
: Uses oneAPI Level Zero interfaces such as zeDriverAllocSharedMem,
  zeDriverFreeMem.

*FI_HMEM_NEURON*
: Uses the AWS Neuron SDK to support AWS Trainium devices.

*FI_HMEM_SYNAPSEAI*
: Uses the SynapseAI API to support Habana Gaudi devices.

## device
Reserved 64 bits for device identifier if using non-standard HMEM interface.
This field is ignore unless the iface field is valid.  Otherwise, the device
field is determined by the value specified through iface.

*cuda*
: For FI_HMEM_CUDA, this is equivalent to CUdevice (int).

*ze*
: For FI_HMEM_ZE, this is equivalent to the index of the device in the
  ze_device_handle_t array.  If there is only a single level zero driver
  present, an application may set this directly.  However, it is
  recommended that this value be set using the fi_hmem_ze_device() macro,
  which will encode the driver index with the device.

*neuron*
: For FI_HMEM_NEURON, the device identifier for AWS Trainium devices.

*synapseai*
: For FI_HMEM_SYNAPSEAI, the device identifier for Habana Gaudi hardware.

## hmem_data
The hmem_data field is reserved for future use and must be null.

## page_size
Page size allows applications to optionally provide a hint at what the
optimal page size is for the an MR allocation. Typically, providers can
select the optimal page size. In cases where VA range has zero pages
backing it, which is supported with FI_MR_ALLOCATED unset, the provider
may not know the optimal page size during registration. Rather than use
a less efficient page size, this attribute allows applications to specify
the page size to be used.

If page size is zero, provider will select the page size.

If non-zero, page size must be supported by OS. If a specific page size
is specified for a memory region during creation, all pages later
associated with the region must be of the given size. Attaching a memory
page of a different size to a region may result in failed transfers to
or from the region.

Providers may choose to ignore page size. This will result in a provider
selected page size always being used.

## base_mr

If non-NULL, create a sub-MR from an existing memory region specified by
the base_mr field.

The sub-MR must be fully contained within the base MR; however, the sub-MR
has its own authorization keys and access rights.  The following attributes
are inherited from the base MR, and as a result, are ignored when creating the
sub-MR:

iface, device, hmem_data, page_size

The sub-MR should hold a reference to the base MR. When fi_close is called
on the base MR, the call would fail if there are any outstanding sub-MRs.

The base_mr field must be NULL if the FI_MR_DMABUF flag is set.

## sub_mr_cnt

The number of sub-MRs expected to be created from the memory region.  This
value is not a limit.  Instead, it is a hint to the provider to allow provider
specific optimization for sub-MR creation.  For example, the provider may
reserve access keys or pre-allocation fid_mr objects.  The provider may
ignore this hint.

## fi_hmem_ze_device

Returns an hmem device identifier for a level zero <driver, device> tuple.
The output of this call should be used to set fi_mr_attr::device.ze for
FI_HMEM_ZE interfaces.  The driver and device index values represent
their 0-based positions in arrays returned from zeDriverGet and zeDeviceGet,
respectively.

## av

For memory registration being allocated against a domain configured with
FI_AV_AUTH_KEY, av is used to define the fid_av which contains the authorization
keys to be associated with the memory region. If the domain is also opened with
FI_MR_ENDPOINT, the specified AV must be the same AV bound to the endpoint.

By default, the memory region will be associated with all authorization keys
in the AV.

## addr

If the domain was opened with FI_DIRECTED_RECV, addr can be used to limit the
memory region to a specific fi_addr_t, including fi_addr_t's return from `fi_av_insert_auth_key`.

# NOTES

Direct access to an application's memory by a remote peer requires that
the application register the targeted memory buffer(s).  This is typically
done by calling one of the fi_mr_reg* routines.  For FI_MR_PROV_KEY, the
provider will return a key that must be used by the peer when accessing
the memory region.  The application is responsible for transferring this
key to the peer.  If FI_MR_RAW mode has been set, the key must be retrieved
using the fi_mr_raw_attr function.

FI_MR_RAW allows support for providers that require more than 8-bytes for
their protection keys or need additional setup before a key can
be used for transfers.  After a raw key has been retrieved, it must be
exchanged with the remote peer.  The peer must use fi_mr_map_raw to convert
the raw key into a usable 64-bit key.  The mapping must be done even if
the raw key is 64-bits or smaller.

The raw key support functions are usable with all registered memory
regions, even if FI_MR_RAW has not been set.  It is recommended that
portable applications target using those interfaces; however, their use
does carry extra message and memory footprint overhead, making it less
desirable for highly scalable apps.

There may be cases where device peer to peer support should not be used or
cannot be used, such as when the PCIe ACS configuration does not permit the
transfer. The FI_HMEM_DISABLE_P2P environment variable can be set to notify
Libfabric that peer to peer transactions should not be used. The provider may
choose to perform a copy instead, or will fail support for FI_HMEM if the
provider is unable to do that.

# FLAGS

The follow flag may be specified to any memory registration call.

*FI_RMA_EVENT*
: This flag indicates that the specified memory region will be
  associated with a completion counter used to count RMA operations
  that access the MR.

*FI_RMA_PMEM*
: This flag indicates that the underlying memory region is backed by
  persistent memory and will be used in RMA operations.  It must be
  specified if persistent completion semantics or persistent data transfers
  are required when accessing the registered region.

*FI_HMEM_DEVICE_ONLY*
: This flag indicates that the memory is only accessible by a device. Which
  device is specified by the fi_mr_attr fields iface and device. This refers
  to memory regions that were allocated using a device API AllocDevice call
  (as opposed to using the host allocation or unified/shared memory allocation).
  This flag is only usable for domains opened with FI_HMEM capability support.

*FI_HMEM_HOST_ALLOC*
: This flag indicates that the memory is owned by the host only. Whether it
  can be accessed by the device is implementation dependent. The fi_mr_attr
  field iface is still used to identify the device API, but the field device
  is ignored. This refers to memory regions that were allocated using a device
  API AllocHost call (as opposed to using malloc-like host allocation,
  unified/shared memory allocation, or AllocDevice).  This flag is only usable
  for domains opened with FI_HMEM capability support.

*FI_MR_DMABUF*
: This flag indicates that the memory region to registered is a DMA-buf backed
  region.  When set, the region is specified through the dmabuf field of the
  fi_mr_attr structure.  This flag is only usable for domains opened with
  FI_HMEM capability support.

*FI_MR_SINGLE_USE*
: This flag indicates that the memory region is only used for a single
  operation.  After the operation is complete, the key associated with the
  memory region is automatically invalidated and can no longer be used for
  remote access.

*FI_AUTH_KEY*
: Only valid with domains configured with FI_AV_AUTH_KEY. When used with
  fi_mr_regattr, this flag denotes that the fi_mr_auth_key::src_addr field
  contains an authorization key fi_addr_t (i.e. fi_addr_t returned from
  fi_av_insert_auth_key) instead of an endpoint fi_addr_t (i.e. fi_addr_t
  return from fi_av_insert / fi_av_insertsvc / fi_av_remove).

# MEMORY DOMAINS

Memory domains identify the physical separation of memory which
may or may not be accessible through the same virtual address space.
Traditionally, applications only dealt with a single memory domain,
that of host memory tightly coupled with the system CPUs.  With
the introduction of device and non-uniform memory subsystems,
applications often need to be aware of which memory domain a particular
virtual address maps to.

As a general rule, separate physical devices can be considered to have
their own memory domains.  For example, a NIC may have user accessible
memory, and would be considered a separate memory domain from memory
on a GPU.  Both the NIC and GPU memory domains are separate from host
system memory.  Individual GPUs or computation accelerators may have
distinct memory domains, or may be connected in such a way (e.g. a GPU
specific fabric) that all GPUs would belong to the same memory domain.
Unfortunately, identifying memory domains is specific to each
system and its physical and/or virtual configuration.

Understanding memory domains in heterogenous memory environments is
important as it can impact data ordering and visibility as viewed
by an application.  It is also important to understand which memory
domain an application is most tightly coupled to.  In most cases,
applications are tightly coupled to host memory.  However, an
application running directly on a GPU or NIC may be more tightly
coupled to memory associated with those devices.

Memory regions are often associated with a single memory domain.
The domain is often indicated by the fi_mr_attr iface and device
fields.  Though it is possible for physical pages backing a virtual
memory region to migrate between memory domains based on access patterns.
For example, the physical pages referenced by a virtual address range
could migrate between host memory and GPU memory, depending on which
computational unit is actively using it.

See the [`fi_endpoint`(3)](fi_endpoint.3.html) and [`fi_cq`(3)](fi_cq.3.html)
man pages for addition discussion on message, data, and completion ordering
semantics, including the impact of memory domains.

# RETURN VALUES

Returns 0 on success.  On error, a negative value corresponding to
fabric errno is returned.

Fabric errno values are defined in
`rdma/fi_errno.h`.

# ERRORS

*-FI_ENOKEY*
: The requested_key is already in use.

*-FI_EKEYREJECTED*
: The requested_key is not available.  They key may be out of the
  range supported by the provider, or the provider may not support
  user-requested memory registration keys.

*-FI_ENOSYS*
: Returned by fi_mr_bind if the provider does not support reporting
  events based on access to registered memory regions.

*-FI_EBADFLAGS*
: Returned if the specified flags are not supported by the provider.

# MEMORY REGISTRATION CACHE

Many hardware NICs accessed by libfabric require that data buffers be
registered with the hardware while the hardware accesses it.  This ensures
that the virtual to physical address mappings for those buffers do not change
while the transfer is occurring.  The performance impact of registering
memory regions can be significant.  As a result, some providers make use
of a registration cache, particularly when working with applications that
are unable to manage their own network buffers.  A registration cache avoids
the overhead of registering and unregistering a data buffer with each
transfer.

If a registration cache is going to be used for host and device memory, the
device must support unified virtual addressing. If the device does not
support unified virtual addressing, either an additional registration cache
is required to track this device memory, or device memory cannot be cached.

As a general rule, if hardware requires the FI_MR_LOCAL mode bit described
above, but this is not supported by the application, a memory registration
cache _may_ be in use.  The following environment variables may be used to
configure registration caches.

*FI_MR_CACHE_MAX_SIZE*
: This defines the total number of bytes for all memory regions that may
  be tracked by the cache.  If not set, the cache has no limit on how many
  bytes may be registered and cached.  Setting this will reduce the
  amount of memory that is not actively being used as part of a data transfer
  that is registered with a provider.  By default, the cache size is
  unlimited.

*FI_MR_CACHE_MAX_COUNT*
: This defines the total number of memory regions that may be registered with
  the cache.  If not set, a default limit is chosen.  Setting this will reduce
  the number of regions that are registered, regardless of their size, which
  are not actively being used as part of a data transfer.  Setting this to
  zero will disable registration caching.

*FI_MR_CACHE_MONITOR*
: The cache monitor is responsible for detecting system memory (FI_HMEM_SYSTEM)
  changes made between the virtual addresses used by an application and the
  underlying physical pages. Valid monitor options are: userfaultfd, memhooks,
  kdreg2, and disabled.  Selecting disabled will turn off the registration cache.
  Userfaultfd is a Linux kernel feature used to report virtual to physical
  address mapping changes to user space. Memhooks operates by intercepting
  relevant memory allocation and deallocation calls which may result in the
  mappings changing, such as malloc, mmap, free, etc.  Note that memhooks
  operates at the elf linker layer, and does not use glibc memory hooks. Kdreg2
  is supplied as a loadable Linux kernel module.

*FI_MR_CUDA_CACHE_MONITOR_ENABLED*
: The CUDA cache monitor is responsible for detecting CUDA device memory
  (FI_HMEM_CUDA) changes made between the device virtual addresses used by an
  application and the underlying device physical pages. Valid monitor options
  are: 0 or 1. Note that the CUDA memory monitor requires a CUDA toolkit version
  with unified virtual addressing enabled.

*FI_MR_ROCR_CACHE_MONITOR_ENABLED*
: The ROCR cache monitor is responsible for detecting ROCR device memory
  (FI_HMEM_ROCR) changes made between the device virtual addresses used by an
  application and the underlying device physical pages. Valid monitor options
  are: 0 or 1. Note that the ROCR memory monitor requires a ROCR version with
  unified virtual addressing enabled.

*FI_MR_ZE_CACHE_MONITOR_ENABLED*
: The ZE cache monitor is responsible for detecting oneAPI Level Zero device
  memory (FI_HMEM_ZE) changes made between the device virtual addresses used by
  an application and the underlying device physical pages. Valid monitor options
  are: 0 or 1.

More direct access to the internal registration cache is possible through the
fi_open() call, using the "mr_cache" service name.  Once opened, custom
memory monitors may be installed.  A memory monitor is a component of the cache
responsible for detecting changes in virtual to physical address mappings.
Some level of control over the cache is possible through the above mentioned
environment variables.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_rma`(3)](fi_rma.3.html),
[`fi_msg`(3)](fi_msg.3.html),
[`fi_atomic`(3)](fi_atomic.3.html)
