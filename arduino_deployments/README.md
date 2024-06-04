## Introduction
This folder contains the source files of model deployment on Arduino nano 33 BLE by tensorflow lite micro. There are also fundamental signal processing implementation such as *Short-time Fourier Transform*, *mel-spectrogram transfrom*, and *int8-quantisation*.

## Input samples data
Based on the model trained on Jupyter lab (in the folder *Keyword_spotting*), the input data is 60*40 flatten array. The raw data is a 1s audio wave signal collected in 16k sample rate. The model input data is *int8* mel-spectrogram with 40 filterbanks, 512 window size, and 256 hop length. 

Based on the experiment on Arduino nano 33 BLE microphone, in the 16000 sample rate, 256 sample data is collected in each loop. Therefore, a sample buffer will be applied in *Fourier Transform* in every two loops, computed into a time-period of *Short-time Fourier Transform*.

There are total 60 steps in the *Short-time Fourier Transform*, hence, the first input data will be generated by 120 loops. After the first input data being invoked, the scan window shifted one time-period, adding another 512 sample data, which a new input data is generated in every two loops.

## Fourier Transform and mel-spectrogam
The auido signal is modified through *Real Fourier Transform* and the *mel-scale*. The *Fast Fourier Transfrom* is embedded implemented, which is *Iterative-fast-Fourier-Transformation*. As for other data processing functions, including *real Fourier Transform*, *hamming*, *mel_spectrogram*, and *amplitude_to_db*, the deteils can be found in the repository of [*mel_spectrogram_library_for_Cpp*](https://github.com/Wingman0618/mel_spectrogram_library_for_Cpp).

## Commands output
The model is train in two keyword, "yes" and "no". For the keyword "yes", output is green led. For the keyword "no", output is red led. For the default stage, output is blue led.

## Issues
The keyword recognition is not accuracy. The output result is always green led, which is recognised as "yes".

## Future work
The next stage is to evaluate the issue with two protential assumptions. 

1. Assumption one: Input data is not fixed with the model. Collecting and processing the audio signal data on the microcontroller. Using those data to bulid a new test dataset. Evaluating the model based on the new test dataset.
2. Assumption two: Training data is not fixed with the microcontroller. As the training dataset is provided by tensorflow, the model could be not fited with the microcontroller as the the audio collected device is varied. Using the Arduino microphone to collect a new dataset. Re-train the model with the new training dataset.

## References
Complex number library: https://github.com/RobTillaart/Complex/tree/master \
Iterative-fast-Fourier-Transformation: https://www.geeksforgeeks.org/iterative-fast-fourier-transformation-polynomial-multiplication/ \
Mel spectrogram: https://github.com/Wingman0618/mel_spectrogram_library_for_Cpp