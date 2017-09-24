#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>


using namespace std;

struct Node{
	int data;
	Node* parent;
	Node* leftChild;
	Node* rightChild;
public:
	Node(int d){
		data = d;
	}

	Node(int d, Node* par){
		data = d;
		parent = par;
	}

	bool isParentOf(Node* child){
		if (child == NULL) 
			return false;
		return child->parent == this;
	}

	bool isLeftChildOf(Node* parent){
		if (parent == NULL) 
			return false;
		return parent->leftChild == this;
	}

	bool isRightChildOf(Node* parent){
		if (parent == NULL) 
			return false;
		return parent->rightChild == this;
	}

	void Remove(){
		if (parent != NULL){
			if (isLeftChildOf(parent)){
				parent->leftChild = NULL;
			}
			if (isRightChildOf(parent)){
				parent->rightChild = NULL;
			}
			parent = NULL;
		}

		if (leftChild != NULL){
			leftChild = NULL;
		}

		if (rightChild != NULL){
			rightChild = NULL;
		}
	}

	Node(){}

	~Node(){
		Remove();
	}
};

class BinarySearchTree{
public:
	BinarySearchTree(){
		Root = NULL;
		heapAllocCounter = 0;
	}

	~BinarySearchTree(){
		for (int i = 0; i < heapAllocCounter; i++){
			if (heapAllocList[i] != NULL){
				heapAllocList[i]->parent = heapAllocList[i]->leftChild = heapAllocList[i]->rightChild = NULL;
				delete heapAllocList[i];
				heapAllocList[i] = NULL;
			}
				
		}
	}
	
	Node* Insert(int data){
		Root = Insert(Root, NULL, data, 0);
		return Root;
	}

	Node* MinimumNodeInTree(Node* n){
		if (n->leftChild != NULL)
			return MinimumNodeInTree(n->leftChild);
		else
			return n;
	}

	Node* MaximumNodeInTree(Node* n){
		if (n->rightChild != NULL)
			return MaximumNodeInTree(n->rightChild);
		else
			return n;
	}

	void PreOrderTravel(Node* n){
		if (n == NULL) 
			return;
		else{
			cout << "Travel : " << n->data << endl;
			if (n->leftChild != NULL)
				PreOrderTravel(n->leftChild);

			if (n->rightChild != NULL)
				PreOrderTravel(n->rightChild);
		}
	}

	void PreOrderPrint(Node* n, int level){
		if (heapAllocCounter == 0)
			return;
		PrintData(n, level);
		if (n != NULL){
			PreOrderPrint(n->leftChild, level + 1);
			PreOrderPrint(n->rightChild, level + 1);
		}
	}

	void InOrderTravel(Node* n){
		if (n == NULL)
			return;
		else{
			if (n->leftChild != NULL)
				InOrderTravel(n->leftChild);
			if (n != NULL)
				cout << "Travel : " << n->data << endl;
			if (n->rightChild != NULL)
				InOrderTravel(n->rightChild);
		}
	}

	Node* GetSuccessor(int num){
		Node* node = Search(Root, num, 0);
		if (node->rightChild != NULL)
			return MinimumNodeInTree(node->rightChild);
		else{
			while (node->isRightChildOf(node->parent)){
				node = node->parent;
			}
			while (node->data == node->parent->data){
				node = node->parent;
			}
			node = node->parent;
			return node;
		}
	}

	void Delete(int num){
		Node* node = Search(Root, num, 0);
		if (node != NULL){
			Delete(node);
		}
		
	}

	Node* Search(int data){
		return Search(Root, data, 0);
	}

	void Clear(){
		while (heapAllocCounter > 0){
			Delete(Root);
		}
		Root = NULL;
	}

private:
	Node* Root;
	int heapAllocCounter;
	Node* heapAllocList[100];

	void PrintData(Node* n, int level){
		for (int i = 0; i < level; i++){
			cout << "\t";
		}
		if (n != NULL)
			cout << n->data << endl;
		else if(n == NULL){
			cout << "-" << endl;
		}
			
	}

	void Delete(Node* node){
		if (!node->leftChild && !node->rightChild){
			DeallocateNode(node);
		}
		else if (!node->rightChild){
			Node* victim = MaximumNodeInTree(node->leftChild);
			node->data += victim->data;
			victim->data = node->data - victim->data;
			node->data -= victim->data;
			Delete(victim);
		}
		else{
			Node* victim = MinimumNodeInTree(node->rightChild);
			node->data += victim->data;
			victim->data = node->data - victim->data;
			node->data -= victim->data;
			Delete(victim);
		}
	}

