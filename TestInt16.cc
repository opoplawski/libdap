
// (c) COPYRIGHT URI/MIT 1999
// Please read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//      jhrg,jimg       James Gallagher (jgallagher@gso.uri.edu)

// Implementation for TestInt16. See TestByte.cc
//
// 3/22/99 jhrg

// $Log: TestInt16.cc,v $
// Revision 1.4  2000/08/02 22:46:49  jimg
// Merged 3.1.8
//
// Revision 1.3.6.1  2000/08/02 20:58:26  jimg
// Included the header config_dap.h in this file. config_dap.h has been
// removed from all of the DODS header files.
//
// Revision 1.3  1999/05/04 19:47:22  jimg
// Fixed copyright statements. Removed more of the GNU classes.
//
// Revision 1.2  1999/04/29 02:29:32  jimg
// Merge of no-gnu branch
//
// Revision 1.1  1999/03/24 23:40:06  jimg
// Added
//

#ifdef __GNUG__
#pragma implementation
#endif

#include "config_dap.h"
#include "TestInt16.h"

Int16 *
NewInt16(const string &n)
{
    return new TestInt16(n);
}

TestInt16::TestInt16(const string &n) : Int16(n)
{
}

BaseType *
TestInt16::ptr_duplicate()
{
    return new TestInt16(*this);
}

bool
TestInt16::read(const string &, int &)
{
    if (read_p())
	return true;

    _buf = 32000;

    set_read_p(true);
    
    return true;
}
