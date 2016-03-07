include ../Makefile.inc
OBJLIBS = ../libSOIL.a
OBJS = image_DXT.o  image_helper.o  SOIL.o  stb_image_aug.o

all : $(OBJLIBS)

../libSOIL.a : $(OBJS)
	$(ECHO) $(AR) $(ARFLAGS) rcs ../libSOIL.a $(OBJS)
	$(AR) $(ARFLAGS) rcs ../libSOIL.a $(OBJS)

clean :
	$(ECHO) cleaining up SOIL
	-$(RM) -f $(OBJS)

