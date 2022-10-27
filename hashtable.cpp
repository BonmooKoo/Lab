#include "header.h"

hashtable::hashtable(int8_t size){
    //size=2^globaldepth;
    global_depth=size;
    table=new Bucket*[pow(2,global_depth)];
    initialize();
}
int hashtable::getSizeTable(){
    return pow(2,global_depth);
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
}

int hashtable::insertKV(char* key,char* value ){
    //삽입
    int index=hashingKey(key);
    int rtnBucket=0;
    if((rtnBucket=table[index]->insert(key,value))==-1){
        //중복이 있거나 삽입에 실패
        return -1;
    }else if(rtnBucket==-2){
        //해당하는 bucket이 꽉참
        //1. doubling 이 필요할 경우 directory doubling
        //2. split
        //3. 다시 해당 key - value를 다시 넣어줌
        
        //1.
        if(table[index]->getLocaldepth()==global_depth){
            doubleTable();    
        }
        
        //2. split
        
        // 이부분 newBucket으로 옮기는거 이걸 다시 해야함.
        Bucket* newBucket=table[index]->split(key);
        //새 bucket을 table 의 index에 넣어줌 
        //ex 기존 bucket 이 4이고 (depth=2)
        //   | | / / 이면
        //   | | | / 으로 바꿔주는 거니까
        //   1,3 을 split하면 
        //   1 에서 2^(depth-1)만큼 더해준 곳이 새 bucket이다.
        table[index+(int)pow(2,global_depth-1)]=newBucket;
        
        
        //3. 
        index=hashingKey(key);
        table[index]->insert(key,value); 
        
        return rtnBucket;
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
    //되는지 확인 필요

    //임시 hash table
    Bucket** temptable=new Bucket*[pow(2,global_depth+1)];
    //기존의 table(size= sizeof(Bucket*) * 2^depth개 )
    //새로운 table로 복사
    memcpy(temptable,table,pow(2,global_depth)*sizeof(Bucket*));
    table=temptable;
    //같은 위치를 가리키도록 변경
    for(int i=0;i<global_depth;i++){
        table[(int)pow(2,global_depth)+i]=table[i];
    }
    //다했으니 global_depth를 1증가
    global_depth++;
    
}

