#include <alsa/asoundlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PCM_DEVICE "default"

int main()
{
  /* Init pcm_handle // AudioContext */ 
  snd_pcm_t *pcm_handle;
  unsigned int pcm = -1;
  unsigned int seconds = 5;
  unsigned int channels = 2;
  unsigned int rate = 44100;
  unsigned int samples = rate * channels * seconds;

  /* Init && allocate hw_params */
  snd_pcm_hw_params_t *params;  
  
  /* Open the PCM device */
  if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0) < 0) 
    printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(pcm));

  /* PCM set parameters : 
     access - format - rate - channels - periods?
     // AudioContext.parameters
  */

  /* Allocate parameters object and fill it with default values*/
  snd_pcm_hw_params_alloca(&params);
 
  snd_pcm_hw_params_any(pcm_handle, params);

  if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
					 SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
    printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
					 SND_PCM_FORMAT_S16_LE) < 0) 
    printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) 
    printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) 
    printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

  /* Write parameters */
  if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
    printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

  /* 
     Write PCM buffer / Decoded buffer 
     // Init the SAB and parse the buffer to copy it to outputs
     in the Audio Worklet Processor
   */

  int fd_src = open("./output.pcm", O_RDWR);
  
  // s32le:
  unsigned int size = 2 * samples; // (size of 1 sample depending on the format => here 16 bits/2 bytes * frames)

  char *src = malloc(size);

  int ret = read (fd_src, src, size);

  if (ret == -1)
  {
    free (src);
    return 1;
  }
  size_t frames = 44100 * 5; 
  // printf("%d", sizeof(src));
  size_t chunks = frames / 128;
  for (size_t i = 0; i < chunks; i += 128)
  {
    snd_pcm_writei(pcm_handle, src, 128);
    src = &(*(src + (128 * 2)));
  }
  /* Cleanup */
  snd_pcm_drain(pcm_handle);
  snd_pcm_close(pcm_handle);
  free (src);
  return 0;
}

// int fd_dst = open("./test.pcm", O_RDWR);
// f32le: 
/* size_t size = sizeof(float) * samples; */
