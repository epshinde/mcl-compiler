/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
*/

#include "parser.h"
#include <sstream>

ostringstream variablesAndFunctions;
int functionLevel = 1;
map<string, bool> defVar;
map<string, int> varAndLine;
map<string, Token> SymTable = {
	{"IF", IF},
	{"if", IF},
    {"ELSE", ELSE},
	{"else", ELSE},
    {"PRINT", PRINT},
	{"print", PRINT},
    {"INT", INT},
	{"int", INT},
    {"FLOAT", FLOAT},
	{"float", FLOAT},
    {"CHAR", CHAR},
	{"char", CHAR},
    {"STRING", STRING},
	{"string", STRING},
    {"BOOL", BOOL},
	{"bool", BOOL},
    {"PROGRAM", PROGRAM},
	{"program", PROGRAM},
    {"TRUE", TRUE},
	{"true", TRUE},
    {"FALSE", FALSE},
	{"false", FALSE}
};

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

//Prog:= PROGRAM IDENT CompStmt
bool Prog(istream& in, int& line) {
	// 
	LexItem t;
	t = Parser::GetNextToken(in, line);
	if (t != PROGRAM) {
		ParseError(line, "Missing Program keyword");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != IDENT) {
		ParseError(line, "Missing Program name");
		return false;
	}

	bool comp = CompStmt(in, line);

	if (!comp) {
		ParseError(line, "Invalid Program");
		return false;
	}

	map<string, bool>::iterator it;

	cout << variablesAndFunctions.str();
	cout << "(DONE)" << endl;

	return true;
}

// StmtList:= 	 { Stmt }
bool StmtList(istream& in, int& line) {
	bool stmt = Stmt(in, line);

	if (!stmt) {
		ParseError(line, "Syntactic error in statement list.");
		return false;
	}

	while (stmt) {
		LexItem tok = Parser::GetNextToken(in, line);
		if (tok == INT || tok == FLOAT || tok == BOOL || tok == CHAR || tok == STRING || tok == IDENT || tok == IF || tok == PRINT || tok == LBRACE) {
			Parser::PushBackToken(tok);
			stmt = Stmt(in, line);
		} else {
			Parser::PushBackToken(tok);
			return true;
		}
		if (!stmt) {
			ParseError(line, "Syntactic error in statement list.");
			return false;
		}
	}

	return true;
}

// Stmt:= DeclStmt | ControlStmt | CompStmt
bool Stmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	bool oneOfTheStatements = false;
	if (tok == INT || tok == FLOAT || tok == BOOL || tok == CHAR || tok == STRING) {
		Parser::PushBackToken(tok);
		oneOfTheStatements = DeclStmt(in, line);
	} else if (tok == IDENT || tok == IF || tok == PRINT){
		Parser::PushBackToken(tok);
		oneOfTheStatements = ControlStmt(in, line);
	} else if (tok == LBRACE) {
		Parser::PushBackToken(tok);
		oneOfTheStatements = CompStmt(in, line);
	}

	if (!oneOfTheStatements) {
		return false;
	}

	return true;
}

// DeclStmt:= (INT | FLOAT | BOOL | CHAR | STRING ) VarList ;
bool DeclStmt(istream& in, int& line) {
	LexItem t;
	t = Parser::GetNextToken(in, line);

	switch (t.GetToken()){
		case INT:
		case FLOAT:
		case BOOL:
		case CHAR:
		case STRING:
			break;
		default:
			return false;
	}



	bool varList = VarList(in, line);

	if (varList) {
		t = Parser::GetNextToken(in, line);
		if (t != SEMICOL) {
			ParseError(line, "Missing semicolon at end of Statement");
			return false;
		}
		return true;
	}

	ParseError(line, "Invalid declaration statement");
	return false;
}

