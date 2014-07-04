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

EXTRA_INCLUDES = \
	-I$(UNITY_FOLDER)/src \
	-I$(UNITY_FOLDER)/extras/fixture/src \
	-I$(UTIL_DIR)
	
EXTRA_CCFLAGS = -DUNIX_TIME_TYPE=int32_t
