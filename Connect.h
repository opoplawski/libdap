
// -*- C++ -*-

// (c) COPYRIGHT URI/MIT 1994-1999
// Please first read the full copyright statement in the file COPYRIGHT.
//
// Authors:
//	jhrg,jimg	James Gallagher (jgallagher@gso.uri.edu)
//	dan		Dan Holloway (dan@hollywood.gso.uri.edu)
//	reza		Reza Nekovei (reza@intcomm.net)

// Connect objects are used as containers for information pertaining to a
// connection that a user program makes to a dataset. The dataset may be
// either local (i.e., a file on the user's own computer) or a remote
// dataset. In the later case a DODS URL will be used to reference the
// dataset. 
//
// Connect contains methods which can be used to read the DOS DAS and DDS
// objects from the remote dataset as well as reading reading data. The class
// understands in a rudimentary way how DODS constraint expressions are
// formed and how to manage the CEs generated by a API to request specific
// variables with the URL initially presented to the class when the object
// was instantiated.
//
// Connect also provides additional services such as automatic decompression
// of compressed data, transmission progress reports and error processing.
//
// Connect will almost certainly need to be specialized for each
// client-library. Connect in not intended for use on the server-side of DODS.
//
// jhrg 9/29/94

/* 
 * $Log: Connect.h,v $
 * Revision 1.48  2000/08/02 22:46:48  jimg
 * Merged 3.1.8
 *
 * Revision 1.38.4.4  2000/08/02 21:10:07  jimg
 * Removed the header config_dap.h. If this file uses the dods typedefs for
 * cardinal datatypes, then it gets those definitions from the header
 * dods-datatypes.h.
 *
 * Revision 1.47  2000/07/26 12:24:01  rmorris
 * Modified intermediate (dod*) file removal under win32 to take into account
 * a 1-to-n correspondence between connect objects and intermediate files.
 * Implemented solution through vector of strings containing the intermediate
 * filenames that are removed when the connect obj's destructor is invoked.
 * Might consider using the same code for unix in the future.  Previous
 * win32 solution incorrectly assumed the correspondence was 1-to-1.
 *
 * Revision 1.46  2000/07/13 07:07:13  rmorris
 * Mod to keep the intermediate file name in the Connect object in the
 * case of win32 (unlink() works differently on win32,  needed another approach).
 *
 * Revision 1.45  2000/07/09 21:57:09  rmorris
 * Mods's to increase portability, minimuze ifdef's in win32 and account
 * for differences between the Standard C++ Library - most notably, the
 * iostream's.
 *
 * Revision 1.44  2000/06/07 18:06:58  jimg
 * Merged the pc port branch
 *
 * Revision 1.43.4.1  2000/06/02 18:16:47  rmorris
 * Mod's for port to Win32.
 *
 * Revision 1.43  2000/04/07 00:19:04  jimg
 * Merged Brent's changes for the progress gui - he added a cancel button.
 * Also repaired the last of the #ifdef Gui bugs so that we can build Gui
 * and non-gui versions of the library that use one set of header files.
 *
 * Revision 1.42  1999/12/15 01:14:37  jimg
 * Added static members to help control the cache.
 *
 * Revision 1.41  1999/12/01 21:37:44  jimg
 * Added members to support caching.
 * Added accessors for cache control. libwww 5.2.9 does not supoort these,
 * however.
 *
 * Revision 1.40  1999/09/03 22:07:44  jimg
 * Merged changes from release-3-1-1
 *
 * Revision 1.39  1999/08/23 18:57:44  jimg
 * Merged changes from release 3.1.0
 *
 * Revision 1.38.4.3  1999/08/28 06:43:03  jimg
 * Fixed the implementation/interface pragmas and misc comments
 *
 * Revision 1.38.4.2  1999/08/10 00:40:04  jimg
 * Changes for the new source code organization
 *
 * Revision 1.38.4.1  1999/08/09 22:57:49  jimg
 * Removed GUI code; reactivate by defining GUI
 *
 * Revision 1.38  1999/05/21 20:41:45  dan
 * Changed default 'gui' flag to 'false' in request_data, and read_data
 * methods.
 *
 * Revision 1.37  1999/04/29 03:04:51  jimg
 * Merged ferret changes
 *
 * Revision 1.36  1999/04/29 02:29:28  jimg
 * Merge of no-gnu branch
 *
 * Revision 1.35.8.1  1999/04/14 22:32:46  jimg
 * Made inclusion of timeval _tv depend on the definition of LIBWWW_5_0. See
 * comments in Connect.cc.
 *
 * Revision 1.35  1999/02/18 19:22:38  jimg
 * Added the field _accept_types and two accessor functions. See Connect.cc and
 * the documentation comments for more information.
 *
 * Revision 1.34  1999/01/21 20:42:01  tom
 * Fixed comment formatting problems for doc++
 *
 * Revision 1.33.4.1  1999/02/02 21:56:56  jimg
 * String to string version
 *
 * Revision 1.33  1998/06/04 06:31:33  jimg
 * Added two new member functions to get/set the www_errors_to_stderr property.
 * Also added a new member _www_errors_to_stderr to hold that property. When
 * true, www errors are printed to stderr in addition to being recorded in the
 * object's Error object. The property is false by default.
 *
 * Revision 1.32  1998/03/19 23:49:28  jimg
 * Removed code associated with the (bogus) caching scheme.
 * Removed _connects.
 *
 * Revision 1.31  1998/02/11 21:28:04  jimg
 * Changed x_gzip/x-gzip to deflate since libwww 5.1 offers internal support
 * for deflate (which uses the same LZW algorithm as gzip without the file
 * processing support of gzip).
 * Added to the arguments accepted by the ctor and www_lib_init so that they
 * can now be called with a flag used to control compression.
 * Fixed up the comments to reflect these changes.
 *
 * Revision 1.30  1998/02/04 14:55:31  tom
 * Another draft of documentation.
 *
 * Revision 1.29  1997/12/18 15:06:10  tom
 * First draft of class documentation, entered in doc++ format,
 * in the comments
 *
 * Revision 1.28  1997/09/22 23:05:45  jimg
 * Added comment info.
 *
 * Revision 1.27  1997/06/06 00:45:15  jimg
 * Added read_data(), parse_mime() and process_data() mfuncs.
 *
 * Revision 1.26  1997/03/23 19:40:20  jimg
 * Added field _comp_childpid. See the note in Connect.cc Re: this field.
 *
 * Revision 1.25  1997/03/05 08:25:28  jimg
 * Removed the static global _logfile. See Connect.cc.
 *
 * Revision 1.24  1997/02/13 17:35:39  jimg
 * Added a field to store information from the MIME header `Server:'. Added
 * a member function to access the value of the field and a MIME header
 * `handler' function to store the value there.
 *
 * Revision 1.23  1997/02/12 21:39:10  jimg
 * Added optional parameter to the ctor for this class; it enables
 * informational error messages from the WWW library layer.
 * * Revision 1.22 1997/02/10 02:31:27 jimg 
 * Changed the return type of request_data() and related functions from DDS &
 * to * DDS *.
 *
 * Revision 1.21  1996/11/25 03:39:25  jimg
 * Added web-error to set of object types.
 * Added two MIME parsers to set of friend functions.
 * Removed unused friend functions.
 *
 * Revision 1.20  1996/11/13 18:57:15  jimg
 * Now uses version 5.0a of the WWW library.
 *
 * Revision 1.19  1996/10/08 17:02:10  jimg
 * Added fields for the projection and selection parts of a CE supplied with
 * the URL passed to the Connect ctor.
 *
 * Revision 1.18  1996/07/10 21:25:34  jimg
 * *** empty log message ***
 *
 * Revision 1.17  1996/06/21 23:14:15  jimg
 * Removed GUI code to a new class - Gui.
 *
 * Revision 1.16  1996/06/18 23:41:01  jimg
 * Added support for a GUI. The GUI is actually contained in a separate program
 * that is run in a subprocess. The core `talks' to the GUI using a pty and a
 * simple command language.
 *
 * Revision 1.15  1996/06/08 00:07:57  jimg
 * Added support for compression. The Content-Encoding header is used to
 * determine if the incoming document is compressed (values: x-plain; no
 * compression, x-gzip; gzip compression). The gzip program is used to
 * decompress the document. The new software uses UNIX IPC and a separate
 * subprocess to perform the decompression.
 *
 * Revision 1.14  1996/06/04 21:33:17  jimg
 * Multiple connections are now possible. It is now possible to open several
 * URLs at the same time and read from them in a round-robin fashion. To do
 * this I added data source and sink parameters to the serialize and
 * deserialize mfuncs. Connect was also modified so that it manages the data
 * source `object' (which is just an XDR pointer).
 *
 * Revision 1.13  1996/05/29 21:51:51  jimg
 * Added the enum ObjectType. This is used when a Content-Description header is
 * found by the WWW library to record the type of the object without first
 * parsing it.
 * Added ctors for the struct constraint.
 * Removed the member _request.
 *
 * Revision 1.12  1996/05/21 23:46:33  jimg
 * Added support for URLs directly to the class. This uses version 4.0D of
 * the WWW library from W3C.
 *
 * Revision 1.11  1996/04/05 01:25:40  jimg
 * Merged changes from version 1.1.1.
 *
 * Revision 1.10  1996/02/01 21:45:33  jimg
 * Added list of DDSs and constraint expressions that produced them.
 * Added mfuncs to manage DDS/CE list.
 *
 * Revision 1.9.2.1  1996/02/23 21:38:37  jimg
 * Updated for new configure.in.
 *
 * Revision 1.9  1995/06/27  19:33:49  jimg
 * The mfuncs request_{das,dds,dods} accept a parameter which is appended to
 * the URL and used by the data server CGI to select which filter program is
 * run to handle a particular request. I changed the parameter name from cgi
 * to ext to better represent what was going on (after getting confused
 * several times myself).
 *
 * Revision 1.8  1995/05/30  18:42:47  jimg
 * Modified the request_data member function so that it accepts the variable
 * in addition to the existing arguments.
 *
 * Revision 1.7  1995/05/22  20:43:12  jimg
 * Removed a paramter from the REQUEST_DATA member function: POST is not
 * needed since we no longer use the post mechanism.
 *
 * Revision 1.6  1995/04/17  03:20:52  jimg
 * Removed the `api' field.
 *
 * Revision 1.5  1995/03/09  20:36:09  jimg
 * Modified so that URLs built by this library no longer supply the
 * base name of the CGI. Instead the base name is stripped off the front
 * of the pathname component of the URL supplied by the user. This class
 * append the suffix _das, _dds or _serv when a Connect object is used to
 * get the DAS, DDS or Data (resp).
 *
 * Revision 1.4  1995/02/10  21:54:52  jimg
 * Modified definition of request_data() so that it takes an additional
 * parameter specifying sync or async behavior.
 *
 * Revision 1.3  1995/02/10  04:43:17  reza
 * Fixed the request_data to pass arguments. The arguments string is added to
 * the file name before being posted by NetConnect. Default arg. is null.
 *
 * Revision 1.2  1995/01/31  20:46:56  jimg
 * Added declaration of request_data() mfunc in Connect.
 *
 * Revision 1.1  1995/01/10  16:23:04  jimg
 * Created new `common code' library for the net I/O stuff.
 *
 * Revision 1.2  1994/10/05  20:23:28  jimg
 * Fixed errors in *.h files comments - CVS bites again.
 * Changed request_{das,dds} so that they use the field `_api_name'
 * instead of requiring callers to pass the api name.
 *
 * Revision 1.1  1994/10/05  18:02:08  jimg
 * First version of the connection management classes.
 * This commit also includes early versions of the test code. 
 */

