#include <TensorFlowLite.h>

#include "Ml_model.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

namespace{
  tflite::ErrorReporter *error_reporter = nullptr;
  const tflite::Model *model = nullptr;
  tflite::MicroInterpreter *interpreter = nullptr;
  TfLiteTensor *model_input = nullptr;
  int32_t previous_time = 0;

  constexpr int kTensorArenaSize = 161 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
  int8_t *model_input_buffer = nullptr;
  }

void setup() {
  // put your setup code here, to run once:
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(g_model);
  if(model->version() != TFLITE_SCHEMA_VERSION){
      TF_LITE_REPORT_ERROR(error_reporter,
      "Schema version %d is not supported version %d.", 
      model->version(), TFLITE_SCHEMA_VERSION);
      return;
  }

  static tflite::MicroMutableOpResolver<5> micro_op_resolver(error_reporter);
  if(micro_op_resolver.AddConv2D() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddDepthwiseConv2D() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddFullyConnected() != kTfLiteOk){
    return;
  }
  if(micro_op_resolver.AddMaxPool2D() != kTfLiteOk){
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

  previous_time = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

}
