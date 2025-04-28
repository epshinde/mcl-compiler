/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
*/

#include "parserInterp.h"
#include <iostream>
#include <string>

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

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

bool IdentList(istream& in, int& line);


//Program is: Prog = PROGRAM IDENT CompStmt
bool Prog(istream& in, int& line)
{
	bool sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			sl = CompStmt(in, line);
			if( !sl  )
			{
				ParseError(line, "Invalid Program");
				return false;
			}
			cout << "(DONE)" << endl;
			return true;
		}
		else
		{
			ParseError(line, "Missing Program name");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	else
	{
		ParseError(line, "Missing Program keyword");
		return false;
	}
}//End of Prog

//StmtList ::= Stmt { Stmt }
bool StmtList(istream& in, int& line)
{
	bool status;
	LexItem tok;
	
	status = Stmt(in, line);
	tok = Parser::GetNextToken(in, line);
	while(status && tok != RBRACE)
	{
		Parser::PushBackToken(tok);
		
		status = Stmt(in, line);
		tok = Parser::GetNextToken(in, line);
	}
	if(!status)
	{
		ParseError(line, "Syntactic error in statement list.");
			
		return false;
	}
	Parser::PushBackToken(tok);
	return true;
}//End of StmtList

//DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING) VarList 
bool DeclStmt(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	if(t == INT || t == FLOAT || t == CHAR || t == BOOL || t == STRING ) {
		
		status = VarList(in, line, t);
		
		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}
		return true;	
	}
	
	
	return true;
}//End of Decl

//Stmt ::= DeclStmt ; | ControlStmt ; | CompStmt
bool Stmt(istream& in, int& line) {
	bool status = true;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT: case IF: case IDENT:
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Invalid control statement.");
			return false;
		}
		
		break;
	case INT: case FLOAT: case CHAR: case BOOL: case STRING:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if(!status)
		{
			ParseError(line, "Invalid declaration statement.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if(t == SEMICOL)	
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}	
		break;
	case LBRACE:
		Parser::PushBackToken(t);
		status = CompStmt(in, line);
		if(!status)
		{
			ParseError(line, "Invalid compound statement.");
			return false;
		}
			
		break;
	default:
		Parser::PushBackToken(t);
		
		return false;
	}

	return status;
}//End of Stmt

//CompStmt ::= '{' StmtList '}'
bool CompStmt(istream& in, int& line)
{
	bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
	if(t == LBRACE)
	{
		status = StmtList(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect statement list");
			return false;
		}
		LexItem t = Parser::GetNextToken(in, line);
		if(t == RBRACE)
		{
			return true;
		}
		else if(t == DONE)
		{
			ParseError(line, "Missing end of program right brace.");
			return false;
		}
		else
		{
			ParseError(line, "Missing right brace.");
			return false;
		}
	}
	return status;
}//End of CompStmt

// ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
bool ControlStmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect Print Statement");
			return false;
		}	
		t = Parser::GetNextToken(in, line);
		
		if(t == SEMICOL)	
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		
		if(t == SEMICOL)	
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		
		break;
	case IF:
		status = IfStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect IF Statement");
			return false;
		}	
		
		break;
	
	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
}//End of ControlStmt

//VarList ::= Var [= Expr] {, Var [= Expr]}
bool VarList(istream& in, int& line, LexItem & idtok) {
	bool status = false, exprstatus = false;
	string identstr;
	
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		identstr = tok.GetLexeme();
		// cout << identstr << endl;
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
	
		ParseError(line, "Missing Variable Name");
		return false;
	}
		
	tok = Parser::GetNextToken(in, line);
	if(tok == ASSOP)
	{
		Value exprVal = Value(); 
		exprstatus = Expr(in, line, exprVal);
		if(!exprstatus)
		{
			ParseError(line, "Incorrect initialization for a variable.");
			return false;
		}

		switch (idtok.GetToken()) {
			case INT:
				if (!exprVal.IsInt()) {
					ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 343");
					return false;
				} 
				break;
			case FLOAT:
				if (!exprVal.IsReal() && !exprVal.IsInt()) {
					ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 349");
					return false;
				} else if (exprVal.IsInt()) {
					exprVal.SetReal(exprVal.GetInt());
					exprVal.SetType(VREAL);
				}
				break;
			case BOOL:
				if (!exprVal.IsBool()) {
					ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 355");
					return false;
				}
				break;
			case CHAR:
				if (!exprVal.IsChar()) {
					ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 361");
					return false;
				}
				break;
			case STRING:
				if (!exprVal.IsString()) {
					ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 367");
					return false;
				}
				break;
			default:
				ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 370");
				return false;
		}

		TempsResults[identstr] = exprVal;

		// cout<< "Initialization of the variable " << identstr <<" in the declaration statement at line " << line << endl;
		tok = Parser::GetNextToken(in, line);
		
		if (tok == COMMA) {
			
			status = VarList(in, line, idtok);
		
		}
		else
		{
			Parser::PushBackToken(tok);
			return true;
		}
	}
	else if (tok == COMMA) {
		
		status = VarList(in, line, idtok);
	}
	else if(tok == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		
		return false;
	}
	else {
	
		Parser::PushBackToken(tok);
		return true;
	}
	
	return status;
	
}//End of VarList

