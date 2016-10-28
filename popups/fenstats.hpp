#ifndef FENSTATS_HPP
#define FENSTATS_HPP

#include <QDialog>

class QListWidget;

class FenStats : public QDialog
{
		Q_OBJECT
	public:
		explicit FenStats(QWidget *parent = 0);

	public slots:
		void reinitialiser();

	private:
		QListWidget *list;
};

#endif // FENSTATS_HPP
