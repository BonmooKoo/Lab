#include "header.h"
#pragma warning(disable:4996)

hashtable::hashtable(int8_t size) {
    //size=2^g  lobaldepth;
    threshold=1000;
    global_depth = size;  
    table = new Bucket * [(int)pow(2, global_depth)];

    initialize();
}
int hashtable::getSizeTable() {
    return pow(2, global_depth);
}
void hashtable::initialize() {
    for (int i = 0; i < (int)pow(2, global_depth); i++) {
        //for(int i=0;i<2;i++){
        table[i] = new Bucket(global_depth);
    }
}
int hashtable::hashingKey(char* key) {
    int pow=1;
    for(int i=0;i<global_depth;i++){
        pow*=2;
    }
    unsigned h = FIRSTH;
    for(int i=0;i<KEY_SIZE;i++){
        h = (h * A) ^ (key[i] * B);
    }
    cout<<key<<endl;
    printf("key=%s\nh=%u,locate=%d\n",key,h,h%pow);
    return h % pow;
}

int hashtable::insertKV(char* key, char* value) {
    //삽입
    int index = hashingKey(key);
    
    int rtnBucket = table[index]->insert(key, value);
    if (rtnBucket == -1) {
        //중복
        return -1;
    }
    else if (rtnBucket == -2) {
        //Split
        if (table[index]->getLocaldepth() == global_depth) {
            this->doubleTable();
        }
        Bucket* newBucket = table[index]->split(index,this->global_depth);
        table[index + (int)pow(2, global_depth - 1)] = newBucket;
        //다시 삽입
        this->insertKV(key,value);
        table[index]->refCount();
        return index;
    }
    else {
        //성공
        return index;
    }
}
char* hashtable::searchKV(char* key) {
    int index = hashingKey(key);
    char* findValue;
    findValue = table[index]->lookup(key);
    printf("LookupKey=%s\nlookupBucket=%d\n",key,index);
    if (findValue == NULL) {
        return NULL;
    }
    table[index]->refCount();
    return findValue;
}
void hashtable::doubleTable() {
    //새 hash table : temptable
    printf("%d:DOUBLE++++++++++++++++++++++++++++++\n",global_depth);
    int total_index=this->getSizeTable();
    int temp_index =this->getSizeTable()*2;
    Bucket** temptable = new Bucket * [temp_index];
    //기존의 table(size= sizeof(Bucket*) * 2^depth개 )
    //새로운 table로 복사
    for (int j = 0; j < total_index; j++) {
        temptable[j] = (Bucket*)table[j];
        temptable[j + (int)pow(2, global_depth)] = (Bucket*)table[j];
        //같은 bucket을 가리킴
    }
    this->table = temptable;
    //다했으니 global_depth를 1증가
    global_depth++;
}
Bucket* hashtable::rtnBucket(int bucket){
    return table[bucket];
}
void hashtable::removeKV(char* key){
    int index = hashingKey(key);
    table[index]->remove(key);
    table[index]->refCount();
}
void hashtable::update(char* key,char* value){
    int index = hashingKey(key);
    table[index]->update(key,value);
    table[index]->refCount();

}
Bucket* hashtable::cacheing(int index){
    if(this->table[index]->getReferenceCounter()>=this->threshold){
        return table[index];
    }
    return NULL;
}