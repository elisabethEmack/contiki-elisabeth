CONTIKI_PROJECT = simple_sky my_nullnet sky_singlehop sky_secondhop sky_simulation
#  nullnet-unicast nullnet-broadcast 
all: $(CONTIKI_PROJECT)

CONTIKI = ../..

#PLATFORMS_EXCLUDE = nrf52dk

MAKE_MAC = MAKE_MAC_TSCH

include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_SERVICES_DIR)/shell

MAKE_NET = MAKE_NET_NULLNET

#CFLAGS += -ffunction-sections
#LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__

include $(CONTIKI)/Makefile.include
