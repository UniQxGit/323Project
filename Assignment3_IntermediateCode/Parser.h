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
#include "CodeGenerator.h"
using namespace std;

bool VContains(vector<std::string> v, std::string s);
bool Follow();
bool First(std::string s);
void PrintCurrentLex();


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

Lexeme currentLex;

void Lexor(){
    currentLex = GetLex();
}

bool Lexor(std::string expected){
    currentLex = GetidLex();
    
    std::string check;
    if (currentLex.token == Identifier)
        check = "_identifier";
    else if (currentLex.token == Integer)
        check = "_integer";
    else if (currentLex.token == Real)
        check = "_real";
    else
        check = currentLex.lexeme;
    if (check != expected)
    {
        ofile << "ERROR: expected " << expected << endl;
        return false;
    }
    return true;
}

void PrintCurrentLex()
{
    ofile << std::left << std::setw(15) << LexToStr(currentLex.token) << "|" << currentLex.lexeme << "|" << std::endl;
}

void PrintCurrentToken()
{
    PrintLexeme(currentLex.token,currentLex.lexeme);
}

void Rat18s()
{
    Lexor();
    ofile << "\t<Rat18S> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List>" << endl;
    
    OptFunctionDefinitions();
    if (currentLex.lexeme == "%%")
    {
        Lexor();
        OptDeclarationList();
        StatementList();
    }else{
        ofile << "<Rat18s> Error: expecting %%" << endl;
    }
    
    ofile << "\t<Finish>" << endl;
}

void OptFunctionDefinitions()
{
    if (currentLex.lexeme == "function")
    {
        ofile << "\t<OptFunctionDefinitions> -> <Function Definitions>" << endl;
        FunctionDefinitions();
    }
    else if (currentLex.lexeme == "%%")
    {
        ofile << "\t<OptFunctionDefinitions> -> ";
        Empty();
    }
    else{
        ofile << "<OptFunctionDefinitions> ERROR: Expecting function or $$" << endl;
    }
}

void FunctionDefinitions()
{
    ofile << "\t<Function Definitions> -> <Function> <Function Definitions Prime>" << endl;
    if (currentLex.lexeme == "function")
    {
        Function();
        FunctionDefinitionsPrime();
    }
}

void FunctionDefinitionsPrime()
{
    
    if (currentLex.lexeme == "function")
    {
        ofile << "\t<Function Definitions Prime> -> <Function Definitions>" << endl;
        FunctionDefinitions();
    }
    else
    {
        ofile << "\t<Function Definitions Prime> ->";
        Empty();
    }
    
}

void Function()
{
    ofile << "\t<Function> -> function <Identifier> [ <Opt Parameter List> ] <Opt Declaration List> <Body>" << endl;
    if (!Lexor("_identifier")){return;}
    if (!Lexor("[")){return;}
    
    Lexor(); //Toss the [
    OptParameterList();
    
    if (currentLex.lexeme != "]")
    {
        ofile << "<Function> ERROR: expecting ]" << endl;
        return;
    }
    Lexor(); //Toss the ']'
    
    OptDeclarationList();
    Body();
}

void OptParameterList()
{
    
    if (currentLex.token == Identifier)
    {
        ofile << "\t<OptParameterList> -> <ParameterList>" << endl;
        ParameterList();
    }else if (currentLex.lexeme == "]")
    {
        ofile << "\t<OptParameterList> -> " << endl;
        Empty();
    }else{
        ofile << "<OptParameterList> ERROR: Expecting Identifier" << endl;
    }
}

void ParameterList()
{
    ofile << "\t<Parameter List> -> <Parameter> , <Parameter List Prime>" << endl;
    if (currentLex.token == Identifier)
    {
        Parameter();
        if (currentLex.lexeme == ","){
            Lexor();//toss the ','
            ParameterListPrime();
        }
    }
}

void ParameterListPrime()
{
    if (currentLex.token == Identifier)
    {
        ofile << "\t<Parameter List Prime> -> <Parameter List>" << endl;
        ParameterList();
    }else{
        ofile << "\t<Parameter List Prime> -> " ;
        Empty();
    }
}

