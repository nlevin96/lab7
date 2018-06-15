#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define TRUE 1
#define FALSE 0
int size = 1;
char* filename = NULL;

//returns FALSE if filename is NULL
int check_file(){
    if(filename == NULL) {
        perror("ERROR! filename is NULL");
        return FALSE;
    }
    return TRUE;
}

//returns FALSE if fopen fails
int check_file_ptr(FILE* fp){
    if(fp == NULL){
        perror("ERROR! couldn't open file");
        return FALSE;
    }
    return TRUE;
}

void set_file_name(){
    if(filename != NULL) free(filename);
    filename = malloc(100);
    printf("%s\n","Please enter a filename:");
    scanf("%s",filename);
}

void set_unit_size(){
    char str_size[100];
    int tmp;
    printf("%s\n","Please enter a unit size:");
    scanf("%s",str_size);
    tmp = atoi(str_size);
    if(tmp == 1 || tmp == 2 || tmp ==4) size = tmp;
    else perror("ERROR! invalid unit size");
}

void file_display(){
    int i;
    char in_str [100];
    int location;
    int length;
    FILE* file;
    if(!check_file()) return;
    file = fopen(filename, "r");
    if(!check_file_ptr(file)) return;
    
    printf("%s\n","Please enter <location> <length>");
    while(fgetc(stdin) != '\n');
    fgets(in_str,100,stdin);
    sscanf(in_str, "%x %d", &location, &length);
    unsigned char* res = (unsigned char*)malloc(length * size);
    fseek(file, location, SEEK_SET);
    fread(res, size, sizeof(res), file);
    fclose(file);
    
    printf("%s\n","Hexadecimal Representation:");
    for (i = 0; i < length*size; i++){
        if (size == 1) printf("%02hhx ", *((char*)&res[i]));
        if (size == 2){
          printf("%04hx ", *((short*)&res[i]));
          i = i + 1;
        }
        if (size == 4){
          printf("%08x ", *((int*)&res[i]));
          i = i + 3;
        }
    }
    printf("%s\n","");
    
    printf("%s\n","Decimal Representation:");
    for (i = 0; i < length*size; i++){
        if (size == 1) printf("%02hhu ", *((char*)&res[i]));
        if (size == 2){
          printf("%04hu ", *((short*)&res[i]));
          i = i + 1;
        }
        if (size == 4){
          printf("%08u ", *((int*)&res[i]));
          i = i + 3;
        }
    }
    printf("%s\n","");
    free(res);
  
}

void file_modify(){
    int location;
    int val;
    char in_str [100];
    FILE* file;
    if(!check_file()) return;
    file = fopen(filename, "r+");
    if(!check_file_ptr(file)) return;
    printf("%s\n","Please enter <location> <val>");
    while(fgetc(stdin) != '\n');
    fgets(in_str,100,stdin);
    sscanf(in_str, "%x %x", &location, &val);
    fseek(file, location, SEEK_SET);
    fwrite(&val,size ,sizeof(val) ,file);
    fclose(file);
}

void copy_from_file(){
    char src_file[100];
    unsigned long int src_offset;
    unsigned long int dst_offset;
    int length;
    FILE* src_fp;
    FILE* dst_fp;
    char in_str[100];
    if(!check_file()) return;
    printf("%s\n","Please enter <src_file> <src_offset> <dst_offset> <length>");
    while(fgetc(stdin) != '\n');
    fgets(in_str,100,stdin);
    sscanf(in_str, "%s %lx %lx %d", src_file, &src_offset, &dst_offset, &length);
    src_fp = fopen(src_file, "r");
    dst_fp = fopen(filename, "r+");
    if(!check_file_ptr(src_fp) || !check_file_ptr(dst_fp)) return;
    fseek(src_fp, src_offset, SEEK_SET);
    char src_res [size*length];
    fread(src_res, size, sizeof(src_res), src_fp);
    fseek(dst_fp, dst_offset, SEEK_SET);
    fwrite(src_res,size ,sizeof(src_res) ,dst_fp);
    
    printf("Copied %d bytes into FROM %s at %lx TO %s at %lx\n", length, src_file, src_offset, filename, dst_offset); 

    fclose(src_fp);
    fclose(dst_fp);
}

void quit(){
    if(filename != NULL) free(filename);
    _exit(0);
}

int main(int argc, char **argv) {
    int num_of_functions = 6;
    char func_strnum[100];
    int func_num;
    
    //set an array of functions
    void(*func_array[num_of_functions])(void);
    
    func_array[0] = set_file_name;
    func_array[1] = set_unit_size;
    func_array[2] = file_display;
    func_array[3] = file_modify;
    func_array[4] = copy_from_file;
    func_array[5] = quit;
    
    while(TRUE){
        
        printf("%s\n","Choose action:");
        printf("%s\n","1-Set File Name");
        printf("%s\n","2-Set Unit Size");
        printf("%s\n","3-File Display");
        printf("%s\n","4-File Modify");
        printf("%s\n","5-Copy from file");
        printf("%s\n","6-Quit");
        
        scanf("%s", func_strnum);
        func_num = atoi(func_strnum) - 1;
        //if invalid input
        if(func_num < 0 || func_num>=num_of_functions) _exit(-1);
        
        (*func_array[func_num])();
        
    }
    
    return 0;
}
