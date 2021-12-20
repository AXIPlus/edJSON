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
        "tests/test1.json",
        0 //this array is null-terminated
    };

    const int results[] = {
        0,
    };

    int i = 0;
    while(filenames[i]) {
        printf("Testing %s ... ", filenames[i]);
        int rc = test(filenames[i]);
        if(rc == -2){
            printf("PARSE ERROR | strcmp() failed\n");
        }    
        else {
            if(rc != results[i]) {
                printf("PARSE ERROR | Result is %d and should have been %d\n", rc, results[i]);
            }
            else {
                printf("SUCCESS\n");
            }
        }
        i++;
    }

    return 0;
}
