#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256

// ESTRUTURA fila ORDENADA
typedef struct node
{
    unsigned char caracter;
    unsigned int freq;
    struct node *next;
    struct node *right;
    struct node *left;
} NODE;

typedef struct
{
    NODE *inicio;
    unsigned int tam;
} LISTA;

// 1 -- tabela de frequencia

// inicializa a tabela com zeros
void inicializa_tabela_com_zero(unsigned int *t)
{
    for (int i = 0; i < TAM; i++)
        t[i] = 0;
}

// preenche a tabela com a frequencia de cada caracter
void preenche_tab_frequencia(unsigned int *t, char *text)
{
    for (int i = 0; text[i]; i++)
        t[text[i]]++;
}

// imprime a tabela de frequencia
void imprime_tab_frequencia(unsigned int *t)
{
    for (int i = 0; i < TAM; i++)
        if (t[i])
            printf("\t%d = %u = %c\n", i, t[i], i);
}


// 2 -- lista encadeada ordenada

// cria a lista
void criar_lista(LISTA *L)
{
    L->inicio = NULL;
    L->tam = 0;
}

// insere ordenado na lista encadeada
void inserir_ordenado(LISTA *L, NODE *N)
{
    // lista nao vazia
    if (L->inicio)
    {
        // tem frequencia menor do que o inicio da lista (novo primeiro)
        if (N->freq < L->inicio->freq)
        {
            N->next = L->inicio;
            L->inicio = N;
        }
        // tem frequencia maior do que o inicio da lista (insercao no meio ou fim)
        else
        {
            NODE *aux = L->inicio;
            // substituir abaixo por aux e aux->next
            while (aux->next && aux->next->freq < N->freq)
                aux = aux->next;
            N->next = aux->next;
            aux->next = N;
        }
    }
    // lista vazia
    else
    {
        L->inicio = N;
    }
    L->tam++;
}

// preenche a lista com os caracteres e suas frequencias
void preencher_lista(unsigned int *tabela, LISTA *L)
{
    int i;
    NODE *new;
    for (i = 0; i < TAM; i++)
    {
        if (tabela[i] > 0)
        {
            new = (NODE *)malloc(sizeof(NODE));
            if (new)
            {

                new->caracter = i;
                new->freq = tabela[i];
                new->right = NULL;
                new->left = NULL;
                new->next = NULL;
                inserir_ordenado(L, new);
            }
            else
            {
                printf("\tErro ao alocar memoria em prencher_lista()!\n");
                break;
            }
        }
    }
}

// imprime a lista
void print_list(LISTA *L)
{
    NODE *aux = L->inicio;
    printf("Imprimindo Lista!\n");
    printf("Tamanho: %d\n", L->tam);
    unsigned short int count = 0;
    while (aux)
    {
        printf("%u.\tchar: %c\toc:%u\t\n", ++count, aux->caracter, aux->freq);
        aux = aux->next;
    }
}


// 3 -- Montagem da arvore a partir da fila/lista ordenada

// remove o primeiro elemento da lista 
NODE *remove_no_inicio(LISTA *L)
{
    NODE *aux = NULL;

    if (L->inicio)
    {
        aux = L->inicio;
        L->inicio = aux->next;
        aux->next = NULL;
        L->tam--;
    }

    return aux;
}

// monta a arvore a partir da lista ordenada
NODE *montar_arvore(LISTA *lista)
{
    NODE *primeiro, *segundo, *novo;
    while (lista->tam > 1)
    {
        primeiro = remove_no_inicio(lista);
        segundo = remove_no_inicio(lista);
        novo = malloc(sizeof(NODE));

        if (novo)
        {
            novo->caracter = '+';
            novo->freq = primeiro->freq + segundo->freq;
            novo->left = primeiro;
            novo->right = segundo;
            novo->next = NULL;

            inserir_ordenado(lista, novo);
        }
        else
        {
            printf("\n\tERRO ao alocar memoria em montar_arvore!\n");
            break;
        }
    }
    return lista->inicio;
}

// imprime a arvore
void imprimir_arvore(NODE *raiz, int tam)
{
    if (raiz->left == NULL && raiz->right == NULL)
        printf("\tFolha: %c\tAltura: %d\n", raiz->caracter, tam);
    else
    {
        imprimir_arvore(raiz->left, tam + 1);
        imprimir_arvore(raiz->right, tam + 1);
    }
}

