/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include <dirent.h>
#include <string.h>
#include "list.h"
#include "set.h"
#include "common.h"
#include "printing.h"

/**
 * @typedef Typedefinition containing operations on sets which use two
 * sets as arguments.
 */
typedef set_t *(*set_oper) (set_t *, set_t *);

/*
 * Case-insensitive comparison function for strings.
 */
static int compare_words(void *a, void *b)
{
    return strcasecmp(a, b);
}

/*
 * Returns the set of (unique) words found in the given file.
 */
static set_t *tokenize(char *filename)
{
	set_t *wordset = set_create(compare_words);
	list_t *wordlist = list_create(compare_words);
	list_iter_t *it;
	FILE *f;
	
	DEBUG_PRINT("TOKENIZE: %s\n", filename);
	f = fopen(filename, "r");
	if (f == NULL) {
		perror("fopen");
		ERROR_PRINT("fopen() failed");
	}
	tokenize_file(f, wordlist);
	
	it = list_createiter(wordlist);
	while (list_hasnext(it)) {
		set_add(wordset, list_next(it));		
	}
	list_destroyiter(it);
	list_destroy(wordlist);
	return wordset;
}

/*
 * Prints a set of words.
 */
static void printwords(char *prefix, set_t *words)
{
	set_iter_t *it;
	
	it = set_createiter(words);
	INFO_PRINT("%s: ", prefix);
	while (set_hasnext(it)) {
		INFO_PRINT(" %s", (char *)set_next(it));
	}
	printf("\n");
	set_destroyiter(it);
}

static void printlist(char *listname, list_t *l)
{
	list_iter_t *it;

	it = list_createiter(l);
	DEBUG_PRINT("%s: ", listname);
	while (list_hasnext(it)) {
		printf(" %s", (char *)list_next(it));
	}
	printf("\n");
	list_destroyiter(it);
}

static void printlistofsets(char *listname, list_t *l)
{
	list_iter_t *it;

	it = list_createiter(l);
	DEBUG_PRINT("%s: ", listname);
	while (list_hasnext(it)) {
		printwords("set", list_next(it));
	}
	printf("\n");
	list_destroyiter(it);
}

/**
 * @brief Filter out specific items from a directory listing.
 *        Returns 0 for not found and 1 for found.
 *
 * @param direntry 
 * @return int
 */
static int filter_dirname(char *direntry) {
	char *blacklist[] = {".", ".."};
	int i, blacklist_tot = 2;

	for (i = 0; i < blacklist_tot; i++) {
		if (!strcmp((char *)direntry, blacklist[i]))
			return 1;
	}

	return 0;
}

/*
static list_t *listdir(char *dirname) 
{
	struct dirent *direntry;
	list_t *wordsetlist;
	DIR *dr;
	char *dir_item, path[100];
	set_t *words;

	wordsetlist = list_create(compare_strings);

	dr = opendir(dirname);

	if (dr == NULL) {
		ERROR_PRINT("Error\n");
	}

	while ((direntry = readdir(dr)) != NULL) {
		dir_item = (char *)direntry->d_name;

		if (!filter_dirname(dir_item)) {
			strcpy(path, dirname);
			strcat(path, "/");
			strcat(path, dir_item);

			words = tokenize(path);
			
			list_addlast(wordsetlist, words);
		}
	}

	closedir(dr);
	return wordsetlist;
}
*/

/**
 * @brief Get the intersection of words from all files in input
 * list.
 *
 * @param files 
 * @return intersection
 */
static set_t *list_apply_oper(list_t *wordsets, set_oper oper)
{
	void *fname;
	set_t *wordset, *keywords;
	list_iter_t *setiter;

	DEBUG_PRINT("list_apply_oper: creating iter...\n");
	printlistofsets("wordset", wordsets);
	setiter = list_createiter(wordsets);
	DEBUG_PRINT("list_apply_oper: Created iterator...\n");

	// Get the first set of words.
	if (list_hasnext(setiter)) {
		keywords = (set_t *)list_next(setiter);
	}

	// Find intersection between all words of all files.
	while (list_hasnext(setiter)) {
		wordset = (set_t *)list_next(setiter);
		keywords = oper(keywords, wordset);
	}

	list_destroyiter(setiter);

	return keywords;
}

/**
 * @brief Filter files in directory mail using sample mails
 * from spam and nonspam to derive rules for classification.
 *
 * More specifically the rule is; a mail file M is classified
 * as spam if and only if:
 * 
 * M U (S1 I S2 I ... I Sn) - (N1 U N2 U ... U Nm) != Ø.
 *
 * Where S denotes known spam mails, N as non spam mail,
 * and Ø the empty set.
 * Using the operations 'U' - union, 'I' - intersection 
 * and '-'- difference
 *
 * @param spam 
 * @param nonspam 
 * @param mail 
 */
static void spamfilter(char *spam, char *nonspam, char *mail)
{
	void *fname;
	char *classification;
	list_t *spamfiles, *nonspamfiles, *mailfiles;
	set_t *spamwords, *nonspamwords, *mailwords, *filterset, *result;
	list_iter_t *mailiter;

	spamfiles = find_files(spam);
	nonspamfiles = find_files(nonspam);
	mailfiles = find_files(mail);

	spamwords = list_apply_oper(spamfiles, set_intersection);
	nonspamwords = list_apply_oper(nonspamfiles, set_union);

	filterset = set_difference(spamwords, nonspamwords);

	mailiter = list_createiter(mailfiles);

	while (list_hasnext(mailiter)) {
		fname = list_next(mailiter);

		mailwords = tokenize(fname);

		result = set_intersection(mailwords, filterset);
		

		char *classification = set_size(result) > 0 ? "SPAM" : "Not spam";

		printf(
			"%s: %d spam word(s) -> %s",
			(char *)fname,
			set_size(result),
			classification
		);
	}

	list_destroyiter(mailiter);
	list_destroy(spamfiles);
	list_destroy(nonspamfiles);
	list_destroy(mailfiles);
	set_destroy(filterset);
	set_destroy(result);
	set_destroy(spamwords);
	set_destroy(nonspamwords);
	set_destroy(mailwords);
}

/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
	char *spamdir, *nonspamdir, *maildir;
	
	if (argc != 4) {
		DEBUG_PRINT("usage: %s <spamdir> <nonspamdir> <maildir>\n", argv[0]);
		return 1;
	}
	spamdir = argv[1];
	nonspamdir = argv[2];
	maildir = argv[3];

	DEBUG_PRINT("%s\n",spamdir);
	DEBUG_PRINT("%s\n",nonspamdir);
	DEBUG_PRINT("%s\n",maildir);

	spamfilter(spamdir, nonspamdir, maildir);
	return 0;
}
