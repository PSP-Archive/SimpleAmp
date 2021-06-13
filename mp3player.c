// mp3player.c: MP3 Player Implementation in C for Sony PSP
//
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <math.h>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspiofilemgr.h>
#include <pspdisplay.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <pspaudiolib.h>
#include "mp3player.h"
#include "graphics.h"


#define FALSE 0
#define TRUE !FALSE
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define MadErrorString(x) mad_stream_errorstr(x)
#define INPUT_BUFFER_SIZE       (5*8192)
#define OUTPUT_BUFFER_SIZE      2048    /* Must be an integer multiple of 4. */


static unsigned char simple_spectrum[8];
extern unsigned char simple_spectrum_bands[32];
extern unsigned char simple_spectrum_full[290];
extern unsigned char simple_spectrum_full_sign[290];

static unsigned char   fileBuffer[2048];
static SceUID          file            = 0;
static unsigned int    fileSize        = 1;
static unsigned int    filePos         = 0;
static char            sa_filename[1024];

/* This table represents the subband-domain filter characteristics. It
* is initialized by the ParseArgs() function and is used as
* coefficients against each subband samples when DoFilter is non-nul.
*/
mad_fixed_t Filter[32];

/* DoFilter is non-nul when the Filter table defines a filter bank to
* be applied to the decoded audio subbands.
*/
int DoFilter = 0;
/* Define printf, just to make typing easier */
#define printf  pspDebugScreenPrintf

#define NUMCHANNELS 2
u8 *ptr;
long size;
long samplesInOutput = 0;

//////////////////////////////////////////////////////////////////////
// Global local variables
//////////////////////////////////////////////////////////////////////

//libmad lowlevel stuff

// The following variables contain the music data, ie they don't change value until you load a new file
struct mad_stream Stream;
struct mad_frame Frame;
struct mad_synth Synth;
mad_timer_t Timer;
signed short OutputBuffer[OUTPUT_BUFFER_SIZE];
unsigned char InputBuffer[INPUT_BUFFER_SIZE + MAD_BUFFER_GUARD],
    *OutputPtr = (unsigned char *) OutputBuffer, *GuardPtr = NULL;
const unsigned char *OutputBufferEnd = (unsigned char *) OutputBuffer + OUTPUT_BUFFER_SIZE * 2;
int Status = 0, i;
unsigned long FrameCount = 0;

// The following variables are maintained and updated by the tracker during playback
static int isPlaying;           // Set to true when a mod is being played

//////////////////////////////////////////////////////////////////////
// These are the public functions
//////////////////////////////////////////////////////////////////////
static int myChannel;
static int eos;

/* Define printf, just to make typing easier */
#define printf  pspDebugScreenPrintf

/*void MP3setStubs(codecStubs * stubs)
{
    stubs->init = MP3_Init;
    stubs->load = MP3_Load;
    stubs->play = MP3_Play;
    stubs->pause = MP3_Pause;
    stubs->stop = MP3_Stop;
    stubs->end = MP3_End;
    stubs->time = MP3_GetTimeString;
    stubs->tick = NULL;
    stubs->eos = MP3_EndOfStream;
    memcpy(stubs->extension, "mp3\0" "\0\0\0\0", 2 * 4);
}*/

static int PrintFrameInfo(struct mad_header *Header)
{
    const char *Layer, *Mode, *Emphasis;

    /* Convert the layer number to it's printed representation. */
    switch (Header->layer) {
    case MAD_LAYER_I:
        Layer = "I";
        break;
    case MAD_LAYER_II:
        Layer = "II";
        break;
    case MAD_LAYER_III:
        Layer = "III";
        break;
    default:
        Layer = "(unexpected layer value)";
        break;
    }

    /* Convert the audio mode to it's printed representation. */
    switch (Header->mode) {
    case MAD_MODE_SINGLE_CHANNEL:
        Mode = "single channel";
        break;
    case MAD_MODE_DUAL_CHANNEL:
        Mode = "dual channel";
        break;
    case MAD_MODE_JOINT_STEREO:
        Mode = "joint (MS/intensity) stereo";
        break;
    case MAD_MODE_STEREO:
        Mode = "normal LR stereo";
        break;
    default:
        Mode = "(unexpected mode value)";
        break;
    }

    /* Convert the emphasis to it's printed representation. Note that
     * the MAD_EMPHASIS_RESERVED enumeration value appeared in libmad
     * version 0.15.0b.
     */
    switch (Header->emphasis) {
    case MAD_EMPHASIS_NONE:
        Emphasis = "no";
        break;
    case MAD_EMPHASIS_50_15_US:
        Emphasis = "50/15 us";
        break;
    case MAD_EMPHASIS_CCITT_J_17:
        Emphasis = "CCITT J.17";
        break;
#if (MAD_VERSION_MAJOR>=1) || \
  ((MAD_VERSION_MAJOR==0) && (MAD_VERSION_MINOR>=15))
    case MAD_EMPHASIS_RESERVED:
        Emphasis = "reserved(!)";
        break;
#endif
    default:
        Emphasis = "(unexpected emphasis value)";
        break;
    }
    pspDebugScreenSetXY(0, 29);
//    printf("%lu kb/s audio MPEG layer %s stream at %dHz\n", Header->bitrate / 1000, Layer, Header->samplerate);
    sceDisplayWaitVblankStart();
    return (0);
}



