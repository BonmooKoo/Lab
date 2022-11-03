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
    Bucket *newBucket = new Bucket(local_depth);
    char *key = (char *)calloc(KEY_SIZE + 1, sizeof(char));
    char *value = (char *)calloc(VALUE_SIZE + 1, sizeof(char));
    // 기존 bucket 탐색
    int size = getSize();
    for (int i = 0; i < size; i++){
        if (this->bitmap[i] == true){
            memcpy(key, array + i * (KEY_SIZE + VALUE_SIZE), KEY_SIZE);
            // new bucket으로 가야하는 값
            int NewIndex = 0;
            for (int j = 0;j< KEY_SIZE; j++){
                NewIndex += key[j];
            }
            NewIndex = NewIndex % (int)(pow(2, global_depth));
            // 기존버킷의 새로운hash값과 다르다면 새로운 버킷으로 옮긴다.
            //ex 기존 3 / 4 였던애 들중에는
            //   3도 있겠지만 7도 있으니까
            //   4> 8 이되면 3/ 7 로 나뉘니깐
            
            if (originalIndex<NewIndex)
            {
                memcpy(value, array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, VALUE_SIZE);
                cout<<local_depth<<"&"<<global_depth<<">"<<key<<":"<<"OriginalHash"<<originalIndex<<"newHash"<<NewIndex<<endl;
                newBucket->insert(key, value);
                //this->remove(key);
                //  memset(array + i * (KEY_SIZE + VALUE_SIZE), 0, KEY_SIZE);
                //  memset(array + i * (KEY_SIZE + VALUE_SIZE) + KEY_SIZE, 0, VALUE_SIZE);

                // bitmap[i] = false;
            }
        }
    }
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