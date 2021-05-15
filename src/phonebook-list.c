#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>

#include "../inc/utility.h"
#include "../inc/db-main.h"

/*TODO: Sửa những chỗ có kieu dl:  gint column ->  enum:
                                    0           ->  COLUMN_NAME
                                    1           ->  COLUMN_SDT
                                    số cột      ->  NUM_COLUMNS
*/
enum
{
  COLUMN_NAME,
  COLUMN_SDT,
  NUM_COLUMNS
};

//==========declare widget==========
/*TODO: Đổi tên biến */
GtkBuilder  *builder;
GtkWidget   *window;
GtkWidget   *button1;
GtkWidget   *button1;
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
GtkWidget   *validateLabel;

GtkWidget   *window2;
GtkEntry   *entry_name;
GtkEntry   *entry_number;


//==============================feature: add==============================
void close_phonebook()
{
    close_db();
}
void close_addthucong() {
    gtk_widget_hide_on_delete (addthucong);
}

void on_ok_btn1_clicked() {
    char *name = (char *)gtk_entry_get_text(entry_name);
    char *number = (char *)gtk_entry_get_text(entry_number);
    enum PhonebookStrErr nameErr = validate_name(name);
    enum PhonebookStrErr numberErr = validate_number(number);

    if(nameErr == STR_OK && numberErr == STR_OK) 
    {
        int success = 0;
        success = insert_db(name, number);
        gtk_widget_hide(addthucong);
        if(success) 
        {
            GtkTreeIter iter;

            gtk_list_store_append (liststore1, &iter);       
            gtk_list_store_set (liststore1, &iter,
                    0, name,
                    1, number,
                    -1);
        }
        gtk_entry_set_text(entry_name, "");
        gtk_entry_set_text(entry_number, "");
        printf("add %s\n", name);
    }else if(nameErr != STR_OK) 
    {
        switch (nameErr)
        {
            case LEN_EQUAL_ZERO:
                gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Tên không được để trống");
                break;
            case FIRST_IS_SPACE:
                gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Tên không được bắt đầu bằng dấu cách");
                break;
            case END_IS_SPACE:
                gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Tên không được kết thúc bằng dấu cách");
                break;
        };

    }else if(numberErr != STR_OK) 
    {
        switch (numberErr)
        {
            case LEN_EQUAL_ZERO:
                gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Số điện thoại không được để trống");
                break;
            case NUMBER_10:
                gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Số điện thoại ít nhất là 10 số");
                break;
        };
    }
}


// gboolean on_window2_destroy_event(GtkWidget *widget,
//                                 GdkEvent  *event,
//                                 gpointer   user_data) {

//     return TRUE;
// }


void click_create (GtkButton *b) {
    printf ("handle_create\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");

    if(addluachon){
        gtk_widget_show_all(addluachon);
    }
}

void click_add_thucong (GtkButton *b) {
    printf ("handle_create\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");

    if(addthucong){
        gtk_widget_show_all(addthucong);
        gtk_widget_hide_on_delete(addluachon);
    }
}

void click_add_file (GtkButton *b) {
    printf ("handle_create\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");

    if(addfile){
        gtk_widget_show_all(addfile);
        gtk_widget_hide_on_delete(addluachon);
    }
}
void close_addfile(GtkButton *b)
{
    if(addfile) 
        gtk_widget_hide_on_delete(addfile);
}
void close_addluachon(GtkButton *b)
{
    gtk_widget_hide_on_delete(addluachon);
}
//phone number entry acept only number
void insert_text_event(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data)
{
    int i;

    for (i = 0; i < length; i++) {
        if (!isdigit(text[i])) {
            g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
            return;
        }
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

//=============================update==============================
void
cell_edited (GtkCellRendererText *cell,
             const gchar         *path_string,
             const gchar         *new_text,
             gpointer             data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
  GtkTreeIter iter;

  gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

  gtk_tree_model_get_iter (model, &iter, path);

    gchar *old_text;

    gtk_tree_model_get (model, &iter, column, &old_text, -1);
    g_free (old_text);

                /*FIXME: Lỗi khi sửa cột SDT*/
                /*FIXME: Chưa validate dl*/
                /*FIXME: Chưa update dữ liệu vào database*/

    gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                column, new_text, 
                -1);

  gtk_tree_path_free (path);
}

void click_update (GtkButton *b) {

    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Double click to update");

}

//==============================main==============================
int main(int argc, char *argv[]) 
{

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

    /*FIXME: Có thể thay editable ở file glade*/
    g_object_set (cr1,
                "editable", TRUE,
                NULL);
    g_object_set (cr2,
                "editable", TRUE,
                NULL);
    /*FIXME: Sửa khi bấm nút update mới có thể sửa giá trị*/
    g_signal_connect (cr1, "edited",
                G_CALLBACK (cell_edited), liststore1);
    g_signal_connect (cr2, "edited",
                G_CALLBACK (cell_edited), liststore1);

    window2 = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    entry_name = GTK_ENTRY(gtk_builder_get_object(builder, "entry3"));
    entry_number = GTK_ENTRY(gtk_builder_get_object(builder, "entry4"));
    validateLabel = GTK_WIDGET(gtk_builder_get_object(builder, "validate_label"));
    // gtk_window_set_title (GTK_WINDOW (window2), "Add Contact");

    //HIEU
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

    // for(int i = 0; i < 10; i++) {
    //     char *a = calloc( 10, sizeof(char));
    //     char b[2];
    //     b[0] = i+'a';
    //     b[1] = '\0';
    //     strcpy(a, "hello");
    //     strcat(a, b);
    //     printf("%s\n", a);
    //   // Add a new row to the model
    //   gtk_list_store_append (liststore1, &iter);
    //   gtk_list_store_set (liststore1, &iter,
    //                 0, a,
    //                 1, "world",
    //                  -1);
    // }

//========= Lay du lieu tu database =======
    open_and_create_db();
    push_to_GUI(builder);




    //show window
    gtk_widget_show_all(window);
    gtk_main();
    close_db();
    return EXIT_SUCCESS;
}
