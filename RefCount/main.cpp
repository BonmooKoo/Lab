#include "header.h"
#include <fstream>
#include <string>
#pragma warning(disable:4996)

int main() {
    Bucket* buffer[BUFFER_SIZE];
    hashtable ht(1,buffer,10);
    char* key = (char*)calloc(KEY_SIZE+1, sizeof(char));
    int num = 100000;
    ifstream is("input.txt");
    if (is.is_open()) {
        for (int i = 0; i < num; i++) {
            string input;
            getline(is, input);
            key = (char*)input.c_str();
            // cout<<"INPUT"<<i<<endl;
            ht.insertKV(key,key);  
        }
    }
    is.close();
    //업데이트
    // char* updateval=(char*)malloc(sizeof(char)*VALUE_SIZE);
    // for(int i=0;i<VALUE_SIZE;i++){
    //     updateval[i]='a';
    // }
    // printf("key:%s\n",ht.searchKV(key));
    // cout<<"update"<<endl;
    // ht.update(key,updateval);
    // printf("Afterupdate:%s\n",ht.searchKV(key));
    // 삭제
    // ht.removeKV(key);
    // printf("%s\n",ht.searchKV(key));

    //탐색
    cout << "find" << endl;
    ifstream i2s("input.txt");
    int count=0;
    if (i2s.is_open()) {
        for (int i = 0; i < num; i++) {
            string input;
            getline(i2s, input);
            key = (char*)input.c_str();
            char* Searchval=(char*)calloc(VALUE_SIZE,sizeof(char));
            // printf("%d\n",i);
            Searchval=ht.searchKV(key);
            if(Searchval==NULL){
                printf("null\n");
                count++;
            }
            else {
                printf("key=%s\nval=%s\n",key,Searchval);  
            }
        }
        printf("null=%d",count);
    }
    i2s.close();
    // for(int i=0;i<ht.getSizeTable();i++){
    //     cout<<"Bucket"<<i<<"============================================================="<<endl;
    //     ht.rtnBucket(i)->checkBucket();
    // }
    //ht.rtnBucket(5)->checkBucket();
}