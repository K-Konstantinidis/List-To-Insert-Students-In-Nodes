/*************************************************************************
        Copyright © 2021 Konstantinidis Konstantinos

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NilValue -1

typedef enum {
    FALSE, TRUE
} boolean;

typedef struct {
    int studentId; //Id of the student
    float grade;   //The grade the student got
} ListElementType;

typedef int ListPointer;

typedef struct {
    ListElementType Data; //Data inside the node
    ListPointer Next; //Shows the next node
} NodeType;

void InitializeStoragePool(NodeType Node[], ListPointer *FreePtr, int NumberOfNodes);
void CreateLList(ListPointer *List);
boolean EmptyLList(ListPointer List);
boolean FullLList(ListPointer FreePtr);
void GetNode(ListPointer *P, ListPointer *FreePtr, NodeType Node[]);
void ReleaseNode(NodeType Node[], ListPointer P, ListPointer *FreePtr);
void Insert(ListPointer *List, NodeType Node[],ListPointer *FreePtr, ListPointer PredPtr, ListElementType Item);
void Delete(ListPointer *List, NodeType Node[], ListPointer *FreePtr, ListPointer PredPtr);
void TraverseLinked(ListPointer List, NodeType Node[]);

main(){
    int students=0;
    ListPointer List, FreePtr, PredPtr;
    ListElementType Item;
    char WhatToDo;

    CreateLList(&List);

    while(students<=0){
        printf("Insert number of students: "); //Get the amount of the students that will be inserted afterwards as entries in the nodes
        scanf("%d",&students);
        if(students<=0)
            printf("Students cannot be less than 1\n"); //There should be at least 1 student
    }

    int NumberOfNodes = students; //Have as many nodes as students
    NodeType Node[NumberOfNodes];
    InitializeStoragePool(Node, &FreePtr, NumberOfNodes);

    int i=0;
    while(i<students){ //If all the students have been added then end
        if(i==0){ //If the list is empty then do not ask the user but make them insert someone
            Insert(&List, Node, &FreePtr, PredPtr, Item);
            TraverseLinked(List, Node);
            i++;
        }

        printf("Do you want to insert the next student or do you want to delete a wrong entry? (n->Next | d->Delete): ");
        scanf(" %c",&WhatToDo);
        while(WhatToDo!='n' && WhatToDo!='d'){
            printf("Wrong input.\nDo you want to insert the next student or do you want to delete a wrong entry? (n->Next | d->Delete): ");
            scanf(" %c",&WhatToDo);
        }

        if(WhatToDo=='n'){
            Insert(&List, Node, &FreePtr, PredPtr, Item);
            TraverseLinked(List, Node);
            i++; //+1 inserted students
        }
        else{
            Delete(&List, Node, &FreePtr, PredPtr);
            TraverseLinked(List, Node);
            i--; //-1 inserted students as someone got deleted
        }
    }

    system("PAUSE");
}

//A function to create a list
void CreateLList(ListPointer *List){*List=NilValue;}

//A function that creates our nodes and makes a pointer point to the 1st one
void InitializeStoragePool(NodeType Node[], ListPointer *FreePtr, int NumberOfNodes){
    int i;

    for(i=0; i<NumberOfNodes-1; i++){
        Node[i].Next = i+1;
        Node[i].Data.studentId = -1;
        Node[i].Data.grade = -1;
    }
    Node[NumberOfNodes-1].Next = NilValue;
    Node[NumberOfNodes-1].Data.studentId = -1;
    Node[NumberOfNodes-1].Data.grade = -1;

    *FreePtr=0;
}

//A function to check if a list is empty
boolean EmptyLList(ListPointer List){return (List==NilValue);}

//A function to check if a list is full
boolean FullLList(ListPointer FreePtr){return (FreePtr == NilValue);}

//A function to insert a student in a node in the list
void Insert(ListPointer *List, NodeType Node[], ListPointer *FreePtr, ListPointer PredPtr, ListElementType Item){
    ListPointer TempPtr;

    printf("Insert identification number to add in the list: "); //Get the id
    scanf("%d",&Item.studentId);

    printf("Insert grade to add in the list: "); //And their grade
    scanf("%fl",&Item.grade);

    printf("Insert the position after which the entry will be inserted: ");//e.g. If your input is 2 then the entry will be inserted after the one with the number 2 (NOT IN THE 2nd PLACE)
    scanf("%d",&PredPtr);

    GetNode(&TempPtr,FreePtr,Node); //Get an empty node to insert the student
    if(!FullLList(TempPtr)) { //If the list is not full
        if(PredPtr==NilValue){ //If we want to add the student in the 1st place of the list then the position the user types above in the 'scanf("%d",&PredPtr);' must be -1
            Node[TempPtr].Data = Item;
            Node[TempPtr].Next = *List;
            *List = TempPtr;
        }
        else{ //Else insert them in the correct position
            Node[TempPtr].Data = Item;
            Node[TempPtr].Next = Node[PredPtr].Next;
            Node[PredPtr].Next = TempPtr;
        }
    }
  else
    printf("The list is full... You cannot insert another student\n");
}

void Delete(ListPointer *List, NodeType Node[], ListPointer *FreePtr, ListPointer PredPtr){
    ListPointer TempPtr ;

    printf("Insert the position before the one that will be deleted: ");//e.g. If your input is 2 then the entry that is after the one with the number 2 will be deleted
    scanf("%d", &PredPtr);

    if(!EmptyLList(*List)) //If the list is not empty
        if (PredPtr == NilValue){ //If we want to delete the 1st position then the user must type -1 above in the 'scanf("%d", &PredPtr);'
            TempPtr = *List;
            *List = Node[TempPtr].Next;
            ReleaseNode(Node,TempPtr,FreePtr);
        }
        else{
            if(Node[PredPtr].Next == NilValue) //If there is not a node after this one but the next position is -1
                printf("\nThere is not an entry after this position to delete\n");
            else{
                TempPtr = Node[PredPtr].Next;
                Node[PredPtr].Next = Node[TempPtr].Next;
                ReleaseNode(Node,TempPtr,FreePtr);
            }
        }
    else
        printf("The list is empty ...\n");
}

//A function that finds an empty node
void GetNode(ListPointer *P, ListPointer *FreePtr, NodeType Node[]){
    *P = *FreePtr;
    if(!FullLList(*FreePtr))
        *FreePtr =Node[*FreePtr].Next;
}

//A function that finds a node and then deletes it
void ReleaseNode(NodeType Node[], ListPointer P, ListPointer *FreePtr){
    Node[P].Next = *FreePtr;
    Node[P].Data.studentId = -1;
    Node[P].Data.grade = -1;

    *FreePtr = P;
}

//A function that displays the list of nodes
void TraverseLinked(ListPointer List, NodeType Node[]){
    ListPointer CurrPtr;
    int count = 0;

    if (!EmptyLList(List)){
        CurrPtr = List;

        while (CurrPtr != NilValue){
            count++;
            CurrPtr = Node[CurrPtr].Next;
        }
        printf("\n");
        printf("Number of data in the list: %d\n", count);
        CurrPtr = List;

        while(CurrPtr != NilValue){
            printf("[%d: (%d , %g) -> %d] \n", CurrPtr, Node[CurrPtr].Data.studentId, Node[CurrPtr].Data.grade, Node[CurrPtr].Next);
            CurrPtr = Node[CurrPtr].Next;
        }
        printf("\n");
    }
    else
        printf("Empty List ...\n");
}
