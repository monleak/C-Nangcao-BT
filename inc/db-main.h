#ifndef DB_MAIN_H_
#define DB_MAIN_H_


// ========== Khai báo hàm ===============
extern void open_and_create_db(GtkBuilder *builder);
extern int insert_db(char name[], char number[],GtkBuilder *builder);
extern void push_to_GUI(GtkBuilder  *builder);
extern void close_db();

int callback2(void *liststore, int argc, char **argv, char **azColName);
extern void insert_db_from_file(char file[], GtkBuilder *builder);

//Update tên và sdt
//UPDATE $tablename SET $colName = 'new_name' WHERE $colName = 'old_name' 
extern int update_db(char *colName, char *old_name, char *new_name);

//Kiểm tra giá trị có tồn tại trong cột không
//	@colName: 	Tên cột
//	@name:		Giá trị kiểm tra  
extern int is_exists_in_db(char *colName, char *name); 	
														
#endif // DB_MAIN_H_