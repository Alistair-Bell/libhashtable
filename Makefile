
include           config.mk

SOURCES			= src/algor.c src/imp.c
OBJECTS			= ${SOURCES:.c=.o}
OUT             = hashtable
OUT_LIB         = libhashtable.a

.c.o:
	$(CC) -c ${CONFIG_CC_FLAGS} $< -o $@

all: ${OUT}


${OUT}: ${OBJECTS}
	$(AR) rcs ${OUT_LIB} ${OBJECTS}
	

clean:
	rm ${OUT_LIB}
	rm src/*.o
	
