#!/usr/bin/env sh

sudo setcap "cap_ipc_lock=ep" build/Release/core
