#include <stdio.h>
#include <stdlib.h>

typedef struct myStruct {
  int val;
  struct myStruct * next;
  struct myStruct * tail;
}
myStruct; //

myStruct * root; //global root pointer
void addElement(int val) {// this inserts from the head
  myStruct * add = malloc(sizeof(myStruct)); // element malloc // allocate memory
  add -> val = val; // vali ekledim // initing val struct here
  add -> tail = add;// add to the tail
  add -> next = root; //bunun nextini root yapt�m// seting add's next to root
  root = add; // bunu da add e�itledim �uan 20 ekledi�imizi var sayal�m 20->NULL �eklinde �zerine 40 ekledi�imizde  40->20->NULL B�R SONRAK� 60 OLSUN 60->40->20->NULL // root= add
}
void AddToTheTail(int val) {// this inserts to the tail
  myStruct * add = malloc(sizeof(myStruct));
  add -> val = val;
  add -> next = NULL;// i inited add 
  add -> tail = add;
  if (root == NULL) {// first time adding to root
    root = add;
    root -> tail = root;
    return;
  }
  root -> tail->next  = add; // making tail next to add
  root -> tail =root->tail->next ;// updating tail
}
void DeleteElement(int target) { //1.derecesinide yaz
  if(root==NULL)
  	return;
  myStruct ** pStart = &root; //ptr to ptr kulland�m
  myStruct * temp; //temp NULL:
  while (( * pStart) -> val != target) { //DEGER targete e�it olana kadar dolas.
    temp = ( * pStart);
    pStart = & ( * pStart) -> next;
    if (  *pStart == NULL) { // bos olursa fonksiyonu terket 
      return;
    }
  }
  if (( * pStart) -> next == NULL) {
    root -> tail = temp;
  }
  temp = * pStart; // temp i�inde silincek de�eri tut.// store the target's address inside temp which will be deleted
  * pStart = ( * pStart) -> next; //�imdi as�l kilit nokta bu listeyi biz 2. de�erden ald�k �imdi X -> X kutucuk yani biz ramdeki adresi g�steriyoruz burada(heap) bu y�zden burada yapt���m�z i�ler kal�c� �imdi burada next diyerek listeden target de�eri kestim.
  // en: changing our pointer to pointer to its next by doing that we update de list
  free(temp); // tempte tuttu�um haf�za de�erini free ile  shared memorye b�rakt�m.//realese the memory
}

void main() {
  addElement(30);
  AddToTheTail(100);
  AddToTheTail(50);
  AddToTheTail(25);
  AddToTheTail(1000);
  AddToTheTail(152);
  addElement(70);
  DeleteElement(152);
  AddToTheTail(200);
  DeleteElement(1193);
  myStruct * pStart = root;
  while (pStart != NULL) {
    printf(" %d->", pStart -> val);
    pStart = pStart -> next;
  }
}
