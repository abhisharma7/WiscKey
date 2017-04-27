#pragma once
#include <assert.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include "leveldb/db.h"
#include "leveldb/filter_policy.h"
#include "leveldb/write_batch.h"

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using leveldb::ReadOptions;
using leveldb::Options;
using leveldb::Status;
using leveldb::WriteBatch;
using leveldb::WriteOptions;
using leveldb::DB;

  static bool
leveldb_get(DB * db, string &key, string &value)
{
  assert(lldb);
  ReadOptions ropt;
  Status s = db->Get(ropt, key, &value);
  assert(s.ok());
  if (s.IsNotFound()) {
    return false;
  } else {
    return true;
  }
}

  static void
leveldb_set(DB * db, string &key, string &value)
{
  WriteBatch wb;
  Status s;
  WriteOptions wopt;
  wb.Put(key, value);
  s = db->Write(wopt, &wb);
  assert(s.ok());
}

  static void
leveldb_del(DB * db, string &key)
{
  WriteOptions wopt;
  Status s;
  s = db->Delete(wopt, key);
  assert(s.ok());
}

  static void
destroy_leveldb(const string &dirname)
{
  Options options;
  leveldb::DestroyDB(dirname, options);
}

  static DB *
open_leveldb(const string &dirname)
{
  Options options;
  options.create_if_missing = true;
  options.filter_policy = leveldb::NewBloomFilterPolicy(10);
  options.write_buffer_size = 1u << 21;
  destroy_leveldb(dirname);
  DB * db = NULL;
  Status s = DB::Open(options, dirname, &db);
  return db;
}
