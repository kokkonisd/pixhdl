library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ALU is
    port (
        OP : in std_logic_vector (1 downto 0);
        A, B, C, D, E, F, G : in std_logic_vector (31 downto 0);
        O : out std_logic_vector (31 downto 0);
        N : out std_logic
    );
end entity;
