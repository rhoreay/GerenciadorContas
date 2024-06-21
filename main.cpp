#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_TAM_NOME 30
#define MAX_CONTAS_PENDENTES 50
#define MAX_LINHA 120

typedef struct{
    int codigoConta;
    char nome[MAX_TAM_NOME];
    float valorTotal;
    int totalParcelas;
    int parcelasRestantes;
    float valorParcela;
    int quitada;
} TConta;

void cabec(){
    printf("\n\n****CADASTRO DE CONTAS PARA PAGAR****");
    printf("\n[1] Registrar nova conta");
    printf("\n[2] Ler todas informacoes uma conta");
    printf("\n[3] Ver todas contas pendentes");
    printf("\n[4] Pagar uma conta");
    printf("\n[5] Mostrar todas contas registradas");
    printf("\n>");
}



//cria nova conta
void criaNovaConta(char nome[MAX_TAM_NOME], float valorTotal, int totalParcelas){
    TConta novaConta;

    //abrindo arquivos 
    FILE * ptrArquivoTodasContas;
    ptrArquivoTodasContas = fopen("./dados/todasContas.txt", "a+");
    FILE * ptrArquivoContasPendente;
    ptrArquivoContasPendente = fopen("./dados/contasPendente.txt", "a+");
    FILE * ptrArquivoStatusContas;
    ptrArquivoStatusContas = fopen("./dados/statusContas.txt", "a+");

    //identifica codigo da conta anterior caso exista
    int codigoContaAnterior = 0, codigoNovaConta;
    char linha[MAX_LINHA];
    while(fgets(linha, MAX_LINHA, ptrArquivoTodasContas) != NULL){//le ate a ultima linha para saber qual vai ser o codigo da proxima conta
        sscanf(linha, "{%d}", &codigoContaAnterior);}
    codigoNovaConta = codigoContaAnterior + 1;

    //atribuindo valores ao objeto conta
    novaConta.codigoConta = codigoNovaConta;
    strcpy(novaConta.nome, nome);
    novaConta.valorTotal = valorTotal;
    novaConta.totalParcelas = totalParcelas;
    novaConta.parcelasRestantes = totalParcelas;
    novaConta.valorParcela = novaConta.valorTotal / novaConta.totalParcelas;
    novaConta.quitada = false;

    //adicionando ao todasContas.txt
    fprintf(ptrArquivoTodasContas, "{%d} {%s} {%.2f} {%d}\n", novaConta.codigoConta, novaConta.nome, novaConta.valorTotal, novaConta.totalParcelas, novaConta.parcelasRestantes);
    //adicionando ao statusConta.txt
    fprintf(ptrArquivoStatusContas, "{%d} {%d}\n", novaConta.codigoConta, novaConta.quitada);//registra 0 se a conta nao estiver quitada e 1 se ela estiver
    //adicionando a tabela contasPendente.txt
    fprintf(ptrArquivoContasPendente, "{%d} {%.2f} {%d}\n", novaConta.codigoConta, novaConta.valorParcela, novaConta.parcelasRestantes);

    //fechando tabelas
    fclose(ptrArquivoContasPendente);
    fclose(ptrArquivoStatusContas);
    fclose(ptrArquivoTodasContas);
}


