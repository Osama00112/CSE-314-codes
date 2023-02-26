echo "Running H2O_zemaphore.c"
g++ H2O_zemaphore.c zemaphore.c -o test-D -lpthread
./test-D
echo
echo "Running H2O.c"
g++ H2O.c zemaphore.c -o test-D_2 -lpthread
./test-D_2
