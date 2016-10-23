/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * contributed by Jon Harrop
 * modified by Alex Mizrahi
 * modified by Andreas Schäfer
 * very minor omp tweak by The Anh Tran
 * modified by Maxime Coste
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include <boost/pool/object_pool.hpp>


const size_t   LINE_SIZE = 64;


struct Node
{
    Node *l, *r;
    int i;

    int check() const { return l ? l->check() + i - r->check() : i; }
};

typedef boost::object_pool<Node> NodePool;


Node *make(int i, int d, NodePool &store)
{
    return (d == 0) ? nullptr : store.construct(Node{make(2*i-1, d-1, store), make(2*i, d-1, store), i});
}

int GetThreadCount()
{
   cpu_set_t cs;
   CPU_ZERO(&cs);
   sched_getaffinity(0, sizeof(cs), &cs);

   int count = 0;
   for (int i = 0; i < 8; i++)
   {
      if (CPU_ISSET(i, &cs))
         count++;
   }
   return count;
}

int run(int input)
{
    int min_depth = 4;
    int max_depth = std::max(min_depth+2, input);
    int stretch_depth = max_depth+1;

   // Alloc then dealloc stretchdepth tree
    {
        NodePool store;
        Node *c = make(0, stretch_depth, store);
        std::cout << "stretch tree of depth " << stretch_depth << "\t "
                  << "check: " << c->check() << std::endl;
    }

    NodePool long_lived_store;
    Node *long_lived_tree = make(0, max_depth, long_lived_store);

   // buffer to store output of each thread
   char *outputstr = (char*)malloc(LINE_SIZE * (max_depth +1) * sizeof(char));

   #pragma omp parallel for default(shared) num_threads(GetThreadCount()) schedule(dynamic, 1)
    for (int d = min_depth; d <= max_depth; d += 2)
    {
        int iterations = 1 << (max_depth - d + min_depth);
        int c = 0;

        for (int i = 1; i <= iterations; ++i)
        {
            NodePool store;
            Node *a = make(i, d, store), *b = make(-i, d, store);
            c += a->check() + b->check();
        }

      // each thread write to separate location
      sprintf(outputstr + LINE_SIZE * d, "%d\t trees of depth %d\t check: %d\n", (2 * iterations), d, c);
   }

   // print all results
   for (int d = min_depth; d <= max_depth; d += 2)
      printf("%s", outputstr + (d * LINE_SIZE) );
   free(outputstr);

    std::cout << "long lived tree of depth " << max_depth << "\t "
             << "check: " << (long_lived_tree->check()) << "\n";

    return 0;
}

#include "nbind/nbind.h"

NBIND_GLOBAL() {
  function(run);
}

/*

NOTES:
64-bit Ubuntu quad core
g++ (Ubuntu 5.4.0-6ubuntu1~16.04.1) 5.4.0 20160609


Wed, 27 Apr 2016 18:02:26 GMT

MAKE:
/usr/bin/g++ -c -pipe -O3 -fomit-frame-pointer -march=native  -std=c++14 binarytrees.gpp-7.c++ -o binarytrees.gpp-7.c++.o &&  \
        /usr/bin/g++ binarytrees.gpp-7.c++.o -o binarytrees.gpp-7.gpp_run -fopenmp -lboost_system
rm binarytrees.gpp-7.c++
8.37s to complete and log all make actions

COMMAND LINE:
./binarytrees.gpp-7.gpp_run 20

PROGRAM OUTPUT:
stretch tree of depth 21   check: -1
2097152  trees of depth 4  check: -2097152
524288   trees of depth 6  check: -524288
131072   trees of depth 8  check: -131072
32768  trees of depth 10   check: -32768
8192   trees of depth 12   check: -8192
2048   trees of depth 14   check: -2048
512  trees of depth 16   check: -512
128  trees of depth 18   check: -128
32   trees of depth 20   check: -32
long lived tree of depth 20  check: -1

*/
