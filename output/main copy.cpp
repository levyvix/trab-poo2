#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <filesystem> // For directory handling
#include <cmath> // For rounding
#include <iomanip> // For formatting output
#include <algorithm> // For remove
#include <utility> // for pair
#include <numeric> // for accumulate

using namespace std;
namespace fs = filesystem; // Alias for filesystem

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
   return dynamic_cast<const Base*>(ptr) != nullptr;
}

void reiniciarArquivoPlanejamento(string pasta) {
    fs::path arquivo = pasta+"/"+"1-planejamento.csv"; // Path to the file

    // Check if the file exists
    if (fs::exists(arquivo)) {
        // Attempt to delete the file
        if (fs::remove(arquivo)) {
            cout << "Arquivo '1-planejamento.csv' foi deletado para ser recriado." << endl;
        } else {
            cerr << "Erro ao tentar deletar o arquivo '1-planejamento.csv'." << endl;
        }
    } else {
        cout << "Arquivo '1-planejamento.csv' não existe." << endl;
    }
}

class YearMonth {
private:
    int year;
    int month;


public:
    YearMonth(int year, int month) : year(year), month(month) {}

    int getYear() const { return year; }
    int getMonth() const { return month; }

    YearMonth plusMonths(int months) const {
        int totalMonths = year * 12 + month - 1 + months;
        int newYear = totalMonths / 12;
        int newMonth = totalMonths % 12 + 1;
        return YearMonth(newYear, newMonth);
    }

    bool isAfter(const YearMonth& other) const {
        return (year > other.year) || (year == other.year && month > other.month);
    }

    bool operator<(const YearMonth& other) const {
        return (year < other.year) || (year == other.year && month < other.month);
    }

    bool operator==(const YearMonth& other) const {
        return year == other.year && month == other.month;
    }
};



string formatYearMonth(const YearMonth& ym) {
    stringstream ss;
    ss << setw(2) << setfill('0') << ym.getMonth() << "/"
       << ym.getYear();
    return ss.str();
}

string formatCurrencyBr(double value) {
    stringstream ss;
    ss << fixed << setprecision(2) << value; // Format with 2 decimal places
    string result = ss.str();

    // Remove thousands separators (commas)
    result.erase(remove(result.begin(), result.end(), ','), result.end());

    // Replace decimal point with comma
    size_t dot = result.find('.');
    if (dot != string::npos) {
        result[dot] = ',';
    }

    return "R$ " + result;
}



// Define the Expense class
class Expense {
public:
    YearMonth start;
    double installment;
    int numParcels;

    Expense(YearMonth start, double installment, int numParcels)
        : start(start), installment(installment), numParcels(numParcels) {}
};

YearMonth parseDateToYearMonth(const string& date) {
    // Assuming date format is "DD/MM/YYYY"
    int year = stoi(date.substr(6, 4));
    int month = stoi(date.substr(3, 2));
    return YearMonth(year, month);
}





class Casal {
private:
    string nome1;
    string nome2;

public:
    Casal(string nome1, string nome2) {
        // Ensure nome1 is lexically smaller than nome2
        if (nome2 < nome1) {
            this->nome1 = nome2;
            this->nome2 = nome1;
        } else {
            this->nome1 = nome1;
            this->nome2 = nome2;
        }
    }

    bool operator<(const Casal& other) const {
        if (nome1 != other.nome1) {
            return nome1 < other.nome1;
        }
        return nome2 < other.nome2;
    }

    string getNome1() const { return nome1; }
    string getNome2() const { return nome2; }

    void setNome1(const string& nome1) { this->nome1 = nome1; }
    void setNome2(const string& nome2) { this->nome2 = nome2; }
};

class Pessoa {
private:
    string id;
    string tipo;
    string nome;
    string telefone;
    string endereco;

public:
    Pessoa(string id, string tipo, string nome, string telefone, string endereco)
        : id(id), tipo(tipo), nome(nome), telefone(telefone), endereco(endereco) {}

    virtual ~Pessoa() = default;

    string getId() const { return id; }
    string getTipo() const { return tipo; }
    string getNome() const { return nome; }
    string getTelefone() const { return telefone; }
    string getEndereco() const { return endereco; }
    bool getisLoja()const {return false;}

    void setId(const string& id) { this->id = id; }
    void setTipo(const string& tipo) { this->tipo = tipo; }
    void setNome(const string& nome) { this->nome = nome; }
    void setTelefone(const string& telefone) { this->telefone = telefone; }
    void setEndereco(const string& endereco) { this->endereco = endereco; }
};

