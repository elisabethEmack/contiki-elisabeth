CONTIKI_PROJECT = nullnet-unicast nullnet-2hop nullnet-2hop-simple multihop_200
all: $(CONTIKI_PROJECT)

CONTIKI = ../..

PLATFORMS_EXCLUDE = nrf52dk

#use this to enable TSCH: MAKE_MAC = MAKE_MAC_TSCH
#MAKE_MAC ?= MAKE_MAC_CSMA
MAKE_MAC = MAKE_MAC_TSCH
MAKE_NET = MAKE_NET_NULLNET
include $(CONTIKI)/Makefile.include
