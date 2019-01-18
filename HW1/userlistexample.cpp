#include<bits/stdc++.h>

using namespace std;

struct Node{
	int data;
	struct Node *next;
};

struct Node *addToEmpty(struct Node *last, int data){
	if(last != NULL)
		return last;
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));	
	temp->data=data;
	last = temp;
	last -> next = last;
	return last;
}

struct Node *addEnd(struct Node *last, int data){
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	temp -> data = data;
	temp -> next = last -> next;
	last -> next = temp;
	last = temp;
	return last;
}

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
