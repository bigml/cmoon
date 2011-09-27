/*
 * gcc -std=c99 -o up up.c -I /home/bigml/web/zettair/include/ -I /home/bigml/web/zettair/src/include/ -I /home/bigml/web/zettair/src/include/compat/ -I /home/bigml/web/zettair/src/include/linux/ -L /usr/local/zettair/lib/ -lzet
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "_index.h"
#include "index.h"
#include "_postings.h"
#include "postings.h"

int main(int argc, char *argv[])
{
    struct index *idx;

    printf("load\n");
    idx = index_load("index", (10 * 1024 * 1024), INDEX_LOAD_NOOPT, NULL);
    if (!idx) {
        printf("failure\n");
        return 1;
    }

    printf("search\n");
    struct index_result *result = malloc(sizeof(*result) * 10);
    unsigned int rnum;
    double tnum;
    int est;
    struct index_search_opt sopt = {
        .u.okapi_k3.k1 = 1.2F,
        .u.okapi_k3.k3 = 1e10,
        .u.okapi_k3.b = 0.75,
        .word_limit = 24817184,
        .accumulator_limit = 32767,
        .summary_type = INDEX_SUMMARISE_PLAIN
    };
    if (index_search(idx, "HAHAHA", 0, 10, result, &rnum, &tnum, &est,
                     INDEX_SEARCH_SUMMARY_TYPE, &sopt)) {
        for (int i = 0; i < rnum; i++) {
            printf("%d docno=%lu\n", i, result[i].docno);
            printf("%d id=%s\n", i, result[i].auxilliary);
            printf("%d score=%f\n", i, result[i].score);
            printf("%d summary=%s\n", i, result[i].summary);
        }
    } else {
        printf("failure\n");
        return 1;
    }

    printf("before UP\n");
    struct postings *pst = idx->post;
    unsigned long int docno = pst->docno;
    pst->docno = 1;
    pst->update_required = 1;

    postings_addword(pst, "HAHAHA", 8);

    struct postings_docstats stats;
    postings_update(pst, &stats);

    pst->docno = docno;


    printf("before commit SE %s\n", strerror(errno));
    
    unsigned int docs;
    struct index_add_opt aopt = {0};
    struct index_commit_opt copt = {0};
    if (!index_commit(idx, INDEX_COMMIT_NOOPT, &copt, INDEX_ADD_NOOPT, &aopt)) {
        printf("error while commiting");
        return 1;
    }

    printf("before search SE %s\n", strerror(errno));
    for (int j = 0; j < 2; j++) {
        if (index_search(idx, "HAHAHA", 0, 10, result, &rnum, &tnum, &est,
                         INDEX_SEARCH_SUMMARY_TYPE, &sopt)) {
            for (int i = 0; i < rnum; i++) {
                printf("%d docno=%lu\n", i, result[i].docno);
                printf("%d id=%s\n", i, result[i].auxilliary);
                printf("%d score=%f\n", i, result[i].score);
                printf("%d summary=%s\n", i, result[i].summary);
            }
        } else {
            printf("failure\n");
            return 1;
        }
    }

    free(result);
    index_delete(idx);
    
    return 0;
}
