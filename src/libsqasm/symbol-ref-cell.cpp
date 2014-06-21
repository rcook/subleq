#include <string>
using namespace std;

#include "symbol-ref-cell.hpp"
#include "common/formatter.hpp"

SymbolRefCell::SymbolRefCell(int address, const string &symbol)
    : Cell(address),
    m_symbol(symbol)
{
}

SymbolRefCell::~SymbolRefCell()
{
}

string SymbolRefCell::toString() const
{
    return Formatter() << "[symbolRef " << m_symbol << "]";
}

int SymbolRefCell::emit(const map<string, int> &symbolTable) const
{
    auto iter(symbolTable.find(m_symbol));
    if (iter != symbolTable.end())
    {
        return iter->second;
    }

    throw runtime_error(Formatter() <<
        "No such symbol \"" << m_symbol << "\" at address " << address());
}
