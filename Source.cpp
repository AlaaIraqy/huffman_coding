#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <bitset>
using namespace std;
string originalText;							
unordered_map<char, string> huffmanCode;
unordered_map<string, char> huffmanCode_decompress;
string encodedText;
string decodedText;

struct Node
{
	char ch;
	int freq;
	Node* left, * right;
};
Node* root;
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		if (l->freq > r->freq) {
			return true;
		}
		else
			return false;
	}
};

// traversing the tree then storing the codes in a map
void encode(Node* root, string str,
	unordered_map<char, string>& huffmanCode)
{
	if (root == nullptr)
		return;

	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}
// left nodes add a 0 to the code while right nodes add 1
	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

// Builds Huffman Tree
void buildHuffmanTree(string text)
{
	unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	priority_queue<Node*, vector<Node*>, comp> pq;

	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{
		//removing 2 nodes with least frequency and replacing them with 1 node 
		Node* left = pq.top(); 
		pq.pop();
		Node* right = pq.top();	
		pq.pop();
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	root = pq.top();

	
	encode(root, "", huffmanCode);
	//print huffman codes
	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair : huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
	}

	// print encoded string
	string str = "";
	for (char ch : text) {
		str += huffmanCode[ch];
	}
	encodedText = str;

}
string strToBinary(string s)
{
	int nItr = s.length();
	string binary = "";
	string tempbinary = "";
	for (int i = 0; i < nItr; i++)
	{
		tempbinary = "";
		int n = (int)s[i];
		while (n > 0)
		{
			if (n % 2 == 0)
			{
				tempbinary += "0";
			}
			else
			{
				tempbinary += "1";
			}
			n /= 2;
		}
		if (tempbinary.length() < 8) {
			int add = 8 - tempbinary.length();
			for (int i = 0; i < add; i++) {
				tempbinary += "0";
			}
		}
		reverse(tempbinary.begin(), tempbinary.end());
		binary += tempbinary;
	}
	return binary;
}
int main()
{
	while (true) {
		int zeros;
		int x;
		cout << "select function: \n" << "1- Compress\t2- Decompress\t3- Exit" << endl;
		cin >> x;
		if (x == 1) {
			string str;
			cout << "enter file name: " << endl;
			cin >> str;
			ifstream MyReadFile(str);
			string myText;
			string text;
			while (getline(MyReadFile, myText)) {
				text += myText;
				text += "\n";
			}
			MyReadFile.close();
			buildHuffmanTree(text);
			cout << encodedText << endl;
			ofstream MyFile("encoded.txt");
			MyFile << encodedText.length() << endl;
			MyFile << encodedText << endl;
			for (auto pair : huffmanCode) {
				MyFile << pair.first << pair.second << endl;
			}
			MyFile.close();
			cout << "size= " << huffmanCode.size() << endl;
		}
		else if (x == 2) {
			string str;
			cout << "enter file name: " << endl;
			cin >> str;
			ifstream MyReadFile(str);
			string myText;
			string text;
			int length;
			int zeros;
			MyReadFile >> length;
			MyReadFile >> myText;
			text += myText;
			getline(MyReadFile, myText);

			while (getline(MyReadFile, myText)) {

				if (myText == "") {
					getline(MyReadFile, myText);
					huffmanCode_decompress.emplace(myText,'\n');
					continue;
				}
				string code;
				for (int i = 1; i < myText.length(); i++) {
					code += myText[i];
				}
				huffmanCode_decompress.emplace(code, myText[0]);
			}
			MyReadFile.close();
			encodedText = text;
			vector<char> encoded;
			for (int i = 0; i < encodedText.length(); i++) {
				encoded.push_back(encodedText[i]);
			}
			string temp = "";
			unordered_map<string, char>::iterator it;
			while (!encoded.empty()) {
				temp += encoded.front();
				encoded.erase(encoded.begin());
				it = huffmanCode_decompress.find(temp);
				if (it != huffmanCode_decompress.end()) {
					decodedText += it->second;
					temp = "";
				}
				
			}
			cout << decodedText << endl;
			ofstream MyFile("decoded.txt");
			MyFile << decodedText << endl;
		}
		else if (x == 3) {
			exit(0);
		}else
			cout << "invalid input";

		
	}
	return 0;
}

