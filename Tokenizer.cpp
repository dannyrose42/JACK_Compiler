#include "Tokenizer.h"
Tokenizer::Tokenizer(string fileName){
    jackFileName = fileName;
    //Open source .jack file
    fin.open(fileName.c_str());
    if(fin.fail())
        cout << "Failed to open input file: " << fileName << endl;
    //Read entire source .jack file into currentJackFile string
    string temp;
    while(!fin.eof()){
        getline(fin, temp);
        currentJackFile += temp;
        currentJackFile += "\n";
    }
    fin.close();
    //Build regular expressions from .txt files
    string comment_regexStr, token_regexStr;

    fin.open("comment_regex.txt");
    if (fin.fail())
        cout << "fail to open comment_regex.txt" << endl;
    getline(fin, comment_regexStr);
    fin.close();
    comment_regex = regex(comment_regexStr);
    
    fin.open("jackToken_regex.txt");
    if (fin.fail())
        cout << "failed to open jackToken_regex.txt" << endl;
    getline(fin, token_regexStr);
    fin.close();
    token_regex = regex(token_regexStr);
    
    //Remove comments from .jack source using regular expression
    currentJackFile = regex_replace(currentJackFile, comment_regex, "");
    
    //Build regex token iterator and eof marker iterator
    token_iterator = sregex_iterator(currentJackFile.begin(),
                                     currentJackFile.end(), token_regex);
    currentToken = token_iterator->str();
}
bool Tokenizer::hasMoreTokens(){
    return token_iterator != sregex_iterator();
}
void Tokenizer::advance(){
    if(hasMoreTokens()){
        token_iterator++;
        if (hasMoreTokens())
            currentToken = token_iterator->str();
    }
}
string Tokenizer::getCurrentToken(){
    return currentToken;
}
TOKEN_TYPE Tokenizer::getTokenType(){   
    
    //If the token is a single char there are only 3 easily determined cases
    if(currentToken.size() == 1){
        if(isalpha(currentToken[0]))
            return IDENTIFIER;
        else if(isdigit(currentToken[0]))
            return INT_CONST;
        else 
            return SYMBOL;
    }
    //check for string literals (34 is ascii value for double quote)
    if(currentToken[0] == 34)
        return STRING_CONST;
    
    //check for 2+ digit int const
    bool allDigits = true;
    for(int i=0;i<currentToken.size()-1;i++){
        if (!isdigit(currentToken[i]))
            allDigits = false;
    }
    if (allDigits)
        return INT_CONST;
    //At this point all symbols, int and string consts have been caught
    //Catch keywords
    if(currentToken.compare("class")==0
            ||currentToken.compare("constructor")==0
            ||currentToken.compare("function")==0
            ||currentToken.compare("method")==0
            ||currentToken.compare("field")==0
            ||currentToken.compare("static")==0
            ||currentToken.compare("var")==0
            ||currentToken.compare("int")==0
            ||currentToken.compare("char")==0
            ||currentToken.compare("boolean")==0
            ||currentToken.compare("void")==0
            ||currentToken.compare("true")==0
            ||currentToken.compare("false")==0
            ||currentToken.compare("null")==0
            ||currentToken.compare("this")==0
            ||currentToken.compare("let")==0
            ||currentToken.compare("do")==0
            ||currentToken.compare("if")==0
            ||currentToken.compare("else")==0
            ||currentToken.compare("while")==0
            ||currentToken.compare("return")==0)
            return KEYWORD;
    //Only remaining options is 2+ character identifier
    //Identifiers must be alphanumeric or underscore(s)(ascii value for _ is 95)  
    bool validIdentifier = true;
    for(int i=0;i<currentToken.size()-1;i++){
        if(!isalnum(currentToken[i])){
            if(!currentToken[i]==95)
                validIdentifier=false;
        }
    }
    if(validIdentifier)
        return IDENTIFIER;
    //At this point if the token hasn't been caught its not a valid token
    cout << "WARNING INVALID TOKEN:" << currentToken << endl;
    return INVALID_TOKEN;  
}
KEY_WORD Tokenizer::getKeyword(){
    if(getTokenType() != KEYWORD){
        cout <<"bad getKeyword() call on:" << currentToken <<endl;
        return KW_CALL_ERROR;
    }
    if(currentToken.compare("constructor")==0)
        return KW_CONSTRUCTOR;
    else if (currentToken.compare("function")==0)
        return KW_FUNCTION;
    else if (currentToken.compare("method")==0)
        return KW_METHOD;
    else if (currentToken.compare("field")==0)
        return KW_FIELD;
    else if (currentToken.compare("static")==0)
        return KW_STATIC;
    else if (currentToken.compare("var")==0)
        return KW_VAR;
    else if (currentToken.compare("int")==0)
        return KW_INT;
    else if (currentToken.compare("char")==0)
        return KW_CHAR;
    else if (currentToken.compare("boolean")==0)
        return KW_BOOLEAN;
    else if (currentToken.compare("void")==0)
        return KW_VOID;
    else if (currentToken.compare("true")==0)
        return KW_TRUE;
    else if (currentToken.compare("false")==0)
        return KW_FALSE;
    else if (currentToken.compare("null")==0)
        return KW_NULL;
    else if (currentToken.compare("this")==0)
        return KW_THIS;
    else if (currentToken.compare("let")==0)
        return KW_LET;
    else if (currentToken.compare("do")==0)
        return KW_DO;
    else if (currentToken.compare("if")==0)
        return KW_IF;
    else if (currentToken.compare("else")==0)
        return KW_ELSE;
    else if (currentToken.compare("while")==0)
        return KW_WHILE;
    else if (currentToken.compare("return")==0)
        return KW_RETURN;
    else{
        cout << "Invalid Token:" << currentToken << endl;
        return KW_ERROR;
    }      
}
char Tokenizer::getSymbol(){
    if(getTokenType() != SYMBOL){
        cout << "bad getSymbol() call on:" << currentToken << endl;
        return '%';
    }
    else return currentToken[0];
}
string Tokenizer::getIdentifier() {
    if (getTokenType() != IDENTIFIER){
        cout << "badd getIdentifier() call on:" << currentToken << endl;
        return "";
    }
    else return currentToken;
}
int Tokenizer::getInt(){
    if (getTokenType() != INT_CONST){
        cout << "bad getInt() call on:" << currentToken << endl;
        return -1;
    }
    return stoi(currentToken);
}
string Tokenizer::getString(){
    if (getTokenType() != STRING_CONST){
        cout << "bad getString() call on :" << currentToken << endl;
        return "";
    }
    //Returns string minus the enclosing double quote chars
    else return currentToken.substr(1,currentToken.size()-2);
}
