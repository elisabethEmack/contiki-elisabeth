
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* Set to enable TSCH security */
//#ifndef WITH_SECURITY
//#define WITH_SECURITY 0
//#endif /* WITH_SECURITY */

/* USB serial takes space, free more space elsewhere */
#define SICSLOWPAN_CONF_FRAG 0
#define UIP_CONF_BUFFER_SIZE 160

/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/

/* Disable the 6TiSCH minimal schedule */
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0

/* Size of the application-specific schedule; use harmonic number 2,4,8,..  */
#define APP_SLOTFRAME_SIZE 32

#define QUEUEBUF_CONF_NUM 32
#define NBR_TABLE_CONF_MAX_NEIGHBORS 8
#define NETSTACK_MAX_ROUTE_ENTRIES 8

#define SICSLOWPAN_CONF_FRAG 0

#if WITH_SECURITY

/* Enable security */
//#define LLSEC802154_CONF_ENABLED 0

#endif /* WITH_SECURITY */

#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE		TSCH_HOPPING_SEQUENCE_2_2

#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_DBG
//#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_INFO
#define TSCH_LOG_CONF_PER_SLOT                     1

#endif /* PROJECT_CONF_H_ */
