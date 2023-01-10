# Fonts
The fonts folder and subfolder show how to add fonts to a project.

The code uses bitmapped monospaced fonts. The font data is stored in
an unsigned char array. The bits in each byte correspond to the foreground
pixels in the glyph. The bytes are stored across the rows. If there are less than multiple of 8 pixels in width, the last byte in each row are padded with zero bits. The bytes are stored MSB first.

The [fontedit](https://github.com/ayoy/fontedit) program can be used to
create these data files. Under Misc Options choose *Export All Glyphs*, select *Reverse Bits (MSB)*, de-select *Invert Bits* and *Include Line Spacing.*

fontedit exports glyphs for ASCII 32 (space) through ASCII 126. To save spce, the program doesn't use the glpyh for space, but clears the character to background. Once the source file has been exported, remove the data for the space character.

To save space in the flash, the font data can be shortened by removing unneeded characters at the end. For example the all characters after the digits can be removed in only digits are used in the program.

Create a subdirectory for each font family and style (i.e. plain, italic, bold).

For each size, there are two files: *fontname and size*data.c and *fontname and size*.c. *fontname and size*.c uses the FONT_SETUP macro to describe the font. It is in a separate file so the *fontname and sizedata*.c file can be be regenerated.

There is also a *fontname*.h file which has extern references to the individual fold files. Include this file in the project to reference the fonts.

By including the individual files in a library, only the fonts actually used in the project are linked into the final .uf2 file.
