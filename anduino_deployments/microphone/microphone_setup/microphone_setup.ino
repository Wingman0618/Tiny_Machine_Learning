//Reference: https://docs.arduino.cc/tutorials/nano-33-ble-sense/microphone-sensor/

#include <PDM.h>
#include <TinyMLShield.h>

int16_t sampleBuffer[512*16];

volatile int samplesRead;
volatile int32_t audio_timestamp = 0;
int32_t time_in_ms = 0;


bool record = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  initializeShield();

  PDM.onReceive(onPDMdata);

  PDM.setGain(20);

  if(!PDM.begin(1, 16000)){
    Serial.println("Failed to start PDM!");
    while(1);
  }

  Serial.println("Press the button to start record the sound");
}

void loop() {
  // put your main code here, to run repeatedly
  bool clicked = readShieldButton();
  if (clicked){
    record = !record;
  }
  
  if(samplesRead){
    if(record){
      digitalWrite(LEDG, LOW);
      for(int i=0; i<samplesRead; i++){
        Serial.println(sampleBuffer[i]);
      }      
    }

    digitalWrite(LEDG, HIGH);
    samplesRead = 0;
    
  }  

}

void onPDMdata(){
  int bytesAvailable = PDM.available();
  // 256 samples read each times
  samplesRead = bytesAvailable / 2;
  // 16 samples per millisecond; 
  time_in_ms = audio_timestamp+(samplesRead / (16000/1000));

  PDM.read(sampleBuffer, bytesAvailable);

  audio_timestamp = time_in_ms;
}
