/* ========== [INCLUSÃO DE BIBLIOTECAS] ========== */
#include <stdio.h>      // Biblioteca padrão de entrada/saída (printf, scanf, fgets, etc.)
#include <string.h>     // Funções para manipulação de strings (strcpy, strcmp, strlen)
#include <ctype.h>      // Funções para manipulação de caracteres (toupper, isdigit)
#include <stdlib.h>     // Funções gerais (system, exit, malloc)
#include <locale.h>     // Configuração de localidade (setlocale para caracteres especiais)
#include <stdarg.h>     // Manipulação de argumentos variáveis (para funções como printf)
#include <windows.h>    // API do Windows (para configurações do console)
#include <math.h>

/* ========== [DEFINIÇÕES DE CONSTANTES] ========== */
#define MAX_CLIENTES 100               // Número máximo de clientes que o sistema suporta
#define ARQUIVO_DADOS "clientes_imc.dat" // Nome do arquivo binário para salvar os dados
#define MASCULINO 'M'                  // Constante para sexo masculino
#define FEMININO 'F'                   // Constante para sexo feminino

/* ========== [ESTRUTURAS DE DADOS] ========== */
// Estrutura para armazenar códigos de cores ANSI
typedef struct {
    const char* reset;       	// Código para resetar formatação (\033[0m)
    const char* preto;       	// Código para texto preto (\033[30m)
    const char* branco;      	// Código para texto branco (\033[97m)
    const char* vermelho;    	// Código para texto vermelho (\033[31m)
    const char* verde;       	// Código para texto verde (\033[32m)
    const char* amarelo;     	// Código para texto amarelo (\033[33m)
    const char* azul;        	// Código para texto azul (\033[94m)
    const char* laranja;     	// Código para texto laranja (\033[38;2;255;165;0m)
    const char* fundo_preto;    // Código para fundo preto (\033[40m)
    const char* fundo_branco;   // Código para fundo branco (\033[47m)
    const char* fundo_vermelho; // Código para fundo vermelho (\033[41m)
    const char* fundo_verde;    // Código para fundo verde (\033[42m)
    const char* fundo_amarelo; 	// Código para fundo amarelo (\033[43m)
} ConsoleCores;

// Estrutura para armazenar dados de um cliente
typedef struct {
    char nome[50];          // String para armazenar o nome do cliente (máx 50 caracteres)
    char sexo;              // Char para armazenar o sexo ('M' ou 'F')
    float peso;             // Float para armazenar o peso em kg
    float altura;           // Float para armazenar a altura em metros
    float imc;              // Float para armazenar o valor do IMC calculado
    char classificacao[30]; // String para armazenar a classificação do IMC
} Cliente;

/* ========== [VARIÁVEIS GLOBAIS] ========== */
Cliente clientes[MAX_CLIENTES];  // Array de estruturas Cliente para armazenar até 100 clientes
int totalClientes = 0;           // Inteiro para contar quantos clientes estão cadastrados
ConsoleCores cor;                // Variável do tipo ConsoleCores para gerenciar cores

/* ========== [PROTÓTIPOS DE FUNÇÕES] ========== */
void inicializarCores();         // Função para inicializar os códigos de cores
void liberarCores();             // Função para "liberar" recursos (não faz nada atualmente)
void imprimirColorido(const char* cor, const char* formato, ...); // Função para imprimir texto colorido
void limparBuffer();             // Função para limpar o buffer de entrada
float lerFloatPositivo(const char* mensagem); // Função para ler um float positivo
void calcularIMC(Cliente *cliente); // Função para calcular IMC e classificação
void cadastrarCliente();         // Função para cadastrar um novo cliente
void listarClientes();           // Função para listar todos os clientes
void pesquisarCliente();         // Função para pesquisar clientes por nome
void salvarDados();              // Função para salvar dados no arquivo binário
void carregarDados();            // Função para carregar dados do arquivo binário
void mostrarMenu();              // Função para exibir o menu principal
void imprimirTabelaIMC();        // Função para exibir a tabela de classificação do IMC
void configuracoes();            // Função para configurar o console
void imprimirTabelaIMC_Homem();  // Função para exibir tabela de IMC para homens
void imprimirTabelaIMC_Mulher(); // Função para exibir tabela de IMC para mulheres
float calcularPesoIdeal(float altura, char sexo); // Função para calcular peso ideal
void imprimir_ascii_art();       // Função para exibir arte ASCII do título

