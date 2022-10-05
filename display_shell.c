#include "headers.h"
int display_shell(char* home_dir){
    struct passwd *pas_w;
    // geteuid() returns the effective user ID of the calling process
    // The getpwuid() function returns a pointer to a structure containing the  broken-out 
    // fields of the record in the password structure db that matches the user ID uid.
    char current_path[PATH_MAX];
	pas_w = getpwuid(geteuid());
    char host_name[HOST_NAME_MAX];
    // getting hostname
    if(gethostname(host_name,sizeof(host_name))){
        printf("error in finding gost");
        return -1;
    }
    // finding present directory path
    if(getcwd(current_path,sizeof(current_path))==NULL){
        printf("error in finding current path");
        return -1;
    }
    // customizing shell path name
    char final_path[PATH_MAX];
    convert_to_tilda1(home_dir,current_path,final_path);
    // finally diaplsying
    if(pas_w){
		printf("\x1b[1;36m" "<%s@%s:" "\x1b[1;37m" "%s>" "\x1b[1;31m ",pas_w->pw_name,host_name,final_path);
    }
	else{
		printf("error in finding username");
		return -1;
	}	

}