#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[50];
    char cor[10];
    int tropas;
} Territorio;

// --- Função para limpar o buffer de entrada (Corrigido o nome) ---
void limparBufferEntrada() {
    int c;
    while (( c = getchar()) != '\n' && c != EOF);
}

/* Lê um inteiro de forma segura */
int lerIntSeguro(const char* prompt) {
    int valor;
    int ok;
    do {
        printf("%s", prompt);
        ok = scanf("%d", &valor);
        if (ok != 1) {
            printf("Entrada inválida. Digite um número inteiro.\n");
            limparBufferEntrada();
        }
    } while (ok != 1);
    limparBufferEntrada();
    return valor;
}

/* Função ADICIONADA: Exibe os territórios para o usuário escolher */
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n--- Mapa Atual ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Território: %-20s | Cor: %-10s | Tropas: %d\n", 
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

/* Cadastra n territórios usando ponteiros */
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n***** Cadastro de Território %d *****\n", i); // Ajustei para mostrar indice real

        printf("Digite o nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;

        mapa[i].tropas = lerIntSeguro("Digite o número de tropas: ");
        if (mapa[i].tropas < 0) mapa[i].tropas = 0;
    }
}

/* Simula um ataque entre dois territórios por ponteiro */
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) {
        printf("Erro: ponteiros inválidos.\n");
        return;
    }

    /* Validação de cor: não pode atacar território próprio */
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Você não pode atacar um território da mesma cor.\n");
        return;
    }

    /* Validação de tropas mínimas para atacar */
    /* Regra War: Precisa de pelo menos 2 tropas (1 ataca, 1 fica) */
    if (atacante->tropas <= 1) {
        printf("O território atacante precisa de mais de 1 tropa para atacar.\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\nRolagem de dados:\n");
    printf("Atacante (%s - %s): %d\n", atacante->nome, atacante->cor, dadoAtacante);
    printf("Defensor (%s - %s): %d\n", defensor->nome, defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        /* Vitória do atacante: defensor muda de cor e recebe metade das tropas do atacante */
        int tropasTransferidas = atacante->tropas / 2;  /* arredonda para baixo */
        
        /* Garante que transfere pelo menos 1 se tiver tropas suficientes */
        if (tropasTransferidas < 1) tropasTransferidas = 1; 

        if (atacante->tropas > tropasTransferidas) {
            atacante->tropas -= tropasTransferidas;
            defensor->tropas = tropasTransferidas; // Defensor passa a ter as tropas que invadiram
        } else {
             // Fallback caso cálculo dê erro, mantém 1 no atacante
             defensor->tropas = atacante->tropas - 1;
             atacante->tropas = 1;
        }
        strcpy(defensor->cor, atacante->cor);

        printf("\n>> Resultado: VITORIA do Atacante!\n");
        printf("O território defensor agora pertence à cor '%s'.\n", defensor->cor);
        printf("%d tropas foram movidas para o novo território.\n", defensor->tropas);
        
    } else if (dadoAtacante < dadoDefensor) {
        /* Derrota do atacante: perde uma tropa */
        if (atacante->tropas > 0) {
            atacante->tropas -= 1;
        }
        printf("\n>> Resultado: VITORIA do Defensor!\n");
        printf("O atacante perdeu 1 tropa. Restantes: %d.\n", atacante->tropas);
        
    } else {
        /* Empate */
        printf("\n>> Resultado: Empate. Nenhuma alteração.\n");
    }
}

/* Libera a memória do vetor de territórios */
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

/* Menu de interação para ataques contínuos */
void loopAtaques(Territorio* mapa, int n) {
    int continuar = 1;
    while (continuar) {
        
        // Chamada da função que estava faltando
        exibirTerritorios(mapa, n);

        int idxAtacante = lerIntSeguro("Selecione o índice do território ATACANTE: ");
        int idxDefensor = lerIntSeguro("Selecione o índice do território DEFENSOR: ");

        /* Validação de índices */
        if (idxAtacante < 0 || idxAtacante >= n || idxDefensor < 0 || idxDefensor >= n) {
            printf("Índices inválidos. Tente novamente.\n");
        } else if (idxAtacante == idxDefensor) {
            printf("Um território não pode atacar a si mesmo.\n");
        } else {
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        }

        printf("\nDeseja realizar outro ataque? (1 - Sim, 0 - Não): ");
        int resp;
        int ok = scanf("%d", &resp);
        if (ok != 1) {
            limparBufferEntrada();
            resp = 0;
        } else {
            limparBufferEntrada();
        }
        continuar = resp != 0;
    }
}

int main() {
    /* Inicializa gerador de números aleatórios */
    srand(time(NULL));

    printf("===== War Estruturado: Cadastro e Ataques =====\n");
    int total = lerIntSeguro("Digite o número total de territórios: ");
    if (total <= 0) {
        printf("Número de territórios inválido.\n");
        return 0;
    }

    /* Alocação dinâmica */
    Territorio* mapa = (Territorio*)calloc(total, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Falha ao alocar memória.\n");
        return 1;
    }

    /* Cadastro */
    cadastrarTerritorios(mapa, total);

    /* Interatividade de ataques */
    loopAtaques(mapa, total);

    /* Liberação de memória */
    liberarMemoria(mapa);

    printf("Encerrando o programa. Obrigado por jogar!\n");
    return 0;
}