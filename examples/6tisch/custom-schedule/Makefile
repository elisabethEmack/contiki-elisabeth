#2-node 2-node_multiple_sf 3-node 2-hop-net
CONTIKI_PROJECT = 100-node 
all: $(CONTIKI_PROJECT)

#PLATFORMS_ONLY = cooja

CONTIKI=../../..

# force Security from command line
MAKE_WITH_SECURITY = 0

MAKE_MAC = MAKE_MAC_TSCH

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_SERVICES_DIR)/shell

ifeq ($(MAKE_WITH_SECURITY),1)
CFLAGS += -DWITH_SECURITY=1
endif

include $(CONTIKI)/Makefile.include
