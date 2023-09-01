//bibliotecas utilizadas
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//a seguir as estruturas de structs que utilizarei com os dados que desejo armazenar
struct Carro {
    //julguei as informações mais importantes pensando no Detram
    char placa[7]; 
    char modelo[50];
    int anoFabricacao;
    int chassi;
};

struct DonoCarro {
    char nome[100];
    char endereco[100];
    int CPF; //como sao so donos e nao necessariamente motoristas eu usei cpf em vez de cnh
    int totalCarros;
    struct Carro carro[100];
};

int main() {
    //nesse problema vamos poder cadastrar ate 100 pessoas e cada uma com ate 100 carros
    struct DonoCarro proprietarios[100];
    int numeroProprietarios = 0; //incializando com 0 pois vamos subindo o indice e armazenando

    while (1) {
        printf("Cadastro de Proprietário %d:\n", numeroProprietarios + 1); //o +1 eh pois subimos o indice
        printf("Nome: ");
        scanf("%s", proprietarios[numeroProprietarios].nome);
        printf("Endereço: ");
        scanf("%s", proprietarios[numeroProprietarios].endereco);

        printf("CPF: ");
        scanf("%d", &proprietarios[numeroProprietarios].CPF);

        printf("Quantidade de carros: ");
        scanf("%d", &proprietarios[numeroProprietarios].totalCarros);
        //tratando erros para caso coloque numeros negativos ou mais que mil carros
        if (proprietarios[numeroProprietarios].totalCarros <= 0 || proprietarios[numeroProprietarios].totalCarros > 100) {
            printf("Quantidade de carros inválida.\n");
            return 1;
        }
        //vamos fazer um laco passando pela quantidade de carros que se deseja cadastrar e armazenando os dados no indice adequado
        for (int i = 0; i < proprietarios[numeroProprietarios].totalCarros; i++) {
            printf("Carro %d:\n", i + 1);
            printf("Placa: ");
            scanf("%s", proprietarios[numeroProprietarios].carro[i].placa);
            printf("Modelo: ");
            scanf("%s", proprietarios[numeroProprietarios].carro[i].modelo);
            printf("Ano de Fabricação: ");
            scanf("%d", &proprietarios[numeroProprietarios].carro[i].anoFabricacao);
            printf("Chassi: ");
            scanf("%d", &proprietarios[numeroProprietarios].carro[i].chassi);
        }

        numeroProprietarios++; //pegando o proximo indice para nao misturar

        char continuar;
        printf("Deseja cadastrar outro proprietário? (S/N): ");
        scanf(" %c", &continuar);
        if (continuar != 'S' && continuar != 's') {
            break;
        }
    }
    //tratando erros para caso nao tenha dados armazenados
    FILE *arquivo = fopen("dados.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fwrite(&numeroProprietarios, sizeof(int), 1, arquivo);
    fwrite(proprietarios, sizeof(struct DonoCarro), numeroProprietarios, arquivo);

    fclose(arquivo);

    // tratando erros para caso o arquivo esteja vazio
    arquivo = fopen("dados.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int numPropLidos;
    fread(&numPropLidos, sizeof(int), 1, arquivo);

    struct DonoCarro proprietariosLidos[100];
    fread(proprietariosLidos, sizeof(struct DonoCarro), numPropLidos, arquivo);

    fclose(arquivo);
    //caso contrario gerará um arquivo binário que pode ser compilado e visualizado

    //printo os dados armazenados corretamente para ser visualizado via terminal tambem
    printf("\nDados cadastrados:\n");
    for (int i = 0; i < numPropLidos; i++) {
        printf("\n");
        printf("Cadastro de Proprietário %d:\n", i + 1);
        printf("Nome: %s\n", proprietariosLidos[i].nome);
        printf("Endereço: %s\n", proprietariosLidos[i].endereco);
        printf("CPF: %d\n", proprietariosLidos[i].CPF);
        printf("Total de carros: %d\n", proprietariosLidos[i].totalCarros);
        for (int j = 0; j < proprietariosLidos[i].totalCarros; j++) {
            printf("\n");
            printf("Carro %d:\n", j + 1);
            printf("Placa: %s\n", proprietariosLidos[i].carro[j].placa);
            printf("Modelo: %s\n", proprietariosLidos[i].carro[j].modelo);
            printf("Ano de Fabricação: %d\n", proprietariosLidos[i].carro[j].anoFabricacao);
            printf("Chassi: %d\n", proprietariosLidos[i].carro[j].chassi);
        }
        printf("\n");
    }

    return 0;
}
