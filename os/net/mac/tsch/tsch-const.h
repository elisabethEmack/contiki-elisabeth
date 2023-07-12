/*
 * Copyright (c) 2015, SICS Swedish ICT.
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
* \addtogroup tsch
* @{
 * \file
 *         TSCH constants
 * \author
 *         Simon Duquennoy <simonduq@sics.se>
 */

#ifndef __TSCH_CONST_H__
#define __TSCH_CONST_H__

/********** Includes **********/
#include "net/packetbuf.h"
#include "net/netstack.h"
#include "net/mac/tsch/tsch-conf.h" /* Required for timestlot timing consts */

/********** Constants **********/

/* Link options */
#define LINK_OPTION_TX              1
#define LINK_OPTION_RX              2
#define LINK_OPTION_SHARED          4
#define LINK_OPTION_TIME_KEEPING    8

/* Default IEEE 802.15.4e hopping sequences, obtained from https://gist.github.com/twatteyne/2e22ee3c1a802b685695 */
/* 16 channels, sequence length 16 */
#define TSCH_HOPPING_SEQUENCE_16_16 (uint8_t[]){ 16, 17, 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_15_15 (uint8_t[]){ 17, 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_14_14 (uint8_t[]){ 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_13_13 (uint8_t[]){ 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_12_12 (uint8_t[]){ 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_11_11 (uint8_t[]){ 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_10_10 (uint8_t[]){ 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_9_9 (uint8_t[]){ 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_8_8 (uint8_t[]){ 19, 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_7_7 (uint8_t[]){ 11, 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_6_6 (uint8_t[]){ 12, 13, 24, 14, 20, 21 }
#define TSCH_HOPPING_SEQUENCE_5_5 (uint8_t[]){ 13, 24, 14, 20, 21 }
/* 4 channels, sequence length 16 */
#define TSCH_HOPPING_SEQUENCE_4_16 (uint8_t[]){ 20, 26, 25, 26, 15, 15, 25, 20, 26, 15, 26, 25, 20, 15, 20, 25 }
/* 4 channels, sequence length 4 */
#define TSCH_HOPPING_SEQUENCE_4_4 (uint8_t[]){ 15, 25, 26, 20 }
#define TSCH_HOPPING_SEQUENCE_3_3 (uint8_t[]){ 15, 25, 26 }
/* 2 channels, sequence length 2 */
#define TSCH_HOPPING_SEQUENCE_2_2 (uint8_t[]){ 20, 25 }
/* 1 channel, sequence length 1 */
#define TSCH_HOPPING_SEQUENCE_1_1 (uint8_t[]){ 20 }

/*-----------------My additions------------------------*/
/** 16 channels
Sequence 1: [2, 15, 7, 9, 13, 8, 5, 12, 14, 1, 4, 0, 3, 11, 10, 6]
Sequence 2: [15, 10, 8, 9, 7, 2, 4, 0, 13, 14, 3, 5, 12, 6, 11, 1]
Index start from: [13, 15, 5, 7, 1, 11, 9, 10, 14, 8, 12, 6, 4, 3, 2, 0]
1 1 2 1 2 1 2 1 1 2 2 1
#define TSCH_ALTERNATION_SEQUENCE_SIZE 8

*/

#define TSCH_ALTERNATION_SEQUENCE (uint8_t[]){1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0}

#define TSCH_RANDOM_SEQUENCE_INDEX_16_1 (uint8_t[]){ 2, 15, 7, 9, 13, 8, 5, 12, 14, 1, 4, 0, 3, 11, 10, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_16_2 (uint8_t[]){ 15, 10, 8, 9, 7, 2, 4, 0, 13, 14, 3, 5, 12, 6, 11, 1 }
#define TSCH_POINTER_SEQUENCE_16 (uint8_t[]){ 13, 15, 5, 7, 1, 11, 9, 10, 14, 8, 12, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_15_1 (uint8_t[]){ 2, 7, 9, 13, 8, 5, 12, 14, 1, 4, 0, 3, 11, 10, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_15_2 (uint8_t[]){ 10, 8, 9, 7, 2, 4, 0, 13, 14, 3, 5, 12, 6, 11, 1 }
#define TSCH_POINTER_SEQUENCE_15 (uint8_t[]){2, 7, 9, 13, 8, 5, 12, 14, 1, 4, 0, 3, 11, 10, 6 }

#define TSCH_RANDOM_SEQUENCE_INDEX_14_1 (uint8_t[]){ 2, 7, 9, 13, 8, 5, 12, 1, 4, 0, 3, 11, 10, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_14_2 (uint8_t[]){ 10, 8, 9, 7, 2, 4, 0, 13, 3, 5, 12, 6, 11, 1 }
#define TSCH_POINTER_SEQUENCE_14 (uint8_t[]){ 2, 7, 9, 13, 8, 5, 12, 1, 4, 0, 3, 11, 10, 6}

