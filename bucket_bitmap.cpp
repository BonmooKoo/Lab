#include <iostream>
#include <cstring>
#define BUCKET_SIZE 1024
#define KEY_SIZE 8
#define VALUE_SIZE 8
#define flag1 0x1 //hex for 0000 0001
#define flag2 0x2 //hex for 0000 0010
#define flag3 0x4 //hex for 0000 0100
#define flag4 0x8 //hex for 0000 1000
#define flag5 0x10 //hex for 0001 0000
#define flag6 0x20 //hex for 0010 0000
#define flag7 0x40 //hex for 0100 0000
#define flag8 0x80 //hex for 1000 0000

using namespace std;

class Bucket
{
    private:
        int8_t local_depth;//0~63
        //BUCKET_SIZE/(KEY_SIZE+VALUE_SIZE) < 들어갈수있는 k-v pair 개수 (지금은 64개)
        //64 bit = 8byte = 8char >> 넉넉하게 2개로 (k-v size절반으로 해도 견딜 수 있게)
        //bitmap (16 byte) / k1 (8byte) -v1(8byte) / k2 (8byte) -v2(8byte) / k3 (8byte) -v3(8byte) ..... / k64 (8byte) -v64(8byte)  
        char bitmap[2]; //(16byte)
        char array[BUCKET_SIZE-sizeof(int8_t)-sizeof(bitmap)];
    public:
        int insert(char* key,char* value);
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
int Bucket::insert(char* key,char* value){
    //중복확인을 해야하네?
    for(int j=0;j<getSize();j++){
        if(bitmap[j]==true){
            if(strncmp(array+j*(KEY_SIZE+VALUE_SIZE)+KEY_SIZE,value,VALUE_SIZE)==0){
                //중복
                return j;
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
    return -1;
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
    printf("end\n");
}