/****************************************************************************
* Converts a sample from libmad's fixed point number format to a signed *
* short (16 bits).                                                                                                                      *
****************************************************************************/
static signed short MadFixedToSshort(mad_fixed_t Fixed)
{
    /* A fixed point number is formed of the following bit pattern:
     *
     * SWWWFFFFFFFFFFFFFFFFFFFFFFFFFFFF
     * MSB                          LSB
     * S ==> Sign (0 is positive, 1 is negative)
     * W ==> Whole part bits
     * F ==> Fractional part bits
     *
     * This pattern contains MAD_F_FRACBITS fractional bits, one
     * should alway use this macro when working on the bits of a fixed
     * point number. It is not guaranteed to be constant over the
     * different platforms supported by libmad.
     *
     * The signed short value is formed, after clipping, by the least
     * significant whole part bit, followed by the 15 most significant
     * fractional part bits. Warning: this is a quick and dirty way to
     * compute the 16-bit number, madplay includes much better
     * algorithms.
     */

    /* Clipping */
    if (Fixed >= MAD_F_ONE)
        return (SHRT_MAX);
    if (Fixed <= -MAD_F_ONE)
        return (-SHRT_MAX);

    /* Conversion. */
    Fixed = Fixed >> (MAD_F_FRACBITS - 15);
    return ((signed short) Fixed);
}

