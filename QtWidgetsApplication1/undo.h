#pragma once
#include "masina.h"
#include "repoMasina.h"
#include "serviceMasina.h"

class undoActiune {
public:
	virtual void doUndo() = 0;
	virtual ~undoActiune() = default;
};

class undoAdauga : public undoActiune {
private:
	Masina m;
	repoAbstract& repo;
public:
	undoAdauga(Masina masina, repoAbstract& r) : m{ masina }, repo{ r } {};
	void doUndo() override {
		repo.erase(m);
	}
};

class undoSterge : public undoActiune {
private:
	Masina m;
	repoAbstract& repo;
public:
	undoSterge(Masina masina, repoAbstract& r) : m{ masina }, repo{ r } {};
	void doUndo() override {
		repo.add(m);
	}
};

class undoModifica : public undoActiune {
private:
	Masina m;
	repoAbstract& repo;
public:
	undoModifica(Masina masina, repoAbstract& r) : m{ masina }, repo{ r } {};
	void doUndo() override {
		repo.modifica_repo(m);
	}
};