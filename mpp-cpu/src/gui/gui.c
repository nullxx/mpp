#include "gui.h"

#include <gtk/gtk.h>

#include "../lib/logger.h"
#include "../lib/thread.h"
#include "../lib/utils.h"
#include "../linker.h"
#include "panels/diagram.h"
#include "panels/programming.h"
#define APP_NAME "M++ SIMULATOR"
#define MEM_WIDGET_HEIGHT 200
#define ROW_HEIGHT 20
#define RUN_MICRO_INSTRUCTION_TEXT "RUN MICRO-INSTRUCTION"
#define RUN_PROGRAM_TEXT "RUN PROGRAM"
#define RUN_STATE_TEXT "RUN STATE"
#define STOP_TEXT "STOP"

static GtkWidget *window;

GtkWidget *pc_entry;
GtkWidget *pch_entry;
GtkWidget *pcl_entry;
GtkWidget *h_entry;
GtkWidget *l_entry;
GtkWidget *sp_entry;
GtkWidget *fc_entry;
GtkWidget *fz_entry;
GtkWidget *b_entry;
GtkWidget *c_entry;
GtkWidget *d_entry;
GtkWidget *e_entry;
GtkWidget *ri_entry;
GtkWidget *acum_entry;
GtkWidget *op2_entry;

GtkWidget *current_state;

GtkWidget *data_bus;
GtkWidget *dir_bus;
GtkWidget *control_bus;

GtkTreeStore *mem_model;
GtkWidget *search_mem_entry;

GtkWidget *run_program_btn;
GtkWidget *run_microinstr_btn;
GtkWidget *run_state_btn;
GtkWidget *open_diagram_btn;
GtkWidget *open_programming_mode_btn;

GtkWidget *tree_view;

int offset_mem_loaded = 0;


static void on_run_end(void) {
    gtk_button_set_label(GTK_BUTTON(run_program_btn), RUN_PROGRAM_TEXT);
    gtk_button_set_label(GTK_BUTTON(run_microinstr_btn), RUN_MICRO_INSTRUCTION_TEXT);
    gtk_button_set_label(GTK_BUTTON(run_state_btn), RUN_STATE_TEXT);
}

static void cancel_running(GtkWidget *widget, gpointer data, const char *last_label) {
    cancel_run();
    gtk_button_set_label(GTK_BUTTON(widget), last_label);
}

static void run_wrapper(RunConfig *ptr) { run(*ptr); }
static void show_running(void) {
    gtk_button_set_label(GTK_BUTTON(run_program_btn), STOP_TEXT);
    gtk_button_set_label(GTK_BUTTON(run_microinstr_btn), STOP_TEXT);
    gtk_button_set_label(GTK_BUTTON(run_state_btn), STOP_TEXT);
}

static void run_state(GtkWidget *widget, gpointer data) {
    // show_stop();
    if (is_running()) return cancel_running(widget, data, RUN_STATE_TEXT);
    show_running();

    gtk_widget_show(GTK_BUTTON(widget));
    RunConfig conf = {.type = STATE};
    // run(conf);
    g_thread_create(run_wrapper, &conf, FALSE, NULL);

    // show_stop();
}

static void run_miroinstruction(GtkWidget *widget, gpointer data) {
    // show_stop();
    if (is_running()) return cancel_running(widget, data, RUN_MICRO_INSTRUCTION_TEXT);
    show_running();

    RunConfig conf = {.type = MICRO_INSTRUCTION};
    // run(conf);
    g_thread_create(run_wrapper, &conf, FALSE, NULL);

    // show_stop();
}

static void run_program(GtkWidget *widget, gpointer data) {
    // show_stop();
    if (is_running()) return cancel_running(widget, data, RUN_PROGRAM_TEXT);
    show_running();

    RunConfig conf = {.type = PROGRAM};
    // run(conf);
    g_thread_create(run_wrapper, &conf, FALSE, NULL);

    // show_stop();
}

