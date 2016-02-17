#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
#define INF 9999
#define NAME_SIZE 8
//vertex 정보를 담을 구조체
struct vertex{
	char v[9];
	int w;
	int visit;
	struct vertex *next;
	struct vertex *link;
};
struct vertex *Nodelist;
int size=-1;
int *visited;
int *distances;
char **hasharray;
int **patharray;
char startnode[9];
char endnode[9];

void hash(char node[9]);//문자를 숫자로 바꾼다
int getHashvalue(char *node);//노드의 정보를 hash value로 가져온다
char* getHashstring(int i);//hash value에 저장된 노드를 불러온다
struct vertex* newNode(char *node1, int weight);//새로운 노드 생성
void addNode(struct vertex *head, char *node1,int weight);//노드를 추가
void addVertex(char *node1, char *node2,int weight);//vertex를 이어주기
void initialize();
void dijkstra(char *start);//다익스트라
int main(int argc,char *argv[])
{
	FILE *fp;
	char node1[9]={"\n"};
	char node2[9]={"\n"};
	char cnode1[9];
	char cnode2[9];
	char tempnode[9];
	struct vertex * temp;
	int cweight=0;
	int weight=0;
	int sep=0;
	int i=0,j=0;
	clock_t start_time, end_time;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("file open error\n");
		return 0;
	}
	
	while(1)
	{
		if(feof(fp))
		{
			break;
		}
		fscanf(fp,"%s %s %d",node1,node2,&weight);//파일로부터 노드 정보를 읽는다
		if(strcmp(cnode1,node1)==0 && strcmp(cnode2,node2)==0 && cweight==weight)
			break;
		else
		{
			addVertex(node1,node2,weight);//정보에 따라 노드들 생성
		}
		strcpy(cnode1,node1);
		strcpy(cnode2,node2);
		cweight=weight;
	}



	for(i=0;i<size;i++)
	{
		struct vertex *temp;
		printf("%s    : ",Nodelist[i].v);
		for(temp=&Nodelist[i];temp!=NULL;temp=temp->link)
			printf("%s,  ",temp->v);
		printf("\n");
	}

	/*사용자로 부터 시작과 끝 노드를 입력받는다.*/
	printf("start node : ");
	scanf("%s",startnode);
	printf("end node : ");
	scanf("%s",endnode);
	start_time=clock();
	dijkstra(startnode);
	end_time=clock();
	printf("Time : %f\n",((double)(end_time-start_time))/CLOCKS_PER_SEC);
	printf("distance\n");
	for(i=0;i<size;i++)
	{
		printf("(%s (%d) : %d)\n",getHashstring(i),i,distances[i]);
	}
	
	/*경로 보여준다. path 배열에 있는 노드들을 탐색*/
	printf("path\n");
	for(i=0;i<size;i++)
	{
		printf("%s->",startnode);
		for(j=0;j<size;j++)
		{
			if(patharray[i][j]==-1)
				continue;
			printf("%s->",getHashstring(patharray[i][j]));
		}
		printf("\n");
	}


	printf("start node : %s\n",startnode);
	printf("end node : %s\n",endnode);

	printf("===================================\n");
	printf("%s->",startnode);
	strcpy(tempnode,startnode);
	for(i=0;i<size;i++)
	{
			if(patharray[getHashvalue(endnode)][i]!=-1)
			{


					for(j=0;j<size;j++)
					{

						   if(strcmp(Nodelist[j].v,tempnode)==0)
						   {
								   for(temp=&Nodelist[j];temp!=NULL;temp=temp->link)
								   {
										   if(strcmp(temp->v,getHashstring(patharray[getHashvalue(endnode)][i]))==0)
										   {
												   if(temp->w!=0)
												   {
												  		 printf("(%d)->",temp->w);
												  		 strcpy(tempnode,temp->v);
												  		 break;
												   }
										   }

								   }


						   }
					}

					printf("%s->",getHashstring(patharray[getHashvalue(endnode)][i]));
			}

	}
	printf("\n");

	printf("distances : %d",distances[getHashvalue(endnode)]);	
	printf("\n===================================\n");	


	fclose(fp);
	return 0;
 }

