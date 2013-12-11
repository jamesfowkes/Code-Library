.PHONY: compiletime.h

compiletime.h:
	@echo "/* This file is autogenerated by compiletime.mk */" > $@
	@echo "#ifndef _COMPILE_TIME_H_" >> $@
	@echo "#define _COMPILE_TIME_H_" >> $@
	
	@echo "// Compile time as UINX epoch time" >> $@
	@echo -n "#define COMPILE_TIME_INT (uint32_t)" >> $@
	@date "+%s" >> $@
	
	@echo "//Compile time as array of digits" >> $@
	@echo -n "#define COMPILE_TIME_DIGITS " >> $@ 
	@echo -n "{" >> $@
	@date "+%s" | sed 's/./&, /g' | tr -d "\n" >> $@
	@echo "}" >> $@
	
	@echo "#endif" >> $@
	mv $@ ../
	