#ifndef _connect_h
#define _connect_h

#ifdef __GNUG__
#pragma interface
#endif

#include <stdio.h>

#ifdef LIBWWW_5_0
#include <sys/time.h>
#endif
#if 0

#ifdef WIN32
#include <rpc.h>
#include <winsock.h>
#include <xdr.h>
#endif

#include <rpc/types.h>
#include <netinet/in.h>
#include <rpc/xdr.h>
#endif

#include <string>
#include <SLList.h>

#include <WWWLib.h>			      /* Global Library Include file */
#include <WWWApp.h>
#include <WWWInit.h>

#include "DAS.h"
#include "DDS.h"
#include "Error.h"
#ifdef GUI
#include "Gui.h"
#endif
#include "util.h"
#if 0
#include "config_dap.h"
#endif

#ifdef WIN32
using std::vector<string>;
#endif

/**

     When a version 2.x or greater DODS data server sends an
     object, it uses the Content-Description header of the response to
     indicate the type of object contained in the response. During the
     parse of the header a member of Connect is set to one of these
     values so that other mfuncs can tell the type of object without
     parsing the stream themselves.  

     \begin{verbatim}
     enum ObjectType {
       unknown_type,
       dods_das,
       dods_dds,
       dods_data,
       dods_error,
       web_error
     };
     \end{verbatim}

     @memo The type of object in the stream coming from the data
     server.  */