class PessoaFisica : public Pessoa {
private:
    string cpf;
    string dataNascimento;
    double dinheiroGuardado;
    double salario;
    double gastosMensais;

public:
    PessoaFisica(string id, string tipo, string nome, string telefone, string endereco,
                 string cpf, string dataNascimento, double dinheiroGuardado, double salario, double gastosMensais)
        : Pessoa(id, tipo, nome, telefone, endereco), cpf(cpf), dataNascimento(dataNascimento),
          dinheiroGuardado(dinheiroGuardado), salario(salario), gastosMensais(gastosMensais) {}

    string getCpf() const { return cpf; }
    string getDataNascimento() const { return dataNascimento; }
    double getDinheiroGuardado() const { return dinheiroGuardado; }
    double getSalario() const { return salario; }
    double getGastosMensais() const { return gastosMensais; }
    bool getisLoja() const { return false; }

    void setCpf(const string& cpf) { this->cpf = cpf; }
    void setDataNascimento(const string& dataNascimento) { this->dataNascimento = dataNascimento; }
    void setDinheiroGuardado(double dinheiroGuardado) { this->dinheiroGuardado = dinheiroGuardado; }
    void setSalario(double salario) { this->salario = salario; }
    void setGastosMensais(double gastosMensais) { this->gastosMensais = gastosMensais; }
};

class PessoaJuridica : public Pessoa {
private:
    string cnpj;

public:
    PessoaJuridica(string id, string tipo, string nome, string telefone, string endereco, string cnpj)
        : Pessoa(id, tipo, nome, telefone, endereco), cnpj(cnpj) {}

    string getCnpj() const { return cnpj; }

    bool getisLoja() const { return false; }

    void setCnpj(const string& cnpj) { this->cnpj = cnpj; }
};


class Loja : public Pessoa {
private:
    string cnpj;

public:
    Loja(string id, string tipo, string nome, string telefone, string endereco, string cnpj)
        : Pessoa(id, tipo, nome, telefone, endereco), cnpj(cnpj) {}

    string getCnpj() const { return cnpj; }

    void setCnpj(const string& cnpj) { this->cnpj = cnpj; }

    bool getisLoja() const { return true; }
};

class Casamento {
private:
    string idCasamento;
    string id1;
    string id2;
    string data;
    string hora;
    string local;

public:
    Casamento(string idCasamento, string id1, string id2, string data, string hora, string local)
        : idCasamento(idCasamento), id1(id1), id2(id2), data(data), hora(hora), local(local) {}

    string getIdCasamento() const { return idCasamento; }
    string getId1() const { return id1; }
    string getId2() const { return id2; }
    string getData() const { return data; }
    string getHora() const { return hora; }
    string getLocal() const { return local; }

    void setIdCasamento(const string& idCasamento) { this->idCasamento = idCasamento; }
    void setId1(const string& id1) { this->id1 = id1; }
    void setId2(const string& id2) { this->id2 = id2; }
    void setData(const string& data) { this->data = data; }
    void setHora(const string& hora) { this->hora = hora; }
    void setLocal(const string& local) { this->local = local; }
};

class Compra {
private:
    string id;
    string idTarefa;
    string idLoja;
    string nomeProduto;
    int qtdeProduto;
    double precoUnitario;
    int numeroParcelas;

public:
    Compra(string id, string idTarefa, string idLoja, string nomeProduto, int qtdeProduto, double precoUnitario, int numeroParcelas)
        : id(id), idTarefa(idTarefa), idLoja(idLoja), nomeProduto(nomeProduto), qtdeProduto(qtdeProduto),
          precoUnitario(precoUnitario), numeroParcelas(numeroParcelas) {}

    string getId() const { return id; }
    string getIdTarefa() const { return idTarefa; }
    string getIdLoja() const { return idLoja; }
    string getNomeProduto() const { return nomeProduto; }
    int getQtdeProduto() const { return qtdeProduto; }
    double getPrecoUnitario() const { return precoUnitario; }
    int getNumeroParcelas() const { return numeroParcelas; }

