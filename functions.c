// Urse Andrei - 312CB
#include "functions.h"

// ========== Operation Call Function ==========
void executeOperations(System *sys) // O(n)
{
	int num_ops;
	char op[OP_SIZE];

	fscanf(sys->input, "%d", &num_ops);

	for (int i = 0; i < num_ops; i++) {
		fscanf(sys->input, "%s", op);

		if (!strcmp("ADD", op)) {
			ADD(sys);

		} else if (!strcmp("DEL", op)) {
			DEL(sys);

		} else if (!strcmp("ADDKW", op)) {
			ADDKW(sys);

		} else if (!strcmp("DELKW", op)) {
			DELKW(sys);

		} else if (!strcmp("FIND", op)) {
			FIND(sys);

		} else if (!strcmp("TOPK", op)) {
			TOPK(sys);

		} else if (!strcmp("PRINT", op)) {
			PRINT(sys);

		} else if (!strcmp("PREFIX", op)) {
			PREFIX(sys);

		}
	}
}

// ============ Initialize System ============== O(1)
FileList *initFileList()
{
	FileList *p = malloc(sizeof(FileList));
	if (!p) {
		return NULL;
	}

	p->head = NULL;
	p->tail = NULL;

	return p;
}

Trie *initTrieNode()
{
	Trie *node = malloc(sizeof(Trie));
	if (!node) {
		return NULL;
	}

	node->letter = '\0';
	node->files = NULL;
	node->left = NULL;
	node->right = NULL;

	return node;
}

System *initSystem()
{
	System *s = malloc(sizeof(System));
	if (!s) {
		return NULL;
	}

	// initialize list and trie
	s->files = initFileList();
	s->trie = initTrieNode();

	// open in/out files
	s->input = fopen("indexare.in", "rt");
	s->output = fopen("indexare.out", "wt");

	return s;
}

// =============== Free System ================= O(n)
void freeFile(File *file)
{
	// free id
	free(file->id);

	// free keywords
	for (int i = 0; i < file->num_keywords; i++) {
		free(file->keywords[i]);
	}
	free(file->keywords);

	free(file);
}

void freeFileList(FileList *files) // O(n * m)
{
	if (!files) {
		return;
	}

	File *p = files->head;
	while (p) {
		File *aux = p->next;

		freeFile(p);

		p = aux;
	}

	free(files);
}

void freeTrie(Trie *root) // O(n + m)
{
	if (!root) {
		return;
	}

	// free reference list
	if (root->files) {
		RefList *p = root->files;
		while (p) {
			RefList *aux = p->next;
			free(p);
			p = aux;
		}
	}

	freeTrie(root->left);
	freeTrie(root->right);
	free(root);
}

void destroySystem(System *sys) // O(sum)
{
	// free file list and trie
	freeFileList(sys->files);
	freeTrie(sys->trie);

	// close in/out files
	fclose(sys->input);
	fclose(sys->output);

	// free system
	free(sys);
}

// ============= Helper Functions ==============
File *searchFile(System *sys, char *id) // O(n)
{
	File *file = sys->files->head;
	while (file) {
		if (!strcmp(file->id,id)) {
			return file;
		}

		file = file->next;
	}

	return NULL;
}

void consumeKeyWords(System *sys, int num_keywords) // O(n)
{
	char buffer[STR_SIZE];
	for (int i = 0; i < num_keywords; i++) {
		fscanf(sys->input, "%s", buffer);
	}
}

File *allocNewFile(System *sys, char *id, int score, int num_keywords) // O(n^2)
{
	File *file = malloc(sizeof(File));
	if (!file) {
		return NULL;
	}

	// add file data
	file->id = strdup(id);
	file->score = score;
	file->keywords = calloc(num_keywords, sizeof(char *));
	file->num_keywords = 0;

	// add keywords to file
	char buffer[STR_SIZE];
	for (int i = 0; i < num_keywords; i++) {
		fscanf(sys->input, "%s", buffer);

		// check for duplicates
		int ok = 0;
		for (int j = 0; j < file->num_keywords; j++) {
			if (!strcmp(buffer, file->keywords[j])) {
				ok = 1;
				break;
			}
		}

		if (!ok) {
			file->keywords[file->num_keywords] = strdup(buffer);
			file->num_keywords++;
		}
	}

	// add file to list
	file->prev = sys->files->tail;
	file->next = NULL;
	if (!sys->files->head && !sys->files->tail) {
		sys->files->head = file;
	} else {
		sys->files->tail->next = file;
	}
	sys->files->tail = file;

	return file;
}

