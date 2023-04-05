#ifndef EXPR_CALC_H
#define EXPR_CALC_H

#define _USE_MATH_DEFINES
#include<math.h>

#include"getOut.h"
#include"vector_c.h"

typedef vector_v cStack;
typedef vector_v cStore;
typedef vector_v cVar;

struct expr;
typedef struct expr expression;

typedef void operation(cStack*, int);
typedef void stackOperation(cStack*, expression, int);
typedef void solveMode(expression exp);

struct expr {
	stackOperation* stackOp;
	operation* op;
	double data;
};

//operations
void eConstCalc(cStack* calcStack, int ch)
{
	double cConst = M_E;
	insertValue(calcStack, double, &cConst);
}

void piConstCalc(cStack* calcStack, int ch)
{
	double cConst = M_PI;
	insertValue(calcStack, double, &cConst);
}

void addCalc(cStack* calcStack, int ch)
{
	double operator1, operator2;

	if(getLastValue(calcStack, double, &operator2) != 0)
		return;
	if(getLastValue(calcStack, double, &operator1) != 0)
		return;

	double result = operator1 + operator2;
	insertValue(calcStack, double, &result);
}

void mulCalc(cStack* calcStack, int ch)
{
	double operator1, operator2;

	if(getLastValue(calcStack, double, &operator2) != 0)
		return;
	if(getLastValue(calcStack, double, &operator1) != 0)
		return;

	double result = operator1 * operator2;
	insertValue(calcStack, double, &result);
}

void divCalc(cStack* calcStack, int ch)
{
	double operator1, operator2;

	if(getLastValue(calcStack, double, &operator2) != 0)
		return;
	if(getLastValue(calcStack, double, &operator1) != 0)
		return;

	double result = operator1 / operator2;
	insertValue(calcStack, double, &result);
}

void powCalc(cStack* calcStack, int ch)
{
	double operator1, operator2;

	if(getLastValue(calcStack, double, &operator2) != 0)
		return;
	if(getLastValue(calcStack, double, &operator1) != 0)
		return;

	double result = pow(operator1, operator2);
	insertValue(calcStack, double, &result);
}

void absCalc(cStack* calcStack, int ch)
{
	double operatorC;

	if(getLastValue(calcStack, double, &operatorC) != 0)
		return;

	double result = fabs(operatorC);
	insertValue(calcStack, double, &result);
}

void variableCalc(cStack* calcStack, int ch)
{
	return;
}

void emptyCalc(cStack* calcStack, int ch)
{
	return;
}

//expression stack
void callProcStackCalc(cStack* calcStack, expression exp, int ch)
{
	(*exp.op)(calcStack, ch);
}

void insertNumStackCalc(cStack* calcStack, expression exp, int ch)
{
	insertValue(calcStack, double, &exp.data);
}

void insertMarkedNumStackCalc(cStack* calcStack, expression exp, int ch)
{
	insertValue(calcStack, double, &exp.data);
}

//expression stack
#define initStackExp(pCStack) ((vector_v*)pCStack)->reserve = 1; ((vector_v*)pCStack)->size = 0; ((vector_v*)pCStack)->buffer = malloc(1);
#define insertStackExp(pCStack, pValue) insertVoid(pCStack, sizeof(expression), pValue)
#define insertIndex(pCVar, pValue) insertVoid(pCVar, sizeof(size_t), pValue)
#define getStackExpAt(pCStack, index, pExpression) getVoidAt(pCStack, sizeof(expression), index, pExpression)
#define getIndexAt(pCVar, index, pSize) getVoidAt(pCVar, sizeof(size_t), index, pSize)
#define destroyCStack(pCStack) destroyVector(pCStack)
#define destroyCStore(pCStore) destroyVector(pCStore)
#define destroyCVar(pCVar) destroyVector(pCVar)

void realMode(expression exp)
{
	return;
}

void debugMode(expression exp)
{
	printf("operation: %s %f\n", exp.stackOp == insertMarkedNumStackCalc ? "insert variable" :
				exp.stackOp == insertNumStackCalc ? "insert value" :
				exp.stackOp != callProcStackCalc ? "invalid function" :
				exp.op == eConstCalc ? "insert e" :
				exp.op == piConstCalc ? "insert pi" :
				exp.op == addCalc ? "add(2)" :
				exp.op == mulCalc ? "mul(2)" :
				exp.op == divCalc ? "div(2)" :
				exp.op == powCalc ? "pow(2)" :
				exp.op == absCalc ? "abs(1)" :
				exp.op == variableCalc ? "insert mark" :
				exp.op == emptyCalc ? "nop" : "invalid operation", exp.data);
}

