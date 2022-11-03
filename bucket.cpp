#include "header.h"
#pragma warning(disable : 4996)

Bucket::Bucket()
{
    initialize();
    local_depth = 1;
}
Bucket::Bucket(int8_t depth)
{
    initialize();
    local_depth = depth;
}
void Bucket::initialize()
{   
    reference_counter=0;
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
    // 중복확인을 해야하네?
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
            // startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
            memcpy(array + i * (KEY_SIZE + VALUE_SIZE), key, KEY_SIZE);
            memcpy(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, value, VALUE_SIZE);
            bitmap[i] = true;
            return i;
        }
    }
    // printf("Bucket: full\n");
    this->reference_counter++;
    return -2;
}

char *Bucket::lookup(char *key)
{
    int size = getSize();
    //char *startpoint = (char *)calloc(KEY_SIZE, sizeof(char));
    
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
            // startpoint=array[i*(KEY_SIZE+VALUE_SIZE)]
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
Bucket *Bucket::split(int originalIndex, int global_depth){
    local_depth=local_depth+1;
    Bucket *newBucket = new Bucket(getLocaldepth());
    char *key = (char *)calloc(KEY_SIZE + 1, sizeof(char));
    char *value = (char *)calloc(VALUE_SIZE + 1, sizeof(char));
    int pow=1;
    for(int i=0;i<global_depth;i++){
        pow*=2;
    }
    // 기존 bucket 탐색
    int size = getSize();
    for (int i = 0; i < size; i++){
        if (this->bitmap[i] == true){
            memcpy(key, array + i * (KEY_SIZE + VALUE_SIZE), KEY_SIZE);
            // new bucket 의 hashtable에서 index
            int NewIndex = 0;
            for (int j = 0;j< KEY_SIZE; j++){
                NewIndex += key[j];
            }
            NewIndex = NewIndex % pow;
            
            if (originalIndex<NewIndex)
            {
                memcpy(value, array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, VALUE_SIZE);
                printf("%d&%d>%s:OriginalHash:%d///NewHash:%d\n",getLocaldepth(),global_depth,key,originalIndex,NewIndex);
                newBucket->insert(key, value);
                //*remove 1
                this->remove(key);
                //*/
                /*remove 2
                memset(array + i * (KEY_SIZE + VALUE_SIZE), 0, KEY_SIZE);
                memset(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, 0, VALUE_SIZE);
                
                //*/
                }
        }
        
    }
    this->checkBucket();
    newBucket->checkBucket();
    return newBucket;
}
int8_t Bucket::getLocaldepth()
{
    return local_depth;
}
void Bucket::addLocaldepth()
{
    local_depth++;
}
void Bucket::checkBucket(){
    int size = getSize();
    char *startpoint = (char *)calloc(KEY_SIZE, sizeof(char));
    //버킷 확인용
    for (int i = 0; i < size; i++) {
        strncpy(startpoint, array + i * (KEY_SIZE + VALUE_SIZE), KEY_SIZE + 1);
        printf("%d>%s\n",i, startpoint);
    }
    
}
int8_t Bucket::getReferenceCounter(){
    return this->reference_counter;
}
void Bucket::refCount(){
    this->reference_counter++;
}