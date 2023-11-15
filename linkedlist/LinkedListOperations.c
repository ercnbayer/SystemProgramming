#include <stdio.h>
#include <stdlib.h>

typedef struct TestStruct {
  int val;
  struct TestStruct * next;
  struct TestStruct* tail;
}
TestStruct; //

TestStruct * g_pRoot; //global root pointer
void addElement(int val) {// this inserts from the head
  TestStruct * add = malloc(sizeof(TestStruct)); // element malloc // allocate memory
  add -> val = val; // vali ekledim // initing val struct here
  add -> tail = add;// add to the tail
  add -> next = g_pRoot; //bunun nextini root yaptým// seting add's next to root
  g_pRoot = add; // bunu da add eþitledim þuan 20 eklediðimizi var sayalým 20->NULL þeklinde üzerine 40 eklediðimizde  40->20->NULL BÝR SONRAKÝ 60 OLSUN 60->40->20->NULL // root= add
  // EN: imagine we added 20 so it's now 20->NULL after that if we add 40  it would be 40->20->NULL
}
void AddToTheTail(int val) {// this inserts to the tail
  TestStruct * add = malloc(sizeof(TestStruct));
  add -> val = val;
  add -> next = NULL;// i inited add 
  add -> tail = add;
  if (g_pRoot == NULL) {// first time adding to root
    g_pRoot = add;
    return;
  }
  g_pRoot ->tail->next  = add; // making tail next to add
  g_pRoot -> tail =g_pRoot->tail->next ;// updating tail
}
void DeleteElement(int target) { //1.derecesinide yaz
  if(g_pRoot==NULL)
  	return;
  TestStruct ** pStart = &g_pRoot; //ptr to ptr kullandým
  TestStruct * temp; //temp NULL:
  while (( * pStart) -> val != target) { //DEGER targete eþit olana kadar dolas.
    temp = ( * pStart);
    pStart = & ( * pStart) -> next;
    if (  *pStart == NULL) { // bos olursa fonksiyonu terket 
      return;
    }
  }
  if (( * pStart) -> next == NULL) {
    g_pRoot -> tail = temp;
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
  TestStruct * pStart = g_pRoot;
  while (pStart != NULL) {
    printf(" %d->", pStart -> val);
    pStart = pStart -> next;
  }
}
