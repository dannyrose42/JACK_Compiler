/* 
 * File:   Tokenizer.h
 * Author: drose
 *
 * Created on September 22, 2015, 3:17 PM
 */

#ifndef TOKENIZER_H
#define	TOKENIZER_H
#include "CompilerUtility.h"
class Tokenizer 
{
public:
    Tokenizer(string jackFileName);
    bool hasMoreTokens();
    void advance();
    string getCurrentToken();
    TOKEN_TYPE getTokenType();
    KEY_WORD getKeyword();
    char getSymbol();
    string getIdentifier();
    int getInt();
    string getString();
    string getJackFileName();
protected:
    fstream fin;
    string jackFileName, currentJackFile, currentToken;
    regex comment_regex;
    regex token_regex;
    sregex_iterator token_iterator;
};
#endif	/* TOKENIZER_H */

