#include "audio_gen.h"
#include "PDM.h"

int16_t sampleBuffer[DEFAULT_PDM_BUFFER_SIZE * 16];
int16_t sampleOutput[512];
volatile int samplesRead;
volatile int32_t timestamp = 0;

void onPDMdata(){
  int bytesAvailable = PDM.available();

  samplesRead = bytesAvailable / 2;
  // 256 samples read each time; 16 samples read per ms; 
  const int32_t time_in_ms = timestamp + (samplesRead/(16000/1000));

  const int32_t sample_offset = timestamp * (16000/1000);

  const int index = sample_offset % (DEFAULT_PDM_BUFFER_SIZE * 16);
  
  PDM.read(sampleBuffer+index, bytesAvailable);

  timestamp = time_in_ms;
}

void SetupRecording(){
  PDM.onReceive(onPDMdata);

  PDM.begin(1, 16000);

  PDM.setGain(20);
}

void GetAudioSamples(int start, int duration, int* sample_size, int16_t** sample){
  const int start_offset = start * (16000/1000);

  const int num_of_samples = duration * (16000/1000);

  for(int i=0; i<num_of_samples; i++){
    const int index = (start_offset+i) % 512;
    sampleOutput[i] = sampleBuffer[index];
  }

  *sample_size = 512;
  *sample = sampleOutput;

}
