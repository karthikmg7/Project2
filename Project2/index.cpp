#include<iostream>
#include<conio.h>
#include"str.h"
#include<fstream>
#include"filereader.h"
#include <sstream>

struct StudentRecord {
	MyString sid;
	MyString firstName;
	MyString lastName;
	MyString major;
	int size;

};

enum command{
	ADD,
	DELETE,
	FIND,
	ENDCMD
}cmd;
char delim='|';
struct index{
	int key;
	long offset;
	int size;
};
struct index*indexArray=NULL;
int indexCount=0,recordAlreadyExists;
long globalOffset;
struct availability{
	int size;
	long offset;
};
struct availabilityList{
	struct availability aInfo;
	struct availabilityList*llink;
	struct availabilityList*rlink;
};
typedef struct availabilityList * aList;
aList first,last,temp;
using namespace std;
void sort()
{
	int i,j,key;
	j=0;
	struct index keyIndex;
	for(i=1;i<indexCount;i++)
	{
		j=i-1;
		keyIndex=indexArray[i];
		while(j>=0 && indexArray[j].key>keyIndex.key)
		{
			indexArray[j+1]=indexArray[j];
			j--;
		}
		indexArray[j+1]=keyIndex;
	}
}
void writeToResultsFile(MyString str)
{
	ofstream outfile;
  outfile.open("resultsFile.txt", std::ios_base::app);
  outfile << str<<endl; 
  outfile.close();
}
void writeToResultsFile(string str)
{
	ofstream outfile;
  outfile.open("resultsFile.txt", std::ios_base::app);
  outfile << str<<endl; 
  outfile.close();
}
void printIndexArray()
{
	MyString str;
	//if(indexCount==0)
	//{
	//	str="NO INDEXES PRESENT";
	//	writeToResultsFile(str);
	//	return;
	//}
	str="INDEX:";
	cout<<str<<endl;
		writeToResultsFile(str);


	for(int j=0;j<indexCount;j++)
	{
		string s=to_string(indexArray[j].key)+":"+to_string(indexArray[j].offset);
		printf( "%d: %ld\n", indexArray[ j ].key, indexArray[ j ].offset );
		//cout<<indexArray[j].key<<"\n"<<indexArray[j].offset<<"\n\n";
		writeToResultsFile(s);
	}
	return;
}
void addToIndexArray(struct index iInfo)
{
	indexArray=(struct index*)realloc(indexArray,sizeof(struct index)*indexCount);
	if(indexArray!=NULL)
	{
		indexArray[indexCount-1].key=iInfo.key;
		indexArray[indexCount-1].offset=iInfo.offset;
		indexArray[indexCount-1].size=iInfo.size;

	}
	sort();
	//printIndexArray();
}
void addToaList(struct availability aInfo)
{
	aList newAlist=new  struct availabilityList;
	newAlist->aInfo=aInfo;
	newAlist->llink=NULL;
	newAlist->rlink=NULL;
	if(first==NULL)
	{
		first=last=newAlist;

	}
	else
	{
		last->rlink=newAlist;
		newAlist->llink=last;
		last=newAlist;
	}

}
long useaList(int size)
{
	if(first==NULL)return -1;
	aList prev=NULL,next=NULL;
	temp=first;
	while(temp!=NULL)
	{
		if(temp->aInfo.size>=size)
		{
			break;
		}
		
		temp=temp->rlink;
	}
	if(temp==NULL)return -1;
	prev=temp->llink;
	next=temp->rlink;
	int originalOffset=temp->aInfo.offset;
	temp->aInfo.offset=temp->aInfo.offset+size;
	temp->aInfo.size=temp->aInfo.size-size;
	if(prev!=NULL)
		prev->rlink=next;
	if(next!=NULL)
		next->llink=prev;
	if(temp==first)
		first=next;
	if(temp->aInfo.size!=0)
	addToaList(temp->aInfo);
	delete temp;
	return originalOffset;

}

void storeAvailabilityListinFile()
{
	ofstream availabilityListFile;
availabilityListFile.open("availability_list_file.txt");
temp=first;
if(temp==NULL)
{
	availabilityListFile.close();
	return;
}
while(temp!=NULL)
{
	availabilityListFile<<temp->aInfo.offset<<endl;
	availabilityListFile<<temp->aInfo.size<<endl;
	temp=temp->rlink;

}
availabilityListFile.close();

}

void readFromAvailabilityListFile()
{
	std::string buffer;
	ifstream  availabilityListFile("availability_list_file.txt");

int i=0;
struct availability aInfo;
int count=0;
long x=0;
if(availabilityListFile==NULL)return;
while(!availabilityListFile.eof())
{
	count++;
	
	if(i==0)
	{
		availabilityListFile>>aInfo.offset;
		i++;
	}
	else
	{
		availabilityListFile>>aInfo.size;
		addToaList(aInfo);
		i=0;

	}
}
availabilityListFile.close();
cout<<count;
return;
}

