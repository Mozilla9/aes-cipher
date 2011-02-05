/******************************************************************************/
/*  Class CIPHER                                                              */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     1.0                                                         */
/*  Date:         2011/02/03 14:17:33                                         */
/******************************************************************************/

#include "stdafx.h"
using namespace std;


//==============================================================================
// App: Remove blank
//==============================================================================
bool_t CIPHER::RemoveBlank(std::string& _str) {

    // Extract param
    if ((_str.empty() == TRUE_T) || (_str.size() < 2)) {
        return FALSE_T;
    };

    if (_str[0] != '-') {
        return FALSE_T;
    };
    _str.erase(_str.begin());

    if (_str[0] == '-') {
        _str.erase(_str.begin());
    };

    return (_str.empty() == TRUE_T) ? FALSE_T : TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Extract key (vector)
//==============================================================================
bool_t CIPHER::ExtractKey(std::string& _hexStr, std::vector <uint8_t>& _dstArray) {

    // Remove blank
    if (RemoveBlank(_hexStr) == FALSE_T) {
        return FALSE_T;
    };

    // Convert from hex-string to bin-array
    if (_hexStr.size() != 32) {
        return FALSE_T;
    };

    _dstArray.clear();
    for (uint32_t i = 0; i < 16; i++) {
        _dstArray.push_back(HexToBin(_hexStr.c_str() + i*2));
    };

    return TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Print help
//==============================================================================
void CIPHER::PrintHelp(const std::string _self) {

    std::cout << "\n";
    std::cout << "syntax: " << _self << " ";
    std::cout << "[options] [input file] [output file] {[128b key] [128b vector]}\n";

    std::cout << "\n";
    std::cout << "       options:\n";
    std::cout << "            -?  --help         - show this help\n";
    std::cout << "            -e  --encrypt      - encrypt output file\n";
    std::cout << "            -b  --bin          - binary output file\n";

    std::cout << "\n";
    std::cout << "       input file:\n";
    std::cout << "            -*.hex (Intel Hexadecimal Object File)\n";

    std::cout << "\n";
    std::cout << "       output file:\n";
    std::cout << "            -*.* file name (File to records results)\n";

    std::cout << "\n";
    std::cout << "       128b key:\n";
    std::cout << "            - if use option -e (--encrypt)\n";
    std::cout << "            - key is the hexadecimal string (32 char)\n";
    std::cout << "            - example: 1A9F1A9FF1F18D7DD3D46DFFAABBCCEE\n";

    std::cout << "\n";
    std::cout << "       128b vector:\n";
    std::cout << "            - if use option -e (--encrypt)\n";
    std::cout << "            - vector is the hexadecimal string (32 char)\n";
    std::cout << "            - example: 1A9F1A9FF1F18D7DD3D46DFFAABBCCEE\n";

    std::cout << "\n";
    std::cout << "       Example of using:\n";
    std::cout << "            cipher.exe -e -fw.hex -fw.bin "
              << "-1A9F1A9FF1F18D7DD3D46DFFAABBCCEE "
              << "-5A111111111111111111111111111133\r\n";
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Create dst file
//==============================================================================
bool_t CIPHER::CreateDstFile() {
    const std::string str = m_tools.dirName + m_tools.binFileName;
    m_tools.binFile.open(str.c_str(), std::ios::in|std::ios::out|std::ios::trunc);

    if (!m_tools.binFile.is_open()) {
        std::cout << "\nCant create " << str << "\n";
        return FALSE_T;
    };

    return TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Get dir
//==============================================================================
void CIPHER::GetDir(const int8_t * const _dir) {
    m_tools.dirName = _dir;
    const uint32_t len = m_tools.dirName.size();
    std::string::iterator it;

    // Dele chars from the end until it reaches '/' or '\'
    for (uint32_t i = 0; i < len; i++) {
        it = m_tools.dirName.end();
        if (((it-1)[0] == '/') || ((it-1)[0] == '\\' )) {
            break;
        };
        m_tools.dirName.erase(it - 1);
    };
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Get settings from cmd line
//==============================================================================
bool_t CIPHER::GetSettFromCmdLine(const int32_t _argc, const int8_t ** _argv) {

    std::string str;

    if(_argc < 4) {
        return FALSE_T;
    };

    // Dir
    GetDir(_argv[0]);

    // Option
    str = _argv[1];

    if ((str == "-?") || (str == "--help")) {
        return FALSE_T;
    }
    else if ((str == "-e") || (str == "--encrypt")) {
        if (_argc == 6) {
            m_tools.opt = eOPT_CIPHER;
        };
    }
    else if ((str == "-b") || (str == "--bin")) {
        if (_argc == 4) {
            m_tools.opt = eOPT_BIN;
        };
    };

    if (m_tools.opt == eOPT_UNDEF) {
        std::cout << "\nBad parameters!\n";
        return FALSE_T;
    };

    // Input hex file name
    str = _argv[2];
    if (RemoveBlank(str) == FALSE_T) {
        std::cout << "\nBad input file name!\n";
        return FALSE_T;
    };
    m_tools.hexFileName = str;

    // Output file name
    str = _argv[3];
    if (RemoveBlank(str) == FALSE_T) {
        std::cout << "\nBad output file name!\n";
        return FALSE_T;
    };
    m_tools.binFileName = str;

    // Key and vector
    switch (m_tools.opt)
    {
        //----------------------------------------------------------------------
        case eOPT_CIPHER:
            // Extract key
            str = _argv[4];
            if (ExtractKey(str, m_tools.key) == FALSE_T) {
                std::cout << "\nBad key!\n";
                return FALSE_T;
            };

            // Extract vector
            str = _argv[5];
            if (ExtractKey(str, m_tools.vector) == FALSE_T) {
                std::cout << "\nBad vector!\n";
                return FALSE_T;
            };
        break;

        //----------------------------------------------------------------------
        case eOPT_BIN:
        break;

        //----------------------------------------------------------------------
        default:
            std::cout << "\nUndefined error!\n";
            return FALSE_T;
    };

    return TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Create info file
//==============================================================================
void CIPHER::CreateInfoFile() {

    SFILE Info(m_tools.dirName + "info.txt");
    if (Info.Open() == 0) {
        std::cout << "Cant create info file!\n";
        return;
    };

    Info.GetFile() << "/*********************/\n";
    Info.GetFile() << "/*        Info       */\n";
    Info.GetFile() << "/*********************/\n";

    Info.GetFile() << "File name: " << m_tools.binFileName << "\n";
    Info.GetFile() << "File len: " << std::dec << GetLen() << "\n";
    Info.GetFile() << "File crc: " << std::hex << GetCrc() << "\n";
    Info.GetFile() << "\n";
    Info.GetFile() << "Segment start addr: " << std::hex
                   << GetSegmentStartAddr() << "\n";

    Info.GetFile() << "Linear start addr: " << std::hex
                   << GetLinearStartAddr() << "\n";
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Convers hex to bin
//==============================================================================
bool_t CIPHER::ConversHexToBin() {
    uint32_t err;

    // Open input file
    if (OpenHexFile(m_tools.dirName + m_tools.hexFileName) == FALSE_T) {
        return 1;
    };

    // Create output file
    if (CreateDstFile() == FALSE_T) {
        return 1;
    };

    // Run conversion
    err = RunConversion();
    if (err != eHEX_OK) {
        PrintError(err);
        return FALSE_T;
    };

    // Copy data to bin file - buffered write
    const std::vector <uint8_t>& data = GetData();
    const std::vector <uint8_t>::size_type len = data.size();
    std::vector <uint8_t>::size_type index = 0;
    int8_t buff[65536];

    while (index < len)
    {
        // Copy data in buff
        const std::vector <uint8_t>::size_type _len = ((len - index) > 65536)
                                                        ? 65536: (len - index);
        for (std::vector <uint8_t>::size_type i = 0; i < _len; i++) {
            buff[i] = data[index++];
        };

        // Write in file
        m_tools.binFile.write((const int8_t *)buff, _len);
    };

    return TRUE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Encript file
//==============================================================================
void CIPHER::Encrypt() {
    if (m_tools.opt != eOPT_CIPHER) {
        return;
    };
}
//==============================================================================
//==============================================================================