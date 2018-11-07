//
//  Parser.h
//  323_SimpleLexorAssignment
//
//  Created by John on 11/6/18.
//  Copyright © 2018 UniQx. All rights reserved.
//
#pragma once

#ifndef Parser_h
#define Parser_h

#include "Lexor.h"
using namespace std;


void Rat18s();
void OptFunctionDefinitions();
void FunctionDefinitions();
void FunctionDefinitionsPrime();
void Function();
void OptParameterList();
void ParameterList();
void ParameterListPrime();
void Parameter();
void Qualifier();
void Body();
void OptDeclarationList();
void DeclarationList();
void DeclarationlistPrime();
void Declaration();

void IDs();
void IDsPrime();
void StatementList();
void StatementListPrime();
void Statement();
void Compound();
void Assign();
void If();
void IfPrime();
void Return();
void ReturnPrime();
void Print();
void Scan();

void While();
void Condition();
void Relop();
void Expression();
void ExpressionPrime();
void Term();
void TermPrime();
void Factor();
void Primary();
void Empty();

void Rat18s()
{
    cout << "<Rat18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;
    OptFunctionDefinitions();
    Lexeme lex = GetLex();
    PrintLexeme(lex.token,lex.lexeme);
}

void OptFunctionDefinitions()
{
    cout << "<OptFunctionDefinitions> -> <Function Definitions> | <Empty>" << endl;
}

void FunctionDefinitions()
{
    cout << "<Function Definitions> -> <Function> <Function Definitions Prime>" << endl;
}

void FunctionDefinitionsPrime()
{
    cout << "<Function Definitions Prime> -> <Function Definitions> | <Empty>" << endl;
}

void Function()
{
    cout << "<Function> -> function <Identifier> [ <Opt Parameter List> ] <Opt Declaration List> <Body>" << endl;
}

void OptParameterList()
{
    cout << "<Opt Parameter List> -> <Parameter List> | <Empty>" << endl;
}

void ParameterList()
{
    cout << "<Parameter List> -> <Parameter> , <Parameter List Prime>" << endl;
}

void ParameterListPrime()
{
    cout << "<Parameter List Prime> -> <Parameter List> | <Empty>" << endl;
}

void Parameter()
{
    cout << "<Parameter> -> <IDs> : <Qualifier>" << endl;
}

void Qualifier()
{
    cout << "<Qualifier> -> int | boolean | real" << endl;
}

void Body()
{
    cout << "<Body> -> { <Statement List> }" << endl;
}

void OptDeclarationList()
{
    cout << "<Opt Declaration List> -> <Declaration List> | <Empty>" << endl;
}

void DeclarationList()
{
    cout << "<Declaration List> -> <Declaration>; <Declaration List Prime>" << endl;
}

void DeclarationlistPrime()
{
    cout << "<Declaration List Prime> -> <Declaration List> | <Empty>" << endl;
}

void Declaration()
{
    cout << "<Declaration> -> <Qualifier> <IDs>" << endl;
}

void IDs()
{
    cout << "<IDs> -> <Identifier>, <IDs Prime>" << endl;
}

void IDsPrime()
{
    cout << "<IDs Prime> -> <IDs> | <Empty>" << endl;
}

void StatementList()
{
    cout << "<Statement List> -> <Statement> <Statement List Prime>" << endl;
}

void StatementListPrime()
{
    cout << "<Statement List Prime> -> <Statement List> | <Empty>" << endl;
}

void Statement()
{
    cout << "<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>"  << endl;
}

void Compound()
{
    cout << "<Compound> -> { <Statement List> }" << endl;
}

void Assign()
{
    cout << "<Assign> -> <Identifier> = <Expression> ;" << endl;
}

void If()
{
    cout << "<If> -> if (<Condition>) <Statement> <If Prime>" << endl;
}

void IfPrime()
{
    cout << "<If Prime> -> endif | else <Statement> endif" << endl;
}

void Return()
{
    cout << "<Return> -> return <Return Prime>;" << endl;
}

void ReturnPrime()
{
    cout << "<Return Prime> -> <Expression> | <Empty>" << endl;
}

void Print()
{
    cout << "<Print> -> put ( <Expression> );" << endl;
}

void Scan()
{
    cout << "<Scan> -> get ( <IDs> );" << endl;
}

void While()
{
    cout << "<While> -> while ( <Condition> ) <Statement>" << endl;
}

void Condition()
{
    cout << "<Condition> -> <Expression> <Relop> <Expression>" << endl;
}

void Relop()
{
    cout << "<Relop> -> == | ^= | > | < | => | =<" << endl;
}

void Expression()
{
    cout << "<Expression> -> <Term><Expression Prime>" << endl;
}

void ExpressionPrime()
{
    cout << "<Expression Prime> -> + <Term><Expression Prime> | - <Term><Expression Prime> | <Term>" << endl;
}

void Term()
{
    cout << "<Term> -> <Factor><Term Prime>" << endl;
}

void TermPrime()
{
    cout << "<Term Prime> -> * <Factor><Term Prime> | / <Factor><Term Prime> | <Factor>" << endl;
}

void Factor()
{
    cout << "<Factor> -> - <Primary> | <Primary>" << endl;
}

void Primary()
{
    cout << "<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) <Real> | true | false" << endl;
}

void Empty()
{
    cout << "<Empty>" << endl;
}


#endif /* Parser_h */
