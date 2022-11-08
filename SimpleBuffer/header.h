#include <iostream>
#include <cstring>
#include <cmath>
//SIZE
#define BUCKET_SIZE 300//Byte
#define KEY_SIZE 8      //Byte
#define VALUE_SIZE 8    //Byte
#define BUFFER_SIZE 100  //Byte
//FOR HASHFUNCTION
#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

using namespace std;

#ifndef _HEADER_H_
#define _HEADER_H_


class Bucket
{
    private:
        //64
        int8_t local_depth;//1
        bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)-2];//62
        char array[BUCKET_SIZE-sizeof(int8_t)-sizeof(bitmap)]; //60 * 16 B
    public:
        Bucket();
        Bucket(int8_t local_depth);
        int8_t getLocaldepth();
        void addLocaldepth();
        int insert(char* key,char* value); //-1은 중복 -2는 bucketfull
        void update(char* key,char* value);
        bool remove(char* key);
        char* lookup(char* key);
        void initialize();
        int getSize();
        Bucket* split(int index);
        void checkBucket();
        int getHashValue();
};

class hashtable{
    private:
    //TABLE_SIZE/BUCKET_SIZE
        int8_t global_depth;
        int8_t threshold;
        Bucket** table;
        char* buffer;
        int8_t buffer_counter;
    public:
        hashtable(int8_t size,char* buffer);
        int insertKV(char* key, char* value);
        void doubleTable();
        void removeKV(char* key);
        void initialize();
        char* searchKV(char* key);
        int getSizeTable();
        int hashingKey(char *key);
        Bucket* rtnBucket(int bucket);  
        void update(char* key,char* value);
        
        Bucket* cacheingKV(char* key, char* value);
        char* lookupBuffer(char* key);
        void removeBuffer(char* key);
        void addBufCounter();
};

#endif 