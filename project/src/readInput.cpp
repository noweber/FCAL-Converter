/* readInput.cpp provides 
    char *readInput (int argc, char **argv) ;
   to return a pointer to a character buffer containing the 
   contents of a file. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>  


char *readInputFromFile (const char *filename) {
    FILE *in_fp ;
    in_fp = fopen(filename,"r") ;
    if ( in_fp==NULL ) {
//        printf ("File \"%s\" not found.\n", argv[1]);
        return NULL ;
    }

    // Determine the size of the file, used to allocate the char buffer.
    struct stat filestatus;
    stat( filename, &filestatus );

    int filesize = filestatus.st_size + 1; // +1 for terminating null char

    // Allocate space for the character buffer.
    char *buffer = (char *) malloc( sizeof(char) * filesize ) ;

    int index = 0 ;
    char ch = getc(in_fp) ;

    while (ch != EOF) {
        buffer[index] = ch ;
        index ++ ; 
        ch = getc(in_fp);
    }
    buffer[index] = '\0' ;

    return buffer ;
}



char *readInput (int argc, char **argv) { 

    // Verify that a file name is provided and that the file exists.
    // Use some new C++ stream features.
    if (argc <= 1) {
//        printf ("Usage: %s <filename>\n", argv[0]) ;
        return NULL ;
    }
    else
        return readInputFromFile (argv[1]) ;
}