/* ========== [FUNÇÃO PRINCIPAL] ========== */
int main() {
    configuracoes();       // Configura o console (cores, locale, etc.)
    inicializarCores();    // Inicializa as cores disponíveis
    carregarDados();       // Carrega clientes do arquivo, se existir

    int opcao;             // Variável para armazenar a opção do menu

    imprimir_ascii_art();  // Exibe a arte ASCII do título
    printf("\n\n\n");      // Espaçamento
    system("pause");       // Pausa para o usuário ver a arte
    system("cls");         // Limpa a tela

    do {
        mostrarMenu();     // Exibe o menu de opções
        scanf("%d", &opcao); // Lê a opção do usuário
        limparBuffer();    // Limpa o buffer do teclado
        
        system("cls");     // Limpa a tela antes de mostrar a próxima tela
        
        switch(opcao) {    // Executa a função correspondente à opção
            case 1:
                imprimirTabelaIMC();  // Mostra a tabela de IMC
                cadastrarCliente();    // Chama função de cadastro
                break;
            case 2:
                listarClientes();     // Lista todos os clientes
                break;
            case 3:
                pesquisarCliente();    // Pesquisa cliente por nome
                break;
            case 5:
                imprimirColorido(cor.verde, "Saindo...\n"); // Mensagem de saída
                break;
            default:
                imprimirColorido(cor.vermelho, "Opção inválida!\n"); // Mensagem de erro
        }
    } while (opcao != 5);  // Repete até o usuário escolher sair (opção 5)

    liberarCores();        // "Libera" recursos (não faz nada atualmente)
    return 0;              // Retorna 0 indicando sucesso
}

/* ========== [IMPLEMENTAÇÃO DAS FUNÇÕES] ========== */

// Configurações iniciais do console
void configuracoes() {
    #ifdef _WIN32
    // Obtém o handle para o console de saída padrão
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Verifica se o handle é válido
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }
    
    // Obtém o modo atual do console
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }
    
    // Ativa o modo de terminal virtual (suporte a códigos ANSI)
    dwMode |= 0x0004;
    if (!SetConsoleMode(hOut, dwMode)) {
        return;
    }
    #endif	
	
    setlocale(LC_ALL, "portuguese"); // Configura locale para português	
}

// Inicializa os códigos de cores ANSI na estrutura 'cor'
void inicializarCores() {
    cor.reset = "\033[0m";          // Resetar formatação
    cor.preto = "\033[30m";         // Texto preto
    cor.branco = "\033[97m";        // Texto branco
    cor.vermelho = "\033[31m";      // Texto vermelho
    cor.verde = "\033[32m";         // Texto verde
    cor.amarelo = "\033[38;2;255;255;0m"; // Texto amarelo (RGB)
    cor.azul = "\033[94m";          // Texto azul
    cor.laranja = "\033[38;2;255;165;0m"; // Texto laranja (RGB)
    cor.fundo_preto = "\033[40m";   // Fundo preto
    cor.fundo_branco = "\033[47m";  // Fundo branco
    cor.fundo_vermelho = "\033[41m"; // Fundo vermelho
    cor.fundo_verde = "\033[42m";   // Fundo verde
    cor.fundo_amarelo = "\033[43m"; // Fundo amarelo
}

// Função vazia (placeholder para futura liberação de recursos)
void liberarCores() {
    // Não é necessário liberar memória pois usamos strings literais
}

