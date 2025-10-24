#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCACOES 5

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char nome[100];
    char endereco[200];
} Cliente;

typedef struct {
    int codigo, ano;
    char titulo[100];
    int status; // 0 - locado, 1 - disponível
} DVD;

typedef struct {
    int codigo;
    DVD* dvd;
    float valor_diaria;
} Locacao;

typedef struct {
    int codigo;
    Cliente* cliente;
    Locacao* locacoes[MAX_LOCACOES];
    int num_locacoes;
    Data data_locacao;
    Data data_devolucao;
    float valor_total_reserva;
} Reserva;

Cliente *clientes;
DVD *dvds;
Locacao *locacoes;
Reserva **reservas;
int num_clientes = 0;
int num_dvds = 0;
int num_locacoes = 0;
int num_reservas = 0;

Cliente* encontrarCliente(int codigo) {
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i].codigo == codigo) return &clientes[i];
    }
    return NULL;
}

DVD* encontrarDVD(int codigo) {
    for (int i = 0; i < num_dvds; i++) {
        if (dvds[i].codigo == codigo) return &dvds[i];
    }
    return NULL;
}

int CadastraCliente() {
    char nome[100];
    printf("Nome do cliente: ");
    scanf(" %[^\n]", nome);
      for (int i = 0; i < num_clientes; i++) {
        if (strcmp(clientes[i].nome, nome) == 0) {
            printf("Cliente já existe!\n");
            return 0;
        }
    }
    
    clientes[num_clientes].codigo = num_clientes + 1;
    strcpy(clientes[num_clientes].nome, nome);
    printf("Endereço: ");
    scanf(" %[^\n]", clientes[num_clientes].endereco);
    num_clientes++;
    
    printf("Cliente cadastrado! Código: %d\n", num_clientes); 
    return 1;
}

void CadastraDVD() {
    printf("Título do DVD: ");
    scanf(" %[^\n]", dvds[num_dvds].titulo);
    
    printf("Ano de lançamento: ");
    scanf("%d", &dvds[num_dvds].ano);
    
    dvds[num_dvds].codigo = num_dvds + 1;
    dvds[num_dvds].status = 1;
    num_dvds++;
    
    printf("DVD cadastrado! Código: %d\n", num_dvds);
}

void CadastraLocacao() {
    int codigo_dvd;
    printf("Código do DVD: ");
    scanf("%d", &codigo_dvd);
    
    DVD *dvd = encontrarDVD(codigo_dvd);
    if (!dvd) {
        printf("DVD não encontrado!\n");
        return;
    }
    
    locacoes[num_locacoes].codigo = num_locacoes + 1;
    locacoes[num_locacoes].dvd = dvd;
    printf("Valor da diária: R$ ");
    scanf("%f", &locacoes[num_locacoes].valor_diaria);
    num_locacoes++;
    
    printf("Locação cadastrada! Código: %d\n", num_locacoes);
}

void FazReserva() {
    int codigo_cliente;
    printf("Código do cliente: ");
    scanf("%d", &codigo_cliente);
    
    Cliente *cliente = encontrarCliente(codigo_cliente);
    if (!cliente) {
        printf("Cliente não encontrado!\n");
        return;
    }
    
    Reserva *nova_reserva = (Reserva*)malloc(sizeof(Reserva));
    nova_reserva->codigo = num_reservas + 1;
    nova_reserva->cliente = cliente;
    nova_reserva->num_locacoes = 0;
    nova_reserva->valor_total_reserva = 0;
    
    for (int i = 0; i < MAX_LOCACOES; i++) {
        nova_reserva->locacoes[i] = NULL;
    }
    
    printf("Data de locação (dia mes ano): ");
    scanf("%d %d %d", &nova_reserva->data_locacao.dia, 
          &nova_reserva->data_locacao.mes, &nova_reserva->data_locacao.ano);
    
    int continuar = 1;
    while (continuar && nova_reserva->num_locacoes < MAX_LOCACOES) {
        printf("\nDVDs disponíveis:\n");
        for (int i = 0; i < num_dvds; i++) {
            if (dvds[i].status == 1) {
                printf("%d - %s (%d)\n", dvds[i].codigo, dvds[i].titulo, dvds[i].ano);
            }
        }
        
        int codigo_dvd;
        printf("Código do DVD (0 para finalizar): ");
        scanf("%d", &codigo_dvd);
        
        if (codigo_dvd == 0) break;
        
        DVD *dvd = encontrarDVD(codigo_dvd);
        if (!dvd || dvd->status == 0) {
            printf("DVD não disponível!\n");
            continue;
        }

        Locacao *locacao_encontrada = NULL;
        for (int i = 0; i < num_locacoes; i++) {
            if (locacoes[i].dvd->codigo == codigo_dvd) {
                locacao_encontrada = &locacoes[i];
                break;
            }
        }
        
        if (locacao_encontrada) {
            nova_reserva->locacoes[nova_reserva->num_locacoes] = locacao_encontrada;
            nova_reserva->num_locacoes++;
            dvd->status = 0;
            printf("DVD '%s' adicionado à reserva!\n", dvd->titulo);
        } else {
            printf("Locação não encontrada para este DVD!\n");
        }
    }
    
    reservas[num_reservas] = nova_reserva;
    num_reservas++;
    
    printf("Reserva criada! Código: %d\n", nova_reserva->codigo);
}

