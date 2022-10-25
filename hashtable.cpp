#include "header.h"

hashtable::hashtable(int8_t size){
    //size=2^globaldepth;
    global_depth=size;
    table=new Bucket*[pow(2,global_depth)];
    initialize();
}
int hashtable::getSizeTable(){
    return TABLE_SIZE/BUCKET_SIZE-sizeof(int8_t);
}
void hashtable::initialize(){
    global_depth=1;
}
int hashtable::hashingKey(char* key){
    //간단한 hash func
    unsigned long i = 0;
    for (int j=0; key[j]; j++){
        i += key[j];
    }
    return i % (long)(pow(2,global_depth));
    return;
}

int hashtable::insertKV(char* key,char* value ){
    //삽입
    int index=hashingKey(key);
    int rtnBucket=0;
    if((rtnBucket=table[index]->insert(key,value))==-1){
        //중복이 있거나 삽입에 실패
        return -1;
    }else if(rtnBucket==-2){
        //꽉참
        if(table[index]->getLocaldepth()<global_depth){
            Bucket* newBucket=table[index]->split(key);
        }else{
            doubleTable();
            //doubling으로 table_size가 변했으니 hashingkey도 다시! 
            index=hashingKey(key);
            //다시 insert
            table[index]->insert(key,value);
            return rtnBucket;
        }
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
void hashtable::doubleTable(){
    Bucket** temptable=new Bucket*[pow(2,global_depth+1)];
    //되는지 확인 필요
    memcpy(temptable,table,pow(2,global_depth));
    table=temptable;
    global_depth++;
}