// Função para imprimir texto colorido no console
void imprimirColorido(const char* cor_, const char* formato, ...) {
    va_list args;                  // Lista de argumentos variáveis
    va_start(args, formato);       // Inicializa a lista de argumentos
    
    printf("%s", cor_);            // Aplica a cor especificada
    vprintf(formato, args);        // Imprime o texto formatado
    printf("%s", cor.reset);       // Reseta a cor
    
    va_end(args);                  // Finaliza a lista de argumentos
}

// Limpa o buffer de entrada para evitar problemas com scanf
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Lê todos os caracteres até encontrar \n ou EOF
}

// Lê um número float positivo do usuário com validação
float lerFloatPositivo(const char* mensagem) {
    float valor;
    do {
        printf("%s", mensagem);    // Exibe a mensagem passada como parâmetro
        scanf("%f", &valor);       // Lê o valor
        limparBuffer();            // Limpa o buffer
        if (valor <= 0) {          // Valida se é positivo
            imprimirColorido(cor.vermelho, "Valor inválido! Digite um número positivo.\n");
        }
    } while (valor <= 0);          // Repete até que um valor positivo seja digitado
    return valor;                  // Retorna o valor válido
}

// Calcula o IMC e classifica com base no sexo
void calcularIMC(Cliente *cliente) {
    // Fórmula do IMC: peso / (altura^2)
    cliente->imc = cliente->peso / (cliente->altura * cliente->altura);
    
    // Classificação para homens
    if (cliente->sexo == 'M' || cliente->sexo == 'm') {
        if (cliente->imc < 20.7) strcpy(cliente->classificacao, "Abaixo do peso");
        else if (cliente->imc < 26.4) strcpy(cliente->classificacao, "Peso normal");
        else if (cliente->imc < 27.8) strcpy(cliente->classificacao, "Marginalmente acima");
        else if (cliente->imc < 31.1) strcpy(cliente->classificacao, "Acima do peso ideal");
        else strcpy(cliente->classificacao, "Obeso");
    } 
    // Classificação para mulheres
    else {
        if (cliente->imc < 19.1) strcpy(cliente->classificacao, "Abaixo do peso");
        else if (cliente->imc < 25.8) strcpy(cliente->classificacao, "Peso normal");
        else if (cliente->imc < 27.3) strcpy(cliente->classificacao, "Marginalmente acima");
        else if (cliente->imc < 32.3) strcpy(cliente->classificacao, "Acima do peso ideal");
        else strcpy(cliente->classificacao, "Obeso");
    }
}

// Calcula o peso ideal baseado na altura e sexo
float calcularPesoIdeal(float altura, char sexo) {    
    float pesoIdeal = .0;
	
    if (sexo == MASCULINO) {
        pesoIdeal = (72.7 * altura) - 58;  // Fórmula para homens
    }
    if (sexo == FEMININO) {
        pesoIdeal = (62.1 * altura) - 44.7;  // Fórmula para mulheres
    } 
    
    return pesoIdeal;
}

