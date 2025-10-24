#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct aluno{
    char nome[100];
    int idade;
    int matricula;
    struct aluno *proximo;
}Aluno;


typedef struct lse{
    Aluno *primeiro;
    int n_elementos;
    char nome[50];
}LSE;

void inicializaLista(LSE *lista, char nome[]){
    //recebe a estrutura de lista e preenche os parâmetros
    lista->primeiro = NULL;
    lista->n_elementos = 0;
    strcpy(lista->nome,nome);
}

void insereInicio(LSE *lista, Aluno *novo){

    if(lista->primeiro == NULL){
        novo->proximo = NULL;
    }else{
        novo->proximo = lista->primeiro;
    }
    lista->primeiro = novo;
    lista->n_elementos++;
}

void insereFim(LSE *lista, Aluno *novo){
    novo->proximo = NULL;
    if(lista->primeiro == NULL){
        lista->primeiro = novo;
    }else{
        Aluno *aux = lista->primeiro;
        while(aux->proximo!=NULL){
            aux = aux->proximo;
        }
        aux->proximo = novo;
    }
    lista->n_elementos++;
}

void inserePosicao(LSE *lista, Aluno *novo, int posicao){
    if(posicao < 0 || posicao > lista->n_elementos){
        printf("\n\t\t  **** Posicao Invalida! ****\n");
        return;
    }
    
    if(posicao == 0){
        insereInicio(lista, novo);
        return;
    }
    
    if(posicao == lista->n_elementos){
        insereFim(lista, novo);
        return;
    }
    
    Aluno *aux = lista->primeiro;
    for(int i = 0; i < posicao - 1; i++){
        aux = aux->proximo;
    }
    
    novo->proximo = aux->proximo;
    aux->proximo = novo;
    lista->n_elementos++;
}
   
Aluno* removeInicio(LSE *lista){
    Aluno *aux = lista->primeiro;
    if(aux != NULL){
        lista->primeiro = lista->primeiro->proximo;
        lista->n_elementos--;
        aux->proximo = NULL;
    }
    else{
        printf("\n\t\t  **** Lista Vazia! ****\n");
    }
    return aux;
}


Aluno* removeFim(LSE *lista){
    if(lista->primeiro == NULL){
        printf("\n\t\t  **** Lista Vazia! ****\n");
        return NULL;
    }
    
    if(lista->primeiro->proximo == NULL){
        Aluno *aux = lista->primeiro;
        lista->primeiro = NULL;
        lista->n_elementos--;
        aux->proximo = NULL;
        return aux;
    }
    
    Aluno *aux = lista->primeiro;
    while(aux->proximo->proximo != NULL){
        aux = aux->proximo;
    }
    
    Aluno *removido = aux->proximo;
    aux->proximo = NULL;
    lista->n_elementos--;
    return removido;
}
    

Aluno* removePosicao(LSE *lista, int posicao){
    if(posicao < 0 || posicao >= lista->n_elementos){
        printf("\n\t\t  **** Posicao Invalida! ****\n");
        return NULL;
    }
    
    if(posicao == 0){
        return removeInicio(lista);
    }
    
    if(posicao == lista->n_elementos - 1){
        return removeFim(lista);
    }
    
    Aluno *aux = lista->primeiro;
    for(int i = 0; i < posicao - 1; i++){
        aux = aux->proximo;
    }
    
    Aluno *removido = aux->proximo;
    aux->proximo = removido->proximo;
    lista->n_elementos--;
    removido->proximo = NULL;
    return removido;
}

void cadastraAluno(Aluno *aluno, char nome[], int idade, int matricula){
    
    strcpy(aluno->nome,nome);
    aluno->idade = idade;
    aluno->matricula = matricula;
    aluno->proximo = NULL;
}


Aluno* informaNovoAluno(){
    
    Aluno *novo = (Aluno*)(malloc(sizeof(Aluno)));
    printf("\nInforme o nome do Aluno:");
    scanf("%s",novo->nome);
    printf("Informe a idade do Aluno:");
    scanf("%d",&novo->idade);
    printf("Informe a matricula do Aluno:");
    scanf("%d",&novo->matricula);
    return novo;
}

void mostraAluno(Aluno novo){
    
    printf("\t\tAluno Nome: %s Idade %d Matricula %d\n",novo.nome,novo.idade,novo.matricula);
}

void mostraLista(LSE lista){
    
    printf("\n\tMostra Lista Simplesmente Encadeada; \n");
    Aluno *aux;
    aux = lista.primeiro;
    while(aux!=NULL){
        mostraAluno(*aux);
        aux = aux->proximo;
    }
    printf("\t\tTotal de alunos na Lista = %d \n",lista.n_elementos);
    printf("\tFim da Lista!\n");
}

void mostraLista2(Aluno *aluno){
   if(aluno!=NULL){
        mostraAluno(*aluno);
        mostraLista2(aluno->proximo);
    }
}

