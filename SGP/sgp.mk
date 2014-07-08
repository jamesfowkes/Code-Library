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
SGP_MODEL_OBJ = $(SGP_MODEL_C:.c=.o)
SGP_OBJS = $(SGP_FILES:.c=.o)

SGP:
	$(CC) $(OPTS) $(LDFLAGS) -DSGP_MODEL_$(SGP_MODEL) -c $(SGP_MODEL_C) -o $(SGP_MODEL_OBJ) $(LD_SUFFIX)
	$(CC) $(OPTS) $(LDFLAGS) -DSGP_MODEL_$(SGP_MODEL) -c $(SGP_DIR)/sgp_conv.c -o $(SGP_DIR)/sgp_conv.o $(LD_SUFFIX)
	$(CC) $(OPTS) $(LDFLAGS) -DSGP_MODEL_$(SGP_MODEL) -c $(SGP_DIR)/sgp_deep.c -o $(SGP_DIR)/sgp_deep.o $(LD_SUFFIX)
	$(CC) $(OPTS) $(LDFLAGS) -DSGP_MODEL_$(SGP_MODEL) -c $(SGP_DIR)/sgp_math.c -o $(SGP_DIR)/sgp_math.o $(LD_SUFFIX)
	$(CC) $(OPTS) $(LDFLAGS) -DSGP_MODEL_$(SGP_MODEL) -c $(SGP_DIR)/sgp_time.c -o $(SGP_DIR)/sgp_time.o $(LD_SUFFIX)
