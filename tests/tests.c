/**
 * MIT License
 * 
 * Copyright (c) 2021 AXIPlus / Adrian Lita / Luiza Rusnac - www.axiplus.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edJSON.h"


int test(const char *filename) {
    FILE *f = fopen(filename, "r");
    char json[262144];
    size_t bytes = fread(json, sizeof(char), 262144, f);
    json[bytes] = 0;
    fclose(f);

    char json_copy[262144];
    strcpy(json_copy, json);

    edJSON_path_t path_buffer[16];

    int result = edJSON_parse(json, path_buffer, 16, 0);
    if(strcmp(json, json_copy) != 0) {
        return -2;
    }

    return result;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    
    const char *filenames[] = {
        "tests/array.json",
        "tests/array1.json",
        "tests/array2.json",
        "tests/array3.json",
        "tests/array4.json",
        "tests/array5.json",
        "tests/array6.json",
        "tests/number_corner_cases.json",
        "tests/numbers.json",
        "tests/numbers1.json",
        "tests/numbers2.json",
        "tests/numbers3.json",
        "tests/numbers4.json",
        "tests/objects.json",
        "tests/objects1.json",
        "tests/objects2.json",
        "tests/objects3.json",
        "tests/strings.json",
        "tests/strings1.json",
        "tests/strings2.json",
        "tests/strings3.json",
        "tests/structure.json",
        "tests/structure1.json",
        "tests/structure2.json",
        "tests/structure3.json",
        "tests/test1.json",
        "tests/test2.json",
        "tests/test3.json",
        0 //this array is null-terminated
    };

    const int results[] = {
          0, //        "tests/array.json",
        523, //        "tests/array1.json",
         35, //        "tests/array2.json",
        141, //        "tests/array3.json",
        209, //        "tests/array4.json",
        262, //        "tests/array5.json",
        298, //        "tests/array6.json",
          0, //        "tests/number_corner_cases.json",
          0, //        "tests/numbers.json",
        436, //        "tests/numbers1.json",
        168, //        "tests/numbers2.json",
        584, //        "tests/numbers3.json",
        760, //        "tests/numbers4.json",
          0, //        "tests/objects.json",
        517, //        "tests/objects1.json",
        561, //        "tests/objects2.json",
        301, //        "tests/objects3.json",
          0, //        "tests/strings.json",
          0, //        "tests/strings1.json",
        413, //        "tests/strings2.json",
        183, //        "tests/strings3.json",
          0, //        "tests/structure.json",
        172, //        "tests/structure1.json",
        107, //        "tests/structure2.json",
        145, //        "tests/structure3.json",
          0, //        "tests/test1.json",
          0, //        "tests/test2.json",
          0, //        "tests/test3.json",
          0, //        0 //this array is null-terminated
    };

    int tfnames = sizeof(filenames)/sizeof(char*);
    int tresults = sizeof(results)/sizeof(int);

    if(tfnames != tresults) {
        printf("\033[1;31mTEST ERROR\033[0m | total filenames is different than total results\n");
        return 1;
    }

    int i = 0;
    while(filenames[i]) {
        printf("Testing %-40s ... ", filenames[i]);
        int rc = test(filenames[i]);
        if(rc == -2){
            printf("\033[1;31mPARSE ERROR\033[0m | strcmp() failed\n");
        }    
        else {
            if(rc != results[i]) {
                printf("\033[1;31mPARSE ERROR\033[0m | Result is %d and should have been %d\n", rc, results[i]);
            }
            else {
                printf("\033[1;32mSUCCESS\033[0m\n");
            }
        }
        i++;
    }

    return 0;
}
