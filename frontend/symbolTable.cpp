#include "symbolTable.h"
#include <utility>

SymbolTable::SymbolTable(bool debug) {
    this->newScope();
}

void SymbolTable::newScope() {
    printf("Adding new scope\n");
	this->stack.push_back(new Scope);
}

int SymbolTable::insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
	this->stack.back()->insertSymbol(name, type, kind, isStatic, isArray, isRecord, linenum);	
	return 0;
}

int SymbolTable::pop() {
    printf("Popping scope\n");
	if (!this->stack.empty()) {
		this->stack.pop_back();
		return 0;
	} else {
		return 1;
	}
}

int SymbolTable::depth() {
	return this->stack.size();
}

bool SymbolTable::searchCurrent(std::string name) {
	return this->stack.back()->search(name);
}

bool SymbolTable::searchAll(std::string name) {
	for (auto i = this->stack.begin(); i != this->stack.end(); ) {
    	if ((*i)->search(name) == false) {
        	i++;
    	}
      	else {
        	return true;
      	}
    }
    return false;
}

int Scope::insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
	Entry *e = new Entry();
	e->type = type; //In case of Record, name of the record type (i.e. Point)
    /* attribute */
    e->kind = kind;
    e->isStatic = isStatic;
    e->isArray = isArray;
    e->isRecord = isRecord;
    e->linenum = linenum;

    this->symbols.insert(std::make_pair(name, *e));

    return 0;

}

bool Scope::search(std::string name) {
	if(this->symbols.find(name) == this->symbols.end()) {
		return false;
	} else {
		return true;
	}
} 





