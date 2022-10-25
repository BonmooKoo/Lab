#include <iostream>
#include <cstring>
#define BUCKET_SIZE 1024//Byte
#define KEY_SIZE 8      //Byte
#define VALUE_SIZE 8    //Byte
#define TABLE_SIZE 1024*16

using namespace std;

#ifndef _HEADER_H_
#define _HEADER_H_

class Bucket
{
    private:
        //64
        int8_t local_depth;//1
        // int8_t version_number; //1
        bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)-1];//63
        //bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)-2];//62
        char array[BUCKET_SIZE-sizeof(int8_t)-sizeof(bitmap)]; //60 * 16 B
    public:
        Bucket();
        int8_t getLocaldepth();
        void addLocaldepth();
        int insert(char* key,char* value); //-1은 중복 -2는 bucketfull
        void update(char* key,char* value);
        bool remove(char* key);
        char* lookup(char* key);
        void initialize();
        int getSize();
        Bucket* split(char* hashkey);
};

class hashtable{
    private:
    //TABLE_SIZE/BUCKET_SIZE
        int8_t global_depth;
        //char* cache[CACHE_SIZE]
        //Bucket* table[TABLE_SIZE/BUCKET_SIZE-sizeof(int16_t)-sizeof(cache)];//1024
        Bucket* table[TABLE_SIZE/BUCKET_SIZE-sizeof(int16_t)];//1024
    public:

        int insertKV(char* key, char* value);
        void doubleTable();
        void removeKV(char* key);
        void initialize();
        char* searchKV(char* key);
        int getSizeTable();
        int hashingKey(char *key);
        Bucket* split(Bucket* splitBucket);
    
};

#endif 