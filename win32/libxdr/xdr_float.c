/* @(#)xdr_float.c	2.1 88/07/29 4.0 RPCSRC */
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 * 
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 * 
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)xdr_float.c 1.12 87/08/11 Copyr 1984 Sun Micro";
#endif

/*
 * xdr_float.c, Generic XDR routines impelmentation.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * These are the "floating point" xdr routines used to (de)serialize
 * most common data items.  See xdr.h for more info on the interface to
 * xdr.
 */

#include <stdio.h>
#include <xdr.h>

#define TRUE	1
#define FALSE	0

bool_t
xdr_float(register XDR *xdrs, register float *fp)
{
	switch (xdrs->x_op) {

	case XDR_ENCODE:
		return (XDR_PUTLONG(xdrs, (long *)fp));
	case XDR_DECODE:
		return (XDR_GETLONG(xdrs, (long *)fp));
	case XDR_FREE:
		return (TRUE);
	}
	return (FALSE);
}

bool_t
xdr_double(register XDR *xdrs, double *dp)
{
	register long *lp;

	switch (xdrs->x_op) {

	case XDR_ENCODE:
		lp = (long *)dp;
		return (XDR_PUTLONG(xdrs, lp++) && XDR_PUTLONG(xdrs, lp));
	case XDR_DECODE:
		lp = (long *)dp;
		return (XDR_GETLONG(xdrs, lp++) && XDR_GETLONG(xdrs, lp));
	case XDR_FREE:
		return (TRUE);
	}
	return (FALSE);
}