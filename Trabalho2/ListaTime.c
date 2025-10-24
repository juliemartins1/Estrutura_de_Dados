#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aluno {
    int matricula;
    char nome[50];
    struct Aluno *proximo;
} Aluno;

typedef struct LSE {
    char nomeTime[20];
    Aluno *inicio;
} Lista;

Lista* novaLista(char *nomeTime) {
    Lista *novaLista = (Lista*) malloc(sizeof(Lista));
    strcpy(novaLista->nomeTime, nomeTime);
    novaLista->inicio = NULL;
    return novaLista;
}


int AlunojaExiste(Lista *times[], int matricula) {
    for (int i = 0; i < 3; i++) {
        Aluno *aux = times[i]->inicio;
        while (aux != NULL) {
            if (aux->matricula == matricula) {
                return 1; 
            }
            aux = aux->proximo;
        }
    }
    return 0; 
}

void inserirAlunoNoTime(Lista *time, int matricula, char *nome, Lista *times[]) {
    if (AlunojaExiste(times, matricula)) {
        printf("Erro: matricula %d ja esta cadastrada em algum time!\n", matricula);
        return;
    }

    Aluno *novo = (Aluno*) malloc(sizeof(Aluno));
    novo->matricula = matricula;
    strcpy(novo->nome, nome);
    novo->proximo = time->inicio;
    time->inicio = novo;

    printf("Aluno %s (matricula %d) adicionado ao time %s\n",nome, matricula, time->nomeTime);
}

void exibirJogadoresTitulares(Lista *time) {
    printf("\nTitulares do %s:\n", time->nomeTime);
    Aluno *atual = time->inicio;
    int i = 0;
    while(atual != NULL && i < 5) {
        printf("%d - %s\n", atual->matricula, atual->nome);
        atual = atual->proximo;
        i++;
    }
    if(i == 0) {
        printf("Sem jogadores inscritos.\n");
    }
}

void exibirTimeCompleto(Lista *time) {
    printf("\nTime completo %s:\n", time->nomeTime);
    Aluno *atual = time->inicio;
    while(atual != NULL) {
        printf("%d - %s\n", atual->matricula, atual->nome);
        atual = atual->proximo;
    }
}

void trocaTitularPeloReserva(Lista *time, int matriculaTitular, int matriculaReserva) {
    if(time->inicio == NULL) return;

    Aluno *antTitular = NULL, *titular = time->inicio;
    Aluno *antReserva = NULL, *reserva = time->inicio;

    while(titular != NULL && titular->matricula != matriculaTitular) {
        antTitular = titular;
        titular = titular->proximo;
    }

    while(reserva != NULL && reserva->matricula != matriculaReserva) {
        antReserva = reserva;
        reserva = reserva->proximo;
    }

    if(titular == NULL || reserva == NULL) {
        printf("O Jogador não foi encontrado.\n");
        return;
    }

    if(antTitular != NULL) antTitular->proximo = reserva;
    else time->inicio = reserva;

    if(antReserva != NULL) antReserva->proximo = titular;
    else time->inicio = titular;

    Aluno *temp = titular->proximo;
    titular->proximo = reserva->proximo;
    reserva->proximo = temp;

    printf("Os Jogadores %d e %d foram trocados no %s.\n", 
           matriculaTitular, matriculaReserva, time->nomeTime);
}

void titularParaOFinal(Lista *time, int matriculaTitular) {
    if(time->inicio == NULL) return;

    Aluno *ant = NULL, *atual = time->inicio;

    while(atual != NULL && atual->matricula != matriculaTitular) {
        ant = atual;
        atual = atual->proximo;
    }

    if(atual == NULL) {
        printf("O Jogador não foi encontrado.\n");
        return;
    }

    if(atual->proximo == NULL) {
        printf("Jogador %d já está no final da lista do %s!\n",matriculaTitular, time->nomeTime);
        return;
    }

    if(ant == NULL) {
        time->inicio = atual->proximo;
    }
    else {
        ant->proximo = atual->proximo;
    }

    Aluno *temp = time->inicio;
    while(temp->proximo != NULL) {
        temp = temp->proximo;
    }

    temp->proximo = atual;
    atual->proximo = NULL;

    printf("Jogador %d foi jogado pro fim da lista do %s!\n", matriculaTitular, time->nomeTime);
}

void transferirJogadorDeTime(Lista *origem, Lista *destino, int matricula) {
    if(origem->inicio == NULL) {
        printf("O Time %s não possui jogadores.\n", origem->nomeTime);
        return;
    }

    Aluno *ant = NULL, *atual = origem->inicio;
    while(atual != NULL && atual->matricula != matricula) {
        ant = atual;
        atual = atual->proximo;
    }

    if(atual == NULL) {
        printf("O Jogador %d não foi encontrado no %s.\n", matricula, origem->nomeTime);
        return;
    }

    if(ant == NULL) origem->inicio = atual->proximo;
    else ant->proximo = atual->proximo;

    atual->proximo = NULL;
    if(destino->inicio == NULL) destino->inicio = atual;
    else {
        Aluno *temp = destino->inicio;
        while(temp->proximo != NULL) temp = temp->proximo;
        temp->proximo = atual;
    }

    printf("O Jogador %d foi transferido do %s para o %s.\n",matricula, origem->nomeTime, destino->nomeTime);
}

