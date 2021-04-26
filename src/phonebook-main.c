#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

//declare widget
GtkBuilder	*builder;
GtkWidget	*window;
GtkWidget	*button1;
GtkWidget	*label1;
GtkWidget	*view1;
GtkWidget	*search1;
GtkTreeStore	*treestore1;
GtkTreeView	*treeview1;
GtkTreeViewColumn	*column1;
GtkTreeViewColumn	*column2;
GtkTreeSelection	*selection1;
GtkCellRenderer	*cr1;
GtkCellRenderer	*cr2;

GtkTreeModelFilter *filtered;
GtkTreeModelSort *sorted;



//handle signal
void on_button_create_clicked (GtkButton *b) {
	printf ("handle_create\n");
	gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "Handle btn create");
}

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

//search
// void on_search1_search_changed(GtkSearchEntry *s) {
// 	const gchar *srch;
// 	GtkTreeIter iter;

// 	GtkTreeModel *model = gtk_tree_view_get_model(treeview1);

// 	srch = gtk_entry_get_text((GtkEntry *)s);

// 	if(strlen(srch) == 0){
// 		gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) "");
// 	 	return;
// 	}
// 	gtk_label_set_text (GTK_LABEL(label1), (const gchar* ) srch);

// 	// printf("\n", );
// }


void on_search1_search_changed(GtkSearchEntry *s) {
	const gchar *srch;
	GtkTreeIter iter;

	GtkTreeModel *model = gtk_tree_view_get_model(treeview1);

	srch = gtk_entry_get_text((GtkEntry *)s);

	gtk_tree_model_filter_refilter (filtered);

}



void on_treeview1_start_interactive_search(GtkTreeView *t) {
	printf("interactive_search\n");
}

gboolean on_treeview1_key_press_event(GtkTreeView *t) {
	printf("on_treeview1_key_press_event\n");
	return FALSE;
}

//filter
static gboolean
row_visible (GtkTreeModel *model,
             GtkTreeIter *iter)
{
    gchar* value;
    gchar* str = "a";

    gtk_tree_model_get (model, iter, 0, &value, -1);

    return strstr(value, str) ? 1 : 0;
}

static void
on_row_activated (GtkTreeView *view,
                  GtkTreePath *path,
                  GtkTreeViewColumn *col)
{
    GtkTreeIter iter;
    GtkTreePath *filtered_path;
    GtkTreePath *child_path;

    filtered_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (sorted),
                                                                    path);

    child_path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER (filtered),
                                                                   filtered_path);

    if (gtk_tree_model_get_iter (GTK_TREE_MODEL (column1), &iter, child_path)) {
        gchar *article;
        gdouble price;

        gtk_tree_model_get (GTK_TREE_MODEL (column1), &iter,
                            column1, &article,
                            column2, &price,
                            -1);

        g_print ("You want to buy %s for %f?\n", article, price);
        g_free (article);
    }
}

//-----------------main--------------------------
int main(int argc, char *argv[]) {

	//init
  	gtk_init(&argc, &argv);

  	//.glade file
	builder = gtk_builder_new_from_file("../list_view.glade");
	gtk_builder_connect_signals(builder, NULL);		//CONNECT SIGNAL FROM .GLADE FILE

	//window
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	//widget
	button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
	label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	view1 =  GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
	search1 = GTK_WIDGET(gtk_builder_get_object(builder, "search1"));
	treeview1 = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview1"));
	treestore1 = GTK_TREE_STORE(gtk_builder_get_object(builder, "treestore1"));
	column1 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "column1"));
	column2 = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "column2"));
	cr1 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr1"));
	cr2 = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "cr2"));
	selection1 = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selection1"));
	filtered = GTK_TREE_MODEL_FILTER(gtk_builder_get_object(builder, "filter1"));
    sorted = GTK_TREE_MODEL_SORT(gtk_builder_get_object (builder, "sort1"));
//filter
    gtk_tree_model_filter_set_visible_func (filtered,
                                            (GtkTreeModelFilterVisibleFunc) row_visible,
                                            NULL, NULL);


	// attach the renderer to the column
	gtk_tree_view_column_add_attribute(column1, cr1, "text", (gint)0);
	gtk_tree_view_column_add_attribute(column2, cr2, "text", (gint)1);

	//iterators
	GtkTreeIter iter;
	GtkTreeIter iterChild1;
	GtkTreeIter iterChild2;

	//add row
	gtk_tree_store_append(treestore1, &iter, NULL);
	gtk_tree_store_set(treestore1, &iter, 0,"row1", -1);
	gtk_tree_store_set(treestore1, &iter, 1,"row1 data", -1);

	gtk_tree_store_append(treestore1, &iterChild1, &iter);
	gtk_tree_store_set(treestore1, &iterChild1, 0,"row1child", -1);
	gtk_tree_store_set(treestore1, &iterChild1, 1,"row1 data", -1);

	gtk_tree_store_append(treestore1, &iter, NULL);
	gtk_tree_store_set(treestore1, &iter, 0,"row2", -1);
	gtk_tree_store_set(treestore1, &iter, 1,"row2 data", -1);

	gtk_tree_store_append(treestore1, &iterChild2, &iter);
	gtk_tree_store_set(treestore1, &iterChild2, 0,"row2child", -1);
	gtk_tree_store_set(treestore1, &iterChild2, 1,"row2child data", -1);

//fix:
	// for(int i = 0; i < 10; i++) {
	// 	gtk_tree_store_append(treestore1, &iter, NULL);
	// 	gtk_tree_store_set(treestore1, &iter, 0, "hello", -1);
	// 	gtk_tree_store_set(treestore1, &iter, 1, "world", -1);
	// }
//
	selection1 = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview1));

	//show window
	gtk_widget_show_all(window);
	gtk_main();

  	return EXIT_SUCCESS;
}
