/* ISLEAM Gabriel-Mihai -> 315CB */

#include "header.h"

void ListDestroy(TLista L)												// distrugere lista de variabile
{
	TLista aux = NULL;
	while (L != NULL)
	{
		aux = L;
		L = L->urm;
		free(((TVar)(aux->info))->nume);									// eliberare nume variabila
		free(aux->info);
		free(aux);
	}
}

void ListInseration(ALista Lista_variabile, char* nume, int valoare)				// crearea unei liste cu variabilele declarate
{
	TVar variabila_noua = (TVar)malloc(sizeof(Variabila));
	if (!variabila_noua)
		return;
	variabila_noua->nume = malloc(strlen(nume) + 1);
	if (!variabila_noua->nume)
	{
		free(variabila_noua);
		return;
	}
	strcpy(variabila_noua->nume, nume);				
	variabila_noua->valoare = valoare;

	TLista element_nou = (TLista)malloc(sizeof(Celula));
	if (!element_nou)
	{
		free(variabila_noua->nume);
		free(variabila_noua);
		return;
	}
	element_nou->info = variabila_noua;					
	element_nou->urm = NULL;

	TLista p = *Lista_variabile;
	if (p != NULL)
		while(p->urm != NULL)													// gasirea ultimului element din lista
			p = p->urm;

	if (p == NULL)
		*Lista_variabile = element_nou;
	else
		p->urm = element_nou;
}

void ReadVariables(ALista Lista_variabile, FILE *input)
{
	TLista p = *Lista_variabile;
	int i, numar_variabile, valoare;
	char *nume_v = malloc(11);
	if (!nume_v)
		return;
	char egal;

	fscanf(input, "%d", &numar_variabile);
	for (i = 0; i < numar_variabile; i++)
	{
		fscanf(input, "%s %c %d", nume_v, &egal, &valoare);
		ListInseration(Lista_variabile, nume_v, valoare);
	}
	free(nume_v);
}

void DestroyTree(TArb r) 							/* functie auxiliara - distruge toate nodurile */
{
	if (!r) return;
	DestroyTree (r->st);     						/* distruge subarborele stang */
	DestroyTree (r->dr);     						/* distruge subarborele drept */
	free (r->simbol);
	free (r);             							/* distruge nodul radacina */
}

int SonsNumber(char *symbol)
{
	char *c[5] = {"+", "-", "*", "/", "^"};
	int i;
	
	for (i = 0; i < 5; i++)
		if (strcmp(symbol, *(c + i)) == 0)
			return 2;														// operator => 2 fii

	if (strcmp(symbol, "sqrt") == 0)
		return 1;															// sqrt => 1 fiu

	return 0;																// numar sau variabila => 0 fii (frunza)
}

TArb CreateNode(char* symbol)
{
	TArb nod = (TArb)malloc(sizeof(TNod));
	nod->simbol = strdup(symbol);
	nod->nr_fii = SonsNumber(symbol);
	nod->st = nod->dr = NULL;
	return nod;
}

int VariableVerification(TLista Lista_variabile, char* variabila, int* eroare)
{
	if (isalpha(variabila[0]) == 0)											// variabliba este deja un numar
		return atoi(variabila);

	TLista L = Lista_variabile;
	int valoare = 0;
	*eroare = 1;															// presupunem ca varibila nu este declarata
	while (L != NULL)
	{
		if (strcmp(variabila, ((TVar)(L->info))->nume) == 0)
		{
			valoare = ((TVar)(L->info))->valoare;
			*eroare = 0;													// s-a gasit variabila cautata 
		}																	// si i se salveaza valoarea
		L = L->urm;
	}
	return valoare;
}

void TreeInseration(TArb arb, TArb fiu, int *fixat)							// crearea arborelui
{
	if (arb->nr_fii == 0)
		return;
	if (arb->st == NULL && *fixat == 0)
	{
		arb->st = fiu;
		*fixat = 1;															// s-a gasit o pozitie pentru fiu => fixat = 1
		return;
	}
	else if (arb->st != NULL)
		TreeInseration(arb->st, fiu, fixat);
	if (arb->dr == NULL && *fixat == 0 && strcmp(arb->simbol, "sqrt") != 0)	// sqrt va avea doar fiu stanga
	{
		arb->dr = fiu;
		*fixat = 1;
		return;
	}
	
	else if (arb->dr != NULL)
		TreeInseration(arb->dr, fiu, fixat);
}

