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


std::map<std::string,Symbol> symbols; //Lexeme,
std::map<std::string,int> instructions;
const int MLOCBASE = 2000;
const int NULLADDR = -999;
int addressCounter = 0;

void GenerateInstruction(std::string instruction, int address)
{
	instructions[instruction] = address;
}

void InsertSymbol(Lexeme lex)
{
	if(symbols.count(lex.lexeme) > 0)
	{
		std::cout << "ERROR: Duplicate Symbol!" << std::endl;
		return;
	}
	Symbol sym = Symbol(lex,MLOCBASE + addressCounter);
	symbols[lex.lexeme] = sym;
	addressCounter++;
}

int GetAddress(Lexeme lex)
{
	return symbols[lex.lexeme].address;
}

void PrintAllInstructions()
{
	ofile << std::left << std::setw(15) << "__Index__" << std::left << std::setw(15) << "__Operation__" << "__Operand__" << std::endl;
	
	int i = 0;
	for (auto const& x : instructions)
	{
		//x.First = key x.second = value
		ofile << std::left << std::setw(15) << i << std::left << std::setw(15) << x.first << (x.second!=NULLADDR?std::to_string(x.second):"") << std::endl;
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
		//x.First = key x.second = value
        ofile << std::left << std::setw(15) << x.first << std::left << std::setw(15) << LexToStr(x.second.lex.token) << x.second.address << std::endl;
	    i++;
	}
	ofile << std::endl;
}

#endif