void storeIndexInFile()
{
	ofstream indexFile;
indexFile.open("index_file.txt");
if(indexCount<=0)
{
	indexFile.close();
	return;
}
for(int j=0;j<indexCount;j++)
{
	indexFile<<indexArray[j].key<<endl;
	indexFile<<indexArray[j].offset<<endl;
	indexFile<<indexArray[j].size<<endl;
	

}
indexFile.close();

}

void readFromIndexFile()
{
	
	ifstream  indexFile("index_file.txt");

int i=0;
struct index iInfo;
int count=0;
long x=0;
if(indexFile ==NULL)return;
while(!indexFile.eof())
{
	count++;
	
	if(i==0)
	{
		indexFile>>iInfo.key;
		i++;
	}
	else if(i==1)
	{
		indexFile>>iInfo.offset;
		i++;

	}
	else if(i==2)
	{
		indexFile>>iInfo.size;
		indexCount++;
		addToIndexArray(iInfo);
		i=0;

	}
}
indexFile.close();
cout<<count;
return;
}

void writeToFile(struct StudentRecord sr)
{

}

void printAvailabilityList()
{
	temp=first;
	MyString str;
	/*if(first==NULL)
	{
		str="NO AVAILABILITY LIST";
		writeToResultsFile(str);
		return;
	}*/
	str="AVAILABILITY:";
	cout<<str<<endl;
	writeToResultsFile(str);
		while(temp!=NULL)
		{
			
			string s=to_string(temp->aInfo.offset)+":"+to_string(temp->aInfo.size);
				printf( "%d: %ld\n",temp->aInfo.offset, temp->aInfo.size );
			writeToResultsFile(s);
			temp=temp->rlink;
		}

	return;
}





struct index* searchIndexArray(struct StudentRecord sr)
{
	int result;
	string str=sr.sid;
	result=stoi(str);
	if(indexCount==0){recordAlreadyExists=0;return NULL;}
	int low=0,high=indexCount-1,mid=0;
	while(low<=high)
	{
		mid=(low+high)/2;
		if(indexArray[mid].key==result){recordAlreadyExists=1;return indexArray+mid;}
		if(result<indexArray[mid].key)high=mid-1;
		else
			low=mid+1;
	}
	recordAlreadyExists=0;
	return NULL;
}

int findStringSize(char*str)
{
	int size=0;

	while(*(str+size++)!='\0');
	return size;
}
int findSize(struct StudentRecord sr)
{
	int size=0;
	size+=findStringSize(sr.sid);
	size+=findStringSize(sr.firstName);
	size+=findStringSize(sr.lastName);
	size+=findStringSize(sr.major);
	//size= +findStringSize(sr.firstName)+findStringSize(sr.lastName)+findStringSize(sr.major);
	return size;
}

int addStudentRecord(struct StudentRecord sr)
{
	ofstream fp;
long offset2=useaList(sr.size+4+3);
if(offset2==-1)
{
fp.open("studentRecord" ,ios::out | ios::app | ios::binary);
}
else
{
	fp.open("studentRecord" ,ios::out | ios::in | ios::binary);
	fp.seekp(0,END);
	globalOffset=fp.tellp();
	fp.seekp(offset2,BEGIN);

	}

int size=sr.size+3;
long tempOff;
fp.write( (char *) &size, sizeof( sr.size ) );
//fp.write_raw(&delim,1);
fp.write(sr.sid,sr.sid.len());
fp.write(&delim,1);
fp.write( sr.firstName,sr.firstName.len() );
fp.write(&delim,1);
fp.write( sr.lastName, sr.lastName.len() );
fp.write(&delim,1);
fp.write( sr.major, sr.major.len() );
if(offset2==-1)
{
	
	tempOff=globalOffset;
	globalOffset=fp.tellp();
}
else
{
	tempOff=offset2;
	globalOffset;
	//globalOffset=fp.tellp();
}
fp.close();

return tempOff;
	
	// fp.close();
}
struct StudentRecord readFromStudentRecordFile(int offset)
{
	char buffer[ 1024 ];
int count=0;


StudentRecord sr;
sr.sid=0;
int i=0,j=2;

ifstream fp;
//ifstream::pos_type size;
//    char * memblock;
//
//    ifstream file ("studentRecord",ios::in|ios::binary|ios::ate);
//    if (file.is_open())
//    {
//       // size = file.tellg();
//        memblock = new char [6];
//        file.seekg (0, ios::beg);
//        file.read (memblock, 6);
//        file.close();
//
//        cout<< memblock<<endl;
//
//        delete[] memblock;
//    }
//    else
//    {
//        cout << "no ok"<<endl;
//    }
fp.open("studentRecord" ,ios::out | ios::in | ios::binary);
//fp.seek( offset, BEGIN );
fp.seekg(offset,ios::beg);
fp.read( (char *)&count, sizeof( int ) );
sr.size=count-3;
fp.read( buffer, count);
string buff=buffer;
cout<<buff.substr(0,count)<<endl;
writeToResultsFile(buff.substr(0,count));
//string str[4];
//
//j=0;
//for(i=0;i<3;i++)
//{
//	string temp;
//	while(buffer[j]!='|')
//		temp+=buffer[j++];
//	j++;
//	str[i]=temp;
//}
//
//	
//	while(j<count)
//		str[i]+=buffer[j++];
//	
//	/*sr.sid=str[0];
//	sr.firstName=str[1];
//	sr.lastName=str[2];
//	sr.major=str[3];*/



fp.close();

return sr;
}