// VarList ::= Var [= Expr] { ,Var [= Expr]}
bool VarList(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	map<string, bool>:: iterator defVarIter;
	
	defVarIter = defVar.find(t.GetLexeme());
	if (defVarIter != defVar.end()) {
		ParseError(line, "Variable Redefinition");
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return false;
	} else {
		Parser::PushBackToken(t);
	}

	bool varCheck = Var(in, line);
	if (!varCheck) {
		ParseError(line, "Missing variable after comma");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	string varName = t.GetLexeme();


	t = Parser::GetNextToken(in, line);

	if (t == ASSOP) {
		bool exprCheck = Expr(in, line);
		if (!exprCheck) {
			ParseError(line, "Missing expression after assignment operator");
			return false;
		}
		variablesAndFunctions << "Initialization of the variable " << varName << " in the declaration statement at line " << line << endl;
		defVar[varName] = true;
	} else {
		defVar[varName] = false;
		Parser::PushBackToken(t);
	}

	t = Parser::GetNextToken(in, line);
	while (t == COMMA) {
		t = Parser::GetNextToken(in, line);
		defVarIter = defVar.find(t.GetLexeme());
		if (defVarIter != defVar.end()) {
			ParseError(line, "Variable Redefinition");
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return false;
		} else {
			Parser::PushBackToken(t);
		}
		bool varCheck = Var(in, line);
		if (!varCheck) {
			ParseError(line, "Missing variable after comma");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		varName = t.GetLexeme();
		t = Parser::GetNextToken(in, line);
		if (t==ASSOP) {
			bool exprCheck = Expr(in, line);
			if (!exprCheck) {
				ParseError(line, "Missing expression after assignment operator");
				return false;
			}
			variablesAndFunctions << "Initialization of the variable " << varName << " in the declaration statement at line " << line << endl;
			defVar[varName] = true;
		} else {
			defVar[varName] = false;
			Parser::PushBackToken(t);
		}
		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);

	return true;
}

// ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
bool ControlStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	bool oneOfTheStatements = false;
	if (tok == IDENT) {
		Parser::PushBackToken(tok);
		oneOfTheStatements = AssignStmt(in, line);
		if (!oneOfTheStatements) {
			ParseError(line, "Invalid control statement.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL) {
			ParseError(--line, "Missing semicolon at end of Statement.");
			oneOfTheStatements = false;
		}
	} else if (tok == IF){
		Parser::PushBackToken(tok);
		oneOfTheStatements = IfStmt(in, line);
		if (!oneOfTheStatements) {
			ParseError(line, "Incorrect IF statement");
		}
	} else {
		Parser::PushBackToken(tok);
		oneOfTheStatements = PrintStmt(in, line);
		if (!oneOfTheStatements) {
			ParseError(line, "Incorrect Print statement.");
			ParseError(line, "Invalid control statement.");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL) {
			ParseError(line, "Missing semicolon at end of Statement.");
			oneOfTheStatements = false;
		}
	}

	if (!oneOfTheStatements) {
		ParseError(line, "Invalid control statement.");
		return false;
	}

	return true;

}

//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in PrintStmt" << endl;
	
	t = Parser::GetNextToken(in, line);

	if (t != PRINT) {
		ParseError(line, "Missing Print Keyword");
		return false;
	}

	t = Parser::GetNextToken(in, line);

	if( t != LPAREN ) {
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return true;
}//End of PrintStmt

//CompStmt:= ‘{‘ StmtList ‘}’
bool CompStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != LBRACE) {
		return false;
	}

	bool stmtListChecker = StmtList(in, line);

	if (!stmtListChecker) {
		ParseError(line, "Incorrect statement list");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RBRACE) {
		ParseError(line, "Syntactic error in statement list.");
		ParseError(line, "Incorrect statement list");
		return false;
	}

	return true;
	 
}

//IfStmt:= IF (Expr) Stmt [ ElSE Stmt ]
bool IfStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != IF) {
		ParseError(line, "No IF keyword");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != LPAREN) {
		ParseError(line, "Missing left Parenthesis before expression");
		return false;
	}

	bool exprCheck = Expr(in, line);

	if (!exprCheck) {
		ParseError(line, "Missing if statement condition");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RPAREN) {
		ParseError(line, "Missing right Parenthesis after expression");
		return false;
	}

	variablesAndFunctions << "in IfStmt then-clause at nesting level: " << functionLevel << endl;
	functionLevel++;

	bool stmtCheck = Stmt(in, line);

	if (!stmtCheck) {
		ParseError(line, "Missing statement");
		return false;
	}
	
	functionLevel--;

	tok = Parser::GetNextToken(in, line);
	if (tok != ELSE) {
		Parser::PushBackToken(tok);
	} else {
		variablesAndFunctions << "in IfStmt else-clause at nesting level: " << functionLevel << endl;
		functionLevel++;
		bool stmtCheckTwo = Stmt(in, line);
		if (!stmtCheckTwo) {
			ParseError(line, "Missing statement");
			return false;
		}
		functionLevel--;
	}
	return true;
}

//AssgnStmt:= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	map<string, bool>::iterator varIter;

	varIter = defVar.find(tok.GetLexeme());
	if (varIter == defVar.end()) {
		ParseError(line, "Undeclared variable");
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		ParseError(line, "Incorrect Assignment Statement");
		return false;
	} else {
		Parser::PushBackToken(tok);
	}

	bool varCheck = Var(in, line);
	if (!varCheck) {
		ParseError(line, "Missing a variable");
		return false;
	} else {
		tok = Parser::GetNextToken(in, line);
	}

	tok = Parser::GetNextToken(in, line);

	if (tok != ASSOP && tok != ADDASSOP && tok != SUBASSOP && tok != MULASSOP && tok != DIVASSOP && tok != REMASSOP) {
		ParseError(line, "Missing Assignment Operator");
		ParseError(line, "Incorrect Assignment Statement");
		return false;
	}

	bool exprCheck = Expr(in, line);
	if (!exprCheck) {
		ParseError(line, "Missing Expression in Assignment Statement");
		ParseError(line, "Incorrect Assignment Statement");
		return false;
	}

	return true;
}

