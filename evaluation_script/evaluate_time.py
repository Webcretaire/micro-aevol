import csv
import os
import subprocess
import sys


def mean(in_file):
    reader = csv.DictReader(in_file)
    step_time = 0
    for row in reader:
        step_time += int(row['total_step'])

    return step_time


def evaluate(program_name, grid_size, genome_size, mutation_rate, threads=0, n_steps=1000, seed=-1):
    cmd = [program_name, '-n', str(n_steps), '-w', str(grid_size), '-h', str(grid_size), '-m', str(mutation_rate), '-g',
           str(genome_size)]
    if threads > 0:
        cmd += ['-t', str(threads)]
    if seed > -1:
        cmd += ['-s', str(seed)]

    subprocess.call(cmd, stdout=subprocess.PIPE)
    in_file = open('time', 'r')
    mean_step_time = mean(in_file)
    in_file.close()
    os.remove('time')

    return mean_step_time / n_steps


if __name__ == '__main__':
    time = evaluate(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    print(time)
