/*
 * Name: Jorge Fernando Flores Pinto
 * ID: V00880059
 * SENG265, Summer 2017
 * phase2.c
 *
 * phase2.c performs the second phase towards data compression.
 * This code was built from Mike's starter code.
 */


#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define ENCODE_MODE 0
#define DECODE_MODE 1

static int INITIAL_SIZE = 100;

//REALLOC_SIZE is the increasing size using for reallocating memory.
static int REALLOC_SIZE = 100;

void usage() {
    fprintf(stderr, "usage: phase2 [--encode|--decode] " \
                    "--infile <filename> --outfile <filename>\n"
    );
}
//Prints the nodes of the list. This is used for testing purposes.
//@param cv (node in the list), arg (format)
void print_charval(charval_t *cv, void *arg) {
    printf((char *)arg,
        cv->c,
        cv->val
    );
}

/*void update_vals(charval_t *cv, void *arg) {
    int intcv;

    intcv = cv->val;
    intcv++;
}*/

/*
 * string_malloc(size_t n)
 *
 * Allocates memory for a string.
 * @param n (size of type)
 * @return string address with allocated memory.
 */
void *string_malloc(size_t n) {
    void *p;

    p = malloc(n * INITIAL_SIZE);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}
/*
 * string_realloc(size_t n)
 *
 * Reallocates memory for a string.
 * @param string (string address to reallocate), n (size of type)
 * @return string address with newly allocated memory.
 */
