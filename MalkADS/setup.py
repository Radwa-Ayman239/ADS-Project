"""
Setup script for building the library_system Python extension
Uses setuptools with pybind11 to compile the C++ extension module
"""

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import os

# Get pybind11 includes
try:
    import pybind11
    pybind11_includes = [pybind11.get_include()]
except ImportError:
    print("Error: pybind11 is required. Install with: pip install pybind11")
    sys.exit(1)

# C++ source files
cpp_sources = [
    'bindings.cpp',
    'src/managers/UsersManager.cpp',
    'src/managers/RoomsManager.cpp',
    'src/managers/LaptopsManager.cpp',
    'src/managers/BooksManager.cpp',
    'src/models/user.cpp',
    'src/models/book.cpp',
    'src/structures/IntervalTreeComplete.cpp',
    'src/structures/queue.cpp',
]

# Include directories
include_dirs = [
    '.',
    'include',
    'include/managers',
    'include/models',
    'include/structures',
    'include/helpers',
] + pybind11_includes

# Extension module
ext_modules = [
    Extension(
        'library_system',
        sources=cpp_sources,
        include_dirs=include_dirs,
        language='c++',
        extra_compile_args=['/std:c++11'] if sys.platform == 'win32' else ['-std=c++11'],
    ),
]


class BuildExt(build_ext):
    """Custom build extension to set C++11 standard"""
    
    def build_extensions(self):
        # Set C++11 standard
        if sys.platform == 'win32':
            for ext in self.extensions:
                ext.extra_compile_args = ['/std:c++11', '/EHsc']
        else:
            for ext in self.extensions:
                ext.extra_compile_args = ['-std=c++11']
        
        build_ext.build_extensions(self)


setup(
    name='library_system',
    version='1.0.0',
    description='AUC Library System - Python bindings for C++ backend',
    ext_modules=ext_modules,
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
