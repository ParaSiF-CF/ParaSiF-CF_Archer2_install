#!/bin/bash
# Exit on error
set -e

git clone https://github.com/hpc-uk/build-instructions.git
cp -r build-instructions/apps/OpenFOAM/ARCHER2-OpenFOAM-v6.20180710/site ./
cp -r build-instructions/apps/OpenFOAM/ARCHER2-OpenFOAM-v6.20180710/README.md ./


export INSTALL_DIR=/work/c01/c01/wendiliu/app/OpenFoam/V6

cp source/OF_6_archer_install.tar.gz ./
tar -xzf OF_6_archer_install.tar.gz
rm OF_6_archer_install.tar.gz

bash ./site/install.sh

. /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf

bash ./site/pre-configure.sh

chmod -R 777 OpenFOAM-6
chmod -R 777 ThirdParty-6

./patch_OF6-MUI

cd ThirdParty-6
rm -r MUI
# create link to MUI folder
# mkdir MUI/
# ln -s /work/c01/c01/wendiliu/app/MUI/MUI-1.1 ./MUI
cd ..

#change etc/bashrc
# L47
# +++ export FOAM_INST_DIR=/work/c01/c01/wendiliu/app/OpenFoam/V6/install/$WM_PROJECT
#
# L108
# +++ /work/c01/c01/wendiliu/app/OpenFoam/V6/install/$WM_PROJECT/$USER $FOAM_USER_APPBIN $FOAM_USER_LIBBIN \
#
# L132
# +++ export WM_PROJECT_USER_DIR=/work/c01/c01/wendiliu/app/OpenFoam/V6/install/$WM_PROJECT/$USER-$WM_PROJECT_VERSION


sbatch --account=c01-eng ./site/q-compile.sh



#rm -r OpenFOAM-6/ ThirdParty-6/ *.changes *.patch compile_all_OF6 patch_all_OF6 patch_OF6-MUI