enum ObjectType {
    unknown_type,
    dods_das,
    dods_dds,
    dods_data,
    dods_error,
    web_error
};

/**  DODS understands two types of encoding: x-plain and deflate,
     which correspond to plain uncompressed data and data compressed
     with zlib's LZW algorithm respectively.  

     \begin{verbatim}
     enum EncodingType {
       unknown_enc,
       deflate,
       x_plain
     };
     \end{verbatim}

     @memo The type of encoding used on the current stream. */

enum EncodingType {
    unknown_enc,
    deflate,
    x_plain
};

/** Connect objects are used as containers for information pertaining
    to the connection a user program makes to a dataset. The
    dataset may be either local (for example, a file on the user's own
    computer) or a remote dataset. In the latter case a DODS URL will
    be used to reference the dataset, instead of a filename.

    Connect contains methods which can be used to read the DODS DAS and
    DDS objects from the remote dataset as well as reading 
    data. The class understands in a rudimentary way how DODS
    constraint expressions are formed and how to manage them.

    Connect also provides additional services such as automatic
    decompression of compressed data, transmission progress reports
    and error processing.  Refer to the Gui and Error classes for more
    information about these features. See the DODSFilter class for
    information on servers that compress data.


    The Connect class must be specialized for each different.
    client-library. Connect is only used on the client-side of a DODS
    client-server connection.

    The Connect class contains a linked list of #constraint# objects.
    These are simple C structures containing a constraint expression
    and a DDS object.  The DDS object contains data received from the
    server, and the accompanying constraint expression was a part of
    the data request.  To support multiple connections to a single
    DODS server, you can use either one Connect object with many
    entries in the #constraint# list, or multiple Connect objects.

    @memo Holds information about the link from a DODS client to a
    dataset.
    @see DDS
    @see DAS
    @see DODSFilter
    @see Error
    @see Gui
    @author jhrg 
    */

