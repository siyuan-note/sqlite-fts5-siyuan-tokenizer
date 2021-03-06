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

    sql = "insert into blocks_fts(content) values ('ð ææºç¬è®°æ¯ä¸æ¬¾æ¬å°ä¼åçä¸ªäººç¥è¯ç®¡çç³»ç»ï¼ æ¯æç»ç²åº¦åçº§å¼ç¨å Markdown æè§å³æå¾ãSiYuan is a local-first personal knowledge management system, supports fine-grained block-level reference, and Markdown WYSIWYG.'),"
          "('ææºç¬è®°æ¯ä¸æ¬¾æ¬å°ä¼åçä¸ªäººç¥è¯ç®¡çç³»ç»ï¼æ¯æå®å¨ç¦»çº¿ä½¿ç¨ï¼åæ¶ä¹æ¯æç«¯å°ç«¯å å¯åæ­¥ã')";
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errmsg);
        return 1;
    }
    
    sql = "select highlight(blocks_fts, 0, 'ã', 'ã') from blocks_fts where content match 'ææº'";
    printf("%s\n", sql);
    rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errmsg);
        return 1;
    }
    printf("Total: %d\n\n", Count);
    Count = 0;

    sql = "select highlight(blocks_fts, 0, 'ã', 'ã') from blocks_fts where content match 'NEAR(åæ¶ æ¯æ, 10)'";
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