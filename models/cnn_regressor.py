from keras.layers.normalization import BatchNormalization
from keras.layers.convolutional import Conv1D
from keras.layers.convolutional import MaxPooling1D
from keras.layers.core import Activation
from keras.layers.core import Dropout
from keras.layers.core import Dense
from keras.layers import Flatten
from keras.layers import Input
from keras.models import Model
from keras.optimizers import Adam
from keras import callbacks
from hparams import create_hparams
import numpy as np

from absl import flags
from absl import app
from datetime import datetime

from tensorflow.contrib.training import HParams
from data_processor import read_data
import tensorflow as tf

import pandas as pd


def log_output(results, hparams, filepath):
	date = str(datetime.today().strftime('%Y%m%d%H%M%S'))
	filename = filepath + 'cnn_regressor_' + str(np.min(results['loss'])) + '_' + date + '.log'
	with open(filename, 'w') as f:
		f.write(str(hparams))
		f.write('\n\n')
		f.write(str(results))


def input_fn(trainingset_path):
	x_train, y_train = read_data(trainingset_path, 'train')
	x_eval, y_eval = read_data(trainingset_path, 'eval')
	x_train = np.reshape(x_train.values, (-1, x_train.shape[1], 1))
	y_train = np.reshape(y_train.values, (-1, 1))
	x_eval = np.reshape(x_eval.values, (-1, x_eval.shape[1], 1))
	y_eval = np.reshape(y_eval.values, (-1, 1))
	return x_train, y_train, x_eval, y_eval


def create_cnn(width, height, filters=[16, 32, 64], dropout=0.5, activation='relu', kernel_size=2):
  
	inputShape = (width, height)
	chanDim = -1

	inputs = Input(shape=inputShape)
	x = inputs
	for (i, f) in enumerate(filters):
		x = Conv1D(f, kernel_size, padding="same")(x)
		x = Activation(activation)(x)
		x = BatchNormalization(axis=chanDim)(x)
		x = MaxPooling1D(pool_size=(2))(x)

	x = Flatten()(x)
	x = Dense(256)(x)
	x = Activation("relu")(x)
	x = BatchNormalization(axis=chanDim)(x)
	if dropout > 0:
		x = Dropout(dropout)(x)

	x = Dense(64)(x)
	x = Activation("relu")(x)
	x = Dense(16)(x)
	x = Activation("relu")(x)

	x = Dense(1, activation="linear")(x)

	model = Model(inputs, x)

	return model


def train_and_eval(hparams, trainingset_path, model_path):
  path = model_path + "/" + id_from_hp(hparams)
  
  x_train, y_train, x_eval, y_eval = input_fn(trainingset_path)
	
  model = create_cnn(x_train.shape[1], 1, 
                     filters=hparams.cnn_filters,
                     dropout=hparams.dropout,
                     activation=hparams.activation,
                     kernel_size=hparams.kernel_size
              )
  opt = Adam(lr=hparams.learning_rate,
             decay=1e-3 / 200)
  model.compile(loss="mean_squared_error", optimizer=opt)

  tbCallBack = callbacks.TensorBoard(
      log_dir=path, histogram_freq=0, 
      write_graph=True, write_images=True)
  
  history_callback = model.fit(
      x_train, 
      y_train, 
      validation_data=(x_eval, y_eval),
      epochs=hparams.num_epochs, 
      batch_size=hparams.batch_size, 
      callbacks=[tbCallBack])
  
  loss_history = history_callback.history
  
  log_output(loss_history, hparams, path)

def id_from_hp(hp):
    return "cnn_kernel{}_filters{}_dropout{:.4f}_lr{}_{}".format(hp.kernel_size, hp.cnn_filters, hp.dropout, hp.learning_rate, hp.activation)

def main(argv):
  HP_NUM_FILTERS = [[32,32,32,32], [128, 128, 64, 32, 32], [256, 256, 256, 256, 256]]
  HP_DROPOUT = [0.0, 0.5, 0.1]
  HP_LEARNING_RATE = [0.0005, 0.001, 0.0001]
  HP_ACTIVATION = ['relu', 'tanh']
  HP_KERNEL_SIZE = [9, 11]
  hparams = create_hparams()
  hparams.num_epochs = 200
  session_num = 0


  for filters in HP_NUM_FILTERS:
    for dropout_rate in HP_DROPOUT:
      for activation in HP_ACTIVATION:
        for learning_rate in HP_LEARNING_RATE:
          for kernel_size in HP_KERNEL_SIZE:
            hparams.cnn_filters = filters
            hparams.dropout = dropout_rate
            hparams.learning_rate = learning_rate
            hparams.activation = activation
            hparams.kernel_size = kernel_size
            run_name = id_from_hp(hparams)
            print('--- Starting trial: %s' % run_name)
            print(str(hparams))
            session_num += 1
            train_and_eval(hparams, '', '/scidata/bazinska/tumor_train/cnn/logs/')


if __name__ == '__main__':
	app.run(main)
        
