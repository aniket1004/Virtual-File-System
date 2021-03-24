#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>


#define MAXINODE 50
#define MAXFILESIZE 1024

#define READ 4
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
	char Birth[60];
	char LastAccessTime[60];
	char LastModifiedTime[60];
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

PINODE head = NULL;
UFDT UArr[50];
SB superblockobj;


void CreateDILB ()
{
	PINODE temp=NULL,newn=NULL;

	for (int i = 1; i <= MAXINODE; i++)
	{
		newn = (PINODE)malloc(sizeof(INODE));

		if (head == NULL)
		{
			head = newn;
			temp = head;
		}
		else
		{
			temp->next = newn;
			temp = newn;
		}

		newn->ReferenceCount = newn->ActualFileSize = newn->FileType = newn->LinkCount = 0;
		newn->Buffer = NULL;
		newn->Ino = i;
		
	}
	
}

void InitialiseSuperB()
{
	for (int i = 0;i < 50;i++)
	{
		UArr[i].ptrfiletable = NULL;
	}
	superblockobj.TInode = MAXINODE;
	superblockobj.FInode = MAXINODE;
}
void DisplayHelp()
{
	printf("\n----------------------------------------------------\n");
	printf("open : It is used to open the file\n");
	printf("close : It is used to close the file\n");
	printf("closeall : It is used to close all opened files\n");
	printf("read : It is used to read the file\n");
	printf("write : It is used to write the file\n");
	printf("delete : It is used to delete the file\n");
	printf("stat : It is used to display information the file\n");
	printf("fstat : It is used to display information of opened the file\n");
	printf("ls : To list out all files present in that directory\n");
	printf("lseek : Read the data where starting point of reading data is depends on user.\n");
	printf("clear : To clear console\n");
	printf("truncate : To shrink or extend the file\n");
	printf("cat : To display the data of file without openeing it\n");
	printf("exit : To exit the application\n");
	printf("-----------------------------------------------------------\n");
	printf("to know more about command type : 'man command_name'\n");
	printf("-----------------------------------------------------------\n");
}


