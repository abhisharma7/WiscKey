#include "lab2_common.h"

  int
main(int argc, char ** argv)
{
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <value-size>" << endl;
    exit(0);
  }
  // value size is provided in bytes
  const size_t value_size = std::stoull(argv[1], NULL, 10);
  if (value_size < 1 || value_size > 100000) {
    cout << "  <value-size> must be positive and less then 100000" << endl;
    exit(0);
  }

  DB * db = open_leveldb("leveldb_test_dir");
  if (db == NULL) {
    cerr << "Open LevelDB failed!" << endl;
    exit(1);
  }
  char * vbuf = new char[value_size];
  for (size_t i = 0; i < value_size; i++) {
    vbuf[i] = rand();
  }
  string value = string(vbuf, value_size);

  size_t nfill = 1000000000 / (value_size + 8);
  clock_t t0 = clock();
  size_t p1 = nfill / 40;
  for (size_t j = 0; j < nfill; j++) {
    string key = std::to_string(((size_t)rand())*((size_t)rand()));
    leveldb_set(db, key, value);
    if (j >= p1) {
      clock_t dt = clock() - t0;
      cout << "progress: " << j+1 << "/" << nfill << " time elapsed: " << dt * 1.0e-6 << endl << std::flush;
      p1 += (nfill / 40);

    }
  }
  clock_t dt = clock() - t0;
  cout << "time elapsed: " << dt * 1.0e-6 << " seconds" << endl;
  	clock_t t1 = clock();
  	for (size_t k = 0; k < 100000; k++) {

                string testingkey = std::to_string(((size_t)rand())*((size_t)rand()));
                string testingvalue = "Abhishek";
                leveldb_set(db,testingkey,testingvalue);
                leveldb_get(db,testingkey,testingvalue);
        }
        clock_t dt1 = clock() - t1;
        cout << "set and read time elapsed: " << dt1 * 1.0e-6 << endl;


  delete db;
  //clock_t dt = clock() - t0;
  //cout << "time elapsed: " << dt * 1.0e-6 << " seconds" << endl;
  destroy_leveldb("leveldb_test_dir");
  exit(0);
}
