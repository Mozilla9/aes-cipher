/******************************************************************************/
/*  PROJECT "Firmware cipher"                                                 */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     2.0                                                         */
/*  Date:         2011/02/02 22:37:33                                         */
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
    if (Cipher.GetSettFromCmdLine(argc - 1, (const int8_t **)(argv + 1)) == FALSE_T) {
        Cipher.PrintHelp(argv[0]);
        return 1;
    };

    // Open input file
    if (Cipher.OpenSourceFile() == FALSE_T) {
        return 1;
    };

    // Create output file
    if (Cipher.CreateDstFile() == FALSE_T) {
        return 1;
    };

    // Main loop
    while(1)
    {

    };

    return 0;
}
/******************************************************************************/
/*                                                                            */
/*      The End                                                               */
/*                                                                            */
/******************************************************************************/