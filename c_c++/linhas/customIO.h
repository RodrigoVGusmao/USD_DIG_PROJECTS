#ifndef CUSTOMIO_H
#define CUSTOMIO_H

///funções auxiliares
int getInt(int *lastChar, int *extract);
void askInput(char* msg, char* fmt, int numArgs, int argsMin, int cicle, void (*func)(uint64_t), void (*textPre)(void));
int getHex(int *lastChar, int *extract);
void askHexInput(char* msg, char* fmt, int numArgs, int argsMin, int cicle, void (*func)(uint64_t), void (*textPre)(void));
void flush(int *ch);
#endif