int DevolveReserva() {
    int codigo;
    printf("Código da reserva: ");
    scanf("%d", &codigo);
    
    Reserva *reserva = NULL;
    for (int i = 0; i < num_reservas; i++) {
        if (reservas[i]->codigo == codigo) {
            reserva = reservas[i];
            break;
        }
    }
    
    if (!reserva) {
        printf("Reserva não encontrada!\n");
        return 0;
    }
    
    printf("Data devolução (dia mes ano): ");
    scanf("%d %d %d", &reserva->data_devolucao.dia, 
          &reserva->data_devolucao.mes, &reserva->data_devolucao.ano);
    
    int dias;
    printf("Número de dias: ");
    scanf("%d", &dias);
    
    reserva->valor_total_reserva = 0;
    for (int i = 0; i < reserva->num_locacoes; i++) {
        if (reserva->locacoes[i] != NULL) {
            reserva->valor_total_reserva += reserva->locacoes[i]->valor_diaria * dias;
            reserva->locacoes[i]->dvd->status = 1;
        }
    }
    
    printf("Devolução OK! Total: R$ %.2f\n", reserva->valor_total_reserva);
    return 1;
}

void MostraCliente(Cliente cliente) {
    printf("\n--- CLIENTE ---\n");
    printf("Código: %d\n", cliente.codigo);
    printf("Nome: %s\n", cliente.nome);
    printf("Endereço: %s\n", cliente.endereco);
}

void MostraDVD(DVD dvd) {
    printf("\n--- DVD ---\n");
    printf("Código: %d\n", dvd.codigo);
    printf("Título: %s\n", dvd.titulo);
    printf("Ano de Lançamento: %d\n", dvd.ano);
    printf("Status: %s\n", dvd.status ? "Disponível" : "Locado");
}

void MostraLocacao(Locacao locacao) {
    printf("\n--- LOCAÇÃO ---\n");
    printf("Código: %d\n", locacao.codigo);
    printf("DVD: %s\n", locacao.dvd->titulo);
    printf("Valor: R$ %.2f\n", locacao.valor_diaria);
}

void MostraReserva(Reserva reserva) {
    printf("\n--- RESERVA ---\n");
    printf("Código: %d\n", reserva.codigo);
    printf("Cliente: %s\n", reserva.cliente->nome);
    printf("Data Locação: %02d/%02d/%d\n", 
           reserva.data_locacao.dia, reserva.data_locacao.mes, reserva.data_locacao.ano);
    printf("Data Devolução: %02d/%02d/%d\n", 
           reserva.data_devolucao.dia, reserva.data_devolucao.mes, reserva.data_devolucao.ano);
    printf("DVDs locados:\n");
    for (int i = 0; i < reserva.num_locacoes; i++) {
        if (reserva.locacoes[i] != NULL) {
            printf("  - %s (R$ %.2f/dia)\n", 
                   reserva.locacoes[i]->dvd->titulo, reserva.locacoes[i]->valor_diaria);
        }
    }
    printf("Valor Total: R$ %.2f\n", reserva.valor_total_reserva);
}

