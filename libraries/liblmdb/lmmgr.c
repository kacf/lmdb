/*
lmmgr.c : Add, remove or manage the maxreaders value of a given database file
*/
#include <stdio.h>
#include <string.h>
#include "lmdb.h"

static int report_error(int rc) 
{
    printf("err(%d): %s\n", rc, mdb_strerror(rc));   
    return rc;
}

int openconn(const char *dbfile, MDB_env **env, long int maxreaders)
{
/*init*/
    int rc;
    rc = mdb_env_create(env);
    if(rc) return report_error(rc);

    rc = mdb_env_open(*env, dbfile, MDB_NOSUBDIR, 0644);
    if(rc) return report_error(rc);
    return 0;
}

int gettxn(MDB_env *env, MDB_txn **txn, MDB_dbi *dbi)
{
    int rc;
/*setup txn*/
    rc = mdb_txn_begin(env, NULL, 0, txn);
    if(rc) return report_error(rc);
    rc = mdb_open(*txn, NULL, 0, dbi);
    if(rc) return report_error(rc);
    return 0;
}

int committxn(MDB_txn *txn)
{
/*commit*/
    int rc;
    rc = mdb_txn_commit(txn);
    return 0;
}

int aborttxn(MDB_txn *txn)
{
/*abort*/
    int rc;
    mdb_txn_abort(txn);
    return 0;
}

int closeall(MDB_env *env, MDB_txn *txn, MDB_dbi dbi)
{
/*end*/
    mdb_close(env, dbi);
    mdb_env_close(env);
    return 0;
}

int do_put(char *dbfile, char *mykey, char *myval)
{
    MDB_env *env;
    openconn(dbfile, &env, 0);

    MDB_txn *txn;
    MDB_dbi dbi;
    gettxn(env, &txn, &dbi);

    int rc;
    MDB_val key, data;
    key.mv_data  = mykey;
    data.mv_data = myval;
    key.mv_size = strlen(mykey) + 1;
    data.mv_size = strlen(myval) + 1;

    rc = mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE);
    if (rc == MDB_SUCCESS)
    {
        rc = mdb_txn_commit(txn);
    }
    else
    {
        mdb_txn_abort(txn);
    }

    mdb_env_close(env);
    return rc;
}

int do_del(char *dbfile, char *mykey)
{
    MDB_env *env;
    openconn(dbfile, &env, 0);

    MDB_txn *txn;
    MDB_dbi dbi;
    gettxn(env, &txn, &dbi);

    int rc;
    MDB_val key;
    key.mv_data  = mykey;
    key.mv_size = strlen(mykey) + 1;

    rc = mdb_del(txn, dbi, &key, NULL);
    if (rc == MDB_SUCCESS)
    {
        rc = mdb_txn_commit(txn);
    }
    else
    {
        mdb_txn_abort(txn);
    }

    mdb_env_close(env);
    return rc;
}

int do_init(char *dbfile, unsigned long maxr)
{
    MDB_env *env;
    openconn(dbfile, &env, maxr);
    mdb_env_close(env);
    return 0;
}

int do_stat(char *dbfile)
{
    int rc;
    MDB_env *env;
    openconn(dbfile, &env, 0);

    MDB_stat stat;
    MDB_envinfo info;

    rc = mdb_env_stat(env, &stat);
    rc = mdb_env_info(env, &info);
    printf("me_maxreaders=%ld\n", info.me_maxreaders);
    mdb_env_close(env);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        goto fail;
    }

    if (!strcmp(argv[1], "put"))
    {
        if (argc == 5)
        {
            int rc = do_put(argv[2], argv[3], argv[4]);
            return rc;
        }
    }
    else if (!strcmp(argv[1], "del"))
    {
        if (argc == 4)
        {
            int rc = do_del(argv[2], argv[3]);
            return rc;
        }
    }
    else if (!strcmp(argv[1], "init"))
    {
        if (argc == 3)
        {
            int rc = do_init(argv[2], 0L);
            return rc;
        }
        else if (argc == 5 && !strcmp(argv[3], "-m"))
        {
            int rc = do_init(argv[2], atol(argv[4]));
            do_stat(argv[2]);
            return rc;
        }
    }
    else if (!strcmp(argv[1], "maxr"))
    {
        if (argc == 3)
        {
            int rc = do_stat(argv[2]);
            return rc;
        }
    }

fail:
    printf("Usage :\n");
    printf("Add a key and value to a DB file :\n");
    printf("\tlmmgr put dbfile key value\n");
    printf("Remove a key from a DB file :\n");
    printf("\tlmmgr del dbfile key\n");
    printf("Set a new maxreaders value of a DB file :\n");
    printf("\tlmmgr init dbfile [-m maxreaders]\n");
    printf("Give maxreaders value of a DB file :\n");
    printf("\tlmmgr maxr dbfile\n");

    return 1;
}
