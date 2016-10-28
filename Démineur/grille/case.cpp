#include "case.hpp"
#include "options.hpp"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <algorithm>
#include <stdexcept>

QHash<qint8,QPixmap*> Case::pixmaps;

enum ValeurCase {
	VAL_MIN = 0,
	VAL_MAX = 8,
	INDEFINIE_MINE = 0x80,
	INDEFINIE_VIDE = 0x0F,		// etat de transition
	DEC_VIDE = 0,				// case decouverte, vide
	DEC_MINE = 0x80,			// click sur une mine :(
	NON_DEC_MINE = 0x10 | 0x80,	// case non decouverte, une mine
	NON_DEC_VIDE = 0x10,		// case non decouverte, pas de mines
	DRAPEAU_MINE = 0x20 | 0x80,	// drapeau pose, une mine
	DRAPEAU_VIDE = 0x20,		// drapeau pose, pas de mine
	PT_INT_MINE = 0x40 | 0x80,	// ? pose, une mine
	PT_INT_VIDE = 0x40,			// ? pose, pas de mine
	//NOMBRE: case découverte, le nombre correspond au nombre de mines que voit la case
};


//******************************** CONSTRUCTEUR / DESTRUCTEUR *******************************//

Case::Case(QGraphicsScene *scene) :
	QGraphicsItemGroup(0), valeur(INDEFINIE_VIDE)
{
	scene->addItem(this);
	setAcceptHoverEvents(true);
}

Case::Case(QGraphicsScene *scene, TypeCase type, bool isMine) :
	QGraphicsItemGroup(0), valeur(INDEFINIE_VIDE)
{
	scene->addItem(this);
	setAcceptHoverEvents(true);
	setType(type);
	setMine(isMine);
}

Case::~Case()
{
}


//***************************************** ACCESSEURS **************************************//

void deleteItem(QGraphicsItem *item)
{
	item->scene()->removeItem(item);
	delete item;
}

void Case::updateColor()
{
	if(getValue() <= 0)
		setType(getType());
	else
		setValue(getValue());
}

void Case::setType(TypeCase type)
{
	bool mine = isMine();
	switch(type)
	{
		case DECOUVERTE:
			if(getType() != DECOUVERTE)
				valeur = DEC_VIDE;
			break;
		case NON_DECOUVERTE:
			valeur = NON_DEC_VIDE;
			break;
		case DRAPEAU:
			valeur = DRAPEAU_VIDE;
			break;
		case PT_INTERROGATION:
			valeur = PT_INT_VIDE;
			break;
		case INDEFINIE:
			valeur = INDEFINIE_VIDE;
			break;
		default:
			break;
	}
	setMine(mine);

	QList<QGraphicsItem *> childs = childItems();
	std::for_each(childs.begin(),childs.end(),deleteItem);

	QGraphicsPixmapItem* image = scene()->addPixmap(*getPixmap(valeur));
	addToGroup(image);
	image->setPos(0,0);

	if(type == PT_INTERROGATION)
	{
		QGraphicsSimpleTextItem* text = scene()->addSimpleText("?", QFont("Arial", 26, 70));
		Options* options = Options::getInstance();
		text->setBrush(QBrush(options->getColor("pt_interro", Qt::white)));
		addToGroup(text);
		text->setPos(8,-1);
	}
}

void Case::setValue(qint8 val)
{
	if(val<0 || val>8)
		throw std::invalid_argument("valeur impossible");

	valeur = val;
	setType(DECOUVERTE);
	if(val==0)
		return;

	QGraphicsSimpleTextItem *element = scene()->addSimpleText(QChar(val+'0'), QFont("Verdana", 25, 70));
	addToGroup(element);
	element->setPos(6,-3);

	Options* options = Options::getInstance();
	QString num = QString::number((int) val);
	element->setBrush(QBrush(options->getColor(num, Qt::black)));
}

void Case::setMine(bool isMine)
{
		if(isMine)
			 valeur |= INDEFINIE_MINE;
		else
			valeur &= ~INDEFINIE_MINE;
}

Case::TypeCase Case::getType() const
{
	if(valeur & NON_DEC_VIDE)
		return NON_DECOUVERTE;
	else if(valeur & DRAPEAU_VIDE)
		return DRAPEAU;
	else if(valeur & PT_INT_VIDE)
		return PT_INTERROGATION;
	else if(/*valeur>=VAL_MIN && */valeur<=DEC_MINE)
		return DECOUVERTE;
	else
		return INDEFINIE;
}

qint8 Case::getValue() const
{
	if(/*valeur>=VAL_MIN && */valeur<=VAL_MAX)
		return valeur;
	return -1;
}

bool Case::isMine() const
{
	return valeur & INDEFINIE_MINE;
}


//*************************************** AUTRES *******************************************//

QPixmap* Case::getPixmap(quint8 val)
{
	QPixmap* p = 0;
	switch(val)
	{
		case NON_DEC_VIDE:
		case NON_DEC_MINE:
		case PT_INT_VIDE:
		case PT_INT_MINE:
			p = pixmaps[NON_DEC_VIDE];
			if(!p)
				p = *(pixmaps.insert(NON_DEC_VIDE,new QPixmap("Images/caseNonDec.bmp")));
			break;
		case DRAPEAU_MINE:
		case DRAPEAU_VIDE:
			p = pixmaps[DRAPEAU_VIDE];
			if(!p)
				p = *(pixmaps.insert(DRAPEAU_VIDE,new QPixmap("Images/caseDrapeau.bmp")));
			break;
		case DEC_MINE:
			p = pixmaps[DEC_MINE];
			if(!p)
				p = *(pixmaps.insert(DEC_MINE,new QPixmap("Images/caseMine.bmp")));
			break;
		default://DEC_VIDE
			p = pixmaps[DEC_VIDE];
			if(!p)
				p = *(pixmaps.insert(DEC_VIDE,new QPixmap("Images/caseVide.bmp")));
			break;
	}
	return p;
}

QRectF Case::boundingRect() const
{
	return QRectF(0,0,size(),size());
}

void Case::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
	if(getType() != DECOUVERTE)
		setOpacity(0.75);
}

void Case::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	setOpacity(1);
}