Trie *insertKWTrie(Trie *root, char *keyword) // O(n * m)
{
	Trie *p = root;
	int len = strlen(keyword);

	for (int i = 0; i < len; i++) {
		if (!p->left) {
			// create node with new letter
			p->left = initTrieNode();
			p->left->letter = keyword[i];
			p = p->left;
		} else {
			// find node with letter
			Trie *current = p->left;
			Trie *previous = NULL;

			while (current && current->letter < keyword[i]) {
				previous = current;
				current = current->right;
			}

			if (current && current->letter == keyword[i]) {
				// found letter
				// if entire keyword found, return terminal node
				p = current;
			} else {
				// letter not found
				Trie *aux = initTrieNode();
				aux->letter = keyword[i];
				aux->right = current;

				if (!previous) {
					// insert node at the beggining
					p->left = aux;
				} else {
					previous->right = aux;
				}

				p = aux;
			}
		}
	}

	// return terminal node
	return p;
}

void addReference(File *file, Trie *node) // O(1)
{
	RefList *aux = malloc(sizeof(RefList));
	aux->ref = file;
	aux->next = node->files;
	node->files = aux;
}

Trie *findTerminalNode(Trie *root, char *keyword) // O(n * m)
{
	Trie *aux = root;
	int len = strlen(keyword);

	for (int i = 0; i < len; i++) {
		char letter = keyword[i];
		Trie *brother = aux->left;

		while (brother && brother->letter != letter) {
			brother = brother->right;
		}

		if (!brother) {
			return NULL;
		}

		aux = brother;
	}
	return aux;
}

int delTrieNodes(Trie **node, char *keyword, int len, int level) // O(n * m)
{
	if (!*node) {
		return 0;
	}

	if ((*node)->letter != keyword[level]) {
		// find letter on current level
		return delTrieNodes(&(*node)->right, keyword, len, level);
	}

	if (level == len - 1) {
		if (!(*node)->files && !(*node)->left) {
			Trie *aux = (*node);
			(*node) = (*node)->right;
			free(aux);
			return 1;
		}

		return 0;
	}

	// verify next child node
	int son = delTrieNodes(&(*node)->left, keyword, len, level + 1);
	if (son && !(*node)->files && !(*node)->left) {
		Trie *aux = (*node);
		(*node) = (*node)->right;
		free(aux);
		return 1;
	}
	
	return 0;
}

void delReference(Trie *node, File *file) // O(n)
{
	RefList *current = node->files;
	RefList *previous = NULL;

	while (current) {
		if (current->ref == file) {
			if (!previous) {
				node->files = current->next;
			} else {
				previous->next = current->next;
			}

			free(current);
			break;
		}

		previous = current;
		current = current->next;
	}
}

void delFile(System *sys, File *file) // O(1)
{
	if (file->prev) {
		file->prev->next = file->next;
	} else {
		sys->files->head = file->next;
	}
	
	if (file->next) {
		file->next->prev = file->prev;
	} else {
		sys->files->tail = file->prev;
	}

	freeFile(file);
}

void allocKWFile(File *file, char *keyword)
{
	file->num_keywords++;
	file->keywords = realloc(file->keywords, file->num_keywords * sizeof(char *));
	file->keywords[file->num_keywords - 1] = strdup(keyword);
}

void delKWFile(System *sys, File *file, char *keyword) // O(n)
{
	for (int i = 0; i < file->num_keywords; i++) {
		if (!strcmp(file->keywords[i], keyword)) {
			free(file->keywords[i]);
			for (int j = i; j < file->num_keywords - 1; j++) {
				file->keywords[j] = file->keywords[j + 1];
			}	
			
			break;
		}
	}

	file->num_keywords--;
	// del file if it has no keywords
	if (!file->num_keywords) {
		delFile(sys, file);
		return;
	}

	file->keywords = realloc(file->keywords, file->num_keywords * sizeof(char *));
}

int countFiles(RefList *p) // O(n)
{
	int count = 0;
	while (p) {
		count++;
		p = p->next;
	}

	return count;
}

void storeIDs(RefList *p, char **id_array) // O(n)
{
	int i = 0;
	while (p) {
		id_array[i++] = p->ref->id;
		p = p->next;
	}
}

void sortIDs(char **id_array, int count) // O(n^2)
{
	char *aux;
	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if (strcmp(id_array[i], id_array[j]) > 0) {
				aux = id_array[i];
				id_array[i] = id_array[j];
				id_array[j] = aux;	
			}
		}
	}
}

void printIDs(System *sys, char **id_array, int count) // O(n)
{
	fprintf(sys->output, "%d", count);

	for (int i = 0; i < count; i++) {
		fprintf(sys->output, " %s", id_array[i]);
	}

	fprintf(sys->output, "\n");
}

void buildMaxHeap(File **heap, int count)
{
	// middle -> root
	for (int i = count / 2 - 1; i >= 0; i--) {
		heapify(heap, count, i);
	}
}

