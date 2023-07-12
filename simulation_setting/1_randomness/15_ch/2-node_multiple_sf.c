/**
 * \file
 *         An example that demonstrates a simple custom scheduler for TSCH.
 *
 * \author Atis Elsts <atis.elsts@edi.lv>
 */

#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/mac/tsch/tsch.h"
#include "lib/random.h"
#include "sys/node-id.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_PORT 8765
// #define SEND_INTERVAL (1 * CLOCK_SECOND / 100)
#define SEND_INTERVAL_32 (32 * CLOCK_SECOND / 100)
#define SEND_INTERVAL_64 (64 * CLOCK_SECOND / 100)
#define SEND_INTERVAL_128 (128 * CLOCK_SECOND / 100)

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
// #define APP_SLOTFRAME_HANDLE 1
/* Put all unicast cells on the same timeslot (for demonstration purposes only) */
#define APP_UNICAST_TIMESLOT 1

static void
initialize_tsch_schedule(void)
{
  int i, j;
  struct tsch_slotframe *sf_common = tsch_schedule_add_slotframe(1, 64);
  struct tsch_slotframe *sf_32 = tsch_schedule_add_slotframe(2, 32);
  struct tsch_slotframe *sf_128 = tsch_schedule_add_slotframe(3, 128);
  uint16_t slot_offset;
  uint16_t channel_offset;

  /* A "catch-all" cell at (0, 0) */
  slot_offset = 0;
  channel_offset = 0;
  tsch_schedule_add_link(sf_common,
                         LINK_OPTION_RX | LINK_OPTION_TX | LINK_OPTION_SHARED,
                         LINK_TYPE_ADVERTISING, &tsch_broadcast_address,
                         slot_offset, channel_offset, 1);

  // TSCH_SCHEDULE_MAX_LINKS = 1 link in a single hop network => 1 coord, 1 sensor
  for (i = 0; i < 2 - 1; ++i)
  {
    // uint8_t link_options;
    linkaddr_t addr;
    // uint16_t remote_id = i + 1;
    uint16_t remote_id = 1;

    for (j = 0; j < sizeof(addr); j += 2)
    {
      addr.u8[j + 1] = remote_id & 0xff;
      addr.u8[j + 0] = remote_id >> 8;
    }

    // channel_offset = i;
    channel_offset = 0;

    tsch_schedule_add_link(sf_common, LINK_OPTION_TX, LINK_TYPE_NORMAL, &addr, 1, channel_offset, 0);
    tsch_schedule_add_link(sf_32, LINK_OPTION_TX, LINK_TYPE_NORMAL, &addr, 4, channel_offset, 0);
    tsch_schedule_add_link(sf_128, LINK_OPTION_TX, LINK_TYPE_NORMAL, &addr, 11, channel_offset, 0);

    tsch_schedule_print();
    LOG_INFO("SCHEDULE INSTALLED!!\n");
  }
}

static void
initialize_coord_schedule(void)
{
  int i, j;
  struct tsch_slotframe *sf_common = tsch_schedule_add_slotframe(1, 64);
  struct tsch_slotframe *sf_32 = tsch_schedule_add_slotframe(2, 32);
  struct tsch_slotframe *sf_128 = tsch_schedule_add_slotframe(3, 128);
  uint16_t slot_offset;
  uint16_t channel_offset;

  /* A "catch-all" cell at (0, 0) */
  slot_offset = 0;
  channel_offset = 0;
  tsch_schedule_add_link(sf_common,
                         LINK_OPTION_RX | LINK_OPTION_TX | LINK_OPTION_SHARED,
                         LINK_TYPE_ADVERTISING, &tsch_broadcast_address,
                         slot_offset, channel_offset, 1);

  // TSCH_SCHEDULE_MAX_LINKS = 1 => single hop network
  for (i = 0; i < 2 - 1; ++i)
  {
    //  uint8_t link_options;
    linkaddr_t addr;
    // uint16_t remote_id = i + 1;
    uint16_t remote_id = 2;

    for (j = 0; j < sizeof(addr); j += 2)
    {
      addr.u8[j + 1] = remote_id & 0xff;
      addr.u8[j + 0] = remote_id >> 8;
    }

    /* Add a unicast cell for each potential neighbor (in Cooja) */
    /* Use the same slot offset; the right link will be dynamically selected at runtime based on queue sizes */
    // slot_offset = APP_UNICAST_TIMESLOT;

    // channel_offset = i;
    channel_offset = 0;

    /* Warning: LINK_OPTION_SHARED cannot be configured, as with this schedule
     * backoff windows will not be reset correctly! */
    // link_options = remote_id == node_id ? LINK_OPTION_RX : LINK_OPTION_TX;

    tsch_schedule_add_link(sf_common, LINK_OPTION_RX, LINK_TYPE_NORMAL, &addr, 1, channel_offset, 0);
    tsch_schedule_add_link(sf_32, LINK_OPTION_RX, LINK_TYPE_NORMAL, &addr, 4, channel_offset, 0);
    tsch_schedule_add_link(sf_128, LINK_OPTION_RX, LINK_TYPE_NORMAL, &addr, 11, channel_offset, 0);

    tsch_schedule_print();
    LOG_INFO("SCHEDULE INSTALLED!!\n");
  }
}

