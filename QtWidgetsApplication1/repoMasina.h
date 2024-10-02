#pragma once
#include "masina.h"
#include "validator.h"
#include <vector>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <random>
#include <algorithm>
#include <memory> 
#include <unordered_map> 


using std::vector;

#define TElem Masina


class repoMasinaException {
	string msg;
public:
	repoMasinaException(string m) :msg{ m } {};
	//functie friend (vreau sa folosesc membru privat msg)
	friend ostream& operator<<(ostream& out, const repoMasinaException& ex);
	string get_mesaj() { return msg; }
};

ostream& operator<<(ostream& out, const repoMasinaException& ex);
class repoAbstract {
public:
	repoAbstract() = default;

	repoAbstract(repoAbstract& oth) = delete;

	virtual void add(const Masina& m) = 0;

	virtual void erase(const Masina& m) = 0;

	virtual int find(const string nrIn) = 0;

	virtual void modifica_repo(const Masina& m) = 0;

	virtual vector<TElem>& get_all() = 0;

	virtual ~repoAbstract() = default;
};



class repoMasina : public repoAbstract
{
private:
	vector<Masina> all;
	
	int exists(const Masina& m);
public:
	//default builder 
	repoMasina() = default;
	//nu permitem copierea 
	//const repoMasina& rm - o masina din memorie  
	repoMasina(const repoMasina& rm) = delete;

	//adaugam o noua masina in vectorul din repo
	//const repoMasina& m
	virtual void add(const Masina& m);

	void erase(const Masina& m);

	int find(const string nrIn);

    void modifica_repo(const Masina& m);

	vector<TElem>& get_all() noexcept{
		return all;
	}
};


class repoMasinaFile : public repoMasina {
private: 
	string fileName;
	repoAbstract& repo;

	//functia returneaza masina ca string pentru salvarea in fisier
	string str(const Masina& m) const;

	//functia face override la functia de adaugare din repo pentru a adauga o masina dupa copie, nu dupa reference  
	void add(const Masina m);
public:
	repoMasinaFile(const string& file, repoAbstract& r) : fileName{ file }, repo{ r } {};

	void loadFile();

	void saveFile();
};

class repoLab : public repoAbstract {
private:
	std::unordered_map <int, Masina> all;
	double probabilitate = 0;

	vector<TElem> masini;
public:
	repoLab() {
		set_probabilitate();
	};

	void add(const Masina& m) override;

	void erase(const Masina& m) override;

	int find(const string nrIn) override;

	void modifica_repo(const Masina& m) override;

	vector<TElem>& get_all() noexcept {
		masini.clear();

		for (auto& f : all)
			masini.push_back(f.second);

		return masini;
	}

	void random();

	void set_probabilitate() {
		std::mt19937 mt{ std::random_device{}() };
		std::uniform_int_distribution<> dist(0, 10);

		probabilitate = dist(mt)/100.0;
		std::cout << probabilitate << std::endl;
	}
};