#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

//Khai báo token trong stack và queue
struct Token{
	int kieu;
	int giaTri;
};

//khai báo list
struct Node{
	Token key;
	Node *pNext;
};

struct List{
	Node *pHead;
	Node *pTail;
};

Node* CreateNode(Token n){
	Node *t = new Node;

	t->key = n;
	t->pNext = NULL;
	return t;
}

List* CreateList(){
	List *l = new List;
	l->pHead = l->pTail = NULL;
	return l;
}

void AddHead(List *l, Token n){
	Node *nAdd = CreateNode(n);
	if (l->pHead == NULL){
		l->pHead = l->pTail = nAdd;
	}
	else{
		nAdd->pNext = l->pHead;
		l->pHead = nAdd;
	}
}

void AddTail(List *l, Token n){
	Node *nAdd = CreateNode(n);
	if (l->pHead == NULL){
		l->pHead = l->pTail = nAdd;
	}
	else{
		l->pTail->pNext = nAdd;
		l->pTail = nAdd;
	}

}

Node* RemoveHead(List *l){
	Node *temp;
	if (l->pHead == NULL){ //Trường hợp không có phần tử nào
		temp = NULL;
	}
	temp = l->pHead;
	l->pHead = l->pHead->pNext;
	temp->pNext = NULL;
	return temp;
}

void PushStack(List *st, Token n){//Thêm đầu để hồi lấy đầu => Stack
	AddHead(st, n);
}


Node* PopStack(List *l){//lấy đầu =>stack
	return RemoveHead(l);
}

void EnQueue(List *qu, Token n){//Thêm cuối để lấy đầu => Queue
	AddTail(qu, n);
}

Node* DeQueue(List *st){//Lấy đầu
	return RemoveHead(st);
}

bool IsEmpty(List *n){// check rỗng
	if (n->pHead == NULL)
		return true;
	return false;
}

Node* TopStack(List *st){// xem phần tử ở đỉnh, giống PopStack nhưng không xóa
	return st->pHead;
}

void PrintQueue(List *st){// xuất ra màn hình Stack
	Node *temp = NULL;
	for (temp = st->pHead; temp != NULL; temp = temp->pNext){
		if (temp->key.kieu == 1)
			cout << temp->key.giaTri;
		else if (temp->key.kieu == 2 || temp->key.kieu == 3)
			cout << (char)temp->key.giaTri;
		cout << " ";
	}
	cout << endl;
}

//bắt đầu lập trình các hàm trong kí pháp ba lan
bool CheckSuitStructures(char *a){
	for (unsigned int i = 0; i < strlen(a); i++){
		if (tolower(a[i]) <= 'z' && tolower(a[i]) >= 'a')
			return false;
	}
	if (strcmp(a, "") == 0)
		return false;
	return true;
}
int GetPriority(char s){
	if (s == '*' || s == '/' || s == '%')
		return 2;
	if (s == '+' || s == '-')
		return 1;
	return 0;
}

Token OpAdd(Token a, Token b){//Cộng 2 token là toán hạng
	Token result;
	result.giaTri = a.giaTri + b.giaTri;
	result.kieu = 1; // toán hạng
	return result;
}

Token OpSub(Token a, Token b){//trừ 2 token là toán hạng
	Token result;
	result.giaTri = a.giaTri - b.giaTri;
	result.kieu = 1; // toán hạng
	return result;
}

Token OpMul(Token a, Token b){//nhân 2 token là toán hạng
	Token result;
	result.giaTri = a.giaTri * b.giaTri;
	result.kieu = 1; // toán hạng
	return result;
}

Token OpDiv(Token a, Token b){// chia 2 token là toán tử
	Token result;
	result.giaTri = a.giaTri / b.giaTri;
	result.kieu = 1; // toán hạng
	return result;
}

