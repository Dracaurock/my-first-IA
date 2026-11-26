#include <cstdio>

// __global__ : mot-clé CUDA qui marque une fonction comme "kernel" -- du code qui
// s'exécute sur le GPU, appelable depuis le CPU
__global__ void direBonjour() {
    printf("Bonjour depuis le GPU !\n");
}

int main() {
    direBonjour<<<1, 1>>>();  // lance le kernel : 1 bloc, 1 thread (syntaxe expliquée bientot)
    cudaDeviceSynchronize();   // attend que le GPU ait fini avant de continuer
    printf("Fin du programme (execute sur le CPU)\n");
    return 0;
}