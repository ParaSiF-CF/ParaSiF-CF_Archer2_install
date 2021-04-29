# export prefix=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/petsc-3.11.4/install

# ./configure \\
  # --known-has-attribute-aligned=1 \\
  # --known-mpi-int64_t=0 \\
  # --known-bits-per-byte=8 \\
  # --known-64-bit-blas-indices=0 \\
  # --known-sdot-returns-double=0 \\
  # --known-snrm2-returns-double=0 \\
  # --known-level1-dcache-assoc=4 \\
  # --known-level1-dcache-linesize=64 \\
  # --known-level1-dcache-size=16384 \\
  # --known-memcmp-ok=1 \\
  # --known-mpi-c-double-complex=1 \\
  # --known-mpi-long-double=0 \\
  # --known-mpi-shared-libraries=0 \\
  # --known-sizeof-MPI_Comm=4 \\
  # --known-sizeof-MPI_Fint=4 \\
  # --known-sizeof-char=1 \\
  # --known-sizeof-double=8 \\
  # --known-sizeof-float=4 \\
  # --known-sizeof-int=4 \\
  # --known-sizeof-long-long=8 \\
  # --known-sizeof-long=8 \\
  # --known-sizeof-short=2 \\
  # --known-sizeof-size_t=8 \\
  # --known-sizeof-void-p=8 \\
  # --CC=cc \\
  # --CXX=CC \\
  # --FC=ftn \\
  # --enable-debug=0 \\
  # --enable-shared=1 \\
  # --with-precision=double \\
  # --with-ar=ar \\
  # --with-batch=0 \\
  # --with-cc=cc \\
  # --with-clib-autodetect=0 \\
  # --with-cxx=CC \\
  # --with-cxxlib-autodetect=0 \\
  # --with-debugging=0 \\
  # --with-dependencies=0 \\
  # --with-fc=ftn \\
  # --with-fortran-datatypes=0 \\
  # --with-fortran-interfaces=0 \\
  # --with-fortranlib-autodetect=0 \\
  # --with-ranlib=ranlib \\
  # --with-scalar-type=real \\
  # --with-shared-ld=ar \\
  # --with-etags=0 \\
  # --with-x=0 \\
  # --with-ssl=0 \\
  # --with-shared-libraries=0 \\
  # --with-mpi=1 \\
  # --with-mpiexec=srun \\
  # --with-blas-lapack=1 \\
  # --with-superlu=1 \\
  # --with-superlu-dir=${prefix} \\
  # --with-superlu_dist=1 \\
  # --with-superlu_dist-dir=${prefix} \\
  # --with-parmetis=1 \\
  # --with-metis=1 \\
  # --with-metis-dir=${prefix} \\
  # --with-scalapack=1 \\
  # --with-ptscotch=1 \\
  # --with-ptscotch-include="${prefix}/include" \\
  # --with-ptscotch-lib="-L${prefix}/lib -lptscotch -lscotch -lptscotcherr -lscotcherr" \\
  # --with-mumps=1 \\
  # --with-mumps-include="${prefix}/include" \\
  # --with-mumps-lib="-L${prefix}/lib -lcmumps -ldmumps -lesmumps -lsmumps -lzmumps -lmumps_common -lptesmumps -lesmumps -lpord ${MUMPS_EXTRA_LIBS}" \\
  # --with-hdf5=1 \\
  # --with-hdf5-dir=$INSTALL_DIR/hdf5-1.10.7 \\
  # --CFLAGS="$CFLAGS $OMPFLAG" \\
  # --CPPFLAGS="-I$prefix/include $CPPFLAGS" \\
  # --CXXFLAGS="$CFLAGS $OMPFLAG" \\
  # --with-cxx-dialect=C++11 \\
  # --FFLAGS="$FFLAGS $FOMPFLAG" \\
  # --LDFLAGS="-L$prefix/lib $OMPFLAG" \\
  # --LIBS="$PE_LIBS $LIBS -lstdc++" \\
  # --prefix=$prefix$

export ROOT_SHARED_DIR=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost

./configure \
--prefix=$ROOT_SHARED_DIR/petsc-3.11.4/install \
--with-mpi=1 \
--CC=cc \
--CFLAGS=-O3 \
--CXX=CC \
--CXXFLAGS=-O3 \
--with-cxx-dialect=C++11 \
--FC=ftn \
--FFLAGS=-O3 \
--enable-debug=0 \
--enable-shared=1 \
--with-precision=double \
--with-hdf5=0 \
--with-hdf5-dir=$ROOT_SHARED_DIR/hdf5-1.10.7_install \
--download-superlu=yes \
--download-superlu_dist=yes \
--download-metis=yes \
--download-parmetis=yes \
--download-ptscotch=yes \
--with-scalapack=1 \
--with-mumps=1 \
--with-mumps-include="/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/MUMPS_5.2.1/include" \
--with-mumps-lib="-L/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/MUMPS_5.2.1/lib -lcmumps -ldmumps -lesmumps -lsmumps -lzmumps -lmumps_common -lptesmumps -lesmumps -lpord"
  
  
##--with-sundials=1 \
####--with-sundials-dir=$ROOT_SHARED_DIR/SUNDIALS/sundials-4.0.2/install \
##--download-scalapack=yes \
##--download-mumps=yes
#--with-hdf5-dir=$ROOT_SHARED_DIR/hdf5-1.10.7 \
