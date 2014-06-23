#include <sstream>
using namespace std;

#include "assembler.hpp"
#include "common/util.hpp"
#include "symbol-ref-cell.hpp"
#include "value-cell.hpp"

Assembler::Assembler(const string &fileName)
    :
    m_file(fileName),
    m_address(0),
    m_lineNumber(0)
{
}

Assembler::~Assembler()
{
}

unique_ptr<CompilationData> Assembler::assemble(const string &fileName)
{
    Assembler assembler(fileName);
    return assembler.assemble();
}

unique_ptr<CompilationData> Assembler::assemble()
{
    tokenizeCodeSection();
    tokenizeDataSection();
    emit();
    return m_builder.create();
}

void Assembler::tokenizeCodeSection()
{
    string line;
    for (; getline(m_file, line); ++m_lineNumber)
    {
        if (line.compare(".data") == 0)
        {
            ++m_lineNumber;
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
                throw runtime_error(Formatter() << "Too many tokens at line " << (m_lineNumber + 1));
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
            parseToken(tokens[0], m_address + 0);
            parseToken(tokens[0], m_address + 1);
            m_builder.cells().push_back(unique_ptr<Cell>(new ValueCell(
                m_address + 2,
                m_address + 3)));
            break;
        case 2:
            parseToken(tokens[0], m_address + 0);
            parseToken(tokens[1], m_address + 1);
            m_builder.cells().push_back(unique_ptr<Cell>(new ValueCell(
                m_address + 2,
                m_address + 3)));
            break;
        case 3:
            parseToken(tokens[0], m_address + 0);
            parseToken(tokens[1], m_address + 1);
            parseToken(tokens[2], m_address + 2);
            break;
        default:
            throw runtime_error("Unexpected");
        }

        m_address += 3;
    }
}

void Assembler::tokenizeDataSection()
{
    string line;
    for (; getline(m_file, line); ++m_lineNumber)
    {
        istringstream stream(line);
        string token;
        while (stream >> token)
        {
            parseToken(token, m_address);
            ++m_address;
        }
    }
}

void Assembler::emit()
{
    m_builder.ops().reserve(m_builder.cells().size());
    for (auto &cell : m_builder.cells())
    {
        int op = cell->emit(m_builder.symbolTable());
        m_builder.ops().push_back(castInt<char>(op));
    }
}

void Assembler::parseToken(const string &token, int address)
{
    size_t colonIndex = token.find(':');
    string valueStr(colonIndex == string::npos ? token : token.substr(colonIndex + 1));

    if (colonIndex != string::npos)
    {
        string symbol(token.substr(0, colonIndex));
        string valueStr(token.substr(colonIndex + 1));
        m_builder.symbolTable().insert(make_pair(symbol, address));
    }

    int value;
    if (tryLexicalCast(valueStr, value))
    {
        m_builder.cells().push_back(unique_ptr<Cell>(new ValueCell(address, value)));
    }
    else
    {
        m_builder.cells().push_back(unique_ptr<Cell>(new SymbolRefCell(address, valueStr)));
    }
}