void heapify(File **heap, int count, int pos)
{
	int highest_score = pos;

	// check left son
	if (2 * pos + 1 < count &&
		checkPriority(heap[2 * pos + 1], heap[highest_score])) {
		highest_score = 2 * pos + 1;
	}

	// check right son
	if (2 * pos + 2 < count &&
		checkPriority(heap[2 * pos + 2], heap[highest_score])) {
		highest_score = 2 * pos + 2;
	}

	// swap root file with the one with highest score
	if (highest_score != pos) {
		File *aux = heap[pos];
		heap[pos] = heap[highest_score];
		heap[highest_score] = aux;

		// rebuild subtree
		heapify(heap, count, highest_score);
	}
}

int checkPriority(File *file1, File *file2)
{
	if (file1->score > file2->score) {
		return 1;
	}

	if (file1->score < file2->score) {
		return 0;
	}

	if (strcmp(file1->id, file2->id) < 0) {
		return 1;
	}

	return 0;
}

void printKFiles(System *sys, File **heap, int k, int count)
{
	// if number of elements less than K, print all
	int true_count = (k < count) ? k : count;
	fprintf(sys->output, "%d ", true_count);

	for (int i = 0; i < true_count; i++) {
		fprintf(sys->output, "%s ", heap[0]->id);

		// print first element
		heap[0] = heap[count - 1];
		count--;

		// rebuild heap
		heapify(heap, count, 0);
	}
	fprintf(sys->output, "\n");
}

void printTrie(System *sys, Trie *node, char *keyword, int level, int *empty)
{
	if (!node) return;

	// build keyword letter by letter
	keyword[level] = node->letter;
	keyword[level + 1] = '\0';

	// terminal node
	if (node->files) {
		*empty = 0;
		// number of files
		int count = countFiles(node->files);

		// store IDs
		char **id_array = malloc (count * sizeof(char *));
		storeIDs(node->files, id_array);

		// sort IDs
		sortIDs(id_array, count);

		// print keyword, count and files IDs
		fprintf(sys->output, "%s ", keyword);
		printIDs(sys, id_array, count);

		free(id_array);
	}

	printTrie(sys, node->left, keyword, level + 1, empty);
	printTrie(sys, node->right, keyword, level, empty);
}

int countPrefixFiles(Trie *node)
{
	if (!node) {
		return 0;
	}

	int count = 0;

	if (node->files) {
		count += countFiles(node->files);
	}

	count += countPrefixFiles(node->left);
	count += countPrefixFiles(node->right);

	return count;
}

void storePrefixIDs(Trie *node, char **id_array, int *i)
{
	if (!node) {
		return;
	}

	if (node->files) {
		RefList *p = node->files;
		while (p) {
			id_array[*i] = p->ref->id;
			(*i)++;
			p = p->next;
		}
	}

	storePrefixIDs(node->left, id_array, i);
	storePrefixIDs(node->right, id_array, i);
}

void filterIDs(char **id_array, int *count) // O(n)
{
	int true_count = 1;
	for (int i = 1; i < *count; i++) {
		// if IDs are the same, ignore the one from current position
		if (strcmp(id_array[i], id_array[i - 1])) {
			id_array[true_count] = id_array[i];
			true_count++;
		}
	}

	*count = true_count;
}

// ================ Operations =================
void ADD(System *sys)
{
	char id[STR_SIZE];
	int score, num_keywords;
	fscanf(sys->input, "%s%d%d", id,
												&score,
												&num_keywords);

	// verify if file exists
	File *file = searchFile(sys, id);

	if (file) {
		// consume keywords to prevent errors
		consumeKeyWords(sys, num_keywords);
		// file already exists
		fprintf(sys->output, "EXISTS\n");
		return;
	}

	// create new file
	file = allocNewFile(sys, id, score, num_keywords);

	// add/find keywords in trie
	for (int i = 0; i < file->num_keywords; i++) {
		// terminal node
		Trie *node = insertKWTrie(sys->trie, file->keywords[i]);

		// add file reference
		addReference(file, node);
	}

	// operation done
	fprintf(sys->output, "OK\n");
}

void DEL(System *sys)
{
	char id[STR_SIZE];
	fscanf(sys->input, "%s", id);

	// verify if file exists
	File *file = searchFile(sys, id);

	if (!file) {
		// file not found
		fprintf(sys->output, "NOT FOUND\n");
		return;
	}

	// del references from terminal nodes
	for (int i = 0; i < file->num_keywords; i++) {
		char *keyword = file->keywords[i];
		// terminal node
		Trie *node = findTerminalNode(sys->trie, keyword);

		// del file reference
		delReference(node, file);

		// verify if keyword has no references
		if (!node->files) {
			delTrieNodes(&sys->trie->left, keyword, strlen(keyword), 0);
		}
	}

	// del file from list
	delFile(sys, file);

	// operation done
	fprintf(sys->output, "OK\n");
}