static void on_mem_end_reached(GtkWidget *widget, gpointer data) { ui_update_mem(); }
static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_window_destroy(GTK_WINDOW(window));
    // exit(0);
}
static void search_memory(GtkWidget *widget, gpointer data) {
    const gchar *offset_hex = gtk_editable_get_text(GTK_EDITABLE(search_mem_entry));
    const int offset = (int)strtol(offset_hex, NULL, 16);
    char *value_hex = NULL;
    MemUpdated mem_updated = get_mem_updated();
    for (int i = 0; i < mem_updated.values_count; i++) {
        if (offset == mem_updated.values[i].offset) {
            value_hex = mem_updated.values[i].value_hex;
            break;
        }
    }

    value_hex = value_hex == NULL ? "NOT FOUND" : value_hex;
    // create a pop up window with some text
    GtkWidget *popup = gtk_dialog_new();
    gtk_window_set_transient_for(GTK_WINDOW(popup), GTK_WINDOW(window));
    gtk_window_set_title(GTK_DIALOG(popup), "Memory Value");
    gtk_widget_set_size_request(popup, 400, 100);
    // table with frame

    // frame
    GtkWidget *frame = gtk_frame_new("Search result");
    gtk_window_set_child(GTK_DIALOG(popup), frame);

    // label
    GtkWidget *label = gtk_label_new(value_hex);
    // font family courier new
    PangoFontDescription *font_desc = pango_font_description_from_string("Courier New");
    // gtk_widget_modify_font(label, font_desc);
    gtk_widget_add_css_class(label, "label-search-mem");
    gtk_frame_set_child(GTK_FRAME(frame), label);

    gtk_widget_show(popup);
}

static void highlight_changes(RegisterWatcher *watcher, GtkWidget *w) {
    if (watcher->changed) {
        gtk_widget_add_css_class(GTK_WIDGET(w), "highlight");
    } else {
        gtk_widget_remove_css_class(GTK_WIDGET(w), "highlight");
    }
}

static void reset_hightlight(GtkWidget *w) { gtk_widget_remove_css_class(GTK_WIDGET(w), "highlight"); }

static void update_ui_watchers(void) {
    Watchers watchers = get_watchers_updated();

    gtk_editable_set_text(GTK_EDITABLE(pc_entry), watchers.PC->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(pch_entry), watchers.PCH->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(pcl_entry), watchers.PCL->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(h_entry), watchers.H->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(l_entry), watchers.L->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(sp_entry), watchers.SP->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(fc_entry), watchers.FC->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(fz_entry), watchers.FZ->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(b_entry), watchers.B->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(c_entry), watchers.C->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(d_entry), watchers.D->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(e_entry), watchers.E->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(ri_entry), watchers.RI->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(acum_entry), watchers.ACUM->hex_repr);
    gtk_editable_set_text(GTK_EDITABLE(op2_entry), watchers.OP2->hex_repr);

    highlight_changes(watchers.PC, pc_entry);
    highlight_changes(watchers.PCH, pch_entry);
    highlight_changes(watchers.PCL, pcl_entry);
    highlight_changes(watchers.H, h_entry);
    highlight_changes(watchers.L, l_entry);
    highlight_changes(watchers.SP, sp_entry);
    highlight_changes(watchers.FC, fc_entry);
    highlight_changes(watchers.FZ, fz_entry);
    highlight_changes(watchers.B, b_entry);
    highlight_changes(watchers.C, c_entry);
    highlight_changes(watchers.D, d_entry);
    highlight_changes(watchers.E, e_entry);
    highlight_changes(watchers.RI, ri_entry);
    highlight_changes(watchers.ACUM, acum_entry);
    highlight_changes(watchers.OP2, op2_entry);
}

static void ui_update_current_status(void) { gtk_editable_set_text(GTK_EDITABLE(current_state), get_current_state_updated()); }

static void ui_update_buses(void) {
    gtk_editable_set_text(GTK_EDITABLE(data_bus), get_data_bus_updated());
    gtk_editable_set_text(GTK_EDITABLE(dir_bus), get_dir_bus_updated());
    gtk_editable_set_text(GTK_EDITABLE(control_bus), get_control_bus_updated());
}

