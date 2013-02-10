import os

env = Environment(ENV=os.environ.copy())
env.Append(CCFLAGS=['-std=c++0x','-g']) #, '-O3'])
env.Append(LIBS = ['pthread', 'boost_system', 'boost_thread', 'websockets'])
env.Program(target = 'ivy', source = ["src/ivy/global.cpp", "src/ivy/routine.cpp", "src/main.cpp"])