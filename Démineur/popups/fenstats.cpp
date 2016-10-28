#include "fenstats.hpp"
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>

#pragma GCC diagnostic ignored "-Wunused-variable"

FenStats::FenStats(QWidget *parent) :
	QDialog(parent)
{
	//boutons du bas
	QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal);
	QPushButton *btnReinitialiser = btns->addButton("Réinitialiser",QDialogButtonBox::ActionRole);
	connect(btns, SIGNAL(accepted()), this, SLOT(accept()));
	connect(btnReinitialiser, SIGNAL(clicked()), this, SLOT(reinitialiser()));

	//liste à gauche
	list = new QListWidget();
	list->addItem(new QListWidgetItem("Facile"));
	list->addItem(new QListWidgetItem("Normal"));
	list->addItem(new QListWidgetItem("Difficile"));

	//layout principal!
	QGridLayout *lPrincipal = new QGridLayout(this);
	lPrincipal->addWidget(list,0,0);
	lPrincipal->addWidget(btns,1,0,1,2);
}

void FenStats::reinitialiser()
{
	if(QMessageBox::question(this,"Réinitialiser les statistiques",
						  "Etes vous sûr de vouloir réinitialiser les statistiques?<BR>"
						  "Cette action est irréversible.",
						  QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
		== QMessageBox::Yes)
	{
	}
}
