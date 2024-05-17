#include <TensorFlowLite.h>
#include <PDM.h>

#include "test_input.h"

#include "Ml_model.h"
#include "Output_analysis.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

namespace{
  // Audio samples
  short sampleBuffer[256];
  volatile int samplesRead;
  
  //Tensorflow namespace
  tflite::ErrorReporter *error_reporter = nullptr;
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *model_input = nullptr;
  TfLiteTensor* model_output = nullptr;

  constexpr int kTensorArenaSize =10 * 1024;
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

//Reference of adding int32_t to StridedSlice: https://stackoverflow.com/questions/64850356/error-with-strided-slice-in-tensorflow-lite
  static tflite::MicroMutableOpResolver<7> micro_op_resolver(error_reporter);
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
  if(micro_op_resolver.AddSoftmax() != kTfLiteOk){
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

  model_output = interpreter->output(0);

  // Setup microphone
  //Reference: https://docs.arduino.cc/learn/built-in-libraries/pdm/ 
  PDM.onReceive(onPDMdata);
  PDM.begin(1, 16000);
  PDM.setGain(20);

}

void loop() {

  if(samplesRead){
    for(int i=0; i<samplesRead; i++){
      Serial.println(sampleBuffer[i]);
    }
    samplesRead = 0;
  }
// Faking inputs
  for(int i=0; i<4096; i++){
    model_input_buffer[i] = input_val[i];
  }

  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    return;
  }
  
  int8_t data_no = model_output->data.int8[0];
  int8_t data_yes = model_output->data.int8[1];

  OutputCommands(data_no, data_yes);
  /*
  Serial.print(data_no);
  Serial.print("  ");
  Serial.println(data_yes);

  if(data_no>=80){
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);
  }else if(data_yes>=80){
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
  }else{
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
  }
  */
  Serial.println("one loop");
}

void onPDMdata(){
  int bytesAvailable = PDM.available();

  PDM.read(sampleBuffer, bytesAvailable);

  samplesRead = bytesAvailable / 2;
}
