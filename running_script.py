import subprocess

config_file = open("config.txt")

infile = config_file.readline().strip().split("=")[1]
output_a = config_file.readline().strip().split("=")[1]
output_b = config_file.readline().strip().split("=")[1]
threads = config_file.readline().strip().split("=")[1]
subprocess.call(["g++","main.cpp", "-pthread", "-std=c++11", "-o", "test"])
subprocess.call(["./test", infile, threads, output_a])


#print(infile + " " + threads + " " + output_a + " ")
#g++ main.cpp -pthread -std=c++11 -o test



