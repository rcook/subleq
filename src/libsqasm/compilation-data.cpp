// Copyright (C) 2014, Richard Cook.
// See licence at following location for details:
// https://github.com/rcook/subleq/blob/master/LICENSE

using namespace std;

#include "compilation-data.hpp"

CompilationData::CompilationData(
    unique_ptr<map<string, int>> symbolTable,
    unique_ptr<vector<unique_ptr<Cell>>> cells,
    unique_ptr<vector<char>> ops)
    :
    m_symbolTable(move(symbolTable)),
    m_cells(move(cells)),
    m_ops(move(ops))
{
}

CompilationData::~CompilationData()
{
}

const map<string, int> &CompilationData::symbolTable() const
{
    return *m_symbolTable;
}

const vector<unique_ptr<Cell>> &CompilationData::cells() const
{
    return *m_cells;
}

const vector<char> &CompilationData::ops() const
{
    return *m_ops;
}
