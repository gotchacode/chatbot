#ifndef CHATBOT_H
#define CHATBOT_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Opaque hash table using separate chaining and FNV-1a hashing.
 * Keys are stored and compared exactly as provided.
 */
typedef struct hashtable hashtable_t;

enum { CHATBOT_LINE_CAP = 256 };

hashtable_t *ht_create( size_t capacity );
void         ht_destroy( hashtable_t *table );
bool         ht_put( hashtable_t *table, const char *key, const char *value );
const char  *ht_get( const hashtable_t *table, const char *key );

#endif /* CHATBOT_H */