// Cadastra um novo cliente no sistema
void cadastrarCliente() {
	
    setlocale(LC_ALL, "Portuguese");
	
	// Verifica se há espaço para mais clientes
    if (totalClientes >= MAX_CLIENTES) {
        imprimirColorido(cor.vermelho, "Limite de clientes atingido!\n");
        return;
    }
    
    Cliente novo;  // Cria uma nova estrutura Cliente
    
    // Exibe cabeçalho
    imprimirColorido(cor.azul, "\n--- Cadastro de Cliente ---\n");
    
    // Lê o nome
    printf("Nome: ");
    
    fgets(novo.nome, 50, stdin);	
    novo.nome[strcspn(novo.nome, "\n")] = 0;  // Remove a quebra de linha do final
    
    
    // Valida e lê o sexo (apenas 'M' ou 'F')
    do {
        printf("Sexo (M/F): ");
        scanf(" %c", &novo.sexo);
        limparBuffer();
        novo.sexo = toupper(novo.sexo);  // Converte para maiúscula
        if (novo.sexo != 'M' && novo.sexo != 'F') {
            imprimirColorido(cor.vermelho, "Sexo inválido! Digite M ou F.\n");
        }
    } while (novo.sexo != 'M' && novo.sexo != 'F');
    
    // Lê peso e altura (com validação)
    novo.peso = lerFloatPositivo("Peso (kg): ");
    novo.altura = lerFloatPositivo("Altura (m): ");
    
    // Calcula IMC e classificação
    calcularIMC(&novo);
    
    // Adiciona o novo cliente ao array
    clientes[totalClientes++] = novo;
    
    system("cls");  // Limpa a tela
    
    // Exibe resumo do cadastro
    imprimirColorido(cor.amarelo, " Dados do Cliente:\n");
    imprimirColorido(cor.azul, "  Nome: %s\n", novo.nome);
    imprimirColorido(cor.azul, "  Sexo: %c\n", novo.sexo);
    imprimirColorido(cor.azul, "  Peso: %.2f\n", novo.peso);
    imprimirColorido(cor.azul, "  Altura: %.2f\n", novo.altura);
    imprimirColorido(cor.verde, "\n IMC calculado: %.2f - %s\n", novo.imc, novo.classificacao);
    printf("\n");
    
    // Mostra tabela de referência apropriada
    if (novo.sexo == MASCULINO) {
        imprimirTabelaIMC_Homem();
    } else {
        imprimirTabelaIMC_Mulher();
    }
    
    // Calcula e exibe o peso ideal
    float pesoIdeal = calcularPesoIdeal(novo.altura, novo.sexo);
    imprimirColorido(cor.amarelo, "\n Peso Ideal: %.2f\t%s você está %.2fkg %s do seu peso ideal\n", 
        pesoIdeal, novo.nome, fabs(novo.peso - pesoIdeal), 
        (novo.peso > pesoIdeal) ? "Acima" : "Abaixo");
    
    printf("\n\n\t");
    salvarDados();  // Salva os dados no arquivo
}

// Lista todos os clientes cadastrados em formato de tabela
void listarClientes() {
	

    // Verifica se há clientes cadastrados
    if (totalClientes == 0) {
        imprimirColorido(cor.amarelo, "\nNenhum cliente cadastrado!\n");
        return;
    }
    
    // Exibe cabeçalho
    imprimirColorido(cor.azul, "\n--- Lista de Clientes ---\n");
    
    // Cabeçalho da tabela com fundo amarelo e texto preto
    printf("%s%s", cor.fundo_amarelo, cor.preto);
    printf("%-30s %-5s %-8s %-8s %-8s %-25s\n%-89s\n", 
           "Nome", "Sexo", "Peso", "Altura", "IMC", "Classificação", " ");
    printf("%s", cor.reset);
    
    // Imprime cada cliente com cores alternadas para melhor legibilidade
    for (int i = 0; i < totalClientes; i++) {
        const char* corLinha = (i % 2 == 0) ? cor.fundo_branco : cor.fundo_preto;
        printf("%s%s", corLinha, (i % 2 == 0) ? cor.preto : cor.branco);
        
        printf("%-30s %-5c %-8.2f %-8.2f %-8.2f %-25s\n", 
               clientes[i].nome, 
               clientes[i].sexo, 
               clientes[i].peso, 
               clientes[i].altura, 
               clientes[i].imc, 
               clientes[i].classificacao);
        
        printf("%s", cor.reset);
    }
}

