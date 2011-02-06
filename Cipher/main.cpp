/******************************************************************************/
/*  PROJECT "Firmware cipher"                                                 */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     2.1                                                         */
/*  Date:         2011/02/06 21:49:33                                         */
/******************************************************************************/


#include "stdafx.h"
using namespace std;



static CIPHER Cipher;



/******************************************************************************/
/*                                                                            */
/*      main                                                                  */
/*                                                                            */
/******************************************************************************/
int32_t _tmain(int32_t argc, int8_t ** argv, int8_t ** envp)
{
    // Get settings
    if (Cipher.GetSettFromCmdLine(argc, (const int8_t **)argv) == FALSE_T) {
        Cipher.PrintHelp(argv[0]);
        return 1;
    };

    // Run conversion
    if (Cipher.ConversHexToBin() == FALSE_T) {
        return 1;
    };

    // Create info file
    Cipher.CreateInfoFile();

    return 0;
}
/******************************************************************************/
/*                                                                            */
/*      The End                                                               */
/*                                                                            */
/******************************************************************************/