#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<io.h>

#define MAXINODE 50
#define MAXFILESIZE 1024

#define READ 1
#define WRITE 2

#define REGULAR 1

#define START 0
#define CURRENT 1
#define END 2



typedef struct inode
{
	char Fname[50];
	int Ino;
	int FileSize;
	int ActualFileSize;
	int FileType;
	int ReferenceCount;
	struct inode *next;
	int Permission;
	char *Buffer; 
	int LinkCount;
}INODE,*PINODE,**PPINODE;

typedef struct FileT
{
	int ReadOffset;
	int WriteOffset;
	int mode;
	int count;
	PINODE ptrinode;
}FT,*PFT;

typedef struct ufdt
{
	PFT ptrfiletable;
}UFDT,*PUFDT;
typedef struct SuperB
{
	int TInode;
	int FInode;
}SB,*PSB;

PINODE head=NULL;
UFDT UArr[50];
SuperB superblockobj;


void CreateDILB ()
{
	PINODE temp=NULL,newn=NULL;

	for (int i=1;i<=MAXINODE;i++)
	{
		newn=(PINODE)malloc(sizeof(INODE));
		if (temp==NULL)
		{
			head=newn;
			temp=head;
		}
		else
		{
			temp->next=newn;
			temp=newn;
		}
		newn->ReferenceCount=newn->ActualFileSize=newn->FileType=newn->LinkCount=0;
		newn->Buffer=NULL;
		newn->Ino=i;
		
	}
	
}

void InitialiseSuperB()
{
	for (int i=0;i<50;i++)
	{
		UArr[i].ptrfiletable=NULL;
	}
	superblockobj.TInode=50;
	superblockobj.FInode=50;
}
void help()
{
	printf ("\n1. open - Open the file which are store in the hard disk.");

printf ("\n2. close - Close the file which are already open.");

printf ("\n3. read - Read the data from file.");

printf ("\n4. write - Write the data from file.");

printf ("\n5. lseek - Read the data where starting point of reading data is depends on user.");

printf ("\n6. stat - Display the information about file like file name, size of file ,etc.");

printf ("\n7. ls - It Display files which are stored in current directory.");

printf ("\n8. rm - Used to remove files.");

printf ("\n9. fstat - Displays file attributes using file name.");

printf ("\n10. man - It displays the manual of particular command. Like how to use command and it's use.");

printf ("\n11. truncate - Used to delete the from file.");

printf ("\n12. closeall - Used to close all the files.");

printf ("\n13. exit - Used to terminate the terminal.");

}
void fstat (int fd)
{
	printf ("File name- %s\n",UArr[fd].ptrfiletable->ptrinode->Fname);
	printf ("Inode number- %d\n",UArr[fd].ptrfiletable->ptrinode->Ino);
	printf ("File size- %d\n",UArr[fd].ptrfiletable->ptrinode->FileSize);
	printf ("File type- %d\n",UArr[fd].ptrfiletable->ptrinode->FileType);
	printf ("Actual File size- %d\n",UArr[fd].ptrfiletable->ptrinode->ActualFileSize);
	printf ("Permission- %d\n",UArr[fd].ptrfiletable->ptrinode->Permission);
	printf ("Link count- %d\n",UArr[fd].ptrfiletable->ptrinode->LinkCount);
	printf ("Reference count- %d\n",UArr[fd].ptrfiletable->ptrinode->ReferenceCount);
}
PINODE Get_Inode (char *name)
{
	/*PINODE temp=head;
	int i=0;
	while (temp!=NULL)
	{
		if (strcmp(name,temp->Fname)==0)
		{
			break;
		}
		temp=temp->next;
	}
	return temp;
	*/
	int i=0;
	for (i=0;(UArr[i].ptrfiletable!=NULL)&&(i<50);i++)
	{
		if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name)==0)
		{
			break;
		}
		
	}
	if (UArr[i].ptrfiletable!=NULL)
	{
		return (UArr[i].ptrfiletable->ptrinode);
	}
	else
	{
		return NULL;
	}
}
int CreateFile(char *name,int permission)
{
	PINODE temp=head;
	int i=0;
	if ((name==NULL)||(permission>3)||(permission<=0))
	{
		return -2;
	}
	if (superblockobj.FInode==0)
	{
		return -3;
	}
	if (Get_Inode(name)!=NULL)
	{
		return -1;
	}
	while(temp->FileType!=0)
	{
		temp=temp->next;
	}
	for (i=0;i<50;i++)
	{
		if (UArr[i].ptrfiletable==NULL)
		{
			break;
		}
	}
	PFT fileobj=(PFT)malloc(sizeof(FT));
	UArr[i].ptrfiletable=fileobj;
	UArr[i].ptrfiletable->ReadOffset=0;
	UArr[i].ptrfiletable->WriteOffset=0;
	UArr[i].ptrfiletable->count=1;
	UArr[i].ptrfiletable->mode=permission;
	UArr[i].ptrfiletable->ptrinode=temp;
	UArr[i].ptrfiletable->ptrinode->Buffer=(char *)malloc(sizeof(MAXFILESIZE));
	UArr[i].ptrfiletable->ptrinode->Permission=permission;
	UArr[i].ptrfiletable->ptrinode->ActualFileSize=0;
	UArr[i].ptrfiletable->ptrinode->FileSize=MAXFILESIZE;
	UArr[i].ptrfiletable->ptrinode->FileType=REGULAR;
	strcpy(UArr[i].ptrfiletable->ptrinode->Fname,name);
	
	(superblockobj.FInode)--;
	return i;
}
int remove (char *name)
{
	int i=0,j=0;
	for (i=0;((UArr[i].ptrfiletable!=NULL)&&(i<50));i++)
	{
		if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name)==0)
		{
			free(UArr[i].ptrfiletable->ptrinode->Buffer);
			UArr[i].ptrfiletable->ptrinode->FileType=0;
			free(UArr[i].ptrfiletable);
			UArr[i].ptrfiletable=NULL;
			j++;
			break;
		}
	}
	if (j==0)
	{
		return -1;
	}
	(superblockobj.FInode)++;
	return 0;
}