Aluno *localizaAluno(LSE lista, char nome[]){
    Aluno *aux = lista.primeiro;
    while(aux!=NULL){
        if(strcmp(aux->nome,nome)==0){
            return aux;
        }
        aux = aux->proximo;
    }
    return NULL;
}

Aluno *localizaPosicao(LSE lista, int posicao){
    if(posicao < 0 || posicao >= lista.n_elementos){
        printf("\n\t\t =====  Posicao Invalida! =====\n");
        return NULL;
    }
    
    Aluno *aux = lista.primeiro;
    for(int i = 0; i < posicao; i++){
        aux = aux->proximo;
    }
    return aux;
}

void apagaLista(LSE *lista){
    Aluno *aux;
    while(lista->primeiro != NULL){
        aux = removeInicio(lista);
        free(aux);
    }
}


void menuTesteLista(LSE *lista) {
    int op = 0, posicao = 0;
    Aluno *aux = NULL;
    printf("\nMenu de operacoes sobre um LSE:\n");
    printf("\n\t1 - Insere no Inicio:");
    printf("\n\t2 - Insere no Fim:");
    printf("\n\t3 - Insere na Posicao:");
    printf("\n\t4 - Remove no Inicio:");
    printf("\n\t5 - Remove no Fim:");
    printf("\n\t6 - Remove na Posicao:");
    printf("\n\t7 - Mostra Lista:");
    printf("\n\t8 - Mostra Aluno na Posicao:");
    printf("\n\t9 - Apaga Lista:");
    printf("\n\t10 - Localiza um Aluno pelo Nome");
    printf("\n\t0 - Para Sair da Funcao Menu:");
    printf("\n\tInforme a opcao:");
    scanf("%d", &op);
    switch (op) {
        case 1:
            printf("\n\tFuncao Insere no Inicio!!");
            insereInicio(lista, informaNovoAluno());
            break;
        case 2:
            printf("\n\tFuncao Insere no Fim!!");
            insereFim(lista, informaNovoAluno());
            break;
        case 3:
            printf("\n\tFuncao Insere na Posicao!!");
            printf("\n\t\tInforme a posicao:");
            scanf("%d", &posicao);
            inserePosicao(lista, informaNovoAluno(), posicao);
            break;
        case 4:
            printf("\n\tFuncao Remove no Inicio:");
            aux = removeInicio(lista);
            if (aux != NULL) {
                mostraAluno(*aux);
                free(aux);
            }
            break;
        case 5:
            printf("\n\tFuncao Remove no Fim:");
            aux = removeFim(lista);
            if (aux != NULL) {
                mostraAluno(*aux);
                free(aux);
            }
            break;
        case 6:
            printf("\n\tFuncao Remove na Posicao!!");
            printf("\n\t\tInforme a posicao:");
            scanf("%d", &posicao);
            aux = removePosicao(lista, posicao);
            if (aux != NULL) {
                mostraAluno(*aux);
                free(aux);
            }
            break;
        case 7:
            printf("\n\nMostra Lista de %s!!!", lista->nome);
            mostraLista(*lista);
            break;
        case 8:
            printf("\n\tFuncao Mostra um Aluno na Posicao!!");
            printf("\n\t\tInforme a posicao:");
            scanf("%d", &posicao);
            aux = localizaPosicao(*lista, posicao);
            if(aux != NULL){
                mostraAluno(*aux);
            }
            break;
        case 9:
            apagaLista(lista);
            printf("\n\tFuncao Apaga toda Lista!");
            break;
        case 10:
            printf("\n\tLocaliza um Aluno pelo Nome");
            printf("\n\t\tInforme o nome:");
            char nome[100];
            scanf("%s", nome);
            aux = localizaAluno(*lista, nome);
            if(aux != NULL){
                mostraAluno(*aux);
            } else {
                printf("\n\t\t  **** Aluno nao encontrado! ****\n");
            }
            break;
        case 0:
            printf("\n\n*** Fim do Programa!!! ***\n");
            break;
        default:
            printf("\n\n*** Opcao Invalida!!! ***\n");
    }
    if (op > 0 && op <= 10) {
        menuTesteLista(lista);
    }
}


 

int main(){

    printf("Exemplo - Lista Simplesmente Encadeada - Lista de Alunos\n");

    LSE matematica;
    inicializaLista(&matematica,"Matematica");

    Aluno *joao = (Aluno*)malloc(sizeof(Aluno));
    cadastraAluno(joao,"Joao Pedro",23,12345);
    insereInicio(&matematica,joao);
    mostraLista(matematica);

  
    Aluno *paulo = (Aluno*)malloc(sizeof(Aluno));
    cadastraAluno(paulo,"Paulo Roberto",34,232323);
    insereInicio(&matematica,paulo);
    mostraLista(matematica);

    Aluno *maria = (Aluno*)malloc(sizeof(Aluno));
    cadastraAluno(maria,"Maria",21,334343);
    insereInicio(&matematica,maria);
    mostraLista(matematica);

    mostraLista2(matematica.primeiro);

    
    menuTesteLista(&matematica);
    
    exit(0);
}