cStack processExpression(cStore expStack, solveMode* mode)
{
	size_t stackSize = expStack.size/sizeof(expression);
	cStack result;
	initVoidVector(&result);

	for(size_t i = 0; i < stackSize; ++i)
	{
		expression exp;
		if(getStackExpAt(&expStack, i, &exp) != 0)
			return NULL_v;
		(*exp.stackOp)(&result, exp, 0);

		if(mode != NULL)
			(*mode)(exp);
	}

	return result;
}

double solveExpression(cStore expStack, solveMode* mode)
{
	cStack resultStack = processExpression(expStack, mode);
	double result;
	getLastValue(&resultStack, double, &result);
	destroyCStack(&resultStack);

	return result;
}

cVar getVariablesIndex(cStore expStack)
{
	size_t stackSize = expStack.size/sizeof(expression);
	cVar result;
	initVoidVector(&result);

	for(size_t i = 0; i < stackSize; ++i)
	{
		expression exp;
		if(getStackExpAt(&expStack, i, &exp) != 0)
			return NULL_v;

		if(exp.stackOp == insertMarkedNumStackCalc)
			insertIndex(&result, &i);
	}

	return result;
}

void changeVariableValue(cStore* expStack, cVar varStack, size_t var, double value)
{
	expression exp;
	exp.stackOp = insertMarkedNumStackCalc;
	exp.op = NULL;
	exp.data = value;

	size_t index;
	getIndexAt(&varStack, var, &index);
	setVoidAt(expStack, sizeof(expression), index, &exp);
}

//getOut.h procs
int cStackNumberInterval(int ch, void** extraData)
{
	if((ch >= '0' && ch <= '9') || ch == '.' || ch == ',' || ch == '-' || ch == 'E')
	{
		*extraData = NULL;
		return ~0;
	}

	switch(ch)
	{
	case '+':
		*extraData = &addCalc;
		break;
	case '*':
		*extraData = &mulCalc;
		break;
	case '/':
	case ':':
		*extraData = &divCalc;
		break;
	case '^':
		*extraData = &powCalc;
		break;
	case 'a':
	case 'A':
		*extraData = &absCalc;
		break;
	case 'e':
		*extraData = &eConstCalc;
		break;
	case 'p':
		*extraData = &piConstCalc;
		break;
	case 'x':
	case 'X':
		*extraData = &variableCalc;
		break;
	case ' ':
	case '\t':
		*extraData = &emptyCalc;
		break;
	default:
		return 0;
	}

	return ~0;
}

int cStackInverseNumberInterval(int ch, void** extraData)
{
	if(ch == '\n')
		return 0;
	return ~cStackNumberInterval(ch, extraData);
}

void* cStackProcessing(int ch, void** data, void** extra)
{
	if(*data == NULL)
	{
		*data = malloc(sizeof(cStore));
		if(*data == NULL)
			return NULL;

		initVoidVector(*data);
		if(((cStack*)(*data))->buffer == NULL)
			return NULL;
	}

	operation* proc = *extra;
	if(proc == NULL)
	{
		int lastChar = ch;
		int error;

		expression exp;
		exp.stackOp = insertNumStackCalc;
		exp.op = NULL;
		exp.data = get_double(&lastChar, &error);
		if(error != 0)
			return NULL;

		if(insertValue((cStore*)(*data), expression, &exp) == NULL)
			return NULL;
	}
	else if(proc == &variableCalc)
	{
		expression exp;
		exp.stackOp = insertMarkedNumStackCalc;
		exp.op = NULL;
		exp.data = 0;

		if(insertValue((cStore*)(*data), expression, &exp) == NULL)
			return NULL;
	}
	else
	{
		expression exp;
		exp.stackOp = callProcStackCalc;
		exp.op = proc;
		exp.data = 0;

		if(insertValue((cStore*)(*data), expression, &exp) == NULL)
			return NULL;
	}

	return *data;
}

//getOut.h wrapper
#define get_calc(pIntCh, pIntError) get_cStack(pIntCh, pIntError)
getInput(cStack)
#endif
