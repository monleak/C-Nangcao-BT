#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "./sqlite/sqlite3.h"
#include "../inc/utility.h"
#include "./db-main.h"

//db_name, table_name
#define dbname "phonebook_data.db"
#define tablename "Phonebook"

//data list
// contacts* list = NULL;
// int list_size = 0;

// Actual database handle
sqlite3 *db = NULL;

// Database commands
/*
 * CREATE_DB
 * @id: INTERGER PRIMARY KEY
 * @name: TEXT
 * @num: INTEGER
 */
static const char create_table[] = "CREATE TABLE IF NOT EXISTS Phonebook(Id INT IDENTITY(1,1) PRIMARY KEY,name TEXT,number TEXT);";

// static const char insert_sql[] = 	"INSERT INTO "
// 							tablename
// 							" (name, num) VALUES (?,?)";

// static const char select_sql[] = 	"SELECT * FROM "
// 							tablename;

// static const char update_sql[] = 	"UPDATE "
// 							tablename
// 							" SET num=? WHERE id=?;";

// static const char delete_sql[] = 	"DELETE from "
// 							tablename
// 							" where ID=?; ";
int KiemTraXau(char s[]) 
{
	int i,ok;
	for(i=0; i<strlen(s); i++) {
		if(s[i] >= '0' && s[i] <= '9') {
			ok = 1;
		} else {
			ok = 0;
			break;
		}
	}
	return ok;
}

extern void open_and_create_db(GtkBuilder *builder)  //Khởi tạo database
{
	GtkWidget   *label;
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	// gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Text");
	// SQLite return value
	int rc;

	// Mở database
	// rc = result_code
	rc = sqlite3_open(dbname, &db);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't open database %s (%i): %s\n", dbname, rc, sqlite3_errmsg(db));
		gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Không mở được database, hãy kiểm tra lại!");
		sqlite3_close(db);
		return ;
	}


	// Báo lỗi của SQLite exec
	char *exec_errmsg;

	// Khởi tạo table 
	rc = sqlite3_exec(db, create_table, 0, 0, &exec_errmsg);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't create table (%i): %s\n", rc, exec_errmsg);
		gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Không mở được database, hãy kiểm tra lại!");
		sqlite3_free(exec_errmsg);
		sqlite3_close(db);
		return ;
	}
}

extern int insert_db(char *name, char *number,GtkBuilder *builder)
{
	GtkWidget   *label;
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	char sql[100]="INSERT INTO Phonebook(name,number) VALUES('";
	strcat(sql, name);
	strcat(sql, "', '");
	strcat(sql, number);
	strcat(sql,"');\0");
	char *err_msg = 0;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "SQL error: %s\n", err_msg);
	    gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Lỗi khi lưu liên lạc,hãy thử lại!");
	    sqlite3_free(err_msg);
	    return 0;
    }else {
    	gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Thêm liên lạc thành công!");
    	printf("Tên: %s\nSố điện thoại: %s\nAdd Success!\n\n", name,number);
    	return 1;
    }
}
int callback2(void *liststore, int argc, char **argv, char **azColName) 
{
    if(KiemTraXau(argv[2]) == 1 && validate_name(argv[1]) == STR_OK && validate_number(argv[2]) == STR_OK)
    {
    	// Chèn dữ liệu mới vào file phonebook_data.db
	    char sql[100]="INSERT INTO Phonebook(name,number) VALUES('";
		strcat(sql, argv[1]);
		strcat(sql, "', '");
		strcat(sql, argv[2]);
		strcat(sql,"');\0");
		char *err_msg = 0;
		int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
		if (rc != SQLITE_OK ) 
		{
		    fprintf(stderr, "SQL error: %s\n", err_msg);
		    sqlite3_free(err_msg);
	    }
	    if (rc == SQLITE_OK )
	    {
	    	// ======== Đẩy dữ liệu file mới lên GUI =======
			GtkTreeIter iter;

		    gtk_list_store_append (liststore, &iter);       
		    gtk_list_store_set (liststore, &iter,
		            0, argv[1],
		            1, argv[2],
		            -1);	    
		    // printf("Tên: %s\nSố điện thoại: %s\nDone!\n\n", argv[1],argv[2]);  ****Thêm dòng này là bị lỗi
	    }
    }

}
extern void insert_db_from_file(char *file, GtkBuilder *builder)
{
	GtkWidget   *label;
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	sqlite3 *new_db = NULL;
	int rc;
	rc = sqlite3_open(file, &new_db);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't open database %s (%i): %s\n", file, rc, sqlite3_errmsg(new_db));
		sqlite3_close(new_db);
	}
	// ======== Đẩy dữ liệu file mới lên GUI =======
	GtkListStore *liststore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
	char *sql = "SELECT * FROM Phonebook ";
	char *err_msg = 0;
	printf("Tải dữ liệu từ %s\n\n",file);
	rc = sqlite3_exec(new_db, sql, callback2, liststore, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "Failed to select data\n");
	    fprintf(stderr, "SQL error: %s\n", err_msg);
	    sqlite3_free(err_msg);
	    sqlite3_close(new_db);
  	}else gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Tải dữ liệu thành công!");
  	sqlite3_close(new_db);
	// ======== Đẩy dữ liệu file mới lên GUI =======
	// Chèn dữ liệu mới vào file phonebook_data.db
	
}