// ------------------  PARTE 4: Montar dicionario ------------------

// retorna a altura da arvore (usado para alocar o dicionario)
int altura_arvore(NODE *raiz)
{
    int left, right;
    if (!raiz)
        return -1;
    else
    {
        left = altura_arvore(raiz->left) + 1;
        right = altura_arvore(raiz->right) + 1;
        if (left > right)
            return left;
        else
            return right;
    }
}

// aloca o dicionario
char **aloca_dicionario(int colunas)
{
    char **dicionario = malloc(sizeof(char *) * TAM);
    for (int i = 0; i < TAM; i++)
    {
        dicionario[i] = calloc(colunas, sizeof(char));
    }
    return dicionario;
}

// gera o dicionario a partir da arvore
void gerar_dicionario(char **dicionario, NODE *raiz, char *caminho, int colunas)
{
    char esquerda[colunas], direita[colunas];
    if (raiz->left == NULL && raiz->right == NULL)
    {
        strcpy(dicionario[raiz->caracter], caminho);
    }
    else
    {
        strcpy(esquerda, caminho);
        strcpy(direita, caminho);

        strcat(esquerda, "0");
        strcat(direita, "1");

        gerar_dicionario(dicionario, raiz->left, esquerda, colunas);
        gerar_dicionario(dicionario, raiz->right, direita, colunas);
    }
}

// imprime o dicionario
void imprime_dicionario(char **dicionario)
{
    printf("\tDicionario:\n");
    for (int i = 0; i < TAM; i++)
        if (strlen(dicionario[i]) > 0)
            printf("\t%3d %s\n", i, dicionario[i]);
}


// -------------------------- PARTE 5: Codificar -----------------------

// retorna o tamanho do codigo do texto usando o dicionario passado como parametro
int calcula_tamanho_string(char **dicionario, unsigned char *texto)
{
    int tam = 0;
    for (int i = 0; texto[i]; i++)           // percorre o texto
        tam += strlen(dicionario[texto[i]]); // soma o tamanho de cada codigo
    return tam + 1;                          // caracteres + 1 fim de string
}

// retorna o codigo do texto usando o dicionario passado como parametro
char *codificar(char **dicionario, unsigned char *texto)
{
    int tam = calcula_tamanho_string(dicionario, texto); // calcula o tamanho do codigo
    char *codigo = calloc(tam, sizeof(char));            // reserva espaco para o codigo

    for (int i = 0; texto[i]; i++)            // percorre o texto
        strcat(codigo, dicionario[texto[i]]); // concatena o codigo

    return codigo; // retorna o codigo
}

// --------------------- PARTE 6: Decodificacao ----------------------

// retorna o texto decodificado usando a arvore passada como parametro

char *decodificar(unsigned char *codigo, NODE *raiz)
{
    NODE *aux = raiz;                                          // auxiliar para percorrer a arvore
    char *decodificado = calloc(strlen(codigo), sizeof(char)); // reserva espaco para o texto decodificado
    char temp[2];                                              // auxiliar para concatenar o caracter no texto decodificado

    for (int i = 0; codigo[i]; i++)
    {                         // percorre o codigo
        if (codigo[i] == '0') // se for 0 vai para a esquerda
            aux = aux->left;
        else // se for 1 vai para a direita
            aux = aux->right;
        if (aux->left == NULL && aux->right == NULL)
        {                               // se for uma folha
            temp[0] = aux->caracter;    // pega o caracter
            temp[1] = '\0';             // coloca o fim de string
            strcat(decodificado, temp); // concatena o caracter no texto decodificado
            aux = raiz;                 // volta para a raiz
        }
    }
    return decodificado; // retorna o texto decodificado
}

// ------------------- PARTE 7 : Compactar ----------------

