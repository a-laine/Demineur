#ifndef COMPTEURS_HPP
#define COMPTEURS_HPP

#include <QObject>
#include <QRectF>

class QTimer;
class QGraphicsScene;
class QGraphicsItemGroup;
class QGraphicsSimpleTextItem;

class Compteurs : public QObject
{
		Q_OBJECT
	public:
		explicit Compteurs(QGraphicsScene *scene, QObject *parent = 0);

		int getTime() const;
		int getMines() const;

	public slots:
		void changeColors() const;
		void redraw(QRectF sceneRect);
		void stopTimer();
		void clearTimer();
		void launchTimer();
		void incTimer();
		void incMines();
		void decMines();
		void clearMines();

	private:
		void setTimerValue(int val);
		void setMinesValue(int val);

		QGraphicsScene *scene;
		QTimer *timer;
		QGraphicsItemGroup *gChrono;
		QGraphicsItemGroup *gMines;
		QGraphicsSimpleTextItem *txtChrono;
		QGraphicsSimpleTextItem *txtMines;

		int chrono;
		int mines;
};

#endif // COMPTEURS_HPP
