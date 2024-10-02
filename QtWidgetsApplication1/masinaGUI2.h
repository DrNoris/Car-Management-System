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
#include "serviceMasina.h"

using std::string;
class masinaGUI2 : public QWidget {
private:
	serviceMasina& serv;
	QVBoxLayout* mainLy;
	QListWidget* lst;
	QLineEdit* line;
public: 
	void asa() {

		mainLy = new QVBoxLayout{};
		lst = new QListWidget;
		line = new QLineEdit;
		auto form = new QFormLayout{};
		form->addRow("Nr in", line);

		auto masini = serv.getAll();

		lst = new QListWidget;
		for (auto masina : masini) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(masina.getNumarInm()));
			item->setData(Qt::BackgroundRole, QBrush{ Qt::red, Qt::SolidPattern });
			lst->addItem(item);
		}
		
		
		mainLy->addWidget(lst);
		mainLy->addLayout(form);
		setLayout(mainLy);

		QObject::connect(lst, &QListWidget::itemSelectionChanged, [&]() {
			auto items = lst->selectedItems();
			if (!items.isEmpty()) {
				auto item = items.at(0);
				item->setData(Qt::BackgroundRole, QBrush{ Qt::green, Qt::SolidPattern });
				line->setText(item->text());
			}
			});
	}
	masinaGUI2(serviceMasina& s) : serv{s} {
		asa();
	}
};