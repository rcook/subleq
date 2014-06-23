#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "compilation-data-builder.hpp"

class Assembler
{
    Assembler() = delete;
    Assembler(const Assembler &) = delete;
    Assembler &operator=(const Assembler &) = delete;
    Assembler(Assembler &&) = delete;
    Assembler &operator=(Assembler &&) = delete;

private:
    Assembler(const std::string &fileName);
    ~Assembler();

public:
    static std::unique_ptr<CompilationData> assemble(const std::string &fileName);

private:
    std::unique_ptr<CompilationData> assemble();
    void tokenizeCodeSection();
    void tokenizeDataSection();
    void emit();
    void parseToken(const std::string &token, int address);

private:
    std::ifstream m_file;
    int m_address;
    int m_lineNumber;
    CompilationDataBuilder m_builder;
};
