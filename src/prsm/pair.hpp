/*
 * pair.hpp
 *
 *  Created on: Jan 1, 2014
 *      Author: xunlikun
 */

#ifndef PAIR_HPP_
#define PAIR_HPP_

namespace prot {

class Pair;
typedef std::shared_ptr<Pair> PairPtr;
class Pair {
public:
	Pair(int x,int y){
		x_=x;
		y_=y;
	}
	int getX(){
		return x_;
	}
	int getY(){
		return y_;
	}
	void setX(int x){
		x_=x;
	}
	void setY(int y){
		y_=y;
	}

	int compareTo(PairPtr p2){
		int x= p2->getX();
		int y= p2->getY();
		if(y-y_<0){
			return 1;
		}
		else if(y-y_>0){
			return -1;
		}
		else if(x-x_<0){
			return 1;
		}
		else if(x-x_>0){
			return -1;
		}
		return 0;
	}

protected:
	int x_=0;
	int y_=0;
};

//bool increaseIJ(int i,int j,double deviation,double tolerace,std::vector<double> ms_masses,std::vector<double> seq_masses);
//std::vector<double> compPpoDeviation(std::vector<double> ms_masses,std::vector<double> theo_masses,double ppo);

} /* namespace prot */

#endif /* PAIR_HPP_ */