//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>;
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

	while (!(*ValQue).empty()){
		cout << (*ValQue).front();
		ValQue->pop();
	}

	cout << endl;	
	return true;
}//End of PrintStmt

//IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
bool IfStmt(istream& in, int& line) {
	bool ex=false, status ; 
	static int nestlevel = 0;
	LexItem t;
		
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	Value IfStmtValue = Value();
	ex = Expr(in, line, IfStmtValue);

	if( !ex ) {
		ParseError(line, "Missing if statement condition");
		return false;
	}

	if (!IfStmtValue.IsBool()) {
		ParseError(line, "Illegal logic operation in if.");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
	
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	nestlevel +=1;
	// cout << "in IfStmt then-clause at nesting level: " << nestlevel << endl;

	if (IfStmtValue.GetBool()) {
		// cout << "Works " << endl;
		status = Stmt(in, line);

		if(!status) {
			ParseError(line, "If-Stmt Syntax Error");
			return false;
		}
	}

	if (!IfStmtValue.GetBool()) {
		t = Parser::GetNextToken(in, line);
		if (t.GetToken() == LBRACE) {
			int numberOfLBrace = 1;
			while (true) {
				t = Parser::GetNextToken(in, line);
				if (t.GetToken() == LBRACE) {
					numberOfLBrace += 1;
				}
				if (t.GetToken() == RBRACE) {
					numberOfLBrace -= 1;
					if (numberOfLBrace <= 0) {
						break;
					}
				}
				if (t.GetToken() == DONE) {
					ParseError(line, "RBRACE is missing - Line 484");
					return false;
				}
			}
		} else {
			while (t.GetToken() != SEMICOL) {
				t = Parser::GetNextToken(in, line);
				if (t.GetToken() == DONE) {
					ParseError(line, "SEMICOL is missing - Line 492");
					return false;
				}
			}
		}
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == ELSE && !IfStmtValue.GetBool()) {
		// cout << "in IfStmt else-clause at nesting level: " << nestlevel << endl;
		
		status = Stmt(in, line);
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		
		nestlevel--;
		return true;
	} else if (t == ELSE) {
		t = Parser::GetNextToken(in, line);
		if (t.GetToken() == LBRACE) {
			int numberOfLBrace = 1;
			while (true) {
				t = Parser::GetNextToken(in, line);
				if (t.GetToken() == LBRACE) {
					numberOfLBrace += 1;
				}
				if (t.GetToken() == RBRACE) {
					numberOfLBrace -= 1;
					if (numberOfLBrace <= 0) {
						break;
					}
				}
				if (t.GetToken() == DONE) {
					ParseError(line, "RBRACE is missing - Line 484");
					return false;
				}
			}
		} else {
			while (t.GetToken() != SEMICOL) {
				t = Parser::GetNextToken(in, line);
				if (t.GetToken() == DONE) {
					ParseError(line, "SEMICOL is missing - Line 522");
					return false;
				}
			}
		}
		nestlevel--;
		return true;
	}
	
	
	nestlevel--;
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function

//Var:= ident
bool Var(istream& in, int& line, LexItem & idtok)
{
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}	
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream& in, int& line) {
	
	bool varstatus = false, status = false;
	LexItem t;
	LexItem i;

	t = Parser::GetNextToken(in, line);

	string varName = t.GetLexeme();
	
	if (defVar.find(varName) == defVar.end()) {
		cout << varName << endl;
		cout << line << endl;
		ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 588");
		return false;
	}

	i = LexItem(SymTable[varName], "", line);

	Parser::PushBackToken(t);
	
	varstatus = Var(in, line, i);
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP || t == ADDASSOP || t == SUBASSOP || t == MULASSOP || t == DIVASSOP || t == REMASSOP)
		{
			if (t == ADDASSOP || t == SUBASSOP || t == MULASSOP || t == DIVASSOP || t == REMASSOP) {
				if (TempsResults.find(varName) == TempsResults.end()) {
					ParseError(line, "Run-Time Error-Illegal Assignment Operation - 605");
					return false;
				}
			}

			// creates value object to determine value of expression
			Value exprVal = Value();
			status = Expr(in, line, exprVal);


			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			
			// verifies if var type matches with expression type
			switch (i.GetToken()) {
				case INT:
					if (!exprVal.IsInt()) {
						ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 624");
						return false;
					} else {
						switch (t.GetToken()) {
							case ASSOP:
								TempsResults[varName] = exprVal;
								break;
							case ADDASSOP:
								TempsResults[varName].SetInt(TempsResults[varName].GetInt() + exprVal.GetInt());
								break;
							case SUBASSOP:
								TempsResults[varName].SetInt(TempsResults[varName].GetInt() - exprVal.GetInt());
								break;
							case MULASSOP:
								TempsResults[varName].SetInt(TempsResults[varName].GetInt() * exprVal.GetInt());
								break;
							case DIVASSOP:
								TempsResults[varName].SetInt(TempsResults[varName].GetInt() / exprVal.GetInt());
								break;
							case REMASSOP:
								TempsResults[varName].SetInt(TempsResults[varName].GetInt() % exprVal.GetInt());
								break;
							default:
								ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 645");
								return false;
						}
					}
					break;
				case FLOAT:
					if (!exprVal.IsReal()) {
						ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 654");
						return false;
					} else {
						switch (t.GetToken()) {
							case ASSOP:
								TempsResults[varName] = exprVal;
								break;
							case ADDASSOP:
								TempsResults[varName].SetReal(TempsResults[varName].GetReal() + exprVal.GetReal());
								break;
							case SUBASSOP:
								TempsResults[varName].SetReal(TempsResults[varName].GetReal() - exprVal.GetReal());
								break;
							case MULASSOP:
								TempsResults[varName].SetReal(TempsResults[varName].GetReal() * exprVal.GetReal());
								break;
							case DIVASSOP:
								TempsResults[varName].SetReal(TempsResults[varName].GetReal() / exprVal.GetReal());
								break;
							default:
								ParseError(line, "Run-Time Error-Illegal Operator - 677");
								return false;
						}
					}
					break;
				case BOOL:
					if (!exprVal.IsBool()) {
						ParseError(line, "Run-Time Error-Illegal Assignment Operation - 681");
						return false;
					} else {
						switch (t.GetToken()) {
							case ASSOP:
								TempsResults[varName] = exprVal;
								break;
							default:
								ParseError(line, "Run-Time Error-Illegal Operator - 692");
								return false;
						}
					}
					break;
				case CHAR:
					if (exprVal.GetType() != VCHAR && exprVal.GetType() != VINT) {
						ParseError(line, "Run-Time Error-Illegal Assignment Operation - Line 696");
						return false;
					} else {
						switch (t.GetToken()) {
							case ASSOP:
								TempsResults[varName] = exprVal;
								break;
							case ADDASSOP:
								if (exprVal.IsChar()) {
									TempsResults[varName].SetChar(TempsResults[varName].GetChar() + exprVal.GetChar());
								} else if (exprVal.IsInt()) {
									int spare = TempsResults[varName].GetChar() + exprVal.GetInt();
									TempsResults[varName].SetType(VINT);
									TempsResults[varName].SetInt(spare);
									SymTable[varName] = INT;

								} else {
									ParseError(line, "Right hand operand is incompatible");
									return false;
								}
								break;
							case SUBASSOP:
								if (exprVal.IsChar()) {
									TempsResults[varName].SetChar(TempsResults[varName].GetChar() - exprVal.GetChar());
								} else if (exprVal.IsInt()) {
									int spare = TempsResults[varName].GetChar() - exprVal.GetInt();
									TempsResults[varName].SetType(VINT);
									TempsResults[varName].SetInt(spare);
									SymTable[varName] = INT;
									// TempsResults[varName].SetChar(TempsResults[varName].GetChar() - exprVal.GetInt());
								} else {
									ParseError(line, "Right hand operand is incompatible");
									return false;
								}
								break;
							case MULASSOP:
								if (exprVal.IsChar()) {
									TempsResults[varName].SetChar(TempsResults[varName].GetChar() * exprVal.GetChar());
								} else if (exprVal.IsInt()) {
									int spare = TempsResults[varName].GetChar() * exprVal.GetInt();
									TempsResults[varName].SetType(VINT);
									TempsResults[varName].SetInt(spare);
									SymTable[varName] = INT;
									// TempsResults[varName].SetChar(TempsResults[varName].GetChar() * exprVal.GetInt());
								} else {
									ParseError(line, "Right hand operand is incompatible");
									return false;
								}
								break;
							case DIVASSOP:
								if (exprVal.IsChar()) {
									TempsResults[varName].SetChar(TempsResults[varName].GetChar() / exprVal.GetChar());
								} else if (exprVal.IsInt()) {
									int spare = TempsResults[varName].GetChar() / exprVal.GetInt();
									TempsResults[varName].SetType(VINT);
									TempsResults[varName].SetInt(spare);
									SymTable[varName] = INT;
									// TempsResults[varName].SetChar(TempsResults[varName].GetChar() / exprVal.GetInt());
								} else {
									ParseError(line, "Right hand operand is incompatible");
									return false;
								}
								break;
							case REMASSOP:
								if (exprVal.IsChar()) {
									TempsResults[varName].SetChar(TempsResults[varName].GetChar() % exprVal.GetChar());
								} else if (exprVal.IsInt()) {
									int spare = TempsResults[varName].GetChar() % exprVal.GetInt();
									TempsResults[varName].SetType(VINT);
									TempsResults[varName].SetInt(spare);
									SymTable[varName] = INT;
									// TempsResults[varName].SetChar(TempsResults[varName].GetChar() % exprVal.GetInt());
								} else {
									ParseError(line, "Right hand operand is incompatible");
									return false;
								}
								break;
							default:
								ParseError(line, "Run-Time Error-Illegal Operator - 708");
								return false;
						}
					}
					break;
				case STRING:
					if (exprVal.GetType() != VSTRING) {
						ParseError(line, "Run-Time Error-Illegal Assignment Operation - 712");
						return false;
					}
					switch (t.GetToken()) {
						case ASSOP:
							TempsResults[varName] = exprVal;
							break;
						case ADDASSOP:
							TempsResults[varName].SetString(TempsResults[varName].GetString() + exprVal.GetString());
							break;
						default:
							ParseError(line, "Run-Time Error-Illegal Operator - 708");
							return false;
					}
					break;
				default:
					ParseError(line, "Runtime Error Line 715");
					return status;
			}

			// checks token of operator through switch statement
			// based on what the operator is, it updates the TempResults map to the correct value
			// added inside each else clause in the switch statement above
			/*switch (t.GetToken()) {
				case ASSOP:
					TempsResults[varName] = exprVal;
				case ADDASSOP:
					TempsResults[varName].set += exprVal;
				case SUBASSOP:
				case MULASSOP:
				case DIVASSOP:
				case REMASSOP:
				default:
			}*/
			


			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value ExprListOne = Value();
	status = Expr(in, line, ExprListOne);

	ValQue->push(ExprListOne);

	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
	
		status = ExprList(in, line);
		
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

//Expr ::= LogANDExpr { || LogANDRxpr }
bool Expr(istream& in, int& line, Value & retVal) {
	Value ExprOne = Value();
	LexItem tok;
	bool t1 = LogANDExpr(in, line, ExprOne);
		
	if( !t1 ) {
		return false;
	}

	retVal.SetType(ExprOne.GetType());

	switch (retVal.GetType()) {
		case VINT:
			retVal.SetInt(ExprOne.GetInt());
			break;
		case VREAL:
			retVal.SetReal(ExprOne.GetReal());
			break;
		case VCHAR:
			retVal.SetChar(ExprOne.GetChar());
			break;
		case VSTRING:
			retVal.SetString(ExprOne.GetString());
			break;
		case VBOOL:
			retVal.SetBool(ExprOne.GetBool());
			break;
		default:
			ParseError(line, "Error Value caught - Line 743");
			return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR ) 
	{
		Value ExprTwo = Value();
		t1 = LogANDExpr(in, line, ExprTwo);

		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		switch (ExprTwo.GetType()) {
			case VBOOL:
				if (retVal.GetType() != VBOOL) {
					ParseError(line, "Left operand is not compatible -> 766");
					return false;
				} else {
					retVal = retVal || ExprTwo;
				}
				break;
			default:
				ParseError(line, "Incompatible Value Caught Line 773");
				return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/LogORExpr

//LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream& in, int& line, Value & retVal) {
	LexItem tok;
	Value LogANDExprValOne = Value();
	bool t1 = EqualExpr(in, line, LogANDExprValOne);
		
	if( !t1 ) {
		return false;
	}

	retVal.SetType(LogANDExprValOne.GetType());

	switch (retVal.GetType()) {
		case VINT:
			retVal.SetInt(LogANDExprValOne.GetInt());
			break;
		case VREAL:
			retVal.SetReal(LogANDExprValOne.GetReal());
			break;
		case VCHAR:
			retVal.SetChar(LogANDExprValOne.GetChar());
			break;
		case VSTRING:
			retVal.SetString(LogANDExprValOne.GetString());
			break;
		case VBOOL:
			retVal.SetBool(LogANDExprValOne.GetBool());
			break;
		default:
			ParseError(line, "Error Value caught - Line 780");
			return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND ) 
	{
		Value LogANDExprValTwo = Value();
		t1 = EqualExpr(in, line, LogANDExprValTwo);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		switch (LogANDExprValTwo.GetType()) {
			case VBOOL:
				if (retVal.GetType() != VBOOL) {
					ParseError(line, "Left operand is not compatible -> 803");
					return false;
				} else {
					retVal = retVal && LogANDExprValTwo;
				}
				break;
			default:
				ParseError(line, "Incompatible VAlue Caught Line 810");
				return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	
	Parser::PushBackToken(tok);
	return true;
}//End of LogANDExpr

//EqualExpr ::= RelExpr [ (== | !=) RelExpr ]
bool EqualExpr(istream& in, int& line, Value & retVal) {
	Value EqualExprValOne = Value();
	LexItem tok;
	bool t1 = RelExpr(in, line, EqualExprValOne);
		
	if( !t1 ) {
		return false;
	}

	ValType EqualExprValOneType = EqualExprValOne.GetType();

	retVal.SetType(EqualExprValOneType);

	switch (EqualExprValOneType) {
		case VINT:
			retVal.SetInt(EqualExprValOne.GetInt());
			break;
		case VREAL:
			retVal.SetReal(EqualExprValOne.GetReal());
			break;
		case VCHAR:
			retVal.SetChar(EqualExprValOne.GetChar());
			break;
		case VBOOL:
			retVal.SetBool(EqualExprValOne.GetBool());
			break;
		case VSTRING:
			retVal.SetString(EqualExprValOne.GetString());
			break;
		default:
			ParseError(line, "Incompatible Operand - 959");
			return false;
	}
	
	tok = Parser::GetNextToken(in, line);
		
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok.GetToken() == EQ || tok.GetToken() == NEQ ) 
	{
		Value EqualExprValTwo = Value();
		t1 = RelExpr(in, line, EqualExprValTwo);
		
		ValType EqualExprValTwoType = EqualExprValTwo.GetType();
		
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok==EQ) {
			switch (EqualExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						// cout << retVal.GetInt() << endl;
						// cout << EqualExprValTwo.GetInt() << endl;
						retVal = Value(retVal.GetInt() == EqualExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() == EqualExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() == EqualExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() == EqualExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() == EqualExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() == retVal.GetReal());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				case VBOOL:
					if (retVal.GetType() == VBOOL) {
						retVal = Value(retVal.GetBool() == EqualExprValTwo.GetBool());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				case VSTRING:
					if (retVal.GetType() == VSTRING) {
						retVal = Value(retVal.GetString() == EqualExprValTwo.GetString());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				default:
					ParseError(line, "Incompatible operand for == operator");
					return false;
			}
		} else if (tok==NEQ) {
			switch (EqualExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() != EqualExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() != EqualExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() != EqualExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() != EqualExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() != EqualExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() != retVal.GetReal());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				case VBOOL:
					if (retVal.GetType() == VBOOL) {
						retVal = Value(retVal.GetBool() != EqualExprValTwo.GetBool());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				case VSTRING:
					if (retVal.GetType() == VSTRING) {
						retVal = Value(retVal.GetString() != EqualExprValTwo.GetString());
					} else {
						ParseError(line, "Incompatible right hand operand to left hand operand on == operator");
						return false;
					}
					break;
				default:
					ParseError(line, "Incompatible operand for == operator");
					return false;
			}
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == EQ || tok == NEQ)
		{
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
		else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	
	return true;
}//End of EqualExpr

//RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
	Value RelExprValOne = Value();
	LexItem tok;
	bool t1 = AddExpr(in, line, RelExprValOne);
	/*
	if (RelExprValOne.IsInt()) {
		cout << RelExprValOne.GetInt() << endl;
	}
	*/
		
	if( !t1 ) {
		return false;
	}

	ValType RelExprValOneType = RelExprValOne.GetType();

	retVal.SetType(RelExprValOneType);

	switch (RelExprValOneType) {
		case VINT:
			retVal.SetInt(RelExprValOne.GetInt());
			break;
		case VREAL:
			retVal.SetReal(RelExprValOne.GetReal());
			break;
		case VCHAR:
			retVal.SetChar(RelExprValOne.GetChar());
			break;
		case VBOOL:
			retVal.SetBool(RelExprValOne.GetBool());
			break;
		case VSTRING:
			retVal.SetString(RelExprValOne.GetString());
			break;
		default:
			ParseError(line, "Incompatible Operand - 1121");
			return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == LTHAN || tok == GTHAN) 
	{
		
		Value RelExprValTwo = Value();
		t1 = AddExpr(in, line, RelExprValTwo);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		ValType RelExprValTwoType = RelExprValTwo.GetType();

		if (tok==LTHAN) {
			switch (RelExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() < RelExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() < RelExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() < RelExprValTwo.GetInt());
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() < RelExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() < RelExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() < RelExprValTwo.GetReal());
					}
					break;
				case CHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() < RelExprValTwo.GetChar());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() < RelExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() < RelExprValTwo.GetChar());
					}
					break;
				default:
					ParseError(line, "Incompatible Operand - 1173");
					return false;
			}
		} else if (tok==GTHAN) {
			switch (RelExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() > RelExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() > RelExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() > RelExprValTwo.GetInt());
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() > RelExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() > RelExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() > RelExprValTwo.GetReal());
					}
					break;
				case CHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() > RelExprValTwo.GetChar());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() > RelExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() > RelExprValTwo.GetChar());
					}
					break;
				default:
					ParseError(line, "Incompatible Operand - 1206");
					return false;
			}
		}
		
		tok = Parser::GetNextToken(in, line);
		
		if(tok == LTHAN || tok == GTHAN)
		{
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
		else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream& in, int& line, Value & retVal) {
	Value AddExprValOne = Value();
	bool t1 = MultExpr(in, line, AddExprValOne);
	LexItem tok;

	/*
	if (AddExprValOne.IsInt()) {
		cout << AddExprValOne.GetInt() << endl;
	}
	*/
	
	if( !t1 ) {
		return false;
	}

	ValType AddExprValOneType = AddExprValOne.GetType();

	retVal.SetType(AddExprValOneType);
	switch (AddExprValOneType) {
		case VINT:
			retVal.SetInt(AddExprValOne.GetInt());
			break;
		case VREAL:
			retVal.SetReal(AddExprValOne.GetReal());
			break;
		case VCHAR:
			retVal.SetChar(AddExprValOne.GetChar());
			break;
		case VBOOL:
			retVal.SetBool(AddExprValOne.GetBool());
			break;
		case VSTRING:
			retVal.SetString(AddExprValOne.GetString());
			break;
		default:
			ParseError(line, "Incompatible Operand - 1253");
			return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS ) 
	{
		Value AddExprValTwo = Value();
		t1 = MultExpr(in, line, AddExprValTwo);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		ValType AddExprValTwoType = AddExprValTwo.GetType();

		if (tok==PLUS) {
			switch (AddExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() + AddExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() + AddExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() + AddExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible operator(s) - 1381");
						return false;
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() + AddExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() + AddExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() + AddExprValTwo.GetReal());
					} else {
						ParseError(line, "Incompatible operator(s) - 1393");
						return false;
					}
					break;
				case VCHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() + AddExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() + AddExprValTwo.GetChar());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() + AddExprValTwo.GetChar());
					} else if (retVal.GetType() == VSTRING) {
						retVal = Value(retVal.GetString() + AddExprValTwo.GetChar());
					} else {
						ParseError(line, "Incompatible operator(s) - 1405");
						return false;
					}
					break;
					// retVal.SetChar(UnaryExprValOne.GetChar());
				case VSTRING:
					if (retVal.IsString()) {
						retVal = Value(retVal.GetString() + AddExprValTwo.GetString());
					} else if (retVal.IsChar()) {
						retVal = Value(retVal.GetChar() + AddExprValTwo.GetString());
					} else {
						ParseError(line, "Incompatible operator(s) - 1416");
						return false;
					}
					break;
				default:
					ParseError(line, "Incompatible operator(s) - 1411");
					return false;
			}
		} else if (tok==MINUS) {
			switch (AddExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() - AddExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() - AddExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() - AddExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible operator(s) - 1424");
						return false;
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() - AddExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() - AddExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() - AddExprValTwo.GetReal());
					} else {
						ParseError(line, "Incompatible operator(s) - 1436");
						return false;
					}
					break;
				case VCHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() - AddExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() - AddExprValTwo.GetChar());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() - AddExprValTwo.GetChar());
					} else {
						ParseError(line, "Incompatible operator(s) - 1448");
						return false;
					}
					break;
					// retVal.SetChar(UnaryExprValOne.GetChar());
				default:
					ParseError(line, "Incompatible operator(s) - 1454");
					return false;
			}
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr

