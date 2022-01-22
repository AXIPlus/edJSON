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

/**
 * on_value - classic usage
 * */
int on_value(const edJSON_path_t *path, size_t path_size, edJSON_value_t value) {
    char pv[128];
    int rc = edJSON_build_path_string(pv, 128, path, path_size);
    if(rc < 0) {
        printf("error on edJSON_build_path_string\n");
        return rc;  //stop parsing
    }
    
    printf("%s: ", pv);

    if(value.value_type == EDJSON_VT_STRING) {
        int rc = edJSON_string_unescape(pv, 128, value.value.string.value, value.value.string.value_size);
        if(rc < 0) {
            printf("error on edJSON_string_unescape\n");
            return rc;  //stop parsing
        }

        printf("\"%s\"", pv);
    }
    else if(value.value_type == EDJSON_VT_INTEGER) {
        printf("%d", value.value.integer);
    }
    else if(value.value_type == EDJSON_VT_DOUBLE) {
        printf("%.4f", value.value.floating);
    }
    else if(value.value_type == EDJSON_VT_BOOL) {
        printf("%d", value.value.boolean);
    }
    else if(value.value_type == EDJSON_VT_NULL) {
        printf("null");
    }
    printf("\n");

    return 0;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    FILE *f = fopen("tests/test1.json", "r");
    char json[2048];    
    size_t bytes = fread(json, sizeof(char), 2048, f);
    json[bytes] = 0;
    fclose(f);

    char json_copy[2048];
    strcpy(json_copy, json);

    edJSON_path_t path_buffer[16];

    int result = edJSON_parse(json, path_buffer, 16, on_value);
    if(strcmp(json, json_copy) != 0) {
        printf("---> strcmp() failed; this is OK if unescaping was done in-place\n");
    }
    
    if(result == 0) {
        printf("Parse SUCCESS\n");
    }
    else {
        printf("PARSE ERROR | Error at position %d\n", result);
    }
    



    return 0;
}