/****************************************************************************
* Applies a frequency-domain filter to audio data in the subband-domain.        *
****************************************************************************/
static void ApplyFilter(struct mad_frame *Frame)
{
    int Channel, Sample, Samples, SubBand,i;

         // Spectrum analizer
         int          freq,band;
         unsigned int freq_value;
         double       x1,x2;
   signed short tmpSample;
   unsigned char  tmpSampleC;
   signed short  bands[32];

   Samples = MAD_NSBSAMPLES(&Frame->header);
   
//// GAIN FILTER 

    mad_fixed_t fixed_gain = (gain_db?mad_f_tofixed(pow(10,gain_db/20)):MAD_F_ONE);

    if (Frame->header.mode != MAD_MODE_SINGLE_CHANNEL)
            for (Channel = 0; Channel < 2; Channel++)
              for (Sample = 0; Sample < Samples; Sample++)
                      for (SubBand = 0; SubBand < 32; SubBand++)
                        Frame->sbsample[Channel][Sample][SubBand] =
                                mad_f_mul(Frame->sbsample[Channel][Sample][SubBand],fixed_gain);
    else
            for (Sample = 0; Sample < Samples; Sample++)
              for (SubBand = 0; SubBand < 32; SubBand++)
                      Frame->sbsample[0][Sample][SubBand] = mad_f_mul(Frame->sbsample[0][Sample][SubBand], fixed_gain);
//////////////////
 
    /* There is two application loops, each optimized for the number
     * of audio channels to process. The first alternative is for
     * two-channel frames, the second is for mono-audio.
     */
     
    /* 
    Samples = MAD_NSBSAMPLES(&Frame->header);
    
    if (Frame->header.mode != MAD_MODE_SINGLE_CHANNEL)
            for (Channel = 0; Channel < 2; Channel++)
              for (Sample = 0; Sample < Samples; Sample++)
                      for (SubBand = 0; SubBand < 32; SubBand++)
                        Frame->sbsample[Channel][Sample][SubBand] =
                                mad_f_mul(Frame->sbsample[Channel][Sample][SubBand], Filter[SubBand]);
    else
            for (Sample = 0; Sample < Samples; Sample++)
              for (SubBand = 0; SubBand < 32; SubBand++)
                      Frame->sbsample[0][Sample][SubBand] = mad_f_mul(Frame->sbsample[0][Sample][SubBand], Filter[SubBand]);
                */      

/*
   for(band = 0; band < 8; band++)
   {
     freq               = (Frame->sbsample[0][0][(band * 4)] + Frame->sbsample[0][0][(band * 4) + 1] + Frame->sbsample[0][0][(band * 4) + 2] + Frame->sbsample[0][0][(band * 4) + 3]) / 4;
     if(freq < 0) freq *= -1;
     freq_value         = (unsigned int) freq;
     x1                 = (double) 0xFFFFFFFF;
     x2                 = (double) freq_value;
     x1                 = (double)((x1/x2)*10.0);
     simple_spectrum[band] = (unsigned char)(((unsigned char)((x1 + 1))) / 25) + 1 ;
     if(simple_spectrum[band] > 10) simple_spectrum[band] = 10;
  }
*/


/*  
  if(eq_delay == 2) 
    for(i = 0; i < 32; i++) bands[i] = 0;
*/
  for( i = 36; i < 290; i++) 
  {
    simple_spectrum_full[i - 36] = simple_spectrum_full[i];
    simple_spectrum_full_sign[i - 36] = simple_spectrum_full_sign[i];
  }  

  for(Sample = 0; Sample < Samples; Sample++)
  {
    tmpSample = 0;
    
    for(SubBand = 0; SubBand < 32; SubBand++)
    {

      tmpSample += MadFixedToSshort(Frame->sbsample[0][Sample][SubBand]);
      
      bands[SubBand] += MadFixedToSshort(Frame->sbsample[0][Sample][SubBand]);
    }
    
    if(tmpSample < 0) 
    {
      tmpSample *= -1;
      simple_spectrum_full_sign[(289 - 36)+Sample] = 0;
    }
    else
      simple_spectrum_full_sign[(289 - 36)+Sample] = 1;
      
      
    tmpSampleC = (unsigned  char) (( tmpSample >> 8 ) & 0xFF);

    if(tmpSampleC > 10) tmpSampleC = (unsigned  char) (( tmpSample >> 10 ) & 0xFF);

    if(tmpSampleC > 20) tmpSampleC = 20;
            
    simple_spectrum_full[(289 - 36) + Sample] = tmpSampleC;
    
    
  }

/*
  if(eq_delay == 2)
  {  
    for(i = 0 ; i < 32; i++)
    {
      if(bands[i] < 0)
        bands[i] *= -1; 
      
      bands[i] *= 50;
       
      simple_spectrum_bands[i] = (unsigned char)(( bands[i] >> 8) & 0xFF);
      if(simple_spectrum_bands[i] > 40)  simple_spectrum_bands[i] = 40;    
    }
    
    eq_delay = 0;
  }
  
  eq_delay++;
*/
}