// Pesquisa clientes por nome ou parte do nome
void pesquisarCliente() {
    // Verifica se há clientes cadastrados
    if (totalClientes == 0) {
        imprimirColorido(cor.amarelo, "\nNenhum cliente cadastrado para pesquisar!\n");
        return;
    }

    char termo[50];  // Buffer para o termo de pesquisa
    printf("\nDigite o nome ou parte do nome para pesquisar: ");
    fgets(termo, 50, stdin);
    termo[strcspn(termo, "\n")] = 0;  // Remove a quebra de linha

    // Exibe cabeçalho
    imprimirColorido(cor.azul, "\n--- Resultados da Pesquisa ---\n");
    
    // Cabeçalho da tabela
    printf("%s%s", cor.fundo_branco, cor.preto);
    printf("%-30s %-5s %-8s %-8s %-8s %-25s\n", 
           "Nome", "Sexo", "Peso", "Altura", "IMC", "Classificação");
    printf("%s", cor.reset);

    int encontrados = 0;  // Contador de clientes encontrados
    
    // Percorre todos os clientes procurando pelo termo
    for (int i = 0; i < totalClientes; i++) {
        if (strstr(clientes[i].nome, termo) != NULL) {  // Verifica se o termo está no nome
            const char* corLinha = (encontrados % 2 == 0) ? cor.fundo_branco : cor.fundo_preto;
            printf("%s%s", corLinha, cor.preto);
            
            printf("%-30s %-5c %-8.2f %-8.2f %-8.2f %-25s\n", 
                   clientes[i].nome, 
                   clientes[i].sexo, 
                   clientes[i].peso, 
                   clientes[i].altura, 
                   clientes[i].imc, 
                   clientes[i].classificacao);
            
            printf("%s", cor.reset);
            encontrados++;
        }
    }

    // Exibe mensagem se nenhum cliente foi encontrado
    if (encontrados == 0) {
        imprimirColorido(cor.vermelho, "\nNenhum cliente encontrado com o termo: %s\n", termo);
    } else {
        imprimirColorido(cor.verde, "\nTotal encontrado: %d\n", encontrados);
    }
}

// Salva os dados dos clientes em um arquivo binário
void salvarDados() {
    // Abre o arquivo para escrita binária
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");
    if (arquivo == NULL) {
        imprimirColorido(cor.vermelho, "Erro ao abrir arquivo para salvar!\n");
        return;
    }
    
    // Escreve o total de clientes e depois o array de clientes
    fwrite(&totalClientes, sizeof(int), 1, arquivo);
    fwrite(clientes, sizeof(Cliente), totalClientes, arquivo);
    
    fclose(arquivo);  // Fecha o arquivo
    imprimirColorido(cor.verde, "Dados salvos com sucesso!\n");
}

// Carrega os dados dos clientes de um arquivo binário
void carregarDados() {
    // Abre o arquivo para leitura binária
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");
    if (arquivo == NULL) {
        return;  // Se o arquivo não existe, simplesmente retorna
    }
    
    // Lê o total de clientes e depois o array de clientes
    fread(&totalClientes, sizeof(int), 1, arquivo);
    fread(clientes, sizeof(Cliente), totalClientes, arquivo);
    
    fclose(arquivo);  // Fecha o arquivo
}

// Exibe o menu principal
void mostrarMenu() {
    imprimirColorido(cor.azul, "\n=== CALCULADORA DE IMC ===\n");
    printf("1. Calcular IMC\n");
    printf("2. Listar clientes\n");
    printf("3. Pesquisar cliente\n");
    printf("5. Sair\n");
    imprimirColorido(cor.amarelo, "Escolha uma opção: ");
}

// Exibe a tabela de classificação do IMC
void imprimirTabelaIMC() {
    // Título
    printf("%s%s", cor.fundo_preto, cor.azul);
    printf("\n=== TABELA DE CLASSIFICAÇÃO IMC ===\n");
    printf("%s", cor.reset);
    
    // Tabelas para homens e mulheres
    imprimirTabelaIMC_Homem();
    imprimirTabelaIMC_Mulher();
}

