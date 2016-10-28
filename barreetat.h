#ifndef BARREETAT_H
#define BARREETAT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

class BarreEtat : public QWidget
{
    Q_OBJECT
    public:
        explicit BarreEtat(QWidget *parent = 0);

    signals:

    public slots:
        void majNbMines(bool plus);
        void nouvellePartie(int mines);

    private:
        QLabel *LTemps;
        QLabel *LMines;
        QHBoxLayout *layout;
		
		int nbMines;
};

#endif // BARREETAT_H
