include ../../Makefile.include
include ../Makefile.include # Library project include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = $(LIBRARY_FILENAME)

.PHONY: all thin

all: $(OBJECTS)
	ar rcs $(TARGET) $^

thin: $(OBJECTS)
	ar rcsT $(TARGET) $^

clean:
	$(RM) $(TARGET) $(OBJECTS)
