TARGET = SimpleAmp
OBJS = main.o graphics.o framebuffer.o mp3player.o lightmap.o error.o

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS =  -lpspgum -lpspgu -lpng -lz -lm -lmad -lpsppower -lpspaudiolib -lpspaudio
LDFLAGS =


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = SimpleAmp v3
PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_PIC1 = PIC1.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

lightmap.o : lightmap.raw
	bin2o -i lightmap.raw lightmap.o lightmap