void Parameter()
{
    ofile << "\t<Parameter> -> <IDs> : <Qualifier>" << endl;
    IDs();
    if (currentLex.lexeme == ":")
    {
        Lexor(); // Toss the ':'
        Qualifier();
    }else{
        ofile << "<Parameter> ERROR: Expecting :" << endl;
    }
    
}

void Qualifier()
{
    if (currentLex.lexeme == "int") {
        ofile << "\t<Qualifier> -> Int" << endl;
    }
    
    else if (currentLex.lexeme == "boolean") {
        ofile << "\t<Qualifier> -> Boolean" << endl;
    }
    
    else if (currentLex.lexeme == "real") {
        ofile << "\t<Qualifier> -> Real" << endl;
    }
    else if (currentLex.lexeme == "true") {
        ofile << "\t<Qualifier> ->    Boolean " << endl;
    }
    
    else if (currentLex.lexeme == "false") {
        ofile << "\t<Qualifier> -> Boolean " << endl;
    }
    else {
        ofile << "<Qualifier> ERROR: expected and int, boolean, or real" << endl;
    }
    Lexor();
}

void Body()
{
    ofile << "\t<Body> -> { <Statement List> }" << endl;
    if (currentLex.lexeme != "{"){
        ofile << "<Body> ERROR: Expected {" << endl;
        return;
    }
    
    Lexor(); //Toss the '{'
    StatementList();
    
    if (currentLex.lexeme != "}"){
        ofile << "<Body> ERROR: Expected }" << endl;
        return;
    }
    Lexor(); //Toss the '}'
}

void OptDeclarationList()
{
    if (currentLex.lexeme == "{"){
        ofile << "\t<Opt Declaration List> -> ";
        Empty();
    }else if (currentLex.lexeme == "int" || currentLex.lexeme == "boolean" || currentLex.lexeme == "real")
    {
        ofile << "\t<Opt Declaration List> -> <Declaration List>" << endl;
        DeclarationList();
    }else {
        ofile << "<OptDeclarationList> ERROR: Expected int, boolean, or real" << endl;
    }
}

void DeclarationList()
{
    ofile << "\t<Declaration List> -> <Declaration>; <Declaration List Prime>" << endl;
    Declaration();
    
    if (currentLex.lexeme == ";"){
        Lexor();
        DeclarationlistPrime();
    }else{
        ofile << "<DeclarationList> ERROR: expecting ;" << endl;
    }
}

void DeclarationlistPrime()
{
    if (currentLex.lexeme == "int" || currentLex.lexeme == "boolean" || currentLex.lexeme == "real"){
        ofile << "\t<Declaration List Prime> -> <Declaration List>" << endl;
        DeclarationList();
    }else{
        ofile << "\t<Declaration List Prime> -> ";
        Empty();
    }
}

void Declaration()
{
    ofile << "\t<Declaration> -> <Qualifier> <IDs>" << endl;
    Qualifier();
    IDs();
}

void IDs()
{
    ofile << "\t<IDs> -> <Identifier>, <IDs Prime>" << endl;
    if (currentLex.token == Identifier)
    {
        InsertSymbol(currentLex);
        if (symbols.count(currentLex.lexeme) > 0)
        {
            GenerateInstruction("STDIN",NULLADDR);
            GenerateInstruction("POPM",GetAddress(currentLex));
        }
        
        Lexor();
        if (currentLex.lexeme == ","){
            Lexor();
            IDsPrime();
        }
    }
    
}

void IDsPrime()
{
    if (currentLex.token == Identifier)
    {
        ofile << "\t<IDs Prime> -> <IDs>" << endl;
        IDs();
        
    }else{
        
        ofile << "\t<IDs Prime> -> ";
        Empty();
    }
    //ofile << "<IDs Prime> -> <IDs> | <Empty>" << endl; original line
}

void StatementList()
{
    ofile << "\t<Statement List> -> <Statement> <Statement List Prime>" << endl;
    Statement();
    StatementListPrime();
}

void StatementListPrime()
{
    //ofile << "<Statement List Prime> -> <Statement List> | <Empty>" << endl;
    // Statement List Prime
    if (currentLex.lexeme == "if" || currentLex.lexeme == "{" || currentLex.token == Identifier || currentLex.lexeme == "put" || currentLex.lexeme == "get" || currentLex.lexeme == "while" || currentLex.lexeme == "return") {
        ofile << "\t<Statement List Prime> -> <Statement>" << endl;
        Statement();
        StatementListPrime();
    }
    else {
        ofile << "\t<Statement List Prime> -> ";
        Empty();
    }
}

