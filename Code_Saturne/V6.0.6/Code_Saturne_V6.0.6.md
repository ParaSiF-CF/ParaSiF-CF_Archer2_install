Instructions for compiling Code_Saturne V6.0.6 for ARCHER2
======================================================
These instructions are for compiling Code_Saturne V6.0.6 on 
[ARCHER2](https://www.archer2.ac.uk).
Code_Saturne V6.0.6 is installed with the following utilities:

```bash
  -GNU: 10.2.0
  -python: 3.8.5

  -scotch: v6.1.0
```

Create the build folders and get the distribution
---------------------------------------------
For sake of clarity, Code_Saturne is installed in the same folder as FEniCS (see [FEniCS_2019.1.0.md](https://github.com/ParaSiF-CF/ParaSiF-CF_Archer2_install/blob/master/FEniCS/V2019.1.0/FEniCS_2019.1.0.md)), and the $INSTALL_FOLDER environment variable is the same as for FEniCS.

```bash
  cd $INSTALL_FOLDER

  mkdir SATURNE
  mkdir SATURNE/V6.0.6

  cd SATURNE/V6.0.6
  wget https://www.code-saturne.org/releases/code_saturne-6.0.6.tar.gz
  tar zxvf code_saturne-6.0.6.tar.gz
```

Install the software
---------------------------------------------
For this purpose the file [InstallHPC.sh](https://github.com/ParaSiF-CF/ParaSiF-CF_Archer2_install/blob/master/Code_Saturne/V6.0.6/InstallHPC.sh) is used. It should be copied under $INSTALL_FOLDER/SATURNE/V6.0.6 on ARCHER2 and run as follows:

```bash
  chmod 755 ./InstallHPC.sh
  ./InstallHPC.sh
```

Check the installation
---------------------------------------------
The idea is to test the command $INSTALL_FOLDER/SATURNE/V6.0.6/code_saturne-6.0.6/arch/Linux/bin/code_saturne, which should return:

```bash
  Usage: $INSTALL_FOLDER/SATURNE/V6.0.6/code_saturne-6.0.6/arch/Linux/bin/code_saturne <topic>

  Topics:
    help
    studymanager
    smgr
    bdiff
    bdump
    compile
    config
    create
    gui
    studymanagergui
    smgrgui
    trackcvg
    update
    up
    info
    run
    salome
    submit

  Options:
    -h, --help  show this help message and exit
```

Activate the link to MUI
---------------------------------------------
The instructions to perform this operation are to be found in [Code_Saturne_MUI_Coupling](https://github.com/MUI-Utilities/MUI_Utilities/tree/main/Code_Saturne_MUI_Coupling), which explains that 2 python files of Code_Saturne's distribution (V6.0.6) have to be changed. The 2 files are cs_compile.py and cs_config.py. They have to be updated following the instructions in [Code_Saturne_MUI_Coupling](https://github.com/MUI-Utilities/MUI_Utilities/tree/main/Code_Saturne_MUI_Coupling), and are to be found in the following folder of ARCHER2: $INSTALL_FOLDER/SATURNE/V6.0.6/code_saturne-6.0.6/arch/Linux/lib/python3.8/site-packages/code_saturne.
