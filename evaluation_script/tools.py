import subprocess
import os


def fetch():
    cmd = ['git', 'fetch']
    subprocess.call(cmd)


def checkout(ref):
    cmd = ['git', 'checkout', ref]
    error = subprocess.call(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if error:
        print('Error : Could not checkout')
        exit(1)
    print('You are on {}'.format(ref))


def tags():
    cmd = ['git', 'tag']
    out = subprocess.check_output(cmd)
    out = str(out, 'utf-8')
    list_out = out.split("\n")
    list_out.pop()
    return list_out


def build_cmake(tag, project_dir, omp=False):
    if not os.path.exists(project_dir + '/CMakeLists.txt'):
        print('Error :Not a CMake project')
        exit(1)

    comp_dir = 'build-' + tag

    os.makedirs(comp_dir, exist_ok=True)
    os.chdir(comp_dir)

    cmake_location = '../' + project_dir
    options = ['-DCMAKE_BUILD_TYPE=Release']
    if omp:
        options.append('-DOMP=1')

    cmd = ['cmake', cmake_location] + options
    error = subprocess.call(cmd, stdout=subprocess.PIPE)
    if error:
        print('Error : CMake pre compiling failed')
        exit(1)
    subprocess.call('make', stdout=subprocess.PIPE)
    print('\tCMake done !')
    if error:
        print('Error : compilation failed')
        exit(1)
    print('\tCompilation done !')
    os.chdir('..')
    return True
