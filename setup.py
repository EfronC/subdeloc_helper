from setuptools import setup, Extension
from Cython.Build import cythonize
import os
import sys

def full_path(module, filename):
    return os.path.join("modules", module, filename)

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
elif sys.platform == "darwin":
    libs = [
        "/opt/homebrew/opt/libass/lib",
        "/opt/homebrew/opt/jsoncpp/lib"
    ]
    includes = [
        "/opt/homebrew/opt/libass/include",
        "/opt/homebrew/opt/jsoncpp/include"
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
    sources=[full_path("modify_subs", "modify_subs.pyx"), full_path("modify_subs", "modifysubs.cpp")],
    language="c++",
    extra_compile_args=["-std=c++11"],
    libraries=["ass", "jsoncpp"],
    library_dirs=libs,
    include_dirs=includes,
)

pair_module = Extension(
    "pair_subs",
    sources=[full_path("pair_subs", "pair_subs.pyx"), full_path("pair_subs", "pairsubs.cpp")],
    language="c++",
    extra_compile_args=["-std=c++11"],
)

setup(
    name="modify_subs",
    version='0.5.0',
    ext_modules=cythonize([ext_module, pair_module]),
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
    python_requires='>=3.8',
)
