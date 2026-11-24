#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 500;

// Variaveis das contas
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;

// Variaveis do historico de movimentacoes
int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
double valorMovimentacao[MAX_MOVIMENTACOES];
int quantidadeMovimentacoes = 0;

// Funcoes de persistencia em arquivos
void carregarContas() {
    ifstream arquivo("contas.txt");
    if (arquivo.is_open()) {
        string numStr, nome, cpfLido, tipoStr, saldoStr, ativaStr;
        while (quantidadeContas < MAX_CONTAS && getline(arquivo, numStr, ';')) {
            if (numStr.empty()) continue;
            getline(arquivo, nome, ';');
            getline(arquivo, cpfLido, ';');
            getline(arquivo, tipoStr, ';');
            getline(arquivo, saldoStr, ';');
            getline(arquivo, ativaStr);

            if (!nome.empty() && nome[0] == ' ') nome.erase(0, 1);
            if (!cpfLido.empty() && cpfLido[0] == ' ') cpfLido.erase(0, 1);

            int ativaInt = 0;
            stringstream(numStr) >> numeroConta[quantidadeContas];
            nomeCliente[quantidadeContas] = nome;
            cpf[quantidadeContas] = cpfLido;
            stringstream(tipoStr) >> tipoConta[quantidadeContas];
            stringstream(saldoStr) >> saldo[quantidadeContas];
            stringstream(ativaStr) >> ativaInt;
            contaAtiva[quantidadeContas] = (ativaInt == 1);

            quantidadeContas++;
        }
        arquivo.close();
    }
}

void salvarContas() {
    ofstream arquivo("contas.txt");
    if (arquivo.is_open()) {
        for (int i = 0; i < quantidadeContas; i++) {
            arquivo << numeroConta[i] << ";"
                    << nomeCliente[i] << ";"
                    << cpf[i] << ";"
                    << tipoConta[i] << ";"
                    << saldo[i] << ";"
                    << contaAtiva[i] << endl;
        }
        arquivo.close();
    }
}

void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");
    if (arquivo.is_open()) {
        string contaStr, tipoStr, valorStr;
        while (quantidadeMovimentacoes < MAX_MOVIMENTACOES && getline(arquivo, contaStr, ';')) {
            if (contaStr.empty()) continue;
            getline(arquivo, tipoStr, ';');
            getline(arquivo, valorStr);

            if (!tipoStr.empty() && tipoStr[0] == ' ') tipoStr.erase(0, 1);

            stringstream(contaStr) >> contaMovimentacao[quantidadeMovimentacoes];
            tipoMovimentacao[quantidadeMovimentacoes] = tipoStr[0];
            stringstream(valorStr) >> valorMovimentacao[quantidadeMovimentacoes];

            quantidadeMovimentacoes++;
        }
        arquivo.close();
    }
}

void salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");
    if (arquivo.is_open()) {
        for (int i = 0; i < quantidadeMovimentacoes; i++) {
            arquivo << contaMovimentacao[i] << ";"
                    << tipoMovimentacao[i] << ";"
                    << valorMovimentacao[i] << endl;
        }
        arquivo.close();
    }
}

// Funcoes auxiliares
int buscarConta(int numeroBusca) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numeroBusca && contaAtiva[i]) {
            return i;
        }
    }
    return -1;
}

bool voltarMenu() {
    char resposta;
    cout << endl;
    cout << "Voce deseja voltar para a pagina inicial do sistema? (S/N): ";
    cin >> resposta;
    if (resposta == 'N' || resposta == 'n') {
        cout << "Encerrando o sistema..." << endl;
        return false;
    }
    return true;
}

