#include "repoMasina.h"

int repoMasina::exists(const Masina& m) {
	auto it = std::find_if(all.begin(), all.end(), [&](const Masina& masina) {
		return m.getModel() == masina.getModel() && m.getNumarInm() == masina.getNumarInm() &&
			m.getProducator() == masina.getProducator() && m.getTip() == m.getTip();
		});

	if (it == all.end())
		throw repoMasinaException{ "Masina cu datele: " + m.getNumarInm() + " " + m.getModel() +
								  " " + m.getProducator() + " " + m.getTip() + " nu exista" };
	else
		return int(std::distance(all.begin(), it));
}

void repoMasina::add(const Masina& m)
{	
	auto it = std::find_if(all.begin(), all.end(), [&](const Masina& masina) {
		if (m.getNumarInm() == masina.getNumarInm())
			return true;
		return false;
		});
	if (it != all.end())
		throw repoMasinaException{ "Masina deja exista" };
	all.push_back(m);
}

void repoMasina::erase(const Masina& m)
{	
	const auto position = exists(m);
	
	all.erase(all.begin() + position);
}

void repoMasina::modifica_repo(const Masina& m){
    const auto index = find(m.getNumarInm());
    Masina& masina = all.at(index);

    masina.setModel(m.getModel());
    masina.setProd(m.getProducator());
    masina.setTip(m.getTip());
}

int repoMasina::find(string nrIn)
{
	auto& masini = get_all();

	auto it = std::find_if(masini.begin(), masini.end(), [&](const Masina masina) {
		return masina.getNumarInm() == nrIn;
	});

	if (it != masini.end())
		return exists(*it);

	throw repoMasinaException{ "Masina cu numarul de inmatriculare: " + nrIn + " nu exista" };
}

ostream& operator<<(ostream& out, const repoMasinaException& ex){
		out << ex.msg;
	return out;
}

string repoMasinaFile::str(const Masina& m) const
{	
	return m.getNumarInm() + " " + m.getProducator() + " " + m.getModel() + " " + m.getTip();
}

void repoMasinaFile::loadFile()
{
	std::ifstream fin(fileName);

	if (!fin.is_open())
		throw repoMasinaException("Fisierul nu s-a deschis");

	repo.get_all().clear();

	string nrIn, producator, model, tip;
	while (fin >> nrIn >> producator >> model >> tip) {
		Masina m{ nrIn, producator, model, tip };
		add(m);
	}

	fin.close();
}

void repoMasinaFile::saveFile()
{
	std::ofstream fout(fileName);

	if (!fout.is_open())
		throw repoMasinaException{ "Fisierul nu s-a deschis" };


	auto& masini = repo.get_all();

	for (const auto& masina : masini)
		fout << str(masina) << '\n';
}

void repoMasinaFile::add(const Masina m) 
{
	repo.get_all().push_back(m);
}

void repoLab::add(const Masina& m){
	random();

	for (auto& masina : all)
		if (masina.second.getNumarInm() == m.getNumarInm())
			throw repoMasinaException{ "Masina deja exista" };
	auto l = all.size();
	
	all.insert(std::make_pair(static_cast<int>(l), m));
}

void repoLab::erase(const Masina& m){
	random();
	if (all.empty())
		throw repoMasinaException{ "Nu exista masini" };

	int key = find(m.getNumarInm());

	all.erase(key);
}

int repoLab::find(const string nrIn)
{
	int key = -1;
	for (auto masina : all)
		if (masina.second.getNumarInm() == nrIn) {
			key = masina.first;
			break;
		}
	if (key == -1)
		throw repoMasinaException{ "Masina cu numarul de inmatriculare: " + nrIn + " nu exista" };
	return key;
}

void repoLab::modifica_repo(const Masina& m){
	random();

	const auto index = find(m.getNumarInm());
	Masina& masina = all.find(index)->second;

	masina.setModel(m.getModel());
	masina.setProd(m.getProducator());
	masina.setTip(m.getTip());
}

void repoLab::random(){
	
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<> dist(0, 100);

	double index = dist(mt);

	while (index >= 1)
		index /= 10;

	//std::cout << index;

	if (index <= probabilitate)
		throw repoMasinaException{ "\n\nRandom exception\n\n" };
}