int callback(void *liststore, int argc, char **argv, char **azColName) 
{
	GtkTreeIter iter;

    gtk_list_store_append (liststore, &iter);       
    gtk_list_store_set (liststore, &iter,
            0, argv[1],
            1, argv[2],
            -1);
    printf("Tên: %s\nSố điện thoại: %s\n\n", argv[1],argv[2]);
	return 0;
}
extern void push_to_GUI(GtkBuilder *builder)
{
	GtkListStore *liststore = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststore1"));
	GtkWidget   *label;
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	char *sql = "SELECT * FROM Phonebook ";
	int rc;
	char *err_msg = 0;
	printf("=============== Dữ liệu tải lên ===============\n\n");
	rc = sqlite3_exec(db, sql, callback, liststore, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "Failed to select data\n");
	    fprintf(stderr, "SQL error: %s\n", err_msg);
	    gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Không thể lấy dữ liệu từ database, hãy kiểm tra lại!");
	    sqlite3_free(err_msg);
	    sqlite3_close(db);
  	}
}


static int exists_callback(void *num_rows, int argc, char **argv, char **azColName){
	*(int *)num_rows = atoi(argv[0]);
}

extern int is_exists_in_db(char *colName, char *name) {
	int num_rows = 0;
	char sql[100];
	char *errMsg;
	strcpy(sql, "SELECT COUNT(*) AS num FROM " tablename " WHERE ");
	strcat(sql, colName);
	strcat(sql, "='");
	strcat(sql, name);
	strcat(sql, "';");
    sqlite3_exec(db, sql, exists_callback, &num_rows, &errMsg);
    return num_rows;
}

extern int update_db(char *old_name, char *new_name, char *new_number, GtkBuilder *builder)
{
	GtkWidget   *label;
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	char sql[100] = "UPDATE "
					tablename
					" SET name = '";
	strcat(sql, new_name);
	strcat(sql, "', number= '");
	strcat(sql, new_number);
	strcat(sql, "' WHERE name = '");
	strcat(sql, old_name);
	strcat(sql, "';\0");
	char *err_msg = 0;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "SQL error update_db_name: %s\n", err_msg);
	    gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Không thể chỉnh sửa liên hệ, hãy thử lại!");
	    sqlite3_free(err_msg);
	    return 0;
    }else
    {
    	gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Chỉnh sửa liên hệ thành công!");
    	return 1;
    }
}

extern int delete_db(char *name, GtkBuilder *builder)
{
	GtkWidget   *label;
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
	char sql[100] = "DELETE FROM "
					tablename
					" WHERE name = '";
	strcat(sql, name);
	strcat(sql, "';\0");
	char *err_msg = 0;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "SQL error: %s\n", err_msg);
	    gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Không thể xóa liên hệ, hãy thử lại!");
	    sqlite3_free(err_msg);
	    return 0;
    }else
    {
    	gtk_label_set_text (GTK_LABEL(label), (const gchar* ) "Xóa liên hệ thành công!");
    	return 1;
    }
}

extern void close_db()
{
	sqlite3_close(db);
}













// extern int insert_data(char name[], char number[]) {
// 	int rc = sqlite3_bind_text(insert_stmt, 1, name, -1, NULL);
// 	if(SQLITE_OK != rc) {
// 		fprintf(stderr, "Error binding value in insert (%i): %s\n", rc, sqlite3_errmsg(db));
// 		sqlite3_close(db);
// 		exit(1);
// 	} else {
// 		printf("Successfully bound text for insert: %s\n", name);
// 	}

// 	rc = sqlite3_bind_text(insert_stmt, 2, number, -1, NULL);
// 	if(SQLITE_OK != rc) {
// 		fprintf(stderr, "Error binding value in insert (%i): %s\n", rc, sqlite3_errmsg(db));
// 		sqlite3_close(db);
// 		exit(1);
// 	} else {
// 		printf("Successfully bound text for insert: %s\n", number);
// 	}

// 	rc = sqlite3_step(insert_stmt);
// 	if(SQLITE_DONE != rc) {
// 		fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));
// 	} else {
// 		printf("INSERT completed\n\n");
// 	}

// 	sqlite3_reset(insert_stmt);

// 	return rc;
// }


