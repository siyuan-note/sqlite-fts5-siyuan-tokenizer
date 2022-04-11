cmake_minimum_required(VERSION 3.12)

project(test
    VERSION 0.1
    LANGUAGES C
)

add_library(sqlite3
    STATIC
        sqlite3.c
        siyuan.c
)

target_compile_definitions(sqlite3
    PRIVATE
        SQLITE_DQS=0
        SQLITE_THREADSAFE=0
        SQLITE_DEFAULT_MEMSTATUS=0
        SQLITE_DEFAULT_WAL_SYNCHRONOUS=1
        SQLITE_LIKE_DOESNT_MATCH_BLOBS
        SQLITE_MAX_EXPR_DEPTH=0
        SQLITE_OMIT_DECLTYPE
        SQLITE_OMIT_DEPRECATED
        SQLITE_OMIT_PROGRESS_CALLBACK
        SQLITE_OMIT_SHARED_CACHE
        SQLITE_USE_ALLOCA
        SQLITE_OMIT_AUTOINIT

        SQLITE_ENABLE_FTS5
        SQLITE_TEMP_STORE=3
        HAVE_USLEEP
        SQLITE_CORE
)

add_executable(main main.c)
target_link_libraries(main
	PRIVATE sqlite3
)

# https://www.sqlite.org/fts5.html#extending_fts5
# https://www.sqlite.org/loadext.html#programming_loadable_extensions
# https://www.sqlite.org/loadext.html#statically_linking_a_run_time_loadable_extension
# https://www.sqlite.org/compile.html