//funcao de buscar informacoes da conta
TConta buscaContaInfo(int codigoConta){
    //abrindo arquivos 
    FILE * ptrArquivoTodasContas;
    ptrArquivoTodasContas = fopen("./dados/todasContas.txt", "a+");
    FILE * ptrArquivoContasPendente;
    ptrArquivoContasPendente = fopen("./dados/contasPendente.txt", "a+");
    FILE * ptrArquivoStatusContas;
    ptrArquivoStatusContas = fopen("./dados/statusContas.txt", "a+");

    TConta conta;
    
    char linha[MAX_LINHA];
    int codigoEncontrado;

    //loops para pegar informações das contas
    while(fgets(linha, MAX_LINHA, ptrArquivoTodasContas) != NULL){//loop para ler arquivo todasContas.txt
        sscanf(linha, "{%d}", &codigoEncontrado);
        if (codigoEncontrado == codigoConta){
            sscanf(linha, "{%d} {%[^}]} {%f} {%d}", &conta.codigoConta, conta.nome, &conta.valorTotal, &conta.totalParcelas);
            break;
        }
    }
    while(fgets(linha, MAX_LINHA, ptrArquivoStatusContas) != NULL){//loop para ler arquivo statusConta.txt
        sscanf(linha, "{%d}", &codigoEncontrado);
        if (codigoEncontrado == codigoConta){
            sscanf(linha, "{%*d} {%d}", &conta.quitada);
            break;
        }
    }
    while(fgets(linha, MAX_LINHA, ptrArquivoContasPendente) != NULL){//loop para ler arquivo contasPendentes.txt
        sscanf(linha, "{%d}", &codigoEncontrado);
        if (codigoEncontrado == codigoConta){
            sscanf(linha, "{%*d} {%*f} {%d}", &conta.parcelasRestantes);
            break;
        }
        else{
            conta.parcelasRestantes = 0;
        }
    }   
    conta.valorParcela = conta.valorTotal / conta.totalParcelas;
    
    //fechando tabelas
    fclose(ptrArquivoContasPendente);
    fclose(ptrArquivoStatusContas);
    fclose(ptrArquivoTodasContas);
    
    
    return conta;
    
}


//funcao busca e imprime todas as contas que ainda precisam ser pagas
void buscaContasPendentes(){
    FILE *ptrArquivoContasPendentes;
    ptrArquivoContasPendentes = fopen("./dados/contasPendente.txt", "r");

    char linha[MAX_LINHA];
    int indice = 0;
    TConta conta;

    while(fgets(linha, MAX_LINHA, ptrArquivoContasPendentes) != NULL){//le todas as linhas do arquivo contasPendentes.txt, transforma em objeto e imprime
        int codigoConta;
        sscanf(linha, "{%d}", &codigoConta);
        conta = buscaContaInfo(codigoConta);
        printf("\n[conta codigo %d]", conta.codigoConta);
        printf("\nnome: %s", conta.nome);
        printf("\nvalor da parcela: %.2f parcelas restantes: %d\n", conta.valorParcela, conta.parcelasRestantes);
        indice++;
    }

    fclose(ptrArquivoContasPendentes);

}

void pagarConta(int codigoBuscar){//funcao de pagar conta precisa editar o arquivo contasPendentes e o arquivo statusConta se necessario

    
    TConta conta = buscaContaInfo(codigoBuscar);

    if (conta.parcelasRestantes - 1 == 0){//verifica se é a ultima parcela, se for, também altera o arquivo statusConta.txt
        char linha[MAX_LINHA];
        int codigoConta;
        FILE * ptrArquivoContasPendentes = fopen("./dados/contasPendente.txt", "a+");
        FILE * ptrArquivoStatusContas = fopen("./dados/statusContas.txt", "a+");
        FILE * ptrTempContasPendentes = fopen("./dados/tempContasPendente.txt", "a+");
        FILE * ptrTempStatusContas = fopen("./dados/tempStatusContas.txt", "a+");

        while(fgets(linha, MAX_LINHA, ptrArquivoContasPendentes) != NULL){
            sscanf(linha, "{%d}", &codigoConta);
            if (codigoConta == codigoBuscar){
                strcpy(linha, "");
            }
        fprintf(ptrTempContasPendentes, linha);
        }
        while(fgets(linha, MAX_LINHA, ptrArquivoStatusContas) != NULL){
            sscanf(linha, "{%d}", &codigoConta);
            if (codigoConta == codigoBuscar){
                sprintf(linha, "{%d} {1}\n", codigoConta);
            }
            fprintf(ptrTempStatusContas, linha);
        }

        fclose(ptrArquivoContasPendentes);
        fclose(ptrTempContasPendentes);
        remove("./dados/contasPendente.txt");
        rename("./dados/tempContasPendente.txt", "./dados/contasPendente.txt");

        fclose(ptrArquivoStatusContas);
        fclose(ptrTempStatusContas);
        remove("./dados/statusContas.txt");
        rename("./dados/tempStatusContas.txt", "./dados/statusContas.txt");
    }
    
    else{
        char linha[MAX_LINHA];
        int codigoConta;
        FILE * ptrArquivoContasPendentes = fopen("./dados/contasPendente.txt", "a+");
        FILE * ptrTempContasPendentes = fopen("./dados/tempContasPendente.txt", "a+");

        while(fgets(linha, MAX_LINHA, ptrArquivoContasPendentes) != NULL){
            sscanf(linha, "{%d}", &codigoConta);
            if (codigoConta == codigoBuscar){
                sprintf(linha, "{%d} {%.2f} {%d}\n", codigoConta, conta.valorParcela, conta.parcelasRestantes - 1);
            }
            fprintf(ptrTempContasPendentes, linha);
        }

        fclose(ptrArquivoContasPendentes);
        fclose(ptrTempContasPendentes);
        remove("./dados/contasPendente.txt");
        rename("./dados/tempContasPendente.txt", "./dados/contasPendente.txt");

    }

    printf("conta '%s' paga com sucesso!", conta.nome);
        
}