    void setId(const string& id) { this->id = id; }
    void setIdTarefa(const string& idTarefa) { this->idTarefa = idTarefa; }
    void setIdLoja(const string& idLoja) { this->idLoja = idLoja; }
    void setNomeProduto(const string& nomeProduto) { this->nomeProduto = nomeProduto; }
    void setQtdeProduto(int qtdeProduto) { this->qtdeProduto = qtdeProduto; }
    void setPrecoUnitario(double precoUnitario) { this->precoUnitario = precoUnitario; }
    void setNumeroParcelas(int numeroParcelas) { this->numeroParcelas = numeroParcelas; }
};

class Tarefa {
private:
    string idTarefa;
    string idLar;
    string idPrestador;
    string dataInicio;
    int prazoEntrega;
    double valorPrestador;
    int numParcelas;

public:
    Tarefa(string idTarefa, string idLar, string idPrestador, string dataInicio, int prazoEntrega, double valorPrestador, int numParcelas)
        : idTarefa(idTarefa), idLar(idLar), idPrestador(idPrestador), dataInicio(dataInicio),
          prazoEntrega(prazoEntrega), valorPrestador(valorPrestador), numParcelas(numParcelas) {}

    string getIdTarefa() const { return idTarefa; }
    string getIdLar() const { return idLar; }
    string getIdPrestador() const { return idPrestador; }
    string getDataInicio() const { return dataInicio; }
    int getPrazoEntrega() const { return prazoEntrega; }
    double getValorPrestador() const { return valorPrestador; }
    int getNumParcelas() const { return numParcelas; }

    void setIdTarefa(const string& idTarefa) { this->idTarefa = idTarefa; }
    void setIdLar(const string& idLar) { this->idLar = idLar; }
    void setIdPrestador(const string& idPrestador) { this->idPrestador = idPrestador; }
    void setDataInicio(const string& dataInicio) { this->dataInicio = dataInicio; }
    void setPrazoEntrega(int prazoEntrega) { this->prazoEntrega = prazoEntrega; }
    void setValorPrestador(double valorPrestador) { this->valorPrestador = valorPrestador; }
    void setNumParcelas(int numParcelas) { this->numParcelas = numParcelas; }
};

class Lar {
private:
    string idLar;
    string id1;
    string id2;
    string rua;
    int numero;
    string complemento;

public:
    Lar(string idLar, string id1, string id2, string rua, int numero, string complemento)
        : idLar(idLar), id1(id1), id2(id2), rua(rua), numero(numero), complemento(complemento) {}

    string getIdLar() const { return idLar; }
    string getId1() const { return id1; }
    string getId2() const { return id2; }
    string getRua() const { return rua; }
    int getNumero() const { return numero; }
    string getComplemento() const { return complemento; }

    void setIdLar(const string& idLar) { this->idLar = idLar; }
    void setId1(const string& id1) { this->id1 = id1; }
    void setId2(const string& id2) { this->id2 = id2; }
    void setRua(const string& rua) { this->rua = rua; }
    void setNumero(int numero) { this->numero = numero; }
    void setComplemento(const string& complemento) { this->complemento = complemento; }
};

class Festa {
private:
    string id;
    string idCasamento;
    string local;
    string data;
    string hora;
    double valorPago;
    int numParcelas;
    vector<string> convidados;

public:
    Festa(string id, string idCasamento, string local, string data, string hora, double valorPago, int numParcelas, vector<string> convidados)
        : id(id), idCasamento(idCasamento), local(local), data(data), hora(hora), valorPago(valorPago), numParcelas(numParcelas), convidados(convidados) {}

    string getId() const { return id; }
    string getIdCasamento() const { return idCasamento; }
    string getLocal() const { return local; }
    string getData() const { return data; }
    string getHora() const { return hora; }
    double getValorPago() const { return valorPago; }
    int getNumParcelas() const { return numParcelas; }
    vector<string> getConvidados() const { return convidados; }

    void setId(const string& id) { this->id = id; }
    void setIdCasamento(const string& idCasamento) { this->idCasamento = idCasamento; }
    void setLocal(const string& local) { this->local = local; }
    void setData(const string& data) { this->data = data; }
    void setHora(const string& hora) { this->hora = hora; }
    void setValorPago(double valorPago) { this->valorPago = valorPago; }
    void setNumParcelas(int numParcelas) { this->numParcelas = numParcelas; }
    void setConvidados(const vector<string>& convidados) { this->convidados = convidados; }
};

// VERIFY

