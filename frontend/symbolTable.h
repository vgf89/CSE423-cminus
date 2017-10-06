#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

#include <map>
#include <vector>
#include <string>


enum class kind_enum {Var, Func};

class Entry {
    /* name */
    //string name (already key of map)
    /* type */
    std::string type; //In case of Record, name of the record type (i.e. Point)
    /* attribute */
    kind_enum kind ;
    bool isStatic;
    bool isArray;
    bool isRecord;
};

class Scope {
public:
    int insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord); /*
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
    int insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord); // Add symbol to current scope (top of stack)
    int pop();  // Removes and frees(?) current scope, return int for errors (0 = success, 1 = already popped global)

    /* Assuming children can redeclare variable... */
    bool searchCurrent(std::string name); // Used for checking during declaration (true if found, false otherwise)
    bool searchAll(std::string name); // Used for checking during type checks (true if found, false otherwise)

private:
    bool debug;
    int depth(); // Just returns stack.length();
    std::vector<Scope *> stack; // stack.at(0) is the global scope

};


#endif /* _SYMBOLTABLE_H_ */