//MultExpr ::= UnaryExpr { ( * | / | %) UnaryExpr }
bool MultExpr(istream& in, int& line, Value & retVal) {
	Value UnaryExprValOne = Value();
	bool t1 = UnaryExpr(in, line, UnaryExprValOne);
	LexItem tok;

	/*
	if (UnaryExprValOne.IsInt()) {
		cout << UnaryExprValOne.GetInt() << endl;
	}
	*/
	
	if( !t1 ) {
		return false;
	}

	ValType UnaryExprValOneType = UnaryExprValOne.GetType();

	retVal.SetType(UnaryExprValOneType);
	switch (UnaryExprValOneType) {
		case VINT:
			retVal.SetInt(UnaryExprValOne.GetInt());
			break;
		case VREAL:
			retVal.SetReal(UnaryExprValOne.GetReal());
			break;
		case VCHAR:
			retVal.SetChar(UnaryExprValOne.GetChar());
			break;
		case VBOOL:
			retVal.SetBool(UnaryExprValOne.GetBool());
			break;
		case VSTRING:
			retVal.SetString(UnaryExprValOne.GetString());
			break;
		default:
			ParseError(line, "Incompatible operand - 1399");
			return false;
	}

	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  || tok == REM)
	{
		Value UnaryExprValTwo = Value();
		t1 = UnaryExpr(in, line, UnaryExprValTwo);
	
		if(!t1) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		ValType UnaryExprValTwoType = UnaryExprValTwo.GetType(); 

		if (tok==MULT) {
			switch (UnaryExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() * UnaryExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() * UnaryExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() * UnaryExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible operator(s) - 1534");
						return false;
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() * UnaryExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() * UnaryExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() * UnaryExprValTwo.GetReal());
					} else {
						ParseError(line, "Incompatible operator(s) - 1546");
						return false;
					}
					break;
				case VCHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() * UnaryExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() * UnaryExprValTwo.GetChar());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() * UnaryExprValTwo.GetChar());
					} else {
						ParseError(line, "Incompatible operator(s) - 1558");
						return false;
					}
					break;
					// retVal.SetChar(UnaryExprValOne.GetChar());
				default:
					ParseError(line, "Incompatible operator(s) - 1564");
					return false;
			}
		} else if (tok==DIV) {
			switch (UnaryExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() / UnaryExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() / UnaryExprValTwo.GetInt());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() / UnaryExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible operator(s) - 1577");
						return false;
					}
					break;
				case VREAL:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() / UnaryExprValTwo.GetReal());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() / UnaryExprValTwo.GetReal());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() / UnaryExprValTwo.GetReal());
					} else {
						ParseError(line, "Incompatible operator(s) - 1589");
						return false;
					}
					break;
				case VCHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() / UnaryExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() / UnaryExprValTwo.GetChar());
					} else if (retVal.GetType() == VREAL) {
						retVal = Value(retVal.GetReal() / UnaryExprValTwo.GetChar());
					} else {
						ParseError(line, "Incompatible operator(s) - 1601");
						return false;
					}
					break;
					// retVal.SetChar(UnaryExprValOne.GetChar());
				default:
					ParseError(line, "Incompatible operator(s) - 1607");
					return false;
			}
		} else if (tok==REM) {
			switch (UnaryExprValTwoType) {
				case VINT:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() % UnaryExprValTwo.GetInt());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() % UnaryExprValTwo.GetInt());
					} else {
						ParseError(line, "Incompatible operator(s) - 1618");
						return false;
					}
					break;
				case VCHAR:
					if (retVal.GetType() == VINT) {
						retVal = Value(retVal.GetInt() % UnaryExprValTwo.GetChar());
					} else if (retVal.GetType() == VCHAR) {
						retVal = Value(retVal.GetChar() % UnaryExprValTwo.GetChar());
					} else {
						ParseError(line, "Incompatible operator(s) - 1628");
						return false;
					}
					break;
					// retVal.SetChar(UnaryExprValOne.GetChar());
				default:
					ParseError(line, "Incompatible operator(s) - 1634");
					return false;
			}
		}


		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value & retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else if (t == NOT)
	{
		sign = 2;
	}
	else {
		Parser::PushBackToken(t);
	}
		
	status = PrimaryExpr(in, line, sign, retVal);

	/*
	if (retVal.IsInt()) {
		cout << retVal.GetInt() << endl;
	}
	*/

	return status;
}//End of UnaryExpr