int getTipoPrioridade(Pessoa* p) {
    if ( dynamic_cast<PessoaFisica*>((p))) {
        return 1; // Highest priority for PF
    } else if ( dynamic_cast<Loja*>((p))) {
        return 3; // Lowest priority for Loja
    } else {
        return 2; // Medium priority for PJ (not Loja)
    }
}

bool isLarOwnedByCouple(const Lar& lar, const string& id1, const string& id2) {
    return (lar.getId1() == id1 && lar.getId2() == id2) || (lar.getId1() == id2 && lar.getId2() == id1);
}

bool isCasamentoOfCouple(const Casamento& casamento, const string& id1, const string& id2) {
    return (casamento.getId1() == id1 && casamento.getId2() == id2) || (casamento.getId1() == id2 && casamento.getId2() == id1);
}

/// FIND BY ID
Lar* findLarById(const vector<Lar>& lares, const string& idLar) {
    for (const auto& lar : lares) {
        if (lar.getIdLar() == idLar) {
            return const_cast<Lar*>(&lar); // Remove const for demonstration purposes
        }
    }
    return nullptr;
}

Tarefa* findTarefaById(const vector<Tarefa>& tarefas, const string& idTarefa) {
    for (const auto& tarefa : tarefas) {
        if (tarefa.getIdTarefa() == idTarefa) {
            return const_cast<Tarefa*>(&tarefa); // Remove const for demonstration purposes
        }
    }
    return nullptr;
}

Casamento* findCasamentoById(const vector<Casamento>& casamentos, const string& idCasamento) {
    for (const auto& casamento : casamentos) {
        if (casamento.getIdCasamento() == idCasamento) {
            return const_cast<Casamento*>(&casamento); // Remove const for demonstration purposes
        }
    }
    return nullptr;
}

/// STRING FUNCTIONS

// Function to split a string by a delimiter
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    // Add the last token
    tokens.push_back(str.substr(start));

    return tokens;
}

string trim(const string& str) {
    // Find the first non-whitespace character
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    // If the string is all whitespace, return an empty string
    if (start == string::npos) {
        return "";
    }

    // Find the last non-whitespace character
    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    // Return the trimmed substring
    return str.substr(start, end - start + 1);
}

// PROCESS CSV

void processPessoasCSV(const string& filePath, vector<Pessoa*>& list_pessoa) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> columns = split(line, ';');

        string id = columns[0];
        string tipo = columns[1];
        string nome = columns[2];
        string telefone = columns[3];
        string endereco = columns[4];

        if (tipo == "F") {
            string cpf = columns[5];
            string data_nascimento = columns[6];
            double dinheiro_guardado = stod(columns[7]);
            double salario = stod(columns[8]);
            double gastos_mensais = stod(columns[9]);
            list_pessoa.push_back(new PessoaFisica(id, tipo, nome, telefone, endereco, cpf, data_nascimento, dinheiro_guardado, salario, gastos_mensais));
        } else {
            string cnpj = columns[5];
            if (tipo == "J"){

                list_pessoa.push_back(new PessoaJuridica(id, tipo, nome, telefone, endereco, cnpj));
            }else{
                list_pessoa.push_back(new Loja(id, tipo, nome, telefone, endereco, cnpj));
                
            }
        }
    }
}

void processFestasCSV(const string& filePath, vector<Festa>& list_festa) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> columns = split(line, ';');

        string id = columns[0];
        string idCasamento = columns[1];
        string local = columns[2];
        string data = columns[3];
        string hora = columns[4];
        double valorPago = stod(columns[5]);
        int numParcelas = stoi(columns[6]);
        vector<string> convidados = split(columns[7], ',');
        Festa festa(id, idCasamento, local, data, hora, valorPago, numParcelas, convidados);
        list_festa.push_back(festa);
    }
}

void processCasamentosCSV(const string& filePath, vector<Casamento>& list_casamento) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> columns = split(line, ';');

        string idCasamento = columns[0];
        string id1 = columns[1];
        string id2 = columns[2];
        string data = columns[3];
        string hora = columns[4];
        string local = columns[5];
        Casamento casamento(idCasamento, id1, id2, data, hora, local);
        list_casamento.push_back(casamento);
    }
}

void processLarCSV(const string& filePath, vector<Lar>& list_lar) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> columns = split(line, ';');

        string id_lar = columns[0];
        string id1 = columns[1];
        string id2 = columns[2];
        string rua = columns[3];
        int numero = stoi(columns[4]);
        string complemento = columns[5];
        Lar lar(id_lar, id1, id2, rua, numero, complemento);
        list_lar.push_back(lar);
    }
}

