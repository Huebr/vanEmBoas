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

int64_t high(int64_t x) {
	return x >> 32;

}

int64_t lower(int64_t x) {
	return x & ((1 << 32) - 1);
}

int64_t join(int64_t c, int64_t i) {
	return (c << 32) | i;
}



struct VEB {
	
	int64_t* min = nullptr;
	int64_t max;
	VEB* resumo = nullptr;
	hashtable<item<VEB>,VEB> cluster;

	//create table to be used by simple hashing
	VEB() {

	}
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
			if (x < *(min)) {
				int temp = x;
				x = *(min);
				*(min) = temp;
			}

			if (cluster.buscar(high(x)) == nullptr) {
				cluster.incluir(high(x), new VEB(),true);
			}
			if (cluster.buscar(high(x))->min == nullptr) {
				if (resumo == nullptr) resumo = new VEB();
				(*resumo).incluir(high(x));
			}
			if (x > max) {
				max = x;
			}
			cluster.buscar(high(x))->incluir(lower(x));
		}

	}

	int64_t* predecessor(int64_t x) {
		int64_t* out;
		if (min == nullptr) return nullptr;
		else if (x > max) {
			out = new int64_t(max);
			return out;
		}
		else if (x < *(min)) return nullptr;
		else if (cluster.buscar(high(x))!=nullptr&&lower(x) > *(cluster.buscar(high(x))->min)) {
			int64_t p = join(high(x), *(cluster.buscar(high(x))->predecessor(lower(x))));
			out = new int64_t(p);
			return out;
		}
		int64_t c1 = *(resumo->predecessor(high(x)));
		out = new int64_t(join(c1, cluster.buscar(c1)->max));
		return out;
	}

	int64_t* sucessor(int64_t x) {
		int64_t* out;
		if (min == nullptr) return nullptr;
		else if (x > max) return nullptr;
		else if (x < *(min)) return min;
		else if (cluster.buscar(high(x)) != nullptr&&lower(x) < cluster.buscar(high(x))->max) {
			int64_t p = join(high(x),*(cluster.buscar(high(x))->sucessor(lower(x))));
			out = new int64_t(p);
			return out;
		}
		int64_t c1 = *(resumo->sucessor(high(x)));
		out = new int64_t(join(c1, *(cluster.buscar(c1)->min)));
		return out;
	}


	void remover(int64_t x) {
		if (min == nullptr) return;
		if (x == *(min)) {
			if (resumo == nullptr) {
				delete min;
				min = nullptr;
				return;
			}
			int64_t c = *(resumo->min);
			*(min) = join(c,*(cluster.buscar(c)->min));
			x = *(min);
		}
		cluster.buscar(high(x))->remover(lower(x));
		if (cluster.buscar(high(x))->min == nullptr) {
			cluster.remover(high(x));
			resumo->remover(high(x));
		}
		if (resumo->min == nullptr) {
			max = *(min);
			delete resumo;
			resumo = nullptr;
		}
		else {
			int64_t c1 = resumo->max;
			max = join(c1, cluster.buscar(c1)->max);
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