//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value & retVal) {
	
	LexItem tok = Parser::GetNextToken(in, line);
	

	if( tok == IDENT ) {
	
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undeclared Variable");
			return false;	
		}
		if (TempsResults.find(lexeme) == TempsResults.end())
		{
			ParseError(line, "Using Uninitialized Variable");
			return false;	
		}
		// checks the type of identifier
		switch (SymTable[lexeme]) {
			case INT:
				if (sign==0 || sign==1) 
					retVal = Value(TempsResults[lexeme].GetInt());
				else if (sign == -1)
					retVal = Value(-1 * TempsResults[lexeme].GetInt());
				else {
					ParseError(line, "Unary Operator Incompatible");
					return false;
				}
				break;
			case FLOAT:
				if (sign==0 || sign==1) 
					retVal = Value(TempsResults[lexeme].GetReal());
				else if (sign == -1)
					retVal = Value(-1 * TempsResults[lexeme].GetReal());
				else {
					ParseError(line, "Unary Operator Incompatible");
					return false;
				}
				break;
			case CHAR:
				if (sign==0) 
					retVal = Value(TempsResults[lexeme].GetChar());
				else {
					ParseError(line, "Unary Operator Incompatible");
					return false;
				}
				break;
			case STRING:
				if (sign==0) 
					retVal = Value(TempsResults[lexeme].GetString());
				else {
					ParseError(line, "Unary Operator Incompatible");
					return false;
				}
				break;
			case BOOL:
				if (sign==0) 
					retVal = Value(TempsResults[lexeme].GetBool());
				else if (sign==2) 
					retVal = Value(!TempsResults[lexeme].GetBool());
				else {
					ParseError(line, "Unary Operator Incompatible");
					return false;
				}
				break;
			default:
				ParseError(line, "Run-Time Error");
				return false;
		}
		return true;
	}
	else if( tok == ICONST ) {
		if (sign == 0||sign == 1) 
			retVal = Value(stoi(tok.GetLexeme()));
		else if (sign == -1)
			retVal = Value(-1 * stoi(tok.GetLexeme()));
		else if (sign == 2) {
			ParseError(line, "Line Error 1646");
			return false;
		}
		return true;
	}
	else if( tok == SCONST ) {
		if (sign==0) 
			retVal = Value(tok.GetLexeme());
		else {
			ParseError(line, "Unary Operator Incompatible");
			return false;
		}
		return true;
	}
	else if( tok == RCONST ) {
		if (sign==0 || sign==1) 
			retVal = Value(stof(tok.GetLexeme()));
		else if (sign == -1)
			retVal = Value(-1 * stof(tok.GetLexeme()));
		else {
			ParseError(line, "Unary Operator Incompatible");
			return false;
		}
		return true;
	}
	else if( tok == BCONST ) {
		if (tok.GetLexeme().compare("true") == 0) {
			retVal= Value(true);
		} else {
			retVal = Value(false);
		}
		return true;
	}
	else if ( tok == CCONST )
	{
		if (sign==0) 
			retVal = Value(tok.GetLexeme()[0]);
		else {
			ParseError(line, "Unary Operator Incompatible");
			return false;
		}
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN ) {
			return ex;
		} else {
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}//End of PrimaryExpr


