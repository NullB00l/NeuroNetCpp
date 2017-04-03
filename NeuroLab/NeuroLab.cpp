#include "stdafx.h"
#include <stdio.h>
#include<time.h>
#include<iostream>
#include<math.h>
//#include<Windows.h>
using namespace std;
//#using<System.dll>
//extern "C" __declspec(dllexport) int MyFunc(const int it);

class Neuron{
public:
	double *w = 0;
	double b = 0.0;
	double s = 0.0;
	double y = 0.0;
	double err = 0.0;

	int w_count = 0;
};

class NeuroLab{
protected:
	static double THAN(double summ){
		return (_CMATH_::exp(2 * summ) - 1) / (_CMATH_::exp(2 * summ) + 1);
	}
	static double THAN_dw(double y){
		return 1 - y * y;
	}

	static double BISIGMOID(double summ){
		return 2 / (1 + _CMATH_::exp(-summ)) - 1;
	}
	static double BISIGMOID_dw(double y){
		return (2 * _CMATH_::exp(-y)) / ((1 + _CMATH_::exp(-y)) * (1 + _CMATH_::exp(-y)));
		//(alpah * 2 * _CMATH_::exp(-y * alpha)) / ((1 + _CMATH_::exp(-y* alpha)) * (1 + _CMATH_::exp(-y* alpha)));
	}
public:
	NeuroLab(){}

	#pragma region net structure:
	int epoch_count = 60;//count of training epoches
	int input_count = 10000;
	int input_vector_count = 20;
	double learning_rate = 0.3;
	//double L2_reg = 0.01;
	double L3_reg = 0.0;
	int output_count = 1;
	int layers_count = 2;//number of hidden layers
	int *neuron_per_layer = new int[layers_count]{ 20, output_count };//NET STRUCTURE
	//double alpha = 1.0;
	#pragma endregion

	Neuron **net = new Neuron*[layers_count];
	double err = 0.0;
	double *err_per_iteration = new double[input_count * epoch_count];
	double **input = new double*[input_count];
	double **output = new double*[input_count];

	double(*act_func)(double) = BISIGMOID;
	double(*act_func_dw)(double) = BISIGMOID_dw;
	//double(*act_func)(double) = THAN;
	//double(*act_func_dw)(double) = THAN_dw;

