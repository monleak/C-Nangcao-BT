#include <gtk/gtk.h>

GtkTreeModel * create_and_fill_model()
{
GtkListStore  *store;
GdkPixbuf     *icon;
GError        *error = NULL;
GtkTreeIter    iter;
store = gtk_list_store_new (9,
                            G_TYPE_STRING,
                            G_TYPE_STRING,
                            G_TYPE_STRING,
                            G_TYPE_STRING,
                            G_TYPE_STRING,
                            G_TYPE_STRING,
                            G_TYPE_UINT,
                            G_TYPE_FLOAT,
                            GDK_TYPE_PIXBUF);
gtk_list_store_append (store, &iter);
icon = gdk_pixbuf_new_from_file("edit.png", &error);
gtk_list_store_set (store, &iter,
                    0, "A5525654",
                    1, "DFFFCCN",
                    2, "Doe",
                    3, "John",
                    4, "M",
                    5, "casablanca",
                    6, 2014,
                    7, 18.22,
                    8,icon,
                    -1);

  for (int i = 0; i < 10; i++)
    {
        char a = (char)(i + 'a');
      // Add a new row to the model
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
                    0, &a,
                    1, "DFFFCCN",
                    2, "Doe",
                    3, "John",
                    4, "M",
                    5, "casablanca",
                    6, 2014,
                    7, 18.22,
                    8,icon,
                    -1);
      // As the store will keep a copy of the string internally,
      // we free some_data.
    }


if (error)
{
    g_warning ("probleme de chargement de l'icone : %s\n", error->message);
    g_error_free(error);
    error = NULL;
}
return GTK_TREE_MODEL (store);
}
GtkWidget * create_view_and_model()
{
GtkCellRenderer     *renderer;
GtkTreeModel        *model;
GtkWidget           *view;

view = gtk_tree_view_new ();
char colonne[9][16] =
{
    "CIN",
    "CNE",
    "Nom",
    "Prenom",
    "Sexe",
    "Etablissement",
    "Annee Obt",
    "Note Gener",
    "Edit"
};
int i;
for(i=0; i<9; i++)
{
    if(i==8)
    {
        renderer = gtk_cell_renderer_pixbuf_new();
        gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                -1,
                colonne[i],
                renderer,
                "pixbuf", i,
                NULL);
    }
    else
    {
        renderer = gtk_cell_renderer_text_new ();
        gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                -1,
                colonne[i],
                renderer,
                "text", i,
                NULL);
    }
}
model = create_and_fill_model ();
gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
g_object_unref (model);
return view;
}
int main (int argc, char **argv)
{
GtkWidget *window;
GtkWidget *view;
gtk_init (&argc, &argv);
window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
g_signal_connect (window, "delete_event", gtk_main_quit, NULL);
view = create_view_and_model ();
gtk_container_add (GTK_CONTAINER (window), view);
gtk_widget_show_all (window);
gtk_main ();
return 0;
}