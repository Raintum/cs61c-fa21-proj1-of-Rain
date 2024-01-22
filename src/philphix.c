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

//�ж϶�����ַ��Ƿ�Ϸ�
int ValidChar(char c) {
    return (c != ' ' && c != '\f' && c != '\n' && c != '\r' && c != '\t'
        && c != '\v');
}


//�ӵ�ǰ�ĵ�λ���������һ��word
//�����ǰλ��������EOF֮ǰ�����ںϷ��ַ������������зǷ��ַ�����ȡ��һ��word(�ڲ��޿ո�ĩβ�޿ո�)
//�����EOFǰ��û�кϷ�word���򷵻ؿ��ַ�
char* read_a_word(FILE** text) {
    int cap = 60; //Ŀǰ��һ��word����Ŀռ�
    int cnt = 0;// ��¼�ַ���ĩβλ�ã����ڽ����ַ���
    char* str = (char*)malloc(cap);
    // ע�⣡ ������Ҫ��������ַ��� �������ڶ���ı�����stack�У������ź������ر�����
    // ��Ȼ����ϣ����������������ַ���Ҳ������������heap������ռ䣬Ҳ�����ɴ˲Ż�������ռ��С������
    char c;
    while ((c = fgetc(*text)) != EOF) { //�������һ���ַ���c�У����ж��Ƿ�ΪEOF
        if (!ValidChar(c)) { //���ǺϷ��ַ� ��������� 1.��û����word 2.word������ֹ
            if (cnt == 0)continue; //��û����word ������
            else break; //word�Ѿ����� ����
        }
        else { //����һ���Ϸ��ַ�
            str[cnt] = c;
            cnt++;
            if (cnt == cap) {  //str�Ѿ����� resize 
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
    FILE* text = fopen(dictName, "r");//��ֻ����ʽ���ļ�,�����ָ��text
    if (!text) {
        fprintf(stderr, "File doesn't exit\n");
        exit(61);
    }
    char* key = NULL;
    while (1) {
       
         key = read_a_word(&text);//��readword�����ڣ�textָ�������ƶ���ָ��ͬ��λ��
        //���仰˵��text��ֵ��ı䡣����ΪC��ֵ�������ԣ��������غ�text��Ϊ�ⲿ������ֵ����ı�
        //�������ﴫ��ָ��text��ָ�룬Ҳ����text�ĵ�ַ����Ϊ����ָ�룬����ʵʱ�ı�text
        
        if (*key == 0)break;//�Ѿ�����Чword ��ȡ����
        else {
            char* data = read_a_word(&text);
            insertData(dictionary, key, data);
            //����key�� ����ػ���һ��Data ��������hashtable
          
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
        otherkey[i] = 0;//otherkeyΪ�ڶ�����̬��key
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
    char c = fgetc(stdin); //���ڽ��ܶ�����ַ�
    char* key;
    char* data;
    int cap = 60;
    int cnt = 0; //�ַ�����ָ��
    while (c != EOF) {
        if(is_Letter(c)){ //����word��ͷ
            key = (char*)malloc(cap);
            while (c != EOF && is_Letter(c)) {
               key[cnt] = c;
                cnt++;
                if (cap == cnt) {
                    cap *= 2;
                    key = realloc(key, cap);
                }
                c = fgetc(stdin);
            }//���굱ǰword,��ǰ��c����EOF���Ƿ�word�ַ�
            key[cnt] = 0;//������ֹ��
            cap = 60;
            cnt = 0;
            data = replace(key);  //�������д������ļ��ϵ��ַ���
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
