/* Implementation of Recursive-Descent Parser
 * parserInt.cpp
 * Programming Assignment 3
 * Spring 2023
*/

#include "parserInt.h"

map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value>* ValQue; //declare a pointer variable to a queue of Value objects

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if (pushed_back) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem& t) {
		if (pushed_back) {
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
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}

//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {

		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex) {
		ParseError(line, "Missing expression list after Print");
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}

	//Evaluate: writeln by printing out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {

		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}
	return true;
}//End of WritelnStmt


//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal;

	status = Expr(in, line, retVal);
	if (!status) {
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA) {
		status = ExprList(in, line);
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else {
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList


bool IdentList(istream& in, int& line);


//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line)
{
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);


	if (tok.GetToken() == DONE && tok.GetLinenum() <= 1) {
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line);

	// Error somewhere in the program
	if (!f1)
	{
		ParseError(line, "Missing Program");
		return false;
	}
	else
		return true;
}//End of Prog



//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line) {
	bool status;

	LexItem tok;

	// Will keep running statements until EOF or error is found
	status = Stmt(in, line);
	while (status)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok == DONE)
		{
			cout << "\n(" << "DONE" << ")" << endl;
			Parser::PushBackToken(tok);
			return true;
		}
		else if (tok == RBRACES)
		{
			Parser::PushBackToken(tok);
			return true;
		}

		if (tok != SEMICOL)
		{
			cout << "Error:" << tok.GetLexeme();
			tok = Parser::GetNextToken(in, line);
			cout << "\nNext token:" << tok.GetLexeme();
			Parser::PushBackToken(tok);


			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		status = Stmt(in, line);

	}

	if (!status)
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}


	tok = Parser::GetNextToken(in, line);
	if (tok == ELSE)
	{
		ParseError(line, "Missing right brace.");
		return false;
	}
	else if (tok == RBRACES)
	{
		Parser::PushBackToken(tok);
		return true;
	}

	else
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}

}//End of StmtList function

//Stmt ::= AssignStme | WriteLnStmt | IfStmt  
bool Stmt(istream& in, int& line) {
	bool status = false;

	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken()) {
	case SIDENT: case NIDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect Assignment Statement.");
			return status;
		}
		break;
	case WRITELN:

		status = WritelnStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect Writeln Statement.");
			return status;
		}
		break;
	case IF:
		status = IfStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect If-Statement.");
			return status;
		}
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	case IDENT:
		ParseError(line, "Invalid variable name");

		Parser::PushBackToken(t);
		return false;
		break;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function



//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status;
	LexItem t;
	Value cv;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN) {

		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}

	ex = Expr(in, line, cv);

	if (!ex)
	{
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}

	if (!(cv.GetType() == VBOOL))
	{
		ParseError(line, "Illegal Type for If statement condition.");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {

		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != LBRACES)
	{
		ParseError(line, "If Statement Syntax Error: Missing left brace");
		return false;
	}

	if (cv.GetBool()) // Bool value was true, first StmtList will be executed
	{
		status = StmtList(in, line);
	}
	else // Bool value was false, first StmtList will be ignored
	{
		t = Parser::GetNextToken(in, line);
		while (!(t.GetToken() == RBRACES))
		{
			t = Parser::GetNextToken(in, line);
		}

		// Pushing back rbraces token
		Parser::PushBackToken(t);
		status = true;
	}

	if (!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Clause");
		return false;
	}

	t = Parser::GetNextToken(in, line);

	if (t != RBRACES)
	{
		ParseError(line, "If Statement Syntax Error: Missing right brace.");
		return false;
	}

	t = Parser::GetNextToken(in, line);

	if (t == ELSE)
	{
		t = Parser::GetNextToken(in, line);
		if (t != LBRACES)
		{
			ParseError(line, "If Statement Syntax Error: Missing left brace");
			return false;
		}

		if (!(cv.GetBool()))  // Bool value was false, second StmtList will be executed
		{
			status = StmtList(in, line);
		}
		else  // Bool value was false, second StmtList will be executed
		{
			t = Parser::GetNextToken(in, line);
			while (!(t.GetToken() == RBRACES))
			{
				t = Parser::GetNextToken(in, line);
			}

			// Pushing back rbraces token
			Parser::PushBackToken(t);
			status = true;
		}

		if (!status)
		{
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}

		t = Parser::GetNextToken(in, line);

		if (t != RBRACES)
		{
			Parser::PushBackToken(t);
			ParseError(line, "If Statement Syntax Error: Missing right brace.");
			return false;
		}
		//Parser::PushBackToken(t);
	}
	else
	{
		Parser::PushBackToken(t);
		return true;
	}
	return true;
}//End of IfStmt function