class Connect {
private:
    bool _local;		// Is this a local connection

    // The following members are valid only if _LOCAL is false.

    static int _num_remote_conns;	// How many remote connections exist?
    static bool _cache_enabled;		// True if the cache is on.
    static char *_cache_root;		// If on, where is the cache?
#ifdef WIN32
	// We need to keep the different filenames associated with _output (over time)
	// around under win32 because an unlink() at time 'now' doesn't delete a file
	// at some time (now + n) as it does under UNIX.  Unix will delete the file
	// when the last process using the file closes it - win32 will not.  Under
	// win32, we count on the Connect destructor using _tfname to remove such
	// intermediate files..
	vector<string> _tfname;			
#endif

    static HTList *_conv;	// List of global converters
    
    ObjectType _type;		// What type of object is in the stream?
    EncodingType _encoding;	// What type of encoding is used?
    string _server;

    DAS _das;			// Dataset attribute structure
    DDS _dds;			// Dataset descriptor structure
    Error _error;		// Error object

#ifdef GUI
    Gui *_gui;			// Used for progress, error display.
#endif

    string _URL;		// URL to remote dataset (minus CE)
    string _proj;		// Projection part of initial CE.
    string _sel;		// Selection of initial CE
    string _accept_types;	// Comma separated list of types understood
    string _cache_control;	// should the request be cached? 
    
    HTParentAnchor *_anchor;
    HTMethod _method;		// What method are we envoking 
    FILE *_output;		// Destination; a temporary file
    XDR *_source;		// Data source stream

    bool _www_errors_to_stderr; // FALSE for messages to stderr
    bool _accept_deflate;

  /* Initialize the W3C WWW Library. This should only be called when a
      Connect object is created and there are no other Connect objects in
      existence.

      @memo Initialize the W3C WWW Library.
      */
    void www_lib_init(bool www_verbose_errors, bool accept_deflate);

  /* Assume that the object's \_OUTPUT stream has been set
      properly.
      Returns true if the read operation succeeds, false otherwise.

      @memo Read a URL.
      */
    bool read_url(string &url, FILE *stream);

  /* Separate the text DDS from the binary data in the data object (which
      is a bastardized multipart MIME document). The returned FILE * points
      to a temporary file which contains the DDS object only. The formal
      parameter IN is advanced so that it points to the first byte of the
      binary data.

      @memo Separate the DDS from the binary data.
      */
    FILE *move_dds(FILE *in);

