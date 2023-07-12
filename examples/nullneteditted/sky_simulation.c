
/**
 * \file
 *         NullNet unicast sky motes
 * \author
 *         Elisabeth Permatasari <erma@iastate.edu>
 *
 */

#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

// add include tsch.h
#include "net/mac/tsch/tsch.h"
#include "sys/clock.h"
#include "dev/leds.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (256 * CLOCK_SECOND / 100)
// #define SEND_INTERVAL_512 (512 * CLOCK_SECOND / 100)
static linkaddr_t dest_addr = {{0x0d, 0xae, 0x5e, 0x11, 0x00, 0x74, 0x12, 0x00}};

static linkaddr_t coordinator_addr = {{0x0d, 0xae, 0x5e, 0x11, 0x00, 0x74, 0x12, 0x00}};

/* Put all cells on the same slotframe */
#define APP_SLOTFRAME_HANDLE 1
#define APP_UNICAST_TIMESLOT 1

/* ID<->MAC address mapping */
struct id_mac
{
  uint16_t id;
  linkaddr_t mac;
};

/* List of ID<->MAC mapping used for different deployments */
static const struct id_mac id_mac_list[] = {
    {1, {{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}, // 1
    {2, {{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}, // 2
    {3, {{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {4, {{0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {5, {{0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {6, {{0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {7, {{0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {8, {{0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {9, {{0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {10, {{0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {11, {{0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {12, {{0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {13, {{0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {14, {{0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {15, {{0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {0, {{0}}}};

static const struct id_mac id_mac_list_2hop[] = {
    {9, {{0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {10, {{0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {11, {{0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {12, {{0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {13, {{0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {14, {{0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {15, {{0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {0, {{0}}}};

static void
install_node_schedule(uint16_t id_node)
{
  struct tsch_slotframe *sf_common = tsch_schedule_add_slotframe(APP_SLOTFRAME_HANDLE, APP_SLOTFRAME_SIZE);

  uint16_t slot_offset;
  uint16_t channel_offset;

  /* A "catch-all" cell at (0, 0) */
  slot_offset = 0;
  channel_offset = 0;
  tsch_schedule_add_link(sf_common,
                         LINK_OPTION_RX | LINK_OPTION_TX | LINK_OPTION_SHARED,
                         LINK_TYPE_ADVERTISING, &tsch_broadcast_address,
                         slot_offset, channel_offset, 1);

  struct tsch_slotframe *sf_tx = tsch_schedule_add_slotframe(2, 512);
  // first transmission
  slot_offset = id_node * id_node;
  LOG_INFO("first slot tx %u \n", slot_offset);
  channel_offset = 0;
  tsch_schedule_add_link(sf_tx, LINK_OPTION_TX, LINK_TYPE_NORMAL, &dest_addr, slot_offset, channel_offset, 0);

  // second tx
  slot_offset = slot_offset + 256;
  channel_offset = 0;
  LOG_INFO("second slot tx %u \n", slot_offset);
  tsch_schedule_add_link(sf_tx, LINK_OPTION_TX, LINK_TYPE_NORMAL, &dest_addr, slot_offset, channel_offset, 0);
    
  // const struct id_mac *curr = id_mac_list_2hop;
  // uint16_t hop_id = id_node + 7;

  // while (curr->id != 0)
  // {
  //   if (curr->id == hop_id)
  //   {
  //     struct tsch_slotframe *sf_hop = tsch_schedule_add_slotframe(3, 512);
  //     linkaddr_t hop_addr = curr->mac;
  //     slot_offset = (hop_id * hop_id);
  //     LOG_INFO("first slot rx %u \n", slot_offset);
  //     channel_offset = 1;
  //     tsch_schedule_add_link(sf_hop, LINK_OPTION_RX, LINK_TYPE_NORMAL, &hop_addr, slot_offset, channel_offset, 0);
  //   }

  //   curr++;
  // }

  tsch_schedule_print();
  LOG_INFO("NODE SCHEDULE INSTALLED!!\n");
}

static void
initialize_coord_schedule(void)
{

  struct tsch_slotframe *sf_common = tsch_schedule_add_slotframe(APP_SLOTFRAME_HANDLE, APP_SLOTFRAME_SIZE);
  struct tsch_slotframe *sf_tx = tsch_schedule_add_slotframe(2, 512);

  uint16_t slot_offset;
  uint16_t channel_offset;

  /* A "catch-all" cell at (0, 0) */
  slot_offset = 0;
  channel_offset = 0;
  tsch_schedule_add_link(sf_common,
                         LINK_OPTION_RX | LINK_OPTION_TX | LINK_OPTION_SHARED,
                         LINK_TYPE_ADVERTISING, &tsch_broadcast_address,
                         slot_offset, channel_offset, 1);

  const struct id_mac *curr = id_mac_list;
  while (curr->id < 9)
  {

    uint16_t id = curr->id;
    linkaddr_t node_addr = curr->mac;

    slot_offset = id * id;
    LOG_INFO("first slot rx %u \n", slot_offset);
    tsch_schedule_add_link(sf_tx, LINK_OPTION_RX, LINK_TYPE_NORMAL, &node_addr, slot_offset, channel_offset, 0);
    slot_offset = slot_offset + 256;
    LOG_INFO("second slot rx %u \n", slot_offset);
    tsch_schedule_add_link(sf_tx, LINK_OPTION_RX, LINK_TYPE_NORMAL, &node_addr, slot_offset, channel_offset, 0);

    curr++;

  }
  tsch_schedule_print();
  LOG_INFO("COORDINATOR SCHEDULE INSTALLED!!\n");
}

/* Returns the node's node-id */
uint16_t
get_node_id(linkaddr_t *addr)
{
  const struct id_mac *curr = id_mac_list;
  while (curr->id != 0)
  {
    /* Assume network-wide unique 16-bit MAC addresses */
    if (curr->mac.u8[0] == addr->u8[0] && curr->mac.u8[1] == addr->u8[1])
    {
      return curr->id;
    }
    curr++;
  }
  return 0;
}

/* Returns the node's linkaddr */
linkaddr_t
get_node_addr(uint16_t sender_id)
{
  const struct id_mac *curr = id_mac_list;
  uint16_t hop_id = sender_id - 7;

  while (curr->id != 0)
  {
    if (curr->id == hop_id)
    {
      linkaddr_t hop_addr = curr->mac;
      return hop_addr;
    }

    curr++;
  }

  linkaddr_t def_addr = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
  return def_addr;
}

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet unicast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
                    const linkaddr_t *src, const linkaddr_t *dest)
{
  if (len == sizeof(unsigned))
  {
    unsigned count;
    leds_on(LEDS_GREEN);
    uint8_t ch = tsch_current_channel;
    clock_time_t t = clock_time();
    memcpy(&count, data, sizeof(count));
    LOG_INFO("Received %u at ch %u time %lu from ", count, ch, t);
    LOG_INFO_LLADDR(src);
    // LOG_INFO("", ch);
    LOG_INFO_("\n");
    leds_off(LEDS_GREEN);
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned count = 0;
  static uint16_t node_id;
  int is_coordinator;
  // static linkaddr_t hop_addr;

  PROCESS_BEGIN();
  is_coordinator = linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr);

  tsch_set_coordinator(is_coordinator);

  if (is_coordinator)
  {
    initialize_coord_schedule();
  }
  else
  {
    node_id = get_node_id(&linkaddr_node_addr);
    count = node_id;

    install_node_schedule(node_id);
  }

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&count;
  nullnet_len = sizeof(count);
  nullnet_set_input_callback(input_callback);

  if (!linkaddr_cmp(&dest_addr, &linkaddr_node_addr))
  {
    etimer_set(&periodic_timer, SEND_INTERVAL);

    while (1)
    {
      // leds_on(LEDS_BLUE);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
      // leds_off(LEDS_BLUE);

      leds_on(LEDS_RED);
      leds_on(LEDS_GREEN);

      if (node_id > 8)
      {
        linkaddr_t hop_addr = get_node_addr(node_id);
        LOG_INFO("Node id %u ", node_id);
        LOG_INFO_("\n");
        LOG_INFO("Sending %u to ", count);
        LOG_INFO_LLADDR(&hop_addr);

        LOG_INFO_("\n");

        NETSTACK_NETWORK.output(&hop_addr);
        leds_off(LEDS_RED);
        leds_off(LEDS_GREEN);
        etimer_reset(&periodic_timer);
      }
      else
      {
        LOG_INFO("Sending %u to ", count);
        LOG_INFO_LLADDR(&dest_addr);

        LOG_INFO_("\n");

        NETSTACK_NETWORK.output(&dest_addr);
        leds_off(LEDS_RED);
        leds_off(LEDS_GREEN);
        etimer_reset(&periodic_timer);
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
