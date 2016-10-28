#include "grille.hpp"
#include "options.hpp"
#include "case.hpp"
#include <ctime>
#include <cstdlib>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>


Grille::Grille(QWidget *parent) :
	QGraphicsScene(parent)
{
	options = Options::getInstance();
	tabC = 0;
    connect(options,SIGNAL(changeDifficulte()),this,SLOT(nouvellePartie()));

	setBackgroundBrush(options->getColor("background"));
	setNewPartie();
}

Grille::~Grille()
{
	Options::deleteInstance();
    clearGrille();
}

int Grille::gethauteur() const
{
    return m_hauteur;
}

int Grille::getlargeur() const
{
    return m_largeur;
}

Grille::EtatPartie Grille::getEtat() const
{
	return etat;
}

void Grille::changeColors()
{
	setBackgroundBrush(options->getColor("background"));
	for(int i=0; i<m_hauteur; i++)
	{
		for(int j=0; j<m_largeur; j++)
		{
			tabC[i][j]->updateColor();
		}
	}
}

void Grille::setNewPartie()
{
    //recuperation des infos
    m_hauteur = options->getHauteur();
    m_largeur = options->getLargeur();
	etat = NOUVELLE_PARTIE;

	int s = Case::size();
	setSceneRect(-30,-30,m_largeur*s+60,m_hauteur*s+100);
	fond = addRect(0,0,m_largeur*s,m_hauteur*s,QPen(),QBrush(Qt::white));

    //creation, dessin et remplissage du tableau
	tabC = new Case** [m_hauteur];
    for(int i=0; i<m_hauteur; i++)
    {
		tabC[i] = new Case* [m_largeur];
        for(int j=0; j<m_largeur; j++)
        {
			tabC[i][j] = new Case(getScene(),Case::NON_DECOUVERTE,false);
			tabC[i][j]->setPos(j*Case::size(),i*Case::size());
        }
    }
}

void Grille::clearGrille()
{
	delete fond;
    for(int i=0; i<m_hauteur; i++)
    {
		for(int j =0; j<m_largeur; j++)
			delete tabC[i][j];
        delete[] tabC[i];
    }
    delete[] tabC;
	emit clearTimer();
	emit clearMines();
}

