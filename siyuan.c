#include "sqlite3ext.h"
#include <stddef.h>
SQLITE_EXTENSION_INIT1

static fts5_api* fts5_api_from_db(sqlite3* db)
{
    fts5_api* pRet = 0;
    sqlite3_stmt* pStmt = 0;

    if (SQLITE_OK == sqlite3_prepare(db, "SELECT fts5(?1)", -1, &pStmt, 0)) {
        sqlite3_bind_pointer(pStmt, 1, (void*)&pRet, "fts5_api_ptr", NULL);
        sqlite3_step(pStmt);
    }
    sqlite3_finalize(pStmt);
    return pRet;
}

typedef struct SiYuanTokenizer {
    int (*xTokenize)(void* pCtx, int flags, const char* pText, int nText, 
		int (*xToken)(
			void *pCtx,         /* Copy of 2nd argument to xTokenize() */
	        int tflags,         /* Mask of FTS5_TOKEN_* flags */
		    const char *pToken, /* Pointer to buffer containing token */
			int nToken,         /* Size of token in bytes */
	        int iStart,         /* Byte offset of token within input text */
	        int iEnd            /* Byte offset of end of token within input text */
      ));
}SiYuanTokenizer;

static const char CharacterBytesForUTF8[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0
};

static int siyuanTokenizer(void* pCtx, int flags, const char* pText, int nText,
                           int (*xToken)(void* pCtx, int tflags, const char* pToken, int nToken, int iStart, int iEnd))
{
	int rc = SQLITE_OK;

    // flags 可能的值有 FTS5_TOKENIZE_QUERY、FTS5_TOKENIZE_PREFIX、FTS5_TOKENIZE_DOCUMENT、FTS5_TOKENIZE_AUX，如果想对不同的输入做不同的处理可以使用这个

    int iStart = 0;
    int iEnd = 0;
    char* p = pCtx;
    
    while (iEnd < nText)
    {
        int length = CharacterBytesForUTF8[(unsigned int)(unsigned char)pText[iStart]];
        iEnd += length;
        if (length == 0 || iEnd > nText)
            return SQLITE_ERROR;

        // 过滤停止词
        // ...

        rc = xToken(pCtx, 0, pText + iStart, length, iStart, iEnd); // 如果需要添加同义词，则第 2 个参数为 FTS5_TOKEN_COLOCATED
        // rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, ..., ..., ..., ...);
        iStart = iEnd;
    }

	return rc;
}

static int fts5SiYuanCreate(void* pCtx, const char** azArg, int nArg, Fts5Tokenizer** ppOut)
{
    for (int i = 0; i != nArg; ++i) {
        // 对传入参数做处理
        // 如：create virtual table blocks_fts using fts5(content, tokenize = 'siyuan arg1 arg2')
    }
    SiYuanTokenizer* p = (SiYuanTokenizer*)sqlite3_malloc(sizeof(SiYuanTokenizer));
    if (p) {
        p->xTokenize = siyuanTokenizer;
        *ppOut = (Fts5Tokenizer*)(p);
        return SQLITE_OK;
    }
    return SQLITE_ERROR;
}

static void fts5SiYuanDelete(Fts5Tokenizer* p) {
    sqlite3_free(p);
}

static int fts5SiYuanTokenize(Fts5Tokenizer* tokenizer_ptr, void* pCtx, int flags, const char* pText, int nText,
                      int (*xToken)(void*, int, const char*, int, int, int))
{
    SiYuanTokenizer* p = (SiYuanTokenizer*)tokenizer_ptr;
    return p->xTokenize(pCtx, flags, pText, nText, xToken);
}


#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_siyuan_init(sqlite3* db, char** pzErrMsg, const sqlite3_api_routines* pApi)
{
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);

    fts5_tokenizer tokenizer = { fts5SiYuanCreate, fts5SiYuanDelete, fts5SiYuanTokenize };
    fts5_api* fts5api = fts5_api_from_db(db);

    if (fts5api)
        rc = fts5api->xCreateTokenizer(fts5api, "siyuan", (void*)fts5api, &tokenizer, NULL);

    return rc;
}