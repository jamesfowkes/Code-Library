.PHONY: compiletime.h

compiletime.h:
	@echo "#ifndef _COMPILE_TIME_H_" > $@
	@echo "#define _COMPILE_TIME_H_" >> $@
	
	@echo -n "#define COMPILE_TIME " >> $@
	@date "+%s" >> $@
	@echo "#endif" >> $@