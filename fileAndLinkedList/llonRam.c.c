#include <stdio.h>

#include <stdlib.h>

#include <string.h>

typedef struct Person {
  char szName[51];
  int index;
  int iPrev;
  int iNext;
}Person;

typedef struct PersonList{
	char szName[51];
	struct PersonList *next;
	 struct PersonList *tail;
}PersonList;

void AddToTheTail(PersonList**root,char*szName){
	
	PersonList *add=malloc(sizeof(PersonList));
	
	strcpy(add->szName,szName);
	add->next=NULL;
	add->tail=add;
	
	if(*root==NULL){  
		*root=add;
		return;
	}
	
	(*root)->tail->next=add;	
	(*root)->tail=(*root)->tail->next;
	
}
int g_rootIndex = 1;

void TestWrite() {
  FILE * pTarget = fopen("dev.cdt", "wb");
  
  if(pTarget ==NULL){
  		return;
  }
  
  Person root={0};

  
  strcpy(root.szName, "ERCAN");
  root.index = 1;
  root.iPrev = 0;
  root.iNext = 2;
  
  fwrite( & root, sizeof(Person), 1, pTarget);
  
  memset(root.szName, 0, 51);
  strcpy(root.szName, "TALHA");
  root.index = 2;
  root.iPrev = 1;
  root.iNext = 3;
  
  fwrite( & root, sizeof(Person), 1, pTarget);
  
  memset(root.szName, 0, 51);
  strcpy(root.szName, "UGUR");
  root.index = 3;
  root.iPrev = 2;
  root.iNext = 4;
  
  fwrite( & root, sizeof(Person), 1, pTarget);
  
  memset(root.szName, 0, 51);
  strcpy(root.szName, "MERTCAN");
  root.index = 4;
  root.iPrev = 3;
  root.iNext = 5;
  
  fwrite( & root, sizeof(Person), 1, pTarget);
  strcpy(root.szName, "HALIL");
  root.index = 5;
  root.iPrev = 4;
  root.iNext = 0;
  
  fwrite( & root, sizeof(Person), 1, pTarget);
  
  fclose(pTarget);
}


void InsertIndex2(int index, char * name, int index2) {
	
  printf("\n\n****NAME IS:%s\n", name);
  
  if (index < 1)
    index = 1;
    
  Person stRead;
  
  stRead.index = -1;

  Person Add;
  
  memset(Add.szName, 0, 51);
  strcpy(Add.szName, name);
  Add.index = index2; //Add'e index verdik.
  
  FILE * pTarget = fopen("dev.cdt", "rb+");
  
  if (pTarget == NULL) {
    return;
  }
  
  fseek(pTarget, sizeof(Person) * (g_rootIndex - 1), 0);
  
  if (g_rootIndex == index) { //roota denk gelirse
    g_rootIndex = index2; // dosyayı linked listmiş gibi okumaya çalıştığım  için rootu değiştiriyorum birdahki eklemede rootta başlasın
  }
  
  int iReaded = 1;
  
  while (stRead.index != index) { // aslında direk lak diye hedef indexe gidebilirim ama ödevin olayı dosyada linked list gibi dolasmak olduğu için böyle yaptım.

    iReaded = fread( & stRead, sizeof(Person), 1, pTarget);
    if (iReaded == 0) {
      //taile eklemeye çalışılıyor. bunu implemente etmedim. sadece olan indexlerin üstüne yazılabilir.
      fclose(pTarget);
      return;
    }

    if (stRead.iNext) // son indexi yerinden etmek istiyorsa hareket etme.	
      fseek(pTarget, sizeof(Person) * ((stRead.iNext) - 1), 0);

  }
  
  Add.iPrev = stRead.iPrev;
  Add.iNext = stRead.index;
  stRead.iPrev = Add.index;
  
  
  fseek(pTarget, -sizeof(Person), SEEK_CUR);  //previ düzeltiyorum.//adjusting prev
  
  fwrite( & stRead, sizeof(Person), 1, pTarget);
  
  if (Add.iPrev) {

    fseek(pTarget, ((Add.iPrev) - 1) * sizeof(Person), 0); //previnin nextini düzelteceğim.//adjusting prev's next
    
	fread( & stRead, sizeof(Person), 1, pTarget);
    
	stRead.iNext = Add.index;
    
	fseek(pTarget, -sizeof(Person), SEEK_CUR);
   
    fwrite( & stRead, sizeof(Person), 1, pTarget);
  }
  
  fseek(pTarget,0,SEEK_END);
  

  fwrite( & Add, sizeof(Person), 1, pTarget);
  fclose(pTarget);
}
void main() {
	
  TestWrite();
  
  InsertIndex2(3, "Taha", 6);
  InsertIndex2(5, "YUNUS", 7);
  InsertIndex2(1, "messi", 8);
  InsertIndex2(5, "neymar", 9);
  
  FILE * pTarget = fopen("dev.cdt", "rb");
  
  if (pTarget == NULL) {
    return;
  }
  
  Person stRead;
     PersonList *pRoot=NULL;
  
  int iReaded = 1;
  
  printf("Dosya icinde siralama En:Order in document \n");
  
  while (iReaded) {
  	
    iReaded = fread( & stRead, sizeof(Person), 1, pTarget);
    
	if (iReaded == 0)
      break;
    
	printf(" Name:%s INDEX:%d PREV:%d NEXT:%d \n", stRead.szName, stRead.index, stRead.iPrev, stRead.iNext);
	
  }
  
  fseek(pTarget, (g_rootIndex - 1) * sizeof(Person), 0);//cursoru roota çekiyorum.//seeking to root
  
  iReaded = 1;
  
  while (iReaded) {//visiting linked list //starting from root 
  	
    iReaded = fread( & stRead, sizeof(Person), 1, pTarget);
    
    if (iReaded == 0) {
      break;
    }
    
    printf("%s %d -> ", stRead.szName, stRead.index);
    
    AddToTheTail(&pRoot,stRead.szName);
    
    fseek(pTarget, sizeof(Person) * ((stRead.iNext) - 1), SEEK_SET);
    
  }
  
  printf("\n l.list on ram \n");
  
  while(pRoot){
  	
  	printf("%s \n",pRoot->szName);
  	
  	pRoot=pRoot->next;
  	
  }
  
  fclose(pTarget);

}