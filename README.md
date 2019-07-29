# pixhdl

<img src="https://github.com/kokkonisd/pixhdl/blob/master/pixhdl.png" width="500">

A command-line tool that produces graphical representations of entities from VHDL source files.


## how to use pixhdl

Pixhdl takes in a `.vhd`/`.vdhl` source file and generates an SVG image as an output.

For example, running

```bash
$ pixhdl my_vhdl_source.vhdl
```

will generate a file called `my_vhdl_source.svg` in the same directory as the source file.

If you want to specify where the output file should be created and how it should be called, you can run:

```bash
$ pixhdl my_vhdl_source.vhdl -o ~/my_entity_diagram.svg
$ # you can also do:
$ # pixhdl -o ~/my_entity_diagram.svg my_vhdl_source.vhdl
```

This will generate an image called `my_entity_diagram.svg` in your home directory (`~/`).

To get an exhaustive list of arguments, you can run:

```bash
$ pixhdl -h
```


## how to install pixhdl

In order to install pixhdl, get the [latest release](), decompress it, `cd` into it, then run:

```bash
$ make
$ make install
```

That will install pixhdl on your computer. You can verify that the installation was successful by running:

```bash
$ pixhdl -v
```

This command will show the current version of pixhdl installed on your computer.

## how to uninstall pixhdl

In order to uninstall pixhdl, you have to `cd` into the folder you got from when you [installed](#how-to-install-pixhdl) it. If you no longer have that folder, you can just download the [latest release](), decompress it, and `cd` into that.

Once you're into your `pixhdl/` folder, you can run:

```bash
$ make uninstall
```

That will uninstall pixhdl from your computer if it was previously installed.
