/******************************************************************************/
/*  Class HEXFILE                                                             */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     1.2                                                         */
/*  Date:         2011/02/03 13:14:33                                         */
/******************************************************************************/


#include "stdafx.h"
using namespace std;



//==============================================================================
// App: Convert hex string to bin
//==============================================================================
uint8_t HEXFILE::HexToBin(const int8_t * _pHex) {
    uint8_t bin;

    bin = (_pHex[0] > 0x39) ? (_pHex[0] - 0x37) : (_pHex[0] - 0x30);
    bin <<= 4;
    bin |= (_pHex[1] > 0x39) ? (_pHex[1] - 0x37) : (_pHex[1] - 0x30);

    return bin;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Open hex file
//==============================================================================
bool_t HEXFILE::OpenHexFile(const std::string& _name) {

    if (_name.size()) {
        m_data.hexFile.open(_name.c_str(), std::ios::in);
    };

    std::cout << "\nOpen hex file: " << (m_data.hexFile.is_open())
               ? "ok\n" : "fail\n";
    return (m_data.hexFile.is_open()) ? TRUE_T : FALSE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Read string from file
//==============================================================================
uint32_t HEXFILE::ReadString(__RECLEN_HEADER_HEX& _recHeader) {
    std::string str;
    uint8_t buf[HEX_ROW_DATA_MAX];

    // Read string from file
    std::getline(m_data.hexFile, str);

    if (str[0] != ':') {
        return eHEX_BAD_MARKER;
    };

    if (str.size() < HEX_STRING_MIN_SIZE) {
        return eHEX_DATALEN;
    };

    if ((str.size() - 1) % 2) {
        return eHEX_PARITY;
    };

    // Convert hex to bin and calc crc
    uint8_t crc = 0;
    uint32_t ind = 0;
    std::string::size_type sz = (str.size() - 1) / 2;

    for (std::string::size_type i = 0; i < sz; i++) {
        buf[ind] = HexToBin(str.c_str() + 1 + i*2);
        crc += buf[ind++];
    };

    // Test crc
    if (crc) {
        return eHEX_CRC;
    };

    // Fill recHeader struct
    _recHeader.len = buf[0];
    _recHeader.offset = buf[1];
    _recHeader.offset <<= 8;
    _recHeader.offset |= buf[2];
    _recHeader.type = buf[3];
    memcpy(_recHeader.rowData, buf + 4, _recHeader.len);

    return eHEX_OK;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Insert void data
//==============================================================================
void HEXFILE::InsertVoidData(std::vector <uint8_t>& _dst,
                             const std::size_t _count) {

    std::vector <uint8_t> temp;

    temp.assign(_count, 0);
    _dst.insert(_dst.end(), temp.begin(), temp.end());
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Print error
//==============================================================================
void HEXFILE::PrintError(const uint32_t _error) {
    std::string err;

    switch (_error)
    {
        //----------------------------------------------------------------------
        case eHEX_OK:
            return;

        //----------------------------------------------------------------------
        case eHEX_BAD_MARKER:
            err = "HEX_BAD_MARKER";
        break;

        //----------------------------------------------------------------------
        case eHEX_DATALEN:
            err = "HEX_DATALEN";
        break;

        //----------------------------------------------------------------------
        case eHEX_PARITY:
            err = "HEX_PARITY";
        break;

        //----------------------------------------------------------------------
        case eHEX_CRC:
            err = "HEX_CRC";
        break;

        //----------------------------------------------------------------------
        case eHEX_BAD_REC:
            err = "HEX_BAD_REC";
        break;

        //----------------------------------------------------------------------
        case eHEX_BAD_REC_LEN:
            err = "HEX_BAD_REC_LEN";
        break;

        //----------------------------------------------------------------------
        case eHEX_LINEAR_ADDR_LESS_CURR_ADDR:
            err = "HEX_LINEAR_ADDR_LESS_CURR_ADDR";
        break;

        //----------------------------------------------------------------------
        case eHEX_SEG_ADDR_LESS_CURR_ADDR:
            err = "HEX_SEG_ADDR_LESS_CURR_ADDR";
        break;

        //----------------------------------------------------------------------
        case eHEX_OFFSET_LESS_CURR_OFFSET:
            err = "HEX_OFFSET_LESS_CURR_OFFSET";
        break;

        //----------------------------------------------------------------------
        case eHEX_FILE:
            err = "HEX_FILE_CLOSED";
        break;

        //----------------------------------------------------------------------
        default:
            err = "UNDEF_ERR";
        break;
    };

    std::cout << "\nError line: " << m_info.stringNum << "\n";
    std::cout << "Error: " << err << "\n";
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Convers hex file to bin file
//==============================================================================
uint32_t HEXFILE::RunConversion() {

    static bool_t isLinearAddrSet = FALSE_T;


    if (!m_data.hexFile.is_open()) {
        return eHEX_FILE;
    };

    // Run
    m_data.data.clear();
    m_data.hexFile.seekg(ios_base::beg);

    while (m_tools.isFileEnd == FALSE_T)
    {
        uint32_t err;
        __RECLEN_HEADER_HEX recHeader;

        // Inc string num
        m_info.stringNum++;

        // Read string from file
        err = ReadString(recHeader);
        if (err) {
            return err;
        };

        // Parsing header
        switch (recHeader.type)
        {
            //------------------------------------------------------------------
            case eHEX_DATA:
                if (recHeader.offset < m_tools.addr.data16[0]) {
                    return eHEX_OFFSET_LESS_CURR_OFFSET;
                }
                else {
                    const uint32_t len = recHeader.len;

                    // Test - need insert void data
                    if (recHeader.offset > m_tools.addr.data16[0]) {
                        InsertVoidData(m_data.data,
                                       recHeader.offset - m_tools.addr.data16[0]);

                        m_tools.addr.data16[0] = recHeader.offset;
                    };

                    // Insert data
                    for (uint32_t i = 0; i < len; i++) {
                        m_data.data.push_back(recHeader.rowData[i]);
                        m_tools.addr.data16[0]++;
                    };
                };
            break;

            //------------------------------------------------------------------
            case eHEX_FILE_END:
                if ((recHeader.len == 0) && (recHeader.offset == 0)) {
                    m_tools.isFileEnd = TRUE_T;
                }
                else {
                    return eHEX_BAD_REC_LEN;
                };
            break;

            //------------------------------------------------------------------
            case eHEX_SEG_ADDR:
                if (recHeader.len == 2) {
                    __UNION_DWORD data;

                    data.data8[3] = recHeader.rowData[0];
                    data.data8[2] = recHeader.rowData[1];
                    data.data32 <<= 4;

                    // Test - need insert void data
                    if (data.data32 > m_tools.addr.data32) {
                        InsertVoidData(m_data.data,
                                       data.data32 - m_tools.addr.data32);

                        m_tools.addr.data32 = data.data32;
                    }
                    else if (data.data32 < m_tools.addr.data32) {
                        return eHEX_SEG_ADDR_LESS_CURR_ADDR;
                    };
                }
                else {
                    return eHEX_BAD_REC_LEN;
                };
            break;

            //------------------------------------------------------------------
            case eHEX_SEG_ST_ADDR:
                // Unused rec, but collect info
                if (recHeader.len == 4) {
                    m_info.segmStartAddr.data8[3] = recHeader.rowData[0];
                    m_info.segmStartAddr.data8[2] = recHeader.rowData[1];
                    m_info.segmStartAddr.data8[1] = recHeader.rowData[2];
                    m_info.segmStartAddr.data8[0] = recHeader.rowData[3];
                }
                else {
                    return eHEX_BAD_REC_LEN;
                };
            break;

            //------------------------------------------------------------------
            case eHEX_LINEAR_ADDR:
                if (recHeader.len == 2) {
                    __UNION_DWORD data;

                    if (isLinearAddrSet == TRUE_T) {
                        data.data8[3] = recHeader.rowData[0];
                        data.data8[2] = recHeader.rowData[1];

                        // Test - need insert void data
                        if (data.data32 > m_tools.addr.data32) {
                            InsertVoidData(m_data.data,
                                           data.data32 - m_tools.addr.data32);

                            m_tools.addr.data32 = data.data32;
                        }
                        else if (data.data32 < m_tools.addr.data32) {
                            return eHEX_LINEAR_ADDR_LESS_CURR_ADDR;
                        };
                    };
                    isLinearAddrSet = TRUE_T;
                }
                else {
                    return eHEX_BAD_REC_LEN;
                };
            break;

            //------------------------------------------------------------------
            case eHEX_LINEAR_ST_ADDR:
                // Unused rec, but collect info
                if (recHeader.len == 4) {
                    m_info.linearStartAddr.data8[3] = recHeader.rowData[0];
                    m_info.linearStartAddr.data8[2] = recHeader.rowData[1];
                    m_info.linearStartAddr.data8[1] = recHeader.rowData[2];
                    m_info.linearStartAddr.data8[0] = recHeader.rowData[3];
                }
                else {
                    return eHEX_BAD_REC_LEN;
                };
            break;

            //------------------------------------------------------------------
            default:
                return eHEX_BAD_REC;
        };
    };

    // Calc crc
    for (std::vector <uint8_t>::size_type i = 0; i < m_data.data.size(); i++) {
        m_info.crc += (uint32_t)m_data.data[i];
    };
    m_info.crc = 0UL - m_info.crc;

    // Save datalen
    m_info.len = m_data.data.size();

    return eHEX_OK;
}
//==============================================================================
//==============================================================================