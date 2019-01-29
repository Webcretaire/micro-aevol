import os
import sys

import tools
import evaluate_time

PROGRAM_NAME = 'pdc_mini_aevol'


def compilation(tags, project_dir):
    cwd = os.getcwd()
    for tag in tags:
        print('Compilation of tag {}'.format(tag))

        os.chdir(project_dir)
        tools.checkout(tag)
        os.chdir(cwd)

        omp = 'omp_' in tag
        tools.build_cmake(tag, project_dir, omp)


def measure(tags, rows, output_file):
    os.chdir('experiments')
    for row in rows:
        print('{} {} {} {}'.format(row[0], row[1], row[2], row[3]))
        output_file.write('{},{},{},{}'.format(row[0], row[1], row[2], row[3]))
        output_file.flush()
        for tag in tags:
            prog_name = '../build-{}/{}'.format(tag, PROGRAM_NAME)
            print('\tOn tag {}'.format(tag))
            if 'seq_' in tag:
                if row[3] > 1:  # do not execute sequential program with multiple threads
                    value = 'N/A'
                else:
                    value = evaluate_time.evaluate(prog_name, row[0], row[1], row[2])
            else:
                value = evaluate_time.evaluate(prog_name, row[0], row[1], row[2], row[3])
            output_file.write(',{}'.format(value))
            output_file.flush()
        output_file.write('\n')
        output_file.flush()
    os.chdir('..')


def generate_rows(max_grid_size, max_genome_size, max_mutation_rate, max_threads):
    rows = []

    current_grid_size = 16
    while current_grid_size <= max_grid_size:
        current_genome_size = 1000
        while current_genome_size <= max_genome_size:
            current_mutation_rate = 0.00001
            while current_mutation_rate <= max_mutation_rate:
                current_num_threads = 1
                while current_num_threads <= max_threads:
                    rows.append([current_grid_size, current_genome_size, current_mutation_rate, current_num_threads])
                    if current_num_threads == 1:
                        current_num_threads += 1
                    elif current_num_threads < 8:
                        current_num_threads += 2
                    elif current_num_threads < 20:
                        current_num_threads += 4
                    elif current_num_threads < 32:
                        current_num_threads += 6
                    else:
                        current_num_threads += 16
                current_mutation_rate *= 10
            current_genome_size *= 2
        current_grid_size *= 2

    for row in rows:
        print(row)
    return rows


def main(max_grid_size, max_genome_size, max_mutation_rate, max_threads, project_dir, output_file):
    if not os.path.exists(project_dir + '/CMakeLists.txt'):
        print('A CMake project should exist')
        return False

    cwd = os.getcwd()
    os.chdir(project_dir)
    tools.fetch()

    tags = tools.tags()

    tags = [tag for tag in tags if 'singleMeasure' in tag]

    labels = ['grid_size', 'genome_size', 'mutation_rate', 'num_threads']
    labels += tags

    rows = generate_rows(max_grid_size, max_genome_size, max_mutation_rate, max_threads)

    os.chdir(cwd)
    os.makedirs('experiments', exist_ok=True)

    compilation(tags, project_dir)
    print('Compilation step completed !')

    file = open(output_file, 'w')
    for i, label in enumerate(labels):
        if i == 0:
            file.write(label)
        else:
            file.write(',{}'.format(label))
    file.write('\n')
    file.flush()

    measure(tags, rows, file)

    file.close()

    os.chdir(project_dir)
    tools.checkout('evaluation')

    return True


if __name__ == '__main__':
    if len(sys.argv) < 7:
        print('Not enough arguments given')
        exit(1)

    max_grid_size = int(sys.argv[1])
    max_genome_size = int(sys.argv[2])
    max_mutation_rate = float(sys.argv[3])
    max_threads = int(sys.argv[4])
    project_dir = sys.argv[5]
    output_file = sys.argv[6]

    if main(max_grid_size, max_genome_size, max_mutation_rate, max_threads, project_dir, output_file):
        print('Evaluation Done !')
    else:
        print('Aouch, something went wrong')
