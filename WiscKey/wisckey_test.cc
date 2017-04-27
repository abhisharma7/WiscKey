#include "lab2_common.h"
#include <fstream>
#include <algorithm> 
#include <vector>      
#include <ctime>       
#include <cstdlib>    

// Author: Abhishek Sharma
// Program: WiscKey Key Value Store

typedef struct WiscKey {
  string dir;
  DB * leveldb;
  FILE * logfile;
} WK;

static bool wisckey_get(WK * wk, string &key, string &value)
{	
	cout << "\n\t\tGet Function\n\n";
	cout << "Key Received: " << key << endl;
	cout << "Value Received: " << value << endl;

	string offsetinfo;
        const bool found = leveldb_get(wk->leveldb, key, offsetinfo);
        if (found) {
       		cout << "Offset and Length: " << offsetinfo << endl;
        }
        else {
       	        cout << "Record:Not Found" << endl;
		return false;
        }
	std::string value_offset;
	std::string value_length;
	std::string s = offsetinfo;
	std::string delimiter = "&&";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
    		token = s.substr(0, pos);
		value_offset = token;
    		s.erase(0, pos + delimiter.length());
	}
	value_length = s;

	cout << "Value Offset: " << value_offset << endl;
	cout << "Value Length: " << value_length << endl;

  	std::string::size_type sz;
  	long offset = std::stol (value_offset,&sz);
	long length = std::stol (value_length,&sz);

	//cout << offset << length << endl;
	std::string value_record;
	//cout << ftell(wk->logread) << endl;
	fseek(wk->logfile,offset,SEEK_SET);
	//cout << ftell(wk->logfile) << endl;
	//rewind(wk->logfile);
	//cout << ftell(wk->logfile) << endl;	
	fread(&value_record,length,1,wk->logfile);
	//rewind(wk->logfile);
	cout << "LogFile Value: " << value_record << endl;
	return true;
}	

static void wisckey_set(WK * wk, string &key, string &value)
{
	long offset = ftell (wk->logfile);
	long size = sizeof(value);
	std::string vlog_offset = std::to_string(offset);
	std::string vlog_size = std::to_string(size);
	std::stringstream vlog_value;
	vlog_value << vlog_offset << "&&" << vlog_size;
	std::string s = vlog_value.str();	
	fwrite (&value, sizeof(value),1,wk->logfile);
	leveldb_set(wk->leveldb,key,s);
}

static void wisckey_del(WK * wk, string &key)
{
 	cout << "Key: " << key << endl; 
	leveldb_del(wk->leveldb,key);
	
}

static WK * open_wisckey(const string& dirname)
{
	WK * wk = new WK;
	wk->leveldb = open_leveldb(dirname);
  	wk->dir = dirname;
	wk->logfile = fopen("logfile","wb+");
  	return wk;
}

static void close_wisckey(WK * wk)
{
	fclose(wk->logfile);
  	delete wk->leveldb;
  	delete wk;
}


// For testing wisckey functionality 
static void testing_function(WK * wk, string &key, string &value) 
{
/* Setting Value and Testing it */
        
	cout << "\n\n\t\tInput Received\n" << endl;
	cout << "Key: " << key << endl;
        cout << "Value: " << value << endl;
	wisckey_set(wk,key,value);
	const bool found = wisckey_get(wk,key,value);
	if (found) {
		cout << "Record Matched" << endl;
	}

/* Deleting Value */
	cout << "\n\n\t\tDelete Operation\n" << endl;
	wisckey_del(wk,key);
	cout << "Delete Successful" << endl;
	
/* Read after Delete */
	cout << "\n\n\t\tInput Received\n" << endl;
        string testkey= "1001224314";
        string testvalue = "Abhishek Sharma";
        cout << "Key: " << testkey << endl;
        cout << "Value: " << testvalue << endl;
	const bool testfound = wisckey_get(wk,testkey,testvalue);
        if (testfound) {
               cout << "Record Matched" << endl;
        }
}	

int main(int argc, char ** argv)
{
	if (argc < 2) {
    		cout << "Usage: " << argv[0] << " <value-size>" << endl;
    		exit(0);
  	}
  	const size_t value_size = std::stoull(argv[1], NULL, 10);
  	if (value_size < 1 || value_size > 100000) {
    		cout << "  <value-size> must be positive and less then 100000" << endl;
   	 	exit(0);
  	}

  	WK * wk = open_wisckey("wisckey_test_dir");
  	if (wk == NULL) {
    		cerr << "Open WiscKey failed!" << endl;
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
    		wisckey_set(wk, key, value);	
   		if (j >= p1) {
      			clock_t dt = clock() - t0;
      			cout << "progress: " << j+1 << "/" << nfill << " time elapsed: " << dt * 1.0e-6 << endl << std::flush;
      			p1 += (nfill / 40);
    		}    
  	}

  	clock_t dt = clock() - t0;
  	cout << "time elapsed: " << dt * 1.0e-6 << " seconds" << endl;
        close_wisckey(wk);
        destroy_leveldb("wisckey_test_dir");       
        remove("logfile");
        exit(0);
}
