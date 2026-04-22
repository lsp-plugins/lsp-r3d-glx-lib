# lsp-r3d-glx-lib

GLX-based 3D backend library for rendering scenes

It provides:
* 3D rendering backend that uses OpenGL 2.0 library and GLX extensions. 

## Supported platforms

The build and correct unit test execution has been confirmed for following platforms:
* FreeBSD
* GNU/Linux

Requirements
======

The following packages need to be installed for building:

* gcc >= 4.9
* make >= 4.0

Building
======

To build the library, perform the following commands:

```bash
make config # Configure the build
make fetch # Fetch dependencies from Git repository
make
sudo make install
```

To get more build options, run:

```bash
make help
```

To uninstall library, simply issue:

```bash
make uninstall
```

To clean all binary files, run:

```bash
make clean
```

To clean the whole project tree including configuration files, run:

```bash
make prune
```
