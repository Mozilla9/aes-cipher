/******************************************************************************/
/*  Class HEXFILE                                                             */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     1.2                                                         */
/*  Date:         2011/02/03 13:14:33                                         */
/******************************************************************************/


#include "stdafx.h"



static const uint8_t s_key[16] =
{
    0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA
};

static const uint8_t s_iv[16] =
{
    0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA
};




//==============================================================================
// App: Constructor
//==============================================================================
HEXFILE::HEXFILE() {
    // Для шифрации
    //aes_setkey_enc(&AesTools.ctx_enc, Key, 128);
    GetIv(AesTools.iv);

    // Буферы
    Fbuf.DataLen = 0;
    Sbuf.DataLen = 0;

    // Флаги
    FlgHex.FlgFileEnd = FALSE_T;

    // HexTools
    HexTools.DataCount = 0UL;
    HexTools.FwCrc = 0UL;
    HexTools.CurrAddr = 0x00010000;

    // RecHeader
    RecHeader.Reclen = 0;
    RecHeader.Offset = 0;
    RecHeader.TypeRec = eHEX_UNDEF_REC;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
uint32_t HEXFILE::ConversDataToFullAddr() {
    uint32_t addr = 0;

    addr = RecHeader.RowData[0];
    addr <<= 8;

    addr |= RecHeader.RowData[1];
    addr <<= 8;

    addr |= RecHeader.RowData[2];
    addr <<= 8;

    addr |= RecHeader.RowData[3];

    return addr;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
uint32_t HEXFILE::ConversDataToUpperAddr() {
    uint32_t addr = 0;

    addr = RecHeader.RowData[0];
    addr <<= 8;

    addr |= RecHeader.RowData[1];
    addr <<= 16;

    return addr;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
uint32_t HEXFILE::ConversDataToLowAddr() {
    uint32_t addr = 0;

    addr = RecHeader.RowData[0];
    addr <<= 8;

    addr |= RecHeader.RowData[1];
    addr <<= 4;

    return addr;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
void HEXFILE::PrepareReadingNewString() {
    RecHeader.Offset = 0;
    RecHeader.Reclen = 0;
    RecHeader.TypeRec = eHEX_UNDEF_REC;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
 void HEXFILE::SetRecHeader(uint8_t * _pSrc) {
     RecHeader.Reclen = *_pSrc++;
     RecHeader.Offset = *_pSrc++;
     RecHeader.Offset <<= 8;
     RecHeader.Offset |= *_pSrc++;
     RecHeader.TypeRec = (__TYPEREC_HEX)(*_pSrc);
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
void HEXFILE::WriteDataToRowBuff(uint8_t * _pSrc) {
    const uint32_t len = RecHeader.Reclen;

    for (uint32_t i = 0; i < len; i++) {
        RecHeader.RowData[i] = *_pSrc++;
    };
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
bool_t HEXFILE::FillBuffVoidData(const uint32_t _len) {
    for (uint32_t i = 0; i < _len; i++) {
       if (Fbuf.DataLen < DATA_BUFF_LEN) {
           Fbuf.Data[Fbuf.DataLen++] = 0x00;
           HexTools.FwCrc += 0x00;
           HexTools.CurrAddr++;
       }
       else {
           return FALSE_T;
       };
    };

    HexTools.DataCount += _len;
    return TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
bool_t HEXFILE::WriteBuffData() {
    bool_t FlgEncDataReady = FALSE_T;
    const uint32_t len = RecHeader.Reclen;

    for (uint32_t i = 0; i < len; i++) {
        Fbuf.Data[Fbuf.DataLen++] = RecHeader.RowData[i];
        HexTools.FwCrc += RecHeader.RowData[i];
    };

    HexTools.DataCount += RecHeader.Reclen;
    HexTools.CurrAddr += RecHeader.Reclen;

    // Проверка, не пора ли шифровать
    if (Fbuf.DataLen >= ENCRYPT_BOUNDARY) {
        //memcpy(Sbuf.Data, Fbuf.Data, ENCRYPT_BOUNDARY);
        aes_crypt_cbc(&AesTools.ctx_enc,
                      AES_ENCRYPT,
                      ENCRYPT_BOUNDARY,
                      AesTools.iv,
                      Fbuf.Data,
                      Sbuf.Data);

         Sbuf.DataLen = ENCRYPT_BOUNDARY;
         FlgEncDataReady = TRUE_T;

         // Проверка, не нужно ли перенести часть данных
         Fbuf.DataLen -= ENCRYPT_BOUNDARY;

         if (Fbuf.DataLen > 0) {
             for (uint32_t i = 0; i < Fbuf.DataLen; i++) {
                 Fbuf.Data[i] = Fbuf.Data[ENCRYPT_BOUNDARY + i];
             };
         };
    };

    return FlgEncDataReady;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
bool_t HEXFILE::FinalCiphering() {
    uint32_t Crc = 0UL;
    uint32_t DataCount = 0UL;

    // Проверка на выравнивание
    while (Fbuf.DataLen % 256) {
        Fbuf.Data[Fbuf.DataLen++] = 0x00;
        Crc += 0x00;
        DataCount++;
    };

    Crc = 0UL - (Crc + HexTools.FwCrc);
    DataCount += HexTools.DataCount;

    // Добавляем размер и контр. сумму fw
    memcpy(Fbuf.Data + Fbuf.DataLen, (uint8_t *)&DataCount, 4);
    Fbuf.DataLen += 4;
    memcpy(Fbuf.Data + Fbuf.DataLen, (uint8_t *)&Crc, 4);
    Fbuf.DataLen += 4;

    for (uint32_t i = 0; i < 8; i++) {
        Fbuf.Data[Fbuf.DataLen++] = 0;
    };

    // Шифруем
    if (Fbuf.DataLen % 16) {
        return FALSE_T;
    }
    else {
        //memcpy(Sbuf.Data, Fbuf.Data, Fbuf.DataLen);
        aes_crypt_cbc(&AesTools.ctx_enc, AES_ENCRYPT,
                      Fbuf.DataLen, AesTools.iv,
                      Fbuf.Data, Sbuf.Data);
        Sbuf.DataLen = Fbuf.DataLen;
    };

    return TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App:
//==============================================================================
void HEXFILE::GetIv(uint8_t * _pIv) {
    memcpy(_pIv, s_iv, 16);
}
//==============================================================================
//==============================================================================