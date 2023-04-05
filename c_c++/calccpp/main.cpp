#include<iostream>
#include<math.h>

#define ABS(x) ((x) > -(x) ? (x) : -(x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

template <typename T> class vector_v {
    public:
		vector_v()
        {
          	size = 0;
        	res = 0;
            data = NULL;
        }

        ~vector_v()
        {
            free(data);
        }

  		bool reserve(size_t reserve)
        {
          	res = reserve;
          	T* temp = (T*)realloc(data, reserve*sizeof(T));
          	if(temp == NULL)
              	return false;
          	data = temp;
          	return true;
        }

  		virtual bool insertData(T data)
        {
          	if(size >= res)
            {
              	if(!reserve(res << 1))
                  return false;
            }

            this->data[size++] = data;

          	return true;
        }

  		T removeData(void)
        {
          	return data[--size];
        }

        T peekData(void)
        {
            return data[size-1];
        }

  		void resize(size_t size)
        {
          	this->size = MIN(this->size, size);
        }

        size_t getSize(void)
        {
            return size;
        }

  	protected:
  		T* data;
		size_t size;
		size_t res;
};

class equation_v : public vector_v<char> {
	public:
		equation_v() : vector_v(){}

  		void fail(void)
        {
            std::cout << (intptr_t)this << " MALLOC FAIL" << std::endl;
            while(1);
        }

        //validações: zero á esquerda é proibido, duas operações binárias seguidas, operação unária e em seguida operação binária, '.' sem número à esquerda ou direita.
        bool insertData(char data)
        {
            if(size >= res)
            {
              	if(!reserve(res << 1))
                  return false;
            }

            //inserção
            this->data[size++] = data;

            std::cout << res << std::endl;

            return true;
        }

        const char* getString(void)
        {
            return (const char*)data;
        }

  		void printString(void)
        {
            data[size] = '\0';
            std::cout << data << std::endl;
        }
};

typedef double lfFunclflf(double, double);
typedef double lfFunclf(double);

double sum(double b, double a) {return a+b;}
double sub(double b, double a) {return a-b;}
double mul(double b, double a) {return a*b;}
double div(double b, double a) {return a/b;}
double rt(double b, double a) {return pow(b, 1.0/a);}
double lg(double b, double a) {return log(b)/log(a);}
double par(double, double) {return 0;}

typedef struct {
    char op;
    bool unaryOperation;
    lfFunclflf* proc;
    int precedence;
} operation;

const operation operations[] = {    {')', false, &par, INT_MIN},
                                    {'(', false, &par, INT_MIN},
                                    {'+', false, &sum, 1},
                                    {'-', false, &sub, 1},
                                    {'x', false, &mul, 2},
                                    {'/', false, &div, 2},
                                    {'^', false, &pow, -3},
                                    {'V', false, &rt, -3},
                                    {'l', false, &lg, -4},
                                    {'n', true, (lfFunclflf*)(lfFunclf*)(&log), -4},
                                    {'x', true, (lfFunclflf*)(lfFunclf*)(&exp), -4},
                                    {'s', true, (lfFunclflf*)(lfFunclf*)(&sin), -4},
                                    {'c', true, (lfFunclflf*)(lfFunclf*)(&cos), -4},
                                    {'t', true, (lfFunclflf*)(lfFunclf*)(&tan), -4},    };

class reversePolish {
    public:
        reversePolish(){}

        //os números são colocados diretamente na saída, as operações são colocadas em uma pilha
        //até que se encontre uma operação com precedencia igual ou menor que a operação anterior
        //nesse caso se retira a que está na pilha e coloca na saída e insere a última operação lida
        bool parse(equation_v& equation)
        {
            char op;
            double value;
            vector_v<operation> stackOP;
            vector_v<double> numberStack;
            numberStack.reserve(4);
            stackOP.reserve(4);
            //-1 por começar a contagem em 0 e -1 pelo '\0' = -2
            for(const char* str = equation.getString(); str <  equation.getString() + equation.getSize();)
            {
                //std::cout << " enter ";
                int chars = sscanf(str, "%lf", &value);
                if(chars != 0 && chars != EOF)
                {
                    numberStack.insertData(value);
                    std::cout << value << ' ';

                    while((*str >= '0' && *str <= '9') || *str == '.') ++str;
                }
                //std::cout << " enter ";
                chars = sscanf(str, "%c", &op);
                if(chars != 0 && chars != EOF)
                {
                    if(op != '(')
                    {
                        while(stackOP.getSize() != 0 &&  //testa se pilha de operações está vazio
                            ((op == ')' && stackOP.peekData().op != '(') ||
                            (ABS(stackOP.peekData().precedence) > ABS(convertToOperation(op)->precedence)) ||     //testa se precedencia da operação no topo da pilha é maior que a precedencia da operação
                            (ABS(stackOP.peekData().precedence) == ABS(convertToOperation(op)->precedence) && convertToOperation(op)->precedence >= 0))) //testa se precedencia da operação no topo da pilha é igual a precedencia da operação e a associação é da esquerda para direita.
                        {
                            operation t = stackOP.removeData();
                            std::cout << t.op << ' ';
                            if(t.unaryOperation == false)
                                numberStack.insertData(t.proc(numberStack.removeData(), numberStack.removeData()));
                            else
                                numberStack.insertData(((lfFunclf*)(t.proc))(numberStack.removeData()));
                            std::cout << "resultado parcial= " << numberStack.peekData() << ' ';
                        }

                        if(op == ')')
                        {
                            stackOP.removeData();
                        }
                    }

                    if(op != ')')
                        stackOP.insertData(*convertToOperation(op));

                    ++str;
                }
            }
            while(stackOP.getSize() != 0)
            {
                operation t = stackOP.removeData();
                std::cout << t.op << ' ';
                if(t.unaryOperation == false)
                    numberStack.insertData(t.proc(numberStack.removeData(), numberStack.removeData()));
                else
                    numberStack.insertData(((lfFunclf*)(t.proc))(numberStack.removeData()));
                std::cout << "resultado parcial= " << numberStack.peekData() << ' ';
            }
            std::cout << std::endl << "result: " << numberStack.removeData() << std::endl;

            return true;
        }
    private:

        const operation* convertToOperation(char ch)
        {
            for(size_t i = 0; i < sizeof(operations)/sizeof(operation); ++i)
            {
                if(ch == operations[i].op)
                    return &(operations[i]);
            }

            return NULL;
        }
};

equation_v equ;

int main(void)
{
    equ.reserve(4);
    equ.insertData('7');
    //equ.insertData('0');
    equ.insertData('+');
    equ.insertData('7');
    /*equ.insertData('3');
    equ.insertData('4');
    equ.insertData('+');
    //equ.insertData('0');
    equ.insertData('4');
    equ.insertData('3');
    equ.insertData('2');
    equ.insertData('1');
    equ.insertData(')');
    equ.insertData('x');
    equ.insertData('(');
    equ.insertData('.');
    equ.insertData('5');
    equ.insertData('8');
    //equ.insertData('/');
    equ.insertData('+');
    equ.insertData('7');
    equ.insertData('.');
    //equ.insertData('.');
    equ.insertData(')');
    equ.insertData('^');
    equ.insertData('3');
    equ.insertData('^');
    equ.insertData('4');
    equ.insertData('.');
    //equ.insertData('+');
    //equ.insertData('.');*/

    equ.printString();

    reversePolish RPN;
    RPN.parse(equ);

    return 0;
}