// static int callback(void *NotUsed, int argc, char **argv, char **azColName){
//   int i;
//   // printf("%d\n", *(int *)NotUsed);
//   *(int *)NotUsed = *(int *)NotUsed+1;
//   list = (contacts *)realloc(list, *(int *)NotUsed*sizeof(contacts));
//   list[*(int *)NotUsed -1].id = atoi(argv[0]);
//   strcpy(list[*(int *)NotUsed -1].name, argv[1]);
//   strcpy(list[*(int *)NotUsed -1].number, argv[2]);
//   list_size = *(int *)NotUsed;
//   return 0;
// }

// extern contacts* select_bind() {
// 	int count = 0;
// 	list = (contacts *)calloc(1, sizeof(contacts));

// 	char *exec_errmsg;
// 	int rc = sqlite3_exec(db, "select * from test", callback, &count, &exec_errmsg);
//     if (rc != SQLITE_OK) {
//         fprintf(stderr, "SQL error: %s\n", exec_errmsg);
//         sqlite3_free(exec_errmsg);
//     } else {
//         printf("SELECT success\n");
//     }
//     printf("after_select\n");
//     printContactsList(list, list_size);
//     return list;
// }

// extern void printContactsList(contacts *list, int list_size) {
// 	for(int i = 0; i < list_size; i++) {
// 		printf("%d, %s, %s \n", list[i].id, list[i].name, list[i].number);
// 	}
// }

// extern int get_list_size_after_select(){
// 	return list_size;
// }

// // UPDATE colName=value WHERE place=des;
// extern void update_bind(char value[], char des[]) {

// 	int rc = 0;

// 	// rc = sqlite3_bind_text(update_stmt, 1, colName, -1, NULL);
// 	// if(SQLITE_OK != rc) {
// 	// 	fprintf(stderr, "Error binding value in update (%i): %s\n", rc, sqlite3_errmsg(db));
// 	// 	sqlite3_close(db);
// 	// 	exit(1);
// 	// } else {
// 	// 	printf("Successfully bind colName for update: %s\n", colName);
// 	// }

// 	rc = sqlite3_bind_text(update_stmt, 1, value, -1, NULL);
// 	if(SQLITE_OK != rc) {
// 		fprintf(stderr, "Error binding value in update (%i): %s\n", rc, sqlite3_errmsg(db));
// 		sqlite3_close(db);
// 		exit(1);
// 	} else {
// 		printf("Successfully bind value for update: %s\n", value);
// 	}

// 	// rc = sqlite3_bind_text(update_stmt, 3, place, -1, NULL);
// 	// if(SQLITE_OK != rc) {
// 	// 	fprintf(stderr, "Error binding place in update (%i): %s\n", rc, sqlite3_errmsg(db));
// 	// 	sqlite3_close(db);
// 	// 	exit(1);
// 	// } else {
// 	// 	printf("Successfully bind place for update: %s\n", place);
// 	// }	

// 	rc = sqlite3_bind_text(update_stmt, 2, des, -1, NULL);
// 	if(SQLITE_OK != rc) {
// 		fprintf(stderr, "Error binding des in update (%i): %s\n", rc, sqlite3_errmsg(db));
// 		sqlite3_close(db);
// 		exit(1);
// 	} else {
// 		printf("Successfully bind des for update: %s\n", des);
// 	}

// 	rc = sqlite3_step(update_stmt);
//     if (rc != SQLITE_DONE) {
//         printf("Error #%d: %s\n", rc, sqlite3_errmsg(db));
//         sqlite3_close(db);
//         exit(1);
//     }
// }

// extern int delete_bind(char id[]) {
// 	int rc = sqlite3_bind_text(delete_stmt, 1, id, -1, NULL);
// 	if(SQLITE_OK != rc) {
// 		fprintf(stderr, "Error binding id in delete (%i): %s\n", rc, sqlite3_errmsg(db));
// 		sqlite3_close(db);
// 		exit(1);
// 	} else {
// 		printf("Successfully bind id for delete: %s\n", id);
// 	}

// 	rc = sqlite3_step(delete_stmt);
//     if (rc != SQLITE_DONE) {
//         printf("Error #%d: %s\n", rc, sqlite3_errmsg(db));
//         sqlite3_close(db);
//         return 1;
//     }
//     return 0;
// }

// extern void DB_CLOSE() {
// 	sqlite3_finalize(insert_stmt);
// 	sqlite3_finalize(select_stmt);
// 	sqlite3_finalize(update_stmt);
// 	sqlite3_finalize(delete_stmt);
// 	sqlite3_close(db);
// }

// extern void DB_FREE_LIST() {
// 	free(list);
// }

// int main() {
// 	//test
// 	DB_INIT();	

// 	insert_bind("a", "b");
// 	select_bind();
// 	printContactsList(list, list_size);
// 	delete_bind("54");
// 	select_bind();
// 	printContactsList(list, list_size);

// 	DB_CLOSE();
// 	DB_FREE_LIST();
// 	return 0;
// }
