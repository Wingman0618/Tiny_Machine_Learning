## Introduction
This folder contains the jupyter lab files used to develop the machine learning model. The folder named *models* contains the output models and the a simple model develpment history description.

## Training data set
The training set is 60*40 flatten decibels of the mel-spectrogram as input. The raw data is 1s 16k sample rate audio. 

## Layers
The model contains a *Reshape* layer, reshape the 2400 size array of one dimension to be 60*40 size array of two dimension.

A 2D convolution layer with 11 3*3 kernels and a 50 percent dropout. 

A full-connected layer with *softmax* activation function to the output results.

![Screenshot from 2024-06-04 17-30-26](https://github.com/Wingman0618/Tiny_Machine_Learning/assets/111321164/73d276aa-a7cb-4ab9-a038-6d85f41681f8)

## Quantisation
The model is quantised into *int8* by tensorflow lite both input and output.

## Dataset reference
Warden, P. (2018). Speech Commands: A Dataset for Limited-Vocabulary Speech Recognition. [online] arXiv.org. Available at: https://arxiv.org/abs/1804.03209.

