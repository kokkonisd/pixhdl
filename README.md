# pixhdl

![master build status](https://travis-ci.org/kokkonisd/pixhdl.svg?branch=master)
![development build status](https://travis-ci.org/kokkonisd/pixhdl.svg?branch=development)

<img src="https://github.com/kokkonisd/pixhdl/blob/master/pixhdl.png" width="500">

A command-line tool that produces graphical representations of entities from
VHDL source files.

## examples

### simple entity (ALU)

Source:

```vhdl
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity ALU is
    port (
        OP : in std_logic_vector (1 downto 0);
        A, B : in std_logic_vector (31 downto 0);
        O : out std_logic_vector (31 downto 0);
        N : out std_logic
    );
end entity;

...
```

Output:

![ALU SVG output](ALU_diagram.svg)

### entity with generic inputs (MUX)

Source:

```vhdl
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity MUX is
    generic (
        N : integer range 0 to 32
    );
    port (
        A, B : in std_logic_vector (N - 1 downto 0);
        COM : in std_logic;
        Y : out std_logic_vector (N - 1 downto 0)
    );
end entity;

...
```

Output:

![MUX SVG output](MUX_diagram.svg)


## how to use pixhdl

Pixhdl takes in a `.vhd`/`.vdhl` source file and generates an SVG image as an
output.

For example, running

```bash
$ pixhdl my_vhdl_source.vhdl
```

will generate a file called `my_vhdl_source.svg` in the same directory as the
source file.

If you want to specify where the output file should be created and how it
should be called, you can run:

```bash
$ pixhdl my_vhdl_source.vhdl -o ~/my_entity_diagram.svg
$ # you can also do:
$ # pixhdl -o ~/my_entity_diagram.svg my_vhdl_source.vhdl
```

This will generate an image called `my_entity_diagram.svg` in your home
directory (`~/`).

To get an exhaustive list of arguments, you can run:

```bash
$ pixhdl -h
```


## how to install pixhdl

In order to install pixhdl, get the
[latest release](https://github.com/kokkonisd/pixhdl/releases/latest),
decompress it, `cd` into it, then run:

```bash
$ make
$ sudo make install
```

That will install pixhdl on your computer. You can verify that the installation
was successful by running:

```bash
$ pixhdl -v
```

This command will show the current version of pixhdl installed on your
computer.

## how to uninstall pixhdl

In order to uninstall pixhdl, you have to `cd` into the folder you got from
when you [installed](#how-to-install-pixhdl) it. If you no longer have that
folder, you can just download the
[latest release](https://github.com/kokkonisd/pixhdl/releases/latest),
decompress it, and `cd` into that.

Once you're into your `pixhdl/` folder, you can run:

```bash
$ sudo make uninstall
```

That will uninstall pixhdl from your computer if it was previously installed.


## further documentation

To read up on more detailed documentation, you can go on pixhdl's
[official site](https://kokkonisd.github.io/pixhdl/).

Or if you prefer, you can generate the docs locally with `doxygen`:

```bash
$ doxygen
```

The docs will be generated in the `docs/doxygen/` directory. Open the
`docs/doxygen/html/index.html` file and read away!