void liberarLista(Lista *time) {
    Aluno *atual = time->inicio;
    while(atual != NULL) {
        Aluno *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    free(time);
}

int main() {
    Lista *inter = novaLista("Inter");
    Lista *gremio = novaLista("Grêmio");
    Lista *juventude = novaLista("Juventude");
    Lista *times[] = {inter, gremio, juventude};

      inserirAlunoNoTime(inter, 12345, "Roberto", times);
      inserirAlunoNoTime(inter, 67894, "Augusto", times);
      inserirAlunoNoTime(inter, 98765, "Andre", times); 
      inserirAlunoNoTime(inter, 43210, "Yuri", times);
      inserirAlunoNoTime(inter, 10547, "Jorge", times); 
      inserirAlunoNoTime(inter, 16452, "Luiz", times); 
      inserirAlunoNoTime(gremio, 22575, "Davi", times); 
      inserirAlunoNoTime(gremio, 23547, "Leonardo", times); 
      inserirAlunoNoTime(gremio, 20345, "Bruno", times); 
      inserirAlunoNoTime(gremio, 20454, "Felipe", times); 
      inserirAlunoNoTime(juventude, 31235, "Ronaldo", times); 
      inserirAlunoNoTime(juventude, 36879, "Jose", times);
      inserirAlunoNoTime(juventude, 31454 ,"Jackson", times);

    int opcao, matricula, matriculaTitular, matriculaReserva;
    char nome[50];
    int escolhaTime, origem, destino;

    do {
        printf("\n======= MENU =======\n");
        printf("1 - Cadastrar jogador em um time\n");
        printf("2 - Mostrar titulares \n");
        printf("3 - Mostrar time completo\n");
        printf("4 - Trocar titular por reserva\n");
        printf("5 - Mandar titular para o final da lista\n");
        printf("6 - Transferir jogador entre times\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite matricula: ");
                scanf("%d", &matricula);
                printf("Digite nome: ");
                scanf(" %[^\n]", nome);
                printf("Escolha o time (1-Inter, 2-Grêmio, 3-Juventude): ");
                scanf("%d", &escolhaTime);

                if(escolhaTime == 1) inserirAlunoNoTime(inter, matricula, nome, times);
                else if(escolhaTime == 2) inserirAlunoNoTime(gremio, matricula, nome, times);
                else if(escolhaTime == 3) inserirAlunoNoTime(juventude, matricula, nome, times);
                else printf("Time inválido!\n");
                break;

            case 2:
                exibirJogadoresTitulares(inter);
                exibirJogadoresTitulares(gremio);
                exibirJogadoresTitulares(juventude);
                break;

            case 3:
                exibirTimeCompleto(inter);
                exibirTimeCompleto(gremio);
                exibirTimeCompleto(juventude);
                break;

            case 4:
                printf("Digite o time (1-Inter, 2-Grêmio, 3-Juventude): ");
                scanf("%d", &escolhaTime);
                printf("Matrícula do titular: ");
                scanf("%d", &matriculaTitular);
                printf("Matrícula do reserva: ");
                scanf("%d", &matriculaReserva);

                if(escolhaTime == 1) trocaTitularPeloReserva(inter, matriculaTitular, matriculaReserva);
                else if(escolhaTime == 2) trocaTitularPeloReserva(gremio, matriculaTitular, matriculaReserva);
                else if(escolhaTime == 3) trocaTitularPeloReserva(juventude, matriculaTitular, matriculaReserva);
                else printf("Time inválido!\n");
                break;

            case 5:
                printf("Digite o time (1-Inter, 2-Grêmio, 3-Juventude): ");
                scanf("%d", &escolhaTime);
                printf("Matrícula do titular: ");
                scanf("%d", &matriculaTitular);

                if(escolhaTime == 1) titularParaOFinal(inter, matriculaTitular);
                else if(escolhaTime == 2) titularParaOFinal(gremio, matriculaTitular);
                else if(escolhaTime == 3) titularParaOFinal(juventude, matriculaTitular);
                else printf("Time inválido!\n");
                break;

            case 6:
                printf("Time origem (1-Inter, 2-Grêmio, 3-Juventude): ");
                scanf("%d", &origem);
                printf("Time destino (1-Inter, 2-Grêmio, 3-Juventude): ");
                scanf("%d", &destino);
                printf("Matrícula do jogador: ");
                scanf("%d", &matricula);

                Lista *timeOrigem = NULL, *timeDestino = NULL;
                if(origem == 1) timeOrigem = inter;
                else if(origem == 2) timeOrigem = gremio;
                else if(origem == 3) timeOrigem = juventude;

                if(destino == 1) timeDestino = inter;
                else if(destino == 2) timeDestino = gremio;
                else if(destino == 3) timeDestino = juventude;

                if(timeOrigem && timeDestino && origem != destino) {
                    transferirJogadorDeTime(timeOrigem, timeDestino, matricula);
                } else {
                    printf("Times inválidos ou iguais!\n");
                }
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    liberarLista(inter);
    liberarLista(gremio);
    liberarLista(juventude);

    return 0;
}