//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line, LexItem& idtok)
{
	string identstr;

	// Language definiton only accepts NIDENT or SIDENT. Anything else is an error
	if (idtok == NIDENT || idtok == SIDENT)
	{
		identstr = idtok.GetLexeme();

		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
		}
		return true;
	}
	else if (idtok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");

		return false;
	}

	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line)
{
	bool varstatus = false, status = false;
	string message;
	LexItem t;
	//Value someValue;
    Value V;

	// Checks variable
	LexItem to = Parser::GetNextToken(in, line);

	// Calls var function
	varstatus = Var(in, line, to);

	// Left hand variable was valid
	if (varstatus)
	{
		t = Parser::GetNextToken(in, line);

		if (t == ASSOP)
		{
			status = Expr(in, line, V);

			if (!status)
			{
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}


			// PERFORMING THE ASSIGNMENT AND UPDATING THE TempsResults map
			if ((SymTable[to.GetLexeme()] == NIDENT) && (V.GetType() == VREAL))
			{
				TempsResults[to.GetLexeme()] = V.GetReal();
			}
			else if ((SymTable[to.GetLexeme()] == SIDENT) && (V.GetType() == VSTRING))
			{
				TempsResults[to.GetLexeme()] = V.GetString();
			}
			else if ((SymTable[to.GetLexeme()] == SIDENT) && (V.GetType() == VREAL))
			{
				//Convert real to string
				string NS = to_string(V.GetReal());    //Convert real to string
				int index = NS.find('.');

				// Edit string to have no decimal or 1 precision
				if (NS[index + 1] == '0')
				{
					NS = NS.substr(0, index);
				}
				else if (index != std::string::npos)
				{
					NS = NS.substr(0, index + 2);
				}

				TempsResults[to.GetLexeme()] = NS;
			}
			else if ((SymTable[to.GetLexeme()] == NIDENT) && (V.GetType() == VSTRING))
			{
				ParseError(line, "Invalid assignment statement conversion of a string value to a double variable.");
				return false;
			}
			else if (V.GetType() == VBOOL)
			{
				ParseError(line, "Illegal Assignment of a boolean value to a numeric or string variable.");
				return false;
			}
		}
		else if (t.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else
		{
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
}//End of AssignStmt


//Expr ::= EqualExpr ::= RelExpr [(-EQ|==) RelExpr ]
bool Expr(istream& in, int& line, Value& retVal)
{
	LexItem tok;
	Value lv;
    
    Value rv;

	bool t1 = RelExpr(in, line, lv);

	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	if (tok == NEQ || tok == SEQ)
	{
		t1 = RelExpr(in, line, rv);

		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		// Convert left and right values to either string or numeric depending on operator
		if (tok == NEQ) // Converting to numeric
		{
			if (lv.GetType() == VREAL && rv.GetType() == VREAL)
			{
				lv.SetBool((lv == rv).GetBool());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation Expr.");
				return false;
			}
		}
		else if (tok == SEQ) // Converting to string
		{
			if (lv.GetType() == VSTRING && rv.GetType() == VSTRING)
			{
				lv.SetBool((lv.SEqual(rv)).GetBool());
			}
			else
			{
				/*lv.SetType(VSTRING);
				rv.SetType(VSTRING);
				cout << "Leftval: " << leftVal.GetString();
				cout << "RightVal: " << rightVal.GetString();*/
				ParseError(line, "Illegal operand type for the operation1.");
				return false;
			}
		}

		// Relational Expression was found so value to be returned will be boolean
		lv.SetType(VBOOL);

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}

	// UPDATING retVal 
	retVal.SetType(lv.GetType());
    
	if (lv.GetType() == VREAL)
    {
		retVal.SetReal(lv.GetReal());
    }
    
	if (lv.GetType() == VSTRING)
    {
		retVal.SetString(lv.GetString());
    }
    
	if (lv.GetType() == VBOOL)
		retVal.SetBool(lv.GetBool());


	Parser::PushBackToken(tok);

	return true;
}//End of Expr/EqualExpr

//RelExpr ::= AddExpr [ ( -LT | -GT | < | > )  AddExpr ]
bool RelExpr(istream& in, int& line, Value& retVal)
{
	LexItem tok;
	Value lv, rv;

	bool t1 = AddExpr(in, line, lv);

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == NGTHAN || tok == NLTHAN || tok == SGTHAN || tok == SLTHAN)
	{
        //Value rl;
		t1 = AddExpr(in, line, rv);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}


		// Convert left and right values to either string or numeric depending on operator
		if (tok == NGTHAN) // Converting to numeric
		{
			if (lv.GetType() == VREAL && rv.GetType() == VREAL)
			{
				lv.SetBool((lv > rv).GetBool());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation NG.");
				return false;
			}
		}
		else if (tok == NLTHAN) // Converting to numeric
		{
			if (lv.GetType() == VREAL && rv.GetType() == VREAL)
			{
				lv.SetBool((lv < rv).GetBool());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation NL.");
				return false;
			}
		}
		else if (tok == SGTHAN) // Converting to string
		{
			lv.SetBool((lv.SGthan(rv)).GetBool());
		}
		else if (tok == SLTHAN) // Converting to string
		{
			lv.SetBool((lv.SLthan(rv)).GetBool());
		}


		// Relational Expression was found so value to be returned will be boolean
		lv.SetType(VBOOL);

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}

	// UPDATING retVal 
	retVal.SetType(lv.GetType());
    
	if (lv.GetType() == VREAL)
    {
		retVal.SetReal(lv.GetReal());
    }
    
	if (lv.GetType() == VSTRING)
    {
		retVal.SetString(lv.GetString());
    }
    
	if (lv.GetType() == VBOOL)
		retVal.SetBool(lv.GetBool());
    

	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr







//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value& retVal)
{
	Value v1;
    //Value v2;

	bool t1 = MultExpr(in, line, v1);
	LexItem tok;

	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	while (tok == PLUS || tok == MINUS || tok == CAT)
	{
		Value v2;

		t1 = MultExpr(in, line, v2);

		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}


		// Convert left and right values to either string or numeric depending on operator
		if (tok == PLUS) // Converting to numeric
		{
			if (v1.GetType() == VREAL && v2.GetType() == VREAL)
			{
				v1.SetReal((v1 + v2).GetReal());
				v1.SetType(VREAL);
			}
			else
			{
				/*if (leftVal.GetType() == VREAL)
					cout << "left is good";
				if (rightVal.GetType() == VREAL)
					cout << "right is good";*/

				ParseError(line, "Illegal operand type for the operation6.");
				return false;
			}

		}
		else if (tok == MINUS) // Converting to numeric
		{
			if (v1.GetType() == VREAL &&v2.GetType() == VREAL)
			{
				v1.SetReal((v1 - v2).GetReal());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation7.");
				return false;
			}
		}
		else if (tok == CAT) // Converting to numeric
		{
			if (v1.GetType() == VSTRING && v2.GetType() == VSTRING)
			{
				v1.SetString((v1.Catenate(v2)).GetString());
				v1.SetType(VSTRING);
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation8.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}

	// UPDATING retVal 
	retVal.SetType(v1.GetType());
    
	if (v1.GetType() == VREAL)
    {
		retVal.SetReal(v1.GetReal());
    }
    
	if (v1.GetType() == VSTRING)
    {
		retVal.SetString(v1.GetString());
    }


	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line, Value& retVal)
{
	Value v1;
    //Value v2;

	bool t1 = ExponExpr(in, line, v1);
	LexItem tok;

	if (!t1) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	while (tok == MULT || tok == DIV || tok == SREPEAT)
	{
		Value v2;
		t1 = ExponExpr(in, line, v2);

		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		// Convert left and right values to either string or numeric depending on operator
		if (tok == MULT) // Converting to numeric
		{
			if (v1.GetType() == VREAL && v2.GetType() == VREAL)
			{
				v1.SetReal((v1 * v2).GetReal());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation Mult.");
				return false;
			}
		}
		else if (tok == DIV) // Converting to numeric
		{
			if (v1.GetType() == VREAL && v2.GetType() == VREAL)
			{
				if (int(v2.GetReal()) == 0)
				{
					ParseError(line, "Run-Time Error-Illegal Division by Zero");
					return false;
				}

				v1.SetReal((v1/ v2).GetReal());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation Div.");
				return false;
			}
		}
		else if (tok == SREPEAT) // Converting to numeric
		{
			if (v1.GetType() == VSTRING && v2.GetType() == VREAL)
			{
				v1.SetString((v1.Repeat(v2)).GetString());
			}
			else
			{
				ParseError(line, "Illegal operand type for the operation11.");
				return false;
			}
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

	}


	// UPDATING retVal 
	
    retVal.SetType(v1.GetType());
    
	if (v1.GetType() == VREAL)
    {
		retVal.SetReal(v1.GetReal());
    }
    
	if (v1.GetType() == VSTRING)
    {
		retVal.SetString(v1.GetString());
    }

	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, Value& retVal)
{
	bool status;
	Value v1;

	status = UnaryExpr(in, line, v1);
	if (!status) {
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT)
	{
		Value v2;

		status = ExponExpr(in, line, v2);

		if (!status) {
			ParseError(line, "Missing operand after operator Exponent");
			return false;
		}

		if (v1.GetType() == VREAL && v2.GetType() == VREAL)
		{
			retVal.SetType(VREAL);
			retVal.SetReal((v1 ^ v2).GetReal());
		}
		else
		{
			ParseError(line, "Illegal exponentiation operation.");
			return false;
		}



		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR) {
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		Parser::PushBackToken(tok);
		return true;
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	// UPDATING retVal 
	retVal.SetType(v1.GetType());
    
	if (v1.GetType() == VREAL)
    {
		retVal.SetReal(v1.GetReal());
    }
    
	if (v1.GetType() == VSTRING)
    {
		retVal.SetString(v1.GetString());
    }

	Parser::PushBackToken(tok);
	return true;
}//End of ExponExpr

//UnaryExpr ::= ( - | + ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	Value condVal;

	if (t == MINUS)
	{
		sign = -1;
	}
	else if (t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);

	status = PrimaryExpr(in, line, sign, condVal);

	if (!status)
	{
		return false;
	}


	if (sign != 0)
	{
		if ( condVal.GetType() == VBOOL|| condVal.GetType() == VERR || condVal.GetType() == VSTRING)
		{
			ParseError(line, "Illegal Operand Type for Sign Operator Unary");
			return false;
		}

		retVal.SetReal(sign * condVal.GetReal());
		retVal.SetType(VREAL);
	}
	else if (sign == 0)
	{
		// UPDATING retVal 
		retVal.SetType(condVal.GetType());
		if (condVal.GetType() == VREAL)
			retVal.SetReal(condVal.GetReal());
		if (condVal.GetType() == VSTRING)
			retVal.SetString(condVal.GetString());
	}

	return status;
}//End of UnaryExpr


//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value& retVal)
{
	//Value exprValue;
    Value fval;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == NIDENT || tok == SIDENT)
	{

		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable");
			return false;
		}

		if (tok == NIDENT)
		{
			retVal.SetType(VREAL);
			retVal.SetReal(TempsResults.find(tok.GetLexeme())->second.GetReal());
		}
		if (tok == SIDENT)
		{
			retVal.SetType(VSTRING);
			retVal.SetString(TempsResults.find(tok.GetLexeme())->second.GetString());
		}

		return true;
	}
	else if (tok == ICONST)
	{
		double primval = stod(tok.GetLexeme());
		retVal.SetType(VREAL);
		retVal.SetReal(primval);

		return true;
	}
	else if (tok == SCONST)
	{
		string primval = tok.GetLexeme();
		retVal.SetType(VSTRING);
		retVal.SetString(primval);

		return true;
	}
	else if (tok == RCONST)
	{
		double primval = stod(tok.GetLexeme());
		retVal.SetType(VREAL);
		retVal.SetReal(primval);

		return true;
	}
	else if (tok == LPAREN)
	{

		bool ex = Expr(in, line, fval);

		if (!ex) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}

		if (fval.GetType() == VBOOL)
		{
			retVal.SetType(VBOOL);
			retVal.SetBool(fval.GetBool());
		}
		if (fval.GetType() == VREAL)
		{
			retVal.SetType(VREAL);
			retVal.SetReal(fval.GetReal());
		}
		if (fval.GetType() == VSTRING)
		{
			retVal.SetType(VSTRING);
			retVal.SetString(fval.GetString());
		}

		if (Parser::GetNextToken(in, line) == RPAREN)
		{
			return ex;
		}
		else
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if (tok.GetToken() == ERR) {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}


