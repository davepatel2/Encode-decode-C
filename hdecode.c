#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<fcntl.h> 
#include<errno.h> 
#include<unistd.h>
#include<stdint.h>
#include <netinet/in.h>
#define BIGNUM 1000
#define SIZE 256

typedef struct nodes *Nodeptr;

typedef struct nodes{
 long numof;
 int letter;
 Nodeptr nextnode;
 Nodeptr left; 
 Nodeptr right;
 unsigned char* fcode;
} Node;

void print_usage_and_exit();
int comparenode(const void *ptrn1, const void *ptrn2);
int checkarg(int argc, char *argv[]);
Nodeptr Makelinkedlist(Nodeptr nptrarr[], int size);
void printLinkedList(Nodeptr ptr);
Nodeptr addNode(Nodeptr list, Nodeptr newnode);
unsigned char *makebytes(unsigned char totalcodearr[], 
int index, unsigned char byte );


unsigned char *makebytes(unsigned char totalcodearr[], 
int index, unsigned char byte ){
    unsigned char mask = 1;
    int ind;

    for(ind = 0; ind<8; ind++){
        totalcodearr[(index + 7) - ind] = (byte & (mask << ind)) != 0;
    }
    
    
    return totalcodearr;
     
}


Nodeptr addNode(Nodeptr list, Nodeptr newnode){
    if (list == NULL){
        list = newnode; 
        return list; 
    }
    if((newnode -> numof == list -> numof) 
    && (newnode -> letter == list -> letter)){
        newnode->nextnode = list; //puts the new parent node 
        //before the 0 index parent node if the same
        list = newnode;
        return list;
    }
    if(newnode -> numof < list -> numof){/* if first in list*/
        //printf("hello");
        newnode->nextnode = list;
        list = newnode;
        //printf("%ld\n %ld", list -> numof, (list ->nextnode) -> numof);
        return list;
    }
    if( newnode->numof == list->numof){
        if(newnode->letter < list -> letter){
            newnode->nextnode = list;
            list = newnode;
            return list;
        }else{
            newnode->nextnode = list -> nextnode;
            list -> nextnode = newnode;
            return list;
        }
    }
   Nodeptr currentnode = list;
    while ( currentnode != NULL){ 
        
        if(currentnode -> nextnode == NULL){
            newnode->nextnode = NULL;
            currentnode -> nextnode = newnode;
            return list;
            }
        if(((currentnode -> nextnode) -> numof == newnode -> numof) &&
         ((currentnode -> nextnode) -> letter == newnode-> letter)){
            newnode -> nextnode = currentnode -> nextnode;
            currentnode -> nextnode = newnode;
            return list;
        }
            
        
        if((currentnode -> nextnode) -> numof > newnode->numof){
            newnode->nextnode = currentnode -> nextnode;
            currentnode -> nextnode = newnode; 
            return list;
        }
        if((currentnode -> nextnode) -> numof == newnode->numof ){
            if((currentnode -> nextnode)-> letter > newnode->letter){
                newnode->nextnode = currentnode -> nextnode; 
                currentnode -> nextnode = newnode;
                return list;
            }
        }
        currentnode = currentnode -> nextnode;
    }
    return list;
}
void printLinkedList(Nodeptr ptr) {
    int numberNodes = 0;
    printf("Begin of list\n");
    while (ptr) {
        if (ptr->letter == '\n')
            printf("  c = \\n (%3d) count=%3ld\n", ptr->letter, ptr->numof);
        else
            printf("  c = %c  (%3d) count=%3ld\n", 
            ptr->letter, ptr->letter, ptr->numof);
        numberNodes += 1;
        ptr = ptr->nextnode;
    }
    printf("  Number of Nodes in list: %d\n", numberNodes);
    printf("End of list\n");
}
void print_usage_and_exit(){
    fprintf(stderr, "usage: hdecode [(infile | -) [outfile]] \n");
    exit(-1);
}
Nodeptr Makelinkedlist(Nodeptr nptrarr[], int size){
    int counter = 0;

    if (size == 0){
        return NULL;
    }
    if(size ==1){
        return nptrarr[0];
    }else{
        while (counter != (size-1)){
            nptrarr[counter] -> nextnode = nptrarr[counter + 1];
            counter++;
        }
    }
    return nptrarr[0];
    
}
int comparenode(const void *ptrn1, const void *ptrn2){
    Nodeptr *ptr0 = (Nodeptr*)ptrn1;
    Nodeptr *ptr1 = (Nodeptr*)ptrn2;
    Nodeptr ptr2 = *ptr0;
    Nodeptr ptr3 = *ptr1;

    if((ptr2 -> numof == ptr3 -> numof) && (ptr2 -> letter == ptr3 -> letter)){
        return 1;
    }
    
    if((ptr2 -> numof) != (ptr3 -> numof)){
        return ptr2 -> numof - ptr3 -> numof; 
    }else{
        return ptr2 -> letter - ptr3 -> letter; 
    }
}
int checkarg(int argc, char *argv[]){
    int cas = 0;
    if(argc == 1){
        print_usage_and_exit();
    }
    if(argc == 3){
        if((access(argv[1], F_OK) == -1) 
        && (strcmp(argv[1], "-")!= 0)){
            fprintf(stderr, "file not found 1\n");
            exit(-1);
        }
        else{
            cas = 2;
            return cas;
        }
    }
    if(argc ==2){
        if((access(argv[1], F_OK) == -1) && (strcmp(argv[1], "-")!= 0)){
            
            fprintf(stderr, "file not found 2\n");
            exit(-1);
        }else{
            cas =3; 
            return cas;
        }
    }
    return cas;
}

