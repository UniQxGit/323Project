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

enum State { Start, StateSeparator, StateKeyword, Final };
enum LexorType { Identifier=0, Keyword=1, Separator=2, Operator=3, Literal = 4};
std::string LexToStr(LexorType l);
LexorType GetState(char& input, std::stringstream& check);
bool DFSM(std::string str);

State currentState;
std::map<std::string,LexorType> tokens;

int main(int argc, const char * argv[]) {
    
    //Read in Keywords.txt
    std::cout << "LOADED KEYWORDS: " << std::endl;
    std::ifstream myfile ("keywords.txt");
    std::string line;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            std::cout << line << std::endl;
            tokens[line] = Keyword;
        }
        myfile.close();
    }else{
        std::cout << "Failed to open file.." << std::endl;
    }
    
    std::cout << "LOADED SEPARATORS: " << std::endl;
    myfile.open("separators.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            std::cout << line << std::endl;
            tokens[line] = Separator;
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
    
    std::cout << std::left << std::setw(15) << "TOKEN" << "LEXEME" << std::endl;
    for(char& c : str) {
        if (c != '\n' && c != ' ')
            check << c;
        type = GetState(c,check);
        //std::cout << "|" << check.str() << "|" << std::endl;
        if (currentState == Final)
        {
            currentState = Start;
            std::cout << std::left << std::setw(15) << LexToStr(type) << check.str() << std::endl;
            check.str("");
        }
    }
    return false;
}

LexorType GetState(char& input, std::stringstream& check)
{
    switch(currentState)
    {
        case Start:
            if (input == ' ' || input == '\n')
                return Identifier;
            
            if (tokens[check.str()] == Separator)
            {
                std::cout << std::left << std::setw(15) << LexToStr(Separator) << input << std::endl;
                currentState = StateSeparator;
                return Literal;
            }else{
                currentState = StateKeyword;
                return Identifier;
            }
            break;
        case StateSeparator:
            if (tokens[std::string(1,input)] == Separator)
            {
                std::cout << std::left << std::setw(15) << LexToStr(Literal) << check.str() << std::endl;
                check.str(std::string(1,input));
                currentState = Final;
                return Separator;
            }
            break;
        case StateKeyword:
            if (input == ' ' || input == '\n')
            {
                currentState = Final;
                return Identifier;
            }else if (tokens[check.str()] == Keyword)
            {
                currentState = Final;
                return Keyword;
            }
            break;
    }
    return Literal;
}

std::string LexToStr(LexorType l)
{
    switch(l)
    {
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
        case Literal:
            return "Literal";
            break;
    }
}
