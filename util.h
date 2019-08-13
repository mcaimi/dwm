/* See LICENSE file for copyright and license details. */

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

#define ISNULL(variable)  (variable == NULL)
#define ISNOTNULL(variable)  (variable != NULL)

enum xresource_type {
  STRING = 0,
  INTEGER = 1,
  FLOAT = 2,
};

struct _resource_atom {
  char *xrdb_entry;
  enum xresource_type type;
  void *target;
};
typedef struct _resource_atom ResourceAtom;

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);
