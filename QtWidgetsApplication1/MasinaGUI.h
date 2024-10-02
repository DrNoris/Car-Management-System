#pragma once
#include <QtWidgets/qwidget.h>
#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <QStringList>
#include <QSizePolicy>
#include <QHeaderView>
#include <QTableWidget>
#include <qdebug.h>
#include <vector>
#include <sstream>
#include <string>
#include <qpainter.h>
#include "serviceMasina.h"

using std::string;

class CosReadOnlyGui : public QWidget, public Observer{
private:
    serviceMasina& serv;

    void paintEvent(QPaintEvent* ev) override {
        QPainter p{ this };
        auto masini = serv.getLista().size();
        for (; masini; --masini) {
            int x = rand() % 500;
            int y = rand() % 500;
            p.drawRect(x, y, 10, 10);
        }
    }

    void update() override {
        repaint();
    }
public: 
    CosReadOnlyGui(serviceMasina& s) : serv{ s } {
        serv.addObserver(this);
        srand(time(NULL));
    }

};

class CosCRUDGUI : public QWidget, public Observer {
private:
    serviceMasina& serv;
    QTableWidget* tabelCos = new QTableWidget{ 10, 4 };
    QPushButton* BtnGolireCos = new QPushButton{ "&Golire" };
    QPushButton* BtnRandom = new QPushButton{ "&Random" };
    QPushButton* BtnExporta = new QPushButton{ "&Exporta" };
    QLineEdit* textRandom = new QLineEdit;
    QVBoxLayout* mainCos = new QVBoxLayout;

    QStringList header{ "Nr. Inmatriculare", "Producator", "Model", "Tip" };
    void update() override {
            auto& masini = serv.getLista();
            tabelCos->clearContents();
            tabelCos->setRowCount(int(masini.size()));

            int linie = 0;
            for (auto& masina : masini) {
                tabelCos->setItem(linie, 0, new QTableWidgetItem(QString::fromStdString(masina.getNumarInm())));
                tabelCos->setItem(linie, 1, new QTableWidgetItem(QString::fromStdString(masina.getProducator())));
                tabelCos->setItem(linie, 2, new QTableWidgetItem(QString::fromStdString(masina.getModel())));
                tabelCos->setItem(linie, 3, new QTableWidgetItem(QString::fromStdString(masina.getTip())));
                linie++;
            }
    }
    void cos_init() {
        tabelCos->setHorizontalHeaderLabels(header);
        tabelCos->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tabelCos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        mainCos->addWidget(tabelCos);
        mainCos->addWidget(BtnGolireCos);
        mainCos->addWidget(BtnRandom);
        mainCos->addWidget(BtnExporta);
        mainCos->addWidget(textRandom);
        setLayout(mainCos);

        BtnGolireCos->setToolTip("Goleste cosul");
        BtnRandom->setToolTip("Introdu numarul de masini");
    }

    void butoane_init() {
        serv.addObserver(this);

        QObject::connect(BtnGolireCos, &QPushButton::clicked, [&]() {
            serv.golesteLista();
            });

        QObject::connect(BtnRandom, &QPushButton::clicked, [&]() {
            auto text = textRandom->text();
            textRandom->clear();

            int number = text.toInt();

            if (!number)
                QMessageBox::warning(this, "Warning Cos", "Trebuie un numar valid");
            else {
                while (number--)
                    serv.randomLista();
            }
            });

        QObject::connect(BtnExporta, &QPushButton::clicked, [&]() {
            auto text = textRandom->text();
            textRandom->clear();

            if (!text.size())
                QMessageBox::warning(this, "Warning Cos", "Nume invalid");
            else
                try {
                serv.exportLista(text.toStdString());
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Cos", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Cos", QString::fromStdString(er.get_mesaj()));
            }

            });
    }

    void loadDataCos() {
        auto& masini = serv.getLista();
        tabelCos->clearContents();
        tabelCos->setRowCount(int(masini.size()));

        int linie = 0;
        for (auto& masina : masini) {
            tabelCos->setItem(linie, 0, new QTableWidgetItem(QString::fromStdString(masina.getNumarInm())));
            tabelCos->setItem(linie, 1, new QTableWidgetItem(QString::fromStdString(masina.getProducator())));
            tabelCos->setItem(linie, 2, new QTableWidgetItem(QString::fromStdString(masina.getModel())));
            tabelCos->setItem(linie, 3, new QTableWidgetItem(QString::fromStdString(masina.getTip())));
            linie++;
        }
    }
public:
    CosCRUDGUI(serviceMasina& s) : serv{ s } {
        cos_init();
        butoane_init();
        loadDataCos();
    }

};


