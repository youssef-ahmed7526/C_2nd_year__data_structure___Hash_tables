/*
هيثم عصام عزت ددرويش
sec:4 BN:42 seat number:32208
يوسف أحمد محمد أحمد يس
sec:4 BN:50 seat number:32216
يوسف خالد السيد عبد الله
sec:4 BN:53 seat number:32219
*/

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// The size of the hashmap
#define SIZE 32
// FLag to the hash function
char hash=3;
char collision=0;
char probing=0;
int tombstones=INT_MAX;


int N=0;//number of students
double total_time=0;
int size;
int location,node_number=0; int delete_call=0;int found=0;




/**String maker function that read string from user*/
char * getString(){ 
    char temp[50];
    scanf("%*c");
    fgets(temp, sizeof(temp), stdin);
    char* str= (char*) malloc(strlen(temp));
    
    for (int i=0;i<strlen(temp);i+=1)
        str[i]=temp[i];
        
    str[strlen(temp)-1]='\0';
    
    return str;
}


typedef struct    //define student structure
{
    char *name;
    int id;
    int day;int month;int year;   //date of birth
    int student_score;
}student;

student arr[SIZE];


/**
 * @brief read the student data
 * 
 * This function reads student data as name, id,
 * date of birth(day, month, year) and his score
 * 
 * @return student object  
*/
student read_st()
{
    student st;
    
    printf("student name:");
    st.name=getString();

    printf("student id:");  scanf("%d",&st.id);
    
    printf("student date of birth:\n");
    printf("day:"); scanf("%d",&st.day);
    printf("month:"); scanf("%d",&st.month);
    printf("year:"); scanf("%d",&st.year);
    
    printf("student last year score:");  scanf("%d",&st.student_score);
    
    return st;
}



typedef struct node_tmp
{   student st;
	struct node_tmp*next;
}node;

node*head;

node*newNode(student st)
{   node*nodeP;
	nodeP=(node*)malloc(sizeof(node));
	nodeP->st = st;
	nodeP->next = NULL;
	return nodeP;
}


int name_to_key(char*name){
    int i=0;int sum=0;
    while(name[i])//get sum of ASCII code of the name characters
    {
      sum += name[i];
      i++;
    }
    int key = sum % size;
    return key;
}


void insert_open_hashing(node*head)
{
    student st=read_st();
    int start=clock();
    int key=name_to_key(st.name);
    node*new_node=newNode(st);
    node*node_pointer=(head+key);
    if (!(node_pointer->st.name))
    {*(node_pointer)=*(newNode(st));}
    else{ while((node_pointer->next)){
        node_pointer=node_pointer->next;
        }
        node_pointer->next=new_node;
        }
    total_time+=((double)(clock()-start)/CLOCKS_PER_SEC*10e3);
    N++;
}

void display_open_hashing(node*head){
    for(int i=0;i<size;i++){
    printf("location%d:",i);
    node*p=(head+i);
    if(!(p->st.name))printf(" empty");
    else{printf(" %s",p->st.name);

    //printf("%s",p->next);

    while ((p->next)){
        p=p->next;
        printf(" -> %s",p->st.name);
        }
    }printf("\n");
    }
}


void find_open_hashing(node*head,char*name){
    node*p;int i=0;
    int key=name_to_key(name);
        p=(head+key);
        if ((p->st.name)){
        if(!strcmp(p->st.name,name)){if(!delete_call)printf("found at location %d, node number 0\n",key);found=1;location=key;node_number=0;}
        while ((p->next)){
        p=p->next;i++;
        if(!strcmp(p->st.name,name)){if(!delete_call)printf("found at location %d, node number %d\n",key,i);found=1;location=key;node_number=i;}
        }}
    if(!found)
        printf("the name:%s is not found\n",name);

}

void delete_open_hashing(node*head,char*name){
    delete_call=1;
    find_open_hashing(head,name);
    if (found){
    node*temp=(head+location);node*pre=temp;
    for(int i=0;i<node_number;i++)
    {pre=temp;
    temp=temp->next;}
    if(!node_number) pre=temp->next;

    if(node_number) pre->next=temp->next;
    free(temp);
    temp->st.name=NULL;
    if(!node_number) {
    if (pre)*(head+location)=*pre;
    if (!pre) free((head+location)->st.name);
    }
    printf("student deleted successfully!\n");
    }
}


int calc_size_open_hashing(node*head){
    int memory_size=size * sizeof(node);
    node*temp;
    for(int i=0;i<size;i++){
        temp=head+i;
    while(temp->next){
        memory_size+=sizeof(node);
        temp=temp->next;
    }
    }
printf("size of the Hash table using open hashing is %d bytes\n",memory_size);
}


