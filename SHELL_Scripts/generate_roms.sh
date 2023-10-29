#!/bin/sh

# set -x 

# This script create ROM module from big file with case/when's conditions
# and it's used to generate Distributed ROM for one frame VGA 640*480 with 24bit color
# so we must have 307200 when line's (0 - 307199) in file roma.vhd
 
# File roma.vhd must look's like:
# -- start address
# when 0 => data <= std_logic_vector(to_unsigned([8bit integer],8)) & std_logic_vector(to_unsigned([8bit integer],8)) & std_logic_vector(to_unsigned([8bit integer],8));
# ...
# when [address integer] => data <= std_logic_vector(to_unsigned([8bit integer],8)) & std_logic_vector(to_unsigned([8bit integer],8)) & std_logic_vector(to_unsigned([8bit integer],8));
# ...
# -- end address plus others
# when others => data <= x"000000";

split -l 1024 -d roma.vhd rom # XXX small pieces, extremely speed-up synthesis time XXX

# file ranges
for i in `find . -maxdepth 1 -regex "\.\/rom[0-9]*" | sort`;
do 

rom=`basename $i`;

OTHERS="others";
MIN="`head -1 ${rom} | awk -F " " '{print $2;}'`";
MAX="`tail -1 ${rom} | awk -F " " '{print $2;}'`";
if [ "${MAX}" = "${OTHERS}" ]; then
MAX=`tail -2 ${rom} | head -1 | awk -F " " '{print $2;}'`;
fi

STRING_HEAD="\
LIBRARY ieee;\n\
USE ieee.std_logic_1164.ALL;\n\
USE ieee.numeric_std.ALL;\n\
ENTITY ROM_${rom} IS\n\
PORT (\n\
address : IN INTEGER RANGE ${MIN} TO ${MAX};\n\
data : OUT STD_LOGIC_VECTOR (23 DOWNTO 0)\n\
);\n\
END ENTITY ROM_${rom};\n\
ARCHITECTURE BEHAVIORAL_${rom} OF ROM_${rom} IS\n\
BEGIN\n\
p0_${rom} : PROCESS (address) IS\n\
BEGIN\n\
CASE (address) IS\n\
";

STRING_TAIL="\
END CASE;\n\
END PROCESS p0_${rom};\n\
END ARCHITECTURE BEHAVIORAL_${rom};\n\
";

# echo "${STRING_HEAD}\n${STRING_TAIL}";
# echo "${MIN} - ${MAX}"

sed -i "1s/^/${STRING_HEAD}/" $i;
sed -i "$ a ${STRING_TAIL}" $i;

mv "${rom}" "${rom}.vhd"

done

# rom mux file

rm -rf rom.vhd
touch rom.vhd

echo "-- comment" > rom.vhd

# head
ROMMUX_HEAD="\
LIBRARY ieee;\n\
USE ieee.std_logic_1164.ALL;\n\
USE ieee.numeric_std.ALL;\n\
ENTITY ROM_MUX IS\n\
PORT (\n\
address : IN INTEGER RANGE 0 TO 640*480-1;\n\
data : OUT STD_LOGIC_VECTOR (23 DOWNTO 0)\n\
);\n\
END ENTITY ROM_MUX;\n\
ARCHITECTURE BEHAVIORAL_ROM_MUX OF ROM_MUX IS\n\
";

sed -i "1s/^/${ROMMUX_HEAD}/" rom.vhd;

p0_data_sens_list="";

# component
for i in `find . -maxdepth 1 -regex "\.\/rom[0-9]+.vhd" | sort`;
do 

rom=`basename $i`;
rom1=`basename $i ".vhd"`;

OTHERS="others";
MIN="`grep "when " ${rom} | head -1 | awk -F " " '{print $2;}'`";
MAX="`grep "when " ${rom} | tail -1 | awk -F " " '{print $2;}'`";
#echo $MIN
#echo $MAX
if [ "${MAX}" = "${OTHERS}" ]; then
MAX=`grep "when " ${rom} | tail -2 | head -1 | awk -F " " '{print $2;}'`;
fi

