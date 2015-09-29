#include "CompilationEngine.h"
#include "Tokenizer.h"
CompilationEngine::CompilationEngine(Tokenizer* t){
    tokenizer = t;
    string jackFileName = tokenizer->getJackFileName(); 
    fout.open(jackFileName.c_str());
    if (fout.fail())
        cout << "Failled to initiliaze compilation engine outputfile:"
                << jackFileName << endl;
}
CompilationEngine::compileClass(){ //throws compileError
    //Insure first token is keyword class
    if(tokenizer->getKeyword()!= KW_CLASS){
       throw compileError("Bad compileClass call on invalid type:");
       return;
    }
    //This check SHOULD be redundant based on the above, but just in case
    if(tokenizer->getCurrentToken().compare("class")!=0){
        throw compileError("ERROR: complileClass expected:'class'");
        return;
    }
    //Write xml markup
    fout << "<class>" << endl;
    fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>"<<endl;
    
    tokenizer->advance();
    //Ensure next token is identifier to be used as class name
    if(tokenizer->getTokenType()!= IDENTIFIER){
        throw compileError("ERROR: complileClass expected: indentifier className");
        return;
    }
    //Write xml markup
    fout << "<identifier> " << tokenizer->getIdentifier() << " </identifier>" endl;
    
    tokenizer->advance();
    //Ensure next token is open curly brace
    if(tokenizer->getSymbol()!='{'){
        throw compileError("ERROR: compileClass expected symbol:'{'");
        return;
    }
    //write xml markup
    fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
    
    tokenizer->advance();
    //Check for empty class case
    if(tokenizer->getSymbol()=='}'){
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        fout << "</class>" << endl;
        return;
    }
    // Use while loop with bool flag to check for 0 or more classVarDec(s) 
    // and/or subroutineDec(s))
    bool done = false;
    while (!done){
        if(tokenizer->getKeyword() == KW_STATIC || KW_FIELD){
            try{
                compileClassVarDec();
            }catch(compileError& error){
                cout << error << endl;
                return;
            }
        }
        else if(tokenizer->getKeyword()= KW_CONSTRUCTOR || KW_FUNCTION || KW_METHOD)
            try{
                compileSubroutine();
            }catch(compileError& error){
                cout << error << endl;
                return;
            }
        else done = true;
    }
    //Ensure non-empty class ends with closing curly brace
    if(tokenizer->getSymbol()!='}'){
        throw compileError("compileClass expected either: classVarDec or SubroutineDec" << endl);
    }
    //Write final class xml markup
    else{
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        fout << "</class>" << endl;
        return;
    }
    //Program should never reach this point
    throw compileError("compileClass expected: classVarDec | subroutineDec |'}' ");
}
CompilationEngine::compileClassVarDec(){//throws compileError
    fout << "<classVarDec>" << endl;
    //Ensure first token is either 'static' or 'field' if not, throw exception and return
    if(tokenizer->getKeyword() == KW_STATIC || KW_FIELD)
        fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>"<<endl;
    else{ 
        throw compileError("compileClassVarDec expected: KW_STATIC | KW_FIELD");
        return;
    }
    tokenizer->advance();
    //Next token is type should be: int | char | boolean | className
    if (tokenizer->getKeyword()==KW_INT|KW_CHAR|KW_BOOLEAN)
        fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>"<<endl;
    //Check for className
    else if (tokenizer->getTokenType()==IDENTIFIER){
        fout << "<identifier> " << tokenizer->getIdentifier() << " </identifier>"<<endl;
    //Token is not int | char | boolean | className. Throw compileError
    }else{
            throw compileError("compileClassVarDec expected type dec:KW_INT | KW_CHAR | KW_BOOLEAN | IDENTIFIER className ");
            return;
        }
    tokenizer->advance();
    
    //Next we should have 0 or varNames seperated by commas. 
    if (tokenizer->getTokenType()==IDENTIFIER)
        fout << "<identifier> " << tokenizer->getIdentifier() << " </identifier>"<<endl;
    else{ throw compileError("compileClassVarDec expected first varName"); 
        return;
    }
    bool multiDec = false; 
    tokenizer->advance();
    if (tokenizer->getCurrentToken().compare(",")==0){
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        multiDec = true;
        while (multiDec){
            tokenizer->advance();
            if (tokenizer->getTokenType()==IDENTIFIER)
                fout << "<identifier> " << tokenizer->getIdentifier() << " </identifier>"<<endl;
            else{
                throw compileError("compileClassCarDec expected next varName in multiDec");
                return;
            }
            tokenizer->advance();
            //If next token is not comma, we've reached the end of th multiDec
            if(tokenizer->getCurrentToken().compare(",")!=0)
                multiDec = false;
            //If the token IS a comma, output it and continue multiDec
            else fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        } 
    }
    //Next token should be ';'
    if (tokenizer->getSymbol()==';'){
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        fout << "</classVarDec>" << endl;
    }else 
        throw compileError("compileClassVarDec expected:';'");    
}
CompilationEngine::compileSubroutine(){
    fout << "<subroutineDec> " << << endl;
    //Constructor, Function, Method?
    if (tokenizer->getKeyword()==KW_CONSTRUCTOR||KW_FUNCTION||KW_METHOD)
        fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>" << endl;
    else{
        throw compileError("compileSubroutine expected:KW_CONSTRUCTOR|KW_FUNCTION|KW_METHOD");
        return; 
    }
    tokenizer->advance();
    //Return type?
    if (tokenizer->getKeyword()==KW_VOID||KW_INT||KW_CHAR||KW_BOOLEAN)
        fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>" << endl;
    else if (tokenizer->getTokenType==IDENTIFIER)
        fout << "<identifier> " << tokenizer->getIdentifier() << endl;
    else {
        throw compileError("compileSubroutine expected: return type dec");
        return;
    }
    tokenizer->advance();
    //Subroutine Name
    if (tokenizer->getTokenType()==IDENTIFIER)
        fout << "<identifier> " << tokenizer->getIdentifier() << endl;
    else{
        throw compileError("compileSubroutine expected: subroutineName");
        return;
    }
    tokenizer->advance();
    //Parameter list
    if (tokenizer->getSymbol()=='('){
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        tokenizer->advance();
        try{
            compileParameterList();
        }catch (compileError& error){
            cout << error << endl;
            return; 
        }
    }else{
        throw compileError("compileSubroutine expected:'('");
        return;
    }
    //Parameter list closing parethesis
    if (tokenizer->getSymbol()==')')
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
    else{
        throw compileError("compileSubroutine expected:')'");
        return;
    }
    tokenizer->advance();
    //class def open parend
    if (tokenizer->getSymbol()=='{')
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
    else{
        throw compileError("compileSubroutine expected:'{'");
        return;
    }
    tokenizer->advance();
    //0 or more Var Dec | statements
    while (tokenizer->getKeyword()==KW_VAR){
        try{
            compileVarDec();
        }catch(compileError& error){
            cout << error << endl;
            return;
        }
    }
    //statements
    try {
        compileStatements();
    }catch(compileError& error){
        cout << error << endl;
        return;
    }
    if(tokenizer->getSymbol()=='}'){
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        fout << "</subroutineDec>" << endl;
        tokenizer->advance();
    }else
        throw compileError("compileSubroutine expected:'}'");  
}
CompilationEngine::compileParameterList(){
    fout << "<parameterList>" << endl;
    //first param type
    if (tokenizer->getKeyword()==KW_INT||KW_CHAR||KW_BOOLEAN)
        fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>" << endl;
    else if (tokenizer->getTokenType==IDENTIFIER)
        fout << "<identifier> " << tokenizer->getIdentifier() << endl;
    else {
        throw compileError("compileParamenterList expected: first parameter type");
        return;
    }
    tokenizer->advance();
    //first param name
    if(tokenizer->getTokenType()==IDENTIFIER)
        fout << "<identifier> " << tokenizer->getIdentifier() << endl;
    else{
        throw compileError("compileParameterList expected:IDENTIFER - first param name");
        return;
    }
    tokenizer->advance();
    //check for more than 1 param
    bool moreParams = false;
    while(tokenizer->getSymbol()==','){
        fout << "<symbol> " << tokenizer->getSymbol() << " </symbol>" << endl;
        tokenizer->advance();
        //next param type
        if (tokenizer->getKeyword()==KW_INT||KW_CHAR||KW_BOOLEAN)
            fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>" << endl;
        else if (tokenizer->getTokenType==IDENTIFIER)
            fout << "<identifier> " << tokenizer->getIdentifier() << endl;
        else {
            throw compileError("compileParamenterList expected: next parameter type");
            return;
        }
        tokenizer->advance();
        //next param name
        if(tokenizer->getTokenType()==IDENTIFIER)
            fout << "<identifier> " << tokenizer->getIdentifier() << endl;
        else{
            throw compileError("compileParameterList expected:IDENTIFER - first param name");
            return;
        }
        tokenizer->advance();
    }
    if (tokenizer->getSymbol()==')'){
        fout << "<keyword> " << tokenizer->getCurrentToken() << " </keyword>" << endl;
        fout << "</parameterList>" << endl;
    }
    else throw compileError("compileParameterList expected:')'");       
}
CompilationEngine::compileVarDec(){
    
}
CompilationEngine::compileStatements(){
    
}
CompilationEngine::compileDo(){
    
}
CompilationEngine::compileLet(){
    
}
CompilationEngine::compileWhile(){
    
}
CompilationEngine::compileReturn(){
    
}
CompilationEngine::compileIf(){
    
}
CompilationEngine::compileExpression(){
    
}
CompilationEngine::compileTerm(){
    
}
CompilationEngine::compileExpressionList(){
    
}