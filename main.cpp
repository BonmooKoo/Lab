#include "header.h"
#include <fstream>
#include <string>
#define BUFFER_SIZE 10
#pragma warning(disable:4996)

int main() {
    char buffer[BUFFER_SIZE*(KEY_SIZE+VALUE_SIZE)];
    hashtable ht(10);
    char* key = (char*)calloc(KEY_SIZE+1, sizeof(char));
    int num = 1000;
    ifstream is("input.txt");
    if (is.is_open()) {
        for (int i = 0; i < num; i++) {
            string input;
            getline(is, input);
            key = (char*)input.c_str();
            cout<<i<<">>";
            ht.insertKV(key,key);
            // if(i>100){
            // cout<<i<<endl;
            // 
            // }
        }
    }
    // ht.rtnBucket(0)->checkBucket();
    is.close();
    char* updateval=(char*)malloc(sizeof(char)*VALUE_SIZE);
    for(int i=0;i<VALUE_SIZE;i++){
        updateval[i]='a';
    }
    // 삭제
    // printf("key:%s\n",ht.searchKV(key));
    // cout<<"update"<<endl;
    // ht.update(key,updateval);
    // printf("Afterupdate:%s\n",ht.searchKV(key));
    // ht.removeKV(key);
    // printf("%s\n",ht.searchKV(key));

    cout << "find" << endl;
    ifstream i2s("input.txt");
    if (i2s.is_open()) {
        for (int i = 0; i < num; i++) {
            string input;
            getline(i2s, input);
            key = (char*)input.c_str();
            // printf("val:%s\n",ht.searchKV(key));  
        }
    }
    i2s.close();
    // for(int i=0;i<8;i++){
    //     cout<<i<<endl;
    //     ht.rtnBucket(i)->checkBucket();
    // }
}