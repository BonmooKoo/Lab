#include "header.h"
#pragma warning(disable:4996)

hashtable::hashtable(int8_t size,Bucket** buffer,int8_t threshold) {
    //size=2^g  lobaldepth;
    this->threshold=threshold;
    this->buffer_counter=0;
    global_depth = size;  
    table = new Bucket * [(int)pow(2, global_depth)];
    this->buffer=buffer;
    initialize();
}
int hashtable::getSizeTable() {
    return pow(2, global_depth);
}
void hashtable::initialize() {
    for (int i = 0; i < (int)pow(2, global_depth); i++) {
        table[i] = new Bucket(global_depth);
    }
}
int hashtable::hashingKey(char* key) {
    unsigned h = FIRSTH;
    for(int i=0;i<KEY_SIZE;i++){
        h = (h * A) ^ (key[i] * B);
    }
    return h % (int)pow(2,global_depth);
}

int hashtable::insertKV(char* key, char* value) {
    //삽입
    int index = hashingKey(key);
    
    int rtnBucket = table[index]->insert(key, value);
    if (rtnBucket == -1) {
        //중복
        if(table[index]->refCount(threshold)){
            cacheing(index);
        }; 
        return -1;
    }
    else if (rtnBucket == -2) {
        //Double
        if (table[index]->getLocaldepth() == global_depth) {
            this->doubleTable();
        }
        
        //Split
        int size=this->getSizeTable();
        int bucketSize=table[index]->getSize();
            //New Bucket
        Bucket* newBucket=table[index]->split(index);
        int currentLocalDepth=table[index]->getLocaldepth();
        int jumpScale=pow(2,currentLocalDepth);
        int startPoint=newBucket->getHashValue();
            //New Bucket 가르키는 index 변경
        for(int i=startPoint;i<size;i+=jumpScale){
            table[i]=newBucket;
        }
        //다시 삽입
        index=hashingKey(key);
        table[index]->insert(key,value);
        if(table[index]->refCount(threshold)){
            cacheing(index);
        };
        return index;
    }
    else {
        //성공
        if(table[index]->refCount(threshold)){
            cacheing(index);
        };
        return index;
    }
}
char* hashtable::searchKV(char* key) {
    char* findValue;
    if((findValue=lookupBuffer(key))!=NULL){
        return findValue;
    };
    int index = hashingKey(key);
    findValue = table[index]->lookup(key);
    if (findValue == NULL) {
        return NULL;
    }
    table[index]->refCount(threshold);
    this->cacheing(index);
    return findValue;
}
void hashtable::doubleTable() {
    //새 hash table : temptable
    int total_index=this->getSizeTable();
    int temp_index =this->getSizeTable()*2;
    Bucket** temptable = new Bucket * [temp_index];
    //기존의 table(size= sizeof(Bucket*) * 2^depth개 )
    //새로운 table로 복사
    for (int j = 0; j < total_index; j++) {
        temptable[j] = (Bucket*)table[j];
        temptable[j + (int)pow(2, global_depth)] = (Bucket*)table[j];
    }
    this->table = temptable;
    global_depth++;
}
Bucket* hashtable::rtnBucket(int bucket){
    return table[bucket];
}
void hashtable::removeKV(char* key){
    this->removeBuffer(key);
    int index = hashingKey(key);
    table[index]->remove(key);
    if(table[index]->refCount(threshold)){
            cacheing(index);
    };
}
void hashtable::update(char* key,char* value){
    int index = hashingKey(key);
    table[index]->update(key,value);
    table[index]->refCount(threshold);
    if(table[index]->refCount(threshold)){
            cacheing(index);
    };

}
Bucket* hashtable::cacheing(int index){
    if(this->table[index]->getReferenceCounter()>=this->threshold){
        return table[index];
    }
    return NULL;
}
char* hashtable::lookupBuffer(char* key){
    for(int i=buffer_counter-1;i>=0;i--){
        
    }
    for(int i=BUFFER_SIZE-1;i>=buffer_counter;i--){
        
    }
}
void hashtable::removeBuffer(char* key){
    
}
