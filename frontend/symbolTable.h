#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <map>
#include <vector>
#include <string>
#include "treenode.h"


//enum class kind_enum {Var, Func};

class Entry {
public: 
    /* name */
    //string name (already key of map)
    /* type */
    enum typeEnum type;
    /* attribute */
    enum kindEnum kind; //In case of Record, name of the record type (i.e. Point)
    bool isStatic;
    bool isArray;
    bool isRecord;

    int linenum;
};

class Scope {
public:
    int insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum); /*
        Add symbol to scope. Return 0 on succcess, 1 on failure (symbol already exists).
        If debugging print new entry
    */
    bool search(std::string name); //Tries to get string name from symbols map. If found, return true, otherwise false

    std::map<std::string, Entry> symbols; //string is name of the symtable entry
private:
};

class SymbolTable {
public:
    SymbolTable(bool debug); // Make new symboltable, initializes the global scope
    void newScope(); // Next (child) scope. new Scope is made inside this function and pushed to stack, if debug print new scope depth

    int insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum); // Add symbol to current scope (top of stack)

    int pop();  // Removes and frees(?) current scope, return int for errors (0 = success, 1 = already popped global)

    /* Assuming children can redeclare variable... */
    bool searchCurrent(std::string name); // Used for checking during declaration (true if found, false otherwise)
    bool searchAll(std::string name); // Used for checking during type checks (true if found, false otherwise)
    int depth(); // Just returns stack.length();

private:
    bool debug;
    std::vector<Scope *> stack; // stack.at(0) is the global scope
};


#endif /* _SYMBOLTABLE_H_ */
