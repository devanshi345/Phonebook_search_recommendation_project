#include<bits/stdc++.h>
#include<fstream>
using namespace std;

// "Doubly linked list" of each node having names possessing proper prefix.
// linked list implementation of priority queue.
struct node_list
{
	string name,mobileNo;
	struct node_list* right;
	struct node_list* left;
	int freq;
};

//each node of tree has 26 child nodes(a,b...,y,z)
struct node
{
	struct node* next[26];
	struct node_list* list1;
};

struct node* getNode()
{
	struct node* temp = new node;
	for(int i=0;i<26;i++)
	{
		temp->next[i] = NULL ;
	}
	temp->list1 = NULL;
	return temp;
}

//adding data in doubly linked list
struct node_list* addInList(string name,string mobileNo,struct node_list* inList)
{
	struct node_list* temp = new node_list;
	temp->name = name;
	temp->mobileNo = mobileNo;
	temp->right = NULL;
	temp->left = NULL;
	temp->freq=0;
	if(inList==NULL)
	{
		inList = temp;
	}
	else
	{
		struct node_list* temp1 = inList;
		while(temp1->right!=NULL)
		{
			temp1=temp1->right;
		}
		temp1->right=temp;
		temp->left=temp1;
	}
	return inList;
}

//adding a contact in the tree
void add_contact(string name,string mobileNo,struct node* root)
{
	struct node* temp = root;
	for(int i=0;i<name.length();i++)
	{
		int ch = name[i]-'a';
		if(temp->next[ch]==NULL)
		{
			temp->next[ch] = getNode();
		}
		temp->next[ch]->list1=addInList(name,mobileNo,(temp->next[ch])->list1);
		temp=temp->next[ch];
	}
}

//searching a contact - Suggestions based on every prefix of the name is displayed.
void search(struct node* root)
{
    printf("Enter name you want to search: ");
	string s,t="";
	cin>>s;
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	bool check=0;
	int n = s.length();
	for(int i=0;i<n;i++)
    {
        char c = s[i];
        t+=s[i];
        root = root->next[c-'a'];
        if(root==NULL)
        {
            cout<<"No results found for "<<t<<"\n";
            break;
        }
        struct node_list* y = root->list1;
        cout<<"\nSuggestions based on "<<t<<": \n\n";
        while(y!=NULL)
        {
            cout<<y->name<<" ( "<<y->freq<<" ) ---> "<<y->mobileNo<<endl;
            if(y->name==s)
            {
                check=1;
            }
            y=y->right;
        }
        if(check && i==n-1)
        {
            cout<<"\nContact exists.\n\n";
            fstream contact;
            contact.open("search_history.txt",ios_base::out|ios_base::app);
            contact<<s;
            contact<<"\n";
            contact.close();
            cout<<"Updated your search history"<<endl;
        }
        else if(check==0 && i==n-1)
        {
            cout<<"\nContact doesn't exists.\n\n";
        }
        y = root->list1;
        while(y!=NULL)
        {
            if(y->name==s)
            {
                (y->freq)++;
                //updating the position in the linked list as per frequency.
                //swapping alternate values in linked list until it get sorted in descending order of frequency.
                while(y!=NULL && y->left!=NULL && ((y->left)->freq)<(y->freq))
                {
                    struct node_list *first,*second;
                    first = y->left;
                    second = y;
                    first->right = second->right;
                    if(first->left!=NULL)
                    first->left->right = second;
                    else
                    {
                        second->left=NULL;
                        root->list1=second;
                    }
                    if(second->right!=NULL)
                    second->right->left=first;
                    second->left=first->left;
                    second->right=first;
                    first->left=second;
                }
            }
            y=y->right;
        }
    }
    printf("\n");
}

//checking if contact having similar name already exists or not.
//if it is present already this will return 1 , else 0.
bool already_exists(string s, struct node* root)
{
    int n = s.length();
	for(int i=0;i<n;i++)
    {
        char c = s[i];
        root = root->next[c-'a'];
        if(root==NULL)
        {
            return 0;
        }
        if(i==n-1)
        {
            struct node_list* y = root->list1;
            while(y!=NULL)
            {
                if(y->name==s)
                    return 1;
            }
        }
    }
    return 0;
}

int main()
{
    cout<<"\n\n 'PHONEBOOK SEARCH RECOMMENDATION' \n\n";
	struct node* root = getNode();
	struct node* root1 = root;
	while(1)
	{
	    root=root1;
		printf("What do you want? \n\n 1. Add a contact. \n 2. Search a contact. \n 3. Exit \n\nEnter your choice(1/2/3): ");
		int choice;
		cin>>choice;
		if(choice==1)
		{
			string name,mobileNo;
			printf("Enter name of contact: ");
			cin>>name;
			transform(name.begin(), name.end(), name.begin(), ::tolower);
			printf("Enter mobile no. of contact: ");
			cin>>mobileNo;
			if(already_exists(name,root))
            {
                cout<<"\nError !!! Entered name contact already exists !! \n\n";
            }
            else
            {
                add_contact(name,mobileNo,root);
                cout<<"\nNew contact added.\n\n";
                fstream data;
                data.open("contact_in_phonebook.txt",ios_base::out|ios_base::app);
                data<<name;
                data<<"\n";
                data.close();
            }
		}
		else if(choice==2)
		{
		    search(root);
 		}
		else if(choice==3)
		{
		    break;
		}
		else
        {
            printf("INVALID CHOICE. \n\n");
        }
	}
}
