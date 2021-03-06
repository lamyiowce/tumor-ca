import sys
import os

if __name__ == "__main__":
	if len(sys.argv) is not 2:
		sys.exit()

	directory = sys.argv[1]
	outdir = os.path.join(directory, 'aliveCount')
	if not os.path.exists(outdir):
		os.mkdir(outdir)
	for filename in os.listdir(directory):
		if filename != 'aliveCount':
			with open(os.path.join(directory, filename)) as f:
				f.readline()
				w = f.readline()
				s = sum(map(int, w.split(',')))
			f_id = os.path.basename(filename)
			with open (os.path.join(outdir, f_id), "w+") as out_f:
				out_f.write(str(s) + "\n")
