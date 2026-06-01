#include "RBTree.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

/* endNode — специальный служебный узел, расположенный после максимального элемента дерева.
   Используется для реализации итератора end().
   Перед операциями вставки и удаления временно отсоединяется от дерева,
   после завершения операции присоединяется обратно. */

static const bool RED = false; // чтобы было понятнее, какой цвет
static const bool BLACK = true;

BinarySearchTree::Node::Node(Key key, Value value,Node* parent, Node* left,Node* right) : keyValuePair(key, value) { 
	this->parent = parent;
	this->left = left;
	this->right = right;
}

BinarySearchTree::Node::Node(const Node& other) : keyValuePair(other.keyValuePair), color(other.color), parent(nullptr), left(nullptr), right(nullptr) { 
	if (other.left) {
		left = new Node(*other.left);
		left->parent = this;
	}
	if (other.right) {
		right = new Node(*other.right);
		right->parent = this;
	}
}

bool BinarySearchTree::Node::operator==(const Node& other) const {
	return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const { // печать поддерева (текущий узел - корень)
	if (left) { // если есть левый ребенок
		left->output_node_tree(); // переходим в него
	}

	std::cout << "key = " << keyValuePair.first << ", value = " << keyValuePair.second
		<< ", color = " << (color ? "BLACK" : "RED") << "\n";

	if (right) { // если есть правый ребенок
		right->output_node_tree(); // переходим в него
	}
}

void BinarySearchTree::Node::insert(const Key&, const Value&) {}
void BinarySearchTree::Node::erase(const Key&) {}

void BinarySearchTree::clear(Node* node) {
	if (node == nullptr) {
		return;
	}
	clear(node->left);
	clear(node->right);
	delete node;
}

BinarySearchTree::~BinarySearchTree()
{
	clear(_root);
	_root = nullptr;
	_size = 0;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : _size(other._size) {
	if (other._root) {
		_root = new Node(*other._root);
	}
	else {
		_root = nullptr;
	}
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
	if (this != &other) {
		BinarySearchTree temp(other);
		std::swap(_root, temp._root);
		std::swap(_size, temp._size);
	}
	return *this;
}
BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept {
	_root = other._root;
	_size = other._size;
	other._root = nullptr;
	other._size = 0;
}
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
	if (this != &other) {
		clear(_root);
		_root = other._root;
		_size = other._size;
		other._root = nullptr;
		other._size = 0;
	}
	return *this;
}

// итераторы

BinarySearchTree::Iterator::Iterator(Node* node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	return &_node->keyValuePair;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const {
	return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const {
	return _node != other._node;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
	if (!_node) {
		throw std::runtime_error("Invalid iterator increment: iterator == end()");
	}
	if (_node->keyValuePair.first == std::numeric_limits<Key>::max() && !_node->right) {
		throw std::runtime_error("Invalid iterator increment: iterator == end()");
	}
	if (_node->right) {
		_node = _node->right;
		while (_node->left) {
			_node = _node->left;
		}
	}
	else {
		Node* p = _node->parent;
		while (p && _node == p->right) {
			_node = p;
			p = p->parent;
		}
		_node = p;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
	Iterator temp = *this;
	++(*this);
	return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	if (_node->left) {
		_node = _node->left;
		while (_node->right) {
			_node = _node->right;
		}
	}
	else {
		Node* p = _node->parent;
		while (p && _node == p->left) {
			_node = p;
			p = p->parent;
		}
		_node = p;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
	Iterator temp = *this;
	--(*this);
	return temp;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node* node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	return &_node->keyValuePair;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const {
	return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const {
	return _node != other._node;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
	if (!_node) {
		throw std::runtime_error("Invalid iterator increment: iterator == end()");
	}
	if (_node->keyValuePair.first == std::numeric_limits<Key>::max() && !_node->right) {
		throw std::runtime_error("Invalid iterator increment: iterator == end()");
	}
	if (_node->right) {
		_node = _node->right;
		while (_node->left) {
			_node = _node->left;
		}
	}
	else {
		const Node* p = _node->parent;
		while (p && _node == p->right) {
			_node = p;
			p = p->parent;
		}
		_node = p;
	}
	return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
	ConstIterator temp = *this;
	++(*this);
	return temp;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
	if (!_node) {
		throw std::runtime_error("Invalid iterator dereference: iterator == end()");
	}
	if (_node->left) {
		_node = _node->left;
		while (_node->right) {
			_node = _node->right;
		}
	}
	else {
		const Node* p = _node->parent;
		while (p && _node == p->left) {
			_node = p;
			p = p->parent;
		}
		_node = p;
	}
	return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
	ConstIterator temp = *this;
	--(*this);
	return temp;
}
std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key& key) { // возвращает диапазон всех элементов с данным ключом
																												// last - первый элемент после всех элементов с ключом key
	Iterator first = find(key);
	if (first == end()) {
		return { end(), end() };
	}
	Iterator last = first;
	while (last != end() && last->first == key) {
		++last;
	}
	return { first, last };
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key& key) const {
	ConstIterator first = find(key);
	if (first == cend()) {
		return { cend(), cend() };
	}
	ConstIterator last = first;
	while (last != cend() && last->first == key) {
		++last;
	}
	return { first, last };
}
BinarySearchTree::Iterator BinarySearchTree::end() {
	if (!_root) {
		return Iterator(nullptr);
	}
	Node* current = _root;
	while (current->right) {
		current = current->right;
	}
	return Iterator(current);  /* В нормальном состоянии current указывает на endNode.
								  Если endNode временно отсоединен, функция вернет максимальный элемент дерева,
                                  поэтому end() нельзя вызывать между detachEndNode() и attachEndNode() */
}
BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
	if (!_root) {
		return ConstIterator(nullptr);
	}
	Node* current = _root;
	while (current->right) {
		current = current->right;
	}
	return ConstIterator(current); /* В нормальном состоянии current указывает на endNode.
								      Если endNode временно отсоединен, функция вернет максимальный элемент дерева,
                                      поэтому end() нельзя вызывать между detachEndNode() и attachEndNode() */
}
BinarySearchTree::Iterator BinarySearchTree::begin() {
	if (_root == nullptr) {
		return end();
	}
	Node* current = _root;
	while (current->left)
	{
		current = current->left;
	}
	return Iterator(current);
}
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
	if (_root == nullptr) {
		return cend();
	}
	Node* current = _root;
	while (current->left)
	{
		current = current->left;
	}
	return ConstIterator(current);
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key) {
	Node* current = _root;
	Node* res = nullptr;
	while (current) {
		if (key < current->keyValuePair.first) { // если ключ меньше
			current = current->left; // идем влево
		}
		else if (key > current->keyValuePair.first) { // если ключ больше
			current = current->right; // идем вправо
		}
		else {
			res = current; // нашли
			current = current->left; // идем вниз, чтобы найти самый левый такой ключ
		}
	}
	if (res) {
		return Iterator(res);
	}
	return end();
}
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const {
	Node* current = _root;
	Node* res = nullptr;
	while (current) {
		if (key < current->keyValuePair.first) {
			current = current->left;
		}
		else if (key > current->keyValuePair.first) {
			current = current->right;
		}
		else {
			res = current;
			current = current->left;
		}
	}
	if (res) {
		return ConstIterator(res);
	}
	return cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::min() const {
	return cbegin();
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const {
	ConstIterator temp1 = find(key);
	if (temp1 == cend()) {
		return cend();
	}
	ConstIterator temp2 = temp1;
	++temp1;
	while (temp1 != cend() && temp1->first == key) {
		if (temp1->second < temp2->second) {
			temp2 = temp1;
		}
		++temp1;
	}
	return temp2;
}
BinarySearchTree::ConstIterator BinarySearchTree::max() const {
	if (!_root) {
		return cend();
	}
	return --cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const {
	ConstIterator temp1 = find(key);
	if (temp1 == cend()) {
		return cend();
	}
	ConstIterator temp2 = temp1;
	++temp1;
	while (temp1 != cend() && temp1->first == key) {
		if (temp1->second > temp2->second) {
			temp2 = temp1;
		}
		++temp1;
	}
	return temp2;
}

BinarySearchTree::Node* BinarySearchTree::detachEndNode() { // отсоединить endNode
	if (!_root) {
		return nullptr;
	}
	Node* cur = _root;
	while (cur->right) { // идем к самому правому узлу
		cur = cur->right;
	}
	if (cur->keyValuePair.first != std::numeric_limits<Key>::max()) { // проверка, что самый правый узел не endNode
		return nullptr;
	}
	Node* endNode = cur; 
	if (endNode->parent) { // отцепляем от родителя
		endNode->parent->right = nullptr; 
		endNode->parent = nullptr;
	}
	else { // если endNode корень
		_root = nullptr;
	}
	return endNode;
}

void BinarySearchTree::attachEndNode(Node* endNode) { // присоединить endNode
	if (!endNode) {
		return;
	}
	endNode->color = BLACK;
	endNode->left = nullptr;
	endNode->right = nullptr;
	if (!_root) {
		_root = endNode;
		endNode->parent = nullptr;
		return;
	}
	Node* cur = _root;
	while (cur->right) { // ищем самый правый узел
		cur = cur->right;
	}
	cur->right = endNode; // прицепляем endNode справа
	endNode->parent = cur;
}

void BinarySearchTree::rotateLeft(Node* x) { // поворот влево вокруг x
	Node* y = x->right; // берем правого ребенка
	x->right = y->left; // левое поддерево y становится правым поддеревом x
	if (y->left) { // исправляем родителя узла
		y->left->parent = x;
	}
	y->parent = x->parent; // теперь y занимает место x
	if (!x->parent) { // если x был корнем — новым корнем становится y
		_root = y;
	}
	else if (x == x->parent->left) { // если x  был левым ребенком
		x->parent->left = y;
	}
	else { // если правым
		x->parent->right = y;
	}
	y->left = x; // теперь x становится левым ребёнком y
	x->parent = y;
}

void BinarySearchTree::rotateRight(Node* x) { // поворот вправо вопруг x 
	Node* y = x->left;
	x->left = y->right;
	if (y->right) {
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (!x->parent) {
		_root = y;
	}
	else if (x == x->parent->right) {
		x->parent->right = y;
	}
	else {
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}

void BinarySearchTree::insertFixup(Node* z) { // восстанавливает свойства красно-черного дерева после вставки
	while (z->parent && z->parent->color == RED) { // пока родитель красный
		Node* gp = z->parent->parent; // дедушка
		if (!gp) {
			break;
		}
		if (z->parent == gp->left) { // родитель слева от дедушки
			Node* y = gp->right; // дядя
			if (y && y->color == RED) { // дядя красный
				z->parent->color = BLACK;
				y->color = BLACK;
				gp->color = RED;
				z = gp;
			}
			else { // дядя черный
				if (z == z->parent->right) {
					z = z->parent;
					rotateLeft(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rotateRight(z->parent->parent);
			}
		}
		else { // родитель справа от дедушки
			Node* y = gp->left; // дядя
			if (y && y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				gp->color = RED;
				z = gp;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					rotateRight(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rotateLeft(z->parent->parent);
			}
		}
	}
	if (_root) {
		_root->color = BLACK; // корень всегда черный
	}
}

void BinarySearchTree::insert(const Key& key, const Value& value) { // вставка
	if (key == std::numeric_limits<Key>::max()) { // проверка на зарезервированный под end() ключ
		throw std::invalid_argument("Cannot insert reserved endNode key");
	}
	Node* endNode = detachEndNode(); // отсоединяем endNode

	Node* z = new Node(key, value); // создаем новый узел
	z->color = RED; // красим в красный

	Node* y = nullptr;
	Node* x = _root;
	while (x) {
		y = x;
		if (key < x->keyValuePair.first) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	z->parent = y;
	if (!y) {
		_root = z;
	}
	else if (key < y->keyValuePair.first) {
		y->left = z;
	}
	else {
		y->right = z;
	}

	insertFixup(z); // восстановка свойств красно-черного дерева

	if (!endNode) {
		endNode = new Node(std::numeric_limits<Key>::max(), Value());
	}
	attachEndNode(endNode); // присоединяем endNode обратно

	++_size;
}

void BinarySearchTree::transplant(Node* u, Node* v) { // заменяем поддерево u на поддерево v
	if (!u->parent) {
		_root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	if (v) {
		v->parent = u->parent;
	}
}

BinarySearchTree::Node* BinarySearchTree::minNode(Node* node) const { // поиск ущла с минимальным ключом в поддереве
	while (node && node->left) {
		node = node->left;
	}
	return node;
}

void BinarySearchTree::eraseFixup(Node* x, Node* xParent) { // вызывается, если удаленный узел был черным 
	while (x != _root && (x == nullptr || x->color == BLACK)) { // пока нарушено свойство одинаковой черной высоты путей
		if (xParent == nullptr) {
			break;
		}
		if (x == xParent->left) { // x - левый ребенок родителя
			Node* w = xParent->right; // брат
			if (w && w->color == RED) { // 1 случай: брат красный
				w->color = BLACK;
				xParent->color = RED;
				rotateLeft(xParent);
				w = xParent->right;
			}
			bool wLB = (!w || !w->left || w->left->color == BLACK); // левый ребенок брата черный
			bool wRB = (!w || !w->right || w->right->color == BLACK); // правый ребенок брата черный
			if (wLB && wRB) { // 2 случай: у брата оба ребенка черные
				if (w) {
					w->color = RED;
				}
				x = xParent;
				xParent = x ? x->parent : nullptr;
			}
			else { 
				if (wRB) { // 3 случай: правый ребенок черный
					if (w && w->left) {
						w->left->color = BLACK;
					}
					if (w) {
						w->color = RED;
						rotateRight(w);
					}
					w = xParent->right;
				}
				if (w) { // 4 случай: правый ребенок красный
					w->color = xParent->color;
				}
				xParent->color = BLACK;
				if (w && w->right) {
					w->right->color = BLACK;
				}
				rotateLeft(xParent);
				x = _root;
				break;
			}
		}
		else { // x - правый ребенок родителя
			Node* w = xParent->left; // брат
			if (w && w->color == RED) {
				w->color = BLACK;
				xParent->color = RED;
				rotateRight(xParent);
				w = xParent->left;
			}
			bool wRB = (!w || !w->right || w->right->color == BLACK);
			bool wLB = (!w || !w->left || w->left->color == BLACK);
			if (wRB && wLB) {
				if (w) {
					w->color = RED;
				}
				x = xParent;
				xParent = x ? x->parent : nullptr;
			}
			else {
				if (wLB) {
					if (w && w->right) {
						w->right->color = BLACK;
					}
					if (w) {
						w->color = RED;
						rotateLeft(w);
					}
					w = xParent->left;
				}
				if (w) {
					w->color = xParent->color;
				}
				xParent->color = BLACK;
				if (w && w->left) {
					w->left->color = BLACK;
				}
				rotateRight(xParent);
				x = _root;
				break;
			}
		}
	}
	if (x) {
		x->color = BLACK;
	}
}

void BinarySearchTree::erase(const Key& key) { // удаление всех узлов с данным ключом
	if (!_root) {
		return;
	}
	if (key == std::numeric_limits<Key>::max()) {
		return;
	}
	Node* endNode = detachEndNode(); // открепляем endNode

	while (_root) { 
		Node* z = _root;
		while (z) { // поиск узла
			if (key < z->keyValuePair.first) {
				z = z->left;
			}
			else if (key > z->keyValuePair.first) {
				z = z->right;
			}
			else {
				break;
			}
		}
		if (!z) {
			break;
		}

		Node* y = z;
		bool yOriginalColor = y->color;
		Node* x = nullptr;
		Node* xParent = nullptr;

		if (!z->left) { // нет левого ребенка
			x = z->right;
			xParent = z->parent;
			transplant(z, z->right);
		}
		else if (!z->right) { // нет правого ребенка
			x = z->left;
			xParent = z->parent;
			transplant(z, z->left);
		}
		else { // есть оба ребенка
			y = minNode(z->right); // преемник(минимум в правом поддереве)
			yOriginalColor = y->color; // сохраняем цвет, если цвет черный, то надо будет исправлять
			x = y->right;
			if (y->parent == z) {
				xParent = y;
			}
			else {
				xParent = y->parent;
				transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z, y); // заменяем z узлом y
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}

		delete z;
		--_size;

		if (yOriginalColor == BLACK && _root) { // если узел был черным
			eraseFixup(x, xParent);
		}
	}

	if (_root) {
		attachEndNode(endNode); // прикрепляем endNode обратно
	}
	else {
		delete endNode;
	}
}

size_t BinarySearchTree::size() const { // размер дерева
	return _size;
}

void BinarySearchTree::output_tree() { // вывести дерево
	if (!_root) {
		return;
	}
	_root->output_node_tree();
}

size_t BinarySearchTree::heightRecursive(Node* node) const { // рекурсивное вычисление высоты поддерева
	if (!node) { // если дошли до пустого поддерева, то высота 0
		return 0;
	}
	if (node->keyValuePair.first == std::numeric_limits<Key>::max()) { // проверка на endNode, т.к. он не является частью дерева => не учитывается при подсчете высоты
		return 0;
	}
	size_t leftHeight = heightRecursive(node->left); // высота левого поддерева
	size_t rightHeight = heightRecursive(node->right); // высота правого поддерева
	return 1 + std::max(leftHeight, rightHeight); // текущий узел + высота самого высокого поддерева
}

size_t BinarySearchTree::max_height() const { // подсчет высоты всего дерева
	return heightRecursive(_root);
}
