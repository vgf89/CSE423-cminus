//NUMCONST Type
typedef struct NCT {
    int lineNumber;
    
    int numericalValue;
    char* stringThatWasTyped;
    char* tokenType;
} NCT;

//ID Type
typedef struct IDT {
    int lineNumber;
    char* tokenType;
    char* stringThatWasTyped;

    char* IDvalue;
} IDT;

//KeyWord Type
typedef struct KWT {
    int lineNumber;
    char* tokenType;

    char* kwtvalue;
} KWT;