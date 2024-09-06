from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys

# Determine platform-specific library paths
if sys.platform == "win32":
    # Windows

    # Check if 32-bits or 64-bits
    is_64bits = sys.maxsize > 2**32
    arch = 'x64-windows' if is_64bits else 'x86-windows'

    vcpkg_root = os.getenv('VCPKG_ROOT', 'C:\\vcpkg')
    libs = [
        os.path.join(vcpkg_root, 'installed', arch, 'lib'),
        os.path.join(vcpkg_root, 'installed', arch, 'bin'),
    ]
    includes = [
        os.path.join(vcpkg_root, 'installed', arch, 'include'),
    ]
else:
    # Linux
    libs = [
        "/usr/lib",
    ]
    includes = [
        "/usr/include/ass",
        "/usr/include/jsoncpp",
    ]

ext_module = Extension(
    "modify_subs",
    sources=["modify_subs.pyx", "modifysubs.cpp"],
    language="c++",
    extra_compile_args=["-std=c++11"],
    libraries=["ass", "jsoncpp"],
    library_dirs=libs,
    include_dirs=includes,
)

setup(
    name="modify_subs",
    version='0.4.0',
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
    python_requires='>=3.7',
)
