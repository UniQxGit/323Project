//
//  Parser.h
//  323_SimpleLexorAssignment
//
//  Created by John on 11/6/18.
//  Copyright © 2018 UniQx. All rights reserved.
//
#pragma once

#ifndef CodeGenerator_h
#define CodeGenerator_h

#include <vector>


void PrintAllSymbols();

struct Symbol
{
	Lexeme lex;
	int address;
	Symbol(){};
	Symbol(Lexeme l, int addr){
		lex = l;
		address = addr;
	}
};


struct Instruction
{
    std::string name;
    int address;
    int index;
    Instruction(){}
    Instruction(std::string name,int address,int index){
        this->name = name;
        this->address = address;
        this->index = index;
    }
};

std::map<std::string,Symbol> symbols; //Lexeme,
std::vector<Instruction*> instructions;
//std::map<std::string,int> instructions;
const int MLOCBASE = 2000;
const int NULLADDR = -999;
int addressCounter = 0;
int instructionCounter = 0;

Instruction* GenerateInstruction(std::string instruction, int address)
{
	Instruction *inst = new Instruction(instruction,address,instructionCounter);
    instructions.push_back(inst);
    instructionCounter++;
    return inst;
}

void InsertSymbol(Lexeme lex)
{
	if(symbols.count(lex.lexeme) > 0)
	{
		std::cout << "ERROR: Duplicate Symbol for " << lex.lexeme << "!" << std::endl;
		return;
	}
	std::cout << addressCounter << std::endl;
	Symbol sym = Symbol(lex,MLOCBASE + addressCounter);
	symbols[lex.lexeme] = sym;
	addressCounter++;
	std::cout << "Added: (" << lex.lexeme << "," << LexToStr(lex.token) << ")" << std::endl;
	PrintAllSymbols();
}

int GetAddress(Lexeme lex)
{
	if(symbols.count(lex.lexeme) == 0)
	{
		std::cout << "ERROR: symbol " << lex.lexeme << " Does not exist!" << std::endl; 
		return -1;
	}
	return symbols[lex.lexeme].address;
}

void PrintAllInstructions()
{
	ofile << std::left << std::setw(15) << "__Index__" << std::left << std::setw(15) << "__Operation__" << "__Operand__" << std::endl;
	
	int i = 0;
    for(Instruction* inst : instructions)
    {
		//x.First = key x.second = value
		ofile << std::left << std::setw(15) << inst->index << std::left << std::setw(15) << inst->name << (inst->address!=NULLADDR?std::to_string(inst->address):"") << std::endl;
	    i++;
	}
	ofile << std::endl;
}

void PrintAllSymbols()
{
	ofile << std::left << std::setw(15) << "__Symbol__" << std::left << std::setw(15) << "__Type__" << "__Memory Address__" << std::endl;
	int i = 0;
	for (auto const& x : symbols)
	{
		if (x.second.lex.lexeme == "")
			continue;
		std::cout << "Symbols: (|" << x.second.lex.lexeme << "|,|" << LexToStr(x.second.lex.token) << "|)" << std::endl;
		//x.First = key x.second = value
        ofile << std::left << std::setw(15) << x.first << std::left << std::setw(15) << LexToStr(x.second.lex.token) << x.second.address << std::endl;
	    i++;
	}
	ofile << std::endl;
}

#endif
