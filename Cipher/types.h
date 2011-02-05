/******************************************************************************/
/*  data_types.h                                                              */
/******************************************************************************/

#ifndef __TYPES_H
#define __TYPES_H



/***************************** Overriding types *******************************/
typedef char               int8_t;
typedef unsigned char      uint8_t;
typedef signed char        sint8_t;
typedef bool               bool_t;
typedef unsigned short     uint16_t;
typedef signed short       sint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;
typedef signed int         sint32_t;
typedef unsigned long      ulong32_t;
typedef signed long        slong32_t;
typedef unsigned long long uint64_t;
typedef signed long long   sint64_t;
typedef float              float32_t;
typedef double             float64_t;
#define TRUE_T             true
#define FALSE_T            false


typedef union
UNION_DWORD
{
    UNION_DWORD() : data32(0) {}

    uint32_t  data32;
    sint32_t  sdata32;
    float32_t dataF;
    uint16_t  data16[2];
    uint8_t   data8[4];
} __UNION_DWORD;


#endif