void Statement()
{
    if (currentLex.lexeme == "{"){
        ofile << "\t<Statement -> <Compound>" << endl;
        Compound();
    }
    else if (currentLex.token == Identifier){
        ofile << "\t<Statement -> <Assign>" << endl;
        Assign();
    }
    else if (currentLex.lexeme == "if"){
        ofile << "\t<Statement> -> <If>" << endl;
        If();
    }
    else if (currentLex.lexeme == "return"){
        ofile << "\t<Statement> -> <Return>" << endl;
        Return();
    }
    else if (currentLex.lexeme == "put"){
        ofile << "\t<Statement -> <Print>" << endl;
        Print();
    }
    else if (currentLex.lexeme == "get"){
        ofile << "\t<Statement> -> <Scan>" << endl;
        Scan();
    }
    else if (currentLex.lexeme == "while"){
        ofile << "\t<Statement> -> <While>" << endl;
        While();
    }
    //ofile << "<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>"  << endl;
}

void Compound()
{
    ofile << "\t<Compound> -> { <Statement List> }" << endl;
    Lexor();
    StatementList();
    if (currentLex.lexeme == "}"){
        Lexor();
    }
    else {
        ofile << "<Compound> ERROR: expected a '}'" << endl;
    }
}

void Assign()
{
    ofile << "\t<Assign> -> <Identifier> = <Expression> ;" << endl;
    Lexor();
    if (currentLex.lexeme == "="){
        Lexor();
        Expression();
        if (currentLex.lexeme == ";"){
            Lexor();
        }
        else {
            ofile << "ERROR: Missing a ';'" << endl;
        }
    }
    else {
        ofile << "ERROR: Expected a '='" << endl;
    }
}

void If()
{
    //ofile << "<If> -> if (<Condition>) <Statement> <If Prime>" << endl;
    if (currentLex.lexeme == "if"){
        Lexor();
        if (currentLex.lexeme == "("){
            Lexor();
            if (currentLex.lexeme == ")"){
                Lexor();
                Statement();
                IfPrime();
            }
            else {
                ofile << "ERROR: expected a ')'" << endl;
            }
        }
        else {
            ofile << "ERROR: expected a '('" << endl;
        }
    }
    else {
        ofile << "ERROR: expected 'if'" << endl;
    }
}

void IfPrime()
{
    //ofile << "<If Prime> -> endif | else <Statement> endif" << endl;
    if (currentLex.lexeme == "endif"){
        ofile << "\t<If Prime> -> endif" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "else"){
        ofile << "\t<If Prime> -> else <Statement> <IfPrime>" << endl;
        Lexor();
        Statement();
        IfPrime();
    }
}

void Return()
{
    //ofile << "<Return> -> return <Return Prime>;" << endl; original
    //ofile << "<Return> -> return <Return Prime>" << endl //new
    if (currentLex.lexeme == "return"){
        ofile << "\t<Return> -> return <Return Prime>" << endl;
        Lexor();
        ReturnPrime();
    }
    else {
        ofile << "Expected 'return'" << endl;
    }
}

void ReturnPrime()
{
    //ofile << "<Return Prime> -> <Expression> | <Empty>" << endl;
    //ofile << "<Return Prime> -> <Expression>; | ;" //new change
    if (currentLex.lexeme == ";"){
        ofile << "\t<Return Prime> -> ;" << endl;
        Lexor();
    }
    else {
        ofile << "\t<Return Prime> -> <Expression>" << endl;
        Expression();
        ReturnPrime();
    }
}

void Print()
{
    if (currentLex.lexeme == "put"){
        ofile << "\t<Print> -> put ( <Expression> );" << endl;
        Lexor();
        if (currentLex.lexeme == "("){
            Lexor();
            Expression();
            if (currentLex.lexeme == ")"){
                Lexor();
                if (currentLex.lexeme == ";"){
                    Lexor();
                }
                else {
                    ofile << "ERROR: expected a ';'" << endl;
                }
            }
        }
        else {
            ofile << "ERROR: expected '('" << endl;
        }
    }
    else{
        ofile << "ERROR: expected 'put'" << endl;
    }
}