	void DeallocateNode(Node* node){
		if (node){
			//node->Remove();
			delete node;
			node = NULL;
			heapAllocCounter--;
		}
	}

	Node* Insert(Node* node, Node* parentToNewNode, int data, int level){
		if (node == NULL){
			if (parentToNewNode != NULL)
				node = CreateNewNode(data, parentToNewNode);
			else
				node = CreateNewNode(data);
			cout << level << endl;
		}
		else if (data >= node->data){
			node->rightChild = Insert(node->rightChild, node, data, level + 1);
		}
		else if (data < node->data){
			node->leftChild = Insert(node->leftChild, node, data, level + 1);
		}
		return node;
	}

	Node* CreateNewNode(int data){
		Node* newNode = new Node(data);
		newNode->parent = newNode->leftChild = newNode->rightChild = NULL;
		heapAllocList[heapAllocCounter] = newNode;
		heapAllocCounter++;
		return newNode;
	}

	Node* CreateNewNode(int data, Node* parent){
		Node* newNode = new Node(data, parent);
		newNode->leftChild = newNode->rightChild = NULL;
		heapAllocList[heapAllocCounter] = newNode;
		heapAllocCounter++;
		return newNode;
	}

	Node* Search(Node* root, int data, int level){
		if (root == NULL) 
			return NULL;
		if (root->data == data){
			cout << level << endl;
			return root;
		}
		else if (data > root->data){
			return Search(root->rightChild, data, level + 1);
		}
		else{
			return Search(root->leftChild, data, level + 1);
		}
	}
};

void processInput(char input[], BinarySearchTree* bst, Node* root){
	if (strstr(input, "insert")){
		int result = 0;
		int multiplier = 1;
		int index = strlen(input) - 1;
		while (isdigit(input[index]) > 0){
			result += ((int)input[index] - '0') * multiplier;
			multiplier *= 10;
			index--;
		}
		bst->Insert(result);
	}
	else if (strstr(input, "delete")){
		int result = 0;
		int multiplier = 1;
		int index = strlen(input) - 1;
		while (isdigit(input[index]) > 0){
			result += ((int)input[index] - '0') * multiplier;
			multiplier *= 10;
			index--;
		}
		bst->Delete(result);
	}
	else if (strstr(input, "search")){
		int result = 0;
		int multiplier = 1;
		int index = strlen(input) - 1;
		while (isdigit(input[index]) > 0){
			result += ((int)input[index] - '0') * multiplier;
			multiplier *= 10;
			index--;
		}
		bst->Search(result);
	}
	else if (strstr(input, "clear")){
		bst->Clear();
	}
	else if (strstr(input, "show")){
		bst->PreOrderPrint(root, 0);
	}
}

int main(){
	BinarySearchTree* bst = new BinarySearchTree();
	Node* rootNode = NULL;
	char input[100];
	
	do{
		gets(input);
		if (strstr(input, "insert")){
			int result = 0;
			int multiplier = 1;
			int index = strlen(input) - 1;
			while (isdigit(input[index]) > 0){
				result += ((int)input[index] - '0') * multiplier;
				multiplier *= 10;
				index--;
			}
			rootNode = bst->Insert(result);
		}
		else if (strstr(input, "delete")){
			int result = 0;
			int multiplier = 1;
			int index = strlen(input) - 1;
			while (isdigit(input[index]) > 0){
				result += ((int)input[index] - '0') * multiplier;
				multiplier *= 10;
				index--;
			}
			bst->Delete(result);
		}
		else if (strstr(input, "search")){
			int result = 0;
			int multiplier = 1;
			int index = strlen(input) - 1;
			while (isdigit(input[index]) > 0){
				result += ((int)input[index] - '0') * multiplier;
				multiplier *= 10;
				index--;
			}
			Node* searchResult = bst->Search(result);
		}
		else if (strstr(input, "clear")){
			bst->Clear();
		}
		else if (strstr(input, "show")){
			bst->PreOrderPrint(rootNode, 0);
		}

	} while (strcpy(input, "exit") != 0);
	return 0;
}