int findfd (char *name)
{
	if (name==NULL)
	{
		return -1;
	}
	int i=0;
	while (UArr[i].ptrfiletable!=NULL)
	{
		if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name)==0)
		{
			break;
		}
		i++;
	}
	if (i<50)
	{
		return i;
	}
	else
	{
		return -2;
	}
}

void File_ls()
{
	for (int i=0;i<50;i++)
	{
		if (UArr[i].ptrfiletable!=NULL)
		{
			printf ("%s\n",UArr[i].ptrfiletable->ptrinode->Fname);
		}
	}
}
int stat(char *name)
{
	int fd=findfd(name);
	/*
	int i=0;
	for (i=0;(UArr[i].ptrfiletable!=NULL)&&(i<50);i++)
	{
		if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name)==0)
		{
			break;
		}
	}*/
	printf ("File name- %s\n",UArr[fd].ptrfiletable->ptrinode->Fname);
	printf ("Inode number- %d\n",UArr[fd].ptrfiletable->ptrinode->Ino);
	printf ("File size- %d\n",UArr[fd].ptrfiletable->ptrinode->FileSize);
	printf ("File type- %d\n",UArr[fd].ptrfiletable->ptrinode->FileType);
	printf ("Actual File size- %d\n",UArr[fd].ptrfiletable->ptrinode->ActualFileSize);
	printf ("Permission- %d\n",UArr[fd].ptrfiletable->ptrinode->Permission);
	printf ("Link count- %d\n",UArr[fd].ptrfiletable->ptrinode->LinkCount);
	printf ("Reference count- %d\n",UArr[fd].ptrfiletable->ptrinode->ReferenceCount);
	return 0;
}

int write (int fd,char *brr,int size)
{
	
	if (UArr[fd].ptrfiletable->ptrinode->ActualFileSize==MAXFILESIZE)
	{
		return -1;
	}
	if ((UArr[fd].ptrfiletable->mode==READ)||(UArr[fd].ptrfiletable->ptrinode->Permission==READ))
	{
		return -2;
	}
				
	strncpy((UArr[fd].ptrfiletable->ptrinode->Buffer)+(UArr[fd].ptrfiletable->WriteOffset),brr,size);
				UArr[fd].ptrfiletable->WriteOffset=(UArr[fd].ptrfiletable->WriteOffset)+size;
				UArr[fd].ptrfiletable->ptrinode->ActualFileSize=UArr[fd].ptrfiletable->ptrinode->ActualFileSize+size;
			
	
				
	return 0;
}

