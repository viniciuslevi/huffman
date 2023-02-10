#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000 // tamanho maximo do arquivo

void write_file(char *filename, char *str)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo %s", filename);
        exit(1);
    }
    fprintf(fp, "%s", str);
    fclose(fp);
}

void read_file(char *filename, char *str)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo %s", filename);
        exit(1);
    }
    char c = fgetc(fp);
    int i = 0;
    while (c != EOF)
    {
        str[i] = c;
        c = fgetc(fp);
        i++;
    }
    str[i] = '\0';
    fclose(fp);
}

int main(void)
{
    int opcao;
    while (1)
    {
        printf("1 - Escrever no arquivo\n");
        printf("2 - Ler do arquivo\n");
        printf("3 - Sair\n");
        scanf("%d", &opcao);

        // escrever no arquivo
        if (opcao == 1)
        {
            setbuf(stdin, NULL);
            printf("Digite o conteudo do arquivo: \n");
            char *conteudo = malloc(1001);
            scanf("%[^\n]", conteudo);
            write_file("arquivo.txt", conteudo);
            free(conteudo);
        }
        // ler do arquivo
        else if (opcao == 2)
        {
            char str[MAX];
            read_file("arquivo.txt", str);
            printf("Conteudo do arquivo: %s\n\n", str);
        }
        // sair
        else if (opcao == 3)
        {
            printf("saindo...\n");
            break;
        }
        setbuf(stdin, NULL);
    }
}