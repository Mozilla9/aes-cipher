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
bool_t CIPHER::RemoveBlank(std::string &_str) {

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
bool_t CIPHER::ExtractKey(std::string &_hexStr, std::vector <uint8_t> &_dstArray) {

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
// App: Open source file
//==============================================================================
bool_t CIPHER::OpenSourceFile() {
    m_tools.hexFile.open(m_tools.hexFileName.c_str(), std::ios::in);

    std::cout << "\nOpen hex file: " << (m_tools.hexFile.is_open())
               ? "ok\n" : "fail\n";
    return (m_tools.hexFile.is_open()) ? TRUE_T : FALSE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Create dst file
//==============================================================================
bool_t CIPHER::CreateDstFile() {
    m_tools.binFile.open(m_tools.hexFileName.c_str(), fstream::in
                                                     |fstream::out
                                                     |fstream::trunc);

    std::cout << "\nCreate bin file: " << (m_tools.binFile.is_open())
               ? "ok\n" : "fail\n";
    return (m_tools.binFile.is_open()) ? TRUE_T : FALSE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Get settings from cmd line
//==============================================================================
bool_t CIPHER::GetSettFromCmdLine(const int32_t _argc, const int8_t ** _argv) {

    std::string str;

    // Parsing parameters
    if(_argc < 3) {
        return FALSE_T;
    }

    // Option
    str = _argv[0];

    if ((str == "-?") || (str == "--help")) {
        return FALSE_T;
    }
    else if ((str == "-e") || (str == "--encrypt")) {
        if (_argc == 5) {
            m_tools.opt = eOPT_CIPHER;
        };
    }
    else if ((str == "-b") || (str == "--bin")) {
        if (_argc == 3) {
            m_tools.opt = eOPT_BIN;
        };
    };

    if (m_tools.opt == eOPT_UNDEF) {
        std::cout << "\nBad parameters!\n";
        return FALSE_T;
    };

    // Input hex file name
    str = _argv[1];
    if (RemoveBlank(str) == FALSE_T) {
        std::cout << "\nBad input file name!\n";
        return FALSE_T;
    };

    // Output file name
    str = _argv[2];
    if (RemoveBlank(str) == FALSE_T) {
        std::cout << "\nBad output file name!\n";
        return FALSE_T;
    };

    // Option
    switch (m_tools.opt)
    {
       //-----------------------------------------------------------------------
       case eOPT_CIPHER:
         // Extract key
         str = _argv[3];
         if (ExtractKey(str, m_tools.key) == FALSE_T) {
            std::cout << "\nBad key!\n";
            return FALSE_T;
         };

         // Extract vector
         str = _argv[3];
         if (ExtractKey(str, m_tools.vector) == FALSE_T) {
            std::cout << "\nBad vector!\n";
            return FALSE_T;
         };
       break;

       //-----------------------------------------------------------------------
       case eOPT_BIN:
       break;

       //-----------------------------------------------------------------------
       default:
         std::cout << "\nUndefined error!\n";
         return FALSE_T;
    };

    return TRUE_T;
}
//==============================================================================
//==============================================================================