void Scan()
{
    if (currentLex.lexeme == "get"){
        ofile << "\t<Scan> -> get ( <IDs> );" << endl;
        Lexor();
        if (currentLex.lexeme == "("){
            Lexor();
            IDs();
            if (currentLex.lexeme == ")"){
                Lexor();
                if (currentLex.lexeme == ";"){
                    Lexor();
                }
                else {
                    ofile << "ERROR: Expected a ';'" << endl;
                }
            }
            else {
                ofile << "ERROR: Expected a ')'" << endl;
            }
        }
        else {
            ofile << "ERROR: Expected a '('" << endl;
        }
    }
    else {
        ofile << "ERROR: Expected 'get'" << endl;
    }
}

void While()
{
    //ofile << "<While> -> while ( <Condition> ) <Statement>" << endl;
    if (currentLex.lexeme == "while")
    {
        ofile << "\t<While> -> while ( <Condition>  )  <Statement>" << endl;
        //outfile << "<While> -> while ( <Condition>  )  <Statement>" << endl;
        Lexor();
        if (currentLex.lexeme == "(")
        {
            Lexor();
            Condition();
            if (currentLex.lexeme == ")")
            {
                Lexor();
                Statement();
            }
            else
            {
                ofile << "ERROR: ')' was expected." << endl;
                //outfile << "ERROR: ')' was expected." << endl;
            }
        }
        else
        {
            ofile << "ERROR: '(' was expected." << endl;
            //outfile << "ERROR: '(' was expected." << endl;
        }
    }
    else
    {
        ofile << "ERROR: while was expected." << endl;
        //outfile << "ERROR: while was expected." << endl;
    }
}

void Condition()
{
    ofile << "\t<Condition> -> <Expression> <Relop> <Expression>" << endl;
    Expression();
    Relop();
    Expression();
    
}

void Relop()
{
    ofile << "\t<Relop> -> == | ^= | > | < | => | =<" << endl;
    if (currentLex.lexeme == "==") {
        ofile << "\t<Relop> -> ==" << endl;
        //outfile << "<Relop> -> ==" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "^=") {
        ofile << "\t<Relop> -> ^=" << endl;
        //outfile << "<Relop> -> ^=" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == ">") {
        ofile << "\t<Relop> -> >" << endl;
        //outfile << "<Relop> -> >" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "<") {
        ofile << "\t<Relop> -> <" << endl;
        //outfile << "<Relop> -> <" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "=>") {
        ofile << "\t<Relop> -> =>" << endl;
        //outfile << "<Relop> -> =>" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "=<") {
        ofile << "\t<Relop> -> =<" << endl;
        //outfile << "<Relop> -> =<" << endl;
        Lexor();
    }
    else {
        ofile << "<Relop> is invalid" << endl;
        //outfile << "<Relop> is invalid" << endl;
    }
}

void Expression()
{
    ofile << "\t<Expression> -> <Term><Expression Prime>" << endl;
    Term();
    ExpressionPrime();
}

void ExpressionPrime()
{
    if (currentLex.lexeme == "+") {
        ofile << "\t<Expression Prime> -> + <Term> <Expression Prime>" << endl;
        //outfile << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
        Lexor();
        Term();
        ExpressionPrime();
    }
    else if (currentLex.lexeme == "-") {
        ofile << "\t<Expression Prime> -> - <Term> <Expression Prime>" << endl;
        //    outfile << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
        Lexor();
        Term();
        ExpressionPrime();
    }
    else if (currentLex.token == Identifier) {
        ofile << "\t<Expression Prime> -> <Term>" << endl;
        //    outfile << "<Expression Prime> -> <Term>" << endl;
        Lexor();
        Term();
    }
    else if (currentLex.token == Identifier) {
        ofile << "\t<Expression Prime> -> <Term>" << endl;
        //    outfile << "<Expression Prime> -> <Term>" << endl;
        Lexor();
        Term();
    }
    else if (currentLex.lexeme == "(") {
        ofile << "\t<Expression Prime> -> <Term>" << endl;
        //outfile << "<Expression Prime> -> <Term>" << endl;
        Lexor();
        Term();
    }
    else if (currentLex.token == Real) {
        ofile << "\t<Expression Prime> -> <Term>" << endl;
        //    outfile << "<Expression Prime> -> <Term>" << endl;
        Lexor();
        Term();
    }
    else if (currentLex.lexeme == "true") {
        ofile << "\t<Expression Prime> -> <Term>" << endl;
        //outfile << "<Expression Prime> -> <Term>" << endl;
        Lexor();
        Term();
    }
    else if (currentLex.lexeme == "false") {
        ofile << "\t<Expression Prime> -> <Term>" << endl;
        //    outfile << "<Expression Prime> -> <Term>" << endl;
        Lexor();
        Term();
    }
    else {
        ofile << "\t<Expression Prime> -> ";
        //    outfile << "<Expression Prime> -> <Empty>" << endl;
        Empty();
    }
}

