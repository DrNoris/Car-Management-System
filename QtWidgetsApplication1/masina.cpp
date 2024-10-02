#include "masina.h"

std::string Masina::getNumarInm() const {
    return numarInm;
}

void Masina::setNumarInm(const std::string& nrIn) {
    numarInm = nrIn;
}

std::string Masina::getProducator() const {
    return producator;
}

void Masina::setProd(const std::string& prod) {
    producator = prod;
}

std::string Masina::getModel() const {
    return model;
}

void Masina::setModel(const std::string& m) {
    model = m;
}

std::string Masina::getTip() const {
    return tip;
}

void Masina::setTip(const std::string& t) {
    tip = t;
}

bool cmpNrIn(const Masina& m1, const Masina& m2)
{
    return m1.getNumarInm() < m2.getNumarInm();
}

bool cmpTip(const Masina& m1, const Masina& m2)
{
    return m1.getTip() < m2.getTip();
}

bool cmpProdModel(const Masina& m1, const Masina& m2)
{
    if (m1.getProducator() != m2.getProducator())
        return m1.getProducator() < m2.getProducator();

    return m1.getModel() < m2.getModel();
}