Token ReadToken(char *str, char a, unsigned int &vt){
	Token result;
	unsigned int len = strlen(str);
	result.giaTri = 0;
	result.kieu = 0;//khởi tạo mặc định
	if (a >= '0' && a <= '9'){//Token là toán hạng: kiểu 1;
		result.giaTri = a - '0';
		while (vt < len){
			if (str[vt+1] >= '0' && str[vt+1] <= '9'){
				result.giaTri = result.giaTri * 10 + (str[vt+1]-'0');
				vt++;
			}
			else
				break;
		}
		result.kieu = 1;
	}
	else if (a == '+' || a == '-' || a == '*' || a == '/'){//Token là toán tử: kiểu 2;
		result.giaTri = (int)a;
		result.kieu = 2;
	}
	else if (a == '(' || a == ')'){//Token là dấu ngoặc: kiểu 3
		result.giaTri = (int)a;
		result.kieu = 3;
	}
	return result;
}

void ConvertMiddleToEnd(char *bieuThuc, List *&st, List *&qu){
	//-Khởi động Stack rỗng 
	st = CreateList();
	//-Khởi động Queue rỗng
	qu = CreateList();
	//while (chưa hết biểu thức) {
	unsigned int i = 0;
	while (i < strlen(bieuThuc)){
		Token tk = ReadToken(bieuThuc, bieuThuc[i], i); //	- Đọc một Token 
		//	- Nếu Token là : 
		if (tk.kieu == 3){// token là dấu ngoặc
			if (tk.giaTri == (int)'('){//Ngoặc trái : Push(Token, Stack).
				PushStack(st,tk);
			}
			else if (tk.giaTri == (int)')'){//Ngoặc phải :  (pop cả ngoặc trái nhưng không push nó vào Queue).
				while (true){//lặp lại thao tác Push(Pop(Stack), Queue) đến khi gặp ngoặc trái.
					Node *temp = PopStack(st);
					if (temp->key.kieu != 3){
						EnQueue(qu, temp->key);
					}
					if (temp->key.kieu == 3 && temp->key.giaTri == (int)'(')
						break;
				}
			}
		}
		if (tk.kieu == 2){//token là toán tử
			if (IsEmpty(st) == true){
				PushStack(st, tk);
			}
			else{
				while(GetPriority((char)tk.giaTri) < GetPriority((char)TopStack(st)->key.giaTri))
					EnQueue(qu, PopStack(st)->key);
				PushStack(st, tk);
			}
			
		}
		if (tk.kieu == 1){//token là toán hạng
			EnQueue(qu, tk); //Push(Token, Queue).
		}
		i++;
	}
	while (!IsEmpty(st)){//while (!isEmpty(Stack)) 
		EnQueue(qu, PopStack(st)->key);	//		Push(Pop(Stack), Queue)
	}
}

int CalculatePolishNotation(List *st, List *qu){
	st = CreateList();// khởi động stack rỗng
	while (IsEmpty(qu) != true){//while (!isEmpty(Queue)) {
		Token temp = DeQueue(qu)->key;// temp = Pop(Queue)
		if (temp.kieu == 1){// Nếu temp là toán hạng
			PushStack(st, temp);
		}
		else if (temp.kieu == 2){//Nếu temp là toán tử
			Token u = PopStack(st)->key;
			Token v = PopStack(st)->key;
			Token y;
			if ((char)temp.giaTri == '+'){// Thực hiện toán tử X cho 2 toán hạng trong Stack. 
				y = OpAdd(v, u);
			}
			else if ((char)temp.giaTri == '-'){
				y = OpSub(v, u);
			}
			else if ((char)temp.giaTri == '*'){
				y = OpMul(v, u);
			}
			else if ((char)temp.giaTri == '/'){
				y = OpDiv(v, u);
			}
			PushStack(st, y);// Thực hiện toán tử X cho 2 toán hạng trong Stack.
		}
	}
	return PopStack(st)->key.giaTri;//return  Pop(Stack);
}

int main(){
	char a[256];
	List *st;
	List *qu;
	do{
		cout << "Nhap bieu thuc can chuyen doi sang ki phap Ba Lan nguoc: ";
		cin.getline(a, 256);
	} while (CheckSuitStructures(a) == false);
	ConvertMiddleToEnd(a, st, qu);
	cout << "Ki phap Ba Lan nguoc: ";
	PrintQueue(qu);
	cout << "Gia tri cua bieu thuc la: " << fixed << CalculatePolishNotation(st, qu) << endl;
	return 0;
}