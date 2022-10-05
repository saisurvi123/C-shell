# Operating Systems and Networks
# Assignment 3
# commands to run terminal
    Note:In 2021101130 folder
    $ make
    $ ./a.out
# commands to run: 
    ls
    pwd
    cd
    echo
    pinfo
    history
    background and foreground process
    discover
    (NOTE)all other commands similar to bash(can run using execvp)
    
## ls:
    ls [OPTION]... [FILE]...
    List information about the FILEs (the current directory by default). 
    examples:
        ls
        ls -a
        ls -l
        ls .
        ls ..
        ls ~
        ls -a -l
        ls -la / ls -al
        ls <directory_name>
        ls <directory_path>
        ls <file_name>
        ls <ﬂags> <directory/file_name>
        included in (ls.c)
## pwd
        print name of current/working directory

## cd:
    change the working directory
    examples:
    cd <absolute_apth>
    cd <relative_path>
    cd ~
    cd ..
    cd -
    included in (run_command.c)

## pinfo
    pid : 231
    process status : {R/S/S+/Z}
    memory : 67854 {Virtual Memory}
    executable path : ~/a.out
    included in (pinfo.c)

## discover:
    discover <target_dir> <type_flags> <file_name>
    target directory specified using ‘.’, ‘..’, ‘~’ or directory_name or directory_path. [if this argument is not present, consider the
    current directory as the target directory]
    -d → searches for all directories
    -f → searches for all files
    If neither the file type nor the file name is provided as a command line argument, print all the contents of the target
    directory.

    included in (pinfo.c)

## jobs:
        gives current existing background processes list

## fg:
        changes background process to foreground for given
        job number
## bg:
        changes stopped state to running for given process
## sig:
        works accordingly for a given signal and pid 
        for bg process

        all above 4 in (jobs.c)
## auto completion:
        basic level of autocompletion upon entering tab

        included in (main.c)

## signal handling :
    for ctrl+c ,ctrl+Z, ctrl+D
    (inluded at approipriate places in code)