void processTarefaCSV(const string& filePath, vector<Tarefa>& list_tarefa) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> columns = split(line, ';');

        string idTarefa = columns[0];
        string idLar = columns[1];
        string idPrestador = columns[2];
        string dataInicio = columns[3];
        int prazoEntrega = stoi(columns[4]);
        double valorPrestador = stod(columns[5]);
        int numParcelas = stoi(columns[6]);
        Tarefa tarefa(idTarefa, idLar, idPrestador, dataInicio, prazoEntrega, valorPrestador, numParcelas);
        list_tarefa.push_back(tarefa);
    }
}

void processComprasCSV(const string& filePath, vector<Compra>& list_compra) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> columns = split(line, ';');

        list_compra.push_back(Compra(columns[0], columns[1], columns[2], columns[3], stoi(columns[4]), stod(columns[5]), stoi(columns[6])));
    }
}

// Function to process a single CSV file
void processCSVFile(const string& filePath, vector<Pessoa*>& list_pessoa, vector<Festa>& list_festa, vector<Casamento>& list_casamento, vector<Lar>& list_lar, vector<Tarefa>& list_tarefa, vector<Compra>& list_compra) {
    string nome_arquivo = filePath.substr(filePath.find_last_of("/\\") + 1);
    if (nome_arquivo == "pessoas.csv") {
        processPessoasCSV(filePath, list_pessoa);
    } else if (nome_arquivo == "festas.csv") {
        processFestasCSV(filePath, list_festa);
    } else if (nome_arquivo == "casamentos.csv") {
        processCasamentosCSV(filePath, list_casamento);
    } else if (nome_arquivo == "lares.csv") {
        processLarCSV(filePath, list_lar);
    } else if (nome_arquivo == "tarefas.csv") {
        processTarefaCSV(filePath, list_tarefa);
    } else if (nome_arquivo == "compras.csv") {
        processComprasCSV(filePath, list_compra);
    } else {
        cout << "Arquivo não suportado: " << nome_arquivo << endl;
    }
}


vector<string> getParesCpf() {
    vector<string> paresCpf;
    string cpfs;

    while (true) {
        getline(cin, cpfs);
        if (cpfs.empty()) {
            break;
        }
        paresCpf.push_back(cpfs);
    }

    return paresCpf;
}

PessoaFisica* findPessoaByCpf(vector<Pessoa*>& pessoas, const string& cpf) {
    for (auto& pessoa : pessoas) {
        // Verifica se a pessoa é do tipo PessoaFisica usando dynamic_cast
        PessoaFisica* pessoaFisica = dynamic_cast<PessoaFisica*>(pessoa);
        if (pessoaFisica && pessoaFisica->getCpf() == cpf) {
            return pessoaFisica;
        }
    }
    return nullptr;
}

