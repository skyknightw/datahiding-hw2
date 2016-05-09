#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <cmath>
#include <fstream>
#include <stdlib.h>
#include<string>
#include <time.h>

using namespace cv;
using namespace std;

#define SEC_NUM5 13107
#define SEC_NUM25 65536
#define SEC_NUM50 131072
#define TOT_SIZE 262144

int sec5[13107] = { 0 };//secret5%
int sec25[65536] = { 0 };//secret25%
int sec50[131072] = { 0 };//secret50%

int walk_ord5[262144] = { 0 };//walk order5%
int walk_ord25[262144] = { 0 };//walk order25%
int walk_ord50[262144] = { 0 };//walk order50%

int ext5[13107] = { 0 };//extract5%
int ext25[65536] = { 0 };//extract25%
int ext50[131072] = { 0 };//extract50%

void lsb_emb5(){
	int counter = 0;
	int tmp_walk = 0;
	srand((unsigned)time(NULL));

	for (int i = 0; i < SEC_NUM5; ++i){//secret
		sec5[i] = (rand() % 2 ) * 2 - 1;
		//cout << sec5[i] << " ";
	}
	//cout << endl;
	//walk ordering
	while (counter < SEC_NUM5){
		tmp_walk = ((rand() * 17 + 3) % TOT_SIZE);
		if (walk_ord5[tmp_walk] == 0){
			walk_ord5[tmp_walk] = 1;
			counter++;
			//cout << tmp_walk << " ";
		}
	}
	//cout << endl;
}

void lsb_emb25(){
	int counter = 1;
	int tmp_walk = 0;
	srand((unsigned)time(NULL));

	for (int i = 0; i < SEC_NUM25; ++i){//secret
		sec25[i] = (rand() % 2) * 2 - 1;
		//cout << sec25[i] << " ";
	}
	cout << 0;
	//cout << endl;
	//walk ordering
	while (counter <= SEC_NUM25){
		walk_ord25[tmp_walk] = 1;
		tmp_walk = tmp_walk + (rand() % 4) + 1;
		counter++;
			//cout << tmp_walk << " ";
	}
	cout << counter;
}

void lsb_emb50(){
	int counter = 1;
	int tmp_walk = 0;
	srand((unsigned)time(NULL));

	for (int i = 0; i < SEC_NUM50; ++i){//secret
		sec50[i] = (rand() % 2) * 2 - 1;
		//cout << sec50[i] << " ";
	}
	//cout << endl;
	//walk ordering
	while (counter <= SEC_NUM50){
		walk_ord50[tmp_walk] = 1;
		tmp_walk = tmp_walk + (rand() % 2) + 1;
		counter++;
			//cout << tmp_walk << " ";
	}
	//cout << endl << counter ;
}


int main(){
	fstream file;
	file.open("pixel.txt", ios::out);
	char* imageName = "1.jpg";

	Mat image;//���
	image = imread(imageName);
	cout << "height: " << image.rows << " ,weidth: " << image.cols << endl;

	Mat emb5_img, emb25_img, emb50_img;
	image.assignTo(emb5_img);
	image.assignTo(emb25_img);
	image.assignTo(emb50_img);

	lsb_emb5();
	lsb_emb25();
	lsb_emb50();
	
	for (int i = 0 , j=0 ; j < SEC_NUM5;i++){//embedding5%
		if (walk_ord5[i] == 1){
			if ((sec5[j] == 1) && (image.data[i] % 2 != 1)){
				emb5_img.data[i] = image.data[i] + 1;
			}
			else if ((sec5[j] == -1) && (image.data[i] % 2 != 0)){
				emb5_img.data[i] = image.data[i] - 1;
				}
			j++;
		}
		file << (int)emb5_img.data[i] << endl;
	}
	imwrite("emb5_img.jpg", emb5_img);

	for (int i = 0, j = 0; j < SEC_NUM5; i++){//extract5%
		if (walk_ord5[i] == 1){
			ext5[j] = (emb5_img.data[i] % 2) * 2 - 1;
			//cout << ext5[j] << " ";
			j++;
		}
	}
	//cout << endl;

	float count5 = 0; //check 5%
	for (int i = 0; i < SEC_NUM5; i++){
		if (sec5[i] == ext5[i]) count5++;
	}
	cout << "5% embed correct rate:" << count5 / SEC_NUM5 * 100 << "%" << endl;
	
	
	for (int i = 0, j = 0; j < SEC_NUM25; i++){//embedding25%
		if (walk_ord25[i] == 1){
			if ((sec25[j] == 1) && (image.data[i] % 2 != 1)){
				emb25_img.data[i] = image.data[i] + 1;
			}
			else if ((sec25[j] == -1) && (image.data[i] % 2 != 0)){
				emb25_img.data[i] = image.data[i] - 1;
			}
			j++;
			//cout << "O" ;
		}
		//cout << endl << i << " " ;
		file << (int)emb25_img.data[i] << endl;
	}
	imwrite("emb25_img.jpg", emb25_img);

	for (int i = 0, j = 0; j < SEC_NUM25; i++){//extract25%
		if (walk_ord25[i] == 1){
			ext25[j] = (emb25_img.data[i] % 2) * 2 - 1;
			//cout << ext25[j] << " ";
			j++;
		}
	}
	//cout << endl;

	float count25 = 0; //check 25%
	for (int i = 0; i < SEC_NUM25; i++){
		if (sec25[i] == ext25[i]) count25++;
	}
	cout << "25% embed correct rate:" << count25 / SEC_NUM25 * 100 << "%" << endl;
	
	for (int i = 0, j = 0; j < SEC_NUM50; i++){//embedding50%
		if (walk_ord50[i] == 1){
			if ((sec50[j] == 1) && (image.data[i] % 2 != 1)){
				emb50_img.data[i] = image.data[i] + 1;
			}
			else if ((sec50[j] == -1) && (image.data[i] % 2 != 0)){
				emb50_img.data[i] = image.data[i] - 1;
			}
			j++;
		}
		file << (int)emb50_img.data[i] << endl;
	}
	imwrite("emb50_img.jpg", emb50_img);
	
	for (int i = 0, j = 0; j < SEC_NUM50; i++){//extract50%
		if (walk_ord50[i] == 1){
			ext50[j] = (emb50_img.data[i] % 2) * 2 - 1;
			//cout << ext50[j] << " ";
			j++;
		}
	}
	//cout << endl;
	
	float count50 = 0; //check 50%
	for (int i = 0; i < SEC_NUM50; i++){
		if (sec50[i] == ext50[i]) count50++;
	}
	cout << "50% embed correct rate:" << count50 / SEC_NUM50 * 100 << "%" << endl;
	

	//namedWindow(imageName, CV_WINDOW_AUTOSIZE);
	namedWindow("Gray image", CV_WINDOW_AUTOSIZE);

	//imshow(imageName, image);
	imshow("Gray image", emb50_img);

	waitKey(0);//�Ȱ��ɶ�msec

	image.release();//release

	return 0;
	
}