int read (int fd,char *brr,int size)
{
	int size_t=0;
	
	if  (UArr[fd].ptrfiletable->mode==WRITE)
	{
			return -1;
	}
	if (UArr[fd].ptrfiletable->ReadOffset==UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
	{
		return -2;
	}
	
	size_t=(UArr[fd].ptrfiletable->ptrinode->ActualFileSize)-(UArr[fd].ptrfiletable->ReadOffset);
			if (size_t<size)
			{
				strncpy(brr,(UArr[fd].ptrfiletable->ptrinode->Buffer)+(UArr[fd].ptrfiletable->ReadOffset),size_t);
				UArr[fd].ptrfiletable->ReadOffset=UArr[fd].ptrfiletable->ReadOffset+size_t;
				size=size_t;
			}
			else
			{
				strncpy(brr,(UArr[fd].ptrfiletable->ptrinode->Buffer)+(UArr[fd].ptrfiletable->ReadOffset),size);
				UArr[fd].ptrfiletable->ReadOffset=UArr[fd].ptrfiletable->ReadOffset+size;
			}

	return size;
}


void man(char *name)
{
		if(name == NULL) 
			return; 
 if(strcmp(name,"create") == 0) 
 { 
 printf("Description : Used to create new regular file\n"); 
 printf("Usage : create File_name Permission\n"); 
 } 
 else if(strcmp(name,"read") == 0) 
 { 
	printf("Description : Read the content from the file\n"); 
	printf("Usage : read File_name size\n");
 } 
 else if(strcmp(name,"write") == 0) 
 { 
printf("Description : Write the data in file\n"); 
	printf("Usage : write File_name \n"); 
 } 
 else if(strcmp(name,"ls") == 0) 
 { 
 printf("Description : Display all file names\n"); 
	printf("Usage : ls\n"); 
 } 
 else if(strcmp(name,"stat") == 0) 
 { 
 printf("Description : Display all information about file (using descriptor)\n"); 
	printf("Usage : stat File_Descriptor\n"); 
 } 
 else if(strcmp(name,"fstat") == 0) 
 { 
 printf("Description : Display all information about file (using file name)\n"); 
	printf("Usage : fstat File_name \n");
 } 
 else if(strcmp(name,"truncate") == 0) 
 { 
 printf("Description : Delete the data from file\n"); 
	printf("Usage : truncate File_name \n");
 } 
 else if(strcmp(name,"open") == 0) 
 { 
 printf("Description : Open the file in specific mode\n"); 
	printf("Usage : open File_name Mode\n");
 } 
 else if(strcmp(name,"close") == 0) 
 { 
 printf("Description :	close the file\n"); 
	printf("Usage : close File_name \n");
 } 
 else if(strcmp(name,"closeall") == 0) 
 { 
 printf("Description : close all files\n"); 
	printf("Usage : closeall\n"); 
 } 
 else if(strcmp(name,"lseek") == 0) 
 { 
 printf("Description : Used to create new regular file\n"); 
	printf("Usage : create File_name Permission\n");
 } 
 else if(strcmp(name,"rm") == 0) 
 { 
 printf("Description : Remove regular file\n"); 
	printf("Usage : rm File_name \n");
 } 
 else 
 { 
 printf("ERROR : No manual entry available.\n");
 }
}

int Deleteall()
{
	 PINODE temp=head;
	 int i=0;
				  
				  for (i=0;(UArr[i].ptrfiletable!=NULL);i++)
				  {
					  free(UArr[i].ptrfiletable->ptrinode->Buffer);
					  
				  }

				  for (i=1;(i<=MAXINODE)&&(temp!=NULL);i++)
				  {
					  
					  head=temp->next;
					  free(temp);
					  temp=head;
				  }
				  return 0;

}
int truncate(char *name)
{

	if (name==NULL)
	{
		return -1;
	}
	int fd=findfd(name);

	//memset(UArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
	for (int i=0;i<8;i++)
	{
		UArr[fd].ptrfiletable->ptrinode->Buffer[i]=0;
	}
	UArr[fd].ptrfiletable->ReadOffset=0;
	UArr[fd].ptrfiletable->WriteOffset=0;
	UArr[fd].ptrfiletable->ptrinode->ActualFileSize=0;

	return 0;
}

int open (char *name,int mode)
{
	int i=0;
	if ((name==NULL)||(mode<=0)||(mode>3))
	{
		return -1;
	}
	PINODE temp=Get_Inode(name);
	while (UArr[i].ptrfiletable!=NULL)
	{
		i++;
	}
	UArr[i].ptrfiletable=(PFT)malloc(sizeof(FT));
	UArr[i].ptrfiletable->count=1;
	UArr[i].ptrfiletable->mode=mode;
	UArr[i].ptrfiletable->ReadOffset=0;
	UArr[i].ptrfiletable->WriteOffset=0;
	UArr[i].ptrfiletable->ptrinode=temp;
	(UArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
	return i;
}

int lseek(char *name,int size,int from)
{
	int fd=findfd(name);
	int size_t;
	if (fd==-1)
	{
		printf ("Invalid Parameter");
	}
	if (fd==-2)
	{
		printf ("File not found");
	}
	if ((UArr[fd].ptrfiletable->mode==READ)||(UArr[fd].ptrfiletable->mode==(READ+WRITE)))
	{
	if	(from==START)
	{
		if (size>UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->ReadOffset=size;
		
	}
	else if (from==CURRENT)
	{
		if ((UArr[fd].ptrfiletable->ReadOffset+size)>(UArr[fd].ptrfiletable->ptrinode->ActualFileSize))
		{
			return -1;
		}
		if ((UArr[fd].ptrfiletable->ReadOffset+size)<0)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->ReadOffset=UArr[fd].ptrfiletable->ReadOffset+size;
	}
	else if (from==END)
	{
		if (size>UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->ReadOffset=UArr[fd].ptrfiletable->ptrinode->ActualFileSize-size;
	}
	}
	else if (UArr[fd].ptrfiletable->mode==WRITE)
	{

		if	(from==START)
	{
		if (size>MAXFILESIZE)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->WriteOffset=size;
		
	}
	else if (from==CURRENT)
	{
		if ((UArr[fd].ptrfiletable->WriteOffset+size)>MAXFILESIZE)
		{
			return -1;
		}
		if ((UArr[fd].ptrfiletable->WriteOffset+size)<0)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->WriteOffset=UArr[fd].ptrfiletable->WriteOffset+size;
	}
	else if (from==END)
	{
		if (size>MAXFILESIZE)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->WriteOffset=MAXFILESIZE-size;
	}
	}
	return 1;
}
void close (char *name)
{
	int fd=0;
	fd=findfd(name);
	if (UArr[fd].ptrfiletable->ptrinode->ReferenceCount>0)
	{
	(UArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
	(UArr[fd].ptrfiletable->ReadOffset)=0;
	UArr[fd].ptrfiletable->WriteOffset=0;
	}
}
int main()
{
	char command[4][80],str[80];
	char arr[1024]={'\0'};
	char *str1=NULL;
	int count=0,ret=0,fd=0;
	InitialiseSuperB();
	CreateDILB();
	while (1)
	{
		fflush(stdin);
		strcpy (str,"");
		printf ("\n\nTerminal Marvellous ->");
		fgets(str,80,stdin);

		 count = sscanf (str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
		 
		 if (count==1)
		 {
			 if (strcmp(command[0],"ls")==0)
			 {
				 File_ls();
				 
			 }
			 else if (strcmp(command[0],"clear")==0)
			 {
				 system("cls");
				 continue;
			 }
			 else if (strcmp(command[0],"exit")==0)
			 {
				  
				 //ret=Deleteall();
				 
				 printf ("\n**Terminate the Terminal Marvellous**\n");
				 break;
			 }
			 else if (strcmp(command[0],"closeall")==0)
			 {
			 }
			 else if (strcmp(command[0],"help")==0)
			 {
				 help();
				 continue;
			 }
			 else
			 {
				 printf ("ERROR :command not found\n");
				 continue;
			 }
		 }//End of count==1
		 else if (count==2)
		 {
			 if (strcmp(command[0],"stat")==0)
			 {
				 ret=stat(command[1]);
				 if (ret==-1)
				 {
					 printf ("Invalid parameter");
				 }
				 if (ret==-2)
				 {
					 printf ("File not found");
				 }
			}
			 else if (strcmp(command[0],"man")==0)
			 {
				 man(command[1]);
			 }
			 else if (strcmp(command[0],"close")==0)
			 {
				 close(command[1]);
				 continue;
			 }
			 else if (strcmp(command[0],"fstat")==0)
			 {
				 fstat(atoi(command[1]));
			 }
			 else if (strcmp(command[0],"rm")==0)
			 {
				 ret=remove(command[1]);
				if (ret==0)
				 {
					 printf ("***Remove successfully***\n");
				 }
				 if (ret==-1)
				 {
					 printf ("File not found");
				 }
	       	 }
			else  if (strcmp(command[0],"write")==0)
				{
					fflush(stdin);
					printf ("\nEnter the data :");
					scanf ("%[^\n]",arr);
					if (strlen(arr)==0)
					{
						printf ("\nInvalid");
						continue;
					}
					ret=strlen(arr);
					fd=findfd(command[1]);
					ret=write(fd,arr,ret);
					if (ret==0)
					{
						 printf ("***Written Sucessfully***\n");
						 continue;
					}
					if (ret==-1)
					{
						 printf ("File size is full");
						 continue;
					}
					if (ret==-2)
					{
						 printf ("No permission for write");
						 continue;
					}
				 }
			else if (strcmp(command[0],"truncate")==0)
			{
				ret=truncate(command[1]);
				if (ret==-1)
				{
					printf ("\nInvalid parameter");
					continue;
				}
				else
				{
					printf ("\n***Data deleted from file***");
					continue;
				}
			}
			else 
			{
				printf ("ERROR:command not found\n");
				continue;
			}

		 }//end of count=2
		 
		 else if (count==3)
		 {
			 if (strcmp(command[0],"create")==0)
			 {
				 ret=CreateFile(command[1],atoi(command[2]));
				 if (ret>=0)
				 {
					 printf ("***File is successfully created with file descriptor- %d***",ret);
						continue;
				 }
				 else if (ret==-1)
				 {
					 printf ("File Already exist");
					 continue;
				 }
				 else if (ret==-2)
				 {
					 printf ("Incorrect Parameter");
				 }
				 else if (ret==-3)
				 {
					 printf ("Inode not available ");
				 }
				 continue;
			 }
			 else if (strcmp(command[0],"open")==0)
			 {
				 ret=open(command[1],atoi(command[2]));
				 printf ("\nOpened file descriptor is %d\n",ret);
			 }

			  else if (strcmp(command[0],"read")==0)
				 {
					fd=findfd(command[1]);
					ret=atoi(command[2]);
					str1=(char *)malloc((sizeof(char)*ret)+1);
					for (int i=0;i<(sizeof(char)*ret)+1;i++)
					{
						str1[i]='\0';
					}
					
					 ret=read(fd,str1,ret);
					 
					 printf ("%s",str1);
					 if (str1==NULL)
					 {
						 ("\nMemory allocation fail");
					 }
					 if (ret==0)
					{
						 printf ("***Read successfully***\n");
						 continue;
					}
					if (ret==-1)
					{
						 printf ("File have not permission to read");
						 continue;
					}
				}
			  else
			  {
				  printf ("ERROR:command not found\n");
				  continue;
			  }
		 }
		else if (count==4)
		 {
				  if (strcmp(command[0],"lseek")==0)
				{
					 ret=lseek(command[1],atoi(command[2]),atoi(command[3]));
					 if (ret==1)
					 {
						 printf ("\nlseek sucessfully done");
					 }
				}
				  continue;
		 }

		}
	return 0;
}
