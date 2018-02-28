all:	inform bin.$(SUFFIX)/${TARGET}

coverage-common:
	$(MAKE) PLATFORM=${PLATFORM} COVERAGE=1
ifneq (${PLATFORM},LINUX)
	# Create the GDB coverage script
	$(MAKE) PLATFORM=${PLATFORM} SUFFIX=${SUFFIX} -C contrib
	# Spawn QEMU with our coverage enabled binary
	${QEMU} -M at697 -nographic -gdb tcp::9976 -S \
		-kernel  bin.$(SUFFIX)/${TARGET} &
	# Spawn GDB and run the coverage script
	sleep 2 ; ${GDB} -x contrib/coverage.gdb </dev/null 2>/dev/null
else
	./bin.$(SUFFIX)/${TARGET}
endif

coverage:	coverage-common
	# Reporting the results
	cd objs.$(SUFFIX)/ ; \
	    ln -s ../src ; \
	    ${GCOV} *gcda
	@mkdir -p coverage ; \
	    rm -f coverage/* ; \
	    mv objs.$(SUFFIX)/*gcov coverage || exit 1
	@rm -f coverage/*inl.gcov # Ignore EDISOFT specific files
	@echo
	@echo =========================================================
	@echo The source code with coverage information is in coverage/
	@ls -la coverage/
	@echo =========================================================

coverage-html:	coverage-common
	lcov --gcov-tool ${GCOV} --capture --directory .   \
	    --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	@echo
	@echo "===================================================="
	@echo "Open coverage/index.html in your browser - e.g. via:"
	@echo
	@echo "    xdg-open coverage/index.html"
	@echo "===================================================="

printUsage:
	@echo " "
	@echo "Invalid or missing configuration specified."
	@echo " "
	@echo "You must specify the PLATFORM when running make, e.g."
	@echo " "
	@echo "    make PLATFORM=OAR"
	@echo " "
	@echo "Possible choices for PLATFORM are 'OAR', 'EDISOFT', 'GAISLER' and 'LINUX'"
	@echo " "
	@echo "Additionally, you can specify:"
	@echo " "
	@echo "    make PLATFORM=OAR CPU=leon2 CFG=debug COVERAGE=1 V=1"
	@echo " "
	@echo "- Possible choices for CPU are 'leon3' and 'leon2' (default)"
	@echo "- Possible choices for CFG are 'release' and 'debug' (default)"
	@echo "- Possible choices for COVERAGE are '1' and '0' (default)"
	@echo "- Possible choices for V are '1' (show commands) and '0' (silent) (default)"
	@echo " "
	@exit 1

inform:
ifneq ($(PLATFORM),OAR)
ifneq ($(PLATFORM),EDISOFT)
ifneq ($(PLATFORM),GAISLER)
ifneq ($(PLATFORM),LINUX)
	$(MAKE) printUsage
endif
endif
endif
endif


bin.$(SUFFIX)/${TARGET}: ${OBJ} | inform
	@mkdir -p $(dir $@)
ifeq ($(V),1)
	$(CC) -g -o $@ $^ ${LDFLAGS}
else
	@echo [LD] $@
	@$(CC) -g -o $@ $^ ${LDFLAGS}
endif
ifeq ($(CFG),release)
	@echo [SEPARATE DEBUG INFO] Making $@.debug
	@${OBJCOPY} --only-keep-debug $@ ${@}.debug
	@echo [STRIP] $@
	@${STRIP} $@
endif
	@echo Built $(CFG) version at $@
ifneq (${PLATFORM},LINUX)
	@echo Build done via RTEMS at ${RTEMS_LIB} for ${CPU}.
endif



objs.$(SUFFIX)/%.o: %.c
	@mkdir -p $(dir $@)
ifeq ($(V),1)
	$(CC) -c $(CFLAGS) -o $@ $<
else
	@echo [CC] $@
	@$(CC) -c $(CFLAGS) -o $@ $<
endif



objs.$(SUFFIX)/%.o: %.cpp
	@mkdir -p $(dir $@)
ifeq ($(V),1)
	$(CC) -c $(CFLAGS) -o $@ $<
else
	@echo [CXX] $@
	@$(CC) -c $(CFLAGS) -o $@ $<
endif



deps.$(SUFFIX)/%.d: %.c
	@mkdir -p $(dir $@)
	@echo Generating dependencies for $<
	@set -e ; $(CDEP) -MM -MP $(INCLUDEFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,objs.$(SUFFIX)\/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

measure_EDISOFT_leon3_N2X:
	$(MAKE) PLATFORM=${PLATFORM} SUFFIX=${SUFFIX} -C contrib
	$(MAKE) clean
	@ps aux | grep 's[s]h.*1324' || \
	    { echo -e '\n\e[1m\e[31mNo active SSH to leon3 machine...\e[0m\n' ; exit 1 ; }
	$(MAKE) PLATFORM=EDISOFT CFG=debug CPU=leon3
	bash -c '[ -h ~/.gdbinit ] && rm -f ~/.gdbinit' ; exit 0;
	-sparc-rtems4.11-gdb -x contrib/measure_l4stat_EDISOFT.gdb \
	    	./bin.debug.leon3.EDISOFT/fputest

measure_OAR_leon3_N2X:
	$(MAKE) PLATFORM=${PLATFORM} SUFFIX=${SUFFIX} -C contrib
	$(MAKE) clean
	@ps aux | grep 's[s]h.*1324' || \
	    { echo -e '\n\e[1m\e[31mNo active SSH to leon3 machine...\e[0m\n' ; exit 1 ; }
	$(MAKE) PLATFORM=OAR CFG=debug CPU=leon3
	bash -c '[ -h ~/.gdbinit ] && rm -f ~/.gdbinit' ; exit 0;
	-sparc-rtems4.11-gdb -x contrib/measure_l4stat.gdb \
	    	./bin.debug.leon3.OAR/fputest

measure_OAR_leon2_TSIM:
	$(MAKE) PLATFORM=${PLATFORM} SUFFIX=${SUFFIX} -C contrib
	$(MAKE) clean
	@ps aux | grep '/t[s]im' || bash -c '/opt/tsim-leon/tsim/linux/tsim-leon -gdb & disown'
	$(MAKE) PLATFORM=OAR CFG=debug CPU=leon2
	bash -c '[ -h ~/.gdbinit ] && rm -f ~/.gdbinit' ; exit 0;
	-sparc-rtems4.11-gdb -x contrib/measure_tsim.gdb \
	    	./bin.debug.leon2.OAR/fputest ; exit 0

clean:
	@rm -rf deps.* objs.* bin.* coverage coverage.info

cleanall:
	@rm -rf deps.* objs.* bin.* coverage coverage.info

.PHONY:	clean coverage coverage-html coverage-common

# Unless "make clean" is called, include the dependency files
# which are auto-generated. Don't fail if they are missing
# (-include), since they will be missing in the first invocation!
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(CFG),)
ifneq ($(PLATFORM),)
-include ${DEP}
endif
endif
endif