  /* Create a new Connect object.

      @memo Copy from one Connect to another. 
      */
    void clone(const Connect &src);

  /* Close the output stream of the Connect object.

      @memo Close the object's \_output stream if it is not NULL or
      STDOUT.  
      */
    void close_output();

  /* Something to do with the DDS. 

    @memo process data?
    */
    DDS *process_data(bool async = false);
    
  /* Use when you cannot use libwww. 

    @memo Simple MIME parser. 
    */
    void parse_mime(FILE *data_source);

    friend BOOL dods_username_password(HTRequest * request, HTAlertOpcode,
				       int, const char *, void *, 
				       HTAlertPar * reply);

    friend BOOL dods_progress(HTRequest * request, HTAlertOpcode op, int, 
			      const char *, void * input, HTAlertPar *);

    friend int timeout_handler(HTRequest *request);

    friend int description_handler(HTRequest *request, HTResponse *response,
				   const char *token, const char *val);

    friend int encoding_handler(HTRequest *request, HTResponse *response,
				const char *token, const char *val);

    friend int server_handler(HTRequest *request, HTResponse *response,
			      const char *token, const char *val);

    friend int header_handler(HTRequest *request, HTResponse *response,
			      const char *token, const char *val);

    friend BOOL dods_error_print (HTRequest * request, HTAlertOpcode, int, 
				  const char *, void * input, HTAlertPar *);

    Connect();			// Never call this.

public:
  /** The Connect constructor requires a #name#, which is the URL to
      which the connection is to be made.  You can specify that you
      want to see the #verbose# form of any WWW library errors.
      This can be useful for debugging.  The default is to suppress
      these errors. Callers can use the #accept_deflate# parameter to
      request that servers are told the client (caller of Connect
      ctor) \em{can} process return documents that are compressed with 
      gzip.

      @memo Create an instance of Connect. */
    Connect(string name, bool www_verbose_errors = false,
	    bool accept_deflate = true); 

  /** The Connect copy construtor. */
    Connect(const Connect &copy_from);
    virtual ~Connect();

    Connect &operator=(const Connect &rhs);

    /** Gets the state of the #www_errors_to_stderr# property. If TRUE this
	means that http errors will be printed to stderr in addition to being
	reported in the Error object. If FALSE only the Error object will be
	used. 

	@return TRUE if WWW errors should got to stderr, FALSE if only the
	Error object should be used. */
    bool get_www_errors_to_stderr();

    /** Sets the #www_errors_To_stderr# property.

	@see is_www_errors_to_stderr
	@param state The state of the property. */
    void set_www_errors_to_stderr(bool state);

    /** Gets the current string of `accepted types'. This string lists all of
	the DODS datatypes that the client can grok. 

	@see set_accepted_types
	@return A string listing the types this client declares to servers it
	can understand. */
    string get_accept_types();

    /** Sets the list of accepted types. This string is meant to list all of
	the DODS datatypes that the client can grok and is sent to
	the server using the XDODS-Accept-Types MIME header. The server will
	try to only send back to the client datatypes that are listed. If the
	value of this header is `All', then the server assumes that the
	client can process all of the DODS datatypes. If only one or two
	types are \emph{not} understood, then they can be listed, each one
	prefixed by `!'. Thus, if a client does not understand `Sequences',
	it could set types to `!Sequences' as opposed to listing all of the
	DODS datatypes. Multiple values are separated by commas (,).

	Not all servers will honor this and some requests may not be possible
	to express with a very limited set of datatypes.

	NB: By default, the value `All' is used.

	@param types The string listing datatypes understood by this client. */
    void set_accept_types(const string &types);

    /** Get the string which describes the default cache control value. This
	is sent with all outgoing messages.<p>
	NB: The libwww 5.2.9 cache does not honor this.
	@return The cache control header value. */
    string get_cache_control();

    /** Set the cache control header value.
	@see get_cache_control
	@see HTTP/1.1 Specification, sec. 14.9.
	@param caching Should be no-cache to disable caching. */
    void set_cache_control(const string &caching);