static void ui_update_mem(void) {
    MemUpdated mem_updated = get_mem_updated();

    gtk_tree_store_clear(mem_model);
    // what to do with large tree store?
    GtkTreeIter iter;

    // int min = offset_mem_loaded;
    int min = 0;
    int max = offset_mem_loaded + MEM_WIDGET_HEIGHT / ROW_HEIGHT;
    if (max > mem_updated.values_count) max = mem_updated.values_count;
    for (int i = min; i < max; i++) {
        gtk_tree_store_append(mem_model, &iter, NULL);
        gtk_tree_store_set(mem_model, &iter, 0, mem_updated.values[i].offset_hex, 1, mem_updated.values[i].value_hex, -1);
    }

    offset_mem_loaded = max;
}

static void on_programming_destroy(void) {
    ui_update_mem();
}
static void on_open_program_mode(void) {
    GtkWidget *win = open_programming_mode();
    g_signal_connect(G_OBJECT(win), "destroy", on_programming_destroy, NULL);
}

static void update_ui_mem_selection(void) {
    // select the first row
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    gtk_tree_selection_unselect_all(selection);

    GtkTreePath *path = gtk_tree_path_new_from_indices(word_to_int(get_watchers(1).PC->reg->value), -1);
    gtk_tree_selection_select_path(selection, path);

    // scroll to the first row
    GtkTreeViewColumn *column = gtk_tree_view_get_column(GTK_TREE_VIEW(tree_view), 0);
    gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(tree_view), path, column, TRUE, 0.5, 0.5);
}

static void ui_update_all(void) {
    log_debug("Received update event");
    update_ui_watchers();
    update_ui_mem_selection();
    ui_update_current_status();
    ui_update_buses();
}

static void on_update(void) { g_idle_add(ui_update_all, NULL); }

static GtkWidget *insert_vbox(GtkWidget *w, gboolean expand) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    // expand and fill
    gtk_box_set_homogeneous(GTK_BOX(vbox), expand);
    gtk_box_append(w, vbox);
    return vbox;
}

static GtkWidget *insert_hbox(GtkWidget *w, gboolean expand) {
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // expand
    gtk_box_set_homogeneous(GTK_BOX(hbox), expand);
    gtk_box_append(GTK_BOX(w), hbox);
    return hbox;
}

static void insert_horizontal_separator(GtkWidget *g) {
    GtkWidget *h = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    gtk_box_append(GTK_BOX(g), h);
}

static void insert_vertical_separator(GtkWidget *g) {
    GtkWidget *v = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(g), v);
}