void writeToResultsFile(char*str)
{
	ofstream outfile;

  outfile.open("resultsFile.txt", std::ios_base::app);
  outfile << str<<endl; 
  outfile.close();
}

void deleteFromIndex(struct index* ix)
{
	
	
	
	ix->key=indexArray[indexCount-1].key+1;
	ix->size=-1;
	sort();
	indexCount--;
}
void freeMemory()
{
	free(indexArray);
	struct availabilityList* temp;
	while(first!=NULL)
	{
		temp=first;
		first=first->rlink;
		free(temp);
	}
}
void readInputFile()
{
	MyString str,buffer[2],srComponents[4],sid;
	struct StudentRecord sr;

	
	ifstream inFile("InputFile.txt");
	//sr.size=0;
	while(inFile>>str)
	{
		sr.size=0;
		str.token(buffer,str.len()," ");
		if(buffer[0]=="add"){
			cmd=ADD;
		buffer[1].token(srComponents,buffer[1].len(),"|");
		sr.sid=srComponents[0];
		sr.firstName=srComponents[1];
		sr.lastName=srComponents[2];
		sr.major=srComponents[3];
		sr.size=sr.sid.len()+sr.firstName.len()+sr.lastName.len()+sr.major.len();
		}
		else if(buffer[0]=="find")
		{
			sr.sid=buffer[1];
			cmd=FIND;
		}
		else if(buffer[0]=="del")
		{
			sr.sid=buffer[1];
			cmd=DELETE;
		}
		else if(buffer[0]=="end")
		{
			
			cmd=ENDCMD;
		}
		switch(cmd)
		{
		case ADD:{
			        searchIndexArray(sr);
					if(!recordAlreadyExists)
					{
						indexCount++;
						int off=addStudentRecord(sr);
						
						struct index ind;
						string sid=sr.sid;
					    ind.key=stoi(sid);
						ind.offset=off;
						ind.size=sr.size+4+3;
						addToIndexArray(ind);
						//readFromStudentRecordFile();
					}
					else
					{
						cout<<"Recoed Already Exists"<<endl;
						writeToResultsFile("Recoed Already Exists");
					}
					break;
				 }
		case FIND:{
			        //sr.sid+="1";
			         struct index* ix=searchIndexArray(sr);
					if(recordAlreadyExists)
					{
						
						readFromStudentRecordFile(ix->offset);
												
					 }
					else
					{
						MyString id=sr.sid;
						MyString temp="No record with SID=";

						temp=temp+id+" exists";
						cout<<temp<<endl;
						writeToResultsFile(temp);

					}
			             break;
				  }
		case DELETE:  {
						 struct index* ix=searchIndexArray(sr);
						 if(recordAlreadyExists)
						 {
							 struct availability aInfo;
							 aInfo.offset=ix->offset;
							 aInfo.size=ix->size;
							 addToaList(aInfo);
							 deleteFromIndex(ix);
						 }
						 else
						 {
					    MyString id=sr.sid;
						MyString temp="No record with SID=";
						temp=temp+id+" exists";
						cout<<temp<<endl;
						writeToResultsFile(temp);

						 }
			             break;
					}
		case ENDCMD: {
				printIndexArray();
				printAvailabilityList();
				storeIndexInFile();
	            storeAvailabilityListinFile();
			    freeMemory();
			break;
			 }
		}



	}
}

void main()
{
	/*struct StudentRecord sr;
	sr.firstName="Karthik";
	sr.lastName="Guruprasad";
	sr.major="CS";
	sr.sid="123";
	sr.size=findSize(sr);*/

	readFromIndexFile();
	readFromAvailabilityListFile();
	readInputFile();
	//addStudentRecord(sr);
	//

	/*readFromAvailabilityListFile();
	struct availability aInfo;
	aInfo.offset=100;
	aInfo.size=200;
	addToaList(aInfo);
	printAvailabilityList();
	storeAvailabilityListinFile();
	
	sort();
	printIndexArray();
	storeIndexInFile();
	cout<<searchIndexArray(90);*/
	
	
	_getch();
	
	/*int size=0;
	char ch[]="jello";
	while(*(ch+size) !='\0')
		size++;
		
	printf("%d",sizeof(ch));
	_getch();*/
	
}