/**
 * ref https://leetcode.com/explore/learn/card/hash-table/182/practical-applications/1139/discuss/768659/Python-Easy-Multiplicative-Hash-explained
 * https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1214/lectures/18/Slides18.pdf
*/
int _eval_hash(int key){
    
   switch(hash){
    case 1: return  key%SIZE; break;
    case 2: return key&SIZE; break;
    case 3:{ 
        //some big odd number (sometimes good idea to use prime number) 
         int a = 1031237;
         /// m: length in bits of output we wan to have
         /// w: size of machine word.
         char m=5, w=10;
        return ((key*a) & (1<<w) - 1)>>(w-m);//((a*key)%(int)(pow(2,w)))/(pow(2,(w-m)));
    }
    break;
   }
}

/**Initialize all data to -1*/
void init()
{   
    int i;
    for(i = 0; i < SIZE; i++)
        arr[i].day = -1;
}

/**
 * @brief check if it is full or not
 * 
*/
int _is_full(student *array){
    
    for(int i=0;i<SIZE;i++)
        if((arr[i].day==-1) || (arr[i].day == tombstones) )
            return 0;
    return 1;
}

/**
 * @brief Search for element in hashmap
 * 
 * @param key birthday.
*/

int search(int key){
    
     int i=0;
    // Check to make sure the value is not existed.
    if((int)probing){
        
        // Linear Probing:
        while(arr[_eval_hash(key+i)].day!=-1 ||
             (arr[_eval_hash(key+i)].day == tombstones)){
             if(arr[_eval_hash(key+i)].day==key) return _eval_hash(key+i);
             i++;
             if(i>=SIZE) return -1;
            }
            
    }else{
        // quadratic probing
        
        while(arr[_eval_hash(key+i*i)].day!=-1 ||
            (arr[_eval_hash(key+i*i)].day == tombstones)){
                
            if(arr[_eval_hash(key+i*i)].day==key) return _eval_hash(key+i*i);
            
            i++;
            if(i>=SIZE) return -1;
        }
    }
    
    return -1;
}
/**
 * @brief Insert new element in hashmap
 * 
 * Make new node assign its value to the value and next to NULL
 * Get the hash value from the hash function
 * Then we add the value to the array if empty 
 * In case of collision we add the value to the 
 * end of the linked list
 * 
 * @param value the student that we want to add.
*/
void insert(student value)
{   
    int key = _eval_hash(value.day);
    
    if(_is_full(arr))
        return;
      
    if(search(value.day)!=-1){
         printf("The value is already exist\n");
         return;
    }
        
  
    
    int start = clock(); 
    int key2=value.day;
    if( (arr[key].day == -1) || (arr[key].day == tombstones) )
    {   
        arr[key] = value;
        printf("%s inserted at arr[%d]\n", value.name,key);
    }else // Collision
    {   
        
        if ((int)probing){
            // Linear Probing:
            int i=1;
            while( (arr[_eval_hash(key2+i)].day != -1) && 
                   (arr[_eval_hash(key2+i)].day != tombstones) ){
                
                printf("Collision : arr[%d] has element %s already!\n",
                       _eval_hash(key2+i),
                       arr[_eval_hash(key2+i)].name);
                i++;
            }
            
            arr[_eval_hash(key2+i)] = value;
            printf("inserted at arr[%d]\n", _eval_hash(key2+i));
            
        }else{// Quadratic Probing
             int i=1;
             while((arr[_eval_hash(key2+i*i)].day != -1) &&
                    (arr[_eval_hash(key2+i*i)].day != tombstones)){
                printf("Collision : arr[%d] has element %s already!\n",
                        _eval_hash(key2+i*i),arr[_eval_hash(key2+i*i)].name);
                i++;
             }
            
             arr[_eval_hash(key2+i*i)] = value;
             printf("inserted at arr[%d]\n",_eval_hash(key2+i*i));
        }
             
    }
    
    total_time+=(double)(clock()-start)/CLOCKS_PER_SEC;
    printf("Total time %f\n", total_time);
    N++;
}


/**
 * @brief delete the element by birthday
 * 
 * This function take the birthday then search for 
 * it and delete it by making the birthday is tombstones
 * which is the largest integer.
 * 
 * @param value value that we want to delete.
*/
void delete(int value)
{
    
    int key = search(value);
    
    if(key!=-1){
        arr[key].day = tombstones;
    }else  
        printf("The value %d doesn't exist\n", value);
}


