#include "compteurs.hpp"
#include "grille/options.hpp"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsSimpleTextItem>

Compteurs::Compteurs(QGraphicsScene *scene, QObject *parent) :
	QObject(parent)
{
	this->scene = scene;
	Options* options = Options::getInstance();
	//chrono
	gChrono = new QGraphicsItemGroup();
	scene->addItem(gChrono);
	gChrono->setPos(0,0);
	txtChrono = new QGraphicsSimpleTextItem("temps: 000");
	txtChrono->setFont(QFont("Arimo", 14, 67));
	txtChrono->setBrush(QBrush(options->getColor("compteurs", Qt::black)));
	gChrono->addToGroup(txtChrono);
	txtChrono->setPos(0,0);
	//mines
	gMines = new QGraphicsItemGroup();
	scene->addItem(gMines);
	gMines->setPos(0,0);
	txtMines = new QGraphicsSimpleTextItem("mines: 000");
	txtMines->setFont(QFont("Arimo", 14, 67));
	txtMines->setBrush(QBrush(options->getColor("compteurs", Qt::black)));
	gMines->addToGroup(txtMines);
	txtMines->setPos(0,0);

	timer = new QTimer(this);
	connect(timer,SIGNAL(timeout()),this,SLOT(incTimer()));
	redraw(scene->sceneRect());
}

int Compteurs::getTime() const
{
	return chrono;
}

int Compteurs::getMines() const
{
	return mines;
}

void Compteurs::setTimerValue(int val)
{
	QString str;
	str.sprintf("temps: %03d", val);
	txtChrono->setText(str);
}

void Compteurs::setMinesValue(int val)
{
	QString str;
	str.sprintf("mines: %03d", val);
	txtMines->setText(str);
}

//******************************* SLOTS ********************************//

void Compteurs::changeColors() const
{
	Options* options = Options::getInstance();
	txtChrono->setBrush(QBrush(options->getColor("compteurs", Qt::black)));
	txtMines->setBrush(QBrush(options->getColor("compteurs", Qt::black)));
}

void Compteurs::redraw(QRectF sceneRect)
{
	clearTimer();
	clearMines();
	qreal decalX = sceneRect.width()/10;
	gChrono->setPos(decalX, sceneRect.bottom()-50);
	gMines->setPos(sceneRect.right()-decalX-130, sceneRect.bottom()-50);
}

void Compteurs::stopTimer()
{
	timer->stop();
}

void Compteurs::clearTimer()
{
	timer->stop();
	chrono = 0;
	setTimerValue(0);
}

void Compteurs::launchTimer()
{
	timer->start(1000);
}

void Compteurs::incTimer()
{
	chrono++;
	setTimerValue(chrono);
}

void Compteurs::incMines()
{
	mines++;
	setMinesValue(mines);
}

void Compteurs::decMines()
{
	mines--;
	setMinesValue(mines);
}

void Compteurs::clearMines()
{
	mines = Options::getInstance()->getNbMines();
	setMinesValue(mines);
}
