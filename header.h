/* ISLEAM Gabriel-Mihai -> 315CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#ifndef _LISTA_H_
#define _LISTA_H_

typedef struct celulag
{
	void *info;
	struct celulag *urm;
} Celula, *TLista, **ALista;

typedef struct variabile
{
	char *nume;
	int valoare;
} Variabila, *TVar;

typedef struct nod {
	char *simbol;
	int nr_fii;
	struct nod *st, *dr;
} TNod, *TArb, **AArb;

typedef struct
{ 
	TLista baza;		/* baza stivei */
	TLista varf; 		/* varful stivei */
} Stiva, *TStiva;

#endif

void ReadVariables(ALista Lista_variabile, FILE *input);
void ProcessingTrees(TLista Lista_variabile, FILE *input, FILE *output);
void ListDestroy(TLista L);