void mostraTodasContas(){
    FILE * ptrArquivoTodasContas = fopen("./dados/todasContas.txt", "r");
    char linha[MAX_LINHA];
    while(fgets(linha, MAX_LINHA, ptrArquivoTodasContas) != NULL){
        TConta conta;
        sscanf(linha, "{%d} {%[^}]} {%f} {%d}", &conta.codigoConta, conta.nome, &conta.valorTotal, &conta.totalParcelas);
        printf("\nCodigo: %d, nome: %s, valor: %.2f", conta.codigoConta, conta.nome, conta.valorTotal);

    }
}

int main(){
    system("clear");
    while(1){
        int comando;
        cabec();
        scanf("%d", &comando);
        switch(comando){
            case 1:
                getchar();
                char nomeConta[MAX_TAM_NOME];
                int parcelasConta;
                float valorConta;
                system("clear");
                printf("\n{REGISTRAR CONTA}\n");
                printf("digite o nome da conta: ");
                scanf("%49[^\n]", nomeConta);
                printf("valor total da conta: ");
                scanf("%f", &valorConta);
                printf("total de parcelas da conta: ");
                scanf("%d", &parcelasConta);
                criaNovaConta(nomeConta, valorConta, parcelasConta);
                system("clear");
                printf("Conta '%s' registrada com sucesso", nomeConta);
                break;
            case 2:
                getchar();
                int codigoBuscado;
                TConta contaEncontrada;
                printf("\n{BUSCAR UMA CONTA}\n");
                printf("digite o codigo da conta que deseja acessar: ");
                scanf("%d", &codigoBuscado);
                contaEncontrada = buscaContaInfo(codigoBuscado);
                system("clear");
                if(contaEncontrada.codigoConta == 0){
                    printf("NENHUMA CONTA FOI ENCONTRADA");
                }else{
                    printf("CONTA ENCONTRADA CODIGO: %d\n", contaEncontrada.codigoConta);
                    printf("nome: %s", contaEncontrada.nome);
                    printf("\nvalor da conta: %.2f", contaEncontrada.valorTotal);
                    printf("\ntotal de parcelas: %d", contaEncontrada.totalParcelas);
                    printf("\nvalor da parcela: %.2f", contaEncontrada.valorParcela);
                    printf("\nquitada: %d", contaEncontrada.quitada);
                    printf("\nparcelas restantes: %d", contaEncontrada.parcelasRestantes);
                }
                break;
            case 3:
                system("clear");
                printf("{VER CONTAS PENDENTES}\n");
                buscaContasPendentes();
                break;
            case 4:
                printf("\n{PAGAR UMA CONTA}");
                int codigoConta;
                TConta conta;
                printf("\nqual o codigo da conta que voce deseja pagar? ");
                scanf("%d", &codigoConta);
                system("clear");
                pagarConta(codigoConta);
                
                break;
            case 5:
                system("clear");
                printf("\n{MOSTRAR TODAS CONTAS}");
                mostraTodasContas();
                break;
            default:
                printf("\n{DIGITE UM VALOR VALIDO}");
                break;
        }
    }

}