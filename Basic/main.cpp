#include "header.h"
#include <fstream>
#include <string>
#include <time.h>
#pragma warning(disable:4996)

int main() {
    clock_t start,end;
    hashtable ht(1);
    char* key = (char*)calloc(KEY_SIZE+1, sizeof(char));
    int num = 1000000;
    ifstream is("input.txt");
    start=clock();
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
    end=clock();
    double result=double(end-start);
    cout<<"input time="<<result/CLOCKS_PER_SEC<<"sec"<<endl;
    //업데이트
    char* updateval=(char*)malloc(sizeof(char)*VALUE_SIZE);
    for(int i=0;i<VALUE_SIZE;i++){
        updateval[i]='a';
    }
    printf("key:%s\n",ht.searchKV(key));
    cout<<"update"<<endl;
    ht.update(key,updateval);
    printf("Afterupdate:%s\n",ht.searchKV(key));
    // 삭제
    // ht.removeKV(key);
    // printf("%s\n",ht.searchKV(key));

    //탐색
    cout << "find" << endl;
    ifstream i2s("input.txt");
    int count=0;
    start=clock();
    if (i2s.is_open()) {
        for (int i = 0; i < num; i++) {
            string input;
            getline(i2s, input);
            key = (char*)input.c_str();
            char* Searchval=(char*)calloc(VALUE_SIZE,sizeof(char));
            // printf("%d\n",i);
            Searchval=ht.searchKV(key);
            if(Searchval==NULL){
                printf("%d:null\n",i);
                count++;
            }
            else {
                //printf("%d:key=%s\nval=%s\n",i,key,Searchval);  
            }
        }
        printf("null=%d\n",count);
    }
    i2s.close();
    end=clock();
    result=double(end-start)/CLOCKS_PER_SEC;
    cout<<"search time="<<result<<"sec"<<endl;
    // for(int i=0;i<ht.getSizeTable();i++){
    //     cout<<"Bucket"<<i<<"============================================================="<<endl;
    //     ht.rtnBucket(i)->checkBucket();
    // }
    //ht.rtnBucket(5)->checkBucket();
}