#include "MasinaGUI.h"
#include "MasinaGUI2.h"
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    repoMasina repo;
    ValidatorMasini val;
    serviceMasina service{ repo, val };
    service.addMasina("mm30ari", "dacia", "spring", "electric");
    service.addMasina("cj87kdb", "audi", "a3 2020", "gri");
    service.addMasina("sj45cne", "audi", "a3 2013", "alb");
    service.addMasina("bn32vnf", "bmw", "x3 2017", "negru");
    service.addMasina("b740ufu", "volkswagen", "passat", "masina");
    service.addMasina("mm53hyt", "dacia", "sandero", "suv");
    MasinaGUI GUI { service };
    MasinaGUI GUI1 { service };

    GUI.show();
    //GUI1.show();


   a.setStyleSheet("QToolTip {"
                    "color: grey;"
                    "font-family: Verdana; "
                    "text-align: center;"
                    "font-weight: bold;"
                    "border-radius: 10px;"
                    "background-color: #ddd;"
        "}");

    return a.exec();
}
