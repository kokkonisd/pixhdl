library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity INOUT is
    port (
        A : in std_logic_vector (3 downto 0);
        B, C, D : inout std_logic;
        E : out std_logic
    );
end entity;


architecture default of INOUT is

begin

end architecture;