void listarClientes() {
    printf("\n=== LISTA DE CLIENTES ===\n");
    for (int i = 0; i < num_clientes; i++) {
        MostraCliente(clientes[i]);
        printf("\n");
    }
}

void listarDVDs() {
    printf("\n=== LISTA DE DVDs ===\n");
    for (int i = 0; i < num_dvds; i++) {
        MostraDVD(dvds[i]);
        printf("\n");
    }
}

void listarReservasPorCliente() {
    int codigo_cliente;
    printf("Código do cliente: ");
    scanf("%d", &codigo_cliente);
    
    Cliente *cliente = encontrarCliente(codigo_cliente);
    if (!cliente) {
        printf("Cliente não encontrado!\n");
        return;
    }
    
    printf("\n=== RESERVAS DO CLIENTE: %s ===\n", cliente->nome);
    int encontrou = 0;
    
    for (int i = 0; i < num_reservas; i++) {
        if (reservas[i]->cliente->codigo == codigo_cliente) {
            MostraReserva(*reservas[i]);
            encontrou = 1;
            printf("\n");
        }
    }
    
    if (!encontrou) {
        printf("Nenhuma reserva encontrada para este cliente!\n");
    }
}

void preCadastros() {
    clientes = (Cliente*)malloc(100 * sizeof(Cliente));
    dvds = (DVD*)malloc(100 * sizeof(DVD));
    locacoes = (Locacao*)malloc(100 * sizeof(Locacao));
    reservas = (Reserva**)malloc(100 * sizeof(Reserva*));
    
    // Clientes
    clientes[0] = (Cliente){1, "Julie", "Rua gaspar de lemos, 123"};
    clientes[1] = (Cliente){2, "Maria", "Honorio aikim, 456"};
    clientes[2] = (Cliente){3, "Pedro", "Rua 6, 789"};
    clientes[3] = (Cliente){4, "Ana", "Rua dr nascimento, 101"};
    num_clientes = 4;
    
    // DVDs
    strcpy(dvds[0].titulo, "Twilight"); dvds[0].codigo = 1; dvds[0].ano = 2019; dvds[0].status = 1;
    strcpy(dvds[1].titulo, "Samurai x"); dvds[1].codigo = 2; dvds[1].ano = 2020; dvds[1].status = 1;
    strcpy(dvds[2].titulo, "Panico"); dvds[2].codigo = 3; dvds[2].ano = 2021; dvds[2].status = 1;
    strcpy(dvds[3].titulo, "Jogos mortais"); dvds[3].codigo = 4; dvds[3].ano = 2018; dvds[3].status = 1;
    num_dvds = 4;
    
    // Locações
    locacoes[0] = (Locacao){1, &dvds[0], 5.00};
    locacoes[1] = (Locacao){2, &dvds[1], 4.50};
    locacoes[2] = (Locacao){3, &dvds[2], 6.00};
    locacoes[3] = (Locacao){4, &dvds[3], 5.50};
    locacoes[4] = (Locacao){5, &dvds[0], 5.00};
    locacoes[5] = (Locacao){6, &dvds[1], 4.50};
    num_locacoes = 6;
    
    printf("Pré-cadastros OK! 4 clientes, 4 DVDs, 6 locações.\n");
}

int main() {
    preCadastros();
    
    int opcao;
    do {
        printf("\n=== LOCADORA ===\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Cadastrar DVD\n");
        printf("3. Cadastrar Locação\n");
        printf("4. Cadastrar Reserva\n");
        printf("5. Devolver Reserva\n");
        printf("6. Listar Clientes\n");
        printf("7. Listar DVDs\n");
        printf("8. Listar Locações de um Cliente - Reserva\n");
        printf("9. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: 
                if (!CadastraCliente()) printf("Cadastro não realizado!\n");
                break;
            case 2: CadastraDVD(); break;
            case 3: CadastraLocacao(); break;
            case 4: FazReserva(); break;
            case 5: 
                if (!DevolveReserva()) printf("Devolução não realizada!\n");
                break;
            case 6: listarClientes(); break;
            case 7: listarDVDs(); break;
            case 8: listarReservasPorCliente(); break;
            case 9: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 9);
    
    for (int i = 0; i < num_reservas; i++) {
        free(reservas[i]);
    }
    free(clientes);
    free(dvds);
    free(locacoes);
    free(reservas);
    
    return 0;
}