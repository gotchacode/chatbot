#include <assert.h>
#include <stdio.h>

#include "graph.h"

#define TEST_SIZE (37)

/* never call this */
static void match_sink(Graph g, int source, int sink, void *data) {
  assert(data && sink == *((int *)data));
}

int main(int argc, char **argv) {
  Graph g;
  int i;
  int j;

  g = graph_create(TEST_SIZE);

  assert(graph_vertex_count(g) == TEST_SIZE);

  /* check it's empty */
  for (i = 0; i < TEST_SIZE; i++) {
    for (j = 0; j < TEST_SIZE; j++) {
      assert(graph_has_edge(g, i, j) == 0);
    }
  }

  /* check it's empty again */
  for (i = 0; i < TEST_SIZE; i++) {
    assert(graph_out_degree(g, i) == 0);
    graph_foreach(g, i, match_sink, 0);
  }

  /* check edge count */
  assert(graph_edge_count(g) == 0);

  /* fill in the diagonal */
  for (i = 0; i < TEST_SIZE; i++) {
    graph_add_edge(g, i, i);
  }

  /* check */
  assert(graph_edge_count(g) == TEST_SIZE);

  for (i = 0; i < TEST_SIZE; i++) {
    for (j = 0; j < TEST_SIZE; j++) {
      assert(graph_has_edge(g, i, j) == (i == j));
    }
  }

  for (i = 0; i < TEST_SIZE; i++) {
    assert(graph_out_degree(g, i) == 1);
    graph_foreach(g, i, match_sink, &i);
  }

  /* fill in all the entries */
  for (i = 0; i < TEST_SIZE; i++) {
    for (j = 0; j < TEST_SIZE; j++) {
      if (i != j)
        graph_add_edge(g, i, j);
    }
  }

  /* test they are all there */
  assert(graph_edge_count(g) == TEST_SIZE * TEST_SIZE);

  for (i = 0; i < TEST_SIZE; i++) {
    assert(graph_out_degree(g, i) == TEST_SIZE);
    for (j = 0; j < TEST_SIZE; j++) {
      assert(graph_has_edge(g, i, j) == 1);
    }
  }

  /* free it */
  graph_destroy(g);

  return 0;
}
