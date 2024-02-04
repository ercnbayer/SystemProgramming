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
  add -> next = g_pRoot; //bunun nextini root yapt�m// seting add's next to root
  g_pRoot = add; // bunu da add e�itledim �uan 20 ekledi�imizi var sayal�m 20->NULL �eklinde �zerine 40 ekledi�imizde  40->20->NULL B�R SONRAK� 60 OLSUN 60->40->20->NULL // root= add
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
  TestStruct ** pStart = &g_pRoot; //ptr to ptr kulland�m
  TestStruct * temp; //temp NULL:
  while (( * pStart) -> val != target) { //DEGER targete e�it olana kadar dolas.
    temp = ( * pStart);
    pStart = & ( * pStart) -> next;
    if (  *pStart == NULL) { // bos olursa fonksiyonu terket 
      return;
    }
  }
  if (( * pStart) -> next == NULL) {
    g_pRoot -> tail = temp;
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
  TestStruct * pStart = g_pRoot;
  while (pStart != NULL) {
    printf(" %d->", pStart -> val);
    pStart = pStart -> next;
  }
}
