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


architecture default of ALU is
    -- Intermediate signal
    signal Y : std_logic_vector (31 downto 0);

begin

    -- Output depends on A, B and OP
    process (A, B, OP)
    begin
        -- OP = 00: Add A and B together
        if OP = "00" then
            Y <= std_logic_vector(signed(A) + signed(B));
        -- OP = 01: Let B pass through
        elsif OP = "01" then
            Y <= B;
        -- OP = 10: Subtract A from B
        elsif OP = "10" then
            Y <= std_logic_vector(signed(A) - signed(B));
        -- OP = 11: Let A pass through
        else
            Y <= A;
        end if;
    end process;

    -- The 31th byte of the output indicates wether it is negative or not
    N <= Y(31);
    -- S takes the value of the intermediate signal Y
    O <= Y;

end architecture;
