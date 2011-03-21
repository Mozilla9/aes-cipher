/******************************************************************************/
/*  Class HEXFILE                                                             */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     1.3                                                         */
/*  Date:         2011/02/06 21:49:33                                         */
/******************************************************************************/


#ifndef __HEXFILE_H
#define __HEXFILE_H


#include "hex_def.h"


//==============================================================================
//  Class HEXFILE
//==============================================================================
class HEXFILE
{
protected:
    HEXFILE() {}

    uint32_t GetLen() const {
        return m_info.len;
    }

    uint32_t GetCrc() const {
        return m_info.crc;
    }

    uint32_t GetSegmentStartAddr() const {
        return m_info.segmStartAddr.data32;
    }

    uint32_t GetLinearStartAddr() const {
        return m_info.linearStartAddr.data32;
    }

    const std::vector <uint8_t>& GetData() const {
        return m_data.data;
    }

    bool_t OpenHexFile(const std::string& _name);
    void PrintError(const uint32_t _error);
    uint32_t RunConversion();

public:
    virtual ~HEXFILE() {
        if (m_data.hexFile.is_open()) {
            m_data.hexFile.close();
        };
    }

    static uint8_t HexToBin(const int8_t * _pHex);


private:
    uint32_t ReadString(__RECLEN_HEADER_HEX& _recHeader);
    void InsertVoidData(std::vector <uint8_t>& _dst, const std::size_t _count);

    __DATA_HEX          m_data;
    __PARSING_TOOLS_HEX m_tools;
    __STAT_INFO_HEX     m_info;
};
//==============================================================================
//==============================================================================


#endif