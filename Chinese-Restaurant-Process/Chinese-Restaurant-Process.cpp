#include<stdio.h>
#include<vector>
#include<stdlib.h>

//Chinese Restaurant Process

int main(){
	std::vector<int> table;
	table.resize(1);
	table[0] = 1;
	double alpha = 10;
	double prob = (double)rand()/((double)RAND_MAX);
	
	double tmp=0;
	double sum = 0;

	//合計の人数
	int N = 1000;
	//kは何人入ってくるか。
	for (int k=1;k<N;k++){
		prob = (double)rand()/((double)RAND_MAX);
		tmp = 0;
		sum = 0;

		for (int i=0;i<table.size();i++){
			sum += (double)table[i];
		}

		//new table
		if(prob>sum/(sum+alpha)){
			table.push_back(1);
		}
		//existing table
		else{
			for (int i=0;i<table.size();i++){
				tmp +=(double)table[i]/(double)(sum+alpha);
				if(tmp>prob){
					table[i]+=1;
					break;
				}
			}
		}
	}

	printf("table num = %d\n",table.size());
}