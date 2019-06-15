from datetime import datetime
import numpy as np
from absl import app
from absl import flags
from data_processor import read_data
from hparams import create_hparams
from keras import callbacks
from keras.layers import Flatten, CuDNNLSTM, TimeDistributed, LSTM
from keras.layers import Input
from keras.layers.core import Activation
from keras.layers.core import Dense
from keras.layers.core import Dropout
from keras.models import Model
from keras.optimizers import Adam

flags.DEFINE_string('model_path', 'dnn_model_dir/', 'Set a value for data source folder.')
FLAGS = flags.FLAGS


def log_output(results, hparams, filepath):
    date = str(datetime.today().strftime('%Y%m%d%H%M%S'))
    filename = filepath + 'rnn_regressor_' + str(np.min(results['loss'])) + '_' + date + '.log'
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


def create_rnn(input_width, hidden_sizes, classifier_dropout=0.5, activation='relu'):

    inputs = Input(shape=(input_width,1))
    x = inputs
    for size in hidden_sizes[:-1]:
        x = CuDNNLSTM(size, return_sequences=True)(x)
    x = CuDNNLSTM(hidden_sizes[-1])(x)

    x = Dense(256)(x)
    x = Activation(activation)(x)
    if classifier_dropout > 0:
        x = Dropout(classifier_dropout)(x)
    x = Dense(64)(x)
    x = Activation(activation)(x)
    if classifier_dropout > 0:
        x = Dropout(classifier_dropout)(x)
    x = Dense(16)(x)
    x = Activation(activation)(x)

    x = Dense(1, activation="linear")(x)

    model = Model(inputs, x)

    return model


def train_and_eval(hparams, trainingset_path, model_path):
    path = model_path + "/" + id_from_hp(hparams)

    x_train, y_train, x_eval, y_eval = input_fn(trainingset_path)

    model = create_rnn(x_train.shape[1],
                       hidden_sizes=hparams.hidden_sizes,
                       classifier_dropout=hparams.dropout,
                       activation=hparams.activation,
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
    return "rnn_hiddens{}_cls_dropout{:.4f}_lr{}_cls_act_{}".format(hp.hidden_sizes, hp.dropout, hp.learning_rate, hp.activation)


def main(argv):
    print("Logging to " + FLAGS.model_path)
    HP_HIDDEN_SIZES = [[32, 32, 32], [1024, 512, 256], [128, 128, 64, 32, 32], [256, 256, 256, 256, 256], [128, 64, 32]]
    HP_CLASSIFIER_DROPOUT = [0.0, 0.001, 0.1, 0.5]
    HP_LEARNING_RATE = [0.0005, 0.001, 0.0001]
    HP_ACTIVATION = ['tanh', 'relu']
    hparams = create_hparams()

    session_num = 0

    for sizes in HP_HIDDEN_SIZES:
        for dropout_rate in HP_CLASSIFIER_DROPOUT:
            for activation in HP_ACTIVATION:
                for learning_rate in HP_LEARNING_RATE:
                        hparams.hidden_sizes = sizes
                        hparams.dropout = dropout_rate
                        hparams.learning_rate = learning_rate
                        hparams.activation = activation
                        run_name = id_from_hp(hparams)
                        print('--- Starting trial: %s' % run_name)
                        print(str(hparams))
                        session_num += 1
                        train_and_eval(hparams, '', FLAGS.model_path)


if __name__ == '__main__':
    app.run(main)

