typedef struct superToken {
    int lineNumber;
    char* tokenType;
    char *stringThatWasTyped;
    char* KWTvalue;
    long long numericalValue;
    char* IDvalue;
    char letterData;
} SPT;