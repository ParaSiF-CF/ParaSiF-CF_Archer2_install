#!/bin/bash
# Exit on error
set -e

export INSTALL_DIR=/work/c01/c01/wendiliu/app/OpenFoam/V6

cp source/OpenFOAM-6-version-6.tar.gz ./
tar -xzf OpenFOAM-6-version-6.tar.gz
rm OpenFOAM-6-version-6.tar.gz
mv OpenFOAM-6-version-6/ OpenFOAM-6/

cp source/ThirdParty-6-version-6.tar.gz ./
tar -xzf ThirdParty-6-version-6.tar.gz
rm ThirdParty-6-version-6.tar.gz
mv ThirdParty-6-version-6/ ThirdParty-6/

cp source/OF_6_archer_install.tar.gz ./
tar -xzf OF_6_archer_install.tar.gz
rm OF_6_archer_install.tar.gz

. /work/c01/c01/wendiliu/app/FEniCS/V2019.1.0/fenics2019_eCSE_FSI.conf

bash ./site/pre-configure.sh

chmod -R 777 OpenFOAM-6
chmod -R 777 ThirdParty-6

./patch_OF6-MUI
echo $pwd

cd ThirdParty-6
rm -r MUI
# create link to MUI folder
# mkdir MUI/
# ln -s /work/c01/c01/wendiliu/app/MUI/v1.0_dev ./MUI
cd ..

sbatch --account=c01-eng ./site/q-compile.sh



#rm -r OpenFOAM-6/ ThirdParty-6/ *.changes *.patch compile_all_OF6 patch_all_OF6 patch_OF6-MUI