int main(int argc, char *argv[]){
    int in, cas, out;
    cas = checkarg(argc, argv);
    
    //printf("%d\n", cas);

    if( cas == 2){
         if(strcmp(argv[1], "-")== 0){
             in = STDIN_FILENO;
        
            out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
        }else{
            //("HELLO");
            in = open(argv[1], O_RDONLY);
            out = open(argv[2], O_WRONLY | O_CREAT 
            | O_TRUNC , S_IRUSR | S_IWUSR);
        }
    }
    if(cas == 3){
         if(strcmp(argv[1], "-")== 0){
             in = STDIN_FILENO;
            out = STDOUT_FILENO;
         }else{
            in = open(argv[1], O_RDONLY);
            out = STDOUT_FILENO;
        }
    }
    
    unsigned char character; 
    unsigned int freqh;

    unsigned int input = read(in, &character, 1);
    if(input <= 0){
        fprintf(stderr, "empty file\n");
        exit(-1);
    }
    int numofdiffc = ((int)character) + 1; /* numof */
    //printf("%d", numofdiffc);

    unsigned char letter;
    unsigned int freq;
    Nodeptr newnodearr[numofdiffc];
    int naindex = 0;
    int copy = numofdiffc;
    int totalchars;
    while(copy != 0){
        input = read(in, &character, 1);/*read character*/
        letter = character;
        input = read(in, &freqh, 4);
        freq = ntohl(freqh); 
        Nodeptr npoint = malloc(sizeof(Nodeptr));
         
        npoint -> numof = freq;
        npoint -> letter = letter;
        npoint -> nextnode = NULL;
        npoint -> left = NULL;
        npoint -> right = NULL;
        newnodearr[naindex] = npoint;
        naindex++;
        copy--;
    }
    qsort(newnodearr, numofdiffc, sizeof(Nodeptr), comparenode);
    Nodeptr Llist = Makelinkedlist(newnodearr, numofdiffc);
    
    //kedList(Llist);

    int nptr = 0;
    Nodeptr parentn = malloc(sizeof(Nodeptr)* BIGNUM);
        
    int lastspot = BIGNUM;
    while((Llist -> nextnode) != NULL){
        if (nptr == lastspot){
            lastspot = lastspot + SIZE;
            parentn = realloc(parentn, sizeof(Nodeptr) * lastspot);
            
        }
        
        int total = Llist -> numof + (Llist -> nextnode) -> numof;
        //printlinkedlist(Llist);
        
        (parentn + nptr) -> numof = total;
        (parentn + nptr) -> letter = (char)-1;
        (parentn + nptr) -> nextnode = NULL;
        (parentn + nptr) -> left = Llist;
        (parentn + nptr) -> right = Llist -> nextnode; 
        
        Llist = Llist -> nextnode;
        Llist = Llist -> nextnode;
        Llist = addNode(Llist, (parentn + nptr));
        nptr++;
        

    }
    totalchars = (Llist -> numof);
    
    //printLinkedList(Llist);
    //printf("%d", totalchars); 
    unsigned char bodybyte;
    unsigned char *codearr = 
    malloc( sizeof(unsigned char)* ((totalchars * 8) + 2 ));
    
    
    lastspot = BIGNUM;
    int cindex =0;
    
    

    while((input = read(in, &bodybyte ,1)) > 0){
        //printf("\n%x\n", bodybyte);
        codearr = makebytes(codearr, cindex, bodybyte);
        cindex = cindex + 8;
        //printf("%d", codearr[cindex]);
    
        
        /*function that returns a string of codes*/
    }
    // int i = 0; 
    // while( i != cindex){
    //     //printf("%d", codearr[i]);
    //     i++; 
    // }

    int finalcounter = 0;
    int ind = 0;
    Nodeptr currnode;
    int letwrite;
    while (finalcounter != totalchars){

        currnode = Llist;

        while(currnode -> left != NULL && currnode -> right != NULL){
            if(codearr[ind] == 0){
                //printf("\nleft"); 
                currnode = currnode -> left; 
                ind++;
            
                continue;
            }
            if ( codearr[ind] == 1){
                //printf("\nright");
                currnode = currnode -> right; 

                ind++;
                continue;
            }
        }

        letwrite = currnode -> letter;
        //printf("%c", letwrite);
        write(out, &letwrite, 1);
        finalcounter++;

          
    }
    








    close(in);
    
    return 0;
}