## Introduction
This folder contains the source files of model deployment on Arduino nano 33 BLE by tensorflow lite micro.

The deployment process focuses on the embedded development, including collecting the audio signal, transforming the audio signal, invoking the model, and output commands.

## Collecting the audio signal
The audio signal is collected by microphone device on the Arduino microcontroller on 16000 sample rate, which is that 256 data is collected in each loop.

## Data pre-processing
The audio signal was modified through *Short-time Fourier Transform* with 512 window size and 256 hop length. 512 data samples which is two loops was put in the input sample buffer.
Then the input buffer was processed by *Fourier Transform* and compluted into mel-scale to be mel-spectrogram. The last stage was to quantlise the sample data into *int8*.

## Model invocation
Invoking the model through tensorflow lite mirco

## Commands output
For the keyword *yes*, turn the green led on. For the keyword *no*, turn the red led on. Defult status: blue led.

## Current issues
The keyword recognition is not accuracy for now.

## Future work
1. Check if it is the issue of input data. Actions: Collect the sample data by microcontroller and create a new test dataset based on the data. Then evaluating the model on jupyter lab.
2. Check if it is the issue of model. Acitons: Use the microcontorller to generate a new training dataset and train the model again.