static void insert_top_bar(GtkWidget *vbox) {
    // add a label to the second vertical box
    GtkWidget *label = gtk_label_new(APP_NAME);

    PangoAttrList *attrs = pango_attr_list_new();
    // set weight to bold
    pango_attr_list_insert(attrs, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    // set the font size to 20
    pango_attr_list_insert(attrs, pango_attr_size_new_absolute(20 * PANGO_SCALE));
    gtk_label_set_attributes(GTK_LABEL(label), attrs);
    gtk_box_append(GTK_BOX(vbox), label);
}

static void insert_action_buttons(GtkWidget *w) {
    GtkWidget *buttons_stack = insert_vbox(w, FALSE);
    GtkWidget *label = gtk_label_new("ACTIONS");

    gtk_box_append(GTK_BOX(buttons_stack), label);
    insert_horizontal_separator(buttons_stack);

    run_program_btn = gtk_button_new_with_label(RUN_PROGRAM_TEXT);
    g_signal_connect(run_program_btn, "clicked", G_CALLBACK(run_program), NULL);

    gtk_box_append(GTK_BOX(buttons_stack), run_program_btn);

    run_microinstr_btn = gtk_button_new_with_label(RUN_MICRO_INSTRUCTION_TEXT);
    g_signal_connect(run_microinstr_btn, "clicked", G_CALLBACK(run_miroinstruction), NULL);
    gtk_box_append(GTK_BOX(buttons_stack), run_microinstr_btn);

    run_state_btn = gtk_button_new_with_label(RUN_STATE_TEXT);
    gtk_button_set_label(GTK_BUTTON(run_state_btn), RUN_STATE_TEXT);
    g_signal_connect(run_state_btn, "clicked", G_CALLBACK(run_state), NULL);
    gtk_box_append(GTK_BOX(buttons_stack), run_state_btn);

    open_diagram_btn = gtk_button_new_with_label("Open diagram");
    g_signal_connect(open_diagram_btn, "clicked", G_CALLBACK(open_diagram), NULL);
    gtk_box_append(GTK_BOX(buttons_stack), open_diagram_btn);

    open_programming_mode_btn = gtk_button_new_with_label("Programming mode");
    g_signal_connect(open_programming_mode_btn, "clicked", G_CALLBACK(on_open_program_mode), NULL);
    gtk_box_append(GTK_BOX(buttons_stack), open_programming_mode_btn);
}

static void insert_current_state(GtkWidget *w) {
    GtkWidget *current_state_stack = insert_vbox(w, FALSE);
    GtkWidget *label = gtk_label_new("CURRENT STATE");

    gtk_box_append(GTK_BOX(current_state_stack), label);
    insert_horizontal_separator(current_state_stack);

    current_state = gtk_entry_new();
    gtk_editable_set_editable(GTK_ENTRY(current_state), FALSE);
    // set text entry size to 20
    gtk_editable_set_width_chars(GTK_ENTRY(current_state), 20);
    gtk_box_append(GTK_BOX(current_state_stack), current_state);
}

static void insert_memory_view(GtkWidget *w) {
    GtkWidget *vbox = insert_vbox(w, FALSE);
    GtkWidget *label = gtk_label_new("MEMORY");
    gtk_box_append(GTK_BOX(vbox), label);
    insert_horizontal_separator(vbox);

    // create a horizontal box: label | entry
    GtkWidget *hbox = insert_hbox(vbox, FALSE);
    GtkWidget *label_offset = gtk_label_new("Search");
    gtk_box_append(GTK_BOX(hbox), label_offset);

    search_mem_entry = gtk_entry_new();
    gtk_editable_set_width_chars(GTK_ENTRY(search_mem_entry), 20);
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_mem_entry), "Enter offset");
    gtk_box_append(GTK_BOX(hbox), search_mem_entry);
    g_signal_connect(search_mem_entry, "activate", G_CALLBACK(search_memory), NULL);

    // CREATE A SUBMIT BUTTON
    GtkWidget *submit_button = gtk_button_new_with_label("Search");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(search_memory), search_mem_entry);
    gtk_box_append(GTK_BOX(hbox), submit_button);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, -1, 200);
    g_signal_connect(scrolled_window, "edge-reached", G_CALLBACK(on_mem_end_reached), NULL);
    // select the first row

    gtk_box_append(GTK_BOX(vbox), scrolled_window);

    // create a tree view for a list with two columns: name and age
    tree_view = gtk_tree_view_new();
    // gtk_box_append(GTK_SCROLLED_WINDOW(scrolled_window), tree_view);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), tree_view);

    // create a model for the tree view
    mem_model = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(mem_model));

    // create a cell renderer for the name column
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Offset", renderer, "text", 0, NULL);

    // create a cell renderer for the age column
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree_view), -1, "Value", renderer, "text", 1, NULL);

    ui_update_mem();
}

static GtkWidget *insert_readonly_entry(GtkWidget *w, const char *label, const char *text) {
    // in a table of 1 row and 2 columns
    GtkWidget *box = insert_hbox(w, TRUE);

    // add a label to the first column
    GtkWidget *label_widget = gtk_label_new(label);
    gtk_box_append(GTK_BOX(box), label_widget);

    // add a text entry to the second column
    GtkWidget *entry = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry), text);
    gtk_editable_set_editable(GTK_ENTRY(entry), FALSE);
    gtk_box_append(GTK_BOX(box), entry);

    return entry;
}