#define TSCH_RANDOM_SEQUENCE_INDEX_13_1 (uint8_t[]){ 2, 7, 9, 8, 5, 12, 1, 4, 0, 3, 11, 10, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_13_2 (uint8_t[]){ 10, 8, 9, 7, 2, 4, 0, 3, 5, 12, 6, 11, 1 }
#define TSCH_POINTER_SEQUENCE_13 (uint8_t[]){ 5, 7, 1, 11, 9, 10, 8, 12, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_12_1 (uint8_t[]){ 2, 7, 9, 8, 5, 1, 4, 0, 3, 11, 10, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_12_2 (uint8_t[]){ 10, 8, 9, 7, 2, 4, 0, 3, 5, 6, 11, 1 }
#define TSCH_POINTER_SEQUENCE_12 (uint8_t[]){ 5, 7, 1, 11, 9, 10, 8, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_11_1 (uint8_t[]){ 2, 7, 9, 8, 5, 1, 4, 0, 3, 10, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_11_2 (uint8_t[]){ 10, 8, 9, 7, 2, 4, 0, 3, 5, 6, 1 }
#define TSCH_POINTER_SEQUENCE_11 (uint8_t[]){ 5, 7, 1, 9, 10, 8, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_10_1 (uint8_t[]){ 2, 7, 9, 8, 5, 1, 4, 0, 3, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_10_2 (uint8_t[]){ 8, 9, 7, 2, 4, 0, 3, 5, 6, 1 }
#define TSCH_POINTER_SEQUENCE_10 (uint8_t[]){ 5, 7, 1, 9, 8, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_9_1 (uint8_t[]){ 2, 7, 8, 5, 1, 4, 0, 3, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_9_2 (uint8_t[]){ 8, 7, 2, 4, 0, 3, 5, 6, 1 }
#define TSCH_POINTER_SEQUENCE_9 (uint8_t[]){ 5, 7, 1, 8, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_8_1 (uint8_t[]){ 2, 7, 5, 1, 4, 0, 3, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_8_2 (uint8_t[]){ 7, 2, 4, 0, 3, 5, 6, 1 }
#define TSCH_POINTER_SEQUENCE_8 (uint8_t[]){ 5, 7, 1, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_7_1 (uint8_t[]){ 2, 5, 1, 4, 0, 3, 6 }
#define TSCH_RANDOM_SEQUENCE_INDEX_7_2 (uint8_t[]){ 2, 4, 0, 3, 5, 6, 1 }
#define TSCH_POINTER_SEQUENCE_7 (uint8_t[]){ 5, 1, 6, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_6_1 (uint8_t[]){ 2, 5, 1, 4, 0, 3 }
#define TSCH_RANDOM_SEQUENCE_INDEX_6_2 (uint8_t[]){ 2, 4, 0, 3, 5, 1 }
#define TSCH_POINTER_SEQUENCE_6 (uint8_t[]){ 5, 1, 4, 3, 2, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_5_1 (uint8_t[]){ 2, 1, 4, 0, 3 }
#define TSCH_RANDOM_SEQUENCE_INDEX_5_2 (uint8_t[]){ 2, 4, 0, 3, 1 }
#define TSCH_POINTER_SEQUENCE_5 (uint8_t[]){ 1, 4, 3, 2, 0 }

/*4 channels*/
#define TSCH_RANDOM_SEQUENCE_INDEX_4_1 (uint8_t[]){ 1, 3, 0, 2 }
#define TSCH_RANDOM_SEQUENCE_INDEX_4_2 (uint8_t[]){ 2, 0, 1, 3 }
#define TSCH_POINTER_SEQUENCE_4 (uint8_t[]){ 3, 1, 0, 2 }

#define TSCH_RANDOM_SEQUENCE_INDEX_3_1 (uint8_t[]){ 1, 0, 2 }
#define TSCH_RANDOM_SEQUENCE_INDEX_3_2 (uint8_t[]){ 2, 0, 1 }
#define TSCH_POINTER_SEQUENCE_3 (uint8_t[]){ 1, 0, 2 }

/*2 channels*/
#define TSCH_RANDOM_SEQUENCE_INDEX_2_1 (uint8_t[]){ 1, 0 }
#define TSCH_RANDOM_SEQUENCE_INDEX_2_2 (uint8_t[]){ 0, 1 }
#define TSCH_POINTER_SEQUENCE_2 (uint8_t[]){ 1, 0 }

#define TSCH_RANDOM_SEQUENCE_INDEX_1_1 (uint8_t[]){ 0 }
#define TSCH_RANDOM_SEQUENCE_INDEX_1_2 (uint8_t[]){ 0 }
#define TSCH_POINTER_SEQUENCE_1 (uint8_t[]){ 0 }

/*Jamming default sequence*/
#define TSCH_JAMMING_SEQUENCE (uint8_t[]){ 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 }
/*-----------------------------------------------------*/

/* Max TSCH packet length equal to the length of the packet buffer */
#define TSCH_PACKET_MAX_LEN PACKETBUF_SIZE

/* The jitter to remove in ticks.
 * This should be the sum of measurement errors on Tx and Rx nodes.
 * */
#define TSCH_TIMESYNC_MEASUREMENT_ERROR US_TO_RTIMERTICKS(32)

/* The approximate number of slots per second */
#define TSCH_SLOTS_PER_SECOND (1000000 / tsch_timing_us[tsch_ts_timeslot_length])

/* Calculate packet tx/rx duration in rtimer ticks based on packet length in bytes. */
#define TSCH_PACKET_DURATION(len) US_TO_RTIMERTICKS(RADIO_BYTE_AIR_TIME * ((len) + RADIO_PHY_OVERHEAD))

/* Convert rtimer ticks to clock and vice versa */
#define TSCH_CLOCK_TO_TICKS(c) (((c) * RTIMER_SECOND) / CLOCK_SECOND)
#define TSCH_CLOCK_TO_SLOTS(c, timeslot_length) ((TSCH_CLOCK_TO_TICKS(c) + timeslot_length - 1) / timeslot_length)

#endif /* __TSCH_CONST_H__ */
/** @} */
