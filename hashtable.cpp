#include "header.h"
#include <fstream>
#include <string>
hashtable::hashtable(int8_t size){
    //size=2^g  lobaldepth;
    global_depth=size;
    table=new Bucket*[(int)pow(2,global_depth)];
    
    initialize();
}
int hashtable::getSizeTable(){
    return pow(2,global_depth);
}
void hashtable::initialize(){
    for(int i=0;i<(int)pow(2,global_depth);i++){
    //for(int i=0;i<2;i++){
        table[i]=new Bucket(global_depth);
    }
}
int hashtable::hashingKey(char* key){
    //간단한 hash func
    unsigned int i = 0;
    for (int j=0; key[j]; j++){
        i += key[j];
    }
    //printf("==%d:%d:%d\n",i,(int)(pow(2,global_depth)),i % (int)(pow(2,global_depth)));
    return i % (int)(pow(2,global_depth));
}

int hashtable::insertKV(char* key,char* value){
    //삽입
    int index=hashingKey(key);
    int rtnBucket=table[index]->insert(key,value);
    if(rtnBucket==-1){
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
        //index=기존 bucket의 hashtable에서의 index값
        Bucket* newBucket=table[index]->split(index,global_depth);
        //새 bucket을 table 의 index에 넣어줌 
        //ex 기존 bucket 이 4이고 (depth=2)
        //   | | / / 이면
        //   | | | / 으로 바꿔주는 거니까
        //   0,2 을 split하면 
        //   0(기존 index) 에서 2^(depth-1)만큼 더해준 곳이 새 bucket이다.
        //cout<<"setBucket"<<endl;
        table[index+(int)pow(2,global_depth-1)]=newBucket;
        //cout<<"reinsert"<<endl;
        //3. 
        index=hashingKey(key);
        table[index]->insert(key,value); 
        // cout<<table[index]->lookup(key)<<endl;
        // cout<<"reinsert end"<<endl;
        return index;
    }
    else{
        //cout<<"Success"<<endl;
        return index;
    }
}
char* hashtable::searchKV(char* key){
    //
    int index=hashingKey(key);
    char* findValue=(char*)malloc(VALUE_SIZE);
    strncpy(findValue,table[index]->lookup(key),VALUE_SIZE);
    if(findValue==NULL){
        cout<<key<<":NO!"<<endl;
        return NULL;
    }
    return findValue;
}
void hashtable::doubleTable(){
    cout<<"startDoubling++++++++++++++++++++++++++++++++++++++"<<endl;
    //새 hash table : temptable
    int temp_index=(int)pow(2,global_depth+1);
    Bucket** temptable=new Bucket*[temp_index];
    //기존의 table(size= sizeof(Bucket*) * 2^depth개 )
    //새로운 table로 복사
    //memcpy(temptable,table,pow(2,global_depth)*sizeof(Bucket*));
    for(int j=0;j<pow(2,global_depth);j++){
        // printf("doubledepth:%d\n",table[j]->getLocaldepth());
        temptable[j]=table[j];
        temptable[j+(int)pow(2,global_depth)]=table[j];
    }
    
    table=temptable;
    //다했으니 global_depth를 1증가
    global_depth=global_depth+1;
}

int main(){
    hashtable ht(1);
    char* key=(char*)malloc(KEY_SIZE);
    char* value=(char*)malloc(VALUE_SIZE);
    int num=999;
    ifstream is("input.txt");
    if(is.is_open()){
        for(int i=0;i<num;i++){
            string input;
            getline(is,input);
            key=(char*)input.c_str();
            //ht.insertKV(key,key);
            cout<<"insert:"<<i<<":"<<input<<":"<<ht.insertKV(key,key)<<endl;;
            
        }   
    }
    is.close();
    cout<<"find"<<endl;
    ifstream i2s("input.txt");
    if(i2s.is_open()){
        for(int i=0;i<num;i++){
            string input;
            getline(i2s,input);
            key=(char*)input.c_str();
            cout<<"search:"<<i<<":"<<input<<":"<<endl;
            cout<<ht.searchKV(key)<<endl;
        }   
    } 
    i2s.close();
}