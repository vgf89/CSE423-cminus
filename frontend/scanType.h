/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#ifndef SCANTYPE_H
#define SCANTYPE_H

typedef struct superToken {
    int lineNumber;
    char* tokenType; //string from scanner
    enum {IntType, VoidType, CharType, BoolType, RecordType, UndefinedType} type; 
    	//enum for ID types, set in grammar
    char *stringThatWasTyped;
    char* KWTvalue;
    long long numericalValue;
    char* IDvalue;
    char letterData;
} SPT;

#endif /* SCANTYPE_H */
