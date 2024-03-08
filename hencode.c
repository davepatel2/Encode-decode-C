#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<fcntl.h> 
#include<errno.h> 
#include<unistd.h>
#include<stdint.h>
#include <netinet/in.h>
#define CODE 100
#define SIZE 256
#define INDEXSIZE 255
#define BIGNUM 1000
#define CHARSIZE 1
typedef struct nodes *Nodeptr;


typedef struct nodes{
 long numof;
 int letter;
 Nodeptr nextnode;
 Nodeptr left; 
 Nodeptr right;
 unsigned char* fcode;
} Node;

Nodeptr addNode(Nodeptr list, Nodeptr newnode);
void printcodes(Nodeptr node, int codes[], int index);
int comparenode(const void *ptrn1, const void *ptrn2);
int comparenode1(const void *ptrn1, const void *ptrn2);
Nodeptr Makelinkedlist(Nodeptr nptrarr[], int size);
void print_usage_and_exit();
int checkarg(int argc, char *argv[]);

void printLinkedList(Nodeptr ptr);

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
    fprintf(stderr, "usage: hencode infile [outfile] \n");
    exit(-1);
}


Nodeptr createSortedLinkedList(Nodeptr first);


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

void printcodes(Nodeptr node, int codes[], int index){
    if(node -> left != NULL){
        
        codes[index]=0;
        printcodes(node -> left, codes, index + 1);
    }
    if(node -> right != NULL){
        
        codes[index] = 1;
        printcodes(node -> right, codes, index + 1);
    }
   
    if((node -> right == NULL) && (node-> left == NULL)){
        unsigned char *newcode = malloc(sizeof(char*) * CODE);
        int codeind = 0;
        //printf("-");
        //printf("\n");
        while(codeind != index){
            //printf("%d", codes[codeind]);
            newcode[codeind] = codes[codeind] + 48;
            //printf("%c", (newcode[codeind]) + 48);
            codeind++;
        }
        newcode[codeind] = '\0';
        
        node -> fcode = newcode;
        
        
        
    }

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

int comparenode1(const void *ptrn1, const void *ptrn2){
    Nodeptr *ptr0 = (Nodeptr*)ptrn1;
    Nodeptr *ptr1 = (Nodeptr*)ptrn2;
    Nodeptr ptr2 = *ptr0;
    Nodeptr ptr3 = *ptr1;
    
    
    return ptr2 -> letter - ptr3 -> letter; 
    
}

int checkarg(int argc, char *argv[]){
    int cas = 0;
    if(argc == 1){
        print_usage_and_exit();
    }
    if(argc == 3){
        if(access(argv[1], F_OK) == -1){
            fprintf(stderr, "File Not Found\n");
            exit(-1);
        }
        else{
            cas = 2;
            return cas;
        }
    }
    if(argc ==2){
        if(access(argv[1], F_OK) == -1){
            fprintf(stderr, "file not found\n");
            exit(-1);
        }else{
            cas =3; 
            return cas;
        }
    }
    return cas;
}

int main(int argc, char *argv[]){
    
    int counterarr[SIZE] = {0};
    unsigned char *contents = 
    (unsigned char*)malloc(sizeof(unsigned char*) * SIZE);
    int in, cas, out, in2;
    cas = checkarg(argc, argv);
    unsigned char character; 
     
    if(cas == 2){
        in = open(argv[1], O_RDONLY);
        out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        
    }
    if(cas == 3){
         in = open(argv[1], O_RDONLY);
         out = STDOUT_FILENO;
         
         
    }
   
    unsigned char letter = read(in, &character, 1);
    in2 = 0;
    int totalspace = SIZE, resize = 2;
    if(letter == 0){
        
        fprintf(stderr, "file is empty");
        return 0;
    }
    
    while(letter > 0){
        
        counterarr[(unsigned int)character]++;
        
        *(contents + in2) = (unsigned char)character; 
        
        in2++; 
        if(in2 == totalspace){
            contents = (unsigned char *)realloc(contents,
             sizeof(unsigned char)* (SIZE*(resize)));
            totalspace = (totalspace + SIZE);
            resize++;
        }
        
        letter = read(in, &character, 1);
        
    }
    *(contents + in2) = '\0';
    
    close(in);

    
    

    
    
    int index;
    int newarrcounter = 0;
    for(index = 0 ; index <= INDEXSIZE ; index ++){
        if(counterarr[index]!= 0){
            newarrcounter++; 
        }   
    }
    
    int newindex =0;
    Nodeptr newnodearr[newarrcounter];
    
    for(index = 0 ; index <= INDEXSIZE ; index ++){
        if(counterarr[index]!= 0){ 
            Nodeptr npoint = malloc(sizeof(Nodeptr));
            npoint -> numof = counterarr[index];
            npoint -> letter = (unsigned char)index;
            npoint -> nextnode = NULL;
            npoint -> left = NULL;
            npoint -> right = NULL;
            newnodearr[newindex] = npoint;
            
            newindex++;
        }
    }
    qsort(newnodearr, newarrcounter, sizeof(Nodeptr), comparenode);
    Nodeptr Llist = Makelinkedlist(newnodearr, newarrcounter);
    qsort(newnodearr, newarrcounter, sizeof(Nodeptr), comparenode1);
    
    int nptr = 0;
    
    Nodeptr parentn = malloc(sizeof(Nodeptr)* BIGNUM);
    
    int lastspot = BIGNUM;
    
    
    while((Llist -> nextnode) != NULL){
        
        if (nptr == lastspot){
            lastspot = lastspot + SIZE;
            parentn = realloc(parentn, sizeof(Nodeptr) * lastspot);
            
        }
        
        int total = Llist -> numof + (Llist -> nextnode) -> numof;
        
        
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
    
    
    int fill = 0;
    int code[CODE] = {2};
    while(fill != 100){
        code[fill]= 2;
        fill++; 
    }

    int cindex = 0;
    
    printcodes(Llist, code, cindex);

    uint8_t diffwords = newarrcounter-1;
    
    write(out, &diffwords, 1);
    
    int i = 0;
    uint32_t freq;
    uint8_t let;
    
    
    
    Nodeptr *nodehash = malloc(sizeof(Nodeptr) * SIZE);
    while(i < SIZE && i < newarrcounter){
        let = newnodearr[i]->letter;
        freq = htonl(newnodearr[i]-> numof);
        
        *(nodehash + newnodearr[i] -> letter) = newnodearr[i];
        write(out, &let, 1);
        /* does the header*/
        write(out, &freq, 4 );
        
       
        i++; 
    }
     
    
    
    
    unsigned char *allcodes = malloc(sizeof(unsigned char) * BIGNUM);
    int index2 = in2;
    in2 = 0;
    int in3 = 0;
    i = 0; 
    lastspot = BIGNUM;
    
    while(i != index2){
        
        unsigned char* newcode = (*(nodehash + contents[i])) -> fcode;
       
        while ( *(newcode + in2) != '\0'){
            
            if(in3 == lastspot){
                lastspot = lastspot + 256;
                allcodes = realloc(allcodes, sizeof(unsigned char) * lastspot);
            }
            *(allcodes + in3) = *(newcode + in2); 
            
            
            in2++;
            in3++;
        }
        in2 = 0; 
        i++;
        
    }
    if (in3 % 8 != 0){
        int newval = 8 - (in3 % 8); 
        while (newval != 0){
            *(allcodes+ (in3)) = '0';
            in3++;
            newval--;
        }
    }
    *(allcodes + in3)='\0';
    

    index = 0;
    unsigned char mask= 1; 
    uint8_t currentbit = 0;
    int bytecount = 1;
    unsigned char *bytetable= malloc(sizeof(unsigned char)* lastspot);
    int bytecounttable= 0;
    while( *(allcodes + index)!= '\0'){
        
        if((bytecount % 8) == 0){
            if( *(allcodes + index) == '1'){
                currentbit = currentbit | mask; 
        } 
            
            write(out, &currentbit, 1);
            *(bytetable + bytecounttable) = currentbit; 
            currentbit = 0;
            bytecounttable++;
            
            
        }
        
        
        if( *(allcodes + index) == '1'){
            currentbit = currentbit | mask;
        } 
        currentbit = currentbit << 1;
        
        
        bytecount ++;
        index++;
        
    }
    *(bytetable + bytecounttable) = '\0';
    

return 0;
}

