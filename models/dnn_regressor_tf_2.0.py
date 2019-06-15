# Works only under TF 2.0!
import tensorflow as tf
from tensorboard.plugins.hparams import api as hp
from absl import flags
from absl import app
from datetime import datetime
from data_processor import read_data


flags.DEFINE_integer('input_width', 241, 'Width of the input for feature columns.')
flags.DEFINE_string('model_path', 'dnn_model_dir/', 'Set a value for data source folder.')
flags.DEFINE_string('trainingset_path', './', 'Set a value for data destination folder.')
flags.DEFINE_string('evalset_path', './', 'Set a value for data destination folder.')
flags.DEFINE_string('mode', 'train', 'Select a mode, either train/eval/infer.')
flags.DEFINE_string('hparams', None, 'List of name-value pairs used for training.')
flags.DEFINE_boolean('hp_tuning', False, 'Whether to do multiple runs to compare params.')

FLAGS = flags.FLAGS


def log_output(results, flag_hparams, hparams):
	date = str(datetime.today().strftime('%Y%m%d%H%M%S'))
	filename = 'dnn_regressor_' + l + '_' + str(flag_hparams) + '_' + date + '.log'
	with open(filename, 'w') as f:
		f.write(str(hparams))
		f.write('\n\n')
		f.write(str(results))


def input_fn(mode, hparams, trainingset_path):
	data_x, data_y = read_data(trainingset_path, mode)
	return tf.compat.v1.estimator.inputs.pandas_input_fn(
		data_x,
		y=data_y,
		batch_size=128,
		num_epochs=10,
		shuffle=True,
		num_threads=2,
		)


def prepare_feature_columns(input_width):
	return [tf.feature_column.numeric_column(str(k)) for k in range(
		1, input_width)]


def estimator_fn(hparams, model_path, input_width):
	if hparams['activation'] == 'relu':
		hparams['activation'] = tf.nn.relu
	elif hparams['activation'] == 'tanh':
		hparams['activation'] = tf.nn.tanh

	dropout = None if hparams['dropout'] == 0 else hparams['dropout']
	return tf.estimator.DNNRegressor(
		model_dir=model_path,
		feature_columns=prepare_feature_columns(input_width),
		hidden_units=[hparams['num_units']] * 4,
		dropout=dropout,
		activation_fn=hparams['activation'],
		optimizer=tf.optimizers.Adam(
			learning_rate=hparams['learning_rate']
			)
		)


def run(run_dir, hparams, estimator_input, input_width):
	with tf.summary.create_file_writer(run_dir).as_default():
		hp.hparams(hparams)  # record the values used in this trial
		estimator = estimator_fn(hparams, run_dir, input_width)
		estimator.train(input_fn=estimator_input)
		results = estimator.evaluate(input_fn=estimator_input)
		log_output(results, FLAGS.hparams, hparams)


def hp_tuning(estimator_input, input_width, model_path):

	HP_NUM_UNITS = hp.HParam('num_units', hp.Discrete([128, 256, 512, 1024]))
	HP_DROPOUT = hp.HParam('dropout', hp.Discrete([0.0, 0.05, 0.1, 0.5]))
	HP_LEARNING_RATE = [0.00001, 0.0005, 0.001]
	HP_ACTIVATION = hp.HParam('activation', hp.Discrete(['relu', 'tanh']))

	METRIC_ACCURACY = 'accuracy'

	with tf.summary.create_file_writer(model_path + '/logs/').as_default():
		hp.hparams_config(
			hparams=[HP_NUM_UNITS, HP_ACTIVATION],
			metrics=[hp.Metric(METRIC_ACCURACY, display_name='Accuracy')],
		)
	session_num = 0

	for num_units in HP_NUM_UNITS.domain.values:
		for dropout_rate in HP_DROPOUT.domain.values:
			for activation in HP_ACTIVATION.domain.values:
				for learning_rate in HP_LEARNING_RATE:
					hparams = {
					'num_units': num_units,
					'dropout': dropout_rate,
					'activation': activation,
					'learning_rate': learning_rate}
					run_name = id_from_hp(hparams)
					print('--- Starting trial: %s' % run_name)
					print({k: v for k, v in hparams.items()})
					run(model_path + '/logs/' + run_name, hparams, 
					estimator_input, input_width)
					session_num += 1


def id_from_hp(hp):
	return "dnn_n_units{}_dropout{:.4f}_lr{:5f}_{}".format(hp['num_units'], hp['dropout'], hp['learning_rate'], hp['activation'])


def main(argv):
	print('Running with arguments...')
	print('Model path: ' + FLAGS.model_path)
	print('Training set path: ' + FLAGS.trainingset_path)
	print('Mode: ' + str(FLAGS.mode))
	print('Hparams: ' + str(FLAGS.hparams))

	hparams = None

	estimator_input = input_fn(FLAGS.mode, hparams, FLAGS.trainingset_path)

	if FLAGS.mode == 'train':
		if FLAGS.hp_tuning:
			hp_tuning(estimator_input, FLAGS.input_width, FLAGS.model_path)
		else:
			print('Training with hparams: ' + str(hparams))
			estimator.train(input_fn=estimator_input)
	elif FLAGS.mode == 'eval':
		print('Starting evaluation...')
		estimator = estimator_fn(hparams, FLAGS.model_path, FLAGS.input_width)
		results = estimator.evaluate(input_fn=estimator_input)
		log_output(results, FLAGS.hparams, hparams)


if __name__ == '__main__':
	app.run(main)
