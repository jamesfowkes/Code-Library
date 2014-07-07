SGP_MODEL = SGP4

SGP_FILES = \
	$(SGP_DIR)/sgp_conv.c \
	$(SGP_DIR)/sgp_deep.c \
	$(SGP_DIR)/sgp_math.c \
	$(SGP_DIR)/sgp_time.c
	
ifeq ($(SGP_MODEL), SGP0)
	SGP_MODEL_C = $(SGP_DIR)/sgp0.c
endif

ifeq ($(SGP_MODEL), SGP4)
	SGP_MODEL_C = $(SGP_DIR)/sgp4sdp4.c
endif

ifeq ($(SGP_MODEL), SDP4)
	SGP_MODEL_C = $(SGP_DIR)/sgp4sdp4.c
endif

ifeq ($(SGP_MODEL), SGP8)
	SGP_MODEL_C = $(SGP_DIR)/sgp8sdp8.c
endif

ifeq ($(SGP_MODEL), SDP8)
	SGP_MODEL_C = $(SGP_DIR)/sgp8sdp8.c
endif


SGP_FILES += $(SGP_MODEL_C)
SGP_MODEL_OBJ = $(SGP_MODEL:.c=.o)
SGP_OBJS = $(SGP_FILES:.c=.o)

.phony: SGP

SGP:
	$(CC) $(OPTS) -O$(OPT_LEVEL) -c $(SGP_MODEL_C) -o $(SGP_MODEL_OBJ)
	$(CC) $(OPTS) -O$(OPT_LEVEL) -c $(SGP_DIR)/sgp_conv.c -o sgp_conv.o
	$(CC) $(OPTS) -O$(OPT_LEVEL) -c $(SGP_DIR)/sgp_deep.c -o sgp_deep.o
	$(CC) $(OPTS) -O$(OPT_LEVEL) -c $(SGP_DIR)/sgp_math.c -o sgp_math.o
	$(CC) $(OPTS) -O$(OPT_LEVEL) -c $(SGP_DIR)/sgp_time.c -o sgp_time.o
