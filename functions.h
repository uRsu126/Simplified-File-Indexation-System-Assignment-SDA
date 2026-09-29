// Urse Andrei - 312CB
#include "structs.h"

// ========== Operation Call Function ==========
void executeOperations(System *sys);

// ============ Initialize System ==============
FileList *initFileList();
Trie *initTrie();
System *initSystem();

// ================ Free System ================
void freeFile(File *file);
void freeFileList(FileList *files);
void freeTrie(Trie *trie);
void destroySystem(System *sys);

// ============= Helper Functions ==============
File *searchFile(System *sys, char *id);
void consumeKeyWords(System *sys, int num_keywords);
File *allocNewFile(System *sys, char *id, int score, int num_keywords);
Trie *insertKWTrie(Trie *root, char *keyword);
void addReference(File *file, Trie *node);
Trie *findTerminalNode(Trie *root, char *keyword);
int delTrieNodes(Trie **node, char *keyword, int len, int level);
void delReference(Trie *node, File *file);
void delFile(System *sys, File *file);
void allocKWFile(File *file, char *keyword);
void delKWFile(System *sys, File *file, char *keyword);
int countFiles(RefList *p);
void storeIDs(RefList *p, char **id_array);
void sortIDs(char **id_array, int count);
void printIDs(System *sys, char **id_array, int count);
void buildMaxHeap(File **heap, int count);
void heapify(File **heap, int count, int i);
int checkPriority(File *file1, File *file2);
void printKFiles(System *sys, File **heap, int k, int count);
void printTrie(System *sys, Trie *node, char *keyword, int level, int *empty);
int countPrefixFiles(Trie *node);
void storePrefixIDs(Trie *node, char **id_array, int *i);
void filterIDs(char **id_array, int *count);

// ================ Operations =================
void ADD(System *sys);
void DEL(System *sys);
void ADDKW(System *sys);
void DELKW(System *sys);
void FIND(System *sys);
void TOPK(System *sys);
void PRINT(System *sys);
void PREFIX(System *sys);