void DisplayManpage(char *str)
{
	if(str==NULL)
	{
		return;
	}
	if(strcasecmp(str,"open")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\topen - open the file"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\topen file_name mode"
				"\n\n"
				"DESCRIPTION\n"
				"\t\topen the existing file which is specified by user.\n" 
			   	"\t\tmode - \t"
			   	"1.\tr or 4 - open file in read mode.\n"
			   	"\t\t\t2.\tw or 2 - open file in write mode\n"
			   	"\t\t\t3.\trw or 6 - open file in read & write both mode"
				"\n\n"
				"RETURN VALUE\n"
				"\t\ton success returned file descriptor, on failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"close")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tclose- close the file"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tclose file_name"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tcloses the all file_descriptor pointing to file which is specified by user.so it may reused."
				"\n\n"
				"RETURN VALUE\n"
				"\t\ton failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"read")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tread - read from a file name"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\t1.\tread file_name\n"
			   	"\t\t2.\tread file_name no_of_bytes"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tread attempts to read up to count bytes from file descriptor fd into the buffer starting at buf."
				"\n\t\tif u did not specify the no of bytes you want to read then it will read the whole file from current offset."
				"\n\n"
				"RETURN VALUE\n"
				"\t\tOn success, the number of bytes read is returned, on failure error name gets displayed appropriately\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"write")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\twrite - write from a file_name"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\twrite file_name"
				"\n\n"
				"DESCRIPTION\n"
				"\t\twrite attempts to write up to count bytes from file name in the file."
				"\n\n"
				"RETURN VALUE\n"
				"\t\tOn success, the number of bytes written is returned, and the file position is advanced by this number and on failure errorname gets display.\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"ls")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tls - displays list"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\t1.\tls\n"
			   	"\t\t2.\tls -i\n"
			   	"\t\t3.\tls fd"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tls displays the list of files present in directory\n"
				"\t\tls -i displays the list of file with their inode number\n"
				"\t\tls fd display the list of files opened with their file descriptor\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"stat")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tstat - information from a file_name"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tstat file_name"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tstat command display the all information related to that file."
				"\n\n"
				"RETURN VALUE\n"
				"\t\tOn success, display information, on failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"fstat")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tfstat - information from a file descriptor"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tfstat file_descriptor"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tfstat command display the all information related to that file descriptor."
				"\n\n"
				"RETURN VALUE\n"
				"\t\tOn success, display information, on failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"truncate")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\ttruncate - shrink or extend the size of a file to the specified size"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\ttruncate file_name size"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tIf  a  Actual File Size is larger than the specified size, the extra data is lost.\n"
				"\t\tIf a Actual File Size is shorter, it is extended and the extended part (hole) reads as zero bytes."
				"\n\n"
				"RETURN VALUE\n"
				"\t\tOn success display success, on failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"create")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tcreate - creates the file"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tcreate file_name permission"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tcreate the new file.\n" 
			   	"\t\tpermssion - \t"
			   	"1.\tr or 4 - gives file only read permission to the file.\n"
			   	"\t\t\t\t2.\tw or 2 - gives only write permission to the file.\n"
			   	"\t\t\t\t3.\trw or 6 - gives read and write both permission to the file."
				"\n\n"
				"RETURN VALUE\n"
				"\t\ton success returned file descriptor, on failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"closeall")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tcloseall - closes all file_descriptor"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tcloseall"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tcloseall closes the all the file_descriptor pointing to any file, so it may be reused."
				"\n\n"
				"RETURN VALUE\n"
				"\t\tOn success display success, on failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"lseek")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tlseek - reposition read/write file offset"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tlseek file_name offset flag"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tlseek repositions the file offset of the open file description associated with the file name\n" 
                "\t\tto the argument offset according to the directive flag as follows:\n"
			   	"\t\tflag \t"
			   	"1.\tstart - The file offset is set to offset bytes.\n"
			   	"\t\t\t2.\tcurrent - The file offset is set to its current location plus offset bytes.\n"
			   	"\t\t\t3.\tend - The file offset is set to the size of the file plus offset bytes.\n"
			   	"\t\t\tlseek() allows the file offset to be set beyond the end of the file and creates hole and file size changes."
				"\n\n"
				"RETURN VALUE\n"
				"\t\ton failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"delete")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tdelete - delete the file"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tdelete file_name"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tdelete command the delete the file specified by user." 
				"\n\n"
				"RETURN VALUE\n"
				"\t\ton failure display error\n");
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"exit")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\texit - terminate the application"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\texit"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tclose the application\n"); 
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"cat")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tcat - display the file data"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tcat File_name"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tdisplay the data of file without openeing the file\n"); 
		printf("\n----------------------------------------------------\n");
	}
	else if(strcasecmp(str,"man")==0)
	{
		printf("\n----------------------------------------------------\n");
		printf(	"NAME\n"
       		   	"\t\tman - an interface to the application reference manuals"
       		   	"\n\n"
			   	"SYNOPSIS\n"
			   	"\t\tman command_name"
				"\n\n"
				"DESCRIPTION\n"
				"\t\tman is the application's manual pager.  Each page argument given to man is the name of a command.\n"
				"\t\tThe manual page associated with each of these arguments is then found and displayed. \n"); 
		printf("\n----------------------------------------------------\n");
	}
	else
	{
		printf("error : man page not found for entry %s\n",str);
	}
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
	printf ("Birth- %s",UArr[fd].ptrfiletable->ptrinode->Birth);
        printf ("Last Access Time- %s",UArr[fd].ptrfiletable->ptrinode->LastAccessTime);
        printf ("Last Modified Time- %s",UArr[fd].ptrfiletable->ptrinode->LastModifiedTime);
}

