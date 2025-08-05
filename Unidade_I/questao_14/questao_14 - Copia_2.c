#include <stdio.h>
#include <math.h>

int main() {
    int n[6] = {10, 20, 40, 80, 160, 320};
    int p = 2;
    double t_seqencial, t_paralelo, t_overhead, speedup, efficiency;
    

    for(int x = 0 ; x < 6; x++){
        printf("------------- TAMANHO DO PROBLEMA: %d -------------\n", n[x]);
        t_seqencial = pow(n[x], 2);
        t_overhead = pow(n[x], 2.5);
        printf("-- TEMPO SEQUENCIAL: %.2f \n", t_seqencial);
        printf("-- TEMPO OVERHEAD: %.2f \n", t_overhead);
        printf("-- PARALELO: \n");
        t_paralelo = (t_seqencial / p) + t_overhead;
        printf("- PARA %d THREADS: \n", p);
        printf("TEMPO: %.2f \n", t_paralelo);
        speedup = t_seqencial / t_paralelo;
        printf("SPEEDUP: %.2f \n", speedup);
        efficiency = t_seqencial / (p*t_paralelo);
        printf("EFICIENCIA: %.2f \n", efficiency);

    }

    return 0;
}