class MyTableModel : public QAbstractTableModel{
    std::vector<Masina> masini;
public:
    MyTableModel(const std::vector<Masina>& m) : masini{ m } {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return masini.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 4;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            Masina p = masini[index.row()];
            if (index.column() == 0) {
                return QString::fromStdString(p.getNumarInm());
            }
            else if (index.column() == 1) {
                return QString::fromStdString(p.getProducator());
            }
            else if (index.column() == 2) {
                return QString::fromStdString(p.getModel());
            }
            else if (index.column() == 3) {
                return QString::fromStdString(p.getTip());
            }
        }
        return QVariant();
    }

    void setMasina(const vector<Masina>& m) {
        masini = m;
        auto topLeft = createIndex(0, 0);
        auto bottomR = createIndex(rowCount(), columnCount());
        emit dataChanged(topLeft, bottomR);
        emit layoutChanged();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Horizontal) {
                if (section == 0)
                    return QString("Nr. In");
                else if (section == 1)
                    return QString("Producator");
                else if (section == 2)
                    return QString("Model");
                else if (section == 3)
                    return QString("Tip");
            }
            else {
                return QString("%1").arg(section);
            }
        }
        return QVariant();
    }
};

class MasinaGUI : public QWidget, public Observer {
private:
    serviceMasina& serv;
    QTableView* tabel = new QTableView;
    MyTableModel* model;
    MyTableModel* model1;

    QStringList header{"Nr. Inmatriculare", "Producator", "Model", "Tip"};

    QListView* flt = new QListView{};
    QListWidget* rpt = new QListWidget{};

    QPushButton* BtnAdauga = new QPushButton{ "&Adauga" };
    QPushButton* BtnSterge = new QPushButton{ "&Sterge" };
    QPushButton* BtnModifica = new QPushButton{ "&Modifica" };

    QPushButton* BtnSortare = new QPushButton{ "&Sortare" };
    QPushButton* BtnRaport = new QPushButton{ "&Raport" };
    QPushButton* BtnFiltrare = new QPushButton{ "&Filtrare" };

    QPushButton* BtnFind = new QPushButton{ "&Find" };
    QPushButton* BtnAdaugaCos = new QPushButton{ "&Adauga Cos" };
    QPushButton* BtnCos = new QPushButton{ "&CosCRUDGUI" }; 
    QPushButton* BtnHistograma = new QPushButton{ "&CosReadOnlyGui" };
    QPushButton* BtnUndo = new QPushButton{ "&Undo" };
    QPushButton* BtnExit = new QPushButton{ "&Exit" };


    QLineEdit* textNrIn = new QLineEdit;
    QLineEdit* textProd = new QLineEdit;
    QLineEdit* textModel = new QLineEdit;
    QLineEdit* textTip = new QLineEdit;

    void update() override {
        loadData();
    }

    void loadData() {
        model->setMasina(serv.getAll());
        tabel->setModel(model);
        tabel->resizeRowsToContents();
    }

    void loadDataFiltrare(vector<Masina> masini) {
        model1 = new MyTableModel{ serv.getAll() };
        model1->setMasina(masini);
        flt->setModel(model1);
        flt->show();
    }
    void loadDataRaport(std::unordered_map<string, DTO> raport) {
        rpt->clear();


        for (auto& masina : raport)
            rpt->addItem(QString::fromStdString(masina.first) + " " 
                + QString::number(masina.second.getCount()));
    }
	void initGUI() {
        QHBoxLayout* lyMain = new QHBoxLayout{};
        QVBoxLayout* lySecond = new QVBoxLayout{};
        QHBoxLayout* lyButMaster = new QHBoxLayout{};
        QVBoxLayout* lyButMain = new QVBoxLayout{};
        QVBoxLayout* lyButSecond = new QVBoxLayout{};

        model = new MyTableModel{ serv.getAll() };

        tabel->setModel(model);
        tabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        flt->setUniformItemSizes(true);
        rpt->setUniformItemSizes(true);

        lyButMain->addWidget(BtnAdauga);
        lyButMain->addWidget(BtnSterge);
        lyButMain->addWidget(BtnModifica);



        lyButSecond->addWidget(BtnSortare);
        lyButSecond->addWidget(BtnFiltrare);
        lyButSecond->addWidget(BtnRaport);



        auto form = new QFormLayout{};

        form->addRow("Numar Inmatriculare", textNrIn);
        form->addRow("Producator", textProd);
        form->addRow("Model", textModel);
        form->addRow("Tip", textTip);

        //butoane

        BtnAdauga->setToolTip("Adauga o noua masina, toate campuri trebuie completate");
        BtnSterge->setToolTip("Sterge masina cu numarul de inmatriculare dat");
        BtnModifica->setToolTip("Modifica masin cu numarul de inamtriculare dat, toate campurile trebuie completate");
        BtnSortare->setToolTip("Nr Inamtriculare/Tip/Producator+Model");
        BtnFiltrare->setToolTip("Producator/Tip");
        BtnRaport->setToolTip("Frecventa in functie de producator");
        BtnCos->setToolTip("Deschide meniul cos");
        BtnExit->setToolTip("Exit");
        BtnUndo->setToolTip("Undo ultima actiune");
        BtnFind->setToolTip("Introdu Nr. Inmatriculare");
        BtnAdaugaCos->setToolTip("Adauga in cos masina dupa Nr. Inmatriculare");
        
        //tabel->setHorizontalHeaderLabels(header);
        tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        tabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


        lyMain->addWidget(tabel);
        lySecond->addLayout(form);
        lyButMaster->addLayout(lyButMain);
        lyButMaster->addLayout(lyButSecond);
        lySecond->addLayout(lyButMaster);
        lySecond->addWidget(BtnFind);
        lySecond->addWidget(BtnHistograma);
        lySecond->addWidget(BtnCos);
        lySecond->addWidget(BtnAdaugaCos);
        lySecond->addWidget(BtnUndo);
        lySecond->addWidget(BtnExit);
        lyMain->addLayout(lySecond);
        setLayout(lyMain);

	}
    void initConnect() {

        serv.addObserver(this);
        
        QObject::connect(BtnExit, &QPushButton::clicked, []() {
            QApplication::quit();
            });

        QObject::connect(BtnAdauga, &QPushButton::clicked, [&]() {
            auto nrIn = textNrIn->text();
            auto prod = textProd->text();
            auto model = textModel->text();
            auto tip = textTip->text();

            GUIclear();
            try {
                serv.addMasina(nrIn.toStdString(), prod.toStdString(),
                    model.toStdString(), tip.toStdString());
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Adaugare", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Adaugare", QString::fromStdString(er.get_mesaj()));
            }
            });

