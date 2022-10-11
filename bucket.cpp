#include <iostream>
#include <cstring>
#define BUCKET_SIZE 1024
#define KEY_SIZE 16
#define VALUE_SIZE 16
using namespace std;

class Bucket
{
    private:
        char array[BUCKET_SIZE];
        int8_t local_depth;
        bool bitmap[BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE)];
        //32칸 첫째칸(32B) : 1B:depth 32bit(4B):bitmap 나머지는? 너무 낭비다.
        //
    public:
        bool insert(char* key,char* value);
        void update(char* key,char* value);
        void remove(char* key);
        char* lookup(char* key);
        void initialize();
};

bool Bucket::insert(char* key,char* value){
    //중복확인을 해야하네?
    for(int i=1;i<32;i++){
        if(bitmap[i]==false){
            //startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
            strncpy(array+i*(KEY_SIZE+VALUE_SIZE),key,KEY_SIZE);
            strncpy(array+i*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,value,VALUE_SIZE);
            //printf("key:%s\n",array+i*(KEY_SIZE+VALUE_SIZE));
            bitmap[i]=true;
            return true;
        }
    }
    return false;
}
char* Bucket::lookup(char* key){
    char* rtnvalue=(char*)malloc(VALUE_SIZE);
    for(int i=1;i<32;i++){
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

int main(){
    Bucket bucket;
    char inkey[16];
    for(int i=0;i<KEY_SIZE;i++){
        inkey[i]='a'+i;
    }
    char invalue[16];
    for(int i=0;i<KEY_SIZE;i++){
        invalue[i]='a'+i;
    }
    bucket.insert(inkey,invalue);
    char* output;
    output=bucket.lookup(inkey);
    printf("%s\n",output);
}