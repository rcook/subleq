#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <vector>
using namespace std;

#include "assembler.hpp"
#include "common/util.hpp"
#include "cell.hpp"
#include "compilation-data.hpp"
#include "compilation-data-builder.hpp"
#include "symbol-ref-cell.hpp"
#include "value-cell.hpp"

static void parseToken(const string &token, int address, CompilationDataBuilder &builder)
{
    size_t colonIndex = token.find(':');
    string valueStr(colonIndex == string::npos ? token : token.substr(colonIndex + 1));


    if (colonIndex != string::npos)
    {
        string symbol(token.substr(0, colonIndex));
        string valueStr(token.substr(colonIndex + 1));
        builder.symbolTable().insert(make_pair(symbol, address));
    }

    int value;
    if (tryLexicalCast(valueStr, value))
    {
        builder.cells().push_back(unique_ptr<Cell>(new ValueCell(address, value)));
    }
    else
    {
        builder.cells().push_back(unique_ptr<Cell>(new SymbolRefCell(address, valueStr)));
    }
}

static void tokenizeCodeSection(ifstream &file, int &address, int &lineNumber, CompilationDataBuilder &builder)
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
        size_t tokenCount = 0;
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
            parseToken(tokens[0], address + 0, builder);
            parseToken(tokens[0], address + 1, builder);
            builder.cells().push_back(unique_ptr<Cell>(new ValueCell(address + 2, address + 3)));
            break;
        case 2:
            parseToken(tokens[0], address + 0, builder);
            parseToken(tokens[1], address + 1, builder);
            builder.cells().push_back(unique_ptr<Cell>(new ValueCell(address + 2, address + 3)));
            break;
        case 3:
            parseToken(tokens[0], address + 0, builder);
            parseToken(tokens[1], address + 1, builder);
            parseToken(tokens[2], address + 2, builder);
            break;
        default:
            throw runtime_error("Unexpected");
        }

        address += 3;
    }
}

static void tokenizeDataSection(ifstream &file, int &address, int &lineNumber, CompilationDataBuilder &builder)
{
    string line;
    for (; getline(file, line); ++lineNumber)
    {
        istringstream stream(line);
        string token;
        while (stream >> token)
        {
            parseToken(token, address, builder);
            ++address;
        }
    }
}

static void emit(CompilationDataBuilder &builder)
{
    builder.ops().reserve(builder.cells().size());
    for (auto &cell : builder.cells())
    {
        int op = cell->emit(builder.symbolTable());
        builder.ops().push_back(castInt<char>(op));
    }
}

unique_ptr<CompilationData> Assembler::assemble(const string &fileName)
{
    CompilationDataBuilder builder;

    ifstream file(fileName);
    int address = 0;
    int lineNumber = 0;

    tokenizeCodeSection(file, address, lineNumber, builder);
    tokenizeDataSection(file, address, lineNumber, builder);

    emit(builder);

    return builder.create();
}
