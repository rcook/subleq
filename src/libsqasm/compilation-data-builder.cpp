using namespace std;

#include "compilation-data-builder.hpp"

CompilationDataBuilder::CompilationDataBuilder()
    :
    m_symbolTable(new map<string, int>),
    m_cells(new vector<unique_ptr<Cell>>),
    m_ops(new vector<char>)
{
}

CompilationDataBuilder::~CompilationDataBuilder()
{
}

map<string, int> &CompilationDataBuilder::symbolTable()
{
    return *m_symbolTable;
}

vector<unique_ptr<Cell>> &CompilationDataBuilder::cells()
{
    return *m_cells;
}

vector<char> &CompilationDataBuilder::ops()
{
    return *m_ops;
}

unique_ptr<CompilationData> CompilationDataBuilder::create()
{
    return unique_ptr<CompilationData>(new CompilationData(
        move(m_symbolTable),
        move(m_cells),
        move(m_ops)));
}