void gerarRelatorioPrestadores(const vector<Pessoa*>& pessoas,
                               const vector<Tarefa>& tarefas,
                               const vector<Compra>& compras,
                               string pasta) {
    map<Pessoa*, double> pessoaValor;

    // Calculate total received for each person
    for (auto& p : pessoas) {
        double totalRecebido = 0.0;

        if (dynamic_cast<PessoaFisica*>(p)) {
            PessoaFisica* pf = dynamic_cast<PessoaFisica*>(p);


            //Sum values from tasks for PF
            for (const auto& t : tarefas) {
                if (t.getIdPrestador() == pf->getId()) {
                    totalRecebido += t.getValorPrestador();
                }
            }
            if (totalRecebido > 0) {
                pessoaValor[p] = totalRecebido;
            }
        } else if (dynamic_cast<PessoaJuridica*>(p) && !p->getisLoja()) {
            PessoaJuridica* pj = dynamic_cast<PessoaJuridica*>(p);

            // Sum values from tasks for PJ (not Loja)
            for (const auto& t : tarefas) {
                if (t.getIdPrestador() == pj->getId()) {
                    totalRecebido += t.getValorPrestador();
                }
            }
            pessoaValor[p] = totalRecebido;
        } else if (dynamic_cast<Loja*>(p)) {
            Loja* lj = dynamic_cast<Loja*>(p);

            // Sum values from purchases for Loja
            for (const auto& c : compras) {
                if (c.getIdLoja() == lj->getId()) {
                    totalRecebido += c.getPrecoUnitario() * c.getQtdeProduto();
                }
            }
            pessoaValor[p] = totalRecebido;
        }
    }

    // Create a sortable list
    vector<pair<Pessoa*, double>> listaOrdenada(pessoaValor.begin(), pessoaValor.end());

    // Sort the list
    sort(listaOrdenada.begin(), listaOrdenada.end(), [](const auto& a, const auto& b) {
        Pessoa* p1 = a.first;
        Pessoa* p2 = b.first;
        double v1 = a.second;
        double v2 = b.second;

        // Rule 1: Priority PF > PJ (not Loja) > Loja
        int tipoComparacao = getTipoPrioridade(p1) - getTipoPrioridade(p2);
        if (tipoComparacao != 0) return tipoComparacao < 0;

        // Rule 3: Sort by value (descending)
        if (v1 != v2) return v1 > v2;

        // Rule 4: Sort by name (ascending)
        return p1->getNome() < p2->getNome();
    });

    // Write the report to a file
    ofstream file(pasta + "/" + "2-estatisticas-prestadores.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo para escrita." << endl;
        return;
    }

    for (const auto& entry : listaOrdenada) {
        Pessoa* p = entry.first;
        double valor = entry.second;
        string tipo = (dynamic_cast<PessoaFisica*>(p)) ? "PF" : (dynamic_cast<Loja*>(p)) ? "Loja" : "PJ";
        file << tipo << ";"
             << p->getNome() << ";"
             << formatCurrencyBr(valor) << "\n";
    }

    cout << "Arquivo '2-estatisticas-prestadores.csv' gerado com sucesso!" << endl;
}

void acrescentarPlanejamentoCSV(PessoaFisica* p1, PessoaFisica* p2, 
                               const vector<YearMonth>& timeline, 
                               const vector<double>& saldos, string pasta) {
    ofstream file(pasta + "/" + "1-planejamento.csv", ios::app);
    
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo para escrita" << endl;
        return;
    }

    // Handle null or invalid cases
    if (!p1 || !p2) {
        file << "Casal não está cadastrado.\n";
        return;
    }

    if (timeline.empty() || saldos.empty()) {
        file << "Casal com CPFs " << p1->getCpf() << " e " << p2->getCpf()
             << " não possui gastos cadastrados.\n";
        return;
    }

    string nome1 = p1->getNome();
    string nome2 = p2->getNome();

    // Sort names alphabetically
    string nomeA, nomeB;
    if (nome1.compare(nome2) < 0) {
        nomeA = nome1;
        nomeB = nome2;
    } else {
        nomeA = nome2;
        nomeB = nome1;
    }

    // Build header line
    stringstream header;
    header << "Nome 1;Nome 2";
    for (const auto& ym : timeline) {
        header << ";" << formatYearMonth(ym);
    }
    file << header.str() << "\n";

    // Build balance line
    stringstream balances;
    balances << nomeA << ";" << nomeB;
    for (double s : saldos) {
        balances << ";" << formatCurrencyBr(s);
    }
    file << balances.str() << "\n";

}