PINODE Get_Inode (char *name)
{
	int i = 0;
	PINODE temp = head;

	if (name == NULL)
	{
		return NULL;
	}

	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			if (strcmp(temp->Fname,name) == 0)
			{
				break;
			}
		}
		temp = temp->next;
	}
	if (temp != NULL)
	{
		return temp;
	}
	else
	{
		return NULL;
	}
}
int CreateFile(char *name,int permission)
{
	PINODE temp = head;
	time_t t;
	int i = 0;

	if ((name == NULL)||((permission != READ) && (permission != WRITE) && (permission != (READ + WRITE))))
	{
		return -2;
	}
	if (superblockobj.FInode == 0)
	{
		return -3;
	}
	if (Get_Inode(name) != NULL)
	{
		return -1;
	}
	while(temp->FileType != 0)
	{
		temp = temp->next;
	}
	for (i = 0; i < 50; i++)
	{
		if (UArr[i].ptrfiletable == NULL)
		{
			break;
		}
	}
	PFT fileobj = (PFT)malloc(sizeof(FT));
	UArr[i].ptrfiletable = fileobj;
	UArr[i].ptrfiletable->ReadOffset = 0;
	UArr[i].ptrfiletable->WriteOffset = 0;
	UArr[i].ptrfiletable->count = 1;
	UArr[i].ptrfiletable->mode = permission;
	UArr[i].ptrfiletable->ptrinode = temp;
	UArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);
	UArr[i].ptrfiletable->ptrinode->Permission = permission;
	UArr[i].ptrfiletable->ptrinode->ActualFileSize = 0;
	UArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
	UArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	strcpy(UArr[i].ptrfiletable->ptrinode->Fname,name);
	
	time(&t);
	
	strcpy(UArr[i].ptrfiletable->ptrinode->Birth,ctime(&t));
	strcpy(UArr[i].ptrfiletable->ptrinode->LastAccessTime,ctime(&t));
	strcpy(UArr[i].ptrfiletable->ptrinode->LastModifiedTime,"-");
	(superblockobj.FInode)--;
	return i;
}
int remove (char *name)
{
	int i = 0;
	PINODE temp = head;

	for (i = 0; i < 50; i++)
	{
		if (UArr[i].ptrfiletable != NULL)
		{
			if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name) == 0)
			{
				free(UArr[i].ptrfiletable->ptrinode->Buffer);
				UArr[i].ptrfiletable->ptrinode->FileType=0;
				memset(UArr[i].ptrfiletable->ptrinode->Fname,'\0',50);
				UArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
				UArr[i].ptrfiletable->ptrinode->ActualFileSize = 0;
				UArr[i].ptrfiletable->ptrinode->ReferenceCount = 0;
				UArr[i].ptrfiletable->ptrinode->Permission = 0; 
				UArr[i].ptrfiletable->ptrinode->LinkCount = 0;
				UArr[i].ptrfiletable->ptrinode = NULL;
				free(UArr[i].ptrfiletable);
				UArr[i].ptrfiletable = NULL;
				break;
			}
		}
	}
	
	if (i == 50)
	{
		while (temp != NULL)
		{
			if (temp->FileType != 0)
			{
				if (strcmp(temp->Fname,name) == 0)
				{
					free(temp->Buffer);
					temp->FileType=0;
					memset(temp->Fname,'\0',50);
					temp->FileSize = MAXFILESIZE;
					temp->ActualFileSize = 0;
					temp->ReferenceCount = 0;
					temp->Permission = 0; 
					temp->LinkCount = 0;
				
					break;
				}
			}
			temp = temp->next;
		}
		if (temp == NULL)
			return -1;
	}
	(superblockobj.FInode)++;
	return 0;
}

int findfd (char *name)
{
	int i = 0;
	if (name == NULL)
	{
		return -1;
	}
	while (i < 50)
	{
		if (UArr[i].ptrfiletable != NULL)
		{
			if (strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name) == 0)
			{
				break;
			}
		}
		i++;
	}
	if (i == 50)
	{
		return -2;
	}

	return i;
}