void compactar(unsigned char *str)
{
    int i = 0, j = 7;
    unsigned char mascara, byte = 0;
    FILE *file = fopen("compactado.cpt", "wb");

    if (file)
    {
        while (str[i])
        {
            mascara = 1;
            if (str[i++] == '1')
            {
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if (j < 0)
            { // depois de manipular o byte de forma a formar o codigo parecido com o da string. Em seguida, mandamos o byte formado para o arquivo e reiniciamos o processo em busca de gerar o proximo byte.
                fwrite(&byte, sizeof(unsigned char), 1, file);

                byte = 0;
                j = 7;
            }
            if (j != 7)
                fwrite(&byte, sizeof(unsigned char), 1, file);
            fclose(file);
        }
    }
    else
    {
        printf("\n\nErro ao abrir/crair arquivo em compactar\n");
    }
}

// ------------------ parte 8: Descompactar --------------------
unsigned int eh_bit_um(unsigned char byte, int i)
{
    unsigned char mascara = (1 << i);
    return byte & mascara;
}

void descompactar(NODE *raiz)
{
    int i;
    NODE *aux = raiz;
    unsigned char byte = 0;
    FILE *file = fopen("compactado.cpt", "rb");
    if (file)
    {
        while (fread(&byte, sizeof(unsigned char), 1, file))
        {
            for (i = 7; i >= 0; i--)
            {
                if (eh_bit_um(byte, i))
                    aux = aux->right;
                else
                    aux = aux->left;
            }
            if (!aux->left && !aux->right)
            {
                printf("%c", aux->caracter);
                aux = raiz;
            }
        }
        fclose(file);
    }
    else
    {
        printf("\nErro ao abrir arquivo em desccompactar\n");
    }
}

int descobrir_tamanho()
{
    FILE *arq = fopen("teste.txt", "r");
    int tam = 0;
    if (arq)
    {
        while (fgetc(arq) != -1)
            tam++;
        fclose(arq);
    }
    else
        printf("\nErro ao abrir arquivo em descobrir_tamanho \n");

    return tam;
}

void ler_texto(unsigned char *texto)
{
    FILE *arq = fopen("input.txt", "r");
    char letra;
    int i = 0;
    if (arq)
    {
        while (!feof(arq))
        {
            letra = fgetc(arq);
            if (letra != -1)
            {
                texto[i++] = letra;
            }
        }
        fclose(arq);
    }
    else
        printf("\nErro ao abrir arquivo em ler texto\n");
}
void clear_dicionario(char **dicionario)
{
}
void clear_arvore(NODE *arvore)
{
}

int main(void)
{
    unsigned char *text;
    unsigned int tabela_frequencia[TAM], tam;
    char **dicionario;
    int colunas;
    char *codificado;
    char *decodificado;

    tam = descobrir_tamanho();
    printf("\nQuantidade; %d\n", tam);
    text = calloc(tam + 2, sizeof(unsigned char));
    ler_texto(text);
    printf("\nTEXTO\n%s\n", text);

    // 1 -- tabela de frequencia
    inicializa_tabela_com_zero(&tabela_frequencia);
    preenche_tab_frequencia(&tabela_frequencia, text);
    imprime_tab_frequencia(&tabela_frequencia);

    // 2 -- lista encadeada ordenada
    LISTA list;
    criar_lista(&list);
    preencher_lista(tabela_frequencia, &list);
    print_list(&list);

    // 3 -- Montagem da arvore a partir da lista/fila ordenada
    NODE *arvore;
    arvore = montar_arvore(&list);
    printf("\n\tImprimindo arvore de huffman\n");
    imprimir_arvore(arvore, 0);

    // 4 -- Montagem do dicionario para o algoritmo
    colunas = altura_arvore(arvore) + 1;
    dicionario = aloca_dicionario(colunas);
    gerar_dicionario(dicionario, arvore, "", colunas);
    imprime_dicionario(dicionario);

    // 5 -- Codificando o texto
    codificado = codificar(dicionario, text);
    printf("\n\tTexto codificado: %s\n", codificado);

    // 6 -- Decodificando o texto
    decodificado = decodificar(codificado, arvore);
    printf("\n\tTexto decodificado: %s\n", decodificado);

    /*até aqui codificamos em formato de string, a ideia agora é transformar uma cadeia de 8 caractéres em um unico byte*/

    // 7 -- Criar arquivo COMPACTADO
    // compactar(codificado);

    // 8 -- Descompactando arquivo
    // printf("\nARQUIVO DESCOMPACTADO!\n");
    // descompactar(arvore);
    // printf("\n\n");

    // free(text);
    // free(codificado);
    // free(decodificado);
    // clear_dicionario(dicionario);
    // clear_arvore(arvore);
}