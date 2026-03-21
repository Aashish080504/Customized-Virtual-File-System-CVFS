//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cvfs.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Entry point function of the project
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitialization();

    printf("\n-----------------------------------------------------------------------------\n");
    printf("--------------------- Marvellous CVFS started sucessfully -------------------\n");
    printf("-----------------------------------------------------------------------------\n");

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  Infinite Listening Shell
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("\nMarvellous CVFS : > ");

        fgets(str, sizeof(str), stdin);

        iCount = sscanf(
                    str,
                    "%s %s %s %s %s",
                    Command[0],
                    Command[1],
                    Command[2],
                    Command[3],
                    Command[4]
                );

        fflush(stdin);

        //////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Single command
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////

        if(iCount == 1)
        {
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the allocated resources\n");
                break;
            }

            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }

            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }

            else if(strcmp("clear",Command[0]) == 0)
            {
                #ifdef _WIN32
                                system("cls");
                #else
                                system("clear");
                #endif
            }

            else
            {
                printf("Command not found\n");
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Two commands
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////

        else if(iCount == 2)
        {
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }

            else if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("ERROR : File does not exist\n");
                }
                else
                {
                    printf("File successfully deleted\n");
                }
            }

            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter data : \n");

                fgets(InputBuffer, MAXFILESIZE, stdin);

                iRet = WriteFile(
                        atoi(Command[1]),
                        InputBuffer,
                        strlen(InputBuffer)
                      );

                if(iRet < 0)
                {
                    printf("ERROR : Unable to write file\n");
                }
                else
                {
                    printf("%d bytes successfully written\n", iRet);
                }
            }

            else
            {
                printf("Command not found\n");
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Three commands
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////

        else if(iCount == 3)
        {
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(
                        Command[1],
                        atoi(Command[2])
                      );

                if(iRet >= 0)
                {
                    printf("File successfully created with FD : %d\n", iRet);
                }
                else
                {
                    printf("ERROR : Unable to create file\n");
                }
            }

            else if(strcmp("read",Command[0]) == 0)
            {
                EmptyBuffer = (char*)malloc(atoi(Command[2]) + 1);

                if(EmptyBuffer == NULL)
                {
                    printf("Memory allocation failed\n");
                    continue;
                }

                memset(EmptyBuffer, 0, atoi(Command[2]) + 1);

                iRet = ReadFile(
                        atoi(Command[1]),
                        EmptyBuffer,
                        atoi(Command[2])
                      );

                if(iRet < 0)
                {
                    printf("ERROR : Unable to read file\n");
                }
                else
                {
                    printf("Data : %s\n", EmptyBuffer);
                }

                free(EmptyBuffer);
            }

            else
            {
                printf("Command not found\n");
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Invalid command
        //
        //////////////////////////////////////////////////////////////////////////////////////////////////

        else
        {
            printf("Command not found\n");
            printf("Use help command\n");
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  End of program
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}