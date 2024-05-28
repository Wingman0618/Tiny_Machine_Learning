/*
Reference: https://www.youtube.com/watch?v=nl9TZanwbBk
           https://www.youtube.com/watch?v=Xw4voABxU5c
library for complex number computing: https://github.com/RobTillaart/Complex/tree/master
*/

#include <cmath>
#include <cstdlib>
#include <Complex.h>
#include <vector>

using namespace std;

unsigned int bitReverse(unsigned int x, int log2n){
  int n=0;

  for(int i=0; i<log2n; i++){
    n <<= 1;
    n |= (x&1);
    x >>= 1;
  }

  return n;
}

void fft(vector<Complex> &a, vector<Complex> &A, int log2n){
  int n = 512;

  for(unsigned int i=0; i<n; i++){
    int rev = bitReverse(i, log2(n));
    A[i] = a[rev];
  }

  Complex j(0, 1);
  for(int s=1; s<=log2(n); s++){
    int m = 1 << s;
    int m2 = m >> 1;
    Complex w(1, 0);
    Complex e (M_E, 0);
    Complex wm = e.c_pow(j*(M_PI/m2));
    for(int i=0; i<m2; i++){
      for(int k=i; k<n; k+=m){
        Complex t = w*A[k+m2];
        Complex u = A[k];

        A[k] = u + t;

        A[k+m2] = u - t;
      }
      w *= wm;
    }
  }
}

double* dft_abs(double* wave, int num_of_inputs){
  int n = num_of_inputs;

  double* result = (double*)malloc(n*sizeof(double));
  vector<Complex> wave_complex(n);
  vector<Complex> result_complex(n);
  
  for(int i=0; i<n; i++){
    Complex c_wave(*(wave+i), 0);
    wave_complex[i] = c_wave;
  }

  fft(wave_complex, result_complex, 9);
//  fft(wave_complex, result_complex, 4);
  for(int k=0; k<n; k++){
    double r = result_complex[k].real();
    double img = result_complex[k].imag();
    *(result+k) = sqrt(pow(r,2)+pow(img,2));
  }

  /*
  // Define the omega w
  // C++ complex implementation tutrial:
  Complex i(0., ((-2.*M_PI)/n));
  Complex e(M_E, 0);
  Complex w = e.c_pow(i);

  for(int k=0; k<n; k++){
    Complex coe = (0, 0);
    for(int j=0; j<n; j++){
      Complex inputs(wave[j], 0);
      coe += inputs*w.c_pow(j*k);
    }
    double r = coe.real();
    double img = coe.imag();
    *(result+k) = sqrt(pow(r,2)+pow(img,2));
  }
*/
  return result;
}

double* hamming_data(int8_t* wave, int num_of_inputs){
  vector<double> weight(num_of_inputs);
  double *result = (double*)malloc(num_of_inputs*sizeof(double));

  // Define min and max values for normalising the data
  double max;
  double min;

  for(int  i=0; i<num_of_inputs; i++){
    weight[i] = 0.54 - 0.46*cos((2.*M_PI*double(i))/(double(num_of_inputs)-1.));
    if(i==0){
      max = weight[i];
      min = weight[i];
    }
    if(i>0){
      if(weight[i]>=max){
        max = weight[i];
      }
      if(weight[i]<=min){
        min = weight[i];
      }
    }
  }
  // Normalise the data
  for(int i=0; i<num_of_inputs; i++){
    weight[i] = (weight[i]-min)/(max-min);
  }
  // weight the data
  for(int i=0; i<num_of_inputs; i++){
    *(result+i) = *(wave+i)*weight[i];
  }
  return result;
}