static void insert_registers(GtkWidget *w) {
    GtkWidget *vbox = insert_vbox(w, FALSE);

    // a frame
    GtkWidget *frame = gtk_frame_new("REGISTERS");
    gtk_box_append(GTK_BOX(vbox), frame);

    // create a scrollable window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();

    gtk_frame_set_child(GTK_FRAME(frame), scrolled_window);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    // gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_window), TRUE);
    gtk_widget_set_size_request(scrolled_window, -1, 300);
    // create a box to hold the widgets
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), box);

    // create a text field read only
    pc_entry = insert_readonly_entry(box, "PC", "0x00");
    pch_entry = insert_readonly_entry(box, "PCH", "0x00");
    pcl_entry = insert_readonly_entry(box, "PCL", "0x00");
    h_entry = insert_readonly_entry(box, "H", "0x00");
    l_entry = insert_readonly_entry(box, "L", "0x00");
    sp_entry = insert_readonly_entry(box, "SP", "0x00");
    fc_entry = insert_readonly_entry(box, "FC", "0x00");
    fz_entry = insert_readonly_entry(box, "FZ", "0x00");
    b_entry = insert_readonly_entry(box, "B", "0x00");
    c_entry = insert_readonly_entry(box, "C", "0x00");
    d_entry = insert_readonly_entry(box, "D", "0x00");
    e_entry = insert_readonly_entry(box, "E", "0x00");
    ri_entry = insert_readonly_entry(box, "RI", "0x00");
    acum_entry = insert_readonly_entry(box, "ACUM", "0x00");
    op2_entry = insert_readonly_entry(box, "2OP", "0x00");

    update_ui_watchers();
}

static void insert_buses(GtkWidget *w) {
    GtkWidget *vbox = insert_vbox(w, FALSE);

    // a frame
    GtkWidget *frame = gtk_frame_new("BUSES");
    gtk_box_append(GTK_BOX(vbox), frame);

    // create a scrollable window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();

    gtk_frame_set_child(GTK_FRAME(frame), scrolled_window);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    // gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_window), TRUE);
    gtk_widget_set_size_request(scrolled_window, -1, 300);
    // create a box to hold the widgets
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), box);

    // create a text field read only
    data_bus = insert_readonly_entry(box, "DATA BUS", "0x00");
    dir_bus = insert_readonly_entry(box, "DIRECTION BUS", "0x00");
    control_bus = insert_readonly_entry(box, "CONTROL BUS", "0x00");
    ui_update_buses();
}

static void insert_alu(GtkWidget *w) {
    GtkWidget *vbox = insert_vbox(w, FALSE);
    // a frame
    GtkWidget *frame = gtk_frame_new("ALU");
    gtk_box_append(GTK_BOX(vbox), frame);

    // create a scrollable window
    GtkWidget *scrolled_window = gtk_scrolled_window_new();

    gtk_frame_set_child(GTK_FRAME(frame), scrolled_window);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    // gtk_scrolled_window_set_propagate_natural_height(GTK_SCROLLED_WINDOW(scrolled_window), TRUE);
    gtk_widget_set_size_request(scrolled_window, -1, 300);
    // create a box to hold the widgets
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), box);

    // create a text field read only
    insert_readonly_entry(box, "A", "00000000");
    insert_readonly_entry(box, "B", "00000000");
}

static void insert_statusbar(GtkWidget *w) {
    GtkWidget *statusbar = gtk_statusbar_new();
    // create a horizontal box and append it to the statusbar
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_append(GTK_BOX(w), statusbar);
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, "Ready");
}

static void present(void) { gtk_window_present(GTK_WINDOW(window)); }

static void activate(GtkApplication *app, gpointer user_data) {
    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), APP_NAME);

    // on window close
    // g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", on_destroy, NULL);
    // load css from file
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    const gchar *css_file = "src/gui/style.css";
    GError *error = 0;
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(css_file));
    g_object_unref(provider);

    // print error
    if (error) {
        g_print("%s\n", error->message);
        g_error_free(error);
    }

    // create a vertical box
    GtkWidget *root_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), root_vbox);

    insert_top_bar(root_vbox);

    insert_horizontal_separator(root_vbox);

    GtkWidget *hbox1 = insert_hbox(root_vbox, TRUE);
    insert_action_buttons(hbox1);
    insert_current_state(hbox1);
    insert_memory_view(hbox1);

    GtkWidget *hbox2 = insert_hbox(root_vbox, TRUE);
    insert_registers(hbox2);
    insert_buses(hbox2);
    insert_alu(hbox2);

    insert_statusbar(root_vbox);

    present();
}

int init_gui(void) {
    set_update_ui(on_update);
    set_run_end(on_run_end);

    GtkApplication *app = gtk_application_new("me.nullx.mpp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), NULL, NULL);

    g_object_unref(app);

    return status;
}