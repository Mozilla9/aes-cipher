/******************************************************************************/
/*  Class CIPHER                                                              */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     1.1                                                         */
/*  Date:         2011/02/06 21:49:33                                         */
/******************************************************************************/


#ifndef __CIPHER_H
#define __CIPHER_H


#include "cipher_def.h"


//==============================================================================
//  Class CIPHER
//==============================================================================
class CIPHER : public HEXFILE
{
public:
    virtual ~CIPHER() {
        if (m_tools.binFile.is_open()) {
            m_tools.binFile.close();
        };
    }
    void PrintHelp(const std::string _self);
    void CreateInfoFile();
    bool_t GetSettFromCmdLine(const int32_t _argc, const int8_t ** _argv);
    bool_t ConversHexToBin();

private:
    void CopyRowData();
    void EncryptRowData();
    void GetDir(const int8_t * const _dir);
    bool_t RemoveBlank(std::string& _str);
    bool_t ExtractKey(std::string& _hexStr, uint8_t * _dstArray);
    bool_t CreateDstFile();

    __TOOLS_CIPHER m_tools;
};
//==============================================================================
//==============================================================================


#endif