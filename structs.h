// Urse Andrei - 312CB
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define OP_SIZE 10
#define STR_SIZE 101

// =================== File ====================
typedef struct file {
	char *id;
	int score;
	char **keywords;
	int num_keywords;
	struct file *next, *prev;
} File;

// ================= File List =================
typedef struct file_list {
	File *head;
	File *tail;
} FileList;

// ============== Reference List ===============
typedef struct ref_list {
	File *ref;
	struct ref_list *next;
} RefList;

// =================== Trie ====================
typedef struct trie {
	char letter;
	RefList *files;
	struct trie *left, *right;
} Trie; // left child, right sibling

// ================== System ===================
typedef struct system {
	FileList *files;
	Trie *trie;
	FILE *input, *output;
} System;
