/******************************************************************************/
/* bin_to_hex.h                                                               */
/******************************************************************************/


#ifndef __BIN_TO_HEX_H
#define __BIN_TO_HEX_H


/**************************** Func declarations *******************************/
uint32_t BinToHex(const uint8_t Bin, uint8_t * const pDst);
uint32_t BinToHex(const uint16_t Bin, uint8_t * const pDst);
uint32_t BinToHex(const uint32_t Bin, uint8_t * const pDst);
uint32_t HexToBin(const uint8_t * const pHex);


#endif