static void fillFileBuffer()
{
//  if (isPlaying == TRUE) {
   if (file <= 0)
   {
      char   fileName[1024];
      
      sprintf(fileName,"%s",sa_filename);

      file = sceIoOpen(fileName, PSP_O_RDONLY, 777);
      if (file <= 0)
         return;

      fileSize = sceIoLseek(file, 0, SEEK_END);
      
      sceIoLseek(file, 0, SEEK_SET);
      
      // SKIP HEADER
      unsigned char id3_header[0x0A];
      
      sceIoRead(file , id3_header , 0x0A);
      
      if( (id3_header[0] == 'I') &&
          (id3_header[1] == 'D') &&
          (id3_header[2] == '3'))
      {
        unsigned int skip_size;
        
        skip_size = (id3_header[9] & 0x7F) | ((id3_header[8] & 0x7F) << 7 ) | ((id3_header[7] & 0x7F) << 14 ) | ((id3_header[6] & 0x7F) << 21 );

        sceIoLseek(file, (skip_size + 0x0A), SEEK_SET);
      }
   }

   const unsigned int   bytesToKeep   = Stream.bufend - Stream.next_frame;
   unsigned int         bytesToFill   = sizeof(fileBuffer) - bytesToKeep;

   // Want to keep any bytes?
   if (bytesToKeep)
   {
      memmove(fileBuffer, fileBuffer + sizeof(fileBuffer) - bytesToKeep,bytesToKeep);
   }

   // Read into the rest of the file buffer.
   unsigned char* bufferPos = fileBuffer + bytesToKeep;
   while (bytesToFill > 0)
   {
      // Read some.
      /*const*/ unsigned int bytesRead = sceIoRead(file, bufferPos,bytesToFill);

      if (bytesRead == 0)
      {
         sceIoLseek(file, 0, SEEK_SET);
         filePos = 0;
         mad_timer_reset(&Timer);
         
         eos = 1;
         //isPlaying = FALSE;
         
        // SKIP HEADER
        unsigned char id3_header[0x0A];
        
        sceIoRead(file , id3_header , 0x0A);
        
        if( (id3_header[0] == 'I') &&
            (id3_header[1] == 'D') &&
            (id3_header[2] == '3'))
        {
          unsigned int skip_size;
          
          skip_size = (id3_header[9] & 0x7F) | ((id3_header[8] & 0x7F) << 7 ) | ((id3_header[7] & 0x7F) << 14 ) | ((id3_header[6] & 0x7F) << 21 );
  
          sceIoLseek(file, (skip_size + 0x0A), SEEK_SET);
        }

        bytesRead = sceIoRead(file, bufferPos,bytesToFill);
        // continue;
      }

      // Adjust where we're writing to.
      bytesToFill -= bytesRead;
      bufferPos += bytesRead;
      filePos += bytesRead;

//      pspDebugScreenPrintf("Read %u bytes from the file, %u left to fill.\n", bytesRead, bytesToFill);
//      pspDebugScreenPrintf("%u%%.\n", filePos * 100 / fileSize);
   }
   
//  }
}




static void MP3Callback(void *_buf2, unsigned int numSamples, void *pdata)
{
  short *_buf = (short *)_buf2;
  
  unsigned long samplesOut = 0;

  if (isPlaying == TRUE) {
    
        if (samplesInOutput > 0) {

            if (samplesInOutput > numSamples) {

                    memcpy((char *) _buf, (char *) OutputBuffer, numSamples * 2 * 2);
                    samplesOut = numSamples;
                    samplesInOutput -= numSamples;

            } else {

                    memcpy((char *) _buf, (char *) OutputBuffer, samplesInOutput * 2 * 2);
                    samplesOut = samplesInOutput;
                    samplesInOutput = 0;

            }
        }

        while (samplesOut < numSamples) {
//              if (Stream.buffer == NULL || Stream.error == MAD_ERROR_BUFLEN) {
//
//                      mad_stream_buffer(&Stream, ptr, size);
//                      Stream.error = 0;
//              }

//          if (mad_frame_decode(&Frame, &Stream)) {
//                  if (MAD_RECOVERABLE(Stream.error)) {
//              
//                      if (Stream.error != MAD_ERROR_LOSTSYNC || Stream.this_frame != GuardPtr) {
//                              printf("recoverable frame level error (%s)\n", MadErrorString(&Stream));
//                              sceDisplayWaitVblankStart();
//                      }
//                  
//                      return; //continue;
//              
//                    } else if (Stream.error == MAD_ERROR_BUFLEN) {
//                  
//                      eos = 1;
//                      return; //continue;
//              
//                    } else {
//                  
//                      printf("unrecoverable frame level error (%s).\n", MadErrorString(&Stream));
//              
//                      sceDisplayWaitVblankStart();
//                      Status = 1;
//                      MP3_Stop();     //break;
//              
//                    }
//            }
//
//          if (FrameCount == 0)
//                  if (PrintFrameInfo(&Frame.header)) {
//                    Status = 1;
//                    //break;
//                  }

////////// HUBEVOLUTION FRAME BUFFERING //////////

         while ( (mad_frame_decode(&Frame, &Stream) == -1) ) //&&
//                 ((Stream.error == MAD_ERROR_BUFLEN) || (Stream.error == MAD_ERROR_BUFPTR)) )
         {
            // Fill up the remainder of the file buffer.
            fillFileBuffer();
            
            if(eos == 1) {
              
              //return;
            }
            
            // Give new buffer to the stream.
            mad_stream_buffer(&Stream, fileBuffer, sizeof(fileBuffer));
         }

////////////////////////////////////////////////////

              FrameCount++;
              mad_timer_add(&Timer, Frame.header.duration);

              ApplyFilter(&Frame);

              mad_synth_frame(&Synth, &Frame);

              for (i = 0; i < Synth.pcm.length; i++) {
                    
                      signed short Sample;

                      if (samplesOut < numSamples) {

                        Sample = MadFixedToSshort(Synth.pcm.samples[0][i]);
                        _buf[samplesOut * 2] = Sample;

                        if (MAD_NCHANNELS(&Frame.header) == 2)
                                Sample = MadFixedToSshort(Synth.pcm.samples[1][i]);

                        _buf[samplesOut * 2 + 1] = Sample;
                        samplesOut++;
                  
                      } else {

                        Sample = MadFixedToSshort(Synth.pcm.samples[0][i]);
                        OutputBuffer[samplesInOutput * 2] = Sample;

                        if (MAD_NCHANNELS(&Frame.header) == 2)
                                Sample = MadFixedToSshort(Synth.pcm.samples[1][i]);
                    
                        OutputBuffer[samplesInOutput * 2 + 1] = Sample;
                        samplesInOutput++;
                      }
              }
            }
            
  } else {                      //  Not Playing , so clear buffer
      
            int count;
          
            for (count = 0; count < numSamples * 2; count++) *(_buf + count) = 0;
    
  }
}