void File_ls()
{
	PINODE temp = head;

	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf ("%s\n",temp->Fname);
		}
		temp = temp->next;
	}
}
int stat(char *name)
{
	PINODE temp = head;
	if (name == NULL)
	{
		return -1;
	}
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			if (strcmp(temp->Fname,name) == 0)
			{
				break;
			}
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
		return -1;
	}

	printf ("File name- %s\n",temp->Fname);
	printf ("Inode number- %d\n",temp->Ino);
	printf ("File size- %d\n",temp->FileSize);
	printf ("File type- %d\n",temp->FileType);
	printf ("Actual File size- %d\n",temp->ActualFileSize);
	printf ("Birth- %s\n",temp->Birth);
	printf ("Last Access Time- %s\n",temp->LastAccessTime);
	printf ("Last Modified Time- %s\n",temp->LastModifiedTime);
	if (temp->Permission == READ)
	{
		printf ("Permission- READ ONLY\n");
	}
	else if (temp->Permission == WRITE)
	{
		printf ("Permission- WRITE ONLY\n");
	}
	else
	{
		printf ("Permission- READ and WRITE\n");
	}
	printf ("Link count- %d\n",temp->LinkCount);
	printf ("Reference count- %d\n",temp->ReferenceCount);
	
	return 0;
}

int write_file (int fd,char *brr,int size)
{
	time_t t;

	if (UArr[fd].ptrfiletable->ptrinode->ActualFileSize == MAXFILESIZE)
	{
		return -1;
	}
	if ((UArr[fd].ptrfiletable->mode == READ)||(UArr[fd].ptrfiletable->ptrinode->Permission == READ))
	{
		return -2;
	}
	if (UArr[fd].ptrfiletable->WriteOffset + size >= MAXFILESIZE)
	{
		size = MAXFILESIZE - UArr[fd].ptrfiletable->WriteOffset;
	}

	strncpy((UArr[fd].ptrfiletable->ptrinode->Buffer)+(UArr[fd].ptrfiletable->WriteOffset),brr,size);
	UArr[fd].ptrfiletable->WriteOffset = (UArr[fd].ptrfiletable->WriteOffset) + size;
	UArr[fd].ptrfiletable->ptrinode->ActualFileSize += size;
	UArr[fd].ptrfiletable->ptrinode->Buffer[UArr[fd].ptrfiletable->ptrinode->ActualFileSize] = '\0';

	time(&t);
	//strcpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime,ctime(&t));
	strcpy(UArr[fd].ptrfiletable->ptrinode->LastModifiedTime,ctime(&t));

	return size;
}

int read_file (int fd,char *brr,int size)
{
	int size_t = 0;
	time_t t;
	if  (UArr[fd].ptrfiletable->mode == WRITE)
	{
			return -1;
	}
	if (UArr[fd].ptrfiletable->ReadOffset == UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
	{
		return -2;
	}
	
	size_t = (UArr[fd].ptrfiletable->ptrinode->ActualFileSize)-(UArr[fd].ptrfiletable->ReadOffset);
			if (size_t<size)
			{
				strncpy(brr,(UArr[fd].ptrfiletable->ptrinode->Buffer)+(UArr[fd].ptrfiletable->ReadOffset),size_t);
				UArr[fd].ptrfiletable->ReadOffset += size_t;
				size = size_t;
			}
			else
			{
				strncpy(brr,(UArr[fd].ptrfiletable->ptrinode->Buffer)+(UArr[fd].ptrfiletable->ReadOffset),size);
				UArr[fd].ptrfiletable->ReadOffset += size;
			}
	time(&t);
	strcpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime,ctime(&t));
	return size;
}
int readWholeFile (int fd)
{
	int size = 0;
	time_t t;

	if  (UArr[fd].ptrfiletable->mode == WRITE)
	{
			return -1;
	}
	printf ("%s",UArr[fd].ptrfiletable->ptrinode->Buffer);
	size = strlen(UArr[fd].ptrfiletable->ptrinode->Buffer);
	time(&t);
	strcpy(UArr[fd].ptrfiletable->ptrinode->LastAccessTime,ctime(&t));

	return size;
}


