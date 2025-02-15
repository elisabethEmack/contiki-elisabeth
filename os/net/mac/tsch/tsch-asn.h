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
 *         TSCH 5-Byte Absolute Slot Number (ASN) management
 * \author
 *         Simon Duquennoy <simonduq@sics.se>
*/

#ifndef __TSCH_ASN_H__
#define __TSCH_ASN_H__

/************ Types ***********/

/** \brief The ASN is an absolute slot number over 5 bytes. */
struct tsch_asn_t {
  uint32_t ls4b; /* least significant 4 bytes */
  uint8_t  ms1b; /* most significant 1 byte */
};

/** \brief For quick modulo operation on ASN */
struct tsch_asn_divisor_t {
  uint16_t val; /* Divisor value */
  uint16_t asn_ms1b_remainder; /* Remainder of the operation 0x100000000 / val */
};

/************ Macros **********/

/** \brief Initialize ASN */
#define TSCH_ASN_INIT(asn, ms1b_, ls4b_) do { \
    (asn).ms1b = (ms1b_); \
    (asn).ls4b = (ls4b_); \
} while(0);

/** \brief Increment an ASN by inc (32 bits) */
#define TSCH_ASN_INC(asn, inc) do { \
    uint32_t new_ls4b = (asn).ls4b + (inc); \
    if(new_ls4b < (asn).ls4b) { (asn).ms1b++; } \
    (asn).ls4b = new_ls4b; \
} while(0);

/** \brief Decrement an ASN by inc (32 bits) */
#define TSCH_ASN_DEC(asn, dec) do { \
    uint32_t new_ls4b = (asn).ls4b - (dec); \
    if(new_ls4b > (asn).ls4b) { (asn).ms1b--; } \
    (asn).ls4b = new_ls4b; \
} while(0);

/** \brief Returns the 32-bit diff between asn1 and asn2 */
#define TSCH_ASN_DIFF(asn1, asn2) \
  ((asn1).ls4b - (asn2).ls4b)

/** \brief Initialize a struct asn_divisor_t */
#define TSCH_ASN_DIVISOR_INIT(div, val_) do { \
    (div).val = (val_); \
    (div).asn_ms1b_remainder = ((0xffffffff % (val_)) + 1) % (val_); \
} while(0);

/** \brief Returns the result (16 bits) of a modulo operation on ASN,
 * with divisor being a struct asn_divisor_t */
#define TSCH_ASN_MOD(asn, div) \
  ((uint16_t)((asn).ls4b % (div).val) \
   + (uint16_t)((asn).ms1b * (div).asn_ms1b_remainder % (div).val)) \
  % (div).val


//MY ADDITIONS:
/** public static int getIndexPointer(int asn, int seq_length) {
        // index = ((asn - 1)/seq_length) % seq_length
        int index = ((asn) / seq_length) % seq_length;
        return index;
    }
\brief Returns the result (16 bits) of a modulo operation on ASN,
 * with divisor being a struct asn_divisor_t */
#define TSCH_ASN_POINTER_MOD(asn, div) \
  ((uint16_t)(((asn).ls4b / (div).val)% (div).val) \
   + (uint16_t)((asn).ms1b / (div).val * (div).asn_ms1b_remainder % (div).val)) \
  % (div).val

/** public static int getSeq_idx(int asn, int seq_length) {
        //col = ((ASN/seq_lenth^2) % 2) +1
        int col = ((asn / ((seq_length * seq_length))) % 2);
        return col;
    }
\brief Returns the result (16 bits) of a modulo operation on ASN,
 * with divisor being a struct asn_divisor_t */
#define TSCH_ASN_SEQUENCE_MOD(asn, div, mod) \
  ((uint16_t)(((asn).ls4b / ((div).val * (div).val)) % mod) \
   + (uint16_t)((asn).ms1b / ((div).val * (div).val) * (div).asn_ms1b_remainder * (div).asn_ms1b_remainder % mod )) \
  % mod


#endif /* __TSCH_ASN_H__ */
/** @} */
