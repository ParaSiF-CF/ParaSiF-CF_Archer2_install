Instructions for compiling Code_Saturne 6.0.6 for ARCHER2
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
For sake of clarity, Code_Saturne is installed in the same folder as FEniCS (see [FEniCS_2019.1.0.md](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/FEniCS/V2019.1.0/FEniCS_2019.1.0.md)), and the $INSTALL_FOLDER environment variable is the same as for FEniCS.

```bash
  cd $INSTALL_FOLDER

  mkdir SATURNE
  mkdir V6.0.6

  cd SATURNE/V6.0.6
  wget https://www.code-saturne.org/releases/code_saturne-6.0.6.tar.gz
  tar zxvf code_saturne-6.0.6.tar.gz
```

Install the software
---------------------------------------------
For this purpose the filed [InstallHPC.sh](https://gitlab.com/Wendi-L/archer2_install/-/blob/master/Code_Saturne/V6.0.6/InstallHPC.sh) is used. It should be copied under $INSTALL_FOLDER/SATURNE/V6.0.6 on ARCHER2 and run as follows:

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

