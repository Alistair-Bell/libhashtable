-include config.mk

SOURCES         = src/algor.c src/imp.c
OBJECTS         = ${SOURCES:.c=.o}
OUT             = hashtable
OUT_LIB         = libhashtable.a

E_SOURCES       = example.c
E_OBJECTS       = ${E_SOURCES:.c=.o}
E_OUT           = example
E_LINKS         = -lhashtable
E_LINK_FLAGS    = -L .

.c.o:
	@echo "cc $@"
	@$(CC) -c $(CFLAGS) $< -o $@

all: ${OUT}

${OUT}: ${OBJECTS}
	@echo "ar ${OUT_LIB}"
	@$(AR) rcs ${OUT_LIB} ${OBJECTS}

${E_OUT}: ${OUT} ${E_OBJECTS}
	@echo "cc ${E_OUT}"
	@$(CC) ${E_LINK_FLAGS} -o $@ ${E_OBJECTS} ${E_LINKS}

clean:
	rm ${OUT_LIB}
	rm src/*.o
    
