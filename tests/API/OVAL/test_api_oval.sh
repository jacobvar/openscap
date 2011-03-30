#!/usr/bin/env bash

# Copyright 2009 Red Hat Inc., Durham, North Carolina.
# All Rights Reserved.
#
# OpenScap OVAL Module Test Suite.
#
# Created on: Nov 30, 2009
#
# Authors:
#      Peter Vrabec, <pvrabec@redhat.com>
#      David Niemoller
#      Ondrej Moris, <omoris@redhat.com>

. ${srcdir}/../../test_common.sh


# Test Cases.

function test_api_oval_definition {
    ./test_api_oval ${srcdir}/scap-rhel5-oval.xml
    return $?
}

function test_api_oval_syschar {
    ./test_api_syschar $srcdir/composed-oval.xml \
	$srcdir/system-characteristics.xml
    return $?
}

function test_api_oval_results {
    ./test_api_results $srcdir/scap-rhel5-oval.xml $srcdir/results.xml
    return $?
}

# Testing.

test_init "test_api_oval.log"

test_run "test_api_oval_definition" test_api_oval_definition
test_run "test_api_oval_syschar" test_api_oval_syschar
test_run "test_api_oval_results" test_api_oval_results

test_exit