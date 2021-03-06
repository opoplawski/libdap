
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

// (c) COPYRIGHT URI/MIT 1995-1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      jhrg,jimg       James Gallagher <jgallagher@gso.uri.edu>

// This is the interface definition file for the abstract class
// Vector. Vector is the parent class for List and Array.

#ifndef _vector_h
#define _vector_h 1

#ifndef _basetype_h
#include "BaseType.h"
#endif

#ifndef _dds_h
#include "DDS.h"
#endif

#ifndef constraint_evaluator_h
#include "ConstraintEvaluator.h"
#endif

namespace libdap
{

/** Holds a one-dimensional array of DAP2 data types.  This class
    takes two forms, depending on whether the elements of the vector
    are themselves simple or compound objects. This class contains
    common functionality for the List and Array classes, and should
    rarely be used directly.

    When each element of the class is a simple data type, the Vector
    is implemented as a simple array of C types, rather than as an
    array of BaseType data types.  A single private ``template''
    BaseType instance (<tt>_var</tt>) is used to hold information in common
    to all the members of the array.  The template is also used as a
    container to pass values back and forth to an application
    program, as in <tt>var()</tt>.

    If the elements of the vector are themselves compound data
    types, the array is stored as a vector of BaseType pointers (see
    the libdap class <b>BaseTypePtrVec</b>). The template is still used to
    hold information in common to all the members of the array, but
    is not used to pass information to and from the application
    program.

    @brief Holds a one-dimensional collection of DAP2 data types.
    @see BaseType
    @see Array
*/
class Vector: public BaseType
{
private:
    int d_length;  // number of elements in the vector
    BaseType *_var;  // base type of the Vector

    // _buf was a pointer to void; delete[] complained. 6/4/2001 jhrg
    char *_buf;   // array which holds cardinal data
    vector<string> d_str;       // special storage for strings. jhrg 2/11/05
    vector<BaseType *> _vec; // array for other data

    // the number of elements we have allocated memory to store.
    // This should be either the sizeof(buf)/width(bool constrained = false) for cardinal data
    // or the capacity of d_str for strings or capacity of _vec.
    unsigned int _capacity;

protected:
    // This function copies the private members of Vector.
    void _duplicate(const Vector &v);

    bool m_is_cardinal_type() const;
    unsigned int m_create_cardinal_data_buffer_for_type(unsigned int numEltsOfType);
    void m_delete_cardinal_data_buffer();

    template <class CardType> void set_cardinal_values_internal(const CardType* fromArray, int numElts);

public:
    Vector(const string &n, BaseType *v, const Type &t);
    Vector(const string &n, const string &d, BaseType *v, const Type &t);
    Vector(const Vector &rhs);

    virtual ~Vector();

    Vector &operator=(const Vector &rhs);
    virtual BaseType *ptr_duplicate() = 0;

    virtual bool is_dap2_only_type();

    virtual void set_name(const std::string& name);

    virtual int element_count(bool leaves);

    virtual void set_send_p(bool state);

    virtual void set_read_p(bool state);

    virtual unsigned int width(bool constrained = false);

    virtual int length() const;

    virtual void set_length(int l);

    virtual void intern_data(ConstraintEvaluator &eval, DDS &dds);
    virtual bool serialize(ConstraintEvaluator &eval, DDS &dds,
			   Marshaller &m, bool ce_eval = true);
    virtual bool deserialize(UnMarshaller &um, DDS *dds, bool reuse = false);

    virtual unsigned int val2buf(void *val, bool reuse = false);
    virtual unsigned int buf2val(void **val);

    void set_vec(unsigned int i, BaseType *val);

    void vec_resize(int l);

    void clear_local_data();

    virtual unsigned int get_value_capacity() const;
    virtual void reserve_value_capacity(unsigned int numElements);
    virtual void reserve_value_capacity();

    virtual unsigned int set_value_slice_from_row_major_vector
      (const Vector& rowMajorData, unsigned int startElement);

    virtual bool set_value(dods_byte *val, int sz);
    virtual bool set_value(vector<dods_byte> &val, int sz);
    virtual bool set_value(dods_int16 *val, int sz);
    virtual bool set_value(vector<dods_int16> &val, int sz);
    virtual bool set_value(dods_uint16 *val, int sz);
    virtual bool set_value(vector<dods_uint16> &val, int sz);
    virtual bool set_value(dods_int32 *val, int sz);
    virtual bool set_value(vector<dods_int32> &val, int sz);
    virtual bool set_value(dods_uint32 *val, int sz);
    virtual bool set_value(vector<dods_uint32> &val, int sz);
    virtual bool set_value(dods_float32 *val, int sz);
    virtual bool set_value(vector<dods_float32> &val, int sz);
    virtual bool set_value(dods_float64 *val, int sz);
    virtual bool set_value(vector<dods_float64> &val, int sz);
    virtual bool set_value(string *val, int sz);
    virtual bool set_value(vector<string> &val, int sz);

    virtual void value(dods_byte *b) const;
    virtual void value(dods_int16 *b) const;
    virtual void value(dods_uint16 *b) const;
    virtual void value(dods_int32 *b) const;
    virtual void value(dods_uint32 *b) const;
    virtual void value(dods_float32 *b) const;
    virtual void value(dods_float64 *b) const;
    virtual void value(vector<string> &b) const;

    void value(vector<unsigned int> *index, dods_byte *b) const;
    void value(vector<unsigned int> *index, dods_int16 *b) const;
    void value(vector<unsigned int> *index, dods_uint16 *b) const;
    void value(vector<unsigned int> *index, dods_int32 *b) const;
    void value(vector<unsigned int> *index, dods_uint32 *b) const;
    void value(vector<unsigned int> *index, dods_float32 *b) const;
    void value(vector<unsigned int> *index, dods_float64 *b) const;
    void value(vector<unsigned int> *index, vector<string> &b) const;

    virtual void *value();

    virtual BaseType *var(const string &name = "", bool exact_match = true,
                          btp_stack *s = 0);
    virtual BaseType *var(const string &name, btp_stack &s);
    virtual BaseType *var(unsigned int i);

    virtual void add_var(BaseType *v, Part p = nil);
    virtual void add_var_nocopy(BaseType *v, Part p = nil);

    virtual bool check_semantics(string &msg, bool all = false);

    virtual void dump(ostream &strm) const ;
};

} // namespace libdap

#endif /* _vector_h */
