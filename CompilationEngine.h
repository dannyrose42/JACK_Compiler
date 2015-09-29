/* 
 * File:   CompilationEngine.h
 * Author: drose
 *
 * Created on September 22, 2015, 3:23 PM
 */

#ifndef COMPILATIONENGINE_H
#define	COMPILATIONENGINE_H
#include "CompilerUtility.h"
class CompilationEngine 
{
public:
    CompilationEngine(Tokenizer * t);
    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    void compileExpressionList();
protected:
    Tokenizer * tokenizer;
    ofstream fout;
    
};
#endif	/* COMPILATIONENGINE_H */

