import os
my = "so/Strategy.so"
for num in range(2, 102, 2):
    component = "TestCases/%d.so" % num
    os.system("./Compete/Compete %s %s ./result/result%d.txt %d" % (my, component, num, 1))
