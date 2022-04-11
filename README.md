# sqlite-fts5-siyuan-tokenizer

SQLite FTS5 中文单字分词器。

感谢 [CChan](https://ld246.com/member/CChan) 贡献。

https://ld246.com/article/1641042386027

## 共享库编译

* Linux `gcc -fPIC -lm -shared siyuan.c -o siyuan-tokenizer.so`
* macOS `gcc -fno-common -dynamiclib siyuan.c -o siyuan-tokenizer.dylib`
* Windows `gcc -shared -I "sqlite3ext.h" -o siyuan-tokenizer.so siyuan.c`

## 静态编译

https://github.com/88250/go-sqlite3