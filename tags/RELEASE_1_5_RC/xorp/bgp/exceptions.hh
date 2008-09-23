// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2008 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

// $XORP: xorp/bgp/exceptions.hh,v 1.12 2007/02/16 22:45:12 pavlin Exp $

#ifndef __BGP_EXCEPTIONS_HH__
#define __BGP_EXCEPTIONS_HH__

#include "libxorp/xorp.h"
#include "libxorp/exceptions.hh"

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif


/**
 * This exception is thrown when a bad input message is received.
 */
class CorruptMessage : public XorpReasonedException {
public:
    CorruptMessage(const char* file, size_t line, const string init_why = "")
 	: XorpReasonedException("CorruptMessage", file, line, init_why),
	  _error(0), _subcode(0), _data(0), _len(0)
    {}

    CorruptMessage(const char* file, size_t line,
		   const string init_why,
		   const int error, const int subcode)
 	: XorpReasonedException("CorruptMessage", file, line, init_why),
	  _error(error), _subcode(subcode), _data(0), _len(0)
    {}

    CorruptMessage(const char* file, size_t line,
		   const string init_why,
		   const int error, const int subcode,
		   const uint8_t *data, const size_t len)
 	: XorpReasonedException("CorruptMessage", file, line, init_why),
	  _error(error), _subcode(subcode), _data(data), _len(len)
    {}

    int error() const				{ return _error; }
    int subcode() const				{ return _subcode; }
    const uint8_t *data() const			{ return _data; }
    size_t len() const				{ return _len; }

private:
    const int		_error;
    const int		_subcode;
    const uint8_t *	_data;
    const size_t	_len;
};

class NoFinder : public XorpReasonedException {
public:
    NoFinder(const char* file, size_t line, const string init_why = "")
 	: XorpReasonedException("NoFinder", file, line, init_why)
    {}
};

#endif // __BGP_EXCEPTIONS_HH__