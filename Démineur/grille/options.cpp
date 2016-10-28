#include "options.hpp"
#include <QFile>
#include <QTextStream>
#include <stdexcept>


Options* Options::This = 0;

Options* Options::getInstance(QObject *parent)
{
	if(This == 0)
		This = new Options(parent);
	return This;
}

void Options::deleteInstance()
{
	delete This;
}



namespace Facile {
	enum {
		hauteur = 9,
		largeur = 9,
		nbMines = 9,
	};
}
namespace Moyen {
	enum {
		hauteur = 16,
		largeur = 16,
		nbMines = 40,
	};
}
namespace Difficile {
	enum {
		hauteur = 16,
		largeur = 30,
		nbMines = 99,
	};
}




//************************************* Methodes **************************************//

Options::Options(QObject *parent) :
	QObject(parent), flags(ALL)
{
	difficulte.difficulte = FACILE;
	difficulte.hauteur = Facile::hauteur;
	difficulte.largeur = Facile::largeur;
	difficulte.nbMines = Facile::nbMines;
	file = new QFile("optionsSave.txt");

	loadDefaultUI();

	lire();
}

Options::~Options()
{
    delete file;
}

// getters

Options::Difficulte Options::getDifficulte() const
{
	return difficulte.difficulte;
}

qint8 Options::getHauteur() const
{
	switch(difficulte.difficulte)
	{
		case FACILE:
			return Facile::hauteur;
		case MOYEN:
			return Moyen::hauteur;
		case DIFFICILE:
			return Difficile::hauteur;
		default:
			return difficulte.hauteur;
	}
}

qint8 Options::getLargeur() const
{
	switch(difficulte.difficulte)
	{
		case FACILE:
			return Facile::largeur;
		case MOYEN:
			return Moyen::largeur;
		case DIFFICILE:
			return Difficile::largeur;
		default:
			return difficulte.largeur;
	}
}

qint16 Options::getNbMines() const
{
	switch(difficulte.difficulte)
	{
		case FACILE:
			return Facile::nbMines;
		case MOYEN:
			return Moyen::nbMines;
		case DIFFICILE:
			return Difficile::nbMines;
		default:
			return difficulte.nbMines;
	}
}

bool Options::isOptionEnabled(OptionsFlags option) const
{
	return (flags & option);
}

QColor Options::getColor(const QString element, const QColor defaultValue) const
{
	return colors.value(element,defaultValue);
}

// setters

void Options::setDifficulte(const Difficulte d, const qint8 h, const qint8 l, const qint16 m)
{
	if(h<5 || h>30 || l<5 || l>30 || m<0 || m>(h*l-9))
		throw std::invalid_argument("valeur impossible");

	if(((difficulte.hauteur != h || difficulte.largeur != l || difficulte.nbMines != m)
			&& d == PERSONNALISE) || d != difficulte.difficulte)
	{
		difficulte.difficulte = d;
		difficulte.hauteur = h;
		difficulte.largeur = l;
		difficulte.nbMines = m;
		emit changeDifficulte();
	}
}

void Options::setOptionFlag(const OptionsFlags option, const bool enabled)
{
	if(enabled)
		flags |= option;
	else
		flags &= ~option;
}

void Options::setColor(const QString element, const QColor color)
{
	if(colors.contains(element))
		colors.insert(element,color);
}

void Options::loadDefaultUI()
{
	colors.insert("background",Qt::gray);
	colors.insert("compteurs",Qt::black);
	colors.insert("pt_interro",Qt::white);
	colors.insert("1",QColor(51,81,255));
	colors.insert("2",Qt::darkGreen);
	colors.insert("3",QColor(180,0,0));
	colors.insert("4",Qt::darkBlue);
	colors.insert("5",QColor(125,37,0));
	colors.insert("6",Qt::darkCyan);
	colors.insert("7",Qt::red);
	colors.insert("8",Qt::black);
}

// IOFile

void Options::ecrire()
{
	if(file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
		QTextStream out(file);
		qint8 diff = difficulte.difficulte;
		out << "Difficulte : " << diff << endl;
		out << "Hauteur : " << difficulte.hauteur << endl;
		out << "Largeur : " << difficulte.largeur << endl;
		out << "NbMines : " << difficulte.nbMines << endl;
		out << "Flags : " << flags << endl;
		for(QMap<QString,QColor>::iterator it = colors.begin();
			it != colors.cend(); ++it)
		{
			out << it.key() << " : "
				<< it.value().red()   << ','
				<< it.value().green() << ','
				<< it.value().blue()  << endl;
		}
        file->close();
    }
}

void Options::lire()
{
	if(file->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		quint8 d=0, h=0, l=0;
		quint16 m=0;
		QTextStream in(file);

		QString read = in.readLine();
		QString key = read.section(' ',0,0,QString::SectionSkipEmpty);
		while(!read.isNull())
		{
			if(colors.contains(key))
			{
				QString col = read.section(' ',-1,-1,QString::SectionSkipEmpty);
				colors.insert(key, QColor(
								  col.section(QRegExp(" *[,;] *"),0,0).toInt(),
								  col.section(QRegExp(" *[,;] *"),1,1).toInt(),
								  col.section(QRegExp(" *[,;] *"),2,2).toInt()));
			}
			else if(key == "Difficulte")
				d = read.section(' ',-1,-1,QString::SectionSkipEmpty).toUInt();
			else if(key == "Hauteur")
				h = read.section(' ',-1,-1,QString::SectionSkipEmpty).toUInt();
			else if(key == "Largeur")
				l = read.section(' ',-1,-1,QString::SectionSkipEmpty).toUInt();
			else if(key == "NbMines")
				m = read.section(' ',-1,-1,QString::SectionSkipEmpty).toUInt();
			else if(key == "Flags")
				flags = read.section(' ',-1,-1,QString::SectionSkipEmpty).toUInt();
			read = in.readLine();
			key = read.section(' ',0,0,QString::SectionSkipEmpty);
		}

		try{
			switch(d)
			{
				case FACILE:
					setDifficulte(FACILE,h,l,m);
					break;
				case MOYEN:
					setDifficulte(MOYEN,h,l,m);
					break;
				case DIFFICILE:
					setDifficulte(DIFFICILE,h,l,m);
					break;
				case PERSONNALISE:
					setDifficulte(PERSONNALISE,h,l,m);
					break;
				default:
					throw std::exception();
			}
		}
		catch(std::exception&)
		{
			setDifficulte(FACILE);
		}
	}
	file->close();
}
