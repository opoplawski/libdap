
// (c) COPYRIGHT URI/MIT 1999
// Please read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//      jhrg,jimg       James Gallagher (jgallagher@gso.uri.edu)

// Implementation for Int32.
//
// jhrg 9/7/94

// $Log: UInt16.cc,v $
// Revision 1.10  2000/07/09 22:05:36  rmorris
// Changes to increase portability, minimize ifdef's for win32 and account
// for differences in the iostreams implementations.
//
// Revision 1.9  2000/06/07 18:06:59  jimg
// Merged the pc port branch
//
// Revision 1.8.20.1  2000/06/02 18:29:31  rmorris
// Mod's for port to Win32.
//
// Revision 1.8  1999/04/29 02:29:34  jimg
// Merge of no-gnu branch
//
// Revision 1.7  1999/03/24 23:37:15  jimg
// Added support for the Int16, UInt16 and Float32 types
//
// Revision 1.6.6.2  1999/02/05 09:32:35  jimg
// Fixed __unused__ so that it not longer clashes with Red Hat 5.2 inlined
// math code. 
//
// Revision 1.6.6.1  1999/02/02 21:57:04  jimg
// String to string version
//
// Revision 1.6  1998/03/19 23:30:29  jimg
// Removed old code (that was surrounded by #if 0 ... #endif).
//
// Revision 1.5  1997/09/22 22:40:54  jimg
// Added DDS * to deserialize parameters.
//
// Revision 1.4  1996/12/02 23:10:29  jimg
// Added dataset as a parameter to the ops member function.
//
// Revision 1.3  1996/10/18 16:49:55  jimg
// Changed variable types used for comparison operations from 16 to 32 bit
// integers.
//
// Revision 1.2  1996/10/01 16:29:16  jimg
// Changed instances of XDT_INT16 to XDR_UINT16.
//
// Revision 1.1  1996/08/26 20:17:49  jimg
// Added.
//

#ifdef __GNUG__
#pragma implementation
#endif

#include "config_dap.h"

static char rcsid[] not_used = {"$Id: UInt16.cc,v 1.10 2000/07/09 22:05:36 rmorris Exp $"};

#include <stdlib.h>
#include <assert.h>

#include "UInt16.h"
#include "DDS.h"
#include "util.h"
#include "parser.h"
#include "expr.tab.h"
#include "Operators.h"
#include "dods-limits.h"
#include "debug.h"

#ifdef TRACE_NEW
#include "trace_new.h"
#endif

#ifdef WIN32
#include <xdr.h>
using std::cerr;
using std::endl;
#endif

UInt16::UInt16(const string &n) 
    : BaseType(n, dods_uint16_c, (xdrproc_t)XDR_UINT16)
{
}

unsigned int
UInt16::width()
{
    return sizeof(dods_uint16);
}

bool
UInt16::serialize(const string &dataset, DDS &dds, XDR *sink,
		  bool ce_eval)
{
    int error;

    if (!read_p() && !read(dataset, error))
	return false;

    if (ce_eval && !dds.eval_selection(dataset))
	return true;

    if (!XDR_UINT16(sink, &_buf))
	return false;

    return true;
}

bool
UInt16::deserialize(XDR *source, DDS *, bool)
{
    unsigned int num = XDR_UINT16(source, &_buf);

    return (num > 0);		/* make the return value a boolean */
}

unsigned int
UInt16::val2buf(void *val, bool)
{
    assert(val);

    _buf = *(dods_uint16 *)val;

    return width();
}

unsigned int
UInt16::buf2val(void **val)
{
    assert(_buf && val);

    if (!*val)
	*val = new dods_uint16;

    *(dods_uint16 *)*val =_buf;

    return width();
}

// Print BUF to stdout with its declaration. Intended mostly for debugging.

void 
UInt16::print_val(ostream &os, string space, bool print_decl_p)
{
    if (print_decl_p) {
	print_decl(os, space, false);
	os << " = " << _buf << ";" << endl;
    }
    else 
	os << _buf;
}

bool
UInt16::ops(BaseType *b, int op, const string &dataset)
{
    int error = 0;

    // Extract the Byte arg's value.
    if (!read_p() && (!read(dataset, error) || error)) {
	assert("This value not read!" && false);
	cerr << "This value not read!" << endl;
	return false;
    }

    // Extract the second arg's value.
    if (!b->read_p() && (!b->read(dataset, error) || error)) {
	assert("This value not read!" && false);
	cerr << "This value not read!" << endl;
	return false;
    }

    switch (b->type()) {
      case dods_byte_c:
	return rops<dods_uint16, dods_byte, Cmp<dods_uint16, dods_byte> >
	    (_buf, dynamic_cast<Byte *>(b)->_buf, op);
      case dods_int16_c:
	return rops<dods_uint16, dods_int16, USCmp<dods_uint16, dods_int16> >
	    (_buf, dynamic_cast<Int16 *>(b)->_buf, op);
      case dods_uint16_c:
	return rops<dods_uint16, dods_uint16, Cmp<dods_uint16, dods_uint16> >
	    (_buf, dynamic_cast<UInt16 *>(b)->_buf, op);
      case dods_int32_c:
	return rops<dods_uint16, dods_int32, USCmp<dods_uint16, dods_int32> >
	    (_buf, dynamic_cast<Int32 *>(b)->_buf, op);
      case dods_uint32_c:
	return rops<dods_uint16, dods_uint32, Cmp<dods_uint16, dods_uint32> >
	    (_buf, dynamic_cast<UInt32 *>(b)->_buf, op);
      case dods_float32_c:
	return rops<dods_uint16, dods_float32, Cmp<dods_uint16, dods_float32> >
	    (_buf, dynamic_cast<Float32 *>(b)->_buf, op);
      case dods_float64_c:
	return rops<dods_uint16, dods_float64, Cmp<dods_uint16, dods_float64> >
	    (_buf, dynamic_cast<Float64 *>(b)->_buf, op);
      default:
	return false;
    }
}
