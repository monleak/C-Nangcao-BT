#include <gtk/gtk.h>

enum
{
    COLUMN_ARTICLE = 0,
    COLUMN_PRICE,
    N_COLUMNS
};

typedef struct
{
    GtkListStore       *articles;
    GtkTreeModelSort   *sorted;
    GtkTreeModelFilter *filtered;
    gdouble             max_price;
} Store;


static gboolean
row_visible (GtkTreeModel *model,
             GtkTreeIter *iter,
             Store *store)
{
    gdouble price;

    gtk_tree_model_get (model, iter,
                        COLUMN_PRICE, &price,
                        -1);

    return price <= store->max_price;
}

static Store *
create_store (void)
{
    Store *store;
    GtkTreeIter iter;

    store = g_new0 (Store, 1);
    store->max_price = 12.99;
    store->articles = gtk_list_store_new (N_COLUMNS,
                                          G_TYPE_STRING,
                                          G_TYPE_DOUBLE);
    
    /* Add some items */
    gtk_list_store_append (store->articles, &iter);
    gtk_list_store_set (store->articles, &iter, COLUMN_ARTICLE, "Spam", COLUMN_PRICE, 1.20, -1);

    gtk_list_store_append (store->articles, &iter);
    gtk_list_store_set (store->articles, &iter, COLUMN_ARTICLE, "Beer", COLUMN_PRICE, 5.99, -1);

    gtk_list_store_append (store->articles, &iter);
    gtk_list_store_set (store->articles, &iter, COLUMN_ARTICLE, "Chewing Gum", COLUMN_PRICE, 0.99, -1);

    store->filtered = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (GTK_TREE_MODEL (store->articles), NULL));
    store->sorted = GTK_TREE_MODEL_SORT (gtk_tree_model_sort_new_with_model (GTK_TREE_MODEL (store->filtered)));

    gtk_tree_model_filter_set_visible_func (store->filtered,
                                            (GtkTreeModelFilterVisibleFunc) row_visible,
                                            store, NULL);

    return store;
}

static void
on_row_activated (GtkTreeView *view,
                  GtkTreePath *path,
                  GtkTreeViewColumn *col,
                  Store *store)
{
    GtkTreeIter iter;
    GtkTreePath *filtered_path;
    GtkTreePath *child_path;

    filtered_path = gtk_tree_model_sort_convert_path_to_child_path (GTK_TREE_MODEL_SORT (store->sorted),
                                                                    path);

    child_path = gtk_tree_model_filter_convert_path_to_child_path (GTK_TREE_MODEL_FILTER (store->filtered),
                                                                   filtered_path);

    if (gtk_tree_model_get_iter (GTK_TREE_MODEL (store->articles), &iter, child_path)) {
        gchar *article;
        gdouble price;

        gtk_tree_model_get (GTK_TREE_MODEL (store->articles), &iter,
                            COLUMN_ARTICLE, &article,
                            COLUMN_PRICE, &price,
                            -1);

        g_print ("You want to buy %s for %f?\n", article, price);
        g_free (article);
    }
}

static GtkTreeView *
create_view (Store *store)
{
    GtkTreeView         *view;
    GtkCellRenderer     *renderer;
    GtkTreeViewColumn   *article_column;
    GtkTreeViewColumn   *price_column;

    view = GTK_TREE_VIEW (gtk_tree_view_new_with_model (GTK_TREE_MODEL (store->sorted)));
    gtk_widget_set_vexpand (GTK_WIDGET (view), TRUE);

    renderer = gtk_cell_renderer_text_new ();

    article_column = gtk_tree_view_column_new_with_attributes ("Article", renderer,
            "text", COLUMN_ARTICLE,
            NULL);

    gtk_tree_view_column_set_sort_column_id (article_column, COLUMN_ARTICLE);
    gtk_tree_view_append_column (view, article_column);

    price_column = gtk_tree_view_column_new_with_attributes ("Price", renderer,
            "text", COLUMN_PRICE,
            NULL);

    gtk_tree_view_column_set_sort_column_id (price_column, COLUMN_PRICE);
    gtk_tree_view_append_column (view, price_column);

    g_signal_connect (view, "row-activated",
                      G_CALLBACK (on_row_activated), store);

    return view;
}

static void
on_max_price_changed (GtkAdjustment *adjustment,
                      Store *store)
{
    store->max_price = gtk_adjustment_get_value (adjustment);
    gtk_tree_model_filter_refilter (store->filtered);
}

gint
main (gint argc,
      gchar *argv[])
{
    GtkWidget *window;
    GtkWidget *view;
    GtkWidget *box;
    GtkWidget *spinbutton;
    GtkAdjustment *max_price;
    Store *store;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    store = create_store ();
    view = GTK_WIDGET (create_view (store));

    max_price = gtk_adjustment_new (10.99, 0.01, 1024.0, 0.01, 1.0, 0.0);
    spinbutton = gtk_spin_button_new (max_price, 1.0, 2);

    gtk_container_add (GTK_CONTAINER (window), box);
    gtk_container_add (GTK_CONTAINER (box), view);
    gtk_container_add (GTK_CONTAINER (box), spinbutton);

    gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

    g_signal_connect (G_OBJECT (window), "delete-event",
                      G_CALLBACK (gtk_main_quit), NULL);

    g_signal_connect (G_OBJECT (max_price), "value-changed",
                      G_CALLBACK (on_max_price_changed), store);

    gtk_widget_show_all (window);
    gtk_main ();

    return 0;
}