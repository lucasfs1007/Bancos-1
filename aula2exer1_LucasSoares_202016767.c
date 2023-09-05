#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Carro {
    char placa[7];
    char modelo[50];
    int anoFabricacao;
    int chassi;
};

struct DonoCarro {
    char nome[100];
    char endereco[100];
    int CPF;
    struct Carro *carros;
    int totalCarros;
};

int verificarCPF(struct DonoCarro *proprietarios, int totalProprietarios, int novoCPF) {
    for (int i = 0; i < totalProprietarios; i++) {
        if (proprietarios[i].CPF == novoCPF) {
            return 1; // CPF ja existe
        }
    }
    return 0; //CPF nao existe
}

int verificarPlaca(struct DonoCarro *proprietarios, int totalProprietarios, char *novaPlaca) {
    for (int i = 0; i < totalProprietarios; i++) {
        for (int j = 0; j < proprietarios[i].totalCarros; j++) {
            if (strcmp(proprietarios[i].carros[j].placa, novaPlaca) == 0) {
                return 1; // placa ja existe
            }
        }
    }
    return 0; // placa nao existe
}

int verificarChassi(struct DonoCarro *proprietarios, int totalProprietarios, int novoChassi) {
    for (int i = 0; i < totalProprietarios; i++) {
        for (int j = 0; j < proprietarios[i].totalCarros; j++) {
            if (proprietarios[i].carros[j].chassi == novoChassi) {
                return 1; // chassi ja existe
            }
        }
    }
    return 0; // chassi nao existe
}

void salvarProprietario(struct DonoCarro *proprietarios, int totalProprietarios) {
    FILE *arquivo;
    arquivo = fopen("proprietarios.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < totalProprietarios; i++) {
        // escrevendo o cpf primeiro no arquivo para ser a chave primaria de identificacao
        fwrite(&proprietarios[i].CPF, sizeof(int), 1, arquivo);

        // em seguids, escrevendo os outros dados
        fwrite(proprietarios[i].nome, sizeof(char), sizeof(proprietarios[i].nome), arquivo);
        fwrite(proprietarios[i].endereco, sizeof(char), sizeof(proprietarios[i].endereco), arquivo);
        fwrite(&proprietarios[i].totalCarros, sizeof(int), 1, arquivo);

        for (int j = 0; j < proprietarios[i].totalCarros; j++) {
            fwrite(&proprietarios[i].carros[j], sizeof(struct Carro), 1, arquivo);
        }
    }
    //fechando conforme solicitado
    fclose(arquivo);
}

//vamos ver os dados cadastrados na sequenciad os arquivos pelo terminal 
void listarProprietarios(struct DonoCarro *proprietarios, int totalProprietarios) {
    for (int i = 0; i < totalProprietarios; i++) {
        printf("CPF: %d\n", proprietarios[i].CPF);
        printf("Nome: %s\n", proprietarios[i].nome);
        printf("Endereço: %s\n", proprietarios[i].endereco);
        printf("Total de carros: %d\n", proprietarios[i].totalCarros);
        for (int j = 0; j < proprietarios[i].totalCarros; j++) {
            printf("Carro %d:\n", j + 1);
            printf("Placa: %s\n", proprietarios[i].carros[j].placa);
            printf("Modelo: %s\n", proprietarios[i].carros[j].modelo);
            printf("Ano de Fabricação: %d\n", proprietarios[i].carros[j].anoFabricacao);
            printf("Chassi: %d\n", proprietarios[i].carros[j].chassi);
        }
        printf("\n");
    }
}

void ordenarProprietariosPorCPF(struct DonoCarro *proprietarios, int totalProprietarios) {
    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < totalProprietarios - 1; i++) {
            if (proprietarios[i].CPF > proprietarios[i + 1].CPF) {
                // trocar os elementos
                struct DonoCarro temp = proprietarios[i];
                proprietarios[i] = proprietarios[i + 1];
                proprietarios[i + 1] = temp;
                trocou = 1;
            }
        }
    } while (trocou);
}

