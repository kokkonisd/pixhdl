---
layout: home
---

## Getting started

To get started, first of all you have to install pixhdl.
After downloading the [latest release](https://github.com/kokkonisd/pixhdl/releases/latest) you can install it as so:

```bash
$ tar xvzf pixhdl-X.Y.Z.tar.gz # for example pixhdl-1.1.1.tar.gz
$ cd pixhdl-X.Y.Z/
$ make
$ sudo make install
$ cd ..
$ rm -rf pixhdl-X.Y.Z/
```

After having successfully installed pixhdl, you can now use it from anywhere on your computer.
Say for example that you want to extract the entity diagram from a file called 'CPU.vhdl', all you have to do is:

```plaintext
$ pixhdl CPU.vhdl CPU_diagram.svg
```

This will produce a file called 'CPU_diagram.svg' that will contain an image of the diagram of the CPU entity.

You can also specify the output explicitly by using the `-o` or `--output` options:

```plaintext
$ pixhdl --output CPU_diagram.svg CPU.vhdl
```

This will produce the exact same result as before.

If you want, you can also choose to print the entity's signals in the terminal. By adding the `-p` or `--print` options, you don't have to specify an output file (SVG generation is optional). Here is some example output of the print option:

```plaintext
$ pixhdl -p ALU.vhdl
---------------------------------------------------------------
| Entity ALU                                                  |
---------------------------------------------------------------
Signal # | Name | Value (generics) / Length (bits) | Direction
===============================================================
---------------------------------------------------------------
1        | OP   | 2                                | IN
2        | A    | 32                               | IN
3        | B    | 32                               | IN
---------------------------------------------------------------
1        | O    | 32                               | OUT
2        | N    | 1                                | OUT
---------------------------------------------------------------
```

If you also specify an output file (either implicitly or explicitly, by adding either one of the output options), an SVG output file will be generated as well.

If you want to check for updates, you can use the `-u` or `--update` options:

```plaintext
$ pixhdl --update
```

If a newer version is available, you will be asked if you want to install it. You must then type 'yes' to accept the installation or 'no' to abort it.

To get a more comprehensive list of commands, run:

```plaintext
$ pixhdl -h
```

---

## About

Pixhdl is a command-line tool that takes VHDL source code as input and produces SVG images as output.
It is open source, and it is written entirely in C by Dimitri Kokkonis ([@kokkonisd](https://github.com/kokkonisd)).
