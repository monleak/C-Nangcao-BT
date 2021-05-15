/*
 *	Xem vi du cach dung o main
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "./sqlite/sqlite3.h"

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
static const char create_sql[] = "DROP TABLE IF EXISTS "tablename";"
"CREATE TABLE "tablename" (id INT PRIMARY KEY,name TEXT,num TEXT)";

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


extern void open_and_create_db()  //Khởi tạo database
{

	// SQLite return value
	int rc;

	// Mở database
	// rc = result_code
	rc = sqlite3_open(dbname, &db);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't open database %s (%i): %s\n", dbname, rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}


	// Báo lỗi của SQLite exec
	char exec_errmsg[100];

	// Khởi tạo table 
	rc = sqlite3_exec(db, create_sql, NULL, NULL, &exec_errmsg);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't create table (%i): %s\n", rc, exec_errmsg);
		sqlite3_free(exec_errmsg);
		sqlite3_close(db);
		return 1;
	}
}

extern int insert_db(char *name, char *number)
{
	char *sql = "INSERT INTO "tablename" VALUES("name", "number");"
	int rc;
	char *err_msg = 0;
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "SQL error: %s\n", err_msg);
	    sqlite3_free(err_msg);
	    return 0;
    }
    return 1;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
	NotUsed = 0;
	GtkTreeIter iter;

    gtk_list_store_append (liststore1, &iter);       
    gtk_list_store_set (liststore1, &iter,
            0, argv[1],
            1, argv[2],
            -1);
	return 0;
}

extern void push_to_GUI()
{
	char *sql = "SELECT * FROM "tablename" ";
	int rc;
	char *err_msg = 0;
	rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
	if (rc != SQLITE_OK ) 
	{
	    fprintf(stderr, "Failed to select data\n");
	    fprintf(stderr, "SQL error: %s\n", err_msg);
	    sqlite3_free(err_msg);
	    sqlite3_close(db);
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
