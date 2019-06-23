import sys
from datetime import datetime
import numpy as np
from absl import app
from absl import flags
from data_processor import read_data
from hparams import create_hparams
from keras import callbacks
from keras.layers import Flatten, TimeDistributed, CuDNNLSTM, CuDNNGRU
from keras.layers import Input
from keras.layers.core import Activation
from keras.layers.core import Dense
from keras.layers.core import Dropout
from keras.models import Model, Sequential
from keras.optimizers import Adam
from sklearn.metrics import roc_curve, precision_recall_curve, average_precision_score
from sklearn.utils.class_weight import compute_class_weight
from sklearn.metrics import auc

flags.DEFINE_string('model_path', 'model_dir/', 'Set a value for data source folder.')
flags.DEFINE_string('hparams', None, 'List of name-value pairs used for training.')
flags.DEFINE_boolean('use_weights', False, 'Whether to use weights for the cross-entropy.')
flags.DEFINE_string('rnn_cell_type', 'lstm', 'RNN memory cell type to use. Might be LSTM or GRU.')

FLAGS = flags.FLAGS


def log_output(results, fpr_keras, tpr_keras, thresholds_keras, precision, recall, thresholds_pr, auc_keras, auc_pr, hparams, filepath):
    date = str(datetime.today().strftime('%Y%m%d%H%M%S'))
    filename = filepath + '_rnn_classifier_' + str(np.min(results['loss'])) + '_' + date + '.log'
    with open(filename, 'w') as f:
        f.write(str(hparams))
        f.write('\n\n')
        f.write(str(results))
        f.write('\n\n')
        np.set_printoptions(threshold=sys.maxsize)
        np.set_printoptions(suppress=True)
        f.write(str(auc_keras) + ' ' + str(auc_pr))


def input_fn(trainingset_path, use_weights):
    x_train, y_train = read_data(trainingset_path, 'train')
    y_train = y_train.apply(lambda x: 1 if x < -1 else 0)
    print('Train set balance: {}/{}'.format(np.count_nonzero(y_train), len(y_train)))
    x_eval, y_eval = read_data(trainingset_path, 'eval')
    y_eval = y_eval.apply(lambda x: 1 if x < -1 else 0)
    print('Eval set balance: {}/{}'.format(np.count_nonzero(y_eval), len(y_eval)))
    x_test, y_test = read_data(trainingset_path, 'infer')
    y_test = y_test.apply(lambda x: 1 if x < -1 else 0)
    print('Infer set balance: {}/{}'.format(np.count_nonzero(y_test), len(y_test)))

    # Compute class weights to optimize binary cross-entropy
    if use_weights:
        weights = compute_class_weight('balanced', np.unique(y_test), y_test)
    else:
        weights = None

    x_train = np.reshape(x_train.values, (-1, x_train.shape[1], 1))
    y_train = np.reshape(y_train.values, (-1, 1))
    x_eval = np.reshape(x_eval.values, (-1, x_eval.shape[1], 1))
    y_eval = np.reshape(y_eval.values, (-1, 1))
    x_test = np.reshape(x_test.values, (-1, x_test.shape[1], 1))
    y_test = np.reshape(y_test.values, (-1, 1))
    return x_train, y_train, x_eval, y_eval, x_test, y_test, weights


def create_rnn(input_width, hidden_sizes, classifier_dropout=0.5, activation='relu', cell_type='lstm'):

    inputs = Input(shape=(input_width,1))
    x = inputs
    for size in hidden_sizes[:-1]:
        if cell_type == 'lstm':
            x = CuDNNLSTM(size, return_sequences=True)(x)
        else:
            x = CuDNNGRU(size, return_sequences=True)(x)
    if cell_type == 'lstm':
        x = CuDNNGRU(hidden_sizes[-1])(x)
    else:
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

    x = Dense(1, activation="sigmoid")(x)

    model = Model(inputs, x)

    return model


def train_and_eval(hparams, trainingset_path, model_path, cell_type, use_weights):
    path = model_path + id_from_hp(hparams)

    x_train, y_train, x_eval, y_eval, x_test, y_test, weights = input_fn(
        trainingset_path, use_weights)

    model = create_rnn(x_train.shape[1],
        hidden_sizes=hparams.hidden_units,
        classifier_dropout=hparams.dropout,
        activation=hparams.activation,
        cell_type=cell_type,
        )

    opt = Adam(lr=hparams.learning_rate,
               decay=1e-3 / 200)
    model.compile(loss="binary_crossentropy", optimizer=opt)

    tbCallBack = callbacks.TensorBoard(
        log_dir=path, histogram_freq=0,
        write_graph=True, write_images=True)

    history_callback = model.fit(
        x_train,
        y_train,
        validation_data=(x_eval, y_eval),
        epochs=hparams.num_epochs,
        batch_size=hparams.batch_size,
        callbacks=[tbCallBack],
        class_weight=weights)

    loss_history = history_callback.history

    y_pred = model.predict(x_test).ravel()
    fpr_keras, tpr_keras, thresholds_keras = roc_curve(y_test, y_pred)
    auc_keras = auc(fpr_keras, tpr_keras)
    auc_pr = average_precision_score(y_test, y_pred)
    precision, recall, thresholds_pr = precision_recall_curve(y_test, y_pred)
    log_output(loss_history, fpr_keras, tpr_keras, thresholds_keras, precision, recall, thresholds_pr, auc_keras, auc_pr, hparams, path)
    model.save(path + '.model')


def id_from_hp(hp):
    return "gru_hiddens{}_cls_dropout{:.4f}_lr{}_cls_act_{}".format(hp.hidden_units, hp.dropout, hp.learning_rate, hp.activation)


def main(argv):
    print("Logging to " + FLAGS.model_path)
    hparams = create_hparams(FLAGS.hparams)

    session_num = 0

    run_name = id_from_hp(hparams)
    print('--- Starting trial: %s' % run_name)
    print(str(hparams))
    session_num += 1
    train_and_eval(hparams, '', FLAGS.model_path, 
        FLAGS.rnn_cell_type, FLAGS.use_weights)


if __name__ == '__main__':
    app.run(main)