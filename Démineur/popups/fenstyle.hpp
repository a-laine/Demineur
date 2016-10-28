#ifndef FENSTYLE_HPP
#define FENSTYLE_HPP

#include <QToolButton>
#include <QMap>
#include <QString>
#include <QColor>

class ColorButton : public QToolButton
{
		Q_OBJECT
	public:
		static QMap<QString,QColor> colors;

		explicit ColorButton(QString name, QColor color = Qt::gray, QWidget *parent = 0);
		void setColor(QColor color);
		QColor getColor() const;

	public slots:
		void compute();

	private:
		static QIcon iconByName(QString name, QColor color);

		QString nom;
		QColor couleur;
};


#include <QDialog>

class FenStyle : public QDialog
{
		Q_OBJECT
	public:
		explicit FenStyle(QWidget *parent = 0);
		~FenStyle();

	public slots:
		void ok();
		void defaultUI();
};

#endif // FENSTYLE_HPP
