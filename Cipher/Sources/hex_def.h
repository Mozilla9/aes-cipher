/******************************************************************************/
/* hex_def.h                                                                  */
/******************************************************************************/


#ifndef __HEX_DEF_H
#define __HEX_DEF_H



/********************************** Constants *********************************/
#define HEX_STRING_MIN_SIZE         (11)
#define HEX_ROW_DATA_MAX            (260)


/*************************** Prototypes struct for HEX ************************/
typedef enum
{
    eHEX_OK = 0,
    eHEX_BAD_MARKER,
    eHEX_DATALEN,
    eHEX_PARITY,
    eHEX_CRC,
    eHEX_BAD_REC,
    eHEX_BAD_REC_LEN,
    eHEX_LINEAR_ADDR_LESS_CURR_ADDR,
    eHEX_SEG_ADDR_LESS_CURR_ADDR,
    eHEX_OFFSET_LESS_CURR_OFFSET,
    eHEX_FILE
} __ERROR_HEX;


typedef enum
{
    eHEX_DATA = 0,                      // Data
    eHEX_FILE_END,                      // File end
    eHEX_SEG_ADDR,                      // Segment addr
    eHEX_SEG_ST_ADDR,                   // Segment start addr
    eHEX_LINEAR_ADDR,                   // Linear addr
    eHEX_LINEAR_ST_ADDR,                // Linear start addr
    eHEX_UNDEF_REC                      // Undef rec
} __TYPEREC_HEX;                        // Rec type


#pragma pack(1)
typedef struct
RECLEN_HEADER_HEX
{
    RECLEN_HEADER_HEX() : len(0), offset(0), type(eHEX_UNDEF_REC) {}

    uint8_t  len;                       // Len data\info
    uint16_t offset;                    // Addr offset
    uint8_t  type;                      // Type rec
    uint8_t  rowData[256];              // Data
} __RECLEN_HEADER_HEX;                  // Header hex-string
#pragma pack()


typedef struct
STAT_INFO_HEX
{
    STAT_INFO_HEX() : crc(0), len(0), stringNum(0) {}

    __UNION_DWORD segmStartAddr;        // Segment start addr
    __UNION_DWORD linearStartAddr;      // Linear start addr
    uint32_t      crc;                  // Crc
    uint32_t      len;                  // Data len
    uint32_t      stringNum;            // Need for debug - make msg
} __STAT_INFO_HEX;                      // Info about hex file


typedef struct
{
    std::vector <uint8_t> data;
    std::fstream          hexFile;
} __DATA_HEX;                           // Data for working Hex


typedef struct
PARSING_TOOLS_HEX
{
    PARSING_TOOLS_HEX() : isFileEnd(FALSE_T) {}

    __UNION_DWORD addr;                 // Addr
    bool_t   isFileEnd;                 // Flag end of file
} __PARSING_TOOLS_HEX;                  // Tools for parsing Hex


#endif