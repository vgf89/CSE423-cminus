#include <map>
#include <vector>
#include <string>

/* kind */
enum class kind_enum {Var, Func};


class Entry {
private:
    /* symbol type */
    std::string m_type;
    
    /* attribute */
    kind_enum m_kind ;
    int m_linenum;
    bool m_isStatic;
    bool m_isArray; 
    bool m_isRecord;

public:

    //constructor
    Entry(std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
        
        if(type.length() >= 1)
            m_type = type;
        else
            printf("type string empty\n");

        m_isStatic = isStatic;
        m_isArray = isArray;
        m_isRecord = isRecord;
        m_linenum = linenum;
    }

    //getters
    std::string getType() {
        return m_type;
    }

    kind_enum getKind() {
        return m_kind;
    }

    bool isStatic() {
        return m_isStatic;
    }

    bool isArray() {
        return m_isArray;
    }

    bool isRecord() {
        return m_isRecord;
    }
};

class Scope {
private:
    std::map<std::string, Entry> m_symbols; //string is name of the symtable entry
    bool m_debug;

public:

    //constructor
    Scope(bool debug) {
        m_debug = debug;
    }

     //Add symbol to scope. Return 0 on succcess, 1 on failure (symbol already exists). If debugging print new entry
    int insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
        
        //if symbol not in list
        if(m_symbols.find(name) == m_symbols.end()) {
            Entry e(type, kind, isStatic, isArray, isRecord, linenum);
            m_symbols.insert(std::make_pair(name, e));
            if(m_debug)
                printf("symbol[%s]: type:%s | kind:%u | isStatic:%d | isArray:%d | isRecord:%d\n", name.c_str(), type.c_str(), kind, isStatic, isArray, isRecord);
            return 0;
        }
        else
            return 1;
    }

    // Used for checking during declaration, returns pointer to found entry or null
    Entry* searchSymbol(std::string name) {
        if(m_symbols.find(name) != m_symbols.end())
            return &m_symbols.at(name);
        else {
            return NULL;
        }
            
    }

    
};

class SymbolTable {
private:
    bool m_debug;
    Scope *cur;
    std::vector<Scope *> stack; // stack.at(0) is the global scope

public:
    //constructor
    SymbolTable(bool debug) {
        m_debug = debug;
    }
    // Next (child) scope. new Scope is made inside this function and pushed to stack, if debug print new scope depth
    void pushScope() {
        Scope s(m_debug);
        cur = &s;
        stack.push_back(&s);

        if(m_debug)
            printf("pushed, depth: %d\n", getDepth());
    }

    // Removes and frees(?) current scope, return 0 if success and 1 if fail
    bool popScope() {

        if(!stack.empty()) {
            stack.pop_back();
            
            //reset cur if stack isn't not empty
            if(!stack.empty())
                cur = stack.back();

            if(m_debug)
                printf("popped, depth: %d\n", getDepth());

            return 0;  
        }
        else
            return 1;
    }  

    //insert symbol in current scope
    int insertSymbol(std::string name, std::string type, kind_enum kind, bool isStatic, bool isArray, bool isRecord, int linenum) {
        return cur->insertSymbol(name, type, kind, isStatic, isArray, isRecord, linenum);
    }

    //get current depth of table
    int getDepth() {
        return stack.size();
    }

    /* Assuming children can redeclare variable... */
    // Used for checking during declaration, returns pointer to found entry or null
    Entry* searchCurrent(std::string name) {
        return cur->searchSymbol(name);
    }

    // Used for checking during type checks (true if found, false otherwise)
    bool searchAll(std::string name) {
        int depth = getDepth();
        bool found = false;

        for (int i = 0; i < depth; ++i) {
            Scope *tmp = stack.at(i);
            if(tmp->searchSymbol(name) != NULL)
                found = true;
        }
        return found;
    }
};