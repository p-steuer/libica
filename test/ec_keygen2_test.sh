#!/bin/sh

if lszcrypt | grep "CEX.C.*online" &> /dev/null; then
	ICAPATH=2 ./ec_keygen_test
else
	exit 77
fi
