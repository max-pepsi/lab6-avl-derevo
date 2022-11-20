#include<iostream>
#include<iomanip>
#include<vector>
using namespace std;

//Подсчитать среднее арифметическое всех листов дерева.
//Затем вычесть из каждого листа данное значение, затем
//обойти дерево еще раз и удалить все элементы, делящиеся без
//остатка на 3 и перестроить дерево с учетом изменений
//Обратный порядок обхода


//- добавлять элементы в сбалансированное дерево поиска;           
//- удалять элементы из сбалансированного дерева поиска;             
//- искать элементы в дереве поиска с выводом количества шагов, за которое осуществляется поиск;
//- выводить дерево на экран
//- выводить список, соответствующий обходу вершин в обратном порядке
//- осуществлять операцию, заданную в таблице 6.


struct vertex {
	vertex* left = NULL;
	vertex* right = NULL;
	int value = 0;
	int balance;

	vertex(int value) {
		this->value = value;
		this->balance = 0;
	}
};

//REBALANCING
void rotateLeft(vertex*& node) {

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
	vertex* b = a->left;
	int ab = a->balance;
	int bb = b->balance;

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

//TREE MANIPULATION
int minimum(vertex* node) {
	if (node->left == NULL) return node->value;
	return minimum(node->left);
}
bool add_elem(int value, vertex* &root) { // false to stop ascention (rebalansing)
	if (root == NULL) {
		root = new vertex(value);
		return true;
	}
	if (value > root->value) {
		if (root->right != NULL) {
			if (!add_elem(value, root->right)) return false; // added and if no need to rebalance - exit
		}
		else {
			vertex* new_v = new vertex(value);
			root->right = new_v;
		}
		root->balance -= 1;
	}

	else if (value < root->value) {
		if (root->left != NULL) {
			if (!add_elem(value, root->left)) return false;
		}
		else {
			vertex* new_v = new vertex(value);
			root->left = new_v;
		}
		root->balance += 1;
	}
	else return false; // it's already in a tree
	
	if (root->balance == 2 || root->balance == -2) {
		rebalance(root);
	}

	if (root->balance == 0) { //if after rebalancing or else it's 0 - stop ascention
		return false;
	}
	return true; //keep ascending
	
}
bool remove_elem(int value, vertex*& root) {
	if (root == NULL) return false;
	if (value < root->value) {
		return remove_elem(value, root->left);
	}
	else if (value > root->value) {
		return remove_elem(value, root->right);
	}
	else if (root->left != NULL && root->right != NULL) {
		root->value = minimum(root->right);
		return remove_elem(root->value, root->right);
	}
	else {
		if (root->left != 0) {
			root = root->left;
		}
		else if (root->right != NULL) {
			root = root->right;
		}
		else {
			root = NULL;
		}
		return true;
	}
}

//TREE DISPLAY
void print(const string& prefix, vertex* node) {
	if (node != nullptr)
	{
		bool hasLeft = node->left != NULL;
		cout  << "__"  << setw(3)<< setfill('_') << node->value;
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
	cout << endl << endl;
}
void traversal(vertex* node) {
	if (node != NULL) {
		traversal(node->left);
		traversal(node->right);
		cout << node->value << " ";
	}
}

//SEARCH AND TASK
vertex* search(int elem, vertex* node, int* steps) {
	if (node == NULL) return NULL;
	if (elem == node->value) return node;
	*steps += 1;
	if (elem < node->value)
		return search(elem, node->left, steps);
	return search(elem, node->right, steps);
}
void collect(vertex* node, vector<int>& v) {
	if (node != NULL) {
		if (node->left == NULL && node->right == NULL) {
			v.push_back(node->value);
		}
		else {
			collect(node->left, v);
			collect(node->right, v);
		}
	}
}
void removal(vertex*& node, vertex*& root) {
	if (node == NULL) return;
	removal(node->left, root);
	removal(node->right, root);
	if (node->value % 3 == 0) remove_elem(node->value, root);
}
void task(vertex*& root) {
	if (root == NULL) return;
	int sum = 0;
	vector<int> v;
	collect(root, v);

	for (int i = 0; i < v.size(); i++) {
		sum += v[i];
	}
	sum /= v.size();

	for (int i = 0; i < v.size(); i++) {
		remove_elem(v[i], root);
		if ((v[i] - sum) % 3 != 0) {
			add_elem(v[i] - sum, root);
		}
	}
	removal(root, root);
}

int main() {
	setlocale(LC_ALL, "rus");

	vertex* root = NULL;
	cout << "Вводите целые числа для добавления в дерево через enter. Чтобы закончить, введите не число: " << endl;
	int v;
	cin >> v;
	while (!cin.fail()) {
		add_elem(v, root);
		cin >> v;
	}
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());


	//vertex* root = new vertex(5);
	//add_elem(3, root);
	//add_elem(6, root);
	//add_elem(7, root);
	//add_elem(0, root);

	print(root);

	cout << "Обход вершин в обратном порядке: " << endl;
	traversal(root);
	cout << endl;

	int a;
	cout << "Выберите какой элемент удалить: ";
	cin >> a;
	if (remove_elem(a, root)) print(root);
	else cout << "Такого элемента нет" << endl;

	int steps = 0;
	cout << "Какой элемент искать: ";
	cin >> a;
	vertex* node = search(a, root, &steps);
	if (node == NULL) cout << "Такого элемента нет" << endl;
	else cout << "Элемент найден, количество шагов: " << steps << endl;

	cout << "Вычесть из каждого листа среднее арифметическое всех листов, затем удалить из дерева все элементы делящиеся на 3 и ребалансировать." << endl;
	task(root);
	cout << "Получившееся дерево: " << endl;
	print(root);
}