        QObject::connect(BtnSterge, &QPushButton::clicked, [&]() {
            auto nrIn = textNrIn->text();

            GUIclear();
            try {
                serv.eraseMasina(nrIn.toStdString());
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Sterge", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Sterge", QString::fromStdString(er.get_mesaj()));
            }

            });
        QObject::connect(BtnModifica, &QPushButton::clicked, [&]() {
            auto nrIn = textNrIn->text();
            auto prod = textProd->text();
            auto model = textModel->text();
            auto tip = textTip->text();

            GUIclear();
            
            try {
                serv.modifica(nrIn.toStdString(), prod.toStdString(),
                    model.toStdString(), tip.toStdString());
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Modifica", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Modifica", QString::fromStdString(er.get_mesaj()));
            }

            });
        QObject::connect(BtnSortare, &QPushButton::clicked, [&]() {
            auto nrIn = textNrIn->text();
            auto prod = textProd->text();
            auto model = textModel->text();
            auto tip = textTip->text();

            GUIclear();
            
            int op = 0;

            if (nrIn.size() && prod.size() == 0 && model.size() == 0 && tip.size() == 0)
                op = 1;
            if (nrIn.size() == 0 && prod.size() == 0 && model.size() == 0 && tip.size())
                op = 2;
            if (nrIn.size() == 0 && prod.size() && model.size() && tip.size() == 0)
                op = 3;

            serv.sortare(op);
            });
        QObject::connect(BtnFiltrare, &QPushButton::clicked, [&]() {
            auto prod = textProd->text();
            auto tip = textTip->text();

            GUIclear();
            vector<Masina> flt;
            if (prod.size() == 0 && tip.size()) 
                flt = serv.filtrare(2, tip.toStdString());
            if (prod.size() && tip.size() == 0)
                flt = serv.filtrare(1, prod.toStdString());

            loadDataFiltrare(flt);
            });
        QObject::connect(BtnFind, &QPushButton::clicked, [&]() {
            auto nrIn = textNrIn->text();

            GUIclear();

            try {
                auto masina = serv.findService(nrIn.toStdString());
                QMessageBox::information(this, "Masina Gasina", QString::fromStdString(masina.getNumarInm() + " " +
                masina.getProducator() + " " + masina.getModel() + " " + masina.getTip()));
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Undo", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Undo", QString::fromStdString(er.get_mesaj()));
            }
            });
        QObject::connect(BtnUndo, &QPushButton::clicked, [&]() {
            GUIclear();
            try {
                serv.undo_action();
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Undo", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Undo", QString::fromStdString(er.get_mesaj()));
            }
            });

        QObject::connect(BtnRaport, &QPushButton::clicked, [&]() {
            GUIclear();

            try {
                std::unordered_map<string, DTO> raport = serv.raport();
                loadDataRaport(raport);
                rpt->show();
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning raport", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning raport", QString::fromStdString(er.get_mesaj()));
            }
            });
        QObject::connect(BtnCos, &QPushButton::clicked, [&]() {
            auto cos = new CosCRUDGUI{ serv };
            cos->show();
            });
        QObject::connect(BtnHistograma, &QPushButton::clicked, [&]() {
            auto cos = new CosReadOnlyGui{ serv };
            cos->show();
            });
        QObject::connect(BtnAdaugaCos, &QPushButton::clicked, [&]() {
            auto nrIn = textNrIn->text();
            textNrIn->clear();

            try {
                serv.adaugaLista(nrIn.toStdString());
            }
            catch (Exception& er) {
                string errors = er.toString();
                QMessageBox::warning(this, "Warning Cos", QString::fromStdString(errors));
            }
            catch (repoMasinaException& er) {
                QMessageBox::warning(this, "Warning Cos", QString::fromStdString(er.get_mesaj()));
            }
            });
    }
    void GUIclear() {
        textNrIn->clear();
        textProd->clear();
        textModel->clear();
        textTip->clear();
    }
public:
    MasinaGUI(serviceMasina& s) : serv{ s } {
        initGUI(); 
        loadData();
        initConnect();
    }
};