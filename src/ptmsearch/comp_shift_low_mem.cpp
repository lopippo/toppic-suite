/*
 * comp_shift_low_mem.cpp
 *
 *  Created on: Dec 27, 2013
 *      Author: xunlikun
 */

#include <ptmsearch/comp_shift_low_mem.hpp>

namespace prot {

CompShiftLowMem::CompShiftLowMem(){
	for(unsigned int i=0;i< max_len_;i++){
		num_.push_back(0);
	}
}

std::vector<std::vector<int>> CompShiftLowMem::findBestShift(std::vector<int> a,std::vector<int> b){
	return findBestShift(a,b,1,0);
}
std::vector<double> CompShiftLowMem::findBestShift(std::vector<int> a,std::vector<int> errors,std::vector<int> b,int total,int minimum_gap,double scale){
	std::vector<std::vector<int>> list = findBestShift(a,errors,b,total,minimum_gap);
	std::vector<double> result;
	for(unsigned int i = 0;i<list.size();i++){
		result.push_back(list[i][0]/scale);
	}
	return result;
}
std::vector<std::vector<int>> CompShiftLowMem::findBestShift(std::vector<int> a,std::vector<int> errors,std::vector<int> b,int total,int minimum_gap){
	const int a_length = a.size();
	const int b_length = b.size();
	std::vector<std::vector<int>> ans;
	if(b_length == 0){
		std::vector<int> temp;
		temp.push_back(0);
		temp.push_back(0);
		ans.push_back(temp);
		return ans;
	}
	const int delta = a[0]-b[b_length-1];
	const int minD = delta-1;
	const int maxD = a[a_length-1]-b[0]+1+errors[a_length-1];
	if(maxD-minD>num_.size()){
		int required_length = maxD-minD+1;
		num_.clear();
		for(int i=0;i<required_length;i++){
			num_.push_back(0);
		}
	}

	for(int i=0;i<a_length;i++){
		int a_value = a[i];
		for(int j =0;j <b_length;j++){
			int d = a_value - b[j]-delta;
			int start = 0;
			if(d >errors[i]){
				start = d- errors[i];
			}
			for(int k = start;k<=d+errors[i];k++){
				num_[k]=0;
			}
		}
	}

	int current_minimum =1;

	for(int i=0;i<a_length;i++){
		int a_value = a[i];
		for(int j =0;j <b_length;j++){
			int d = a_value - b[j]-delta;
			int start = 0;
			if(d >errors[i]){
				start = d- errors[i];
			}
			for(int k = start;k<=d+errors[i];k++){
				num_[k]++;
				current_minimum = checkD(ans,k,current_minimum,total,minimum_gap);
			}
		}
	}

	for(int i=0;i<ans.size();i++){
		ans[i][0] += minD;
	}

	return ans;
}
std::vector<std::vector<int>> CompShiftLowMem::findBestShift(std::vector<int> a,std::vector<int> b,int total,int minimum_gap) {
	const int a_length = a.size();
	const int b_length = b.size();
	std::vector<std::vector<int>> ans;
	const int delta = a[0]-b[b_length-1];
	const int minD = delta-1;
	const int maxD = a[a_length-1]-b[0]+1;
	if(maxD-minD >num_.size()){
		int required_length = maxD-minD+1;
		num_.clear();
		for(int i=0;i<required_length;i++){
			num_.push_back(0);
		}
	}

	for(int i=0;i<a_length;i++){
		int a_value = a[i];
		for(int j =0;j <b_length;j++){
			int d = a_value - b[j]-delta;
			num_[d] =0;
			num_[d+1] =0;
			num_[d+2] = 0;
		}
	}

	for(int i=0;i<a_length;i++){
		int a_value = a[i];
		for(int j =0;j <b_length;j++){
			int d = a_value - b[j]-delta;
			num_[d] ++;
			num_[d+1] ++;
			num_[d+2] ++;
		}
	}

	int current_minimum =1;

	for(int i=0;i<a_length;i++){
		int a_value = a[i];
		for(int j =0;j <b_length;j++){
			int d = a_value - b[j]-delta;
			current_minimum=checkD(ans,d+1,current_minimum,total,minimum_gap);
			current_minimum=checkD(ans,d,current_minimum,total,minimum_gap);
			current_minimum=checkD(ans,d+2,current_minimum,total,minimum_gap);
		}
	}

	for(unsigned int i=0;i<ans.size();i++){
		ans[i][0] +=minD;
	}
	return ans;
}
int CompShiftLowMem::checkD(std::vector<std::vector<int>> &ans,int d,int currentMinimum,int total,int minimumu_gap){
	short new_value = num_[d];
	if(new_value < currentMinimum){
		return currentMinimum;
	}

	for(unsigned int i =0;i<ans.size();i++){
		std::vector<int>  cur = ans[i];
		if(std::abs(cur[0]-d)<= currentMinimum){
			if(cur[1]<new_value){
				ans.erase(ans.begin()+i,ans.begin()+i+1);
			}
			else {
				return currentMinimum;
			}
		}
	}

	int insert_pos = ans.size()-1;

	while(insert_pos >= 0 && ans[insert_pos][1] < new_value){
		insert_pos--;
	}
	std::vector<int> insert_temp = {d,new_value};
	ans.insert(ans.begin()+insert_pos, insert_temp);
	if(ans.size()>total){
		ans.pop_back();
	}
	return ans.size()==total?ans[ans.size()-1][1]+1:1;
}

} /* namespace prot */