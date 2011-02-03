/******************************************************************************/
/*  Company "3SD" 2011                                                        */
/*  PROJECT "Firmware cipher for SmartBUG"                                    */
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
int _tmain(int argc, char * argv[], char * envp[])
{
    // Get settings
    if (Cipher.GetSettFromCmdLine(argc, (const char **)argv) == FALSE_T) {
        Cipher.PrintHelp();
        return 1;
    };

    // Open files
    if (Cipher.OpenSourceFile() == FALSE_T) {
        Cipher.PrintHelp();
        return 1;
    };

    if (Cipher.CreateDstFile() == FALSE_T) {
        Cipher.PrintHelp();
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