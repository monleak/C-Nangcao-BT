#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

//==========declare widget==========
GtkBuilder  *builder;
GtkWidget   *window;
GtkWidget   *button1;
GtkWidget   *label1;
GtkWidget   *view1;
GtkWidget   *search1;
GtkListStore    *liststore1;
GtkTreeView *treeview1;
GtkTreeViewColumn   *column1;
GtkTreeViewColumn   *column2;
GtkTreeSelection    *selection1;
GtkCellRenderer *cr1;
GtkCellRenderer *cr2;
GtkTreeModelFilter *filtered;
GtkTreeModelSort *sorted;

gchar *searchValue = "";

GtkWidget   *addluachon;
GtkWidget   *addfile;
GtkWidget   *addthucong;

//==============================add feature==============================
// void on_cacel_btn2_clicked() {
//     // gtk_container_remove(addluachon);
//      gtk_window_close(GTK_WINDOW(addluachon));
//     // gtk_widget_hide(addluachon);
// }

gboolean on_window2_destroy_event(GtkWidget *widget,
                                GdkEvent  *event,
                                gpointer   user_data) {

    return TRUE;
}


void on_button_create_clicked (GtkButton *b) {
    printf ("handle_create\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");

    if(addluachon)
        gtk_widget_show_all(addluachon);
}

void add_thucong (GtkButton *b) {
    printf ("handle_create\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");

    if(addthucong){
        gtk_widget_show_all(addthucong);
        gtk_window_close(GTK_WINDOW(addluachon));
    }
}

void add_file (GtkButton *b) {
    printf ("handle_create\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");

    if(addfile){
        gtk_widget_show_all(addfile);
        gtk_window_close(GTK_WINDOW(addluachon));
    }
}
//==============================handle signal==============================


void on_selection1_changed(GtkWidget *c){
    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected(
        GTK_TREE_SELECTION(c), &model, &iter) == FALSE)
        return;

    gtk_tree_model_get(model, &iter, 0, &value, -1);
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) value);
    printf("SELECTED: %s\n", value);
}

void on_search1_search_changed(GtkSearchEntry *s) {
    const gchar *srch;
    GtkTreeIter iter;

    srch = gtk_entry_get_text((GtkEntry *)s);
    searchValue = (char *)srch;
    gtk_tree_model_filter_refilter (filtered);

    if(strlen(srch) == 0){
        gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "");
    }else{
        gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) srch);
    }
}

// void on_treeview1_start_interactive_search(GtkTreeView *t) {
//     printf("interactive_search\n");
// }

// gboolean on_treeview1_key_press_event(GtkTreeView *t) {
//     printf("on_treeview1_key_press_event\n");
//     return FALSE;
// }


//==============================filter==============================
static gboolean
row_visible (GtkTreeModel *model,
             GtkTreeIter *iter,
             gpointer data)
{
    gchar* value;

    gtk_tree_model_get (model, iter, 0, &value, -1);
    // printf("%s: %s: %d\n",searchValue, value, strstr(value, searchValue) ? 1 : 0);

    gboolean ret = strstr(value, searchValue) ? (gboolean)TRUE : (gboolean)FALSE;
    return ret;
}

static void
on_row_activated (GtkTreeView *view,
                  GtkTreePath *path,
                  GtkTreeViewColumn *col)
{
    GtkTreeIter iter;
    GtkTreePath *filtered_path;
    GtkTreePath *child_path;
    GtkTreeModel *model;

    // filtered_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (sorted),
                                                                    // path);

    child_path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER (filtered),
                                                                   filtered_path);

    if (gtk_tree_model_get_iter (GTK_TREE_MODEL (liststore1), &iter, child_path)) {
        gchar *name;
        gchar *number;

        gtk_tree_model_get (GTK_TREE_MODEL (liststore1), &iter,
                            0, &name,
                            1, &number,
                            -1);
    }
}


//==============================main==============================
int main(int argc, char *argv[]) {

    //init
    gtk_init(&argc, &argv);

    //.glade file
    builder = gtk_builder_new_from_file("../phonebook-list.glade");
    gtk_builder_connect_signals(builder, NULL);     //CONNECT SIGNAL FROM .GLADE FILE

    //window
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //widget
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    view1 =  GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
    search1 = GTK_WIDGET(gtk_builder_get_object(builder, "search1"));
    treeview1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview1"));
    liststore1 = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
    column1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "column1"));
    column2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "column2"));
    cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr1"));
    cr2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr2"));
    selection1 = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selection1"));
    filtered = GTK_TREE_MODEL_FILTER(gtk_builder_get_object(builder, "filter1"));
    sorted = GTK_TREE_MODEL_SORT(gtk_builder_get_object(builder, "sort1"));

    addluachon = GTK_WIDGET(gtk_builder_get_object(builder, "addluachon"));
    gtk_window_set_title (GTK_WINDOW (addluachon), "Add Contact");

    addthucong = GTK_WIDGET(gtk_builder_get_object(builder, "addthucong"));
    gtk_window_set_title (GTK_WINDOW (addthucong), "Add Contact");

    addfile = GTK_WIDGET(gtk_builder_get_object(builder, "addfile"));
    gtk_window_set_title (GTK_WINDOW (addfile), "Add Contact");


    //set filter function
    gtk_tree_model_filter_set_visible_func (filtered,
                                            (GtkTreeModelFilterVisibleFunc) row_visible,
                                            NULL, NULL);

    selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview1));

    //attach the renderer to the column
    gtk_tree_view_column_add_attribute(column1, cr1, "text", (gint)0);
    gtk_tree_view_column_add_attribute(column2, cr2, "text", (gint)1);

    //iterators
    GtkTreeIter iter;

    //add mock data
    for(int i = 0; i < 10; i++) {
        char *a = calloc( 10, sizeof(char));
        char b[2];
        b[0] = i+'a';
        b[1] = '\0';
        strcpy(a, "hello");
        strcat(a, b);
        printf("%s\n", a);
      // Add a new row to the model
      gtk_list_store_append (liststore1, &iter);
      gtk_list_store_set (liststore1, &iter,
                    0, a,
                    1, "world");
    }

    //show window
    gtk_widget_show_all(window);
    gtk_main();

    return EXIT_SUCCESS;
}
