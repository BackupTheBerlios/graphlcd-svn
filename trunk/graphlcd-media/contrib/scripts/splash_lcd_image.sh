#! /bin/sh
#

# This script shows an animated picture on your GLCD. Refering to an Idea of HULK (vdr-portal).
# Take a look at contrib/splash/README
# Hannes Stein <hannsens@macnews.de>

# -----------------------------------------------------------------------------------
# --------------- TYPE YOUR DEFAULT PATHS AND CONFIG HERE ---------------------------
# -----------------------------------------------------------------------------------

# ------
# PATHS

# Where is showpic located?
#	Path of the Showpic Program. Showpic is normally found in the GLCD-Sources in "tools/showpic/showpic"
#	If you linked it to a PATH you might find it by typing "whereis showpic"
# ??????????
SHOWPIC=/usr/local/bin/showpic

# Where are the directories with the images that you want to show?
#	This is the path to you "splash" directory. You should find it in /etc/vdr/plugins/graphlcd/splash or 
#	/video/plugins/graphlcd/splash by default - if you don't have it, just copy this directory from the sources
#	(you can find it in contrib/splash).
# ??????????
IMAGEPATH=/etc/vdr/plugins/graphlcd/splash

## ----
# CONFIG

# What is the configuration of your Display? 
#	Just like the options needed for showpic and starting VDR
#	see the Showpic README
#	If you've got an inverted Display you might use "-i" here as well to see the colors the right way 'round...
#	c't-VDR: Just use the entry in your /etc/vdr/plugins/plugin.graphlcd.conf
# ??????????
CONFIGOPTS="-c ks0108 -d /dev/parport0 -x 128 -y 64 -i"
CONFIGOPTS_shutdown="-c ks0108 -d /dev/parport0 -x 128 -y 64"

# How long should each picture be displayed? (Miliseconds / ms)
# ??????????
DISPLAYED_TIME=800
DISPLAYED_TIME_shutdown=300



# -----------------------------------------------------------------------------------
# ----------------- YOU SHOULD NOT NEED TO EDIT BELOW THIS LINE ---------------------
# -----------------------------------------------------------------------------------
 
	IMAGEPATH="$IMAGEPATH/$1"

	IMAGELIST=`find $IMAGEPATH -name "*.glcd" -follow -printf "%f\n"`;

#       printf "\nPictures found in $IMAGEPATH for GLCD-Splash:\n$IMAGELIST \n" ;

	for i in $IMAGELIST; do
	ALL_IMAGES="$ALL_IMAGES $IMAGEPATH/$i";
	done

printf "\n Showing GLCD-Splashimage \n";
 if [ "$1" = stop ]; then
	CONFIGOPTS=$CONFIGOPTS_shutdown ;
	DISPLAYED_TIME=$DISPLAYED_TIME_shutdown ;
	printf "\n (GLCD shutdown animation) \n";

fi ;

printf "$SHOWPIC $CONFIGOPTS -s $DISPLAYED_TIME $ALL_IMAGES & ";

`$SHOWPIC $CONFIGOPTS -s $DISPLAYED_TIME $ALL_IMAGES`;
