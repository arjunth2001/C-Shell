#include "headers.h"

extern int errno;
extern char pwd[1024];

char * getPerms(struct stat s){
    char * temp= malloc(1024);
    strcpy(temp,"");
    mode_t mode =s.st_mode;
    S_ISDIR(mode) ? strcat(temp, "d") : strcat(temp,"-");
    if(mode&S_IRUSR){
        strcat(temp,"r");
    }
    else strcat(temp,"-");
    
    if(mode&S_IWUSR){
        strcat(temp,"w");
    }
    else strcat(temp,"-");
    if(mode&S_IXUSR){
        strcat(temp,"x");
    }
    else strcat(temp,"-");
    
    
    if(mode&S_IRGRP){
        strcat(temp,"r");
    }
    else strcat(temp,"-");
    
    if(mode&S_IWGRP){
        strcat(temp,"w");
    }
    else strcat(temp,"-");
    if(mode&S_IXGRP){
        strcat(temp,"x");
    }
    else strcat(temp,"-");
    
    
    if(mode&S_IROTH){
        strcat(temp,"r");
    }
    else strcat(temp,"-");
    
    if(mode&S_IWOTH){
        strcat(temp,"w");
    }
    else strcat(temp,"-");
    if(mode&S_IXOTH){
        strcat(temp,"x");
    }
    else strcat(temp,"-");

    strcat(temp,"\0");

    return temp;

}

void ls(char * argument, int showall, int list, int count){
    char * dirname= malloc(1024);
    if(argument==NULL){
        strcpy(dirname,"./");
        //printf("argument is null. So directory name is: %s\n",dirname);
    }
    else{

        dirname = getSystemPath(argument);

    }
    
    
    struct stat d;
    mode_t mode;
    if(dirname==NULL){
        dirname=malloc(10);
        strcpy(dirname,"./");
    }
    if(count!=0)
    printf("\n%s : \n",dirname);
    if(stat(dirname,&d)!=0){
        printf("Invalid File/Directory: %s\n",dirname);
        set_err(0);
        return;
    }
    
    mode = d.st_mode;
    if(S_ISDIR(mode)){
        strcat(dirname,"/");
        //printf("came here\n");
        DIR *dir;

        struct dirent *entry;
        if((dir=opendir(dirname))==NULL){
            perror("Directory Opening: ");
            set_err(0);
            return;
        }
        errno=0;
        if((entry=readdir(dir))==NULL){
            if(errno!=0)
            perror("Error in seeing entries of Directory: ");
            set_err(0);
        }
        struct stat innerFile;
        char * innerFilePath = malloc(1024);
        char * innerFileName;
        char * permissions;
        nlink_t number;
        struct passwd *userName;
        struct group *groupName; 
        char * time= malloc(1024);
        off_t size;
        struct tm *temp;

        while(entry){
           // printf("moo\n");
            innerFileName= entry->d_name;
            strcpy(innerFilePath,dirname);
            strcat(innerFilePath,innerFileName);
           // printf("%s\n",innerFilePath);
            if(stat(innerFilePath,&innerFile)==0){
                if(!showall&&innerFileName[0]=='.'){
                    entry = readdir(dir);
                    continue;

                }
                    

                //printf("meh\n");
                if(list)
                {
                    permissions= getPerms(innerFile);
                    number= innerFile.st_nlink;
                    userName= getpwuid(innerFile.st_uid);
                    groupName=getgrgid(innerFile.st_gid);
                    temp = localtime(&innerFile.st_mtime);
                    strftime(time, 1024, "%b %d %H:%M", temp);
                    size= innerFile.st_size;
                    printf("%s %2d %s %s %10lld %s %s\n", permissions , number, userName->pw_name, groupName->gr_name, size, time, innerFileName);
                }
                else{
                    printf("%s\n",innerFileName);
                }
                entry = readdir(dir);


            }
            else{
                perror("Opening Directory entry:");
                entry = readdir(dir);
                set_err(0);

            }


        }
        closedir(dir);

    }
    else{
        struct stat innerFile=d;
        char * innerFileName = dirname;
        char * permissions;
        nlink_t number;
        struct passwd *userName;
        struct group *groupName; 
        char * time= malloc(1024);
        off_t size;
        struct tm *temp;
        int shouldIshow=1;
        if(!showall&&innerFileName[0]=='.')
            shouldIshow=0;
            if(shouldIshow){
                if(list)
                {
                    permissions= getPerms(innerFile);
                    number= innerFile.st_nlink;
                    userName= getpwuid(innerFile.st_uid);
                    groupName=getgrgid(innerFile.st_gid);
                    temp = localtime(&innerFile.st_mtime);
                    strftime(time, 1024, "%b %d %H:%M", temp);
                    size= innerFile.st_size;
                    printf("%s %2d %s %s %10lld %s %s\n", permissions , number, userName->pw_name, groupName->gr_name, size, time, innerFileName);

                    
                }
                else{
                    printf("%s\n",innerFileName);
                }

            }
                    
    }
    free(dirname);
}

void lsChainStarter(char ** tokenizedCommand){

    int showall=0;
    int list=0;
    int flag =1;
    char *s;
    char temp[1024];

    for(int i=0; tokenizedCommand[i]!=NULL; i++){

        if(strcmp(tokenizedCommand[i],"-al")==0||strcmp(tokenizedCommand[i],"-la")==0||strcmp(tokenizedCommand[i],"-a")==0)
        {
            showall=1;

        }
        if(strcmp(tokenizedCommand[i],"-al")==0||strcmp(tokenizedCommand[i],"-la")==0||strcmp(tokenizedCommand[i],"-l")==0)
        {
            list=1;
            
        }
        if(tokenizedCommand[i][0]=='-'){
            strcpy(temp,tokenizedCommand[i]);
            if((s=strchr(temp,'a'))!=NULL){
                showall=1;
            }
            if((s=strchr(temp,'l'))!=NULL){
                list=1;
            }

            
        }

    }
    int count=0;
    for(int i=1; tokenizedCommand[i]!=NULL; i++){
        if((strcmp(tokenizedCommand[i],"-al")!=0)&&(strcmp(tokenizedCommand[i],"-l")!=0)&&(strcmp(tokenizedCommand[i],"-a")!=0)&&(strcmp(tokenizedCommand[i],"-la")!=0))
        {

            if(tokenizedCommand[i][0]!='-'){
                count++;

            }
            else
            {
                for(int j=0; j<strlen(tokenizedCommand[i]);j++){
                    if(tokenizedCommand[i][j]=='-'||tokenizedCommand[i][j]=='a'||tokenizedCommand[i][j]=='l'){
                        continue;

                    }
                    else{
                        printf("Invalid Option\n");
                        return;
                    }

                }
            }
            
            

        }

    }
    
    for(int i=1; tokenizedCommand[i]!=NULL; i++){
        if((strcmp(tokenizedCommand[i],"-al")!=0)&&(strcmp(tokenizedCommand[i],"-l")!=0)&&(strcmp(tokenizedCommand[i],"-a")!=0)&&(strcmp(tokenizedCommand[i],"-la")!=0)&&(tokenizedCommand[i][0]!='-'))
        {

            ls(tokenizedCommand[i],showall,list,count);
            flag=0;
            //printf("The flag is activated\n");
            

        }

    }
    if(flag){
       // printf("%d %d\n",showall,list);
        ls(NULL,showall,list,0);
    }

}