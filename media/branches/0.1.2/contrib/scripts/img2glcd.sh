#!/bin/sh
# Converts a xcf or gif image with multiple layer to *.glcd

CONVPIC=/usr/local/src/vdr/plugins/graphlcd/graphlcd/tools/convpic/convpic 
PREFIX=vdr-starting_240x128_
###################################################################
# Splitt XCF or GIF to PNG
convert -monochrome $1 PNG:$PREFIX%02d.png

###################################################################
# Translate PNG2BMP
old=png
new=bmp
for file in ./*."$old"; do
  out=$(echo "$file" | sed -e s/$old/$new/g;)
  convert -colors 2 "$file"  "$out"
  rm -f $file
done
###################################################################
# Translate BMP2GLCD
old=bmp
new=glcd
for file in ./*."$old"; do
  $CONVPIC -i "$file" -o "`basename \"$file\" \"$old\"`$new"
  rm -f $file
done

