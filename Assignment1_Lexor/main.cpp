    //
    //  main.cpp
    //  323_SimpleLexorAssignment
    //
    //  Created by John Lee on 9/6/18.
    //  Copyright © 2018 UniQx. All rights reserved.
    //

    
    #include "Parser.h"

    int main(int argc, const char * argv[]) {
        currentState = Start;
        LoadTokens("tokens.txt");
        std::string loadedCode = LoadFile();
        
        std::cout << std::endl;
        
        
		ofile.open("lexeme.txt");	//outputs lexeme results to file
        DFSM(loadedCode);
		ofile.close();				//closes file stream
        
        Rat18s();
         return 0;
    }

    
