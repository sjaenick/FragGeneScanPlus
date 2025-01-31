#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "util_lib.h"

// which corresponds to either 0 (done) or 1 (empty) buffer
void enqueue(thread_data* td, unsigned int buffer, unsigned int which) {

    QUEUE* item = (QUEUE*) malloc(sizeof(QUEUE));
    item->td = td;
    item->buffer = buffer;
    item->next = 0;

    QUEUE** head = 0;
    QUEUE** tail = 0;

    if(which) {
        head = &q_empty_head;
        tail = &q_empty_tail;
    } else {
        head = &q_done_head;
        tail = &q_done_tail;
    }

//    printf("INSIDE ENQ, head %d tail %d\n", *head, *tail);
    if(!*head) {
        *head = item;
        *tail = item;
    } else {
        (*tail)->next = item;
        *tail = item;
    }
}

void printq(unsigned int which) {
    QUEUE* head;

    if(which) head = q_empty_head;
    else head = q_done_head;

//    printf("QUEUE %d head %d\n", which, head);
    while(head) {
        printf("QUEUE %d has id %d buffer %d\n", which, head->td->id, head->buffer);
        head = head->next;
    }

}

// which corresponds to either 0 (done) or 1 (empty) buffer
QUEUE* deq(unsigned int which) {

    QUEUE** head = 0;
    if(which) head = &q_empty_head;
    else head = &q_done_head;

    if (!*head) return 0;

    QUEUE* temp = *head;
    *head = (*head)->next;
    return temp;
}

// which corresponds to either 0 (done) or 1 (empty) buffer
void cutnpaste_q(QUEUE** dest, unsigned int which) {

    QUEUE** head = 0;

    if(which) {
        head = &q_empty_head;
    } else {
        head = &q_done_head;
    }

    if(!*head) {
        *dest = 0;
        return;
    }

    *dest = *head;
    *head = 0;
}

double** dmatrix(int num_col) {

    // NUM_STATE = NUM_STATE. This is the viterbi score matrix for this sequence.

    int i, j;
    double **m;

    m = calloc(NUM_STATE, sizeof(double*));
    if (!m) {
        fprintf(stderr, "%s\n", "ERROR: Allocation failure for points to rows in dmatrix()");
        exit(EXIT_FAILURE);
    }

    for(i=0; i < NUM_STATE; i++) {
        m[i] = calloc(num_col, sizeof(double));
        if (!m[i]) {
            fprintf(stderr, "%s %d %s\n", "ERROR: Allocation failure for the row ", i, " in dmatrix()");
            exit(EXIT_FAILURE);
        }
    }
    return m;
}


int** imatrix(int num_col) {

    // NUM_STATE = NUM_STATE. This is the viterbi score matrix for this sequence.

    int i, j;
    int **m;

    m = calloc(NUM_STATE, sizeof(int*));
    if (!m) {
        fprintf(stderr, "%s\n", "ERROR: Allocation failure for points to rows in imatrix()");
        exit(EXIT_FAILURE);
    }

    for(i=0; i < NUM_STATE; i++) {
        m[i] = calloc(num_col, sizeof(int));
        if (!m[i]) {
            fprintf(stderr, "%s %d %s\n", "ERROR: Allocation failure for the row ", i ," in imatrix()");
            exit(EXIT_FAILURE);
        }
    }
    return m;
}

int* ivector(int nh) {

    int j;
    int *v;

    v = calloc(nh, sizeof(int));

    if (!v) {
        fprintf(stderr, "%s\n", "ERROR: Allocation failure in ivector()");
        exit(EXIT_FAILURE);
    }

    for(j=0; j<nh; j++) {
        v[j] = 0;
    }
    return v;
}

void free_dmatrix(double **m) {

    int i;

    for(i=NUM_STATE-1; i>=0; i--) {
        free(m[i]);
        m[i] = 0;
    }
    free(m);
    m = 0;
}


void free_imatrix(int **m) {

    int i;

    for(i=NUM_STATE-1; i>=0; i--) {
        free(m[i]);
        m[i] = 0;
    }
    free(m);
    m = 0;
}


int tr2int (char *tr) {

    int result;

    if      (strcmp(tr, "MM")  == 0) {
        result = 0;
    } else if (strcmp(tr, "MI")  == 0) {
        result = 1;
    } else if (strcmp(tr, "MD")  == 0) {
        result = 2;
    } else if (strcmp(tr, "II")  == 0) {
        result = 3;
    } else if (strcmp(tr, "IM")  == 0) {
        result = 4;
    } else if (strcmp(tr, "DD")  == 0) {
        result = 5;
    } else if (strcmp(tr, "DM")  == 0) {
        result = 6;
    } else if (strcmp(tr, "GE")  == 0) {
        result = 7;
    } else if (strcmp(tr, "GG")  == 0) {
        result = 8;
    } else if (strcmp(tr, "ER")  == 0) {
        result = 9;
    } else if (strcmp(tr, "RS")  == 0) {
        result = 10;
    } else if (strcmp(tr, "RR")  == 0) {
        result = 11;
    } else if (strcmp(tr, "ES")  == 0) {
        result = 12;   /* ES: E+ -> S+, E- -> S- */
    } else if (strcmp(tr, "ES1") == 0) {
        result = 13;   /* ES1: E+ -> S-, E- -> S+ */
    }

    return result;
}


