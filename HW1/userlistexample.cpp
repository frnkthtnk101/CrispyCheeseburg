/*
 *userlistexample.cpp
 *a way to prove the end of question 1
 *Franco Pettigrosso
 */
#include<bits/stdc++.h>

using namespace std;
// a struct for a list of nodes
struct Node{
	int data;
	struct Node *next;
};
/*
 * addToEmpty
 * adds an element to an empty linked list 
 */
struct Node *addToEmpty(struct Node *last, int data){
	if(last != NULL)
		return last;
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));	
	temp->data=data;
	last = temp;
	last -> next = last;
	return last;
}
/*
 * addToEmpty
 * adds an element to a linked list at the end
 */
struct Node *addEnd(struct Node *last, int data){
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	temp -> data = data;
	temp -> next = last -> next;
	last -> next = temp;
	last = temp;
	return last;
}

/*
 *traverse
 *goes through a linked list
 */
void traverse(struct Node *last){
	struct Node *p;
	if (last == NULL){
		cout << "List is Empty" <<endl;
		return;
	}
	p = last -> next;
	do{
	 cout << p->data<<" ";
	 p = p->next;
	}while(p != last->next);
	return;
}
/*
 *main
 * the main function for this program. makes a list, displays infor, and deletels it
 */
int main()
{
	struct Node *last = NULL;
	char a;
	int i;
	last = addToEmpty(last, -1);
	for (i = 0; i < 5; i++)
		last= addEnd(last, i);
	traverse(last);
	cin >> a;
	return 0;
}
