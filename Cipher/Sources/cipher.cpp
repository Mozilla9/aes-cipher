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
// App: Get settings from cmd line
//==============================================================================
bool_t CIPHER::GetSettFromCmdLine(const int32_t _argc, const char ** _argv) {

    // Test
    if(_argc != 4) {
        return FALSE_T;
    };

    // Source file name - *.hex
    m_tools.hexFileName = _argv[1];
    if (m_tools.hexFileName.empty() == TRUE_T) {
        return FALSE_T;
    };

    // Dst file name
    m_tools.binFileName = _argv[2];
    if (m_tools.binFileName.empty() == TRUE_T) {
        return FALSE_T;
    };

    // Option
    switch (_argv[2][1])
    {
       case 'c':
         m_tools.opt = eOPT_CIPHER;
       break;

       case 'b':
         m_tools.opt = eOPT_BIN;
       break;

       default:
         m_tools.opt = eOPT_UNDEF;
       break;
    };

    if (m_tools.opt == eOPT_UNDEF) {
        return FALSE_T;
    };

    return FALSE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Print help
//==============================================================================
void CIPHER::PrintHelp() {
    std::cout << "[src file *.hex] [output file name *.bin] -[option]\r\n"
              << "option: 'c'- ciphering output file\r\n"
              << "        'b'- row bin file"
              << std::endl;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Open source file
//==============================================================================
bool_t CIPHER::OpenSourceFile() {
    m_tools.hexFile.open(m_tools.hexFileName.c_str(), std::ios::in);

    return (m_tools.hexFile.is_open()) ? TRUE_T : FALSE_T;
}
//==============================================================================
//==============================================================================


//==============================================================================
// App: Create dst file
//==============================================================================
bool_t CIPHER::CreateDstFile() {
    m_tools.binFile.open(m_tools.hexFileName.c_str(), fstream::in | fstream::out | fstream::trunc);

    return (m_tools.binFile.is_open()) ? TRUE_T : FALSE_T;
}
//==============================================================================
//==============================================================================