int nt2int (char nt) {

    int result;

    if      (nt == 'A' || nt == 'a') {
        result = 0;
    } else if (nt == 'C' || nt == 'c') {
        result = 1;
    } else if (nt == 'G' || nt == 'g') {
        result = 2;
    } else if (nt == 'T' || nt == 't') {
        result = 3;
    } else                            {
        result = 4;
    }

    return result;
}


int nt2int_rc (char nt) {

    int result;

    if      (nt == 'A' || nt == 'a') {
        result = 3;
    } else if (nt == 'C' || nt == 'c') {
        result = 2;
    } else if (nt == 'G' || nt == 'g') {
        result = 1;
    } else if (nt == 'T' || nt == 't') {
        result = 0;
    } else                            {
        result = 4;
    }

    return result;
}

int nt2int_rc_indel (char nt) {

    int result;

    if      (nt == 'A' ) {
        result = 3;
    } else if (nt == 'C' ) {
        result = 2;
    } else if (nt == 'G' ) {
        result = 1;
    } else if (nt == 'T' ) {
        result = 0;
    } else if (nt == 'a' ) {
        result = 8;
    } else if (nt == 'c' ) {
        result = 7;
    } else if (nt == 'g' ) {
        result = 6;
    } else if (nt == 't' ) {
        result = 5;
    } else if (nt == 'n' ) {
        result = 9;
    } else if (nt == 'x' ) {
        result = 10;
    } else                {
        result = 4;
    }

    return result;
}


int trinucleotide (char a, char b, char c) {

    int freq_id;

    if      (a == 'A' || a == 'a') {
        freq_id = 0;
    } else if (a == 'C' || a == 'c') {
        freq_id = 16;
    } else if (a == 'G' || a == 'g') {
        freq_id = 32;
    } else if (a == 'T' || a == 't') {
        freq_id = 48;
    } else {
        freq_id = 0;
    }

    if      (b == 'A' || b == 'a') {
        freq_id += 0;
    } else if (b == 'C' || b == 'c') {
        freq_id += 4;
    } else if (b == 'G' || b == 'g') {
        freq_id += 8;
    } else if (b == 'T' || b == 't') {
        freq_id += 12;
    } else {
        freq_id = 0;
    }

    if      (c == 'A' || c == 'a') {
        freq_id += 0;
    } else if (c == 'C' || c == 'c') {
        freq_id += 1;
    } else if (c == 'G' || c == 'g') {
        freq_id += 2;
    } else if (c == 'T' || c == 't') {
        freq_id += 3;
    } else {
        freq_id = 0;
    }

    return freq_id;
}

int trinucleotide_pep (char a, char b, char c) {

    int freq_id;

    if      (a == 'A' || a == 'a') {
        freq_id = 0;
    } else if (a == 'C' || a == 'c') {
        freq_id = 16;
    } else if (a == 'G' || a == 'g') {
        freq_id = 32;
    } else if (a == 'T' || a == 't') {
        freq_id = 48;
    } else {
        freq_id = 64;
    }

    if (freq_id <64) {
        if      (b == 'A' || b == 'a') {
            freq_id += 0;
        } else if (b == 'C' || b == 'c') {
            freq_id += 4;
        } else if (b == 'G' || b == 'g') {
            freq_id += 8;
        } else if (b == 'T' || b == 't') {
            freq_id += 12;
        } else {
            freq_id = 64;
        }
    }

    if (freq_id < 64) {
        if      (c == 'A' || c == 'a') {
            freq_id += 0;
        } else if (c == 'C' || c == 'c') {
            freq_id += 1;
        } else if (c == 'G' || c == 'g') {
            freq_id += 2;
        } else if (c == 'T' || c == 't') {
            freq_id += 3;
        } else {
            freq_id = 64;
        }
    }
    return freq_id;
}

void get_rc_dna(char *dna, char *dna1) {

    char codon[5] = {'A', 'C', 'G', 'T', 'N'};
    int i;
    int dna_len = strlen(dna);
    for (i=0; i < dna_len; i++) {
        dna1[dna_len-i-1] = codon[nt2int_rc(dna[i])];
    }
}

void get_rc_dna_indel(char *dna, char *dna1) {

    char codon[11] = {'A', 'C', 'G', 'T', 'N', 'a', 'c', 'g', 't', 'n', 'x'};
    int i;
    int dna_len = strlen(dna);
    for (i=0; i < dna_len; i++) {

        dna1[dna_len-i-1] = codon[nt2int_rc_indel(dna[i])];
    }
}


