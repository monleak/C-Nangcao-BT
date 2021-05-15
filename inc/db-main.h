#ifndef DB_MAIN_H_
#define DB_MAIN_H_


// ========== Khai báo hàm ===============
extern void open_and_create_db();
extern int insert_db(char name[], char number[]);
extern void push_to_GUI(GtkBuilder  *builder);
extern void close_db();


#endif // DB_MAIN_H_