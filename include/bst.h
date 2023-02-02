#include "common.h"

struct tree;
typedef struct tree tree_t;

struct tree_iter;
typedef struct tree_iter tree_iter_t;

tree_t *tree_create(cmpfunc_t cmpfunc);

void tree_destroy(tree_t *tree);

size_t tree_size(tree_t *tree);

int tree_add(tree_t *tree, void *elem);

int tree_find(tree_t *tree, void *elem);

// int tree_maxdepth(tree_t *tree);

tree_iter_t *tree_createiter(tree_t *tree);

void tree_destroyiter(tree_iter_t *iter);

int tree_hasnext(tree_iter_t *iter);

void *tree_next(tree_iter_t *iter);

