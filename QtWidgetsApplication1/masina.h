#pragma once
#include <string>
#include <expected>
#include <iostream>


using std::string;

class Masina
{
private:
	string numarInm;
	string producator;
	string model;
	string tip;

public:
	Masina() = default;

	Masina(const string n, const string p, const string m, const string t) : numarInm{ n }, producator{ p }, model{ m }, tip{ t } {}

	Masina(const Masina& m) : numarInm{ m.getNumarInm()}, producator{ m.getProducator() }, 
							model{ m.getModel() }, tip{ m.getTip() }
	{
		//std::cout << "\n\nCopia nu are valoare\n\n";
		
	}

	std::string getNumarInm() const;

	void setNumarInm(const std::string& nrIn);

	std::string getProducator() const;

	void setProd(const std::string& prod);

	std::string getModel() const;

	void setModel(const std::string& m);

	std::string getTip() const;

	void setTip(const std::string& t);
};

bool cmpNrIn(const Masina& m1, const Masina& m2);

bool cmpTip(const Masina& m1, const Masina& m2);

bool cmpProdModel(const Masina& m1, const Masina& m2);

