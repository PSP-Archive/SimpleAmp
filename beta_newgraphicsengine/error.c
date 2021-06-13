#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdio.h>
#include "error.h"

void errorThrow(char* inMessage) {
     SceCtrlData inputPad;
     int inputLock = 25;
     while(1) {
          pspDebugScreenClear ();
          
          pspDebugScreenPrintf("ERROR!\n\n");
          pspDebugScreenPrintf(inMessage);
          pspDebugScreenPrintf("\n\nPress [X] to continue or [O] to quit.\n");
          
          if(inputLock > 0) {
               inputLock--;
          } else {
               sceCtrlReadBufferPositive(&inputPad, 1);
               if(inputPad.Buttons & PSP_CTRL_CROSS) {
                    break;
               } else if(inputPad.Buttons & PSP_CTRL_CIRCLE) {
                    //terminate();
                    sceKernelExitGame();
               }
          }     
     }
}

