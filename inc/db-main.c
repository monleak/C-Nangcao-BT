/*
 *	Xem vi du cach dung o main
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "./db-main.h"

//db_name, table_name
#define dbname "test.db"
#define tablename "test"

//data list
contacts* list = NULL;
int list_size = 0;

// Actual database handle
sqlite3 *db = NULL;

// Database commands
/*
 * CREATE_DB
 * @id: INTERGER PRIMARY KEY
 * @name: TEXT
 * @num: INTEGER
 */
static const char create_sql[] = 	"CREATE TABLE "
							tablename 
        					" ("
							"id INTEGER PRIMARY KEY,"
							"name TEXT,"
							"num INTEGER"
							")";

static const char insert_sql[] = 	"INSERT INTO "
							tablename
							" (id, name, num) VALUES (NULL,?,?)";

static const char select_sql[] = 	"SELECT * FROM "
							tablename;

static const char update_sql[] = 	"UPDATE "
							tablename
							" SET num=? WHERE id=?;";

static const char delete_sql[] = 	"DELETE from "
							tablename
							" where ID=?; ";


// For the insert and select, we will prepare statements
static sqlite3_stmt *insert_stmt = NULL;
static sqlite3_stmt *select_stmt = NULL;
static sqlite3_stmt *update_stmt = NULL;
static sqlite3_stmt *delete_stmt = NULL;


extern void DB_INIT() {

	// SQLite return value
	int rc;

	// Open the database
	// rc = result_code
	rc = sqlite3_open(dbname, &db);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't open database %s (%i): %s\n", dbname, rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}


	// SQLite exec returns errors with this
	char *exec_errmsg;

	// Use exec to run simple statements that can only fail/succeed
	rc = sqlite3_exec(db, create_sql, NULL, NULL, &exec_errmsg);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't create table (%i): %s\n", rc, exec_errmsg);
		// sqlite3_free(exec_errmsg);
		// sqlite3_close(db);
		// exit(1);
	}

	// Prepare the two statements we need to prepare
	//  The last parameter, dbend, is used if you have multiple sql statements in one string
	rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't prepare insert statment %s (%i): %s\n", insert_sql, rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't prepare select statment %s (%i): %s\n", insert_sql, rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

    rc = sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't prepare update statment %s (%i): %s\n", update_sql, rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	rc = sqlite3_prepare_v2(db, delete_sql, -1, &delete_stmt, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Can't prepare delete statment %s (%i): %s\n", delete_sql, rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
}

extern int insert_bind(char name[], char number[]) {
	int rc = sqlite3_bind_text(insert_stmt, 1, name, -1, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Error binding value in insert (%i): %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	} else {
		printf("Successfully bound text for insert: %s\n", name);
	}

	rc = sqlite3_bind_text(insert_stmt, 2, number, -1, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Error binding value in insert (%i): %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	} else {
		printf("Successfully bound text for insert: %s\n", number);
	}

	rc = sqlite3_step(insert_stmt);
	if(SQLITE_DONE != rc) {
		fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));
	} else {
		printf("INSERT completed\n\n");
	}

	return rc;
}


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  // printf("%d\n", *(int *)NotUsed);
  *(int *)NotUsed = *(int *)NotUsed+1;
  list = (contacts *)realloc(list, *(int *)NotUsed*sizeof(contacts));
  list[*(int *)NotUsed -1].id = atoi(argv[0]);
  strcpy(list[*(int *)NotUsed -1].name, argv[1]);
  strcpy(list[*(int *)NotUsed -1].number, argv[2]);
  list_size = *(int *)NotUsed;
  return 0;
}

extern contacts* select_bind() {
	int count = 0;
	list = (contacts *)calloc(1, sizeof(contacts));

	char *exec_errmsg;
	int rc = sqlite3_exec(db, "select * from test", callback, &count, &exec_errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", exec_errmsg);
        sqlite3_free(exec_errmsg);
    } else {
        printf("SELECT success\n");
    }
    return list;
}

static void printContactsList() {
	for(int i = 0; i < list_size; i++) {
		printf("%d, %s, %s \n", list[i].id, list[i].name, list[i].number);
	}
}

// UPDATE colName=value WHERE place=des;
extern void update_bind(char value[], char des[]) {

	int rc = 0;

	// rc = sqlite3_bind_text(update_stmt, 1, colName, -1, NULL);
	// if(SQLITE_OK != rc) {
	// 	fprintf(stderr, "Error binding value in update (%i): %s\n", rc, sqlite3_errmsg(db));
	// 	sqlite3_close(db);
	// 	exit(1);
	// } else {
	// 	printf("Successfully bind colName for update: %s\n", colName);
	// }

	rc = sqlite3_bind_text(update_stmt, 1, value, -1, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Error binding value in update (%i): %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	} else {
		printf("Successfully bind value for update: %s\n", value);
	}

	// rc = sqlite3_bind_text(update_stmt, 3, place, -1, NULL);
	// if(SQLITE_OK != rc) {
	// 	fprintf(stderr, "Error binding place in update (%i): %s\n", rc, sqlite3_errmsg(db));
	// 	sqlite3_close(db);
	// 	exit(1);
	// } else {
	// 	printf("Successfully bind place for update: %s\n", place);
	// }	

	rc = sqlite3_bind_text(update_stmt, 2, des, -1, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Error binding des in update (%i): %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	} else {
		printf("Successfully bind des for update: %s\n", des);
	}

	rc = sqlite3_step(update_stmt);
    if (rc != SQLITE_DONE) {
        printf("Error #%d: %s\n", rc, sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
}

extern int delete_bind(char id[]) {
	int rc = sqlite3_bind_text(delete_stmt, 1, id, -1, NULL);
	if(SQLITE_OK != rc) {
		fprintf(stderr, "Error binding id in delete (%i): %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	} else {
		printf("Successfully bind id for delete: %s\n", id);
	}

	rc = sqlite3_step(delete_stmt);
    if (rc != SQLITE_DONE) {
        printf("Error #%d: %s\n", rc, sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    return 0;
}

extern void DB_CLOSE() {
	sqlite3_finalize(insert_stmt);
	sqlite3_finalize(select_stmt);
	sqlite3_finalize(update_stmt);
	sqlite3_finalize(delete_stmt);
	sqlite3_close(db);
}

extern void DB_FREE_LIST() {
	free(list);
}

int main() {
	//test
	DB_INIT();	

	insert_bind("a", "b");
	select_bind();
	printContactsList();
	delete_bind("54");
	select_bind();
	printContactsList();

	DB_CLOSE();
	DB_FREE_LIST();
	return 0;
}
