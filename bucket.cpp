#include <iostream>
#include <cstring>
#define BUCKET_SIZE 1024
#define KEY_SIZE 8
#define VALUE_SIZE 8
using namespace std;

class Bucket
{
    private:
        int8_t local_depth;
        bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)-1];
        char array[BUCKET_SIZE-sizeof(int8_t)-sizeof(bitmap)];
    public:
        bool insert(char* key,char* value);
        void update(char* key,char* value);
        bool remove(char* key);
        char* lookup(char* key);
        void initialize();
        int getSize();
};
void Bucket::initialize(){
    for(int i=0;i<getSize();i++){
        bitmap[i]=false;
    }
}
int Bucket::getSize(){
    return sizeof(array)/(KEY_SIZE+VALUE_SIZE);
}
bool Bucket::insert(char* key,char* value){
    //중복확인을 해야하네?
    for(int i=0;i<getSize();i++){
        if(bitmap[i]==false){
            //startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
            strncpy(array+i*(KEY_SIZE+VALUE_SIZE),key,KEY_SIZE);
            strncpy(array+i*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,value,VALUE_SIZE);
            bitmap[i]=true;
            return true;
        }
    }
    return false;
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
int main(){
    Bucket bucket;
    bucket.initialize();
    int bucketsize=bucket.getSize();
    char inkey[KEY_SIZE];
    char invalue[VALUE_SIZE];
    //printf("%d",sizeof(bucket.array));
    for(int j=0;j<bucketsize;j++){
        for(int i=0;i<KEY_SIZE;i++){
            inkey[i]='z'-j;
        }
        for(int i=0;i<VALUE_SIZE;i++){
            invalue[i]='z'-j;
        }
        bucket.insert(inkey,invalue);
    }
    printf("start\n");
    for(int j=0;j<bucketsize;j++){
        for(int i=0;i<KEY_SIZE;i++){
            inkey[i]='z'-j;
        }
        char* output;
        output=bucket.lookup(inkey);
        printf("%d:",j);
        printf("%s\n",output);
    }
    for(int i=0;i<KEY_SIZE;i++){
            inkey[i]='z'-3;
    }
    bucket.remove(inkey);
    char* output=bucket.lookup(inkey);
    printf("%s\n",output);
}