#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//Check if there is a flag we are looking for 
int checkFlag(char* string, char t){
    int i=1;
    while(string[i]!= '\0'){
        if(string[i] == t){
            return 1;
        }
        i++;
    }
    return 0;
}


char* checkSymbolic(char *path,struct stat file){
 int size = 1024;
 char* buf[1024];
 int result;
 char* re = NULL;
 
 if(file.st_mode & S_IFLNK){  //symbolic link
   //buf = (char*)malloc(size);
   //buf = (char*)realloc(buf, size);
   memset(buf,0x00, sizeof(buf));
   result = readlink(path, buf, size);
   if(result != -1){
        //printf("buf%s",*buf);
        if(*buf != NULL)
        re = (char*)buf;
        return re;
    }
 }
  return NULL;
}


////////////////get type of the file and user authorization///////////
void filePermission(struct stat file) {
  if (S_ISLNK(file.st_mode)) {     //link
    printf("l");
  } 
  else if (S_ISDIR(file.st_mode)) { //directory
    printf("d");
  } 
  else if (S_ISREG(file.st_mode)) { //file
    printf("-");
  }

  //user authorization
  printf((file.st_mode & S_IRUSR) ? "r" : "-");
  printf((file.st_mode & S_IWUSR) ? "w" : "-");
  printf((file.st_mode & S_IXUSR) ? "x" : "-");
  //group authorization
  printf((file.st_mode & S_IRGRP) ? "r" : "-");
  printf((file.st_mode & S_IWGRP) ? "w" : "-");
  printf((file.st_mode & S_IXGRP) ? "x" : "-");
  //other user authorization
  printf((file.st_mode & S_IROTH) ? "r" : "-");
  printf((file.st_mode & S_IWOTH) ? "w" : "-");
  printf((file.st_mode & S_IXOTH) ? "x " : "- ");
}

//Get when the file was modified as last 
void fileTime(struct stat file) {
  struct tm* time = localtime(&file.st_mtime);
  char month[4];
  int day = time->tm_mday;
  int year = time->tm_year + 1900;
  int hour = time->tm_hour;
  int minute = time->tm_min;
  
  switch (time->tm_mon) {
    case 0:
      strcpy(month, "Jan");
      break;
    case 1:
      strcpy(month, "Feb");
      break;
    case 2:
      strcpy(month, "Mar");
      break;
    case 3:
      strcpy(month, "Apr");
      break;
    case 4:
      strcpy(month, "May");
      break;
    case 5:
      strcpy(month, "Jun");
      break;
    case 6:
      strcpy(month, "Jul");
      break;
    case 7:
      strcpy(month, "Aug");
      break;
    case 8:
      strcpy(month, "Sep");
      break;
    case 9:
      strcpy(month, "Oct");
      break;
    case 10:
      strcpy(month, "Nov");
      break;
    case 11:
      strcpy(month, "Dec");
      break;
  }
  printf("%s %2d %d %02d:%02d ", month, day, year, hour, minute);
}

