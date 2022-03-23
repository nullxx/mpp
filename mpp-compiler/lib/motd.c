//
//  motd.c
//  mpp
//
//  Created by Jon Lara trigo on 20/3/22.
//

#include "motd.h"

#include <stdio.h>

const char* MOTD =
    "╭────────────────────────────────────────────────────────────────────────────╮"
    "\n│                                                                            │"
    "\n│                                                                            │"
    "\n│                             888b     d888                                  │"
    "\n│                             8888b   d8888                                  │"
    "\n│                             88888b.d88888                                  │"
    "\n│                             888Y88888P888    888     888                   │"
    "\n│                             888 Y888P 888  8888888 8888888                 │"
    "\n│                             888  Y8P  888    888     888                   │"
    "\n│                             888   \"   888                                  │"
    "\n│                             888       888                                  │"
    "\n│                                                                            │"
    "\n│                                                                            │"
    "\n│                          Welcome to M++ compiler                           │"
    "\n│                      More info: https://mpp.nullx.me                       │"
    "\n╰────────────────────────────────────────────────────────────────────────────╯";

void print_welcome(void) { printf("%s\n", MOTD); }