void ADDKW(System *sys)
{
	char id[STR_SIZE], keyword[STR_SIZE];
	fscanf(sys->input, "%s%s", id, keyword);

	// verify if file exists
	File *file = searchFile(sys, id);

	if (!file) {
		// file not found
		fprintf(sys->output, "NOT FOUND\n");
		return;
	}

	// verify if keyword is associated
	for (int i = 0; i < file->num_keywords; i++) {
		if (!strcmp(file->keywords[i], keyword)) {
			// keyword found
			fprintf(sys->output, "OK\n");
			return;
		}
	}
	
	// alloc space for new keyword
	allocKWFile(file, keyword);

	// add/find keyword in trie
	Trie *node = insertKWTrie(sys->trie, keyword);

	// add file reference
	addReference(file, node);

	// operation done
	fprintf(sys->output, "OK\n");
}

void DELKW(System *sys)
{
	char id[STR_SIZE], keyword[STR_SIZE];
	fscanf(sys->input, "%s%s", id, keyword);

	// verify if file exists
	File *file = searchFile(sys, id);
	
	if (!file) {
		// file not found
		fprintf(sys->output, "NOT FOUND\n");
		return;
	}

	// verify if keyword is associated
	int found = 0;
	for (int i = 0; i < file->num_keywords; i++) {
		if (!strcmp(file->keywords[i], keyword)) {
			// keyword found
			found = 1;
			break;
		}
	}

	if (!found) {
		// keyword not found
		fprintf(sys->output, "OK\n");
		return;
	}

	// terminal node
	Trie *node = findTerminalNode(sys->trie, keyword);

	// del file reference
	delReference(node, file);

	// verify if keyword has no references
	if (!node->files) {
		delTrieNodes(&sys->trie->left, keyword, strlen(keyword), 0);
	}

	// del keyword from file 
	delKWFile(sys, file, keyword);

	// operation done
	fprintf(sys->output, "OK\n");
}

void FIND(System *sys)
{
	char keyword[STR_SIZE];
	fscanf(sys->input, "%s", keyword);

	// terminal node
	Trie *node = findTerminalNode(sys->trie, keyword);

	// check if node exists / has references
	if (!node || !node->files) {
		fprintf(sys->output, "EMPTY\n");
		return;
	}

	// number of files
	int count = countFiles(node->files);

	char **id_array = malloc(count * sizeof(char *));
	if (!id_array) {
		return;
	}

	// store IDs
	storeIDs(node->files, id_array);

	// sort IDs
	sortIDs(id_array, count);

	// print count and file IDs
	printIDs(sys, id_array, count);

	free(id_array);
}

void TOPK(System *sys)
{
	char keyword[STR_SIZE];
	int k;
	fscanf(sys->input, "%s%d", keyword, &k);

	// terminal node
	Trie *node = findTerminalNode(sys->trie, keyword);

	// check if node exists / has references
	if (!node || !node->files) {
		fprintf(sys->output, "EMPTY\n");
		return;
	}

	int count = countFiles(node->files);

	File **heap = malloc (count * sizeof(File *));
	if (!heap) {
		return;
	}

	// add references to heap
	int i = 0;
	RefList *p = node->files;
	while (p) {
		heap[i++] = p->ref;
		p = p->next;
	}

	// build heap
	buildMaxHeap(heap, count);

	// print files
	printKFiles(sys, heap, k, count);

	free(heap);
}

void PRINT(System *sys)
{
	char keyword[STR_SIZE];
	int empty = 1;

	printTrie(sys, sys->trie->left, keyword, 0, &empty);

	// if no result
	if (empty) {
		fprintf(sys->output, "EMPTY\n");
	}
}

void PREFIX(System *sys)
{
	char prefix[STR_SIZE];
	fscanf(sys->input, "%s", prefix);

	// node with last letter of prefix
	Trie *node = findTerminalNode(sys->trie, prefix);

	// prefix not found
	if (!node) {
		fprintf(sys->output, "EMPTY\n");
		return;
	}

	int count = 0;

	// number of files from prefix last node ,
	count = countFiles(node->files);

	// number of files from subtrie terminal nodes
	count += countPrefixFiles(node->left);

	char **id_array = malloc(count * sizeof(char *));
	if (!id_array) {
		return;
	}

	// store references from prefix last node ,
	storeIDs(node->files, id_array);

	int i = 0;
	// store references from subtrie terminal nodes
	storePrefixIDs(node->left, id_array, &i);

	// sort IDs
	sortIDs(id_array, count);

	// search for duplicates and erase them
	filterIDs(id_array, &count);

	// print IDs
	printIDs(sys, id_array, count);

	free(id_array);
}
