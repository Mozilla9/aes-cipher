/******************************************************************************/
/* FIPS-197 compliant AES implementation                                      */
/*                                                                            */
/* Copyright (C) 2006-2009, Paul Bakker <polarssl_maintainer at polarssl.org> */
/* All rights reserved.                                                       */
/*                                                                            */
/* Joined copyright on original XySSL code with: Christophe Devine            */
/*                                                                            */
/* This program is free software; you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation; either version 2 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              */
/* GNU General Public License for more details.                               */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program; if not, write to the Free Software Foundation, Inc.,    */
/* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                */
/*                                                                            */
/*                                                                            */
/* The AES block cipher was designed by Vincent Rijmen and Joan Daemen.       */
/*                                                                            */
/* http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf                  */
/* http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf                */
/******************************************************************************/


#ifndef __AES_H
#define __AES_H


/****************************** Lib settings **********************************/
#define POLARSSL_HAVE_INT16
#define AES_ENCRYPT                       TRUE_T
#define AES_DECRYPT                       FALSE_T
#define ERR_AES_INVALID_KEY_LENGTH        (0x00000800)


/********************************** Macros ************************************/
#define GET_ULONG_LE(n,b,i)                   \
{                                             \
    (n) = ((uint32_t) (b)[(i)    ]      )     \
        | ((uint32_t) (b)[(i) + 1] <<  8)     \
        | ((uint32_t) (b)[(i) + 2] << 16)     \
        | ((uint32_t) (b)[(i) + 3] << 24);    \
}


#define PUT_ULONG_LE(n,b,i)                   \
{                                             \
    (b)[(i)    ] = (uint8_t) ((n)      );     \
    (b)[(i) + 1] = (uint8_t) ((n) >>  8);     \
    (b)[(i) + 2] = (uint8_t) ((n) >> 16);     \
    (b)[(i) + 3] = (uint8_t) ((n) >> 24);     \
}


#define AES_FROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)   \
{                                             \
    X0 = *RK++ ^ FT0[ (Y0      ) & 0xFF ] ^   \
                 FT1[ (Y1 >>  8) & 0xFF ] ^   \
                 FT2[ (Y2 >> 16) & 0xFF ] ^   \
                 FT3[ (Y3 >> 24) & 0xFF ];    \
                                              \
    X1 = *RK++ ^ FT0[ (Y1      ) & 0xFF ] ^   \
                 FT1[ (Y2 >>  8) & 0xFF ] ^   \
                 FT2[ (Y3 >> 16) & 0xFF ] ^   \
                 FT3[ (Y0 >> 24) & 0xFF ];    \
                                              \
    X2 = *RK++ ^ FT0[ (Y2      ) & 0xFF ] ^   \
                 FT1[ (Y3 >>  8) & 0xFF ] ^   \
                 FT2[ (Y0 >> 16) & 0xFF ] ^   \
                 FT3[ (Y1 >> 24) & 0xFF ];    \
                                              \
    X3 = *RK++ ^ FT0[ (Y3      ) & 0xFF ] ^   \
                 FT1[ (Y0 >>  8) & 0xFF ] ^   \
                 FT2[ (Y1 >> 16) & 0xFF ] ^   \
                 FT3[ (Y2 >> 24) & 0xFF ];    \
}


#define AES_RROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)   \
{                                             \
    X0 = *RK++ ^ RT0[ (Y0      ) & 0xFF ] ^   \
                 RT1[ (Y3 >>  8) & 0xFF ] ^   \
                 RT2[ (Y2 >> 16) & 0xFF ] ^   \
                 RT3[ (Y1 >> 24) & 0xFF ];    \
                                              \
    X1 = *RK++ ^ RT0[ (Y1      ) & 0xFF ] ^   \
                 RT1[ (Y0 >>  8) & 0xFF ] ^   \
                 RT2[ (Y3 >> 16) & 0xFF ] ^   \
                 RT3[ (Y2 >> 24) & 0xFF ];    \
                                              \
    X2 = *RK++ ^ RT0[ (Y2      ) & 0xFF ] ^   \
                 RT1[ (Y1 >>  8) & 0xFF ] ^   \
                 RT2[ (Y0 >> 16) & 0xFF ] ^   \
                 RT3[ (Y3 >> 24) & 0xFF ];    \
                                              \
    X3 = *RK++ ^ RT0[ (Y3      ) & 0xFF ] ^   \
                 RT1[ (Y2 >>  8) & 0xFF ] ^   \
                 RT2[ (Y1 >> 16) & 0xFF ] ^   \
                 RT3[ (Y0 >> 24) & 0xFF ];    \
}


/************************* Prototypes struct for AES **************************/
typedef struct
{
    uint32_t nr;           // number of rounds
    uint32_t * rk;         // AES round keys
    uint32_t buf[68];      // unaligned data
} __aes_context;


/**************************** Func declarations *******************************/
// AES key schedule (encryption)
// param _ctx       AES context to be initialized
// param _key       encryption key
// param _keysize   must be 128, 192 or 256
// return           0 if successful, or ERR_AES_INVALID_KEY_LENGTH
uint32_t aes_set_key_enc(__aes_context * _ctx,
                         const uint8_t * const _key,
                         const uint32_t _keysize);


// AES _key schedule (decryption)
// param _ctx      AES context to be initialized
// param _key      decryption key
// param _keysize  must be 128, 192 or 256
// return          0 if successful, or ERR_AES_INVALID_KEY_LENGTH
uint32_t aes_set_key_dec(__aes_context * const _ctx,
                         const uint8_t * const _key,
                         const uint32_t _keysize);


// AES-ECB block encryption/decryption
// param _ctx       AES context
// param _mode      AES_ENCRYPT or AES_DECRYPT
// param _inputs    16-byte inputs block
// param _outputs   16-byte outputs block
void aes_crypt_ecb(__aes_context * const _ctx,
                   const bool_t _mode,
                   uint8_t _inputs[16],
                   uint8_t _outputs[16]);


// AES-CBC buffer encryption/decryption
// length should be a multiple of the block size (16 bytes)
// param _ctx       AES context
// param _mode      AES_ENCRYPT or AES_DECRYPT
// param _length    length of the inputs data
// param _iv        initialization vector (updated after use)
// param _inputs    buffer holding the inputs data
// param _outputs   buffer holding the outputs data
void aes_crypt_cbc(__aes_context * const _ctx,
                   const bool_t _mode,
                   uint32_t _length,
                   uint8_t * const _iv,
                   uint8_t * _inputs,
                   uint8_t * _outputs);


// AES-CFB128 buffer encryption/decryption.
// param _ctx       AES context
// param _mode      AES_ENCRYPT or AES_DECRYPT
// param _length    length of the inputs data
// param _iv_off    offset in iv (updated after use)
// param _iv        initialization vector (updated after use)
// param _inputs    buffer holding the _inputs data
// param _outputs   buffer holding the _outputs data
void aes_crypt_cfb128(__aes_context * const _ctx,
                      const bool_t _mode,
                      uint32_t _length,
                      uint32_t * _iv_off,
                      uint8_t _iv[16],
                      uint8_t * _inputs,
                      uint8_t * _outputs);



#endif