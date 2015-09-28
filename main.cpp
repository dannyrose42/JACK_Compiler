/* 
 * File:   main.cpp
 * Author: drose
 *
 * Created on September 22, 2015, 3:04 PM
 */

#include <cstdlib>
#include "Tokenizer.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    string input, output;
    ofstream fout;
       
    cout << "Enter .jack File name with extension" << endl;
    cin >> input;
    
    output = input;
    output.erase(input.size()-4,4);
    output += "xml";
    
    fout.open(output.c_str());
    if (fout.fail())
        cout << "Failed to initialize output file:" << output << endl;
    
    
    Tokenizer  *tokenizer = new Tokenizer(input);
    fout << "<tokens>" << endl;
  
    while(tokenizer->hasMoreTokens()){

        cout << tokenizer->getCurrentToken() << " - ";
        if(tokenizer->getTokenType()==0){
            cout << "KEYWORD" << endl;
            fout << "<keyword> " << tokenizer->getCurrentToken()
                    << " </keyword>" << endl;
        }
        else if(tokenizer->getTokenType()==1){
            cout << "SYMBOL" << endl;
            fout << "<symbol> " << tokenizer->getSymbol()
                    << " </symbol>" << endl;
        }
        else if(tokenizer->getTokenType()==2){
            cout << "IDENTIFIER" << endl;
            fout << "<identifier> " << tokenizer->getIdentifier()
                    << " </identifier>" << endl;
        }
        else if(tokenizer->getTokenType()==3){
            cout << "INT_CONST" << endl;
            fout << "<integerConstant> " << tokenizer->getInt()
                    << " </integerConstant>" << endl;
        }
        else if(tokenizer->getTokenType()==4){
            cout << "STRING_CONST" << endl;
            fout << "<stringConstant> " << tokenizer->getString()
                    << " </stringConstant>" << endl;
        }
        else if(tokenizer->getTokenType()==5){
            cout << "INVALID_TOKEN" << endl;
            fout << "<invalidToken> " << tokenizer->getCurrentToken()
                    << " </invalidToken>" << endl;
        }
        tokenizer->advance();
    }
    fout << "</tokens>";
    fout.close();
    return 0;
}

