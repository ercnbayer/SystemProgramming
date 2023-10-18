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
  add -> next = root; //bunun nextini root yaptým// seting add's next to root
  root = add; // bunu da add eþitledim þuan 20 eklediðimizi var sayalým 20->NULL þeklinde üzerine 40 eklediðimizde  40->20->NULL BÝR SONRAKÝ 60 OLSUN 60->40->20->NULL // root= add
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
  myStruct ** pStart = &root; //ptr to ptr kullandým
  myStruct * temp; //temp NULL:
  while (( * pStart) -> val != target) { //DEGER targete eþit olana kadar dolas.
    temp = ( * pStart);
    pStart = & ( * pStart) -> next;
    if (  *pStart == NULL) { // bos olursa fonksiyonu terket 
      return;
    }
  }
  if (( * pStart) -> next == NULL) {
    root -> tail = temp;
  }
  temp = * pStart; // temp içinde silincek deðeri tut.// store the target's address inside temp which will be deleted
  * pStart = ( * pStart) -> next; //þimdi asýl kilit nokta bu listeyi biz 2. deðerden aldýk þimdi X -> X kutucuk yani biz ramdeki adresi gösteriyoruz burada(heap) bu yüzden burada yaptýðýmýz iþler kalýcý þimdi burada next diyerek listeden target deðeri kestim.
  // en: changing our pointer to pointer to its next by doing that we update de list
  free(temp); // tempte tuttuðum hafýza deðerini free ile  shared memorye býraktým.//realese the memory
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