// Exibe a tabela de classificação do IMC para homens
void imprimirTabelaIMC_Homem() {
    printf("\n%sCLASSIFICAÇÃO PARA HOMENS%s\n", cor.azul, cor.reset);
    printf("%s---------------------------------------------------%s\n", cor.branco, cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.amarelo ,"Faixa de IMC", "Classificação", cor.reset);
    printf("%s|-------------------------------------------------|%s\n", cor.branco, cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.azul, "Abaixo de 20.7", "Abaixo do peso", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.verde, "20.7 - 26.4", "Peso normal", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.amarelo, "26.4 - 27.8", "Marginalmente acima", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.laranja, "27.8 - 31.1", "Acima do peso ideal", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.vermelho, "Acima de 31.1", "Obeso", cor.reset);
    printf("%s---------------------------------------------------%s\n", cor.branco, cor.reset);
}

// Exibe a tabela de classificação do IMC para mulheres
void imprimirTabelaIMC_Mulher() {
    printf("\n%sCLASSIFICAÇÃO PARA MULHERES%s\n", cor.azul, cor.reset);
    printf("%s---------------------------------------------------%s\n", cor.branco, cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.amarelo, "Faixa de IMC", "Classificação", cor.reset);
    printf("%s|-------------------------------------------------|%s\n", cor.branco, cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.azul, "Abaixo de 19.1", "Abaixo do peso", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.verde, "19.1 - 25.8", "Peso normal", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.amarelo, "25.8 - 27.3", "Marginalmente acima", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.laranja, "27.3 - 32.3", "Acima do peso ideal", cor.reset);
    printf("%s| %-25s | %-19s |%s\n", cor.vermelho, "Acima de 32.3", "Obeso", cor.reset);
    printf("%s---------------------------------------------------%s\n\n", cor.branco, cor.reset);
}

