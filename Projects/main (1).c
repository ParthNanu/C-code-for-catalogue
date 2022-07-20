#include<stdio.h>
#include<stdbool.h>		// true and false
#include<stdlib.h>
#include<string.h>
#include<conio.h>

#define MAX_NAME_SIZE 50
#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_USER_PASSWORD 20
#define FILE_PATH  "studentRecordSystem.bin"
#define MAX_YR  9999
#define MIN_YR  1900




struct date
{
	int yyyy;
	int mm;
	int dd;
};
struct student
{
	
	char student_name[MAX_NAME_SIZE];
	char father_name[MAX_NAME_SIZE];
	char surname[MAX_NAME_SIZE];
	int student_id;
	struct date studentAdmDate;
};
struct credential
{
	char username[MAX_SIZE_USER_NAME];
	char password[MAX_SIZE_USER_PASSWORD];
};
//struct date obj_date;
//struct student obj_student;
//struct credential obj_credential;
void init();
int isFilePresent(const char *filepath);
bool login();
void addStudentInfo();
bool isValidData(const char *data);
int isCharacter(const char ch);
void welcomeMessage();
int isValidDate(struct date *isDate);
int isLeapyear(int year);
void displayAllStudentInfo();
void searchStudentInfo();
void deleteStudentInfo();
void updateUserNamePassword();
int main()
{
	int sel=0;
	welcomeMessage();
	init();
	if(!login())
	{
		printf("Login faild\n\n");
		exit(1);
	}
	do
	{
		printf("\n\t\t1.Add Student");
        printf("\n\t\t2.Display all Student Data");
        printf("\n\t\t3.Search specific Student information");
        printf("\n\t\t4.Delete Student Information ");
        printf("\n\t\t5.Update Password");
        printf("\n\t\t0.Exit");
		printf("\n\n\n\t\tEnter choice => ");
		scanf("%d",&sel);	
		switch(sel)
		{
			case 1:
			{
				addStudentInfo();
				break;
			}
			case 2:
			{
				displayAllStudentInfo();
				break;
			}
			case 3:
			{
				searchStudentInfo();
				break;
			}
			case 4:
			{
				deleteStudentInfo();
				break;
			}
			case 5:
			{
				updateUserNamePassword();
				break;
			}
			case 0:
			{
				printf("\n\n\n\t\tThank you!!!\n\n\n\n\n");
				exit(1);
				break;
			}
			default:
			{
				printf("\n\n\n\t\tINVALID INPUT!!! Try again...");
				break;
			}
        }    
	}while(sel !=0);
	
	
	return 0;
	
}
void updateUserNamePassword()
{
	struct credential obj_credential= {0};
	unsigned char defaultUsername[MAX_SIZE_USER_NAME] ={'\n'};
    unsigned char defaultPassword[MAX_SIZE_USER_PASSWORD] ={'\n'};
	
	FILE *fp=NULL;
	fp = fopen(FILE_PATH,"rb+");
	if(fp == NULL)
	{
		printf("File is not opened\n");
        exit(1);
	}
	fread(&obj_credential,sizeof(obj_credential),1,fp);
	if (fseek(fp,0,SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while updating password\n");
        exit(1);
    }
	printf("\n\n\t\tNew Username:");
    fflush(stdin);
    fgets(defaultUsername,MAX_NAME_SIZE,stdin);
	
	printf("\n\n\t\tNew Password:");
    fflush(stdin);
    fgets(defaultPassword,MAX_SIZE_USER_PASSWORD,stdin);
	
	strncpy(obj_credential.username,defaultUsername,sizeof(defaultUsername));
	strncpy(obj_credential.password,defaultPassword,sizeof(defaultPassword));
	
	fwrite(&obj_credential,sizeof(obj_credential), 1, fp);
    fclose(fp);
	printf("\n\t\tYour Password has been changed successfully");
    printf("\n\t\ttLogin Again:");
    fflush(stdin);
    getchar();
    exit(1);
	
}
void deleteStudentInfo()
{
	int studentID=0;
	int isDataPresent=0;
	FILE *fp=NULL;
	FILE *fpTmp=NULL;
	struct student obj_student;
	struct credential obj_credential;
	
	fp=fopen(FILE_PATH,"rb");
	if(fp == NULL)
	{
		printf("File is not opened\n");
        exit(1);
	}
	fpTmp=fopen("tmp.bin","wb");
	if(fpTmp == NULL)
	{
		fclose(fp);
		printf("File is not opened\n");
        exit(1);
	}
	fread(&obj_credential,sizeof(obj_credential),1,fp);
	fwrite(&obj_credential,sizeof(obj_credential),1,fpTmp);
	printf("\n\t\tEnter Student ID NO. for delete:");
    scanf("%d",&studentID);
	printf("\n\n\n%d\n\n",studentID);
	while(fread(&obj_student,sizeof(obj_student),1,fp))
	{
		if(studentID != obj_student.student_id)
		{
			printf("obj_student.student_id:%d\n",obj_student.student_id);
			fwrite(&obj_student,sizeof(obj_student),1,fpTmp);
		}
		else
		{
			isDataPresent=1;
		}
	}
	(isDataPresent)? printf("\n\t\tRecord deleted successfully....."):printf("\n\t\tRecord not found");
	fclose(fp);
    fclose(fpTmp);
    remove(FILE_PATH);
    rename("tmp.bin",FILE_PATH);
	
}
void searchStudentInfo()
{
	int isDataPresent=0;
	int studentId=0;
	struct student obj_student;
	FILE *fp=NULL;
	fp = fopen(FILE_PATH,"rb");
	if(fp == NULL)
	{
		printf("File is not opened\n");
        exit(1);
	}
	if(fseek(fp,sizeof(struct credential),SEEK_SET))
	{
		fclose(fp);
        printf("Facing issue while reading file\n");
        exit(1);
	}
	printf("\n\n\t\tEnter Student ID NO to search:");
    fflush(stdin);
    scanf("%u",&studentId);
	while(fread(&obj_student,sizeof(obj_student),1,fp))
	{
		if(studentId == obj_student.student_id)
		{
			isDataPresent=1;
			printf("\t\tStudent id = %u\n",obj_student.student_id);
			printf("\t\tStudent Name = %s",obj_student.student_name);
			printf("\t\tFather Name = %s",obj_student.father_name);
			printf("\t\tSurname = %s",obj_student.surname);
			printf("\t\tStudent Admission Date(day/month/year) =  (%d/%d/%d)\n\n",obj_student.studentAdmDate.dd,
					obj_student.studentAdmDate.mm,obj_student.studentAdmDate.yyyy);
			break;
		}
		
	}
	if(!isDataPresent)
	{
		printf("\n\t\tNo Record present.");
	}
	fclose(fp);
	printf("\n\n\t\tPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
	
}
void displayAllStudentInfo()
{
	int isDataPresent=0;
	struct student obj_student;
	FILE *fp=NULL;
	unsigned int studentCount = 1;
	fp = fopen(FILE_PATH,"rb");
	if(fp == NULL)
	{
		printf("File is not opened\n");
        exit(1);
	}
	if(fseek(fp,sizeof(struct credential),SEEK_SET))
	{
		fclose(fp);
        printf("Facing issue while reading file\n");
        exit(1);
	}
	//Read file
	printf("\n\t\tStudent Information.\n\n");
	while(fread(&obj_student,sizeof(obj_student),1,fp))
	{
		printf("\n\t\tStudent = %d\n",studentCount);
		printf("\t\tStudent id = %u\n",obj_student.student_id);
		printf("\t\tStudent Name = %s",obj_student.student_name);
		printf("\t\tFather Name = %s",obj_student.father_name);
		printf("\t\tSurname = %s",obj_student.surname);
		printf("\t\tStudent Admission Date(day/month/year) =  (%d/%d/%d)\n\n",obj_student.studentAdmDate.dd,
					obj_student.studentAdmDate.mm,obj_student.studentAdmDate.yyyy);
		isDataPresent=1;
		studentCount++;
		
	}
	if(!isDataPresent)
	{
		printf("\n\t\tNo Record present.");
	}
	fclose(fp);
	printf("\n\n\t\tPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
		
}
void addStudentInfo()
{
	struct student obj_student;
	FILE *fp=NULL;
	int flag=0;
	fp = fopen(FILE_PATH,"ab+");
	if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
	printf("\n\n\t\tENTER YOUR DETAILS BELOW:");
    printf("\n\t\t----------------------------------\n");
    printf("\n\t\tStudent ID  = ");
    fflush(stdin);
    scanf("%u",&obj_student.student_id);
	do
	{
		printf("\n\t\tStudent Name  = ");
		fflush(stdin);
		fgets(obj_student.student_name,MAX_NAME_SIZE,stdin);
		flag = isValidData(obj_student.student_name);
		if(!flag)
		{
			printf("\n\t\tName contain invalid character. Please enter again.");
		}
	}while(!flag);
	do
	{
		printf("\n\t\tFather Name  = ");
		fflush(stdin);
		fgets(obj_student.father_name,MAX_NAME_SIZE,stdin);
		flag = isValidData(obj_student.father_name);
		if(!flag)
		{
			printf("\n\t\tName contain invalid character. Please enter again.");
		}
	}while(!flag);
	do
	{
		printf("\n\t\tSurname  = ");
		fflush(stdin);
		fgets(obj_student.surname,MAX_NAME_SIZE,stdin);
		flag = isValidData(obj_student.surname);
		if(!flag)
		{
			printf("\n\t\tName contain invalid character. Please enter again.");
		}
	}while(!flag);	
	do
	{
		printf("\n\t\tEnter Date in format (day/month/year): = ");
		scanf("%d%d%d",&obj_student.studentAdmDate.dd,&obj_student.studentAdmDate.mm,&obj_student.studentAdmDate.yyyy);
		flag= isValidDate(&obj_student.studentAdmDate);
	}while(!flag);
	fwrite(&obj_student,sizeof(obj_student),1,fp);
	fclose(fp);
	
}
int isValidDate(struct date *isDate)
{
	if(isDate->yyyy > MAX_YR || isDate->yyyy < MIN_YR)	
	{
		return 0;
	}
	if (isDate->mm < 1 || isDate->mm > 12)
	{
        return 0;
	}
	if (isDate->dd < 1 || isDate->dd > 31)
	{
        return 0;
	}
	 //Handle feb days in leap year
    if (isDate->mm == 2)
    {
        if (isLeapyear(isDate->yyyy))
            return (isDate->dd <= 29);
        else
            return (isDate->dd <= 28);
    }
	//handle months which has only 30 days
    if (isDate->mm == 4 || isDate->mm == 6 || isDate->mm == 9 || isDate->mm == 11)
	{
        return (isDate->dd <= 30);
	}
    return 1;
}
int isLeapyear(int year)
{
	return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}
bool isValidData(const char *data)
{
	
	int dataLen = 0;
    int dataIndex = 0;
	dataLen=strlen(data);
	
	for(dataIndex=0;dataIndex<dataLen;dataIndex++)
	{
		if((isCharacter(data[dataIndex])) && (data[dataIndex] != '\n') && (data[dataIndex] != ' '))
		{
			return true;
		}
	}
	return false;
}
int isCharacter(const char ch)
{
	if(((ch >= 'a') && (ch <='z')) || ((ch >= 'A') && (ch <='Z')))
	{
		return 1;
	}
	return 0;
}
bool login()
{
	unsigned char defaultUsername[MAX_SIZE_USER_NAME]={0};
	unsigned char defaultPassword[MAX_SIZE_USER_PASSWORD]={0};
	struct credential obj_credential;
	int attempt=0;
	
	FILE *fp = NULL;
	fp = fopen(FILE_PATH,"rb");
	if(fp == NULL)
	{
		printf("Error: File not open\n");
		exit(1);
	}
	else
	{
		fread(&obj_credential,sizeof(obj_credential),1,fp);
		fclose(fp);
	}
	do
	{
		printf("\n\n\t\tEnter Username : ");
		fgets(defaultUsername,sizeof(defaultUsername),stdin);
		printf("\t\tEnter Password : ");
		fgets(defaultPassword,sizeof(defaultPassword),stdin);
		
		if((!(strcmp(defaultUsername,obj_credential.username))) && (!(strcmp(defaultPassword,obj_credential.password))))
		{
			printf("\n\t\tPassword Match\n\n");
			break;
		}
		else
		{
			printf("\n\t\tYou have enter wrong password please reenter username and password.\n");
			attempt++;
		}
	}while(attempt<3);
	if(attempt == 3)
	{
		
		return false;
	}
	return true;
}

void init()
{
	FILE *fp=NULL;
	int status=0;
	const char defaultUsername[] ="admin\n";
    const char defaultPassword[] ="admin123\n";
	struct credential obj_credential;
	status=isFilePresent(FILE_PATH);
	if(!(status))
	{
		fp = fopen(FILE_PATH,"wb");
		if(fp != NULL)
		{
			strncpy(obj_credential.username,defaultUsername,sizeof(defaultUsername));
			strncpy(obj_credential.password,defaultPassword,sizeof(defaultPassword));
			fwrite(&obj_credential,sizeof(obj_credential),1,fp);
			fclose(fp);
		}
	}
	
}
//Check file exist or not
int isFilePresent(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    int status = 0;
   
    if (fp != NULL)
    {
        status = 1;
        fclose(fp);
    }
    return status;
}

//Display message
void welcomeMessage()
{
    printf("\n\n\n");
    printf("\n\t\t  * * * * * * * * * * * * * * * * * * * * * * * * * * *\n\n");
    //printf("\n\t\t        * * * * * * * * * * * * * * * * * * * * * * *");
    printf("\n\t\t                           WELCOME                   ");
    printf("\n\t\t                             TO                      ");
    printf("\n\t\t                        Student MANAGEMENT               ");
    printf("\n\t\t                            SYSTEM                   ");
   // printf("\n\t\t        * * * * * * * * * * * * * * * * * * * * * * *");
    printf("\n\n\t\t  * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("\n\n\n\t\t Enter any key to continue.....");
    getch();
}