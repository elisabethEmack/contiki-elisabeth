/*
 * Copyright (c) 2015, SICS Swedish ICT.
 * Copyright (c) 2020, Institute of Electronics and Computer Science (EDI)
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
 */

/**
 * \author Simon Duquennoy <simonduq@sics.se>
 *         Atis Elsts <atis.elsts@edi.lv>
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_

/* Set to enable TSCH security */
#ifndef WITH_SECURITY
#define WITH_SECURITY 0
#endif /* WITH_SECURITY */

/* USB serial takes space, free more space elsewhere */
#define SICSLOWPAN_CONF_FRAG 0
#define UIP_CONF_BUFFER_SIZE 160

/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/

/* Disable the 6TiSCH minimal schedule */
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0
// #define UIP_CONF_BUFFER_SIZE 32

/* Size of the application-specific schedule; use harmonic number 2,4,8,..  */
#define APP_SLOTFRAME_SIZE 256
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 8
//64 default
#define QUEUEBUF_CONF_NUM 8
#define NBR_TABLE_CONF_MAX_NEIGHBORS 8
#define NETSTACK_MAX_ROUTE_ENTRIES 8

#define SICSLOWPAN_CONF_FRAG 0

#define UIP_CONF_UDP 0

#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE		TSCH_JAMMING_SEQUENCE
#define TSCH_CONF_JOIN_HOPPING_SEQUENCE		TSCH_JAMMING_SEQUENCE
//#define TSCH_CONF_DEFAULT_RANDOM_SEQUENCE_INDEX_1 	TSCH_RANDOM_SEQUENCE_INDEX_16_1 TSCH_JAMMING_SEQUENCE
//#define TSCH_CONF_DEFAULT_RANDOM_SEQUENCE_INDEX_2 	TSCH_RANDOM_SEQUENCE_INDEX_16_2
//#define TSCH_CONF_DEFAULT_POINTER_SEQUENCE 		TSCH_POINTER_SEQUENCE_16
//#define TSCH_CONF_ALTERNATION_SEQUENCE 	TSCH_ALTERNATION_SEQUENCE

/*******************************************************/
/************* Other system configuration **************/
/***
*#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_WARN
*#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_WARN
*#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_WARN
***************************************************/

/* Logging */
// #define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_INFO
//#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_WARN
#define TSCH_LOG_CONF_PER_SLOT                     1

#endif /* PROJECT_CONF_H_ */