void Term()
{
    ofile << "\t<Term> -> <Factor><Term Prime>" << endl;
    Factor();
    TermPrime();
}

void TermPrime()
{
    //ofile << "<Term Prime> -> * <Factor><Term Prime> | / <Factor><Term Prime> | <Factor>" << endl;
    if (currentLex.lexeme == "*") {
        ofile << "\t<Term Prime> -> *  <Factor> <Term Prime>" << endl;
        //outfile << "<Term Prime> -> *  <Factor> <Term Prime>" << endl;
        Lexor();
        Factor();
        TermPrime();
    }
    else if (currentLex.lexeme == "/") {
        ofile << "\t<Term Prime> -> /  <Factor> <Term Prime>" << endl;
        //outfile << "<Term Prime> -> /  <Factor> <Term Prime>" << endl;
        Lexor();
        Factor();
        TermPrime();
    }
    else if (currentLex.token == Identifier) {
        ofile << "\t<Term Prime> -> <Factor>" << endl;
        //outfile << "<Term Prime> -> <Factor>" << endl;
        Factor();
    }
    else if (currentLex.token == Integer) {
        ofile << "\t<Term Prime> -> <Factor>" << endl;
        //outfile << "<Term Prime> -> <Factor>" << endl;
        Factor();
    }
    else if (currentLex.lexeme == "(") {
        ofile << "\t<Term Prime> -> <Factor> << endl" << endl;
        //outfile << "<Term Prime> -> <Factor>" << endl;
        Factor();
    }
    else if (currentLex.token == Real) {
        ofile << "\t<Term Prime> -> <Factor>" << endl;
        //outfile << "<Term Prime> -> <Factor>" << endl;
        Factor();
    }
    else if (currentLex.lexeme == "true") {
        ofile << "\t<Term Prime> -> <Factor>" << endl;
        //outfile << "<Term Prime> -> <Factor>" << endl;
        Factor();
    }
    else if (currentLex.lexeme == "false") {
        ofile << "\t<Term Prime> -> <Factor>" << endl;
        //outfile << "<Term Prime> -> <Factor>" << endl;
        Factor();
    }
    else {
        ofile << "\t<Term Prime> -> ";
        //outfile << "<Term Prime> -> <Empty>" << endl;
        Empty();
    }
}

void Factor()
{
    // ofile << "<Factor> -> - <Primary> | <Primary>" << endl;
    if (currentLex.lexeme == "-") {
        ofile << "\t<Factor> -> -  <Primary>" << endl;
        //outfile << "<Factor> -> -  <Primary>" << endl;
        Lexor();
        Primary();
    }
    else {
        ofile << "\t<Factor> -> <Primary>" << endl;
        //outfile << "<Factor> -> <Primary>" << endl;
        Primary();
    }
}

