static char const cvsid[] = "$Id: main.c,v 2.1 2005/06/14 22:16:48 jls Exp $";

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>
#include <stdlib.h>

#define MAX_OBM_SIZE 512000

void subr (int64_t I0[], int64_t Out[], int num, int64_t thr, int *idx, int64_t *time, int mapnum);


int main (int argc, char *argv[]) {
    FILE *res_map, *res_cpu;
    int i, num, iter, cpu_iter;
    int64_t *A, *B, *MB;
    int64_t tm, res, accum=0, thr;
    int mapnum = 0;

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
        }

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
        }

    if (argc < 3) {
	fprintf (stderr, "need number of elements and threshold as args\n");
	exit (1);
	}

    if (sscanf (argv[1], "%d", &num) < 1) {
	fprintf (stderr, "need number of elements and threshold as args\n");
	exit (1);
	}

    if (sscanf (argv[2], "%lld", &thr) < 1) {
	fprintf (stderr, "need number of elements and threshold as args\n");
	exit (1);
	}

    if ((num < 1) || (num > MAX_OBM_SIZE)) {
        fprintf (stderr, "number of elements must be in the range 1 through %d\n", MAX_OBM_SIZE);
	exit (1);
	}

    A = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
    B = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));
    MB = (int64_t*) Cache_Aligned_Allocate (num * sizeof (int64_t));

    srandom (99);

    for (i=0; i<num; i++) {
        A[i] = random () & 0xff;
	}

    accum = 0;
    for (i=0; (i<num)&&(accum<thr); i++) {
        if (A[i] < 128)
	    accum += A[i];
	MB[i] = accum;
	}
    cpu_iter = i;

    map_allocate (1);

    subr (A, B, num, thr, &iter, &tm, mapnum);

    printf ("%lld clocks\n", tm);

    fprintf (res_map, "iter = %d\n", iter);
    fprintf (res_cpu, "iter = %d\n", cpu_iter);

    for (i=0; i<iter; i++) {
        fprintf (res_map, "%lld\n", B[i]);
        fprintf (res_cpu, "%lld\n", MB[i]);
	}

    map_free (1);

    exit(0);
    }
