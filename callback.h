// TWILIGHT ZONE! <do doo do doo>
static int exitRequest = 0;

int running()
{
	return !exitRequest;
}
/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
          exitRequest=1;
          sceKernelExitGame();
          return 0;
}
// I +think+ that's saying it needs to keep telling the Kernel not to exit
/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
          int cbid;

          cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
          sceKernelRegisterExitCallback(cbid);

          sceKernelSleepThreadCB();

          return 0;
}
// Executing the call exit_callback thread?
/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
          int thid = 0;

          thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
          if(thid >= 0) {
                    sceKernelStartThread(thid, 0, 0);
          }

          return thid;
}
// Starting the exit_callback thread...?
// End of TWILIGHT ZONE! <do doo do doo>

