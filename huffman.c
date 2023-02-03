#include <stdio.h>
#include <stdlib.h>

#define TAM 256

// ESTRUTURA fila ORDENADA
typedef struct node {
    unsigned char caracter;
    unsigned int freq;
    struct node * next;
    struct node * right;
    struct node * left;
} NODE;

typedef struct {
    NODE *  inicio;
    unsigned int tam;
} LISTA;

// 1 -- tabela de frequencia
void inicializa_tabela_com_zero(unsigned int *t)
{
    for (int i = 0; i < TAM; i++)
        t[i] = 0;
}

void preenche_tab_frequencia(unsigned int *t, char *text)
{
    for(int i = 0; text[i]; i++)
        t[text[i]]++;
}

void imprime_tab_frequencia(unsigned int *t)
{
    for (int i = 0; i < TAM; i++)
        if (t[i])
            printf("\t%d = %u = %c\n", i, t[i], i);
}


// 2 -- lista encadeada ordenada

NODE * get_next_node (NODE * N) {
    return N->next;
}

void criar_lista (LISTA * L) {
    L->inicio = NULL;
    L->tam = 0;
}

void inserir_ordenado (LISTA * L, NODE * N) {
    // lista nao vazia
    if(L->inicio){
        // tem frequencia menor do que o inicio da lista (novo primeiro)
        if(N->freq < L->inicio->freq){
            N->next = L->inicio;
            L->inicio = N;
        }
        // tem frequencia maior do que o inicio da lista (insercao no meio ou fim)
        else {
            NODE * aux = L->inicio;
            // substituir abaixo por aux e aux->next
            while(get_next_node(aux) && get_next_node(aux)->freq < N->freq)
                aux = aux->next;    
            N->next = aux->next;
            aux->next = N;
        }
    }
    // lista vazia
    else {
      L->inicio = N;  
    }
    L->tam++;
}

/*
     Procedimento para preencher a lista a partir da tabela de frequência
*/

void preencher_lista (unsigned int * tabela, LISTA * L) {
    int i;
    NODE * new;
    for(i = 0; i<TAM; i++){
        if(tabela[i] > 0){
            new = (NODE*) malloc(sizeof(NODE));
            if(new){

                new->caracter = i;
                new->freq = tabela[i];
                new->right = NULL;
                new->left = NULL;
                new->next = NULL;
                inserir_ordenado(L, new);
            }
            else {
                printf("\tErro ao alocar memoria em prencher_lista()!\n");
                break; // o problema era o break
            }
        }
    }
}

void print_list (LISTA * L) {
    NODE * aux = L->inicio;
    printf("Imprimindo Lista!\n");
    printf("Tamanho: %d\n", L->tam);
    unsigned short int count = 0;
    while(aux){
        printf("%u.\tchar: %c\toc:%u\t\n", ++count, aux->caracter, aux->freq);
        aux = aux->next;
    }
}


// 3 -- Montagem da arvore a partir da fila/lista ordenada
NODE * remove_no_inicio (LISTA *L){
    NODE * aux = NULL;

    if(L->inicio){
        aux = L->inicio;
        L->inicio = aux->next;
        aux->next = NULL;
        L->tam--;
    }

    return aux;
}

/*
     Procedimento para montar a árvore de huffman.
*/
NODE* montar_arvore(LISTA *lista){
    NODE *primeiro, *segundo, *novo;
    while(lista->tam > 1){
        primeiro = remove_no_inicio(lista);
        segundo = remove_no_inicio(lista);
        novo = malloc(sizeof(NODE));

        if(novo){
            novo->caracter = '+';
            novo->freq = primeiro->freq + segundo->freq;
            novo->left = primeiro;
            novo->right = segundo;
            novo->next = NULL;

            inserir_ordenado(lista, novo);
        }
        else{
            printf("\n\tERRO ao alocar memoria em montar_arvore!\n");
            break;
        }
    }
    return lista->inicio;
}

/*
      Procedimento para imprimir na tela a árvore de huffman.
*/
void imprimir_arvore(NODE *raiz, int tam){
    if(raiz->left == NULL && raiz->right == NULL)
        printf("\tFolha: %c\tAltura: %d\n", raiz->caracter, tam);
    else{
        imprimir_arvore(raiz->left, tam + 1);
        imprimir_arvore(raiz->right, tam + 1);
    }
}

int main(void)
{
    unsigned char text[] = "teste";
    unsigned int tabela_frequencia[TAM];

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
    NODE * arvore;
    arvore = montar_arvore(&list);
    printf("\n\tImprimindo arvore de huffman\n");
    imprimir_arvore(arvore, 0);

}