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
            static bool isNum = false;
            if(size >= res)
            {
              	if(!reserve(res << 1))
                  return false;
            }

            //validação
            //if(data >= '0' && data <= '9' || data == '.')

            //inserção
            this->data[size++] = data;

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

double sum(double a, double b) {return a+b;}
double sub(double a, double b) {return a-b;}
double mul(double a, double b) {return a*b;}
double div(double a, double b) {return a/b;}
double rt(double a, double b) {return pow(b, 1.0/a);}
double par(double, double) {return 0;}

typedef struct {
    char op;
    lfFunclflf* proc;
    int precedence;
} operation;

const operation operations[] = { {'(', &par, (~0)>>1}, {'+', &sum, 1}, {'-', &sub, 1}, {'x', &mul, 2}, {'/', &div, 2}, {'^', &pow, -3}, {'V', &rt, -3} };

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
            for(const char* str = equation.getString(); str <  equation.getString() + equation.getSize() - 1;)
            {
                //std::cout << " enter ";
                int chars = sscanf(str, "%lf", &value);
                if(chars != 0 && chars != EOF)
                {
                    numberStack.insertData(value);
                    std::cout << value << ' ';

                    while(*str >= '0' && *str <= '9' || *str == '.') ++str;
                }
                //std::cout << " enter ";
                chars = sscanf(str, "%c", &op);
                if(chars != 0 && chars != EOF)
                {
                    if(op != '(')
                    {
                        while(stackOP.getSize() != 0 &&  //testa se pilha de operações está vazio
                            ((op == ')' && stackOP.peekData().proc != &par) ||
                            ((stackOP.peekData().precedence > convertToOperation(op)->precedence) ||     //testa se precedencia da operação no topo da pilha é maior que a precedencia da operação
                            ((stackOP.peekData().precedence == convertToOperation(op)->precedence)) && (convertToOperation(op)->precedence) >= 0))) //testa se precedencia da operação no topo da pilha é igual a precedencia da operação e a associação é da esquerda para direita.
                        {
                            operation t = stackOP.removeData();
                            std::cout << t.op << ' ';
                            double secondArg = numberStack.removeData();
                            numberStack.insertData(t.proc(secondArg, numberStack.removeData()));
                        }

                        if(op == ')')
                        {
                            stackOP.removeData();
                            std::cout << "debug" << ' ';
                        }
                        std::cout << "debug" << ' ';
                    }

                    if(op != ')')
                        stackOP.insertData(*convertToOperation(op));

                    ++str;
                }
            }
            while(stackOP.getSize() != 0)
            {
                std::cout << stackOP.removeData().op << ' ';
            }
            std::cout << std::endl;

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
    equ.insertData('(');
    //equ.insertData('0');
    equ.insertData('1');
    equ.insertData('2');
    equ.insertData('3');
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
    //equ.insertData('.');

    equ.printString();

    reversePolish RPN;
    RPN.parse(equ);

    return 0;
}
