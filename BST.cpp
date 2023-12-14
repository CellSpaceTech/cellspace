#include <stdlib.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <regex>

using namespace std;

struct ValueNode {
	string partOfSpeech;
	string definition;
	struct ValueNode* next = NULL;
	struct ValueNode* prev = NULL;
};

class BST {
public:
	struct ValueNode* value;
	BST* left, * right;
	BST()
	{
		value = NULL;
		left = right = NULL;

	}
	BST(struct ValueNode* v)
	{
		value = v;
		left = right = NULL;
	}

	BST* insert(BST* root, struct ValueNode* value) {
		if (!root) {
			return new BST(value);
		}
		string def = value->definition, this_def = root->value->definition;
		transform(def.begin(), def.end(), def.begin(), ::tolower);
		transform(this_def.begin(), this_def.end(), this_def.begin(), ::tolower);
	
		if (value->partOfSpeech > root->value->partOfSpeech) {
			root->right = insert(root->right, value);
		}
		else if (value->partOfSpeech < root->value->partOfSpeech) {
			root->left = insert(root->left, value);
		}
		else {
			if (def > this_def) {
				root->right = insert(root->right, value);
			}
			else {
				root->left = insert(root->left, value);
			}
		}

		return root;

	}
	bool keyExists(BST* node, struct ValueNode* value) {
		if (!node)
			return false;

		string def = value->definition, this_def = node->value->definition;
		transform(def.begin(), def.end(), def.begin(), ::tolower);
		transform(this_def.begin(), this_def.end(), this_def.begin(), ::tolower);


		if (node->value->partOfSpeech == value->partOfSpeech && this_def == def) {
			return true;
		}

		if (node->value->partOfSpeech.compare(value->partOfSpeech) < 0) {
			return keyExists(node->right, value);
		}
		else if (node->value->partOfSpeech.compare(value->partOfSpeech) > 0) {
			return keyExists(node->left, value);
		}

		if (def > this_def) {
			return keyExists(node->right, value);
		}
		else {
			return keyExists(node->left, value);
		}
	}
	void printOrder(BST* root) {
		if (!root) {
			return;
		}
		printOrder(root->left);
		cout << root->value->partOfSpeech << " -> " << root->value->definition << endl;
		printOrder(root->right);
	}

	void printOrderReversed(BST* root) {
		if (!root) {
			return;
		}
		printOrder(root->right);
		cout << root->value->partOfSpeech << " -> " << root->value->definition << endl;
		printOrder(root->left);
	}
};

class LinkedValues {

	ValueNode* head;
	ValueNode* tail;
public:
	LinkedValues() {
		head = tail = NULL;
	}
	ValueNode* getHead() {
		return head;
	}
	void insert(string pos, string def) {
		struct ValueNode* new_node = new ValueNode;
		new_node->partOfSpeech = pos;
		new_node->definition = def;
		new_node->next = NULL;
		new_node->prev = tail;
		if (head == nullptr) {
			head = tail = new_node;
			return;
		}
		tail->next = new_node;
		tail = new_node;
	}

	LinkedValues getValueByPoS(string pos) {
		LinkedValues linked;
		struct ValueNode* current_node = head;
		while (current_node != NULL) {
			if (current_node->partOfSpeech == pos) {
				linked.insert(current_node->partOfSpeech, current_node->definition);
			}
			current_node = current_node->next;
		}
		return linked;
	}
	LinkedValues getDistinctValues() {
		LinkedValues linked;
		BST bst, * root = NULL;
		string key;
		struct ValueNode* current_node = head;
		root = bst.insert(root, current_node);

		while (current_node != NULL) {
			if (!bst.keyExists(root, current_node)) {

				bst.insert(root, current_node);
			}
			current_node = current_node->next;
		}
		getDistinct(root, &linked);

		return linked;
	}
	void  getDistinct(BST* node, LinkedValues* values) {
		if (!node)
			return;

		getDistinct(node->left, values);
		values->insert(node->value->partOfSpeech, node->value->definition);
		getDistinct(node->right, values);
	}
	LinkedValues getReverseValues() {
		LinkedValues linked;
		BST bst, * root = NULL;
		string key;
		struct ValueNode* current_node = head;
		root = bst.insert(root, current_node);
		current_node = current_node->next;
		while (current_node != NULL) {
			bst.insert(root, current_node);
			current_node = current_node->next;
		}
		getDistinct(root, &linked);
		linked = linked.reverse();
		return linked;
	}
	LinkedValues  getRerversed(BST* node, LinkedValues values) {
		if (node == NULL)
			return values;

		getRerversed(node->right, values);
		values.insert(node->value->partOfSpeech, node->value->definition);
		getRerversed(node->left, values);

	}
	LinkedValues reverse() {
		LinkedValues newLinkedValues;
		struct ValueNode* current_node = tail;
		while (current_node != NULL) {
			newLinkedValues.insert(current_node->partOfSpeech, current_node->definition);
			current_node = current_node->prev;
		}
		return newLinkedValues;
	}
	LinkedValues getOrderedValues() {
		LinkedValues linked;
		BST bst, * root = NULL;
		string key;
		struct ValueNode* current_node = head;
		root = bst.insert(root, current_node);
		current_node = current_node->next;
		while (current_node != NULL) {
			bst.insert(root, current_node);
			current_node = current_node->next;
		}
		getDistinct(root, &linked);
		return linked;
	}
	void printAll(string key) {
		struct ValueNode* current_node = head;
		while (current_node != NULL) {
			cout << "\t" << key << " [" << current_node->partOfSpeech << "]:" << current_node->definition << endl;
			current_node = current_node->next;
		}
	}
};
