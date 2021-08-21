ifneq (,$(wildcard config.mk))
	include config.mk
endif

SOURCES			= src/algor.c src/imp.c
OBJECTS			= ${SOURCES:.c=.o}
OUT             = hashtable
OUT_LIB         = libhashtable.a

.c.o:
	$(CC) -c ${CONFIG_CC_FLAGS} $< -o $@

all: ${OUT}

${OUT}: ${OBJECTS}
	$(AR) rcs ${OUT_LIB} ${OBJECTS}

target_lib:
	@echo ${OUT_LIB}

clean:
	rm ${OUT_LIB}
	rm src/*.o
	
