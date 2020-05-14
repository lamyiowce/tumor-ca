import numpy as np


for seti in range(6):
    for idi in range(10):
        with open("outs/" + str(seti) + '/' + str(idi)) as fp:
            with open("outsA/" + str(seti) + '/' + str(idi)) as fp2:
                with open("outsB/" + str(seti) + '/' + str(idi)) as fp3:
                    line = fp.readline()
                    results1 = [int(x) for x in line[:-2].split(', ')]
                    line = fp2.readline()
                    results2 = [int(x) for x in line[:-2].split(', ')]
                    line = fp3.readline()
                    results3 = [int(x) for x in line[:-2].split(', ')] 
                    print(np.mean(results1 + results2 + results3), end=' ')
                    print(np.var(results1 + results2 + results3))

