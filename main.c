#include <stdio.h>
#include "sqlite3.h"

int sqlite3_siyuan_init(sqlite3* db, char** pzErrMsg, const sqlite3_api_routines* pApi);

int Count = 0;

int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    Count += 1;

    printf("Result: \n");
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n",azColName[i], argv[i]);
    }

    return SQLITE_OK;
}

int main()
{
    sqlite3_initialize();
    sqlite3_auto_extension((void(*)(void))sqlite3_siyuan_init);

    sqlite3* db;
    char* errmsg = 0;
    printf("%s\n", errmsg);
    int rc = sqlite3_open(":memory:", &db);

	char* sql = "create virtual table blocks_fts using fts5(content, tokenize = 'siyuan')";
	rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errmsg);
        return 1;
    }

    sql = "insert into blocks_fts(content) values ('📕 思源笔记是一款本地优先的个人知识管理系统， 支持细粒度块级引用和 Markdown 所见即所得。SiYuan is a local-first personal knowledge management system, supports fine-grained block-level reference, and Markdown WYSIWYG.'),"
          "('思源笔记是一款本地优先的个人知识管理系统，支持完全离线使用，同时也支持端到端加密同步。')";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errmsg);
        return 1;
    }
    
    sql = "select highlight(blocks_fts, 0, '【', '】') from blocks_fts where content match '思源'";
    printf("%s\n", sql);
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errmsg);
        return 1;
    }
    printf("Total: %d\n\n", Count);
    Count = 0;

    sql = "select highlight(blocks_fts, 0, '【', '】') from blocks_fts where content match 'NEAR(同时 支持, 10)'";
    printf("%s\n", sql);
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errmsg);
        return 1;
    }
    printf("Total: %d\n\n", Count);
    Count = 0;

    return 0;
}