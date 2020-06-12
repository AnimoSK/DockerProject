#ifndef ENCRYPT_H
#define ENCRYPT_H

char *aesEncrypt(char *);

void addKey(int **, int **);

void substitution(int *);

void shift(int *, int);

void mixColumns(int **);

void keySchedule(int **, int);

void printMatrix(int **);

#endif