void Primary()
{
    
    //ofile << "<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) <Real> | true | false" << endl;
    if (currentLex.token == Identifier) {
        ofile << "\t<Primary> -> <Identifier>" << endl;
        //    outfile << "<Primary> -> <Identifier>" << endl;
        Lexor();
        if (currentLex.lexeme == "(") {
            ofile << "\t<Primary> -> <Identifier>  ( <ID> )" << endl;
            //        outfile << "<Primary> -> <Identifier>  ( <ID> )" << endl;
            Lexor();
            if (currentLex.token == Identifier)
            {
                ofile << "\t<Primary> -> <Identifier> ( <IDs> ) " << endl;
                
            }else{
                ofile << "ERROR: Expected Identifier Type" << endl;
                return;
            }
            Lexor();
            if (currentLex.lexeme == ")") {
                Lexor();
            }
            else {
                ofile << "ERROR: Expected a ')'" << endl;
                //    outfile << "ERROR: Expected a ')'" << endl;
                return;
            }
        }
    }
    else if (currentLex.token == Integer) {
        ofile << "\t<Primary> -> <Integer>" << endl;
        //outfile << "<Primary> -> <Integer>" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "(") {
        ofile << "\t<Primary> -> ( <Expression> )" << endl;
        //outfile << "<Primary> -> ( <Expression> )" << endl;
        Lexor();
        Expression();
        if (currentLex.lexeme == ")") {
            Lexor();
        }
        else {
            ofile << "ERROR: Expected a ')'" << endl;
            //outfile << "ERROR: Expected a ')'" << endl;
        }
    }
    else if (currentLex.token == Real) {
        ofile << "\t<Primary> -> <Real>" << endl;
        //outfile << "<Primary> -> <Real>" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "true") {
        ofile << "\t<Primary> -> true" << endl;
        //outfile << "<Primary> -> true" << endl;
        Lexor();
    }
    else if (currentLex.lexeme == "false") {
        ofile << "\t<Primary> -> false" << endl;
        //outfile << "<Primary> -> false" << endl;
        Lexor();
    }
    else {
        ofile << "ERROR: Primary is invalid." << endl;
        //outfile << "ERROR: Primary is invalid." << endl;
        Lexor();
    }
}

void Empty()
{
    ofile << "<Empty>" << endl;
}

