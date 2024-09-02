from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys

# Determine platform-specific library paths
if sys.platform == "win32":
    # Windows
    vcpkg_root = os.getenv('VCPKG_ROOT', 'C:\\vcpkg')  # Set your vcpkg path
    libass_head = os.path.join(vcpkg_root, 'installed', 'x64-windows', 'include')
    libass_lib = os.path.join(vcpkg_root, 'installed', 'x64-windows', 'lib', 'ass.lib')
    json_lib = os.path.join(vcpkg_root, 'installed', 'x64-windows', 'lib', 'jsoncpp.lib')
    json_head = os.path.join(vcpkg_root, 'installed', 'x64-windows', 'include')
else:
    # Linux
    libass_lib = "/usr/lib"
    libass_head = "/usr/include/ass"
    json_lib = "/usr/lib"
    json_head = "/usr/include/jsoncpp"

ext_module = Extension(
    "subdeloc_helper",
    sources=["modify_subs.pyx", "modifysubs.cpp"],
    language="c++",
    extra_compile_args=["-std=c++11"],
    libraries=["ass", "jsoncpp"],
    library_dirs=[libass_lib, json_lib],
    include_dirs=[libass_head, json_head],
)

setup(
    name="modify_subs",
    version='0.1.0',
    ext_modules=cythonize([ext_module]),
    author='Efrain Cardenas',  
    author_email='',
    description='Subtitles delocalizer helper.',
    long_description=open("README.md", encoding='utf-8').read(),
    long_description_content_type="text/markdown",
    url="https://github.com/EfronC/subdeloc_helper",
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',  # License type
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)
