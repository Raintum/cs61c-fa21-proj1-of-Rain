/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

/* Task 3 */

//判断读入的字符是否合法
int ValidChar(char c) {
    return (c != ' ' && c != '\f' && c != '\n' && c != '\r' && c != '\t'
        && c != '\v');
}


//从当前文档位置往后读入一个word
//如果当前位置往后（在EOF之前）存在合法字符，则跳过所有非法字符，读取第一个word(内部无空格，末尾无空格)
//如果在EOF前均没有合法word，则返回空字符
char* read_a_word(FILE** text) {
    int cap = 60; //目前给一个word申请的空间
    int cnt = 0;// 记录字符串末尾位置，用于建立字符串
    char* str = (char*)malloc(cap);
    // 注意！ 我们需要返回这个字符串 而函数内定义的变量在stack中，会随着函数返回被销毁
    // 显然我们希望函数结束后这个字符串也保留，所以在heap上申请空间，也正因由此才会有申请空间大小的问题
    char c;
    while ((c = fgetc(*text)) != EOF) { //往后读入一个字符到c中，并判断是否为EOF
        if (!ValidChar(c)) { //不是合法字符 有两种情况 1.还没读到word 2.word读入终止
            if (cnt == 0)continue; //还没读到word 继续读
            else break; //word已经读完 返回
        }
        else { //读到一个合法字符
            str[cnt] = c;
            cnt++;
            if (cnt == cap) {  //str已经读满 resize 
                cap *= 2;
                str = realloc(str, cap);
            }
        }
    }
    str[cnt] = 0;
    return str;
}


void readDictionary(char *dictName) {
  // -- TODO --
 // fprintf(stderr, "You need to implement readDictionary\n");
    FILE* text = fopen(dictName, "r");//以只读形式打开文件,获得流指针text
    if (!text) {
        fprintf(stderr, "File doesn't exit\n");
        exit(61);
    }
    char* key = NULL;
    while (1) {
       
         key = read_a_word(&text);//在readword函数内，text指针会向后移动，指向不同的位置
        //换句话说，text的值会改变。但因为C的值传递特性，函数返回后text作为外部变量，值不会改变
        //所以这里传入指向text的指针，也就是text的地址，作为二级指针，用于实时改变text
        
        if (*key == 0)break;//已经无有效word 读取结束
        else {
            char* data = read_a_word(&text);
            insertData(dictionary, key, data);
            //读到key了 后面必还有一个Data 读入后插入hashtable
          
        }
       
    }
}

/* Task 4 */
char* mystrcpy(char* key, char* data) {
    free(key);
    key = malloc(strlen(data) + 1);
    strcpy(key, data);
    return key;
}
int is_Letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

char* replace(char* key) {
    char* data = findData(dictionary, key);
     if (data)return data;

    
        char* otherkey = malloc(strlen(key) + 1);
        int i;
        otherkey[0] = key[0];
        for (i = 1; key[i]; i++) {
            otherkey[i] = tolower(key[i]);
        }
        otherkey[i] = 0;//otherkey为第二种形态的key
        data = findData(dictionary, otherkey);
        if (data) { free(otherkey); return data; }
            otherkey[0] = tolower(otherkey[0]);
            data = findData(dictionary, otherkey);
        
        
        free(otherkey);
    
    
 
    return data;
}

void processInput() {
  // -- TODO --
  //  fprintf(stderr, "You need to implement processInput\n");
    char c = fgetc(stdin); //用于接受读入的字符
    char* key;
    char* data;
    int cap = 60;
    int cnt = 0; //字符串内指针
    while (c != EOF) {
        if(is_Letter(c)){ //读到word的头
            key = (char*)malloc(cap);
            while (c != EOF && is_Letter(c)) {
               key[cnt] = c;
                cnt++;
                if (cap == cnt) {
                    cap *= 2;
                    key = realloc(key, cap);
                }
                c = fgetc(stdin);
            }//读完当前word,当前的c不是EOF就是非word字符
            key[cnt] = 0;//补充终止符
            cap = 60;
            cnt = 0;
            data = replace(key);  //获得最终写在输出文件上的字符串
            char* wrt = (data ? data : key);
            fputs(wrt, stdout);
            free(key);
        }
        else {
            fputc(c, stdout);
            c = fgetc(stdin);
        }
    }


}
