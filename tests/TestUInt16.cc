
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.
 
// (c) COPYRIGHT URI/MIT 1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      jhrg,jimg       James Gallagher <jgallagher@gso.uri.edu>

// Implementation for TestUInt16. See TestByte.cc
//
// 3/22/99 jhrg


#include "config.h"

#ifndef WIN32
#include <unistd.h>
#else
#include <io.h>
#include <fcntl.h>
#include <process.h>
#endif

#include "TestUInt16.h"

extern int test_variable_sleep_interval;

void
TestUInt16::_duplicate(const TestUInt16 &ts)
{
    d_series_values = ts.d_series_values;
}


TestUInt16::TestUInt16(const string &n, const string &ds)
    : UInt16(n, ds), d_series_values(false)
{
    _buf = 1;
}

TestUInt16::TestUInt16(const TestUInt16 &rhs) : UInt16(rhs), TestCommon(rhs)
{
    _duplicate(rhs);
}

TestUInt16 &
TestUInt16::operator=(const TestUInt16 &rhs)
{
    if (this == &rhs)
	return *this;

    dynamic_cast<UInt16 &>(*this) = rhs; // run Constructor=

    _duplicate(rhs);

    return *this;
}


BaseType *
TestUInt16::ptr_duplicate()
{
    return new TestUInt16(*this);
}

bool
TestUInt16::read(const string &)
{
    if (read_p())
	return true;

    if (test_variable_sleep_interval > 0)
	sleep(test_variable_sleep_interval);

    if (get_series_values()) {
        _buf = (short)(16 * _buf);
    }
    else {
        _buf = 64000;
    }
    
    set_read_p(true);
    
    return true;
}
