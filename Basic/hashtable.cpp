#include "header.h"
#pragma warning(disable:4996)

hashtable::hashtable(int8_t size) {
    //size=2^g  localdepth;
    global_depth = size;
    table = new Bucket * [(int)pow(2, global_depth)];
    initialize();
}

int hashtable::getSizeTable() {
    return pow(2, global_depth);
}
void hashtable::initialize() {
    for (int i = 0; i < (int)pow(2, global_depth); i++) {
        table[i] = new Bucket(global_depth);
        table[i]->setpagenumber(i);
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
        printf("split:%d,newBucketindex:%d\n",index,startPoint);
        newBucket->setpagenumber(startPoint);
            //New Bucket 가르키는 index 변경
        
        for(int i=startPoint;i<size;i+=jumpScale){
            table[i]=newBucket;  
             
        }
        if(index!=startPoint){
            table[index]->setpagenumber(index);
            for(int i=index;i<size;i+=jumpScale){
                table[i]=table[index];
            }
        }
        
        //다시 삽입
        index=hashingKey(key);
        table[index]->insert(key,value);
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
    if (findValue == NULL) {
        return NULL;
    }
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
    int index = hashingKey(key);
    table[index]->remove(key);
}
void hashtable::update(char* key,char* value){
    int index = hashingKey(key);
    table[index]->update(key,value);
}
void hashtable::writeBucket(int fd){
    int size=this->getSizeTable();
    int offset=0;
    for(int i=0;i<size;i++){
        //if bucket의 pagenum == 해당하는 index 일경우에만 write
        if(table[i]->getpagenumber()==i){
            printf("write:%d\n",i);
            write(fd,table[i],BUCKET_SIZE);
        }
    }
    // for(int i=0;i<size/4;i++){
    //     for(int j=0;j<4;j++){
    //         if(table[i]->getpagenumber()==i){
    //         write(fd,table[i],BUCKET_SIZE);
    //         }
    //     }
    //     //bucket의 pagenumber != table index
    // }
    
}

void hashtable::readBucket(int fd){
    //txt를 읽어서 Bucket을 생성
    lseek(fd,0,SEEK_SET);
    int offset=0;
    int rtn=0;
    char* buffer=(char*)malloc(BUCKET_SIZE);
    while(read(fd,buffer,BUCKET_SIZE)>0){
        printf("read\n");
        //읽은 내용을 bucket에 써주기
        Bucket* newBucket=new Bucket();
        memcpy(newBucket,buffer,BUCKET_SIZE);
        int index=newBucket->getpagenumber();
        //만약 index가 현재 table보다 클경우 directory doubling 이 필요!
        while(index+1>this->getSizeTable()){
            printf("double\n");
            this->doubleTable();


        }
        int size=this->getSizeTable();
        table[index]=newBucket;
        int8_t localdepth=newBucket->getLocaldepth();
        //localdepth==global_depth일 경우,
          

    }
    /*
    int size=this->getSizeTable();
    int offset=0;
    int rtn=0;
    char* buffer=(char*)malloc(4096);
    
    for(int i=0;i<size/4;i++){
        read(fd,buffer,4096);
        //읽은 내용을 bucket에 써주기
        int readCount=4096/BUCKET_SIZE;
        for(int j=0;j<readCount;j++){
            Bucket* newBucket=new Bucket();
            memcpy(newBucket,buffer+(BUCKET_SIZE*j),BUCKET_SIZE);
            int index=newBucket->getpagenumber();
           
            //만약 index가 현재 table보다 클경우 directory doubling 이 필요!
            while(index>this->getSizeTable()){
                this->doubleTable();
            }
            table[index]=newBucket;
            int8_t localdepth=newBucket->getLocaldepth();
            //localdepth==global_depth일 경우,
            if(localdepth!=this->global_depth){
                int8_t time=(int8_t)pow(2,global_depth-localdepth)-1;
                int8_t gap=size/time;
                //ex gd = 4 ld =1 
                //그러면 총 2^(4-1)=8개의 Bucket* 가 해당 bucket을 가리키고 있다는 뜻 
                //이 경우 : 복사 횟수 (time)=8-1 회
                //          복사하는 간격 = 시작에서부터 
                for(int k=1;k<=time;k++){
                    table[index+gap*k]=newBucket;
                }   
            }
        }
    }
    */
}
