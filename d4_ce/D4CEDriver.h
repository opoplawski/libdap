/*
 * D4CEDriver.h
 *
 *  Created on: Aug 8, 2013
 *      Author: jimg
 */

#ifndef D4CEDRIVER_H_
#define D4CEDRIVER_H_

#include <string>
#include <vector>

namespace libdap {

class location;
class DMR;
class BaseType;
class D4Dimension;

/**
 * Driver for the DAP4 Constraint Expression parser.
 */
class D4CEDriver {
	struct index {
		// start and stride are simple numbers; stop is either the stopping index or
		// if to_end is true, is ignored and the subset runs to the end of the dimension
		unsigned long long start, stride, stop;
		bool rest;

		// Added because the parser code needs it. Our code does not use this. jhrg 11/26/13
		index(): start(0), stride(0), stop(0), rest(false) {}
		index(unsigned long long i, unsigned long long s, unsigned long long e, bool r)
			: start(i), stride(s), stop(e), rest(r) {}
	};

	index make_index() { return index(0, 1, 0, true); }

	index make_index(const std::string &is);

	index make_index(const std::string &i, const std::string &s, const std::string &e);
	index make_index(const std::string &i, unsigned long long s, const std::string &e);

	index make_index(const std::string &i, const std::string &s);
	index make_index(const std::string &i, unsigned long long s);

	bool d_trace_scanning;
	bool d_trace_parsing;
	bool d_result;
	std::string d_expr;

	DMR *d_dmr;

	std::vector<index> d_indexes;

	// d_expr should be set by parse! Its value is used by the parser right before
	// the actual parsing operation starts. jhrg 11/26/13
	std::string *expression() { return &d_expr; }
	BaseType *mark_variable(const std::string &id);
	BaseType *mark_array_variable(const std::string &id);
	D4Dimension *slice_dimension(const std::string &id, const index &i);

	void push_index(const index &i) { d_indexes.push_back(i); }

	friend class D4CEParser;

public:
	D4CEDriver() : d_trace_scanning(false), d_trace_parsing(false), d_result(false), d_expr(""), d_dmr(0) { }
	D4CEDriver(DMR *dmr) : d_trace_scanning(false), d_trace_parsing(false), d_result(false), d_expr(""), d_dmr(dmr) { }

	virtual ~D4CEDriver() { }

	bool parse(const std::string &expr);

	bool trace_scanning() const { return d_trace_scanning; }
	void set_trace_scanning(bool ts) { d_trace_scanning = ts; }

	bool trace_parsing() const { return d_trace_parsing; }
	void set_trace_parsing(bool tp) { d_trace_parsing = tp; }

	bool result() const { return d_result; }
	void set_result(bool r) { d_result = r; }

	DMR *dmr() const { return d_dmr; }
	void set_dmr(DMR *dmr) { d_dmr = dmr; }

	void error(const libdap::location &l, const std::string &m);
};

} /* namespace libdap */
#endif /* D4CEDRIVER_H_ */
