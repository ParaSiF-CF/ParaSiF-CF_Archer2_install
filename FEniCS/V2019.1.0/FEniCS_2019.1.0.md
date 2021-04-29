Instructions for compiling FEniCS 2019.1.0 for ARCHER2
======================================================

These instructions are for compiling FEniCS 2019.1.0 on 
[ARCHER2](https://www.archer2.ac.uk).


Load modules and set python paths/build paths
---------------------------------------------

```bash
  module restore PrgEnv-gnu
  module load cray-python
  module load cmake
  export PYTHONUSERBASE=/work/c01/c01/wendiliu/.local
  export PATH=$PYTHONUSERBASE/bin:$PATH
  #pip install --user virtualenv
  virtualenv --version
  virtualenv --system-site-packages fenics2019_eCSE_FSI
  #cp -r source/ fenics2019_eCSE_FSI/source

  BUILD_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0
  cd $BUILD_DIR
  export PATH=$PATH:$BUILD_DIR/bin
  export PATH=$PATH:$BUILD_DIR/shared/bin
  export PYTHONPATH=$PYTHONPATH:$BUILD_DIR/lib/python3.8/site-packages
  export LD_LIBRARY_PATH=$BUILD_DIR/lib:$LD_LIBRARY_PATH
  export CC=cc
  export CXX=CC

```

Download, configure, and build pybind
-------------------------------------

```bash
  cd $BUILD_DIR
  export PYBIND11_VERSION=2.6.1
  wget -nc --quiet https://github.com/pybind/pybind11/archive/v${PYBIND11_VERSION}.tar.gz
  tar -xf v${PYBIND11_VERSION}.tar.gz
  mkdir pybind11-${PYBIND11_VERSION}/build
  cd pybind11-${PYBIND11_VERSION}/build
  cmake -DPYBIND11_TEST=off .. -DCMAKE_INSTALL_PREFIX=$(pwd) -DPYTHON_EXECUTABLE:FILEPATH=$BUILD_DIR/fenics2019_eCSE_FSI/bin/python3
  make install
```

Download, configure, and install BOOST
--------------------------------------

While BOOST is available centrally on ARCHER2, the shared libraries are not 
(and that's what we need for FEniCS/DOLFIN). For this, we'll use the build 
script provided by the ARCHER2 CSE team.

```bash
  cd $BUILD_DIR
  git clone https://github.com/ARCHER2-HPC/pe-scripts.git
  mv pe-scripts boost
  cd  boost
  git checkout cse-develop
  sed -i 's/make_shared=0/make_shared=1/g' sh/.preamble.sh
  ./sh/boost.sh --prefix=$(pwd)/boost
```

Download, configure, and install EIGEN
--------------------------------------

```bash
  cd $BUILD_DIR
  wget https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.gz
  tar -xvf eigen-3.3.9.tar.gz
  mkdir eigen-3.3.9/build; cd eigen-3.3.9/build
  cmake ../ -DCMAKE_INSTALL_PREFIX=build -DPYTHON_EXECUTABLE:FILEPATH=$BUILD_DIR/fenics2019_eCSE_FSI/bin/python3
  make -j 8 install
  cd ../../
```

Download, configure, and install mpi4py
---------------------------------------------------------

```bash
. fenics2019_eCSE_FSI/bin/activate
pip install pkgconfig
pip install h5py==3.0.0rc1
```

Download, configure, and install hdf5-1.10.7
--------------------------------------

```bash
  cd $BUILD_DIR/boost
  wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.7/src/hdf5-1.10.7.tar.gz
  tar -xvf hdf5-1.10.7.tar.gz
  cd hdf5-1.10.7
  ./configure \
  --prefix=$BUILD_DIR/boost/hdf5-1.10.7_install \
  CC=cc \
  CFLAGS=-O3 \
  CXX=CC \
  CXXFLAGS=-O3 \
  --enable-cxx \
  --enable-parallel \
  --enable-unsupported
  
  make
  make install
  export LD_LIBRARY_PATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/lib:$LD_LIBRARY_PATH
  export LD_RUN_PATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/lib:$LD_RUN_PATH
```


Download, configure, and install FEniCS python components
---------------------------------------------------------

```bash
  cd $BUILD_DIR
  wget https://bitbucket.org/fenics-project/ffc/downloads/ffc-2019.1.0.post0.tar.gz
  tar -xvf ffc-2019.1.0.post0.tar.gz
  cd ffc-2019.1.0.post0/
  python3 setup.py install
  cd ..
```


Download, configure, and install metis
---------------------------------------------------------

modify $BUILD_DIR/boost/sh/tpsl.sh
L21
--- printf "%s\n" glm hypre matio metis scotch parmetis mumps sundials superlu superlu-dist \
+++ printf "%s\n" glm hypre matio metis scotch parmetis mumps superlu superlu-dist \

```bash
  cd  boost
  ./sh/tpsl.sh --prefix=$(pwd)/boost
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/metis-5.1.0/include
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/parmetis-4.0.3/include
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/superlu-5.2.1/SRC
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/superlu_dist-6.1.1/SRC
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/scotch_6.0.10/include
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/MUMPS_5.2.1/include
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/include
  export PATH=$PATH:/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/boost/include
 export LD_LIBRARY_PATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/lib:$LD_LIBRARY_PATH
  export LD_RUN_PATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/lib:$LD_RUN_PATH
  export HDF5_INCLUDE_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/include
  cd ..
```

Download, configure, and install PETSc
---------------------------------------
# go to sh/petsc.sh
# L10
# +++  3.11.4:006177b4059cd40310a3e9a4bf475f3a8c276b62d8cca4df272ef88bdfc2f83a
# L62
# +++ #fn_check_includes HDF5 hdf5.h
# L205
# +++   --with-hdf5=0 \\

# ```bash
  # cd  boost
  # ./sh/petsc.sh --prefix=$(pwd)/boost
  # cd ..
  # export PETSC_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4 
  # export PETSC_ARCH=x86-rome
# ```



```bash
  cd  boost
  wget https://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-3.11.4.tar.gz
  tar xf petsc-3.11.4.tar.gz
  cd petsc-3.11.4
  cp ../../INSTALL_PETSC.sh ./
  chmod +x INSTALL_PETSC.sh
  ./INSTALL_PETSC.sh
  make PETSC_DIR=`pwd` all
  make PETSC_DIR=`pwd` install
  cd ..
  export PETSC_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4
  export PETSC_ARCH=arch-linux-c-opt
```

Download, configure, and install PETSc4py
---------------------------------------
```bash
cd  /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost
wget https://bitbucket.org/petsc/petsc4py/downloads/petsc4py-3.11.0.tar.gz
tar -xzf petsc4py-3.11.0.tar.gz
cd petsc4py-3.11.0
```

go to src/PETSc/cyclicgc.pxi

	L27 -    if arg == NULL and _Py_AS_GC(d).gc_refs == 0:
	L28 -        _Py_AS_GC(d).gc_refs = 1

	L17 -    ctypedef struct PyGC_Head:
	L18 -       Py_ssize_t gc_refs"gc.gc_refs"
	L19 -    PyGC_Head *_Py_AS_GC(PyObject*)

```bash
rm -f src/petsc4py.PETSc.c
python3 setup.py install
```
Download, configure, and install DOLFIN
---------------------------------------

Download DOLFIN, and make sure that all dependencies are correct:

```bash
  cd $BUILD_DIR
  export FENICS_VERSION=2019.1.0.post0
  git clone --branch=$FENICS_VERSION https://bitbucket.org/fenics-project/dolfin
  mkdir dolfin/build; cd dolfin/build
  export BOOST_ROOT=$BUILD_DIR/boost
  export EIGEN3_INCLUDE_DIR=$BUILD_DIR/eigen-3.3.9/build/build/include/eigen3
  export SCOTCH_DIR=$BUILD_DIR/boost/boost
  
 #export LD_LIBRARY_PATH=/work/c01/c01/shared/vcz18385/SOFTWARE/GNU/10.1.0/HDF5/hdf5-1.10.7_install/lib:$LD_LIBRARY_PATH
  #export LD_RUN_PATH=/work/c01/c01/shared/vcz18385/SOFTWARE/GNU/10.1.0/HDF5/hdf5-1.10.7_install/lib:$LD_RUN_PATH
  #export HDF5_INCLUDE_DIR=/work/c01/c01/shared/vcz18385/SOFTWARE/GNU/10.1.0/HDF5/hdf5-1.10.7_install/include
  #export PETSC_DIR=/work/c01/c01/shared/vcz18385/SOFTWARE/GNU/10.1.0/PETSC/petsc-3.11.4 
  #export PETSC_ARCH=arch-linux-c-opt  
  
 export LD_LIBRARY_PATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/lib:$LD_LIBRARY_PATH
  export LD_RUN_PATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/lib:$LD_RUN_PATH
  export HDF5_INCLUDE_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/include
  export PETSC_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4
  export PETSC_ARCH=arch-linux-c-opt

```

At this point, we need to edit `$BUILD_DIR/dolfin/CMakeLists.txt`. We need to 
add the following text to the top of that file:

```bash
  SET( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
  IF( NOT EIGEN3_INCLUDE_DIR )
      MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
  ENDIF()
  INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )
```
At this point, Edit cmake/modules/FindPETSc.cmake,  near the top you should edit 

```bash
  pkg_search_module(PETSC craypetsc_real PETSc) -> 
  pkg_search_module(PETSC petsc PETSc)
```

Run CMake:

```bash

  cmake -DCMAKE_INSTALL_PREFIX=$(pwd)   \
  -DPYTHON_EXECUTABLE:FILEPATH=$BUILD_DIR/fenics2019_eCSE_FSI/bin/python3   \
  -DDOLFIN_ENABLE_PYTHON=true \
  -DDOLFIN_USE_PYTHON3=true \
  -DDOLFIN_ENABLE_PETSC=true \
  -DPETSC_DIR="/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4" \
  -DPETSC_LIBRARY="/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4/arch-linux-c-opt/lib/libpetsc.so" \
  -DDOLFIN_SKIP_BUILD_TESTS=true \
  -DCMAKE_REQUIRED_LIBRARIES="-lmpifort" \
  -DCMAKE_CXX_FLAGS_RELEASE="-Wno-literal-suffix -O3 -DNDEBUG" \
  -DHDF5_ROOT="/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install" \
  -DHDF5_INCLUDE_DIRS="/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/hdf5-1.10.7_install/include" \
  -DPTESMUMPS_LIBRARY="/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4/install/lib/libptesmumps.a" \
  ..

  make -j 8 install
  source $BUILD_DIR/dolfin/build/share/dolfin/dolfin.conf
  #source /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/dolfin/build/share/dolfin/dolfin.conf
```

Build python build

```
  cd ../python
  export pybind11_DIR=$BUILD_DIR/pybind11-2.6.1/build//share/cmake/pybind11/
  export DOLFIN_DIR=$BUILD_DIR/dolfin/build/share/dolfin/cmake
  python3 setup.py install
```

> ## Note for running
> 
> When wanting to use FEniCS and DOLFIN, you will need to run:
>  `source $BUILD_DIR/dolfin/build/share/dolfin/dolfin.conf`
>
{. :note}
===================================================
#salloc --nodes=1 --tasks-per-node=128 --cpus-per-task=1 --time=00:20:00 --partition=standard --qos=short --reservation=shortqos --account=c01-eng

#srun --distribution=block:block --hint=nomultithread python3 a.py
===================================================
#python3 -c "from dolfin import *"
#python3 -c "from dolfin import VectorFunctionSpace"
#python3 -c "from dolfin import BoxMesh"
===================================================
#rm -r boost/ dolfin/ eigen-3.3.9/ fenics2019_eCSE_FSI/ ffc-2019.1.0.post0/ hdf5-1.10.7/ pybind11-2.6.1/ eigen-3.3.9.tar.gz hdf5-1.10.7.tar.gz v2.6.1.tar.gz