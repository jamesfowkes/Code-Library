# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 
# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ========================================== 

EXTRA_SRC_FILES = \
	$(LIBS_DIR)/Generics/memorypool.c

EXTRA_CCFLAGS = -DMEMORY_POOL_BYTES=5096

EXTRA_INCLUDES = \
	-I$(LIBS_DIR)/Generics