void initialize(char *start)
{
				/*사용할 변수 초기화*/
	int i=0;
	int j=0;
	visited=(int *)malloc(sizeof(int)*(size+1));
	for(i=0;i<=size;i++)
		visited[i]=0;
	distances=(int *)malloc(sizeof(int)*(size+1));
	for(i=0;i<=size;i++)
		distances[i]=INF;
	hasharray=(char **)malloc(sizeof(char*)*(size+1));
	for(i=0;i<=size;i++)
		hasharray[i]=(char *)malloc(sizeof(char)*8);
	for(i=0;i<=size;i++)
		strcpy(hasharray[i],"\0");
	for(i=0;i<size;i++)
	{
		hash(Nodelist[i].v);
	}
	
	patharray=(int **)malloc(sizeof(int*)*(size));
	for(i=0;i<size;i++)
	{
		patharray[i]=(int *)malloc(sizeof(int*)*size);
	}
	
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			patharray[i][j]=-1;
		}
	}

}
void hash(char node[9])
{

	int i=0;
	int hashvalue=0;
	for(i=0;i<(NAME_SIZE);i++)
	{
		if(node[i]=='\n'||node[i]=='\r'||node[i]=='\0') break;
		hashvalue+=(node[i]*(i+1));
	}
	hashvalue=(hashvalue%size);

	if(strcmp(hasharray[hashvalue],"\0")==0)
	{
		hasharray[hashvalue]=node;
	}else{
		for(i=0;i<size;i++)
		{
			if(strcmp(hasharray[i],"\0")==0)
			{
				hasharray[i]=node;
				break;
			}
		}
	}
}
int getHashvalue(char *node)
{
	int i=0;
	for(i=0;i<size;i++)
	{
		if(strcmp(hasharray[i],node)==0)
			return i;
	}
}
char* getHashstring(int i)
{
	return hasharray[i];
}
struct vertex* newNode(char *node1,int weight)
{
	struct vertex* Node=(struct vertex*)malloc(sizeof(struct vertex));
    strcpy(Node->v,node1);
    Node->w=weight;
	Node->next=NULL;
	Node->link=NULL;
	Node->visit=0;
	return Node;
 }

void addNode(struct vertex* head, char *node1, int weight)
{
	struct vertex *temp;
	temp=head;

	for(temp=head;temp->link!=NULL;temp=temp->link)
	{
		
	}
	
	temp->link=newNode(node1,weight);
}

 void addVertex(char *node1, char *node2,int weight)
 {
	 struct vertex *n;
	int i=0;
	int find_flag=0;
	if(size!=-1)
	{
		for(i=0;i<size;i++)
		{
			if(strcmp(Nodelist[i].v,node1)==0)
			{
				addNode(&Nodelist[i],node2,weight);
				find_flag=1;
			}
		}

		if(find_flag==0)
		{
			size++;
			Nodelist=(struct vertex*)realloc(Nodelist,(sizeof(struct vertex)*size));
			strcpy(Nodelist[size-1].v,node1);
			Nodelist[size-1].w=0;
			Nodelist[size-1].next=NULL;
			Nodelist[size-1].link=newNode(node2,weight);
		}
		find_flag=0;



		for(i=0;i<size;i++)
		{
			if(strcmp(Nodelist[i].v,node2)==0)
			{
				addNode(&Nodelist[i],node1,weight);
				find_flag=1;
			}
		}

		if(find_flag==0)
		{
			size++;
			Nodelist=(struct vertex*)realloc(Nodelist,(sizeof(struct vertex)*size));
			strcpy(Nodelist[size-1].v,node2);
			Nodelist[size-1].w=0;
			Nodelist[size-1].next=NULL;
			Nodelist[size-1].link=newNode(node1,weight);
		}
		find_flag=0;
	}else
	{
		Nodelist=(struct vertex *)malloc(sizeof(struct vertex)*2);

		strcpy(Nodelist[0].v,node1);
		Nodelist[0].w=0;
		Nodelist[0].link=newNode(node2,weight);
		Nodelist[0].visit=0;
		Nodelist[0].next=NULL;
		strcpy(Nodelist[1].v,node2);
		Nodelist[1].w=0;
		Nodelist[1].link=newNode(node1,weight);
		Nodelist[1].visit=0;
		Nodelist[1].next=NULL;
		size=2;
	}
 }

 void dijkstra(char *start)
{
	int min=0;
	int i=0,j=0,k=0,p=0,q=0,r=0;
	int temp=0;
	int Nodeindex=0;
	struct vertex *head;

	initialize(start);

	printf("number of Node\n");
	for(i=0;i<size;i++)
	{
		printf("%s : %d\n",hasharray[i],i);
	}




	visited[getHashvalue(start)]=0;
	distances[getHashvalue(start)]=0;
	for(i=0;i<size;i++)
	{
		min=INF;
		for(j=0;j<size;j++)
		{
			if(visited[j]==0 && distances[j]<min)
			{
				visited[j]=1;
				temp=j;
				break;
			}
		}
		visited[temp]=1;

		for(p=0;p<size;p++)
		{
			if(getHashvalue(Nodelist[p].v)==temp)
			{
				Nodeindex=p;
				break;
			}
		}

		for(head=Nodelist[Nodeindex].link;head!=NULL;head=head->link)
		{
			if((distances[temp]+head->w)<distances[getHashvalue(head->v)]&&(visited[getHashvalue(head->v)]==0))
			{
				//printf("%s->%s\n",getHashstring(temp),getHashstring(getHashvalue(head->v)));
				for(r=0;r<size;r++)
				{
					if(getHashvalue(getHashstring(temp))==-1)
						break;
					patharray[getHashvalue(head->v)][r]=patharray[getHashvalue(getHashstring(temp))][r];
				}
				for(q=0;q<size;q++)
				{
					if(patharray[getHashvalue(head->v)][q]==-1)
					{		
						patharray[getHashvalue(head->v)][q]=getHashvalue(head->v);
						break;
					}
					
				}
				distances[getHashvalue(head->v)]=distances[temp]+head->w;
			}

		}
	}
 }