TArb CreateTree(char *line)
{
	char* delims = " \n";
	char* symbol = strtok(line, delims);									// preluare simbol (operator sau operand)
	TArb arb = NULL;

	while (symbol != NULL)
	{
		TArb fiu = CreateNode(symbol);										// creare nod nou pentru symbol 
		if (arb == NULL)
			arb = fiu;
		else
		{
			int zero = 0;
			TreeInseration(arb, fiu, &zero);								// inserarea noului nod in arbore
		}
		symbol = strtok(NULL, delims);
	}
	return arb;
}

void TreeEvaluation(TArb arb, int* eroare, TLista L, FILE* output, int* rez)
{
	int rez_st, rez_dr;
	if (arb->nr_fii == 0)
	{
		if (*eroare == 1)													// daca s-a gasit deja o eroare
		{																	// programul se opreste
			*rez = 0;
			return;
		}
		else
		{
			int valoare = VariableVerification(L, arb->simbol, eroare);		// se cauta valoarea pentru fiecare varibila
			if (*eroare == 0)
				*rez = valoare;
			else
			{ 
				*rez = 0;
				fprintf(output, "Variable %s undeclared\n", arb->simbol);
				return;
			}
		}
	}
	else if (strcmp(arb->simbol, "+") == 0)
	{
		TreeEvaluation(arb->st, eroare, L, output, &rez_st);				// se evalueaza pe rand subarborele stand
		TreeEvaluation(arb->dr, eroare, L, output, &rez_dr);				// iar apoi cel drept si se efectueaza
		*rez = rez_st + rez_dr;												// operatia necesara intre cei doi
	}
	else if (strcmp(arb->simbol, "-") == 0)
	{
		TreeEvaluation(arb->st, eroare, L, output, &rez_st);
		TreeEvaluation(arb->dr, eroare, L, output, &rez_dr);
		*rez = rez_st - rez_dr;
	}
	else if (strcmp(arb->simbol, "*") == 0)
	{
		TreeEvaluation(arb->st, eroare, L, output, &rez_st);
		TreeEvaluation(arb->dr, eroare, L, output, &rez_dr);
		*rez = rez_st * rez_dr;
	}
	else if (strcmp(arb->simbol, "sqrt") == 0)
	{
		TreeEvaluation(arb->st, eroare, L, output, &rez_st);
		if (rez_st >= 0)
			*rez = sqrt(rez_st);
		else																// expresie invalida in cazul in care
		{																	// trebuie rezolvat un radical din numar negativ
			if (*eroare == 0)
			{
				fprintf(output, "Invalid expression\n");
				*eroare = 1;
			}
			*rez = 0;
			return;
		}
	}
	else if (strcmp(arb->simbol, "^") == 0)
	{
		TreeEvaluation(arb->st, eroare, L, output, &rez_st);
		TreeEvaluation(arb->dr, eroare, L, output, &rez_dr);
		*rez = pow(rez_st, rez_dr);
	}
	else if (strcmp(arb->simbol, "/") == 0)
	{
		TreeEvaluation(arb->st, eroare, L, output, &rez_st);
		TreeEvaluation(arb->dr, eroare, L, output, &rez_dr);
		if (rez_dr == 0)
		{
			if (*eroare == 0)												// expresie invalida cand se imparte la 0
			{
				fprintf(output, "Invalid expression\n");
				*eroare = 1;
			}
			*rez = 0;
			return;
		}
		else
			*rez = rez_st / rez_dr;
	}
}

void ProcessingTrees(TLista Lista_variabile, FILE *input, FILE *output)
{
	int nr_expr, i;
	size_t characters, len = 150;
	char* line = (char*)malloc(len * sizeof(char));
	if (!line)
		return;
	TArb arb = NULL;

	fscanf(input, "%d ", &nr_expr);

	for (i = 0; i < nr_expr; i++)
	{		
		characters = getline(&line, &len, input);								// preluarea fiecarei linii din fisierul de input
		arb = CreateTree(line);												// crearea arborelui de expresie

		int eroare = 0, rezultat = 0;
		TreeEvaluation(arb, &eroare, Lista_variabile, output, &rezultat);		// evaluarea arborelui de expresie
		
		if (eroare == 0)
			fprintf(output, "%d\n", rezultat);									// in cazul in care nu s-a gasit nicio eroare 
																				// se printeaza rezultatul
		DestroyTree(arb);
	}
	free(line);
}


