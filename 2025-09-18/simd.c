// PEGAR O CODIGO E PARALELIZAR COM SIMD

// gcc 01a-pi-seq.c -o 01a-pi-seq

// gcc -O3 -fopt-info-vec-optimized simd.c -o 01a-pi-seq
// -O3 habilita otimizações de código do compilador
// -fopt-info-vec-optimized informa os laços vetorizados.
// -fopt-info-vec-missed infomaria quais oportunidades de otimização por vetorização foram perdidas

// gcc -O3 -fopt-info-vec-optimized -ffast-math simd.c -o 01a-pi-seq
// ffast-math permite otimizações mesmo que a precisão da operação não possa ser garantida
//
// gcc -Ofast simd.c -o 01a-pi-seq
// -Ofast = -O3 gcom -ffast-math e outras
//

#include <stdio.h>

long long num_passos = 1000000000;
double passo;

int main(){
	int i;
	double x, pi, soma=0.0;
    
	passo = 1.0/(double)num_passos;
    
    #pragma omp parallel for simd reduction(+:soma) schedule(simd:static)
	for(i=0; i < num_passos; i++){
		x = (i + 0.5)*passo;
		soma = soma + 4.0/(1.0 + x*x);
	}
	pi = soma*passo;

	printf("O valor de PI é: %.20lf\n", pi);
	return 0;

}