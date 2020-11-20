CONTIKI_PROJECT = minimal_6tisch_sync
CONTIKI = ../..
CONTIKI_SOURCEFILES += helper_functions.c

ifeq ($(MAKECMDGOALS),minimal_6tisch_sync.upload)
MAKE_MAC = MAKE_MAC_TSCH
# Define the role of each node
PAN_COORDINATOR_ID = 1
ADVERTISERS_IDS = 2 3 4  # except the PAN coordinator
SAMPLERS_IDS = 5 6 7 8 9 10 11 12
#############################

# Define macros for node types #
PAN_COORDINATOR = 0
ADVERTISER = 1
SAMPLER = 2
CFLAGS += -DPAN_COORDINATOR=$(PAN_COORDINATOR)
CFLAGS += -DADVERTISER=$(ADVERTISER)
CFLAGS += -DSAMPLER=$(SAMPLER)
##############################

ifndef NODEID
  $(error NODEID has not been defined)
endif

ifeq ($(NODEID), $(PAN_COORDINATOR_ID))
  CFLAGS += -DNODE_TYPE=$(PAN_COORDINATOR)
else ifneq ($(filter $(NODEID),$(ADVERTISERS_IDS)),)
  CFLAGS += -DNODE_TYPE=$(ADVERTISER)
else ifneq ($(filter $(NODEID),$(SAMPLERS_IDS)),)
  CFLAGS += -DNODE_TYPE=$(SAMPLER)
else
  $(error There is no role specified for the node with the given id)
endif

endif # MAKECMDGOALS == minimal_6tisch_sync.upload

all: $(CONTIKI_PROJECT)

include $(CONTIKI)/Makefile.include