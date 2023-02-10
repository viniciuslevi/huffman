#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256

// ESTRUTURA DOS NODOS
typedef struct node
{
    unsigned char caracter;
    unsigned int freq; // a frequencia sera a prioridade da fila de prioridade
    struct node *next;
    struct node *right;
    struct node *left;
} NODE;

// Estrutura da fila de prioridade
typedef struct
{
    NODE *front;
    unsigned int size;
} PriorityQueue;

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


// 2 -- fila de prioridade ordenada

// inicializa a fila
void init_queue(PriorityQueue *queue)
{
    queue->front = NULL;
    queue->size = 0;
}

// Verifica se a fila está vazia
int is_empty(PriorityQueue *queue)
{
    return queue->front == NULL;
}

// Insere um nó na fila de prioridade
void enqueue(PriorityQueue *queue, NODE *node)
{

    // Se a fila estiver vazia
    if (is_empty(queue))
    {
        queue->front = node;
    }

    // Se o novo nó tiver prioridade menor que o primeiro elemento da fila
    else if (node->freq < queue->front->freq)
    {
        node->next = queue->front;
        queue->front = node;
    }

    // Se o novo nó tiver prioridade maior que o primeiro elemento da fila
    else
    {
        NODE *p = queue->front;
        while (p->next && p->next->freq < node->freq)
            p = p->next;
        node->next = p->next;
        p->next = node;
    }

    queue->size++;
}

// preenche a fila com os caracteres que aparecem no texto
void fill_queue(unsigned int *tabela, PriorityQueue *queue)
{
    for (int i = 0; i < TAM; i++)
    {
        if (tabela[i] > 0)
        {
            NODE *new = (NODE *)malloc(sizeof(NODE));
            new->caracter = i;
            new->freq = tabela[i];
            new->right = new->left = new->next = NULL;
            enqueue(queue, new);
        }
    }
}

// imprime a fila de prioridade (apenas para debug)
void print_queue(PriorityQueue *queue)
{
    NODE *aux = queue->front;
    printf("Imprimindo Fila!\n");
    printf("Tamanho: %d\n", queue->size);
    unsigned short int count = 0;
    while (aux)
    {
        printf("%u.\tchar: %c\toc:%u\t\n", ++count, aux->caracter, aux->freq);
        aux = aux->next;
    }
}

// 3 -- Montagem da arvore a partir da fila de prioridade

// Remove o primeiro elemento da fila de prioridade e retorna o nó
NODE *dequeue(PriorityQueue *queue)
{
    // Se a fila estiver vazia retorna NULL
    NODE *aux = NULL;

    // Se a fila não estiver vazia
    if (queue->front)
    {
        // Remove o primeiro elemento da fila
        aux = queue->front;
        queue->front = aux->next;
        aux->next = NULL;
        queue->size--;
    }

    // Retorna o nó removido
    return aux;
}

// monta a arvore a partir da fila de prioridade
NODE *montar_arvore(PriorityQueue *queue)
{
    NODE *primeiro, *segundo, *novo;
    while (queue->size > 1)
    {
        // remove os dois primeiros elementos da fila de prioridade e os armazena em primeiro e segundo
        primeiro = dequeue(queue);
        segundo = dequeue(queue);
        // cria um novo nó e o insere na fila de prioridade ordenada de acordo com a sua frequencia (prioridade)
        novo = malloc(sizeof(NODE));
        novo->freq = primeiro->freq + segundo->freq;
        novo->left = primeiro;
        novo->right = segundo;
        novo->next = NULL;
        enqueue(queue, novo);
    }
    return queue->front;
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
    // aloca a matriz de strings (dicionario) e retorna o ponteiro para a primeira linha da matriz
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
    // cria duas strings para armazenar os caminhos da esquerda e da direita
    char esquerda[colunas], direita[colunas];

    // se o nó for uma folha (não tem filhos) adiciona o caminho no dicionario
    if (raiz->left == NULL && raiz->right == NULL)
    {
        strcpy(dicionario[raiz->caracter], caminho);
    }

    // se o nó não for uma folha, chama a função recursivamente para os filhos da esquerda e da direita 
    // concatenando o caminho com 0 ou 1 para indicar a direção do caminho (esquerda ou direita)
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
            {
                // depois de manipular o byte de forma a formar o codigo parecido com o da string.
                // Em seguida, mandamos o byte formado para o arquivo e reiniciamos
                // o processo em busca de gerar o proximo byte.
                fwrite(&byte, sizeof(unsigned char), 1, file);

                byte = 0;
                j = 7;
            }
            
        }
        if (j != 7)
                fwrite(&byte, sizeof(unsigned char), 1, file);
            fclose(file);
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
    unsigned char byte;
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

                if (!aux->left && !aux->right)
                {
                    printf("%c", aux->caracter);
                    aux = raiz;
                }
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
    FILE *arq = fopen("input.txt", "r");
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

    // 2 -- fila de prioridade
    PriorityQueue queue;
    init_queue(&queue);
    fill_queue(tabela_frequencia, &queue);
    print_queue(&queue);

    // 3 -- Montagem da arvore a partir da fila de prioridade
    NODE *arvore;
    arvore = montar_arvore(&queue);
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

    /*até aqui codificamos em formato de string, a ideia agora é transformar uma
    cadeia de 8 caractéres em um unico byte*/

    // 7 -- Criar arquivo COMPACTADO
     compactar(codificado);

    // 8 -- Descompactando arquivo
     printf("\nARQUIVO DESCOMPACTADO!\n");
     descompactar(arvore);
     printf("\n\n");

     free(text);
     free(codificado);
     free(decodificado);
     clear_dicionario(dicionario);
     clear_arvore(arvore);
}