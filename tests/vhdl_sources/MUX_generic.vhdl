library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity MUX is
    generic (
        N : integer range 0 to 32;
        M : time 0.2 ns
    );
    port (
        A, B : in std_logic_vector (N - 1 downto 0);
        COM : in std_logic;
        Y : out std_logic_vector (N - 1 downto 0)
    );
end entity;


architecture default of MUX is
begin

    Y <= A when (COM = '0') else B;

end architecture;
