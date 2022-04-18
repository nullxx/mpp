/*
 * File: /src/gui/panels/programming.c
 * Project: mpp-cpu
 * File Created: Monday, 18th April 2022 10:50:51 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Monday, 18th April 2022 10:51:07 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#include "programming.h"

#include "../../lib/components/mem.h"
#include "../../lib/utils.h"

GtkWidget *window;
GtkWidget *text_view;
GtkWidget *mem_pos_entry;

static void on_text_change(GtkTextBuffer *buffer, gpointer user_data) { g_print("Text changed\n"); }

void insert_text_buffer(GtkWidget *w) {
    // get the entry text
    const char *mem_pos_str = gtk_editable_get_text(GTK_EDITABLE(mem_pos_entry));
    g_print("Insert text buffer\n");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    // read buffer to string
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    char *str = strdup(text);
    // g_free(text);
    // gtk_text_iter_free(&start);
    // gtk_text_iter_free(&end);

    int mem_pos = (int)strtol(mem_pos_str, NULL, 16);
    g_print("mem_pos: %d\n", mem_pos);

    char *pch = NULL;
    pch = strtok(str, "\n");

    while (pch != NULL) {
        int op = (int)strtol(pch, NULL, 16);
        g_print("op: %d\n", op);
        MemValue m = {.offset = mem_pos++, .value = op};
        set_mem_value(m);
        pch = strtok(NULL, "\n");
    }
    // free(str);
    gtk_window_close(GTK_WINDOW(window));

}

GtkWidget *open_programming_mode(void) {
    // create a new window
    window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Programming Mode");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // create a text view widget and and two buttons in the bottom
    text_view = gtk_text_view_new();
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);
    // on text change
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    g_signal_connect(buffer, "changed", G_CALLBACK(on_text_change), NULL);

    GtkWidget *button_run = gtk_button_new_with_label("Save to memory");
    // on buton click
    g_signal_connect(button_run, "clicked", G_CALLBACK(insert_text_buffer), text_view);
    mem_pos_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(mem_pos_entry), "0000 (hex)");

    // create a scrolled window to hold the text view
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);
    // scroll window height of the window
    gtk_widget_set_size_request(scrolled_window, 800, 600);

    // create a grid to hold the buttons
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), mem_pos_entry, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_run, 1, 0, 1, 1);

    // create a grid to hold the scrolled window and the buttons
    GtkWidget *grid_main = gtk_grid_new();
    gtk_widget_set_hexpand(grid, TRUE);

    gtk_grid_attach(GTK_GRID(grid_main), scrolled_window, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_main), grid, 0, 1, 1, 1);

    // add the grid to the window
    gtk_window_set_child(GTK_WINDOW(window), grid_main);

    // make the grid_main fit all the window
    gtk_widget_set_hexpand(grid_main, TRUE);
    gtk_widget_set_vexpand(grid_main, TRUE);

    gtk_window_present(GTK_WINDOW(window));
    return window;
}