  /** Fetch the contents of the indicated URL and put its contents
      into an output file.  A pointer to this file can be retrieved
      with the #output()# function.  If {\it async} is true, then the
      operation is asynchronous, and the function returns before the
      data transfer is complete.

      Note that the asynchronous transfer feature of DODS is not
      currently enabled.  All invocations of this function will be
      synchronous, no matter what the value of the {\it async}
      parameter. 

      @memo Dereference a URL.  
      @return Returns false if an error is detected, otherwise returns
      true.  
      @param url A string containing the URL to be dereferenced.  The
      data referred to by this URL will wind up available through a
      file pointer retrieved from the #output()# function.
      @param async If true, the read operation will proceed
      asynchronously.  In other words, the function may return before
      the read is complete.
      @see Connect::output */
    bool fetch_url(string &url, bool async = false);

  /** Returns a file pointer which can be used to read the data
      fetched from a URL.

      Note that occasionally this may be directed to #stdout#.  If this
      is the case, users should avoid closing it.

      @memo Access the information contained in this Connect instance.
      @see Connect::fetch_url
      @return A #(FILE *)# indicating a file containing the data
      received from a dereferenced URL.  */
    FILE *output();

  /** The data retrieved from a remote DODS server will be in XDR
      format.  Use this function to initialize an XDR decoder for that
      data and to return an XDR pointer to the data.

      @memo Access the XDR input stream (source) for this connection.

      @return Returns a XDR pointer tied to the current output
      stream.  
      @see Connect::output
      */
    XDR *source();

  /** The Connect class can be used for ``connections'' to local
      files.  This means that local files can continue to be accessed
      with a DODS-compliant API.

      @memo Does this object refer to a local file?  

      @return Return TRUE if the Connect object refers to a local
      file, otherwise returns FALSE.  */
    bool is_local();

  /** Return the Connect object's URL in a string.  The URL was set by
      the class constructor, and may not be reset.  If you want to
      open another URL, you must create another Connect object.  There
      is a Connections class created to handle the management of
      multiple Connect objects.

      @memo Get the object's URL.
      @see Connections
      @return A string containing the URL of the data to which the
      Connect object refers.  If the object refers to local data,
      the function returns the null string.  
      @param CE If TRUE, the returned URL will include any constraint
      expression enclosed with the Connect object's URL (including the
      #?#).  If FALSE, any constraint expression will be removed from
      the URL.  The default is TRUE.
      */
    string URL(bool CE = true);

  /** Return the constraint expression (CE) part of the Connect URL. Note
      that this CE is supplied as part of the URL passed to the
      Connect's constructor.  It is not the CE passed to the 
      #request_data()# function.

      @memo Get the Connect's constraint expression.
      @return A string containing the constraint expression (if any)
      submitted to the Connect object's constructor.  */
    string CE();

  /** During the parse of the message headers returned from the
      dereferenced URL, the object type is set. Use this function to
      read that type information. This will be valid {\it before} the
      return object is completely parsed so it can be used to decide
      which parser to call to read the data remaining in
      the input stream.

      The object types are Data, DAS, DDS, Error, and undefined.

      @memo What type is the most recent object sent from the
      server?
      @return The type of the object.
      @see ObjectType */
    ObjectType type();

  /** During the parse of the message headers returned from the
      dereferenced URL, the encoding type is set. Use this function to
      read that type information. This will be valid {\it before} the
      return object is completely parsed so it can be used to decide
      which decoder to call (if any) to read the data remaining in
      the input stream.

      The encoding types are currently limited to x-plain (no special
      decoding required) and x-gzip (compressed using GNU's gzip).  

      @memo What type of encoding was used on the data in the stream? 
      @return The type of the compression.
      @see EncodingType
      */
    EncodingType encoding();

  /** Returns a string containing the version of DODS used by the
      server. */
    string server_version();

  /** All DODS datasets define a Data Attribute Structure (DAS), to
      hold a variety of information about the variables in a
      dataset. This function returns the DAS for the dataset indicated
      by this Connect object.

      @memo Return a reference to the Connect's DAS object. 
      @return A reference to the DAS object.
      @see DAS 
      */
    DAS &das();

  /** All DODS datasets define a Data Descriptor Structure (DDS), to
      hold the data type of each of the variables in a dataset.  This
      function returns the DDS for the dataset indicated by this
      Connect object.

      @memo Return a reference to the Connect's DDS object. 
      @return A reference to the DDS object.
      @see DDS 
      */
    DDS &dds();

  /** The DODS server uses Error objects to signal error conditions to
      the client.  If an error condition has occurred while fetching a
      URL, the Connect object will contain an Error object with
      information about that error.  The Error object may also contain
      a program to run to remedy the error.  This function returns the
      latest Error object received by the Connect object.

      @memo Get a reference to the last error.
      @return The last Error object sent from the server. If no error has
      been sent from the server, returns a reference to an empty error
      object. 
      @see Error 
      */
    Error &error();

