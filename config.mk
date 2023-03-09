# dwm version
VERSION = 6.4

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# Xinerama, comment if you don't want it
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA

# Shape extension flags
XSHAPELIBS = -lXext
XSHAPEFLAGS= -DSHAPE

# freetype
FREETYPELIBS = -lfontconfig -lXft
ifeq ($(shell uname),OpenBSD)
FREETYPEINC = ${X11INC}/freetype2
else
FREETYPEINC = /usr/include/freetype2
endif

# includes and libs
INCS = -I${X11INC} -I${FREETYPEINC} `pkg-config --cflags xft pango pangoxft`
LIBS = -L${X11LIB} -lX11 ${XINERAMALIBS} ${XSHAPELIBS} ${FREETYPELIBS} -lXrender `pkg-config --libs xft pango pangoxft`

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS} ${XSHAPEFLAGS}
#CFLAGS   = -g -std=c99 -pedantic -Wall -O0 ${INCS} ${CPPFLAGS}
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
