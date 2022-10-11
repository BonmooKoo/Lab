#include <iostream>
#include <cstring>
#define TABLE_SIZE 1024*1024

using namespace std;

class hashtable{
    private:
        int16_t global_depth;
    public :
        void insert();
        void remove();
        void search(char* key, char* value);
};