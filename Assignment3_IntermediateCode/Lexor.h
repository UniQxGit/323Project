//
//  Lexor.h
//  323_SimpleLexorAssignment
//
//  Created by John on 11/6/18.
//  Copyright © 2018 UniQx. All rights reserved.
//

#ifndef Lexor_h
#define Lexor_h

#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <iostream>




enum State { Start=0, StateNoMatch=1, StateIdentifier=2, StateIdentifierLoop=3, StateInteger=4, StateReal=5, StateComment=6, StateMatch=7};
enum LexorType {NoMatch = 0, Identifier=1, Keyword=2, Separator=3, Operator=4, Integer = 5, Real = 6, Comment = 7};
std::string LexToStr(LexorType l);
LexorType StrToLex(std::string str);
void ResetState(std::stringstream& check);
void GetState(char& input, std::stringstream& check);
void PrintLexeme(LexorType type,std::string lexeme);
bool DFSM(std::string str);
bool CheckInt(char c);
bool CheckLetter(char c);
bool CheckAccepted(int state);
LexorType AcceptingToLex(int state, char& input, std::string str);

class Lexeme
{
public:
    LexorType token;
    std::string lexeme;
    
    Lexeme(LexorType t, std::string l)
    {
        token = t;
        lexeme = l;
    }
    
    Lexeme()
    {
        token = Comment;
        lexeme = "";
    }
};

Lexeme GetLex();

std::queue<Lexeme> lexemes;


const int Lexer_DFSM_State[13][9] =
{
    11, 11,  2,  4,  8,  6, 10,  0, 10, //A Starting state
    11, 12,  2,  2,  8, 10,  1,  1, 10, //B Identifier
    11,  1,  2,  2,  8, 10,  1,  1, 10, //B1
    11, 12, 10,  4,  8,  6, 10,  3, 10, //dfsC Integer
    11,  3, 10,  4,  8,  6, 10,  3, 10, //C1
    11, 12, 10,  6,  8, 10, 10,  5, 10, //D Real
    11,  5, 10,  6,  8, 10, 10,  5, 10, //D1
    8,  8,  8,  8,  7,  8,  8,  8,  8, //E Comment
    8,  8,  8,  8,  7,  8,  8,  8,  8, //E1
    11, 12, 10, 10,  7, 10, 10,  9, 10, //F No Match
    11, 12, 10, 10,  7, 10, 10,  9, 10, //F1
    11, 11, 11, 11, 11, 11, 11, 11, 11, //H Single Character Match
    12, 12, 12, 12, 12, 12, 12, 12, 11  //G Multi Cahracter Match
};

const int Lexer_DFSM_Accepted[6] = {11,1,3,5,7,12};

int currentState;
std::map<std::string,LexorType> tokens;

std::ofstream ofile;        //initialize output file stream

void LoadTokens(std::string fileName)
{
    //Read in Tokens.txt
    std::cout << "LOADED Tokens: " << std::endl;
    std::ifstream myfile (fileName);
    std::string line;
    
    if (myfile.is_open())
    {
        LexorType curLexor;
        
        
        while ( getline (myfile,line) )
        {
            if(line == "--KEYWORDS--")
                curLexor = Keyword;
            else if (line == "--SEPARATORS--")
                curLexor = Separator;
            else if (line == "--OPERATORS--")
                curLexor = Operator;
            else if (line == "--COMMENT--")
                curLexor = Comment;
            
            
            tokens[line] = curLexor;
            std::cout << std::left << std::setw(15) << line << LexToStr(curLexor) << std::endl;
        }
        myfile.close();
    }else{
        std::cout << "Failed to open file.." << std::endl;
    }
}

std::string LoadFile()
{
    
    
    std::cout << "Enter a filename (Without Extension): ";    //change here to add user-inputted file
    std::string fname;
    std::cin >> fname;
    fname += ".txt";
    std::ifstream myfile ("TestCases/" + fname);
    std::stringstream ss;
    char c;
    if (myfile.is_open())
    {
        while ( myfile.get (c) )
        {
            ss << c;
        }
        myfile.close();
    }else{
        std::cout << "Failed to open file.." << std::endl;
    }
    
    return ss.str();
}

