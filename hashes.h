typedef unsigned char byte;
#define true 1
#define false 0
#include <stdio.h>
#include <stdlib.h>
#define eq(a, b) (strcmp(a, b)==0)
typedef struct
{
	char **Keys;
	char **Values;
} Hash;

void hashInit(Hash *HashTable)
{
	(*HashTable).Keys=malloc(sizeof(char*));
	(*HashTable).Values=malloc(sizeof(char*));
	(*HashTable).Values[0]=NULL;
	(*HashTable).Keys[0]=NULL;
}

void hashInsert(Hash *HashTable, char *Key, char *Value)
{
	int i=0;
	while((*HashTable).Keys[i]!=NULL){i++;}
	(*HashTable).Keys=realloc((*HashTable).Keys, sizeof(char*)*(i+1));
	(*HashTable).Values=realloc((*HashTable).Values, sizeof(char*)*(i+1));
	(*HashTable).Keys[i]=Key;
	(*HashTable).Values[i]=Value;
	(*HashTable).Keys[i+1]=NULL;
	(*HashTable).Keys[i+1]=NULL;
}

char *hashSearch(Hash HashTable, char *Key)
{
	int i=0;
	while(HashTable.Keys[i]!=NULL)
	{
		if(eq(HashTable.Keys[i], Key))
		{
			return HashTable.Values[i];
		}
	i++;
	}
	return NULL;
}
