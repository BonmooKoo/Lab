#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <cmath>

#define BUCKET_SIZE 1024//Byte
#define KEY_SIZE 8      //Byte
#define VALUE_SIZE 8    //Byte
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
        // int8_t fingerprint;
        bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)-1];
        char array[BUCKET_SIZE-sizeof(int8_t)-sizeof(bitmap)];
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
        void writeBucket(int fd,int offset);
};

class hashtable{
    private:
        int8_t global_depth;
        Bucket** table;
        char* buffer;

    public:
        hashtable(int8_t size);
        int insertKV(char* key, char* value);
        void doubleTable();
        void removeKV(char* key);
        void initialize();
        char* searchKV(char* key);
        int getSizeTable();
        int hashingKey(char *key);
        Bucket* rtnBucket(int bucket);  
        void update(char* key,char* value);
        void writeBucket(int fd);
        void readBucket(int fd);
};

#endif 