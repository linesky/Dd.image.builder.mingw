#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *entry_size;
GtkWidget *entry_filename;
GtkWidget *textview_report;
GtkTextBuffer *textbuffer_report;
//gcc -o dd_interface dd_interface.c `pkg-config --cflags --libs gtk+-3.0`
void build_file(const char *filename, int size_mb);

void on_build_button_clicked(GtkWidget *widget, gpointer data) {
    const char *size_str = gtk_entry_get_text(GTK_ENTRY(entry_size));
    const char *filename = gtk_entry_get_text(GTK_ENTRY(entry_filename));
    
    int size_mb = atoi(size_str);
    if (size_mb <= 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Please enter a valid size in megabytes.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        build_file(filename, size_mb);
    }
}

void build_file(const char *filename, int size_mb) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to create file.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    char *buffer = (char *)calloc(1, 1024 * 1024); // 1MB buffer filled with zeros
    if (!buffer) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed to allocate memory.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        fclose(file);
        return;
    }

    GtkTextIter iter;
    for (int i = 0; i < size_mb; i++) {
        fwrite(buffer, 1, 1024 * 1024, file);

        char report[256];
        snprintf(report, sizeof(report), "Written %d MB of %d MB\n", i + 1, size_mb);
        
        gtk_text_buffer_get_end_iter(textbuffer_report, &iter);
        gtk_text_buffer_insert(textbuffer_report, &iter, report, -1);
        while (gtk_events_pending()) gtk_main_iteration();
    }

    free(buffer);
    fclose(file);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "File created successfully.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "DD Interface");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1.0, 1.0, 0.0, 1.0}); // Fundo amarelo

    GtkWidget *label_size = gtk_label_new("Size (MB):");
    gtk_fixed_put(GTK_FIXED(fixed), label_size, 20, 20);
    entry_size = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_size), "100");
    gtk_fixed_put(GTK_FIXED(fixed), entry_size, 80, 25);

    GtkWidget *label_filename = gtk_label_new("Filename:");
    gtk_fixed_put(GTK_FIXED(fixed), label_filename, 20, 60);
    entry_filename = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_filename), "output.img");
    gtk_fixed_put(GTK_FIXED(fixed), entry_filename, 200, 25);

    GtkWidget *label_report = gtk_label_new("Report:");
    gtk_fixed_put(GTK_FIXED(fixed), label_report, 20, 100);
    textview_report = gtk_text_view_new();
    textbuffer_report = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview_report));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview_report), FALSE);
    gtk_fixed_put(GTK_FIXED(fixed), textview_report, 90, 140);

    GtkWidget *button_build = gtk_button_new_with_label("Build");
    gtk_fixed_put(GTK_FIXED(fixed), button_build, 100, 65);
    g_signal_connect(button_build, "clicked", G_CALLBACK(on_build_button_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

