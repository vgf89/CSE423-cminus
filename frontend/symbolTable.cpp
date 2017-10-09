#include "symbolTable.h"
#include <utility>
#include <iostream>

SymbolTable::SymbolTable(bool debug) {
    this->newScope();
}

void SymbolTable::newScope() {
	this->stack.push_back(new Scope);
    //if(yydebug) printf("New Scope: %d\n", this->getDepth());
}

// Returns NULL on success, or the previous-existing symbol on failure
Entry* SymbolTable::insertSymbol(std::string name, enum typeEnum type, enum kindEnum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
    if (this->searchCurrent(name) == NULL) {
        this->stack.back()->insertSymbol(name, type, kind, isStatic, isArray, isRecord, linenum);
        return NULL;
    }

        return this->searchCurrent(name);
}

int SymbolTable::pop() {
    //printf("Popping scope\n");
    //if(yydebug) printf("Remove Scope: %d\n", this->getDepth());
	if (!this->stack.empty()) {
		this->stack.pop_back();
		return 0;
	} else {
		return 1;
	}
}

int SymbolTable::getDepth() {
    return this->stack.size();
}

// Returns the symbol found in the current scope
Entry* SymbolTable::searchCurrent(std::string name) {
	return this->stack.back()->search(name);
}

// Returns the symbol found in the scope stack
Entry* SymbolTable::searchAll(std::string name) {
    Entry* e;
	for (std::size_t i = 0; i < this->stack.size(); i++) {
        e = this->stack[i]->search(name);
    	if (e != NULL) {
        	return e;
    	}
    }
    return NULL;
}

bool SymbolTable::getDebug() {
	return this->debug;
}

void SymbolTable::setDebug(bool d) {
	this->debug = d;
}

Entry* SymbolTable::getParentLast() {
	if (stack.size() < 2) {
		return NULL;
    }
    auto test = this->stack[stack.size()-2];
    auto test2 = test->symbols;
    auto test3 = test2.rbegin();
    //if (yydebug) printf("stack size: %d\n", this->getDepth());
    auto test4 = test3->second;
	return test4; 
}

Entry* Scope::insertSymbol(std::string name, enum typeEnum type, enum kindEnum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
	Entry *e = new Entry();
	e->type = type;
    /* attribute */
    e->kind = kind;
    e->isStatic = isStatic;
    e->isArray = isArray;
    e->isRecord = isRecord;
    e->linenum = linenum;

    if(this->symbols.find(name) != this->symbols.end())
        return this->symbols.find(name)->second;
    
    //if (yydebug) printf("Adding symbol: %s\n", name.c_str());
    this->symbols[name] = e;
    return NULL;

}

Entry* Scope::search(std::string name) {
    auto e = this->symbols.find(name);
	if(e == this->symbols.end()) {
		return NULL;
	} else {
		return e->second;
	}
} 