void print()
{
    int i;
    for(i = 0; i < SIZE; i++)
        printf("arr[%d] = %d\n",i,arr[i].day);
}

void print_by_key(int key2)
{
    int key=search(key2);
    printf("KEY %d\n", key);
    if(key!=-1){
        printf("Name    |  Id  | Day  | Month |  Year  |  Score\n");
        printf("__________________________________________________\n");
                 
        printf("%7s |  %2d  |  %2d  |  %2d   |  %4d  |  %3d\n", 
                arr[key].name, arr[key].id, arr[key].day,
                arr[key].month, arr[key].year, 
                arr[key].student_score);
        printf("---------------------------------------------------\n");
    }else
        printf("The key %d is not found\n", key2);
}

int main()
{
    
    init();
    
    printf("Welcome sir!\n");
    printf("For open hashing press 1 for closed hashing press 2 \n");
    int option;
    scanf("%d",&option);

    while ((option!=1)&&(option!=2)){
        printf("Please enter 1 or 2 \n" );
        scanf("%d",&option);
    }
    
    if(option==1){
        
        printf("Please enter size of the hash table: ");
        scanf("%d",&size);
        head=(node*)(malloc(size*sizeof(node))); head->st.name=NULL;
        int n;
        printf("Please enter number of students: ");
        scanf("%d",&n);
        for(int i=0;i<n;i+=1)
        {
            printf("Enter data of student number %d \n",i+1);
            insert_open_hashing(head);

        }

        
    }else{
        
        printf("Please, enter the hash function\n1. Modulus (key %% SIZE)");
        printf("\n2. Ampersand (key&SIZE)\n3. Multiplicative hashing ");
        printf("[(a*key)%%(2^w)/(2^(w-m))]\n>>");
        scanf("%d", &hash);
        
        while(1){
                    
            printf("\npress 1 to insert a student \n");
            printf("press 2 to delete a student \n");
            printf("press 3 to find a student in the map \n");
            printf("press 4 to calculate average time taken for insertion \n");
            printf("press 5 to display the hash table \n");
            printf("press 6 to exit \n");
            
            int option2=1;
            scanf("%d", &option2);
            
            switch(option2){
                case 1:{
                  unsigned int after = sizeof(arr);
                  insert(read_st()); 
    
                  printf("Memory requried to store the data for probing %lu",
                            sizeof(arr)-after);
                  break;  
                } 
                // Delete a student by birthday
                case 2:{
                    printf("Please enter the student birthday");
                    int day;
                    scanf("%d", &day);
                    delete(day);
                break;
                }
                // Search for student by birthday
                case 3:{
                    printf("Please enter the student birthday: ");
                    int day;
                    scanf("%d", &day);
                    print_by_key(day);
                break;
                }
                case 4:{
                    
                    int n;
                    printf("Please enter number of students: ");
                    scanf("%d",&n);
                    for(int i=0;i<n;i+=1)
                    {
                        printf("Enter data of student number %d \n",i+1);
                        insert(read_st()); 
            
                    }
                    
                    printf("Average time to insert %d student is %lf milli second\n"
                            ,n, total_time/N*10e3);
                    
                break;
                } 
                case 5: print();break;
                case 6: exit(0);break;
            }
        }       
    }
    
    
    
    if (option==1)
    {
        int option2=1;
        while (option2!=7){
        printf("\npress 1 to insert more students \n");
        printf("press 2 to delete a student from the table \n");
        printf("press 3 to find a student in the table \n");
        printf("press 4 to calculate average time taken for insertion \n");
        printf("press 5 to calculate the memory size required for the table \n");
        printf("press 6 to display the hash table \n");
        printf("press 7 to exit \n");
            scanf("%d",&option2);
        while ((option2!=1)&&(option2!=2)&&(option2!=3)&&(option2!=4)&&(option2!=5)&&(option2!=6)&&(option2!=7)){
            printf("please enter 1,2,3,4,5,6 or 7 \n" );
            scanf("%d",&option2);}
            if(option2==1)insert_open_hashing(head);
            if(option2==2){
                printf("please enter the name of the student:");
                delete_open_hashing(head,getString());
            }
            if(option2==3){
                printf("please enter the name of the student:");
                find_open_hashing(head,getString());
            }
            if(option2==4)printf("average time taken for insertion=%f milli second\n",(double)(total_time/N));
            if(option2==5)calc_size_open_hashing(head);
            if(option2==6)display_open_hashing(head);
            if(option2==7) break;
    
    
    
        }
    }
    return 0;
}