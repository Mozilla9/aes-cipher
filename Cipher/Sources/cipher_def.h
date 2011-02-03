/******************************************************************************/
/* cipher_def.h                                                               */
/******************************************************************************/


#ifndef __CIPHER_DEF_H
#define __CIPHER_DEF_H


/********************************** Constants *********************************/


/************************** Struct and macros *********************************/
typedef enum
{
   eOPT_CIPHER = 0,
   eOPT_BIN,
   eOPT_UNDEF
} __OPT_CIPHER;


typedef struct
TOOLS_CIPHER
{
    TOOLS_CIPHER() : opt(eOPT_UNDEF) {}

    uint32_t opt;
    std::vector <uint8_t> binData;
    std::fstream hexFile;
    std::fstream binFile;
    std::string hexFileName;
    std::string binFileName;
} __TOOLS_CIPHER;




#endif