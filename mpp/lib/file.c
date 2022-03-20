//
//  file.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "file.h"
#include "error.h"
#include "logger.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const unsigned MAX_FILE_SIZE = 500 * 1024 * 1024; // 500 MB
const unsigned MAX_FILE_LINE_SIZE = 1 * 1024 * 1024; // 1 MB

const char EOL = '\n';
const char NULL_TERMINATOR = '\0';

void rm_breaklines(char *content) {
    if (content == NULL) return;

    const size_t content_size = strlen(content);
    for (unsigned int i = 0; i < content_size; i++) {
        if (content[i] == EOL) {
            content[i] = NULL_TERMINATOR;
        }
    }
}

void read_file(const char *path, RawSentenceT *raw_sentence_t) {
    Error err;

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        err.type = FATAL;
        err.message = "Error reading file";
        goto error;
    }

    char *buffer = malloc(MAX_FILE_SIZE);
    char *line_buffer = malloc(MAX_FILE_LINE_SIZE);
    RawSentence *raw_sentences = malloc(sizeof(RawSentence) * 1); // 1 by default
    if (buffer == NULL || line_buffer == NULL || raw_sentences == NULL) {
        err.type = FATAL;
        err.message = "Error reading file";
        goto error;
    }


    int row = 0;
    while (fgets(line_buffer, MAX_FILE_LINE_SIZE, fp)) {
        rm_breaklines(line_buffer);
        if (line_buffer == NULL) continue;

        RawSentence *rs = malloc(sizeof(RawSentence));
        rs->row = row++;
        rs->content = malloc(sizeof(char) * strlen(line_buffer) + 1);
        strcpy(rs->content, line_buffer);

        raw_sentences = realloc(raw_sentences, sizeof(RawSentence) * (rs->row + 1));
        raw_sentences[rs->row] = *rs;
    }

    raw_sentence_t->count = row;
    raw_sentence_t->raw_sentences = raw_sentences;

    fclose(fp);

    free(buffer);
    free(line_buffer);
    
    return;

    error:
        process_error(&err);
}