void MP3_Init(int channel)
{
    int i;
  
    gain_db = 0.0;
    myChannel = channel;
    isPlaying = FALSE;
    pspAudioSetChannelCallback(myChannel, MP3Callback,0);
    /* First the structures used by libmad must be initialized. */
    mad_stream_init(&Stream);
    mad_frame_init(&Frame);
    mad_synth_init(&Synth);
    mad_timer_reset(&Timer);
    //ModPlay_Load("",data);
    
    for( i = 0; i < 290; i++) { simple_spectrum_full[i] = 0; simple_spectrum_full_sign[i] = 1; }

    file = -1;
}


void MP3_FreeTune()
{
    /* The input file was completely read; the memory allocated by our
     * reading module must be reclaimed.
     */
    if (ptr)
        free(ptr);
    //sceIoClose(BstdFile->fd);
    //BstdFileDestroy(BstdFile);

    /* Mad is no longer used, the structures that were initialized must
     * now be cleared.
     */
    mad_synth_finish(&Synth);
    mad_frame_finish(&Frame);
    mad_stream_finish(&Stream);

    /* If the output buffer is not empty and no error occurred during
     * the last write, then flush it.
     */
    /*if(OutputPtr!=OutputBuffer && Status!=2)
       {
       size_t       BufferSize=OutputPtr-OutputBuffer;

       if(fwrite(OutputBuffer,1,BufferSize,OutputFp)!=BufferSize)
       {
       fprintf(stderr,"%s: PCM write error (%s).\n",
       ProgName,strerror(errno));
       Status=2;
       }
       } */

    /* Accounting report if no error occurred. */
    if (!Status) {
        char Buffer[80];

        /* The duration timer is converted to a human readable string
         * with the versatile, but still constrained mad_timer_string()
         * function, in a fashion not unlike strftime(). The main
         * difference is that the timer is broken into several
         * values according some of it's arguments. The units and
         * fracunits arguments specify the intended conversion to be
         * executed.
         *
         * The conversion unit (MAD_UNIT_MINUTES in our example) also
         * specify the order and kind of conversion specifications
         * that can be used in the format string.
         *
         * It is best to examine libmad's timer.c source-code for details
         * of the available units, fraction of units, their meanings,
         * the format arguments, etc.
         */
        mad_timer_string(Timer, Buffer, "%lu:%02lu.%03u", MAD_UNITS_MINUTES, MAD_UNITS_MILLISECONDS, 0);
//      printf("%lu frames decoded (%s).\n", FrameCount, Buffer);
        sceDisplayWaitVblankStart();
        sceKernelDelayThread(500000);
    }
    

}


void MP3_End()
{
    MP3_Stop();
    pspAudioSetChannelCallback(myChannel, 0,0);
    MP3_FreeTune();
}

//////////////////////////////////////////////////////////////////////
// Functions - Local and not public
//////////////////////////////////////////////////////////////////////

