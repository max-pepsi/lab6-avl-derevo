#include<iostream>
#include<windows.h>
#include<iomanip>
using namespace std;

//Подсчитать среднее арифметическое всех листов дерева.
//Затем вычесть из каждого листа данное значение, затем
//обойти дерево еще раз и удалить все элементы, делящиеся без
//остатка на 3 и перестроить дерево с учетом изменений
//Обратный порядок обхода

//- добавлять элементы в сбалансированное дерево поиска;            DONE
//-удалять элементы из сбалансированного дерева поиска;             DONE
//- искать элементы в дереве поиска с выводом количества шагов, за  DONE
//которое осуществляется поиск;
//- выводить дерево на экран                                        DONE
//- выводить список, соответствующий обходу вершин в обратном порядке
//- осуществлять операцию, заданную в таблице 6.


struct vertex {
	vertex* left = NULL;
	vertex* right = NULL;
	int value = 0;
	int balance;
	vertex() {}
	vertex(vertex* left, vertex* right, int balance, int value) {
		this->left = left;
		this->right = right;
		this->balance = balance;
		this->value = value;
	}
	vertex(int value) {
		this->value = value;
		this->balance = 0;
	}
};

void rotateLeft(vertex*& node) {

	//bn = 1
	//	an = 0
	//	bn = 0 if ((ab + bb) < -2) // true
	//	bn = 2 if (ab + bb) == 0
	//	an = 1 if (ab == bb)
	//	an = -1 else if (ab + bb == -2)
	vertex* a = node;
	vertex* b = a->right;
	int ab = a->balance;
	int bb = b->balance;

	a->right = b->left;
	b->left = a;

	b->balance = 1;
	a->balance = 0;
	if ((ab + bb) < -2) b->balance = 0;
	if (ab + bb == 0) b->balance = 2;
	if (ab == bb) a->balance = 1;
	else if (ab + bb == -2) a->balance = -1;
	

	node = b;
}

void rotateRight(vertex*& node) {
	vertex* a = node;
	vertex* b = a->left; // 1 or 0
	int ab = a->balance;
	int bb = b->balance;

	//bn = -1
	//	an = 0
	//	bn = 0 if ((ab + bb) > 2)
	//	bn = -2 if (ab + bb) == 0
	//	an = -1 if (ab == bb)
	//	an = 1 else if (ab + bb == 2)

	a->left = b->right;
	b->right = a;
	
	b->balance = -1;
	a->balance = 0;
	if ((ab + bb) > 2) b->balance = 0;
	if (ab + bb == 0) b->balance = -2;
	if (ab == bb) a->balance = -1;
	else if (ab + bb == 2) a->balance = 1;

	node = b;
}

void bigRotateLeft(vertex*& a) {
	rotateRight(a->right);
	rotateLeft(a);
}

void bigRotateRight(vertex*& a) {
	rotateLeft(a->left);
	rotateRight(a);
}

void rebalance(vertex*& node) {
	if (node->balance == -2) {
		if (node->right->balance < 1) {
			rotateLeft(node);
		}
		else bigRotateLeft(node);
	}
	else {
		if (node->left->balance > -1) {
			rotateRight(node);
		}
		else bigRotateRight(node);
	}
}

bool add_elem(int value, vertex* &node) { // false to stop ascention (rebalansing)
	if (value > node->value) {
		if (node->right != NULL) {
			if (!add_elem(value, node->right)) return false; // just finishing the recur
		}
		else {
			vertex* new_v = new vertex(value);
			node->right = new_v;
		}
		node->balance -= 1;
	}
	else if (value < node->value) {
		if (node->left != NULL) {
			if (!add_elem(value, node->left)) return false;
		}
		else {
			vertex* new_v = new vertex(value);
			node->left = new_v;
		}
		node->balance += 1;
	}
	else return false; // it's already in a tree
	
	if (node->balance == 2) {
		rebalance(node);
	}
	else if (node->balance == -2) {
		rebalance(node);
	}
	if (node->balance == 0) { //if after rebalancing or else it's 0 - stop ascention
		return false;
	}
	return true;
	
}

int minimum(vertex* node) {
	if (node->left == NULL) return node->value;
	return minimum(node->left);
}

void remove_elem(int value, vertex*& root) {
	if (root == NULL) return;
	if (value < root->value) remove_elem(value, root->left);
	else if (value > root->value) remove_elem(value, root->right);
	else if (root->left != NULL && root->right != NULL) {
		root->value = minimum(root->right);
		remove_elem(root->value, root->right);
	}
	else {
		if (root->left != 0) root = root->left;
		else if (root->right != NULL) root = root->right;
		else root = NULL;
	}
}

void print(const string& prefix, vertex* node) {
	if (node != nullptr)
	{
		bool hasLeft = node->left != NULL;
		cout  << "--"  << setw(3)<< setfill('-') << node->value;
		print(hasLeft ? prefix + "|    " : prefix + "    ", node->right);

		if (hasLeft) {
			cout << endl << prefix << "L";
			print(prefix + "     ", node->left);
		}
	}
}

void print(vertex* head) {
	const string prefix = "    ";
	print(prefix, head);
}

vertex* search(int elem, vertex* node, int* steps) {
	if (node == NULL) return NULL;
	if (elem == node->value) return node;
	*steps += 1;
	if (elem < node->value)
		return search(elem, node->left, steps);
	return search(elem, node->right, steps);
}


int main() {
	setlocale(LC_ALL, "rus");

	vertex* root = new vertex(6);
	add_elem(7, root);
	add_elem(2, root);
	add_elem(1, root);
	add_elem(4, root);
	add_elem(3, root);
	add_elem(5, root);
	print(root);
	cout << endl << endl;
	int steps = 0;
	vertex* node = search(1, root, &steps);
	cout << steps << endl;
	steps = 0;
	node = search(-1, root, &steps);
	cout << steps << endl;
	steps = 0;
	node = search(4, root, &steps);
	cout << steps << node->right->value;


	//vertex* S = new vertex(1);
	//vertex* R = new vertex(2);
	//vertex* Q = new vertex(3);
	//vertex* P = new vertex(4);
	//vertex* c = new vertex(Q, R, -1, 5);
	//vertex* b = new vertex(c, S, 1, 6);
	//vertex* a = new vertex(P, b, -2, 7);

	//vertex* root = bigRotateLeft(a);
	
	

	//if (root->right == b) cout << "1 ";
	//if (root->left == a) cout << "2 ";
	//if (root->right->right == S) cout << "3 ";
	//if (root->left->right == Q) cout << "4 ";
	//if (root->balance == 0) cout << "5 ";
	//if (root->right->balance == 0) cout << "6 ";
	//if (root->left->balance == 1) cout << "7 ";
	//cout << root->left->balance;



}