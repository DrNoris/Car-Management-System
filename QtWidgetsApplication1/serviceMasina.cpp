#include "serviceMasina.h"

void serviceMasina::addMasina(const string nrIn, const string prod, const string model, const string tip) {
    Masina m(nrIn, prod, model, tip);
    val.validate(m);
    repo.add(m);
    undo.push_back(std::make_unique<undoAdauga>(m, repo));
    notify();
}

void serviceMasina::eraseMasina(const string nrIn) {
    Masina m = findService(nrIn);
    repo.erase(m);
    undo.push_back(std::make_unique<undoSterge>(m, repo));
    notify();
}

void serviceMasina::modifica(const string nrIn, const string prod, const string model, const string tip) {
    Masina m(nrIn, prod, model, tip);
    val.validate(m);

    undo.push_back(std::make_unique<undoModifica>(findService(nrIn), repo));
    repo.modifica_repo(m);
    notify();
}

void serviceMasina::undo_action() {
    if (undo.empty())
        throw (repoMasinaException{ "Undo gol" });
       
    undo.back()->doUndo();
    undo.pop_back();
    notify();
}

Masina serviceMasina::findService(const string nrIn) {
    return getAll().at(repo.find(nrIn));
}

vector<TElem>& serviceMasina::getAll() noexcept {
    return repo.get_all();
}

vector<TElem> serviceMasina::filtrare(int op, string crit) {
    auto masini = getAll();
    vector<Masina> masini_filtrate;

    if (op == 1) {
        std::copy_if(masini.begin(), masini.end(), std::back_inserter(masini_filtrate),
            [crit](const Masina& m) { return m.getProducator() == crit; });
    }
    else if (op == 2) {
        std::copy_if(masini.begin(), masini.end(), std::back_inserter(masini_filtrate),
            [crit](const Masina& m) { return m.getTip() == crit; });
    }

    return masini_filtrate;
}


void serviceMasina::sortare(int op) {
    auto& masini = getAll();

    auto compareNrInmatriculare = [](const Masina& m1, const Masina& m2) {
        return m1.getNumarInm() < m2.getNumarInm();
        };

    auto compareTip = [](const Masina& m1, const Masina& m2) {
        return m1.getTip() < m2.getTip();
        };

    auto compareProducatorModel = [](const Masina& m1, const Masina& m2) {
        if (m1.getProducator() == m2.getProducator())
            return m1.getModel() < m2.getModel();
        return m1.getProducator() < m2.getProducator();
        };

    if (op == 1)
        std::sort(masini.begin(), masini.end(), compareNrInmatriculare);
    if (op == 2)
        std::sort(masini.begin(), masini.end(), compareTip);
    if (op == 3)
        std::sort(masini.begin(), masini.end(), compareProducatorModel);
    notify();
}


void serviceMasina::citire_masini_fisier(string numeFisier) {
    repoMasinaFile file{ numeFisier, repo };

    file.loadFile();
}

void serviceMasina::scriere_masini_fisier(string numeFisier) {
    repoMasinaFile file{ numeFisier, repo };

    file.saveFile();
}

vector<TElem>& serviceMasina::getLista() noexcept {
    return lista;
}

void serviceMasina::golesteLista() noexcept {
    lista.clear();
    notify();
}

void serviceMasina::adaugaLista(const string nrIn) {
    lista.push_back(findService(nrIn));
    notify();
}

void serviceMasina::randomLista() {
    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<> dist(0, int(getAll().size() - 1));

    auto index = dist(mt);

    adaugaLista(getAll().at(index).getNumarInm());
    notify();
}

void serviceMasina::exportLista(string nume) {
    // Get the path to the desktop
    char* desktopPath = nullptr;
    size_t len;
    errno_t err = _dupenv_s(&desktopPath, &len, "USERPROFILE");
    if (err != 0) {
        throw repoMasinaException("Error: Unable to get desktop path\n");
    }
    std::string desktopDir(desktopPath);

    // Free the allocated memory
    free(desktopPath);

    // Construct the file path for the HTML file
    std::string filePath = desktopDir + "\\Desktop\\" + nume + ".html";

    // Open the HTML file for writing
    std::ofstream outputFile(filePath);
    if (!outputFile) {
        throw repoMasinaException("Error: Unable to create HTML file\n");
    }

    // Write the initial content from html.txt
    std::ifstream htmlContentFile("html.txt");
    if (htmlContentFile) {
        outputFile << htmlContentFile.rdbuf();
        htmlContentFile.close();
    }
    else {
        throw repoMasinaException("Error: Unable to open html.txt\n");
    }

    // Write divs with text representing the priorities from the vector
    auto& masini = getLista();
    for (auto masina : masini) 
        outputFile << "<div class=\"car-info\"><p> Numar de inmatriculare: " << masina.getNumarInm() <<
            " producator: " << masina.getProducator() << " model: " << masina.getModel() << " tip: "
            << masina.getTip() << "</p></div>\n";
    

    // Write HTML closing tags
    outputFile << "</body>\n</html>";
}

std::unordered_map<string, DTO> serviceMasina::raport() {
    std::unordered_map<std::string, DTO> map;

    auto& masini = repo.get_all();

    for (auto& masina : masini) {
        auto it = map.find(masina.getProducator());
        if (it != map.end()) {
            it->second.increment();
        }
        else {
            map[masina.getProducator()] = DTO(masina.getProducator(), 1);
        }
    }

    return map;
}
