#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* ------------------------- Utilidades ------------------------- */

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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

/* Função para rolar dado */
int rolarDado(int lados) {
    return rand() % lados + 1;
}

/* Função para sortear missão */
int sortearMissao(int totalMissoes) {
    return rand() % totalMissoes;
}

/* ------------------------- Exibição de mapa ------------------------- */

void exibirMapa(Territorio* mapa, int n) {
    printf("\n--- Mapa Atual ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Território: %-20s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

/* ------------------------- Cadastro de territórios ------------------------- */

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n***** Cadastro de Território %d *****\n", i);

        printf("Digite o nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        if (strlen(mapa[i].nome) == 0) strcpy(mapa[i].nome, "SemNome");

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;
        if (strlen(mapa[i].cor) == 0) strcpy(mapa[i].cor, "neutra");

        mapa[i].tropas = lerIntSeguro("Digite o número de tropas: ");
        if (mapa[i].tropas < 0) mapa[i].tropas = 0;
    }
}

/* ------------------------- Ataque ------------------------- */

void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return;

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Você não pode atacar um território da mesma cor.\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("O território atacante precisa de mais de 1 tropa.\n");
        return;
    }

    int dadoAtacante = rolarDado(6);
    int dadoDefensor = rolarDado(6);

    printf("\nRolagem de dados:\n");
    printf("Atacante (%s - %s): %d\n", atacante->nome, atacante->cor, dadoAtacante);
    printf("Defensor (%s - %s): %d\n", defensor->nome, defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;

        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;
        strcpy(defensor->cor, atacante->cor);

        printf("\n>> Resultado: VITÓRIA do Atacante!\n");
    } else if (dadoAtacante < dadoDefensor) {
        atacante->tropas -= 1;
        printf("\n>> Resultado: VITÓRIA do Defensor!\n");
    } else {
        printf("\n>> Resultado: Empate.\n");
    }
}

/* ------------------------- Missões ------------------------- */

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = sortearMissao(totalMissoes);
    strcpy(destino, missoes[idx]);
}

void exibirMissao(const char* nomeJogador, const char* corJogador, const char* missao) {
    printf("\nMissão de %s (cor %s): %s\n", nomeJogador, corJogador, missao);
}

/* ------------------------- Main ------------------------- */

int main() {
    srand(time(NULL)); // inicializa rand()

    printf("===== War Estruturado: Missões Estratégicas =====\n");

    int total = lerIntSeguro("Digite o número total de territórios: ");
    if (total <= 0) {
        printf("Número de territórios inválido.\n");
        return 0;
    }

    Territorio* mapa = (Territorio*)calloc(total, sizeof(Territorio));
    cadastrarTerritorios(mapa, total);

    /* Vetor de missões */
    char* missoesBase[] = {
        "Possuir ao menos 3 territórios da sua cor",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar ao menos metade do mapa",
        "Ter pelo menos 20 tropas em seus territórios",
        "Eliminar todas as tropas da cor azul"
    };
    int totalMissoes = sizeof(missoesBase) / sizeof(missoesBase[0]);

    /* Exemplo de atribuição de missão para um jogador */
    char missaoJogador[120];
    atribuirMissao(missaoJogador, missoesBase, totalMissoes);
    exibirMissao("Jogador1", "vermelha", missaoJogador);

    /* Exemplo de ataque */
    if (total >= 2) {
        atacar(&mapa[0], &mapa[1]);
    }

    free(mapa);
    return 0;
}
