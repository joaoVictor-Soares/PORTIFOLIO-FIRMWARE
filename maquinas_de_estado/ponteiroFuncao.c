#include <stdio.h>
#include <stdlib.h>

void (*PonteiroDeFuncao)();

void EstadoInicial(void);
void CaracterDigitadoA(void);
void CaracterDigitadoB(void);
void CaracterDigitadoC(void);

void EstadoInicial(void){
    char TeclaLida;
    fflush(stdout);
    TeclaLida = getchar();
    if (TeclaLida == '\n' || TeclaLida == '\r') {
        return;
    }

    if(TeclaLida == 'a'){
        PonteiroDeFuncao = CaracterDigitadoA;
        printf("Proximo estagio\n");
    }else{
        printf("Caracter esperado: a. Permanecendo no estado inical");
    }
}

void CaracterDigitadoA(void){
    char TeclaLida;
    fflush(stdout);
    TeclaLida = getchar();
    if (TeclaLida == '\n' || TeclaLida == '\r') {
        return;
    }

    if(TeclaLida == 'b'){
        PonteiroDeFuncao = CaracterDigitadoB;
        printf("Proximo estagio\n");
    }else{
        printf("Caracter esperado: b. Permanecendo no estado inical");
        PonteiroDeFuncao = EstadoInicial;
    }
}

void CaracterDigitadoB(void){
    char TeclaLida;
    fflush(stdout);
    TeclaLida = getchar();
    if (TeclaLida == '\n' || TeclaLida == '\r') {
        return;
    }

    if(TeclaLida == 'c'){
        PonteiroDeFuncao = CaracterDigitadoC;
        printf("Proximo estagio\n");
    }else{
        printf("Caracter esperado: c. Permanecendo no estado inical");
        PonteiroDeFuncao = EstadoInicial;
    }
}

void CaracterDigitadoC(void){
    char TeclaLida;
    fflush(stdout);
    TeclaLida = getchar();
    if (TeclaLida == '\n' || TeclaLida == '\r') {
        return;
    }

    if(TeclaLida != 'd'){
       printf("Caracter esperado: d. Caracter Lido: %c. Voltando ao estado inicial", TeclaLida);
    }else{
        printf("OK");
    }
    PonteiroDeFuncao = EstadoInicial;
}

int main(int argc, char const *argv[])
{
    PonteiroDeFuncao = EstadoInicial;

    while (1)
    {
        (*PonteiroDeFuncao)();
    }

    return 0;
}