int main() {
    int maxProprietarios = 100; 
    struct DonoCarro *proprietarios = malloc(maxProprietarios * sizeof(struct DonoCarro));
    int totalProprietarios = 0;

    while (1) {
        if (totalProprietarios >= maxProprietarios) {
            printf("Limite de proprietários atingido.\n");
            break;
        }

        struct DonoCarro *proprietario = &proprietarios[totalProprietarios];

        printf("Cadastro de Proprietário:\n");
        printf("Nome: ");
        scanf("%s", proprietario->nome);
        
        // verificar se o CPF jaexiste
        int novoCPF;
        do {
            printf("CPF: ");
            scanf("%d", &novoCPF);
            if (verificarCPF(proprietarios, totalProprietarios, novoCPF)) {
                printf("CPF já cadastrado. Insira um CPF único.\n");
            }
        } while (verificarCPF(proprietarios, totalProprietarios, novoCPF));

        proprietario->CPF = novoCPF;

        printf("Endereço: ");
        scanf("%s", proprietario->endereco);

        int numeroCarros = 0;
        proprietario->carros = malloc(sizeof(struct Carro)); // alocacao inicial

        do {
            if (numeroCarros > 0) {
                char continuar;
                printf("Deseja cadastrar outro carro? (S/N): ");
                scanf(" %c", &continuar);
                if (continuar != 'S' && continuar != 's') {
                    break;
                }
            }

            printf("Carro %d:\n", numeroCarros + 1);
            
            // verificando se a placa ja existe
            char novaPlaca[7];
            do {
                printf("Placa: ");
                scanf("%s", novaPlaca);
                if (verificarPlaca(proprietarios, totalProprietarios, novaPlaca)) {
                    printf("Placa já cadastrada. Insira uma placa única.\n");
                }
            } while (verificarPlaca(proprietarios, totalProprietarios, novaPlaca));
            
            strcpy(proprietario->carros[numeroCarros].placa, novaPlaca);
            
            printf("Modelo: ");
            scanf("%s", proprietario->carros[numeroCarros].modelo);
            
            // verificando se o chassi ja existe
            int novoChassi;
            do {
                printf("Ano de Fabricação: ");
                scanf("%d", &proprietario->carros[numeroCarros].anoFabricacao);
                printf("Chassi: ");
                scanf("%d", &novoChassi);
                if (verificarChassi(proprietarios, totalProprietarios, novoChassi)) {
                    printf("Chassi já cadastrado. Insira um chassi único.\n");
                }
            } while (verificarChassi(proprietarios, totalProprietarios, novoChassi));

            proprietario->carros[numeroCarros].chassi = novoChassi;

            numeroCarros++;

            proprietario->carros = realloc(proprietario->carros, (numeroCarros + 1) * sizeof(struct Carro));
        } while (1);

        proprietario->totalCarros = numeroCarros;
        totalProprietarios++;

        char cadastrarOutroProprietario;
        printf("Deseja cadastrar outro proprietário? (S/N): ");
        scanf(" %c", &cadastrarOutroProprietario);
        if (cadastrarOutroProprietario != 'S' && cadastrarOutroProprietario != 's') {
            break;
        }
    }

    // ordenando os proprietarios por CPF
    ordenarProprietariosPorCPF(proprietarios, totalProprietarios);

    // salvando os proprietarios em um arquivo binario
    salvarProprietario(proprietarios, totalProprietarios);

    // listando os dados dos proprietarios cadastrados
    printf("\nDados cadastrados (ordenados por CPF):\n");
    listarProprietarios(proprietarios, totalProprietarios);

    // liberando a memoria alocada
    for (int i = 0; i < totalProprietarios; i++) {
        free(proprietarios[i].carros);
    }
    free(proprietarios);

    return 0;
}
