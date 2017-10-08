#include "symbolTable.h"

SymbolTable::SymbolTable(bool debug) {
    this->newScope();
}

void SymbolTable::newScope() {
	this->stack.push_back(new Scope);
}

int SymbolTable::insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord) {
	this->stack.back()->insertSymbol(name, type, kind, isStatic, isArray, isRecord);	
	return 0;
}

int SymbolTable::pop() {
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

int Scope::insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord) {
	Entry *e = new Entry();
	e->type = type; //In case of Record, name of the record type (i.e. Point)
    /* attribute */
    e->kind = kind;
    e->isStatic = isStatic;
    e->isArray = isArray;
    e->isRecord = isRecord;

    this->symbols.insert(name, e);

    return 0;

}

bool Scope::search(std::string name) {
	if(this->symbols.find(name) == this->symbols.end()) {
		return false;
	} else {
		return true;
	}
} 






