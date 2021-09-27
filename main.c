#include <stdio.h>
#include <stdlib.h>
#include "sim.h"
#include <math.h>
#include "rngs.h"
#include <string.h>
#include <time.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void arrival();
void departure();
void initial();
void impatientFunc();
double system_num = 0;
node* FEL, CUS_QUE;
node* FEL_impa;
struct node* nextNodePtr;
double lambda, mu, impa;
double mean, now_time, previous_time;
char* arr = "arr";
char* dep = "dep";
char* impaStr = "impa";
double nextDep_time;
double impaArr[100];

int main(int argc, char *argv[]) {
	
	lambda = atof(argv[1]);
	mu = atof(argv[2]);
	impa = atof(argv[3]);
	
	for(int i = 0; i < 100; i++){
		impaArr[i] = 0;
	}

	//printf("%6s%6s%15s%15s\n", "lambda", "mu", "theoretical", "simulation");
	for(int j = 0; j < 1; j++){
		initial();
		
		for(int i = 0; i < 20000; i++){
			printf("系統人數：%f\n", system_num);
			printLists(FEL);
			
			FEL = removeNode(FEL, &nextNodePtr);
			now_time = nextNodePtr->clock;
			
			mean = mean + (now_time - previous_time) * system_num;
			
			if(nextNodePtr->type == arr){
				arrival();
			}else if(nextNodePtr->type == dep){
				departure();
			}else{
				impatientFunc();
			}
			previous_time = now_time;
		}
		mean = mean / now_time;
		double theoValue = lambda / (mu - lambda);
		printf("%6.2f%6.0f%15f%15f\n", lambda, mu, theoValue, mean);
		lambda += 0.05;
	}
	
	return 0;
}

void arrival(){
	system_num++;
	double nextArr_time = now_time + expTest(1, lambda);
	node* FEL_arr = createNode(arr, nextArr_time);
	FEL = insertNode(FEL, FEL_arr);
			
	if(system_num == 1){
		nextDep_time = now_time + expTest(2, mu);
		node* FEL_dep = createNode(dep, nextDep_time);
		FEL = insertNode(FEL, FEL_dep);
	}else{
		/*
		如果系統有一個人以上
		就要給這個arr一個不耐煩時間
		用一個array去紀錄每一筆arr的不耐煩時間
		然後判斷正在系統裡的事件的dep time跟不耐煩時間的先後順序
		如果不耐煩事件的時間小於dep那就會插入到FEL裡
		相反就不插入
		*/
		double impa_time = now_time + expTest(3, impa);
		
		int cus = system_num - 2;
		impaArr[cus] = impa_time;
		printf("不耐煩時間：%f 等待人數：%d\n", impa_time, cus);
		printf("%f %f\n", impaArr[0], nextDep_time);

		if(cus == 0 && impaArr[0] < nextDep_time){
			//printf("%f %f\n", impaArr[0], nextDep_time);
			FEL_impa = createNode(impaStr, impaArr[0]);
			FEL = insertNode(FEL, FEL_impa);
		}
	}
}

void departure(){
	system_num--;
	if(system_num >= 1){
		nextDep_time = now_time + expTest(2, mu);
		node* FEL_dep = createNode(dep, nextDep_time);
		FEL = insertNode(FEL, FEL_dep);

		for(int i = 0; i < 99; i++){
			impaArr[i] = impaArr[i+1];
		}

		if(system_num > 1 && impaArr[0] < nextDep_time){
			FEL_impa = createNode(impaStr, impaArr[0]);
			FEL = insertNode(FEL, FEL_impa);
		}
	}
}

void impatientFunc(){
	srand(time(NULL));
	double probability = rand() % 2;
	printf("機率：%f\n", probability);
	if(probability < 0.5){
		system_num--;
		for(int i = 0; i < 99; i++){
			impaArr[i] = impaArr[i+1];
		}
	}
}


void initial(){
	FEL = NULL;
	system_num = 0;
	mean = 0;
	now_time = 0;
	previous_time = 0;
	FEL = createNode(arr, 0.0);
	nextNodePtr = NULL;
}

