/* 
 * File:   CompilerUtility.h
 * Author: drose
 *
 * Created on September 22, 2015 3:10pm
 */

#ifndef COMPILERUTILITY_H
#define	COMPILERUTILITY_H
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <regex>
#include <exception>
using namespace std;

enum TOKEN_TYPE {
    KEYWORD,     //0
    SYMBOL,      //1
    IDENTIFIER,  //2
    INT_CONST,   //3
    STRING_CONST,//4
    INVALID_TOKEN//5
};
//"KW_" added as prefix avoid conflicts with C++ keywords
enum KEY_WORD {
    KW_CLASS,       //0
    KW_METHOD,      //1
    KW_FUNCTION,    //2
    KW_CONSTRUCTOR, //3
    KW_INT,         //4
    KW_BOOLEAN,     //5
    KW_CHAR,        //6
    KW_VOID,        //7
    KW_VAR,         //8
    KW_STATIC,      //9
    KW_FIELD,       //10
    KW_LET,         //11
    KW_DO,          //12
    KW_IF,          //13
    KW_ELSE,        //14
    KW_WHILE,       //15
    KW_RETURN,      //16
    KW_TRUE,        //17
    KW_FALSE,       //18
    KW_NULL,        //19
    KW_THIS,        //20
    KW_CALL_ERROR,  //21
    KW_ERROR        //22
};
struct compileError : public std::exception
{
   std::string s;
   compileError(std::string ss) : s(ss) {}
   ~compileError() throw () {} 
   const char* what() const throw() { return s.c_str(); }
}                  
#endif