// Exibe arte ASCII do título
void imprimir_ascii_art() {
    printf("\n");
    imprimirColorido(cor.azul,     " __________    __       __   ___________       ___________   ___________   ___________  ____     ____  ___________   ___________  ___       ___  ___________  ___       ___  _________\n");
imprimirColorido(cor.vermelho, "¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦     ¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦     ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦     ¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦\n");
imprimirColorido(cor.vermelho, " ¯¯¯¯¦¦¦¯¯¯¯  ¦¦¦¦¦   ¦¦¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯      ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦¦¦   ¦¦¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯   ¯¯¯ ¦¦¦¯¯¯¯  ¦¦¦       ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯  ¦¦¦       ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯ \n");
imprimirColorido(cor.vermelho, "     ¦¦¦      ¦¦¦¦¦¦ ¦¦¦¦¦¦ ¦¦¦               ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦ ¦¦¦¦¦¦ ¦¦¦                ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦          \n");
imprimirColorido(cor.vermelho, "     ¦¦¦      ¦¦¦ ¦¦¦¦¦ ¦¦¦ ¦¦¦               ¦¦¦_______¦¦¦ ¦¦¦_______¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦ ¦¦¦¦¦ ¦¦¦ ¦¦¦_________       ¦¦¦      ¦¦¦_______¦¦¦ ¦¦¦_________  ¦¦¦       ¦¦¦ ¦¦¦_________ \n");
imprimirColorido(cor.vermelho, "     ¦¦¦      ¦¦¦  ¦¦¦  ¦¦¦ ¦¦¦               ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦  ¦¦¦  ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦      ¦¦¦      ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦\n");
imprimirColorido(cor.vermelho, "     ¦¦¦      ¦¦¦   ¯   ¦¦¦ ¦¦¦               ¦¦¦¯¯¯¯¯¯¯¯¯  ¦¦¦¯¯¯¯¦¦¦¯¯  ¦¦¦       ¦¦¦ ¦¦¦   ¯   ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯       ¦¦¦      ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯  ¦¦¦       ¦¦¦  ¯¯¯¯¯¯¯¯¦¦¦\n");
imprimirColorido(cor.vermelho, "     ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦               ¦¦¦           ¦¦¦     ¦¦¦   ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦                ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦           ¦¦¦       ¦¦¦          ¦¦¦\n");
imprimirColorido(cor.vermelho, " ____¦¦¦____  ¦¦¦       ¦¦¦ ¦¦¦_________      ¦¦¦           ¦¦¦      ¦¦¦  ¦¦¦_______¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦_________       ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦_________  ¦¦¦_______¦¦¦  ________¦¦¦\n");
imprimirColorido(cor.vermelho, "¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦     ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦      ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦\n");
imprimirColorido(cor.azul, 	   " ¯¯¯¯¯¯¯¯¯¯¯  ¯¯¯       ¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯      ¯¯¯           ¯¯¯       ¯¯¯ ¯¯¯¯¯¯¯¯¯¯¯¯¯ ¯¯¯       ¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯       ¯¯¯      ¯¯¯       ¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯¯¯   ¯¯¯¯¯¯¯¯¯¯¯¯\n");
                                                                                                                                                                              
imprimirColorido(cor.verde,   "____       ___  ___________   ___________       ___________  ___       ___ ____     ____  ___________   ___________ ___________   ___       ___  ___________\n");                          
imprimirColorido(cor.amarelo, "¦¦¦¦      ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦     ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦     ¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦\n");                          
imprimirColorido(cor.amarelo, "¦¦¦¦¦     ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦     ¦¦¦¯¯¯¯¯¯¯¯¯  ¦¦¦       ¦¦¦ ¦¦¦¦¦   ¦¦¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¯¯¯¯ ¦¦¦¯¯¯¯  ¦¦¦       ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯ \n");                          
imprimirColorido(cor.amarelo, "¦¦¦¦¦¦    ¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦     ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦¦¦¦ ¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦      ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦          \n");                          
imprimirColorido(cor.amarelo, "¦¦¦ ¦¦¦   ¦¦¦ ¦¦¦_______¦¦¦ ¦¦¦       ¦¦¦     ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦ ¦¦¦¦¦ ¦¦¦ ¦¦¦_______¦¦¦ ¦¦¦_______¦¦¦      ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦_________ \n");                          
imprimirColorido(cor.amarelo, "¦¦¦  ¦¦¦  ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦     ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦  ¦¦¦  ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦      ¦¦¦      ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦\n");                          
imprimirColorido(cor.amarelo, "¦¦¦   ¦¦¦ ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¦¦¦ ¦¦¦       ¦¦¦     ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦   ¯   ¦¦¦ ¦¦¦¯¯¯¯¯¯¯¯¯  ¦¦¦¯¯¯¯¦¦¦¯¯       ¦¦¦      ¦¦¦       ¦¦¦  ¯¯¯¯¯¯¯¯¦¦¦\n");                          
imprimirColorido(cor.amarelo, "¦¦¦    ¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦     ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦           ¦¦¦     ¦¦¦        ¦¦¦      ¦¦¦       ¦¦¦          ¦¦¦\n");                          
imprimirColorido(cor.amarelo, "¦¦¦     ¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦_______¦¦¦     ¦¦¦_________  ¦¦¦_______¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦           ¦¦¦      ¦¦¦   ____¦¦¦____  ¦¦¦_______¦¦¦  ________¦¦¦\n");                          
imprimirColorido(cor.amarelo, "¦¦¦      ¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦     ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦       ¦¦¦ ¦¦¦           ¦¦¦       ¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦ ¦¦¦¦¦¦¦¦¦¦¦¦¦\n");                          
imprimirColorido(cor.verde,   "¯¯¯       ¯¯  ¯¯¯       ¯¯¯ ¯¯¯¯¯¯¯¯¯¯¯¯       ¯¯¯¯¯¯¯¯¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯¯  ¯¯¯       ¯¯¯ ¯¯¯           ¯¯¯       ¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯  ¯¯¯¯¯¯¯¯¯¯¯    ¯¯¯¯¯¯¯¯¯¯¯ \n");            
    printf("\n");
}
