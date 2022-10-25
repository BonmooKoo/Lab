#include "header.h"

hashtable::hashtable(int8_t size){
    table=new Bucket*[size];
    table_size=size;
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
    for (int j=0; key[j]; j++)
        i += key[j];
    return i % getSizeTable();
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
            index=hashingKey(key);//doubling으로 key값이 바뀜
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

}

