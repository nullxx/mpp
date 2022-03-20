//
//  main.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "lib/file.h"
#include "lib/lexer/lexer.h"
#include "lib/motd.h"

int main(int argc, const char * argv[]) {
    print_welcome();
    
    RawSentenceT *raw_sentence_t = malloc(sizeof(RawSentence));
    read_file("/Users/nullx/Documents/mpp/mpp/program.txt", raw_sentence_t);
    run_lexer(raw_sentence_t);

    free(raw_sentence_t);

    return 0;
}
