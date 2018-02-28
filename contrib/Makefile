LINEA:=$(shell grep -n Emitting ../src/gcov_hooks.c  | cut -d: -f1 )
LINEAA:=$(shell echo $$[${LINEA} + 2])
LINEBB:=$(shell echo $$[${LINEAA} + 3])

LINECC:=$(shell grep -n '1 . getMicros' ../src/task1.c | cut -d: -f1 )
LINED:=$(shell grep -n '2 . getMicros' ../src/task1.c | cut -d: -f1 )
LINEDD:=$(shell echo $$[${LINED} + 3])
LINEEE:=$(shell echo $$[${LINED} + 6])

all:
	cat coverage.gdb.template \
	   | sed "s/LINEA/${LINEAA}/;s/LINEB/${LINEBB}/" \
	   | sed "s/SUFFIX/${SUFFIX}/" \
	   > coverage.gdb
	for i in measure_*template ; do \
	    cat "$$i" \
	    | sed "s/LINEC/${LINECC}/;s/LINED/${LINEDD}/;s/LINEE/${LINEEE}/" \
	    > "$${i/.template/}" ; \
	done
