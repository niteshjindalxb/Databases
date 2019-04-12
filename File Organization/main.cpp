#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "heapFile.h"

using namespace std;

int main()
{
    int pageSize = 100;

    HeapFile d(pageSize);
    bool create_diskFile = false;
    bool inserted_records = false;
    while(true){
        int x;
        printf("\nEnter :\n 1 : Create New HeapFile\n 2 : Insert Record\n 3 : Delete Record\n 4 : Display HeapFile Structure");
        printf("\n 5 : Defragment a data page\n 6 : Defragment the whole heap file\n -1 : Exit\n");
        printf("Choice : ");
        cin >> x;
        switch (x)
        {
            case 1: {
                        if(create_diskFile == false){
                            int pageSize;
                            printf("Enter size of DataPages : ");
                            cin >> pageSize;
                            printf("Enter number of DataPages to create : ");
                            int n;
                            cin >> n;

                            d = HeapFile(n, pageSize);
                            create_diskFile = true;
                        }
                        else
                            printf("HeapFile has already been created.\n");
                    }
                    break;

            case 2: {
                        if(create_diskFile){
                            printf("Enter Record identifier in int to insert into Pages : ");
                            int id;
                            cin >> id;
                            printf("Enter Record length in bytes : ");
                            int l;
                            cin >> l;
                            int pageId = d.insertRecord(id, l);
                            if (pageId > 0)
                                cout << "Record inserted in Page : " << pageId << endl;
                        }
                        else
                            printf("You can not enter Records into Pages without creating the initial HeapFile.\n");
                    }
                    break;

            case 3: {   
                        if(create_diskFile){
                            printf("Enter Record identifier in int to delete from Pages : ");
                            int id;
                            cin >> id;
                            int pageId = d.deleteRecord(id);
                            if (pageId > 0)
                                cout << "Record deleted from Page : " << pageId << endl;
                            else
                                cout << "Record is not found." << endl;
                        }
                        else
                            printf("You can not delete Records without creating the initial HeapFile or without inserting few Records into it.\n");
                    }
                    break;

            case 4: {
                       if(create_diskFile){
                            d.print();
                        }
                        else
                            printf("You can not view Pages & Records without creating the initial HeapFile.\n");
                    }
                    break;

            case 5: {
                       if(create_diskFile){
                            cout << "Enter Page number to fragment : ";
                            int pageId;
                            cin >> pageId;
                            d.defragmentPage(pageId);
                        }
                        else
                            printf("You can not view Pages & Records without creating the initial HeapFile.\n");
                    }
                    break;

            case 6: {
                       if(create_diskFile){
                            d.defragmentHeap();
                        }
                        else
                            printf("You can not view Pages & Records without creating the initial HeapFile.\n");
                    }
                    break;

            case -1: exit(0);

            default: printf("Choice other than 1, 2, 3, 4 and -1\n");
                    continue;
        }
    }
    return 0;
}