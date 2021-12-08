#!/bin/sh

#################################
## Which version of the code ? ##
#################################

export CODE_VERSION=6.0.6
export KER_VERSION=${CODE_VERSION}
export KERNAME=code_saturne-${KER_VERSION}

################################################
## Installation PATH in the current directory ##
################################################

export INSTALLPATH=`pwd`
echo $INSTALLPATH

#####################################
## Environment variables and PATHS ##
#####################################
  
export NOM_ARCH=`uname -s`
export CS_HOME=${INSTALLPATH}/${KERNAME}

export PATH=$CS_HOME/bin:$PATH

##############
## Cleaning ##
##############

rm -rf $CS_HOME/arch/*
rm -rf $INSTALLPATH/$KERNAME.build

#########################
## Kernel Installation ##
#########################

export KERSRC=$INSTALLPATH/$KERNAME
export KERBUILD=$INSTALLPATH/$KERNAME.build/arch/$NOM_ARCH
export KEROPT=$INSTALLPATH/$KERNAME/arch/$NOM_ARCH

export PTSCOTCHPATH=${INSTALL_FOLDER}/FEniCS/V2019.1.0/boost/scotch_6.1.0

mkdir -p $KERBUILD
cd $KERBUILD

$KERSRC/configure                                            \
--enable-shared                                              \
--disable-nls                                                \
--without-modules                                            \
--disable-gui                                                \
--enable-long-gnum                                           \
--disable-mei                                                \
--enable-debug                                               \
--with-scotch-include=$PTSCOTCHPATH/include                  \
--with-scotch-lib=$PTSCOTCHPATH/lib                          \
--prefix=$KEROPT                                             \
--with-blas=$CRAY_LIBSCI_PREFIX_DIR                          \
--with-blas-include=$CRAY_LIBSCI_PREFIX_DIR/include          \
--with-blas-lib=$CRAY_LIBSCI_PREFIX_DIR/lib                  \
--with-blas-libs=-lsci_gnu                                   \
CC="cc"                                                      \
CFLAGS="-O3"                                                 \
FC="ftn"                                                     \
FCFLAGS="-O3"                                                \
CXX="CC"                                                     \
CXXFLAGS="-O3"

make -j 8
make install

cd $INSTALLPATH

