#include "header.h"

Bucket::Bucket(){
    initialize();
}
void Bucket::initialize(){
    local_depth=1;
    for(int i=0;i<getSize();i++){
        bitmap[i]=false;
    }
}
int Bucket::getSize(){
    return sizeof(array)/(KEY_SIZE+VALUE_SIZE);
}
int Bucket::insert(char* key,char* value){
    //중복확인을 해야하네?
    for(int j=0;j<getSize();j++){
        if(bitmap[j]==true){
            if(strncmp(array+j*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,value,VALUE_SIZE)==0){
                return -1;
            }
        }
    }
    for(int i=0;i<getSize();i++){
        if(bitmap[i]==false){
            //startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
            strncpy(array+i*(KEY_SIZE+VALUE_SIZE),key,KEY_SIZE);
            strncpy(array+i*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,value,VALUE_SIZE);
            bitmap[i]=true;
            return i;
        }
    }
    printf("Bucket: insert Error");
    return NULL;
}

char* Bucket::lookup(char* key){
    char* rtnvalue=(char*)malloc(VALUE_SIZE);
    for(int i=0;i<getSize();i++){
        if(bitmap[i]==true){
            //startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
            if(strncmp(array+i*(KEY_SIZE+VALUE_SIZE),key,KEY_SIZE)==0){
                strncpy(rtnvalue,array+i*(KEY_SIZE+VALUE_SIZE)+VALUE_SIZE,VALUE_SIZE);
                return rtnvalue;
            }
        }
    }
    return NULL;
}
bool Bucket::remove(char* key){
    char* erasekey=(char*)malloc(KEY_SIZE);
    char* erasevalue=(char*)malloc(VALUE_SIZE);
    for(int i=0;i<getSize();i++){
        if(bitmap[i]==true){
            //startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
            if(strncmp(array+i*(KEY_SIZE+VALUE_SIZE),key,KEY_SIZE)==0){
                strncpy(array+i*(KEY_SIZE+VALUE_SIZE),erasekey,KEY_SIZE);
                strncpy(array+i*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,erasevalue,VALUE_SIZE);
                return true;
            }
        }
    }
    return false;
}
Bucket* Bucket::split(int hash,int global_depth){
    //
    Bucket* newBucket=new Bucket();
    local_depth++;
    for(int i=0;i<getSize();i++){
        char* key;
        char* value;
        strncpy(key,array+i*(KEY_SIZE+VALUE_SIZE),KEY_SIZE);
        //new bucket으로 가야하는 값
        int hashingKey=0;
        for (int j=0; key[j]; j++){
        hashingKey += key[j];
        }
        hashingKey= hashingKey % (long)(pow(2,global_depth));
        //기존버킷의 hash값과 다르다면 새로운 버킷으로 옮긴다.
        if(hashingKey!=hash){
            strncpy(value,array+i*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,VALUE_SIZE);            
            delete(key);
            newBucket->insert(key,value);
        }
    }
    return newBucket;
}
int8_t Bucket::getLocaldepth(){
    return local_depth;
}
void Bucket::addLocaldepth(){
    local_depth++;
}

// int main(){
//     Bucket bucket;
//     bucket.initialize();
//     int bucketsize=bucket.getSize();
//     char inkey[KEY_SIZE];
//     char invalue[VALUE_SIZE];
//     //printf("%d",sizeof(bucket.array));
//     for(int j=0;j<bucketsize;j++){
//         for(int i=0;i<KEY_SIZE;i++){
//             inkey[i]='z'-j;
//         }
//         for(int i=0;i<VALUE_SIZE;i++){
//             invalue[i]='z'-j;
//         }
//         bucket.insert(inkey,invalue);
//     }
//     printf("start\n");
//     for(int j=0;j<bucketsize;j++){
//         for(int i=0;i<KEY_SIZE;i++){
//             inkey[i]='z'-j;
//         }
//         char* output;
//         output=bucket.lookup(inkey);
//         printf("%d:",j);
//         printf("%s\n",output);
//     }
//     for(int i=0;i<KEY_SIZE;i++){
//             inkey[i]='@';
//     }
    
//     char* output=bucket.lookup(inkey);
//     printf("%s\n",output);
//     bucket.remove(inkey);

//     char* output1=bucket.lookup(inkey);
//     printf("%s\n",output1);

// }~