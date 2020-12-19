
#include "voxc.h"

//result = sqlite3_prepare_v2(pdb, "insert into ttest values (9801)", 32, &pstmt, &pzTail);
//result = sqlite3_prepare_v2(pdb, "select * from ttest", 32, &pstmt, &pzTail);
//while (SQLITE_ROW == sqlite3_step(pstmt))
//{
//    result = sqlite3_column_int(pstmt, 0);
//}

//int8_t regType = 0;
//physx::PxRigidStatic* rigidStatic = NULL;
//uint16_t flags;

void blockdb_create_db()
{
    sqlite3* pdb = nullptr;
    sqlite3_stmt* pstmt = nullptr;
    const char* pzTail;

    int result = sqlite3_open("block.sdb", &pdb);
    if (result != SQLITE_OK) throw new std::runtime_error("failed to create db");
    
    result = sqlite3_prepare_v2(pdb, "create table entities (blockid INTEGER PRIMARY KEY, block16id INTEGER, regtype INTEGER, flags INTEGER)", 55, &pstmt, &pzTail);
    if (result != SQLITE_DONE) throw new std::runtime_error("failed to create table");

    sqlite3_finalize(pstmt);

    sqlite3_close(pdb);

}