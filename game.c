# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// Cadastrar cinco territórios com seus respectivos nomes, tropas e cor do exercito

struct Territorio {
    char nome[50];
    char cor[10];
    int tropas;
};

// --- Função para limpar o buffer de entrada ---
void limparBufferEtrada() {
    int c;
    while (( c = getchar()) != '\n' && c != EOF);
};

int main() {
    struct Territorio territorios[5];
    int totalTerritorios = 0;

    

    do {
        printf("***** Cadastro de Territórios *****\n");

        printf("Digite o nome do território %d: ", totalTerritorios + 1);
        fgets(territorios[totalTerritorios].nome, sizeof(territorios[totalTerritorios].nome), stdin);
        territorios[totalTerritorios].nome[strcspn(territorios[totalTerritorios].nome, "\n")] = 0; // Remover o '\n' do final
        
        printf("Digite a cor do exército: ");
        fgets(territorios[totalTerritorios].cor, sizeof(territorios[totalTerritorios].cor), stdin);
        territorios[totalTerritorios].cor[strcspn(territorios[totalTerritorios].cor, "\n")] = 0; // Remover o '\n' do final
       
        printf("Digite o número de tropas: ");
        scanf("%d", &territorios[totalTerritorios].tropas);
        limparBufferEtrada(); // Limpar o buffer de entrada após o scanf
        
        totalTerritorios++;
        printf("\n");

    } while (totalTerritorios < 5);

    printf("\n***** Territórios Cadastrados *****\n");
    for (int i = 0; i < totalTerritorios; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exército: %s\n", territorios[i].cor);
        printf("Número de Tropas: %d\n\n", territorios[i].tropas);
    };

    return 0;
};

