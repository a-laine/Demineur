#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class QAction;
class QStackedWidget;
class Grille;
class QGraphicsView;
class Compteurs;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        MainWindow(QWidget *parent = 0);
		void resizeEvent(QResizeEvent *event);
		void closeEvent(QCloseEvent *event);

    public slots:
		void updateSize(QRectF rect);
		void popupFin(bool gagne);
		void scores();
		void options();
		void apparence();
        void aPropos();

    private:
		/* Barre de menu
		 * actions de la barre:
		 *  - menuPartie
		 *		- actionNouvPartie
		 *		- actionScores
		 *		- actionOptions
		 *		- actionApparence
		 *		- actionQuitter
		 *  - menuAide
		 *		- actionInfo
		 */
		QAction *actionNouvPartie;
		QAction *actionScores;
		QAction *actionOptions;
		QAction *actionApparence;
		QAction *actionQuitter;
		QAction *actionInfo;

		QStackedWidget *widgetCentral;

        //la grille
		Grille *grille;
		Compteurs *compteurs;
		QGraphicsView *vue;
};

#endif // MAINWINDOW_H
