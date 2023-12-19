import cffi

ffibuilder = cffi.FFI()

ffibuilder.cdef(
    """

    int add(int a, int b);

"""
)

ffibuilder.set_source(
    "python_foo_lib",
    "#include <foo.h>",
    include_dirs=["foo_lib/include"],
    libraries=["foo"],
    library_dirs=["foo_lib/build"],
)

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
