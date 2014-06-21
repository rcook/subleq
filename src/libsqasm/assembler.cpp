#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
using namespace std;

#include "assembler.hpp"
#include "common/util.hpp"

class Cell
{
    Cell() = delete;
    Cell(const Cell &) = delete;
    Cell &operator=(const Cell &) = delete;
    Cell(Cell &&) = delete;
    Cell &operator=(Cell &&) = delete;

public:
    virtual ~Cell() { }
    virtual string toString() const = 0;
    virtual int emit(const map<string, int> &symbolTable) const = 0;
    int address() const { return m_address; }

protected:
    Cell(int address) : m_address(address) { }

private:
    const int m_address;
};

class SymbolRefCell : public Cell
{
    SymbolRefCell();
    SymbolRefCell(const SymbolRefCell &) = delete;
    SymbolRefCell &operator=(const SymbolRefCell &) = delete;
    SymbolRefCell(SymbolRefCell &&) = delete;
    SymbolRefCell &operator=(SymbolRefCell &&) = delete;

public:
    SymbolRefCell(int address, const string &symbol) : Cell(address), m_symbol(symbol) { }

    ~SymbolRefCell() { }

    string toString() const { return Formatter() << "[symbolRef " << m_symbol << "]"; }

    int emit(const map<string, int> &symbolTable) const
    {
        auto iter(symbolTable.find(m_symbol));
        if (iter != symbolTable.end())
        {
            return iter->second;
        }

        throw runtime_error(Formatter() <<
            "No such symbol \"" << m_symbol << "\" at address " << address());
    }

private:
    const string m_symbol;
};

class ValueCell : public Cell
{
    ValueCell();
    ValueCell(const ValueCell &) = delete;
    ValueCell &operator=(const ValueCell &) = delete;
    ValueCell(ValueCell &&) = delete;
    ValueCell &operator=(ValueCell &&) = delete;

public:
    ValueCell(int address, int value) : Cell(address), m_value(value) { }

    ~ValueCell() { }

    string toString() const { return Formatter() << "[value " << m_value << "]"; }

    int emit(const map<string, int> &symbolTable) const
    {
        return m_value;
    }

private:
    const int m_value;
};

static void parseToken(const string &token, int address, map<string, int> &symbolTable, vector<unique_ptr<Cell>> &cells)
{
    size_t colonIndex = token.find(':');
    string valueStr(colonIndex == string::npos ? token : token.substr(colonIndex + 1));


    if (colonIndex != string::npos)
    {
        string symbol(token.substr(0, colonIndex));
        string valueStr(token.substr(colonIndex + 1));
        symbolTable.insert(make_pair(symbol, address));
    }

    int value;
    if (tryLexicalCast(valueStr, value))
    {
        cells.push_back(unique_ptr<Cell>(new ValueCell(address, value)));
    }
    else
    {
        cells.push_back(unique_ptr<Cell>(new SymbolRefCell(address, valueStr)));
    }
}

static void tokenizeCodeSection(ifstream &file, int &address, int &lineNumber, map<string, int> &symbolTable, vector<unique_ptr<Cell>> &cells)
{
    string line;
    for (; getline(file, line); ++lineNumber)
    {
        if (line.compare(".data") == 0)
        {
            ++lineNumber;
            return;
        }

        string tokens[3];
        istringstream stream(line);
        string token;
        int tokenCount = 0;
        while (stream >> token)
        {
            if (tokenCount >= extent<decltype(tokens)>::value)
            {
                throw runtime_error(Formatter() << "Too many tokens at line " << (lineNumber + 1));
            }
            tokens[tokenCount++] = token;
        }

        if (tokenCount == 0)
        {
            continue;
        }

        switch (tokenCount)
        {
        case 1:
            parseToken(tokens[0], address + 0, symbolTable, cells);
            parseToken(tokens[0], address + 1, symbolTable, cells);
            cells.push_back(unique_ptr<Cell>(new ValueCell(address + 2, address + 3)));
            break;
        case 2:
            parseToken(tokens[0], address + 0, symbolTable, cells);
            parseToken(tokens[1], address + 1, symbolTable, cells);
            cells.push_back(unique_ptr<Cell>(new ValueCell(address + 2, address + 3)));
            break;
        case 3:
            parseToken(tokens[0], address + 0, symbolTable, cells);
            parseToken(tokens[1], address + 1, symbolTable, cells);
            parseToken(tokens[2], address + 2, symbolTable, cells);
            break;
        default:
            throw runtime_error("Unexpected");
        }

        address += 3;
    }
}

static void tokenizeDataSection(ifstream &file, int &address, int &lineNumber, map<string, int> &symbolTable, vector<unique_ptr<Cell>> &cells)
{
    string line;
    for (; getline(file, line); ++lineNumber)
    {
        istringstream stream(line);
        string token;
        while (stream >> token)
        {
            parseToken(token, address, symbolTable, cells);
            ++address;
        }
    }
}

static void emit(const map<string, int> &symbolTable, const vector<unique_ptr<Cell>> &cells, vector<char> &ops)
{
    ops.reserve(cells.size());
    for (auto &cell : cells)
    {
        int op = cell->emit(symbolTable);
        ops.push_back(castInt<char>(op));
    }
}

void Assembler::assemble(const string &fileName, vector<char> &ops, const bool dumpSymbols)
{
    map<string, int> symbolTable;
    vector<unique_ptr<Cell>> cells;

    ifstream file(fileName);
    int address = 0;
    int lineNumber = 0;

    tokenizeCodeSection(file, address, lineNumber, symbolTable, cells);
    tokenizeDataSection(file, address, lineNumber, symbolTable, cells);

    if (dumpSymbols)
    {
        if (!symbolTable.empty())
        {
            cout << "Symbol table" << endl;
            for (auto &pair : symbolTable)
            {
                cout << "  symbol: " << pair.first << " = " << pair.second << endl;
            }
        }

        if (!cells.empty())
        {
            cout << "Cells" << endl;
            for (auto &cell : cells)
            {
                cout << "  cell: " << cell->toString() << endl;
            }
        }
    }

    emit(symbolTable, cells, ops);
}