void *string_realloc(char *string, size_t n) {
    void *p;
    REALLOC_SIZE = REALLOC_SIZE * 2;

    p = realloc(string, n * REALLOC_SIZE);
    if (p == NULL) {
        fprintf(stderr, "realloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}
/* Used for mtf_encoding and mtf_decoding. As repeated characters appear in
 * the sequence of characters, the list is "reordered" - the character that
 * was repeated goes to the front of the list, and the values that came
 * before it shift by one.
 *
 * @param char_list (list of unique characters), cv (node to move)
 * @return reordered list of characters.
 */
charval_t *reorder_list(charval_t *char_list, charval_t *cv) {
    charval_t *curr;
    for(curr = char_list; curr->c != cv->c; curr = curr->next) {
        curr->val++;
    }
    char_list = remove_charval(char_list, cv);
    cv->val = 1;
    char_list = add_front(char_list, cv);

    return char_list;

}
charval_t *reorder_listtest(charval_t *char_list, charval_t *cv, charval_t *tail) {
    charval_t *curr;
    charval_t *prev;
    for(curr = char_list; curr->c != cv->c; curr = curr->next) {
        curr->val++;
        prev = curr;
    }
    if (cv == tail) {
        tail = prev;
    }
    char_list = remove_charval(char_list, cv);
    cv->val = 1;
    char_list = add_front(char_list, cv);

    return char_list;

}
/* Performs the Move To Front (MTF) encoding.
 * @param the mtf_list, infile_string (input string), length (length of string)
 * @return a list ordered with the MTF encoding.
 */
charval_t *mtf_encoding(charval_t *mtf_list, char infile_string[], int length) {

    int count_unique = 1;
    int i;
    int iterations = 0;
    charval_t *char_listp, *temp_char, *temp_mtf;
    charval_t *char_list = NULL;
    charval_t *char_list_tail = NULL;
    charval_t *mtf_list_tail = NULL;


    for (i = 8; i < length; i++) {
        temp_char = new_charval(infile_string[i], count_unique);
        char_listp = find(char_list, temp_char);

        if (char_listp == NULL) {
            temp_mtf = new_charval(infile_string[i], temp_char->val);
            if (iterations == 0) {
                char_list = add_end(char_list, temp_char);
                mtf_list = add_end(mtf_list, temp_mtf);
                char_list_tail = char_list;
                mtf_list_tail = mtf_list;
                count_unique++;
            }
            else {
                char_list_tail = add_with_tail(char_list_tail, temp_char);
                mtf_list_tail = add_with_tail(mtf_list_tail, temp_mtf);
                count_unique++;
            }
        }
        else {
            if (iterations == 0) {
                temp_mtf = new_char(char_listp->val);
                mtf_list = add_end(mtf_list, temp_mtf);
                mtf_list_tail = mtf_list;
                temp_char = new_charval(infile_string[i], 1);
                char_list = reorder_listtest(char_list, temp_char, char_list_tail);
            }
            else {
                temp_mtf = new_char(char_listp->val);
                mtf_list_tail = add_with_tail(mtf_list_tail, temp_mtf);
                temp_char = new_charval(infile_string[i], 1);
                char_list = reorder_listtest(char_list, temp_char, char_list_tail);
            }
        }
        iterations++;
    }
    return mtf_list;
}
/* Performs the Run Length encoding.
 * @param the run_length_list, and the mtf_list
 * @return a list ordered with the Run Length encoding.
 */
charval_t *run_length_encoding(charval_t *run_length_list, charval_t *mtf_list) {
    int one_counter = 0;
    int i;
    int iterations = 0;
    charval_t *curr;
    charval_t *add, *zero_node; 
    charval_t *prev_charval = NULL;
    charval_t *run_list_tail = NULL;

    for (curr = mtf_list; curr != NULL; curr = curr->next) {
        add = new_charval(curr->c, curr->val);

        if (curr->val == 1 && curr->c == 0) {
            one_counter++;
        }
        else {
            if(one_counter >= 1 && one_counter < 3) {
                for(i = 0; i < one_counter; i++) {
                    prev_charval = new_charval(prev_charval->c, prev_charval->val);
                    if (iterations == 0) {
                        run_length_list = add_end(run_length_list, prev_charval);
                        run_list_tail = run_length_list;
                    }
                    else {
                        run_list_tail = add_with_tail(run_list_tail, prev_charval);
                    }
                    iterations++;
                }
                run_list_tail = add_with_tail(run_list_tail, add);
                iterations++;
                one_counter = 0;
            }
            else if (one_counter >= 3) {
                prev_charval->val = one_counter;
                zero_node = new_charval('0', 0);
                if (iterations == 0) {
                    run_length_list = add_end(run_length_list, zero_node);
                    run_list_tail = run_length_list;
                }
                else {
                    run_list_tail = add_with_tail(run_list_tail, zero_node);
                }
                run_list_tail = add_with_tail(run_list_tail, prev_charval);
                run_list_tail = add_with_tail(run_list_tail, add);
                one_counter = 0;
                iterations++;
            }
            else if (one_counter == 0) {
                if (iterations == 0) {
                    run_length_list = add_end(run_length_list, add);
                    run_list_tail = run_length_list;
                    iterations++;
                }
                else {
                    run_list_tail = add_with_tail(run_list_tail, add);
                    iterations++;
                }

            }

        }
        prev_charval = new_charval(add->c, add->val);
    }
    if (one_counter >= 1 && one_counter < 3) {
        for(i = 0; i < one_counter; i++) {
            run_list_tail = add_with_tail(run_list_tail, prev_charval);
            iterations++;
        }
    }
    return run_length_list;

}
/* Converts characters into ASCII. If the element in the list is an
 * integer, it adds 128 (hex 0x80).
 * @param the run_length_list.
 * @return the encoded string.
 */
char *into_ascii(charval_t *run_length_list) {
    REALLOC_SIZE = INITIAL_SIZE;
    charval_t *curr;
    char character;
    int value = 0;
    char ascii_char;
    int chars_copied = 0;

    char *encoded_string = (char *) string_malloc(sizeof(char));

    for(curr = run_length_list; curr != NULL; curr = curr->next) {
        character = curr->c;
        value = curr->val;
        value += 128;
        ascii_char = value;
        if (chars_copied >= REALLOC_SIZE - 2) {
            encoded_string = (char *) string_realloc(encoded_string, sizeof(char));
        }

        if (character == '\0' || (character == '0' && curr->val == 0)) {
            strncat(encoded_string, &ascii_char, 1);
            chars_copied++;
        }
        else {
            strncat(encoded_string, &ascii_char, 1);
            strncat(encoded_string, &character, 1);
            chars_copied += 2;
        }
    }

    return encoded_string;
}
/* Converts characters into a format the program can handle. This just means
 * that characters with an ASCII value greater than 128 are readjusted
 * to integers. This is the first step towards decoding.
 * @param the initial_string (starting string), length (length of starting string).
 * @return the list of characters.
 */
charval_t *from_ascii(char *initial_string, int length) {
    charval_t *char_list = NULL;
    charval_t *temp = NULL;
    charval_t *zero_node = NULL;
    charval_t *char_list_tail = NULL;
    int iterations = 0;
    int next_char = 0;
    int curr_char = 0;

    int i;

    for (i = 8; i < length - 1; i++) {
        next_char = initial_string[i + 1];
        curr_char = initial_string[i];

        if(curr_char == -128) {
            zero_node = new_charval('0', 0);
            if (iterations == 0) {
                char_list = add_end(char_list, zero_node);
                char_list_tail = char_list;
            }
            else {
                char_list_tail = add_with_tail(char_list_tail, zero_node);
            }
            iterations++;
        }
        else if(curr_char < 0 && next_char > 0) {
            curr_char += 128;

            temp = new_charval(initial_string[i + 1], curr_char);
            if (iterations == 0) {
                char_list = add_end(char_list, temp);
                char_list_tail = char_list;
            }
            else {
                char_list_tail = add_with_tail(char_list_tail, temp);
            }
            i++;
            iterations++;
        }
        else if(curr_char < 0 && next_char < 0) {
            curr_char += 128;

            temp = new_char(curr_char);
            if (iterations == 0) {
                char_list = add_end(char_list, temp);
                char_list_tail = char_list;
            }
            else {
                char_list_tail = add_with_tail(char_list_tail, temp);
            }
            iterations++;
        }
    }
    curr_char = initial_string[i];
    if (curr_char < 0) {
        curr_char += 128;
        temp = new_char(curr_char);
        //char_list = add_end(char_list, temp);
        char_list_tail = add_with_tail(char_list_tail, temp);
    }

    return char_list;
}
/* Performs the Run Length decoding.
 * @param the run_length_list, and the mtf_list
 * @return a list ordered with the Run Length decoding, ready for mtf_decoding.
 */
charval_t *run_length_decoding(charval_t *mtf_list, charval_t *run_length_list) {
    charval_t *curr;
    char character = 0;
    int value = 0;
    int i;
    charval_t *temp = NULL;

    for(curr = run_length_list; curr != NULL; curr = curr->next) {
        character = curr->c;
        value = curr->val;
        if (character == '0' && value == 0) {
            curr = curr->next;
            value = curr->val;
            for (i = 0; i < value; i++) {
                temp = new_char(1);
                mtf_list = add_end(mtf_list, temp);
            }
        }
        else {
            temp = new_charval(curr->c, curr->val);
            mtf_list = add_end(mtf_list, temp);
        }
    }

    return mtf_list;
}
/* Performs the Move To Front (MTF) decoding.
 * @param the result_string (string resulting after the MTF decoding), mtf_list
 * (the MTF ordered list to generate the result_string).
 * @return decoded string.
 */
char *mtf_decoding(char *result_string, charval_t *mtf_list) {
    REALLOC_SIZE = INITIAL_SIZE;
    charval_t *curr, *temp_char, *char_listp;
    charval_t *char_list = NULL;
    int control = 0;
    char copy;

    for (curr = mtf_list; curr != NULL; curr = curr->next) {
        if (control >= REALLOC_SIZE - 2) {
            result_string = (char *) string_realloc(result_string, sizeof(char));
        }
        char_listp = find_char(char_list, curr->val);
        temp_char = new_charval(curr->c, curr->val);

        if(char_listp == NULL) {
            char_list = add_end(char_list, temp_char);
            copy = temp_char->c;
            strncat(result_string, &copy, 1);

        }
        else {
            temp_char = new_charval(char_listp->c, char_listp->val);
            copy = char_listp->c;
            strncat(result_string, &copy, 1);
            char_list = reorder_list(char_list, temp_char);
        }
        control++;
    }
    return result_string;
}
int main(int argc, char *argv[]) {
    FILE *fp;
    int c;
    char *infile_name = NULL;
    char *outfile_name = NULL;
    char phase1_bytes[] = {'\xab', '\xba', '\xbe', '\xef', '\0'};
    char phase2_bytes[] = {'\xda', '\xaa', '\xaa', '\xad', '\0'};
    char blocksize[5];
    int mode;
    int encode_flag = 0;
    int decode_flag = 0;

    /*
     * Based on http://bit.ly/2tHBpo1
     */
    for (;;) {
        static struct option long_options[] = {
            {"encode",     no_argument,       0, 'e'},
            {"decode",     no_argument,       0, 'd'},
            {"infile",     required_argument, 0, 'i'},
            {"outfile",    required_argument, 0, 'o'},
            {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long (argc, argv, "edi:o:",
            long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c) {
        case 'i':
            infile_name = optarg;
            break;
        case 'o':
            outfile_name = optarg;
            break;
        case 'e':
            encode_flag = 1;
            break;
        case 'd':
            decode_flag = 1;
            break;
        default:
            fprintf(stderr, "shouldn't be here...");
            assert(0);
        }
    }

    if (encode_flag == 0 && decode_flag == 0) {
        usage();
        exit(1);
    } else if (encode_flag == 1 && decode_flag == 1) {
        fprintf(stderr, "usage: choose one of --decode or --encode\n");
        exit(1);
    } else if (encode_flag == 1) {
        mode = ENCODE_MODE;
    } else if (decode_flag == 1) {
        mode = DECODE_MODE; 
    } else {
        fprintf(stderr, "shouldn't be here...\n");
        assert(0);
    }

    if (infile_name == NULL) {
        usage();
        exit(1);
    }
        
    if (outfile_name == NULL) {
        usage();
        fprintf(stderr, "%s: need --outfile <filename>\n", argv[0]);
        exit(1);
    }
    
    char *infile_string = (char *) string_malloc(sizeof(char));
    int numChars = 0;
    fp = fopen(infile_name, "r");
    if (fp == NULL) {
        fprintf(stderr, 
            "FileNotFoundException: The filename %s does not exist in directory.\n", 
            infile_name);
        exit(1);
    }
    char nextChar = getc(fp);
    infile_string[numChars] = nextChar;
    /*I found this useful while loop at:
    https://stackoverflow.com/questions/2645598/counting-unknown-numbers-of-chars-from-a-file-in-c*/
    char *outfile_string;
    while (nextChar != EOF) {
        if (numChars >= REALLOC_SIZE - 1) {
            infile_string = (char *) string_realloc(infile_string, sizeof(char));
        }
        numChars++;
        nextChar = getc(fp);
        infile_string[numChars] = nextChar;
    }
    infile_string[numChars] = 0;
    fclose(fp);

    if (mode == ENCODE_MODE) {
        int i;
        for(i = 0; i < 4; i++) {
            if(infile_string[i] != phase1_bytes[i]) {
                fprintf(stderr, "The input file '%s' is not a .ph1 file.\n", infile_name);
                exit(1);
            }
        }
        for(i = 0; i < 4; i++) {
            blocksize[i] = infile_string[i + 4];
        }
        blocksize[4] = 0;
        charval_t *mtf_list = NULL;
        charval_t *run_length_list = NULL;

        mtf_list = mtf_encoding(mtf_list, infile_string, numChars);

        run_length_list = run_length_encoding(run_length_list, mtf_list);

        outfile_string = into_ascii(run_length_list);

        fp = fopen(outfile_name, "w");
        if (fp == NULL) {
            fprintf(stderr, "There was an issue writing the file.\n");
        }
        fputs(phase2_bytes, fp);
        for (i = 0; i < 4; i++) {
            fputc(blocksize[i], fp);
        }
        fputs(outfile_string, fp);
        fclose(fp);

    } else if (mode == DECODE_MODE) {
        int i;
        for(i = 0; i < 4; i++) {
            if(infile_string[i] != phase2_bytes[i]) {
                fprintf(stderr, "The input file '%s' is not a .ph2 file.\n", infile_name);
                exit(1);
            }
        }
        for(i = 0; i < 4; i++) {
            blocksize[i] = infile_string[i + 4];
        }
        blocksize[4] = 0;

        charval_t *run_length_list = NULL;
        charval_t *mtf_list = NULL;

        run_length_list = from_ascii(infile_string, numChars);

        mtf_list = run_length_decoding(mtf_list, run_length_list);

        outfile_string = (char *) string_malloc(sizeof(char));

        outfile_string = mtf_decoding(outfile_string, mtf_list);

        fp = fopen(outfile_name, "w");
        if (fp == NULL) {
            fprintf(stderr, "There was an issue writing the file.\n");
        }
        fputs(phase1_bytes, fp);
        for (i = 0; i < 4; i++) {
            fputc(blocksize[i], fp);
        }
        fputs(outfile_string, fp);
        fclose(fp);        

    } else {
        /* Shouldn't be here!! */
        assert(0);
    }

    return 0;
}