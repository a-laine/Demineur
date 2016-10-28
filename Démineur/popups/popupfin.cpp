#include "popupfin.hpp"
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

PopupFin::PopupFin(const bool gagne, QWidget *parent) :
	QDialog(parent)
{
	// Le texte
	text = new QLabel();
	text->setText(getText(gagne));

	// les boutons du bas
	QDialogButtonBox *btns = new QDialogButtonBox(Qt::Horizontal);
	btnQuitter = btns->addButton("Quitter", QDialogButtonBox::ActionRole);
	connect(btnQuitter,SIGNAL(clicked()),parent,SLOT(close()));
	if(!gagne)
	{
		btnRecommencer = btns->addButton("Recommencer", QDialogButtonBox::ActionRole);
		connect(btnRecommencer,SIGNAL(clicked()),this,SLOT(reject()));
	}
	btnNewPartie = btns->addButton("Nouvelle Partie", QDialogButtonBox::RejectRole);
	connect(btnNewPartie,SIGNAL(clicked()),this,SLOT(accept()));
	btnNewPartie->setDefault(true);

	// Le layout principal, qui fait toute la fenetre
	QVBoxLayout *lPrincipal = new QVBoxLayout(this);
	lPrincipal->addWidget(text);
	lPrincipal->addWidget(btns);
}

QString PopupFin::getText(bool gagne)
{
	QString str("");
	if(gagne)
		str.append("<h3 align=\"center\">Gagné!</h3>");
	else
		str.append("<h3 align=\"center\">Perdu :(</h3>");

	return str;
}
