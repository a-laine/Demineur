#ifndef FENOPTIONS_H
#define FENOPTIONS_H

#include <QDialog>

class Options;
class QRadioButton;
class QGroupBox;
class QSpinBox;
class QCheckBox;
class QComboBox;


class FenOptions : public QDialog
{
    Q_OBJECT
    public:
		explicit FenOptions(QWidget *parent);
        void chargerOptions();

    public slots:
        void ok();
        void champsChanged();
        void persoChecked(bool checked);

    private:
        Options *m_options;

        //Choix de la difficulte
		QRadioButton *btnFacile;
		QRadioButton *btnMoyen;
		QRadioButton *btnDifficile;
		QRadioButton *btnPerso;
		QGroupBox *gPerso;
			QSpinBox *chHauteur;
			QSpinBox *chLargeur;
			QSpinBox *chMines;

        //flags
		QCheckBox *btnPtInterrog;
		QComboBox *btnDecouverte;
};

#endif // FENOPTIONS_H