  /** The DODS client can display graphic information to a user with
      the DODS Graphical User Interface (GUI).  Typically used for a
      progress indicator, the GUI is simply a shell capable of
      interpreting arbitrary graphical commands (with tcl/tk).  The
      Gui object is created anew when the Connect object is first
      created.  This function returns a pointer to the Gui object, so
      you can modify the GUI as desired.

      This member will be removed since its presence makes it hard to build
      Gui and non-gui versions of the DAP. The Gui object is accessed is in
      Connect and Error, but in the later case an instance of Gui is always
      passed to the instance of Error. Thus, even though it is a dubious
      design, we can use the private member _gui and pass the pointer to
      outside classes. Eventually, Connect must be redesigned.

      @memo Returns a pointer to a Gui object.
      @return a pointer to the Gui object associated with this
      connection. 
      @deprecated 
      @see Gui */
    void *gui();

  /** Reads the DAS corresponding to the dataset in the Connect
      object's URL. 

      @memo Get the DAS from a server.
      @return TRUE if the DAS was successfully received. FALSE
      otherwise. 
      @param gui If TRUE, use the client GUI.  Most DAS's are too
      small to make this worthwhile.
      @param ext The extension to append to the URL to retrieve the
      dataset DAS.  This parameter is included for compatibility with
      future versions of the DODS software.  It currently defaults to
      the only possible working value, ``das''.
      */
    bool request_das(bool gui = false,  const string &ext = "das");

  /** Reads the DDS corresponding to the dataset in the Connect
      object's URL. 

      @memo Get the DDS from a server.
      @return TRUE if the DDS was successfully received. FALSE
      otherwise. 
      @param gui If TRUE, use the client GUI.  Most DDS's are too
      small to make this worthwhile.
      @param ext The extension to append to the URL to retrieve the
      dataset DDS.  This parameter is included for compatibility with
      future versions of the DODS software.  It currently defaults to
      the only possible working value, ``dds''.
      */
    bool request_dds(bool gui = false, const string &ext = "dds");

  /** Reads data from the Connect object's server.  This function sets
      up the BaseType variables in a DDS, and sends a request using
      #fetch_url()#.  Upon return, it caches the data on a disk, then
      unpacks it into the DDS storage.

      @return A reference to the DataDDS object which contains the
      variables (BaseType pointers) generated from the DDS sent with
      the data. These variables are guaranteed to be large enough to
      hold the data, even if the constraint expression changed the
      type of the variable from that which appeared in the original
      DDS received from the dataset when this connection was made.
      @param expr A string containign a constraint expression.  The
      function adds the clauses of this constraint expression to the
      Connect object's original CE.  If the constraint expression
      contains one or more Sequences, these must be the {\it last}
      objects specified in the projection clause.  If you request N
      variables and M of them are Sequences, all the M sequences must
      follow the N-M other variables. 
      @param gui If this is TRUE, use the DODS client GUI.  See the
      Gui class for a description of this feature.
      @param async  If this is TRUE, this function reads data
      asynchronously, returning before the read completes. Synchronous
      reads are the default, and the only possible action as of DODS
      version 2.15.
      @param ext The extension to append to the URL to retrieve the
      dataset data.  This parameter is included for compatibility with
      future versions of the DODS software.  It currently defaults to
      the only possible working value, ``dods''.
      @see DataDDS
      @see Gui */
    DDS *request_data(string expr, bool gui = false, bool async = false, 
		      const string &ext = "dods");

  /** This function reads cached data from a disk file.

      @return A reference to the DataDDS object which contains the
      variables (BaseType pointers) generated from the DDS sent with
      the data. 
      @param gui If this is TRUE, use the DODS client GUI.  See the
      Gui class for a description of this feature.
      @param async  If this is TRUE, this function reads data
      asynchronously, returning before the read completes. Synchronous
      reads are the default, and the only possible action as of DODS
      version 2.15.
      @param ext The extension to append to the URL to retrieve the
      dataset data.  This parameter is included for compatibility with
      future versions of the DODS software.  It currently defaults to
      the only possible working value, ``dods''.
      @see DataDDS
      @see Gui 
      */
    DDS *read_data(FILE *data_source, bool gui = false, bool async = false);
};

#endif // _connect_h
