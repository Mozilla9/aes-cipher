/******************************************************************************/
/* cipher_def.h                                                               */
/******************************************************************************/


#ifndef __CIPHER_DEF_H
#define __CIPHER_DEF_H


/******************************** Constants ***********************************/


/****************************** Struct and macros *****************************/
typedef enum
{
   eOPT_CIPHER = 0,
   eOPT_BIN,
   eOPT_UNDEF
} __OPT_CIPHER;


typedef struct
TOOLS_CIPHER
{
    TOOLS_CIPHER() : opt(eOPT_UNDEF), dirName(""),
                     hexFileName(""), binFileName("") {}

    uint32_t opt;
    uint8_t key[16];
    uint8_t vector[16];
    std::fstream binFile;
    std::string dirName;
    std::string hexFileName;
    std::string binFileName;
} __TOOLS_CIPHER;



#endif