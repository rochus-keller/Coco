using System;
using System.Collections.Generic;


//----------------------------------------------------------------------------------------------
// Objects
//----------------------------------------------------------------------------------------------
public class Obj {
    public const int VAR = 0, TYPE = 1, FUNC = 2;
    public int    kind;    // VAR, TYPE, FUNC
    public string name;
    public Struct type;
    public Obj(int k, string n, Struct t) { kind = k; name = n; type = t; }
}


//----------------------------------------------------------------------------------------------
// Types
//----------------------------------------------------------------------------------------------
public class Struct {
    public const int // type kinds
	NONE = 0, CHAR = 1, SHORT = 2, INT = 3, LONG = 4, USHORT = 5, UINT = 6, ULONG = 7,
	FLOAT = 8, DOUBLE = 9, STRUCT = 10, UNION = 11, ENUM = 12, PTR = 13, ARRAY = 14, FUNC = 15;
	
    public static Struct // predeclared primitive types
	None = new Struct(NONE),
	Char = new Struct(CHAR),
	Short = new Struct(SHORT),
	Int = new Struct(INT),
	Long = new Struct(LONG),
	UShort = new Struct(USHORT),
	UInt = new Struct(UINT),
	ULong = new Struct(ULONG),
	Float = new Struct(FLOAT),
	Double = new Struct(DOUBLE);

    public int          kind;     // NONE, CHAR, SHORT, ...
    public Obj          obj;      // to defining object if such an object exists
    public Struct       elemType; // for ARRAY, PTR, FUNC
    public List<Struct> parTypes; // for FUNC
    public List<Obj>    fields;   // for STRUCT, UNION, ENUM
    
    public Struct(int k) { kind = k; }
}


//----------------------------------------------------------------------------------------------
// Scopes
//----------------------------------------------------------------------------------------------
public class Scope {
    public List<Obj> locals = new List<Obj>();
    public Scope outer;
}


//----------------------------------------------------------------------------------------------
// Symbol Table
//----------------------------------------------------------------------------------------------
public class SymTab {
    public Scope  curScope;
    public Obj    noObj;
    
    public SymTab() {
	curScope = new Scope();
	noObj = new Obj(Obj.VAR, "???", Struct.None);
    }
    
    public Obj Insert(int kind, string name, Struct type) {
	Obj obj = new Obj(kind, name, type);
	foreach (Obj x in curScope.locals) {
	    if (x.name.Equals(name)) Error("-- " + name + " declared twice");
	}
	curScope.locals.Add(obj);
	return obj;
    }
    
    public Obj Find(string name) {
	for (Scope s = curScope; s != null; s = s.outer) {
	    foreach (Obj x in s.locals) {
		if (x.name.Equals(name)) return x;
	    }
	}
	// when all declarations are processed correctly this error should be reported
	// Error("-- " + name + " undeclared");
	return noObj;
    }
    
    public void OpenScope() {
	Scope s = curScope;
	curScope = new Scope();
	curScope.outer = s;
    }
    
    public void CloseScope() {
	curScope = curScope.outer;
    }
    
    private void Error(string s) {
	Console.WriteLine(s);
    }
    
}
