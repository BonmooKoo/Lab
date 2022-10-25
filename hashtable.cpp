#include "header.h"

int hashtable::getSizeTable(){
    return TABLE_SIZE/BUCKET_SIZE-sizeof(int8_t);
}
void hashtable::initialize(){
    global_depth=1;
}
int hashtable::hashingKey(char* key){
    //간단한 hash func
    unsigned long i = 0;
    for (int j=0; key[j]; j++)
        i += key[j];
    return i % getSizeTable();
    return;
}
Bucket* hashtable::split(Bucket* splitBucket){
    Bucket* newBucket=new Bucket();
    for(int i=0;i<splitBucket->getSize();i++){
        
        
        newBucket->insert();
    }
    //
    addLocaldepth();
    return newBucket;
}
int hashtable::insertKV(char* key,char* value ){
    //삽입
    int index=hashingKey(key);
    int rtnBucket=0;
    if((rtnBucket=table[index]->insert(key,value))==-1){
        //중복이 있거나 삽입에 실패
        return -1;
    }
    else{
        return rtnBucket;
    }
}
char* hashtable::searchKV(char* key){
    //
    int index=hashingKey(key);
    char* rtnValue;
    rtnValue=table[index]->lookup(key);
    return NULL;
}

