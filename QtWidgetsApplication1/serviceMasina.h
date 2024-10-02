#pragma once
#include "repoMasina.h"
#include "observer.h"
#include "undo.h"

class DTO {
public:
	DTO() = default;
	DTO(string t, int count) :tip{ t }, count(count) {}
	int getCount() { return count; }
	void increment() {
		this->count++;
	}
private:
	int count;
	string tip;
};

class serviceMasina : public Observable
{
private:
	repoAbstract& repo;
	ValidatorMasini& val;
	vector<TElem> lista;

	vector<std::unique_ptr<undoActiune>> undo;
public:
	serviceMasina() = default;
	 
	serviceMasina(repoAbstract& r, ValidatorMasini& v) noexcept : repo{ r }, val{ v } {};

	serviceMasina(const serviceMasina& sm) = delete;

	void addMasina(const string nrIn, const string prod, const string model, const string tip);

	void eraseMasina(const string nrIn);
	
	void modifica(const string nrIn, const string prod, const string model, const string tip);

	void undo_action();
	
	Masina findService(const string nrIn);

	vector<TElem>& getAll() noexcept;

	vector<TElem> filtrare(int op, string crit);

	void sortare(int optiune);
	
	//fisier
	void citire_masini_fisier(string numeFisier);

	void scriere_masini_fisier(string numeFisier);

	//lista de lucru
	vector<TElem>& getLista() noexcept;

	void golesteLista() noexcept;

	void adaugaLista(const string nrIn);

	void randomLista();

	void exportLista(string nume);

	std::unordered_map<string, DTO> raport();
};