//  This is the initialiser and module loader
//  This is a general call, which loads the module from the 
//  given address into the modplayer
//
//  It basically loads into an internal format, so once this function
//  has returned the buffer at 'data' will not be needed again.
int MP3_Load(char *filename)
{
// 
//   int fd;
//   eos = 0;
//   //psp_stats pstat;
//   //sceIoGetstat(filename, &pstat);
//   if ((fd = sceIoOpen(filename, PSP_O_RDONLY, 0777)) > 0) {
//      //  opened file, so get size now
//      size = sceIoLseek(fd, 0, PSP_SEEK_END);
//      sceIoLseek(fd, 0, PSP_SEEK_SET);
//      ptr = (unsigned char *) malloc(size + 8);
//      memset(ptr, 0, size + 8);
//      if (ptr != 0) {         // Read file in
//          sceIoRead(fd, ptr, size);
//      } else {
//          printf("Error allocing\n");
//          sceIoClose(fd);
//          return 0;
//      }
//      // Close file
//      sceIoClose(fd);
//    } else {
//      return 0;
//    }
    //  Set volume to full ready to play
    //SetMasterVolume(64);
 
    sprintf(sa_filename,"%s",filename);
    
    if(file > 0) {
      sceIoClose(file);
      file = -1;
    }
    
    eos = 0;
    
    fillFileBuffer();
 
    isPlaying = FALSE;
    return 1;
}

// This function initialises for playing, and starts
int MP3_Play()
{
    // See if I'm already playing
    if (isPlaying)
        return FALSE;

    isPlaying = TRUE;
    return TRUE;
}

void MP3_Pause()
{
    if(file != -1)
      isPlaying = !isPlaying;
}

int MP3_Stop()
{
    //stop playing
    isPlaying = FALSE;

    //clear buffer
    memset(OutputBuffer, 0, OUTPUT_BUFFER_SIZE);
    OutputPtr = (unsigned char *) OutputBuffer;

    //seek to beginning of file
    //sceIoLseek(BstdFile->fd, 0, SEEK_SET);

    sceIoClose(file);
    file = -1;

    return TRUE;
}

void MP3_GetTimeString(char *dest)
{
    mad_timer_string(Timer, dest, "%02lu:%02u:%02u", MAD_UNITS_HOURS, MAD_UNITS_MILLISECONDS, 0);
}

int MP3_EndOfStream()
{
    if (eos == 1)
        return 1;
    return 0;
}

int MP3_SimpleSpectrum(int x)
{ 
  return simple_spectrum[x]; 
}

unsigned char MP3_SimpleSpectrumFULL(int x)
{
  return simple_spectrum_full[x]; 
}

void MP3_Seek(int step)
{
  unsigned int  x;
  
  if((file > 0) && (isPlaying))
  { 
    //isPlaying = FALSE;
    
    x = sceIoLseek(file, 0, SEEK_CUR);        
 
    x = (x / 2048);
    x = x * 2048;
    
    if(x > 0)
    {
      
      if(step < 0)
      {
        step *= -1;
                
        if((x - step) > 0)
        {
          mad_timer_t tmp_timer;
          
          tmp_timer.seconds  = (Frame.header.duration.seconds * (step / (Stream.next_frame - Stream.this_frame))  );
          
          sceIoLseek(file, (x - step) , SEEK_SET);  
          
          mad_timer_negate(&tmp_timer);
          mad_timer_add(&Timer, tmp_timer );
        }
        else
        {
          sceIoLseek(file, 0 , SEEK_SET);
          mad_timer_reset(&Timer);
        }
      }
      else         
      { 
        if(( x + step) > fileSize)
        {
          sceIoLseek(file, (fileSize - 2048), SEEK_CUR);
          mad_timer_reset(&Timer);      
        }
        else
        {
          mad_timer_t tmp_timer;
 
          tmp_timer.seconds  = (Frame.header.duration.seconds * (step / (Stream.next_frame - Stream.this_frame))  );
          
          sceIoLseek(file, step, SEEK_CUR);        
          mad_timer_add(&Timer, tmp_timer );
        }
      }      
      
    }

    //isPlaying = TRUE;
  }
  

}

void MP3_SetGain(double db) 
{ 
  gain_db = db ;

}

double MP3_GetGain()
{
  return gain_db; 
}

unsigned int MP3_GetFileSize()
{
 return fileSize;
}

unsigned int MP3_GetBytesPlayed()
{
	unsigned int x;
	x = sceIoLseek(file, 0, SEEK_CUR);
	return x;
}

unsigned int MP3_GetFractionPlayed()
{
	unsigned int x;
	x = sceIoLseek(file, 0, SEEK_CUR);
	x = (x / fileSize);
	return x;
}

unsigned int MP3_GetPercentagePlayed()
{
	unsigned int x;
	//x = fileSize;
	x = sceIoLseek(file, 0, SEEK_CUR);
	x = (x * 100);
	x = (x / fileSize);
	return x;
}