	void CREATE(){
		//inputs 0 outputd
		for(int i = 0; i < input_count; i++){
			input[i] = new double[input_vector_count];
			output[i] = new double[output_count];
			for(int j = 0; j < input_vector_count; j++){
				input[i][j] = _CMATH_::sin(((double)(j + i) / 180.0) * 3.14159265);
			}
			for(int j = 0; j < output_count; j++){
				output[i][j] = _CMATH_::sin(((double)(input_vector_count + j + i) / 180.0) * 3.14159265);
			}
		}

		net[0] = new Neuron[neuron_per_layer[0]];
		for(int n = 0; n < neuron_per_layer[0]; n++){
			net[0][n].w = new double[input_vector_count];
			net[0][n].w_count = input_vector_count;

			for(int w = 0; w < input_vector_count; w++){
				net[0][n].w[w] = ((double)(rand() % 1000) / 1000.0) * 0.04 - 0.02;
			}
			net[0][n].b = ((double)(rand() % 1000) / 1000.0) * 0.04 - 0.02;
		}

		for(int l = 1; l < layers_count; l++){
			net[l] = new Neuron[neuron_per_layer[l]];
			for(int n = 0; n < neuron_per_layer[l]; n++){
				net[l][n].w = new double[neuron_per_layer[l - 1]];
				net[l][n].w_count = neuron_per_layer[l - 1];

				for(int w = 0; w < neuron_per_layer[l - 1]; w++){
					net[l][n].w[w] = ((double)(rand() % 1000) / 1000.0) * 0.04 - 0.02;
				}
				net[l][n].b = ((double)(rand() % 1000) / 1000.0) * 0.04 - 0.02;
			}
		}
	}
	void FORWARD(int iteration, int epoch){
		//if(iteration > 90){
		//	int u = 0;
		//}

		for(int i = 0; i < neuron_per_layer[0]; i++){
			net[0][i].s = 0.0;
			for(int j = 0; j < input_vector_count; j++){
				net[0][i].s += net[0][i].w[j] * input[iteration][j];
			}
			net[0][i].s += net[0][i].b;
			net[0][i].y = act_func(net[0][i].s);
			
			//if(isnan(net[0][i].y)){
			//	int u = 0;
			//}
		}
		
		for(int l = 1; l < layers_count; l++){
			for(int i = 0; i < neuron_per_layer[l]; i++){
				net[l][i].s = 0.0;
				for(int j = 0; j < neuron_per_layer[l - 1]; j++){
					net[l][i].s += net[l][i].w[j] * net[l - 1][j].y;
				}
				net[l][i].s += net[l][i].b;
				net[l][i].y = act_func(net[l][i].s);
			}
		}

		//error
		double _err = 0.0;
		for(int i = 0; i < neuron_per_layer[layers_count - 1]; i++){
			_err += net[layers_count - 1][i].y - output[iteration][i];
		}
		err_per_iteration[iteration + input_count * epoch] = _CMATH_::abs(_err);
		err += (_CMATH_::abs(_err) / (double)neuron_per_layer[layers_count - 1]);
	}
	void BACKPROP(int iteration){
		if(layers_count > 1){
			//for output:
			for(int n = 0; n < neuron_per_layer[layers_count - 1]; n++){
				net[layers_count - 1][n].err = (net[layers_count - 1][n].y - output[iteration][n]) * act_func_dw(net[layers_count - 1][n].y);
			}
			//for hidden:
			for(int l = layers_count - 2; l > 0; l--){
				for(int n = 0; n < neuron_per_layer[l]; n++){
					net[l][n].err = 0.0;
					for(int nUP = 0; nUP < neuron_per_layer[l + 1]; nUP++){
						net[l][n].err += net[l + 1][nUP].err * net[l + 1][nUP].w[n];// *act_func_dw(net[l][n].y);
					}
					net[l][n].err *= act_func_dw(net[l][n].y);
				}
			}
			//input:
			for(int n = 0; n < neuron_per_layer[0]; n++){
				net[0][n].err = 0.0;
				for(int nUP = 0; nUP < neuron_per_layer[1]; nUP++){
					net[0][n].err += net[1][nUP].err * net[1][nUP].w[n];// *act_func_dw(net[0][n].y);
				}
				net[0][n].err *= act_func_dw(net[0][n].y);
			}
		}
		else{//ONLY ONE OUTPUT=INPUT LAYER:
			for(int n = 0; n < neuron_per_layer[0]; n++){
				net[0][n].err = (net[0][n].y - output[iteration][n]) * act_func_dw(net[0][n].y);
			}
		}
	}
	void UPDATE(int iteration){
		if(L3_reg > 0.0){
			//INPUT LAYER:
			for(int n = 0; n < neuron_per_layer[0]; n++){//>
				for(int w = 0; w < net[0][n].w_count; w++){
					net[0][n].w[w] -= net[0][n].err * input[iteration][w] * learning_rate - net[0][n].w[w] * L3_reg * _CMATH_::abs(net[0][n].err);
				}
				net[0][n].b -= net[0][n].err * learning_rate;
			}

			for(int l = 1; l < layers_count; l++){
				for(int n = 0; n < neuron_per_layer[l]; n++){//
					for(int w = 0; w < net[l][n].w_count; w++){
						net[l][n].w[w] -= net[l][n].err * net[l - 1][w].y * learning_rate - net[l][n].w[w] * L3_reg * _CMATH_::abs(net[l][n].err);
					}
					net[l][n].b -= net[l][n].err * learning_rate;
				}
			}
		}
		else{
			//INPUT LAYER:
			for(int n = 0; n < neuron_per_layer[0]; n++){//>
				for(int w = 0; w < net[0][n].w_count; w++){
					net[0][n].w[w] -= net[0][n].err * input[iteration][w] * learning_rate;
				}
				net[0][n].b -= net[0][n].err * learning_rate;
			}

			for(int l = 1; l < layers_count; l++){
				for(int n = 0; n < neuron_per_layer[l]; n++){//
					for(int w = 0; w < net[l][n].w_count; w++){
						net[l][n].w[w] -= net[l][n].err * net[l - 1][w].y * learning_rate;
					}
					net[l][n].b -= net[l][n].err * learning_rate;
				}
			}
		}

	}
	void TEACH(){

		CREATE();

		for(int e = 0; e < epoch_count; e++){
			for(int i = 0; i < input_count; i++){
				FORWARD(i, e);
				BACKPROP(i);
				UPDATE(i);
			}
		}
		DISPOSE();
	}
	void DISPOSE(){
		delete[] neuron_per_layer;	
		delete[] err_per_iteration;

		for(int l = 0; l < layers_count; l++){
			for(int n = 0; n < neuron_per_layer[l]; n++){
				delete[] net[l][n].w;
			}
		}

		for(int i = 0; i < layers_count; i++){
			delete[] net[i];
		}
		delete[] net;

		for(int i = 0; i < input_count; i++){
			delete[] input[i];
			delete[] output[i];
		}
		delete[] input;
		delete[] output;
	}
};

int main(){

	clock_t _tm = clock();
	std::cout << "START: " << _tm << "\r\n";

	NeuroLab lab;
	lab.TEACH();
	std::cout << "Iterations: " << (lab.epoch_count * lab.input_count) / 1000 << "K" << endl;
	std::cout << "Error avg: " << lab.err / (float)(lab.epoch_count * lab.input_count) << ";  Tatoal Err: " << lab.err << endl;
	_tm = clock() - _tm;
	std::cout << "PC FINISH: " << _tm << "\r\n";

	getchar();
	return 0;
}
