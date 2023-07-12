#include "contiki.h"
#include "sys/node-id.h"
#include "sys/log.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-sr.h"
#include "net/mac/tsch/tsch.h"
#include "net/routing/routing.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_DBG
#define DEBUG DEBUG_PRINT
#include "net/ipv6/uip-debug.h"
#include "lib/list.h"
#include "lib/memb.h"


/* List Structure to store IPv6 address from the routing table */
struct list_neighbor {
  struct list_neigbor *next;
  const uip_ipaddr_t ipaddr;
};

/* Configuration */
static linkaddr_t dest_addr =         {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
static linkaddr_t coordinator_addr =  {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

static linkaddr_t src_2_addr =         {{ 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
//static linkaddr_t src_3_addr =         {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

LIST(neighb_table);                    // Contiki defined list 
MEMB(neighbor_mem, struct list_neighbor, 5);    //memory block for the list MEMB(name, structure, num);

/*---------------------------------------------------------------------------*/
PROCESS(node_process, "Discover Neighbor");
AUTOSTART_PROCESSES(&node_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(node_process, ev, data)
{
  int is_coordinator;

  PROCESS_BEGIN();

/* Initialize the memory for the neighbor table entries. */
  memb_init(&route_mem);

/* Initialize the list used for the neighbor table. */
  list_init(neighb_table);



  is_coordinator = 0;

#if CONTIKI_TARGET_COOJA || CONTIKI_TARGET_Z1
  is_coordinator = (node_id == 1);
#endif

  if(is_coordinator) {
    NETSTACK_ROUTING.root_start();
  }
  NETSTACK_MAC.on();
  static struct etimer et;
  /* Print out routing tables every minute */
  etimer_set(&et, CLOCK_SECOND * 60);
  
  while(1) {
//  	LOG_INFO("Routing entries: %u\n", uip_ds6_route_num_routes());
  	LOG_INFO("Neighbor entries: %u\n", uip_ds6_nbr_num());
//  	if(uip_ds6_route_num_routes()){ 
	if(uip_ds6_nbr_num()){
  		uip_ds6_nbr_t *neighbor = uip_ds6_nbr_head();
      	struct list_neighbor *nbr;
      	while(neighbor) {
        	LOG_INFO("Neighbor ");
        	LOG_INFO_6ADDR(&neighbor->ipaddr);
        	//LOG_INFO_("/128 via ");
        	//LOG_INFO_6ADDR(uip_ds6_route_nexthop(route));
        	LOG_INFO_("\n");
    
    	/* Removing any previous same entry from the list (to avoid buffer overflow)*/
    	for(e = list_head(neighb_table); nbr != NULL; nbr = nbr->next) {
    		if(uip_ip6addr_cmp(&nbr->ipaddr,uip_ds6_nbr_next(neighbor))) {
    			list_remove(neighb_table, nbr);
        		memb_free(&route_mem, nbr);
        	}
    	}


    	nbr = memb_alloc(&neighbor_mem);                 // Allocate memory from the route_mem pool
    	nbr->next=NULL;
    	uip_ip6addr_copy(&nbr->ipaddr, uip_ds6_route_next(neighbor));     //Copy IPv6 address
    	list_add(neighb_table, nbr);                                      //Append the structure in the list

    	route = uip_ds6_route_next(route);
    	}
    
    	/* Print out the list */
    	nbr = list_head(neighb_table);
    	while (nbr != NULL){
        	LOG_INFO_("Neighbor IPV6 ");
        	LOG_INFO_6ADDR(&nbr->ipaddr);
        	LOG_INFO_("\n");
        	nbr=nbr->next;
        	}
    
    	}
      PROCESS_YIELD_UNTIL(etimer_expired(&et));
      etimer_reset(&et);
    
  }
  PROCESS_END();
}
