#ifndef GRILLE_H
#define GRILLE_H

#include <QGraphicsScene>

class Case;
class Options;
class GrilleGraph;
class QTimer;
class MainWindow;

class Grille : public QGraphicsScene
{
    Q_OBJECT
    public:
		enum EtatPartie {
			NOUVELLE_PARTIE,
			RECOMMENCER,
			EN_COURS,
			GAGNE,
			PERDU
		};

        explicit Grille(QWidget *parent = 0);
        ~Grille();

        int gethauteur() const;
        int getlargeur() const;
		EtatPartie getEtat() const;
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
		QGraphicsScene *getScene()
			{ return this; }

    public slots:
        void nouvellePartie();
        void recommencer();
		void changeColors();

	signals:
		void endGame(bool);
		void stopTimer();
		void clearTimer();
		void launchTimer();
		void incMines();
		void decMines();
		void clearMines();

    private:
        //clics
        void clicGauche(const qint8 li, const qint8 co);
        void clicDroit(const qint8 li, const qint8 co);
        void doubleClic(const qint8 li, const qint8 co);

        //internes a la partie
        void clearGrille();
        void setNewPartie();
        void innitTab(const qint8 li, const qint8 co);
		void decouvrirCase(const qint8 li, const qint8 co, bool &mineTrouvee);
		int voitMines(const qint8 li, const qint8 co) const;
		int voitDrapeaux(const qint8 li, const qint8 co) const;
        void gameOver();
		bool isFinished() const;

        Options *options;
		
		QGraphicsRectItem *fond;
		Case ***tabC; // tableau a 2 dimentions

        qint8 m_hauteur;
        qint8 m_largeur;
		EtatPartie etat;
};

#endif // GRILLE_H
