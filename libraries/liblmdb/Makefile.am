lib_LTLIBRARIES = liblmdb.la
liblmdb_la_SOURCES = mdb.c midl.c
liblmdb_la_LDFLAGS=-no-undefined -avoid-version

bin_PROGRAMS = mdb_stat mdb_copy lmdump lmmgr
mdb_stat_LDADD = liblmdb.la
mdb_copy_LDADD = liblmdb.la
lmdump_LDADD = liblmdb.la
lmmgr_LDADD = liblmdb.la

include_HEADERS = lmdb.h
