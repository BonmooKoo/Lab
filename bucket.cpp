#include <iostream>
#include <cstring>
#define BUCKET_SIZE 1024;
#define KEY_SIZE 16;
#define VALUE_SIZE 16;
using namespace std;

class Bucket
{
    private:
        char array[BUCKET_SIZE];
        
    public:
        void insert(char* key,char* value);
        void update(char* key,char* value);
        void remove(char* key);
        
};

void Bucket::insert(char* key,char* value){

}

int main(){
    
}