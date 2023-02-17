/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include <dirent.h>
#include <string.h>
#include "list.h"
#include "set.h"
#include "common.h"
#include "printing.h"

/**
 * @typedef Operations on sets which use two
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


/**
 * @brief Get the intersection of words from all files in input
 * list.
 *
 * @param files 
 * @return intersection
 */
static set_t *list_apply_oper(list_t *filelist, set_oper oper)
{
	char *fname;
	set_t *wordset, *keywords;
	list_iter_t *fiter;

	fiter = list_createiter(filelist);

	// Get the first set of words.
	if (list_hasnext(fiter)) {

		// Get first filename
		fname = list_next(fiter);

		// Tokenize the file, and get the set.
		keywords = tokenize(fname);
	}

	// Find intersection between all words of all files.
	while (list_hasnext(fiter)) {
		fname = list_next(fiter);
		wordset = tokenize(fname);

		// Apply parameter operation on all files.
		keywords = oper(keywords, wordset);
	}

	list_destroyiter(fiter);

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

	// Put filenames of all files of input directories
	// into separate lists.
	spamfiles = find_files(spam);
	nonspamfiles = find_files(nonspam);
	mailfiles = find_files(mail);

	// Apply intersection to spamwords and union to non-spamwords.
	spamwords = list_apply_oper(spamfiles, set_intersection);
	nonspamwords = list_apply_oper(nonspamfiles, set_union);

	// Find the difference between spam and non-spam.
	filterset = set_difference(spamwords, nonspamwords);

	mailiter = list_createiter(mailfiles);

	// Iterate over the mail files.
	while (list_hasnext(mailiter)) {
		fname = list_next(mailiter);

		// Tokenize into sets of words.
		mailwords = tokenize(fname);

		// Find intersection between the filterset and mailwords.
		result = set_intersection(mailwords, filterset);
		

		// Returns SPAM if more than 0 spamwords found. Else Not spam.
		classification = set_size(result) > 0 ? "SPAM" : "Not spam";

		// Match format of comparison file.
		printf(
			"%s: %d spam word(s) -> %s\n",
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
