/* base includes required by any module */
%include "stdint.i"
%include "exception.i"
%include "std_string.i"

%{
 /* required for python */
 #define SWIG_FILE_WITH_INIT

 #include <shogun/base/init.h>
 #include <shogun/lib/common.h>
 #include <shogun/lib/io.h>
 #include <shogun/lib/SGSerializable.h>
 #include <shogun/lib/ShogunException.h>
 #include <shogun/base/Version.h>
 #include <shogun/base/Parallel.h>
 #include <shogun/base/SGObject.h>

 extern void sg_global_print_message(FILE* target, const char* str);
 extern void sg_global_print_warning(FILE* target, const char* str);
 extern void sg_global_print_error(FILE* target, const char* str);
#ifndef DISABLE_CANCEL_CALLBACK
 extern void sg_global_cancel_computations(bool &delayed, bool &immediately);
#endif

#ifdef SWIGR
 #include <Rdefines.h>
#endif

 using namespace shogun;
%}

%init %{
#ifndef DISABLE_CANCEL_CALLBACK
    shogun::init_shogun(&sg_global_print_message, &sg_global_print_warning,
            &sg_global_print_error, &sg_global_cancel_computations);
#else
    shogun::init_shogun(&sg_global_print_message, &sg_global_print_warning,
            &sg_global_print_error);
#endif

#ifdef SWIGPYTHON
    import_array();
#endif
%}

%exception
{
    try
    {
        $action
    }
    catch (std::bad_alloc)
    {
        SWIG_exception(SWIG_MemoryError, const_cast<char*>("Out of memory error.\n"));
        SWIG_fail;
    }
    catch (shogun::ShogunException e)
    {
        SWIG_exception(SWIG_SystemError, const_cast<char*>(e.get_exception_string()));
        SWIG_fail;
    }
}

%ignore NUM_LOG_LEVELS;
%ignore FBUFSIZE;

%rename(SGSerializable) CSGSerializable;
%feature("ref")   CSGSerializable "SG_REF($this);"
%feature("unref") CSGSerializable "SG_UNREF($this);"

%rename(SGObject) CSGObject;

%include <shogun/lib/common.h>

%include "swig_typemaps.i"

%include <shogun/lib/ShogunException.h>
%include <shogun/lib/io.h>
%include <shogun/lib/SGSerializable.h>
%include <shogun/base/SGObject.h>
%include <shogun/base/Version.h>
%include <shogun/base/Parallel.h>




%include stl.i
/* instantiate the required template specializations */
namespace std {
  %template(IntVector)    vector<int32_t>;
  %template(DoubleVector) vector<float64_t>;
  %template(StringVector) vector<string>;
}

#ifdef SWIGPYTHON

%pythoncode %{
import tempfile, random, os, exceptions

try: import Library as shogunLibrary
except ImportError: import shogun.Library as shogunLibrary

def __SGgetstate__(self):
	fname = tempfile.gettempdir() + "/" + tempfile.gettempprefix() \
		+ str(random.randint(0, 1e15))

	fstream = shogunLibrary.SerializableAsciiFile(fname, "w")
	if not self.save_serializable(fstream):
		fstream.close(); os.remove(fname)
		raise exceptions.IOError("Could not dump Shogun object!")
	fstream.close()

	fstream = open(fname, "r"); result = fstream.read();
	fstream.close()

	os.remove(fname)
	return result

def __SGsetstate__(self, state_str):
	self.__init__()

	fname = tempfile.gettempdir() + "/" + tempfile.gettempprefix()	\
		+ str(random.randint(0, 1e15))

	fstream = open(fname, "w"); fstream.write(state_str);
	fstream.close()

	fstream = shogunLibrary.SerializableAsciiFile(fname, "r")
	if not self.load_serializable(fstream):
		fstream.close(); os.remove(fname)
		raise exceptions.IOError("Could not load Shogun object!")
	fstream.close()

	os.remove(fname)

SGSerializable.__setstate__ = __SGsetstate__
SGSerializable.__getstate__ = __SGgetstate__
%}

#endif /* SWIGPYTHON  */
