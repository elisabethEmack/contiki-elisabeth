/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         NullNet unicast example
 * \author
 *         Simon Duquennoy <simon.duquennoy@ri.se>
 *
 */

#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "net/packetbuf.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (512 * CLOCK_SECOND / 10)
static linkaddr_t dest_addr = {{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
// static linkaddr_t hop_addr = {{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
// static linkaddr_t sender_addr = {{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr = {{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
#endif /* MAC_CONF_WITH_TSCH */

/* Put all cells on the same slotframe */
#define APP_SLOTFRAME_HANDLE 1
/* Put all unicast cells on the same timeslot (for demonstration purposes only) */
#define APP_UNICAST_TIMESLOT 1

/* ID<->MAC address mapping */
struct id_mac
{
  uint16_t id;
  linkaddr_t mac;
};

/* List of ID<->MAC mapping used for different deployments */
static const struct id_mac id_mac_list[] = {
    {1, {{0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}, // 105
    {2, {{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}, // 95
    {3, {{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {4, {{0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {5, {{0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {6, {{0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {7, {{0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {8, {{0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {9, {{0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {10, {{0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {11, {{0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {12, {{0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {13, {{0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {14, {{0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {15, {{0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {16, {{0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {17, {{0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {18, {{0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {19, {{0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {20, {{0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {21, {{0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {22, {{0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {23, {{0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    // {24, {{0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
    {0, {{0}}}};

static const struct id_mac id_mac_list_2hop[] = {
    {3, {{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},
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

  if (id_node > 2)
  {
    // if id_node = 101, temp = 1; curr[1]->id = hop_id = 2
    const struct id_mac *curr = id_mac_list;
    uint16_t hop_id = id_node - 1;

    while (curr->id != 0)
    {
      if (curr->id == hop_id)
      {
        struct tsch_slotframe *sf_tx = tsch_schedule_add_slotframe(2, 512);
        linkaddr_t hop_addr = curr->mac;
        slot_offset = hop_id + 200;
        channel_offset = 1;
        tsch_schedule_add_link(sf_tx, LINK_OPTION_RX | LINK_OPTION_TX, LINK_TYPE_NORMAL, &hop_addr, slot_offset, channel_offset, 0);

        LOG_INFO("Hop_addr: ");
        LOG_INFO_LLADDR(&hop_addr);
        LOG_INFO_("\n");
        break;
      }

      curr++;
    }
  }
  else
  {
    struct tsch_slotframe *sf_tx = tsch_schedule_add_slotframe(2, 512);
    // first transmission
    slot_offset = id_node;
    channel_offset = 0;
    tsch_schedule_add_link(sf_tx, LINK_OPTION_RX | LINK_OPTION_TX, LINK_TYPE_NORMAL, &dest_addr, slot_offset, channel_offset, 0);

    const struct id_mac *curr = id_mac_list_2hop;
    uint16_t hop_id = id_node + 1;

    while (curr->id != 0)
    {
      if (curr->id == hop_id)
      {
        struct tsch_slotframe *sf_hop = tsch_schedule_add_slotframe(3, 512);
        linkaddr_t hop_addr = curr->mac;
        slot_offset = id_node + 200;
        channel_offset = 1;
        tsch_schedule_add_link(sf_hop, LINK_OPTION_RX | LINK_OPTION_TX, LINK_TYPE_NORMAL, &hop_addr, slot_offset, channel_offset, 0);

        // second tx
        slot_offset = hop_id;
        channel_offset = 0;
        tsch_schedule_add_link(sf_hop, LINK_OPTION_RX | LINK_OPTION_TX, LINK_TYPE_NORMAL, &dest_addr, slot_offset, channel_offset, 0);
        break;
      }

      curr++;
    }
  }

  tsch_schedule_print();
  LOG_INFO("NODE SCHEDULE INSTALLED!!\n");
}

static void
initialize_coord_schedule(void)
{

  struct tsch_slotframe *sf_common = tsch_schedule_add_slotframe(APP_SLOTFRAME_HANDLE, APP_SLOTFRAME_SIZE);
  struct tsch_slotframe *sf_tx = tsch_schedule_add_slotframe(2, 512);
  struct tsch_slotframe *sf_hop = tsch_schedule_add_slotframe(3, 512);
  // struct tsch_slotframe *sf_tx_2 = tsch_schedule_add_slotframe(3, 256);
  // int i;

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
  while (curr->id < 101)
  {
    uint16_t id = curr->id;
    linkaddr_t node_addr = curr->mac;
    slot_offset = id;

    tsch_schedule_add_link(sf_tx, LINK_OPTION_RX | LINK_OPTION_TX, LINK_TYPE_NORMAL, &node_addr, slot_offset, channel_offset, 0);

    tsch_schedule_add_link(sf_hop, LINK_OPTION_RX | LINK_OPTION_TX, LINK_TYPE_NORMAL, &node_addr, slot_offset + 1, channel_offset, 0);

    curr++;
    // LOG_INFO("ID %u \n", curr->id);
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
  uint16_t hop_id = sender_id - 1;

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
    memcpy(&count, data, sizeof(count));
    LOG_INFO("Received %u from ", count);
    LOG_INFO_LLADDR(src);
    LOG_INFO_("\n");

    if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr))
    {
      /* Initialize NullNet */
      unsigned fwd_packet;
      memcpy(&fwd_packet, data, sizeof(fwd_packet));
      
      // packetbuf_clear();

      nullnet_buf = (uint8_t *)&fwd_packet;
      nullnet_len = sizeof(fwd_packet);

      packetbuf_copyfrom(nullnet_buf, nullnet_len);

      packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &coordinator_addr);

      packetbuf_set_addr(PACKETBUF_ADDR_SENDER, src);
      LOG_INFO("sending %u bytes to ", packetbuf_datalen());
      LOG_INFO_LLADDR(packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
      LOG_INFO_("\n");
      NETSTACK_MAC.send(NULL, NULL);

      // nullnet_set_input_callback(input_callback);
      // LOG_INFO("Sending %u to ", count);
      // LOG_INFO_LLADDR(&dest_addr);
      // LOG_INFO_("\n");

      NETSTACK_NETWORK.output(&dest_addr);
    }
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned count = 0;
  static uint16_t node_id;
  int is_coordinator;

  PROCESS_BEGIN();

  // #if MAC_CONF_WITH_TSCH
  //   tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
  // #endif /* MAC_CONF_WITH_TSCH */
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

      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
      // leds_on(LEDS_RED);

      if (node_id > 2)
      {
        linkaddr_t hop_addr = get_node_addr(node_id);
        // LOG_INFO("Node id %u ", node_id);
        // LOG_INFO_("\n");
        LOG_INFO("Sending %u to ", count);
        LOG_INFO_LLADDR(&hop_addr);

        LOG_INFO_("\n");

        NETSTACK_NETWORK.output(&hop_addr);
        // leds_off(LEDS_RED);
        etimer_reset(&periodic_timer);
      }
      else
      {
        LOG_INFO("Sending %u to ", count);
        LOG_INFO_LLADDR(&dest_addr);

        LOG_INFO_("\n");

        NETSTACK_NETWORK.output(&dest_addr);
        // leds_off(LEDS_RED);
        etimer_reset(&periodic_timer);
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
