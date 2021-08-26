-include config.mk

SOURCES			= src/algor.c src/imp.c
OBJECTS			= ${SOURCES:.c=.o}
OUT             = hashtable
OUT_LIB         = libhashtable.a

S_SOURCES       = sandbox.c
S_OBJECTS       = ${S_SOURCES:.c=.o}
S_OUT           = sandbox 
S_LINKS         = -lhashtable
S_LINK_FLAGS    = -L .

.c.o:
	$(CC) -c ${CONFIG_CC_FLAGS} $< -o $@

all: ${OUT} ${S_OUT}

${OUT}: ${OBJECTS}
	$(AR) rcs ${OUT_LIB} ${OBJECTS}

${S_OUT}: ${S_OBJECTS}
	$(CC) ${S_LINK_FLAGS} -o $@ ${S_OBJECTS} ${S_LINKS}

clean:
	rm ${OUT_LIB} ${S_OUT}
	rm src/*.o
	rm *.o
	
