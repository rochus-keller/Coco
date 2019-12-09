

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n, const char *func_name) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n, const char *func_name) {
	if (la->kind==n) Get(); else { SynErr(n, func_name); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n, __FUNCTION__);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n, __FUNCTION__);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::Test() {
		A();
		E();
		C();
		G();
		H();
		I();
		J();
}

void Parser::A() {
		if (la->kind == _a) {
			Get();
		} else if (la->kind == _a || la->kind == _b || la->kind == _c) {
			B();
		} else SynErr(11,__FUNCTION__);
}

void Parser::E() {
		if (la->kind == _e || la->kind == _f) {
			F();
		} else if (la->kind == _e) {
		} else SynErr(12,__FUNCTION__);
		Expect(_e,__FUNCTION__);
}

void Parser::C() {
		while (la->kind == _a) {
			Get();
		}
		if (la->kind == _d) {
			D();
		}
		B();
}

void Parser::G() {
		if (la->kind == _a || la->kind == _b) {
			if (eee) {
				if (la->kind == _a) {
					Get();
				} else if (la->kind == _b) {
					Get();
				} else SynErr(13,__FUNCTION__);
			} else {
				Get();
			}
		}
		Expect(_a,__FUNCTION__);
}

void Parser::H() {
		if (la->kind == _a) {
			Get();
		}
		if (hhh) {
			Expect(_a,__FUNCTION__);
		}
		if (hhh) {
			Expect(_a,__FUNCTION__);
		}
		Expect(_a,__FUNCTION__);
}

void Parser::I() {
		while (la->kind == _a) {
			Get();
		}
		if (iii) {
			if (la->kind == _a) {
				Get();
			} else if (la->kind == _b) {
				Get();
			} else SynErr(14,__FUNCTION__);
		} else if (la->kind == _b) {
			Get();
		} else SynErr(15,__FUNCTION__);
}

void Parser::J() {
		while (aaa) {
			Expect(_a,__FUNCTION__);
		}
		while (la->kind == _a || la->kind == _b) {
			if (eee) {
				if (la->kind == _a) {
					Get();
				} else if (la->kind == _b) {
					Get();
				} else SynErr(16,__FUNCTION__);
			} else {
				Get();
			}
		}
		Expect(_a,__FUNCTION__);
}

void Parser::B() {
		while (la->kind == _b) {
			Get();
		}
		if (la->kind == _c) {
			Get();
		} else if (la->kind == _a) {
		} else SynErr(17,__FUNCTION__);
		Expect(_a,__FUNCTION__);
}

void Parser::D() {
		Expect(_d,__FUNCTION__);
		if (la->kind == _b) {
			Get();
		}
}

void Parser::F() {
		if (la->kind == _f) {
			Get();
		}
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};

	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};

	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	Test();
	Expect(0,__FUNCTION__);
}

Parser::Parser(Scanner *scanner) {
	maxT = 10;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[1][12] = {
		{T,x,x,x, x,x,x,x, x,x,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"a expected"); break;
			case 2: s = coco_string_create(L"b expected"); break;
			case 3: s = coco_string_create(L"c expected"); break;
			case 4: s = coco_string_create(L"d expected"); break;
			case 5: s = coco_string_create(L"e expected"); break;
			case 6: s = coco_string_create(L"f expected"); break;
			case 7: s = coco_string_create(L"g expected"); break;
			case 8: s = coco_string_create(L"h expected"); break;
			case 9: s = coco_string_create(L"i expected"); break;
			case 10: s = coco_string_create(L"??? expected"); break;
			case 11: s = coco_string_create(L"invalid A"); break;
			case 12: s = coco_string_create(L"invalid E"); break;
			case 13: s = coco_string_create(L"invalid G"); break;
			case 14: s = coco_string_create(L"invalid I"); break;
			case 15: s = coco_string_create(L"invalid I"); break;
			case 16: s = coco_string_create(L"invalid J"); break;
			case 17: s = coco_string_create(L"invalid B"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s);
	exit(1);
}


