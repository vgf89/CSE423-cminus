//NUMCONST Type
typedef struct NCT {
    int lineNumber;
    char *tokenType;
    char* stringThatWasTyped;

    long long numericalValue;
} NCT;

//CHARCONST Type
typedef struct CCT {
    int lineNumber;
    char *tokenType;
    char *stringThatWasTyped;

    char letterData;
} CCT;

//BOOLCONST Type
typedef struct BCT {
    int lineNumber;
    char *tokenType;
    char *stringThatWasTyped;

    int numericalValue;
} BCT;

//Record Type
typedef struct RECT {
    int lineNumber;
    char *tokenType;
    char *stringThatWasTyped;
} RECT;

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
    char* KWTvalue;
} KWT;