COMPONENT="\
COMPONENT ROM_${rom1} IS\n\
PORT (\n\
address : IN INTEGER RANGE ${MIN} TO ${MAX};\n\
data : OUT STD_LOGIC_VECTOR (23 DOWNTO 0)\n\
);\n\
END COMPONENT ROM_${rom1};\n\
SIGNAL ROM_${rom1}_address : INTEGER RANGE ${MIN} TO ${MAX};\n\
SIGNAL ROM_${rom1}_data : STD_LOGIC_VECTOR (23 DOWNTO 0);\n\
";
sed -i "$ a ${COMPONENT}" rom.vhd

done

sed -i "$ a BEGIN\n" rom.vhd

# instantinate
for i in `find . -maxdepth 1 -regex "\.\/rom[0-9]+.vhd" | sort`;
do 

rom=`basename $i`;
rom1=`basename $i ".vhd"`;

OTHERS="others";
MIN="`grep "when " ${rom} | head -1 | awk -F " " '{print $2;}'`";
MAX="`grep "when " ${rom} | tail -1 | awk -F " " '{print $2;}'`";
#echo $MIN
#echo $MAX
if [ "${MAX}" = "${OTHERS}" ]; then
MAX=`grep "when " ${rom} | tail -2 | head -1 | awk -F " " '{print $2;}'`;
fi

INSTANT="\
inst_ROM_${rom1} : ROM_${rom1}\n\
PORT MAP (\n\
address => ROM_${rom1}_address,\n\
data => ROM_${rom1}_data\n\
);\n\
";
sed -i "$ a ${INSTANT}" rom.vhd

p0_data_sens_list="${p0_data_sens_list},ROM_${rom1}_data"

done

P0a="\
p0 : process (address) is\n\
begin\n\
case (address) is\n\
";

sed -i "$ a ${P0a}\n" rom.vhd

# p0 address
for i in `find . -maxdepth 1 -regex "\.\/rom[0-9]+.vhd" | sort`;
do 

rom=`basename $i`;
rom1=`basename $i ".vhd"`;
rom2=`basename $i ".vhd" | cut -b 4-`;

OTHERS="others";
MIN="`grep "when " ${rom} | head -1 | awk -F " " '{print $2;}'`";
MAX="`grep "when " ${rom} | tail -1 | awk -F " " '{print $2;}'`";
#echo $MIN
#echo $MAX
if [ "${MAX}" = "${OTHERS}" ]; then
MAX=`grep "when " ${rom} | tail -2 | head -1 | awk -F " " '{print $2;}'`;
fi

RANGE="when ${MIN} to ${MAX} => ROM_rom${rom2}_address <= address; -- (${MIN} to ${MAX});";
sed -i "$ a ${RANGE}" rom.vhd

done

P0b="\
when others => null; -- data <= x\"000000\";\n\
end case;\n\
end process p0;\n\
";

sed -i "$ a ${P0b}\n" rom.vhd

# p1 data
P1a="\
p1 : process (address ${p0_data_sens_list}) is\n\
begin\n\
case (address) is\n\
";

sed -i "$ a ${P1a}\n" rom.vhd

for i in `find . -maxdepth 1 -regex "\.\/rom[0-9]+.vhd" | sort`;
do 

rom=`basename $i`;
rom1=`basename $i ".vhd"`;
rom2=`basename $i ".vhd" | cut -b 4-`;

OTHERS="others";
MIN="`grep "when " ${rom} | head -1 | awk -F " " '{print $2;}'`";
MAX="`grep "when " ${rom} | tail -1 | awk -F " " '{print $2;}'`";
#echo $MIN
#echo $MAX
if [ "${MAX}" = "${OTHERS}" ]; then
MAX=`grep "when " ${rom} | tail -2 | head -1 | awk -F " " '{print $2;}'`;
fi

RANGE="when ${MIN} to ${MAX} => data <= ROM_rom${rom2}_data;";
sed -i "$ a ${RANGE}" rom.vhd

done

P1b="\
when others => data <= x\"000000\";\n\
end case;\n\
end process p1;\n\
";

sed -i "$ a ${P1b}\n" rom.vhd

# end
P10="\
end architecture BEHAVIORAL_ROM_MUX;\n\
";

sed -i "$ a ${P10}\n" rom.vhd

