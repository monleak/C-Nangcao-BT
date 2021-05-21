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
/*TODO: Memory leak, giải phóng dữ liệu*/
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
GtkWidget   *file1;
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
GtkWidget   *addthucong;
GtkWidget   *validateLabel;
GtkWidget   *window_err;
GtkEntry    *entry_name;
GtkEntry    *entry_number;

GtkWidget   *update_window;
GtkEntry    *update_entry_name;
GtkEntry    *update_entry_sdt;
GtkWidget   *update_validate_label;
GtkWidget   *update_ok_btn;
GtkWidget   *update_cacel_btn;

GtkWidget   *xacnhandelete;
GtkWidget   *delete_name;
GtkWidget   *delete_number;

//==============================feature: add==============================
void close_addthucong() {
    printf ("[ Close addthucong window ]\n");
    gtk_widget_hide_on_delete (addthucong);
}

void chon_file(GtkFileChooserButton *f)
{
    insert_db_from_file(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f)),builder);
    printf ("[ Close addluachon window ]\n");
    gtk_widget_hide_on_delete(addluachon);
}

void on_ok_btn1_clicked() {
    char *name      = (char *)gtk_entry_get_text(entry_name);
    char *number    = (char *)gtk_entry_get_text(entry_number);
    int nameErr     = validate_name(name);
    int numberErr   = validate_number(number);

    if(is_exists_in_db("name", name))   //tên mới trùng với 1 tên khác trong csdl
    {
        gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Tên đã tồn tại");
    }else if(is_exists_in_db("number", number)) //số mới trùng với 1 số khác trong csdl
    {
        gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Số điện thoại đã tồn tại");
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
            case NUMBER_FIRST_ERR:
                gtk_label_set_text (GTK_LABEL(validateLabel), (const gchar* ) "Số điện thoại phải bắt đầu bằng 0");
                break;
        };
    }else{
        int success = 0;
        success = insert_db(name, number,builder);
        gtk_widget_hide(addthucong);
        if(success) 
        {
            GtkTreeIter iter;

            gtk_list_store_append (liststore1, &iter);       
            gtk_list_store_set (liststore1, &iter,
                    COLUMN_NAME, name,
                    COLUMN_SDT, number,
                    -1);
        }
        gtk_entry_set_text(entry_name, "");
        gtk_entry_set_text(entry_number, "");
    }
}

void click_create (GtkButton *b) {
    printf ("[ Open Create window ]\n");
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "");
    if(addluachon){
        gtk_widget_show_all(addluachon);
    }
}

void click_add_thucong (GtkButton *b) {
    printf ("[ Close create window ]\n");
    printf ("[ Open Addthucong window ]\n");

    if(addthucong){
        gtk_widget_show_all(addthucong);
        gtk_widget_hide_on_delete(addluachon);
    }
}

void close_addluachon(GtkButton *b)
{
    printf ("[ Close create window ]\n");
    gtk_widget_hide_on_delete(addluachon);
}
//phone number entry acept only number
void insert_text_event(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data)
{
    int i;

    for (i = 0; i < length; i++) {
        if (!isdigit(text[i]) && text[i] != '+') {
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
    // gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) value);
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
    GtkTreeModel *model;

    filtered_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (sorted),
                                                                    path);

    if (gtk_tree_model_get_iter (GTK_TREE_MODEL (liststore1), &iter, path)) {
        gchar *name;
        gchar *number;

        gtk_tree_model_get (GTK_TREE_MODEL (liststore1), &iter,
                            0, &name,
                            1, &number,
                            -1);
    }
}

//=============================update==============================

void click_update (GtkButton *b) {
    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "");
    if(gtk_tree_selection_get_selected(selection1, &model, &iter) == FALSE)
    {
        printf("Chọn liên hệ muốn chỉnh sửa!\n");
        gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Chọn liên hệ muốn chỉnh sửa!");
    }else{
        gtk_tree_model_get(model, &iter, 0, &value, -1);
        // gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) value);

        gchar *old_name;
        gchar *old_number;

        gtk_tree_model_get(model, &iter, COLUMN_NAME, &old_name,
                                         COLUMN_SDT, &old_number,
                                          -1);
        gtk_entry_set_text(update_entry_name, old_name);
        gtk_entry_set_text(update_entry_sdt, old_number);

        gtk_widget_show_all(update_window);
        printf ("[ Open Update window ]\n");
    }
}

