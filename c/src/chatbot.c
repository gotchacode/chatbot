#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "chatbot.h"

/* Portable strdup replacement: pure ISO C, no _GNU_SOURCE needed. */
static char *dup_str( const char *source ) {
  size_t length = strlen( source );
  if( length == SIZE_MAX ) {
    return NULL;
  }

  size_t size = length + 1;
  char *copy = malloc( size );
  if( copy ) {
    memcpy( copy, source, size );
  }
  return copy;
}

/* ASCII-only in-place lowercasing. */
static void ascii_lower( char *text ) {
  for( unsigned char *character = (unsigned char *)text; *character;
       ++character ) {
    if( *character >= 'A' && *character <= 'Z' ) {
      *character = (unsigned char)(*character + ('a' - 'A'));
    }
  }
}

typedef struct entry {
  char         *key;
  char         *value;
  struct entry *next;
} entry_t;

struct hashtable {
  size_t    capacity;
  size_t    count;
  entry_t **buckets;
};

/* FNV-1a hash. */
static uint64_t hash_string( const char *text ) {
  uint64_t hash = UINT64_C(14695981039346656037);
  for( const char *character = text; *character; ++character ) {
    hash ^= (unsigned char)*character;
    hash *= UINT64_C(1099511628211);
  }
  return hash;
}

static size_t ht_index( const hashtable_t *table, const char *key ) {
  return (size_t)(hash_string( key ) % table->capacity);
}

static entry_t *entry_create( const char *key, const char *value ) {
  entry_t *entry = malloc( sizeof *entry );
  if( !entry ) {
    return NULL;
  }

  entry->key = dup_str( key );
  if( !entry->key ) {
    free( entry );
    return NULL;
  }

  entry->value = dup_str( value );
  if( !entry->value ) {
    free( entry->key );
    free( entry );
    return NULL;
  }

  entry->next = NULL;
  return entry;
}

static void entry_destroy( entry_t *entry ) {
  free( entry->key );
  free( entry->value );
  free( entry );
}

enum {
  MIN_TABLE_CAPACITY = 8,
  INITIAL_TABLE_CAPACITY = MIN_TABLE_CAPACITY * 2
};

hashtable_t *ht_create( size_t capacity ) {
  if( capacity < MIN_TABLE_CAPACITY ) {
    capacity = MIN_TABLE_CAPACITY;
  }

  hashtable_t *table = malloc( sizeof *table );
  if( !table ) {
    return NULL;
  }

  if( capacity > SIZE_MAX / sizeof *table->buckets ) {
    free( table );
    return NULL;
  }

  table->buckets = malloc( capacity * sizeof *table->buckets );
  if( !table->buckets ) {
    free( table );
    return NULL;
  }
  for( size_t i = 0; i < capacity; ++i ) {
    table->buckets[i] = NULL;
  }

  table->capacity = capacity;
  table->count = 0;
  return table;
}

void ht_destroy( hashtable_t *table ) {
  if( !table ) {
    return;
  }
  for( size_t i = 0; i < table->capacity; ++i ) {
    entry_t *entry = table->buckets[i];
    while( entry ) {
      entry_t *next = entry->next;
      entry_destroy( entry );
      entry = next;
    }
  }
  free( table->buckets );
  free( table );
}

bool ht_put( hashtable_t *table, const char *key, const char *value ) {
  size_t index = ht_index( table, key );

  /* Walk the chain, update if key already exists. */
  for( entry_t *entry = table->buckets[index]; entry; entry = entry->next ) {
    if( strcmp( key, entry->key ) == 0 ) {
      char *new_value = dup_str( value );
      if( !new_value ) {
        return false;
      }
      free( entry->value );
      entry->value = new_value;
      return true;
    }
  }

  /* Key not found, prepend new entry to chain. */
  entry_t *entry = entry_create( key, value );
  if( !entry ) {
    return false;
  }
  entry->next = table->buckets[index];
  table->buckets[index] = entry;
  table->count++;
  return true;
}

const char *ht_get( const hashtable_t *table, const char *key ) {
  size_t index = ht_index( table, key );

  for( entry_t *entry = table->buckets[index]; entry; entry = entry->next ) {
    if( strcmp( key, entry->key ) == 0 ) {
      return entry->value;
    }
  }
  return NULL;
}

int main( void ) {
  static const char sep[] = " *.,\"\n";

  hashtable_t *table = ht_create( INITIAL_TABLE_CAPACITY );
  if( !table ) {
    (void)fputs( "out of memory\n", stderr );
    return 1;
  }

  if( !ht_put( table, "hi", "hello" ) ||
      !ht_put( table, "hey", "hello" ) ||
      !ht_put( table, "hear", "What you heard is right" ) ||
      !ht_put( table, "python", "Yo, I love Python" ) ||
      !ht_put( table, "light", "I like light" ) ||
      !ht_put( table, "what", "It is clear, ain't it?" ) ) {
    (void)fputs( "out of memory\n", stderr );
    ht_destroy( table );
    return 1;
  }

  puts( "$ Chatbot v1.0.0!" );

  char line[CHATBOT_LINE_CAP];
  bool running = true;

  while( running ) {
    (void)fputs( "\n$ (user) ", stdout );

    if( !fgets( line, sizeof line, stdin ) || line[0] == '\n' ) {
      break;
    }

    for( char *word = strtok( line, sep ); word;
         word = strtok( NULL, sep ) ) {
      ascii_lower( word );

      if( strcmp( word, "exit" ) == 0 ) {
        running = false;
        break;
      }

      const char *reply = ht_get( table, word );
      printf( "\n$ (chatbot) %s\n",
              reply ? reply : "Sorry, I don't know what to say about that" );
    }
  }

  ht_destroy( table );
  return 0;
}
