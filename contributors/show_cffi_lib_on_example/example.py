import sys

try:
    from python_foo_lib import lib
except ImportError:
    print("run build_foo_lib.py first, then make sure the shared object is on sys.path")
    sys.exit(1)

if __name__ == "__main__":
    print("2 + 3 =", lib.add(2, 3))
