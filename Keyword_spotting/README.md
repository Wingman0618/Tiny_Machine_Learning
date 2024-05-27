# **Keyword spotting model**
04/28 2024 \
model version 1.0 \
model size before quantising: 634256 \
model size after quantising: 164208

05/14 2024 \
model version 1.1
model size before quantising: 98656 \
model size after quantising: 28768 \
Description:
1. Change the dataset size to be orgin (128*32)
1. Change the Conv2D layer to be DepthwiseConv2D

05/17 2024 \
model version 1.2
model size before quantising: 38504 \
model size after quantising: 12736 \
Description:
1. Remove the DepthwiseConv2d layers
1. Remove the MaxPool2D layers
1. Remove Dense(128) layer
1. Add Conv2D layer with 10 3*3 kernel
1. Flatten the input and add Reshape layer ((4096,)->(128, 32, 1))

05/18 2024 \
model version 1.3
model size before quantising: 21732 \
model size after quantising: 9008 \
Description:
1. Shrink the 128 audio samples for better deployment samples generating
1. Shrink the window size to 512, 32ms
1. Shrink the ste to 256, 16ms
1. Shrink the number of mels to 40
1. New model is a bit underfiting, could be fixed by adding epoches

05/27 2024 \
model version 1.4
model size before quantising: 27260 \
model size after quantising: 10472 \
Description:
1. Replace the librosa to custom mel spectrogram generator
1. Add the number of kernal to 11
1. Using amplitude to db instead of power to db for better quantilsation
1. Model is fitted in about 97% accuracy
