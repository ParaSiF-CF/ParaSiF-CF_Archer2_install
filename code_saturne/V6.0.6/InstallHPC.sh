#!/bin/sh

INSTALL_FOLDER=/work/c01/c01/wendiliu/app_test
cd ${INSTALL_FOLDER}/code_saturne/V6.0.6

#################################
## Which version of the code ? ##
#################################

CODE_VERSION=6.0.6
KER_VERSION=${CODE_VERSION}
KERNAME=code_saturne-${KER_VERSION}

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

#########################
## Kernel Installation ##
#########################

KERSRC=$INSTALLPATH/$KERNAME
KERBUILD=$INSTALLPATH/$KERNAME.build/arch/$NOM_ARCH
KEROPT=$INSTALLPATH/$KERNAME/arch/$NOM_ARCH

export KEROPT

PARMETISPATH=${INSTALL_FOLDER}/FEniCS/V2019.1.0/boost/parmetis-4.0.3
PTSCOTCHPATH=${INSTALL_FOLDER}/FEniCS/V2019.1.0/boost/scotch_6.1.0

mkdir -p $KERBUILD
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
--with-blas=$CRAY_LIBSCI_PREFIX_DIR          \
--with-blas-include=$CRAY_LIBSCI_PREFIX_DIR/include          \
--with-blas-lib=$CRAY_LIBSCI_PREFIX_DIR/lib          \
--with-blas-libs=-lsci_gnu          \
CC="cc"                    \
CFLAGS="-O3"               \
FC="ftn"                   \
FCFLAGS="-O3"              \
--with-mpi             \
--host=x86_64-unknown-linux-gnu             \
CXX="CC"                   \
CXXFLAGS="-O3"

###make -j 8
###make install
###make distclean

cd $INSTALLPATH
###rm -rf $INSTALLPATH/Kernel/ncs-${KER_VERSION}.build
export PATH=${INSTALL_FOLDER}/code_saturne/V6.0.6/code_saturne-6.0.6/arch/Linux/bin:$PATH

