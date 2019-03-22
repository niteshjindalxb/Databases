#include "modified_extendibleHashing.h"
#include <iostream>
using namespace std;

int main(){
	
	int choice;
	directory *d = NULL;
	do {
		cout<<"1. Create new ExtendibleHashTable"<<endl;
		cout<<"2. Add an element to the HashTable"<<endl;
		cout<<"3. Search an element to the HashTable"<<endl;
		cout<<"4. Delete an element from the HashTable"<<endl;
		cout<<"5. Print HashTable"<<endl;

		cout<<"Any other choice to exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		cout<<"You entered "<<choice<<endl;
		switch(choice){
			case 1:
				int globalDepth;
				cout<<"Enter global depth: ";
				cin>>globalDepth;
				int bucketSize;
				cout<<"Enter number of entries in bucket: ";
				cin>>bucketSize;

                if(d != NULL)
					delete d;
				d = new directory(globalDepth, bucketSize);
				break;

			case 2:
                if (d == NULL) {
                    cout << "You've not created Extendible Hash Table\n";
                    break;
                }
				int add;
				cout<<"Enter element to add: ";
				cin>>add;
				d->add_value(add);
				break;

			case 3:
                if (d == NULL) {
                    cout << "You've not created Extendible Hash Table\n";
                    break;
                }
				int search;
				cout<<"Enter element to search: ";
				cin>>search;
				if(d->search(search))
					cout<<"Element is found"<<endl;
				else
					cout<<"Element is not found"<<endl;
				break;
			case 4:
                if (d == NULL) {
                    cout << "You've not created Extendible Hash Table\n";
                    break;
                }
				int del;
				cout<<"Enter element to delete: ";
				cin>>del;
				if(d->deleteElement(del))
					cout<<"Element is deleted"<<endl;
				else
					cout<<"Element is not found"<<endl;
				break;
			case 5:
                if (d == NULL) {
                    cout << "You've not created Extendible Hash Table\n";
                    break;
                }
				d->print();
				break;
			default:
				return 0;
		}
	} while(true);
}