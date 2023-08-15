#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE 128//read file 

typedef struct Team//create team struct
{
	char name[63];
	int metric;
	struct Team * next;//pointer
}team;

team *create_list(team * head,char *argv[]);
void list_sort(team * head);//bubble sort
void print_list(team * head);//output
void free_list(team * head);

int main(int argc, char *argv[])
{
	team * head = NULL;
	head = create_list(head,argv);// read file & create list
	list_sort(head);
	print_list(head);
	free_list(head);
	return 0;
} 

team *create_list(team * head,char *argv[])
{
	team *prev, *current;
	char name[63];
	int point_scored, point_given_up,rebounding_differential;//the three number
	
	char buf[MAX_LINE]; //file process
	FILE *fp;           
	int len;             
	if((fp = fopen(argv[1],"r")) == NULL)
	{
		perror("fail to read");
		exit (1) ;
	}
	fscanf(fp,"%s",name);
	while(strcmp(name ,"DONE") != 0)
	{
		fscanf(fp,"%d",&point_scored);
		fscanf(fp,"%d",&point_given_up);
		fscanf(fp,"%d",&rebounding_differential);
		
		current = (team *)malloc(sizeof(team));// space allocation
		if(head == NULL)
			head = current;
		else
			prev -> next = current;
		current -> next = NULL;
		
		strcpy(current -> name , name);
		current -> metric = point_scored - point_given_up + rebounding_differential;
		prev = current;
		
		fscanf(fp,"%s",name);
	}

	fclose(fp); 
	return head;
}

void list_sort(team * head)//bubble sort
{
	int temp;
	char temp1[62];
	team *current, *prev, *p, *tail;
	tail = NULL;
	while (head -> next != tail)
	{
		prev = head;
		p = head -> next;
		if(head -> metric < p -> metric || (head -> metric == p -> metric && strcmp(head -> name,p -> name) > 0))//compare the fisrt two node value
		{
			temp = head -> metric;
			head -> metric = p -> metric;
			p -> metric = temp;
			strcpy(temp1,head -> name);
			strcpy(head -> name,p -> name);
			strcpy(p -> name,temp1);
		}
		while (p -> next != tail)//later node compare
		{
			if (p -> metric < p -> next -> metric || (p -> metric == p -> next -> metric && strcmp(p -> name,p -> next -> name) > 0))
			{
				prev -> next = p -> next;
				p -> next = prev -> next -> next;
				prev -> next -> next = p;
				
			}
			else
				p = p -> next;
			prev = prev -> next;
		}
		tail = p;
	}
		
}
void print_list(team * head)
{
	team * current = head;
	while(current != NULL)
	{
		printf("%s ",current -> name);
		printf("%d\n",current -> metric);
		current = current -> next;
	} 
}

void free_list(team * head)
{
	team *current = head;
	
	while(current != NULL)
	{
		head = current->next;
		free(current);
		current = head;
	} 
}
