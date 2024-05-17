#include <TensorFlowLite.h>

#include "PDM.h"
#include "libmfcc.h"

#include "Ml_model.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Audio samples
int16_t sampleBuffer[DEFAULT_PDM_BUFFER_SIZE];
std::vector<float> input_sampleBuffer(DEFAULT_PDM_BUFFER_SIZE * 20);
int counter = 0;
volatile int samplesRead;
// fake input
std::vector<std::vector<std::vector<float>>> mfcc;

namespace{
  //Tensorflow namespace
  tflite::ErrorReporter *error_reporter = nullptr;
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *model_input = nullptr;

  constexpr int kTensorArenaSize = 10 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
  int8_t *model_input_buffer = nullptr;
}



void setup() {
  // Setup tensorflow lite micro
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(g_model);
  if(model->version() != TFLITE_SCHEMA_VERSION){
      TF_LITE_REPORT_ERROR(error_reporter,
      "Schema version %d is not supported version %d.", 
      model->version(), TFLITE_SCHEMA_VERSION);
      return;
  }

  static tflite::MicroMutableOpResolver<6> micro_op_resolver(error_reporter);
  if(micro_op_resolver.AddConv2D() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddShape() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddStridedSlice() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddPack() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddFullyConnected() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddReshape() != kTfLiteOk){
    return;
  }

  static tflite::MicroInterpreter static_interpreter(
    model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if(allocate_status != kTfLiteOk){
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  model_input = interpreter->input(0);

  model_input_buffer = model_input->data.int8;

  // Setup microphone
  //Reference: https://docs.arduino.cc/learn/built-in-libraries/pdm/
  PDM.onReceive(onPDMdata);
  PDM.begin(1, 16000);
  PDM.setGain(20);
}

void loop() {
  if(samplesRead){
    digitalWrite(LEDG, LOW);
    for(int i=0; i<samplesRead; i++){
      Serial.print(sampleBuffer[i]);
      Serial.print(" ");
    }
    Serial.print("\n");
/*  Checking the copied sampleBuffer
    for(int i=0; i<samplesRead; i++){
      Serial.print(input_sampleBuffer[counter - 256 + i]);
      Serial.print(" ");
    }
    Serial.print("\n");
*/
    samplesRead = 0;
  }

  Serial.print("\n");
  Serial.println("one loop");
  delay(1000);
  
}

void onPDMdata(){
  int bytesAvailable = PDM.available();
  samplesRead = bytesAvailable / 2;
  PDM.read(sampleBuffer, bytesAvailable);
}