static void
rx_packet(struct simple_udp_connection *c,
          const uip_ipaddr_t *sender_addr,
          uint16_t sender_port,
          const uip_ipaddr_t *receiver_addr,
          uint16_t receiver_port,
          const uint8_t *data,
          uint16_t datalen)
{
  uint32_t seqnum;

  if (datalen >= sizeof(seqnum))
  {
    memcpy(&seqnum, data, sizeof(seqnum));

    LOG_INFO("Received from ");
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_(", seqnum %" PRIu32 "\n", seqnum);
  }
}



PROCESS_THREAD(node_process, ev, data)
{
  static struct simple_udp_connection udp_conn;
  // static struct etimer periodic_timer;
  static struct etimer periodic_timer_32, periodic_timer_64, periodic_timer_128;
  static uint32_t seqnum;
  uip_ipaddr_t dst;

  PROCESS_BEGIN();

  /* Initialization; `rx_packet` is the function for packet reception */
  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, rx_packet);
  // etimer_set(&periodic_timer, SEND_INTERVAL);
  etimer_set(&periodic_timer_32, SEND_INTERVAL_32);
  etimer_set(&periodic_timer_64, SEND_INTERVAL_64);
  etimer_set(&periodic_timer_128, SEND_INTERVAL_128);
  // random_rand() %

  if (node_id == 1)
  { /* Running on the root? */
    initialize_coord_schedule();
    NETSTACK_ROUTING.root_start();
  }

  if (node_id != 1)
  { /* Running on the root? */
    initialize_tsch_schedule();
  }

  /* Main loop */
  while (1)
  {

    PROCESS_YIELD();
    if (etimer_expired(&periodic_timer_32))
    {
      if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dst))
      {
        /* Send network uptime timestamp to the network root node */
        if (node_id != 1)
        {
          seqnum++;
          LOG_INFO("Send to ");
          LOG_INFO_6ADDR(&dst);
          LOG_INFO_(", seqnum %" PRIu32 "\n", seqnum);
          simple_udp_sendto(&udp_conn, &seqnum, sizeof(seqnum), &dst);
        }
      }
      etimer_set(&periodic_timer_32, SEND_INTERVAL_32);
    }
    else if (etimer_expired(&periodic_timer_64))
    {
      if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dst))
      {
        /* Send network uptime timestamp to the network root node */
        if (node_id != 1)
        {
          seqnum++;
          LOG_INFO("Send to ");
          LOG_INFO_6ADDR(&dst);
          LOG_INFO_(", seqnum %" PRIu32 "\n", seqnum);
          simple_udp_sendto(&udp_conn, &seqnum, sizeof(seqnum), &dst);
        }
      }
      etimer_set(&periodic_timer_64, SEND_INTERVAL_64);
    }
    else if (etimer_expired(&periodic_timer_128))
    {
      if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dst))
      {
        /* Send network uptime timestamp to the network root node */
        if (node_id != 1)
        {
          seqnum++;
          LOG_INFO("Send to ");
          LOG_INFO_6ADDR(&dst);
          LOG_INFO_(", seqnum %" PRIu32 "\n", seqnum);
          simple_udp_sendto(&udp_conn, &seqnum, sizeof(seqnum), &dst);
        }
      }
      etimer_set(&periodic_timer_128, SEND_INTERVAL_128);
    }
  }
  PROCESS_END();
}
