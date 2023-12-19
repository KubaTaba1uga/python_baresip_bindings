# CFFI example

## Intro

API mode is promoted by CFFI, so let's start with that one.

In docs for `FFI.set_source` is written:
```
The keywords arguments to set_source() control how the C compiler will be called. They are passed directly to distutils or setuptools and include at least sources, include_dirs, define_macros, undef_macros, libraries, library_dirs, extra_objects, extra_compile_args and extra_link_args. You typically need at least libraries=['foo'] in order to link with libfoo.so or libfoo.so.X.Y, or foo.dll on Windows. The sources is a list of extra .c files compiled and linked together (the file module_name.c shown above is always generated and automatically added as the first argument to sources). See the distutils documentations for more information about the other arguments.
```

So basically we need to port GCC compilation command, into `FFI.set_source` kwargs.

Note:<br>
`foo_lib` tries to mimic cmake libraries layout, so example is closer to baresip/re layout.

## Build foo_lib

```
cd foo_lib
bash compile.sh
cd ..
```
If everything succeeded `libfoo.so` should appear in `foo_lib/build`.

## Build foo_lib bindings
```
python3 -m virtualenv .venv
. .venv/bin/activate
```
```
pip install -r requirements.txt
python3 build_foo_lib.py
```

## Use foo_lib

```
python3 example.py
```