void cacel_update() {
    gtk_widget_hide_on_delete (update_window);
    printf ("[ Close update window ]\n");
}

void on_update_ok_btn_clicked(GtkButton *b) {
    GtkTreeIter iter;
    GtkTreeModel *model = (GtkTreeModel *) liststore1;
    GtkTreePath *path;
    gtk_tree_selection_get_selected(selection1, &model, &iter);
    path = gtk_tree_model_get_path(model, &iter);
    gchar *old_name;
    gchar *old_number;

    gtk_tree_model_get(model, &iter, COLUMN_NAME, &old_name,
                                     COLUMN_SDT, &old_number,
                                     -1);

    char *name          = (char *)gtk_entry_get_text(update_entry_name);
    char *number        = (char *)gtk_entry_get_text(update_entry_sdt);

    int nameErr         = validate_name(name);
    int numberErr       = validate_number(number);
    int isExistsName    = is_exists_in_db("name", name);
    int isExistsNumber  = is_exists_in_db("number", number);

    //log
    printf("UPDATE\n");
    printf("{\n");
    printf("\t'old_name':\t\t%s\n", old_name);
    printf("\t'old_number':\t\t%s\n", old_number);
    printf("\t'new_name':\t\t%s\n", name);
    printf("\t'new_number':\t\t%s\n", number);
    printf("\t'is_exists_name':\t%d\n", isExistsName);
    printf("\t'is_exists_number':\t%d\n", isExistsNumber);
    printf("}\n");

    if(isExistsName && strcmp(old_name, name) != 0)   //tên mới khác tên cũ và trùng với 1 tên khác trong csdl
    {
        gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Tên đã tồn tại");
    }else if(isExistsNumber && strcmp(old_number, number) != 0) //số mới khác số cũ và trùng với 1 số khác trong csdl
    {
        gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Số điện thoại đã tồn tại");
    }else if(strcmp(old_name, name) == 0 && strcmp(old_number, number) == 0)  //tên mới trùng tên cũ và số trùng số cũ
    {
        gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Bạn chưa thay đổi giá trị nào");
    }else if(nameErr != STR_OK) 
    {
        switch (nameErr)
        {
            case LEN_EQUAL_ZERO:
                gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Tên không được để trống");
                break;
            case FIRST_IS_SPACE:
                gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Tên không được bắt đầu bằng dấu cách");
                break;
            case END_IS_SPACE:
                gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Tên không được kết thúc bằng dấu cách");
                break;
        };
    }else if(numberErr != STR_OK) 
    {
        switch (numberErr)
        {
            case LEN_EQUAL_ZERO:
                gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Số điện thoại không được để trống");
                break;
            case NUMBER_10:
                gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Số điện thoại ít nhất là 10 số");
                break;
            case NUMBER_FIRST_ERR:
                gtk_label_set_text (GTK_LABEL(update_validate_label), (const gchar* ) "Số điện thoại phải bắt đầu bằng 0 hoặc +");
                break;
        };
    }else{
        gtk_widget_hide_on_delete(update_window);
        printf ("[ Close update window ]\n");
        if(update_db(old_name,name,number,builder))
        {
            gtk_tree_model_get_iter (GTK_TREE_MODEL (liststore1),
                           &iter,
                           path);
            gtk_tree_path_free (path);
            gtk_list_store_set (liststore1, &iter,
                                COLUMN_NAME, name,
                                COLUMN_SDT, number,
                                -1);
        }
        gtk_entry_set_text(update_entry_name, "");
        gtk_entry_set_text(update_entry_sdt, "");
    }
}
//==============================DELETE==============================
void click_delete (GtkButton *b) {
    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;
    gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "");
    if(gtk_tree_selection_get_selected(selection1, &model, &iter) == FALSE)
    {
        printf("Chọn liên hệ muốn xóa!\n");
        gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Chọn liên hệ muốn xóa!");
    }else{
        gtk_tree_model_get(model, &iter, 0, &value, -1);
        // gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) value);

        gchar *old_name;
        gchar *old_number;

        gtk_tree_model_get(model, &iter, COLUMN_NAME, &old_name,
                                         COLUMN_SDT, &old_number,
                                          -1);
        gtk_label_set_text (GTK_LABEL(delete_name), old_name);
        gtk_label_set_text (GTK_LABEL(delete_number), old_number);
        gtk_widget_show_all(xacnhandelete);
    }
}
void close_delete()
{
    gtk_widget_hide_on_delete (xacnhandelete);
}
void click_xacnhan_delete()
{
    gchar *value;
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreePath *path;
    gtk_tree_selection_get_selected(selection1, &model, &iter);
    path = gtk_tree_model_get_path(model, &iter);
    // gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) value);

    gchar *name;

    gtk_tree_model_get(model, &iter, COLUMN_NAME, &name,
                                      -1);
    if(delete_db(name,builder))
    {
        gtk_tree_model_get_iter (GTK_TREE_MODEL (liststore1),
                           &iter,
                           path);
        gtk_tree_path_free (path);
        gtk_list_store_remove (liststore1, &iter);
        gtk_widget_hide_on_delete(xacnhandelete);
    }
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
    //main_window
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

    //>>>>>GTK_TREE_VIEW in main_window
    //set filter function
    gtk_tree_model_filter_set_visible_func (filtered,
                                            (GtkTreeModelFilterVisibleFunc) row_visible,
                                            NULL, NULL);
    //selection
    selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview1));
    //attach the renderer to the column
    gtk_tree_view_column_add_attribute(column1, cr1, "text", COLUMN_NAME);
    gtk_tree_view_column_add_attribute(column2, cr2, "text", COLUMN_SDT);
    //<<<<<

    //create_window
    file1 = GTK_WIDGET(gtk_builder_get_object(builder, "chon_file"));
    entry_name = GTK_ENTRY(gtk_builder_get_object(builder, "entry3"));
    entry_number = GTK_ENTRY(gtk_builder_get_object(builder, "entry4"));
    validateLabel = GTK_WIDGET(gtk_builder_get_object(builder, "validate_label"));
    addluachon = GTK_WIDGET(gtk_builder_get_object(builder, "addluachon"));
    gtk_window_set_title (GTK_WINDOW (addluachon), "Add Contact");
    addthucong = GTK_WIDGET(gtk_builder_get_object(builder, "addthucong"));
    gtk_window_set_title (GTK_WINDOW (addthucong), "Add Contact");

    //update_window
    update_window           = GTK_WIDGET(gtk_builder_get_object(builder, "update_window"));
    update_entry_name       = GTK_ENTRY(gtk_builder_get_object(builder, "update_entry_name"));
    update_entry_sdt        = GTK_ENTRY(gtk_builder_get_object(builder, "update_entry_sdt"));
    update_validate_label   = GTK_WIDGET(gtk_builder_get_object(builder, "update_validate_label"));
    update_ok_btn           = GTK_WIDGET(gtk_builder_get_object(builder, "update_ok_btn"));
    update_cacel_btn        = GTK_WIDGET(gtk_builder_get_object(builder, "update_cacel_btn"));
    gtk_window_set_title (GTK_WINDOW (update_window), "Update");

    //delete_window
    xacnhandelete           = GTK_WIDGET(gtk_builder_get_object(builder, "xacnhandelete"));
    gtk_window_set_title (GTK_WINDOW (xacnhandelete), "Delete");
    delete_name = GTK_WIDGET(gtk_builder_get_object(builder, "delete_name"));
    delete_number = GTK_WIDGET(gtk_builder_get_object(builder, "delete_number"));


    //iterators
    GtkTreeIter iter;

//========= Lay du lieu tu database =======
    open_and_create_db(builder);
    push_to_GUI(builder);

    //show window
    gtk_widget_show_all(window);
    gtk_main();
    close_db();
    return EXIT_SUCCESS;
}
