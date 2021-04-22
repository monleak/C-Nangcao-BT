#ifndef DB_MAIN_H_
#define DB_MAIN_H_

typedef struct contacts {
    int id;
    char name[100];
    char number[100];
}contacts;

extern void DB_INIT() ;

extern int insert_bind(char name[], char number[]);

extern contacts* select_bind();

// UPDATE num=value WHERE id=des;
extern void update_bind(char value[], char des[]);

extern int delete_bind(char id[]);

extern void DB_CLOSE();

#endif // DB_MAIN_H_