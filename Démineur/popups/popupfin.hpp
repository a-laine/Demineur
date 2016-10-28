#ifndef POPUPFIN_H
#define POPUPFIN_H

#include <QDialog>
#include <QString>

class QLabel;
class QPushButton;

class PopupFin : public QDialog
{
    Q_OBJECT
    public:
		explicit PopupFin(const bool gagne, QWidget *parent = 0);

    private:
		static QString getText(bool gagne);

		QLabel *text;
		QPushButton *btnQuitter;
		QPushButton *btnNewPartie;
		QPushButton *btnRecommencer;
};

#endif // POPUPFIN_H
