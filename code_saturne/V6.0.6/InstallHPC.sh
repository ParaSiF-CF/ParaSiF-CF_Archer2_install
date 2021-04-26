#!/bin/sh

#################################
## Which version of the code ? ##
#################################

CODE_VERSION=6.0.6
KER_VERSION=${CODE_VERSION}
KERNAME=code_saturne-${KER_VERSION}

#################################
## Module Load ##
#################################

module restore PrgEnv-gnu
# module load hypre/2.18.0
# module load mumps/5.2.1
# module load parmetis/4.0.3
# module load petsc/3.13.3 
# module load scotch/6.0.10
# module load superlu-dist/6.1.1
# module load superlu/5.2.1
module load cray-python
# module load cray-hdf5-parallel
module load cmake
# module load qt # Can be skipped if building without GUI
# module load perftools-base/6.3.0
# module load perftools/6.3.0
export CXX=CC
export CC=cc
export FC=ftn
export F77=ftn
export F90=ftn

################################################
## Installation PATH in the current directory ##
################################################

INSTALLPATH=`pwd`

echo $INSTALLPATH

#####################################
## Environment variables and PATHS ##
#####################################
  
NOM_ARCH=`uname -s`

CS_HOME=${INSTALLPATH}/${KERNAME}

export PATH=$CS_HOME/bin:$PATH

##############
## Cleaning ##
##############

rm -rf $CS_HOME/arch/*
rm -rf $INSTALLPATH/$KERNAME.build

wget https://www.code-saturne.org/cms/sites/default/files/releases/code_saturne-6.0.6.tar.gz
tar -xzf code_saturne-6.0.6.tar.gz
rm code_saturne-6.0.6.tar.gz


#########################
## Kernel Installation ##
#########################

KERSRC=$INSTALLPATH/$KERNAME
KERBUILD=$INSTALLPATH/$KERNAME.build/arch/$NOM_ARCH
KEROPT=$INSTALLPATH/$KERNAME/arch/$NOM_ARCH
TPBUILD=$INSTALLPATH/thirdParty.build

export KEROPT

PARMETISPATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/parmetis-4.0.3
PTSCOTCHPATH=/work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/boost/scotch_6.0.10

mkdir -p $KERBUILD
mkdir -p $TPBUILD

cd $KERBUILD

$KERSRC/configure          \
--enable-shared            \
--disable-nls              \
--without-modules          \
--disable-gui              \
--enable-long-gnum         \
--disable-mei              \
--enable-debug             \
--with-metis-include=$PARMETISPATH/include   \
--with-metis-lib=$PARMETISPATH/lib           \
--with-scotch-include=$PTSCOTCHPATH/include  \
--with-scotch-lib=$PTSCOTCHPATH/lib          \
--prefix=$KEROPT           \
CC="cc"                    \
CFLAGS="-O3"               \
FC="ftn"                   \
FCFLAGS="-O3"              \
CXX="CC"                   \
CXXFLAGS="-O3"

###make -j 8
###make install
###make distclean

cd $INSTALLPATH
###rm -rf $INSTALLPATH/Kernel/ncs-${KER_VERSION}.build
export PATH=/work/c01/c01/wendiliu/app/code_saturne/V6.0.6/code_saturne-6.0.6/arch/Linux/bin:$PATH

