#!/usr/bin/env bash

modules=(
    libfabric
    fabtests
)

rootdir=$HOME

declare -A srcdir

srcdir[libfabric]=$rootdir/libfabric
srcdir[fabtests]=${srcdir[libfabric]}/fabtests

git_desc=$(cd ${srcdir[libfabric]} && git describe)
prefix=$rootdir/install/$git_desc

declare -A conf_opts=(
    [libfabric]=$(echo --enable-efa --disable-{verbs,rxd,opx,rstream})
    [fabtests]="--with-libfabric=$prefix"
)

build_rootdir=$rootdir/build/$git_desc

mkdir -p $(dirname $build_rootdir)
mkdir -p $build_rootdir

clean() (
    cd ${srcdir[$1]} && git clean -xdf
)

autogen() (
    cd ${srcdir[$1]} && ./autogen.sh
)

build() (
    local module=$1
    local builddir=$build_rootdir/$module

    mkdir -p $builddir && \
    cd $builddir && \
    ${srcdir[$module]}/configure \
        --prefix=$prefix \
        --with-cuda=/usr/local/cuda \
        ${conf_opts[$module]} && \
    make -j install
)

clean libfabric || exit $?

for module in ${modules[@]}; do
    autogen $module || exit $?
    build   $module || exit $?
done

base_cmd="FI_EFA_USE_DEVICE_RDMA=0 timeout 360 $prefix/bin/fi_rdm_pingpong -p efa -S all -m -E --pin-core 1-2"
pids=()
ssh $server "$base_cmd -D cuda" & pids+=($!)
sleep 5
ssh $client "$base_cmd $server -D cuda" & pids+=($!)
for pid in ${pids[*]}; do
    if ! wait $pid; then
        echo "[$git_desc] Failure detected"
        exit 1
    fi
done
echo "[$git_desc] Success!"
exit 0
