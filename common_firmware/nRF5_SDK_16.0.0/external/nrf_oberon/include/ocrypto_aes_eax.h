/**
 * Copyright (c) 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**@file
 * @defgroup nrf_oberon_aes_eax AES EAX APIs
 * @ingroup nrf_oberon_aes
 * @{
 * @brief Type definitions and APIS for AES EAX (Encrypt-then-authenticate-then-translate)
 *
 * AES (advanced encryption standard) is a symmetric encryption algorithm standardized by NIST.
 * AES transfers a 128-bit block of data into an encrypted block of the same size.
 *
 * AES-EAX (encrypt-then-authenticate-then-translate) is an AES mode which effectively turns the
 * block cipher into a stream cipher. The AES block cipher primitive is used in CTR mode for
 * encryption and as OMAC for authentication over each block.
 */

#ifndef OCRYPTO_AES_EAX_H
#define OCRYPTO_AES_EAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>


/**
 * AES-EAX encryption.
 *
 * @param[out] ct     Cyphertext.
 * @param[out] tag    Authentication tag.
 * @param      pt     Plaintext.
 * @param      pt_len Plaintext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32 bytes).
 * @param      iv     Initial vector.
 * @param      iv_len Initial vector length.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct and @p pt can point to the same address.
 */
void ocrypto_aes_eax_encrypt (
    uint8_t* ct, uint8_t tag[16], const uint8_t* pt, size_t pt_len,  const uint8_t *key, size_t size,
    const uint8_t* iv, size_t iv_len, const uint8_t *aa, size_t aa_len);

/**
 * AES-EAX decryption.
 *
 * @param[out] pt     Plaintext.
 * @param      tag    Authentication tag.
 * @param      ct     Cyphertext.
 * @param      ct_len Cyphertext length.
 * @param      key    AES key.
 * @param      size   Key size (16, 24, or 32 bytes).
 * @param      iv     Initial vector.
 * @param      iv_len Initial vector length.
 * @param      aa     Additional authentication data.
 * @param      aa_len Additional authentication data length.
 *
 * @remark @p ct and @p pt can point to the same address.
 *
 * @retval 0  If @p tag is valid.
 * @retval -1 Otherwise.
 */
int ocrypto_aes_eax_decrypt (
    uint8_t* pt, const uint8_t tag[16], const uint8_t* ct, size_t ct_len, const uint8_t *key, size_t size,
    const uint8_t* iv, size_t iv_len, const uint8_t *aa, size_t aa_len);


#ifdef __cplusplus
}
#endif

#endif
/** @} */