//Var:= IDENT
bool Var(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != IDENT) {
		ParseError(line, "Missing an identifier");
		return false;
	}
	defVar.insert({tok.GetLexeme(), false});
	Parser::PushBackToken(tok);
	return true;
} 

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Expr:= LogANDExpr { || LogANDExpr }
bool Expr(istream& in, int& line) {
	bool LogANDExprCheck = LogANDExpr(in, line);
	if (!LogANDExprCheck) {
		return false;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	while (tok == OR) {
		bool LogANDExprChecker = LogANDExpr(in, line);
		if (!LogANDExprChecker) {
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(tok);
	return true;
}

//LogANDExpr:= EqualExpr { && EqualExpr }
bool LogANDExpr(istream& in, int& line) {
	bool EqualExprCheck = EqualExpr(in, line);
	if (!EqualExprCheck) {
		return false;
	}
	LexItem tok = Parser::GetNextToken(in, line);
	while (tok == AND) {
		bool EqualExprChecker = EqualExpr(in, line);
		if (!EqualExprChecker) {
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(tok);
	return true;
}

//EqualExpr:= RelExpr [ (== | != ) RelExpr ]
bool EqualExpr(istream& in, int& line) {
	bool RelExprCheck = RelExpr(in, line);
	if (!RelExprCheck) {
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != EQ && tok != NEQ) {
		Parser::PushBackToken(tok);
	} else {
		bool RelExprChecker = RelExpr(in, line);
		if (!RelExprChecker) {
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		
		if (tok == EQ || tok == NEQ) {
			ParseError(line, "Illegal Equality Expression.");
			return false;
		} else {
			Parser::PushBackToken(tok);
		}
	}

	return true;
}

//RelExpr:= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream& in, int& line) {
	bool AddExprCheck = AddExpr(in, line);
	if (!AddExprCheck) {
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != GTHAN && tok != LTHAN) {
		Parser::PushBackToken(tok);
	} else {
		bool AddExprChecker = AddExpr(in, line);
		if (!AddExprChecker) {
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok == GTHAN || tok == LTHAN) {
			ParseError(line, "Illegal Relational Expression.");
			return false;
		} else {
			Parser::PushBackToken(tok);
		}
	}

	return true;
}

//AddExpr:= MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream& in, int& line) {
	bool MultExprCheck = MultExpr(in, line);
	if (!MultExprCheck) {
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	while (tok == PLUS || tok == MINUS) {
		bool MultExprChecker = MultExpr(in, line);
		if (!MultExprChecker) {
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}

	Parser::PushBackToken(tok);
	return true;
}

//MultExpr:= UnaryExpr { ( * | / | % ) UnaryExpr }
bool MultExpr(istream& in, int& line) {
	bool UnaryExprCheck = UnaryExpr(in, line);
	if (!UnaryExprCheck) {
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	while (tok == MULT || tok == DIV || tok == REM) {
		bool UnaryExprChecker = UnaryExpr(in, line);
		if (!UnaryExprChecker) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(tok);
	return true;
}

//UnaryExpr:= [( - | + | ! )] PrimaryExpr
bool UnaryExpr(istream& in, int& line) {
	int sign = 17;
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != MINUS && tok != PLUS && tok != NOT) {
		Parser::PushBackToken(tok);
	} else {
		switch(tok.GetToken()) {
			case MINUS:
				sign = 18;
				break;
			case PLUS:
				sign = 17;
				break;
			case NOT:
				sign = 33;
				break;
			default:
				break;
		}
	}

	bool PrimaryExprCheck = PrimaryExpr(in, line, sign);
	if (!PrimaryExprCheck) {
		return false;
	}

	return true;
}

//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok != IDENT && tok != ICONST && tok != RCONST && tok != RCONST && tok != SCONST && tok != BCONST && tok != CCONST) {
		// cout << "if: " << tok;
		if (tok == LPAREN) {
			bool ExprCheck = Expr(in, line);
			// cout << "if2: " << int(ExprCheck) << endl;
			if (!ExprCheck) {
				ParseError(line, "Missing expression after Left Parenthesis");
				return false;
			}
			tok = Parser::GetNextToken(in, line);
			// cout << "if3: " << tok;
			if (tok != RPAREN) {
				ParseError(line, "Missing right Parenthesis after expression");
				return false;
			}
			return true;
		} else {
			return false;
		}
	} else {
		// cout << "else: " << tok;
		return true;
	}
}

