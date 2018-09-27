//
//  main.cpp
//  323_SimpleLexorAssignment
//
//  Created by John Lee on 9/6/18.
//  Copyright © 2018 UniQx. All rights reserved.
//

#include <iostream>
#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

enum State { Start, StateNoMatch, StateIdentifier, StateKeyword, StateSeparator, StateOperator, StateInteger, StateReal, StateComment, Final };
enum LexorType {NoMatch = 0, Identifier=1, Keyword=2, Separator=3, Operator=4, Integer = 5, Real = 6, Comment = 7};
std::string LexToStr(LexorType l);
LexorType StrToLex(std::string str);
void ResetState(std::stringstream& check);
void GetState(char& input, std::stringstream& check);
void PrintLexeme(LexorType type,std::string lexeme);
bool DFSM(std::string str);
bool CheckInt(char c);
bool CheckLetter(char c);

State currentState;
std::map<std::string,LexorType> tokens;

int main(int argc, const char * argv[]) {
    
    //Read in Tokens.txt
    std::cout << "LOADED Tokens: " << std::endl;
    std::ifstream myfile ("tokens.txt");
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
    
    std::cout << std::endl;
    currentState = Start;
    myfile.open("code.txt");
    std::stringstream ss;
    char c;
    if (myfile.is_open())
    {
        while ( myfile.get (c) )
        {
            ss << c;
            //std::cout << "GOT: " <<  ss.str() << std::endl;
        }
        myfile.close();
    }else{
        std::cout << "Failed to open file.." << std::endl;
    }
    
    DFSM(ss.str());
     return 0;
}

bool DFSM(std::string str)
{
    std::stringstream check;
    LexorType type;
    
    std::cout << "--------\n" << "OUTPUT" << "\n--------" << std::endl;
    std::cout << std::left << std::setw(15) << "TOKEN" << "LEXEME"  << std::endl;
    for(char& c : str) {
        if (c != '\n' && c != ' ' && c != '\t')
            check << c;
        GetState(c,check);
        //std::cout << "|" << check.str() << "|" << std::endl;
    }
    return false;
}

void ResetState(std::stringstream& check)
{
    currentState = Start;
    check.str("");
}

void GetState(char& input, std::stringstream& check)
{
    //std::cout << input << "|" << check.str() << std::endl;
    switch(currentState)
    {
        case Start:
            if (input == ' ' || input == '\n' || input == '\t')
            {
                ResetState(check);
                return;
            }
            if (tokens[std::string(1,input)] != NoMatch)
            {
                PrintLexeme(tokens[std::string(1,input)], std::string(1,input));
                ResetState(check);
            }else if (CheckLetter(input))
            {
//                std::cout << input << "->Identifier" << std::endl;
                currentState = StateIdentifier;
            }
            else if (CheckInt(input)){
//                std::cout << input << "->Integer" << std::endl;
                currentState = StateInteger;
            }else if (tokens[std::string(1,input)] == Comment)
            {
//                std::cout << input << "->Comment" << std::endl;
                currentState = StateComment;
            }else{
//                std::cout << input << "->NoMatch" << std::endl;
                currentState = StateNoMatch;
            }
            break;
        case StateNoMatch:
            if (tokens[check.str()] != NoMatch)
            {
                PrintLexeme(Operator, check.str());
                ResetState(check);
            }else if(input == ' ' || input == '\n' || input == '\t')
            {
                PrintLexeme(NoMatch, check.str());
                ResetState(check);
            }
            break;
        case StateIdentifier:
            if (input == ' ' || input == '\n' || input == '$')
            {
                PrintLexeme(Identifier, check.str());
                ResetState(check);
            }else if (tokens[check.str()] == Keyword)
            {
                PrintLexeme(Keyword, check.str());
                ResetState(check);
            }else if (!CheckInt(input) && !CheckLetter(input))
            {
                PrintLexeme(Identifier, check.str().erase(check.str().find(input)));
                ResetState(check);
                GetState(input,check);
            }
            break;
        case StateInteger:
            if (input == ' ' || input == '\n')
            {
                PrintLexeme(Integer, check.str());
                ResetState(check);
            }else if (!CheckInt(input))
            {
                PrintLexeme(Integer, check.str().erase(check.str().find(input)));
                ResetState(check);
                GetState(input,check);
            }else if (input == '.')
            {
                currentState = StateReal;
                GetState(input, check);
            }
            break;
        case StateReal:
            if (input == ' ' || input == '\n')
            {
                PrintLexeme(Real, check.str());
                ResetState(check);
            }else if (!CheckInt(input))
            {
                PrintLexeme(Real, check.str().erase(check.str().find(input)));
                ResetState(check);
                GetState(input,check);
            }
            break;
        case StateComment:
            if (tokens[std::string(1,input)] == Comment)
            {
                PrintLexeme(Comment, check.str());
                ResetState(check);
            }
            break;
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

void PrintLexeme(LexorType type,std::string lexeme)
{
    lexeme = "|" + lexeme + "|";
    std::cout << std::left << std::setw(15) << LexToStr(type) << lexeme << std::endl;
}
