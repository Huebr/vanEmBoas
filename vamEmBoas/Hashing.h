// SimpleTableHashing.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <chrono>
#include <stdint.h>
#include <iostream>
#include <random>
#include <vector>
#include <cstdio>
#pragma warning(disable:4996)//disable some vs safety warnings

using namespace std;

template <class elem>
struct item { 
	int64_t data = 0;
	elem *p = nullptr;
	int8_t  bit = 0;
};

int64_t T[8][256];
void initialize_table() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 256; ++j) {
			T[i][j] = generator();
		}
	}

}

template <class element,class tipo>
struct hashtable{
	int64_t n_elems = 0;
	int64_t n_removed = 0;
	vector<element> *ht;
	//create table to be used by simple hashing
	hashtable() {
		ht = new vector<element>(4);
		ht->resize(4);
	}
	hashtable(int size) {
		ht = new vector<element>(size);
		ht->resize(size);
	}
	~hashtable() {
		if (ht != nullptr) {
			for (int i = 0; i < ht->capacity(); i++)
			{
				if ((*ht)[i].p != nullptr) {
					delete (*ht)[i].p;
					(*ht)[i].p = nullptr;
				}
			}
			delete ht;
			ht = nullptr;
		}
	}



	// simple table hashing implementation
	int64_t SimpleTableHashing(int64_t input)
	{
		int64_t i;
		int64_t h = 0;
		int8_t c;
		for (i = 0; i < 8; i++) {
			c = input;
			h ^= T[i][c];
			input = input >> 8;
		}
		return h;
	}


	void dobrar() {
		int64_t m = ht->capacity();
		vector<element> *temp = ht;
		ht = new vector<element>(m * 2);
		n_removed = 0;
		for (int i = 0; i < m; ++i) {
			if ((*temp)[i].bit == 1)incluir((*temp)[i].data, (*temp)[i].p,false);
		}
		delete temp;
	}

	void dividir() {
		int64_t m = ht->capacity();
		vector<element> *temp = ht;
		ht = new vector<element>(m / 2);
		n_removed = 0;
		for (int i = 0; i < m; ++i) {
			if ((*temp)[i].bit == 1)incluir((*temp)[i].data, (*temp)[i].p,false);
		}
		delete temp;
	}


	
	void incluir(int64_t data,tipo *ponteiro,bool debug) {
		int64_t hash = SimpleTableHashing(data);
		int64_t m = ht->capacity();
		for (int i = 0; i < m; ++i) {
			int64_t idx = (hash + i) % m;
			if ((*ht)[idx].bit != 1) {
				if ((*ht)[idx].bit == 3) n_removed--;
				(*ht)[idx].data = data;
				(*ht)[idx].p = ponteiro;
				(*ht)[idx].bit = 1;
				break;
			}
		}
		//use debug variable to verify if function is external or internal
		if (debug) n_elems++;
		if (n_elems > (m/2)&&debug) {
			dobrar();
		}

	}

	tipo* buscar(int64_t data) {
		int64_t hash = SimpleTableHashing(data);
		int64_t m = ht->capacity();
		for (int i = 0; i < m; ++i) {
			int64_t idx = (hash + i) % m;
			if (data == (*ht)[idx].data && (*ht)[idx].bit == 1) {
				return (*ht)[idx].p;
			}
			else if ((*ht)[idx].bit == 0) break;
		}
		return nullptr;
	}
	void limpar() {
		int64_t m = ht->capacity();
		vector<element> *temp = ht;
		ht = new vector<element>(m);
		for (int i = 0; i < m; ++i) {
			if ((*temp)[i].bit == 1)incluir((*temp)[i].data, (*temp)[i].p, false);
		}
		delete temp;
	}

	void remover(int64_t data) {
		int64_t hash = SimpleTableHashing(data);
		int64_t m = ht->capacity();
		bool flag=false;
		bool cleared = false;

		for (int i = 0; i < m; ++i) {
			int64_t idx = (hash + i) % m;
			if ((*ht)[idx].data == data && (*ht)[idx].bit == 1) {
				(*ht)[idx].bit = 3;
				delete (*ht)[idx].p;
				(*ht)[idx].p = nullptr;
				n_removed++;
				cleared = true;
				break;
			}
		}

		if (cleared) {
			n_elems--;
			if (n_elems < (m / 4)) {
				dividir();
			}
			else if (n_removed > m / 4) limpar();
		}
		else cout << hash % m << " " << -1 << endl;
	}

};




