/*
Reference: https://www.youtube.com/watch?v=nl9TZanwbBk
           https://www.youtube.com/watch?v=Xw4voABxU5c
library for complex number computing: https://github.com/RobTillaart/Complex/tree/master
*/

#include <cmath>
#include <cstdlib>
#include <Complex.h>

Complex* dft(float* wave, int num_of_inputs){
  int n = num_of_inputs;

  Complex* result = (Complex*)malloc(n*sizeof(Complex));

  // Define the omega w
  // C++ complex implementation tutrial:
  //  https://www.geeksforgeeks.org/complex-numbers-c-set-1/
  Complex i(0., ((-2.*M_PI)/n));
  Complex e(M_E, 0);
  Complex w = e.c_pow(i);

  for(int k=0; k<n; k++){
    Complex coe(0, 0);
    for(int j=0; j<n; j++){
    Complex inputs(wave[j], 0);
      coe += inputs*w.c_pow(j*k);
    }
    *(result+k) = coe;
  }

  return result;
}

float* dft_abs(float* wave, int num_of_inputs){
  int n = num_of_inputs;

  float* result = (float*)malloc(n*sizeof(float));

  // Define the omega w
  // C++ complex implementation tutrial:
  //  https://www.geeksforgeeks.org/complex-numbers-c-set-1/
  Complex i(0., ((-2.*M_PI)/n));
  Complex e(M_E, 0);
  Complex w = e.c_pow(i);

  for(int k=0; k<n; k++){
    Complex coe = (0, 0);
    for(int j=0; j<n; j++){
      Complex inputs(wave[j], 0);
      coe += inputs*w.c_pow(j*k);
    }
    float r = coe.real();
    float img = coe.imag();
    *(result+k) = sqrtf(pow(r,2)+pow(img,2));
  }

  return result;
}

float* hamming(int num_of_inputs){
  float *weight = (float*)malloc(num_of_inputs*sizeof(float));

  // Define min and max values for normalising the data
  float max;
  float min;
  for(int  i=0; i<num_of_inputs; i++){
    *(weight+i) = 0.54 - 0.46*cos((2.*M_PI*float(i))/(float(num_of_inputs)-1.));
    if(i==0){
      max = *(weight+i);
      min = *(weight+i);
    }
    if(i>0){
      if(*(weight+i)>=max){
        max = *(weight+i);
      }
      if(*(weight+i)<=min){
         min = *(weight+i);
      }
    }
  }
  // Normalise the data
  for(int i=0; i<num_of_inputs; i++){
    *(weight+i) = (*(weight+i)-min)/(max-min);
  }

  return weight;
}
