#include "popups/fenstyle.hpp"
#include "grille/options.hpp"
#include <QAction>
#include <QColorDialog>
#include <QPainter>
#include <QPixmap>

QMap<QString,QColor> ColorButton::colors;


ColorButton::ColorButton(QString name, QColor color, QWidget *parent) :
	QToolButton(parent), nom(name), couleur(color)
{
	QAction *action = new QAction(this);
	setDefaultAction(action);
	connect(action, SIGNAL(triggered()), this, SLOT(compute()));
	setIcon(iconByName(name,color));
}

void ColorButton::setColor(QColor color)
{
	couleur = color;
}

QColor ColorButton::getColor() const
{
	return couleur;
}

void ColorButton::compute()
{
	QColor col = QColorDialog::getColor(couleur,this,"Apparence: couleur");
	if(col.isValid())
	{
		couleur = col;
		setIcon(iconByName(nom,couleur));
		colors.insert(nom,couleur);
	}
}

QIcon ColorButton::iconByName(QString name, QColor color)
{

	QPixmap image(36,36);
	QPainter painter(&image);
	painter.setBrush(QBrush(Qt::lightGray));
	painter.drawRect(-1,-1,37,37);

	painter.setBrush(QBrush(color));
	painter.setPen(color);
	if(name == "background" || name == "compteurs")
	{
		painter.drawRect(-1,-1,37,37);
	}
	else if(name == "pt_interro")
	{
		painter.setFont(QFont("Arial", 34, 70));
		painter.drawText(0,0,36,36,Qt::AlignCenter,"?");
	}
	else if(name.size() == 1)
	{
		painter.setFont(QFont("Verdana", 32, 70));
		painter.drawText(0,0,36,36,Qt::AlignCenter,name);
	}
	return QIcon(image);
}






#include <QTabWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

FenStyle::FenStyle(QWidget *parent) :
	QDialog(parent)
{
	Options *op = Options::getInstance();

	//boutons ok et annuler
	QDialogButtonBox *btnsBas = new QDialogButtonBox(
				QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
	QPushButton *btnReinit = new QPushButton("Tout réinitialiser");
	btnsBas->addButton(btnReinit,QDialogButtonBox::ActionRole);
	connect(btnsBas, SIGNAL(accepted()), this, SLOT(ok()));
	connect(btnsBas, SIGNAL(rejected()), this, SLOT(reject()));
	connect(btnReinit, SIGNAL(clicked()), this, SLOT(defaultUI()));

	QTabWidget *tab = new QTabWidget();
	//éléments génériques
	QWidget *page1 = new QWidget();
	tab->addTab(page1,"Aspect général");
	QFormLayout *lpage1 = new QFormLayout(page1);
	lpage1->addRow("Arrière plan :",new ColorButton("background",op->getColor("background")));
	lpage1->addRow("Compteurs :",new ColorButton("compteurs",op->getColor("compteurs")));
	lpage1->addRow("Points d'interrogation :",new ColorButton("pt_interro",op->getColor("pt_interro")));

	//nombres
	QWidget *page2 = new QWidget();
	tab->addTab(page2,"Nombres");
	QFormLayout *lpage2 = new QFormLayout(page2);
	lpage2->addRow("Numéro 1 : ",new ColorButton("1",op->getColor("1")));
	lpage2->addRow("Numéro 2 : ",new ColorButton("2",op->getColor("2")));
	lpage2->addRow("Numéro 3 : ",new ColorButton("3",op->getColor("3")));
	lpage2->addRow("Numéro 4 : ",new ColorButton("4",op->getColor("4")));
	lpage2->addRow("Numéro 5 : ",new ColorButton("5",op->getColor("5")));
	lpage2->addRow("Numéro 6 : ",new ColorButton("6",op->getColor("6")));
	lpage2->addRow("Numéro 7 : ",new ColorButton("7",op->getColor("7")));
	lpage2->addRow("Numéro 8 : ",new ColorButton("8",op->getColor("8")));

	//positionnement de tous les elements
	QVBoxLayout *lFen = new QVBoxLayout(this);
	lFen->addWidget(tab);
	lFen->addWidget(btnsBas);

	ColorButton::colors.clear();
}

FenStyle::~FenStyle()
{
	ColorButton::colors.clear();
}

void FenStyle::ok()
{
	Options *op = Options::getInstance();
	for(QMap<QString,QColor>::iterator it = ColorButton::colors.begin();
		it != ColorButton::colors.cend(); ++it)
	{
		op->setColor(it.key(),it.value());
	}
	op->ecrire();
	accept();
}

void FenStyle::defaultUI()
{
	Options::getInstance()->loadDefaultUI();
	Options::getInstance()->ecrire();
	accept();
}