//Print File or Directory 
void printList(char* string, int l, int R, int i, int recursive){
    DIR * dir;
    struct dirent *dirent;
    struct stat file;
    int count=0;
    struct passwd *passwd;
    struct group  *group;
    char dirPath[4000]={0,};

    //If opendir() is NULL
    if((dir = opendir(string)) ==NULL){
        if(recursive == 0){
            //Assume it is a file since there is no dir
            if(access(string, F_OK) < 0){
                fprintf(stderr, "%s: No such file or directory\n",string);
            }else{
                //Get info from the file
                if(lstat(string, &file)==0){
                    fflush(stdin);
                    if(l==0){
                        printf("%s\n",string);
                    }else if(l==1){
                        passwd = getpwuid(file.st_uid);
                        group  = getgrgid(file.st_gid);
                        filePermission(file);//print file permission
                        printf("%hu\t", file.st_nlink);//print file hard link => print number of hard link 
                        printf("%s\t", passwd -> pw_name);//print owner of file
                        printf("%s\t", group -> gr_name); //print name of the group
                        printf("%llu\t", file.st_size); //print the file size
                        fileTime(file);
                        printf("%s\t", dirent -> d_name); //print file/directory name
                        char* originalPath = checkSymbolic(dirent->d_name,file);
                        if(originalPath != NULL){
                            printf("->%s", originalPath);
                        }
                        printf("\n");  
                    }
                }
            }
        }
        closedir(dir);
    }else{ //opendir() is not NULL
        printf("%s:\n",string);
        while((dirent = readdir(dir)) != NULL){
            //Get inode from the file
            //printf("print dirent d name %s\n",dirent->d_name);
            if(strcasecmp(string,"/") == 0)
                sprintf(dirPath, "%s%s",string,dirent->d_name);
            else
                sprintf(dirPath, "%s/%s",string,dirent->d_name);
            lstat(dirPath, &file );
            //Don't get inode if it is "."
            if(dirent->d_name[0] == '.'){
                continue;
            }
            //output options depending on l
            if(l == 0){
              if(i==0){
                //printf("HERE2\n");  
                printf(" %s\n",dirent->d_name);//
              }
              else{
                //printf("HERE3\n");  
                //printf("%d\t",(int)file.st_ino);//number of hard link
                printf("%-6llu ", file.st_ino);
                printf(" %s\n",dirent->d_name);//
              }
            }else if (l > 0){
              if(i==0){
                //printf("HERE4\n");  
                passwd = getpwuid(file.st_uid);
                group  = getgrgid(file.st_gid);
                filePermission(file);//print file permission
                printf("%hu\t", file.st_nlink);//print file hard link
                printf("%s\t", passwd -> pw_name);//print owner of file
                printf("%s\t", group -> gr_name); //print name of the group
                printf("%llu\t",file.st_size); //print the file size
                fileTime(file);
                printf("%s\t", dirent -> d_name); //print file/directory name
                char* originalPath = checkSymbolic(dirent->d_name,file);
                if(originalPath != NULL){
                   printf("->%s", originalPath);
                  }
                printf("\n"); 
              }
              else {
                //printf("HERE5\n");  
                //printf("%d\t",(int)file.st_ino);//number of hard link
                printf("%-6llu ", file.st_ino);//number of hard link
                passwd = getpwuid(file.st_uid);
                group  = getgrgid(file.st_gid);
                filePermission(file);//print file permission
                printf("%hu\t",file.st_nlink);//print file hard link
                printf("%s\t", passwd -> pw_name);//print owner of file
                printf("%s\t", group -> gr_name); //print name of the group
                printf("%llu\t",file.st_size); //print the file size
                fileTime(file);
                printf("%s\t", dirent -> d_name); //print file/directory name
                char* originalPath = checkSymbolic(dirent->d_name,file);
                if(originalPath != NULL){
                   printf("->%s", originalPath);
                  }
              
                printf("\n");  
               }
             }
          count++;
        }
        //Recursive if there is R
        if(R == 1){
            rewinddir(dir);
            while((dirent = readdir(dir)) != NULL){
                if(strcasecmp(string,"/") == 0){
                  //printf("HERE6\n");  
                  sprintf(dirPath, "%s%s",string,dirent->d_name);
                }
                else{
                  //printf("HERE7\n");  
                  sprintf(dirPath, "%s/%s",string,dirent->d_name);
                }
                lstat(dirPath, &file);
                //If it is a direcotry
                if(S_ISDIR(file.st_mode)){
                    //Ignore . && ..
                    //printf("HERE8\n");  
                    if(strcmp(".",dirent->d_name)==0 || strcmp("..",dirent->d_name)==0) continue;
                    if(dirent->d_name[0] == '.') continue;
                    //call R
                    printList(dirPath,l,R,i, 1);
                }
            }
        }
        printf("\n");
        closedir(dir);
    }
}
 
int main(int argc, char * argv[]){
    //Option Flags parameter
    int lFlag =0, RFlag=0, iFlag =0;
    //Check if File name is attached right
    int dFlag = 0;
    //Initial directory is .
    char *InitDir = ".";
    //Count option flags
    int flagCount=1;
 
    //Check Parameter: No extra parameter
    if(argc == 1){
        printList(InitDir,lFlag,RFlag,iFlag,0);
    //Check Parameter: There is extra parameter
    }else if(argc >= 2){
        //Loop to read option flags
        for(int i=1; i<argc;i++){
            if(argv[i][0]=='-'){
              //printf("argv[i][0] %c\n",argv[i][0]);
              if(argc > 2){
              if(lFlag != 1){lFlag = checkFlag(argv[i], 'l');}
              if(RFlag != 1){RFlag = checkFlag(argv[i], 'R');}
              if(iFlag != 1){iFlag = checkFlag(argv[i],'i');}
              }else if(argc==2){
               lFlag = checkFlag(argv[i], 'l');
               RFlag = checkFlag(argv[i], 'R');
               iFlag = checkFlag(argv[i],'i');
              }
             flagCount++;
             
            }
        }
 
        //check if option flags are attached together (ex.-Ri,-li)
        if((argc-flagCount) > 0) dFlag = 1;
 
        //Loop again to read files
        if(dFlag == 0){
                printList(InitDir,lFlag,RFlag,iFlag, 0);
        }else{
            for(int i=0; i<argc-1;i++){
                if(argv[i+1][0] != '-'){
                    fflush(stdin);
                    printList(argv[i+1], lFlag,RFlag,iFlag, 0);
                }
            }
        }
    }
 
    return 0;
}
