/*
 * File: /src/gui/panels/diagram.c
 * Project: mpp-cpu
 * File Created: Monday, 18th April 2022 12:42:33 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Monday, 18th April 2022 12:49:13 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "diagram.h"

GdkPixbuf *image_pixbuf;
GtkWidget *image;

void configure_callback(GtkWindow *window, GdkEvent *event, gpointer data) {
    int window_width = gtk_widget_get_allocated_width(window);
    int window_height = gtk_widget_get_allocated_height(window);
}
static void on_window_resize(GtkWidget *widget, gpointer data) {
    int window_width = gtk_widget_get_allocated_width(widget);
    int window_height = gtk_widget_get_allocated_height(widget);

    gdk_pixbuf_scale(image_pixbuf, image_pixbuf, -1, -1, window_width, window_height, -1, -1, -1, -1, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), image_pixbuf);
}

GtkWidget *draw_register(GtkWidget *parent_grid, const char *text, int posx, int posy) {
    GtkWidget *box = gtk_frame_new(text);
    gtk_widget_set_size_request(box, 100, 40);
    gtk_grid_attach(GTK_GRID(parent_grid), box, posx, posy, 1, 1);
    // create a entry
    GtkWidget *entry = gtk_entry_new();
    gtk_editable_set_width_chars(GTK_EDITABLE(entry), 3);
    gtk_editable_set_text(GTK_ENTRY(entry), "");
    gtk_widget_set_size_request(entry, 40, 40);
    gtk_frame_set_child(GTK_FRAME(box), entry);

    return entry;
}

GtkWidget *draw_line_horizontal(GtkWidget *parent_grid, int x, int y, int width) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(box), TRUE);

    // create two labels and append
    GtkWidget *label = gtk_label_new("");
    gtk_widget_set_size_request(label, width, 1);
    gtk_box_append(GTK_BOX(box), label);
    gtk_widget_add_css_class(label, "separator-2");

    label = gtk_label_new("");
    gtk_widget_set_size_request(label, width, 1);
    gtk_box_append(GTK_BOX(box), label);
    gtk_widget_add_css_class(label, "separator-1");

    gtk_grid_attach(GTK_GRID(parent_grid), box, x, y, 1, 1);

    return box;
}
GtkWidget *draw_line_vertical(GtkWidget *parent_grid, int x, int y, int height) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_set_homogeneous(GTK_BOX(box), TRUE);

    // create two labels and append
    GtkWidget *label = gtk_label_new("");
    gtk_widget_set_size_request(label, 1, height);
    gtk_box_append(GTK_BOX(box), label);
    gtk_widget_add_css_class(label, "separator-4");

    label = gtk_label_new("");
    gtk_widget_set_size_request(label, 1, height);
    gtk_box_append(GTK_BOX(box), label);
    gtk_widget_add_css_class(label, "separator-3");

    gtk_grid_attach(GTK_GRID(parent_grid), box, x, y, 1, 1);

    return box;
}

void open_diagram(void) {
    // open a image
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Diagram");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);

    GtkWidget *overlay = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), overlay);

    image_pixbuf = gdk_pixbuf_new_from_file_at_scale("diagram.png", 3645, 2558, TRUE, NULL);
    image = gtk_picture_new_for_pixbuf(image_pixbuf);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), image);

    // create a absolute layout
    // GtkWidget *grid = gtk_grid_new();
    // gtk_grid_set_row_spacing(GTK_GRID(grid), 0);
    // gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
    // gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    // gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    // gtk_overlay_add_overlay(GTK_OVERLAY(overlay), grid);

    // create a entry and set position
    // GtkWidget *entry = draw_register(grid, "R0", 0, 0);

    // show the window
    gtk_window_present(GTK_WINDOW(window));
}