int Deleteall()
{
	 PINODE temp = head;
	 int i = 0;
				  
				for (i = 0; i < 50; i++)
				{
					if (UArr[i].ptrfiletable != NULL)
					{
						UArr[i].ptrfiletable->ptrinode = NULL;
						free(UArr[i].ptrfiletable);
						UArr[i].ptrfiletable = NULL;
					}
				}

				while (temp != NULL)
				{
					if (temp->FileType != 0)
					{
						free(temp->Buffer);
						temp->Buffer = NULL;
						temp->FileType = 0;
						memset(temp->Fname,'\0',50);
						temp->FileSize = MAXFILESIZE;
						temp->ActualFileSize = 0;
						temp->ReferenceCount = 0;
						temp->Permission = 0; 
						temp->LinkCount = 0;
					}
					temp = temp->next;
				}
	printf ("All Files Delete Successfully\n");			
				  return 0;

}
int truncate_file(char *name, int size)
{
	int start = 0;
	if (name == NULL)
	{
		return -1;
	}
	int fd = findfd(name);
	if (fd == -2)
	{
		printf ("File not exist or not open\n");
		return -1;
	}
	if (size < 0)
	{
		printf ("Invalid Parameter\n");
		return -1;
	}
	else if (size <= UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
	{
		UArr[fd].ptrfiletable->ptrinode->Buffer[size] = '\0';
		UArr[fd].ptrfiletable->ptrinode->ActualFileSize = size;
	}
	else if ((size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize) && (size <= MAXFILESIZE))
	{
		for (start = UArr[fd].ptrfiletable->ptrinode->ActualFileSize; start < size; start++)
		{
			UArr[fd].ptrfiletable->ptrinode->Buffer[start] = ' ';
		}
		UArr[fd].ptrfiletable->ptrinode->Buffer[start-1] = '\0';
		UArr[fd].ptrfiletable->ptrinode->ActualFileSize = size;
	}
	else
	{
		printf ("Invalid Parameter\n");
		return -1;
	}
	return 0;
}

int open_file (char *name,int mode)
{
	int i = 0;
	time_t t;

	if ((name == NULL)||((mode != READ) && (mode != WRITE) && (mode != (READ +WRITE))))
	{
		return -1;
	}
	PINODE temp = Get_Inode(name);
	if (temp == NULL)
	{
		printf ("File %s not exist\n",name);
		return -1;
	}

	while (i < 50)
	{
		if (UArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;
	}
	if (i == 50)
	{
		printf ("UFDT reaches its limit\n");
		return -1;
	}
	time(&t);
	UArr[i].ptrfiletable=(PFT)malloc(sizeof(FT));
	UArr[i].ptrfiletable->count = 1;
	UArr[i].ptrfiletable->mode = mode;
	UArr[i].ptrfiletable->ReadOffset = 0;
	UArr[i].ptrfiletable->WriteOffset = 0;
	UArr[i].ptrfiletable->ptrinode = temp;
	(UArr[i].ptrfiletable->ptrinode->ReferenceCount)++;
	strcpy(UArr[i].ptrfiletable->ptrinode->LastAccessTime,ctime(&t));
	
	return i;
}

int lseek_file(char *name,int size,int from)
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
	if ((UArr[fd].ptrfiletable->mode == READ)||(UArr[fd].ptrfiletable->mode == (READ+WRITE)))
	{
	if(from == START)
	{
		if (size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->ReadOffset = size;
		
	}
	else if (from == CURRENT)
	{
		if ((UArr[fd].ptrfiletable->ReadOffset+size) > (UArr[fd].ptrfiletable->ptrinode->ActualFileSize))
		{
			return -1;
		}
		if ((UArr[fd].ptrfiletable->ReadOffset+size) < 0)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->ReadOffset = UArr[fd].ptrfiletable->ReadOffset+size;
	}
	else if (from == END)
	{
		if (size > UArr[fd].ptrfiletable->ptrinode->ActualFileSize)
		{
			return -1;
		}
		UArr[fd].ptrfiletable->ReadOffset=UArr[fd].ptrfiletable->ptrinode->ActualFileSize - size;
	}
	}
	else if (UArr[fd].ptrfiletable->mode == WRITE)
	{
		if	(from == START)
		{
			if (size > MAXFILESIZE)
			{
				return -1;
			}
			UArr[fd].ptrfiletable->WriteOffset = size;
		
		}
		else if (from == CURRENT)
		{
			if ((UArr[fd].ptrfiletable->WriteOffset + size) > MAXFILESIZE)
			{
				return -1;
			}
			if ((UArr[fd].ptrfiletable->WriteOffset + size) < 0)
			{
				return -1;
			}
			UArr[fd].ptrfiletable->WriteOffset = UArr[fd].ptrfiletable->WriteOffset + size;
		}
		else if (from == END)
		{
			if (size > MAXFILESIZE)
			{
				return -1;
			}
			UArr[fd].ptrfiletable->WriteOffset = MAXFILESIZE - size;
		}
	}
	return 1;
}
void close_file (char *name)
{
	int fd = 0, i = 0;

	for (i = 0; i < 50; i++)
	{
		if (UArr[i].ptrfiletable != NULL)
		{
			if ((strcmp(UArr[i].ptrfiletable->ptrinode->Fname,name) == 0) && (UArr[i].ptrfiletable->ptrinode->ReferenceCount > 0))
			{
				(UArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
				UArr[i].ptrfiletable->ptrinode = NULL;
				free(UArr[i].ptrfiletable);
				UArr[i].ptrfiletable = NULL;
				fd = 1; 
			}
		}
	}
	if (fd == 0)
	{
		printf ("File %s not exist or not open\n",name);
		return;
	}
	printf ("File %s close successfully\n",name);

}

void cat_file(char *name)
{
	if (name == NULL)
	{
		printf ("Invalid parameter\n");
		return;
	}
	PINODE temp = head;
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			if (strcmp(temp->Fname,name) == 0)
			{
				if (temp->Buffer != NULL)
				{
					printf ("%s",temp->Buffer);
				}
				break;
			}
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
		printf ("File %s not exist\n",name);
		return;
	}
}

void BackupData()
{
	int fd = 0;

	char data[MAXFILESIZE] = "aniket";
	PINODE temp = head;
	int size = 0;
	fd = creat("backup.txt", 0777);
	if (fd != -1)
	{
		close(fd);
	}
	fd = open("backup.txt",O_WRONLY);


	while(temp != NULL)
	{
		if(temp->FileType != 0)
		{
			write(fd,temp,sizeof(INODE));
			write(fd,temp->Buffer,MAXFILESIZE);
		}
		temp = temp->next;
	}
	close(fd);
}

void RestoreData()
{
	int fd= 0,ret = 0;
	char Data[1024];
	INODE iobj;
	PINODE temp = head;

	fd = open("backup.txt",O_RDONLY);
	if(fd == -1)
	{
		return;
	}

	int size = lseek(fd,0,SEEK_END);
	if(size == 0)
	{
		return;
	}

	lseek(fd,0,SEEK_SET);

	while((ret = read(fd,&iobj,sizeof(INODE))) > 0)
	{

		temp->Ino = iobj.Ino;
		
		strcpy(temp->Fname,iobj.Fname);
		
		temp->FileSize=iobj.FileSize;
		
		temp->FileType=iobj.FileType;
		
		temp->ActualFileSize = iobj.ActualFileSize;
		
		temp->LinkCount=iobj.LinkCount;
		
		temp->ReferenceCount = iobj.ReferenceCount;

		temp->Permission = iobj.Permission;
		
		strcpy(temp->Birth,iobj.Birth);
		
		strcpy(temp->LastAccessTime,iobj.LastAccessTime);
		
		strcpy(temp->LastModifiedTime,iobj.LastModifiedTime);

		temp->Buffer = (char *)malloc(MAXFILESIZE);
		read(fd,temp->Buffer,MAXFILESIZE);
		
		temp = temp->next;
		
	}

	close(fd);
	printf("Data Restored\n");
}

void sighandle(int s)
{
	BackupData();
	printf ("\n**Terminate the Terminal Aniket**\n");
	exit(0);
}

int main()
{
	char command[4][80],str[80];
	char arr[1024] = {'\0'};
	char *str1 = NULL;
	int count = 0,ret = 0,fd = 0;
	InitialiseSuperB();
	CreateDILB();
	RestoreData();

	signal(SIGINT,sighandle);
	while (1)
	{
		
		printf ("\nTerminal Aniket $ ");
		scanf(" %[^'\n']s",str);

		count = sscanf (str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
		 
		 if (count==1)
		 {
			 if (strcmp(command[0],"ls")==0)
			 {
				 File_ls();
				 
			 }
			 else if (strcmp(command[0],"clear")==0)
			 {
				 system("clear");
				 continue;
			 }
			 else if (strcmp(command[0],"exit")==0)
			 {
				  BackupData();
				  
				 printf ("\n**Terminate the Terminal Aniket**\n");
				 break;
			 }
			 else if (strcmp(command[0],"deleteall")==0)
			 {
				 Deleteall();
			 }
			 else if (strcmp(command[0],"help")==0)
			 {
				 DisplayHelp();
				 continue;
			 }
			 else
			 {
				 printf ("ERROR :command not found\n");
				 continue;
			 }
		 }//End of count==1
		 else if (count == 2)
		 {
			 if (strcmp(command[0],"stat")==0)
			 {
				 ret = stat(command[1]);
				 if (ret==-1)
				 {
					 printf ("Invalid parameter");
				 }
				 if (ret==-2)
				 {
					 printf ("File not found");
				 }
			 }
			 else if (strcmp(command[0],"cat") == 0)
			 {
				 cat_file(command[1]);
			 }
			 else if (strcmp(command[0],"man")==0)
			 {
				 DisplayManpage(command[1]);
			 }
			else if (strcmp(command[0],"read") == 0)
			 {
				fd = findfd(command[1]);
				if (fd == -2)
				{
					printf ("File not exist or not exist\n");
					continue;
				}
				readWholeFile(fd);
			 }
			 else if (strcmp(command[0],"close")==0)
			 {
				 close_file(command[1]);
				 continue;
			 }
			 else if (strcmp(command[0],"fstat")==0)
			 {
				 ret = atoi(command[1]);
				 if (UArr[ret].ptrfiletable == NULL)
				 {
					 printf ("File not exist or not open\n");
					 continue;
				 }
				 fstat(atoi(command[1]));
			 }
			 else if (strcmp(command[0],"rm")==0)
			 {
				 ret = remove(command[1]);
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
					fd = findfd(command[1]);
					if (fd == -2)
					{
						printf ("File not exist or not exist\n");
					}
					
					printf ("\nEnter the data :");
					scanf (" %[^\n]s",arr);
					fflush(stdin);
					if (strlen(arr) == 0)
					{
						printf ("\nInvalid");
					}
					ret = strlen(arr);
					ret = write_file(fd,arr,ret);
				
					if (ret > 0)
					{
						 printf ("\n***Written Sucessfully***\n");
					}
					if (ret == -1)
					{
						 printf ("File size is full");
					}
					if (ret == -2)
					{
						 printf ("No permission for write");
						 
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
				 ret=open_file(command[1],atoi(command[2]));
				 printf ("\nOpened file descriptor is %d\n",ret);
			 }
			 else if (strcmp(command[0],"truncate")==0)
			 {
				ret = truncate_file(command[1], atoi(command[2]));
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
			  else if (strcmp(command[0],"read")==0)
				 {
					fd = findfd(command[1]);
					if (fd == -2)
					{
						printf ("File not exist or not exist\n");
						continue;
					}
					ret = atoi(command[2]);
					str1 = (char *)malloc((sizeof(char)*ret)+1);
					for (int i = 0;i < (sizeof(char)*ret) + 1; i++)
					{
						str1[i]='\0';
					}
					
					 ret = read_file(fd,str1,ret);
					 
					 printf ("%s",str1);
					 if (str1 == NULL)
					 {
						 ("\nMemory allocation fail");
					 }
					 if (ret == 0)
					 {
						 printf ("***Read successfully***\n");
						 continue;
					 }
					if (ret == -1)
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
					 ret=lseek_file(command[1],atoi(command[2]),atoi(command[3]));
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
