//
//  main.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include <stdio.h>
#include <stdlib.h>

#include "lib/error.h"
#include "lib/file.h"
#include "lib/lexer/lexer.h"
#include "lib/logger.h"
#include "lib/motd.h"
#include "lib/parser/parser.h"

int main(int argc, const char *argv[]) {
    print_welcome();

    if (argc < 2) {
        Error err;
        err.message = "Usage: <path>";
        err.show_errno = false;
        err.type = FATAL;
        process_error(&err);
        return 1;
    };

    RawSentenceT *raw_sentence_t = malloc(sizeof(RawSentence));

    log_debug("Reading file");
    read_file(argv[1], raw_sentence_t);

    log_debug("Running lexer");
    SentenceT *sentencest = run_lexer(raw_sentence_t);

    log_debug("Running parser");
    run_parser(sentencest);

    free(raw_sentence_t);
    return 0;
}
