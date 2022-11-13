#include "header.h"
#pragma warning(disable : 4996)

Bucket::Bucket()
{
    initialize();
    fingerprint=9;
    local_depth = 1;
}
Bucket::Bucket(int8_t depth){
    initialize();
    fingerprint=9;
    local_depth = depth;
}
void Bucket::initialize()
{   
    for (int i = 0; i < getSize(); i++)
    {
        bitmap[i] = false;
    }
}
int Bucket::getSize()
{
    return sizeof(array) / (KEY_SIZE + VALUE_SIZE);
}
int Bucket::insert(char *key, char *value)
{
    // 중복
    int size = getSize();
    for (int j = 0; j < size; j++)
    {
        if (bitmap[j] == true)
        {
            if (strncmp(array + j * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, value, VALUE_SIZE) == 0)
            {
                cout << "중복이 존재!" << value << endl;
                return -1;
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (bitmap[i] == false)
        {
            memcpy(array + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE);
            memcpy(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, value, VALUE_SIZE);
            bitmap[i] = true;
            return i;
        }
    }
    return -2;
}

char *Bucket::lookup(char *key)
{
    int size = getSize();
    for (int i = 0; i < size; i++)
    {
        if (bitmap[i] == true)
        {
            if (strncmp(array + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE) == 0)
            {
                char *rtnvalue = (char *)calloc(KEY_SIZE+1, sizeof(char));
                memcpy(rtnvalue, array + i * (KEY_SIZE + VALUE_SIZE) + VALUE_SIZE, VALUE_SIZE);
                return rtnvalue;
            }
        }
    }

    return NULL;
}
void Bucket::update(char* key,char* value){
    int size = getSize();
    for (int i = 0; i < size; i++)
    {
        if (bitmap[i] == true)
        {
            if (strncmp(array + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE) == 0)
            {
                memcpy(array + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE);
                memcpy(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, value, VALUE_SIZE);
                bitmap[i] = true;
            }
        }
    }
}
bool Bucket::remove(char *key)
{
    char *erasekey = NULL;
    char *erasevalue = NULL;

    int size = getSize();
    for (int i = 0; i < size; i++)
    {
        if (bitmap[i] == true)
        {
            if (strncmp(array + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE) == 0)
            {
                memset(array + i * (KEY_SIZE + VALUE_SIZE), 0, KEY_SIZE);
                memset(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, 0, VALUE_SIZE);
                bitmap[i] = false;
                return true;
            }
        }
    }
    return false;
}
Bucket *Bucket::split(int index){
    local_depth++;
    int newHash=index%(int)pow(2,local_depth);
    Bucket *newBucket = new Bucket(getLocaldepth());
    char *key = (char *)calloc(KEY_SIZE, sizeof(char));
    char *value = (char *)calloc(VALUE_SIZE, sizeof(char));
    int size = getSize();
    
    // 기존 bucket 탐색
    for (int i = 0; i < size; i++){
        if (this->bitmap[i] == true){
            memcpy(key, array + i * (KEY_SIZE + VALUE_SIZE), KEY_SIZE);
            // new bucket 의 hashtable에서 index
            unsigned h = FIRSTH;
            for(int i=0;i<KEY_SIZE;i++){
                h = (h * A) ^ (key[i] * B);
            }
            int keysHash= h % (int)pow(2,local_depth);
            if (newHash!=keysHash)
            {
                memcpy(value, array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, VALUE_SIZE);
                newBucket->insert(key, value);
                //*remove 1
                this->remove(key);
                //*/
                /*remove 2
                memset(array + i * (KEY_SIZE + VALUE_SIZE), 0, KEY_SIZE);
                memset(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, 0, VALUE_SIZE);
                //*/
                //*remove3 -lazy deletion
                bitmap[i]=false;
                //*/
                
            }
        }
    }
    
    return newBucket;
}
int8_t Bucket::getLocaldepth(){
    return local_depth;
}
void Bucket::addLocaldepth()
{
    local_depth++;
}
void Bucket::checkBucket(){
//버킷 확인용
    int size = getSize();
    char *key = (char *)calloc(KEY_SIZE, sizeof(char));
    char *value = (char *)calloc(KEY_SIZE, sizeof(char));
    
    for (int i = 0; i < size; i++) {
        strncpy(key, array + i * (KEY_SIZE + VALUE_SIZE), KEY_SIZE);
        strncpy(value, array + i * (KEY_SIZE + VALUE_SIZE)+KEY_SIZE, VALUE_SIZE);
        
        printf("%d>%s\n",i, key);
        printf("%d>%s\n",i, value);
        
    }
    
}

int Bucket::getHashValue(){
    int hash=0;
    char *key = (char *)calloc(KEY_SIZE, sizeof(char));
    for(int i=0;i<getSize();i++){
        if(bitmap[i]==true){
            memcpy(key, array + i * (KEY_SIZE + VALUE_SIZE), KEY_SIZE);            
            unsigned h = FIRSTH;
            for(int i=0;i<KEY_SIZE;i++){
                h = (h * A) ^ (key[i] * B);
            }
            return h % (int)pow(2,local_depth);
        }
    }
}

int Bucket::writeBucket(int fd,int offset){
    int size=getSize();
    char* enter="\n";
    char* t="1";
    char* f="0";
    lseek(fd,offset,SEEK_SET);
    //int8_t local depth , fingerprint
    write(fd,this,sizeof(int8_t)*2);

    //bitmap
    int boolsize=sizeof(bitmap);
    char bitmapbuffer[boolsize];
    for(int i=0;i<boolsize;i++){
        if(this->bitmap[i]){
            write(fd,t,1);
        }
        else{
            write(fd,f,1);
        }
    }

    //full array
    int arrsize=sizeof(array);
    write(fd,array,arrsize);
}
void Bucket::readBucket(int fd,int offset){
    // printf("depth=%d",this->local_depth);
    char* t="1";
    char* f="0";
    //파일 처음부터
    lseek(fd,offset,SEEK_SET);
    char* buffer=(char*)malloc(sizeof(int8_t)*2);
    read(fd,buffer,sizeof(int8_t)*2);
    this->local_depth=buffer[0];
    this->fingerprint=buffer[1];
    printf("depth=%d\n",buffer[0]);
    printf("finger=%d\n",buffer[1]);
    
    //bool
    int boolsize=sizeof(bitmap);
    char* boolbit=(char*)malloc(boolsize);
    read(fd,boolbit,boolsize);
    for(int i=0;i<boolsize;i++){
        if(boolbit[i]=='1'){
            this->bitmap[i]=true;
        }else{
            this->bitmap[i]=false;
        }
    }
    // readBucket()
    read(fd,array,sizeof(array));    
    printf("endread\n");

    checkBucket();
    printf("endcheck\n");
}