int main() {
    for (int i = 0; i < MAX_CONTAS; i++) {
        contaAtiva[i] = false;
    }

    carregarContas();
    carregarMovimentacoes();

    int opcao = 0;

    while (opcao != 8) {
        cout << endl;
        cout << "**************************************" << endl;
        cout << "**         BANCO INF101             **" << endl;
        cout << "**************************************" << endl;
        cout << "1 - Cadastrar conta" << endl;
        cout << "2 - Listar contas cadastradas" << endl;
        cout << "3 - Consultar uma conta pelo numero" << endl;
        cout << "4 - Realizar depositos" << endl;
        cout << "5 - Realizar saques, verificando saldo suficiente." << endl;
        cout << "6 - Realizar transferencias entre duas contas." << endl;
        cout << "7 - Exibir o extrato de uma conta." << endl;
        cout << "8 - Sair do sistema." << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                if (quantidadeContas >= MAX_CONTAS) {
                    cout << "Limite maximo de contas atingido." << endl;
                    break;
                }

                cout << endl;
                cout << "==== CADASTRO DE CONTA ====" << endl;
                cout << "Digite seu nome: ";
                cin >> nomeCliente[quantidadeContas];

                do {
                    cout << "Digite seu CPF (apenas 11 numeros): ";
                    cin >> cpf[quantidadeContas];
                    if (cpf[quantidadeContas].length() != 11) {
                        cout << "O CPF deve conter exatamente 11 digitos." << endl;
                    }
                } while (cpf[quantidadeContas].length() != 11);

                cout << "Digite o numero da conta: ";
                cin >> numeroConta[quantidadeContas];

                do {
                    cout << "Selecione o tipo da sua conta:" << endl;
                    cout << "1 - Corrente" << endl;
                    cout << "2 - Poupanca" << endl;
                    cout << "Opcao: ";
                    cin >> tipoConta[quantidadeContas];
                    if (tipoConta[quantidadeContas] != 1 && tipoConta[quantidadeContas] != 2) {
                        cout << "Tipo de conta invalido. Escolha 1 ou 2." << endl;
                    }
                } while (tipoConta[quantidadeContas] != 1 && tipoConta[quantidadeContas] != 2);

                do {
                    cout << "Digite o saldo inicial: R$ ";
                    cin >> saldo[quantidadeContas];
                    if (saldo[quantidadeContas] < 0) {
                        cout << "O saldo inicial nao pode ser negativo." << endl;
                    }
                } while (saldo[quantidadeContas] < 0);

                contaAtiva[quantidadeContas] = true;
                quantidadeContas++;
                salvarContas();

                cout << "Conta cadastrada com sucesso." << endl;
                break;
            }

            case 2: {
                if (quantidadeContas == 0) {
                    cout << "Não existe conta cadastrada no momento." << endl;
                } else {
                    for (int i = 0; i < quantidadeContas; i++) {
                        cout << endl;
                        cout << "==== LISTAGEM DE CONTAS ====" << endl;
                        cout << "---- CONTA " << (i + 1) << " ----" << endl;
                        cout << "Numero da conta: " << numeroConta[i] << endl;
                        cout << "Cliente: " << nomeCliente[i] << endl;
                        cout << "CPF: " << cpf[i] << endl;
                        cout << "Tipo de conta: " << (tipoConta[i] == 1 ? "Corrente" : "Poupanca") << endl;
                        cout << "Saldo: R$ " << saldo[i] << endl;
                    }
                }
                if (!voltarMenu()) return 0;
                break;
            }

            case 3: {
                cout << endl;
                cout << "==== CONSULTAR CONTA ====" << endl;
                int numeroBusca;
                cout << "Digite o numero da conta que deseja buscar: ";
                cin >> numeroBusca;

                int indiceConta = buscarConta(numeroBusca);
                if (indiceConta != -1) {
                    cout << endl;
                    cout << "--- CONTA ENCONTRADA ---" << endl;
                    cout << "Cliente: " << nomeCliente[indiceConta] << endl;
                    cout << "CPF: " << cpf[indiceConta] << endl;
                    cout << "Tipo de conta: " << (tipoConta[indiceConta] == 1 ? "Corrente" : "Poupanca") << endl;
                    cout << "Saldo: R$ " << saldo[indiceConta] << endl;
                } else {
                    cout << "Não existe conta cadastrada com o número " << numeroBusca << "." << endl;
                }
                if (!voltarMenu()) return 0;
                break;
            }

            case 4: {
                cout << endl;
                cout << "==== REALIZAR DEPOSITO BANCARIO ====" << endl;
                int numeroBusca;
                cout << "Digite o numero da conta para realizar o deposito: ";
                cin >> numeroBusca;

                int indiceConta = buscarConta(numeroBusca);
                if (indiceConta != -1) {
                    char fazerNovoDeposito;
                    do {
                        double valorDeposito;
                        do {
                            cout << "Digite o valor para ser depositado na conta: R$ ";
                            cin >> valorDeposito;
                            if (valorDeposito <= 0) {
                                cout << "O valor do deposito deve ser maior que zero." << endl;
                            }
                        } while (valorDeposito <= 0);

                        saldo[indiceConta] += valorDeposito;

                        if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
                            contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceConta];
                            tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                            valorMovimentacao[quantidadeMovimentacoes] = valorDeposito;
                            quantidadeMovimentacoes++;

                            salvarContas();
                            salvarMovimentacoes();

                            cout << "Deposito de R$ " << valorDeposito << " realizado com sucesso!" << endl;
                            cout << "Novo saldo: R$ " << saldo[indiceConta] << endl;
                        } else {
                            cout << "Limite de movimentacoes do sistema atingido." << endl;
                        }

                        cout << endl;
                        cout << "Deseja realizar um novo deposito nesta conta? (S/N): ";
                        cin >> fazerNovoDeposito;
                    } while (fazerNovoDeposito == 'S' || fazerNovoDeposito == 's');
                } else {
                    cout << "Não existe conta cadastrada com o numero " << numeroBusca << "." << endl;
                }
                if (!voltarMenu()) return 0;
                break;
            }

            case 5: {
                cout << endl;
                cout << "==== REALIZAR SAQUE BANCARIO ====" << endl;
                int numeroBusca;
                cout << "Digite o numero da conta para realizar o saque: ";
                cin >> numeroBusca;

                int indiceConta = buscarConta(numeroBusca);
                if (indiceConta != -1) {
                    char fazerNovoSaque;
                    do {
                        double valorSaque;
                        do {
                            cout << "Digite o valor que ira ser sacado: R$ ";
                            cin >> valorSaque;
                            if (valorSaque <= 0) {
                                cout << "O valor do saque deve ser maior que zero." << endl;
                            }
                        } while (valorSaque <= 0);

                        if (saldo[indiceConta] >= valorSaque) {
                            saldo[indiceConta] -= valorSaque;

                            if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
                                contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indiceConta];
                                tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                                valorMovimentacao[quantidadeMovimentacoes] = valorSaque;
                                quantidadeMovimentacoes++;

                                salvarContas();
                                salvarMovimentacoes();

                                cout << "Saque de R$ " << valorSaque << " realizado com sucesso!" << endl;
                                cout << "Novo saldo: R$ " << saldo[indiceConta] << endl;
                            } else {
                                cout << "Limite de movimentacoes do sistema atingido." << endl;
                            }
                        } else {
                            cout << "Saldo insuficiente. Seu saldo atual e: R$ " << saldo[indiceConta] << endl;
                        }

                        cout << endl;
                        cout << "Deseja realizar um novo saque nesta conta? (S/N): ";
                        cin >> fazerNovoSaque;
                    } while (fazerNovoSaque == 'S' || fazerNovoSaque == 's');
                } else {
                    cout << "Nenhuma conta ativa encontrada com o numero " << numeroBusca << "." << endl;
                }
                if (!voltarMenu()) return 0;
                break;
            }

            case 6: {
                cout << endl;
                cout << "==== REALIZAR TRANSFERENCIA ENTRE CONTAS ====" << endl;
                int numOrigem, numDestino;
                cout << "Digite o numero da conta que vai realizar a transferencia: ";
                cin >> numOrigem;
                cout << "Digite o numero da conta que vai receber a transferencia: ";
                cin >> numDestino;

                int indOrigem = buscarConta(numOrigem);
                int indDestino = buscarConta(numDestino);

                if (indOrigem != -1 && indDestino != -1) {
                    double valorTransferencia;
                    cout << "Digite o valor da transferencia: R$ ";
                    cin >> valorTransferencia;

                    if (valorTransferencia <= 0) {
                        cout << "O valor da transferencia deve ser maior que zero!" << endl;
                    } else if (saldo[indOrigem] < valorTransferencia) {
                        cout << "Saldo insuficiente na conta de origem!" << endl;
                    } else if (quantidadeMovimentacoes + 1 >= MAX_MOVIMENTACOES) {
                        cout << "O sistema atingiu o limite de movimentacoes permitidas!" << endl;
                    } else {
                        saldo[indOrigem] -= valorTransferencia;
                        saldo[indDestino] += valorTransferencia;

                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indOrigem];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'T';
                        valorMovimentacao[quantidadeMovimentacoes] = valorTransferencia;
                        quantidadeMovimentacoes++;

                        contaMovimentacao[quantidadeMovimentacoes] = numeroConta[indDestino];
                        tipoMovimentacao[quantidadeMovimentacoes] = 'R';
                        valorMovimentacao[quantidadeMovimentacoes] = valorTransferencia;
                        quantidadeMovimentacoes++;

                        salvarContas();
                        salvarMovimentacoes();

                        cout << "Transferencia de R$ " << valorTransferencia << " realizada com sucesso!" << endl;
                    }
                } else {
                    cout << "Conta de origem ou destino nao encontrada (ou inativa)!" << endl;
                }
                if (!voltarMenu()) return 0;
                break;
            }

            case 7: {
                cout << endl;
                cout << "==== EXTRATO BANCARIO DE UMA CONTA ====" << endl;
                int numeroBusca;
                cout << "Digite o numero da conta para exibir o seu extrato bancario: ";
                cin >> numeroBusca;

                int indiceConta = buscarConta(numeroBusca);
                if (indiceConta != -1) {
                    cout << endl;
                    cout << "--- HISTORICO DE MOVIMENTACOES ---" << endl;
                    bool temMovimentacao = false;

                    for (int i = 0; i < quantidadeMovimentacoes; i++) {
                        if (contaMovimentacao[i] == numeroBusca) {
                            temMovimentacao = true;
                            if (tipoMovimentacao[i] == 'D') {
                                cout << "Deposito: + R$ " << valorMovimentacao[i] << endl;
                            } else if (tipoMovimentacao[i] == 'S') {
                                cout << "Saque: - R$ " << valorMovimentacao[i] << endl;
                            } else if (tipoMovimentacao[i] == 'T') {
                                cout << "Transferencia enviada: - R$ " << valorMovimentacao[i] << endl;
                            } else if (tipoMovimentacao[i] == 'R') {
                                cout << "Transferencia recebida: + R$ " << valorMovimentacao[i] << endl;
                            }
                        }
                    }

                    if (!temMovimentacao) {
                        cout << "No momento, nao ha movimentacoes registradas nessa conta." << endl;
                    }

                    cout << "----------------------------------" << endl;
                    cout << "Saldo atual: R$ " << saldo[indiceConta] << endl;
                } else {
                    cout << "Nao existe nenhuma conta cadastrada com o numero " << numeroBusca << "." << endl;
                }
                if (!voltarMenu()) return 0;
                break;
            }

            default:
                if (opcao != 8) {
                    cout << endl;
                    cout << "Opcao invalida. Tente novamente." << endl;
                }
                break;
        }
    }

    cout << endl;
    cout << "Encerrando o sistema..." << endl;

    return 0;
}