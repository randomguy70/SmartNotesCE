# Put each of your .fnt files on this next line.
# Look at the documentation for convfont for more information on font properties
temp.bin: ex.fnt
	convfont -o fontpack -N "fira" -P "ASCII" -V "1" -A "randomguy" \
	-D "example font" \
	-f ex.fnt -a 1 -b 1 -w bold -s sans-serif -s upright -s proportional \

#------------------
# Don't forget to change font_pack_file_name on both these lines.
# Set PACKNAME to the on-calculator appvar name you want
fira.8xv: temp.bin
	convhex -a -v -n fira temp.bin fira.8xv

all: fira.8xv