void process_files(vector<Pessoa*>& pessoas, vector<Lar>& lares, vector<Tarefa>& tarefas,
                   vector<Casamento>& casamentos, vector<Festa>& festas, vector<Compra>& compras,
                   vector<string>& paresCpf, vector<Casal>& casais,
                   map<Casal, double>& gastos, map<Casal, int>& festasConvidados, string pasta) {

    

    for (const string& parCPF : paresCpf) {
        string cpf1, cpf2;

        vector<string> cpfs = split(parCPF, ',');
        cpf1 = trim(cpfs[0]);
        cpf2 = trim(cpfs[1]);

        if (cpf1.empty() || cpf2.empty()) {
            cout << "Entrada inválida: " << parCPF << cpf1 << ", " << cpf2 << endl;
            continue;
        }


        PessoaFisica* p1 = findPessoaByCpf(pessoas, cpf1);
        PessoaFisica* p2 = findPessoaByCpf(pessoas, cpf2);

        if (!p1 || !p2) {
            cout << "CPF não encontrado: " << cpf1 << " ou " << cpf2 << endl;
            continue;
        }

        string nome1 = p1->getNome();
        string nome2 = p2->getNome();

        string id1 = p1->getId();
        string id2 = p2->getId();

        vector<Expense> expenses;

        // TODO: acrescentar relatorio vazio quando falha verificacao

        for (const auto& tarefa : tarefas) {
            Lar* lar = findLarById(lares, tarefa.getIdLar());
            if (lar != nullptr && isLarOwnedByCouple(*lar, id1, id2)) {
                YearMonth start = parseDateToYearMonth(tarefa.getDataInicio());
                double installment = tarefa.getValorPrestador() / tarefa.getNumParcelas();
                expenses.emplace_back(start, installment, tarefa.getNumParcelas());
                // cout << "TAREFA: Data de início: " << start.getYear() << "-" << start.getMonth() << endl;
                // cout << "Valor da parcela: " << installment << endl;
                // cout << "Número de parcelas: " << tarefa.getNumParcelas() << endl;
            }
        }

        for (const auto& festa : festas) {
            Casamento* casamento = findCasamentoById(casamentos, festa.getIdCasamento());
            if (casamento != nullptr && isCasamentoOfCouple(*casamento, id1, id2)) {
                YearMonth start = parseDateToYearMonth(festa.getData());
                double installment = festa.getValorPago() / festa.getNumParcelas();
                expenses.emplace_back(start, installment, festa.getNumParcelas());
                // cout << "FESTA: Data de início: " << start.getYear() << "-" << start.getMonth() << endl;
                // cout << "Valor da parcela: " << installment << endl;
                // cout << "Número de parcelas: " << festa.getNumParcelas() << endl;
            }
        }

        for (const auto& compra : compras) {
            Tarefa* tarefa = findTarefaById(tarefas, compra.getIdTarefa());
            if (tarefa != nullptr) {
                Lar* lar = findLarById(lares, tarefa->getIdLar());
                if (lar != nullptr && isLarOwnedByCouple(*lar, id1, id2)) {
                    YearMonth start = parseDateToYearMonth(tarefa->getDataInicio());
                    double totalCompra = compra.getQtdeProduto() * compra.getPrecoUnitario();
                    double installment = totalCompra / compra.getNumeroParcelas();
                    expenses.emplace_back(start, installment, compra.getNumeroParcelas());
                    // cout << "COMPRA: Data de início: " << start.getYear() << "-" << start.getMonth() << endl;
                    // cout << "Valor da parcela: " << installment << endl;
                    // cout << "Número de parcelas: " << compra.getNumeroParcelas() << endl;
                }
            }
        }

        YearMonth minMonth(9999, 12);
        YearMonth maxMonth(0, 1);
        map<YearMonth, double> monthlyExpense;

        for (const auto& exp : expenses) {
            for (int i = 0; i < exp.numParcels; i++) {
                YearMonth due = exp.start.plusMonths(i);
                monthlyExpense[due] += exp.installment;

                if (due < minMonth) {
                    minMonth = due;
                }
                if (due.isAfter(maxMonth)) {
                    maxMonth = due;
                }
            }
        }

        // Create the timeline
        vector<YearMonth> timeline;
        YearMonth current = minMonth;
        while (!current.isAfter(maxMonth)) {
            timeline.push_back(current);
            current = current.plusMonths(1);
        }

        // Simulate the savings account
        double poupanca = p1->getDinheiroGuardado() + p2->getDinheiroGuardado();
        double gastosMensais = p1->getGastosMensais() + p2->getGastosMensais();
        vector<double> saldos;

        for (const auto& ym : timeline) {
            double combinedSalary = p1->getSalario() + p2->getSalario();

            // Add 13th salary in December
            if (ym.getMonth() == 12) {
                if (p1->getSalario() > 0)
                    combinedSalary += p1->getSalario();
                if (p2->getSalario() > 0)
                    combinedSalary += p2->getSalario();
            }

            double expense = monthlyExpense[ym];

            // Apply savings interest (0.5% per month)
            poupanca *= 1.005;

            // Update balance
            poupanca = poupanca + combinedSalary - expense - gastosMensais;

            // Round to 2 decimal places
            poupanca = round(poupanca * 100.0) / 100.0;

            // Add to balance history
            saldos.push_back(poupanca);
        }

        Casal casal(nome1, nome2);

        // Add the casal to the list
        casais.push_back(casal);

        // Calculate total expenses for the casal
        double totalGasto = accumulate(expenses.begin(), expenses.end(), 0.0,
                                    [](double sum, const Expense& e) {
                                        return sum + (e.installment * e.numParcels);
                                    });
        gastos[casal] = totalGasto;

        // Find their wedding (Casamento)
        string idCasamento;
        for (const auto& c : casamentos) {
            if ((c.getId1() == id1 && c.getId2() == id2) || (c.getId1() == id2 && c.getId2() == id1)) {
                idCasamento = c.getIdCasamento();
                break;
            }
        }

        // If no wedding is found, set a default value
        if (idCasamento.empty()) {
            idCasamento = "1"; // Default value
        }

        // Count shared parties (Festa)
        for (const auto& f : festas) {
            const vector<string>& convidados = f.getConvidados();
            bool containsNome1 = find(convidados.begin(), convidados.end(), nome1) != convidados.end();
            bool containsNome2 = find(convidados.begin(), convidados.end(), nome2) != convidados.end();

            if (containsNome1 && containsNome2 && f.getIdCasamento() != idCasamento) {
                festasConvidados[casal]++;
            }
        }


        acrescentarPlanejamentoCSV(p1, p2, timeline, saldos, pasta);





    }
}



