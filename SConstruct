import os

env = Environment(ENV=os.environ.copy())
env.Append(CCFLAGS=['-std=c++0x','-g']) #, '-O3'])
env.Append(LIBS = ['pthread'])
env.Program(target = 'ivy', source = ["src/ivy/global.cpp", "src/ivy/routine.cpp", "src/main.cpp"])