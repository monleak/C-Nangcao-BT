#ifndef DB_MAIN_H_
#define DB_MAIN_H_


// ========== Khai báo hàm ===============
extern void open_and_create_db(GtkBuilder *builder);
extern int insert_db(char name[], char number[],GtkBuilder *builder);
extern void push_to_GUI(GtkBuilder  *builder);
extern void close_db();

int callback2(void *liststore, int argc, char **argv, char **azColName);
extern void insert_db_from_file(char file[], GtkBuilder *builder);


#endif // DB_MAIN_H_