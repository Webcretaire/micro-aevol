import subprocess
import os


def fetch():
    cmd = ['git', 'fetch']
    subprocess.call(cmd)


def checkout(ref):
    cmd = ['git', 'checkout', ref]
    subprocess.call(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print('You are on {}'.format(ref))


def tags():
    cmd = ['git', 'tag']
    out = subprocess.check_output(cmd)
    out = str(out, 'utf-8')
    list_out = out.split("\n")
    list_out.pop()
    return list_out


def build_cmake(omp=False):
    if not os.path.exists('CMakeLists.txt'):
        return False

    os.makedirs('build', exist_ok=True)
    os.chdir('build')
    cmd = ['cmake', '..', '-DCMAKE_BUILD_TYPE=Release']
    if omp:
        cmd.append('-DOMP')
    subprocess.call(cmd, stdout=subprocess.PIPE)
    subprocess.call('make', stdout=subprocess.PIPE)
    print('compilation done !')
    os.chdir('..')
    return True
