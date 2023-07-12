/**
 * \file
 *         An example that demonstrates a simple custom scheduler for TSCH.
 *
 * \author Atis Elsts <atis.elsts@edi.lv>
 */

#include "contiki.h"
// #include "net/ipv6/simple-udp.h"
#include "net/mac/tsch/tsch.h"
#include "lib/random.h"
#include "sys/node-id.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

// #define UDP_PORT 8765
//#define SEND_INTERVAL		  (CLOCK_SECOND)
// #define RADIO_RX_MODE_ADDRESS_FILTER 0

PROCESS(node_process, "TSCH Schedule Node");
AUTOSTART_PROCESSES(&node_process);

/*
 * Note! This is not an example how to design a *good* schedule for TSCH,
 * nor this is the right place for complete beginners in TSCH.
 * We recommend using the default Orchestra schedule for a start.
 *
 * The intention of this file is instead to serve a starting point for those interested in building
 * their own schedules for TSCH that are different from Orchestra and 6TiSCH minimal.
 */

/* Put all cells on the same slotframe */
#define APP_SLOTFRAME_HANDLE 1
/* Put all unicast cells on the same timeslot (for demonstration purposes only) */
//#define APP_UNICAST_TIMESLOT 1

static void
initialize_jammer_schedule(void)
{
  int i, j;
  struct tsch_slotframe *sf_common = tsch_schedule_add_slotframe(APP_SLOTFRAME_HANDLE, APP_SLOTFRAME_SIZE);
  uint16_t channel_offset;

  channel_offset = 0;

  // TSCH_SCHEDULE_MAX_LINKS = 1 => single hop network
  for (i = 0; i < 5; ++i)
  {
    // uint8_t link_options;
    linkaddr_t addr;
    uint16_t remote_id = i;
    // uint16_t remote_id = 2;

    for (j = 0; j < sizeof(addr); j += 2)
    {
      addr.u8[j + 1] = remote_id & 0xff;
      addr.u8[j + 0] = remote_id >> 8;
    }

    int x;
    for (x = 0; x < APP_SLOTFRAME_SIZE; x++)
    {
      tsch_schedule_add_link(sf_common, LINK_OPTION_RX, LINK_TYPE_NORMAL, &addr, x, channel_offset, 0);
      // tsch_schedule_add_link(sf_common, LINK_OPTION_RX, LINK_TYPE_NORMAL, &addr, 2, channel_offset, 0);
    }

    tsch_schedule_print();
    LOG_INFO("SCHEDULE INSTALLED!!\n");
  }
}
PROCESS_THREAD(node_process, ev, data)
{
  // static struct simple_udp_connection udp_conn;
  radio_value_t radio_rx_mode;
  NETSTACK_RADIO.get_value(RADIO_PARAM_RX_MODE, &radio_rx_mode);
  radio_rx_mode &= ~RADIO_RX_MODE_ADDRESS_FILTER;
  if (NETSTACK_RADIO.set_value(RADIO_PARAM_RX_MODE, radio_rx_mode) != RADIO_RESULT_OK)
  {
    LOG_WARN("radio does not support setting RADIO_PARAM_RX_MODE\n");
  }
  else
  {
    LOG_INFO("turned of Address filtering.\n");
  }

  PROCESS_BEGIN();

  initialize_jammer_schedule();
  

  /* Initialization; `rx_packet` is the function for packet reception */
  // simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, rx_packet);

  PROCESS_END();
}
