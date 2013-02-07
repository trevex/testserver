import os
import subprocess

#NOT WORKING! ENVIRONMENT HAS TO BE SETUP MANUALLY!
#prepared_env = os.environ.copy()
#prep_command = 'bash -i -c "source /opt/intel/composerxe/bin/compilervars.sh intel64"'
#prep_event = subprocess.Popen(prep_command, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, env=prepared_env)
#prep_output = prep_event.communicate()[0]
#if prep_output == '':
#	print 'Intel compilervars.sh executed successfully.'

# get current environment from terminal, assumes intel compiler vars are set up

env = Environment(ENV=os.environ.copy())
#env.Replace(CXX='icpc')
env.Append(CCFLAGS=['-std=c++0x','-g']) #, '-O3'])
env.Append(LIBS = ['pthread'])
env.Program(target = 'actor', source = ["main.cpp"])