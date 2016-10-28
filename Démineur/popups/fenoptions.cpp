#include "fenoptions.hpp"
#include "grille/options.hpp"
//Widget groupe:
#include <QGroupBox>
//Layouts:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
//Boutons et champs de saisie:
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>

FenOptions::FenOptions(QWidget *parent) :
    QDialog(parent)
{
	m_options = Options::getInstance();

    //boutons ok et annuler
	QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
	connect(btns, SIGNAL(accepted()), this, SLOT(ok()));
	connect(btns, SIGNAL(rejected()), this, SLOT(reject()));

    //choix de la difficulte
	QGroupBox *gDifficulte = new QGroupBox("Difficulté:");
	QGridLayout *lDifficulte = new QGridLayout(gDifficulte);
        //facile
        btnFacile = new QRadioButton("Débutant:\n10 mines\ngrille 9x9");
        lDifficulte->addWidget(btnFacile,0,0);
        //moyen
        btnMoyen = new QRadioButton("Intermédiaire:\n40 mines\ngrille 16x16");
        lDifficulte->addWidget(btnMoyen,1,0);
        //difficile
        btnDifficile = new QRadioButton("Avencé:\n99 mines\ngrille 16x30");
        lDifficulte->addWidget(btnDifficile,2,0);
        //personnalise
        btnPerso = new QRadioButton("Personnalisé:");
        lDifficulte->addWidget(btnPerso,0,1);
		connect(btnPerso,SIGNAL(toggled(bool)),this,SLOT(persoChecked(bool)));
			gPerso = new QGroupBox();
			QFormLayout *lPerso = new QFormLayout(gPerso);
			chHauteur = new QSpinBox();
            chHauteur->setAccelerated(true);
            chHauteur->setMinimum(5);
            chHauteur->setMaximum(30);
			lPerso->addRow("Hauteur(5-30):", chHauteur);
			connect(chHauteur,SIGNAL(valueChanged(int)),this,SLOT(champsChanged()));
			chLargeur = new QSpinBox();
            chLargeur->setAccelerated(true);
            chLargeur->setMinimum(5);
            chLargeur->setMaximum(30);
			lPerso->addRow("Largeur(5-30):",chLargeur);
			connect(chLargeur,SIGNAL(valueChanged(int)),this,SLOT(champsChanged()));
			chMines = new QSpinBox();
            chMines->setAccelerated(true);
            chMines->setMinimum(1);
            chMines->setMaximum(891);
            lPerso->addRow("Mines(1-891):",chMines);
            lDifficulte->addWidget(gPerso,1,1,2,1);

    //flags
	QGroupBox *gFlags = new QGroupBox("Autres Options:");
	QFormLayout *lFlags = new QFormLayout(gFlags);
	btnPtInterrog = new QCheckBox();
	lFlags->addRow("Autoriser les points d'interrogation (deux clics droits)",btnPtInterrog);
	btnDecouverte = new QComboBox();
	btnDecouverte->addItem("Désactivé");
	btnDecouverte->addItem("Simple clic");
	btnDecouverte->addItem("Double clic");
	lFlags->addRow("Autoriser la découverte de plusieurs cases",btnDecouverte);

    //positionnement de tous les elements
	QVBoxLayout *lFen = new QVBoxLayout(this);
    lFen->addWidget(gDifficulte);
    lFen->addWidget(gFlags);
	lFen->addWidget(btns);

    chargerOptions();
}

void FenOptions::ok()
{
	accept();

    //difficulte
    if(btnFacile->isChecked())
    {
		m_options->setDifficulte(Options::FACILE);
    }
    else if(btnMoyen->isChecked())
    {
		m_options->setDifficulte(Options::MOYEN);
    }
    else if(btnDifficile->isChecked())
    {
		m_options->setDifficulte(Options::DIFFICILE);
    }
    else if(btnPerso->isChecked())
    {
		m_options->setDifficulte(Options::PERSONNALISE, chHauteur->value(), chLargeur->value(), chMines->value());
    }

    //flags
	m_options->setOptionFlag(Options::PT_INTERROGATION,btnPtInterrog->isChecked());
	if(btnDecouverte->currentText() == "Désactivé")
	{
		m_options->setOptionFlag(Options::DECOUVERTE,false);
	}
	else if(btnDecouverte->currentText() == "Simple clic")
	{
		m_options->setOptionFlag(Options::DECOUVERTE,true);
		m_options->setOptionFlag(Options::MODE_CLIC_DOUBLE,false);
	}
	else
	{
		m_options->setOptionFlag(Options::DECOUVERTE,true);
		m_options->setOptionFlag(Options::MODE_CLIC_DOUBLE,true);
	}

    m_options->ecrire();
}

void FenOptions::chargerOptions()
{
    //difficulte
    gPerso->setEnabled(false);
    chHauteur->setValue(9);
    chLargeur->setValue(9);
    chMines->setValue(10);
    chMines->setMaximum(72);
    switch(m_options->getDifficulte())
    {
		case Options::FACILE:
            btnFacile->setChecked(true);
            break;
		case Options::MOYEN:
            btnMoyen->setChecked(true);
            break;
		case Options::DIFFICILE:
            btnDifficile->setChecked(true);
            break;
		case Options::PERSONNALISE:
            btnPerso->setChecked(true);
            gPerso->setEnabled(true);
            chHauteur->setValue(m_options->getHauteur());
            chLargeur->setValue(m_options->getLargeur());
            champsChanged();
            chMines->setValue(m_options->getNbMines());
            break;
        default:
            btnFacile->setChecked(true);
    }

    //flags
	btnPtInterrog->setChecked(m_options->isOptionEnabled(Options::PT_INTERROGATION));
	if(m_options->isOptionEnabled(Options::DECOUVERTE))
	{
		if(m_options->isOptionEnabled(Options::MODE_CLIC_DOUBLE))
			btnDecouverte->setCurrentText("Double clic");
		else
			btnDecouverte->setCurrentText("Simple clic");
	}
	else
		btnDecouverte->setCurrentText("Désactivé");
}

void FenOptions::champsChanged()
{
    chMines->setMaximum(chHauteur->value()*chLargeur->value()-9);
}

void FenOptions::persoChecked(bool checked)
{
	gPerso->setEnabled(checked);
}
