#include <stdio.h>
#include <stdlib.h>

#define ESTADO_INICIAL 0
#define ESTADO_DIGITOU_LETRA_A 1
#define ESTADO_DIGITOU_LETRA_B 2
#define ESTADO_DIGITOU_LETRA_C 3

volatile char ControleMaquinaEstado;

void ExecutarMaquinaEstado(void);

void ExecutarMaquinaEstado(void) {
    char TeclaLida;

    int c;
    do{
        TeclaLida = getchar();
    }while (TeclaLida == '\n');

    TeclaLida = getchar();

    switch (ControleMaquinaEstado) {
    case ESTADO_INICIAL:
        if (TeclaLida == 'a') {
            ControleMaquinaEstado = ESTADO_DIGITOU_LETRA_A;
        } else {
            ControleMaquinaEstado = ESTADO_INICIAL;
            printf("\n\nCaracter esperado: a. Caracter lido: %c. Voltando ao estado inicial\n\n", TeclaLida);
        }
        break;

    case ESTADO_DIGITOU_LETRA_A:
        if (TeclaLida == 'b') {
            ControleMaquinaEstado = ESTADO_DIGITOU_LETRA_B;
        } else {
            ControleMaquinaEstado = ESTADO_INICIAL;
            printf("\n\nCaracter esperado: b. Caracter lido: %c. Voltando ao estado inicial\n\n", TeclaLida);
        }
        break;

    case ESTADO_DIGITOU_LETRA_B:
        if (TeclaLida == 'c') {
            ControleMaquinaEstado = ESTADO_DIGITOU_LETRA_C;
        } else {
            ControleMaquinaEstado = ESTADO_INICIAL;
            printf("\n\nCaracter esperado: c. Caracter lido: %c. Voltando ao estado inicial\n\n", TeclaLida);
        }
        break;

    case ESTADO_DIGITOU_LETRA_C:
        if (TeclaLida == 'd') {
            printf("\n\nOK\n\n");
            ControleMaquinaEstado = ESTADO_INICIAL;
        } else {
            ControleMaquinaEstado = ESTADO_INICIAL;
            printf("\n\nCaracter esperado: d. Caracter lido: %c. Voltando ao estado inicial\n\n", TeclaLida);
        }
        break;

    default:
        break;
    }
}

int main(int argc, char const *argv[]) {
    ControleMaquinaEstado = ESTADO_INICIAL;

    printf("Digite a sequencia: a -> b -> c -> d\n");
    printf("(Pressione Enter apos cada caractere)\n\n");

    while (1) {
        ExecutarMaquinaEstado();
    }

    return 0;
}