void gerarEstatisticasCasaisCSV(const vector<Casal>& casais,
                                const map<Casal, double>& gastos,
                                const map<Casal, int>& festasEmComum,
                                string pasta) {
    ofstream file(pasta + "/" + "3-estatisticas-casais.csv");
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo para escrita." << endl;
        return;
    }

    // Create a sortable list of casais
    vector<Casal> listaOrdenada = casais;

    // Sort the list: first by total expenses (desc), then by nome1 (asc)
    sort(listaOrdenada.begin(), listaOrdenada.end(),
         [&gastos](const Casal& a, const Casal& b) {
             double gastoA = gastos.count(a) ? gastos.at(a) : 0.0;
             double gastoB = gastos.count(b) ? gastos.at(b) : 0.0;

             if (gastoA != gastoB) {
                 return gastoA > gastoB; // Descending order by total expenses
             } else {
                 return a.getNome1() < b.getNome1(); // Ascending order by nome1
             }
         });

    // Write the report to the file
    for (const auto& casal : listaOrdenada) {
        string nome1 = casal.getNome1();
        string nome2 = casal.getNome2();
        double totalGasto = gastos.count(casal) ? gastos.at(casal) : 0.0;
        int festas = festasEmComum.count(casal) ? festasEmComum.at(casal) : 0;

        file << nome1 << ";"
             << nome2 << ";"
             << formatCurrencyBr(totalGasto) << ";"
             << festas << "\n";
    }

    cout << "Arquivo '3-estatisticas-casais.csv' gerado com sucesso!" << endl;
}





int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <folder_path>" << endl;
        return 1;
    }

    string folderPath = argv[1];

    if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
        cerr << "Invalid folder path: " << folderPath << endl;
        return 1;
    }

    // Declare vectors in the main function
    vector<Pessoa*> list_pessoa;
    vector<Festa> list_festa;
    vector<Casamento> list_casamento;
    vector<Lar> list_lar;
    vector<Tarefa> list_tarefa;
    vector<Compra> list_compra;

    // Iterate through all files in the folder
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            cout << "Processing file: " << entry.path() << endl;
            processCSVFile(entry.path().string(), list_pessoa, list_festa, list_casamento, list_lar, list_tarefa, list_compra);
        }
    }

    // Now you can access the vectors in the main function
    // For example, you can print the size of each vector
    cout << "Number of pessoas: " << list_pessoa.size() << endl;
    cout << "Number of festas: " << list_festa.size() << endl;
    cout << "Number of casamentos: " << list_casamento.size() << endl;
    cout << "Number of lares: " << list_lar.size() << endl;
    cout << "Number of tarefas: " << list_tarefa.size() << endl;
    cout << "Number of compras: " << list_compra.size() << endl;


    cout<< "Pares de CPF"<<endl;

    vector<string> paresCpf;

    paresCpf = getParesCpf();

    vector<Casal> casais;
    map<Casal, double> gastos;

    map<Casal, int> festasConvidados;

    reiniciarArquivoPlanejamento(folderPath);

    process_files(list_pessoa, list_lar, list_tarefa, list_casamento, list_festa, list_compra, paresCpf, casais, gastos, festasConvidados, folderPath);

    gerarRelatorioPrestadores(list_pessoa, list_tarefa, list_compra, folderPath);

    gerarEstatisticasCasaisCSV(casais, gastos, festasConvidados, folderPath);


    return 0;
}