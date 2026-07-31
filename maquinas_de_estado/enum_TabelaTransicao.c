#include <stdio.h>
#include <stdlib.h>

typedef enum{
    ESTADO_AGUARDANDO,
    ESTADO_VALIDANDO,
    ESTADO_LIBERANDO,
    ESTADO_ERRO,
    ESTADO_CONFIRMADO,
    ESTADO_DEVOLUCAO
} Estado;

typedef enum {
    EVENTO_LEITURA_QR,       
    EVENTO_VALIDACAO_OK,     
    EVENTO_VALIDACAO_FALHA,  
    EVENTO_GUARDA_CHUVA_PULL,
    EVENTO_DEVOLUCAO_SLOT,   
    EVENTO_RESET             
} Evento;

typedef void (*Acao)(void);

void AcaoAguardando(void){ printf("Aguardando QR Code...\n"); }
void AcaoValidada(void){ printf("QR Code liberado, verificando saldo...\n"); }
void AcaoLiberada(void){ printf("Transação Aprovada\n");}
void AcaoErro(void){ printf("Saldo Insuficiente\n"); }
void AcaoConfirmada(void){ printf("Retirada com Sucesso\n"); }
void AcaoDevolucao(void){ printf("Devolutiva feita com sucesso\n"); }

typedef struct 
{
    Estado estadoAtual;
    Evento escolha;
    Estado proximoEstado;
    Acao acaoExecutar;
} Transicao;

const Transicao tabela[] = {
    { ESTADO_AGUARDANDO, EVENTO_LEITURA_QR , ESTADO_VALIDANDO, AcaoValidada },
    { ESTADO_VALIDANDO, EVENTO_VALIDACAO_OK, ESTADO_LIBERANDO, AcaoLiberada },
    { ESTADO_LIBERANDO, EVENTO_GUARDA_CHUVA_PULL, ESTADO_CONFIRMADO, AcaoConfirmada},
    { ESTADO_CONFIRMADO, EVENTO_RESET, ESTADO_AGUARDANDO, NULL},

    { ESTADO_AGUARDANDO, EVENTO_DEVOLUCAO_SLOT, ESTADO_DEVOLUCAO, AcaoDevolucao},
    { ESTADO_DEVOLUCAO, EVENTO_RESET, ESTADO_AGUARDANDO, NULL},

    { ESTADO_VALIDANDO, EVENTO_VALIDACAO_FALHA, ESTADO_ERRO, AcaoErro},
    { ESTADO_ERRO, EVENTO_RESET, ESTADO_AGUARDANDO, NULL}
};

#define TAMANHO_TABELA (sizeof(tabela) / sizeof(tabela[0]))

Estado ProcessarEvento(Estado estadoAtual, char escolha){
    for(size_t i = 0; i < TAMANHO_TABELA; i++){
        if(tabela[i].estadoAtual == estadoAtual && tabela[i].escolha == escolha){
            if(tabela[i].acaoExecutar != NULL){
                tabela[i].acaoExecutar();
            }
            return tabela[i].proximoEstado;
        }
    }
    AcaoErro();
    return (escolha == '*') ? ESTADO_AGUARDANDO:ESTADO_VALIDANDO;
}

Evento LerEventoDoTeclado(void) {
    char opcao;
    printf("\nDigite: [1] QR Code | [2] Sucesso API | [3] Falha API | [4] Puxar Guarda-Chuva | [5] Devolver Guarda-Chuva: ");
    scanf(" %c", &opcao);

    switch (opcao) {
        case '1': return EVENTO_LEITURA_QR;
        case '2': return EVENTO_VALIDACAO_OK;
        case '3': return EVENTO_VALIDACAO_FALHA;
        case '4': return EVENTO_GUARDA_CHUVA_PULL;
        case '5': return EVENTO_DEVOLUCAO_SLOT;
        default:  return EVENTO_RESET;
    }
}

int main(void) {
    Estado estado = ESTADO_AGUARDANDO;

    while (1) {
        Evento ev = LerEventoDoTeclado();
        estado = ProcessarEvento(estado, ev);
    }

    return 0;
}