void get_protein(char *dna, char *protein,  int strand) {

    int i;
    char codon_code[65] = {'K','N','K','N',
                           'T','T','T','T',
                           'R','S','R','S',
                           'I','I','M','I',
                           'Q','H','Q','H',
                           'P','P','P','P',
                           'R','R','R','R',
                           'L','L','L','L',
                           'E','D','E','D',
                           'A','A','A','A',
                           'G','G','G','G',
                           'V','V','V','V',
                           '*','Y','*','Y',
                           'S','S','S','S',
                           '*','C','W','C',
                           'L','F','L','F', 'X'
                          };

    char anti_codon_code[65] = {'F','V','L','I',
                                'C','G','R','S',
                                'S','A','P','T',
                                'Y','D','H','N',
                                'L','V','L','M',
                                'W','G','R','R',
                                'S','A','P','T',
                                '*','E','Q','K',
                                'F','V','L','I',
                                'C','G','R','S',
                                'S','A','P','T',
                                'Y','D','H','N',
                                'L','V','L','I',
                                '*','G','R','R',
                                'S','A','P','T',
                                '*','E','Q','K','X'
                               };
    int dna_len = strlen(dna);
    int protein_len = dna_len/3;

    if (strand ==1) {

        for (i=0; i<dna_len; i+=3) {
            protein[i/3] = codon_code[trinucleotide_pep(dna[i], dna[i+1], dna[i+2])];
        }
    } else {

        if (dna_len % 3 == 2) {
            dna_len -= 2;
        } else if (dna_len % 3 == 1) {
            dna_len -= 1;
        }

        for (i=0; i<dna_len; i+=3) {
            protein[(dna_len-i)/3-1] = anti_codon_code[trinucleotide_pep(dna[i], dna[i+1], dna[i+2])];
            protein_len --;
        }
    }
}


void print_usage() {

    printf("%s", "NOTE: FGS+ should always be run from the FragGeneScan+ directory where the train directory is.\n");
    printf("%s", "USAGE: ./FGS+ -s [seq_file_name] -m [max_mem_use] -o [output_file_name] -w [1 or 0] -t [train_file_name] -p [thread_num] -e [1 or 0] -d [1 or 0] \n");
    printf("%s", "EXAMPLE USAGE: ./FGS+ -s example/NC_000913-454.fna -o output -w 0 -t 454_5 -p 16 \n");
    printf("%s", "MINIMAL USAGE: ./FGS+ -s [seq_file_name] -o [output_file_name] -w [1 or 0] -t [train_file_name] \n");
    printf("%s", "INFO: FragGeneScanPlus will only output the amino acid files by default. To obtain the meta information set -e 1 and for the DNA files set -d 1\n\n");
    printf("%s", "       Mandatory parameters\n");
    printf("%s", "       -s [seq_file_name]:    sequence file name including the full path\n");
    printf("%s", "                              for standard input specify as stdin\n");
    printf("%s", "       -o [output_file_name]: output file name including the full path\n");
    printf("%s", "                              for standard output specify as stdout\n");
    printf("%s", "       -w [1 or 0]:           1 if the sequence file has complete genomic sequences\n");
    printf("%s", "       		                    0 if the sequence file has short sequence reads\n");
    printf("%s", "       -t [train_file_name]:  file name that contains model parameters; this file should be in the -r directory\n");
    printf("%s", "                           Note that four files containing model parameters already exist in the \"train\" directory\n");
    printf("%s", "                           [complete] for complete genomic sequences or short sequence reads without sequencing error\n");
    printf("%s", "                           [sanger_5] for Sanger sequencing reads with about 0.5% error rate\n");
    printf("%s", "                           [sanger_10] for Sanger sequencing reads with about 1% error rate\n");
    printf("%s", "                           [454_5] for 454 pyrosequencing reads with about 0.5% error rate\n");
    printf("%s", "                           [454_10] for 454 pyrosequencing reads with about 1% error rate\n");
    printf("%s", "                           [454_30] for 454 pyrosequencing reads with about 3% error rate\n");
    printf("%s", "                           [illumina_1] for Illumina sequencing reads with about 0.1% error rate\n");
    printf("%s", "                           [illumina_5] for Illumina sequencing reads with about 0.5% error rate\n");
    printf("%s", "                           [illumina_10] for Illumina sequencing reads with about 1% error rate\n\n");
    printf("%s", "       Optional flags\n");
    printf("%s", "       -r [train_file_dir]:  full path of directory containing the model files. If not specified \"train/\" assumed\n");
    printf("%s", "       -p [thread_num]     The number of threads used by FragGeneScanPlus; default is 1 thread.\n");
    printf("%s", "       -e [1 or 0]         Output metadata for sequences.\n");
    printf("%s", "       -d [1 or 0]         Output DNA file.\n");
    printf("%s", "       -m [max_mem_usage]  Maximum amount of memory to be used by the application, in megabytes, default 1024 for 1GB\n");

}

void stopMemset(char* ptr, int length) {
    int i;
    for(i=0; i<length; i++) {
        if(ptr[i] == '\0') {
            return;
        }
        ptr[i] = '\0';
    }
}
