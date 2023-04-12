g++ -fPIC -O3 -fPIE -c -g  NeuralNets.c
g++ -fPIC -O3 -no-pie -g *.o -lm -o NeuralNets

