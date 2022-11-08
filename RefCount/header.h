#include <iostream>
#include <cstring>
#include <cmath>
//SIZE
#define BUCKET_SIZE 300//Byte
#define KEY_SIZE 8      //Byte
#define VALUE_SIZE 8    //Byte
#define BUFFER_SIZE 10  //Byte
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
        int8_t reference_counter; //1
        bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)-2];//62
        char array[BUCKET_SIZE-sizeof(int8_t)-sizeof(int8_t)-sizeof(bitmap)]; //60 * 16 B
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
        int8_t getReferenceCounter();
        bool refCount(int threshold);
        int getHashValue();
};

class hashtable{
    private:
    //TABLE_SIZE/BUCKET_SIZE
        int8_t global_depth;
        int8_t threshold;
        Bucket** table;
        Bucket** buffer;
        int8_t buffer_counter;
    public:
        hashtable(int8_t size,Bucket** buffer,int8_t threshold);
        int insertKV(char* key, char* value);
        void doubleTable();
        void removeKV(char* key);
        void initialize();
        char* searchKV(char* key);
        int getSizeTable();
        int hashingKey(char *key);
        Bucket* rtnBucket(int bucket);  
        void update(char* key,char* value);
        

        /*
        Basic reference counter idea:
        모든 insert, search, update, remove 시 reference counter ++
        if reference counter >= threshold {
            bucket을 buffer에 저장
        }
        
        search : 
            먼저 buffer을 확인
            buffer에 bucket이 있으면 해당 bucket 확인

        insert : 
            먼저 buffer을 확인
        */
        Bucket* cacheing(int index);
        char* lookupBuffer(char* key);
        void removeBuffer(char* key)
};

#endif 