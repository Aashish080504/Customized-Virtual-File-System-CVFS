//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cvfs.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Global variable or objects used in the project
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOTBLOCK bootobj;
SUPERBLOCK superobj;
UAREA uareaobj;

PINODE head = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitializeUAREA
//  Description :       It is used to initialize UAREA members
//  Author :            Sahil Rajaram Thorat
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitializeUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UAREA gets initialized successfully\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitializeSuperBlock
//  Description :       It is used to initialize Super block members
//  Author :            Sahil Rajaram Thorat
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitializeSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super block gets initialized successfully\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linkedlist of Inodes
//  Author :            Sahil Rajaram Thorat
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName, "\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(head == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB created successfully\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitialization
//  Description :       It is used to all such functions which are used to initialize auxyilary data.
//  Author :            Sahil Rajaram Thorat
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialization()
{
    strcpy(bootobj.Information, "Booting process of Marvellous CVFS is done");

    printf("%s\n", bootobj.Information);

    InitializeSuperBlock();

    CreateDILB();

    InitializeUAREA();

    printf("Marvellous CVFS : Auxillary data initialized successfully\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is used to diplay the help page.
//  Author :            Sahil Rajaram Thorat
//  Date :              14/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("\n-----------------------------------------------------------------------------\n");
    printf("------------------------- Marvellous CVFS Help Page -------------------------\n");
    printf("-----------------------------------------------------------------------------\n");

    printf("man    : It is used to display manual page\n");
    printf("clear  : It is used to clear the terminal\n");
    printf("creat  : It is used to create new file\n");
    printf("write  : It is used to write the data into the file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("stat   : It is used to display statistical information\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate Marvellous CVFS\n");

    printf("-----------------------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is used to diplay Man page.
//  Author :            Sahil Rajaram Thorat
//  Date :              14/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    char path[100];
    char ch;
    FILE *fp = NULL;

    sprintf(path, "../man/%s.txt", Name);

    fp = fopen(path, "r");

    if(fp == NULL)
    {
        printf("No manual entry for %s\n", Name);
        return;
    }

    while((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
    }

    fclose(fp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description   :     It is used to check whether file is already exist or not.
//  Input         :     It accepts file name
//  Output        :     It returns true or false
//  Author        :     Sahil Rajaram Thorat
//  Date          :     16/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(char *name)
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name, temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }

    return bFlag;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description   :     It is used to crate new regular file.
//  Input         :     It accepts file name and permissions
//  Output        :     It returns the file descriptor
//  Author        :     Sahil Rajaram Thorat
//  Date          :     16/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(char *name, int permission)
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n", superobj.FreeInodes);

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        temp = temp->next;
    }

    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    uareaobj.UFDT[i]->ptrinode = temp;

    strcpy(temp->FileName, name);
    temp->FileSize = MAXFILESIZE;
    temp->ActualFileSize = 0;
    temp->FileType = REGULARFILE;
    temp->ReferenceCount = 1;
    temp->Permission = permission;

    temp->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile
//  Description   :     It is used to list all files.
//  Input         :     Nothing
//  Output        :     Nothing
//  Author        :     Sahil Rajaram Thorat
//  Date          :     16/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("\n-----------------------------------------------------------------------------\n");
    printf("---------------------- Marvellous CVFS Files Information --------------------\n");
    printf("-----------------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%d\t%s\t%d\n",
            temp->InodeNumber,
            temp->FileName,
            temp->ActualFileSize);
        }

        temp = temp->next;
    }

    printf("\n-----------------------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description   :     It is used to delete the file.
//  Input         :     File Name
//  Output        :     Nothing
//  Author        :     Sahil Rajaram Thorat
//  Date          :     22/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int UnlinkFile(char *name)
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                free(uareaobj.UFDT[i]->ptrinode->Buffer);

                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName,
                '\0',
                sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                free(uareaobj.UFDT[i]);

                uareaobj.UFDT[i] = NULL;

                superobj.FreeInodes++;

                break;
            }
        }
    }

    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description   :     It is used to write the data into the file.
//  Input         :     File descriptor
//                      Address of Buffer which contains data
//                      Size of data that we want to write
//  Output        :     Number of bytes successfully written
//  Author        :     Sahil Rajaram Thorat
//  Date          :     22/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int WriteFile(int fd, char *data, int size)
{
    if((fd < 0) || (fd >= MAXOPENFILES))
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    strncpy(
        uareaobj.UFDT[fd]->ptrinode->Buffer +
        uareaobj.UFDT[fd]->WriteOffset,
        data,
        size
    );

    uareaobj.UFDT[fd]->WriteOffset += size;

    uareaobj.UFDT[fd]->ptrinode->ActualFileSize += size;

    return size;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description   :     It is used to read the data from the file.
//  Input         :     File descriptor
//                      Address of empty Buffer
//                      Size of data that we want to read
//  Output        :     Number of bytes successfully read
//  Author        :     Sahil Rajaram Thorat
//  Date          :     22/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReadFile(int fd, char *data, int size)
{
    if((fd < 0) || (fd >= MAXOPENFILES))
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize -
        uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    strncpy(
        data,
        uareaobj.UFDT[fd]->ptrinode->Buffer +
        uareaobj.UFDT[fd]->ReadOffset,
        size
    );

    uareaobj.UFDT[fd]->ReadOffset += size;

    return size;
}