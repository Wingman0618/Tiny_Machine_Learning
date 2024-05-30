#include "Output_analysis.h"

void OutputCommands(int8_t data_0, int8_t data_1){
  if(data_0>=125){
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);
  }else if(data_1>=127){
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
  }else{
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
  }
      
}
