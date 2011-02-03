/******************************************************************************/
/* hex_def.h                                                                  */
/******************************************************************************/


#ifndef __HEX_DEF_H
#define __HEX_DEF_H



/********************************** Constants *********************************/
#define DATA_BUFF_LEN           (2048)
#define MAXLEN_HEX_STRING       (523)               // Maxlen row string hex-file
#define ENCRYPT_BOUNDARY        (DATA_BUFF_LEN/2)


/*************************** Prototypes struct for HEX ************************/
typedef enum
{
    eHEX_DATA = 0,                      // Data
    eHEX_FILE_END,                      // File end
    eHEX_SEG_ADDR,                      // Segment addr
    eHEX_SEG_ST_ADDR,                   // Segment start addr
    eHEX_LINE_ADDR,                     // Line addr
    eHEX_LINE_ST_ADDR,                  // Line start addr
    eHEX_UNDEF_REC                      // Undef rec
} __TYPEREC_HEX;                        // Rec type


typedef struct
{
    uint8_t Reclen;                     // Len data\info
    uint16_t Offset;                    // Addr offset
    __TYPEREC_HEX TypeRec;              // Type rec
    uint8_t RowData[256];               // Data
} __RECLEN_HEADER_HEX;                  // Header hex-string


typedef struct
{
    uint8_t Data[DATA_BUFF_LEN];        // Буфер для данных
    uint32_t DataLen;                   // Количество данных в буфере
} __BUFF_TOOLS_HEX;                     // Тип своповый буфер данных


typedef struct
{
    bool_t FlgFileEnd;                  // Флаг - файл закончен
} __FLG_HEX;                            // Тип флаги hex-файла


typedef struct
{
    uint32_t DataCount;                 // Счетчик данных
    uint32_t FwCrc;                     // Контрольная сумма fw
    uint32_t CurrAddr;                  // Адрес сегмента hex-файла
} __PARSING_TOOLS_HEX;                  // Тип инструменты для парсинга


typedef struct
{
    __aes_context ctx_enc;
    uint8_t iv[16];
} __ENCRYPT_TOOLS_HEX;                  // Инструменты для шифрования


#endif