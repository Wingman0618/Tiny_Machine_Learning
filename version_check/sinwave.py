## Reference: https://gist.github.com/ShawnHymel/79237fe6aee5a3653c497d879f746c0c

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import math
from tensorflow.keras import layers

print('Numpy ' + np.__version__)
print('TensorFlow ' + tf.__version__)
##print('Keras ' + tf.keras.version())

nsamples = 1000
val_ratio = 0.2
test_ratio = 0.2 
tflite_model_name = 'sine_model'
c_model_name = 'sine_model'

np.random.seed(1234)
x_values = np.random.uniform(low=0, high=(2 * math.pi), size=nsamples)
y_values = np.sin(x_values) + (0.1 * np.random.randn(x_values.shape[0]))
print("Generate the samples")

val_split = int(val_ratio * nsamples)
test_split = int(val_split + (test_ratio * nsamples))
x_val, x_test, x_train = np.split(x_values, [val_split, test_split])
y_val, y_test, y_train = np.split(y_values, [val_split, test_split])
assert(x_train.size + x_val.size + x_test.size) == nsamples
print("dataset splited")

model = tf.keras.Sequential()
model.add(layers.Reshape((1,)))
model.add(layers.Dense(16, activation='relu', input_shape=(1,)))
model.add(layers.Dense(16, activation='relu'))
model.add(layers.Dense(1))

model.summary()

model.compile(optimizer='rmsprop', loss='mae', metrics=['mae'])

history = model.fit(x_train,
                    y_train,
                    epochs=500,
                    batch_size=100,
                    validation_data=(x_val, y_val))

import pathlib
export_dir = 'models/sinwave'
model.export(export_dir)

converter = tf.lite.TFLiteConverter.from_saved_model(export_dir)
tflite_model = converter.convert()

tflite_model_file = pathlib.Path('models/sinewave.tflite')
tflite_model_file.write_bytes(tflite_model)

## Convert the hex value for C programming
def hex_to_c_array(hex_data, var_name):

  c_str = ''

  # Create header guard
  c_str += '#ifndef ' + var_name.upper() + '_H\n'
  c_str += '#define ' + var_name.upper() + '_H\n\n'

  # Add array length at top of file
  c_str += '\nunsigned int ' + var_name + '_len = ' + str(len(hex_data)) + ';\n'

  # Declare C variable
  c_str += 'unsigned char ' + var_name + '[] = {'
  hex_array = []
  for i, val in enumerate(hex_data) :

    # Construct string from hex
    hex_str = format(val, '#04x')

    # Add formatting so each line stays within 80 characters
    if (i + 1) < len(hex_data):
      hex_str += ','
    if (i + 1) % 12 == 0:
      hex_str += '\n '
    hex_array.append(hex_str)

  # Add closing brace
  c_str += '\n ' + format(' '.join(hex_array)) + '\n};\n\n'

  # Close out header guard
  c_str += '#endif //' + var_name.upper() + '_H'

  return c_str

with open(c_model_name + '.h', 'w') as file:
  file.write(hex_to_c_array(tflite_model, c_model_name))
  
print("Finished")
