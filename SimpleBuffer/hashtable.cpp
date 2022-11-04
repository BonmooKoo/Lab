#include "header.h"
#pragma warning(disable:4996)

hashtable::hashtable(int8_t size) {
    //size=2^g  lobaldepth;
    global_depth = size;  
    table = new Bucket * [(int)pow(2, global_depth)];

    initialize();
}
hashtable::hashtable(int8_t size,char* buffer,int8_t threshold) {
    //size=2^g  lobaldepth;
    threshold=1000;
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
        table[index]->refCount();    
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
        table[index]->refCount();
        return index;
    }
    else {
        //성공
        table[index]->refCount();
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
    table[index]->refCount();
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
    table[index]->refCount();
    this->cacheing(key,value);
}
void hashtable::update(char* key,char* value){
    int index = hashingKey(key);
    table[index]->update(key,value);
    table[index]->refCount();
    this->cacheing(key,value);

}
Bucket* hashtable::cacheing(int index){
    
}
char* hashtable::lookupBuffer(char* key){
    char *rtnvalue = (char *)calloc(KEY_SIZE+1, sizeof(char));   
    for(int i=buffer_counter;i<BUFFER_SIZE;i++){
        if (strncmp(buffer + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE) == 0)
            {
                memcpy(rtnvalue, buffer + i * (KEY_SIZE + VALUE_SIZE) + VALUE_SIZE, VALUE_SIZE);
                return rtnvalue;
            }
    }
    for(int i=0;i<buffer_counter;i++){
        if (strncmp(buffer + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE) == 0)
            {
                memcpy(rtnvalue, buffer + i * (KEY_SIZE + VALUE_SIZE) + VALUE_SIZE, VALUE_SIZE);
                return rtnvalue;
            }
    }
    return NULL;
}
void hashtable::removeBuffer(char* key){
    for(int i=0;i<BUFFER_SIZE;i++){
        if (strncmp(buffer + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE) == 0)
            {
                memset(buffer + i * (KEY_SIZE + VALUE_SIZE), 0, KEY_SIZE);
                memset(buffer+ i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, 0, VALUE_SIZE);
            }
    }
}
void hashtable::addBufCounter(){
    buffer_counter++;
    if(buffer_counter>BUFFER_SIZE){
        buffer_counter=0;
    }
}