void Grille::nouvellePartie()
{
	if(etat == EN_COURS || etat == RECOMMENCER)
	{
		if(QMessageBox::question(QApplication::activeWindow(),"Nouvelle partie",
						"Commencer une nouvelle partie perdra la progression actuelle"
						" et la partie sera considérée comme perdue.\nVoulez vous vraiment continuer?",
						QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			clearGrille();
			setNewPartie();
		}
	}
	else
	{
		clearGrille();
		setNewPartie();
	}
}

void Grille::recommencer()
{
    for(int i=0; i<m_hauteur; i++)
        for(int j=0; j<m_largeur; j++)
			tabC[i][j]->setType(Case::NON_DECOUVERTE);
	etat=RECOMMENCER;
	emit clearTimer();
	emit clearMines();
}

void Grille::innitTab(const qint8 li, const qint8 co)
{
    //tirage des mines
    srand(time(0));
	for(int xa=0, ya=0, mines=options->getNbMines(); mines>0; )
    {
        xa=rand()%m_largeur;
        ya=rand()%m_hauteur;
		if( ((xa != co && xa != co-1 && xa != co+1) ||
			(ya != li && ya != li-1 && ya != li+1)) &&
			!tabC[ya][xa]->isMine())
        {
			tabC[ya][xa]->setMine(true);
            mines--;
        }
    }
}

void Grille::gameOver()
{
	emit stopTimer(); //stop
	etat=PERDU;
	for(int i=0; i<m_hauteur; i++)
	{
		for(int j=0; j<m_largeur; j++)
		{
			if(tabC[i][j]->isMine())
				tabC[i][j]->setType(Case::DECOUVERTE);
		}
	}
	emit endGame(false);
}

bool Grille::isFinished() const
{
	for(int i=0; i<m_hauteur; i++)
		for(int j=0; j<m_largeur; j++)
			if(!tabC[i][j]->isMine() && tabC[i][j]->getType() != Case::DECOUVERTE)
				return false;
	return true;
}

void Grille::decouvrirCase(const qint8 li, const qint8 co, bool &mineTrouvee)
{
	if(tabC[li][co]->isMine())
        mineTrouvee = true;
    else
    {
		tabC[li][co]->setValue(voitMines(li,co));
		if(tabC[li][co]->getValue() == 0)
            doubleClic(li,co);
    }
}

void Grille::clicGauche(const qint8 li, const qint8 co)
{
	switch(etat)
	{
		case NOUVELLE_PARTIE:
			innitTab(li,co);
		case RECOMMENCER:
			emit launchTimer(); //start
			etat=EN_COURS;
		default:
			break;
	}
	bool mineTrouvee = false;
    decouvrirCase(li,co,mineTrouvee);
    if(mineTrouvee)
        gameOver();
	else if(isFinished())
    {
		etat=GAGNE;
		emit stopTimer(); //stop
		emit endGame(true);
    }
}

void Grille::clicDroit(const qint8 li, const qint8 co)
{
	switch(tabC[li][co]->getType())
    {
		case Case::NON_DECOUVERTE:
			tabC[li][co]->setType(Case::DRAPEAU);
			emit decMines();
            break;
		case Case::DRAPEAU:
			if(options->isOptionEnabled(Options::PT_INTERROGATION))
				tabC[li][co]->setType(Case::PT_INTERROGATION);
            else
				tabC[li][co]->setType(Case::NON_DECOUVERTE);
			emit incMines();
            break;
		case Case::PT_INTERROGATION:
			tabC[li][co]->setType(Case::NON_DECOUVERTE);
            break;
		default:
			tabC[li][co]->setType(Case::PT_INTERROGATION);
			break;
    }
}

void Grille::doubleClic(const qint8 li, const qint8 co)
{
	bool mineTrouvee = false;
    if(li-1>=0)
    {
		if(tabC[li-1][co]->getType() == Case::NON_DECOUVERTE ||
						tabC[li-1][co]->getType() == Case::PT_INTERROGATION)
            decouvrirCase(li-1,co,mineTrouvee); //2
        if(co-1>=0)
			if(tabC[li-1][co-1]->getType() == Case::NON_DECOUVERTE ||
							tabC[li-1][co-1]->getType() == Case::PT_INTERROGATION)
                decouvrirCase(li-1,co-1,mineTrouvee); //1
        if(co+1<m_largeur)
			if(tabC[li-1][co+1]->getType() == Case::NON_DECOUVERTE ||
							tabC[li-1][co+1]->getType() == Case::PT_INTERROGATION)
				decouvrirCase(li-1,co+1,mineTrouvee); //3
    }
    if(co-1>=0)
		if(tabC[li][co-1]->getType() == Case::NON_DECOUVERTE ||
						tabC[li][co-1]->getType() == Case::PT_INTERROGATION)
            decouvrirCase(li,co-1,mineTrouvee); //4
    if(co+1<m_largeur)
		if(tabC[li][co+1]->getType() == Case::NON_DECOUVERTE ||
						tabC[li][co+1]->getType() == Case::PT_INTERROGATION)
            decouvrirCase(li,co+1,mineTrouvee); //5
    if(li+1<m_hauteur)
    {
		if(tabC[li+1][co]->getType() == Case::NON_DECOUVERTE ||
						tabC[li+1][co]->getType() == Case::PT_INTERROGATION)
            decouvrirCase(li+1,co,mineTrouvee); //7
        if(co-1>=0)
			if(tabC[li+1][co-1]->getType() == Case::NON_DECOUVERTE ||
							tabC[li+1][co-1]->getType() == Case::PT_INTERROGATION)
                decouvrirCase(li+1,co-1,mineTrouvee); //6
        if(co+1<m_largeur)
			if(tabC[li+1][co+1]->getType() == Case::NON_DECOUVERTE ||
							tabC[li+1][co+1]->getType() == Case::PT_INTERROGATION)
                decouvrirCase(li+1,co+1,mineTrouvee); //8
    }
    if(mineTrouvee)
        gameOver();
	if(isFinished())
    {
		etat=GAGNE;
		emit stopTimer(); //stop
		emit endGame(true);
    }
}

int Grille::voitMines(const qint8 li, const qint8 co) const
{
    int voit_mines=0;
    if(li-1>=0)
    {
        if(co-1>=0)
			if(tabC[li-1][co-1]->isMine()) //1
                voit_mines++;
		if(tabC[li-1][co]->isMine()) //2
            voit_mines++;
        if(co+1<m_largeur)
			if(tabC[li-1][co+1]->isMine()) //3
                voit_mines++;
    }
    if(co-1>=0)
	if(tabC[li][co-1]->isMine()) //4
            voit_mines++;
    if(co+1<m_largeur)
	if(tabC[li][co+1]->isMine()) //5
            voit_mines++;
    if(li+1<m_hauteur)
    {
        if(co-1>=0)
			if(tabC[li+1][co-1]->isMine()) //6
                voit_mines++;
		if(tabC[li+1][co]->isMine()) //7
            voit_mines++;
        if(co+1<m_largeur)
			if(tabC[li+1][co+1]->isMine()) //8
                voit_mines++;
    }
    return voit_mines;
}

int Grille::voitDrapeaux(const qint8 li, const qint8 co) const
{
    int voit_B=0;
    if(li-1>=0)
    {
        if(co-1>=0)
			if(tabC[li-1][co-1]->getType()==Case::DRAPEAU) //1
                voit_B++;
		if(tabC[li-1][co]->getType()==Case::DRAPEAU) //2
            voit_B++;
        if(co+1<m_largeur)
			if(tabC[li-1][co+1]->getType()==Case::DRAPEAU) //3
                voit_B++;
    }
    if(co-1>=0)
		if(tabC[li][co-1]->getType()==Case::DRAPEAU) //4
            voit_B++;
    if(co+1<m_largeur)
		if(tabC[li][co+1]->getType()==Case::DRAPEAU) //5
            voit_B++;
    if(li+1<m_hauteur)
    {
        if(co-1>=0)
			if(tabC[li+1][co-1]->getType()==Case::DRAPEAU) //6
                voit_B++;
		if(tabC[li+1][co]->getType()==Case::DRAPEAU) //7
            voit_B++;
        if(co+1<m_largeur)
			if(tabC[li+1][co+1]->getType()==Case::DRAPEAU) //8
                voit_B++;
    }
    return voit_B;
}

void Grille::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(etat==GAGNE || etat==PERDU)
	{
		//emit endGame(etat==GAGNE);
	}
	else
    {
		QPointF p = event->scenePos();
		qint8 li = static_cast<qint8>(p.y()/Case::size());
		qint8 co = static_cast<qint8>(p.x()/Case::size());
		if(p.x()>=0 && p.y()>=0 && co<m_largeur && li<m_hauteur)
        {

			Case *c = tabC[li][co];
            switch(event->button())
			{
                case Qt::LeftButton:
					if(c->getType()==Case::DECOUVERTE)
					{
						if(c->getValue()==voitDrapeaux(li,co)
								&& options->isOptionEnabled(Options::DECOUVERTE)
								&& !options->isOptionEnabled(Options::MODE_CLIC_DOUBLE))
                            doubleClic(li,co);
					}
					else if(c->getType()!=Case::DRAPEAU)
                        clicGauche(li,co);
                    break;
                case Qt::RightButton:
					if(c->getType()!=Case::DECOUVERTE)
                        clicDroit(li,co);
                    break;
                default:
					QGraphicsScene::mousePressEvent(event);
                    break;
            }
        }
        else
        {
			QGraphicsScene::mousePressEvent(event);
        }
	}
}

void Grille::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF p = event->scenePos();
	qint8 li = static_cast<qint8>(p.y()/Case::size());
	qint8 co = static_cast<qint8>(p.x()/Case::size());
	if(event->button() == Qt::LeftButton
			&& p.x()>=0 && p.y()>=0 && co<m_largeur && li<m_hauteur)
	{

		Case *c = tabC[li][co];
		if(c->getType()==Case::DECOUVERTE
				&& options->isOptionEnabled(Options::DECOUVERTE)
				&& options->isOptionEnabled(Options::MODE_CLIC_DOUBLE))
		{
			if(c->getValue()==voitDrapeaux(li,co))
				doubleClic(li,co);
			QGraphicsScene::mousePressEvent(event);
		}
		else
		{
			mousePressEvent(event);
		}
	}
	else
	{
		mousePressEvent(event);
	}
}
