// vamEmBoas.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
//
#include <chrono>
#include <stdint.h>
#include <iostream>
#include <random>
#include <vector>
#include <cstdio>
#include "Hashing.h"
#pragma warning(disable:4996)//disable some vs safety warnings

using namespace std;

int64_t high(int64_t x,int8_t half_size) {
	return int64_t(x) >> half_size;

}

int64_t lower(int64_t x,int8_t half_size) {
	return x&(int64_t(1) << half_size) - 1;
}

int64_t join(int64_t c, int64_t i,int8_t half_size) {
	return (c << half_size) | i;
}



struct VEB {
	
	int64_t* min = nullptr;
	int64_t max;
	VEB* resumo = nullptr;
	int8_t hf;
	hashtable<item<VEB>,VEB> cluster;

	//create table to be used by simple hashing
	VEB() {
		hf = 32;
	}
	VEB(int8_t size) :hf(size) {};
	~VEB() {
		delete min;
		delete resumo;
		cluster.~hashtable();
	}

	void incluir(int64_t x) {
		if (min == nullptr) {
			min = new int64_t(x);
			max = x;
		}
		else {
			if (x == *(min))return;
			if (x < *(min)) {
				int temp = x;
				x = *(min);
				*(min) = temp;
			}
			if (hf > 1) {
				if (cluster.buscar(high(x, hf)) == nullptr) {
					cluster.incluir(high(x, hf), new VEB(hf >> 1), true);
				}
				if (cluster.buscar(high(x, hf))->min == nullptr) {
					if (resumo == nullptr) resumo = new VEB(hf >> 1);
					(*resumo).incluir(high(x, hf));
				}
				cluster.buscar(high(x, hf))->incluir(lower(x, hf));
			}
			if (x > max) {
				max = x;
			}
		}

	}

	int64_t* predecessor(int64_t x) {
		int64_t* out;
		if (min == nullptr) return nullptr;
		else if (hf==1) {
			if (x == 1 && *(min) == 0) {
				out = new int64_t(0);
				return out;
			}
			else return nullptr;
		}
		else if (x > max) {
			out = new int64_t(max);
			return out;
		}
		else if (x < *(min)) return nullptr;
		else if (cluster.buscar(high(x,hf))!=nullptr&&lower(x,hf) > *(cluster.buscar(high(x,hf))->min)) {
			int64_t p = join(high(x,hf), *(cluster.buscar(high(x,hf))->predecessor(lower(x,hf))),hf);
			out = new int64_t(p);
			return out;
		}
		int64_t c1 = *(resumo->predecessor(high(x,hf)));
		out = new int64_t(join(c1, cluster.buscar(c1)->max,hf));
		return out;
	}

	int64_t* sucessor(int64_t x) {
		int64_t* out;
		if (min == nullptr) return nullptr;
		else if (hf == 1) {
			if (x == 0 && *(min) == 1) {
				out = new int64_t(1);
				return out;
			}
			else return nullptr;
		}
		else if (x > max) return nullptr;
		else if (x < *(min)) return min;
		else if (cluster.buscar(high(x,hf)) != nullptr&&lower(x,hf) < cluster.buscar(high(x,hf))->max) {
			int64_t p = join(high(x,hf),*(cluster.buscar(high(x,hf))->sucessor(lower(x,hf))),hf);
			out = new int64_t(p);
			return out;
		}
		int64_t c1 = *(resumo->sucessor(high(x,hf)));
		out = new int64_t(join(c1, *(cluster.buscar(c1)->min),hf));
		return out;
	}


	void remover(int64_t x) {
		if (min == nullptr) return;
		else if (hf==1) {
			if (x == 0)*(min) = 1;
			else *(min) = 0;
			max = *(min);
		}
		else if (x == *(min)) {
			if (resumo == nullptr) {
				delete min;
				min = nullptr;
				return;
			}
			int64_t c = *(resumo->min);
			*(min) = join(c,*(cluster.buscar(c)->min),hf);
			x = *(min);
		}
		cluster.buscar(high(x,hf))->remover(lower(x,hf));
		if (cluster.buscar(high(x,hf))->min == nullptr) {
			cluster.remover(high(x,hf));
			resumo->remover(high(x,hf));
		}
		if (resumo->min == nullptr) {
			max = *(min);
			delete resumo;
			resumo = nullptr;
		}
		else {
			int64_t c1 = resumo->max;
			max = join(c1, cluster.buscar(c1)->max,hf);
		}
	}

};




int main(int argc, char **argv)
{
	if (argc != 3) {
		cout << "Arquivos não foram passados como argumento" << endl;
		cout << "usage: program input_name.txt output_name.txt" << endl;
		cout << "obs: crie o arquivo de saida antes de chamar o programa." << endl;
		return -1;
	}

	FILE *fp;
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		cout << "erro abrindo arquivo de entrada.";
		return -1;
	}
	if (freopen(argv[2], "w+", stdout) == NULL) {
		cout << "erro abrindo arquivo de saida.";
		return -1;
	}

	char op[3];
	VEB veb;
	int data;
	char trash;
	int64_t *result;
	initialize_table();

	if (fscanf(fp, "%c%c%c:%d", &op[0], &op[1], &op[2], &data) == 4) {
		do {
			switch (op[0])
			{
			case 'I':
				cout << "INC:" << data << endl;
				veb.incluir(data);
				break;
			case 'R':
				cout << "REM:" << data << endl;
				veb.remover(data);
				break;
			case 'S':
				cout << "SUC:" << data << endl;
				result = veb.sucessor(data);
				if (result != nullptr) {
					cout << *(result) << endl;
					delete result;
					result = nullptr;
				}
				else cout << "NULO"<< endl;
				break;
			case 'P':
				cout << "PRE:" << data << endl;
				result = veb.predecessor(data);
				if (result != nullptr) {
					cout << *(result) << endl;
					delete result;
					result = nullptr;
				}
				else cout << "NULO" << endl;
				break;
			default:
				cout << "codigo de operacao invalido" << endl;
				break;
			}
			cout << endl;
		} while (fscanf(fp, "%c%c%c%c:%d", &trash, &op[0], &op[1], &op[2], &data) == 5);
	}

}
