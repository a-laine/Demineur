#include "mainwindow.hpp"
#include "grille/grille.hpp"
#include "compteurs.hpp"
#include "popups/fenstats.hpp"
#include "popups/fenoptions.hpp"
#include "popups/fenstyle.hpp"
#include "popups/popupfin.hpp"
#include <QResizeEvent>
#include <QMessageBox>
// menu
#include <QMenu>
#include <QMenuBar>
#include <QAction>
// centre
#include <QStackedWidget>
#include <QGraphicsView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	// organisation du centre
	widgetCentral = new QStackedWidget();
	setCentralWidget(widgetCentral);

    //création de la grille centrale
	grille = new Grille(this);
	vue = new QGraphicsView(grille->getScene());
	widgetCentral->addWidget(vue);
	connect(grille, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(updateSize(QRectF)));
	connect(grille, SIGNAL(endGame(bool)), this, SLOT(popupFin(bool)));

	//création des compteurs du bas
	compteurs = new Compteurs(grille->getScene());
	connect(grille,SIGNAL(launchTimer()),compteurs,SLOT(launchTimer()));
	connect(grille,SIGNAL(stopTimer()),compteurs,SLOT(stopTimer()));
	connect(grille,SIGNAL(clearTimer()),compteurs,SLOT(clearTimer()));
	connect(grille,SIGNAL(incMines()),compteurs,SLOT(incMines()));
	connect(grille,SIGNAL(decMines()),compteurs,SLOT(decMines()));
	connect(grille,SIGNAL(clearMines()),compteurs,SLOT(clearMines()));
	connect(grille,SIGNAL(sceneRectChanged(QRectF)),compteurs,SLOT(redraw(QRectF)));

    //création de la barre de menu
	QMenu *menuPartie = menuBar()->addMenu("&Partie");
	actionNouvPartie = menuPartie->addAction("&Nouvelle partie");
		actionNouvPartie->setShortcut(QKeySequence("F2"));
		connect(actionNouvPartie, SIGNAL(triggered()), grille, SLOT(nouvellePartie()));
	menuPartie->addSeparator();
	actionScores = menuPartie->addAction("&Statistiques");
		actionScores->setShortcut(QKeySequence("F4"));
		connect(actionScores, SIGNAL(triggered()), this, SLOT(scores()));
	actionOptions = menuPartie->addAction("&Options");
		actionOptions->setShortcut(QKeySequence("F5"));
		connect(actionOptions, SIGNAL(triggered()), this, SLOT(options()));
	actionApparence = menuPartie->addAction("&Apparence");
		actionApparence->setShortcut(QKeySequence("F7"));
		connect(actionApparence, SIGNAL(triggered()), this, SLOT(apparence()));
	menuPartie->addSeparator();
	actionQuitter = menuPartie->addAction("&Quitter");
		connect(actionQuitter, SIGNAL(triggered()), this, SLOT(close()));
	QMenu *menuAide = menuBar()->addMenu("&?");
	actionInfo = menuAide->addAction("A propos");
		connect(actionInfo, SIGNAL(triggered()), this, SLOT(aPropos()));
}


// ************************************ SLOTS ************************************* //

void MainWindow::updateSize(QRectF rect)
{
	vue->resetTransform();
	if(isMaximized())
	{
		qreal x = static_cast<qreal>(size().width()-13) /
				   static_cast<qreal>(rect.width());
		qreal y = static_cast<qreal>(size().height()-23) /
				   static_cast<qreal>(rect.height());
		if(x<y)
			vue->scale(x,x);
		else
			vue->scale(y,y);
	}
	else
		resize(rect.width()+13,rect.height()+23);
}

void MainWindow::popupFin(bool gagne)
{
	PopupFin popup(gagne,this);
	switch(popup.exec())
	{
		case QDialog::Accepted: //nouvelle partie
			grille->nouvellePartie();
			break;
		case QDialog::Rejected: //recommencer
			grille->recommencer();
			break;
		default: //sait-on jamais?
			close();
			break;
	}
}

void MainWindow::scores()
{
	FenStats f(this);
	f.exec();
}

void MainWindow::options()
{
	FenOptions f(this);
	f.exec();
}

void MainWindow::apparence()
{
	FenStyle f(this);
	switch(f.exec())
	{
		case QDialog::Accepted:
			grille->changeColors();
			compteurs->changeColors();
	}
}

void MainWindow::aPropos()
{
	QMessageBox::about(this,"A propos de Démineur",
				"<b>Démineur</b><br>Auteur: LAINÉ Aurélien<br>Logiciel réalisé avec Qt5");
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
	if(vue)
	{
		vue->resetTransform();
		qreal x = static_cast<qreal>(event->size().width()-13) /
				   static_cast<qreal>(vue->sceneRect().width());
		qreal y = static_cast<qreal>(event->size().height()-23) /
				   static_cast<qreal>(vue->sceneRect().height());
		if(x<y)
			vue->scale(x,x);
		else
			vue->scale(y,y);
	}
	QMainWindow::resizeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(grille->getEtat() == Grille::EN_COURS)
	{
		event->accept();
	}
	else
		event->accept();
}