bool DFSM(std::string str)
{
    std::stringstream check;
    LexorType type;
    
    std::cout << "--------\n" << "OUTPUT" << "\n--------" << std::endl;
    std::cout << std::left << std::setw(15) << "TOKEN" << "LEXEME"  << std::endl;
    
    ResetState(check);
    for(char& c : str) {
        //            if (c != '\n' && c != ' ' && c != '\t')
        //                check << c;
        GetState(c,check);
        //std::cout << "|" << check.str() << "|" << std::endl;
    }
    return false;
}

void ResetState(std::stringstream& check)
{
    currentState = 0;
    check.str("");
}

int GetCol(char& input, std::stringstream& check)
{
    if (tokens[check.str()+input] != NoMatch)
    {
        return 0;
    }else if (tokens[std::string(1,input)])
    {
        return 1;
    }
    else if (CheckLetter(input))
    {
        return 2;
    }else if (CheckInt(input))
    {
        return 3;
    }else if (input == '!')
    {
        return 4;
    }else if (input == '.')
    {
        return 5;
    }else if (input == '$')
    {
        return 6;
    }else if (input == ' ' || input == '\n' || input == '\t')
    {
        return 7;
    }else{
        return 8;
    }
}

void GetState(char& input, std::stringstream& check)
{
    int col  = GetCol(input,check);
    
    currentState = Lexer_DFSM_State[currentState][col];
    //        std::cout << currentState << "|" << check.str() << "|" << input << "|" << Lexer_DFSM_State[currentState][col] << std::endl;
    if(CheckAccepted(currentState))
    {
        if (col != 1 && col != 7)
            check << input;
        PrintLexeme(AcceptingToLex(currentState,input,check.str()),check.str());
        if (CheckAccepted(Lexer_DFSM_State[currentState][col]) && Lexer_DFSM_State[currentState][col] != currentState)
        {
            check.str("");
            GetState(input,check);
        }
        ResetState(check);
    }else{
        if (col != 7)
            check << input;
    }
}

std::string LexToStr(LexorType l)
{
    switch(l)
    {
        case NoMatch:
            return "No Match";
            break;
        case Keyword:
            return "Keyword";
            break;
        case Separator:
            return "Separator";
            break;
        case Identifier:
            return "Identifier";
            break;
        case Operator:
            return "Operator";
            break;
        case Integer:
            return "Integer";
            break;
        case Real:
            return "Real";
            break;
        case Comment:
            return "Comment";
            break;
        default:
            return "";
            break;
    }
}

bool CheckAccepted(int state)
{
    if (state > 100)
        state -= 100;
    for (int i = 0; i < 6; i++)
    {
        if (Lexer_DFSM_Accepted[i] == state)
        {
            return true;
        }
    }
    return false;
}

LexorType AcceptingToLex(int state, char& input, std::string str)
{
    switch (state) {
        case 11:
            return tokens[str];
            break;
        case 1:
            return Identifier;
            break;
        case 3:
            return Integer;
            break;
        case 5:
            return Real;
            break;
        case 7:
            return Comment;
            break;
        case 12:
            return tokens[std::string(1,input)];
            break;
        default:
            return NoMatch;
            break;
    }
}

//string type not recognized in switch statement
LexorType StrToLex(std::string str)
{
    if (str == "Keyword")
        return Keyword;
    else if (str == "Separator")
        return Separator;
    else if (str == "Identifier")
        return Identifier;
    else if (str == "Operator")
        return Operator;
    else if (str == "Integer")
        return Integer;
    else if (str == "Real")
        return Real;
    else if (str == "Comment")
        return Comment;
    else
        return NoMatch;
}

bool CheckInt(char c)
{
    return (c >= 48 && c <= 57);
}

bool CheckLetter(char c)
{
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == '_');
}

Lexeme GetLex()
{
    Lexeme lex;
    if (lexemes.empty())
    {
        lex = Lexeme(NoMatch,"$");
        return lex;
    }
    
    lex = lexemes.front();
    lexemes.pop();
    while (lex.token == Comment)
    {
        lex = lexemes.front();
        lexemes.pop();
    }
    
    ofile << "GotLexor: " << std::left << std::setw(15) << LexToStr(lex.token) << "|" << lex.lexeme << "|" << std::endl;
    return lex;
}

void PrintLexeme(LexorType type,std::string lexeme)
{
    lexemes.push(Lexeme(type,lexeme));
    lexeme = "|" + lexeme + "|";
    std::cout << std::left << std::setw(15) << LexToStr(type) << lexeme << std::endl;
    ofile << std::left << std::setw(15) << LexToStr(type) << lexeme << std::endl;    //outputs line as given in console output above, to file
}
#endif /* Lexor_h */