//bool First(std::string s)
//{
//    std::map<std::string,vector<std::string>> firstList;
//    firstList["<Rat18s>"] = vector<std::string>{"function","_epsilon"};
//    firstList["<OptFunctionDefinitions>"] = vector<std::string>{"function","_epsilon"};
//    firstList["<FunctionDefinitions>"] = vector<std::string>{"function","_epsilon"};
//    firstList["<FunctionDefinitionsPrime>"] = vector<std::string>{"function","_epsilon"};
//    firstList["<Function>"] = vector<std::string>{"function"};
//    firstList["<OptParameterList>"] = vector<std::string>{"_identifier","_epsilon"};
//    firstList["<ParameterList>"] = vector<std::string>{"_identifier"};
//    firstList["<ParameterListPrime>"] = vector<std::string>{""};
//    firstList["<Parameter>"] = vector<std::string>{"_identifier"};
//    firstList["<Qualifier>"] = vector<std::string>{"int","boolean","real"};
//    firstList["<Body>"] = vector<std::string>{"{"};
//    firstList["<OptDeclarationList>"] = vector<std::string>{"int","boolean","real"};
//    firstList["<DeclarationList>"] = vector<std::string>{"int","boolean","real"};
//    firstList["<DeclarationListPrime>"] = vector<std::string>{"int","boolean","real"};
//    firstList["<Declaration>"] = vector<std::string>{"int","boolean","real"};
//    firstList["<IDs>"] = vector<std::string>{"_identifier"};
//    firstList["<IDsPrime>"] = vector<std::string>{"_identifier"};
//    firstList["<StatementList>"] = vector<std::string>{"{","_identifier","if","return","put","get","while"};
//    firstList["<StatementListPrime>"] = vector<std::string>{"{","_identifier","if","return","put","get","while"};
//    firstList["<Statement>"] = vector<std::string>{"{","_identifier","if","return","put","get","while"};
//    firstList["<Compound>"] = vector<std::string>{"{"};
//    firstList["<Assign>"] = vector<std::string>{"_identifier"};
//    firstList["<If>"] = vector<std::string>{"if"};
//    firstList["<IfPrime>"] = vector<std::string>{"endif","else"};
//    firstList["<Return>"] = vector<std::string>{"return"};
//    firstList["<ReturnPrime>"] = vector<std::string>{"-","_identifier","_integer","(","true","false"};
//    firstList["<Print>"] = vector<std::string>{"put"};
//    firstList["<Scan>"] = vector<std::string>{"get"};
//    firstList["<While>"] = vector<std::string>{"while"};
//    firstList["<Condition>"] = vector<std::string>{"-","_identifier","_integer","(","true","false"};
//    firstList["<Relop>"] = vector<std::string>{"==","^=",">","<","=>","=<"};
//    firstList["<Expression>"] = vector<std::string>{"-","_identifier","_integer","(","true","false"};
//    firstList["<ExpressionPrime>"] = vector<std::string>{"+","-","-","_identifier","_integer","(","true","false"};
//    firstList["<Term>"] = vector<std::string>{"-","_identifier","_integer","(","true","false"};
//    firstList["<TermPrime>"] = vector<std::string>{"*","/","-","_identifier","_integer","(","true","false"};
//    firstList["<Factor>"] = vector<std::string>{"-","_identifier","_integer","(","true","false"};
//    firstList["<Primary>"] = vector<std::string>{"_identifier","_integer","(","true","false"};
//    firstList["<Empty>"] = vector<std::string>{""};
//
//
//    if (firstList[s].size() == 0){return false;}
//
//    std::string check;
//    if (currentLex.token == Identifier)
//        check = "_identifier";
//    else if (currentLex.token == Integer)
//        check = "_integer";
//    else if (currentLex.token == Real)
//        check = "_real";
//    else
//        check = currentLex.lexeme;
//
//    return (VContains(firstList[s],check));
//}
//
//bool Follow(std::string s)
//{
//    std::map<std::string,vector<std::string>> followList;
//    followList["<Rat18s>"] = vector<std::string>{"$"};
//    followList["<OptFunctionDefinitions>"] = vector<std::string>{""};
//    followList["<FunctionDefinitions>"] = vector<std::string>{""};
//    followList["<FunctionDefinitionsPrime>"] = vector<std::string>{""};
//    followList["<Function>"] = vector<std::string>{""};
//    followList["<OptParameterList>"] = vector<std::string>{""};
//    followList["<ParameterList>"] = vector<std::string>{""};
//    followList["<ParameterListPrime>"] = vector<std::string>{""};
//    followList["<Parameter>"] = vector<std::string>{""};
//    followList["<Qualifier>"] = vector<std::string>{""};
//    followList["<Body>"] = vector<std::string>{""};
//    followList["<OptDeclarationList>"] = vector<std::string>{""};
//    followList["<DeclarationList>"] = vector<std::string>{""};
//    followList["<DeclarationListPrime>"] = vector<std::string>{""};
//    followList["<Declaration>"] = vector<std::string>{""};
//    followList["<IDs>"] = vector<std::string>{""};
//    followList["<IDsPrime>"] = vector<std::string>{""};
//    followList["<StatementList>"] = vector<std::string>{""};
//    followList["<StatementListPrime>"] = vector<std::string>{""};
//    followList["<Compound>"] = vector<std::string>{""};
//    followList["<Assign>"] = vector<std::string>{""};
//    followList["<If>"] = vector<std::string>{""};
//    followList["<IfPrime>"] = vector<std::string>{""};
//    followList["<Return>"] = vector<std::string>{""};
//    followList["<ReturnPrime>"] = vector<std::string>{""};
//    followList["<Print>"] = vector<std::string>{""};
//    followList["<Scan>"] = vector<std::string>{""};
//    followList["<While>"] = vector<std::string>{""};
//    followList["<Condition>"] = vector<std::string>{""};
//    followList["<Relop>"] = vector<std::string>{""};
//    followList["<Expression>"] = vector<std::string>{""};
//    followList["<ExpressionPrime>"] = vector<std::string>{""};
//    followList["<Term>"] = vector<std::string>{""};
//    followList["<TermPrime>"] = vector<std::string>{""};
//    followList["<Factor>"] = vector<std::string>{""};
//    followList["<Primary>"] = vector<std::string>{""};
//    followList["<Empty>"] = vector<std::string>{""};
//
//
//    if (followList[s].size() == 0){return false;}
//
//    std::string check;
//    if (currentLex.token == Identifier)
//        check = "_identifier";
//    else if (currentLex.token == Integer)
//        check = "_integer";
//    else if (currentLex.token == Real)
//        check = "_real";
//    else
//        check = currentLex.lexeme;
//
//    return (VContains(followList[s],check));
//}

//bool VContains(vector<std::string> v, std::string s)
//{
//    if (std::find(v.begin(), v.end(), "_epsilon") != v.end())
//    {
//        Empty();
//        return true;
//    }
//    return (std::find(v.begin(), v.end(), s) != v.end());
//}
#endif /* Parser_h */
